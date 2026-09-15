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

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>

#include "Setup/DataRelease.hpp"

namespace Setup {

// Async wrapper around DataRelease for the About-screen UPDATE button.
//
// DataRelease itself is synchronous: QueryLatest blocks on a GitHub API
// round-trip; FetchAndInstall blocks on the download + the extract. Running
// either of those from the SDL render loop freezes the window for several
// seconds. UpdateChecker runs them on a worker thread and exposes a state
// machine that the main loop polls each frame.
//
// State transitions only happen on the main thread (via Pump()), so the
// callsite never has to think about thread safety beyond "ask for status,
// pump once a frame, kick a check off when the button is clicked." The
// worker thread publishes its result via an atomic state store; the main
// thread reads it with acquire ordering.
//
// Singleton because (a) only one About screen is open at a time, and (b)
// we never want two concurrent updates fighting over installed.json.
class UpdateChecker {
public:
    enum class State : uint8_t {
        Idle,                  // Nothing running. Pump() is a no-op.
        QueryRunning,          // Worker thread is hitting GitHub.
        QueryDoneNoUpdate,     // Worker done; we're up to date. Pump emits "latest" toast.
        QueryDoneNewer,        // Worker done; updates exist. Pump emits the install prompt.
        InstallRunning,        // User accepted. Worker downloading + extracting.
        InstallDoneOk,         // Pump refreshes resources and reloads installed scripts.
        InstallDoneFailed,     // Query OR install failed. Pump emits the captured error.
    };

    // Process-wide singleton. Joins its worker thread on destruction.
    static UpdateChecker& Instance();

    UpdateChecker(const UpdateChecker&) = delete;
    UpdateChecker& operator=(const UpdateChecker&) = delete;

    // Begin a query if currently Idle. No-op if a check is already
    // running, so spamming the UPDATE button is harmless.
    void StartQuery();

    // Drain finished worker work. MUST be called from the main thread,
    // every frame, while a screen that surfaces update progress (the
    // About screen) is active. Opens any modal dialogs the state machine
    // currently wants to show. Cheap when state == Idle.
    void Pump();

    // Single-line status the caller can paint on screen — empty string
    // when no work is in flight. Returned uppercase to match the engine
    // font's preferred casing.
    std::string StatusLine() const;

    // True until a worker's result has been applied on the main thread. Gates
    // re-entry on this (we already do internally in StartQuery), but it's
    // also useful for "grey out the UPDATE button" affordances.
    bool IsBusy() const;

private:
    UpdateChecker() = default;
    ~UpdateChecker();

    void RunQueryWorker();
    void RunInstallWorker();
    void JoinWorker();
    void Fail(const std::string& pMessage);
    bool ApplyInstalledContent();

    // ---------------------------------------------------------------------
    // State. mState is the only field touched by both threads — everything
    // else is published via the release/acquire pairing on mState.store /
    // mState.load. The worker writes its result fields, then stores into
    // mState; the main thread loads mState (acquire), then reads the
    // result fields. mErrorMessage and mProgressPercent are exceptions
    // because they may be touched WHILE the worker is running (status-
    // line read; progress callback write); those have their own sync.
    // ---------------------------------------------------------------------
    std::atomic<State> mState{State::Idle};
    std::thread        mWorker;

    // Result fields owned by the worker, read by Pump after a state
    // transition. Not synchronised — the release/acquire on mState is the
    // happens-before edge.
    DataRelease        mRelease;
    ReleaseManifest    mDataManifest;
    ReleaseManifest    mScriptManifest;
    int                mInstalledDataVer   = 0;
    int                mInstalledScriptVer = 0;
    bool               mDataNewer          = false;
    bool               mScriptNewer        = false;
    bool               mDataInstalled      = false;
    bool               mScriptsInstalled   = false;
    std::string        mDataTargetDir;
    std::string        mScriptsTargetDir;

    // Worker writes during a transfer for status-line display; main
    // thread reads each frame.
    std::atomic<int>   mProgressPercent{0};

    // Error text touched from both threads (worker writes on failure,
    // StatusLine could observe it). Guarded by its own mutex.
    mutable std::mutex mStringMutex;
    std::string        mErrorMessage;
};

} // namespace Setup
