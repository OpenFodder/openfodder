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
#include "Setup/DownloadProgress.hpp"
#include "ResourceMan.hpp"

#include <SDL3/SDL.h>

#include <filesystem>

namespace Setup {

ColdStartChoice ColdStartPrompt::PromptIfNoData()
{
    const bool headless = g_Fodder && g_Fodder->mStartParams &&
        g_Fodder->mStartParams->mDisableVideo;
    const bool haveData = g_ResourceMan && g_ResourceMan->isDataAvailable();
    const bool haveScripts = g_ResourceMan &&
        !g_ResourceMan->GetScriptPath("Settings.js").empty() &&
        !g_ResourceMan->GetScriptPath("Common/OpenFodder.js").empty() &&
        !g_ResourceMan->GetScriptPath("random.js").empty();
    const bool haveCampaigns = g_ResourceMan && !g_ResourceMan->GetCampaigns().empty();
    const bool haveAbout = g_ResourceMan && !g_ResourceMan->GetAboutFile().empty();
    // UI artwork is irrelevant when no video will be displayed.
    const bool haveResources = haveCampaigns && (headless || haveAbout);
    if (haveData && haveScripts && haveResources)
        return ColdStartChoice::Continue;

    if (headless) {
        std::string missing;
        if (!haveData) missing += " game data;";
        if (!haveScripts) missing += " required scripts;";
        if (!haveCampaigns) missing += " campaigns;";
        g_Debugger->Error("Cannot start headlessly. Missing runtime resources:" + missing);
        return ColdStartChoice::Quit;
    }

    // Flatpak sandbox: refuse the auto-download path. The Flatpak ships
    // its own data and updates flow through `flatpak update`. A direct
    // GitHub fetch would (a) need --share=network granted, (b) put the
    // installed tree out of sync with the manifest's recorded data
    // extension version. Tell the user how to fix it manually.
    if (InstallPaths::IsFlatpak()) {
        ShowDownloadNotice("OpenFodder",
            "OpenFodder cannot find its game data or scripts.\n\n"
            "This Flatpak install is missing its data files.\n"
            "Try:  flatpak repair --user org.openfodder.OpenFodder\n\n"
            "Or reinstall from Flathub:\n"
            "  flatpak install flathub org.openfodder.OpenFodder");
        return ColdStartChoice::Quit;
    }

    // Per-platform install root. On a portable layout (dev checkout,
    // Windows .zip, sibling-of-exe install) this is <cwd>/Data and
    // <cwd>/Scripts. On a Linux distro install where the exe lives at
    // /usr/bin/openfodder and cwd is read-only, this is
    // $XDG_DATA_HOME/OpenFodder/{Data,Scripts} — which is what
    // cResourceMan::addDefaultDirs already scans on the read side, so
    // the next launch picks up the install automatically.
    const InstallTargets targets = InstallPaths::Resolve();

    std::string err;
    if (!RunDownload(std::filesystem::path(targets.mDataDir).parent_path().string(), err)) {
        if (!err.empty()) g_Debugger->Error("Download failed: " + err);
        return ColdStartChoice::Quit;
    }
    return ColdStartChoice::Download;
}

bool ColdStartPrompt::RunDownload(const std::string& pRuntimeRoot, std::string& pError)
{
    const DownloadPrompt prompt{"GAME FILES NEEDED",
        "OpenFodder cannot find its game data or scripts.\n\n"
        "Download the complete demos, campaigns and scripts from GitHub?\n\n"
        "An internet connection is required."};
    return RunDownloadWithProgress("OpenFodder - Download game data",
        [&](DownloadProgress& progress, std::string& error) {
            DataRelease release;
            ReleaseManifest manifest;
            const auto queryProgress = [&](uint64_t, uint64_t) { return !progress.Read().canceled; };
            if (!release.QueryLatest(DataRelease::DataRepoOwner(), DataRelease::DataRepoName(), manifest, queryProgress) ||
                !release.FetchAndInstall(manifest, pRuntimeRoot,
                    [&](uint64_t done, uint64_t total) { return progress.Download(done, total); },
                    [&](size_t entry, size_t count, const std::string&) { return progress.Extract(entry, count); },
                    [&](InstallStage stage) { return progress.SetStage(stage); })) {
                error = release.LastError();
                return false;
            }
            return true;
        }, pError, nullptr, [] {
            if (g_ResourceMan) g_ResourceMan->refresh();
        }, &prompt);
}

} // namespace Setup
