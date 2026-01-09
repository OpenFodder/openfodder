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

#ifdef WIN32
#include <windows.h>
#endif

const int16 SIDEBAR_WIDTH = 48;

cFodder::cFodder(std::shared_ptr<cWindow> pWindow)
{

    mExit = false;
    mVersions = std::make_shared<cVersions>();
    mVersionCurrent = 0;
    mVersionDefault = 0;

    mParams = std::make_shared<sFodderParameters>();
    mStartParams = std::make_shared<sFodderParameters>();

    mOpenFodder_Intro_Done = false;
    mCustom_Mode = eCustomMode_None;
    mGraphics = 0;
    mSound = 0;
    mWindow = pWindow;

    mGUI_Select_File_ShownItems = 4;
    mTicksDiff = 0;
    mTicks = 0;

    mMouseX = 0;
    mMouseY = 0;
    mMouseX_Offset = 0;
    mMouseY_Offset = 0;
    mMouse_LeftWindow = SDL_GetTicks();

    mIntroDone = false;

    mMouse_EventLastButtonsPressed = 0;

    mMouseButtonStatus = 0;
    mInputMouseX = 0;
    mInputMouseY = 0;

    mButtonPressLeft = mButtonPressRight = 0;
    mMouse_Button_Left_Toggle = 0;
    mMouse_Button_Right_Toggle = 0;
    mMouse_Button_LeftRight_Toggle = mSquad_Member_Fire_CoolDown_Override = false;
    mMouse_Button_LeftRight_Toggle2 = false;

    mMouse_Exit_Loop = false;

    mSprite_Frame_1 = 0;
    mSprite_Frame_Modifier_Update_Countdown = 0;
    mSprite_Frame_2 = 0;
    mSprite_Frame3_ChangeCount = 0;
    mSprite_Frame_3 = 0;

    mEnemy_BuildingCount = 0;
    mPhase_Aborted = false;
    mPhase_EscapeKeyAbort = false;

    mSprite_SpareUsed = 0;
    mSprite_SpareUsed2 = 0;
    mSquad_WalkTargetX = 0;
    mSquad_WalkTargetY = 0;
    mSprite_Bullet_Destroy = 0;
    mSprite_Helicopter_DestroyLight = 0;
    mSprite_DistanceTo_Squad0 = 0;
    mSprite_Tank_SpriteX = 0;
    mSprite_Tank_SpriteY = 0;
    mSprite_Tank_Squad0_X = 0;
    mSprite_Tank_DistanceTo_Squad0 = 0;
    mSprite_Missile_LaunchDistance_X = 0;
    mSprite_Missile_LaunchDistance_Y = 0;
    mMapTile_ColumnOffset = 0;
    mMapTile_RowOffset = 0;
    mGUI_Select_File_String_Input_Callback = 0;

    mMapTile_Ptr = 0;

    mSoundEffectToPlay = 0;
    mBriefing_Aborted = 0;
    mGUI_Mouse_Modifier_X = 0;
    mGUI_Mouse_Modifier_Y = 0;

    mMouseSpriteCurrent = 0;
    mService_ExitLoop = 0;
    mVideo_Draw_FrameDataPtr = 0;
    mVideo_Draw_PosX = 0;
    mVideo_Draw_PosY = 0;
    mVideo_Draw_Columns = 0;
    mVideo_Draw_Rows = 0;
    mVideo_Draw_PaletteIndex = 0;
    mDraw_Source_SkipPixelsPerRow = 0;
    mDraw_Dest_SkipPixelsPerRow = 0;
    mKeyCode = 0;

    mCameraX = 0;
    mCameraY = 0;

    mMapTile_TargetX = 0;
    mMapTile_TargetY = 0;
    mMapTile_SpeedX = 0;
    mMapTile_SpeedY = 0;

    mSurfaceMapOverview = 0;

    mMap = 0;

    mVersionPlatformSwitchDisabled = false;
    mGame_Data.mGamePhase_Data.mIsComplete = false;
    mSidebar_Draw_Y = 0;
    word_3A3BF = 0;
    word_82176 = 0;
    word_82178 = 0;

    mDebug_PhaseSkip = 0;

    mVideo_Ticked = false;
    mPhase_InterruptTicks = 0;
    mInterruptTick = 0;
    mInput_Enabled = false;

    mKeyControlPressed = 0;
    mVersionReturnAfterPhase = false;

    mInput_LastKey = 0;
    mCustom_ExitMenu = 0;

    mBriefingHelicopter_NotDone = 0;
    word_3A05F = 0;
    byte_44AC0 = 0;
    mSoundDisabled = false;
    Squad_Walk_Target_SetAll(0);
    mPhase_Completed_Timer = 0;
    mVideo_Draw_ColumnsMax = 0;
    mBriefingHelicopter_DirectionIndexF = 0;
    mBriefingHelicopter_DirectionIndex = 0;
    mBriefingHelicopter_Speed = 0;
    word_428BA = 0;
    mHelicopterPosX = 0;
    mHelicopterPosY = 0;
    mBriefingHelicopter_TargetDirection = 0;
    mBriefingHelicopter_TargetSpeed = 0;
    mBriefingHelicopter_NextUpdateCountdown = 0;

    mBriefingHelicopter_Moving = 0;
    mHelicopterOffsetIndex = 0;

    mBriefingHelicopter_FrameCounter = 0;
    mBriefingHelicopter_Moving = 0;
    mTroop_InRange_Callpad = 0;

    for (unsigned int x = 0; x < 0x18; ++x)
    {
        mMission_Save_Blocked[x] = 0;
    }

    mService_Promotion_Exit_Loop = 0;

    //      Field_0,                 X, Width, Y, Height, mMouseInsideFuncPtr
    mSidebar_OverlayButtons[0] = {
        &cFodder::GUI_Button_NoAction,
        0,   // X
        47,  // Width
        0,   // Y
        225, // Height
        &cFodder::GUI_Button_NoAction2};

    mSidebar_OverlayButtons[1] = {0, 0, 0, 0, 0, 0};

    mGUI_Handle_Button_SelectSquad_Array[0] = &cFodder::GUI_Handle_Button_SelectSquad_0;
    mGUI_Handle_Button_SelectSquad_Array[1] = &cFodder::GUI_Handle_Button_SelectSquad_1;
    mGUI_Handle_Button_SelectSquad_Array[2] = &cFodder::GUI_Handle_Button_SelectSquad_2;

    mDemo_ExitMenu = 0;

    mSurfaceMapTop = 0;
    mSurfaceMapLeft = 0;

    mSprite_SheetPtr = 0;

    mWindow_Focus = false;

    Sprite_Clear_All();

    Phase_EngineReset();

    Sprite_Table_Setup();
}

cFodder::~cFodder()
{

    delete mSurfaceMapOverview;
}

/**
 * Execute one cycle of the current phase
 *
 * -1 = Phase Try Again
 *  0 = Phase Won
 *  1 = Phase Running
 */
int16 cFodder::Phase_Cycle()
{

    // If demo playback is enabled, and a record resume cycle is set
    if (mStartParams->mDemoPlayback && mStartParams->mDemoRecordResumeCycle)
    {
        // See if we hit the tick count
        if (mGame_Data.mDemoRecorded.mTick >= mStartParams->mDemoRecordResumeCycle)
        {
            // Then resume recording
            mStartParams->mDemoPlayback = false;
            mStartParams->mDemoRecord = true;
            mStartParams->mDemoRecordResumeCycle = 0;
            mParams->mSleepDelta = 0;
            mParams->mDemoRecord = mStartParams->mDemoRecord;
            mParams->mDemoPlayback = mStartParams->mDemoPlayback;
            mStartParams->mDisableVideo = false;
            mStartParams->mDisableSound = false;
            Mouse_Setup();
        }
    }

    if (mStartParams->mDemoPlayback)
    {

        if (!mGame_Data.mDemoRecorded.GetVideoTick(mGame_Data.mDemoRecorded.mTick))
        {
            return 1;
        }
    }
    else
    {

#ifdef EMSCRIPTEN
        if (mPhase_InterruptTicks < 3)
        {

            return 1;
        }
        if (mExit)
        {
            return -1;
        }
#else

        do
        {
            Video_Sleep();

            if (mPhase_Paused)
            {
                Phase_Paused();
                continue;
            }

            if (mExit)
            {
                return -1;
            }
        } while (mPhase_InterruptTicks < 3);
#endif
    }

    ++mMission_EngineTicks;
    mPhase_InterruptTicks = 0;

    if (mStartParams->mDemoRecord && !mStartParams->mDemoRecordResumeCycle)
    {
        mGame_Data.mDemoRecorded.AddVideoTick(mGame_Data.mDemoRecorded.mTick);
    }

    if (mCamera_Start_Adjust)
    {
        Camera_SetTargetToStartPosition();
        mCamera_Start_Adjust = false;
        return 1;
    }

    // loc_1074E
    if (mGUI_Sidebar_Setup >= 0 && !mPhase_TryAgain)
        GUI_Sidebar_Setup();
    else
    {
        GUI_Sidebar_Draw();
    }

    // loc_10768
    Phase_Progress_Check();
    mHelicopterCallPadPressedCount = 0;
    if (word_3A9B8 >= 0)
        --word_3A9B8;

    Sprite_Find_HumanVehicles();

    // Cheat
    if (mDebug_PhaseSkip == -1)
    {
        mDebug_PhaseSkip = 0;
        mPhase_Complete = true;
    }
    else
        Phase_Goals_Check();

    // loc_1079C
    Squad_Walk_Steps_Decrease();
    Squad_Rebuild();

    Mission_Sprites_Handle();
    Squad_Switch_Timer();

    // Mouse_DrawCursor();

#ifndef EMSCRIPTEN
    if (mSurface->isPaletteAdjusting())
        mSurface->palette_FadeTowardNew();
#endif
    if (mPhase_ShowMapOverview && mSurfaceMapOverview)
    {

        // Dont show the map while recording
        if (!mParams->mDemoRecord)
            Phase_Map_Overview_Show();

        mPhase_ShowMapOverview = 0;
    }

    if (mGame_Data.mGamePhase_Data.mIsComplete)
    {

        if (mPhase_Aborted)
            Sprite_Handle_Player_Destroy_Unk();
        else
        {
            if (!mPhase_TryAgain)
                return 0;

            Sprite_Handle_Player_DestroyAll();
        }
        return -1;
    }

    // loc_10841
    Sprite_Bullet_SetData();
    Sound_Tick();
    Squad_EnteredVehicle_TimerTick();
    Squad_Set_CurrentVehicle();
    Music_Check_MapTile_TrackChange();

    // No squad is selected, so set count down timer
    if (mSquad_Selected < 0 && !mSquad_Select_Timer)
        mSquad_Select_Timer = 0x14;

    Sprite_HelicopterCallPad_Check();
    Mission_Final_Timer();

    return 1;
}

void cFodder::Phase_Paused()
{
    Draw_Phase_Paused();

    // Fade the background out, and the 'mission paused' message in
    mSurface->palette_FadeTowardNew();
    mSurface->palette_FadeTowardNew();
    mSurface->palette_FadeTowardNew();

    while (mPhase_Paused)
    {

        mSurface->draw();

        // Copy the rendered surface of the 'mission paused' message over the top of the main surface
        mSurface->mergeFrom(mSurface2);

        Video_Sleep();
    }

    mGraphics->PaletteSet();
    mSurface->palette_SetFromNew();
    mSurface->surfaceSetToPalette();

    mPhase_Aborted = false;

    // Redraw the screen
    mGraphics->MapTiles_Draw();
    Sprites_Draw();
    mGraphics->Sidebar_Copy_To_Surface();
}

void cFodder::Phase_Prepare()
{

    Map_Load();
    Map_Load_Sprites();
    Map_Overview_Prepare();

    // Prepare Squads
    Phase_Soldiers_Count();
    mGame_Data.Soldier_Sort();
    Phase_Soldiers_Prepare(false);
    Phase_Soldiers_AttachToSprites();

    mPhase_Aborted = false;

    Map_Load();
    mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);

    MapTiles_ResetScrollState();
    Camera_Reset();

    Mouse_ReadInputs();
    Sprite_Frame_Modifier_Update();

    mSound->Stop();
    Sprite_Aggression_Set();

    // seg000:05D1

    Phase_Goals_Set();
    Map_Collect_TileTracks();

    Sprite_Bullet_SetData();
    Sprite_Handle_Loop();
    Sprite_Create_Rank();

    mCamera_Start_Adjust = true;
    mCamera_StartPosition_X = mSprites[0].mPosX;
    mCamera_StartPosition_Y = mSprites[0].mPosY;

    Music_Play_Tileset();
    word_82176 = 0;

    // Is map 17 x 12
    {
        if (mMapLoaded->getWidth() == 17)
        {
            if (mMapLoaded->getHeight() == 12)
                mPhase_MapIs17x12 = -1;
        }
    }

    GUI_Element_Reset();
    mInput_Enabled = true;
    Camera_Prepare();

    mGUI_Mouse_Modifier_X = 0;
    mGUI_Mouse_Modifier_Y = 4;
    mCamera_Start_Adjust = true;

    Squad_Prepare_Explosives();

    mGraphics->PaletteSet();

    GUI_Sidebar_Prepare_Squads();
    Squad_Select_Grenades();
    mMap_Destroy_Tiles.clear();
    Sprite_Count_HelicopterCallPads();
    Mission_Set_Final_TimeRemaining();

    mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;

    mPhase_Aborted = false;
    mPhase_Paused = false;
    mPhase_In_Progress = true;
    mPhase_EscapeKeyAbort = false;

    mPhase_Finished = false;
    mPhase_ShowMapOverview = 0;

    Window_UpdateScreenSize();

    mSurface->Save();
}

