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
#  include <unistd.h>
#  include <fcntl.h>
#  define SOCKET int
#  define INVALID_SOCKET (-1)
#  define SOCKET_ERROR (-1)
#  define closesocket close
#endif

#include <array>
#include <atomic>
#include <string>
#include <cstdint>
#include <cstring>

#include "HubFrame.hpp"
#include "NetworkTypes.hpp"

// Lobby packet exchanged via UDP side-channel during campaign selection.
// Sent periodically (~every frame) so both players stay in sync.
//
// VERSION 6 (P0 N-player, 2026-06): the legacy `playerId` per-lobby sender
// nonce was removed — Frame.peer (authenticated by the OFHUB/2 HMAC tag) is
// the authoritative sender identity now. Peers are tracked via a per-slot
// table keyed on Frame.peer (see sLobbyPeerSlot below).
struct sLobbyPacket {
    static const uint32_t MAGIC = 0x4C4F4232; // "LOB2"
    static const uint8_t  VERSION = 6;

    uint32_t magic;
    uint8_t  type;
    uint8_t  version;
    uint8_t  ready;          // 1 = this player is ready
    uint8_t  started;        // 1 = host says GO, load this campaign
    int16_t  selection;      // campaign list index the host is viewing
    uint8_t  connected;      // 1 = peer has received at least one packet
    uint8_t  mapTerrainSub;  // random-map terrain sub-tileset (was padding); synced host->joiner for map determinism
    char     campaign[64];   // campaign name string (null-terminated)
    uint8_t  gameMode;       // eNetworkGameMode
    uint8_t  teamCount;
    uint8_t  teamSize;
    uint8_t  friendlyFire;
    uint32_t mapSeed;
    uint16_t killLimit;
    uint16_t timeLimitSeconds;
    uint8_t  selectedTeam;
    uint8_t  selectedClass;
    uint8_t  lockedIn;
    uint8_t  mapSize;
    uint8_t  mapTerrain;
    uint8_t  vehicleSet;
    uint8_t  pickupDensity;
    uint8_t  coverDensity;

    enum eType : uint8_t {
        LOBBY_STATE = 0x01,
    };
};

// V5 had a 4-byte playerId nonce (removed); V6 is 100 - 4 = 96 bytes.
static_assert(sizeof(sLobbyPacket) == 96, "sLobbyPacket must be 96 bytes");

// Per-slot peer state, indexed by Frame.peer (0..kMaxRoomCapacity-1).
// First-sighting binds peer.addr from the recvfrom source address. Each
// slot owns its own ReplayWindow so out-of-order frames from different
// peers don't cross-poison the seq dedup.
struct sLobbyPeerSlot {
    sockaddr_in                              addr {};
    bool                                     bound = false;       // first-sighting binds
    bool                                     ready = false;
    bool                                     connected = false;
    bool                                     started = false;
    bool                                     lockedIn = false;
    int16_t                                  selection = 0;
    uint8_t                                  team = 0;
    uint8_t                                  klass = 0;
    sNetworkMatchSettings                    matchSettings;
    std::string                              campaign;
    openfodder_hubframe::ReplayWindow        replay;
};

class cNetworkLobby {
public:
    cNetworkLobby();
    ~cNetworkLobby();

    bool Start(
        uint16_t pLocalPort,
        const std::string& pRemoteHost,
        uint16_t pRemotePort,
        bool pIsHost,
        const std::array<unsigned char, openfodder_hubframe::kSessionKeySize>& pSessionKey = {},
        uint8_t pPeerIndex = 0,
        bool pPreserveRemoteEndpoint = false,
        std::atomic<uint32_t>* pSharedSeq = nullptr);
    void Stop();

    // Current value of the shared seq counter (or local mLocalSeq on LAN). Used
    // by callers that bridge to a follow-on phase (briefing READY-sync, GGPO)
    // to start their seq above the lobby's last sent value.
    uint32_t CurrentSeq() const {
        return mSharedSeq ? mSharedSeq->load(std::memory_order_relaxed) : mLocalSeq;
    }

    // Call once per frame to send/receive
    void Poll();

    // Local state setters (host sets selection, either player sets ready)
    void SetSelection(int16_t pIndex, const std::string& pCampaign);
    void SetMatchSettings(const sNetworkMatchSettings& pSettings);
    void SetPlayerSelection(uint8_t pTeam, uint8_t pClass, bool pLockedIn);
    void SetReady(bool pReady);
    void SetStarted();

