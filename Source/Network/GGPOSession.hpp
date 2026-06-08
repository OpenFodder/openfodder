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

#ifdef WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#endif

#include "NetworkTypes.hpp"
#include "HubFrame.hpp"
#include <ggponet.h>
#include <array>
#include <atomic>
#include <cstdint>
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

    // Start a P2P session.
    //   localPlayerIndex : 0 = player 1 is local, 1 = player 2 is local, ...
    //   localPort        : UDP port this instance listens on
    //   remoteHost       : IP / hostname of the remote peer
    //   remotePort       : UDP port of the remote peer
    //
    // NOTE (P0 Phase B1): the single (remoteHost, remotePort) pair is a
    // 2-player compatibility shim. Phase C / P1 will widen this to a
    // per-peer vector once the gameplay layer is N-player aware. For now
    // every non-local slot is wired to the same remote endpoint, and the
    // GGPO session is still started with NETWORK_MAX_PLAYERS=2 inside.
    bool Start(int localPlayerIndex,
               unsigned short localPort,
               const std::string& remoteHost,
               unsigned short remotePort,
               const std::array<unsigned char, openfodder_hubframe::kSessionKeySize>& sessionKey = {},
               uint8_t peerIndex = 0,
               uint8_t numPlayers = NETWORK_MAX_PLAYERS,
               std::atomic<uint32_t>* sharedSeq = nullptr);

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

    // Synchronise inputs; fills `inputs[kMaxRollbackPlayers]` and returns
    // whether all active peers' inputs are available. Slots beyond
    // mNumPlayers (the active-peer count, in [2..kMaxRollbackPlayers]) are
    // left untouched by GGPO — callers must zero-init the array.
    // [P1 input-bus widening 2026-06-08: was NETWORK_MAX_PLAYERS=2; now sized
    //  at the GGPO rollback ceiling so a 4-player session has slots 2..3
    //  available. ggpo_synchronize_input is invoked with the full kMaxRollbackPlayers
    //  byte-size; GGPO writes to slots 0..mNumPlayers-1 and ignores the rest.]
    bool SynchronizeInput(sNetworkInput inputs[kMaxRollbackPlayers], int& disconnectFlags);

    // Notify GGPO that the frame has been fully simulated.
    void AdvanceFrame();

    // True while GGPO is rolling back (replaying old frames).
    bool IsRollingBack() const { return mRollingBack; }

    // OFHUB/2 wrap/unwrap callbacks. The ggpo-patch agent will install these
    // on GGPO's internal Udp socket so every datagram GGPO emits/consumes is
    // wrapped in the OFHUB/2 binary frame (HMAC-tagged with mSessionKey) when
    // mUseRelayFraming is true. ctx is the cGGPOSession* so the static
    // trampoline can reach instance state (seq counter, replay window, key).
    //
    // OfhubUnwrap also rewrites recv_addr_inout to a synthetic per-peer
    // address (127.0.0.<peer+1>) so GGPO's address-keyed demux in
    // Peer2PeerBackend::OnMsg / UdpProtocol::HandlesMsg routes each
    // verified peer's datagrams to the right endpoint. Without this
    // every datagram appears to come from the hub's sockaddr and only
    // endpoint 0 receives traffic (HandlesMsg spike, Documentation/
    // MultiplayerNPlayerScope_HandlesMsgSpike.md §4 option (b)).
    //
    // OfhubWrap symmetrically rewrites dst_inout to mHubAddr so the
    // sendto() syscall in Udp::SendTo delivers to the real hub instead
    // of the synthetic 127.0.0.<i+1> address that GGPO caches as each
    // UdpProtocol's _peer_addr (since we register peers with the
    // synthetic addr via ggpo_add_player). Without this rewrite GGPO's
    // outbound packets would be dropped on the floor by the loopback
    // stack with no listener.
    static int OfhubWrap(void* ctx, const char* in, int inLen,
                         char* outBuffer, int capacity,
                         struct sockaddr_in* dst_inout);
    static int OfhubUnwrap(void* ctx, const char* in, int inLen,
                           char* outBuffer, int capacity,
                           struct sockaddr_in* recv_addr_inout);

private:
    GGPOSession*    mSession;
    GGPOPlayerHandle mLocalHandle;
    // P0 Phase B1: per-peer GGPO handles, indexed by player_num-1
    // (i.e. the same 0-based slot index used in Start()'s players[] loop).
    // mPeerHandles[localPlayerIndex] is unused (stays GGPO_INVALID_HANDLE).
    // Production still runs at mNumPlayers=2; the type is N-shaped so the
    // receive-side state has the right footprint when Phase C widens
    // gameplay to N peers.
    std::array<GGPOPlayerHandle, kMaxRollbackPlayers> mPeerHandles{};
    bool            mRollingBack;
    bool            mSessionReady;
    int             mLocalPlayerIndex;
    // Active peer count for this session. Still 2 in production (Phase B1
    // is data-shape only); Phase C/P1 raises this up to kMaxRollbackPlayers
    // once the lobby/briefing layer can negotiate N>2 endpoints.
    uint8_t         mNumPlayers = 2;

    // OFHUB/2 relay framing state (populated by Start()).
    std::array<unsigned char, openfodder_hubframe::kSessionKeySize> mSessionKey{};
    uint8_t                          mPeerIndex        = 0;
    bool                             mUseRelayFraming  = false;
    // OFHUB/2 outbound DATA seq counter. When mSharedSeq is non-null GGPO
    // uses it (cFodderMultiplayer::mRelaySeq) so lobby + briefing + GGPO
    // share one monotonic stream against the hub's per-peer 1024-bit replay
    // window — the three sockets collapse to one external (ip,port) under
    // port-preservation NAT, so independent counters cause REPLAY_DROP at
    // every phase transition. mLocalSeq remains as a fallback when no
    // shared counter is plumbed (LAN-direct / SyncTest).
    std::atomic<uint32_t>*           mSharedSeq = nullptr;
    std::atomic<uint32_t>            mLocalSeq{1};
    // Real hub address resolved at Start() time. OfhubWrap rewrites the
    // sendto() destination to this so GGPO's outbound packets actually
    // reach the hub, even though _peer_addr in each UdpProtocol caches
    // the synthetic 127.0.0.<i+1> address used for HandlesMsg demux.
    sockaddr_in                      mHubAddr{};
    // Local listening port — GGPO registers every remote peer at
    // (127.0.0.<i+1>, mLocalPort). UdpProtocol::HandlesMsg compares BOTH
    // sin_addr AND sin_port (udp_proto.cpp:297), so OfhubUnwrap has to
    // rewrite sin_port to this value too. Without it the recv'd sin_port
    // is the hub's data-plane port (30000) and every peer's UdpProtocol
    // rejects the frame at the port check → sync handshake never lands.
    unsigned short                   mLocalPort = 0;
    // Per-peer replay window, indexed by ParsedFrame::peer. Each remote peer
    // owns an independent OFHUB/2 sequence space, so collapsing them into
    // a single window would cause spurious replay drops at N>2 (latent
    // bug at 2P because there's only one remote sender). mReplay[localSlot]
    // is unused.
    std::array<openfodder_hubframe::ReplayWindow, kMaxRollbackPlayers> mReplay;

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