int16 cFodder::Phase_Loop()
{
    int16 result = 1;

    // -1 = Phase Try Again
    //  0 = Phase Won
    //  1 = Phase Running
    for (result = 1; result == 1; result = Phase_Cycle())
    {

        // Demo playback mode
        if (mStartParams->mDemoPlayback)
        {
            if (mPhase_Aborted)
                return -1;

            {
                std::lock_guard<std::mutex> lock(mSurfaceMtx);
                mVideo_Ticked = false;
                mVideo_Done = true;

                // mStartParams->mDisableVideo = false;
                Video_SurfaceRender(false, false);
                mSurface->Restore();

                mWindow->Cycle();
                eventsProcess();
            }

#ifdef EMSCRIPTEN
            mVideo_Ticked = true;
#else
            while (!mVideo_Ticked && !mExit)
            {
                SDL_Delay(0);
            }
#endif
        }
    }

    return result;
}

void cFodder::Video_Sleep(cSurface *pSurface, const bool pShrink, const bool pVsync)
{
    mVideo_Ticked = false;
    mVideo_Done = true;
    if (!pSurface)
    {
        pSurface = mSurface;
    }

    {
        std::lock_guard<std::mutex> lock(mSurfaceMtx);

        if (!mStartParams->mDisableVideo)
        {
            if (!pShrink)
            {
                Video_SurfaceRender(false, false, pSurface);
            }
            else
            {
                Video_SurfaceRender(false, true, pSurface, false);
            }
            mSurface->Restore();
        }
    }

    // If not in vsync mode, then wait for the Amiga 50Hz Interrupt
#if defined(_OFED) || defined(EMSCRIPTEN)
    mVideo_Ticked = true;
#else
    if (!pVsync)
    {
        while (!mVideo_Ticked)
        {
            SDL_Delay(1);
        }
    }
#endif

    mWindow->Cycle();
    eventsProcess();
}

/**
 * Interrupt screen redraw
 */
void cFodder::Interrupt_Redraw()
{
    {
        std::lock_guard<std::mutex> lock(mSurfaceMtx);

        if (mParams->mDemoPlayback || mParams->mDemoRecord)
            mGame_Data.mDemoRecorded.Tick();

        Phase_Loop_Interrupt();

        if (mInterruptCallback)
        {
            mInterruptCallback();
        }
        else
        {
            if (mPhase_In_Progress && !mPhase_ShowMapOverview)
            {
                if (!mStartParams->mDisableVideo)
                {
                    mGraphics->MapTiles_Draw();
                }
                Sprites_Draw();
                mGraphics->Sidebar_Copy_To_Surface(0, mSurface);
                Mouse_DrawCursor();
            }
        }
        mVideo_Done = false;
        mVideo_Ticked = true;
    }
}

/**
 * Interrupt Simulation
 */
void cFodder::Interrupt_Sim()
{
    Uint32 startTick = SDL_GetTicks();

    while (!mExit)
    {
        Uint32 currentTick = SDL_GetTicks();

        if (currentTick - startTick >= g_Fodder->mParams->mSleepDelta)
        {
            startTick = currentTick;

            Interrupt_Redraw();

            // wait for video_sleep call to finish
            while (!mVideo_Done && !mExit)
            {
                SDL_Delay(g_Fodder->mParams->mSleepDelta == 0 ? 0 : 1);
            }
        }
        else
        {
            SDL_Delay(1);
        }
    }
}

#ifdef EMSCRIPTEN
void cFodder::Interrupt_Sim_Tick()
{
    static Uint32 startTick = SDL_GetTicks();
    Uint32 currentTick = SDL_GetTicks();

    if (currentTick - startTick < mParams->mSleepDelta)
    {
        // return;
    }

    startTick = currentTick;
    Interrupt_Redraw();
}
#endif

void cFodder::Phase_Loop_Interrupt()
{

    // Input disabled or Mission not paused?
    if (!mInput_Enabled || !mPhase_Paused)
    {

        ++mPhase_InterruptTicks;
        ++mInterruptTick;

        Mouse_ReadInputs();

        if (mInput_Enabled)
        {
            Camera_Handle();

            if (!mPhase_Finished)
                Mouse_Inputs_Check();
        }
    }

    if (mSound)
    {
        Music_Increase_Channel_Volume();
    }
    // sub_A3DBC
    // sub_8A27A
    //
    // loc_108AE
    if (mSquad_SwitchWeapon)
    {
        --mSquad_SwitchWeapon;
        Squad_Switch_Weapon();
    }

    if (mMouseCursor_Enabled)
        Mouse_DrawCursor();

    if (!mInput_Enabled)
        return;

    if (!mPhase_In_Progress)
        return;

    if (mPhase_Completed_Timer || mMapTile_Music_Play || mPhase_Complete || mPhase_TryAgain || mPhase_Aborted)
    {
        mPhase_Finished = true;
        return;
    }

    if (!(mInterruptTick & 3))
        Music_Fade_SwitchTrack();

    // sub_A3B14

    if (!mSoundEffectToPlay)
        return;

    Sound_Play(mSquad_Leader, mSoundEffectToPlay, 0);
    mSoundEffectToPlay = 0;
}

void cFodder::GameData_Reset()
{
    mDebug_PhaseSkip = 0;
    mInput_Enabled = false;
    mPhase_InterruptTicks = 0;
    mMission_EngineTicks = 0;
    mRecruit_Mission_Restarting = false;

    for (unsigned int x = 0; x < 9; ++x)
    {
        mMission_Troops_SpritePtrs[x] = 0;
    }

    mPhase_TryingAgain = false;
    mGame_Data.Clear();
    mGame_Data.mGamePhase_Data.Clear();
}

void cFodder::GameData_Backup()
{
    mGame_Data_Backup = mGame_Data;
}

void cFodder::GameData_Restore()
{
    mGame_Data = mGame_Data_Backup;

    // Reset grave pointers
    mGame_Data.mGamePhase_Data.mHeroesCount = mGame_Data.mHeroes.size();
}

void cFodder::Phase_EngineReset()
{
    // Clear memory 2454 to 3B58

    mPhase_EscapeKeyAbort = false;
    mPhase_Aborted2 = false;
    mButtonPressLeft = 0;
    mButtonPressRight = 0;
    mMouse_Button_Left_Toggle = 0;
    mMouse_Button_Right_Toggle = 0;
    mMouse_Button_LeftRight_Toggle = false;
    mMouse_Button_LeftRight_Toggle2 = false;

    mMapTile_Music_Play = false;
    mVehicle_Input_Disabled = false;
    mMouse_Exit_Loop = false;
    mSquad_Member_Fire_CoolDown_Override = false;

    mCameraX = 0;
    mCameraY = 0;
    dword_39F36 = 0;
    mCamera_Scroll_Speed = 0;
    mCamera_SquadLeaderX = 0;
    mCamera_SquadLeaderY = 0;
    mCamera_Panning_ToTarget = false;

    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;
    mCamera_AccelerationX = 0;

    mCamera_AccelerationY = 0;
    mCamera_Speed_Reset_X = 0;
    mCamera_Speed_Reset_Y = 0;
    mCamera_TileSpeed_Overflow = 0;
    mCamera_TileX = 0;
    mCamera_TileY = 0;
    mCamera_TileSpeedX = 0;
    mCamera_TileSpeedY = 0;

    mKeyCodeAscii = 0;
    mMapTile_TargetX = 0;
    mMapTile_TargetY = 0;
    mMapTile_SpeedX = 0;
    mMapTile_SpeedY = 0;

    mMapTile_MoveDirectionX_Previous = 0;
    mMapTile_MoveDirectionY_Previous = 0;
    mMapTile_DrawX = 0;
    mMapTile_DrawY = 0;
    mMapTile_SpeedX_Previous = 0;
    mMapTile_SpeedY_Previous = 0;
    mMapTile_MovedHorizontal = 0;
    mMapTile_MovedVertical = 0;
    mMapTile_ColumnOffset = 0;
    mMapTile_RowOffset = 0;

    mSquad_Leader = 0;
    mMapTile_MoveDirectionX = 0;
    mMapTile_MoveDirectionY = 0;
    mCamera_MoveDirectionX = 0;
    mCamera_MoveDirectionY = 0;
    mCamera_MovePauseX = 0;
    mCamera_MovePauseY = 0;
    mSquad_Selected = 0;
    mSquad_JoiningTo = 0;

    mSprite_Frame_1 = 0;
    mSprite_Frame_Modifier_Update_Countdown = 0;
    mSprite_Frame_2 = 0;
    mSprite_Frame3_ChangeCount = 0;
    mSprite_Frame_3 = 0;

    mTroop_Weapon_Grenade_Disabled = false;
    mTroop_Weapon_Bullet_Disabled = false;
    mTroop_Weapon_Rocket_Disabled = false;
    mSidebar_SmallMode = 0;
    dword_3A030 = 0;
    mCamera_PanTargetX = 0;
    mCamera_PanTargetY = 0;
    mCamera_StartPosition_X = 0;

    mStoredSpriteX = 0;
    mStoredSpriteY = 0;
    mTmp_FrameNumber = 0;

    mGame_Data.mGamePhase_Data.mIsComplete = false;
    mSidebar_Draw_Y = 0;
    word_3A3BF = 0;
    mDirectionMod = 0;
    mPhase_Aborted = false;
    mPhase_Aborted2 = false;
    mSquad_SwitchWeapon = 0;
    word_3A9B8 = 0;
    for (uint8 x = 0; x < 3; ++x)
    {
        mSquad_Walk_Target_Indexes[x] = 0;
        mSquad_Walk_Target_Steps[x] = 0;
    }

    mSprite_Bumped_Into_SquadMember = 0;
    mSprite_Player_CheckWeapon = 0;
    mPhase_Paused = false;
    for (uint8 x = 0; x < 4; ++x)
    {
        mSprite_Projectile_Counters[x] = 0;
        mSprite_Missile_Projectile_Counters[x] = 0;
    }

    mMouseX_Offset = 0;
    mMouseY_Offset = 0;
    mMouseSpriteNew = 0;

    mCamera_Speed_Max = 0;

    mMouseSetToCursor = 0;
    mSprites_Found_Count = 0;

    for (uint16 x = 0; x < 3; ++x)
    {
        mSquad_Grenades[x] = 0;
        mSquad_Rockets[x] = 0;
        mGUI_Squad_Icon[x] = 0;
    }

    mTroops_Enemy_Count = 0;
    mHostage_Count = 0;
    mEnemy_BuildingCount = 0;

    mCamera_Start_Adjust = false;
    word_3AA1D = 0;
    mCamera_Reached_Target = 0;
    word_3AA21 = 0;
    mSprite_FaceWeaponTarget = 0;
    word_3AA45 = 0;
    mSquad_Select_Timer = 0;
    mSprite_Find_Distance = 0;
    mMouseCursor_Enabled = 0;
    mRecruit_Sidebar_Draw_Y_Start = 0;
    mRecruit_Render_Name_SmallGap = 0;
    mRecruit_Truck_Frame = 0;
    mRecruit_Truck_FrameAnimBufferPtr = 0;
    mRecruit_Screen_Active = false;

    word_3ABB1 = 0;
    mSquad_Member_Fire_CoolDown = 0;
    mTroop_Rotate_Next = 0;
    mPhase_MapIs17x12 = 0;
    mSprite_Weapon_Data.mSpeed = 0;
    mSprite_Weapon_Data.mAliveTime = 0;
    mSprite_Weapon_Data.mCooldown = 0;
    mSprite_Weapon_Data.mDeviatePotential = 0;
    mSprite_Weapon_Data.field_8 = 0;

    mSprite_Reached_Target = 0;
    mStoredSpriteFrame = 0;
    mSprite_Bullet_Time_Modifier = 0;
    mSprite_Bullet_Fire_Speed_Modifier = 0;
    mSquad_Member_Clicked_TroopInSameSquad = 0;
    mSquad_Member_Clicked_TroopPtr = 0;

    mString_GapCharID = 0;
    mGUI_Loop_Squad_Current = 0;
    mGUI_Loop_Draw_Y = 0;
    word_3AC2D[0] = 0;
    word_3AC2D[1] = 0;
    word_3AC2D[2] = 0;

    mGUI_Print_String_To_Sidebar = 0;
    mGUI_Squad_NextDraw_Y = 0;
    mGUI_Sidebar_Setup = 0;
    mGUI_RefreshSquadGrenades[0] = 0;
    mGUI_RefreshSquadGrenades[1] = 0;
    mGUI_RefreshSquadGrenades[2] = 0;
    mGUI_RefreshSquadRockets[0] = 0;
    mGUI_RefreshSquadRockets[1] = 0;
    mGUI_RefreshSquadRockets[2] = 0;

    mSquad_CurrentWeapon[0] = eWeapon_None;
    mSquad_CurrentWeapon[1] = eWeapon_None;
    mSquad_CurrentWeapon[2] = eWeapon_None;

    mMouseDisabled = 0;
    mGUI_Loop_Is_CurrentSquad = 0;

    word_3AC4B = 0;
    word_3AC4D = 0;
    word_3AC51 = 0;

    mMap_Destroy_Tile_LastTile = 0;
    mMap_Destroy_Tile_X = 0;
    mMap_Destroy_Tile_Y = 0;

    mMap_Destroy_Tiles.clear();

    mMap_Destroy_Tiles_Countdown = 0;

    for (uint16 x = 0; x < 42; ++x)
    {
        mGUI_Elements[x].field_0 = 0;
        mGUI_Elements[x].mX = 0;
        mGUI_Elements[x].mWidth = 0;
        mGUI_Elements[x].mY = 0;
        mGUI_Elements[x].mHeight = 0;
        mGUI_Elements[x].mMouseInsideFuncPtr = 0;
    }

    mGUI_NextFreeElement = 0;
    mGUI_TroopName_DrawOffset = 0;

    mSprite_SpareUsed = 0;
    mSquad_WalkTargetX = 0;
    mSquad_WalkTargetY = 0;
    mSprite_Bullet_Destroy = 0;
    mSprite_Helicopter_DestroyLight = 0;
    mSprite_Bullet_Deviate_Counter = 0;
    dword_3B1CB = 0;

    for (uint16 x = 0; x < 0x0F; ++x)
        mRecruit_Hill_Positions_Use[x] = 0;

    mRecruit_Truck_Animation_Play = 0;
    mRecruit_Truck_Troops_ToEnter_Count = 0;

    mSquad_CurrentVehicle = 0;
    mPhase_In_Progress = false;
    mSprite_HumanVehicles_Found = false;
    mSprites_HumanVehicles.clear();
    dword_3B24B = 0;

    for (uint16 x = 0; x < 3; ++x)
    {
        mSquad_CurrentVehicles[x] = 0;
    }

    word_3B25B = 0;
    word_3B25D = 0;
    mGUI_SaveLoadAction = 0;
    word_3B2CF = 0;
    for (uint16 x = 0; x < 6; ++x)
        word_3B2D1[x] = 0;

    mSprite_Find_Types[0] = 0;
    mSprite_Find_Types[1] = 0;
    mSprite_Find_Types[2] = 0;
    mSprite_Find_Types[3] = 0;
    mSprite_Find_Types[4] = 0;
    word_3B2ED = 0;
    mSpawnSpriteType = 0;
    word_3B2F1 = 0;
    word_3B2F3 = 0;
    mSprite_Field10_Saved = 0;
    word_3B2F7 = 0;
    mRecruit_Button_Load_Pressed = 0;
    mRecruit_Button_Save_Pressed = 0;

    mGUI_Temp_X = 0;
    mGUI_Temp_Width = 0;
    mGUI_Temp_Y = 0;
    mGUI_Draw_LastHeight = 0;
    mInput.clear();
    mGUI_Select_File_CurrentIndex = 0;
    mGUI_Select_File_Count = 0;
    mGUI_Select_File_SelectedFileIndex = 0;
    mIntro_PlayTextDuration = 0;
    mSoundEffectToPlay_Set = 0;

    memset(mSound_Priority, 0, sizeof(mSound_Priority));
    memset(mSound_Timer, 0, sizeof(mSound_Timer));

    for (uint16 x = 0; x < 3; ++x)
        mSquad_EnteredVehicleTimer[x] = 0;

    mSprite_OpenCloseDoor_Ptr = 0;
    mSprite_Civilian_GotHome = 0;
    mSwitchesActivated = false;
    mSprite_Civilian_Tmp_X = 0;
    mSprite_Civilian_Tmp_Y = 0;
    word_3B481 = 0;
    word_3B483 = 0;
    mHelicopterCallPadCount = 0;
    mHelicopterCallPadPressedCount = 0;

    for (uint16 x = 0; x < 16; ++x)
    {
        mSprite_TroopsAlive[x] = 0;
    }
    mHelicopterCall_X = 0;
    mHelicopterCall_Y = 0;
    mMission_Final_TimeToDie_Ticker = 0;
    mMission_Final_TimeRemain = 0;
    mMission_Final_TimeToAbort = 0;
    mGUI_Sidebar_MapButton_Prepared = 0;
    mPhase_ShowMapOverview = 0;

    mTurretFires_HomingMissile = 0;
    word_3B4ED[0] = 0;
    word_3B4ED[1] = 0;
    mPhase_Finished = false;
    mImage_Aborted = 0;
    mBriefing_Aborted = 0;
    mHostage_Rescue_Tent = 0;

    mSprite_Missile_LaunchDistance_X = 0;
    mSprite_Missile_LaunchDistance_Y = 0;
    mKeyControlPressed = 0;

    mVideo_Draw_PosX = 0;
    mVideo_Draw_PosY = 0;

    mMapTile_Column_CurrentScreen = 0;
    mMapTile_Row_CurrentScreen = 0;

    mTroop_InRange_Callpad = 0;
    mMusicTrack_unk = 0;
    mMusic_CurrentTrack = 0;
    mMusic_TargetVolume = 0;
    mMusic_SlowVolumeDecrease = 0;
    word_829F6 = 0;

    mSprite_DrawList_Final.clear();
}

