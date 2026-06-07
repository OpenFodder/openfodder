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
};

// Probe a fixed list of likely retail-data locations and return the candidates
// that match at least one known release with at least one file present.
//
// Cheap-ish (a handful of stat() calls per candidate, MD5 only on actual hits).
// Safe to call with no engine running — uses g_ResourceMan for filesystem ops.
std::vector<sScanCandidate> AutoScan();

} // namespace Setup
