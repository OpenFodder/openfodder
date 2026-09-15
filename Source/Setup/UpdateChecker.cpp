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
#include "Setup/UpdateChecker.hpp"
#include "Setup/EngineVersion.hpp"
#include "Setup/InstallPaths.hpp"
#include "ResourceMan.hpp"

#include <SDL3/SDL.h>

#include <utility>
#include <filesystem>
#include <stdexcept>

namespace Setup {

UpdateChecker& UpdateChecker::Instance() {
    static UpdateChecker s;
    return s;
}

UpdateChecker::~UpdateChecker() {
    JoinWorker();
}

void UpdateChecker::JoinWorker() {
    if (mWorker.joinable())
        mWorker.join();
}

bool UpdateChecker::IsBusy() const {
    const State s = mState.load(std::memory_order_acquire);
    return s != State::Idle;
}

std::string UpdateChecker::StatusLine() const {
    switch (mState.load(std::memory_order_acquire)) {
        case State::Idle:                  return {};
        case State::QueryRunning:          return "CHECKING FOR UPDATES...";
        case State::InstallRunning: {
            const int pct = mProgressPercent.load(std::memory_order_relaxed);
            if (pct <= 0)
                return "INSTALLING...";
            return "INSTALLING " + std::to_string(pct) + "%";
        }
        // Terminal states clear themselves the moment Pump() runs, so under
        // normal use these are unreachable here. Keep them for completeness.
        case State::QueryDoneNoUpdate:     return "UP TO DATE";
        case State::QueryDoneNewer:        return "UPDATES AVAILABLE";
        case State::InstallDoneOk:         return "INSTALL COMPLETE";
        case State::InstallDoneFailed:     return "UPDATE FAILED";
    }
    return {};
}

void UpdateChecker::StartQuery() try {
    // Flatpak: refuse the in-app update path. Updates flow through the
    // Flatpak runtime (`flatpak update org.openfodder.OpenFodder`); a
    // direct GitHub fetch would put the data tree out of sync with the
    // manifest's recorded extension version, and would also fail unless
    // --share=network was granted to the sandbox. Show guidance instead
    // and stay in Idle.
    if (InstallPaths::IsFlatpak()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "OpenFodder Update",
                                 "Updates for the Flatpak build are handled by Flatpak.\n\n"
                                 "Run:  flatpak update org.openfodder.OpenFodder",
                                 nullptr);
        return;
    }

    // Re-entrancy guard. CAS Idle → QueryRunning; loses race → no-op. The
    // CAS publishes a release on success so the worker thread sees the
    // up-to-date result fields written below.
    State expected = State::Idle;
    if (!mState.compare_exchange_strong(expected, State::QueryRunning,
                                        std::memory_order_acq_rel,
                                        std::memory_order_relaxed)) {
        return;
    }

    // Snapshot the install dirs on the main thread — std::filesystem
    // calls aren't guaranteed safe to do from a worker depending on the
    // platform's TLS state at thread start, and InstallPaths::Resolve
    // reads $XDG_DATA_HOME / %USERPROFILE% which we'd rather not race
    // against (the engine itself never mutates them, but a screen-locker
    // or session-manager could).
    const InstallTargets targets = InstallPaths::Resolve();
    mDataTargetDir    = targets.mDataDir;
    mScriptsTargetDir = targets.mScriptsDir;
    {
        std::lock_guard<std::mutex> lk(mStringMutex);
        mErrorMessage.clear();
    }
    mProgressPercent.store(0, std::memory_order_relaxed);
    mDataInstalled = false;
    mScriptsInstalled = false;

    JoinWorker(); // safe — last worker has fully exited if state was Idle.
    mWorker = std::thread([this] { RunQueryWorker(); });
} catch (const std::exception& ex) {
    Fail(std::string("Could not start update check: ") + ex.what());
}

void UpdateChecker::Fail(const std::string& pMessage) {
    {
        std::lock_guard<std::mutex> lk(mStringMutex);
        mErrorMessage = pMessage;
    }
    mState.store(State::InstallDoneFailed, std::memory_order_release);
}

