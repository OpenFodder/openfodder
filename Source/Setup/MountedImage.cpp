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

#include "Setup/MountedImage.hpp"
#include "FileSystem.hpp"
#include "Setup/CapsImageLoader.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <regex>
#include <set>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#ifdef OPENFODDER_HAS_FIRY
// firy.hpp transitively pulls in everything we need (filesystem/*, images/*,
// buffer.hpp). Don't include those individually — firy headers have no
// include guards and would redefine.
#include "firy.hpp"
#endif

namespace Setup {

// -- helpers -----------------------------------------------------------------

static std::string toLower(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (unsigned char c : s)
        out.push_back((char)std::tolower(c));
    return out;
}

// -- cMountedImage -----------------------------------------------------------

cMountedImage::cMountedImage() = default;
cMountedImage::~cMountedImage() = default;

bool cMountedImage::Open(const std::string& pImagePath) {
    namespace fs = std::filesystem;
    mError.clear();
    mFiles.clear();
    mRawFiles.clear();
    mDisks.clear();
    mImage.reset();
    mImagePath.clear();

    // Only replace an explicit disk number. Keep publisher, language and
    // revision tags intact so neighbouring editions never get mixed together.
    const std::regex diskNumber("(disk|disc)[ _.-]*([0-9]+|[a-z])(?=[^a-z0-9]|$)",
                                std::regex::icase);
    const std::regex ofNumber("([0-9]+)([ _.-]*of[ _.-]*[0-9]+)", std::regex::icase);
    const auto setName = [&](const fs::path& path) {
        return std::regex_replace(std::regex_replace(toLower(path.stem().string()),
            diskNumber, "$1#"), ofNumber, "#$2");
    };
    const auto isFloppy = [](const std::string& extension) {
        return extension == ".adf" || extension == ".ipf" || extension == ".raw";
    };
    const fs::path selected = FileSystem::Resolve(pImagePath);
    if (selected.empty()) { mError = "Disk image or folder not found."; return false; }
    std::error_code ec;
    const bool folder = fs::is_directory(selected, ec);
    const std::string stem = toLower(selected.stem().string());
    const std::string extension = toLower(selected.extension().string());
    const bool numbered = isFloppy(extension) &&
        (std::regex_search(stem, diskNumber) || std::regex_search(stem, ofNumber));
    if (!folder && !numbered)
        return OpenSingle(selected.u8string());

    const fs::path parent = folder ? selected :
        (selected.has_parent_path() ? selected.parent_path() : fs::path("."));
    std::vector<fs::path> paths;
    fs::directory_iterator end;
    for (fs::directory_iterator it(parent, ec); !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec)) continue;
        const auto& path = it->path();
        const std::string candidateExtension = toLower(path.extension().string());
        if (isFloppy(candidateExtension) &&
            (folder || (candidateExtension == extension && setName(path) == setName(selected))))
            paths.push_back(path);
    }
    if (ec || paths.empty() || (!folder && !fs::is_regular_file(selected, ec)))
        return false;
    if (folder) {
        std::map<std::string, std::vector<fs::path>> numberedSets;
        for (const auto& path : paths) {
            const auto name = toLower(path.stem().string());
            if (std::regex_search(name, diskNumber) || std::regex_search(name, ofNumber))
                numberedSets[toLower(path.extension().string()) + setName(path)].push_back(path);
        }
        if (numberedSets.size() > 1) {
            mError = "This folder contains several disk sets. Select one numbered disk to choose its matching set.";
            return false;
        }
        // A save disk alongside one numbered game set is not a set member.
        if (numberedSets.size() == 1) paths = numberedSets.begin()->second;
    }
    std::sort(paths.begin(), paths.end());

    // Build transactionally: an unreadable member invalidates the set instead
    // of silently leaving files from the remaining disks available.
    std::vector<std::shared_ptr<cMountedImage>> disks;
    std::map<std::string, std::string> files;
    for (const auto& path : paths) {
        auto disk = std::make_shared<cMountedImage>();
        if (!disk->OpenSingle(path.u8string())) {
            mError = disk->LastError();
            if (mError.empty()) mError = "Could not read disk: " + path.filename().string();
            return false;
        }
        const std::string prefix = "/@disk/" + std::to_string(disks.size()) + "/";
        for (const auto& entry : disk->mFiles)
            files.emplace(entry.first, prefix + entry.second);
        disks.push_back(std::move(disk));
    }
    mImagePath = selected.u8string();
    mDisks = std::move(disks);
    mFiles = std::move(files);
    return true;
}

