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

#include "stdafx.hpp"
#include "NetworkHubClient.hpp"

#ifdef OPENFODDER_ENABLE_NETWORK

#ifdef WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>
#  include <netdb.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <sys/time.h>
#  include <unistd.h>
#  define SOCKET int
#  define INVALID_SOCKET (-1)
#  define SOCKET_ERROR (-1)
#  define closesocket close
#endif

#ifdef OPENFODDER_HAVE_SODIUM
#  include <sodium.h>
#endif

#include <curl/curl.h>

#include <array>
#include <chrono>
#include <cstring>
#include <ctime>
#include <sstream>

// ----------------------------------------------------------------------------
// Local helpers
// ----------------------------------------------------------------------------

static std::vector<std::string> Hub_SplitWhitespace(const std::string& pText) {
    std::istringstream Stream(pText);
    std::vector<std::string> Tokens;
    std::string Token;
    while (Stream >> Token)
        Tokens.push_back(Token);
    return Tokens;
}

static std::vector<std::string> Hub_SplitChar(const std::string& pText, char pSeparator) {
    std::vector<std::string> Parts;
    std::string Current;
    for (char Ch : pText) {
        if (Ch == pSeparator) {
            Parts.push_back(Current);
            Current.clear();
        }
        else {
            Current.push_back(Ch);
        }
    }
    Parts.push_back(Current);
    return Parts;
}

static std::string Hub_ResponseValue(const std::string& pResponse, const std::string& pKey) {
    const std::string Prefix = pKey + "=";
    const auto Tokens = Hub_SplitWhitespace(pResponse);
    for (const std::string& Token : Tokens) {
        if (Token.find(Prefix) == 0)
            return Token.substr(Prefix.size());
    }
    return "";
}

static bool Hub_ParseUint16(const std::string& pText, uint16_t& pValue) {
    if (pText.empty())
        return false;

    int Value = 0;
    for (char Ch : pText) {
        if (Ch < '0' || Ch > '9')
            return false;
        Value = (Value * 10) + (Ch - '0');
        if (Value > 65535)
            return false;
    }

    pValue = (uint16_t)Value;
    return true;
}

static bool Hub_ParseUint8(const std::string& pText, uint8_t& pValue) {
    uint16_t Value = 0;
    if (!Hub_ParseUint16(pText, Value) || Value > 255)
        return false;
    pValue = (uint8_t)Value;
    return true;
}

static bool Hub_SameEndpoint(const struct sockaddr_in& pA, const struct sockaddr_in& pB) {
    return pA.sin_family == pB.sin_family &&
        pA.sin_port == pB.sin_port &&
        pA.sin_addr.s_addr == pB.sin_addr.s_addr;
}

static int64_t Hub_UnixNow() {
    return (int64_t)std::time(nullptr);
}

// ----------------------------------------------------------------------------
// Construction / lifecycle
// ----------------------------------------------------------------------------

cNetworkHubClient::cNetworkHubClient() {
#ifdef WIN32
    WSADATA WsaData;
    WSAStartup(MAKEWORD(2, 2), &WsaData);
#endif

#ifdef OPENFODDER_HAVE_SODIUM
    // sodium_init() is idempotent and thread-safe; calling it from each hub
    // client constructor is fine. Required before any libsodium primitive is
    // used (randombytes_buf for nonces, base64 helpers).
    if (sodium_init() < 0) {
        mLastError = "sodium_init failed";
    }
#endif
}

cNetworkHubClient::~cNetworkHubClient() {
#ifdef WIN32
    WSACleanup();
#endif
}

bool cNetworkHubClient::Configure(const std::string& pHubHost, uint16_t pHubPort) {
    mHubHost = pHubHost.size() ? pHubHost : NETWORK_HUB_DEFAULT_HOST;
    mHubPort = pHubPort ? pHubPort : NETWORK_HUB_DEFAULT_PORT;
    mCookieValid = false;
    mCookie.clear();
    return ResolveHub();
}

