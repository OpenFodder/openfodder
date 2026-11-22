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

#include <functional>
#include <string>
#include <vector>

namespace Setup {

// One-shot probe of a candidate retail-data folder against the engine's
// KnownGameVersions table. Mirrors cResourceMan::findVersions but operates on
// a single arbitrary folder without mutating engine state — used by the setup
// wizard to validate a user-picked folder before committing it to [paths].
//
// Supports files directly in pFolder, pFolder/Data, or known release
// subdirectories in pFolder and pFolder/Data. Never searches parent folders.
// Directly selected release folders are imported rather than registering an
// ancestor and exposing its sibling games.

struct sVersionMatch {
    const sGameVersion* mVersion = nullptr;     // Pointer into KnownGameVersions[]
    std::string         mResolvedPath;          // The folder actually scanned (with trailing /)
    size_t              mFilesFound = 0;        // How many of mFiles[] were present (by name)
    size_t              mFilesExpected = 0;     // = mFiles.size()
    size_t              mFilesMd5Match = 0;     // Of those found, how many had a matching MD5
    size_t              mFilesMd5Checked = 0;   // Of those found, how many we MD5-checked at all
                                                // (i.e. their manifest entry carried a non-empty
                                                // mChecksum). Used as the denominator when
                                                // judging authenticity — filenames alone overlap
                                                // heavily between CF1/CF2/demos and even
                                                // floppy-vs-CD32, so we use MD5 ratio to reject
                                                // filename-coincidence matches.
    bool                mIsRawFolder = false;   // True if mResolvedPath holds the version files
                                                // directly (i.e. the picked folder IS the release
                                                // folder, no Data/<release>/ wrapping). Used by
                                                // CommitPick to copy from this folder to cwd/Data/.
    bool                IsComplete() const { return mFilesExpected > 0 && mFilesFound == mFilesExpected; }
    bool                IsRetail() const;       // True iff mVersion->mRelease == eRelease::Retail

    // True iff every checked file MD5-matched. Different from IsComplete()
    // — that's filename-only. IsAuthentic implies the bytes on disk are
    // the canonical release, not a re-named lookalike.
    bool                IsAuthentic() const {
        return mFilesMd5Checked > 0 && mFilesMd5Match == mFilesMd5Checked;
    }
};

struct sValidationResult {
    std::string                 mError;             // Image decoder/setup failure, if present.
    std::string                 mInputFolder;       // What the caller passed in
    std::string                 mResolvedRoot;      // The candidate <root> that contains Data/
    bool                        mAlreadyAvailable = false; // All recognised releases were already playable.
    bool                        mLooksLikeRoot = false;     // <root>/Data exists
    std::vector<sVersionMatch>  mMatches;            // One entry per known version that had ≥1 file present
};

// Validate a folder and return everything we found. Cheap: only scans the
// folder the caller named, computes MD5 of each candidate file, and never
// touches g_ResourceMan's internal state.
using CancelCheck = std::function<bool()>;
sValidationResult ValidateFolder(const std::string& pFolder, const CancelCheck& pCancelled = {});

// Explicit folder picks also inspect immediate subfolders for collections of
// extracted games. Does not recurse through arbitrary directory trees.
sValidationResult ValidateFolderCollection(const std::string& pFolder, const CancelCheck& pCancelled = {});

// Resolve partial overlaps and competing editions using checksum evidence.
void RemoveOverlappingMatches(sValidationResult& pResult);

// Remove releases that were playable before opening the wizard.
void ExcludeAvailable(sValidationResult& pResult, const std::vector<const sGameVersion*>& pAvailable);

class cMountedImage;
sValidationResult ValidateImage(const cMountedImage& pImage, const CancelCheck& pCancelled = {});

// Helpers for the wizard's UI — turn a result into something the player can read.
std::string DescribeResult(const sValidationResult& pResult);
std::string DescribeMatch(const sVersionMatch& pMatch);

// Returns a human-readable list of mFiles[] entries that were NOT found in the
// resolved release directory. Empty if pMatch.IsComplete().
std::vector<std::string> MissingFiles(const sVersionMatch& pMatch);

// Diagnostic: open a disk image via firy, walk its directory tree, and print
// a one-screen summary to stdout. Returns 0 on success / non-zero if the
// image couldn't be opened. Used by --probe-image at the CLI for verifying
// the firy integration without launching the full setup wizard.
int ProbeImage(const std::string& pPath);

} // namespace Setup
