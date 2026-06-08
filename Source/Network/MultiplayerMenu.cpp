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
#include "NetworkMenuText.hpp"

#ifdef OPENFODDER_ENABLE_NETWORK

static bool MultiplayerMenu_PortFromText(const std::string& pText, uint16& pPort)
{
    if (pText.empty())
        return false;

    int Value = 0;
    for (char Char : pText)
    {
        if (Char < '0' || Char > '9')
            return false;

        Value = (Value * 10) + (Char - '0');
        if (Value > 65535)
            return false;
    }

    if (Value <= 0)
        return false;

    pPort = (uint16)Value;
    return true;
}

static std::string MultiplayerMenu_DisplayHost(std::string pHost)
{
    for (char& Char : pHost)
    {
        if (Char == '.' || Char == '-')
            Char = ' ';
    }

    return pHost;
}

static const char* MultiplayerMenu_ModeShortName(eNetworkGameMode pMode)
{
    switch (pMode) {
    case eNetworkGameMode_CoopCampaign:     return "COOP";
    case eNetworkGameMode_Deathmatch:       return "DM";
    case eNetworkGameMode_SquadDeathmatch:  return "SQUAD DM";
    case eNetworkGameMode_RescuePrisoner:   return "RESCUE";
    case eNetworkGameMode_AvatarDeathmatch: return "AVATAR";
    case eNetworkGameMode_TeamAvatar:       return "TEAM";
    default:                                return "UNKNOWN";
    }
}

static const char* MultiplayerMenu_DiscoveryStateName(uint8_t pState)
{
    switch (pState) {
    case eNetworkDiscoveryState_Setup:      return "OPEN";
    case eNetworkDiscoveryState_InProgress: return "PLAYING";
    case eNetworkDiscoveryState_Full:       return "FULL";
    case eNetworkDiscoveryState_Locked:     return "LOCKED";
    default:                                return "LOCKED";
    }
}

static bool MultiplayerMenu_DiscoveryStateJoinable(uint8_t pState)
{
    return pState == eNetworkDiscoveryState_Setup;
}

static std::string MultiplayerMenu_BuildMapLabel(eNetworkMapSize pSize, eNetworkMapTerrain pTerrain)
{
    return std::string(Network_MapSizeName(pSize)) + " " + Network_MapTerrainName(pTerrain);
}

static sNetworkHubMetadata MultiplayerMenu_BuildHubMetadata(
    eNetworkGameMode pMode,
    uint32 pMapSeed,
    uint16 pKillLimit,
    uint16 pTimeLimitSeconds,
    uint8 pTeamCount,
    uint8 pTeamSize,
    bool pFriendlyFire,
    eNetworkMapSize pMapSize,
    eNetworkMapTerrain pMapTerrain,
    eNetworkCoverDensity pCoverDensity)
{
    sNetworkHubMetadata Metadata;
    Metadata.mGameName = "OpenFodder Lobby";
    Metadata.mGameMode = Network_GameModeName(pMode);
    Metadata.mMapName = Network_IsPvPMode(pMode) ? MultiplayerMenu_BuildMapLabel(pMapSize, pMapTerrain) : "Campaign Select";
    Metadata.mVersion = "N" + std::to_string((int)NETWORK_COMPATIBILITY_VERSION);
    Metadata.mOptions =
        "seed=" + std::to_string(pMapSeed) +
        ";kill=" + std::to_string((int)pKillLimit) +
        ";time=" + std::to_string((int)pTimeLimitSeconds) +
        ";teams=" + std::to_string((int)pTeamCount) +
        ";size=" + std::to_string((int)pTeamSize) +
        ";ff=" + std::to_string(pFriendlyFire ? 1 : 0) +
        ";cover=" + std::string(Network_CoverDensityName(pCoverDensity));
    return Metadata;
}

cMultiplayerMenu::cMultiplayerMenu() {
}

cMultiplayerMenu::~cMultiplayerMenu() {
}

void cMultiplayerMenu::Open() {
    mDone = false;
    mStarted = false;
    mEditField = eEditField::None;
    mState = eState::Main;
    mDiscoveryFailed = false;
    StopLanBrowser();
    mPlayerIndex = g_Fodder->mStartParams->mNetworkPlayerIndex;
    mRemoteHost = g_Fodder->mStartParams->mNetworkRemoteHost.size()
        ? g_Fodder->mStartParams->mNetworkRemoteHost
        : "127.0.0.1";
    mRemotePort = g_Fodder->mStartParams->mNetworkRemotePort;
    mLocalPort = g_Fodder->mStartParams->mNetworkLocalPort;
    mRemotePortText = std::to_string(mRemotePort);
    mLocalPortText = std::to_string(mLocalPort);
    mInternet = g_Fodder->mStartParams->mNetworkInternet;
    mInternetHost = false;
    mHubHost = g_Fodder->mStartParams->mNetworkHubHost.size()
        ? g_Fodder->mStartParams->mNetworkHubHost
        : NETWORK_HUB_DEFAULT_HOST;
    mHubPort = g_Fodder->mStartParams->mNetworkHubPort
        ? g_Fodder->mStartParams->mNetworkHubPort
        : NETWORK_HUB_DEFAULT_PORT;
    mHubPortText = std::to_string(mHubPort);
    mRoomCode = g_Fodder->mStartParams->mNetworkRoomCode;
    mGameMode = g_Fodder->mStartParams->mNetworkGameMode;
    mMapSeed = g_Fodder->mStartParams->mNetworkMapSeed;
    mMapSeedText = std::to_string(mMapSeed);
    mKillLimit = g_Fodder->mStartParams->mNetworkKillLimit;
    mTimeLimitSeconds = g_Fodder->mStartParams->mNetworkTimeLimitSeconds;
    mTeamCount = g_Fodder->mStartParams->mNetworkTeamCount;
    mTeamSize = g_Fodder->mStartParams->mNetworkTeamSize;
    mFriendlyFire = g_Fodder->mStartParams->mNetworkFriendlyFire;
    mMapSize = g_Fodder->mStartParams->mNetworkMapSize;
    mMapTerrain = g_Fodder->mStartParams->mNetworkMapTerrain;
    mMapTerrainSub = (uint8)g_Fodder->mStartParams->mNetworkMapTerrainSub;
    mVehicleSet = g_Fodder->mStartParams->mNetworkVehicleSet;
    mPickupDensity = g_Fodder->mStartParams->mNetworkPickupDensity;
    mCoverDensity = g_Fodder->mStartParams->mNetworkCoverDensity;

    const eNetworkMenuStart MenuStart = g_Fodder->mStartParams->mNetworkMenuStart;
    g_Fodder->mStartParams->mNetworkMenuStart = eNetworkMenuStart_None;

    switch (MenuStart) {
    case eNetworkMenuStart_Host:
        mState = eState::Host;
        mPlayerIndex = 0;
        break;
    case eNetworkMenuStart_HostMapOptions:
        mPlayerIndex = 0;
        OpenMapOptions();
        break;
    case eNetworkMenuStart_Join:
        mState = eState::Join;
        mPlayerIndex = 1;
        break;
    case eNetworkMenuStart_FindLan:
        mState = eState::FindLan;
        mPlayerIndex = 1;
        StartLanBrowser();
        break;
    case eNetworkMenuStart_FindInternet:
        mState = eState::FindInternet;
        mPlayerIndex = 1;
        mInternet = true;
        StartInternetBrowser();
        break;
    case eNetworkMenuStart_Main:
    case eNetworkMenuStart_None:
    default:
        break;
    }
}

