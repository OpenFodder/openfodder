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

#include <cstdint>
#include <string>
#include <vector>

namespace Setup {

// Minimal CUE/BIN sheet parser. Targets CD32 dumps (track 1 = ISO9660 game
// data, track 2+ = MPEG / audio tracks). Intentionally tiny: we only
// understand the FILE / TRACK / INDEX directives that real .cue files
// in the wild use, and we only translate sector counts → byte ranges for
// the binary-data and MODE2 tracks the engine cares about.
//
// All the fancy cue features (PREGAP, POSTGAP, ISRC, CATALOG, REM date) are
// ignored as comments. Multi-FILE sheets are supported; each track records
// which file it lives in.

struct sCueTrack {
    int         mNumber = 0;        // 1-based track index from "TRACK 01 ..."
    std::string mMode;              // e.g. "MODE1/2048", "MODE2/2352", "AUDIO"
    size_t      mFileIndex = 0;     // Index into sCueSheet::mFiles
    uint64_t    mStartByte = 0;     // Byte offset within the file where this track's INDEX 01 begins
    uint64_t    mEndByteExclusive = 0; // Byte offset where this track ends (= next track's start, or EOF)

    // Sector size in bytes implied by mMode. 2048 for MODE1/2048, 2352 for
    // MODE1/2352, 2336 for MODE2/2336, 2352 for MODE2/2352 / AUDIO.
    size_t      mSectorSize = 2048;
};

struct sCueFile {
    std::string mPath;              // Resolved absolute path to the BIN/MPEG/etc.
    std::string mFormat;            // "BINARY", "MOTOROLA", "WAVE", "MP3" — usually "BINARY"
    uint64_t    mSizeBytes = 0;     // Cached for tail-track end calculation
};

struct sCueSheet {
    std::string             mCuePath;
    std::vector<sCueFile>   mFiles;
    std::vector<sCueTrack>  mTracks;

    // Convenience: find the first track whose mNumber == pNumber. Returns
    // null if not present.
    const sCueTrack* FindTrack(int pNumber) const;
};

// Parse a .cue file. Returns false if the file can't be opened or every
// FILE directive references a missing data file. On success pOut is
// populated and ready for queries.
bool ParseCueSheet(const std::string& pCuePath, sCueSheet& pOut);

// Extract track N's payload bytes to pDestPath. Returns the number of
// bytes written, or -1 on I/O failure.
//
// For MODE1/2048 tracks the data is written verbatim. For MODE1/2352 and
// MODE2/2352 tracks the per-sector ECC/EDC headers are stripped down to
// the user-data payload (2048 bytes per sector for MODE1, 2336 for MODE2
// raw — for our purposes we extract the 2048-byte cooked payload). AUDIO
// tracks are written verbatim (raw 2352-byte CD-DA samples).
int ExtractTrackTo(const sCueSheet& pSheet, int pTrackNumber, const std::string& pDestPath);

// True iff pPath has a .cue extension (case-insensitive). Convenience for
// the wizard's image-vs-folder dispatcher.
bool LooksLikeCueSheet(const std::string& pPath);

} // namespace Setup
