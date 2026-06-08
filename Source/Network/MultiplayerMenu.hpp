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
#include "NetworkHubClient.hpp"
#include "RandomMapOptionsMenu.hpp"

class cNetworkDiscovery;
class cNetworkHubClient;

class cMultiplayerMenu {
public:
    cMultiplayerMenu();
    ~cMultiplayerMenu();

    void Open();
    void Close();
    void Draw();
    void Tick();

    void OnRowClick(int16 pAction, int16 pArg);
    void OnBack();

    // Static trampoline for GUI_Button_Setup_New
    static void OnButtonClick(void* ctx, int16 action, int16 arg) {
        static_cast<cMultiplayerMenu*>(ctx)->OnRowClick(action, arg);
    }

    bool IsDone() const { return mDone; }
    bool WasStarted() const { return mStarted; }

    // Results — used by cFodderMultiplayer to configure mStartParams
    int         GetPlayerIndex() const { return mPlayerIndex; }
    std::string GetRemoteHost() const { return mRemoteHost; }
    uint16      GetRemotePort() const { return mRemotePort; }
    uint16      GetLocalPort() const { return mLocalPort; }
    bool        IsSyncTest() const { return mSyncTest; }
    eNetworkGameMode GetGameMode() const { return mGameMode; }
    uint32      GetMapSeed() const { return mMapSeed; }
    uint16      GetKillLimit() const { return mKillLimit; }
    uint16      GetTimeLimitSeconds() const { return mTimeLimitSeconds; }
    uint8       GetTeamCount() const { return mTeamCount; }
    uint8       GetTeamSize() const { return mTeamSize; }
    bool        GetFriendlyFire() const { return mFriendlyFire; }
    eNetworkMapSize GetMapSize() const { return mMapSize; }
    eNetworkMapTerrain GetMapTerrain() const { return mMapTerrain; }
    uint8       GetMapTerrainSub() const { return mMapTerrainSub; }
    eNetworkVehicleSet GetVehicleSet() const { return mVehicleSet; }
    eNetworkPickupDensity GetPickupDensity() const { return mPickupDensity; }
    eNetworkCoverDensity GetCoverDensity() const { return mCoverDensity; }
    bool        IsInternet() const { return mInternet; }
    std::string GetHubHost() const { return mHubHost; }
    uint16      GetHubPort() const { return mHubPort; }
    std::string GetRoomCode() const { return mRoomCode; }
    // Host-side OFHUB/2 plumbing. After CreateInternetRoom or
    // SelectInternetGame, the menu owns the binary 32-byte session key, the
    // peer index it was issued for (0=host, 1=joiner), and — host only — the
    // verified-host JWT used to mint the room. Joiners get an empty bearer
    // (anonymous JOIN per spec § 4.4.2 doesn't require one).
    const std::array<unsigned char, 32>& GetSessionKey() const { return mSessionKey; }
    bool        HasSessionKey() const { return mHasSessionKey; }
    uint8       GetPeerIndex() const { return mPeerIndex; }
    const sHubAuthToken& GetHubBearer() const { return mHubBearer; }

    enum eAction : int16 {
        ACT_NONE = 0,
        ACT_HOST,
        ACT_HOST_INTERNET,
        ACT_JOIN,
        ACT_FIND_LAN,
        ACT_FIND_INTERNET,
        ACT_DIRECT_CONNECT,
        ACT_JOIN_DISCOVERED,
        ACT_JOIN_INTERNET,
        ACT_JOIN_ROOM_CODE,
        ACT_REFRESH_LAN,
        ACT_REFRESH_INTERNET,
        ACT_SYNC_TEST,
        ACT_BACK,
        ACT_ROW,
        ACT_EDIT_REMOTE_HOST,
        ACT_EDIT_REMOTE_PORT,
        ACT_EDIT_LOCAL_PORT,
        ACT_EDIT_MAP_SEED,
        ACT_EDIT_HUB_HOST,
        ACT_EDIT_HUB_PORT,
        ACT_EDIT_ROOM_CODE,
        ACT_CYCLE_MODE,
        ACT_MAP_OPTIONS,
        ACT_START,
        // Auth-prompt modal — shown when the user picks an internet flow but
        // has no cached hub token. Confirm fires BeginPairing (opens browser
        // + advances to the pair-code entry sub-screen); cancel returns to
        // the main multiplayer menu.
        ACT_AUTH_CONFIRM,
        ACT_AUTH_CANCEL,
        ACT_AUTH_PAIR,           // submit the typed pair code
        ACT_AUTH_REOPEN,         // re-fire BeginPairing with a fresh device code
    };

private:
    enum class eState {
        Main,
        Host,
        Join,
        FindLan,
        FindInternet,
        MapOptions,
        AuthPrompt,         // "open in browser?" modal before BeginPairing
        AuthPairing,        // 6-char pair-code entry after BeginPairing
    };

    enum class eEditField {
        None,
        RemoteHost,
        RemotePort,
        LocalPort,
        MapSeed,
        HubHost,
        HubPort,
        RoomCode,
    };