void cFodder::Phase_SquadPrepare()
{
    mSquad_Grenade_SplitMode = eSquad_Weapon_Split_Half;
    mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;
    mGUI_Sidebar_TroopList_Name_BreakOnSpace = 5;
    // -1
    mSquads[0] = mSquad_0_Sprites;
    mSquads[1] = mSquad_1_Sprites;
    mSquads[2] = mSquad_2_Sprites;
    mSquads[3] = mSquad_3_Sprites;
    mSquads[4] = mSquad_4_Sprites;

    for (unsigned int x = 0; x < 9; ++x)
        mSquad_0_Sprites[x] = INVALID_SPRITE_PTR;

    for (unsigned int x = 0; x < 9; ++x)
        mSquad_1_Sprites[x] = INVALID_SPRITE_PTR;

    for (unsigned int x = 0; x < 9; ++x)
        mSquad_2_Sprites[x] = INVALID_SPRITE_PTR;

    for (unsigned int x = 0; x < 9; ++x)
        mSquad_3_Sprites[x] = INVALID_SPRITE_PTR;

    for (unsigned int x = 0; x < 9; ++x)
        mSquad_4_Sprites[x] = INVALID_SPRITE_PTR;

    mSprite_Frame1_Modifier = 1;
    mSprite_Frame2_Modifier = 1;
    mSprite_Frame_3_Modifier = 1;
    mGUI_Mouse_Modifier_X = 0x1A;
    mGUI_Mouse_Modifier_Y = 0x12;

    word_3BED5[0] = 2;
    word_3BED5[1] = 2;
    word_3BED5[2] = 2;
    word_3BED5[3] = 2;
    word_3BED5[4] = 2;

    mSquad_Join_TargetSquad[0] = -1;
    mSquad_Join_TargetSquad[1] = -1;
    mSquad_Join_TargetSquad[2] = -1;

    mSquad_Join_TargetSprite[0] = 0;
    mSquad_Join_TargetSprite[1] = 0;
    mSquad_Join_TargetSprite[2] = 0;
    mSquad_Join_TargetSprite[3] = 0;
    mSquad_Join_TargetSprite[4] = 0;
    mSquad_Join_TargetSprite[5] = 0;

    mMouse_Locked = false;

    mSquad_Selected = 0;
    mPhase_TryAgain = false;
    mPhase_Complete = false;
    mPhase_Completed_Timer = 0;

    mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;

    mSquads_TroopCount[0] = 0;
    mSquads_TroopCount[1] = 0;
    mSquads_TroopCount[2] = 0;
    mSquads_TroopCount[3] = 0;

    mCheckPattern_Position.mX = 0;
    mCheckPattern_Position.mY = 0;

    mCamera_StartPosition_X = 0;
    mCamera_StartPosition_Y = 0;

    Squad_Walk_Target_SetAll(-1);
}

int16 cFodder::Tile_FindType(eTerrainFeature pType)
{

    for (int16 TileID = 0; TileID < sizeof(mTile_Hit) / sizeof(int16); ++TileID)
    {

        int16 TerrainType = mTile_Hit[TileID];

        // Single Type Tile
        if (TerrainType >= 0)
        {

            // Match?
            if ((TerrainType & 0x0F) == pType)
            {

                return TileID;
            }
        }
        else
        {

            if (TerrainType < 0)
            {
                // loc_2A8D9
                //  X
                /*pData10 >>= 1;
                pData10 &= 0x07;

                int16 TilePixel = 7;
                TilePixel -= pData10;

                // Y
                pData14 >>= 1;
                pData14 &= 0x07;

                int8 RowTerrainType = mTile_BHit[TileID][pData14];

                // If the bit for this X position is set, we use the UpperBits for the terrain type
                if (RowTerrainType & (1 << TilePixel))
                    TerrainType >>= 4;*/
            }
        }
    }

    return -1;
}

std::vector<int16> cFodder::Tile_FindType(const eTerrainFeature pType, const eTerrainFeature pType2)
{
    std::vector<int16> Results;

    for (int16 TileID = 0; TileID < sizeof(mTile_Hit) / sizeof(int16); ++TileID)
    {

        int16 TerrainType = mTile_Hit[TileID];

        // Second Type?
        if (TerrainType < 0)
        {
            auto Type1 = (TerrainType & 0x0F);
            auto Type2 = (TerrainType >> 4) & 0x0F;

            if ((Type1 == pType && Type2 == pType2) || (Type1 == pType2 || Type2 == pType))
            {

                Results.push_back(TileID);
            }
        }
    }

    return Results;
}

void cFodder::Phase_Soldiers_Count()
{
    mGame_Data.mGamePhase_Data.mSoldiers_Required = 0;
    sSprite *Sprite = mSprites.data();

    // TODO: This counter needs fixing
    // How many player sprites are on this map
    for (int16 mTmpCount = 0x1D; mTmpCount > 0; --mTmpCount, ++Sprite)
    {
        if (Sprite->mPosX != -32768)
        {

            if (Sprite->mSpriteType == eSprite_Player)
                ++mGame_Data.mGamePhase_Data.mSoldiers_Required;
        }
    }

    // How many recruits have already been allocated to the mission
    mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count = 0;

    for (auto &Troop : mGame_Data.mSoldiers_Allocated)
    {

        if (Troop.mRecruitID != -1)
        {
            --mGame_Data.mGamePhase_Data.mSoldiers_Required;
            ++mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count;
        }
    }

    // Do we still require troops?
    int16 ax = mGame_Data.mGamePhase_Data.mSoldiers_Required;
    if (mGame_Data.mGamePhase_Data.mSoldiers_Required >= 0)
    {

        // Do we need more than available?
        if (ax > mGame_Data.mRecruits_Available_Count)
            ax = mGame_Data.mRecruits_Available_Count;

        // Set the Required amount
        mGame_Data.mGamePhase_Data.mSoldiers_Required = ax;

        // Reduce the available recruits by the number taken by the mission
        mGame_Data.mRecruits_Available_Count -= ax;
    }

    mGame_Data.mGamePhase_Data.mSoldiers_Available = mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count + ax;
}

void cFodder::Phase_Soldiers_Prepare(const bool pPrebriefing)
{
    Mission_Troops_Clear_Selected();

    if (!pPrebriefing)
    {

        // Set the sMission_Troop Sprites from mMission_Troops_SpritePtrs
        if (mGame_Data.mGamePhase_Data.mSoldiers_Prepare_SetFromSpritePtrs)
        {
            sSprite **Data24 = mMission_Troops_SpritePtrs;

            for (auto &Troop : mGame_Data.mSoldiers_Allocated)
            {
                Troop.mSprite = *Data24++;
            }
        }

        mGame_Data.mGamePhase_Data.mSoldiers_Prepare_SetFromSpritePtrs = true;

        sSprite **Data24 = mMission_Troops_SpritePtrs;
        for (auto &Troop : mGame_Data.mSoldiers_Allocated)
        {
            *Data24++ = Troop.mSprite;
        }
    }

    // Join recruits into the squad
    int16 Data1C = mGame_Data.mGamePhase_Data.mSoldiers_Required - 1;
    while (Data1C >= 0)
    {
        Mission_Troop_Prepare_Next_Recruits();
        --Data1C;
    }

    if (pPrebriefing)
        return;

    // Remove recruits which arn't needed for the map
    Data1C = mGame_Data.mGamePhase_Data.mSoldiers_Available;
    for (auto &Troop : mGame_Data.mSoldiers_Allocated)
    {

        if (Troop.mSprite == INVALID_SPRITE_PTR)
            continue;

        if (Data1C)
            --Data1C;
        else
            Troop.mSprite = INVALID_SPRITE_PTR;
    }
}

