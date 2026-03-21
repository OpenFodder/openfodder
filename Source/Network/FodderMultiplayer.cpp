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

cFodderMultiplayer::cFodderMultiplayer(std::shared_ptr<cWindow> pWindow)
    : cFodder(pWindow)
{
}

cFodderMultiplayer::~cFodderMultiplayer() {
    Network_Stop();
}

// ============================================================
// Start() override
// If network mode is active, show the multiplayer menu before
// campaign selection.  Otherwise fall through to cFodder::Start().
// ============================================================

void cFodderMultiplayer::Start() {

    if (!mStartParams->mNetworkEnabled) {
        cFodder::Start();
        return;
    }

    if (mParams->mShowAbout) {
        About();
        return;
    }

    if (mParams->mDemoPlayback) {
        Demo_Load();
        mGame_Data.mDemoRecorded.playback();
        mParams = mGame_Data.mDemoRecorded.mParams;
        mParams->mDefaultPlatform = mStartParams->mDefaultPlatform;
        mOpenFodder_Intro_Done = false;
    }

    if (mParams->mDemoRecord)
        mGame_Data.mDemoRecorded.clear();

Start:;
    mGame_Data.mCampaign.Clear();
    mSound = 0;
    mVersionDefault = 0;
    mVersionCurrent = 0;

    VersionSwitch(mVersions->GetRetail(mParams->mDefaultPlatform, mParams->mDefaultGame));

    if (!mVersionCurrent) {
        VersionSwitch(mVersions->GetDemo());
        if (!mVersionCurrent)
            return;
    }

    if (mParams->mDemoRecord && mGame_Data.mDemoRecorded.mRecordedPlatform == ePlatform::Any)
        mGame_Data.mDemoRecorded.mRecordedPlatform = mVersionCurrent->mPlatform;

    mGame_Data.mDemoRecorded.save();

    Intro_OpenFodder();

    if (mParams->mPlayground) {
        Playground();
        return;
    }

    // Skip campaign selection if already specified on command line
    if (mParams->mCampaignName.size() && Campaign_Load(mParams->mCampaignName)) {
        // Campaign loaded from CLI
    } else {
        // Show campaign selection with multiplayer button
        Campaign_Selection();

        if (mGUI_SaveLoadAction == 1)
            return;

        if (mGUI_SaveLoadAction == 4) {
            About();
            goto Start;
        }
    }

    mVersionDefault = mVersionCurrent;
    Mouse_Setup();
    Mouse_ReadInputs();

    if (Engine_Loop())
        goto Start;
}

// ============================================================
// Phase_Prepare_Network() override
// Called at the end of Phase_Prepare to start the GGPO session.
// ============================================================

void cFodderMultiplayer::Phase_Prepare_Network() {
    if (!mStartParams->mNetworkEnabled)
        return;

    // Always stop and restart the GGPO session between phases.
    if (mNetSession) {
        g_Debugger->Notice("[GGPO] Phase_Prepare: stopping previous session.");
        Network_Stop();
    }
    {
        g_Debugger->Notice("[GGPO] Phase_Prepare: starting network session...");
        if (!Network_Start())
            g_Debugger->Error("[GGPO] Warning: failed to start network session; reverting to single-player.");
    }

    // Redistribute squads every phase.
    if (mNetSession && mNetSession->IsRunning()) {
        Network_RedistributeSquads();
        mNet_P2CamInitialised = false;
        memset(mNet_WalkTargetX, 0, sizeof(mNet_WalkTargetX));
        memset(mNet_WalkTargetY, 0, sizeof(mNet_WalkTargetY));
        memset(mNet_CameraPanTargetX, 0, sizeof(mNet_CameraPanTargetX));
        memset(mNet_CameraPanTargetY, 0, sizeof(mNet_CameraPanTargetY));
    }
}

// ============================================================
// Mission_Loop() override
// Same as cFodder::Mission_Loop() but uses network recruit/briefing
// and calls Network_Stop() between phases.
// ============================================================