void cMultiplayerMenu::Close() {
    StopLanBrowser();
    StopInternetBrowser();
}

void cMultiplayerMenu::Tick() {
    if (mState == eState::MapOptions) {
        mMapOptionsMenu.Tick();
        if (mMapOptionsMenu.IsDone()) {
            if (mMapOptionsMenu.WasAccepted())
                ApplyMapOptions(mMapOptionsMenu.GetOptions());

            mState = eState::Host;
            mEditField = eEditField::None;
        }
        return;
    }

    HandleTextInput();
    HandleAuthTextInput();

    if (mState == eState::FindLan && mDiscovery)
        mDiscovery->PollBrowser();
}

void cMultiplayerMenu::OnBack() {
    if (mEditField != eEditField::None) {
        mEditField = eEditField::None;
        return;
    }

    if (mState == eState::FindLan)
        StopLanBrowser();
    if (mState == eState::FindInternet)
        StopInternetBrowser();

    if (mState == eState::MapOptions) {
        mMapOptionsMenu.OnBack();
    } else if (mState == eState::AuthPrompt) {
        OnRowClick(ACT_AUTH_CANCEL, 0);
    } else if (mState == eState::AuthPairing) {
        // Treat Esc on the pair-code page the same as the modal's cancel —
        // honours block-on-fail for hosting.
        OnRowClick(ACT_AUTH_CANCEL, 0);
    } else if (mState == eState::Host || mState == eState::Join || mState == eState::FindLan || mState == eState::FindInternet) {
        mState = eState::Main;
    } else {
        mDone = true;
        mStarted = false;
    }
}

void cMultiplayerMenu::OnRowClick(int16 pAction, int16 pArg) {
    switch (pAction) {
    case ACT_HOST:
        mState = eState::Host;
        mEditField = eEditField::None;
        mPlayerIndex = 0;
        mInternet = false;
        mInternetHost = false;
        mRoomCode.clear();
        mRemoteHost = "127.0.0.1";
        mLocalPort = 7000;
        mRemotePort = 7001;
        mLocalPortText = std::to_string(mLocalPort);
        mRemotePortText = std::to_string(mRemotePort);
        break;

    case ACT_HOST_INTERNET:
        // Hosting on the public hub blocks until we have a token: the
        // /room/create call requires Authorization. Joining is more
        // permissive (the hub allows anonymous LIST/JOIN), so the cancel
        // semantics differ between host and find — see RequireHubToken.
        if (!RequireHubToken(eState::Host, /*pBlockOnFail=*/true))
            break;
        mState = eState::Host;
        mEditField = eEditField::None;
        mPlayerIndex = 0;
        mInternet = true;
        mInternetHost = true;
        mRoomCode.clear();
        mLocalPort = 7000;
        mLocalPortText = std::to_string(mLocalPort);
        break;

    case ACT_FIND_LAN:
        mState = eState::FindLan;
        mEditField = eEditField::None;
        mPlayerIndex = 1;
        mInternet = false;
        mInternetHost = false;
        mRoomCode.clear();
        StartLanBrowser();
        break;

    case ACT_FIND_INTERNET:
        // Joining only needs anonymous LIST/JOIN, so we still prompt for
        // pairing (the hub uses the token for moderation telemetry) but
        // proceed if the user skips it.
        if (!RequireHubToken(eState::FindInternet, /*pBlockOnFail=*/false))
            break;
        mState = eState::FindInternet;
        mEditField = eEditField::None;
        mPlayerIndex = 1;
        mInternet = true;
        mInternetHost = false;
        StartInternetBrowser();
        break;

    case ACT_DIRECT_CONNECT:
    case ACT_JOIN:
        mState = eState::Join;
        mEditField = eEditField::None;
        mPlayerIndex = 1;
        mInternet = false;
        mInternetHost = false;
        mRoomCode.clear();
        mLocalPort = 7001;
        mRemotePort = 7000;
        mLocalPortText = std::to_string(mLocalPort);
        mRemotePortText = std::to_string(mRemotePort);
        break;

    case ACT_JOIN_DISCOVERED:
        SelectDiscoveredGame((size_t)pArg);
        break;

    case ACT_JOIN_INTERNET:
        SelectInternetGame((size_t)pArg);
        break;

    case ACT_JOIN_ROOM_CODE:
        SelectInternetGame((size_t)-1);
        break;

    case ACT_REFRESH_LAN:
        RefreshLanBrowser();
        break;

    case ACT_REFRESH_INTERNET:
        RefreshInternetBrowser();
        break;

    case ACT_SYNC_TEST:
        mSyncTest = true;
        mPlayerIndex = 0;
        mDone = true;
        mStarted = true;
        break;

    case ACT_BACK:
        OnBack();
        break;

    case ACT_START:
        SyncPortValues();
        if (CanStart()) {
            if (!mInternetHost || CreateInternetRoom()) {
                mDone = true;
                mStarted = true;
            }
        }
        break;

    case ACT_EDIT_REMOTE_HOST:
    case ACT_EDIT_REMOTE_PORT:
    case ACT_EDIT_LOCAL_PORT:
    case ACT_EDIT_MAP_SEED:
    case ACT_EDIT_HUB_HOST:
    case ACT_EDIT_HUB_PORT:
    case ACT_EDIT_ROOM_CODE:
        SelectField(pAction);
        break;

    case ACT_CYCLE_MODE:
        mEditField = eEditField::None;
        mGameMode = Network_NormalizeGameMode((uint8_t)mGameMode + 1);
        break;

    case ACT_MAP_OPTIONS:
        OpenMapOptions();
        break;

    case ACT_AUTH_CONFIRM: {
        // User clicked OPEN BROWSER on the auth modal. Fire BeginPairing
        // (this opens the user's default browser to the Discord OAuth URL)
        // and pivot to the pair-code entry sub-screen regardless of whether
        // the browser open itself succeeded — the user can copy the URL
        // manually if it didn't, and we still need somewhere to type the
        // resulting pair code.
        mPairCode.clear();
        mPairingError.clear();
        mDeviceCode.clear();
        std::string fresh;
        if (!mHubAuth.BeginPairing(fresh))
            mPairingError = mHubAuth.GetLastError();
        mDeviceCode = fresh;
        mState = eState::AuthPairing;
        mEditField = eEditField::None;
        // Swallow the click's keycode so HandleAuthTextInput on the next
        // tick doesn't see a stale Enter / Space.
        g_Fodder->mInput_LastKey = g_Fodder->mKeyCode;
        break;
    }

    case ACT_AUTH_CANCEL:
        // User declined the auth modal. Block-on-fail means hosting can't
        // proceed without a token, so we drop back to Main; for joining we
        // honour the user's "no thanks" by entering the requested state
        // anyway and let the hub answer with whatever its anonymous policy
        // is.
        mPairCode.clear();
        mPairingError.clear();
        mDeviceCode.clear();
        if (mAuthBlockOnFail) {
            mState = eState::Main;
            mEditField = eEditField::None;
        } else {
            // Pretend the user re-clicked the original entry button so the
            // existing path runs without re-asking (RequireHubToken guards
            // against re-prompting because we leave the cache untouched
            // — this branch is only reached when block-on-fail is false).
            const eState target = mAuthTarget;
            mAuthTarget = eState::Main;
            mState = target;
            mEditField = eEditField::None;
            if (target == eState::FindInternet) {
                mPlayerIndex = 1;
                mInternet = true;
                mInternetHost = false;
                StartInternetBrowser();
            }
        }
        break;

    case ACT_AUTH_PAIR:
        SubmitPairCode();
        break;

    case ACT_AUTH_REOPEN: {
        // Re-fire BeginPairing with a fresh device code in case the user
        // lost the browser tab or the prior code timed out server-side.
        mPairCode.clear();
        mPairingError.clear();
        std::string fresh;
        if (!mHubAuth.BeginPairing(fresh))
            mPairingError = mHubAuth.GetLastError();
        mDeviceCode = fresh;
        break;
    }

    default:
        break;
    }
}