bool cNetworkHubClient::ResolveHub() {
    struct addrinfo Hints;
    memset(&Hints, 0, sizeof(Hints));
    Hints.ai_family = AF_INET;
    Hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo* Result = nullptr;
    const int Status = getaddrinfo(mHubHost.c_str(), nullptr, &Hints, &Result);
    if (Status != 0 || !Result) {
        mLastError = "unable to resolve hub host";
        return false;
    }

    char AddrText[INET_ADDRSTRLEN];
    memset(AddrText, 0, sizeof(AddrText));
    const auto* Addr = (const struct sockaddr_in*)Result->ai_addr;
    inet_ntop(AF_INET, &Addr->sin_addr, AddrText, sizeof(AddrText));
    mResolvedHost = AddrText;
    freeaddrinfo(Result);
    return true;
}

// ----------------------------------------------------------------------------
// HTTPS transport (one shot, single response).
//
// The deployed OFHUB/2 hub fronts the Fargate UDP control plane behind an
// AWS API Gateway + Lambda HTTPS proxy. The Lambda re-emits the OFHUB/2
// ASCII datagram as UDP toward whichever Fargate task owns the room and
// returns the reply. Everything outside the data-plane game socket goes
// over HTTPS POST to <host>/ofhub. (Spec § 1: "The serverless deployment
// additionally exposes https://<host>/ofhub HTTPS POST UDP-bridged
// control commands.") Self-hosted hubs that put the C++ relay binary
// directly on the public Internet are NOT supported by this client; that
// would need the prior UDP path back, gated on a cmake option.
// ----------------------------------------------------------------------------

namespace {

size_t Hub_CurlWriteToString(char* pData, size_t pSize, size_t pNmemb, void* pUser) {
    auto* Out = static_cast<std::string*>(pUser);
    const size_t Bytes = pSize * pNmemb;
    Out->append(pData, Bytes);
    return Bytes;
}

} // namespace

bool cNetworkHubClient::SendRaw(const std::string& pRequest, std::string& pResponse) {
    if (pRequest.size() > 1200) {
        mLastError = "hub request too large";
        return false;
    }

    pResponse.clear();

    CURL* Curl = curl_easy_init();
    if (!Curl) {
        mLastError = "curl_easy_init failed";
        return false;
    }

    const std::string Url = "https://" + mHubHost + "/ofhub";

    struct curl_slist* Headers = nullptr;
    Headers = curl_slist_append(Headers, "Content-Type: text/plain");
    Headers = curl_slist_append(Headers, "Accept: text/plain");

    curl_easy_setopt(Curl, CURLOPT_URL,             Url.c_str());
    curl_easy_setopt(Curl, CURLOPT_POST,            1L);
    curl_easy_setopt(Curl, CURLOPT_POSTFIELDS,      pRequest.c_str());
    curl_easy_setopt(Curl, CURLOPT_POSTFIELDSIZE,   (long)pRequest.size());
    curl_easy_setopt(Curl, CURLOPT_HTTPHEADER,      Headers);
    curl_easy_setopt(Curl, CURLOPT_FOLLOWLOCATION,  1L);
    curl_easy_setopt(Curl, CURLOPT_MAXREDIRS,       3L);
    curl_easy_setopt(Curl, CURLOPT_CONNECTTIMEOUT,  5L);
    curl_easy_setopt(Curl, CURLOPT_TIMEOUT,         10L);
    curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYPEER,  1L);
    curl_easy_setopt(Curl, CURLOPT_SSL_VERIFYHOST,  2L);
    curl_easy_setopt(Curl, CURLOPT_NOSIGNAL,        1L);
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION,   &Hub_CurlWriteToString);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA,       &pResponse);

    const CURLcode Rc = curl_easy_perform(Curl);
    long HttpStatus = 0;
    curl_easy_getinfo(Curl, CURLINFO_RESPONSE_CODE, &HttpStatus);

    curl_slist_free_all(Headers);
    curl_easy_cleanup(Curl);

    if (Rc != CURLE_OK) {
        std::ostringstream Os;
        Os << "hub HTTPS error " << (int)Rc << ": " << curl_easy_strerror(Rc);
        mLastError = Os.str();
        return false;
    }

    // The Lambda always returns 200 with the OFHUB/2 reply in the body
    // (ERR responses are also delivered as 200 OK with "ERR OFHUB/2 ..."
    // text, mirroring the UDP envelope shape).
    if (HttpStatus != 200) {
        std::ostringstream Os;
        Os << "hub HTTPS status " << HttpStatus;
        mLastError = Os.str();
        return false;
    }

    // Strip any trailing newline / whitespace the Lambda may have added.
    while (!pResponse.empty() &&
           (pResponse.back() == '\n' || pResponse.back() == '\r' ||
            pResponse.back() == ' '  || pResponse.back() == '\t')) {
        pResponse.pop_back();
    }

    if (pResponse.find("ERR OFHUB/2") == 0) {
        mLastError = pResponse;
        return false;
    }
    if (pResponse.find("OK OFHUB/2") != 0) {
        mLastError = "bad hub response: " + pResponse.substr(0, 80);
        return false;
    }

    mLastError.clear();
    return true;
}