int16 cFodderMultiplayer::Mission_Loop() {

    if (!mStartParams->mNetworkEnabled)
        return cFodder::Mission_Loop();

    for (;;) {
        mGame_Data.mDemoRecorded.save();

        if (GameOverCheck())
            return -1;

        Phase_EngineReset();
        Phase_SquadPrepare();

        mInput_Enabled = false;

        Intro_Main();

        // Single / Random Map mode
        if (mCustom_Mode == eCustomMode_Map) {
            if (mVersionDefault->mName == "Random Map") {
                sMapParams Params(mRandom.get());
                CreateRandom(Params);
                mGame_Data.mMission_Recruitment = 0;
            } else {
                Custom_ShowMapSelection();
            }

            if (mCustom_Mode == eCustomMode_None)
                return -1;
        } else {
            // Prepare a new game?
            if (mGame_Data.mMission_Recruitment && !mParams->mSkipRecruit) {
                mGame_Data.mMission_Recruitment = 0;
                Network_Recruit_Show();
            }
        }

        mMusic_SlowVolumeDecrease = true;
        WindowTitleSet(true);

        Network_Briefing_Show();

        while (Music_Decrease_Channel_Volume()) {
            sleepLoop(1);
        }

        if (mStartParams->mDemoPlayback || mStartParams->mDemoRecord) {
            std::lock_guard<std::mutex> lock(g_Fodder->mSurfaceMtx);
            mVideo_Done = true;
        }

        Phase_Prepare();
        mMusic_SlowVolumeDecrease = false;

        if (!Phase_Loop()) {
            mKeyCode = 0;
            mPhase_In_Progress = false;
            Network_Stop();
            Squad_Member_PhaseCount();
            mPhase_TryingAgain = true;
        } else {
            mKeyCode = 0;
            mPhase_In_Progress = false;
            Network_Stop();

            // Game over?
            if (!mGame_Data.mRecruits_Available_Count) {
                if (mVersionCurrent->isRetail() || mCustom_Mode == eCustomMode_Set) {
                    if (!mPhase_Aborted)
                        Service_Show();
                }
                break;
            }
        }

        if (mParams->mSinglePhase)
            return 0;

        if (mPhase_TryAgain) {
            mPhase_TryingAgain = true;
            continue;
        }

        if (mVersionReturnAfterPhase) {
            mVersionReturnAfterPhase = false;
            VersionSwitch(mVersionDefault);
        }

        if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set) {
            if (!mPhase_Aborted && mVersionCurrent->isCustom())
                Service_Show();

            if (!mVersionCurrent->isAmigaTheOne())
                break;
        }

        if (mPhase_Aborted)
            continue;

        if (mGame_Data.mMission_Phases_Remaining > 1)
            continue;

        Service_Show();
    }

    return 0;
}

// ============================================================
// Phase_Loop() override
// Runs the GGPO network loop instead of the normal Phase_Cycle()
// / Video_Sleep() pairing.
// ============================================================

int16 cFodderMultiplayer::Phase_Loop() {

    if (!mStartParams->mNetworkEnabled || !mNetSession || !mNetSession->IsRunning())
        return cFodder::Phase_Loop();

    int16 result = 1;
    int netInterruptTicks = 0;
    bool wasWaiting = true;
    bool mouseReleasedForWait = false;
    bool firstSimFrameDone = false;

    while (!mExit) {
        // Wait for the next interrupt tick (~20 ms at default mSleepDelta).
        mVideo_Ticked = false;
        mVideo_Done   = true;
        while (!mVideo_Ticked && !mExit)
            SDL_Delay(1);

        // Pump GGPO during the waiting phase so handshake packets flow.
        mNetSession->Idle(0);

        // While waiting for peer synchronization, show "WAITING FOR PLAYER"
        if (!mNetSession->IsSessionReady()) {
            if (!mouseReleasedForWait) {
                mWindow->SetRelativeMouseMode(false);
                SDL_ShowCursor();
                mouseReleasedForWait = true;
            }
            std::lock_guard<std::mutex> lock(mSurfaceMtx);
            if (!mStartParams->mDisableVideo) {
                Network_Draw_WaitingForPlayer();
                mSurface->draw();
                mSurface2->draw();
                mSurface->mergeSurfaceBuffer(mSurface2);
                mWindow->RenderAt(mSurface);
                mWindow->FrameEnd();
            }
            mWindow->Cycle();
            eventsProcess();
            continue;
        }

        // First frame after peer sync completes: restore palette and recapture mouse.
        if (wasWaiting) {
            wasWaiting = false;
            mGraphics->PaletteSet();
            mSurface->palette_SetFromNew();
            mSurface->surfaceSetToPalette();
            if (mouseReleasedForWait) {
                mouseReleasedForWait = false;
                SDL_HideCursor();
                mWindow->SetRelativeMouseMode(true);
            }
        }

        // Update local cursor position at 50 Hz
        Mouse_Cursor_Handle();
        {
            const int16 xAdj = -32, yAdj = 4;
            int16 clampedX = mInputMouseX;
            int16 clampedY = mInputMouseY;
            const int16 minX = mSidebar_SmallMode ? (xAdj + 16) : xAdj;
            const int16 maxX = static_cast<int16>(mWindow->GetScreenSize().getWidth() + xAdj - 1);
            const int16 maxY = static_cast<int16>(mWindow->GetScreenSize().getHeight() + yAdj - 1);
            if (clampedX < minX) clampedX = minX;
            if (clampedX > maxX) clampedX = maxX;
            if (clampedY < yAdj) clampedY = yAdj;
            if (clampedY > maxY) clampedY = maxY;
            mMouseX = clampedX;
            mMouseY = clampedY;

            // Always update world-space cursor from SDL position.
            // Without this, losing window focus freezes the world position
            // while the camera keeps moving, making the cursor appear to
            // follow P1's squad leader.
            mNet_LocalCursorWorldX = static_cast<int16>(clampedX + static_cast<int16>(mCameraX >> 16));
            mNet_LocalCursorWorldY = static_cast<int16>(clampedY + static_cast<int16>(mCameraY >> 16));
        }

        // Render every interrupt tick (50 Hz).
        // Skip rendering until the first simulation tick has completed.
        if (firstSimFrameDone) {
            std::lock_guard<std::mutex> lock(mSurfaceMtx);
            if (!mStartParams->mDisableVideo) {
                mGraphics->MapTiles_Draw();
                Sprites_Draw();
                Network_Sidebar_ForceSquadIcons();
                mGraphics->Sidebar_Copy_To_Surface(0, mSurface);
                Mouse_DrawCursor();
                Video_SurfaceRender(false, false);
                mSurface->Restore();
            }
        }
        mWindow->Cycle();
        eventsProcess();

        // Run the simulation every 3 interrupt ticks (~60 ms)
        if (++netInterruptTicks >= 3) {
            netInterruptTicks = 0;

            result = Network_Tick();
            if (!firstSimFrameDone)
                firstSimFrameDone = true;
            if (result != 1)
                return result;
        }
    }
    return -1;
}

