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

#include <cstdint>
#include <string>
#include <vector>

#include "NetworkTypes.hpp"

static const uint16_t NETWORK_DISCOVERY_PORT = 7002;

enum eNetworkDiscoveryState : uint8_t {
    eNetworkDiscoveryState_Setup = 0,
    eNetworkDiscoveryState_InProgress,
    eNetworkDiscoveryState_Full,
    eNetworkDiscoveryState_Locked,
};

struct sNetworkDiscoveryGame {
    std::string mHostAddress;
    std::string mHostName;
    std::string mGameName;
    sNetworkMatchSettings mSettings;
    uint16_t mLobbyPort = 0;
    uint16_t mGameplayPort = 0;
    uint8_t mCurrentPlayers = 0;
    uint8_t mMaxPlayers = NETWORK_MAX_PLAYERS;
    uint8_t mState = eNetworkDiscoveryState_Setup;
    uint8_t mCompatibilityVersion = NETWORK_COMPATIBILITY_VERSION;
    uint32_t mLastSeenTicks = 0;
};

struct sNetworkDiscoveryPacket {
    static const uint32_t MAGIC = 0x4F46444Cu; // "OFDL"
    static const uint8_t VERSION = 5;

    uint32_t magic;
    uint8_t type;
    uint8_t version;
    uint8_t state;
    uint8_t currentPlayers;
    uint8_t maxPlayers;
    uint8_t gameMode;
    uint8_t teamCount;
    uint8_t teamSize;
    uint8_t friendlyFire;
    uint8_t networkCompatibilityVersion;
    uint8_t mapSize;
    uint8_t mapTerrain;
    uint8_t mapTerrainSub;
    uint8_t vehicleSet;
    uint8_t pickupDensity;
    uint8_t coverDensity;
    uint16_t lobbyPort;
    uint16_t gameplayPort;
    uint16_t killLimit;
    uint16_t timeLimitSeconds;
    uint32_t mapSeed;
    char hostName[32];
    char gameName[32];

    enum eType : uint8_t {
        ADVERTISEMENT = 0x01,
    };
};

static_assert(sizeof(sNetworkDiscoveryPacket) == 96, "sNetworkDiscoveryPacket must be 96 bytes");

class cNetworkDiscovery {
public:
    cNetworkDiscovery();
    ~cNetworkDiscovery();

    bool StartHost();
    bool StartBrowser();
    void Stop();

    void Advertise(const sNetworkDiscoveryGame& pGame);
    void PollBrowser();

    const std::vector<sNetworkDiscoveryGame>& GetGames() const { return mGames; }
    uint32_t GetLastVersionMismatchTicks() const { return mLastVersionMismatchTicks; }
    uint32_t GetLastCompatibilityMismatchTicks() const { return mLastCompatibilityMismatchTicks; }
    uint32_t GetLastExpiredGameTicks() const { return mLastExpiredGameTicks; }

private:
    bool OpenSocket(uint16_t pBindPort, bool pBroadcast);
    void SetNonBlocking();
    void ExpireGames(uint32_t pNow);
    void UpsertGame(const sNetworkDiscoveryGame& pGame);

    SOCKET mSocket = INVALID_SOCKET;
    bool mBrowser = false;
    uint32_t mLastAdvertiseTicks = 0;
    uint32_t mLastVersionMismatchTicks = 0;
    uint32_t mLastCompatibilityMismatchTicks = 0;
    uint32_t mLastExpiredGameTicks = 0;
    std::vector<sNetworkDiscoveryGame> mGames;
};

#endif // OPENFODDER_ENABLE_NETWORK