bool cMountedImage::OpenSingle(const std::string& pImagePath) {
    if (OpenSensibleDisk(pImagePath)) return true;
    const auto extension = toLower(std::filesystem::path(pImagePath).extension().string());
    if (extension == ".ipf" || extension == ".raw") return false;
#ifdef OPENFODDER_HAS_FIRY
    if (!firy::gFiry)
        return false;

    auto img = firy::gFiry->openImage(pImagePath);
    if (!img)
        return false;

    mImagePath = pImagePath;
    mImage = img;
    mFiles.clear();
    IndexTree();
    return true;
#else
    (void)pImagePath;
    return false;
#endif
}

// SOS directory/block layout: https://github.com/OpenFodder/SOS_Unpacker
bool cMountedImage::OpenSensibleDisk(const std::string& path) {
    const std::string extension = toLower(std::filesystem::path(path).extension().string());
    std::vector<std::vector<uint8_t>> tracks;
    if (extension == ".adf") {
        std::ifstream input(std::filesystem::u8path(path), std::ios::binary | std::ios::ate);
        if (!input || input.tellg() != 901120) return false;
        std::vector<uint8_t> bytes(901120);
        input.seekg(0);
        if (!input.read(reinterpret_cast<char*>(bytes.data()), bytes.size())) return false;
        int swapped = -1;
        for (int side = 0; side < 2; ++side)
            if (std::memcmp(bytes.data() + (2 ^ side) * 5632, "Sensi disk", 10) == 0) swapped = side;
        if (swapped < 0) return false;
        tracks.resize(160);
        for (size_t i = 0; i < tracks.size(); ++i) {
            const size_t at = (i ^ swapped) * 5632;
            tracks[i].assign(bytes.begin() + at, bytes.begin() + at + 5632);
        }
    } else if (extension == ".ipf" || extension == ".raw") {
        if (!LoadSensibleTracks(path, tracks, mError)) return false;
    } else return false;

    const auto fail = [&](const std::string& detail = "") {
        mError = "The Sensible disk directory or a required file chain is damaged or incomplete. " + detail;
        return false;
    };
    // Original SOS volume labels differ from the cracked ADF labels. The
    // preservation path already validated this track's SOS6 checksum.
    if (tracks.size() < 3 || tracks[2].size() < 32) return fail();
    const auto& directory = tracks[2];
    const auto be32 = [&](size_t at) {
        return (uint32_t(directory[at]) << 24) | (uint32_t(directory[at + 1]) << 16) |
               (uint32_t(directory[at + 2]) << 8) | directory[at + 3];
    };
    const size_t count = be32(24);
    if (!count || count > (directory.size() - 32) / 32) return fail();
    std::map<std::string, tSharedBuffer> files;
    std::map<std::string, std::string> names;
    for (size_t i = 0; i < count; ++i) {
        const size_t entry = 32 + i * 32;
        std::string name;
        for (size_t j = 0; j < 26 && directory[entry + j]; ++j) {
            const uint8_t c = directory[entry + j];
            if (c < 32 || c > 126 || c == '/' || c == '\\') return fail();
            name.push_back(char(c));
        }
        if (name.empty() || name.size() == 26 || name == "." || name == "..") return fail();
        const size_t size = be32(entry + 28);
        if (size > tracks.size() * 6144) return fail();
        size_t track = directory[entry + 26], block = directory[entry + 27];
        auto data = std::make_shared<std::vector<uint8_t>>();
        data->reserve(size);
        std::set<size_t> visited;
        while (data->size() < size) {
            if (track >= tracks.size() || (block + 1) * 512 > tracks[track].size() ||
                !visited.insert(track * 256 + block).second)
                return fail(name + " track " + std::to_string(track) + " block " + std::to_string(block));
            const auto& bytes = tracks[track];
            const size_t at = block * 512;
            const size_t take = std::min<size_t>(510, size - data->size());
            data->insert(data->end(), bytes.begin() + at, bytes.begin() + at + take);
            track = bytes[at + 510];
            block = bytes[at + 511];
        }
        const std::string filePath = "/" + name;
        const auto alias = names.find(toLower(name));
        if (alias != names.end()) {
            // Original disks contain distinct DISK2.RAW/disk2.raw markers.
            // Match the regular image index's case-insensitive first-win rule.
            continue;
        }
        names.emplace(toLower(name), filePath);
        files.emplace(filePath, std::move(data));
    }
    mRawFiles = std::move(files);
    mFiles = std::move(names);
    mImagePath = path;
    return true;
}

std::string cMountedImage::ImageTypeShort() const {
    if (!mDisks.empty()) return mDisks.front()->ImageTypeShort();
    if (!mRawFiles.empty()) return toLower(std::filesystem::path(mImagePath).extension().string()).substr(1) + "-sos";
#ifdef OPENFODDER_HAS_FIRY
    return mImage ? mImage->imageTypeShort() : std::string{};
#else
    return {};
#endif
}