bool cNetworkHubClient::SendCommand(const std::string& pCommand, const std::string& pTail,
                                    std::string& pResponse) {
    if (!EnsureCookie())
        return false;

    std::string Nonce;
    if (!MakeNonce(Nonce))
        return false;

    std::string Request = "OFHUB/2 ";
    Request += pCommand;
    Request += " cookie=";
    Request += mCookie;
    Request += " nonce=";
    Request += Nonce;
    Request += " ts=";
    Request += std::to_string(Hub_UnixNow());
    if (!pTail.empty()) {
        Request += ' ';
        Request += pTail;
    }

    if (SendRaw(Request, pResponse))
        return true;

    // If the cookie went stale (clock skew, secret rotation), drop it and force
    // a re-HELLO on the next call. The caller surfaces the error as-is.
    if (mLastError.find("BADCOOKIE") != std::string::npos ||
        mLastError.find("STALE") != std::string::npos) {
        mCookieValid = false;
        mCookie.clear();
    }
    return false;
}

// ----------------------------------------------------------------------------
// Cookie / nonce
// ----------------------------------------------------------------------------

bool cNetworkHubClient::EnsureCookie() {
    if (mCookieValid) {
        const auto Age = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - mCookieAcquiredAt).count();
        if (Age < NETWORK_HUB_COOKIE_LIFETIME_SECONDS)
            return true;
        mCookieValid = false;
        mCookie.clear();
    }
    return Hello();
}

bool cNetworkHubClient::MakeNonce(std::string& pOut) {
#ifdef OPENFODDER_HAVE_SODIUM
    unsigned char Random[16];
    randombytes_buf(Random, sizeof(Random));
    pOut = Base64UrlEncode(Random, sizeof(Random));
    return pOut.size() == 22;
#else
    (void)pOut;
    mLastError = "libsodium required for OFHUB/2 nonce generation";
    return false;
#endif
}

bool cNetworkHubClient::Hello() {
    std::string Response;
    if (!SendRaw("OFHUB/2 HELLO ver=2", Response))
        return false;

    const std::string Cookie = Hub_ResponseValue(Response, "val");
    if (Cookie.size() != 22) {
        mLastError = "hub HELLO returned malformed cookie";
        mCookieValid = false;
        return false;
    }

    mCookie = Cookie;
    mCookieAcquiredAt = std::chrono::steady_clock::now();
    mCookieValid = true;
    return true;
}

// ----------------------------------------------------------------------------
// Response parsing
// ----------------------------------------------------------------------------

