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

#include "HubAuth.hpp"

class cMultiplayerMenu;
class cNetworkHubClient;

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
    void    Phase_Show_Complete() override;

    // --- Network-specific functions ---
    // (moved from cFodder — previously in Fodder_Network.cpp)

    bool            Network_Start();
    void            Network_Stop();
    void            Network_RedistributeSquads();
    int16           Network_Tick();

    void            Network_GatherLocalInput(sNetworkInput& out);
    void            Network_ApplyInputs(const sNetworkInput inputs[NETWORK_MAX_PLAYERS]);
    bool            Network_AdvanceFrame(const sNetworkInput inputs[NETWORK_MAX_PLAYERS]);
    int16           Sprite_Troop_Dies(sSprite* pSprite) override;
    bool            Sprite_UseNetworkHostilityRules() const override;
    void            Sprite_RecordDamage(sSprite* pDamageSource, sSprite* pTarget) override;
    bool            Sprite_CanDamageTarget(const sSprite* pDamageSource, const sSprite* pTarget) const override;
    bool            Sprite_CanTargetSprite(const sSprite* pActor, const sSprite* pTarget) const override;
    bool            Sprite_CanVehicleDamageTarget(const sSprite* pVehicle, const sSprite* pTarget) const override;
    bool            Sprite_ShouldDamagePlayerInRegion(const sSprite* pDamageSource, const sSprite* pTarget) const override;
    bool            Sprite_AreHostile(const sSprite* pLeft, const sSprite* pRight) const override;
    bool            Sprite_IsIndependentlyControlledSquadMember(const sSprite* pSprite) const override;
    void            Sprite_GetPlayerRankContext(sSprite* pSprite, int16& pSquad, sSprite*& pLeader) override;
    void            Sprite_UpdatePlayerRankLeader(sSprite* pSprite, sSprite* pLeader) override;
    void            Sprite_GetMouseDirectionTarget(sSprite* pSprite, int16& pTargetX, int16& pTargetY) override;
    bool            Sprite_TryHandleSharedPickupBox(sSprite* pSprite, bool pRocketBox) override;
    bool            Sprite_ShouldUseSelectedSquadWeapon(const sSprite* pSprite) const override;
    void            Sprite_ClearSelectedSquadWeaponUse(const sSprite* pSprite) override;
    int16           Sprite_Find_Hostile_By_Types(sSprite* pSprite, int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData10, sSprite*& pData28) override;
    int16           Network_GetSpriteOwner(const sSprite* pSprite) const;
    int16           Network_GetSpriteTeam(const sSprite* pSprite) const;
    bool            Network_AreHostile(const sSprite* pLeft, const sSprite* pRight) const;
    void            Network_ResetSquadOwnership();
    bool            Network_UsesPrivateSplitSquads() const;
    int16           Network_GetSquadOwner(int16 pSquad) const;
    bool            Network_PlayerOwnsSquad(int16 pPlayer, int16 pSquad) const;
    int16           Network_GetPlayerPrimarySquad(int16 pPlayer) const;
    int16           Network_GetPlayerSquadForLocalSlot(int16 pPlayer, int16 pLocalSlot) const;
    int16           Network_GetSquadLocalSlot(int16 pPlayer, int16 pSquad) const;
    int16           Network_FindAliveOwnedSquad(int16 pPlayer) const;
    int16           Network_GetPlayerSelectedSquad(int16 pPlayer) const;
    void            Network_SetPlayerSelectedSquad(int16 pPlayer, int16 pSquad);
    void            Network_ValidateSelectedSquads();
    int16           Network_CountSelectedTroopsInSquad(int16 pSquad) const;
    int16           Network_FindEmptyOwnedSquad(int16 pPlayer) const;
    bool            Network_PlayerCanSplitSelectedSquad(int16 pPlayer) const;
    void            Network_ToggleSelectedTroop(int16 pPlayer, int16 pTroopRow);
    void            Network_SplitSelectedTroops(int16 pPlayer);
    int16           Network_CountAliveOwnedTroops(int16 pOwner, const sSprite* pExclude) const;
    void            Network_ResetMatchState();
    void            Network_SetMatchWinner(int16 pWinnerTeam);
    void            Network_UpdateMatchRules();
    void            Network_UpdateRescuePrisonerRules();
    void            Network_NormalizeSquadAssignments();
    void            Network_DistributeSquadExplosives();

    void            Network_GUI_Sidebar_Draw();

    int16           Network_Recruit_Show();
    int16           Network_Briefing_Show();
    void            Network_Briefing_ReadySync();

    void            Network_Sidebar_ForceSquadIcons();
    void            Network_DrawMatchOverlay();
    void            Network_Draw_WaitingForPlayer();
    void            Network_DrawP2Cursor();
    void            Network_DrawLiveMapOverlay();

    bool            Network_SaveState(uint8_t** buffer, int* len, int* checksum);
    bool            Network_LoadState(const uint8_t* buffer, int len);
    void            Network_FreeBuffer(void* buffer);

    void            Network_CameraSave(sNetCameraState& out);
    void            Network_CameraRestore(const sNetCameraState& in);

    // Multiplayer menu / lobby
    bool            Multiplayer_Menu_Run();
    bool            Multiplayer_ReopenLobby();
    void            Lobby_CampaignSelection();
    bool            ConsumeReturnToMultiplayerLobby();

    // Lobby_CampaignSelection helpers (extracted for readability; pure glue).
    void            Lobby_PushLocalState(bool pIsHost, int16 pSelectedIndex, bool pLocalReady);
    void            Lobby_AdvertiseGame(cNetworkDiscovery& pDiscovery, int16 pSelectedIndex);
    void            Lobby_SendStartReliable();

    // Multiplayer menu object (parallel to mOptionsMenu on cFodder)
    std::unique_ptr<cMultiplayerMenu> mMultiplayerMenu;

    // Deterministic match rules state for PvP/objective modes.
    sNetworkMatchState      mNetMatchState;

    // UDP lobby for campaign selection sync
    std::unique_ptr<cNetworkLobby> mLobby;

    // OFHUB/2 verified-host auth, used by the lobby loop to replay the cached
    // bearer JWT for UpdateAuth + HeartbeatHost while the host stays parked
    // on campaign select. The cache is populated by the multiplayer menu
    // (cMultiplayerMenu::mHubAuth); having a parallel handle here lets the
    // lobby refresh / re-load without going through the menu.
    cHubAuth                       mHubAuth;

    // Long-lived hub client used by Lobby_CampaignSelection so we amortize
    // HELLO+COOKIE across UPDATE/HEARTBEATHOST (host) and HEARTBEATANON
    // (joiner) ticks instead of re-handshaking every 2.5s/30s. The cookie
    // cache + BADCOOKIE/STALE auto-invalidation already lives inside
    // cNetworkHubClient (NetworkHubClient.cpp EnsureCookie / SendCommand);
    // a stable instance lets that machinery actually do its job. Reset on
    // lobby exit so a fresh resolve happens next session.
    std::unique_ptr<cNetworkHubClient> mLobbyHub;

private:
    // Lazy-init / accessor for mLobbyHub. Returns nullptr when the underlying
    // Configure() (resolve + socket bind) fails so the lobby loop can retry
    // on its next throttled tick without partially-initialized state.
    cNetworkHubClient*      AcquireLobbyHub();

    void                    Network_SetActiveSquadContext(int16 pSquad);
    bool                    Network_ShouldShowLiveMapMarker(const sSprite* pSprite) const;
    void                    Network_DrawLiveMapMarker(const sSprite* pSprite, bool pLocalPlayer);

    bool                    mReturnToMultiplayerLobby = false;
};

#endif // OPENFODDER_ENABLE_NETWORK