void cMultiplayerMenu::Draw() {
    switch (mState) {
    case eState::Main:
        DrawMainMenu();
        break;
    case eState::Host:
        DrawHostMenu();
        break;
    case eState::Join:
        DrawJoinMenu();
        break;
    case eState::FindLan:
        DrawFindLanMenu();
        break;
    case eState::FindInternet:
        DrawFindInternetMenu();
        break;
    case eState::MapOptions:
        mMapOptionsMenu.Draw();
        break;
    case eState::AuthPrompt:
        DrawAuthPromptMenu();
        break;
    case eState::AuthPairing:
        DrawAuthPairingMenu();
        break;
    }
}

void cMultiplayerMenu::DrawMainMenu() {
    mDrawStrings.clear();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("MULTIPLAYER", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    size_t YOffset = PLATFORM_BASED(0, 25);
    int16 rowY = 0x36;
    int16 rowH = 0x15;

    // HOST LAN GAME
    g_Fodder->GUI_Button_Draw_Small("HOST LAN GAME", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_HOST);

    rowY += rowH;

    // FIND LAN GAME
    g_Fodder->GUI_Button_Draw_Small("FIND LAN GAME", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_FIND_LAN);

    rowY += rowH;

    // HOST INTERNET GAME
    g_Fodder->GUI_Button_Draw_Small("HOST INTERNET GAME", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_HOST_INTERNET);

    rowY += rowH;

    // FIND INTERNET GAME
    g_Fodder->GUI_Button_Draw_Small("FIND INTERNET GAME", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_FIND_INTERNET);

    rowY += rowH;

    // DIRECT CONNECT
    g_Fodder->GUI_Button_Draw_Small("DIRECT CONNECT", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_DIRECT_CONNECT);

    rowY += rowH;

    // SYNC TEST
    g_Fodder->GUI_Button_Draw_Small("SYNC TEST", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_SYNC_TEST);

    // BACK
    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
}

void cMultiplayerMenu::DrawHostMenu() {
    DrawConnectionMenu(mInternetHost ? "HOST INTERNET GAME" : "HOST LAN GAME", "PEER IP", "PEER PORT", true);
}

void cMultiplayerMenu::DrawJoinMenu() {
    DrawConnectionMenu("DIRECT CONNECT", "HOST IP", "HOST PORT", false);
}

void cMultiplayerMenu::DrawFindLanMenu() {
    mDrawStrings.clear();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("FIND LAN GAME", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    size_t YOffset = PLATFORM_BASED(0, 25);
    int16 rowY = 0x3E;
    const uint32_t Now = (uint32_t)SDL_GetTicks();
    const bool RecentVersionMismatch = mDiscovery &&
        mDiscovery->GetLastVersionMismatchTicks() &&
        Now - mDiscovery->GetLastVersionMismatchTicks() < 5000;
    const bool RecentCompatibilityMismatch = mDiscovery &&
        mDiscovery->GetLastCompatibilityMismatchTicks() &&
        Now - mDiscovery->GetLastCompatibilityMismatchTicks() < 5000;
    const bool RecentTimeout = mDiscovery &&
        mDiscovery->GetLastExpiredGameTicks() &&
        Now - mDiscovery->GetLastExpiredGameTicks() < 3000;

    if (mDiscoveryFailed) {
        g_Fodder->String_Print_Small("LAN BROWSER FAILED", rowY);
    }
    else if (!mDiscovery || mDiscovery->GetGames().empty()) {
        if (RecentCompatibilityMismatch)
            g_Fodder->String_Print_Small("INCOMPATIBLE BUILD", rowY);
        else if (RecentVersionMismatch)
            g_Fodder->String_Print_Small("INCOMPATIBLE LAN GAME", rowY);
        else if (RecentTimeout)
            g_Fodder->String_Print_Small("LAN GAME TIMED OUT", rowY);
        else
            g_Fodder->String_Print_Small("NO LAN GAMES FOUND", rowY);
    }
    else {
        const auto& Games = mDiscovery->GetGames();
        const size_t Count = std::min<size_t>(Games.size(), 4);
        for (size_t Index = 0; Index < Count; ++Index) {
            const auto& Game = Games[Index];
            const bool Joinable = MultiplayerMenu_DiscoveryStateJoinable(Game.mState) && Game.mLobbyPort;
            const uint8_t MaxPlayers = Game.mMaxPlayers ? Game.mMaxPlayers : NETWORK_MAX_PLAYERS;
            const uint32_t AgeSeconds = (Now - Game.mLastSeenTicks) / 1000;
            std::string Label = Game.mGameName.size() ? Game.mGameName : (Game.mHostName.size() ? Game.mHostName : Game.mHostAddress);
            Label += " ";
            Label += MultiplayerMenu_ModeShortName(Game.mSettings.mGameMode);
            Label += " ";
            Label += Network_IsPvPMode(Game.mSettings.mGameMode)
                ? MultiplayerMenu_BuildMapLabel(
                    Network_NormalizeMapSize((uint8_t)Game.mSettings.mMapSize),
                    Network_NormalizeMapTerrain((uint8_t)Game.mSettings.mMapTerrain))
                : "CAMPAIGN";
            Label += " ";
            Label += std::to_string((int)Game.mCurrentPlayers);
            Label += "/";
            Label += std::to_string((int)MaxPlayers);
            Label += " ";
            Label += MultiplayerMenu_DiscoveryStateName(Game.mState);
            Label += " ";
            Label += "N";
            Label += std::to_string((int)Game.mCompatibilityVersion);
            Label += " ";
            Label += std::to_string((int)AgeSeconds);
            Label += "S";
            Label = NetworkMenu_FitText(Label, 190);

            g_Fodder->GUI_Button_Draw_Small(Label.c_str(), rowY, Joinable ? 0xB2 : 0xF2, Joinable ? 0xB3 : 0xF3);
            if (Joinable)
                g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_JOIN_DISCOVERED, (int16)Index);
            rowY += 0x15;
        }
    }

    g_Fodder->GUI_Button_Draw_Small("REFRESH", 0x9E + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_REFRESH_LAN);

    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
}

void cMultiplayerMenu::DrawFindInternetMenu() {
    mDrawStrings.clear();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("FIND INTERNET GAME", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    size_t YOffset = PLATFORM_BASED(0, 25);
    int16 rowY = 0x32;

    DrawField("HUB HOST", MultiplayerMenu_DisplayHost(mHubHost), rowY, ACT_EDIT_HUB_HOST, mEditField == eEditField::HubHost);
    rowY += 0x12;
    DrawField("HUB PORT", mHubPortText, rowY, ACT_EDIT_HUB_PORT, mEditField == eEditField::HubPort);
    rowY += 0x12;
    DrawField("ROOM CODE", mRoomCode, rowY, ACT_EDIT_ROOM_CODE, mEditField == eEditField::RoomCode);
    rowY += 0x16;

    g_Fodder->GUI_Button_Draw_Small("JOIN CODE", rowY, mRoomCode.size() ? 0xB2 : 0xF2, mRoomCode.size() ? 0xB3 : 0xF3);
    if (mRoomCode.size())
        g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_JOIN_ROOM_CODE);
    rowY += 0x15;

    if (mHubFailed) {
        g_Fodder->String_Print_Small("HUB BROWSER FAILED", rowY);
    }
    else if (mInternetGames.empty()) {
        g_Fodder->String_Print_Small("NO INTERNET GAMES FOUND", rowY);
    }
    else {
        const size_t Count = std::min<size_t>(mInternetGames.size(), 2);
        for (size_t Index = 0; Index < Count; ++Index) {
            const auto& Game = mInternetGames[Index];
            const bool Joinable = Game.mRelayPort && Game.mCurrentPlayers < Game.mMaxPlayers;
            std::string Label = Game.mMetadata.mGameName.size() ? Game.mMetadata.mGameName : Game.mRoomCode;
            Label += " ";
            Label += Game.mMetadata.mGameMode.size() ? Game.mMetadata.mGameMode : "MODE";
            Label += " ";
            Label += Game.mMetadata.mMapName.size() ? Game.mMetadata.mMapName : "MAP";
            Label += " ";
            Label += std::to_string((int)Game.mCurrentPlayers);
            Label += "/";
            Label += std::to_string((int)Game.mMaxPlayers);
            Label += " ";
            Label += Game.mRoomCode;
            Label = NetworkMenu_FitText(Label, 190);

            g_Fodder->GUI_Button_Draw_Small(Label.c_str(), rowY, Joinable ? 0xB2 : 0xF2, Joinable ? 0xB3 : 0xF3);
            if (Joinable)
                g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_JOIN_INTERNET, (int16)Index);
            rowY += 0x15;
        }
    }

    g_Fodder->GUI_Button_Draw_Small("REFRESH", 0x9E + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_REFRESH_INTERNET);

    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
}