// ============================================================
// Lobby_CampaignSelection
// Networked campaign selection where both players can see
// what's being selected. Host navigates, P2 sees selection
// and can ready up. Both must agree before proceeding.
// ============================================================

void cFodderMultiplayer::Lobby_CampaignSelection() {
    if (!mLobby || !mLobby->IsRunning()) {
        mStartParams->mNetworkEnabled = false;
        return;
    }

    const bool isHost = mLobby->IsHost();

    // We're reusing the existing campaign list (mCampaignList) which was
    // already populated by Campaign_Select_Setup.
    if (mCampaignList.empty()) {
        g_Debugger->Error("[Lobby] No campaigns available.");
        mStartParams->mNetworkEnabled = false;
        return;
    }

    int16 selectedIndex = 0;
    bool localReady = false;
    bool done = false;
    bool cancelled = false;

    // Override the interrupt callback so the campaign select screen
    // doesn't draw over our lobby screen from the timer thread.
    const auto prevInterruptCallback = mInterruptCallback;
    mInterruptCallback = nullptr;

    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();
    mMouse_Button_Left_Toggle = 0;

    while (!done && !mExit) {
        // Poll the lobby
        mLobby->Poll();

        // Host sends current selection
        if (isHost) {
            if (selectedIndex >= (int16)mCampaignList.size())
                selectedIndex = (int16)mCampaignList.size() - 1;
            if (selectedIndex < 0) selectedIndex = 0;
            mLobby->SetSelection(selectedIndex, mCampaignList[selectedIndex]);
        }

        // P2 auto-follows host's selection
        if (!isHost && mLobby->IsConnected()) {
            std::string remoteCampaign = mLobby->GetRemoteCampaign();
            if (!remoteCampaign.empty()) {
                // Find campaign in our list
                for (int16 i = 0; i < (int16)mCampaignList.size(); ++i) {
                    if (mCampaignList[i] == remoteCampaign) {
                        selectedIndex = i;
                        break;
                    }
                }
            }
        }

        // Check if host said GO
        if (!isHost && mLobby->IsRemoteStarted()) {
            done = true;
            break;
        }

        // Draw the lobby campaign selection screen
        mSurface->clearBuffer();
        mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
        GUI_Element_Reset();

        // Title
        mString_GapCharID = 0x25;
        if (isHost)
            String_Print_Large("HOST", true, 0x01);
        else
            String_Print_Large("JOIN", true, 0x01);
        mString_GapCharID = 0x00;

        String_Print_Large("SELECT CAMPAIGN", false, 0x18);

        size_t YOffset = PLATFORM_BASED(0, 25);
        int16 rowY = 0x30;

        // Connection status
        if (!mLobby->IsConnected()) {
            String_Print_Small("WAITING FOR PEER", rowY);
            rowY += 0x12;
        } else {
            if (isHost) {
                if (mLobby->IsRemoteReady()) {
                    String_Print_Small("PLAYER 2 READY", rowY);
                } else {
                    String_Print_Small("PLAYER 2 NOT READY", rowY);
                }
            } else {
                String_Print_Small("CONNECTED TO HOST", rowY);
            }
            rowY += 0x12;
        }

        // Campaign list (show up to 5 items centered on selection)
        {
            int16 startIdx = selectedIndex - 2;
            if (startIdx < 0) startIdx = 0;
            int16 endIdx = startIdx + 5;
            if (endIdx > (int16)mCampaignList.size())
                endIdx = (int16)mCampaignList.size();

            rowY = 0x48;
            for (int16 i = startIdx; i < endIdx; ++i) {
                bool isSelected = (i == selectedIndex);

                if (isSelected) {
                    GUI_Button_Draw_Small(mCampaignList[i].c_str(), rowY, 0xB2, 0xB3);
                } else {
                    // Draw unselected items dimmer
                    GUI_Button_Draw_Small(mCampaignList[i].c_str(), rowY);
                }

                if (isHost) {
                    // Allow host to click campaign items
                    auto el = GUI_Button_Setup_New(
                        [](void* ctx, int16 action, int16 arg) {
                            *static_cast<int16*>(ctx) = arg;
                        },
                        &selectedIndex, 0, i
                    );
                }

                rowY += 0x15;
            }
        }

        // Scroll buttons for host (above and below campaign list)
        if (isHost && (int16)mCampaignList.size() > 5) {
            GUI_Button_Draw_Small("UP", 0x3C);
            GUI_Button_Setup_New(
                [](void* ctx, int16, int16) {
                    int16& idx = *static_cast<int16*>(ctx);
                    if (idx > 0) --idx;
                }, &selectedIndex);

            GUI_Button_Draw_Small("DOWN", 0x99 + YOffset);
            GUI_Button_Setup_New(
                [](void* ctx, int16 action, int16 arg) {
                    int16& idx = *static_cast<int16*>(ctx);
                    if (idx < arg) ++idx;
                }, &selectedIndex, 0, (int16)(mCampaignList.size() - 1));
        }

        // Ready / Start buttons
        rowY = 0xA6 + YOffset;

        if (isHost) {
            if (mLobby->IsConnected() && mLobby->IsRemoteReady()) {
                GUI_Button_Draw_Small("START GAME", rowY, 0xB2, 0xB3);
                GUI_Button_Setup_New(
                    [](void* ctx, int16, int16) {
                        *static_cast<bool*>(ctx) = true;
                    }, &done);
            }
        } else {
            // P2: READY toggle
            if (localReady) {
                GUI_Button_Draw_Small("READY", rowY, 0xB2, 0xB3);
            } else {
                GUI_Button_Draw_Small("NOT READY", rowY);
            }
            GUI_Button_Setup_New(
                [](void* ctx, int16, int16) {
                    bool& r = *static_cast<bool*>(ctx);
                    r = !r;
                }, &localReady);
            mLobby->SetReady(localReady);
        }

        // BACK button
        GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
        GUI_Button_Setup_New(
            [](void* ctx, int16, int16) {
                *static_cast<bool*>(ctx) = true;
            }, &cancelled);

        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        Mouse_DrawCursor();

        // Handle input
        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        if (Mouse_Button_Left_Toggled() >= 0)
            GUI_Handle_Element_Mouse_Check(mGUI_Elements);

        // Mouse wheel scrolling for host
        if (isHost) {
            if (mMouse_EventLastWheel.mY > 0) {
                if (selectedIndex > 0) --selectedIndex;
                mMouse_EventLastWheel.mY = 0;
            } else if (mMouse_EventLastWheel.mY < 0) {
                if (selectedIndex < (int16)mCampaignList.size() - 1) ++selectedIndex;
                mMouse_EventLastWheel.mY = 0;
            }
        }

        // Escape to go back
        if (mPhase_EscapeKeyAbort || mPhase_Aborted) {
            mPhase_EscapeKeyAbort = false;
            mPhase_Aborted = false;
            cancelled = true;
        }

        if (cancelled) {
            mInterruptCallback = prevInterruptCallback;
            mStartParams->mNetworkEnabled = false;
            return;
        }

        Video_Sleep();
    }

    // Host tells P2 to start
    if (isHost && done) {
        mLobby->SetStarted();
        // Send a few extra packets to ensure delivery
        for (int i = 0; i < 15; ++i) {
            mLobby->Poll();
            SDL_Delay(5);
        }
    }

    mInterruptCallback = prevInterruptCallback;

    // Set the campaign selection result so Campaign_Select_File returns the right name
    mGUI_Select_File_CurrentIndex = 0;
    mGUI_Select_File_SelectedFileIndex = selectedIndex;
}

#endif // OPENFODDER_ENABLE_NETWORK
