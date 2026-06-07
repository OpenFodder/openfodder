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
#include "Setup/Validation.hpp"
#include "Utils/md5.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>

#ifdef OPENFODDER_HAS_FIRY
// firy.hpp transitively includes the filesystem/* and images/* helpers we
// need; firy's headers don't have include guards, so don't pull them in
// individually after firy.hpp.
#include "firy.hpp"
#endif

namespace Setup {

bool sVersionMatch::IsRetail() const {
    return mVersion && mVersion->mRelease == eRelease::Retail;
}

static std::string ToLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

// Strip a single trailing '/' or '\' if present. Don't strip if it's the only
// character (e.g. unix root, or "/").
static std::string StripTrailingSlash(const std::string& pPath) {
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

// File-presence check that doesn't lie about directories. Mirrors the test
// cResourceMan::FileExists already does (stat-based) but kept local to avoid
// pulling in the full ResourceMan API surface.
static bool PathExists(const std::string& pPath) {
    return g_ResourceMan && g_ResourceMan->FileExists(pPath);
}

static bool DirExists(const std::string& pPath) {
    return g_ResourceMan && g_ResourceMan->DirExists(pPath);
}

// "Fingerprint" files that uniquely identify a release subfolder if the user
// picked the release dir itself rather than its parent. Maps to mDataPath.
//
// We deliberately keep this list short and only use files that appear in the
// release as actual on-disk filenames (not in DAT containers).
struct sFingerprint {
    const char* mFile;         // case-insensitive match against directory listing
    const char* mDataPath;     // matches sGameVersion::mDataPath
};

static const sFingerprint kFingerprints[] = {
    { "CFTITLE.RAW",  "Amiga"     },     // Amiga retail CF1 (also several demos)
    { "CF_ENG.DAT",   "Dos_CD"    },     // PC retail CF1 (treated as CF1; if CF2's CF_ENG.DAT we'll learn from full validation)
    { "CFCOVER.DAT",  "PCFormat"  },     // PC Format demo
    { "CANNON2.DAT",  "PCFormat"  },     // PC Format demo (alt name)
};

// True if pFolder itself has a known release-fingerprint filename in it.
// If so, returns the matching mDataPath via pOutDataPath.
static bool LooksLikeReleaseSubfolder(const std::string& pFolder, std::string& pOutDataPath) {
    if (!g_ResourceMan)
        return false;

    auto files = g_ResourceMan->DirectoryList(pFolder, "");
    for (auto& f : files) {
        std::string lower = ToLower(f);
        for (auto& fp : kFingerprints) {
            if (lower == ToLower(fp.mFile)) {
                pOutDataPath = fp.mDataPath;
                return true;
            }
        }
    }
    return false;
}

// Try to coerce pInput into a "<root>" the engine will accept — i.e. a folder
// that has a "Data" subdirectory whose children include known release dirs.
//
// Strategy:
//   1. If <pInput>/Data exists, pInput IS the root.
//   2. Otherwise, test if pInput is itself a release subfolder (e.g. user
//      pointed straight at a folder containing CFTITLE.RAW). If so, walk up
//      twice to a synthetic root: pInput → <parent>/Data/<mDataPath> mapping.
//      We can't synthesise on-disk dirs from here, so we expose this as an
//      explicit "the user picked a release folder" hint and the wizard
//      writes the parent as the [paths] entry.
//   3. Otherwise, give up — the caller will report "no Data/ subfolder found".
//
// Returns the folder to USE as the <root> [paths] entry, plus a boolean
// indicating whether we actually found a Data/ child there.
static std::string ResolveRoot(const std::string& pInput, bool& pOutHasDataChild) {
    pOutHasDataChild = false;

    if (pInput.empty())
        return pInput;

    std::string normalised = StripTrailingSlash(Canonicalise(pInput));
    if (normalised.empty() || normalised == "/")
        return pInput;

    // Case 1: pInput already has a Data/ subdirectory.
    if (DirExists(normalised + "/Data")) {
        pOutHasDataChild = true;
        return normalised;
    }

    // Case 2: pInput IS a release subfolder. Try to walk up to a synthetic
    // root, BUT only if a sibling chain "../Data/<dataPath>" makes sense.
    // (User dragged the Amiga/ folder itself from somewhere.)
    std::string releaseDataPath;
    if (LooksLikeReleaseSubfolder(normalised, releaseDataPath)) {
        // The release files are right here. The most useful behaviour is to
        // tell the engine "this folder == the Data/<X>/ folder". We do that
        // by going two parents up — but only if the path layout matches.
        //
        // E.g. C:/Games/CF/Data/Amiga/  → root = C:/Games/CF/
        size_t lastSlash = normalised.find_last_of('/');
        if (lastSlash != std::string::npos) {
            std::string parent = normalised.substr(0, lastSlash);
            // Is parent ".../Data"?
            size_t parentSlash = parent.find_last_of('/');
            if (parentSlash != std::string::npos) {
                std::string parentLeaf = ToLower(parent.substr(parentSlash + 1));
                if (parentLeaf == "data") {
                    std::string grandparent = parent.substr(0, parentSlash);
                    if (DirExists(grandparent + "/Data")) {
                        pOutHasDataChild = true;
                        return grandparent;
                    }
                }
            }
        }

        // Fall through — caller will see mLooksLikeRoot=false and we still
        // surface the matched fingerprint so the wizard can offer guidance.
    }

    return normalised;
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
                                           bool pIsRawFolder) {
    sVersionMatch m;
    m.mVersion = &pVersion;
    m.mFilesExpected = pVersion.mFiles.size();
    m.mIsRawFolder = pIsRawFolder;

    if (m.mFilesExpected == 0)
        return m;     // Pseudo-versions (Custom / Random) have empty file lists; skip.

    std::string base = pFolder;
    if (!base.empty() && base.back() != '/')
        base.push_back('/');

    if (!DirExists(StripTrailingSlash(base)))
        return m;

    m.mResolvedPath = base;

    auto baseFiles = g_ResourceMan->DirectoryList(StripTrailingSlash(base), "");

    // Build a lowercase index of files actually on disk for O(N+M) matching.
    std::vector<std::pair<std::string,std::string>> diskLower; // <lower,original>
    diskLower.reserve(baseFiles.size());
    for (auto& bf : baseFiles)
        diskLower.emplace_back(ToLower(bf), bf);

    for (auto& kf : pVersion.mFiles) {
        std::string wantLower = ToLower(kf.mName);
        const bool hasExpectedMd5 = (kf.mChecksum != nullptr) && (kf.mChecksum[0] != '\0');

        for (auto& d : diskLower) {
            if (d.first == wantLower) {
                ++m.mFilesFound;
                if (hasExpectedMd5) {
                    ++m.mFilesMd5Checked;
                    std::string fullPath = base + d.second;
                    std::string md5 = g_ResourceMan->FileMD5(fullPath);
                    if (!md5.empty() && md5 == kf.mChecksum)
                        ++m.mFilesMd5Match;
                }
                break;
            }
        }
    }

    return m;
}

// Engine-layout shorthand: <root>/Data/<mDataPath>/.
static sVersionMatch ScanVersion(const std::string& pRoot, const sGameVersion& pVersion) {
    std::string base = pRoot;
    if (!base.empty() && base.back() != '/')
        base.push_back('/');
    base += "Data/";
    base += pVersion.mDataPath;
    base += '/';
    return ScanFolderForVersion(base, pVersion, /*pIsRawFolder=*/false);
}

sValidationResult ValidateFolder(const std::string& pFolder) {
    sValidationResult result;
    result.mInputFolder = pFolder;

    if (!g_ResourceMan)
        return result;

    bool hasDataChild = false;
    std::string root = ResolveRoot(pFolder, hasDataChild);
    result.mResolvedRoot = root;
    result.mLooksLikeRoot = hasDataChild;

    // Even when hasDataChild is false we still attempt scans so we can offer
    // useful diagnostics; ScanVersion bails fast on missing dirs.
    //
    // ::KnownGameVersions is declared in Versions.hpp at the global scope.
    // Skip pseudo-versions (mFiles empty) — they're handled by the engine
    // via "any retail present implies Custom/Random can be used".
    //
    // Two scan modes per version:
    //   (a) Engine layout:    <root>/Data/<mDataPath>/<files>
    //   (b) Raw extraction:   <root>/<files>           (the picked folder is itself the release)
    //
    // The user might point the wizard at either: a parent that contains a
    // proper Data/ tree (typical of a previous OpenFodder install), OR a
    // bare folder full of CFTITLE.RAW + 1.RAW + ... extracted directly from
    // the original media. Both are valid inputs.
    //
    // If both scans hit for the same version, prefer the more-complete one;
    // tie-break on engine layout (it matches what the engine reads at
    // runtime, no copying needed).
    // Filename-only matches are unreliable: CF1 and CF2 share many filenames
    // (1.RAW, FONT.RAW, ...) but the bytes differ. Same for retail-vs-demo,
    // and same between CF1 Amiga floppy and CF1 Amiga CD32 — they share
    // ~9 splash/title MD5s but differ on the other ~220 files.
    //
    // Rule: more than half of the verifiable MD5s we managed to read must
    // match the manifest. A few overlapping MD5s with another release is
    // not evidence — it's filename + identical-byte coincidence on a small
    // subset of files. Real data has near-100% MD5 match.
    auto looksAuthentic = [](const sVersionMatch& m) -> bool {
        if (m.mFilesFound == 0)
            return false;
        if (m.mFilesMd5Checked == 0)
            return true;     // No checksums to verify against — trust filenames.

        // Strict majority of MD5-checked files must match the manifest.
        // CF1 Amiga floppy and CF1 Amiga CD32 share ~9 MD5s out of ~226
        // files (the splash RAWs); requiring >50% rejects the wrong
        // manifest cleanly while still accepting near-complete real data
        // with a couple of legitimate MD5 mismatches (e.g. localised
        // builds, minor patches).
        return (m.mFilesMd5Match * 2) > m.mFilesMd5Checked;
    };

    for (size_t i = 0; i < 20; ++i) {
        const sGameVersion& kv = ::KnownGameVersions[i];
        if (kv.mFiles.empty())
            continue;

        sVersionMatch engineMatch = ScanVersion(root, kv);
        sVersionMatch rawMatch    = ScanFolderForVersion(root, kv, /*pIsRawFolder=*/true);

        const bool engineOk = looksAuthentic(engineMatch);
        const bool rawOk    = looksAuthentic(rawMatch);

        // Pick whichever found more files; prefer engine layout on a tie.
        // Filter out filename-only collisions before comparing.
        sVersionMatch* keep = nullptr;
        if (engineOk && rawOk) {
            keep = (engineMatch.mFilesFound >= rawMatch.mFilesFound) ? &engineMatch : &rawMatch;
        } else if (engineOk) {
            keep = &engineMatch;
        } else if (rawOk) {
            keep = &rawMatch;
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
    auto disk = g_ResourceMan->DirectoryList(StripTrailingSlash(pMatch.mResolvedPath), "");
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
#ifdef OPENFODDER_HAS_FIRY
static void probe_DumpDir(std::shared_ptr<firy::filesystem::sDirectory> pDir,
                            const std::string& pPrefix,
                            int pDepth,
                            int pMaxDepth) {
    if (!pDir || pDepth > pMaxDepth) return;

    auto& nodes = pDir->mNodes;
    int shown = 0;
    for (auto& node : nodes) {
        if (++shown > 20) {
            std::cout << pPrefix << "  (and " << (nodes.size() - 20) << " more)\n";
            break;
        }
        if (node->isDirectory()) {
            std::cout << pPrefix << "[" << node->nameGet() << "]\n";
            auto sub = std::dynamic_pointer_cast<firy::filesystem::sDirectory>(node);
            if (sub) {
                if (!sub->entriesLoaded()) {
                    auto fs = sub->filesystemGet().lock();
                    if (fs) fs->filesystemDirectoryLoad(sub);
                }
                probe_DumpDir(sub, pPrefix + "  ", pDepth + 1, pMaxDepth);
            }
        } else {
            std::cout << pPrefix << node->nameGet()
                      << " (" << node->sizeInBytesGet() << " bytes)\n";
        }
    }
}
#endif

int ProbeImage(const std::string& pPath) {
#ifdef OPENFODDER_HAS_FIRY
    if (!firy::gFiry) {
        std::cout << "FAIL: firy global is null (link order?)\n";
        return 1;
    }
    auto image = firy::gFiry->openImage(pPath);
    if (!image) {
        std::cout << "FAIL: openImage(\"" << pPath << "\") returned null\n";
        std::cout << "      (file missing? format unsupported?)\n";
        return 1;
    }

    std::cout << "OK: " << pPath << "\n"
              << "    type=" << image->imageType() << "\n"
              << "    label=\"" << image->filesystemNameGet() << "\"\n"
              << "    contents:\n";

    auto root = image->filesystemPath("/");
    if (!root) {
        std::cout << "    (no root directory)\n";
        return 0;
    }
    probe_DumpDir(root, "      ", 0, 3);
    return 0;
#else
    (void)pPath;
    std::cout << "FAIL: firy not compiled in (OPENFODDER_HAS_FIRY undefined)\n";
    return 1;
#endif
}

} // namespace Setup
