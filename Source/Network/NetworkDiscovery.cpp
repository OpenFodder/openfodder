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

static uint32_t Discovery_NowTicks() {
    return (uint32_t)SDL_GetTicks();
}

static void Discovery_CopyString(char* pDest, size_t pDestSize, const std::string& pSource) {
    if (!pDestSize)
        return;

    memset(pDest, 0, pDestSize);
    size_t Len = pSource.size();
    if (Len >= pDestSize)
        Len = pDestSize - 1;

    memcpy(pDest, pSource.c_str(), Len);
}

cNetworkDiscovery::cNetworkDiscovery() {
}

cNetworkDiscovery::~cNetworkDiscovery() {
    Stop();
}

bool cNetworkDiscovery::StartHost() {
    Stop();
    mBrowser = false;
    return OpenSocket(0, true);
}

bool cNetworkDiscovery::StartBrowser() {
    Stop();
    mBrowser = true;
    return OpenSocket(NETWORK_DISCOVERY_PORT, false);
}

bool cNetworkDiscovery::OpenSocket(uint16_t pBindPort, bool pBroadcast) {
#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        g_Debugger->Error("[Discovery] WSAStartup failed.");
        return false;
    }
#endif

    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mSocket == INVALID_SOCKET) {
        g_Debugger->Error("[Discovery] socket() failed.");
        return false;
    }

    int Opt = 1;
    setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&Opt, sizeof(Opt));
    if (pBroadcast)
        setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (const char*)&Opt, sizeof(Opt));

    struct sockaddr_in LocalAddr;
    memset(&LocalAddr, 0, sizeof(LocalAddr));
    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_addr.s_addr = INADDR_ANY;
    LocalAddr.sin_port = htons(pBindPort);

    if (bind(mSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR) {
        g_Debugger->Error("[Discovery] bind() failed on port " + std::to_string(pBindPort));
        closesocket(mSocket);
        mSocket = INVALID_SOCKET;
#ifdef WIN32
        WSACleanup();
#endif
        return false;
    }

    SetNonBlocking();
    return true;
}

void cNetworkDiscovery::SetNonBlocking() {
#ifdef WIN32
    u_long NonBlocking = 1;
    ioctlsocket(mSocket, FIONBIO, &NonBlocking);
#else
    int Flags = fcntl(mSocket, F_GETFL, 0);
    fcntl(mSocket, F_SETFL, Flags | O_NONBLOCK);
#endif
}

void cNetworkDiscovery::Stop() {
    if (mSocket != INVALID_SOCKET) {
        closesocket(mSocket);
        mSocket = INVALID_SOCKET;
#ifdef WIN32
        WSACleanup();
#endif
    }

    mGames.clear();
    mLastAdvertiseTicks = 0;
    mLastVersionMismatchTicks = 0;
    mLastCompatibilityMismatchTicks = 0;
    mLastExpiredGameTicks = 0;
}

void cNetworkDiscovery::Advertise(const sNetworkDiscoveryGame& pGame) {
    if (mSocket == INVALID_SOCKET || mBrowser)
        return;

    const uint32_t Now = Discovery_NowTicks();
    if (mLastAdvertiseTicks && Now - mLastAdvertiseTicks < 1000)
        return;
    mLastAdvertiseTicks = Now;

    sNetworkDiscoveryPacket Packet;
    memset(&Packet, 0, sizeof(Packet));
    Packet.magic = sNetworkDiscoveryPacket::MAGIC;
    Packet.type = sNetworkDiscoveryPacket::ADVERTISEMENT;
    Packet.version = sNetworkDiscoveryPacket::VERSION;
    Packet.state = pGame.mState;
    Packet.currentPlayers = pGame.mCurrentPlayers;
    Packet.maxPlayers = pGame.mMaxPlayers;
    Packet.gameMode = (uint8_t)pGame.mSettings.mGameMode;
    Packet.teamCount = pGame.mSettings.mTeamCount;
    Packet.teamSize = pGame.mSettings.mTeamSize;
    Packet.friendlyFire = pGame.mSettings.mFriendlyFire;
    Packet.networkCompatibilityVersion = pGame.mCompatibilityVersion;
    Packet.mapSize = (uint8_t)pGame.mSettings.mMapSize;
    Packet.mapTerrain = (uint8_t)pGame.mSettings.mMapTerrain;
    Packet.vehicleSet = (uint8_t)pGame.mSettings.mVehicleSet;
    Packet.pickupDensity = (uint8_t)pGame.mSettings.mPickupDensity;
    Packet.coverDensity = (uint8_t)pGame.mSettings.mCoverDensity;
    Packet.lobbyPort = pGame.mLobbyPort;
    Packet.gameplayPort = pGame.mGameplayPort;
    Packet.killLimit = pGame.mSettings.mKillLimit;
    Packet.timeLimitSeconds = pGame.mSettings.mTimeLimitSeconds;
    Packet.mapSeed = pGame.mSettings.mMapSeed;
    Discovery_CopyString(Packet.hostName, sizeof(Packet.hostName), pGame.mHostName);
    Discovery_CopyString(Packet.gameName, sizeof(Packet.gameName), pGame.mGameName);

    struct sockaddr_in BroadcastAddr;
    memset(&BroadcastAddr, 0, sizeof(BroadcastAddr));
    BroadcastAddr.sin_family = AF_INET;
    BroadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;
    BroadcastAddr.sin_port = htons(NETWORK_DISCOVERY_PORT);

    sendto(mSocket, (const char*)&Packet, sizeof(Packet), 0,
           (struct sockaddr*)&BroadcastAddr, sizeof(BroadcastAddr));
}