    // Remote state getters
    bool        IsConnected() const { return mRemoteConnected; }
    bool        IsRemoteReady() const { return mRemoteReady; }
    bool        IsRemoteStarted() const { return mRemoteStarted; }
    int16_t     GetRemoteSelection() const { return mRemoteSelection; }
    std::string GetRemoteCampaign() const { return mRemoteCampaign; }
    sNetworkMatchSettings GetRemoteMatchSettings() const { return mRemoteMatchSettings; }
    uint8_t     GetRemoteSelectedTeam() const { return mRemoteSelectedTeam; }
    uint8_t     GetRemoteSelectedClass() const { return mRemoteSelectedClass; }
    bool        IsRemoteLockedIn() const { return mRemoteLockedIn; }
    std::string GetRemoteHost() const;
    uint16_t    GetRemotePort() const;
    bool        IsHost() const { return mIsHost; }
    bool        IsRunning() const { return mSocket != INVALID_SOCKET; }

    // P1 A1: per-slot accessors for the N-player roster UI. The legacy
    // mRemote* getters above stay as the "first non-local peer" aggregate
    // for 2P parity; the new accessors expose the underlying mPeers table
    // so the lobby campaign-select screen can render one row per peer.
    uint8_t     GetLocalPeerIndex() const { return mPeerIndex; }
    const sLobbyPeerSlot& GetPeerSlot(uint8_t pSlot) const { return mPeers[pSlot]; }

private:
    void Send();
    void SendRelayRegistration();
    void Receive();
    void RefreshRemoteAggregate();

    SOCKET              mSocket = INVALID_SOCKET;
    struct sockaddr_in  mRemoteAddr;        // hub/peer endpoint we send to (relay or LAN partner)
    bool                mIsHost = false;
    bool                mPreserveRemoteEndpoint = false;
    uint32_t            mLastRelayRegisterTicks = 0;

    // OFHUB/2 binary frame plumbing. When mHasSessionKey is true the lobby
    // wraps every outbound datagram in a type=0x01 DATA frame and verifies
    // every inbound datagram with the same key (re-tagged by the hub on
    // fan-out per OFHUB2.md §6.4). LAN/SyncTest paths leave mHasSessionKey
    // false and exchange raw sLobbyPacket bytes as before.
    //
    // V6: per-slot mPeers[] table replaces the V5 single-remote
    // (mRemotePlayerId nonce + scalar mReplay) shape. Frame.peer is HMAC-
    // authenticated by the OFHUB/2 hub on fan-out, so it's safe to use as a
    // direct table index.
    std::array<unsigned char, openfodder_hubframe::kSessionKeySize> mSessionKey{};
    uint8_t                          mPeerIndex = 0;
    bool                             mHasSessionKey = false;
    // Pointer to cFodderMultiplayer::mRelaySeq when relay framing is active —
    // shared with Network_Briefing_ReadySync and cGGPOSession so all three
    // socket lifetimes use one monotonic seq stream against the hub's per-
    // (peer, session_key) replay window. nullptr on LAN/SyncTest paths
    // (mHasSessionKey=false), where the legacy local mLocalSeq is used.
    std::atomic<uint32_t>*           mSharedSeq = nullptr;
    uint32_t                         mLocalSeq = 1;

    // Per-slot peer table — Frame.peer (HMAC-authenticated) is the index.
    // mPeers[mPeerIndex] is the local slot; remote slots are bound on first
    // sighting from a verified Data frame.
    std::array<sLobbyPeerSlot, kMaxRoomCapacity> mPeers;

    // Local state (what we send)
    int16_t     mLocalSelection = 0;
    std::string mLocalCampaign;
    sNetworkMatchSettings mLocalMatchSettings;
    uint8_t     mLocalSelectedTeam = 0;
    uint8_t     mLocalSelectedClass = 0;
    bool        mLocalLockedIn = false;
    bool        mLocalReady = false;
    bool        mLocalStarted = false;

    // Aggregated "first non-local peer" view, refreshed from mPeers after
    // every Receive(). Preserves the legacy 2P getter API; Phase C will
    // expose explicit per-slot accessors for the 3..N-player UI.
    bool        mRemoteConnected = false;
    bool        mRemoteReady = false;
    bool        mRemoteStarted = false;
    int16_t     mRemoteSelection = 0;
    std::string mRemoteCampaign;
    sNetworkMatchSettings mRemoteMatchSettings;
    uint8_t     mRemoteSelectedTeam = 0;
    uint8_t     mRemoteSelectedClass = 0;
    bool        mRemoteLockedIn = false;
};

#endif // OPENFODDER_ENABLE_NETWORK
