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
#include "Setup/MountedImage.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <mutex>
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

std::string cMountedImage::ImageTypeShort() const {
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
    return mFiles.find(pBasenameLower) != mFiles.end();
}

std::vector<std::string> cMountedImage::ListBasenamesLower() const {
    std::vector<std::string> out;
    out.reserve(mFiles.size());
    for (auto& [k, _] : mFiles)
        out.push_back(k);
    return out;
}

std::string cMountedImage::ResolveBasename(const std::string& pBasenameLower) const {
    auto it = mFiles.find(pBasenameLower);
    if (it == mFiles.end())
        return {};
    return it->second;
}

std::shared_ptr<std::vector<uint8_t>> cMountedImage::Read(const std::string& pInImagePath) const {
#ifdef OPENFODDER_HAS_FIRY
    if (!mImage)
        return std::make_shared<std::vector<uint8_t>>();

    auto file = mImage->filesystemFile(pInImagePath);
    if (!file)
        return std::make_shared<std::vector<uint8_t>>();

    auto buf = file->read();
    if (!buf)
        return std::make_shared<std::vector<uint8_t>>();

    // firy's spBuffer is a wrapper around std::vector<uint8_t>; the engine
    // expects a bare std::vector<uint8_t>. Copy out — these reads happen at
    // most a few thousand times per session (per file in the manifest).
    auto out = std::make_shared<std::vector<uint8_t>>(buf->size());
    if (buf->size())
        std::memcpy(out->data(), buf->data(), buf->size());
    return out;
#else
    (void)pInImagePath;
    return std::make_shared<std::vector<uint8_t>>();
#endif
}

// Best-effort mkdir-p. Walks the path, creating intermediate dirs along the
// way. Returns false only if the final directory could not be ensured.
static bool ensureDirExists(const std::string& pPath) {
    if (pPath.empty())
        return false;

    // Split on '/' (and '\' on Windows) and create progressively.
    std::string current;
    current.reserve(pPath.size());
    for (size_t i = 0; i < pPath.size(); ++i) {
        char c = pPath[i];
        current.push_back(c);
        const bool sep = (c == '/' || c == '\\');
        const bool last = (i + 1 == pPath.size());
        if (!sep && !last)
            continue;

        // Trim trailing slash for the mkdir call but leave it in `current`
        // for the next iteration's path-building.
        std::string tryDir = current;
        if (sep && tryDir.size() > 1)
            tryDir.pop_back();

        // Skip drive roots ("C:") on Windows and the empty string for
        // POSIX absolute paths starting with '/'.
        if (tryDir.empty())
            continue;
        if (tryDir.size() == 2 && tryDir[1] == ':')
            continue;

#ifdef _WIN32
        // _mkdir returns 0 on success; EEXIST on already-exists.
        if (_mkdir(tryDir.c_str()) != 0 && errno != EEXIST)
            return false;
#else
        if (mkdir(tryDir.c_str(), 0755) != 0 && errno != EEXIST)
            return false;
#endif
    }
    return g_ResourceMan ? g_ResourceMan->DirExists(pPath) : true;
}

int cMountedImage::CopyToFolder(const std::string& pDestDir,
                                 const std::vector<std::string>& pBasenamesLowerSubset) const {
    if (!ensureDirExists(pDestDir))
        return -1;

    // Build a set for fast subset membership checks.
    std::set<std::string> wanted;
    if (!pBasenamesLowerSubset.empty()) {
        for (auto& b : pBasenamesLowerSubset)
            wanted.insert(b);
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

        std::ofstream out(outPath.c_str(), std::ios::binary | std::ios::trunc);
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