void cFodder::Mission_Troop_Prepare_Next_Recruits()
{

    // Loop each troop
    for (auto &Troop : mGame_Data.mSoldiers_Allocated)
    {

        // Does troop member have a recruit id?
        if (Troop.mRecruitID == -1)
        {

            if (mGame_Data.mRecruit_NextID >= 360)
                return;

            Troop.mRecruitID = mGame_Data.mRecruit_NextID;

            // All troops are equal during unit testing
            if (mParams->mUnitTesting)
            {
                Troop.mRank = 0;
            }
            else
            {
                // Demo sets static ranks
                if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set)
                {

                    Troop.mRank = (mGame_Data.mMission_Number - 1) >> 1;

                    // Jops
                    if (Troop.mRecruitID == 1)
                        Troop.mRank = 2;

                    // Jools
                    if (Troop.mRecruitID == 0)
                        Troop.mRank = 4;
                }
                else
                {
                    Troop.mRank = (mGame_Data.mMission_Number - 1) / 3;
                }
            }

            Troop.mAnimationVariant = 3;
            ++mGame_Data.mRecruit_NextID;

            return;
        }
    }
}

void cFodder::Phase_Soldiers_AttachToSprites()
{

    int16 TroopsRemaining = mGame_Data.mGamePhase_Data.mSoldiers_Available;
    sSprite *Sprite = mSprites.data();
    sMission_Troop *Troop = mGame_Data.mSoldiers_Allocated;

    // TODO: This counter needs fixing
    // Loop the game sprites looking for 'player' sprite
    for (int16 Data18 = 0x1D; Data18 >= 0; --Data18, ++Sprite)
    {

        if (Sprite->mPosX == -32768)
            continue;

        if (Sprite->mSpriteType != eSprite_Player)
            continue;

        //
        if (--TroopsRemaining < 0)
        {
            Troop->mSprite = INVALID_SPRITE_PTR;
            Sprite->mPosX = -32768;
            Sprite->mSpriteType = eSprite_Null;
            Sprite->mSheetIndex = 0x7C;
            ++Troop;
        }
        else
        {
            // loc_1166B

            // Attach a Mission Troop to the sprite
            Sprite->mMissionTroop = Troop;

            Troop->mSprite = Sprite;
            Sprite->mDirection = 0x40;
            Sprite->mPersonType = eSprite_PersonType_Human;

            ++Troop;
        }
    }
}

void cFodder::Music_Check_MapTile_TrackChange()
{
    if (mPhase_Completed_Timer != 0 || mGame_Data.mGamePhase_Data.mIsComplete || mMapTile_Music_Play)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return;

    int16_t spriteX = mSquad_Leader->mPosX >> 4;
    int16_t spriteY = mSquad_Leader->mPosY >> 4;

    int16_t BestDistance = 0x10;
    int16_t PlaySong = 0;

    for (const auto &tile : mMapTileTracks)
    {
        if (tile.X < 0)
            break;

        if (tile.Track > 4)
            continue;

        // Call to sub_A10F2
        int16_t d0 = spriteX;
        int16_t d1 = spriteY;
        int16_t d2 = tile.X;
        int16_t d3 = tile.Y;
        int16_t d4 = 0x0F;
        int16_t result = Map_Get_Distance_BetweenPoints(d0, d1, d2, d4, d3);

        if (result < BestDistance)
        {
            BestDistance = result;
            PlaySong = tile.Track;
        }
    }

    int16_t d0, d1;

    if (BestDistance > 4)
    {
        if (BestDistance >= 0xA)
        {
            d0 = 1;
            d1 = (BestDistance - 8) << 1;
            d1 = d1 < 0x30 ? d1 : 0x30;
        }
        else
        {
            d0 = PlaySong;
            d1 = (BestDistance - 5) << 3;
            d1 = 0x38 - d1;
        }
    }
    else
    {
        d0 = PlaySong;
        d1 = 0x40;
    }

    Music_setup_track_unk(d0, d1);
}

void cFodder::Music_Fade_SwitchTrack()
{
    int16_t d0 = mMusicTrack_unk;

    if (d0 == 0)
        return;

    int16_t d1 = mSound->Music_GetVolume(0); // byte_A4B5E[0x28];

    // Changing track?
    if (mMusic_CurrentTrack != d0)
    {
        --d1;

        // Once volume below 8, change to new song
        if (d1 < 8)
        {
            d1 = 8;
            mMusic_CurrentTrack = d0;
            Music_Play_Tileset(d0);
        }
    }
    else
    {

        if (d1 < mMusic_TargetVolume)
        {
            d1++;
        }
        else
        {
            d1 = mMusic_TargetVolume;
            mMusicTrack_unk = 0;
        }
    }

    // loc_878F4
    //
    mSound->Music_SetVolume(-1, d1);
    // byte_A4B5E[0x28] = d1;

    if (word_829F6 == 0)
    {
        // byte_A4B5E[0x29] = d1;
    }
}

void cFodder::Music_setup_track_unk(int16_t d0, int16_t d1)
{
    if (mMusicTrack_unk != 0)
        return;

    if (mMusic_CurrentTrack != d0)
    {
        mMusic_TargetVolume = d1;
        mMusicTrack_unk = d0;
    }
    else
    {
        // byte_A4B5E[0x28] = (uint8_t)d1;
        mSound->Music_SetVolume(-1, d1);

        if (word_829F6 == 0)
        {
            // byte_A4B5E[0x29] = (uint8_t)d1;
        }
    }
}

void cFodder::Music_Increase_Channel_Volume()
{

    if (word_82176)
    {
        int16 d0 = mInterruptTick & 3;
        if (!d0)
        {
            for (int i = 0; i < 4; ++i)
            {
                int16 v = mSound->Music_GetVolume(i);
                if (v < 0x40)
                {
                    mSound->Music_SetVolume(i, ++v);
                }
            }
        }
    }

    if (word_82178)
    {
        Music_Decrease_Channel_Volume();
    }

    if (mMusic_SlowVolumeDecrease)
    {
        int16 d0 = mInterruptTick & 0x3F;
        if (!d0)
        {
            Music_Decrease_Channel_Volume();
        }
    }
}

bool cFodder::Music_Decrease_Channel_Volume()
{
    bool ret = false;
    if (mSound)
    {

        for (int i = 0; i < 4; ++i)
        {
            int16 v = mSound->Music_GetVolume(i);
            if (v > 0)
            {
                ret = true;
                mSound->Music_SetVolume(i, --v);
            }
        }
    }
    return ret;
}

/**
 * Load the Base and Sub Tile BLK Files
 *
 * @return true if both files are loaded
 */
bool cFodder::Tiles_Load_Data()
{
    std::string BaseName, SubName;

    // junbase.blk
    BaseName.append(mMap->data(), mMap->data() + 11);

    // junsub0.blk
    SubName.append(mMap->data() + 0x10, mMap->data() + 0x10 + 11);

    mTile_BaseBlk = mResources->fileGet(BaseName);
    mTile_SubBlk = mResources->fileGet(SubName);

    if (!mTile_BaseBlk->size() || !mTile_SubBlk->size())
        return false;

    return true;
}

void cFodder::Music_Play(int16 pTrack, int16 pSong)
{
    mMapTile_Music_Play = true;

    word_82176 = -1;
    word_82178 = 0;

    mSound->Music_Play(pTrack, pSong);
    mSound->Music_SetVolume(-1, 0);
}

void cFodder::Music_SetFullVolume()
{
    for (int i = 0; i < 4; ++i)
    {
        mSound->Music_SetVolume(i, 0x40);
    }
}

void cFodder::Music_Play_Tileset(int16_t pSong)
{

    if (!mStartParams->mDisableSound)
    {
        if (pSong == -1)
            pSong = 2;

        mSound->Music_Play(mMapLoaded->getTileType() + 0x32, pSong);
        mSound->Music_SetVolume(-1, 0);
    }
    mMapTile_Music_Play = false;
}

/**
 * Move the mouse position to follow the terrain as the camera pans
 */

void cFodder::Mission_Sprites_Handle()
{

#ifndef _OFED
    Sprite_Frame_Modifier_Update();
    Sprite_Handle_Loop();
#endif

    Sprite_Sort_DrawList();

    MapTile_UpdateFromCamera();

    Map_Destroy_Tiles();
    Sprites_Draw(mSurfaceFinal);
}

void cFodder::Phase_Goals_Check()
{

    int16 Enemys = 0;
    int16 Buildings = 0;

    for (auto &Sprite : mSprites)
    {
        sSprite *Data20 = &Sprite;

        if (Data20->mPosX == -32768)
            continue;

        int16 Data10 = Data20->mSpriteType;
        if (Data10 == eSprite_Computer_1 || Data10 == eSprite_Computer_2 || Data10 == eSprite_Computer_3 || Data10 == eSprite_BuildingDoor3 || Data10 == eSprite_BuildingDoor_Reinforced)
            goto loc_12620;

        if (Data10 == eSprite_BuildingDoor2 || Data10 == eSprite_BuildingDoor)
        {
            if (Data20->mAnimState == eSprite_Anim_Die1)
                continue;

        loc_12620:;
            if (Data20->mAnimState == eSprite_Anim_Die3)
                continue;

            ++Buildings;
            continue;
        }

        const int16 *Data24 = mEnemy_Unit_Types;
        for (; *Data24 >= 0; ++Data24)
        {
            if (Data10 != *Data24)
                continue;

            ++Enemys;
        }
    }

    // The one demo just has two objectives for each map
    if (mVersionCurrent->isAmigaTheOne())
    {

        if (Buildings || Enemys)
            return;

        mPhase_Complete = true;
        return;
    }

    mEnemy_BuildingCount = Buildings;
    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Destroy_Enemy_Buildings - 1])
    {
        if (Buildings)
            return;
    }

    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Kill_All_Enemy - 1])
    {
        if (Enemys)
            return;
    }

    if (!mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Kidnap_Leader - 1])
    {
        if (!mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Rescue_Hostages - 1])
            if (!mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Rescue_Hostage - 1])
                goto loc_126A6;
    }

    if (mHostage_Count)
        return;

loc_126A6:;

    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Get_Civilian_Home - 1])
    {
        if (!mSprite_Civilian_GotHome)
            return;
    }

    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Activate_All_Switches - 1])
    {
        if (!mSwitchesActivated)
            return;
    }

    mPhase_Complete = true;
}

void cFodder::Phase_Goals_Set()
{

    for (auto &Goal : mGame_Data.mGamePhase_Data.mGoals_Remaining)
        Goal = false;

    for (auto Goal : mGame_Data.mPhase_Current->mGoals)
    {

        mGame_Data.mGamePhase_Data.mGoals_Remaining[Goal - 1] = true;
    }
}

void cFodder::Phase_Progress_Check()
{

    if (!mPhase_Complete)
        if (!mPhase_Aborted)
            if (!mPhase_TryAgain)
                return;

    if (mPhase_Completed_Timer < 0)
        return;

    if (mPhase_Completed_Timer)
        goto loc_1280A;

    mPhase_Completed_Timer = 0x64;

    if (mPhase_Aborted)
    {
        mPhase_Completed_Timer = 0x32;
        goto MissionTryAgain;
    }

    if (mPhase_Complete)
    {
        if (mPhase_TryAgain)
            goto MissionTryAgain;

        Phase_Show_Complete();
        goto loc_1280A;
    }
    // loc_127E1
    if (mGame_Data.mRecruits_Available_Count)
    {
    MissionTryAgain:;

        // Try Again sprites not availabe in 'The One'
        if (!mVersionCurrent->isAmigaTheOne())
            Phase_Show_TryAgain();
    }
    else
    {
        // loc_127FA
        Phase_GameOver();
    }
loc_1280A:;

    if (mPhase_Completed_Timer == 0x19)
    {
        mSurface->paletteNew_SetToBlack();
    }
    --mPhase_Completed_Timer;
    if (mPhase_Completed_Timer && !mParams->mUnitTesting)
        return;

    mPhase_Completed_Timer = -1;
    mGame_Data.mGamePhase_Data.mIsComplete = true;
}

