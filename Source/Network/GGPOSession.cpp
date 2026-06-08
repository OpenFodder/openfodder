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

#ifdef OPENFODDER_ENABLE_NETWORK

#include "stdafx.hpp"
#include "Network/GGPOSession.hpp"
#include <algorithm>
#include <cstring>

#ifdef WIN32
#  include <winsock2.h>
#  pragma comment(lib, "ws2_32.lib")
#endif

// -----------------------------------------------------------------------
// Static instance pointer for callback routing
// -----------------------------------------------------------------------
cGGPOSession* cGGPOSession::sInstance = nullptr;

// -----------------------------------------------------------------------
cGGPOSession::cGGPOSession()
    : mSession(nullptr)
    , mLocalHandle(GGPO_INVALID_HANDLE)
    , mRemoteHandle(GGPO_INVALID_HANDLE)
    , mRollingBack(false)
    , mSessionReady(false)
    , mLocalPlayerIndex(0)
{
    sInstance = this;

#ifdef WIN32
    // GGPO is built as a static lib so DllMain is never called.
    // We must initialise Winsock ourselves before any socket() calls.
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        g_Debugger->Error("[GGPO] WSAStartup failed – network will not work.");
    }
#endif
}

cGGPOSession::~cGGPOSession() {
    Stop();
    if (sInstance == this)
        sInstance = nullptr;

#ifdef WIN32
    WSACleanup();
#endif
}