void cMultiplayerMenu::DrawConnectionMenu(const char* pTitle, const char* pRemoteHostLabel, const char* pRemotePortLabel, bool pHostSetup) {
    mDrawStrings.clear();
    SyncPortValues();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large(pTitle, false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    size_t YOffset = PLATFORM_BASED(0, 25);
    int16 rowY = pHostSetup ? (mInternetHost ? 0x24 : 0x34) : 0x44;

    if (pHostSetup) {
        if (mInternetHost) {
            DrawField("HUB HOST", MultiplayerMenu_DisplayHost(mHubHost), rowY, ACT_EDIT_HUB_HOST, mEditField == eEditField::HubHost);
            rowY += 0x12;
            DrawField("HUB PORT", mHubPortText, rowY, ACT_EDIT_HUB_PORT, mEditField == eEditField::HubPort);
            rowY += 0x12;
        }
        DrawValueButton("MODE", Network_GameModeName(mGameMode), rowY, ACT_CYCLE_MODE);
        rowY += 0x12;
        DrawField("MAP SEED", mMapSeedText, rowY, ACT_EDIT_MAP_SEED, mEditField == eEditField::MapSeed);
        rowY += 0x12;
        DrawValueButton("MAP", std::string(Network_MapSizeName(mMapSize)) + " " + Network_MapTerrainName(mMapTerrain), rowY, ACT_MAP_OPTIONS);
        rowY += 0x12;
    }

    if (!pHostSetup) {
        DrawField(pRemoteHostLabel, MultiplayerMenu_DisplayHost(mRemoteHost), rowY, ACT_EDIT_REMOTE_HOST, mEditField == eEditField::RemoteHost);
        rowY += 0x18;
        DrawField(pRemotePortLabel, mRemotePortText, rowY, ACT_EDIT_REMOTE_PORT, mEditField == eEditField::RemotePort);
        rowY += 0x18;
    }

    DrawField("LOCAL PORT", mLocalPortText, rowY, ACT_EDIT_LOCAL_PORT, mEditField == eEditField::LocalPort);
    rowY += pHostSetup ? 0x1E : 0x24;

    // START
    g_Fodder->GUI_Button_Draw_Small("START", rowY, CanStart() ? 0xB2 : 0xF2, CanStart() ? 0xB3 : 0xF3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_START);

    // BACK
    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
}

void cMultiplayerMenu::DrawValueButton(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction) {
    const size_t FieldX1 = 0x88;
    const size_t FieldX2 = 0x128;
    const std::string FittedValue = NetworkMenu_FitText(pValue, (int)(FieldX2 - FieldX1 - 4));

    g_Fodder->String_Print_Small(pLabel, 0x20, pY);
    g_Fodder->String_Print_Small_LeftInBox(FittedValue, FieldX1, FieldX2, pY, 2);

    g_Fodder->mGUI_Temp_X = (int16)FieldX1;
    g_Fodder->mGUI_Temp_Y = pY;
    g_Fodder->mGUI_Temp_Width = (int16)(FieldX2 - FieldX1);
    if (!g_Fodder->mGUI_Draw_LastHeight)
        g_Fodder->mGUI_Draw_LastHeight = 6;

    g_Fodder->GUI_Box_Draw(0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, pAction);
}

void cMultiplayerMenu::DrawField(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction, bool pActive) {
    const size_t FieldX1 = 0x88;
    const size_t FieldX2 = 0x128;
    const std::string Value = pValue.size() ? pValue : "ENTER";
    const std::string FittedValue = NetworkMenu_FitText(Value, (int)(FieldX2 - FieldX1 - 4));

    g_Fodder->String_Print_Small(pLabel, 0x20, pY);
    g_Fodder->String_Print_Small_LeftInBox(FittedValue, FieldX1, FieldX2, pY, 2);

    g_Fodder->mGUI_Temp_X = (int16)FieldX1;
    g_Fodder->mGUI_Temp_Y = pY;
    g_Fodder->mGUI_Temp_Width = (int16)(FieldX2 - FieldX1);
    if (!g_Fodder->mGUI_Draw_LastHeight)
        g_Fodder->mGUI_Draw_LastHeight = 6;

    g_Fodder->GUI_Box_Draw(pActive ? 0xF2 : 0xB2, pActive ? 0xF3 : 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, pAction);
}

void cMultiplayerMenu::HandleTextInput() {
    if (mEditField == eEditField::None)
        return;

    int16 KeyAscii = 0;
    if (g_Fodder->mKeyCode != g_Fodder->mInput_LastKey) {
        g_Fodder->mInput_LastKey = g_Fodder->mKeyCode;

        if (g_Fodder->mKeyCode) {
            if (g_Fodder->mKeyCode >= SDL_SCANCODE_A && g_Fodder->mKeyCode <= SDL_SCANCODE_Z)
                KeyAscii = 'A' + (g_Fodder->mKeyCode - SDL_SCANCODE_A);

            if (g_Fodder->mKeyCode >= SDL_SCANCODE_1 && g_Fodder->mKeyCode <= SDL_SCANCODE_9)
                KeyAscii = '1' + (g_Fodder->mKeyCode - SDL_SCANCODE_1);

            if (g_Fodder->mKeyCode == SDL_SCANCODE_0)
                KeyAscii = '0';

            if (g_Fodder->mKeyCode == SDL_SCANCODE_PERIOD || g_Fodder->mKeyCode == SDL_SCANCODE_KP_PERIOD)
                KeyAscii = '.';

            if (g_Fodder->mKeyCode == SDL_SCANCODE_MINUS)
                KeyAscii = '-';

            if (g_Fodder->mKeyCode == SDL_SCANCODE_BACKSPACE)
                KeyAscii = 8;

            if (g_Fodder->mKeyCode == SDL_SCANCODE_RETURN)
                KeyAscii = 0x0D;
        }
    }

    if (!KeyAscii)
        return;

    if (KeyAscii == 0x0D) {
        mEditField = eEditField::None;
        return;
    }

    std::string* Target = 0;
    size_t MaxLength = 0;
    bool NumericOnly = false;

    if (mEditField == eEditField::RemoteHost) {
        Target = &mRemoteHost;
        MaxLength = 31;
    }
    else if (mEditField == eEditField::RemotePort) {
        Target = &mRemotePortText;
        MaxLength = 5;
        NumericOnly = true;
    }
    else if (mEditField == eEditField::LocalPort) {
        Target = &mLocalPortText;
        MaxLength = 5;
        NumericOnly = true;
    }
    else if (mEditField == eEditField::MapSeed) {
        Target = &mMapSeedText;
        MaxLength = 10;
        NumericOnly = true;
    }
    else if (mEditField == eEditField::HubHost) {
        Target = &mHubHost;
        MaxLength = 63;
    }
    else if (mEditField == eEditField::HubPort) {
        Target = &mHubPortText;
        MaxLength = 5;
        NumericOnly = true;
    }
    else if (mEditField == eEditField::RoomCode) {
        Target = &mRoomCode;
        MaxLength = 12;
    }

    if (!Target)
        return;

    if (KeyAscii == 8) {
        if (Target->size())
            Target->pop_back();
        SyncPortValues();
        return;
    }

    if (Target->size() >= MaxLength)
        return;

    if (NumericOnly) {
        if (KeyAscii < '0' || KeyAscii > '9')
            return;

        std::string Candidate = *Target + (char)KeyAscii;
        uint16 ParsedPort = 0;
        if (mEditField == eEditField::MapSeed) {
            uint32 ParsedSeed = 0;
            if (!NetworkMenu_UInt32FromText(Candidate, ParsedSeed))
                return;
        }
        else if (!MultiplayerMenu_PortFromText(Candidate, ParsedPort)) {
            return;
        }

        Target->push_back((char)KeyAscii);
        SyncPortValues();
        return;
    }

    const bool ValidHostChar = (KeyAscii >= 'A' && KeyAscii <= 'Z')
        || (KeyAscii >= '0' && KeyAscii <= '9')
        || KeyAscii == '.'
        || KeyAscii == '-';

    if (mEditField == eEditField::RoomCode) {
        if ((KeyAscii >= 'A' && KeyAscii <= 'Z') || (KeyAscii >= '0' && KeyAscii <= '9'))
            Target->push_back((char)KeyAscii);
        return;
    }

    if (ValidHostChar)
        Target->push_back((char)std::tolower((unsigned char)KeyAscii));
}

void cMultiplayerMenu::SelectField(int16 pAction) {
    if (pAction == ACT_EDIT_REMOTE_HOST)
        mEditField = eEditField::RemoteHost;
    else if (pAction == ACT_EDIT_REMOTE_PORT)
        mEditField = eEditField::RemotePort;
    else if (pAction == ACT_EDIT_LOCAL_PORT)
        mEditField = eEditField::LocalPort;
    else if (pAction == ACT_EDIT_MAP_SEED)
        mEditField = eEditField::MapSeed;
    else if (pAction == ACT_EDIT_HUB_HOST)
        mEditField = eEditField::HubHost;
    else if (pAction == ACT_EDIT_HUB_PORT)
        mEditField = eEditField::HubPort;
    else if (pAction == ACT_EDIT_ROOM_CODE)
        mEditField = eEditField::RoomCode;

    g_Fodder->mInput_LastKey = g_Fodder->mKeyCode;
}

void cMultiplayerMenu::StartLanBrowser() {
    StopLanBrowser();

    mDiscovery = std::make_unique<cNetworkDiscovery>();
    mDiscoveryFailed = !mDiscovery->StartBrowser();
}

void cMultiplayerMenu::RefreshLanBrowser() {
    StartLanBrowser();
}

void cMultiplayerMenu::StopLanBrowser() {
    if (mDiscovery) {
        mDiscovery->Stop();
        mDiscovery.reset();
    }
}

void cMultiplayerMenu::StartInternetBrowser() {
    StopInternetBrowser();
    SyncPortValues();

    mHubClient = std::make_unique<cNetworkHubClient>();
    // OFHUB/2 surface — drop the legacy List() shim. ListAnonymous and List
    // are byte-identical today, but switching makes the OFHUB/2 boundary
    // grep-able and lets us delete the shim once the four call sites land.
    mHubFailed = !mHubClient->Configure(mHubHost, mHubPort) || !mHubClient->ListAnonymous(mInternetGames);
}

void cMultiplayerMenu::RefreshInternetBrowser() {
    StartInternetBrowser();
}

void cMultiplayerMenu::StopInternetBrowser() {
    mInternetGames.clear();
    mHubClient.reset();
    mHubFailed = false;
}

bool cMultiplayerMenu::CreateInternetRoom() {
    SyncPortValues();

    cNetworkHubClient Hub;
    if (!Hub.Configure(mHubHost, mHubPort)) {
        mHubFailed = true;
        return false;
    }

    const sNetworkHubMetadata Metadata = MultiplayerMenu_BuildHubMetadata(
        mGameMode,
        mMapSeed,
        mKillLimit,
        mTimeLimitSeconds,
        mTeamCount,
        mTeamSize,
        mFriendlyFire,
        mMapSize,
        mMapTerrain,
        mCoverDensity
    );

    // OFHUB/2 verified-host CREATE. The hub rejects /room/create without a
    // bearer JWT; RequireHubToken (called from ACT_HOST_INTERNET with
    // pBlockOnFail=true) should have warmed the cache by now, so this is
    // belt-and-braces. Stash the bearer locally so HeartbeatHost / UpdateAuth
    // can replay it without re-touching disk every tick.
    sHubAuthToken Auth;
    if (!mHubAuth.LoadCachedToken(Auth) || Auth.mJwt.empty()) {
        // Cache miss here is a token-expiry race against RequireHubToken (the
        // user paired, entered Host, then sat on the screen long enough for
        // the JWT to lapse before hitting Start). Mirror RequireHubToken's
        // pivot — re-prompt for pairing and bounce back to Host on success —
        // rather than surface the generic "HUB BROWSER FAILED" banner, which
        // misleads the user into thinking the hub is unreachable.
        mAuthTarget = eState::Host;
        mAuthBlockOnFail = true;
        mDeviceCode.clear();
        mPairCode.clear();
        mPairingError.clear();
        mState = eState::AuthPrompt;
        mEditField = eEditField::None;
        mHubFailed = false;
        return false;
    }

    sNetworkHubRoom Room;
    if (!Hub.CreateAuth(Auth, NETWORK_MAX_PLAYERS, Metadata, true, Room)) {
        mHubFailed = true;
        return false;
    }

    mInternet = true;
    mInternetHost = true;
    // Spec § 4.4.1: CREATEOK returns host=<ipv4> as the Fargate task's public
    // address. Gameplay traffic goes there, NOT to the hub. Fall back to the
    // resolved hub host for older relays that don't include host=.
    mRemoteHost = Room.mHostIp.size() ? Room.mHostIp : Room.mRelayHost;
    mRemotePort = Room.mRelayPort;
    mRemotePortText = std::to_string(mRemotePort);
    mRoomCode = Room.mRoomCode;
    // Capture the binary 32-byte session key + peer index for the data plane;
    // keeping the raw bytes here avoids an extra decode at REGISTER time.
    std::memcpy(mSessionKey.data(), Room.mSessionKey.data(), 32);
    mHasSessionKey = true;
    mPeerIndex = 0;             // host is always peer 0
    mHubBearer = Auth;          // stashed for UpdateAuth / HeartbeatHost
    mHubFailed = false;
    return true;
}

void cMultiplayerMenu::SelectDiscoveredGame(size_t pIndex) {
    if (!mDiscovery)
        return;

    const auto& Games = mDiscovery->GetGames();
    if (pIndex >= Games.size())
        return;

    const auto Game = Games[pIndex];
    if (!MultiplayerMenu_DiscoveryStateJoinable(Game.mState) ||
        !Game.mLobbyPort) {
        return;
    }

    mPlayerIndex = 1;
    mInternet = false;
    mInternetHost = false;
    mRoomCode.clear();
    mRemoteHost = Game.mHostAddress;
    mRemotePort = Game.mLobbyPort;
    mRemotePortText = std::to_string(mRemotePort);
    mLocalPort = 7001;
    mLocalPortText = std::to_string(mLocalPort);
    mGameMode = Game.mSettings.mGameMode;
    mMapSeed = Game.mSettings.mMapSeed;
    mMapSeedText = std::to_string(mMapSeed);
    mKillLimit = Game.mSettings.mKillLimit;
    mTimeLimitSeconds = Game.mSettings.mTimeLimitSeconds;
    mTeamCount = Game.mSettings.mTeamCount;
    mTeamSize = Game.mSettings.mTeamSize;
    mFriendlyFire = (Game.mSettings.mFriendlyFire != 0);
    mMapSize = Network_NormalizeMapSize((uint8_t)Game.mSettings.mMapSize);
    mMapTerrain = Network_NormalizeMapTerrain((uint8_t)Game.mSettings.mMapTerrain);
    mMapTerrainSub = Game.mSettings.mMapTerrainSub;
    mVehicleSet = Network_NormalizeVehicleSet((uint8_t)Game.mSettings.mVehicleSet);
    mPickupDensity = Network_NormalizePickupDensity((uint8_t)Game.mSettings.mPickupDensity);
    mCoverDensity = Network_NormalizeCoverDensity((uint8_t)Game.mSettings.mCoverDensity);

    StopLanBrowser();
    mDone = true;
    mStarted = true;
}

void cMultiplayerMenu::SelectInternetGame(size_t pIndex) {
    const bool JoinByCode = (pIndex == (size_t)-1);
    if (!JoinByCode && pIndex >= mInternetGames.size())
        return;

    SyncPortValues();

    std::string RoomCode = mRoomCode;
    if (!JoinByCode) {
        const sNetworkHubGame Game = mInternetGames[pIndex];
        if (!Game.mRelayPort || Game.mCurrentPlayers >= Game.mMaxPlayers)
            return;
        RoomCode = Game.mRoomCode;
    }
    if (RoomCode.empty())
        return;

    cNetworkHubClient Hub;
    if (!Hub.Configure(mHubHost, mHubPort)) {
        mHubFailed = true;
        return;
    }

    // OFHUB/2 anonymous JOIN. Spec § 4.4.2: bearer is OPTIONAL on JOIN —
    // verified-tier just gets higher quota, anonymous joiners are first-class.
    // Opportunistically warm the auth cache so future flows benefit, but fall
    // through anonymous either way.
    sHubAuthToken Auth;
    (void)mHubAuth.LoadCachedToken(Auth);

    sNetworkHubRoom Room;
    if (!Hub.JoinAnonymous(RoomCode, Room)) {
        mHubFailed = true;
        return;
    }

    mInternet = true;
    mInternetHost = false;
    mPlayerIndex = 1;
    // Same as the Create path: gameplay endpoint is the room's task IP, not
    // the hub IP. JOINOK echoes host=<ipv4>; mRelayHost is the hub fallback.
    mRemoteHost = Room.mHostIp.size() ? Room.mHostIp : Room.mRelayHost;
    mRemotePort = Room.mRelayPort;
    mRemotePortText = std::to_string(mRemotePort);
    mLocalPort = 7001;
    mLocalPortText = std::to_string(mLocalPort);
    mRoomCode = Room.mRoomCode;
    // Joiner gets its own per-peer session key from JOINOK; no bearer is
    // recorded (anonymous tier) so mHubBearer stays default-constructed.
    std::memcpy(mSessionKey.data(), Room.mSessionKey.data(), 32);
    mHasSessionKey = true;
    mPeerIndex = 1;
    mHubFailed = false;

    StopInternetBrowser();
    mDone = true;
    mStarted = true;
}

sRandomMapOptions cMultiplayerMenu::BuildMapOptions() const {
    sRandomMapOptions Options;
    Options.mGameMode = mGameMode;
    Options.mSeed = mMapSeed;
    Options.mMapSize = mMapSize;
    Options.mMapTerrain = mMapTerrain;
    Options.mMapTerrainSub = mMapTerrainSub;
    Options.mVehicleSet = mVehicleSet;
    Options.mPickupDensity = mPickupDensity;
    Options.mCoverDensity = mCoverDensity;
    Options.mProfile = eNetworkMapProfile_Custom;
    return Options;
}

void cMultiplayerMenu::ApplyMapOptions(const sRandomMapOptions& pOptions) {
    mGameMode = pOptions.mGameMode;
    mMapSeed = pOptions.mSeed;
    mMapSeedText = std::to_string(mMapSeed);
    mMapSize = pOptions.mMapSize;
    mMapTerrain = pOptions.mMapTerrain;
    mMapTerrainSub = (uint8)pOptions.mMapTerrainSub;
    mVehicleSet = pOptions.mVehicleSet;
    mPickupDensity = pOptions.mPickupDensity;
    mCoverDensity = pOptions.mCoverDensity;
}

void cMultiplayerMenu::OpenMapOptions() {
    mMapOptionsMenu.Open(BuildMapOptions(), cRandomMapOptionsMenu::eContext::Multiplayer);
    mState = eState::MapOptions;
    mEditField = eEditField::None;
}

bool cMultiplayerMenu::CanStart() const {
    uint16 ParsedRemotePort = 0;
    uint16 ParsedLocalPort = 0;
    uint16 ParsedHubPort = 0;
    uint32 ParsedSeed = 0;
    const bool CommonOk = MultiplayerMenu_PortFromText(mLocalPortText, ParsedLocalPort)
        && NetworkMenu_UInt32FromText(mMapSeedText, ParsedSeed);

    if (!CommonOk)
        return false;

    if (mInternetHost)
        return mHubHost.size() && MultiplayerMenu_PortFromText(mHubPortText, ParsedHubPort);

    return mRemoteHost.size() && MultiplayerMenu_PortFromText(mRemotePortText, ParsedRemotePort);
}

void cMultiplayerMenu::SyncPortValues() {
    uint16 ParsedPort = 0;
    if (MultiplayerMenu_PortFromText(mRemotePortText, ParsedPort))
        mRemotePort = ParsedPort;

    if (MultiplayerMenu_PortFromText(mLocalPortText, ParsedPort))
        mLocalPort = ParsedPort;
    if (MultiplayerMenu_PortFromText(mHubPortText, ParsedPort))
        mHubPort = ParsedPort;

    uint32 ParsedSeed = 0;
    if (NetworkMenu_UInt32FromText(mMapSeedText, ParsedSeed))
        mMapSeed = ParsedSeed;
}

// ============================================================
// Public-matchmaking gating: token check + Discord pairing modal.
// ============================================================

bool cMultiplayerMenu::RequireHubToken(eState pTarget, bool pBlockOnFail) {
    sHubAuthToken Existing;
    if (mHubAuth.LoadCachedToken(Existing))
        return true;        // happy path — caller continues

    // Stash the redirect target + the host/join policy difference; the auth
    // modal callback uses both.  Drawing happens out of DrawAuthPromptMenu.
    mAuthTarget = pTarget;
    mAuthBlockOnFail = pBlockOnFail;
    mDeviceCode.clear();
    mPairCode.clear();
    mPairingError.clear();
    mState = eState::AuthPrompt;
    mEditField = eEditField::None;
    return false;
}

void cMultiplayerMenu::SubmitPairCode() {
    if (mDeviceCode.empty() || mPairCode.empty()) {
        mPairingError = "MISSING DEVICE OR PAIR CODE";
        return;
    }

    sHubAuthToken Token;
    if (!mHubAuth.ClaimToken(mPairCode, mDeviceCode, Token)) {
        mPairingError = mHubAuth.GetLastError();
        return;
    }

    // ClaimToken already wrote the token to disk via SaveToken; do it again
    // belt-and-braces so a soft-failure on the first save (e.g. transient
    // permission glitch) is retried before we drop back into the menu.
    if (Token.mExpiry > 0)
        (void)mHubAuth.SaveToken(Token);

    mPairCode.clear();
    mPairingError.clear();
    mDeviceCode.clear();

    // Token in hand — enter the originally-requested state.  Mirrors what
    // ACT_HOST_INTERNET / ACT_FIND_INTERNET would have done if the cache
    // had been warm to begin with.
    const eState Target = mAuthTarget;
    mAuthTarget = eState::Main;
    mState = Target;
    if (Target == eState::Host) {
        mPlayerIndex = 0;
        mInternet = true;
        mInternetHost = true;
        mRoomCode.clear();
        mLocalPort = 7000;
        mLocalPortText = std::to_string(mLocalPort);
    } else if (Target == eState::FindInternet) {
        mPlayerIndex = 1;
        mInternet = true;
        mInternetHost = false;
        StartInternetBrowser();
    }
}

void cMultiplayerMenu::HandleAuthTextInput() {
    if (mState != eState::AuthPairing)
        return;

    int16 KeyAscii = 0;
    if (g_Fodder->mKeyCode != g_Fodder->mInput_LastKey) {
        g_Fodder->mInput_LastKey = g_Fodder->mKeyCode;
        const int Kc = g_Fodder->mKeyCode;
        if (Kc >= SDL_SCANCODE_A && Kc <= SDL_SCANCODE_Z)
            KeyAscii = 'A' + (Kc - SDL_SCANCODE_A);
        else if (Kc >= SDL_SCANCODE_1 && Kc <= SDL_SCANCODE_9)
            KeyAscii = '1' + (Kc - SDL_SCANCODE_1);
        else if (Kc == SDL_SCANCODE_0)
            KeyAscii = '0';
        else if (Kc == SDL_SCANCODE_BACKSPACE)
            KeyAscii = 8;
        else if (Kc == SDL_SCANCODE_RETURN || Kc == SDL_SCANCODE_KP_ENTER)
            KeyAscii = 0x0D;
    }

    if (!KeyAscii)
        return;

    if (KeyAscii == 0x0D) {
        if (mPairCode.size() == 6 && !mDeviceCode.empty())
            SubmitPairCode();
        return;
    }
    if (KeyAscii == 8) {
        if (!mPairCode.empty())
            mPairCode.pop_back();
        return;
    }
    if (mPairCode.size() < 6)
        mPairCode.push_back((char)KeyAscii);
}

void cMultiplayerMenu::DrawAuthPromptMenu() {
    mDrawStrings.clear();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("PUBLIC MATCHMAKING", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    // The "small modal" the task asks for is just a centred body block over
    // the empty briefing background plus two buttons.  Single confirm/cancel
    // pair, mouse-driven — the briefing renderer doesn't have z-ordered
    // overlays so a real popup would clobber its own click targets.
    int16 rowY = 0x40;

    // Two-line wrapped explanation. Wording matches the task spec verbatim.
    const char* Lines[] = {
        "PUBLIC MATCHMAKING REQUIRES",
        "A ONE TIME DISCORD PAIRING",
        "",
        "OPEN IN BROWSER ?",
    };
    for (auto* L : Lines) {
        if (*L)
            g_Fodder->String_Print_Small_CentreInBox(L, 0x10, 0x130, rowY);
        rowY += 0x0E;
    }

    // OPEN BROWSER (confirm) + CANCEL.
    const size_t yBottom = 0xA8;
    const size_t buttonW = 0x60;
    const size_t gap = 0x08;
    const size_t totalW = (buttonW * 2) + gap;
    const size_t xStart = 160 - (totalW / 2);
    const size_t xConfirmL = xStart;                  const size_t xConfirmR = xConfirmL + buttonW;
    const size_t xCancelL  = xConfirmR + gap;          const size_t xCancelR  = xCancelL + buttonW;

    g_Fodder->GUI_Button_Draw_SmallBoxAt("OPEN BROWSER", xConfirmL, xConfirmR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_AUTH_CONFIRM);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("CANCEL", xCancelL, xCancelR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_AUTH_CANCEL);
}

void cMultiplayerMenu::DrawAuthPairingMenu() {
    mDrawStrings.clear();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("ENTER PAIR CODE", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    int16 rowY = 0x32;
    g_Fodder->String_Print_Small_CentreInBox(
        "TYPE THE 6 CHARACTER CODE FROM THE WEB PAGE", 0x10, 0x130, rowY);
    rowY += 0x18;

    // Pair code field, centred and spaced for legibility.
    std::string Display = mPairCode;
    while (Display.size() < 6) Display.push_back('_');
    if (Display.size() > 6) Display = Display.substr(0, 6);
    std::string Spaced;
    for (size_t i = 0; i < Display.size(); ++i) {
        if (i) Spaced.push_back(' ');
        Spaced.push_back(Display[i]);
    }
    g_Fodder->String_Print_Small_CentreInBox(Spaced, 0x60, 0xE0, rowY);
    rowY += 0x18;

    // Status / error line (visible only when there's something to say —
    // collapses cleanly when empty so the layout doesn't jitter).
    if (!mPairingError.empty()) {
        g_Fodder->String_Print_Small_CentreInBox(mPairingError, 0x10, 0x130, rowY);
    } else if (mDeviceCode.empty()) {
        g_Fodder->String_Print_Small_CentreInBox(
            "BROWSER DID NOT OPEN  USE REOPEN", 0x10, 0x130, rowY);
    }

    // PAIR / REOPEN / CANCEL.
    const size_t yBottom = 0xB8;
    const size_t buttonW = 0x4A;
    const size_t gap = 0x06;
    const size_t totalW = (buttonW * 3) + (gap * 2);
    const size_t xStart = 160 - (totalW / 2);
    const size_t xPairL   = xStart;                   const size_t xPairR   = xPairL + buttonW;
    const size_t xRetryL  = xPairR + gap;              const size_t xRetryR  = xRetryL + buttonW;
    const size_t xCancelL = xRetryR + gap;             const size_t xCancelR = xCancelL + buttonW;

    const bool CanSubmit = (mPairCode.size() >= 4) && !mDeviceCode.empty();
    g_Fodder->GUI_Button_Draw_SmallBoxAt("PAIR", xPairL, xPairR, yBottom,
        CanSubmit ? 0xB2 : 0xC8, CanSubmit ? 0xB3 : 0xC8, eTextAlign::Centre);
    if (CanSubmit)
        g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_AUTH_PAIR);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("REOPEN", xRetryL, xRetryR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_AUTH_REOPEN);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("CANCEL", xCancelL, xCancelR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_AUTH_CANCEL);
}

// ============================================================
// Multiplayer_Menu_Run — hosted on cFodderMultiplayer
// Follows the same pattern as Options_Menu_Run.
// ============================================================

bool cFodderMultiplayer::Multiplayer_Menu_Run() {
    if (mLobby) {
        mLobby->Stop();
        mLobby.reset();
    }

    mMultiplayerMenu = std::make_unique<cMultiplayerMenu>();
    mMultiplayerMenu->Open();

    mMap_Destroy_Tiles.clear();
    mMouse_Button_Left_Toggle = 0;

    const auto prevInterruptCallback = mInterruptCallback;
    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();

    mInterruptCallback = [this]() {
        Sprites_Draw();
        mMultiplayerMenu->Draw();
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        Mouse_DrawCursor();
    };

    while (!mMultiplayerMenu->IsDone()) {
        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        if (Mouse_Button_Left_Toggled() >= 0)
            GUI_Handle_Element_Mouse_Check(mGUI_Elements);

        if (mPhase_EscapeKeyAbort || mPhase_Aborted) {
            mPhase_EscapeKeyAbort = false;
            mPhase_Aborted = false;
            mMultiplayerMenu->OnBack();
        }

        mMultiplayerMenu->Tick();
        Video_Sleep();
    }

    const bool started = mMultiplayerMenu->WasStarted();

    if (started) {
        // Configure mStartParams from the menu results
        mStartParams->mNetworkEnabled = true;
        mStartParams->mNetworkPlayerIndex = mMultiplayerMenu->GetPlayerIndex();
        mStartParams->mNetworkRemoteHost  = mMultiplayerMenu->GetRemoteHost();
        mStartParams->mNetworkRemotePort  = mMultiplayerMenu->GetRemotePort();
        mStartParams->mNetworkLocalPort   = mMultiplayerMenu->GetLocalPort();
        mStartParams->mNetworkSyncTest    = mMultiplayerMenu->IsSyncTest();
        mStartParams->mNetworkInternet    = mMultiplayerMenu->IsInternet();
        mStartParams->mNetworkHubHost     = mMultiplayerMenu->GetHubHost();
        mStartParams->mNetworkHubPort     = mMultiplayerMenu->GetHubPort();
        mStartParams->mNetworkRoomCode    = mMultiplayerMenu->GetRoomCode();
        // OFHUB/2 plumbing into StartParams. The data plane (Fodder_Network.cpp,
        // a different agent) reads the binary session key + peer index when
        // building REGISTER/HEARTBEAT tags; the lobby loop replays the host
        // bearer for UpdateAuth / HeartbeatHost. Joiner sessions leave the
        // bearer empty (anonymous tier).
        mStartParams->mNetworkSessionKey      = mMultiplayerMenu->GetSessionKey();
        mStartParams->mNetworkSessionKeyValid = mMultiplayerMenu->HasSessionKey();
        mStartParams->mNetworkPeerIndex       = mMultiplayerMenu->GetPeerIndex();
        mStartParams->mNetworkHostBearer      = mMultiplayerMenu->GetHubBearer().mJwt;
        mStartParams->mNetworkGameMode    = mMultiplayerMenu->GetGameMode();
        mStartParams->mNetworkMapSeed     = mMultiplayerMenu->GetMapSeed();
        mStartParams->mNetworkKillLimit   = mMultiplayerMenu->GetKillLimit();
        mStartParams->mNetworkTimeLimitSeconds = mMultiplayerMenu->GetTimeLimitSeconds();
        mStartParams->mNetworkTeamCount   = mMultiplayerMenu->GetTeamCount();
        mStartParams->mNetworkTeamSize    = mMultiplayerMenu->GetTeamSize();
        mStartParams->mNetworkFriendlyFire = mMultiplayerMenu->GetFriendlyFire();
        mStartParams->mNetworkMapSize     = mMultiplayerMenu->GetMapSize();
        mStartParams->mNetworkMapTerrain  = mMultiplayerMenu->GetMapTerrain();
        mStartParams->mNetworkMapTerrainSub = mMultiplayerMenu->GetMapTerrainSub();
        mStartParams->mNetworkVehicleSet  = mMultiplayerMenu->GetVehicleSet();
        mStartParams->mNetworkPickupDensity = mMultiplayerMenu->GetPickupDensity();
        mStartParams->mNetworkCoverDensity = mMultiplayerMenu->GetCoverDensity();

        // Start the UDP lobby immediately for campaign selection sync
        if (!mMultiplayerMenu->IsSyncTest()) {
            mLobby = std::make_unique<cNetworkLobby>();
            bool lobbyOk = mLobby->Start(
                mStartParams->mNetworkLocalPort,
                mStartParams->mNetworkRemoteHost,
                mStartParams->mNetworkRemotePort,
                mStartParams->mNetworkPlayerIndex == 0,  // host = player 0
                mStartParams->mNetworkSessionKey,
                mStartParams->mNetworkPeerIndex,
                mStartParams->mNetworkInternet
            );
            if (!lobbyOk) {
                g_Debugger->Error("[Lobby] Failed to start lobby, aborting.");
                mLobby.reset();
                mStartParams->mNetworkEnabled = false;
            }
        }
    }

    mInterruptCallback = prevInterruptCallback;
    mMultiplayerMenu->Close();
    mMultiplayerMenu.reset();

    mMouse_Button_Left_Toggle = 0;
    mMouse_EventLastButtonsPressed = 0;
    mKeyCode = 0;
    mKeyCodeAscii = 0;
    mInput_LastKey = 0;
    return started;
}

bool cFodderMultiplayer::Multiplayer_ReopenLobby() {
    if (!mStartParams->mNetworkEnabled || mStartParams->mNetworkSyncTest)
        return false;

    if (mLobby) {
        mLobby->Stop();
    }
    else {
        mLobby = std::make_unique<cNetworkLobby>();
    }

    const bool LobbyOk = mLobby->Start(
        mStartParams->mNetworkLocalPort,
        mStartParams->mNetworkRemoteHost,
        mStartParams->mNetworkRemotePort,
        mStartParams->mNetworkPlayerIndex == 0,
        mStartParams->mNetworkSessionKey,
        mStartParams->mNetworkPeerIndex,
        mStartParams->mNetworkInternet
    );

    if (!LobbyOk) {
        g_Debugger->Error("[Lobby] Failed to reopen lobby after match.");
        mLobby.reset();
        mStartParams->mNetworkEnabled = false;
        return false;
    }

    Lobby_CampaignSelection();
    return mStartParams->mNetworkEnabled;
}

#endif // OPENFODDER_ENABLE_NETWORK
