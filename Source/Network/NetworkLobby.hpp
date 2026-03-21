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

#include <string>
#include <cstdint>
#include <cstring>

// Lobby packet exchanged via UDP side-channel during campaign selection.
// Sent periodically (~every frame) so both players stay in sync.
struct sLobbyPacket {
    static const uint32_t MAGIC = 0x4C4F4259; // "LOBY"

    uint32_t magic;
    uint8_t  type;
    uint8_t  ready;          // 1 = this player is ready
    int16_t  selection;      // campaign list index the host is viewing
    uint8_t  started;        // 1 = host says GO, load this campaign
    uint8_t  connected;      // 1 = peer has received at least one packet
    uint8_t  padding[2];
    char     campaign[64];   // campaign name string (null-terminated)

    enum eType : uint8_t {
        LOBBY_STATE = 0x01,
    };
};

static_assert(sizeof(sLobbyPacket) == 76, "sLobbyPacket must be 76 bytes");

class cNetworkLobby {
public:
    cNetworkLobby();
    ~cNetworkLobby();

    bool Start(uint16_t pLocalPort, const std::string& pRemoteHost, uint16_t pRemotePort, bool pIsHost);
    void Stop();

    // Call once per frame to send/receive
    void Poll();

    // Local state setters (host sets selection, either player sets ready)
    void SetSelection(int16_t pIndex, const std::string& pCampaign);
    void SetReady(bool pReady);
    void SetStarted();

    // Remote state getters
    bool        IsConnected() const { return mRemoteConnected; }
    bool        IsRemoteReady() const { return mRemoteReady; }
    bool        IsRemoteStarted() const { return mRemoteStarted; }
    int16_t     GetRemoteSelection() const { return mRemoteSelection; }
    std::string GetRemoteCampaign() const { return mRemoteCampaign; }
    bool        IsHost() const { return mIsHost; }
    bool        IsRunning() const { return mSocket != INVALID_SOCKET; }

private:
    void Send();
    void Receive();

    SOCKET              mSocket = INVALID_SOCKET;
    struct sockaddr_in  mRemoteAddr;
    bool                mIsHost = false;

    // Local state (what we send)
    int16_t     mLocalSelection = 0;
    std::string mLocalCampaign;
    bool        mLocalReady = false;
    bool        mLocalStarted = false;

    // Remote state (what we received)
    bool        mRemoteConnected = false;
    bool        mRemoteReady = false;
    bool        mRemoteStarted = false;
    int16_t     mRemoteSelection = 0;
    std::string mRemoteCampaign;
};

#endif // OPENFODDER_ENABLE_NETWORK
