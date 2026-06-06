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

#include <array>
#include <sstream>

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

cNetworkHubClient::cNetworkHubClient() {
#ifdef WIN32
    WSADATA WsaData;
    WSAStartup(MAKEWORD(2, 2), &WsaData);
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

bool cNetworkHubClient::SendRequest(const std::string& pRequest, std::string& pResponse) {
    if (mResolvedHost.empty() && !ResolveHub())
        return false;
    if (pRequest.size() > 1200) {
        mLastError = "hub request too large";
        return false;
    }

    SOCKET Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (Socket == INVALID_SOCKET) {
        mLastError = "hub socket failed";
        return false;
    }

#ifdef WIN32
    DWORD TimeoutMs = 1200;
    setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&TimeoutMs, sizeof(TimeoutMs));
#else
    struct timeval Timeout;
    Timeout.tv_sec = 1;
    Timeout.tv_usec = 200000;
    setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&Timeout, sizeof(Timeout));
#endif

    struct sockaddr_in RemoteAddr;
    memset(&RemoteAddr, 0, sizeof(RemoteAddr));
    RemoteAddr.sin_family = AF_INET;
    RemoteAddr.sin_port = htons(mHubPort);
    if (inet_pton(AF_INET, mResolvedHost.c_str(), &RemoteAddr.sin_addr) != 1) {
        closesocket(Socket);
        mLastError = "hub address invalid";
        return false;
    }

    const int Sent = sendto(Socket, pRequest.c_str(), (int)pRequest.size(), 0,
                            (struct sockaddr*)&RemoteAddr, sizeof(RemoteAddr));
    if (Sent == SOCKET_ERROR) {
        closesocket(Socket);
        mLastError = "hub send failed";
        return false;
    }

    std::array<char, 1400> Buffer;
    struct sockaddr_in FromAddr;
#ifdef WIN32
    int FromLen = sizeof(FromAddr);
#else
    socklen_t FromLen = sizeof(FromAddr);
#endif
    const int Received = recvfrom(Socket, Buffer.data(), (int)Buffer.size(), 0,
                                  (struct sockaddr*)&FromAddr, &FromLen);
    closesocket(Socket);

    if (Received <= 0) {
        mLastError = "hub did not respond";
        return false;
    }
    if (!Hub_SameEndpoint(FromAddr, RemoteAddr)) {
        mLastError = "hub response source mismatch";
        return false;
    }

    pResponse.assign(Buffer.data(), (size_t)Received);
    if (pResponse.find("ERR OFHUB/1") == 0) {
        mLastError = pResponse;
        return false;
    }
    if (pResponse.find("OK OFHUB/1") != 0) {
        mLastError = "bad hub response";
        return false;
    }

    mLastError.clear();
    return true;
}

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

bool cNetworkHubClient::ParseRoomResponse(const std::string& pResponse, sNetworkHubRoom& pRoom) {
    pRoom = sNetworkHubRoom();
    pRoom.mRelayHost = mResolvedHost;
    pRoom.mRoomCode = Hub_ResponseValue(pResponse, "room");
    pRoom.mToken = Hub_ResponseValue(pResponse, "token");
    if (!Hub_ParseUint16(Hub_ResponseValue(pResponse, "port"), pRoom.mRelayPort))
        return false;
    Hub_ParseUint8(Hub_ResponseValue(pResponse, "capacity"), pRoom.mMaxPlayers);
    Hub_ParseUint8(Hub_ResponseValue(pResponse, "peers"), pRoom.mCurrentPlayers);

    PercentDecode(Hub_ResponseValue(pResponse, "name"), pRoom.mMetadata.mGameName);
    PercentDecode(Hub_ResponseValue(pResponse, "mode"), pRoom.mMetadata.mGameMode);
    PercentDecode(Hub_ResponseValue(pResponse, "map"), pRoom.mMetadata.mMapName);
    PercentDecode(Hub_ResponseValue(pResponse, "options"), pRoom.mMetadata.mOptions);
    PercentDecode(Hub_ResponseValue(pResponse, "version"), pRoom.mMetadata.mVersion);
    return pRoom.mRoomCode.size() && pRoom.mRelayPort && pRoom.mToken.size();
}

