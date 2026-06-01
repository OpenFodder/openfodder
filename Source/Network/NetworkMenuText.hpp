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

// Shared menu-text helpers used by the random-map options screen and the
// multiplayer menu. Previously duplicated as file-static
// RandomMapOptions_/MultiplayerMenu_ pairs in those two TUs.

// Parse a decimal string into a uint32. Returns false on empty input, any
// non-digit, or overflow past 0xFFFFFFFF.
bool NetworkMenu_UInt32FromText(const std::string& pText, uint32& pValue);

// Truncate pText (upper-cased, A-Z/0-9/space only) to fit within pMaxPx pixels
// using the briefing font widths, trimming trailing spaces.
std::string NetworkMenu_FitText(const std::string& pText, int pMaxPx);
