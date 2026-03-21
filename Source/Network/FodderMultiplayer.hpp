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

class cMultiplayerMenu;

class cFodderMultiplayer : public cFodder {
public:
    cFodderMultiplayer(std::shared_ptr<cWindow> pWindow);
    virtual ~cFodderMultiplayer();

    // --- Overrides ---
    // These replace the #ifdef OPENFODDER_ENABLE_NETWORK branches
    // scattered through cFodder with clean virtual dispatch.
    void    Start() override;
    int16   Mission_Loop() override;
    int16   Phase_Loop() override;
    void    Phase_Prepare_Network() override;

    // --- Network-specific functions ---
    // (moved from cFodder — previously in Fodder_Network.cpp)

    bool            Network_Start();
    void            Network_Stop();
    void            Network_RedistributeSquads();
    int16           Network_Tick();

    void            Network_GatherLocalInput(sNetworkInput& out);
    void            Network_ApplyInputs(const sNetworkInput inputs[NETWORK_MAX_PLAYERS]);
    bool            Network_AdvanceFrame(const sNetworkInput inputs[NETWORK_MAX_PLAYERS]);

    void            Network_GUI_Sidebar_Draw();

    int16           Network_Recruit_Show();
    int16           Network_Briefing_Show();
    void            Network_Briefing_ReadySync();

    void            Network_Sidebar_ForceSquadIcons();
    void            Network_Draw_WaitingForPlayer();
    void            Network_DrawP2Cursor();

    bool            Network_SaveState(uint8_t** buffer, int* len, int* checksum);
    bool            Network_LoadState(const uint8_t* buffer, int len);
    void            Network_FreeBuffer(void* buffer);

    void            Network_CameraSave(sNetCameraState& out);
    void            Network_CameraRestore(const sNetCameraState& in);

    // Multiplayer menu / lobby
    bool            Multiplayer_Menu_Run();
    void            Lobby_CampaignSelection();

    // Multiplayer menu object (parallel to mOptionsMenu on cFodder)
    std::unique_ptr<cMultiplayerMenu> mMultiplayerMenu;

    // UDP lobby for campaign selection sync
    std::unique_ptr<cNetworkLobby> mLobby;
};

#endif // OPENFODDER_ENABLE_NETWORK
