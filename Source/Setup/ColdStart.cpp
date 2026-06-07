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
#include "Setup/InstallPaths.hpp"
#include "ResourceMan.hpp"

#include <SDL3/SDL.h>

#include <filesystem>

namespace Setup {

ColdStartChoice ColdStartPrompt::PromptIfNoData()
{
    if (g_ResourceMan && g_ResourceMan->isDataAvailable())
        return ColdStartChoice::Continue;

    // Flatpak sandbox: refuse the auto-download path. The Flatpak ships
    // its own data and updates flow through `flatpak update`. A direct
    // GitHub fetch would (a) need --share=network granted, (b) put the
    // installed tree out of sync with the manifest's recorded data
    // extension version. Tell the user how to fix it manually.
    if (InstallPaths::IsFlatpak()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenFodder",
            "OpenFodder cannot find any game data.\n\n"
            "This Flatpak install is missing its data files.\n"
            "Try:  flatpak repair --user org.openfodder.OpenFodder\n\n"
            "Or reinstall from Flathub:\n"
            "  flatpak install flathub org.openfodder.OpenFodder",
            nullptr);
        return ColdStartChoice::Quit;
    }

    SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Download" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Quit" }
    };
    SDL_MessageBoxData mb = { SDL_MESSAGEBOX_INFORMATION, nullptr,
        "OpenFodder",
        "OpenFodder cannot find any game data.\n\n"
        "Download the latest demo data and scripts from GitHub now?\n\n"
        "(About 10-15 MB total; requires an internet connection.)",
        SDL_arraysize(buttons), buttons, nullptr };
    int picked = -1;
    if (!SDL_ShowMessageBox(&mb, &picked) || picked == 0)
        return ColdStartChoice::Quit;

    // Per-platform install root. On a portable layout (dev checkout,
    // Windows .zip, sibling-of-exe install) this is <cwd>/Data and
    // <cwd>/Scripts. On a Linux distro install where the exe lives at
    // /usr/bin/openfodder and cwd is read-only, this is
    // $XDG_DATA_HOME/OpenFodder/{Data,Scripts} — which is what
    // cResourceMan::addDefaultDirs already scans on the read side, so
    // the next launch picks up the install automatically.
    const InstallTargets targets = InstallPaths::Resolve();

    std::string err;
    if (!RunDownload(targets.mDataDir, targets.mScriptsDir, err)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenFodder",
            ("Download failed:\n\n" + err).c_str(), nullptr);
        return ColdStartChoice::Quit;
    }
    if (g_ResourceMan)
        g_ResourceMan->refresh();
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "OpenFodder",
        "Data and scripts installed. Continuing to OpenFodder...", nullptr);
    return ColdStartChoice::Download;
}

bool ColdStartPrompt::RunDownload(const std::string& pDataTargetDir,
                                  const std::string& pScriptsTargetDir,
                                  std::string& pError)
{
    DataRelease release;

    // Data repo — the on-disk file tree. Without this the engine has
    // nothing to render.
    ReleaseManifest dataManifest;
    if (!release.QueryLatest(DataRelease::DataRepoOwner(), DataRelease::DataRepoName(), dataManifest)) {
        pError = "data: " + release.LastError();
        return false;
    }
    if (!release.FetchAndInstall(dataManifest, pDataTargetDir)) {
        pError = "data: " + release.LastError();
        return false;
    }

    // Scripts repo — JS for random maps, multiplayer, level editor. The
    // engine's title-sequence works without scripts, but every modern
    // menu path (random.js, multiplayer.js, mapeditor.js) hard-references
    // them; a data-only install gets you to the menu and immediately
    // crashes when you click anything.
    ReleaseManifest scriptManifest;
    if (!release.QueryLatest(DataRelease::ScriptsRepoOwner(), DataRelease::ScriptsRepoName(), scriptManifest)) {
        pError = "scripts: " + release.LastError();
        return false;
    }
    if (!release.FetchAndInstall(scriptManifest, pScriptsTargetDir)) {
        pError = "scripts: " + release.LastError();
        return false;
    }

    return true;
}

} // namespace Setup