bool cNetworkHubClient::ParseRoomResponse(const std::string& pResponse, sNetworkHubRoom& pRoom) {
    pRoom = sNetworkHubRoom();
    pRoom.mRelayHost = mResolvedHost;
    pRoom.mRoomCode = Hub_ResponseValue(pResponse, "id");
    pRoom.mHostIp = Hub_ResponseValue(pResponse, "host");

    if (!Hub_ParseUint16(Hub_ResponseValue(pResponse, "port"), pRoom.mRelayPort))
        return false;
    Hub_ParseUint8(Hub_ResponseValue(pResponse, "capacity"), pRoom.mMaxPlayers);
    Hub_ParseUint8(Hub_ResponseValue(pResponse, "peers"), pRoom.mCurrentPlayers);

    const std::string SessionKeyB64 = Hub_ResponseValue(pResponse, "session_key");
    std::vector<unsigned char> SessionKeyBytes;
    if (!Base64UrlDecode(SessionKeyB64, SessionKeyBytes) || SessionKeyBytes.size() != 32) {
        mLastError = "hub returned malformed session_key";
        return false;
    }
    std::memcpy(pRoom.mSessionKey.data(), SessionKeyBytes.data(), 32);

    PercentDecode(Hub_ResponseValue(pResponse, "name"), pRoom.mMetadata.mGameName);
    PercentDecode(Hub_ResponseValue(pResponse, "mode"), pRoom.mMetadata.mGameMode);
    PercentDecode(Hub_ResponseValue(pResponse, "map"), pRoom.mMetadata.mMapName);
    PercentDecode(Hub_ResponseValue(pResponse, "options"), pRoom.mMetadata.mOptions);
    PercentDecode(Hub_ResponseValue(pResponse, "version"), pRoom.mMetadata.mVersion);
    return pRoom.mRoomCode.size() && pRoom.mRelayPort;
}

bool cNetworkHubClient::ParseListEntry(const std::string& pValue, sNetworkHubGame& pGame) {
    // Per spec § 4.4.5:
    //   r0=<id>,<host>,<port>,<peers>,<capacity>,<%-name>,<%-mode>,
    //      <%-map>,<%-opts>,<%-ver>
    const auto Parts = Hub_SplitChar(pValue, ',');
    if (Parts.size() < 10)
        return false;

    pGame = sNetworkHubGame();
    pGame.mRoomCode = Parts[0];
    pGame.mRelayHost = Parts[1].size() ? Parts[1] : mResolvedHost;
    if (!Hub_ParseUint16(Parts[2], pGame.mRelayPort))
        return false;
    Hub_ParseUint8(Parts[3], pGame.mCurrentPlayers);
    Hub_ParseUint8(Parts[4], pGame.mMaxPlayers);
    PercentDecode(Parts[5], pGame.mMetadata.mGameName);
    PercentDecode(Parts[6], pGame.mMetadata.mGameMode);
    PercentDecode(Parts[7], pGame.mMetadata.mMapName);
    PercentDecode(Parts[8], pGame.mMetadata.mOptions);
    PercentDecode(Parts[9], pGame.mMetadata.mVersion);
    return pGame.mRoomCode.size() && pGame.mRelayPort;
}

// ----------------------------------------------------------------------------
// Public API — OFHUB/2 commands
// ----------------------------------------------------------------------------

bool cNetworkHubClient::ListAnonymous(std::vector<sNetworkHubGame>& pOut) {
    pOut.clear();

    std::string Response;
    if (!SendCommand("LIST", "", Response))
        return false;

    const auto Tokens = Hub_SplitWhitespace(Response);
    for (const std::string& Token : Tokens) {
        if (Token.size() < 4 || Token[0] != 'r')
            continue;
        const size_t Equals = Token.find('=');
        if (Equals == std::string::npos)
            continue;
        // Only entries of the form rN=... where N is digit-only.
        bool DigitsOnly = Equals > 1;
        for (size_t i = 1; i < Equals && DigitsOnly; ++i) {
            if (Token[i] < '0' || Token[i] > '9')
                DigitsOnly = false;
        }
        if (!DigitsOnly)
            continue;

        sNetworkHubGame Game;
        if (ParseListEntry(Token.substr(Equals + 1), Game))
            pOut.push_back(Game);
    }
    return true;
}

