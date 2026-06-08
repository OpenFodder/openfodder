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
#include <cstdio>
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
    , mRollingBack(false)
    , mSessionReady(false)
    , mLocalPlayerIndex(0)
{
    mPeerHandles.fill(GGPO_INVALID_HANDLE);
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
                         uint8_t peerIndex,
                         uint8_t numPlayers,
                         std::atomic<uint32_t>* sharedSeq)
{
    mLocalPlayerIndex = localPlayerIndex;
    // P1 A1: caller (cFodderMultiplayer::Network_Start) hands the negotiated
    // peer count via numPlayers. Defaults to NETWORK_MAX_PLAYERS so the
    // legacy 2P path stays byte-identical. Clamp into [2..kMaxRollbackPlayers]
    // — GGPO's static arrays below are sized at kMaxRollbackPlayers and the
    // ggpo_start_session call must match.
    if (numPlayers < 2)
        numPlayers = 2;
    if (numPlayers > kMaxRollbackPlayers)
        numPlayers = (uint8_t)kMaxRollbackPlayers;
    mNumPlayers       = numPlayers;

    // Cache OFHUB/2 framing state. mUseRelayFraming gates both the REGISTER
    // burst below and (eventually) the wrap/unwrap hook install. An all-zero
    // key means the caller is on the LAN/direct path and skips framing.
    mSessionKey      = sessionKey;
    mPeerIndex       = peerIndex;
    mUseRelayFraming = !std::all_of(sessionKey.begin(), sessionKey.end(),
                                    [](unsigned char b) { return b == 0; });
    mSharedSeq = sharedSeq;
    mLocalPort = localPort;
    // mLocalSeq is the LAN/SyncTest fallback; on the relay path
    // OfhubWrap reads from mSharedSeq (cFodderMultiplayer::mRelaySeq) so
    // lobby/briefing/GGPO share one monotonic stream.
    mLocalSeq.store(1u, std::memory_order_relaxed);
    for (auto& replay : mReplay)
        replay = openfodder_hubframe::ReplayWindow{};

    // Cache the real hub address so OfhubWrap can rewrite GGPO's per-tick
    // sendto() destinations off the synthetic 127.0.0.<i+1> _peer_addr and
    // back onto the hub. Mirrors the inet_pton/htons pattern used for the
    // REGISTER burst below. Only meaningful when mUseRelayFraming is true,
    // but populating unconditionally keeps the sockaddr in a defined state
    // (zeroed family means "no rewrite" if anyone ever flipped the hook on
    // without going through Start()).
    if (mUseRelayFraming) {
        std::memset(&mHubAddr, 0, sizeof(mHubAddr));
        mHubAddr.sin_family = AF_INET;
        mHubAddr.sin_port   = htons(remotePort);
        inet_pton(AF_INET, remoteHost.c_str(), &mHubAddr.sin_addr);
    } else {
        std::memset(&mHubAddr, 0, sizeof(mHubAddr));
    }

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

    // P1 A1: ggpo_start_session takes the active peer count; the surrounding
    // arrays (players[], handles[]) are statically sized at kMaxRollbackPlayers
    // (the GGPO ceiling) but only mNumPlayers slots are populated.
    GGPOErrorCode result = ggpo_start_session(
        &mSession, &cb, "OpenFodder",
        mNumPlayers,
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

    // P1 A1: arrays sized at the rollback ceiling so the build is one shape;
    // we only populate mNumPlayers slots and ggpo_add_player ignores the rest.
    GGPOPlayer players[kMaxRollbackPlayers];
    memset(players, 0, sizeof(players));

    for (int i = 0; i < (int)mNumPlayers; ++i) {
        players[i].size         = sizeof(GGPOPlayer);
        players[i].player_num   = i + 1;

        if (i == localPlayerIndex) {
            players[i].type = GGPO_PLAYERTYPE_LOCAL;
        } else {
            players[i].type = GGPO_PLAYERTYPE_REMOTE;
            if (mUseRelayFraming) {
                // Synthetic per-peer address (127.0.0.<peer+1>). The
                // peer index here is the slot index `i`, matching the
                // ParsedFrame::peer value OfhubUnwrap stamps into
                // recv_addr_inout->sin_addr. The actual outbound dest
                // for sendto() is the hub -- OfhubWrap's framing layer
                // and GGPO's _peer_addr cache combine such that GGPO
                // calls SendTo with the synthetic addr but the real
                // hub address ends up on the wire (sendto() in
                // Udp::SendTo passes whatever sockaddr we hand it, and
                // we hand it the cached _peer_addr; that's where the
                // OFHUB/2 wrap path ought to be reading the hub addr
                // from -- wiring is unchanged here, only the demux key
                // is). See HandlesMsg spike §4 option (b).
                char synth_ip[16];
                std::snprintf(synth_ip, sizeof(synth_ip),
                              "127.0.0.%d", i + 1);
                strncpy(players[i].u.remote.ip_address, synth_ip,
                        sizeof(players[i].u.remote.ip_address) - 1);
                players[i].u.remote.port = localPort;
            } else {
                // Direct (non-relay) path: real remote endpoint.
                strncpy(players[i].u.remote.ip_address, remoteHost.c_str(),
                        sizeof(players[i].u.remote.ip_address) - 1);
                players[i].u.remote.port = remotePort;
            }
        }
    }

    GGPOPlayerHandle handles[kMaxRollbackPlayers];
    for (int i = 0; i < (int)mNumPlayers; ++i) {
        result = ggpo_add_player(mSession, &players[i], &handles[i]);
        if (!GGPO_SUCCEEDED(result)) {
            g_Debugger->Error("[GGPO] ggpo_add_player failed for player " + std::to_string(i) + ": " + std::to_string(result));
            Stop();
            return false;
        }
    }

    mLocalHandle = handles[localPlayerIndex];
    // Phase B1: scatter remote handles into the per-peer slot array. Slot
    // == localPlayerIndex stays GGPO_INVALID_HANDLE (set by the constructor
    // / Stop()) and is never read. mNumPlayers == 2 in production but the
    // loop is N-shaped so adding more remote slots in Phase C is mechanical.
    for (int i = 0; i < mNumPlayers; ++i) {
        if (i == localPlayerIndex) continue;
        mPeerHandles[i] = handles[i];
    }

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
    mNumPlayers       = NETWORK_MAX_PLAYERS;
    GGPOPlayer p1, p2;
    p1.size = p2.size = sizeof(GGPOPlayer);
    p1.player_num = 1;  p1.type = GGPO_PLAYERTYPE_LOCAL;
    p2.player_num = 2;  p2.type = GGPO_PLAYERTYPE_LOCAL;

    ggpo_add_player(mSession, &p1, &mLocalHandle);
    ggpo_add_player(mSession, &p2, &mPeerHandles[1]);

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
        mPeerHandles.fill(GGPO_INVALID_HANDLE);
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
bool cGGPOSession::SynchronizeInput(sNetworkInput inputs[kMaxRollbackPlayers],
                                    int& disconnectFlags) {
    if (!mSession) return false;
    // [P1 input-bus widening 2026-06-08: array dimension is kMaxRollbackPlayers
    //  so a 4-player session has the bus it needs. ggpo_synchronize_input
    //  writes only mNumPlayers slots (the active peer count handed to
    //  ggpo_start_session) and leaves slots mNumPlayers..kMaxRollbackPlayers-1
    //  untouched; callers zero-init the array so those tail slots stay {0}
    //  for 2P sessions, preserving bit-identical behaviour at N=2.]
    GGPOErrorCode result = ggpo_synchronize_input(
        mSession,
        static_cast<void*>(inputs),
        sizeof(sNetworkInput) * kMaxRollbackPlayers,
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

    // [P1 input-bus widening 2026-06-08: stack-allocated bus widened from
    //  NETWORK_MAX_PLAYERS=2 to kMaxRollbackPlayers=4. GGPO writes only the
    //  active peer slots; the memset ensures inactive tail slots are {0} so
    //  rollback replay sees exactly the same input shape as a live frame at
    //  any N in [2..4].]
    sNetworkInput inputs[kMaxRollbackPlayers];
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
                            char* outBuffer, int capacity,
                            struct sockaddr_in* dst_inout) {
    auto* self = static_cast<cGGPOSession*>(ctx);
    if (!self || inLen < 0 || !outBuffer) return 0;

    const std::size_t needed =
        openfodder_hubframe::kMinFrameSize + static_cast<std::size_t>(inLen);
    if (capacity < 0 || static_cast<std::size_t>(capacity) < needed) return 0;

    // Pull the next seq from the shared lobby/briefing/GGPO counter when
    // available. With port-preservation NAT all three sockets collapse to
    // one external (ip,port) at the hub, so the per-(peer, session_key)
    // 1024-bit replay window must see one monotonic stream.
    std::atomic<uint32_t>* counter = self->mSharedSeq ? self->mSharedSeq : &self->mLocalSeq;
    const uint32_t seq = counter->fetch_add(1, std::memory_order_relaxed);
    const auto written = openfodder_hubframe::BuildAndTag(
        openfodder_hubframe::FrameType::Data,
        seq,
        self->mPeerIndex,
        reinterpret_cast<const unsigned char*>(in),
        static_cast<std::size_t>(inLen),
        self->mSessionKey,
        reinterpret_cast<unsigned char*>(outBuffer));

    // Rewrite the sendto() destination off the synthetic 127.0.0.<i+1>
    // address (which GGPO cached as each UdpProtocol::_peer_addr from the
    // ggpo_add_player call) and onto the real hub address. Without this,
    // sendto() would deliver the framed packet to localhost loopback with
    // no listener and the hub would never see a DATA frame from us.
    // Symmetric counterpart to OfhubUnwrap's recv_addr_inout rewrite.
    if (dst_inout) {
        *dst_inout = self->mHubAddr;
    }
    return static_cast<int>(written);
}

int cGGPOSession::OfhubUnwrap(void* ctx, const char* in, int inLen,
                              char* outBuffer, int capacity,
                              struct sockaddr_in* recv_addr_inout) {
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
    // Per-peer replay window. Each remote sender owns its own seq space, so
    // a 2-slot collapse would (incorrectly) reject perfectly valid frames at
    // N>2. Bounds-check defensively: a malformed/out-of-range peer index
    // must be dropped before indexing into the array.
    if (f.peer >= kMaxRollbackPlayers) return 0;
    if (!self->mReplay[f.peer].AcceptAndAdvance(f.seq)) return 0;
    if (capacity < 0 || static_cast<std::size_t>(capacity) < f.payloadLen) return 0;
    std::memcpy(outBuffer, f.payload, f.payloadLen);

    // Rewrite the recvfrom() sockaddr to a deterministic per-peer
    // synthetic address (127.0.0.<peer+1>, mLocalPort) so GGPO's
    // address-keyed endpoint demux (Peer2PeerBackend::OnMsg /
    // UdpProtocol::HandlesMsg) routes verified frames to the right
    // UdpProtocol instance. Under OFHUB/2 fan-out every datagram arrives
    // from the shared hub address; without this rewrite endpoint 0
    // swallows all peer traffic and endpoints 1..N-1 starve.
    //
    // HandlesMsg compares BOTH sin_addr AND sin_port
    // (udp_proto.cpp:297-298), and Start() registers each remote slot at
    // (synthetic_ip, localPort), so we must rewrite sin_port to mLocalPort
    // here too — leaving the hub's source port (30000) in place causes
    // every frame to be rejected at the port check and the GGPO sync
    // handshake never completes.
    if (recv_addr_inout) {
        recv_addr_inout->sin_family      = AF_INET;
        recv_addr_inout->sin_addr.s_addr = htonl(0x7F000001u + f.peer);
        recv_addr_inout->sin_port        = htons(self->mLocalPort);
    }
    return static_cast<int>(f.payloadLen);
}

#endif // OPENFODDER_ENABLE_NETWORK
