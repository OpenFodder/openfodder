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
#include "Setup/CueSheet.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace Setup {

// CD-DA: 75 frames per second; each frame = 1 sector.
static constexpr uint64_t kFramesPerSecond = 75;

namespace {

    std::string trim(const std::string& s) {
        size_t a = 0;
        while (a < s.size() && std::isspace((unsigned char)s[a])) ++a;
        size_t b = s.size();
        while (b > a && std::isspace((unsigned char)s[b - 1])) --b;
        return s.substr(a, b - a);
    }

    std::string toUpper(const std::string& s) {
        std::string r;
        r.reserve(s.size());
        for (unsigned char c : s) r.push_back((char)std::toupper(c));
        return r;
    }

    std::string parentDir(const std::string& pPath) {
        std::string p = pPath;
        std::replace(p.begin(), p.end(), '\\', '/');
        size_t slash = p.find_last_of('/');
        if (slash == std::string::npos)
            return std::string{};
        return p.substr(0, slash);
    }

    // Resolve a FILE-directive path against the cue sheet's directory.
    // CUE files always reference data files relative to the sheet itself.
    std::string resolveAgainstCue(const std::string& pCuePath, const std::string& pRelOrAbs) {
        // Already absolute? Drive-letter on Windows or leading slash on POSIX.
        if (pRelOrAbs.size() >= 2 && pRelOrAbs[1] == ':')
            return pRelOrAbs;
        if (!pRelOrAbs.empty() && (pRelOrAbs[0] == '/' || pRelOrAbs[0] == '\\'))
            return pRelOrAbs;

        std::string dir = parentDir(pCuePath);
        if (dir.empty())
            return pRelOrAbs;

        std::string out = dir;
        if (out.back() != '/')
            out.push_back('/');
        out += pRelOrAbs;
        return out;
    }

    // Pop one whitespace-delimited or quoted-string token from a line.
    // Updates pPos to point after the token.
    bool nextToken(const std::string& pLine, size_t& pPos, std::string& pOut) {
        // Skip leading whitespace.
        while (pPos < pLine.size() && std::isspace((unsigned char)pLine[pPos]))
            ++pPos;
        if (pPos >= pLine.size())
            return false;

        if (pLine[pPos] == '"') {
            ++pPos;
            size_t end = pLine.find('"', pPos);
            if (end == std::string::npos) {
                pOut = pLine.substr(pPos);
                pPos = pLine.size();
            } else {
                pOut = pLine.substr(pPos, end - pPos);
                pPos = end + 1;
            }
            return true;
        }

        size_t start = pPos;
        while (pPos < pLine.size() && !std::isspace((unsigned char)pLine[pPos]))
            ++pPos;
        pOut = pLine.substr(start, pPos - start);
        return !pOut.empty();
    }

    // Parse an INDEX timestamp ("MM:SS:FF" — minutes:seconds:frames). One
    // frame = 1 sector at 75 Hz; sector index = ((mm*60)+ss)*75 + ff.
    // Returns -1 if the input doesn't parse.
    int64_t parseTimestamp(const std::string& pTs) {
        int mm = 0, ss = 0, ff = 0;
        if (std::sscanf(pTs.c_str(), "%d:%d:%d", &mm, &ss, &ff) != 3)
            return -1;
        int64_t sectors = (int64_t)mm * 60 * (int64_t)kFramesPerSecond
                         + (int64_t)ss * (int64_t)kFramesPerSecond
                         + (int64_t)ff;
        return sectors;
    }

    // Map a TRACK MODE string to the implied sector size in bytes.
    size_t sectorSizeForMode(const std::string& pMode) {
        const std::string m = toUpper(pMode);
        if (m == "MODE1/2048") return 2048;
        if (m == "MODE1/2352") return 2352;
        if (m == "MODE2/2336") return 2336;
        if (m == "MODE2/2352") return 2352;
        if (m == "AUDIO")      return 2352;
        if (m == "CDI/2336")   return 2336;
        if (m == "CDI/2352")   return 2352;
        return 2352;            // Conservative default — most CUE tracks are 2352.
    }

    uint64_t fileSizeOnDisk(const std::string& pPath) {
        struct stat info;
        if (::stat(pPath.c_str(), &info) != 0)
            return 0;
        return (uint64_t)info.st_size;
    }

} // namespace

