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

cMultiplayerMenu::cMultiplayerMenu() {
}

cMultiplayerMenu::~cMultiplayerMenu() {
}

void cMultiplayerMenu::Open() {
    mDone = false;
    mStarted = false;
    mEditField = eEditField::None;
    mState = eState::Main;
    mRemoteHost = g_Fodder->mStartParams->mNetworkRemoteHost.size()
        ? g_Fodder->mStartParams->mNetworkRemoteHost
        : "127.0.0.1";
}

void cMultiplayerMenu::Close() {
}

void cMultiplayerMenu::Tick() {
    HandleTextInput();
}

void cMultiplayerMenu::OnBack() {
    if (mEditField != eEditField::None) {
        mEditField = eEditField::None;
        return;
    }

    if (mState == eState::Host || mState == eState::Join) {
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
        mLocalPort = 7000;
        mRemotePort = 7001;
        mLocalPortText = std::to_string(mLocalPort);
        mRemotePortText = std::to_string(mRemotePort);
        break;

    case ACT_JOIN:
        mState = eState::Join;
        mEditField = eEditField::None;
        mPlayerIndex = 1;
        mLocalPort = 7001;
        mRemotePort = 7000;
        mLocalPortText = std::to_string(mLocalPort);
        mRemotePortText = std::to_string(mRemotePort);
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
        SelectField(pAction);
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
    int16 rowY = 0x44;
    int16 rowH = 0x15;

    // HOST GAME
    g_Fodder->GUI_Button_Draw_Small("HOST GAME", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_HOST);

    rowY += rowH;

    // JOIN GAME
    g_Fodder->GUI_Button_Draw_Small("JOIN GAME", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_JOIN);

    rowY += rowH;

    // SYNC TEST
    g_Fodder->GUI_Button_Draw_Small("SYNC TEST", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_SYNC_TEST);

    // BACK
    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
}

void cMultiplayerMenu::DrawHostMenu() {
    DrawConnectionMenu("HOST GAME", "PEER IP", "PEER PORT");
}

void cMultiplayerMenu::DrawJoinMenu() {
    DrawConnectionMenu("JOIN GAME", "HOST IP", "HOST PORT");
}

void cMultiplayerMenu::DrawConnectionMenu(const char* pTitle, const char* pRemoteHostLabel, const char* pRemotePortLabel) {
    mDrawStrings.clear();
    SyncPortValues();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large(pTitle, false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    size_t YOffset = PLATFORM_BASED(0, 25);
    int16 rowY = 0x44;

    DrawField(pRemoteHostLabel, MultiplayerMenu_DisplayHost(mRemoteHost), rowY, ACT_EDIT_REMOTE_HOST, mEditField == eEditField::RemoteHost);
    rowY += 0x18;
    DrawField(pRemotePortLabel, mRemotePortText, rowY, ACT_EDIT_REMOTE_PORT, mEditField == eEditField::RemotePort);
    rowY += 0x18;
    DrawField("LOCAL PORT", mLocalPortText, rowY, ACT_EDIT_LOCAL_PORT, mEditField == eEditField::LocalPort);
    rowY += 0x24;

    // START
    g_Fodder->GUI_Button_Draw_Small("START", rowY, CanStart() ? 0xB2 : 0xF2, CanStart() ? 0xB3 : 0xF3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_START);

    // BACK
    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
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
        if (!MultiplayerMenu_PortFromText(Candidate, ParsedPort))
            return;

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

    g_Fodder->mInput_LastKey = g_Fodder->mKeyCode;
}

bool cMultiplayerMenu::CanStart() const {
    uint16 ParsedRemotePort = 0;
    uint16 ParsedLocalPort = 0;
    return mRemoteHost.size()
        && MultiplayerMenu_PortFromText(mRemotePortText, ParsedRemotePort)
        && MultiplayerMenu_PortFromText(mLocalPortText, ParsedLocalPort);
}

void cMultiplayerMenu::SyncPortValues() {
    uint16 ParsedPort = 0;
    if (MultiplayerMenu_PortFromText(mRemotePortText, ParsedPort))
        mRemotePort = ParsedPort;

    if (MultiplayerMenu_PortFromText(mLocalPortText, ParsedPort))
        mLocalPort = ParsedPort;
}

// ============================================================
// Multiplayer_Menu_Run — hosted on cFodderMultiplayer
// Follows the same pattern as Options_Menu_Run.
// ============================================================

bool cFodderMultiplayer::Multiplayer_Menu_Run() {
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

        if (mMouse_Button_Left_Toggle)
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

#endif // OPENFODDER_ENABLE_NETWORK