void cFodder::Phase_Show_Complete()
{
    Sprite_Destroy(&mSprites[40]);

    if (mGame_Data.mMission_Phases_Remaining == 1)
        Phase_TextSprite_Create_Mission(&mSprites[41]);
    else
        Phase_TextSprite_Create_Phase(&mSprites[41]);

    Phase_TextSprite_Create_Complete(&mSprites[42]);
}

void cFodder::Phase_TextSprite_Create_Mission(sSprite *pData2C)
{
    Phase_TextSprite_Prepare(pData2C);

    pData2C->mPosY -= 0x14;
    pData2C->mPosX += 0x12;
    pData2C->mSheetIndex = 0xA2;
    pData2C->mSpriteType = eSprite_Text_Mission;
    if (!mStartParams->mDisableSound)
        Music_Play(6);
    Music_SetFullVolume();
}

void cFodder::Phase_TextSprite_Create_Phase(sSprite *pData2C)
{
    Phase_TextSprite_Prepare(pData2C);

    pData2C->mPosY -= 0x14;
    pData2C->mPosX += 0x1B;
    pData2C->mSheetIndex = 0xA1;
    pData2C->mSpriteType = eSprite_Text_Phase;

    if (!mStartParams->mDisableSound)
        Music_Play(0x0C);

    Music_SetFullVolume();
}

void cFodder::Phase_TextSprite_Create_Complete(sSprite *pData2C)
{
    Phase_TextSprite_Prepare(pData2C);

    pData2C->mSheetIndex = 0xA0;
    pData2C->mSpriteType = eSprite_Text_Complete;
}

void cFodder::Phase_TextSprite_Prepare(sSprite *pData2C)
{

    pData2C->mPosX = mMapTile_TargetX >> 16;
    pData2C->mPosX += (getCameraWidth() / 2) - 52;

    pData2C->mPosY = mMapTile_TargetY >> 16;
    pData2C->mPosY += getCameraHeight() + 10;

    pData2C->mFrameIndex = 0;
    pData2C->mHeight = 0;
    pData2C->mRowsToSkip = 0;
    pData2C->field_32 = -1;
    pData2C->mDrawOrder = eSprite_Draw_OnTop;
}

void cFodder::Phase_Show_TryAgain()
{

    Sprite_Destroy(&mSprites[40]);

    Phase_TextSprite_Create_Try(&mSprites[41]);
    Phase_TextSprite_Create_Again(&mSprites[42]);

    if (!mStartParams->mDisableSound)
        Music_Play(0x0F);
    Music_SetFullVolume();
}

void cFodder::Phase_TextSprite_Create_Try(sSprite *pData2C)
{
    Phase_TextSprite_Prepare(pData2C);
    pData2C->mPosY -= 0x14;
    pData2C->mPosX += 0x12;
    pData2C->mSheetIndex = 0xCA;
    pData2C->mSpriteType = eSprite_Text_Try;
}

void cFodder::Phase_TextSprite_Create_Again(sSprite *pData2C)
{
    Phase_TextSprite_Prepare(pData2C);

    pData2C->mSheetIndex = 0xCB;
    pData2C->mSpriteType = eSprite_Text_Again;
}

void cFodder::Phase_Map_Overview_Show()
{

    // We allow the overview map to be shown on all versions in debug mode
#ifndef _DEBUG
    // Overview map is disabled for demos
    if (mVersionCurrent->isDemo() && !mVersionCurrent->isCustom())
        return;
#endif

    int16 word_3A016 = 0;
    mVideo_Draw_PosX = (mSquad_Leader->mPosX) + (mSurfaceMapLeft * 16);
    mVideo_Draw_PosY = (mSquad_Leader->mPosY - 0x10) + (mSurfaceMapTop * 16);

    mPhase_Paused = true;
    mInput_Enabled = false;

    if (mVideo_Draw_PosX < 0)
        mVideo_Draw_PosX = 0;

    if (mVideo_Draw_PosY < 0)
        mVideo_Draw_PosY = 0;

    mVideo_Draw_PaletteIndex = 0xF0;

    mGraphics->PaletteSetOverview();
    mSurfaceMapOverview->Save();
    mGraphics->SetImage(mSurfaceMapOverview);

    do
    {
        if (mSquad_Leader != INVALID_SPRITE_PTR)
        {
            ++word_3A016;
            word_3A016 &= 0x1F; // Original Value: 0x3F;

            if (word_3A016 < 0x10)
            {
                mVideo_Draw_FrameDataPtr = mGraphics->GetSpriteData(eGFX_PSTUFF);
                mVideo_Draw_Rows = 0x10;

                mVideo_Draw_FrameDataPtr += PLATFORM_BASED(0x46B8, ((113 * 40) + 6));
                mVideo_Draw_Columns = PLATFORM_BASED(0x10, 0x02);

                mGraphics->Video_Draw_8();
            }
            else
                mSurfaceMapOverview->Restore();
        }

        if (mPhase_Aborted)
            break;
        Video_Sleep(mSurfaceMapOverview, true);

    } while (Mouse_Button_Left_Toggled() < 0);

    mSurfaceMapOverview->Restore();
    mGraphics->SetImageOriginal();

    mPhase_Aborted = false;
    mPhase_Paused = false;
    mInput_Enabled = true;
}

void cFodder::eventProcess(const cEvent &pEvent)
{
    switch (pEvent.mType)
    {

    case eEvent_Focus:
        mWindow_Focus = pEvent.mHasFocus;
        break;

    case eEvent_KeyDown:
        keyProcess(pEvent.mButton, true);
        break;

    case eEvent_KeyUp:
        keyProcess(pEvent.mButton, false);
        break;

    case eEvent_MouseLeftDown:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastPositionRelative = pEvent.mPositionRelative;
        mMouse_EventLastButtonsPressed |= 1;
        break;

    case eEvent_MouseRightDown:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastPositionRelative = pEvent.mPositionRelative;
        mMouse_EventLastButtonsPressed |= 2;
        break;

    case eEvent_MouseLeftUp:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastPositionRelative = pEvent.mPositionRelative;
        mMouse_EventLastButtonsPressed &= ~1;
        break;

    case eEvent_MouseRightUp:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastPositionRelative = pEvent.mPositionRelative;
        mMouse_EventLastButtonsPressed &= ~2;
        break;

    case eEvent_MouseMove:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastPositionRelative = pEvent.mPositionRelative;
        break;

    case eEvent_MouseWheel:
        mMouse_EventLastWheel = pEvent.mPosition;
        break;

    case eEvent_None:
        break;

    case eEvent_Quit:
        Exit(0);
        break;
    }
}

void cFodder::eventsProcess()
{

    mMouse_EventLastWheel.Clear();

    if (mParams->mDemoPlayback)
    {
        for (auto Event : mGame_Data.mDemoRecorded.GetEvents(mGame_Data.mDemoRecorded.mTick))
            eventProcess(Event);
    }
    else
    {
        for (auto Event : *mWindow->EventGet())
        {
            if (mParams->mDemoRecord)
            {
                if (Event.mType != eEventType::eEvent_MouseMove)
                    mGame_Data.mDemoRecorded.AddEvent(mGame_Data.mDemoRecorded.mTick, Event);
            }
            eventProcess(Event);
        }
    }

    mWindow->EventGet()->clear();
}

void cFodder::keyProcess(uint8 pKeyCode, bool pPressed)
{
    if (pPressed)
        mKeyCode = pKeyCode;
    else
        mKeyCode = 0;

    // Switch between platforms
    if (!mVersionPlatformSwitchDisabled)
    {
        mVersionPlatformSwitchDisabled = true;
        if ((mVersionDefault && mVersionDefault->isRetail()) || (mVersionCurrent && mVersionCurrent->isRetail()))
        {

            if (pKeyCode == SDL_SCANCODE_F1 && pPressed)
            {
                mVersionDefault = mVersions->GetForCampaign(mVersionCurrent->mName, ePlatform::Amiga);
                mParams->mDefaultPlatform = ePlatform::Amiga;
                VersionSwitch(mVersionDefault);
            }
            if (pKeyCode == SDL_SCANCODE_F2 && pPressed)
            {
                mVersionDefault = mVersions->GetForCampaign(mVersionCurrent->mName, ePlatform::PC);
                mParams->mDefaultPlatform = ePlatform::PC;
                VersionSwitch(mVersionDefault);
            }
        }
        mVersionPlatformSwitchDisabled = false;
    }

    if (pKeyCode == SDL_SCANCODE_F3 && pPressed)
    {
        if (mParams->mDemoRecord)
        {
            mStartParams->mDemoRecordResumeCycle = mGame_Data.mDemoRecorded.mTick - 200;
            mGame_Data.mGamePhase_Data.mIsComplete = true;
            mPhase_TryAgain = true;
        }
    }

    if ((pKeyCode == SDL_SCANCODE_EQUALS && pPressed) || (pKeyCode == SDL_SCANCODE_KP_PLUS && pPressed))
    {
        mWindow->WindowIncrease();
    }

    if ((pKeyCode == SDL_SCANCODE_MINUS && pPressed) || (pKeyCode == SDL_SCANCODE_KP_MINUS && pPressed))
    {
        mWindow->WindowDecrease();
    }

    if (pKeyCode == SDL_SCANCODE_F11 && pPressed)
    {
        mWindow->ToggleFullscreen();
    }

    if (pKeyCode == SDL_SCANCODE_F12 && pPressed)
    {
        mParams->mMouseLocked = !mParams->mMouseLocked;

        if (mStartParams->mMouseAlternative)
            mWindow->SetRelativeMouseMode(mParams->mMouseLocked);
    }

    if (pKeyCode == SDL_SCANCODE_ESCAPE && pPressed && mPhase_Aborted)
        mPhase_Aborted2 = true;

    if (pKeyCode == SDL_SCANCODE_ESCAPE && pPressed)
    {
        mPhase_Aborted = true;
        mPhase_EscapeKeyAbort = true;
    }
    // In Mission and not on map overview
    if (mPhase_In_Progress && !mPhase_ShowMapOverview)
    {

        if (pKeyCode == SDL_SCANCODE_LCTRL || pKeyCode == SDL_SCANCODE_RCTRL)
        {
            if (pPressed)
                mKeyControlPressed = -1;
            else
                mKeyControlPressed = 0;
        }

        if (pKeyCode == SDL_SCANCODE_P && pPressed)
            mPhase_Paused = !mPhase_Paused;

        if (pKeyCode == SDL_SCANCODE_SPACE && pPressed)
            ++mSquad_SwitchWeapon;

        if (pKeyCode == SDL_SCANCODE_M && pPressed)
        {
            if (mPhase_Finished == false)
                mPhase_ShowMapOverview = -1;
        }

        if (pKeyCode == SDL_SCANCODE_1 && pPressed)
        {
            if (mSquads_TroopCount[0])
                Squad_Select(0, false);
        }

        if (pKeyCode == SDL_SCANCODE_2 && pPressed)
        {
            if (mSquads_TroopCount[1])
                Squad_Select(1, false);
        }
        if (pKeyCode == SDL_SCANCODE_3 && pPressed)
        {
            if (mSquads_TroopCount[2])
                Squad_Select(2, false);
        }

        if (mParams->mCheatsEnabled)
        {

            // Debug: Mission Complete
            if (pKeyCode == SDL_SCANCODE_F10 && pPressed)
            {
                mDebug_PhaseSkip = -1;
            }

            // Debug: Make current squad invincible
            if (pKeyCode == SDL_SCANCODE_F9 && pPressed)
            {
                if (mSquad_Selected >= 0)
                {
                    sSprite **Data28 = mSquads[mSquad_Selected];
                    for (; *Data28 != INVALID_SPRITE_PTR;)
                    {

                        sSprite *Data2C = *Data28++;
                        Data2C->field_75 |= eSprite_Flag_Invincibility;
                    }
                }
            }
        }
    }
}

void cFodder::WindowTitleSet(bool pInMission)
{
    std::stringstream Title;
    Title << mWindowTitle.str();

    if (pInMission && mGame_Data.mPhase_Current && mGame_Data.mMission_Current)
    {
        if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set)
        {
            Title << " ( Mission: ";
            Title << mGame_Data.mPhase_Current->mName;
        }
        else
        {
            Title << " ( Mission: " << mGame_Data.mMission_Number;
            Title << " " << mGame_Data.mMission_Current->mName;

            Title << "  Phase: " << (mGame_Data.mMission_Phase) << " ";

            if (mGame_Data.mMission_Current->NumberOfPhases() > 1)
            {
                Title << "of " << mGame_Data.mMission_Current->NumberOfPhases();
                Title << " " << mGame_Data.mPhase_Current->mName;
            }
            else
                Title << mGame_Data.mPhase_Current->mName;
        }

        Title << " )";
    }

    mWindow->SetWindowTitle(Title.str());
}