const sCueTrack* sCueSheet::FindTrack(int pNumber) const {
    for (auto& t : mTracks)
        if (t.mNumber == pNumber)
            return &t;
    return nullptr;
}

bool LooksLikeCueSheet(const std::string& pPath) {
    if (pPath.size() < 4)
        return false;
    char c0 = (char)std::tolower((unsigned char)pPath[pPath.size() - 4]);
    char c1 = (char)std::tolower((unsigned char)pPath[pPath.size() - 3]);
    char c2 = (char)std::tolower((unsigned char)pPath[pPath.size() - 2]);
    char c3 = (char)std::tolower((unsigned char)pPath[pPath.size() - 1]);
    return c0 == '.' && c1 == 'c' && c2 == 'u' && c3 == 'e';
}

bool ParseCueSheet(const std::string& pCuePath, sCueSheet& pOut) {
    pOut = {};
    pOut.mCuePath = pCuePath;

    std::ifstream in(pCuePath.c_str(), std::ios::binary);
    if (!in)
        return false;

    // Per-track running state. CUE format is sequential — each TRACK in the
    // current FILE inherits that file context; the first INDEX 01 we see
    // for that track decides the start byte.
    int currentFileIndex = -1;
    int currentTrackIndex = -1;     // index into pOut.mTracks
    bool sawIndex01ForCurrent = false;

    std::string line;
    while (std::getline(in, line)) {
        // Drop stray CR from CRLF.
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        std::string trimmed = trim(line);
        if (trimmed.empty())
            continue;

        // Tokenise.
        size_t pos = 0;
        std::string keyword;
        if (!nextToken(trimmed, pos, keyword))
            continue;
        std::string up = toUpper(keyword);

        if (up == "REM" || up == "CATALOG" || up == "PERFORMER" || up == "TITLE" ||
            up == "SONGWRITER" || up == "ISRC" || up == "FLAGS" || up == "PREGAP" ||
            up == "POSTGAP" || up == "CDTEXTFILE") {
            continue;
        }

        if (up == "FILE") {
            std::string path, format;
            if (!nextToken(trimmed, pos, path))
                continue;
            nextToken(trimmed, pos, format);

            sCueFile f;
            f.mPath = resolveAgainstCue(pCuePath, path);
            f.mFormat = format.empty() ? "BINARY" : toUpper(format);
            f.mSizeBytes = fileSizeOnDisk(f.mPath);

            pOut.mFiles.push_back(std::move(f));
            currentFileIndex = (int)pOut.mFiles.size() - 1;
            continue;
        }

        if (up == "TRACK") {
            if (currentFileIndex < 0)
                continue;       // TRACK before any FILE — malformed.

            std::string num, mode;
            if (!nextToken(trimmed, pos, num)) continue;
            if (!nextToken(trimmed, pos, mode)) continue;

            sCueTrack t;
            t.mNumber = std::atoi(num.c_str());
            t.mMode = toUpper(mode);
            t.mFileIndex = (size_t)currentFileIndex;
            t.mSectorSize = sectorSizeForMode(t.mMode);

            pOut.mTracks.push_back(std::move(t));
            currentTrackIndex = (int)pOut.mTracks.size() - 1;
            sawIndex01ForCurrent = false;
            continue;
        }

        if (up == "INDEX") {
            if (currentTrackIndex < 0)
                continue;

            std::string num, ts;
            if (!nextToken(trimmed, pos, num)) continue;
            if (!nextToken(trimmed, pos, ts)) continue;

            int idxNum = std::atoi(num.c_str());
            int64_t sectors = parseTimestamp(ts);
            if (sectors < 0)
                continue;

            // INDEX 00 is the pregap start; INDEX 01 is the actual track
            // start. Use INDEX 01 to set mStartByte. If only INDEX 00
            // exists, we fall back to that.
            sCueTrack& cur = pOut.mTracks[(size_t)currentTrackIndex];
            const uint64_t byteOffset = (uint64_t)sectors * cur.mSectorSize;
            if (idxNum == 1) {
                cur.mStartByte = byteOffset;
                sawIndex01ForCurrent = true;
            } else if (idxNum == 0 && !sawIndex01ForCurrent) {
                cur.mStartByte = byteOffset;
            }
            continue;
        }

        // Unknown line — ignore.
    }

    // Compute end offsets. A track ends where the next track-in-the-same-file
    // begins; the last track in a file ends at EOF. Tracks in different
    // FILEs don't chain.
    for (size_t i = 0; i < pOut.mTracks.size(); ++i) {
        sCueTrack& t = pOut.mTracks[i];
        const sCueFile& file = pOut.mFiles[t.mFileIndex];

        uint64_t end = file.mSizeBytes;
        for (size_t j = i + 1; j < pOut.mTracks.size(); ++j) {
            const sCueTrack& nxt = pOut.mTracks[j];
            if (nxt.mFileIndex != t.mFileIndex)
                continue;
            if (nxt.mStartByte >= t.mStartByte) {
                end = nxt.mStartByte;
                break;
            }
        }
        t.mEndByteExclusive = end;
    }

    if (pOut.mFiles.empty() || pOut.mTracks.empty())
        return false;

    // Sanity: at least one FILE must point at a real, readable BIN.
    for (auto& f : pOut.mFiles) {
        if (f.mSizeBytes > 0)
            return true;
    }
    return false;
}