    void DrawMainMenu();
    void DrawHostMenu();
    void DrawJoinMenu();
    void DrawFindLanMenu();
    void DrawFindInternetMenu();
    void DrawAuthPromptMenu();
    void DrawAuthPairingMenu();
    void DrawConnectionMenu(const char* pTitle, const char* pRemoteHostLabel, const char* pRemotePortLabel, bool pHostSetup);
    void DrawField(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction, bool pActive);
    void DrawValueButton(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction);
    void HandleTextInput();
    void SelectField(int16 pAction);
    void StartLanBrowser();
    void RefreshLanBrowser();
    void StopLanBrowser();
    void StartInternetBrowser();
    void RefreshInternetBrowser();
    void StopInternetBrowser();
    bool CreateInternetRoom();
    void SelectDiscoveredGame(size_t pIndex);
    void SelectInternetGame(size_t pIndex);

    // Public-matchmaking auth gating. Returns true if the user already has a
    // valid cached token and the caller may proceed; returns false and
    // pivots the menu to AuthPrompt otherwise. pBlockOnFail controls the
    // post-prompt behaviour: true (host flows) means "if pairing is skipped,
    // do not enter the requested state"; false (join flows) means "let the
    // user proceed even without a token — joining a room only needs the
    // relay credentials, not a hub token".
    bool RequireHubToken(eState pTarget, bool pBlockOnFail);
    void HandleAuthTextInput();
    void SubmitPairCode();
    bool CanStart() const;
    void SyncPortValues();
    void OpenMapOptions();
    void ApplyMapOptions(const sRandomMapOptions& pOptions);
    sRandomMapOptions BuildMapOptions() const;

    eState      mState = eState::Main;
    eEditField  mEditField = eEditField::None;
    bool        mDone = false;
    bool        mStarted = false;

    int         mPlayerIndex = 0;   // 0 = host/P1, 1 = join/P2
    std::string mRemoteHost = "127.0.0.1";
    uint16      mRemotePort = 7001;
    uint16      mLocalPort  = 7000;
    std::string mRemotePortText = "7001";
    std::string mLocalPortText = "7000";
    bool        mSyncTest = false;
    bool        mInternet = false;
    bool        mInternetHost = false;
    std::string mHubHost = NETWORK_HUB_DEFAULT_HOST;
    uint16      mHubPort = NETWORK_HUB_DEFAULT_PORT;
    std::string mHubPortText = std::to_string(NETWORK_HUB_DEFAULT_PORT);
    std::string mRoomCode;
    // Binary copy of OFHUB/2 session_key returned by CREATEOK / JOINOK; raw
    // bytes survive the round-trip through StartParams. Stays zeroed for
    // LAN / direct-connect flows.
    std::array<unsigned char, 32> mSessionKey{};
    bool        mHasSessionKey = false;
    uint8       mPeerIndex = 0;          // 0 host, 1 first joiner
    sHubAuthToken mHubBearer;            // populated only on host CreateAuth path
    eNetworkGameMode mGameMode = eNetworkGameMode_CoopCampaign;
    uint32      mMapSeed = NETWORK_MAP_SEED_DEFAULT;
    std::string mMapSeedText = std::to_string(NETWORK_MAP_SEED_DEFAULT);
    uint16      mKillLimit = NETWORK_KILL_LIMIT_DEFAULT;
    uint16      mTimeLimitSeconds = NETWORK_TIME_LIMIT_DEFAULT;
    uint8       mTeamCount = NETWORK_TEAM_COUNT_DEFAULT;
    uint8       mTeamSize = NETWORK_TEAM_SIZE_DEFAULT;
    bool        mFriendlyFire = false;
    eNetworkMapSize mMapSize = NETWORK_MAP_SIZE_DEFAULT;
    eNetworkMapTerrain mMapTerrain = NETWORK_MAP_TERRAIN_DEFAULT;
    uint8       mMapTerrainSub = 0;
    eNetworkVehicleSet mVehicleSet = NETWORK_VEHICLE_SET_DEFAULT;
    eNetworkPickupDensity mPickupDensity = NETWORK_PICKUP_DENSITY_DEFAULT;
    eNetworkCoverDensity mCoverDensity = NETWORK_COVER_DENSITY_DEFAULT;
    bool        mDiscoveryFailed = false;
    std::unique_ptr<cNetworkDiscovery> mDiscovery;
    bool        mHubFailed = false;
    std::unique_ptr<cNetworkHubClient> mHubClient;
    std::vector<sNetworkHubGame> mInternetGames;
    cRandomMapOptionsMenu mMapOptionsMenu;

    // Auth/pairing state — populated once the user picks an internet flow
    // without a cached token.  mAuthTarget remembers which state to switch
    // back to after a successful pairing (or after a skip when joining).
    cHubAuth    mHubAuth;
    eState      mAuthTarget = eState::Main;
    bool        mAuthBlockOnFail = false;
    std::string mDeviceCode;
    std::string mPairCode;
    std::string mPairingError;

    // Keep drawn strings alive for GUI draw calls
    std::vector<std::string> mDrawStrings;
};

#endif // OPENFODDER_ENABLE_NETWORK
