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

#ifdef OPENFODDER_ENABLE_NETWORK

#include <algorithm>
#include "HubFrame.hpp"

static uint32_t Lobby_GeneratePlayerId(uint16_t pLocalPort, bool pIsHost) {
    uint32_t Id = 2166136261u;

    auto Mix = [&Id](uint32_t pValue) {
        Id ^= pValue;
        Id *= 16777619u;
    };

    Mix((uint32_t)SDL_GetTicks());
    Mix((uint32_t)pLocalPort);
    Mix(pIsHost ? 0x484F5354u : 0x4A4F494Eu); // HOST / JOIN
    Mix((uint32_t)(uintptr_t)&Id);

    return Id ? Id : 1;
}

cNetworkLobby::cNetworkLobby() {
    memset(&mRemoteAddr, 0, sizeof(mRemoteAddr));
}

cNetworkLobby::~cNetworkLobby() {
    Stop();
}

bool cNetworkLobby::Start(
    uint16_t pLocalPort,
    const std::string& pRemoteHost,
    uint16_t pRemotePort,
    bool pIsHost,
    const std::array<unsigned char, openfodder_hubframe::kSessionKeySize>& pSessionKey,
    uint8_t pPeerIndex,
    bool pPreserveRemoteEndpoint) {
    Stop();
    mIsHost = pIsHost;
    mSessionKey = pSessionKey;
    mPeerIndex = pPeerIndex;
    // An all-zero key is the "no session" sentinel — Parameters.hpp resets
    // mNetworkSessionKey to all zeros and only the hub claim populates it.
    // OFHUB/2 §6.2 specifies the key is 32 random bytes, so any non-zero byte
    // means we have a real key.
    mHasSessionKey = std::any_of(pSessionKey.begin(), pSessionKey.end(),
                                 [](unsigned char b) { return b != 0; });
    mLocalSeq = 1; // type=0x01 DATA frames; REGISTER reserves seq=0.
    mReplay = openfodder_hubframe::ReplayWindow();
    mPreserveRemoteEndpoint = pPreserveRemoteEndpoint;
    mLastRelayRegisterTicks = 0;
    mLocalPlayerId = Lobby_GeneratePlayerId(pLocalPort, pIsHost);
    mRemotePlayerId = 0;

#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        g_Debugger->Error("[Lobby] WSAStartup failed.");
        return false;
    }
