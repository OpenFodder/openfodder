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
    mLobbyHub.reset();
}

// Lazy hub client for the campaign-select lobby loop. Construction +
// Configure (DNS resolve, socket bind) are not free, and the underlying
// cookie cache only pays off if we keep the same instance across ticks —
// so we build it once on first need and reuse it for the rest of the
// lobby session. A null return means Configure failed; the caller skips
// this tick and we'll try again on the next interval.
cNetworkHubClient* cFodderMultiplayer::AcquireLobbyHub() {
    if (mLobbyHub)
        return mLobbyHub.get();

    auto Hub = std::make_unique<cNetworkHubClient>();
    if (!Hub->Configure(mStartParams->mNetworkHubHost, mStartParams->mNetworkHubPort)) {
        // Leave mLobbyHub null so the next tick retries from scratch.
        return nullptr;
    }
    mLobbyHub = std::move(Hub);
    return mLobbyHub.get();
}

bool cFodderMultiplayer::ConsumeReturnToMultiplayerLobby() {
    const bool ReturnToLobby = mReturnToMultiplayerLobby;
    mReturnToMultiplayerLobby = false;
    return ReturnToLobby;
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

    // Generated-map modes (PvP + Coop Random) skip the OpenFodder studio
    // intro since they don't run a campaign. Coop Campaign keeps it.
    if (Network_UsesGeneratedMap(mStartParams->mNetworkGameMode))
        mOpenFodder_Intro_Done = true;
    else
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
        mNet_LocalCamInitialised = false;
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

        // Generated-map modes (PvP + Coop Random) bypass the campaign intro
        // and run the random-map generator before the phase. Coop Campaign
        // is the only mode that runs Intro_Main and the campaign mission
        // sequence.
        const bool UsesGeneratedMap = Network_UsesGeneratedMap(mStartParams->mNetworkGameMode);
        const bool IsPvPMatch = Network_IsPvPMode(mStartParams->mNetworkGameMode);
        if (UsesGeneratedMap) {
            mIntroDone = true;
            mGame_Data.mMission_Recruitment = 0;
            mGraphics->Load_pStuff();
        }
        else {
            Intro_Main();
        }

        if (UsesGeneratedMap) {
            const std::string PreviousScriptRun = mParams->mScriptRun;
            const std::string PreviousRandomFilename = mParams->mRandomFilename;

            mParams->mScriptRun = "multiplayer.js";
            mParams->mRandomFilename = "multiplayer";

            sMapParams Params(mStartParams->mNetworkMapSeed);
            CreateRandom(Params);

            mParams->mScriptRun = PreviousScriptRun;
            mParams->mRandomFilename = PreviousRandomFilename;
            mGame_Data.mMission_Recruitment = 0;
        }
        // Single / Random Map mode
        else if (mCustom_Mode == eCustomMode_Map) {
            if (mVersionDefault->mName == "Random Map") {
                if (!mGame_Data.mCampaign.isRandom()) {
                    const uint32 RandomSeed = mStartParams->mRandomMapOptionsEnabled
                        ? mStartParams->mRandomMapSeed
                        : (uint32)mRandom.get();
                    sMapParams Params(RandomSeed);
                    CreateRandom(Params);
                    mGame_Data.mMission_Recruitment = 0;
                }
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

        const int16 PhaseResult = Phase_Loop();

        if (!PhaseResult) {
            mKeyCode = 0;
            mPhase_In_Progress = false;
            Network_Stop();
            Squad_Member_PhaseCount();
            mPhase_TryingAgain = true;

            // Generated-map modes bounce back to the lobby after a phase
            // (PvP rounds + Coop Random) — there's no campaign sequence to
            // continue. Coop Campaign falls through to the regular
            // mission-loop flow.
            if (UsesGeneratedMap) {
                mReturnToMultiplayerLobby = true;
                return -1;
            }
        } else {
            mKeyCode = 0;
            mPhase_In_Progress = false;
            Network_Stop();

            if (UsesGeneratedMap) {
                mReturnToMultiplayerLobby = true;
                return -1;
            }

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

    auto RestoreLocalViewForRender = [&]() {
        if (!mNet_LocalCamInitialised)
            return;

        const int16 localSq = static_cast<int16>(mNetLocalPlayerIndex);
        if (localSq < 0 || localSq >= NETWORK_MAX_PLAYERS)
            return;

        Network_CameraRestore(mNet_LocalCam);
        Network_SetActiveSquadContext(localSq);

        const int16 camX = static_cast<int16>(mCameraX >> 16);
        const int16 camY = static_cast<int16>(mCameraY >> 16);
        mMouseX      = static_cast<int16>(mNet_LocalCursorWorldX - camX);
        mMouseY      = static_cast<int16>(mNet_LocalCursorWorldY - camY);
        mInputMouseX = mMouseX;
        mInputMouseY = mMouseY;
    };

    while (!mExit) {
        // Wait for the next interrupt tick (~20 ms at default mSleepDelta).
        mVideo_Ticked = false;
        mVideo_Done   = true;
        while (!mVideo_Ticked && !mExit)
            SDL_Delay(1);

        // Pump GGPO during the waiting phase so handshake packets flow.
        mNetSession->Idle(0);
        RestoreLocalViewForRender();

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
                RestoreLocalViewForRender();
                if (mNetMapOverlayActive && mSurfaceMapOverview) {
                    Network_DrawLiveMapOverlay();
                } else {
                    mGraphics->MapTiles_Draw();
                    Sprites_Draw();
                    Network_Sidebar_ForceSquadIcons();
                    mGraphics->Sidebar_Copy_To_Surface(0, mSurface);
                    Network_DrawMatchOverlay();
                    Mouse_DrawCursor();
                    Video_SurfaceRender(false, false);
                    mSurface->Restore();
                }
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
            if (result != 1) {
                mNetMapOverlayActive = false;
                return result;
            }
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

static sNetworkMatchSettings Lobby_MatchSettingsFromParams(const std::shared_ptr<sFodderParameters>& pParams) {
    sNetworkMatchSettings Settings;
    Settings.mGameMode = pParams->mNetworkGameMode;
    Settings.mMapSeed = pParams->mNetworkMapSeed;
    Settings.mKillLimit = pParams->mNetworkKillLimit;
    Settings.mTimeLimitSeconds = pParams->mNetworkTimeLimitSeconds;
    Settings.mTeamCount = pParams->mNetworkTeamCount;
    Settings.mTeamSize = pParams->mNetworkTeamSize;
    Settings.mFriendlyFire = pParams->mNetworkFriendlyFire ? 1 : 0;
    Settings.mMapSize = pParams->mNetworkMapSize;
    Settings.mMapTerrain = pParams->mNetworkMapTerrain;
    Settings.mMapTerrainSub = (uint8_t)pParams->mNetworkMapTerrainSub;
    Settings.mVehicleSet = pParams->mNetworkVehicleSet;
    Settings.mPickupDensity = pParams->mNetworkPickupDensity;
    Settings.mCoverDensity = pParams->mNetworkCoverDensity;
    return Settings;
}

static void Lobby_ApplyMatchSettingsToParams(const sNetworkMatchSettings& pSettings, const std::shared_ptr<sFodderParameters>& pParams) {
    pParams->mNetworkGameMode = pSettings.mGameMode;
    pParams->mNetworkMapSeed = pSettings.mMapSeed;
    pParams->mNetworkKillLimit = pSettings.mKillLimit;
    pParams->mNetworkTimeLimitSeconds = pSettings.mTimeLimitSeconds;
    pParams->mNetworkTeamCount = pSettings.mTeamCount;
    pParams->mNetworkTeamSize = pSettings.mTeamSize;
    pParams->mNetworkFriendlyFire = (pSettings.mFriendlyFire != 0);
    pParams->mNetworkMapSize = Network_NormalizeMapSize((uint8_t)pSettings.mMapSize);
    pParams->mNetworkMapTerrain = Network_NormalizeMapTerrain((uint8_t)pSettings.mMapTerrain);
    pParams->mNetworkMapTerrainSub = pSettings.mMapTerrainSub;
    pParams->mNetworkVehicleSet = Network_NormalizeVehicleSet((uint8_t)pSettings.mVehicleSet);
    pParams->mNetworkPickupDensity = Network_NormalizePickupDensity((uint8_t)pSettings.mPickupDensity);
    pParams->mNetworkCoverDensity = Network_NormalizeCoverDensity((uint8_t)pSettings.mCoverDensity);
}

static sNetworkHubMetadata Lobby_BuildHubMetadata(const std::shared_ptr<sFodderParameters>& pParams, const std::string& pCampaign) {
    sNetworkHubMetadata Metadata;
    Metadata.mGameName = pCampaign.size() ? pCampaign : "OpenFodder Lobby";
    Metadata.mGameMode = Network_GameModeName(pParams->mNetworkGameMode);
    // Generated-map modes (PvP + Coop Random) advertise the map params
    // (size + terrain); Coop Campaign advertises "Campaign" since the
    // actual mission set lives in the picked campaign name.
    Metadata.mMapName = Network_UsesGeneratedMap(pParams->mNetworkGameMode)
        ? (std::string(Network_MapSizeName(pParams->mNetworkMapSize)) + " " + Network_MapTerrainName(pParams->mNetworkMapTerrain))
        : "Campaign";
    Metadata.mVersion = "N" + std::to_string((int)NETWORK_COMPATIBILITY_VERSION);
    Metadata.mOptions =
        "seed=" + std::to_string(pParams->mNetworkMapSeed) +
        ";kill=" + std::to_string((int)pParams->mNetworkKillLimit) +
        ";time=" + std::to_string((int)pParams->mNetworkTimeLimitSeconds) +
        ";teams=" + std::to_string((int)pParams->mNetworkTeamCount) +
        ";size=" + std::to_string((int)pParams->mNetworkTeamSize) +
        ";ff=" + std::to_string(pParams->mNetworkFriendlyFire ? 1 : 0);
    return Metadata;
}

static void Lobby_ApplyPeerEndpointToParams(const cNetworkLobby& pLobby, const std::shared_ptr<sFodderParameters>& pParams) {
    if (pParams->mNetworkInternet)
        return;

    if (!pLobby.IsConnected())
        return;

    const std::string RemoteHost = pLobby.GetRemoteHost();
    const uint16_t RemotePort = pLobby.GetRemotePort();
    if (RemoteHost.empty() || !RemotePort)
        return;

    if (pParams->mNetworkRemoteHost == RemoteHost &&
        pParams->mNetworkRemotePort == RemotePort) {
        return;
    }

    pParams->mNetworkRemoteHost = RemoteHost;
    pParams->mNetworkRemotePort = RemotePort;
    g_Debugger->Notice("[Lobby] Gameplay endpoint set to " + RemoteHost + ":" + std::to_string(RemotePort));
}

// Push this peer's per-frame state into the lobby: the host advertises its
// campaign selection + match settings; the joiner advertises its team/class
// pick + ready flag.
void cFodderMultiplayer::Lobby_PushLocalState(bool pIsHost, int16 pSelectedIndex, bool pLocalReady) {
    if (pIsHost) {
        // Generated-map modes don't pick a campaign — the campaign list is
        // hidden in the lobby, and there may not even be any entries (e.g.
        // hub-launched games with no installed campaigns). Fall back to the
        // mode name as the selection label so anything reading mCampaign on
        // the joiner side gets a stable value.
        std::string SelectionName;
        if (Network_UsesGeneratedMap(mStartParams->mNetworkGameMode)) {
            SelectionName = Network_GameModeName(mStartParams->mNetworkGameMode);
        } else if (pSelectedIndex >= 0 && pSelectedIndex < (int16)mCampaignList.size()) {
            SelectionName = mCampaignList[pSelectedIndex];
        }
        mLobby->SetSelection(pSelectedIndex, SelectionName);
        mLobby->SetMatchSettings(Lobby_MatchSettingsFromParams(mStartParams));
    } else {
        mLobby->SetPlayerSelection(
            mStartParams->mNetworkSelectedTeam,
            mStartParams->mNetworkSelectedClass,
            mStartParams->mNetworkLockedIn
        );
        mLobby->SetReady(pLocalReady);
    }
}

// Broadcast this host's game on the LAN discovery channel so browsers can find it.
void cFodderMultiplayer::Lobby_AdvertiseGame(cNetworkDiscovery& pDiscovery, int16 pSelectedIndex) {
    sNetworkDiscoveryGame Game;
    Game.mHostName = "OPENFODDER";
    if (Network_UsesGeneratedMap(mStartParams->mNetworkGameMode)) {
        Game.mGameName = Network_GameModeName(mStartParams->mNetworkGameMode);
    } else if (pSelectedIndex >= 0 && pSelectedIndex < (int16)mCampaignList.size()) {
        Game.mGameName = mCampaignList[pSelectedIndex];
    }
    Game.mSettings = Lobby_MatchSettingsFromParams(mStartParams);
    Game.mLobbyPort = mStartParams->mNetworkLocalPort;
    Game.mGameplayPort = mStartParams->mNetworkLocalPort;
    Game.mCurrentPlayers = mLobby->IsConnected() ? 2 : 1;
    Game.mMaxPlayers = NETWORK_MAX_PLAYERS;
    Game.mState = mLobby->IsConnected() ? eNetworkDiscoveryState_Full : eNetworkDiscoveryState_Setup;
    pDiscovery.Advertise(Game);
}

// Deliver the host's START to the joiner. The lobby has no ACK/retransmit, so
// we resend a handful of times to paper over UDP loss. (A real fix is a small
// reliability layer in cNetworkLobby; this preserves the prior behaviour.)
void cFodderMultiplayer::Lobby_SendStartReliable() {
    mLobby->SetStarted();
    for (int i = 0; i < 15; ++i) {
        mLobby->Poll();
        Lobby_ApplyPeerEndpointToParams(*mLobby, mStartParams);
        SDL_Delay(5);
    }
}

void cFodderMultiplayer::Lobby_CampaignSelection() {
    if (!mLobby || !mLobby->IsRunning()) {
        mStartParams->mNetworkEnabled = false;
        return;
    }

    const bool isHost = mLobby->IsHost();

    // We're reusing the existing campaign list (mCampaignList) which was
    // populated by Campaign_Select_Setup BEFORE network was enabled — so
    // Campaign_Select_Setup's "hide SP placeholders when MP is active"
    // gate didn't fire (mNetworkEnabled was still false at that point).
    // Strip Single Map / Random Map here so the lobby's campaign list
    // shows only real campaigns the host can actually pick for coop.
    mCampaignList.erase(
        std::remove_if(mCampaignList.begin(), mCampaignList.end(),
            [](const std::string& Name) {
                return Name == "Single Map" || Name == "Random Map";
            }),
        mCampaignList.end());

    // Generated-map modes (PvP + Coop Random) don't need a campaign — the
    // gameplay surface is built from MAP / SEED on the host setup screen —
    // so an empty list is fine in those modes.
    if (mCampaignList.empty() && !Network_UsesGeneratedMap(mStartParams->mNetworkGameMode)) {
        g_Debugger->Error("[Lobby] No campaigns available.");
        mStartParams->mNetworkEnabled = false;
        return;
    }

    int16 selectedIndex = 0;
    bool localReady = false;
    bool done = false;
    bool cancelled = false;
    const uint32_t LobbyStartedTicks = (uint32_t)SDL_GetTicks();
    uint32_t LastHubUpdateTicks = 0;
    // Joiners hit OFHUB/2 anonymous HEARTBEAT on a much slower cadence than
    // the host's UPDATE/HEARTBEATHOST ticker. The hub reaps anonymous peer
    // slots near 90 s of silence; 30 s sits well inside that window without
    // compounding cost with the 2.5 s host loop.
    uint32_t LastHubJoinerHeartbeatTicks = 0;
    static const uint32_t kHubJoinerHeartbeatIntervalMs = 30000;
    cNetworkDiscovery Discovery;
    const bool DiscoveryStarted = isHost && Discovery.StartHost();

    // Override the interrupt callback so the campaign select screen
    // doesn't draw over our lobby screen from the timer thread.
    const auto prevInterruptCallback = mInterruptCallback;
    mInterruptCallback = nullptr;

    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();
    mMouse_Button_Left_Toggle = 0;

    while (!done && !mExit) {
        if (isHost) {
            if (selectedIndex >= (int16)mCampaignList.size())
                selectedIndex = (int16)mCampaignList.size() - 1;
            if (selectedIndex < 0)
                selectedIndex = 0;
        }

        Lobby_PushLocalState(isHost, selectedIndex, localReady);

        // Poll the lobby
        mLobby->Poll();
        Lobby_ApplyPeerEndpointToParams(*mLobby, mStartParams);

        if (DiscoveryStarted)
            Lobby_AdvertiseGame(Discovery, selectedIndex);
        if (isHost && mStartParams->mNetworkInternet && !mStartParams->mNetworkRoomCode.empty()) {
            const uint32_t Now = (uint32_t)SDL_GetTicks();
            if (!LastHubUpdateTicks || Now - LastHubUpdateTicks > 2500) {
                LastHubUpdateTicks = Now;
                // Reuse the lobby-scoped hub client. The cookie cache
                // (NetworkHubClient EnsureCookie, ~9 min) and BADCOOKIE/STALE
                // auto-invalidation in SendCommand only pay off if we hold a
                // stable instance across ticks — Configure() resets cookie
                // state, which is why the per-tick `cNetworkHubClient Hub`
                // version was effectively re-HELLOing every 2.5 s.
                cNetworkHubClient* Hub = AcquireLobbyHub();
                if (Hub) {
                    // OFHUB/2 verified-host UPDATE. The legacy Update() shim
                    // unconditionally fails ("missing bearer"); UpdateAuth
                    // carries the JWT we cached at room-create time. Prefer
                    // the StartParams copy (zero-disk-IO replay) but fall
                    // back to the auth cache if it has been blanked between
                    // sessions — the host could have re-paired in another
                    // process, and a stale empty string here would lock us
                    // out for the rest of the lobby.
                    sHubAuthToken Token;
                    Token.mJwt = mStartParams->mNetworkHostBearer;
                    if (Token.mJwt.empty())
                        (void)mHubAuth.LoadCachedToken(Token);
                    if (!Token.mJwt.empty()) {
                        if (!Hub->UpdateAuth(
                                Token,
                                mStartParams->mNetworkRoomCode,
                                Lobby_BuildHubMetadata(mStartParams, mCampaignList[selectedIndex]),
                                true)) {
                            // SendCommand already invalidated the cookie on
                            // BADCOOKIE/STALE so the next call will re-HELLO
                            // automatically; surface other failures (network,
                            // bearer rejection) for dev visibility.
                            g_Debugger->Notice(std::string("[hub] update failed: ") + Hub->GetLastError());
                        }
                        // Verified-host rooms idle out at 600 s; HEARTBEATHOST
                        // resets that timer. Throttling alongside UPDATE keeps
                        // the per-tick cost flat (one extra UDP round-trip
                        // every ~2.5 s) and means the user can sit on the
                        // campaign select screen indefinitely without the
                        // hub reaping the room behind them.
                        if (!Hub->HeartbeatHost(Token, mStartParams->mNetworkRoomCode)) {
                            g_Debugger->Notice(std::string("[hub] heartbeat-host failed: ") + Hub->GetLastError());
                        }
                    }
                }
            }
        }

        // Joiner-side anonymous heartbeat. Verified-host UPDATE keeps the
        // room alive but the joiner's anonymous peer slot has its own
        // shorter idle timer at the hub; without this ping it gets reaped
        // (~90 s) and the next packet bounces with "unknown peer". On a
        // BADCOOKIE/STALE response cNetworkHubClient drops the cached cookie
        // so the next HEARTBEATANON re-HELLOs automatically — that's why we
        // hand the same instance back through AcquireLobbyHub each tick.
        if (!isHost && mStartParams->mNetworkInternet && !mStartParams->mNetworkRoomCode.empty()) {
            const uint32_t Now = (uint32_t)SDL_GetTicks();
            if (!LastHubJoinerHeartbeatTicks || Now - LastHubJoinerHeartbeatTicks > kHubJoinerHeartbeatIntervalMs) {
                LastHubJoinerHeartbeatTicks = Now;
                cNetworkHubClient* Hub = AcquireLobbyHub();
                if (Hub) {
                    if (!Hub->HeartbeatAnonymous(mStartParams->mNetworkRoomCode)) {
                        g_Debugger->Notice(std::string("[hub] heartbeat-anon failed: ") + Hub->GetLastError());
                    }
                }
            }
        }

        // P2 auto-follows host's selection
        if (!isHost && mLobby->IsConnected()) {
            Lobby_ApplyMatchSettingsToParams(mLobby->GetRemoteMatchSettings(), mStartParams);

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

        // Draw the lobby campaign selection screen.
        //
        //   Y=0x01  HOST / JOIN title (briefing, underlined)
        //   Y=0x18  "SELECT CAMPAIGN" (briefing)
        //   Y=0x32  Status band — pstuff sidebar (small) font, 0x09 row pitch.
        //           Up to ~6 lines (3 roster + 3 info) fit in ~54 px between
        //           Y=0x32 and Y=0x60 without crashing into the campaign list.
        //   Y=0x66  Campaign list — briefing font, 3 rows (kept big: this is
        //           the focal element + handles long custom names with
        //           lower-case / digits which the sidebar font lacks).
        //   Y=0x60 / Y=0xA0  UP / DOWN labels — sidebar font, immediately
        //           flanking the list.
        //   Y=0xA6  START / READY (briefing)
        //   Y=0xB3  BACK         (briefing)
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

        // Title varies by mode: campaign-pick screen for Coop Campaign,
        // pre-game lobby for generated-map modes.
        String_Print_Large(
            Network_UsesGeneratedMap(mStartParams->mNetworkGameMode)
                ? "LOBBY"
                : "SELECT CAMPAIGN",
            false, 0x18);

        size_t YOffset = PLATFORM_BASED(0, 25);

        // ===== Status band (sidebar font). =====
        constexpr int16 kStatusLeftX = 0x10;
        constexpr int16 kStatusRowY0 = 0x32;
        constexpr int16 kStatusRowH  = 0x09;     // sidebar-font row pitch
        int16 statusY = kStatusRowY0;

        auto printStatus = [&](const std::string& s) {
            Sidebar_Menu_Print(s, kStatusLeftX, (size_t)statusY,
                               eSidebarMenuColor::Normal);
            statusY += kStatusRowH;
        };

        // Connection status
        if (!mLobby->IsConnected()) {
            const uint32_t Now = (uint32_t)SDL_GetTicks();
            if (!isHost && Now - LobbyStartedTicks > 8000)
                printStatus("HOST NOT RESPONDING");
            else
                printStatus("WAITING FOR PEER");
        } else {
            // P1 A1 roster: one row per remote peer with connected/ready state.
            // Joiner perspective summarises to "CONNECTED TO HOST"; host walks
            // its remote slots so a 4-player room shows three roster rows.
            if (isHost) {
                const uint8_t LocalSlot = mLobby->GetLocalPeerIndex();
                const uint8_t TotalPeers = mStartParams->mNetworkNumPlayers
                    ? mStartParams->mNetworkNumPlayers
                    : (uint8_t)NETWORK_MAX_PLAYERS;
                for (uint8_t slot = 0; slot < TotalPeers; ++slot) {
                    if (slot == LocalSlot)
                        continue;
                    const sLobbyPeerSlot& peer = mLobby->GetPeerSlot(slot);
                    std::string label = "PLAYER " + std::to_string((int)slot + 1);
                    if (!peer.connected)
                        label += " WAITING";
                    else if (peer.ready)
                        label += " READY";
                    else
                        label += " NOT READY";
                    // Sidebar font lacks digits; the "PLAYER N" prefix needs
                    // them. The Sidebar_Menu_Print path silently skips
                    // unsupported chars, so "PLAYER 2" renders as "PLAYER ".
                    // Acceptable for now — at most 3 host roster rows so the
                    // visible difference (truncated number) is small. A
                    // future iteration can author digit glyphs.
                    printStatus(label);
                }
            } else {
                printStatus("CONNECTED TO HOST");
            }
        }

        // MODE always; SEED / MAP only for PvP modes where the surface is a
        // generated map. Coop campaign uses the picked campaign's bundled
        // maps, so seed/size/terrain/cover are unused — surfacing them here
        // would show "MAP RANDOM" / "SEED ..." that doesn't track anything
        // the game actually consumes.
        printStatus(std::string("MODE ") + Network_GameModeName(mStartParams->mNetworkGameMode));
        if (Network_UsesGeneratedMap(mStartParams->mNetworkGameMode)) {
            // SEED has digits the sidebar font can't render — show only the
            // word. Power users who want the seed value can use --net-seed.
            printStatus("SEED");
            printStatus(std::string("MAP ") +
                        Network_MapSizeName(mStartParams->mNetworkMapSize) + " " +
                        Network_MapTerrainName(mStartParams->mNetworkMapTerrain) +
                        " COVER " + Network_CoverDensityName(mStartParams->mNetworkCoverDensity));
        }

        // ===== Campaign list (Coop Campaign only). =====
        // Generated-map modes (PvP + Coop Random) don't have a campaign to
        // pick — the host's MAP/SEED choices on the host setup screen drive
        // the gameplay surface, so the campaign list and UP/DOWN scroll are
        // hidden in those modes. Coop Campaign keeps the list since picking
        // CF1 vs CF2 vs custom is the whole point of that flow.
        const bool ShowCampaignList = !Network_UsesGeneratedMap(mStartParams->mNetworkGameMode);

        constexpr int16 kListRowY0 = 0x66;
        constexpr int16 kListRowH  = 0x15;
        constexpr int16 kListVisible = 3;
        int16 rowY = 0x30;
        if (ShowCampaignList) {
            int16 startIdx = selectedIndex - 1;
            if (startIdx < 0) startIdx = 0;
            int16 endIdx = startIdx + kListVisible;
            if (endIdx > (int16)mCampaignList.size())
                endIdx = (int16)mCampaignList.size();

            rowY = kListRowY0;
            for (int16 i = startIdx; i < endIdx; ++i) {
                const bool isSelected = (i == selectedIndex);
                if (isSelected)
                    GUI_Button_Draw_Small(mCampaignList[i].c_str(), rowY, 0xB2, 0xB3);
                else
                    GUI_Button_Draw_Small(mCampaignList[i].c_str(), rowY);

                if (isHost) {
                    GUI_Button_Setup_New(
                        [](void* ctx, int16 action, int16 arg) {
                            *static_cast<int16*>(ctx) = arg;
                        },
                        &selectedIndex, 0, i
                    );
                }
                rowY += kListRowH;
            }
        }

        // ===== UP / DOWN labels (host only, paging campaign list). =====
        // Hidden alongside the campaign list when the mode skips it.
        if (ShowCampaignList && isHost && (int16)mCampaignList.size() > kListVisible) {
            constexpr int16 kArrowX  = 0xF0;
            constexpr int16 kArrowW  = 0x14;
            const int16     kUpY     = kListRowY0 - 0x06;
            const int16     kDownY   = kListRowY0 + (kListVisible * kListRowH);

            Sidebar_Menu_Print("UP", kArrowX, (size_t)kUpY, eSidebarMenuColor::Normal);
            mGUI_Temp_X = kArrowX - 2;
            mGUI_Temp_Y = kUpY;
            mGUI_Temp_Width = kArrowW;
            mGUI_Draw_LastHeight = 8;
            GUI_Box_Draw(0xB2, 0xB3);
            GUI_Button_Setup_New(
                [](void* ctx, int16, int16) {
                    int16& idx = *static_cast<int16*>(ctx);
                    if (idx > 0) --idx;
                }, &selectedIndex);

            Sidebar_Menu_Print("DOWN", kArrowX, (size_t)kDownY, eSidebarMenuColor::Normal);
            mGUI_Temp_X = kArrowX - 2;
            mGUI_Temp_Y = kDownY;
            mGUI_Temp_Width = kArrowW;
            mGUI_Draw_LastHeight = 8;
            GUI_Box_Draw(0xB2, 0xB3);
            GUI_Button_Setup_New(
                [](void* ctx, int16 action, int16 arg) {
                    int16& idx = *static_cast<int16*>(ctx);
                    if (idx < arg) ++idx;
                }, &selectedIndex, 0, (int16)(mCampaignList.size() - 1));
        }

        // Ready / Start buttons
        rowY = (int16)(0xA6 + YOffset);

        if (isHost) {
            // P1 A1: at N>2 we need EVERY remote peer to be ready before
            // START is enabled, not just the aggregate "first non-local
            // peer" (mLobby->IsRemoteReady()). Walk the per-slot table to
            // fold them. mLobby->IsConnected() stays as the cheap fast-out
            // for the 2P path — at least one peer must be live before we
            // bother polling the rest.
            bool AllPeersReady = mLobby->IsConnected();
            if (AllPeersReady) {
                const uint8_t LocalSlot = mLobby->GetLocalPeerIndex();
                const uint8_t TotalPeers = mStartParams->mNetworkNumPlayers
                    ? mStartParams->mNetworkNumPlayers
                    : (uint8_t)NETWORK_MAX_PLAYERS;
                for (uint8_t slot = 0; slot < TotalPeers; ++slot) {
                    if (slot == LocalSlot)
                        continue;
                    const sLobbyPeerSlot& peer = mLobby->GetPeerSlot(slot);
                    if (!peer.connected || !peer.ready) {
                        AllPeersReady = false;
                        break;
                    }
                }
            }
            if (AllPeersReady) {
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

        // Mouse wheel scrolling for host (campaign list paging).
        // No-op when the list is hidden (generated-map modes).
        if (isHost && !Network_UsesGeneratedMap(mStartParams->mNetworkGameMode)) {
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
            if (mLobby)
                mLobby->Stop();
            mLobbyHub.reset();
            mStartParams->mNetworkEnabled = false;
            // Drain input state on exit so the click that triggered BACK
            // doesn't bleed through to the next screen — the campaign-select
            // EXIT button sits at the same Y as our BACK button, and a stale
            // mMouse_Button_Left_Toggle was triggering it on the next tick.
            // Mirrors Multiplayer_Menu_Run's exit cleanup.
            mMouse_Button_Left_Toggle = 0;
            mMouse_EventLastButtonsPressed = 0;
            mKeyCode = 0;
            mKeyCodeAscii = 0;
            mInput_LastKey = 0;
            return;
        }

        Video_Sleep();
    }

    // Host tells P2 to start
    if (isHost && done)
        Lobby_SendStartReliable();

    if (mLobby) {
        Lobby_ApplyPeerEndpointToParams(*mLobby, mStartParams);
        mLobby->Stop();
        g_Debugger->Notice("[Lobby] Closed before GGPO gameplay.");
    }

    // Drop the lobby-scoped hub client; gameplay uses GGPO/the relay path.
    // A fresh instance will be built next time the lobby opens.
    mLobbyHub.reset();

    mInterruptCallback = prevInterruptCallback;

    // Set the campaign selection result so Campaign_Select_File returns the right name
    mGUI_Select_File_CurrentIndex = 0;
    mGUI_Select_File_SelectedFileIndex = selectedIndex;

    // Same input drain on the START path so the click that triggered START
    // GAME doesn't carry into the briefing / mission frame.
    mMouse_Button_Left_Toggle = 0;
    mMouse_EventLastButtonsPressed = 0;
    mKeyCode = 0;
    mKeyCodeAscii = 0;
    mInput_LastKey = 0;
}

#endif // OPENFODDER_ENABLE_NETWORK
