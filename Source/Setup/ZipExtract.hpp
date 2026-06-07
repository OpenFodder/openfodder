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

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace Setup {

// Progress callback fired once per archive entry during ExtractAllTo. Return
// false to abort extraction — the in-progress entry will not be completed and
// ExtractAllTo will set LastError() to "aborted by callback" and return false.
//   pEntryIdx   — zero-based index of the entry about to be extracted
//   pEntryCount — total number of entries in the archive
//   pCurrentName — the entry's archive-relative name (post-sanitisation)
using ZipProgressCallback = std::function<bool(size_t pEntryIdx, size_t pEntryCount, const std::string& pCurrentName)>;

// RAII wrapper around a miniz zip reader. The miniz handle is held opaquely
// (mImpl) so callers don't need miniz.h on their include path. One archive per
// instance — call Close() (or destruct) before re-opening.
//
// Use OpenFromFile for archives on disk and OpenFromMemory for archives held
// in a buffer (e.g. read out of a disk image). Memory archives are copied
// into mMemoryCopy at Open time so the caller's buffer can be freed
// immediately after the call returns.
class ZipArchive {
public:
    ZipArchive();
    ~ZipArchive();

    ZipArchive(const ZipArchive&) = delete;
    ZipArchive& operator=(const ZipArchive&) = delete;

    // Open an in-memory zip. The pointed-to bytes are copied into an internal
    // buffer that lives for the lifetime of the archive — the caller may free
    // pData as soon as this call returns. Returns false on failure with
    // LastError() set.
    bool OpenFromMemory(const void* pData, size_t pSize);

    // Open a zip file from disk. Returns false on failure with LastError() set.
    bool OpenFromFile(const std::string& pPath);

    // Close any currently-open archive. Safe to call when not open. Always
    // called by the destructor.
    void Close();

    bool IsOpen() const;

    // Number of entries (files + directories) in the archive, or 0 when not open.
    size_t EntryCount() const;

    // Append every entry's archive-relative name to pOut. Returns false on
    // failure with LastError() set; pOut is left in a valid but partially-
    // populated state in that case.
    bool ListEntries(std::vector<std::string>& pOut) const;

    // Extract every entry to pOutDir, preserving the archive's directory
    // structure.
    //
    /* SAFETY: rejects absolute paths, paths containing ".." segments, and any
       entry whose lexically-normal target path escapes pOutDir. Validated
       BEFORE any file is created or written. */
    //
    // Returns false on the first failure (sanitisation reject, miniz error,
    // filesystem error, or pProgress returning false) with LastError() set.
    bool ExtractAllTo(const std::string& pOutDir, ZipProgressCallback pProgress = {});

    const std::string& LastError() const { return mLastError; }

private:
    void*                mImpl = nullptr;   // mz_zip_archive*
    std::string          mLastError;
    std::vector<uint8_t> mMemoryCopy;       // owned buffer when OpenFromMemory used
};

} // namespace Setup