void cMountedImage::IndexTree() {
#ifdef OPENFODDER_HAS_FIRY
    if (!mImage)
        return;

    // Walk from the root depth-first. We index BASENAME-LOWERCASE because
    // OpenFodder's resource layer is also basename-driven (mAmigaFiles etc.
    // in Versions_Files.hpp are bare names like "CFTITLE.RAW", not paths).
    //
    // First-write-wins for collisions: if two directories contain a file
    // with the same basename, the first one we encounter is what
    // FileExists/Read returns. In practice that's fine — Cannon Fodder's
    // disc layouts don't have collisions of interest.
    using Frame = std::pair<std::shared_ptr<firy::filesystem::sDirectory>, std::string>;
    std::vector<Frame> stack;

    auto root = mImage->filesystemPath("/");
    if (!root)
        return;
    stack.emplace_back(root, std::string("/"));

    while (!stack.empty()) {
        auto [dir, path] = stack.back();
        stack.pop_back();
        if (!dir)
            continue;

        if (!dir->entriesLoaded()) {
            auto fs = dir->filesystemGet().lock();
            if (!fs || !fs->filesystemDirectoryLoad(dir))
                continue;
        }

        for (auto& node : dir->mNodes) {
            if (!node) continue;

            if (node->isDirectory()) {
                auto sub = std::dynamic_pointer_cast<firy::filesystem::sDirectory>(node);
                if (sub) {
                    std::string subPath = path;
                    if (!subPath.empty() && subPath.back() != '/')
                        subPath.push_back('/');
                    subPath += node->nameGet();
                    stack.emplace_back(sub, subPath);
                }
            } else {
                std::string basename = node->nameGet();
                std::string keyLower = toLower(basename);

                std::string fullInImage = path;
                if (!fullInImage.empty() && fullInImage.back() != '/')
                    fullInImage.push_back('/');
                fullInImage += basename;

                // First-write-wins.
                mFiles.emplace(keyLower, fullInImage);
            }
        }
    }
#endif
}

bool cMountedImage::FileExists(const std::string& pBasenameLower) const {
    return !ResolveBasename(pBasenameLower).empty();
}

std::vector<std::string> cMountedImage::ListBasenamesLower() const {
    std::vector<std::string> out;
    out.reserve(mFiles.size());
    for (auto& [k, _] : mFiles)
        out.push_back(k);
    return out;
}

std::string cMountedImage::ResolveBasename(const std::string& pBasenameLower) const {
    auto it = mFiles.find(toLower(pBasenameLower));
    if (it != mFiles.end()) return it->second;
    for (const auto& entry : mFiles)
        if (FileSystem::Equal(entry.first, pBasenameLower)) return entry.second;
    return {};
}

tSharedBuffer cMountedImage::Read(const std::string& requestedPath) const {
    std::string pInImagePath = requestedPath;
    std::replace(pInImagePath.begin(), pInImagePath.end(), '\\', '/');
    // Use the indexed original spelling, including the disk routing prefix.
    for (const auto& entry : mFiles) {
        if (FileSystem::Equal(entry.second, pInImagePath)) {
            pInImagePath = entry.second;
            break;
        }
    }
    const auto raw = mRawFiles.find(pInImagePath);
    // Resource consumers may unpack or modify their buffer. Preserve the
    // mounted disk's bytes for subsequent reads, just like the firy path.
    if (raw != mRawFiles.end()) return std::make_shared<std::vector<uint8_t>>(*raw->second);
    if (!mDisks.empty()) {
        for (size_t i = 0; i < mDisks.size(); ++i) {
            const std::string prefix = "/@disk/" + std::to_string(i) + "/";
            if (pInImagePath.compare(0, prefix.size(), prefix) == 0)
                return mDisks[i]->Read(pInImagePath.substr(prefix.size()));
        }
        return nullptr;
    }
#ifdef OPENFODDER_HAS_FIRY
    if (!mImage)
        return nullptr;

    auto file = mImage->filesystemFile(pInImagePath);
    if (!file)
        return nullptr;

    auto buf = file->read();
    if (!buf)
        return nullptr;

    // firy's spBuffer is a wrapper around std::vector<uint8_t>; the engine
    // expects a bare std::vector<uint8_t>. Copy out — these reads happen at
    // most a few thousand times per session (per file in the manifest).
    auto out = std::make_shared<std::vector<uint8_t>>(buf->size());
    if (buf->size())
        std::memcpy(out->data(), buf->data(), buf->size());
    return out;
#else
    (void)pInImagePath;
    return nullptr;
#endif
}

// Create and verify the destination without depending on engine globals.
static bool ensureDirExists(const std::string& pPath) {
    if (pPath.empty()) return false;
    std::error_code ec;
    const auto path = FileSystem::Resolve(pPath, true);
    if (path.empty()) return false;
    std::filesystem::create_directories(path, ec);
    return !ec && std::filesystem::is_directory(path, ec) && !ec;
}

