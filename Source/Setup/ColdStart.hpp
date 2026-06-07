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

namespace Setup {

enum class ColdStartChoice : uint8_t { Continue, Download, Quit };

class ColdStartPrompt {
public:
    // Called early in startup AFTER SDL_Init(SDL_INIT_VIDEO) but BEFORE
    // window creation. If g_ResourceMan->isDataAvailable() is already true,
    // returns Continue without showing any UI.
    // If no data available: shows an SDL_ShowMessageBox asking
    //   "OpenFodder cannot find any game data. Download it now?"
    //   buttons: [Download] [Quit]
    // On Download: runs DataRelease::QueryLatest + FetchAndInstall for the
    // OpenFodder/data repo into <exe-dir>/Data/, calls
    // g_ResourceMan->refresh(), then returns Download (caller proceeds).
    // On Quit: returns Quit (caller should SDL_Quit + exit).
    // On any error (network, install failure): shows an error message box
    // and returns Quit.
    static ColdStartChoice PromptIfNoData();

private:
    static bool RunDownload(const std::string& pTargetDir, std::string& pError);
};

} // namespace Setup