// -----------------------------------------------------------------------
bool cGGPOSession::Start(int localPlayerIndex,
                         unsigned short localPort,
                         const std::string& remoteHost,
                         unsigned short remotePort,
                         const std::array<unsigned char, openfodder_hubframe::kSessionKeySize>& sessionKey,
                         uint8_t peerIndex)
{
    mLocalPlayerIndex = localPlayerIndex;

    // Cache OFHUB/2 framing state. mUseRelayFraming gates both the REGISTER
    // burst below and (eventually) the wrap/unwrap hook install. An all-zero
    // key means the caller is on the LAN/direct path and skips framing.
    mSessionKey      = sessionKey;
    mPeerIndex       = peerIndex;
    mUseRelayFraming = !std::all_of(sessionKey.begin(), sessionKey.end(),
                                    [](unsigned char b) { return b == 0; });
    mLocalSeq.store(1, std::memory_order_relaxed);
    mReplay = openfodder_hubframe::ReplayWindow{};

    if (mUseRelayFraming) {
        SOCKET RegisterSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (RegisterSocket != INVALID_SOCKET) {
            struct sockaddr_in LocalAddr;
            memset(&LocalAddr, 0, sizeof(LocalAddr));
            LocalAddr.sin_family = AF_INET;
            LocalAddr.sin_addr.s_addr = INADDR_ANY;
            LocalAddr.sin_port = htons(localPort);

            if (bind(RegisterSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr)) == 0) {
                struct sockaddr_in RemoteAddr;
                memset(&RemoteAddr, 0, sizeof(RemoteAddr));
                RemoteAddr.sin_family = AF_INET;
                RemoteAddr.sin_port = htons(remotePort);
                inet_pton(AF_INET, remoteHost.c_str(), &RemoteAddr.sin_addr);

                // OFHUB/2 §6 binary REGISTER: type=0x02, seq=0, peer index,
                // zero-length payload, 8-byte HMAC tag. Mirrors the briefing
                // socket's pattern in Fodder_Network.cpp.
                unsigned char RegisterFrame[openfodder_hubframe::kMinFrameSize];
                const std::size_t RegisterFrameLen = openfodder_hubframe::BuildAndTag(
                    openfodder_hubframe::FrameType::Register,
                    /*seq=*/0,
                    mPeerIndex,
                    /*payload=*/nullptr,
                    /*payloadLen=*/0,
                    mSessionKey,
                    RegisterFrame);
                for (int i = 0; i < 3; ++i) {
                    sendto(RegisterSocket,
                           reinterpret_cast<const char*>(RegisterFrame),
                           static_cast<int>(RegisterFrameLen), 0,
                           (struct sockaddr*)&RemoteAddr, sizeof(RemoteAddr));
                    SDL_Delay(5);
                }
            }
            closesocket(RegisterSocket);
        }
    }

    GGPOSessionCallbacks cb;
    memset(&cb, 0, sizeof(cb));
    cb.begin_game      = cb_BeginGame;
    cb.save_game_state = cb_SaveGameState;
    cb.load_game_state = cb_LoadGameState;
    cb.log_game_state  = cb_LogGameState;
    cb.free_buffer     = cb_FreeBuffer;
    cb.advance_frame   = cb_AdvanceFrame;
    cb.on_event        = cb_OnEvent;

    GGPOErrorCode result = ggpo_start_session(
        &mSession, &cb, "OpenFodder",
        NETWORK_MAX_PLAYERS,
        sizeof(sNetworkInput),
        localPort
    );

    if (!GGPO_SUCCEEDED(result)) {
        g_Debugger->Error("[GGPO] ggpo_start_session failed: " + std::to_string(result));
        return false;
    }

    // Install OFHUB/2 wrap/unwrap hooks on GGPO's Udp socket. Without
    // this, GGPO's own data-plane packets would bypass OFHUB/2 framing
    // (the lobby/briefing paths still tag correctly because they use
    // their own sockets). The public `ggpo_p2p_install_ofhub_hooks` API
    // lives in our vendored GGPO patch (cmake/ggpo-ofhub2-hooks.patch)
    // and is only valid for peer-to-peer sessions.
    if (mUseRelayFraming) {
        GGPOErrorCode hookResult = ggpo_p2p_install_ofhub_hooks(
            mSession, &OfhubWrap, &OfhubUnwrap, this);
        if (!GGPO_SUCCEEDED(hookResult)) {
            g_Debugger->Error("[GGPO] ggpo_p2p_install_ofhub_hooks failed: "
                              + std::to_string(hookResult));
        }
    }

    ggpo_set_disconnect_timeout(mSession, 3000);
    ggpo_set_disconnect_notify_start(mSession, 1000);

    GGPOPlayer players[NETWORK_MAX_PLAYERS];
    memset(players, 0, sizeof(players));

    for (int i = 0; i < NETWORK_MAX_PLAYERS; ++i) {
        players[i].size         = sizeof(GGPOPlayer);
        players[i].player_num   = i + 1;

        if (i == localPlayerIndex) {
            players[i].type = GGPO_PLAYERTYPE_LOCAL;
        } else {
            players[i].type = GGPO_PLAYERTYPE_REMOTE;
            strncpy(players[i].u.remote.ip_address, remoteHost.c_str(),
                    sizeof(players[i].u.remote.ip_address) - 1);
            players[i].u.remote.port = remotePort;
        }
    }

    GGPOPlayerHandle handles[NETWORK_MAX_PLAYERS];
    for (int i = 0; i < NETWORK_MAX_PLAYERS; ++i) {
        result = ggpo_add_player(mSession, &players[i], &handles[i]);
        if (!GGPO_SUCCEEDED(result)) {
            g_Debugger->Error("[GGPO] ggpo_add_player failed for player " + std::to_string(i) + ": " + std::to_string(result));
            Stop();
            return false;
        }
    }

    mLocalHandle  = handles[localPlayerIndex];
    mRemoteHandle = handles[1 - localPlayerIndex];

    // Frame delay adds latency to LOCAL input: input for frame N is held
    // until frame N+delay, giving the remote peer time to receive it before
    // needing to predict.  Without this, every frame requires prediction and
    // any network latency causes visible rollback "jumps" when remote input
    // arrives.  2 GGPO frames ≈ 120 ms at ~16.67 Hz simulation rate — fine
    // for a strategy game, and eliminates rollback artefacts on LAN/localhost.
    ggpo_set_frame_delay(mSession, mLocalHandle, 2);

    g_Debugger->Notice("[GGPO] Session started. Local player: " + std::to_string(localPlayerIndex + 1) +
                       ", listening on port " + std::to_string(localPort));
    return true;
}