void cNetworkDiscovery::PollBrowser() {
    if (mSocket == INVALID_SOCKET || !mBrowser)
        return;

    const uint32_t Now = Discovery_NowTicks();
    ExpireGames(Now);

    for (;;) {
        sNetworkDiscoveryPacket Packet;
        struct sockaddr_in FromAddr;
#ifdef WIN32
        int FromLen = sizeof(FromAddr);
#else
        socklen_t FromLen = sizeof(FromAddr);
#endif

        int Bytes = recvfrom(mSocket, (char*)&Packet, sizeof(Packet), 0,
                             (struct sockaddr*)&FromAddr, &FromLen);
        if (Bytes == SOCKET_ERROR)
            break;

        if (Bytes != sizeof(Packet))
            continue;

        if (Packet.magic != sNetworkDiscoveryPacket::MAGIC ||
            Packet.type != sNetworkDiscoveryPacket::ADVERTISEMENT) {
            continue;
        }

        if (Packet.version != sNetworkDiscoveryPacket::VERSION) {
            mLastVersionMismatchTicks = Now;
            continue;
        }

        if (Packet.networkCompatibilityVersion != NETWORK_COMPATIBILITY_VERSION) {
            mLastCompatibilityMismatchTicks = Now;
            continue;
        }

        char AddrText[INET_ADDRSTRLEN];
        memset(AddrText, 0, sizeof(AddrText));
        inet_ntop(AF_INET, &FromAddr.sin_addr, AddrText, sizeof(AddrText));

        Packet.hostName[sizeof(Packet.hostName) - 1] = '\0';
        Packet.gameName[sizeof(Packet.gameName) - 1] = '\0';

        sNetworkDiscoveryGame Game;
        Game.mHostAddress = AddrText;
        Game.mHostName = Packet.hostName;
        Game.mGameName = Packet.gameName;
        Game.mSettings.mGameMode = Network_NormalizeGameMode(Packet.gameMode);
        Game.mSettings.mTeamCount = Packet.teamCount ? Packet.teamCount : NETWORK_TEAM_COUNT_DEFAULT;
        Game.mSettings.mTeamSize = Packet.teamSize ? Packet.teamSize : NETWORK_TEAM_SIZE_DEFAULT;
        Game.mSettings.mFriendlyFire = Packet.friendlyFire ? 1 : 0;
        Game.mSettings.mMapSeed = Packet.mapSeed;
        Game.mSettings.mKillLimit = Packet.killLimit;
        Game.mSettings.mTimeLimitSeconds = Packet.timeLimitSeconds;
        Game.mSettings.mMapSize = Network_NormalizeMapSize(Packet.mapSize);
        Game.mSettings.mMapTerrain = Network_NormalizeMapTerrain(Packet.mapTerrain);
        Game.mSettings.mVehicleSet = Network_NormalizeVehicleSet(Packet.vehicleSet);
        Game.mSettings.mPickupDensity = Network_NormalizePickupDensity(Packet.pickupDensity);
        Game.mSettings.mCoverDensity = Network_NormalizeCoverDensity(Packet.coverDensity);
        Game.mLobbyPort = Packet.lobbyPort;
        Game.mGameplayPort = Packet.gameplayPort;
        Game.mCurrentPlayers = Packet.currentPlayers;
        Game.mMaxPlayers = Packet.maxPlayers ? Packet.maxPlayers : NETWORK_MAX_PLAYERS;
        Game.mState = Packet.state <= eNetworkDiscoveryState_Locked ? Packet.state : eNetworkDiscoveryState_Locked;
        Game.mCompatibilityVersion = Packet.networkCompatibilityVersion;
        Game.mLastSeenTicks = Now;
        UpsertGame(Game);
    }
}

void cNetworkDiscovery::ExpireGames(uint32_t pNow) {
    bool ExpiredAny = false;

    for (auto it = mGames.begin(); it != mGames.end();) {
        if (pNow - it->mLastSeenTicks > 5000) {
            ExpiredAny = true;
            it = mGames.erase(it);
        }
        else {
            ++it;
        }
    }

    if (ExpiredAny)
        mLastExpiredGameTicks = pNow;
}

void cNetworkDiscovery::UpsertGame(const sNetworkDiscoveryGame& pGame) {
    for (auto& Game : mGames) {
        if (Game.mHostAddress == pGame.mHostAddress && Game.mLobbyPort == pGame.mLobbyPort) {
            Game = pGame;
            return;
        }
    }

    mGames.push_back(pGame);
}

#endif // OPENFODDER_ENABLE_NETWORK