void UpdateChecker::RunQueryWorker() try {
    if (!mRelease.QueryLatest(DataRelease::DataRepoOwner(),
                              DataRelease::DataRepoName(),
                              mDataManifest)) {
        Fail("Could not check data repo: " + mRelease.LastError());
        return;
    }
    if (!mRelease.QueryLatest(DataRelease::ScriptsRepoOwner(),
                              DataRelease::ScriptsRepoName(),
                              mScriptManifest)) {
        Fail("Could not check scripts repo: " + mRelease.LastError());
        return;
    }

    // Read installed records from each subtree independently; ignore the
    // out-param that doesn't apply to that subtree.
    int dataIgnored = 0;
    int scriptsIgnored = 0;
    mInstalledDataVer = 0;
    mInstalledScriptVer = 0;
    const bool haveDataRecord = mRelease.ReadInstalledManifest(mDataTargetDir, mInstalledDataVer, dataIgnored);
    if (!haveDataRecord && std::filesystem::exists(std::filesystem::path(mDataTargetDir) / "installed.json"))
        throw std::runtime_error(mRelease.LastError());
    const bool haveScriptRecord = mRelease.ReadInstalledManifest(mScriptsTargetDir, scriptsIgnored, mInstalledScriptVer);
    if (!haveScriptRecord && std::filesystem::exists(std::filesystem::path(mScriptsTargetDir) / "installed.json"))
        throw std::runtime_error(mRelease.LastError());

    mDataNewer   = (mDataManifest.mDataVersion > mInstalledDataVer)
                && IsDataVersionCompatible(mDataManifest.mDataVersion);
    mScriptNewer = (mScriptManifest.mScriptVersion > mInstalledScriptVer)
                && IsScriptVersionCompatible(mScriptManifest.mScriptVersion);

    // Publish: result fields are visible to the main thread because of
    // the release-store on mState below.
    if (mDataNewer || mScriptNewer)
        mState.store(State::QueryDoneNewer, std::memory_order_release);
    else
        mState.store(State::QueryDoneNoUpdate, std::memory_order_release);
} catch (const std::exception& ex) {
    Fail(std::string("Update check failed: ") + ex.what());
} catch (...) {
    Fail("Update check failed unexpectedly.");
}

void UpdateChecker::RunInstallWorker() try {
    // Wire HTTP progress through to the status-line atomic. We never read
    // pTotal — releases sized in the tens of megabytes overflow our int
    // doubling check less than progress accuracy is worth.
    auto httpProgress = [this](uint64_t pDone, uint64_t pTotal) -> bool {
        if (pTotal > 0) {
            const int pct = int((pDone * 100ULL) / pTotal);
            mProgressPercent.store(pct, std::memory_order_relaxed);
        }
        return true; // never abort
    };

    if (mDataNewer) {
        mProgressPercent.store(0, std::memory_order_relaxed);
        if (!mRelease.FetchAndInstall(mDataManifest, mDataTargetDir, httpProgress)) {
            Fail("Data install failed: " + mRelease.LastError());
            return;
        }
        mDataInstalled = true;
    }
    if (mScriptNewer) {
        mProgressPercent.store(0, std::memory_order_relaxed);
        if (!mRelease.FetchAndInstall(mScriptManifest, mScriptsTargetDir, httpProgress)) {
            Fail("Scripts install failed: " + mRelease.LastError());
            return;
        }
    }

    mScriptsInstalled = mScriptNewer;
    mState.store(State::InstallDoneOk, std::memory_order_release);
} catch (const std::exception& ex) {
    Fail(std::string("Update install failed: ") + ex.what());
} catch (...) {
    Fail("Update install failed unexpectedly.");
}

bool UpdateChecker::ApplyInstalledContent() {
    if (!mDataInstalled && !mScriptsInstalled)
        return true;
    if (mScriptsInstalled && g_ScriptingEngine)
        g_ScriptingEngine->Disable();
    if (g_ResourceMan)
        g_ResourceMan->refresh();
    if (mScriptsInstalled) {
        const auto activeRoot = g_ResourceMan ? g_ResourceMan->GetScriptPath("") : std::string{};
        if (activeRoot.empty() || !std::filesystem::equivalent(activeRoot, mScriptsTargetDir))
            throw std::runtime_error("Scripts were installed to " + mScriptsTargetDir +
                ", but a different scripts folder is active. Check your configured resource paths before generating maps.");
        // The worker has joined and About still owns the main thread. Drop
        // all old bindings together; new entry points must never use old JS.
        // On a load error the replacement stays disabled, so generation cannot
        // fall back to a mismatched old runtime.
        g_ScriptingEngine = std::make_shared<cScriptingEngine>();
        mScriptsInstalled = false;
        if (!g_ScriptingEngine->IsLoaded())
            throw std::runtime_error("Scripts were installed but could not be loaded. Reinstall compatible scripts before generating maps.");
    }
    mDataInstalled = false;
    return true;
}