int ExtractTrackTo(const sCueSheet& pSheet, int pTrackNumber, const std::string& pDestPath) {
    const sCueTrack* track = pSheet.FindTrack(pTrackNumber);
    if (!track)
        return -1;
    if (track->mFileIndex >= pSheet.mFiles.size())
        return -1;

    const sCueFile& file = pSheet.mFiles[track->mFileIndex];
    if (file.mSizeBytes == 0)
        return -1;

    std::ifstream in(file.mPath.c_str(), std::ios::binary);
    if (!in)
        return -1;
    in.seekg((std::streamoff)track->mStartByte, std::ios::beg);

    std::ofstream out(pDestPath.c_str(), std::ios::binary | std::ios::trunc);
    if (!out)
        return -1;

    const size_t sectorSize = track->mSectorSize;
    const uint64_t totalBytes = track->mEndByteExclusive - track->mStartByte;

    // For MODE1/2352 and MODE2/2352 we strip the 16-byte sync+header at the
    // start of each sector and the 288-byte EDC+ECC at the end, leaving the
    // 2048-byte cooked user data payload that ISO9660 expects. For 2048-
    // byte tracks (already cooked) and AUDIO tracks (raw stream), we copy
    // through verbatim.
    auto isStripMode = [&](const std::string& m) {
        return m == "MODE1/2352" || m == "MODE2/2352";
    };

    int bytesWritten = 0;

    if (sectorSize == 2048 || track->mMode == "AUDIO" || !isStripMode(track->mMode)) {
        // Verbatim copy. Use a chunky buffer; CD32 binary tracks can be
        // hundreds of MB.
        std::vector<char> buf(64 * 1024);
        uint64_t remaining = totalBytes;
        while (remaining > 0 && in) {
            const size_t n = (size_t)std::min<uint64_t>(remaining, buf.size());
            in.read(buf.data(), (std::streamsize)n);
            const std::streamsize got = in.gcount();
            if (got <= 0)
                break;
            out.write(buf.data(), got);
            if (!out)
                return -1;
            remaining -= (uint64_t)got;
            bytesWritten += (int)got;
        }
        return bytesWritten;
    }

    // Strip-and-cook path. ISO9660 sectors are 2048 bytes of payload; raw
    // sectors are 2352 bytes (16 sync+header + 2048 user data + 288 ECC/EDC
    // for MODE1; 16 sync+header + 8 subheader + 2048 user data + 280 ECC/EDC
    // for MODE2 form 1).
    const size_t userDataOffset = (track->mMode == "MODE2/2352") ? 24 : 16;
    const size_t userDataSize   = 2048;

    std::vector<char> sector(sectorSize);
    uint64_t remaining = totalBytes;

    while (remaining >= sectorSize && in) {
        in.read(sector.data(), (std::streamsize)sectorSize);
        if (in.gcount() != (std::streamsize)sectorSize)
            break;
        out.write(sector.data() + userDataOffset, (std::streamsize)userDataSize);
        if (!out)
            return -1;
        remaining -= sectorSize;
        bytesWritten += (int)userDataSize;
    }

    return bytesWritten;
}

} // namespace Setup