bool cNetworkHubClient::JoinAnonymous(const std::string& pRoomCode, sNetworkHubRoom& pOut) {
    std::string Response;
    if (!SendCommand("JOIN", "id=" + pRoomCode, Response))
        return false;
    return ParseRoomResponse(Response, pOut);
}

bool cNetworkHubClient::CreateAuth(const sHubAuthToken& pAuth, uint8_t pCapacity,
                                   const sNetworkHubMetadata& pMeta, bool pListed,
                                   sNetworkHubRoom& pOut) {
    if (pAuth.mJwt.empty()) {
        mLastError = "CREATE requires bearer token";
        return false;
    }

    std::string Tail;
    Tail += "bearer=" + pAuth.mJwt;
    Tail += " capacity=" + std::to_string((int)pCapacity);
    Tail += pListed ? " listed=1" : " listed=0";
    Tail += " name=" + PercentEncode(pMeta.mGameName);
    Tail += " mode=" + PercentEncode(pMeta.mGameMode);
    Tail += " map=" + PercentEncode(pMeta.mMapName);
    Tail += " options=" + PercentEncode(pMeta.mOptions);
    Tail += " version=" + PercentEncode(pMeta.mVersion);

    std::string Response;
    if (!SendCommand("CREATE", Tail, Response))
        return false;
    return ParseRoomResponse(Response, pOut);
}

bool cNetworkHubClient::UpdateAuth(const sHubAuthToken& pAuth, const std::string& pRoomCode,
                                   const sNetworkHubMetadata& pMeta, bool pListed) {
    if (pAuth.mJwt.empty()) {
        mLastError = "UPDATE requires bearer token";
        return false;
    }

    std::string Tail;
    Tail += "bearer=" + pAuth.mJwt;
    Tail += " id=" + pRoomCode;
    Tail += pListed ? " listed=1" : " listed=0";
    Tail += " name=" + PercentEncode(pMeta.mGameName);
    Tail += " mode=" + PercentEncode(pMeta.mGameMode);
    Tail += " map=" + PercentEncode(pMeta.mMapName);
    Tail += " options=" + PercentEncode(pMeta.mOptions);
    Tail += " version=" + PercentEncode(pMeta.mVersion);

    std::string Response;
    return SendCommand("UPDATE", Tail, Response);
}

bool cNetworkHubClient::HeartbeatAnonymous(const std::string& pRoomCode) {
    std::string Response;
    return SendCommand("HEARTBEAT", "id=" + pRoomCode, Response);
}

bool cNetworkHubClient::HeartbeatHost(const sHubAuthToken& pAuth, const std::string& pRoomCode) {
    if (pAuth.mJwt.empty()) {
        mLastError = "host HEARTBEAT requires bearer token";
        return false;
    }

    std::string Tail = "bearer=" + pAuth.mJwt + " id=" + pRoomCode;
    std::string Response;
    return SendCommand("HEARTBEAT", Tail, Response);
}

// ----------------------------------------------------------------------------
// Encoding helpers
// ----------------------------------------------------------------------------

std::string cNetworkHubClient::PercentEncode(const std::string& pValue) {
    static const char Hex[] = "0123456789ABCDEF";
    std::string Encoded;
    for (unsigned char Ch : pValue) {
        if ((Ch >= 'A' && Ch <= 'Z') || (Ch >= 'a' && Ch <= 'z') || (Ch >= '0' && Ch <= '9') ||
            Ch == '-' || Ch == '_' || Ch == '.' || Ch == '~') {
            Encoded.push_back((char)Ch);
        }
        else {
            Encoded.push_back('%');
            Encoded.push_back(Hex[(Ch >> 4) & 0x0F]);
            Encoded.push_back(Hex[Ch & 0x0F]);
        }
    }
    return Encoded;
}