// -----------------------------------------------------------------------
bool cGGPOSession::StartSyncTest(int checkDistance) {
    GGPOSessionCallbacks cb;
    memset(&cb, 0, sizeof(cb));
    cb.begin_game      = cb_BeginGame;
    cb.save_game_state = cb_SaveGameState;
    cb.load_game_state = cb_LoadGameState;
    cb.log_game_state  = cb_LogGameState;
    cb.free_buffer     = cb_FreeBuffer;
    cb.advance_frame   = cb_AdvanceFrame;
    cb.on_event        = cb_OnEvent;

    GGPOErrorCode result = ggpo_start_synctest(
        &mSession, &cb, "OpenFodder",
        NETWORK_MAX_PLAYERS,
        sizeof(sNetworkInput),
        checkDistance
    );

    if (!GGPO_SUCCEEDED(result)) {
        g_Debugger->Error("[GGPO] ggpo_start_synctest failed: " + std::to_string(result));
        return false;
    }

    mLocalPlayerIndex = 0;
    GGPOPlayer p1, p2;
    p1.size = p2.size = sizeof(GGPOPlayer);
    p1.player_num = 1;  p1.type = GGPO_PLAYERTYPE_LOCAL;
    p2.player_num = 2;  p2.type = GGPO_PLAYERTYPE_LOCAL;

    ggpo_add_player(mSession, &p1, &mLocalHandle);
    ggpo_add_player(mSession, &p2, &mRemoteHandle);

    mSessionReady = true;  // no peer handshake needed in sync-test mode
    g_Debugger->Notice("[GGPO] Sync-test session started (check_distance=" + std::to_string(checkDistance) + ")");
    return true;
}

// -----------------------------------------------------------------------
void cGGPOSession::Stop() {
    if (mSession) {
        ggpo_close_session(mSession);
        mSession      = nullptr;
        mLocalHandle  = GGPO_INVALID_HANDLE;
        mRemoteHandle = GGPO_INVALID_HANDLE;
    }
}

// -----------------------------------------------------------------------
void cGGPOSession::Idle(int timeoutMs) {
    if (mSession)
        ggpo_idle(mSession, timeoutMs);
}

// -----------------------------------------------------------------------
bool cGGPOSession::AddLocalInput(const sNetworkInput& input) {
    if (!mSession) return false;
    GGPOErrorCode result = ggpo_add_local_input(
        mSession, mLocalHandle,
        const_cast<void*>(static_cast<const void*>(&input)),
        sizeof(sNetworkInput)
    );
    return GGPO_SUCCEEDED(result);
}

// -----------------------------------------------------------------------
bool cGGPOSession::SynchronizeInput(sNetworkInput inputs[NETWORK_MAX_PLAYERS],
                                    int& disconnectFlags) {
    if (!mSession) return false;
    GGPOErrorCode result = ggpo_synchronize_input(
        mSession,
        static_cast<void*>(inputs),
        sizeof(sNetworkInput) * NETWORK_MAX_PLAYERS,
        &disconnectFlags
    );
    return GGPO_SUCCEEDED(result);
}

// -----------------------------------------------------------------------
void cGGPOSession::AdvanceFrame() {
    if (mSession)
        ggpo_advance_frame(mSession);
}

// -----------------------------------------------------------------------
// Static GGPO callbacks
// -----------------------------------------------------------------------

bool __cdecl cGGPOSession::cb_SaveGameState(unsigned char** buffer, int* len,
                                             int* checksum, int /*frame*/) {
    if (!sInstance || !sInstance->cbSaveState) return false;
    return sInstance->cbSaveState(buffer, len, checksum, 0);
}

bool __cdecl cGGPOSession::cb_LoadGameState(unsigned char* buffer, int len) {
    if (!sInstance || !sInstance->cbLoadState) return false;
    return sInstance->cbLoadState(buffer, len);
}

void __cdecl cGGPOSession::cb_FreeBuffer(void* buffer) {
    if (sInstance && sInstance->cbFreeBuffer)
        sInstance->cbFreeBuffer(buffer);
}

bool __cdecl cGGPOSession::cb_AdvanceFrame(int flags) {
    if (!sInstance || !sInstance->cbAdvanceFrame) return false;

    sInstance->mRollingBack = true;

    sNetworkInput inputs[NETWORK_MAX_PLAYERS];
    memset(inputs, 0, sizeof(inputs));
    int disconnectFlags = 0;
    sInstance->SynchronizeInput(inputs, disconnectFlags);

    bool ok = sInstance->cbAdvanceFrame(inputs, flags);

    // GGPO requires advance_frame callbacks to call ggpo_advance_frame
    // so it can increment its internal frame counter and save state.
    // (Same pattern as the vectorwar reference implementation.)
    sInstance->AdvanceFrame();

    sInstance->mRollingBack = false;
    return ok;
}

