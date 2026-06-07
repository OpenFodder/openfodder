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

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

// Forward decl — firy types are pointer-only here so no firy header leaks.
namespace firy {
    namespace images { class cImage; }
    using spImage = std::shared_ptr<images::cImage>;
}

namespace Setup {

// Wraps a firy::spImage and exposes a flat case-insensitive lookup table
// (lowercase basename → in-image absolute path) of every regular file the
// image contains. The setup wizard / cResourceMan use this to mount disk
// images at runtime instead of extracting them.
//
// Each cMountedImage gets a unique numeric id assigned at registration time;
// virtual paths are encoded as "firy://<id>/<inImagePath>" so the engine's
// existing FileRead / FindVersionPath / GetFilePath plumbing can route reads
// to the right image without changing its public API.
class cMountedImage {
public:
    cMountedImage();
    ~cMountedImage();

    // Open pImagePath via firy and index every regular file. Returns false
    // if firy can't recognise the format. Idempotent — calling twice on the
    // same instance reopens.
    bool Open(const std::string& pImagePath);

    // Path that was passed to Open(). Used for INI persistence and as the
    // human-readable label in the wizard.
    const std::string& ImagePath() const { return mImagePath; }

    // Short type tag, e.g. "iso" or "adf". Empty if not opened.
    std::string ImageTypeShort() const;

    // Indexed file presence — case-insensitive on basename.
    bool FileExists(const std::string& pBasenameLower) const;

    // List every basename-lower the image contains. Used by ResourceMan
    // to feed findVersions-style scanning logic.
    std::vector<std::string> ListBasenamesLower() const;

    // Read the file identified by an in-image absolute path (the value
    // returned by ResolveBasename). Returns null on miss.
    std::shared_ptr<std::vector<uint8_t>> Read(const std::string& pInImagePath) const;

    // For a basename-lower we previously indexed, return the in-image
    // absolute path (e.g. "/CF_ENG.DAT"). Empty if missing.
    std::string ResolveBasename(const std::string& pBasenameLower) const;

    // Copy every file we have indexed into pDestDir (created if missing),
    // preserving the original basename casing. The destination directory
    // is overlaid: existing files with the same name are overwritten so
    // re-running the wizard updates a previously-extracted install.
    //
    // On success returns the number of files written. Returns -1 on
    // I/O failure (mid-copy) so the caller can roll back if needed.
    //
    // pBasenamesLowerSubset: if non-empty, only files whose basename-lower
    // is in this set are copied. Used by "copy this version's manifest".
    // If empty, everything in mFiles is copied.
    int CopyToFolder(const std::string& pDestDir,
                     const std::vector<std::string>& pBasenamesLowerSubset) const;

private:
    std::string mImagePath;
    firy::spImage mImage;

    // basename-lower → in-image absolute path. Built once at Open() time so
    // every subsequent FileRead is a single hash lookup.
    std::map<std::string, std::string> mFiles;

    // Walk the in-image directory tree depth-first, populating mFiles.
    void IndexTree();
};

// -- Global registry ---------------------------------------------------------
//
// cResourceMan addresses mounted images via a numeric id baked into the
// virtual path "firy://<id>/...". The registry owns the cMountedImage
// instances so paths remain valid for as long as the image is mounted.
//
// The id-zero slot is reserved for "no image" so a default-constructed id
// can be used as a sentinel.

// Mount a fresh image and return its registry id. Returns 0 on failure.
int MountedImageRegister(std::shared_ptr<cMountedImage> pImage);

// Look up by id. Returns null if the slot is empty.
std::shared_ptr<cMountedImage> MountedImageGet(int pId);

// Drop an entry. The image's ref-count decrements; firy state is released
// when the last shared_ptr goes away.
void MountedImageUnregister(int pId);

// All currently mounted (id, image) pairs — for INI write-back and for the
// wizard's "mounted images" diagnostic.
std::vector<std::pair<int, std::shared_ptr<cMountedImage>>> MountedImagesAll();

// -- Virtual path helpers ----------------------------------------------------
//
// Format: "firy://<id>/<inImagePath>"
//
// Why a custom scheme rather than the host filesystem path? Two reasons:
//   1. cResourceMan::FileRead is plain std::ifstream — passing a real path
//      that doesn't actually exist on disk would silently return an empty
//      buffer. The "firy://" prefix gives FileRead an unambiguous cue to
//      route the read to the registry instead.
//   2. The same image can be mounted twice (rare but possible — e.g. user
//      adds a CF1 ADF and a CF2 ADF). The id keeps them distinct without
//      having to embed the disk path inside every release-files map entry.

bool VirtualPathParse(const std::string& pPath, int& pOutId, std::string& pOutInImagePath);
std::string VirtualPathBuild(int pId, const std::string& pInImagePath);
bool VirtualPathIs(const std::string& pPath);

} // namespace Setup