void cFodder::WindowTitleBaseSetup()
{

    mWindowTitle.str("");
    mWindowTitle << "Open Fodder";

    if (mVersionCurrent)
    {

        // Ensure we have a campaign
        if (mGame_Data.mCampaign.getName().size())
        {
            if (mGame_Data.mCampaign.isCustom())
            {
                mWindowTitle << ": Custom (" << mGame_Data.mCampaign.getName() << ")";
            }
            else
            {
                mWindowTitle << ": " << mGame_Data.mCampaign.getName();
            }
        }
    }

    WindowTitleSet(mPhase_In_Progress);
}

/**
 * This function loads a new version of the game, and is generally called on startup,
 * or AFTER a button on the campaign select screen is pushed
 *
 */
void cFodder::VersionSwitch(const sGameVersion *pVersion)
{
    const sGameVersion *VersionPrevious = mVersionCurrent;

    if (!pVersion)
        return;

    if (mVersionCurrent == pVersion)
        return;

    Image_FadeOut();

    mVersionCurrent = pVersion;

    WindowTitleBaseSetup();

    // Sound must be released first, to unlock the audio device
    // But only if we actually have to change the sound object
    if (mVersionCurrent && VersionPrevious && !(mVersionCurrent->CanUseAmigaSound() && VersionPrevious->CanUseAmigaSound()))
        mSound = 0;

    mResources = g_Resource = mVersionCurrent->GetResources();
    mGraphics = mVersionCurrent->GetGraphics();

    if (!mSound)
        mSound = mVersionCurrent->GetSound();

    if (!mResources)
    {
        std::cout << "Unknown Platform";
        exit(1);
    }

    mGUI_Select_File_ShownItems = PLATFORM_BASED(4, 5);

    Window_UpdateScreenSize();

    mGraphics->Load_Sprite_Font();
    mGraphics->Load_Hill_Data();
    mGraphics->Load_pStuff();

    if (mPhase_In_Progress)
    {
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);

        // Reload Map Data
        Map_Load_Resources();
        Map_Overview_Prepare();
        mGraphics->MapTiles_Draw();

        // Redraw sidebar
        mGUI_Sidebar_Setup = 0;
        while (mGUI_Sidebar_Setup >= 0)
            GUI_Sidebar_Setup();

        mSurface->palette_FadeTowardNew();
        mMusicTrack_unk = mMusic_CurrentTrack;
        mMusic_CurrentTrack = 0;
    }

    if (mRecruit_Screen_Active && mVersionCurrent->hasGfx(eGFX_HILL))
    {
        Recruit_Prepare();

        mRecruit_RenderedNext = mRecruit_Rendereds.begin();
        for (int i = 0; i < mGame_Data.mGamePhase_Data.mSoldiers_Required - mRecruit_Truck_Troops_ToEnter_Count; ++i)
        {
            Recruit_Sidebar_Render_SquadName();
        }

        if (!mStartParams->mDisableSound)
            Music_Play(0);
    }
}

int16 cFodder::getWindowWidth() const
{
    if (!mParams->mWindowColumns)
    {
        if (mVersionCurrent)
            return 320;

        return 352;
    }

    return (int16)mParams->mWindowColumns * 16;
}

cDimension cFodder::getSurfaceSize() const
{
    if (!mParams->mWindowColumns || !mParams->mWindowRows)
    {
        if (mVersionCurrent)
            return mVersionCurrent->GetScreenSize();

        return {352, 364};
    }

    return {(unsigned int)(mParams->mWindowColumns + 2) * 16, (unsigned int)(mParams->mWindowRows + 2) * 16};
}

cDimension cFodder::getWindowSize() const
{
    if (!mParams->mWindowColumns || !mParams->mWindowRows)
    {
        if (mVersionCurrent)
            return mVersionCurrent->GetScreenSize();

        return {336, 348};
    }

    return {(unsigned int)mParams->mWindowColumns * 16, (unsigned int)mParams->mWindowRows * 16};
}

int16 cFodder::getWindowRows() const
{
    if (!mParams->mWindowRows)
    {
        return 16;
    }
    return (int16)mParams->mWindowRows;
}

int16 cFodder::getWindowColumns() const
{
    if (!mParams->mWindowColumns)
    {
        if (mVersionCurrent->isAmiga())
            return 21;

        return 22;
    }
    return (int16)mParams->mWindowColumns;
}

bool cFodder::isAmigaMapBounds() const
{
    return mVersionCurrent && mVersionCurrent->isAmiga();
}

int16 cFodder::getMapVisibleOriginX() const
{
    if (isAmigaMapBounds())
        return 1;
    return 0;
}

int16 cFodder::getMapVisibleOriginY() const
{
    return 0;
}

int16 cFodder::getMapVisibleWidth() const
{
    if (!mMapLoaded)
        return 0;

    int16 width = static_cast<int16>(mMapLoaded->getWidth());
    if (isAmigaMapBounds())
    {
        width -= 2;
        if (width < 0)
            width = 0;
    }
    return width;
}

int16 cFodder::getMapVisibleHeight() const
{
    if (!mMapLoaded)
        return 0;

    int16 height = static_cast<int16>(mMapLoaded->getHeight());
    if (isAmigaMapBounds())
    {
        const int16 cameraTiles = (getCameraHeight() >> 4);
        if (height == cameraTiles + 1)
            height -= 1;
        if (height < 0)
            height = 0;
    }
    return height;
}

int32 cFodder::getMapVisibleWidthPixels() const
{
    return (int32)getMapVisibleWidth() << 4;
}

int32 cFodder::getMapVisibleHeightPixels() const
{
    return (int32)getMapVisibleHeight() << 4;
}

void cFodder::DataNotFound()
{
    g_Debugger->Error("No game data could be found, including the demos, have you installed the data pack?");

    g_Debugger->Error("We are looking for the 'Data' directory in: ");
    for (auto path : g_ResourceMan->getAllPaths())
    {
        g_Debugger->Error(path);
    }

    g_Debugger->Error("Press enter to quit");
    std::cin.get();
    exit(1);
}

void cFodder::Prepare(std::shared_ptr<sFodderParameters> pParams)
{
    mParams = std::make_shared<sFodderParameters>(*pParams);
    mStartParams = std::make_shared<sFodderParameters>(*pParams);

    g_ResourceMan->refresh();

    if (!g_ResourceMan->isDataAvailable())
        DataNotFound();

    mWindow->InitWindow("Open Fodder");
    mWindow->SetWindowSize((int)mParams->mWindowScale);

    mTile_BaseBlk = tSharedBuffer();
    mTile_SubBlk = tSharedBuffer();

    mMap = tSharedBuffer();

    mSidebar_Buffer_Size = 0x30 * getCameraHeight();

    mSidebar_Back_Buffer = (uint16 *)new uint8[mSidebar_Buffer_Size];
    mSidebar_Screen_Buffer = (uint16 *)new uint8[mSidebar_Buffer_Size];
    mSidebar_Screen_BufferPtr = mSidebar_Screen_Buffer;

    memset(mSidebar_Back_Buffer, 0, mSidebar_Buffer_Size);
    memset(mSidebar_Screen_Buffer, 0, mSidebar_Buffer_Size);

    mSurface = new cSurface(getSurfaceSize());
    mSurface2 = new cSurface(getSurfaceSize());
    mSurfaceFinal = new cSurface(getSurfaceSize());
    mSurfaceRecruit = new cSurface(getSurfaceSize());

    Sprite_Clear_All();

    g_ScriptingEngine = std::make_shared<cScriptingEngine>();
}

void cFodder::Mission_Set_Final_TimeRemaining()
{
    mMission_Final_TimeRemain = 0x64;
    mMission_Final_TimeToDie_Ticker = 0x28;
}

void cFodder::Mission_Final_Timer()
{

    if (mVersionCurrent->isRetail() && mVersionCurrent->isCannonFodder1())
    {

        if (!(mGame_Data.mMission_Number == 24 && mGame_Data.mMission_Phase == 6))
            return;

        if (mMission_Final_TimeRemain)
            return;

        if (!mMission_Final_TimeToAbort)
            mMission_Final_TimeToAbort = 0x28;

        --mMission_Final_TimeToAbort;
        if (!mMission_Final_TimeToAbort)
            mPhase_Aborted = true;

        if (!(mMission_EngineTicks & 3))
            Sprite_Create_RandomExplosion();
    }
}

void cFodder::Draw_Phase_Paused()
{
    mSurface2->clearBuffer();

    mGraphics->PaletteSet(mSurface2);
    mSurface2->surfaceSetToPaletteNew();

    // Dim the current surface
    mSurface->paletteNew_SetToBlack();

    // Draw to the secondary surface
    {
        mGraphics->SetImage(mSurface2);

        mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
        mString_GapCharID = 0x25;

        String_CalculateWidth(320 + SIDEBAR_WIDTH, mFont_Underlined_Width, "GAME PAUSED");
        String_Print(mFont_Underlined_Width, 1, mGUI_Temp_X, 0x54, "GAME PAUSED");

        mSurface2->draw();
        mString_GapCharID = 0;
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        mGraphics->SetImageOriginal();
    }
}

void cFodder::Phase_GameOver()
{
    sSprite *Data24 = &mSprites[40];
    Sprite_Destroy(Data24);

    sSprite *Data2C = &mSprites[41];
    Phase_TextSprite_Create_GameOver(Data2C);
}

void cFodder::Phase_TextSprite_Create_GameOver(sSprite *pData2C)
{
    Phase_TextSprite_Prepare(pData2C);
    pData2C->mSheetIndex = 0xC1;
    pData2C->mSpriteType = eSprite_Text_GameOver;

    if (!mStartParams->mDisableSound)
        Music_Play(8);
}

void cFodder::Sprites_Draw(cSurface *pSurface)
{

    std::lock_guard<std::mutex> lock(mSpriteMtx);

    for (auto &Sprite : mSprite_DrawList_Final)
    {

        if (Sprite->mPendingDestroy)
        {
            Sprite->mPendingDestroy = 0;
            Sprite->mPosX = -32768;
            mSprite_SpareUsed = 0;
            mSprite_SpareUsed2 = 0;
        }
        else
        {
            int16 Data0 = Sprite->mSheetIndex;
            int16 Data4 = Sprite->mFrameIndex;
            Sprite_Draw_Frame(Sprite, Data0, Data4, pSurface);
        }
    }
}

void cFodder::Sound_Tick()
{
    int16 *a4 = mSound_Timer;

    for (int16 d3 = 3; d3 >= 0; --d3)
    {
        if (a4[d3] != 0)
        {
            a4[d3]--;
        }
    }
}
void cFodder::Sound_Play(sSprite *pSprite, int16 pSoundEffect, int16 pPriority)
{

    if (mSoundDisabled)
        return;

    static uint16 word_81DF6 = 0;

    uint16_t d0 = word_81DF6 + 1;
    d0 = (d0 & 3) | 2;
    word_81DF6 = d0;

    int16 *a4 = mSound_Timer;
    int16 *a6 = mSound_Priority;

    if (a6[d0] <= pPriority)
    {
        a6[d0] = pPriority;
    }
    else
    {
        if (a4[d0] != 0)
        {
            return;
        }
        a6[d0] = pPriority;
    }
    a4[d0] = 0x0C;

    // loc_14BD4
    int16 Data8 = mCameraX >> 16;
    Data8 += getCameraWidth() / 2;

    if (pSprite != INVALID_SPRITE_PTR)
        Data8 -= pSprite->mPosX;

    int16 DataC = mCameraY >> 16;
    DataC += getCameraHeight() / 2;

    if (pSprite != INVALID_SPRITE_PTR)
        DataC -= pSprite->mPosY;

    int16 X = 0;
    int16 Y = 0;

    Map_Distance_Within_DoubleWindow(X, Y, Data8, DataC);

    int Volume = 40;
    Volume -= (X / 16);

    if (Volume <= 0)
        return;

    if (!mStartParams->mDisableSound)
        mSound->Sound_Play(mMapLoaded->getTileType(), pSoundEffect, Volume, d0);
}

/**
 * Display a list of custom maps
 */
void cFodder::Custom_ShowMapSelection()
{
    // If demo data is loaded, we need to enture a retail release is loaded for the menu draw data
    if (mVersionCurrent->isDemo())
        VersionSwitch(mVersionDefault);

    Image_FadeOut();
    mGraphics->PaletteSet();

    std::string File = mParams->mSingleMap;

    if (!File.size())
    {
        auto Maps = g_ResourceMan->GetMaps();
        File = GUI_Select_File("SELECT MAP", {}, Maps);
        // Exit Pressed?
        if (mGUI_SaveLoadAction == 1 || !File.size())
        {

            // Return to custom menu
            mDemo_ExitMenu = 1;
            mCustom_Mode = eCustomMode_None;

            return;
        }
    }

    mGame_Data.mCampaign.LoadCustomMapFromPath(g_ResourceMan->GetMapPath(File));

    mGame_Data.mMission_Phases_Remaining = 1;
    mGame_Data.mMission_Number = 0;
    mGame_Data.mMission_Phase = 0;
    mGame_Data.Phase_Next();

    mDemo_ExitMenu = 1;
    mCustom_ExitMenu = 1;
    mCustom_Mode = eCustomMode_Map;
}