bool __cdecl cGGPOSession::cb_OnEvent(GGPOEvent* info) {
    if (!info) return true;
    switch (info->code) {
        case GGPO_EVENTCODE_CONNECTED_TO_PEER:
            g_Debugger->Notice("[GGPO] Connected to peer (player " + std::to_string(info->u.connected.player) + ")");
            break;
        case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
            g_Debugger->Notice("[GGPO] Synchronizing with peer (player " + std::to_string(info->u.synchronizing.player) +
                               ") " + std::to_string(info->u.synchronizing.count) +
                               "/" + std::to_string(info->u.synchronizing.total));
            break;
        case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
            g_Debugger->Notice("[GGPO] Synchronised with peer (player " + std::to_string(info->u.synchronized.player) + ")");
            break;
        case GGPO_EVENTCODE_RUNNING:
            g_Debugger->Notice("[GGPO] Session is running");
            if (sInstance) sInstance->mSessionReady = true;
            break;
        case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
            g_Debugger->Notice("[GGPO] Connection interrupted (player " + std::to_string(info->u.connection_interrupted.player) +
                               ", disconnect_timeout=" + std::to_string(info->u.connection_interrupted.disconnect_timeout) + ")");
            break;
        case GGPO_EVENTCODE_CONNECTION_RESUMED:
            g_Debugger->Notice("[GGPO] Connection resumed (player " + std::to_string(info->u.connection_resumed.player) + ")");
            break;
        case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
            g_Debugger->Notice("[GGPO] Disconnected from peer (player " + std::to_string(info->u.disconnected.player) + ")");
            break;
        case GGPO_EVENTCODE_TIMESYNC:
            // Slow the local loop so the remote peer can catch up.
            // Each GGPO frame is ~60 ms at our 16.67 Hz sim rate.
            // This mirrors the vectorwar reference implementation.
            SDL_Delay(info->u.timesync.frames_ahead * 60);
            break;
        default:
            break;
    }
    return true;
}

// -----------------------------------------------------------------------
// OFHUB/2 wrap/unwrap callbacks
//
// These are static C-style trampolines (signature dictated by the
// ggpo-patch agent's planned ggpo_install_ofhub_hooks API). `ctx` is the
// owning cGGPOSession*, recovered with a static_cast. Both functions
// return the number of bytes written into outBuffer, or 0 on drop/error.
// -----------------------------------------------------------------------

int cGGPOSession::OfhubWrap(void* ctx, const char* in, int inLen,
                            char* outBuffer, int capacity) {
    auto* self = static_cast<cGGPOSession*>(ctx);
    if (!self || inLen < 0 || !outBuffer) return 0;

    const std::size_t needed =
        openfodder_hubframe::kMinFrameSize + static_cast<std::size_t>(inLen);
    if (capacity < 0 || static_cast<std::size_t>(capacity) < needed) return 0;

    // mLocalSeq starts at 1 (REGISTER reserves seq=0). post-increment so the
    // first DATA frame goes out as seq=1 and we monotonically advance.
    const uint32_t seq = self->mLocalSeq.fetch_add(1, std::memory_order_relaxed);
    const auto written = openfodder_hubframe::BuildAndTag(
        openfodder_hubframe::FrameType::Data,
        seq,
        self->mPeerIndex,
        reinterpret_cast<const unsigned char*>(in),
        static_cast<std::size_t>(inLen),
        self->mSessionKey,
        reinterpret_cast<unsigned char*>(outBuffer));
    return static_cast<int>(written);
}

int cGGPOSession::OfhubUnwrap(void* ctx, const char* in, int inLen,
                              char* outBuffer, int capacity) {
    auto* self = static_cast<cGGPOSession*>(ctx);
    if (!self || inLen < 0 || !in || !outBuffer) return 0;

    openfodder_hubframe::ParsedFrame f;
    if (!openfodder_hubframe::VerifyAndParse(
            reinterpret_cast<const unsigned char*>(in),
            static_cast<std::size_t>(inLen),
            self->mSessionKey,
            &f)) {
        return 0;
    }
    if (f.type != openfodder_hubframe::FrameType::Data) return 0;
    if (!self->mReplay.AcceptAndAdvance(f.seq)) return 0;
    if (capacity < 0 || static_cast<std::size_t>(capacity) < f.payloadLen) return 0;
    std::memcpy(outBuffer, f.payload, f.payloadLen);
    return static_cast<int>(f.payloadLen);
}

#endif // OPENFODDER_ENABLE_NETWORK