// ---------------------------------------------------------------------------
// Pump — main-thread state-machine drain. Runs once per frame from
// cAbout::Cycle. Any modal SDL_ShowMessageBox calls happen here, never on
// the worker thread (SDL message boxes must run on the main thread on
// Windows or they spawn a window from the wrong queue).
// ---------------------------------------------------------------------------
void UpdateChecker::Pump() try {
    const State s = mState.load(std::memory_order_acquire);

    switch (s) {
        case State::Idle:
        case State::QueryRunning:
        case State::InstallRunning:
            return;

        case State::QueryDoneNoUpdate: {
            JoinWorker();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                     "OpenFodder Update",
                                     "You have the latest compatible version.",
                                     nullptr);
            mState.store(State::Idle, std::memory_order_release);
            return;
        }

        case State::QueryDoneNewer: {
            JoinWorker();

            std::string message = "Updates available:\n";
            if (mDataNewer)
                message += "  data: v" + std::to_string(mInstalledDataVer)
                         + " \xE2\x86\x92 v" + std::to_string(mDataManifest.mDataVersion) + "\n";
            if (mScriptNewer)
                message += "  scripts: v" + std::to_string(mInstalledScriptVer)
                         + " \xE2\x86\x92 v" + std::to_string(mScriptManifest.mScriptVersion) + "\n";
            message += "\nInstall now?";

            const SDL_MessageBoxButtonData buttons[2] = {
                { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Install" },
                { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Cancel"  },
            };
            SDL_MessageBoxData prompt{};
            prompt.flags       = SDL_MESSAGEBOX_INFORMATION;
            prompt.window      = nullptr;
            prompt.title       = "OpenFodder Update";
            prompt.message     = message.c_str();
            prompt.numbuttons  = 2;
            prompt.buttons     = buttons;
            prompt.colorScheme = nullptr;

            int chosen = -1;
            if (!SDL_ShowMessageBox(&prompt, &chosen) || chosen != 1) {
                mState.store(State::Idle, std::memory_order_release);
                return;
            }

            // User accepted — kick the install worker. The transition
            // QueryDoneNewer → InstallRunning is single-threaded (Pump is
            // main-thread only and the previous worker has already
            // joined), so a plain store is fine.
            mState.store(State::InstallRunning, std::memory_order_release);
            mProgressPercent.store(0, std::memory_order_relaxed);
            mWorker = std::thread([this] { RunInstallWorker(); });
            return;
        }

        case State::InstallDoneOk: {
            JoinWorker();
            ApplyInstalledContent();

            std::string done = "Updated to";
            if (mDataNewer)
                done += " data v" + std::to_string(mDataManifest.mDataVersion);
            if (mDataNewer && mScriptNewer)
                done += " /";
            if (mScriptNewer)
                done += " scripts v" + std::to_string(mScriptManifest.mScriptVersion);
            done += ".";

            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                     "OpenFodder Update",
                                     done.c_str(),
                                     nullptr);
            mState.store(State::Idle, std::memory_order_release);
            return;
        }

        case State::InstallDoneFailed: {
            JoinWorker();
            // A data update may have succeeded before the scripts download
            // failed. Refresh that completed update even on the failure path.
            ApplyInstalledContent();
            std::string err;
            {
                std::lock_guard<std::mutex> lk(mStringMutex);
                err = std::move(mErrorMessage);
            }
            if (err.empty())
                err = "Unknown failure.";
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                     "OpenFodder Update",
                                     err.c_str(),
                                     nullptr);
            mState.store(State::Idle, std::memory_order_release);
            return;
        }
    }
} catch (const std::exception& ex) {
    mDataInstalled = false;
    mScriptsInstalled = false;
    Fail(std::string("Could not apply update: ") + ex.what());
} catch (...) {
    mDataInstalled = false;
    mScriptsInstalled = false;
    Fail("Could not apply update.");
}

} // namespace Setup
