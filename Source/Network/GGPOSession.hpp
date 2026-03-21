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

#ifdef OPENFODDER_ENABLE_NETWORK

#include "NetworkTypes.hpp"
#include <ggponet.h>
#include <string>
#include <functional>

class cFodder;

// -----------------------------------------------------------------------
// cGGPOSession
//
// Thin wrapper around a GGPOSession*.  Owns the session lifetime and
// routes all GGPO callbacks back into the cFodder instance.
// -----------------------------------------------------------------------
class cGGPOSession {
public:
    // Callbacks wired into cFodder
    std::function<bool(uint8_t**, int*, int*, int)>  cbSaveState;
    std::function<bool(uint8_t*, int)>               cbLoadState;
    std::function<void(void*)>                       cbFreeBuffer;
    std::function<bool(sNetworkInput*, int)>         cbAdvanceFrame;

    cGGPOSession();
    ~cGGPOSession();

    // Start a two-player P2P session.
    //   localPlayerIndex : 0 = player 1 is local, 1 = player 2 is local
    //   localPort        : UDP port this instance listens on
    //   remoteHost       : IP / hostname of the remote peer
    //   remotePort       : UDP port of the remote peer
    bool Start(int localPlayerIndex,
               unsigned short localPort,
               const std::string& remoteHost,
               unsigned short remotePort);

    // Start a local sync-test session (no network, tests determinism).
    bool StartSyncTest(int checkDistance = 8);

    // Shut down and free the GGPO session.
    void Stop();

    bool IsRunning() const { return mSession != nullptr; }

    // True once GGPO fires GGPO_EVENTCODE_RUNNING, meaning both peers
    // have completed their initial synchronization handshake.
    bool IsSessionReady() const { return mSessionReady; }

    // Per-frame API -----------------------------------------------------------

    // Give GGPO CPU time to pump the network (call once per frame).
    void Idle(int timeoutMs = 0);

    // Submit local player input for the current frame.
    bool AddLocalInput(const sNetworkInput& input);

    // Synchronise inputs; fills `inputs[NETWORK_MAX_PLAYERS]` and returns
    // whether both players' inputs are available.
    bool SynchronizeInput(sNetworkInput inputs[NETWORK_MAX_PLAYERS], int& disconnectFlags);

    // Notify GGPO that the frame has been fully simulated.
    void AdvanceFrame();

    // Frame number that GGPO is currently at.
    int CurrentFrame() const;

    // True while GGPO is rolling back (replaying old frames).
    bool IsRollingBack() const { return mRollingBack; }

private:
    GGPOSession*    mSession;
    GGPOPlayerHandle mLocalHandle;
    GGPOPlayerHandle mRemoteHandle;
    bool            mRollingBack;
    bool            mSessionReady;
    int             mLocalPlayerIndex;

    // -----------------------------------------------------------------------
    // Static trampoline functions – GGPO calls these; they delegate to the
    // instance callbacks stored on the session's context pointer.
    // -----------------------------------------------------------------------
    static bool __cdecl cb_BeginGame(const char* /*game*/) { return true; }
    static bool __cdecl cb_SaveGameState(unsigned char** buffer, int* len, int* checksum, int frame);
    static bool __cdecl cb_LoadGameState(unsigned char* buffer, int len);
    static bool __cdecl cb_LogGameState(char* /*filename*/, unsigned char* /*buffer*/, int /*len*/) { return true; }
    static void __cdecl cb_FreeBuffer(void* buffer);
    static bool __cdecl cb_AdvanceFrame(int flags);
    static bool __cdecl cb_OnEvent(GGPOEvent* info);

    // The single live session pointer so static callbacks can reach it.
    static cGGPOSession* sInstance;
};

#endif // OPENFODDER_ENABLE_NETWORK
