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

cMultiplayerMenu::cMultiplayerMenu() {
}

cMultiplayerMenu::~cMultiplayerMenu() {
}

void cMultiplayerMenu::Open() {
    mDone = false;
    mStarted = false;
    mState = eState::Main;
}

void cMultiplayerMenu::Close() {
}

void cMultiplayerMenu::Tick() {
}

void cMultiplayerMenu::OnBack() {
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
        mPlayerIndex = 0;
        mLocalPort = 7000;
        mRemotePort = 7001;
        break;

    case ACT_JOIN:
        mState = eState::Join;
        mPlayerIndex = 1;
        mLocalPort = 7001;
        mRemotePort = 7000;
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
        mDone = true;
        mStarted = true;
        break;

    case ACT_IP_INPUT:
        // IP address input — handled by the text input system
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
    mDrawStrings.clear();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("HOST GAME", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    size_t YOffset = PLATFORM_BASED(0, 25);
    int16 rowY = 0x44;

    // Show current settings
    {
        std::string portStr = "LOCAL PORT  " + std::to_string(mLocalPort);
        mDrawStrings.push_back(portStr);
        g_Fodder->String_Print_Small(mDrawStrings.back(), 0x20, rowY);
    }

    rowY += 0x20;

    // START
    g_Fodder->GUI_Button_Draw_Small("START", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_START);

    // BACK
    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
}

void cMultiplayerMenu::DrawJoinMenu() {
    mDrawStrings.clear();

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("JOIN GAME", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    size_t YOffset = PLATFORM_BASED(0, 25);
    int16 rowY = 0x44;

    // Show current settings
    {
        // The briefing font has no '.' glyph width, so replace dots
        std::string displayHost = mRemoteHost;
        std::replace(displayHost.begin(), displayHost.end(), '.', ' ');

        std::string hostStr = "HOST  " + displayHost;
        mDrawStrings.push_back(hostStr);
        g_Fodder->String_Print_Small(mDrawStrings.back(), 0x20, rowY);
        rowY += 0x12;

        std::string portStr = "PORT  " + std::to_string(mRemotePort);
        mDrawStrings.push_back(portStr);
        g_Fodder->String_Print_Small(mDrawStrings.back(), 0x20, rowY);
    }

    rowY += 0x20;

    // START
    g_Fodder->GUI_Button_Draw_Small("START", rowY, 0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_START);

    // BACK
    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
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