int cMountedImage::CopyToFolder(const std::string& pDestDir,
                                 const std::vector<std::string>& pBasenamesLowerSubset) const {
    if (!ensureDirExists(pDestDir))
        return -1;

    // Build a set for fast subset membership checks.
    std::set<std::string> wanted;
    if (!pBasenamesLowerSubset.empty()) {
        for (auto& b : pBasenamesLowerSubset)
            wanted.insert(toLower(b));
    }

    int written = 0;

    for (auto& [keyLower, inImagePath] : mFiles) {
        if (!wanted.empty() && wanted.find(keyLower) == wanted.end())
            continue;

        // Recover original basename casing from the in-image path so the
        // file lands on disk with whatever casing the source disc used.
        // Engine compares lower-case anyway, but case-preservation is the
        // courteous default.
        std::string basename;
        size_t lastSlash = inImagePath.find_last_of('/');
        basename = (lastSlash == std::string::npos) ? inImagePath
                                                     : inImagePath.substr(lastSlash + 1);
        if (basename.empty())
            continue;

        auto buf = Read(inImagePath);
        if (!buf)
            return -1;

        std::string outPath = pDestDir;
        if (!outPath.empty() && outPath.back() != '/' && outPath.back() != '\\')
            outPath.push_back('/');
        outPath += basename;

        const auto target = FileSystem::Resolve(outPath, true);
        if (target.empty()) return -1;
        std::ofstream out(target, std::ios::binary | std::ios::trunc);
        if (!out)
            return -1;
        if (!buf->empty()) {
            out.write(reinterpret_cast<const char*>(buf->data()), (std::streamsize)buf->size());
            if (!out)
                return -1;
        }
        out.close();
        ++written;
    }

    return written;
}

// -- Global registry ---------------------------------------------------------

namespace {
    std::mutex& registryMutex() {
        static std::mutex m;
        return m;
    }

    // Slot 0 is reserved for "no image". Real ids are >= 1.
    std::map<int, std::shared_ptr<cMountedImage>>& registry() {
        static std::map<int, std::shared_ptr<cMountedImage>> r;
        return r;
    }
}

int MountedImageRegister(std::shared_ptr<cMountedImage> pImage) {
    if (!pImage)
        return 0;
    std::lock_guard<std::mutex> lock(registryMutex());
    auto& reg = registry();

    int id = 1;
    while (reg.count(id))
        ++id;

    reg.emplace(id, std::move(pImage));
    return id;
}

std::shared_ptr<cMountedImage> MountedImageGet(int pId) {
    std::lock_guard<std::mutex> lock(registryMutex());
    auto& reg = registry();
    auto it = reg.find(pId);
    return (it == reg.end()) ? nullptr : it->second;
}

void MountedImageUnregister(int pId) {
    std::lock_guard<std::mutex> lock(registryMutex());
    registry().erase(pId);
}

std::vector<std::pair<int, std::shared_ptr<cMountedImage>>> MountedImagesAll() {
    std::lock_guard<std::mutex> lock(registryMutex());
    std::vector<std::pair<int, std::shared_ptr<cMountedImage>>> out;
    out.reserve(registry().size());
    for (auto& kv : registry())
        out.emplace_back(kv.first, kv.second);
    return out;
}

// -- Virtual path helpers ----------------------------------------------------

static const char* kVirtScheme = "firy://";
static const size_t kVirtSchemeLen = 7;

bool VirtualPathIs(const std::string& pPath) {
    return pPath.size() >= kVirtSchemeLen &&
           std::memcmp(pPath.data(), kVirtScheme, kVirtSchemeLen) == 0;
}

std::string VirtualPathBuild(int pId, const std::string& pInImagePath) {
    std::string out = kVirtScheme;
    out += std::to_string(pId);
    if (!pInImagePath.empty() && pInImagePath.front() != '/')
        out.push_back('/');
    out += pInImagePath;
    return out;
}

bool VirtualPathParse(const std::string& pPath, int& pOutId, std::string& pOutInImagePath) {
    if (!VirtualPathIs(pPath))
        return false;

    // After the scheme, expect <digits>/<rest>.
    size_t pos = kVirtSchemeLen;
    int id = 0;
    bool gotDigit = false;
    while (pos < pPath.size() && std::isdigit((unsigned char)pPath[pos])) {
        id = id * 10 + (pPath[pos] - '0');
        ++pos;
        gotDigit = true;
    }
    if (!gotDigit)
        return false;
    if (pos >= pPath.size())
        return false;       // missing trailing path
    if (pPath[pos] != '/')
        return false;

    pOutId = id;
    pOutInImagePath = pPath.substr(pos);     // keep the leading '/'
    return true;
}

} // namespace Setup