bool cFodder::Demo_Amiga_ShowMenu()
{
    mSound->Music_Stop();

    if (mVersionCurrent->isAmigaNotVeryFestive() || mVersionCurrent->isAmigaAlienLevels())
        mGraphics->Load_And_Draw_Image("VMENU", 32);
    else
        mGraphics->Load_And_Draw_Image("apmenu.lbm", 32);

    // Amiga Demos have a different cursor palette
    GetGraphics<cGraphics_Amiga>()->SetCursorPalette(0x10);
    mWindow->SetScreenSize(mVersionCurrent->GetSecondScreenSize());

    mGUI_Mouse_Modifier_Y = 4;

    Menu_Loop(
        [this]()
        {
            if (mButtonPressLeft)
            {
                if (mVersionCurrent->isAmigaXmas())
                    GUI_Element_Mouse_Over(mAfx_Buttons);

                if (mVersionCurrent->isAmigaPower())
                    GUI_Element_Mouse_Over(mPlus_Buttons);

                if (mVersionCurrent->isAmigaAction() || mVersionCurrent->isAmigaNotVeryFestive() || mVersionCurrent->isAmigaAlienLevels())
                    GUI_Element_Mouse_Over(mAmigaAction_Buttons);
            }
        });

    GetGraphics<cGraphics_Amiga>()->SetCursorPalette(0xE0);
    mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());
    return mPhase_Aborted;
}

int16 cFodder::Direction_Between_Points(int16 &pData0, int16 &pData4, int16 &pData8, int16 &pDataC)
{
    int16 Data10 = 0, Data14 = 0;

    pData0 -= pData8;
    pData4 -= pDataC;
    if (pData0 < 0)
        Data10 = 1;

    Data10 = -Data10;
    if (pData0 < 0)
        pData0 = -pData0;

    if (pData4 < 0)
        Data14 = 1;

    Data14 = -Data14;
    if (pData4 < 0)
        pData4 = -pData4;

    pData8 = 9;
    do
    {
        int32 eax = 1 << pData8;

        if (pData0 & eax)
            break;

        if (pData4 & eax)
            break;

    } while (--pData8 >= 0);

    pData8 -= 4;
    if (pData8 >= 0)
    {
        pData0 >>= pData8;
        pData4 >>= pData8;
    }

    pData4 <<= 5;
    pData4 |= pData0;
    // pData4 <<= 1;
    pData4 = mMap_DirectionsBetweenPoints[pData4];
    if (pData4 < 0)
        return pData4;

    pData4 <<= 1;

    if (Data10 < 0)
        goto loc_29FC2;

    if (Data14 < 0)
        goto loc_29FAD;

    pData0 = 0x80;
    pData0 -= pData4;
    pData0 &= 0x1FE;

    pData4 = pData0;
    pData0 = 0;
    return 0;

loc_29FAD:;
    pData4 += 0x80;
    pData4 &= 0x1FE;
    pData0 = 0;
    return 0;

loc_29FC2:;
    if (Data14 >= 0)
    {
        pData4 += 0x180;
        pData4 &= 0x1FE;
        pData0 = 0;
        return 0;
    }

    pData0 = 0x180;
    pData0 -= pData4;
    pData0 &= 0x1FE;
    pData4 = pData0;
    pData0 = 0;

    return 0;
}

int16 cFodder::map_GetRandomX()
{

    return tool_RandomGet(1, mMapLoaded->getWidth() - 1);
}

int16 cFodder::map_GetRandomY()
{

    return tool_RandomGet(1, mMapLoaded->getHeight() - 1);
}

uint16 cFodder::tool_RandomGet(size_t pMin, size_t pMax)
{
    uint16 Rand = mRandom.getu();
    uint16 Mod = (uint16)(pMax - pMin + 1);

    return (uint16)((Rand % Mod) + pMin);
}

int16 cFodder::tool_RandomGet()
{

    return mRandom.get();
}

void cFodder::Scale_Dimensions_Percent(int16 pColumns, int16 pRows, int16 *pData8, int16 *pDataC)
{
    int32 Columns = pColumns * *pData8;
    int32 Rows = pRows * *pData8;

    // Return
    *pDataC = (int16)(Rows / 0x64);
    *pData8 = (int16)(Columns / 0x64);
}

int16 cFodder::Mission_Troop_GetDeviatePotential(sMission_Troop *pData24)
{
    int16 Data0 = 0;

    if (pData24)
        Data0 = pData24->mRank;

    Data0 += 8;

    if (Data0 > 0x0F)
        Data0 = 0x0F;

    return mSprite_Bullet_UnitData[Data0].mDeviatePotential;
}

void cFodder::Game_Save_Wrapper()
{

    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);
    Game_Save();
}

void cFodder::Game_Save()
{
    mInput.clear();
    mGUI_Select_File_String_Input_Callback = 0;
    mSurface->clearBuffer();

    GUI_Element_Reset();

    GUI_Render_Text_Centred("TYPE A SAVE NAME IN", 0x32);

    GUI_Button_Draw("EXIT", 0xA0);
    GUI_Button_Setup(&cFodder::GUI_Button_Load_Exit);

    mGUI_Select_File_String_Input_Callback = &cFodder::String_Input_Print;
    GUI_Select_File_Loop(true);
    mGUI_Select_File_String_Input_Callback = 0;

    if (mGUI_SaveLoadAction != 2)
    {
        mGUI_SaveLoadAction = 1;
        return;
    }

    {
        std::string Filename = g_ResourceMan->GetSaveNewName();
        std::ofstream outfile(Filename, std::ofstream::binary);
        outfile << mGame_Data.ToJson(mInput);
        outfile.close();
    }

    g_ResourceMan->refresh();
    mMouse_Exit_Loop = false;
}

void cFodder::Game_Load()
{
    auto Files = g_ResourceMan->GetSaves();
    std::vector<sSavedGame> SaveFiles;

    SaveFiles = Game_Load_Filter(Files);
    Files.clear();

    const std::string File = GUI_Select_File("SELECT SAVED GAME", SaveFiles, Files);
    if (!File.size())
        return;

    auto SaveData = g_ResourceMan->FileReadStr(g_ResourceMan->GetSave(File));

    // Load the game data from the JSONstd::string((char*)SaveData->data(), SaveData->size()))
    if (!mGame_Data.FromJson(SaveData))
    {
        return;
    }

    // If the game was saved on a different platform, lets look for it and attempt to switch
    if (mGame_Data.mSavedVersion.mPlatform != mVersionCurrent->mPlatform)
    {

        VersionSwitch(mVersions->GetForCampaign(mGame_Data.mCampaignName, mGame_Data.mSavedVersion.mPlatform));
    }

    mMouse_Exit_Loop = false;

    for (int16 x = 0; x < 8; ++x)
        mMission_Troops_SpritePtrs[x] = INVALID_SPRITE_PTR;

    for (auto &Troop : mGame_Data.mSoldiers_Allocated)
        Troop.mSprite = INVALID_SPRITE_PTR;

    GameData_Backup();
    GameData_Restore();
}

std::vector<sSavedGame> cFodder::Game_Load_Filter(const std::vector<std::string> &pFiles)
{
    std::vector<sSavedGame> Results;

    for (auto &CurrentFile : pFiles)
    {

        auto SaveData = g_ResourceMan->FileRead(g_ResourceMan->GetSave(CurrentFile));

        // Verify the savegame is for the current campaign
        try
        {
            sGameData NewData(std::string((char *)SaveData->data(), SaveData->size()));

            // Ensure for this campaign
            if (NewData.mCampaignName != mGame_Data.mCampaign.getName())
                continue;

            // Ensure the game is the same
            if (NewData.mSavedVersion.mGame != mVersionCurrent->mGame)
                continue;

            Results.push_back({CurrentFile, NewData.mSavedName});
        }
        catch (...)
        {
            continue;
        }
    }

    // Sort newest first
    std::sort(Results.begin(), Results.end(), [](sSavedGame &pLeft, sSavedGame &pRight)
              { return std::atoll(pLeft.mFileName.c_str()) > std::atoll(pRight.mFileName.c_str()); });

    return Results;
}

void cFodder::Menu_Button_Reset()
{
    mCustom_ExitMenu = 0;
    mPhase_Aborted = false;
    mDemo_ExitMenu = -1;
}

/**
 * Caller of Menu_Loop is responsible for cleaning up button states
 */
void cFodder::Menu_Loop(const std::function<void()> pButtonHandler)
{

    Menu_Button_Reset();

    mSurface->Save();
    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();

    mInterruptCallback = [this]()
    {
        Mouse_DrawCursor();
    };

    for (;;)
    {

        Video_Sleep();

        if (Menu_Draw(pButtonHandler))
            break;
    }

    mInterruptCallback = nullptr;
    Image_FadeOut();
    mSurface->clearBuffer();
}

bool cFodder::Menu_Draw(const std::function<void()> pButtonHandler)
{

    // Button Pressed?
    if (mButtonPressLeft)
        pButtonHandler();

    // Exit Time?
    if (mDemo_ExitMenu > 0 || mPhase_Aborted || mCustom_ExitMenu)
        return true;

    if (mSurface->isPaletteAdjusting())
        mSurface->palette_FadeTowardNew();

    return false;
}

void cFodder::Demo_Quiz_ShowScreen(const char *pFilename)
{

    Image_FadeOut();

    mGraphics->Load_And_Draw_Image(pFilename, 32);
    mGraphics->PaletteSet();

    mSurface->Save();
    mSurface->palette_FadeTowardNew();

    for (;;)
    {
        Video_Sleep();

        if (mButtonPressLeft || mPhase_Aborted)
            break;

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();
    }

    Image_FadeOut();

    mGraphics->Load_And_Draw_Image("1.lbm", 32);
    mGraphics->PaletteSet();

    mSurface->Save();
    mSurface->palette_FadeTowardNew();

    Menu_Button_Reset();
}

void cFodder::Demo_Quiz()
{
    Image_FadeOut();

    mGraphics->Load_And_Draw_Image("1.lbm", 32);
    Mouse_Setup();

    Menu_Loop(
        [this]()
        {
            if (mButtonPressLeft)
                GUI_Element_Mouse_Over(mPlusQuiz_Buttons);
        });

    mGraphics->Load_And_Draw_Image("apmenu.lbm", 32);
    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();
    mSurface->Save();

    Menu_Button_Reset();
}

void cFodder::Mission_Set_Initial_Weapon()
{

    // segra: I don't believe this condition can ever occur
    if (mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] == eWeapon_Grenade)
    {

        if (mSquad_Grenades[mGUI_Loop_Squad_Current])
            return;
        if (!mSquad_Rockets[mGUI_Loop_Squad_Current])
            return;

        mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] = eWeapon_Rocket;
        return;
    }

    // loc_2EFF1
    if (mSquad_Rockets[mGUI_Loop_Squad_Current])
        return;

    if (!mSquad_Grenades[mGUI_Loop_Squad_Current])
        return;

    mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] = eWeapon_Grenade;
}

void cFodder::SetActiveSpriteSheetPtr(const sSpriteSheet **pSpriteSheet)
{

    mSprite_SheetPtr = pSpriteSheet;
}

void cFodder::Video_SurfaceRender(const bool pRestoreSurface, const bool pShrink, cSurface *pSurface, const bool pSkip)
{

    if (mStartParams->mDisableVideo)
        return;

    if (!pSurface)
    {
        pSurface = mSurface;
    }

    pSurface->draw(pSkip == true ? 16 : 0, pSkip == true ? 16 : 0);
    if (pShrink)
    {
        mWindow->RenderShrunk(pSurface);
    }
    else
    {
        mWindow->RenderAt(pSurface);
    }
    mWindow->FrameEnd();

    // if(pRestoreSurface)
    //     mSurfaceFinal->Restore();
}

void cFodder::sleepLoop(int64 pMilliseconds)
{
#ifdef EMSCRIPTEN
    return;
#endif
    uint64 TimeStarted = SDL_GetTicks();
    uint64 TimeFinish = TimeStarted + pMilliseconds;

    do
    {
        mWindow->EventCheck();

        if (SDL_GetTicks() >= TimeFinish)
            break;

        SDL_Delay(1);

    } while (1);
}

void cFodder::WonGame()
{

    if (mParams->mSinglePhase)
        return;

    mMouseX = -1;
    mMouseY = -1;

    mWindow->SetScreenSize(mVersionCurrent->GetSecondScreenSize());

    if (mVersionCurrent->isAmigaTheOne())
    {
        // mGraphics->Load_And_Draw_Image("PRETENTIOUS2", 0x100);
        mGraphics->Load_And_Draw_Image("PRETENTIOUS3", 0x100);
    }
    else
    {
        Music_Play(17);
        mGraphics->Load_And_Draw_Image("WON", 0x100);
    }

    Image_FadeIn();
    mMouse_Exit_Loop = false;

    for (int count = 10000; count >= 0; --count)
    {
        Video_Sleep();

        if (mPhase_Aborted || mMouse_Exit_Loop)
            break;
    }

    Image_FadeOut();

    mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());
}

