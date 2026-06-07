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

#include <string>

// ---------------------------------------------------------------------------
// Engine + data/scripts compatibility constants.
//
// kEngineVersion* mirrors project(openfodder VERSION ...) in CMakeLists.txt.
// Bump kDataVersionMax / kScriptVersionMax whenever a new compatible release
// of the OpenFodder/data or OpenFodder/scripts repo ships. The "Update"
// button refuses to install a release whose dataVersion / scriptVersion
// falls outside [Min, Max] — this is the hard compatibility gate, not just
// a soft warning.
//
// Both repos publish a "compat.json" alongside their main archive containing:
//   { "dataVersion": <int>, "scriptVersion": <int>, "notes": "..." }
// ---------------------------------------------------------------------------

namespace Setup {

constexpr int          kEngineVersionMajor  = 1;
constexpr int          kEngineVersionMinor  = 5;
constexpr int          kEngineVersionPatch  = 4;
constexpr const char*  kEngineVersionString = "1.5.4";

// Compatible OpenFodder/data release range
constexpr int kDataVersionMin = 1;
constexpr int kDataVersionMax = 1;

// Compatible OpenFodder/scripts release range
constexpr int kScriptVersionMin = 1;
constexpr int kScriptVersionMax = 1;

inline bool IsDataVersionCompatible(int pVersion) {
    return pVersion >= kDataVersionMin && pVersion <= kDataVersionMax;
}

inline bool IsScriptVersionCompatible(int pVersion) {
    return pVersion >= kScriptVersionMin && pVersion <= kScriptVersionMax;
}

} // namespace Setup
