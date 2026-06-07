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
#include "Setup/ColdStart.hpp"
#include "Setup/DataRelease.hpp"
#include "Setup/EngineVersion.hpp"
#include "ResourceMan.hpp"

#include <SDL3/SDL.h>

#include <filesystem>

namespace Setup {

ColdStartChoice ColdStartPrompt::PromptIfNoData()
{
    if (g_ResourceMan && g_ResourceMan->isDataAvailable())
        return ColdStartChoice::Continue;

    SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Download" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Quit" }
    };
    SDL_MessageBoxData mb = { SDL_MESSAGEBOX_INFORMATION, nullptr,
        "OpenFodder",
        "OpenFodder cannot find any game data.\n\n"
        "Download the latest demo data from GitHub now?\n\n"
        "(About 5-10 MB; requires an internet connection.)",
        SDL_arraysize(buttons), buttons, nullptr };
    int picked = -1;
    if (!SDL_ShowMessageBox(&mb, &picked) || picked == 0)
        return ColdStartChoice::Quit;

    // Determine target directory: <exe-dir>/Data/ — use the existing
    // ResourceMan default. If unsure, the wizard's known target is the
    // working directory's "Data" subfolder. Use std::filesystem::current_path().
    std::filesystem::path targetDir = std::filesystem::current_path() / "Data";
    std::string err;
    if (!RunDownload(targetDir.string(), err)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenFodder",
            ("Download failed:\n\n" + err).c_str(), nullptr);
        return ColdStartChoice::Quit;
    }
    if (g_ResourceMan)
        g_ResourceMan->refresh();
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "OpenFodder",
        "Data installed. Continuing to OpenFodder...", nullptr);
    return ColdStartChoice::Download;
}

bool ColdStartPrompt::RunDownload(const std::string& pTargetDir, std::string& pError)
{
    DataRelease release;
    ReleaseManifest manifest;
    if (!release.QueryLatest(DataRelease::DataRepoOwner(), DataRelease::DataRepoName(), manifest)) {
        pError = release.LastError();
        return false;
    }
    if (!release.FetchAndInstall(manifest, pTargetDir)) {
        pError = release.LastError();
        return false;
    }
    return true;
}

} // namespace Setup