#endif

    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mSocket == INVALID_SOCKET) {
        g_Debugger->Error("[Lobby] socket() failed.");
        return false;
    }

    // Bind to local port
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family      = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port        = htons(pLocalPort);

    if (bind(mSocket, (struct sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
        g_Debugger->Error("[Lobby] bind() failed on port " + std::to_string(pLocalPort));
        closesocket(mSocket);
        mSocket = INVALID_SOCKET;
        return false;
    }

    // Set non-blocking
#ifdef WIN32
    u_long nonBlocking = 1;
    ioctlsocket(mSocket, FIONBIO, &nonBlocking);
#else
    int flags = fcntl(mSocket, F_GETFL, 0);
    fcntl(mSocket, F_SETFL, flags | O_NONBLOCK);
#endif

    // Remote address
    memset(&mRemoteAddr, 0, sizeof(mRemoteAddr));
    mRemoteAddr.sin_family = AF_INET;
    mRemoteAddr.sin_port   = htons(pRemotePort);
    inet_pton(AF_INET, pRemoteHost.c_str(), &mRemoteAddr.sin_addr);

    mRemoteConnected = false;
    mRemoteReady = false;
    mRemoteStarted = false;
    mRemoteSelection = 0;
    mRemoteCampaign.clear();
    mRemoteMatchSettings = sNetworkMatchSettings();
    mRemoteSelectedTeam = 0;
    mRemoteSelectedClass = 0;
    mRemoteLockedIn = false;
    mLocalSelection = 0;
    mLocalCampaign.clear();
    mLocalMatchSettings = sNetworkMatchSettings();
    mLocalSelectedTeam = 0;
    mLocalSelectedClass = 0;
    mLocalLockedIn = false;
    mLocalReady = false;
    mLocalStarted = false;

    g_Debugger->Notice("[Lobby] Started on port " + std::to_string(pLocalPort) +
                       " -> " + pRemoteHost + ":" + std::to_string(pRemotePort) +
                       " id=" + std::to_string(mLocalPlayerId) +
                       (pIsHost ? " (HOST)" : " (JOIN)"));
    SendRelayRegistration();
    return true;
}

void cNetworkLobby::Stop() {
    if (mSocket != INVALID_SOCKET) {
        closesocket(mSocket);
        mSocket = INVALID_SOCKET;
#ifdef WIN32
        WSACleanup();
#endif
        g_Debugger->Notice("[Lobby] Stopped.");
    }
}

void cNetworkLobby::Poll() {
    if (mSocket == INVALID_SOCKET)
        return;

    Send();
    Receive();
}

void cNetworkLobby::SetSelection(int16_t pIndex, const std::string& pCampaign) {
    mLocalSelection = pIndex;
    mLocalCampaign = pCampaign;
}

void cNetworkLobby::SetMatchSettings(const sNetworkMatchSettings& pSettings) {
    mLocalMatchSettings = pSettings;
    mLocalMatchSettings.mGameMode = Network_NormalizeGameMode((uint8_t)pSettings.mGameMode);
    mLocalMatchSettings.mMapSize = Network_NormalizeMapSize((uint8_t)pSettings.mMapSize);
    mLocalMatchSettings.mMapTerrain = Network_NormalizeMapTerrain((uint8_t)pSettings.mMapTerrain);
    mLocalMatchSettings.mVehicleSet = Network_NormalizeVehicleSet((uint8_t)pSettings.mVehicleSet);
    mLocalMatchSettings.mPickupDensity = Network_NormalizePickupDensity((uint8_t)pSettings.mPickupDensity);
    mLocalMatchSettings.mCoverDensity = Network_NormalizeCoverDensity((uint8_t)pSettings.mCoverDensity);
    if (!mLocalMatchSettings.mTeamCount)
        mLocalMatchSettings.mTeamCount = NETWORK_TEAM_COUNT_DEFAULT;
    if (!mLocalMatchSettings.mTeamSize)
        mLocalMatchSettings.mTeamSize = NETWORK_TEAM_SIZE_DEFAULT;
}

void cNetworkLobby::SetPlayerSelection(uint8_t pTeam, uint8_t pClass, bool pLockedIn) {
    mLocalSelectedTeam = pTeam;
    mLocalSelectedClass = pClass;
    mLocalLockedIn = pLockedIn;
}

void cNetworkLobby::SetReady(bool pReady) {
    mLocalReady = pReady;
}

void cNetworkLobby::SetStarted() {
    mLocalStarted = true;
}

std::string cNetworkLobby::GetRemoteHost() const {
    char AddrText[INET_ADDRSTRLEN];
    memset(AddrText, 0, sizeof(AddrText));
    if (!inet_ntop(AF_INET, &mRemoteAddr.sin_addr, AddrText, sizeof(AddrText)))
        return "";

    return AddrText;
}

uint16_t cNetworkLobby::GetRemotePort() const {
    return ntohs(mRemoteAddr.sin_port);
}

void cNetworkLobby::Send() {
    SendRelayRegistration();

    sLobbyPacket pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.magic     = sLobbyPacket::MAGIC;
    pkt.playerId  = mLocalPlayerId;
    pkt.type      = sLobbyPacket::LOBBY_STATE;
    pkt.version   = sLobbyPacket::VERSION;
    pkt.ready     = mLocalReady ? 1 : 0;
    pkt.selection = mLocalSelection;
    pkt.started   = mLocalStarted ? 1 : 0;
    pkt.connected = 1;
    pkt.gameMode = (uint8_t)mLocalMatchSettings.mGameMode;
    pkt.teamCount = mLocalMatchSettings.mTeamCount;
    pkt.teamSize = mLocalMatchSettings.mTeamSize;
    pkt.friendlyFire = mLocalMatchSettings.mFriendlyFire ? 1 : 0;
    pkt.mapSeed = mLocalMatchSettings.mMapSeed;
    pkt.killLimit = mLocalMatchSettings.mKillLimit;
    pkt.timeLimitSeconds = mLocalMatchSettings.mTimeLimitSeconds;
    pkt.selectedTeam = mLocalSelectedTeam;
    pkt.selectedClass = mLocalSelectedClass;
    pkt.lockedIn = mLocalLockedIn ? 1 : 0;
    pkt.mapSize = (uint8_t)mLocalMatchSettings.mMapSize;
    pkt.mapTerrain = (uint8_t)mLocalMatchSettings.mMapTerrain;
    pkt.mapTerrainSub = mLocalMatchSettings.mMapTerrainSub;
    pkt.vehicleSet = (uint8_t)mLocalMatchSettings.mVehicleSet;
    pkt.pickupDensity = (uint8_t)mLocalMatchSettings.mPickupDensity;
    pkt.coverDensity = (uint8_t)mLocalMatchSettings.mCoverDensity;

    // Copy campaign name (truncate if too long)
    size_t len = mLocalCampaign.size();
    if (len >= sizeof(pkt.campaign))
        len = sizeof(pkt.campaign) - 1;
    memcpy(pkt.campaign, mLocalCampaign.c_str(), len);
    pkt.campaign[len] = '\0';

    if (mHasSessionKey) {
        // OFHUB/2 §6.1: type=0x01 DATA, monotonic per-peer seq, our peer
        // index, sLobbyPacket as opaque payload, 8-byte HMAC-SHA256 tag.
        unsigned char FrameBuf[openfodder_hubframe::kMinFrameSize + sizeof(pkt)];
        const std::size_t FrameLen = openfodder_hubframe::BuildAndTag(
            openfodder_hubframe::FrameType::Data,
            mLocalSeq++,
            mPeerIndex,
            reinterpret_cast<const unsigned char*>(&pkt),
            sizeof(pkt),
            mSessionKey,
            FrameBuf);
        sendto(mSocket, reinterpret_cast<const char*>(FrameBuf),
               static_cast<int>(FrameLen), 0,
               (struct sockaddr*)&mRemoteAddr, sizeof(mRemoteAddr));
    } else {
        sendto(mSocket, (const char*)&pkt, sizeof(pkt), 0,
               (struct sockaddr*)&mRemoteAddr, sizeof(mRemoteAddr));
    }
}

void cNetworkLobby::SendRelayRegistration() {
    if (mSocket == INVALID_SOCKET || !mHasSessionKey)
        return;

    const uint32_t Now = (uint32_t)SDL_GetTicks();
    if (mLastRelayRegisterTicks && Now - mLastRelayRegisterTicks < 1000)
        return;
    mLastRelayRegisterTicks = Now;

    // OFHUB/2 §6.1 type=0x02 REGISTER: empty payload, seq=0 (reserved), peer
    // index, 8-byte HMAC tag — same shape Fodder_Network.cpp's briefing
    // handshake socket uses to register with the hub.
    unsigned char RegisterFrame[openfodder_hubframe::kMinFrameSize];
    const std::size_t RegisterFrameLen = openfodder_hubframe::BuildAndTag(
        openfodder_hubframe::FrameType::Register,
        /*seq=*/0,
        mPeerIndex,
        /*payload=*/nullptr,
        /*payloadLen=*/0,
        mSessionKey,
        RegisterFrame);
    sendto(mSocket, reinterpret_cast<const char*>(RegisterFrame),
           static_cast<int>(RegisterFrameLen), 0,
           (struct sockaddr*)&mRemoteAddr, sizeof(mRemoteAddr));
}

void cNetworkLobby::Receive() {
    sLobbyPacket pkt;
    sLobbyPacket latestPkt;
    memset(&latestPkt, 0, sizeof(latestPkt));

    struct sockaddr_in fromAddr;
    memset(&fromAddr, 0, sizeof(fromAddr));
#ifdef WIN32
    int fromLen;
#else
    socklen_t fromLen;
#endif

    // Drain all pending packets, keep the latest
    bool gotPacket = false;
    for (;;) {
        fromLen = sizeof(fromAddr);

        if (mHasSessionKey) {
            // OFHUB/2 path. The hub re-tags fan-out frames with OUR session
            // key (§6.4), so verify with our own key, drop on tag mismatch,
            // drop non-Data types, then run the seq through the 1024-bit
            // replay window before extracting the payload.
            unsigned char RecvFrame[2048];
            int n = recvfrom(mSocket, reinterpret_cast<char*>(RecvFrame),
                             static_cast<int>(sizeof(RecvFrame)), 0,
                             (struct sockaddr*)&fromAddr, &fromLen);
            if (n <= 0)
                break;
            openfodder_hubframe::ParsedFrame Frame{};
            if (!openfodder_hubframe::VerifyAndParse(
                    RecvFrame, static_cast<std::size_t>(n), mSessionKey, &Frame))
                continue;
            if (Frame.type != openfodder_hubframe::FrameType::Data)
                continue;
            if (!mReplay.AcceptAndAdvance(Frame.seq))
                continue;
            if (Frame.payloadLen != sizeof(pkt))
                continue;
            memcpy(&pkt, Frame.payload, sizeof(pkt));
        } else {
            int n = recvfrom(mSocket, (char*)&pkt, sizeof(pkt), 0,
                             (struct sockaddr*)&fromAddr, &fromLen);
            if (n != sizeof(pkt))
                break;
        }

        if (pkt.magic != sLobbyPacket::MAGIC)
            continue;
        if (pkt.version != sLobbyPacket::VERSION)
            continue;
        if (pkt.type != sLobbyPacket::LOBBY_STATE)
            continue;
        if (!pkt.playerId || pkt.playerId == mLocalPlayerId)
            continue;
        if (mRemotePlayerId && pkt.playerId != mRemotePlayerId)
            continue;

        if (!mRemotePlayerId)
            mRemotePlayerId = pkt.playerId;

        // The host learns the joiner's address from incoming lobby packets.
        // Joiners already have the host endpoint from the menu/discovery; do
        // not rewrite it from packet source addresses because loopback/LAN
        // routes can alternate on the same machine and produce unstable
        // gameplay endpoints.
        if (mIsHost && !mPreserveRemoteEndpoint)
            mRemoteAddr = fromAddr;
        latestPkt = pkt;
        gotPacket = true;
    }

    if (!gotPacket)
        return;

    mRemoteConnected = (latestPkt.connected != 0);
    mRemoteReady     = (latestPkt.ready != 0);
    mRemoteStarted   = (latestPkt.started != 0);
    mRemoteSelection = latestPkt.selection;

    latestPkt.campaign[sizeof(latestPkt.campaign) - 1] = '\0';
    mRemoteCampaign = latestPkt.campaign;
    mRemoteMatchSettings.mGameMode = Network_NormalizeGameMode(latestPkt.gameMode);
    mRemoteMatchSettings.mTeamCount = latestPkt.teamCount ? latestPkt.teamCount : NETWORK_TEAM_COUNT_DEFAULT;
    mRemoteMatchSettings.mTeamSize = latestPkt.teamSize ? latestPkt.teamSize : NETWORK_TEAM_SIZE_DEFAULT;
    mRemoteMatchSettings.mFriendlyFire = latestPkt.friendlyFire ? 1 : 0;
    mRemoteMatchSettings.mMapSeed = latestPkt.mapSeed;
    mRemoteMatchSettings.mKillLimit = latestPkt.killLimit;
    mRemoteMatchSettings.mTimeLimitSeconds = latestPkt.timeLimitSeconds;
    mRemoteMatchSettings.mMapSize = Network_NormalizeMapSize(latestPkt.mapSize);
    mRemoteMatchSettings.mMapTerrain = Network_NormalizeMapTerrain(latestPkt.mapTerrain);
    mRemoteMatchSettings.mMapTerrainSub = latestPkt.mapTerrainSub;
    mRemoteMatchSettings.mVehicleSet = Network_NormalizeVehicleSet(latestPkt.vehicleSet);
    mRemoteMatchSettings.mPickupDensity = Network_NormalizePickupDensity(latestPkt.pickupDensity);
    mRemoteMatchSettings.mCoverDensity = Network_NormalizeCoverDensity(latestPkt.coverDensity);
    mRemoteSelectedTeam = latestPkt.selectedTeam;
    mRemoteSelectedClass = latestPkt.selectedClass;
    mRemoteLockedIn = (latestPkt.lockedIn != 0);
}

#endif // OPENFODDER_ENABLE_NETWORK
