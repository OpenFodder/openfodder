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

static bool MultiplayerMenu_UInt32FromText(const std::string& pText, uint32& pValue)
{
    if (pText.empty())
        return false;

    uint64_t Value = 0;
    for (char Char : pText)
    {
        if (Char < '0' || Char > '9')
            return false;

        Value = (Value * 10) + (uint64_t)(Char - '0');
        if (Value > 0xFFFFFFFFu)
            return false;
    }

    pValue = (uint32)Value;
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

static std::string MultiplayerMenu_FitText(const std::string& pText, int pMaxPx)
{
    std::string Result;
    int Width = 0;

    for (char RawChar : pText)
    {
        unsigned char Char = (unsigned char)std::toupper((unsigned char)RawChar);
        if (!((Char >= 'A' && Char <= 'Z') || (Char >= '0' && Char <= '9') || Char == ' '))
            Char = ' ';

        const int CharWidth = (int)mFont_Briefing_Width[Char];
        if (Width + CharWidth > pMaxPx)
            break;

        Result.push_back((char)Char);
        Width += CharWidth;
    }

    while (!Result.empty() && Result.back() == ' ')
        Result.pop_back();

    return Result;
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
    case eNetworkMenuStart_Main:
    case eNetworkMenuStart_None:
    default:
        break;
    }
}

void cMultiplayerMenu::Close() {
    StopLanBrowser();
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

    if (mState == eState::MapOptions) {
        mMapOptionsMenu.OnBack();
    } else if (mState == eState::Host || mState == eState::Join || mState == eState::FindLan) {
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
        mRemoteHost = "127.0.0.1";
        mLocalPort = 7000;
        mRemotePort = 7001;
        mLocalPortText = std::to_string(mLocalPort);
        mRemotePortText = std::to_string(mRemotePort);
        break;

    case ACT_FIND_LAN:
        mState = eState::FindLan;
        mEditField = eEditField::None;
        mPlayerIndex = 1;
        StartLanBrowser();
        break;

    case ACT_DIRECT_CONNECT:
    case ACT_JOIN:
        mState = eState::Join;
        mEditField = eEditField::None;
        mPlayerIndex = 1;
        mLocalPort = 7001;
        mRemotePort = 7000;
        mLocalPortText = std::to_string(mLocalPort);
        mRemotePortText = std::to_string(mRemotePort);
        break;

    case ACT_JOIN_DISCOVERED:
        SelectDiscoveredGame((size_t)pArg);
        break;

    case ACT_REFRESH_LAN:
        RefreshLanBrowser();
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
            mDone = true;
            mStarted = true;
        }
        break;

    case ACT_EDIT_REMOTE_HOST:
    case ACT_EDIT_REMOTE_PORT:
    case ACT_EDIT_LOCAL_PORT:
    case ACT_EDIT_MAP_SEED:
        SelectField(pAction);
        break;

    case ACT_CYCLE_MODE:
        mEditField = eEditField::None;
        mGameMode = Network_NormalizeGameMode((uint8_t)mGameMode + 1);
        break;

    case ACT_MAP_OPTIONS:
        OpenMapOptions();
        break;

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
    case eState::MapOptions:
        mMapOptionsMenu.Draw();
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
    DrawConnectionMenu("HOST LAN GAME", "PEER IP", "PEER PORT", true);
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
            std::string Label = Game.mHostName.size() ? Game.mHostName : Game.mHostAddress;
            Label += " ";
            Label += MultiplayerMenu_ModeShortName(Game.mSettings.mGameMode);
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
            Label = MultiplayerMenu_FitText(Label, 190);

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
    int16 rowY = pHostSetup ? 0x34 : 0x44;

    if (pHostSetup) {
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
    const std::string FittedValue = MultiplayerMenu_FitText(pValue, (int)(FieldX2 - FieldX1 - 4));

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
    const std::string FittedValue = MultiplayerMenu_FitText(Value, (int)(FieldX2 - FieldX1 - 4));

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
            if (!MultiplayerMenu_UInt32FromText(Candidate, ParsedSeed))
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
    mVehicleSet = Network_NormalizeVehicleSet((uint8_t)Game.mSettings.mVehicleSet);
    mPickupDensity = Network_NormalizePickupDensity((uint8_t)Game.mSettings.mPickupDensity);
    mCoverDensity = Network_NormalizeCoverDensity((uint8_t)Game.mSettings.mCoverDensity);

    StopLanBrowser();
    mDone = true;
    mStarted = true;
}

sRandomMapOptions cMultiplayerMenu::BuildMapOptions() const {
    sRandomMapOptions Options;
    Options.mGameMode = mGameMode;
    Options.mSeed = mMapSeed;
    Options.mMapSize = mMapSize;
    Options.mMapTerrain = mMapTerrain;
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
    uint32 ParsedSeed = 0;
    return mRemoteHost.size()
        && MultiplayerMenu_PortFromText(mRemotePortText, ParsedRemotePort)
        && MultiplayerMenu_PortFromText(mLocalPortText, ParsedLocalPort)
        && MultiplayerMenu_UInt32FromText(mMapSeedText, ParsedSeed);
}

void cMultiplayerMenu::SyncPortValues() {
    uint16 ParsedPort = 0;
    if (MultiplayerMenu_PortFromText(mRemotePortText, ParsedPort))
        mRemotePort = ParsedPort;

    if (MultiplayerMenu_PortFromText(mLocalPortText, ParsedPort))
        mLocalPort = ParsedPort;

    uint32 ParsedSeed = 0;
    if (MultiplayerMenu_UInt32FromText(mMapSeedText, ParsedSeed))
        mMapSeed = ParsedSeed;
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
        mStartParams->mNetworkGameMode    = mMultiplayerMenu->GetGameMode();
        mStartParams->mNetworkMapSeed     = mMultiplayerMenu->GetMapSeed();
        mStartParams->mNetworkKillLimit   = mMultiplayerMenu->GetKillLimit();
        mStartParams->mNetworkTimeLimitSeconds = mMultiplayerMenu->GetTimeLimitSeconds();
        mStartParams->mNetworkTeamCount   = mMultiplayerMenu->GetTeamCount();
        mStartParams->mNetworkTeamSize    = mMultiplayerMenu->GetTeamSize();
        mStartParams->mNetworkFriendlyFire = mMultiplayerMenu->GetFriendlyFire();
        mStartParams->mNetworkMapSize     = mMultiplayerMenu->GetMapSize();
        mStartParams->mNetworkMapTerrain  = mMultiplayerMenu->GetMapTerrain();
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
                mStartParams->mNetworkPlayerIndex == 0  // host = player 0
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
        mStartParams->mNetworkPlayerIndex == 0
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