bool cNetworkHubClient::PercentDecode(const std::string& pValue, std::string& pDecoded) {
    pDecoded.clear();
    for (size_t Index = 0; Index < pValue.size(); ++Index) {
        const char Ch = pValue[Index];
        if (Ch != '%') {
            pDecoded.push_back(Ch);
            continue;
        }
        if (Index + 2 >= pValue.size())
            return false;

        auto HexValue = [](char Value) -> int {
            if (Value >= '0' && Value <= '9') return Value - '0';
            if (Value >= 'A' && Value <= 'F') return 10 + (Value - 'A');
            if (Value >= 'a' && Value <= 'f') return 10 + (Value - 'a');
            return -1;
        };

        const int High = HexValue(pValue[Index + 1]);
        const int Low = HexValue(pValue[Index + 2]);
        if (High < 0 || Low < 0)
            return false;

        pDecoded.push_back((char)((High << 4) | Low));
        Index += 2;
    }
    return true;
}

std::string cNetworkHubClient::Base64UrlEncode(const unsigned char* pData, size_t pLength) {
    // Unpadded base64url. We emit our own table rather than rely on libsodium's
    // sodium_bin2base64 (which would also work but pulls in a bigger surface
    // and forces a specific padding variant per call).
    static const char Alphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    std::string Out;
    Out.reserve(((pLength + 2) / 3) * 4);

    size_t i = 0;
    while (i + 3 <= pLength) {
        const uint32_t Triplet = ((uint32_t)pData[i] << 16) |
                                 ((uint32_t)pData[i + 1] << 8) |
                                 (uint32_t)pData[i + 2];
        Out.push_back(Alphabet[(Triplet >> 18) & 0x3F]);
        Out.push_back(Alphabet[(Triplet >> 12) & 0x3F]);
        Out.push_back(Alphabet[(Triplet >> 6) & 0x3F]);
        Out.push_back(Alphabet[Triplet & 0x3F]);
        i += 3;
    }

    const size_t Remain = pLength - i;
    if (Remain == 1) {
        const uint32_t Triplet = (uint32_t)pData[i] << 16;
        Out.push_back(Alphabet[(Triplet >> 18) & 0x3F]);
        Out.push_back(Alphabet[(Triplet >> 12) & 0x3F]);
    }
    else if (Remain == 2) {
        const uint32_t Triplet = ((uint32_t)pData[i] << 16) | ((uint32_t)pData[i + 1] << 8);
        Out.push_back(Alphabet[(Triplet >> 18) & 0x3F]);
        Out.push_back(Alphabet[(Triplet >> 12) & 0x3F]);
        Out.push_back(Alphabet[(Triplet >> 6) & 0x3F]);
    }
    return Out;
}

bool cNetworkHubClient::Base64UrlDecode(const std::string& pInput, std::vector<unsigned char>& pOut) {
    pOut.clear();
    pOut.reserve((pInput.size() * 3) / 4);

    auto DecodeChar = [](char Ch) -> int {
        if (Ch >= 'A' && Ch <= 'Z') return Ch - 'A';
        if (Ch >= 'a' && Ch <= 'z') return 26 + (Ch - 'a');
        if (Ch >= '0' && Ch <= '9') return 52 + (Ch - '0');
        if (Ch == '-') return 62;
        if (Ch == '_') return 63;
        return -1;
    };

    uint32_t Buffer = 0;
    int BitCount = 0;
    for (char Ch : pInput) {
        if (Ch == '=')                     // tolerate, but unpadded is the norm
            break;
        const int Decoded = DecodeChar(Ch);
        if (Decoded < 0)
            return false;
        Buffer = (Buffer << 6) | (uint32_t)Decoded;
        BitCount += 6;
        if (BitCount >= 8) {
            BitCount -= 8;
            pOut.push_back((unsigned char)((Buffer >> BitCount) & 0xFF));
        }
    }

    // Any bits left in the buffer must be zero (canonical encoding).
    if (BitCount > 0 && (Buffer & ((1u << BitCount) - 1u)) != 0)
        return false;

    return true;
}

#endif // OPENFODDER_ENABLE_NETWORK
