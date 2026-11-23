/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stdafx.hpp"
#include "FileSystem.hpp"
#include "Setup/Validation.hpp"
#include "Setup/MountedImage.hpp"
#include "Utils/md5.hpp"
#include "Amiga/dernc.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>


namespace Setup {

bool sVersionMatch::IsRetail() const {
    return mVersion && mVersion->mRelease == eRelease::Retail;
}

static std::string ToLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

// Keep root separators: "D:" means the drive's current directory, not "D:/".
static std::string StripTrailingSlash(const std::string& pPath) {
    const auto path = std::filesystem::u8path(pPath);
    if (path == path.root_path()) return pPath;
    if (pPath.size() > 1) {
        char c = pPath.back();
        if (c == '/' || c == '\\')
            return pPath.substr(0, pPath.size() - 1);
    }
    return pPath;
}

// Normalise to forward slashes (matches gPathSeperator at Source/stdafx.cpp:32)
// and ensure a single trailing slash. Empty input → empty output.
static std::string Canonicalise(const std::string& pPath) {
    if (pPath.empty())
        return pPath;
    std::string out = pPath;
    std::replace(out.begin(), out.end(), '\\', '/');
    if (out.back() != '/')
        out.push_back('/');
    return out;
}

static bool DirExists(const std::string& pPath) {
    std::error_code ec;
    return std::filesystem::is_directory(FileSystem::Resolve(pPath), ec);
}

// Keep validation rooted at the user's selection. In particular, choosing
// Data/Amiga must never widen the search to its parent's other games.
static std::string ResolveRoot(const std::string& pInput, bool& pOutHasDataChild) {
    pOutHasDataChild = false;
    const auto resolved = FileSystem::Resolve(pInput);
    if (resolved.empty() || !DirExists(resolved.u8string()))
        return {};
    const std::string root = Canonicalise(resolved.u8string());
    pOutHasDataChild = !root.empty() && DirExists(root + "Data");
    return root;
}

static std::string FileHash(const std::string& path, const CancelCheck& cancelled) {
    std::ifstream input(FileSystem::Resolve(path), std::ios::binary);
    if (!input) return {};
    md5_context context;
    md5_starts(&context);
    uint8 buffer[65536];
    while (input) {
        if (cancelled && cancelled()) return {};
        input.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
        md5_update(&context, buffer, (uint32)input.gcount());
    }
    if (!input.eof()) return {};
    uint8 digest[16];
    md5_finish(&context, digest);
    std::string hash;
    for (auto byte : digest) {
        hash += "0123456789ABCDEF"[byte >> 4];
        hash += "0123456789ABCDEF"[byte & 15];
    }
    return hash;
}

static std::string UnpackedHash(const std::vector<uint8>& packed, const CancelCheck& cancelled) {
    if (packed.size() < 18 || std::memcmp(packed.data(), "RNC\1", 4) != 0 || (cancelled && cancelled())) return {};
    const auto outputSize = readBEDWord(packed.data() + 4);
    const auto packedSize = readBEDWord(packed.data() + 8);
    if (!outputSize || outputSize > 32 * 1024 * 1024 || packedSize > packed.size() - 18) return {};
    std::vector<uint8> output(outputSize);
    if (rnc_unpack_checked(packed.data(), packed.size(), output.data(), output.size()) != (long)outputSize || (cancelled && cancelled())) return {};
    md5_context context; uint8 digest[16];
    md5_starts(&context);
    md5_update(&context, output.data(), (uint32)output.size());
    md5_finish(&context, digest);
    std::string hash;
    for (auto byte : digest) {
        hash += "0123456789ABCDEF"[byte >> 4];
        hash += "0123456789ABCDEF"[byte & 15];
    }
    return hash;
}

static std::string UnpackedFileHash(const std::string& path, const CancelCheck& cancelled) {
    std::ifstream input(FileSystem::Resolve(path), std::ios::binary);
    uint8 header[18];
    if (!input.read(reinterpret_cast<char*>(header), 18) || std::memcmp(header, "RNC\1", 4) != 0) return {};
    const auto packedSize = readBEDWord(header + 8);
    if (packedSize > 32 * 1024 * 1024 || (cancelled && cancelled())) return {};
    std::vector<uint8> bytes(18 + packedSize);
    std::copy_n(header, 18, bytes.data());
    if (!input.read(reinterpret_cast<char*>(bytes.data() + 18), packedSize)) return {};
    return UnpackedHash(std::move(bytes), cancelled);
}

using HashCache = std::map<std::string, std::pair<std::string, std::string>>;

static bool LooksAuthentic(const sVersionMatch& match) {
    return match.mFilesFound > 0 && (match.mFilesMd5Checked == 0 ||
        match.mFilesMd5Match * 2 > match.mFilesMd5Checked);
}

// Scan ONE folder against ONE known version's mFiles manifest. Returns a
// match describing how many of the expected files were present (and how many
// had matching MD5s). The folder argument is used verbatim — the caller
// composes it.
//
// Used twice per version per ValidateFolder() call:
//   1. <root>/Data/<mDataPath>/ — the canonical engine layout
//   2. <root>/ itself           — raw-extracted release files, no wrapping dirs
static sVersionMatch ScanFolderForVersion(const std::string& pFolder,
                                           const sGameVersion& pVersion,
                                           bool pIsRawFolder, const CancelCheck& cancelled, HashCache& hashes) {
    sVersionMatch m;
    m.mVersion = &pVersion;
    m.mFilesExpected = pVersion.mFiles.size();
    m.mIsRawFolder = pIsRawFolder;

    if (m.mFilesExpected == 0)
        return m;     // Pseudo-versions (Custom / Random) have empty file lists; skip.

    const auto resolved = FileSystem::Resolve(pFolder);
    if (resolved.empty()) return m;
    std::string base = resolved.generic_u8string();
    if (!base.empty() && base.back() != '/')
        base.push_back('/');

    if (!DirExists(StripTrailingSlash(base)))
        return m;

    m.mResolvedPath = base;

    auto baseFiles = FileSystem::List(StripTrailingSlash(base), "", false);

    // Build a lowercase index of files actually on disk for O(N+M) matching.
    std::vector<std::pair<std::string,std::string>> diskLower; // <lower,original>
    diskLower.reserve(baseFiles.size());
    for (auto& bf : baseFiles)
        diskLower.emplace_back(ToLower(bf), bf);

    for (auto& kf : pVersion.mFiles) {
        if (cancelled && cancelled()) return {};
        std::string wantLower = ToLower(kf.mName);
        const bool hasExpectedMd5 = (kf.mChecksum != nullptr) && (kf.mChecksum[0] != '\0');

        for (auto& d : diskLower) {
            if (d.first == wantLower) {
                ++m.mFilesFound;
                if (hasExpectedMd5) {
                    ++m.mFilesMd5Checked;
                    std::string fullPath = base + d.second;
                    auto hash = hashes.find(fullPath);
                    if (hash == hashes.end()) {
                        const auto raw = FileHash(fullPath, cancelled);
                        hash = hashes.emplace(fullPath, std::make_pair(raw, UnpackedFileHash(fullPath, cancelled))).first;
                    }
                    if (hash->second.first == kf.mChecksum || hash->second.second == kf.mChecksum)
                        ++m.mFilesMd5Match;
                }
                break;
            }
        }
    }

    return m;
}

// Engine-layout shorthand: <root>/Data/<mDataPath>/.
static sVersionMatch ScanVersion(const std::string& pRoot, const sGameVersion& pVersion, const CancelCheck& cancelled, HashCache& hashes) {
    std::string base = pRoot;
    if (!base.empty() && base.back() != '/')
        base.push_back('/');
    base += "Data/";
    base += pVersion.mDataPath;
    base += '/';
    return ScanFolderForVersion(base, pVersion, false, cancelled, hashes);
}

sValidationResult ValidateFolder(const std::string& pFolder, const CancelCheck& cancelled) {
    sValidationResult result;
    result.mInputFolder = pFolder;

    bool hasDataChild = false;
    std::string root = ResolveRoot(pFolder, hasDataChild);
    result.mResolvedRoot = root;
    result.mLooksLikeRoot = hasDataChild;
    if (root.empty())
        return result;

    // Even when hasDataChild is false we still attempt scans so we can offer
    // useful diagnostics; ScanVersion bails fast on missing dirs.
    //
    // ::KnownGameVersions is declared in Versions.hpp at the global scope.
    // Skip pseudo-versions (mFiles empty) — they're handled by the engine
    // via "any retail present implies Custom/Random can be used".
    //
    // Try engine and extracted layouts without ever widening the search to
    // an ancestor. Prefer the most complete authentic match, with engine
    // layout winning ties because it can be used without copying files.
    // Filename-only matches are unreliable: CF1 and CF2 share many filenames
    // (1.RAW, FONT.RAW, ...) but the bytes differ. Same for retail-vs-demo,
    // and same between CF1 Amiga floppy and CF1 Amiga CD32 — they share
    // ~9 splash/title MD5s but differ on the other ~220 files.
    //
    // Rule: more than half of the verifiable MD5s we managed to read must
    // match the manifest. A few overlapping MD5s with another release is
    // not evidence — it's filename + identical-byte coincidence on a small
    // subset of files. Real data has near-100% MD5 match.
    HashCache hashes;
    for (size_t i = 0; i < 20; ++i) {
        if (cancelled && cancelled()) return {};
        const sGameVersion& kv = ::KnownGameVersions[i];
        if (kv.mFiles.empty())
            continue;

        // All supported layouts stay inside the chosen folder: installation
        // root, release files directly, an extracted Data/ folder, or the
        // Data folder of an existing installation. Only the full engine
        // layout can be registered as a search root without copying.
        std::vector<sVersionMatch> candidates;
        candidates.push_back(ScanVersion(root, kv, cancelled, hashes));
        candidates.push_back(ScanFolderForVersion(root, kv, true, cancelled, hashes));
        candidates.push_back(ScanFolderForVersion(root + "Data/", kv, true, cancelled, hashes));
        candidates.push_back(ScanFolderForVersion(root + kv.mDataPath + "/", kv, true, cancelled, hashes));

        sVersionMatch* keep = nullptr;
        for (auto& candidate : candidates) {
            if (LooksAuthentic(candidate) &&
                (!keep || candidate.mFilesFound > keep->mFilesFound))
                keep = &candidate;
        }
        if (keep)
            result.mMatches.push_back(*keep);
    }

    // No across-version dedupe: every KnownGameVersions entry has a unique
    // mDataPath, so a single folder can legitimately satisfy many of them
    // (e.g. Run/Data/ contains AmigaTheOne, AmigaAction, AmigaFormat_XMAS,
    // Plus, AmigaFormat_NVF, AmigaAlienLevels, PCFormat all in their own
    // subdirectories — the user wants to see them all). Filename collisions
    // between distinct releases are filtered out earlier by the MD5
    // looksAuthentic() rule.

    return result;
}

sValidationResult ValidateFolderCollection(const std::string& pFolder, const CancelCheck& cancelled) {
    auto result = ValidateFolder(pFolder, cancelled);
    if (result.mResolvedRoot.empty())
        return result;
    // A picked release is already specific enough. Do not inspect its
    // ancillary directories as if they were separate games.
    for (const auto& match : result.mMatches)
        if (match.IsComplete() && FileSystem::Equal(match.mResolvedPath, result.mResolvedRoot))
            return result;

    const auto scanChildren = [&](const auto& self, const std::filesystem::path& folder, int depth) -> bool {
        if (cancelled && cancelled()) return false;
        if (depth == 2) return true;
        for (const auto& child : FileSystem::List(folder.u8string(), "", true)) {
            if (cancelled && cancelled()) return false;
            const auto path = folder / std::filesystem::u8path(child);
            std::error_code ec;
            if (std::filesystem::is_symlink(path, ec) || ec) continue;
            auto found = ValidateFolder(path.u8string(), cancelled);
            bool releaseFolder = false;
            for (auto match : found.mMatches) {
                // Shared splash/terrain files in unrelated subfolders can look
                // like partial demos. Broad discovery requires a complete game;
                // selecting that folder directly still gives incomplete reports.
                if (!match.IsComplete()) continue;
                if (FileSystem::Equal(match.mResolvedPath, found.mResolvedRoot)) releaseFolder = true;
                const bool duplicate = std::any_of(result.mMatches.begin(), result.mMatches.end(),
                    [&](const sVersionMatch& existing) {
                        return existing.mVersion == match.mVersion &&
                            FileSystem::Equal(existing.mResolvedPath, match.mResolvedPath);
                    });
                if (!duplicate) {
                    match.mIsRawFolder = true;
                    result.mMatches.push_back(std::move(match));
                }
            }
            // A release's support directories are not another collection.
            if (!releaseFolder && !self(self, path, depth + 1)) return false;
        }
        return true;
    };
    if (!scanChildren(scanChildren, std::filesystem::u8path(result.mResolvedRoot), 0)) return {};
    // A collection offers complete games. Loose support files in sibling
    // folders must not become extra games after installed releases are hidden.
    if (std::any_of(result.mMatches.begin(), result.mMatches.end(),
            [](const sVersionMatch& match) { return match.IsComplete(); }))
        result.mMatches.erase(std::remove_if(result.mMatches.begin(), result.mMatches.end(),
            [](const sVersionMatch& match) { return !match.IsComplete(); }), result.mMatches.end());
    return result;
}

void RemoveOverlappingMatches(sValidationResult& result) {
    const auto original = result.mMatches;
    result.mMatches.erase(std::remove_if(result.mMatches.begin(), result.mMatches.end(),
        [&](const sVersionMatch& match) {
            if (!match.mVersion) return true;
            return std::any_of(original.begin(), original.end(), [&](const sVersionMatch& other) {
                if (!other.mVersion || !other.IsComplete() ||
                    !FileSystem::Equal(other.mResolvedPath, match.mResolvedPath)) return false;
                if (!match.IsComplete()) return true;
                return match.mFilesMd5Checked && other.mFilesMd5Checked &&
                    match.mVersion->mRelease == eRelease::Retail && other.mVersion->mRelease == eRelease::Retail &&
                    match.mVersion->mGame == other.mVersion->mGame && match.mVersion->mPlatform == other.mVersion->mPlatform &&
                    other.mFilesMd5Match * match.mFilesMd5Checked > match.mFilesMd5Match * other.mFilesMd5Checked;
            });
        }), result.mMatches.end());
}

void ExcludeAvailable(sValidationResult& result, const std::vector<const sGameVersion*>& available) {
    RemoveOverlappingMatches(result);
    result.mMatches.erase(std::remove_if(result.mMatches.begin(), result.mMatches.end(),
        [&](const sVersionMatch& match) {
            if (std::find(available.begin(), available.end(), match.mVersion) == available.end()) return false;
            const bool recorded = std::any_of(result.mAvailableMatches.begin(), result.mAvailableMatches.end(),
                [&](const sVersionMatch& existing) {
                    return existing.mVersion == match.mVersion &&
                        FileSystem::Equal(existing.mResolvedPath, match.mResolvedPath);
                });
            if (!recorded) result.mAvailableMatches.push_back(match);
            return true;
        }), result.mMatches.end());
    result.mAlreadyAvailable = !result.mAvailableMatches.empty() && result.mMatches.empty();
}

sValidationResult ValidateImage(const cMountedImage& image, const CancelCheck& cancelled) {
    sValidationResult result;
    result.mInputFolder = result.mResolvedRoot = image.ImagePath();
    HashCache hashes;
    for (const auto& version : KnownGameVersions) {
        if (cancelled && cancelled()) return {};
        if (version.mFiles.empty()) continue;
        sVersionMatch match;
        match.mVersion = &version;
        match.mFilesExpected = version.mFiles.size();
        match.mResolvedPath = image.ImagePath();
        for (const auto& file : version.mFiles) {
            if (cancelled && cancelled()) return {};
            const auto name = ToLower(file.mName);
            if (!image.FileExists(name)) continue;
            ++match.mFilesFound;
            if (!file.mChecksum || !*file.mChecksum) continue;
            ++match.mFilesMd5Checked;
            auto hash = hashes.find(name);
            if (hash == hashes.end()) {
                std::string value;
                std::string unpacked;
                if (const auto bytes = image.Read(image.ResolveBasename(name))) {
                    unpacked = UnpackedHash(*bytes, cancelled);
                    md5_context context;
                    md5_starts(&context);
                    md5_update(&context, const_cast<uint8*>(bytes->data()), (uint32)bytes->size());
                    uint8 digest[16]; md5_finish(&context, digest);
                    for (auto byte : digest) {
                        value += "0123456789ABCDEF"[byte >> 4];
                        value += "0123456789ABCDEF"[byte & 15];
                    }
                }
                hash = hashes.emplace(name, std::make_pair(std::move(value), std::move(unpacked))).first;
            }
            if (hash->second.first == file.mChecksum || hash->second.second == file.mChecksum) ++match.mFilesMd5Match;
        }
        if (LooksAuthentic(match)) result.mMatches.push_back(match);
    }
    return result;
}

std::string DescribeMatch(const sVersionMatch& pMatch) {
    if (!pMatch.mVersion)
        return "(none)";

    std::ostringstream os;
    // Short name keeps the result-screen line under control; the long name
    // would frequently push past the 320-px row content budget.
    if (!pMatch.mVersion->mShortName.empty())
        os << pMatch.mVersion->mShortName;
    else
        os << pMatch.mVersion->mName;

    if (pMatch.IsComplete()) {
        os << " - complete";
        if (pMatch.mFilesMd5Checked > 0 && pMatch.mFilesMd5Match < pMatch.mFilesMd5Checked) {
            os << " " << pMatch.mFilesMd5Match << "/" << pMatch.mFilesMd5Checked << " MD5";
        }
    } else {
        os << " - " << pMatch.mFilesFound << "/" << pMatch.mFilesExpected << " files";
    }
    if (pMatch.mIsRawFolder)
        os << " [RAW]";
    return os.str();
}

std::string DescribeResult(const sValidationResult& pResult) {
    std::ostringstream os;
    if (pResult.mMatches.empty()) {
        os << "No Cannon Fodder data recognised in this folder.";
        return os.str();
    }

    bool anyComplete = false;
    for (auto& m : pResult.mMatches)
        if (m.IsComplete()) { anyComplete = true; break; }

    if (anyComplete) {
        os << "Detected: ";
        bool first = true;
        for (auto& m : pResult.mMatches) {
            if (!m.IsComplete())
                continue;
            if (!first) os << "; ";
            os << DescribeMatch(m);
            first = false;
        }
    } else {
        os << "Partial match: ";
        bool first = true;
        for (auto& m : pResult.mMatches) {
            if (!first) os << "; ";
            os << DescribeMatch(m);
            first = false;
        }
    }
    return os.str();
}

std::vector<std::string> MissingFiles(const sVersionMatch& pMatch) {
    std::vector<std::string> missing;
    if (!pMatch.mVersion || pMatch.IsComplete() || pMatch.mResolvedPath.empty())
        return missing;

    // Re-list the resolved directory and find which of mFiles[] are absent.
    auto disk = FileSystem::List(StripTrailingSlash(pMatch.mResolvedPath), "", false);
    std::vector<std::string> diskLower;
    diskLower.reserve(disk.size());
    for (auto& d : disk)
        diskLower.push_back(ToLower(d));

    for (auto& kf : pMatch.mVersion->mFiles) {
        std::string wantLower = ToLower(kf.mName);
        if (std::find(diskLower.begin(), diskLower.end(), wantLower) == diskLower.end())
            missing.push_back(kf.mName);
    }
    return missing;
}

// ---------------------------------------------------------------------------
// Diagnostic: --probe-image <path>
// Mounts a disk image via firy and prints what's inside. Lives next to the
// validation logic because both modules ultimately do the same job — answer
// "what's in this thing?" — but ProbeImage is for any image type whereas
// ValidateFolder is for already-extracted Cannon Fodder folders on disk.
// ---------------------------------------------------------------------------
int ProbeImage(const std::string& pPath) {
    cMountedImage image;
    if (!image.Open(pPath)) {
        std::cout << "FAIL: could not open image or disk set: " << pPath << "\n"
                  << "      " << (image.LastError().empty() ?
                      "File missing, unreadable member, or unsupported disk format." : image.LastError()) << "\n";
        return 1;
    }
    std::cout << "OK: " << pPath << "\n"
              << "    type=" << image.ImageTypeShort() << "\n"
              << "    disks=" << image.DiskCount() << "\n";
    for (const auto& name : image.ListBasenamesLower()) {
        auto data = image.Read(image.ResolveBasename(name));
        if (!data) {
            std::cout << "FAIL: could not read " << name << "\n";
            return 1;
        }
        std::cout << "    " << name << " (" << data->size() << " bytes)\n";
    }
    return 0;
}

} // namespace Setup
