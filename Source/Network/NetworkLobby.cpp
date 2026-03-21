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

cNetworkLobby::cNetworkLobby() {
    memset(&mRemoteAddr, 0, sizeof(mRemoteAddr));
}

cNetworkLobby::~cNetworkLobby() {
    Stop();
}

bool cNetworkLobby::Start(uint16_t pLocalPort, const std::string& pRemoteHost, uint16_t pRemotePort, bool pIsHost) {
    Stop();
    mIsHost = pIsHost;

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
    mLocalSelection = 0;
    mLocalCampaign.clear();
    mLocalReady = false;
    mLocalStarted = false;

    g_Debugger->Notice("[Lobby] Started on port " + std::to_string(pLocalPort) +
                       " -> " + pRemoteHost + ":" + std::to_string(pRemotePort) +
                       (pIsHost ? " (HOST)" : " (JOIN)"));
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

void cNetworkLobby::SetReady(bool pReady) {
    mLocalReady = pReady;
}

void cNetworkLobby::SetStarted() {
    mLocalStarted = true;
}

void cNetworkLobby::Send() {
    sLobbyPacket pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.magic     = sLobbyPacket::MAGIC;
    pkt.type      = sLobbyPacket::LOBBY_STATE;
    pkt.ready     = mLocalReady ? 1 : 0;
    pkt.selection = mLocalSelection;
    pkt.started   = mLocalStarted ? 1 : 0;
    pkt.connected = 1;

    // Copy campaign name (truncate if too long)
    size_t len = mLocalCampaign.size();
    if (len >= sizeof(pkt.campaign))
        len = sizeof(pkt.campaign) - 1;
    memcpy(pkt.campaign, mLocalCampaign.c_str(), len);
    pkt.campaign[len] = '\0';

    sendto(mSocket, (const char*)&pkt, sizeof(pkt), 0,
           (struct sockaddr*)&mRemoteAddr, sizeof(mRemoteAddr));
}

void cNetworkLobby::Receive() {
    sLobbyPacket pkt;
    struct sockaddr_in fromAddr;
#ifdef WIN32
    int fromLen = sizeof(fromAddr);
#else
    socklen_t fromLen = sizeof(fromAddr);
#endif

    // Drain all pending packets, keep the latest
    bool gotPacket = false;
    for (;;) {
        int n = recvfrom(mSocket, (char*)&pkt, sizeof(pkt), 0,
                         (struct sockaddr*)&fromAddr, &fromLen);
        if (n != sizeof(pkt))
            break;
        if (pkt.magic != sLobbyPacket::MAGIC)
            continue;
        gotPacket = true;
    }

    if (!gotPacket)
        return;

    mRemoteConnected = (pkt.connected != 0);
    mRemoteReady     = (pkt.ready != 0);
    mRemoteStarted   = (pkt.started != 0);
    mRemoteSelection = pkt.selection;

    pkt.campaign[sizeof(pkt.campaign) - 1] = '\0';
    mRemoteCampaign = pkt.campaign;
}

#endif // OPENFODDER_ENABLE_NETWORK
