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
 */

#pragma once

#ifdef OPENFODDER_ENABLE_NETWORK

#include <cstdint>
#include <string>
#include <vector>

static const uint16_t NETWORK_HUB_DEFAULT_PORT = 27770;
static const char* const NETWORK_HUB_DEFAULT_HOST = "hub.openfodder.com";

struct sNetworkHubMetadata {
    std::string mGameName;
    std::string mGameMode;
    std::string mMapName;
    std::string mOptions;
    std::string mVersion;
};

struct sNetworkHubGame {
    std::string mRoomCode;
    std::string mRelayHost;
    uint16_t    mRelayPort = 0;
    uint8_t     mCurrentPlayers = 0;
    uint8_t     mMaxPlayers = 0;
    sNetworkHubMetadata mMetadata;
};

struct sNetworkHubRoom {
    std::string mRoomCode;
    std::string mRelayHost;
    uint16_t    mRelayPort = 0;
    std::string mToken;
    uint8_t     mCurrentPlayers = 0;
    uint8_t     mMaxPlayers = 0;
    sNetworkHubMetadata mMetadata;
};

class cNetworkHubClient {
public:
    cNetworkHubClient();
    ~cNetworkHubClient();

    bool Configure(const std::string& pHubHost, uint16_t pHubPort);

    bool List(std::vector<sNetworkHubGame>& pGames);
    bool Create(uint8_t pCapacity, const sNetworkHubMetadata& pMetadata, bool pListed, sNetworkHubRoom& pRoom);
    bool Join(const std::string& pRoomCode, sNetworkHubRoom& pRoom);
    bool Update(const std::string& pRoomCode, const std::string& pToken, const sNetworkHubMetadata& pMetadata, bool pListed);
    bool Heartbeat(const std::string& pRoomCode, const std::string& pToken);

    const std::string& GetHubHost() const { return mHubHost; }
    uint16_t GetHubPort() const { return mHubPort; }
    const std::string& GetResolvedHost() const { return mResolvedHost; }
    const std::string& GetLastError() const { return mLastError; }

    static std::string PercentEncode(const std::string& pValue);
    static bool PercentDecode(const std::string& pValue, std::string& pDecoded);

private:
    bool SendRequest(const std::string& pRequest, std::string& pResponse);
    bool ResolveHub();
    bool ParseRoomResponse(const std::string& pResponse, sNetworkHubRoom& pRoom);
    bool ParseListEntry(const std::string& pValue, sNetworkHubGame& pGame);

    std::string mHubHost = NETWORK_HUB_DEFAULT_HOST;
    std::string mResolvedHost;
    uint16_t    mHubPort = NETWORK_HUB_DEFAULT_PORT;
    std::string mLastError;
};

#endif // OPENFODDER_ENABLE_NETWORK
