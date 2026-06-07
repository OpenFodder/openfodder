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

#include "Setup/ZipExtract.hpp"

#include "Utils/miniz.h"

#include <cstring>
#include <filesystem>
#include <system_error>

namespace Setup {

namespace {

// Pull the most recent miniz error string for a given archive into a std::string,
// falling back to a generic message if miniz can't (or won't) describe it.
std::string MinizError(mz_zip_archive* pZip, const char* pFallback) {
    if (pZip != nullptr) {
        const mz_zip_error err = mz_zip_get_last_error(pZip);
        if (err != MZ_ZIP_NO_ERROR) {
            const char* msg = mz_zip_get_error_string(err);
            if (msg != nullptr && msg[0] != '\0')
                return msg;
        }
    }
    return pFallback != nullptr ? std::string(pFallback) : std::string("unknown miniz error");
}

// True if pName is an unsafe archive entry name. Rejects:
//   - empty names
//   - leading '/' or '\\' (absolute on POSIX / UNC-ish on Windows)
//   - any path component equal to ".."
//   - drive-letter prefixes like "C:" / "c:..."
// Pure string-level test; the post-normalise containment check is separate.
bool IsUnsafeEntryName(const std::string& pName) {
    if (pName.empty())
        return true;

    const char first = pName.front();
    if (first == '/' || first == '\\')
        return true;

    // Windows drive letter ("C:", "C:/foo", "C:\\foo")
    if (pName.size() >= 2) {
        const unsigned char c = static_cast<unsigned char>(pName[0]);
        const bool isAlpha = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        if (isAlpha && pName[1] == ':')
            return true;
    }

    // Walk components, splitting on either separator. Reject any "..".
    size_t start = 0;
    for (size_t i = 0; i <= pName.size(); ++i) {
        const bool atEnd = (i == pName.size());
        const char ch = atEnd ? '/' : pName[i];
        if (ch == '/' || ch == '\\') {
            const size_t len = i - start;
            if (len == 2 && pName[start] == '.' && pName[start + 1] == '.')
                return true;
            start = i + 1;
        }
    }

    return false;
}

// True if pCandidate (after lexically_normal) is pBase or sits underneath it.
// Both inputs must already be absolute. Compares via std::filesystem::path so
// platform separators are handled consistently.
bool IsPathInside(const std::filesystem::path& pBase, const std::filesystem::path& pCandidate) {
    auto baseIt = pBase.begin();
    auto candIt = pCandidate.begin();
    for (; baseIt != pBase.end(); ++baseIt, ++candIt) {
        if (candIt == pCandidate.end())
            return false;
        if (*baseIt != *candIt)
            return false;
    }
    return true;
}

} // namespace

ZipArchive::ZipArchive() = default;

ZipArchive::~ZipArchive() {
    Close();
}

bool ZipArchive::IsOpen() const {
    return mImpl != nullptr;
}

void ZipArchive::Close() {
    if (mImpl != nullptr) {
        mz_zip_archive* zip = static_cast<mz_zip_archive*>(mImpl);
        mz_zip_reader_end(zip);
        delete zip;
        mImpl = nullptr;
    }
    mMemoryCopy.clear();
    mMemoryCopy.shrink_to_fit();
}

bool ZipArchive::OpenFromFile(const std::string& pPath) {
    Close();
    mLastError.clear();

    mz_zip_archive* zip = new mz_zip_archive();
    std::memset(zip, 0, sizeof(*zip));

    if (!mz_zip_reader_init_file(zip, pPath.c_str(), 0)) {
        mLastError = MinizError(zip, "mz_zip_reader_init_file failed");
        mz_zip_reader_end(zip);
        delete zip;
        return false;
    }

    mImpl = zip;
    return true;
}

bool ZipArchive::OpenFromMemory(const void* pData, size_t pSize) {
    Close();
    mLastError.clear();

    if (pData == nullptr || pSize == 0) {
        mLastError = "OpenFromMemory: empty buffer";
        return false;
    }

    // Copy so the caller's buffer can be freed immediately after this call.
    mMemoryCopy.assign(static_cast<const uint8_t*>(pData),
                       static_cast<const uint8_t*>(pData) + pSize);

    mz_zip_archive* zip = new mz_zip_archive();
    std::memset(zip, 0, sizeof(*zip));

    if (!mz_zip_reader_init_mem(zip, mMemoryCopy.data(), mMemoryCopy.size(), 0)) {
        mLastError = MinizError(zip, "mz_zip_reader_init_mem failed");
        mz_zip_reader_end(zip);
        delete zip;
        mMemoryCopy.clear();
        mMemoryCopy.shrink_to_fit();
        return false;
    }

    mImpl = zip;
    return true;
}

size_t ZipArchive::EntryCount() const {
    if (mImpl == nullptr)
        return 0;
    mz_zip_archive* zip = static_cast<mz_zip_archive*>(mImpl);
    return static_cast<size_t>(mz_zip_reader_get_num_files(zip));
}

bool ZipArchive::ListEntries(std::vector<std::string>& pOut) const {
    if (mImpl == nullptr) {
        const_cast<ZipArchive*>(this)->mLastError = "ListEntries: archive not open";
        return false;
    }

    mz_zip_archive* zip = static_cast<mz_zip_archive*>(mImpl);
    const mz_uint count = mz_zip_reader_get_num_files(zip);
    pOut.reserve(pOut.size() + count);

    for (mz_uint i = 0; i < count; ++i) {
        // Query required size first so we can size the buffer exactly. Returned
        // size includes the trailing NUL, so the std::string gets size-1 chars.
        const mz_uint needed = mz_zip_reader_get_filename(zip, i, nullptr, 0);
        if (needed == 0) {
            const_cast<ZipArchive*>(this)->mLastError = MinizError(zip, "mz_zip_reader_get_filename failed");
            return false;
        }
        std::string name(needed, '\0');
        const mz_uint written = mz_zip_reader_get_filename(zip, i, name.data(), needed);
        if (written == 0) {
            const_cast<ZipArchive*>(this)->mLastError = MinizError(zip, "mz_zip_reader_get_filename failed");
            return false;
        }
        // Strip the trailing NUL miniz writes.
        if (!name.empty() && name.back() == '\0')
            name.pop_back();
        pOut.push_back(std::move(name));
    }

    return true;
}

bool ZipArchive::ExtractAllTo(const std::string& pOutDir, ZipProgressCallback pProgress) {
    if (mImpl == nullptr) {
        mLastError = "ExtractAllTo: archive not open";
        return false;
    }

    mz_zip_archive* zip = static_cast<mz_zip_archive*>(mImpl);

    namespace fs = std::filesystem;
    std::error_code ec;

    fs::path outDir(pOutDir);
    fs::create_directories(outDir, ec);
    if (ec) {
        mLastError = "create_directories failed: " + ec.message();
        return false;
    }

    // Resolve outDir to its absolute lexically-normal form so we can detect
    // entries that escape it after path joining. We deliberately use
    // weakly_canonical → fallback to absolute, both lexically_normal'd, so
    // this works even on filesystems that don't fully resolve symlinks.
    fs::path outAbs = fs::weakly_canonical(outDir, ec);
    if (ec || outAbs.empty()) {
        ec.clear();
        outAbs = fs::absolute(outDir, ec);
        if (ec) {
            mLastError = "absolute(outDir) failed: " + ec.message();
            return false;
        }
    }
    outAbs = outAbs.lexically_normal();

    const mz_uint count = mz_zip_reader_get_num_files(zip);
    for (mz_uint i = 0; i < count; ++i) {
        mz_zip_archive_file_stat st;
        if (!mz_zip_reader_file_stat(zip, i, &st)) {
            mLastError = MinizError(zip, "mz_zip_reader_file_stat failed");
            return false;
        }

        // st.m_filename is guaranteed zero-terminated.
        const std::string name(st.m_filename);

        if (pProgress && !pProgress(static_cast<size_t>(i), static_cast<size_t>(count), name)) {
            mLastError = "aborted by callback";
            return false;
        }

        /* SAFETY: rejects absolute paths, paths containing ".." segments, and any
           entry whose lexically-normal target path escapes pOutDir. Validated
           BEFORE any file is created or written. */
        if (IsUnsafeEntryName(name)) {
            mLastError = "unsafe archive entry name: " + name;
            return false;
        }

        // Compute and verify the destination before touching the filesystem.
        fs::path target = (outDir / fs::path(name)).lexically_normal();
        fs::path targetAbs = fs::weakly_canonical(target, ec);
        if (ec || targetAbs.empty()) {
            ec.clear();
            targetAbs = fs::absolute(target, ec);
            if (ec) {
                mLastError = "absolute(target) failed: " + ec.message();
                return false;
            }
        }
        targetAbs = targetAbs.lexically_normal();

        if (!IsPathInside(outAbs, targetAbs)) {
            mLastError = "archive entry escapes output directory: " + name;
            return false;
        }

        const bool isDir = st.m_is_directory
                            || (!name.empty() && (name.back() == '/' || name.back() == '\\'));

        if (isDir) {
            fs::create_directories(target, ec);
            if (ec) {
                mLastError = "create_directories failed for " + name + ": " + ec.message();
                return false;
            }
            continue;
        }

        // Ensure parent directory exists for the file we're about to write.
        if (target.has_parent_path()) {
            fs::create_directories(target.parent_path(), ec);
            if (ec) {
                mLastError = "create_directories(parent) failed for " + name + ": " + ec.message();
                return false;
            }
        }

        const std::string targetStr = target.string();
        if (!mz_zip_reader_extract_to_file(zip, i, targetStr.c_str(), 0)) {
            mLastError = MinizError(zip, "mz_zip_reader_extract_to_file failed") + " (" + name + ")";
            return false;
        }
    }

    return true;
}

} // namespace Setup
