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

#include "Setup/Validation.hpp"

#include <string>
#include <vector>

namespace Setup {

// One candidate-folder hit produced by the auto-scan.
struct sScanCandidate {
    std::string         mPath;          // Folder we'd add to [paths], or full path to image file
    sValidationResult   mResult;        // ValidateFolder(mPath) — pre-computed (folders only)
    std::string         mLabel;         // Short human label, e.g. "GOG: Cannon Fodder"
    bool                mIsImage = false; // true → candidate is a disk image; click → mountImage
    std::string         mFormat;
    size_t              mDisks = 0;
};

// Expand a folder result into one independently selectable entry per release.
// Paths point to the matched release directories, keeping sibling games out
// of the subsequent import when the user selects a single entry.
std::vector<sScanCandidate> FolderCandidates(const sValidationResult& pResult,
                                             const std::string& pLabel);

// Probe a fixed list of likely retail-data locations and return the candidates
// that match at least one known release with at least one file present.
// Unverified disk images are only listed when explicitly browsing a folder.
//
// Cheap-ish (a handful of stat() calls per candidate, MD5 only on actual hits).
// Safe to call with no engine running — uses g_ResourceMan for filesystem ops.
struct sAutoScanInput {
    std::vector<std::string> mPaths;
    std::string mWorkingDirectory;
    std::vector<const sGameVersion*> mAvailable;
};
// Capture paths and availability on the UI thread before starting a worker.
std::vector<sScanCandidate> AutoScan(const sAutoScanInput& pInput, const CancelCheck& pCancelled = {});
std::vector<sScanCandidate> AutoScan();
std::vector<sScanCandidate> AutoScan(const std::vector<const sGameVersion*>& pAvailable, const CancelCheck& pCancelled = {});

} // namespace Setup