bool cNetworkHubClient::ParseListEntry(const std::string& pValue, sNetworkHubGame& pGame) {
    const auto Parts = Hub_SplitChar(pValue, ',');
    if (Parts.size() < 9)
        return false;

    pGame = sNetworkHubGame();
    pGame.mRelayHost = mResolvedHost;
    pGame.mRoomCode = Parts[0];
    if (!Hub_ParseUint16(Parts[1], pGame.mRelayPort))
        return false;
    Hub_ParseUint8(Parts[2], pGame.mCurrentPlayers);
    Hub_ParseUint8(Parts[3], pGame.mMaxPlayers);
    PercentDecode(Parts[4], pGame.mMetadata.mGameName);
    PercentDecode(Parts[5], pGame.mMetadata.mGameMode);
    PercentDecode(Parts[6], pGame.mMetadata.mMapName);
    PercentDecode(Parts[7], pGame.mMetadata.mOptions);
    PercentDecode(Parts[8], pGame.mMetadata.mVersion);
    return pGame.mRoomCode.size() && pGame.mRelayPort;
}

bool cNetworkHubClient::List(std::vector<sNetworkHubGame>& pGames) {
    pGames.clear();
    std::string Response;
    if (!SendRequest("OFHUB/1 LIST", Response))
        return false;

    const auto Tokens = Hub_SplitWhitespace(Response);
    for (const std::string& Token : Tokens) {
        if (Token.size() < 4 || Token[0] != 'r')
            continue;
        const size_t Equals = Token.find('=');
        if (Equals == std::string::npos)
            continue;

        sNetworkHubGame Game;
        if (ParseListEntry(Token.substr(Equals + 1), Game))
            pGames.push_back(Game);
    }
    return true;
}

bool cNetworkHubClient::Create(uint8_t pCapacity, const sNetworkHubMetadata& pMetadata, bool pListed, sNetworkHubRoom& pRoom) {
    std::string Request = "OFHUB/1 CREATE capacity=" + std::to_string((int)pCapacity);
    Request += pListed ? " listed=1" : " listed=0";
    Request += " name=" + PercentEncode(pMetadata.mGameName);
    Request += " mode=" + PercentEncode(pMetadata.mGameMode);
    Request += " map=" + PercentEncode(pMetadata.mMapName);
    Request += " options=" + PercentEncode(pMetadata.mOptions);
    Request += " version=" + PercentEncode(pMetadata.mVersion);

    std::string Response;
    return SendRequest(Request, Response) && ParseRoomResponse(Response, pRoom);
}

bool cNetworkHubClient::Join(const std::string& pRoomCode, sNetworkHubRoom& pRoom) {
    std::string Response;
    return SendRequest("OFHUB/1 JOIN room=" + pRoomCode, Response) && ParseRoomResponse(Response, pRoom);
}

bool cNetworkHubClient::Update(const std::string& pRoomCode, const std::string& pToken, const sNetworkHubMetadata& pMetadata, bool pListed) {
    std::string Request = "OFHUB/1 UPDATE room=" + pRoomCode + " token=" + pToken;
    Request += pListed ? " listed=1" : " listed=0";
    Request += " name=" + PercentEncode(pMetadata.mGameName);
    Request += " mode=" + PercentEncode(pMetadata.mGameMode);
    Request += " map=" + PercentEncode(pMetadata.mMapName);
    Request += " options=" + PercentEncode(pMetadata.mOptions);
    Request += " version=" + PercentEncode(pMetadata.mVersion);

    std::string Response;
    return SendRequest(Request, Response);
}

bool cNetworkHubClient::Heartbeat(const std::string& pRoomCode, const std::string& pToken) {
    std::string Response;
    return SendRequest("OFHUB/1 HEARTBEAT room=" + pRoomCode + " token=" + pToken, Response);
}

#endif // OPENFODDER_ENABLE_NETWORK
