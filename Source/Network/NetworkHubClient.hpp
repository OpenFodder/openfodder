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

#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "HubAuth.hpp"

static const uint16_t NETWORK_HUB_DEFAULT_PORT = 27770;
static const char* const NETWORK_HUB_DEFAULT_HOST = "hub.openfodder.com";

// Cookie lives ~10 minutes server-side (±10 min tolerance per spec § 4.1).
// We refresh after 9 minutes so we never race the boundary.
static const int NETWORK_HUB_COOKIE_LIFETIME_SECONDS = 9 * 60;

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

// Verified-host bearer credential (compact JWT) issued by the hub OAuth flow.
// Defined in HubAuth.hpp as sHubAuthToken { mJwt, mExpiry, mSubject } and shared
// between the OAuth pairing layer (cHubAuth) and the hub-protocol client below.

struct sNetworkHubRoom {
    std::string                     mRoomCode;
    std::string                     mRelayHost;     // resolved hub IP (anonymous mode)
    std::string                     mHostIp;        // host=<ipv4> from CREATEOK/JOINOK if present
    uint16_t                        mRelayPort = 0;
    std::array<unsigned char, 32>   mSessionKey{};  // base64url-decoded session_key
    uint8_t                         mCurrentPlayers = 0;
    uint8_t                         mMaxPlayers = 0;
    sNetworkHubMetadata             mMetadata;
};

class cNetworkHubClient {
public:
    cNetworkHubClient();
    ~cNetworkHubClient();

    bool Configure(const std::string& pHubHost, uint16_t pHubPort);

    // Performs HELLO+COOKIE round-trip and caches the cookie for ~9 minutes.
    // Must succeed before any other command; subsequent calls below auto-refresh
    // the cookie when it is close to expiry.
    bool Hello();

    // Anonymous browse + join. Both require a valid cookie (no bearer).
    bool ListAnonymous(std::vector<sNetworkHubGame>& pOut);
    bool JoinAnonymous(const std::string& pRoomCode, sNetworkHubRoom& pOut);

    // Verified-host operations. Carry bearer=<jwt> in addition to cookie/nonce/ts.
    bool CreateAuth(const sHubAuthToken& pAuth, uint8_t pCapacity,
                    const sNetworkHubMetadata& pMeta, bool pListed,
                    sNetworkHubRoom& pOut);
    bool UpdateAuth(const sHubAuthToken& pAuth, const std::string& pRoomCode,
                    const sNetworkHubMetadata& pMeta, bool pListed);

    // Heartbeats. Anonymous flavour is for joiners; the host flavour carries bearer=.
    bool HeartbeatAnonymous(const std::string& pRoomCode);
    bool HeartbeatHost(const sHubAuthToken& pAuth, const std::string& pRoomCode);

    const std::string& GetHubHost() const { return mHubHost; }
    uint16_t GetHubPort() const { return mHubPort; }
    const std::string& GetResolvedHost() const { return mResolvedHost; }
    const std::string& GetLastError() const { return mLastError; }

    static std::string PercentEncode(const std::string& pValue);
    static bool PercentDecode(const std::string& pValue, std::string& pDecoded);

    // Base64url encode/decode without padding (matches the spec's cookie /
    // nonce / session_key shapes).
    static std::string Base64UrlEncode(const unsigned char* pData, size_t pLength);
    static bool Base64UrlDecode(const std::string& pInput, std::vector<unsigned char>& pOut);

private:
    bool ResolveHub();
    bool SendRaw(const std::string& pRequest, std::string& pResponse);
    bool SendCommand(const std::string& pCommand, const std::string& pTail,
                     std::string& pResponse);
    bool EnsureCookie();
    bool MakeNonce(std::string& pOut);
    bool ParseRoomResponse(const std::string& pResponse, sNetworkHubRoom& pRoom);
    bool ParseListEntry(const std::string& pValue, sNetworkHubGame& pGame);

    std::string mHubHost = NETWORK_HUB_DEFAULT_HOST;
    std::string mResolvedHost;
    uint16_t    mHubPort = NETWORK_HUB_DEFAULT_PORT;
    std::string mLastError;

    std::string mCookie;                                // 22-char base64url
    std::chrono::steady_clock::time_point mCookieAcquiredAt{};
    bool        mCookieValid = false;
};

#endif // OPENFODDER_ENABLE_NETWORK
