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
    //
    // If no data available, shows an SDL_ShowMessageBox asking the user
    // whether to download the latest demo data + scripts releases from
    // GitHub. The two repos (OpenFodder/data, OpenFodder/scripts) are
    // installed in tandem because installing data without scripts leaves
    // the random-map and multiplayer menus broken — the engine references
    // .js files from Run/Scripts at runtime.
    //
    // On Download success: refreshes ResourceMan and returns Download
    //   (caller proceeds into the regular startup flow).
    // On user-Cancel:       returns Quit (caller should SDL_Quit + exit).
    // On any failure:       shows an error message box and returns Quit.
    //
    // Synchronous — runs before the window exists, so there's no UI to
    // freeze; the SDL message box plus blocking install are exactly what
    // the user expects in the cold-start case.
    static ColdStartChoice PromptIfNoData();

private:
    // Installs both repos into the supplied target dirs. On any failure,
    // pError is populated with a human-readable message and false is
    // returned. The half-installed state (data succeeded, scripts failed)
    // is acceptable — the caller will surface the error and quit; the
    // user's next launch will see partial data and can retry.
    static bool RunDownload(const std::string& pDataTargetDir,
                            const std::string& pScriptsTargetDir,
                            std::string& pError);
};

} // namespace Setup