void cFodder::Game_Setup()
{

    if (mParams->mMissionNumber < 1)
        mParams->mMissionNumber = 1;

    GameData_Reset();

    mIntroDone = false;
    mPhase_Complete = false;

    mGame_Data.mMission_Phases_Remaining = 1;
    mGame_Data.mMission_Number = (uint16)(mParams->mMissionNumber);
    mGame_Data.mMission_Phase = (uint16)(mParams->mPhaseNumber ? (mParams->mPhaseNumber) : 1);

    if (!mGame_Data.Phase_Start())
    {
        g_Debugger->Error("Invalid Mission!");
        exit(1);
    }

    mPhase_TryAgain = true;
    mGraphics->Load_pStuff();
}

// This function is for viewing/iterating sprites
void cFodder::Playground()
{
    // return;
    if (!Campaign_Load(mParams->mCampaignName))
    {
        g_Debugger->Error("Invalid Campaign!");
        exit(1);
    }
    mGame_Data.Phase_Start();
    Map_Load();

    mGraphics->PaletteSet();

    Recruit_Truck_Anim_Prepare();
    mGraphics->Load_Hill_Data();
    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);
    // mGraphics->Recruit_Draw_Hill();

    // I think this functionality is useless anyway because theres no way to change between sprite sets at runtime
    if (!mVersionCurrent->isDemo())
    {
        Recruit_Copy_Sprites();

        if (mVersionCurrent->mPlatform == ePlatform::Amiga)
        {

            GetGraphics<cGraphics_Amiga>()->Hill_Prepare_Overlays();
        }
    }
    // Load Icon
    mSurface->clearBuffer();
    mSurface->palette_FadeTowardNew();
    mSurface->Save();
    mString_GapCharID = 0x25;
    mGUI_Print_String_To_Sidebar = false;

    int32 SpriteID = 0x62;
    int32 Frame = 0;

    for (;;)
    {
        mKeyCode = 0;
        {
            mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
            std::stringstream SPRITE_STRING;
            SPRITE_STRING << "SPRITE: 0x" << std::hex << SpriteID;
            SPRITE_STRING << " FRAME: 0x" << std::hex << Frame;

            String_Print_Large(SPRITE_STRING.str(), false, 0);
        }

        {
            mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
            GUI_Draw_Frame_8(SpriteID, Frame, 65, 65);
        }

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        // Mouse_ReadInputs();
        // Mouse_DrawCursor();

        Video_Sleep();

        // Q
        if (mKeyCode == 0x14)
        {
            --SpriteID;
        }
        // W
        if (mKeyCode == 0x1A)
        {
            ++SpriteID;
        }

        // A
        if (mKeyCode == 0x04)
        {
            --Frame;
        }

        // S
        if (mKeyCode == 0x16)
        {
            ++Frame;
        }

        // Safety
        if (SpriteID >= 232)
            SpriteID = 0;

        if (Frame >= 20)
            Frame = 0;

        if (mDemo_ExitMenu)
            break;
    }
}

bool cFodder::Demo_Load()
{

    std::ifstream DemoContent(mStartParams->mDemoFile, std::ios::binary);
    if (DemoContent.is_open())
    {

        std::string SaveGameContent(
            (std::istreambuf_iterator<char>(DemoContent)),
            (std::istreambuf_iterator<char>()));

        mGame_Data.mDemoRecorded.FromJson(SaveGameContent);
        return true;
    }
    return false;
}

void cFodder::Window_UpdateScreenSize()
{

    // We can override the window
    mWindow->SetScreenSize(getWindowSize());
    mWindow->SetOriginalRes(getWindowSize());

    // This next section is done
    //
    // If we're playing back a demo
    if (mParams->mDemoPlayback && g_Fodder->mVersionCurrent)
    {
        // And the current platform, does not match the platform the demo was recorded with
        if (mGame_Data.mDemoRecorded.mRecordedPlatform != mVersionCurrent->mPlatform && mGame_Data.mDemoRecorded.mRecordedPlatform != ePlatform::Any)
        {
            // Alter the screen size, to the other platforms
            switch (mGame_Data.mDemoRecorded.mRecordedPlatform)
            {
            case ePlatform::Amiga: // Viewing Amiga recording on PC Data
                mWindow->SetScreenSize({320, 225});
                break;
            case ePlatform::PC: // Viewing PC recording on Amiga Data
                mWindow->SetScreenSize({320, 200});
                break;
            case ePlatform::Any:
                break;
            }
        }
    }
}

void cFodder::About()
{

    mService_Draw_List.clear();
    VersionSwitch(mVersions->GetRetail(mParams->mDefaultPlatform, mParams->mDefaultGame));
    if (!mVersionCurrent)
        VersionSwitch(mVersions->GetDemo());

    cAbout About;
    while (About.Cycle())
    {
    }

    g_Fodder->mPhase_Aborted = false;
}

void cFodder::CreateRandom(sMapParams pParams)
{
    mSurface->clearBuffer();

    mGame_Data.mCampaign.CreateCustomCampaign();
    mGame_Data.mCampaign.setRandom(true);
    mGame_Data.mCampaign.setName("Random");

    VersionSwitch(mVersions->GetRetail(mParams->mDefaultPlatform, mParams->mDefaultGame));

    if (!mParams->mRandomFilename.size())
    {
        mParams->mRandomFilename = "random";
    }

    Map_Create(pParams);

    if (mParams->mScriptRun.size() == 0)
        mParams->mScriptRun = "test.js";

    mGame_Data.mMission_Phases_Remaining = 1;
    mGame_Data.mMission_Number = 1;
    mGame_Data.mMission_Phase = 1;
    mGame_Data.Phase_Start();

    mGame_Data.mPhase_Current->SetMapFilename(mParams->mRandomFilename);

    // Fade in so text can be drawn
    Image_FadeIn();

    if (g_Fodder->mStartParams->mDebugger)
        g_ScriptingEngine->debuggerEnable();

    if (g_ScriptingEngine->Run(mParams->mScriptRun))
    {

        // Ensure final phase is saved
        mMapLoaded->save(mGame_Data.mPhase_Current->GetMapFilename(), true);
    }

    // Fade out again
    Image_FadeOut();

    Map_Load_Sprites();

    // Back to mission 1 phase 1
    mGame_Data.mMission_Number = 1;
    mGame_Data.mMission_Phase = 1;
    mGame_Data.Phase_Start();
}

void cFodder::Start()
{

    if (mParams->mShowAbout)
    {
        About();
        return;
    }

    if (mParams->mDemoPlayback)
    {
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

    if (!mVersionCurrent)
    {
        VersionSwitch(mVersions->GetDemo());

        // This should never happen ,as a check in Prepare ensures atleast 1 ver is available
        if (!mVersionCurrent)
            return;
    }

    if (mParams->mDemoRecord && mGame_Data.mDemoRecorded.mRecordedPlatform == ePlatform::Any)
        mGame_Data.mDemoRecorded.mRecordedPlatform = mVersionCurrent->mPlatform;

    mGame_Data.mDemoRecorded.save();

    // Play the intro
    Intro_OpenFodder();

    if (mParams->mPlayground)
    {
        Playground();
        return;
    }

    // Start a random map?
    if (mParams->mRandom || mParams->mSingleMap.size())
    {

        mGame_Data.mCampaign.SetSingleMapCampaign();
        mCustom_Mode = eCustomMode_Map;

        if (mParams->mRandom)
            VersionSwitch(mVersions->GetForCampaign("Random Map", mParams->mDefaultPlatform));
        else
            VersionSwitch(mVersions->GetForCampaign("Single Map", mParams->mDefaultPlatform));
    }
    else
    {

        // Select campaign menu
        if (!(mParams->mCampaignName.size() && Campaign_Load(mParams->mCampaignName)))
        {
            Campaign_Selection();
            // Exit pushed?
            if (mGUI_SaveLoadAction == 1)
                return;

            if (mGUI_SaveLoadAction == 4)
            {
                About();
                goto Start;
            }
        }
    }

    mVersionDefault = mVersionCurrent;
    Mouse_Setup();
    Mouse_ReadInputs();

    if (Engine_Loop())
        goto Start;
}

/**
 * Main Engine Loop
 *
 * false = Exit
 * true  = Restart
 */
bool cFodder::Engine_Loop()
{

    for (;;)
    {

        Game_Setup();

        if (Mission_Loop() == -1)
            return true;

        if (mParams->mSinglePhase)
            break;
    }

    return false;
}

bool cFodder::GameOverCheck()
{

    if (!mParams->mUnitTesting)
    {
        // Mission completed?
        if (!mPhase_Aborted && !mPhase_TryAgain)
        {

            // Demo / Custom Mission restart
            if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set && !mVersionCurrent->isAmigaTheOne())
                return false;

            // Reached last map in this mission set?
            if (!mGame_Data.Phase_Next() && !mVersionDefault->isRandom() && mCustom_Mode != eCustomMode_Map)
            {

                mGame_Data.mGameWon = true;
                WonGame();
                return true;
            }

            if (mVersionDefault->isRandom())
            {
                mGame_Data.mMission_Recruitment = -1;
            }
        }
    }

    // Double escape aborts out to OF selection, on Amiga the one
    if (mPhase_Aborted2 && mVersionCurrent->isAmigaTheOne())
    {
        return true;
    }

    return false;
}

int16 cFodder::Mission_Loop()
{

    for (;;)
    {
        mGame_Data.mDemoRecorded.save();

        if (GameOverCheck())
            return -1;

        // Prepare the next mission
        Phase_EngineReset();
        Phase_SquadPrepare();

        mInput_Enabled = false;

        Intro_Main();

        // Single / Random Map mode
        if (mCustom_Mode == eCustomMode_Map)
        {
            if (mVersionDefault->mName == "Random Map")
            {
                sMapParams Params(mRandom.get());
                CreateRandom(Params);
                mGame_Data.mMission_Recruitment = 0;
            }
            else
            {
                Custom_ShowMapSelection();
            }

            if (mCustom_Mode == eCustomMode_None)
                return -1;
        }
        else
        {
            // Prepare a new game?
            if (mGame_Data.mMission_Recruitment && !mParams->mSkipRecruit)
            {
                mGame_Data.mMission_Recruitment = 0;

                switch (Recruit_Show())
                {
                case 0: // Start Mission
                    break;

                case -1: // Return to version select
                    return -1;

                case -2: // Custom set mode
                    return -2;

                case -3: // Load/Save pressed
                    continue;
                }
            }
        }

        mMusic_SlowVolumeDecrease = true;

        WindowTitleSet(true);

        switch (Briefing_Show())
        {

        case -1:
            return -1; // Return to version select

        case 0: // Back to hill
            continue;

        case 1: // Continue to phase
            break;
        }

        while (Music_Decrease_Channel_Volume())
        {
            sleepLoop(1);
        }

        if (mStartParams->mDemoPlayback || mStartParams->mDemoRecord)
        {
            std::lock_guard<std::mutex> lock(g_Fodder->mSurfaceMtx);
            mVideo_Done = true;
        }

        Phase_Prepare();
        mMusic_SlowVolumeDecrease = false;

        if (!Phase_Loop())
        {
            mKeyCode = 0;
            mPhase_In_Progress = false;
            Squad_Member_PhaseCount();
            mPhase_TryingAgain = true;
        }
        else
        {
            mKeyCode = 0;
            mPhase_In_Progress = false;

            // Game over?
            if (!mGame_Data.mRecruits_Available_Count)
            {

                // Retail/Custom Set can do the service screen
                if (mVersionCurrent->isRetail() || mCustom_Mode == eCustomMode_Set)
                {

                    if (!mPhase_Aborted)
                        Service_Show();
                }
                break;
            }
        }

        // Single Phase?
        if (mParams->mSinglePhase)
        {
            return 0;
        }

        // loc_106F1
        if (mPhase_TryAgain)
        {
            mPhase_TryingAgain = true;
            continue;
        }

        // Switch back to the default data?
        if (mVersionReturnAfterPhase)
        {
            mVersionReturnAfterPhase = false;
            VersionSwitch(mVersionDefault);
        }

        // Demo/Single mission, but not set mode (its handled at the end of the loop)
        if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set)
        {

            // Custom can do the service screen, as it requires a retail release
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

void cFodder::Exit(unsigned int pExitCode)
{

    exit(pExitCode);
}

void cFodder::Mission_Troops_Clear_Selected()
{

    for (auto &Troop : mGame_Data.mSoldiers_Allocated)
        Troop.mSelected &= 0;
}
