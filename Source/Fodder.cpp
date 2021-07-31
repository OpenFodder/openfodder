/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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
#include "Map/Random.hpp"

#ifdef WIN32
#include <windows.h>
#endif
     
const int32 CAMERA_PAN_TO_START_ACCELARATION = 0x80000;     // Dos: Original 0x80000
const int32 CAMERA_PAN_TO_SQUAD_ACCELERATION = 0x20000;     // Dos: Original 0x20000
const int32 CAMERA_TOWARD_SQUAD_SPEED = 0x14;               // Dos: Original 0x14

const int16 MOUSE_POSITION_X_ADJUST = -32;
const int16 MOUSE_POSITION_Y_ADJUST = 4;

const int16 SIDEBAR_WIDTH = 48;

const int16 mBriefing_Helicopter_Offsets[] =
{
    0x0180, 0x0040, 0x0004, 0x01A0,
    0x0040, 0x000F, 0x01C0, 0x0040,
    0x0007, 0x01E0, 0x0020, 0x0007,
    0x0000, 0x0010, 0x000A, 0x0080,
    0x0010, 0x001E, 0x0100, 0x0010,
    0x000A, 0x01A0, 0x0008, 0x0019,
    0x0170, 0x0064, 0x000A, -1,
    -1, -1
};

cFodder::cFodder(std::shared_ptr<cWindow> pWindow) {

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
    mBriefing_Render_1_Mode = 0;

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

    mDebug_PhaseSkip = 0;

    mGame_InputTicks = 0;
    mKeyControlPressed = 0;
    mVersionReturnAfterPhase = false;

    mInput_LastKey = 0;
    mCustom_ExitMenu = 0;

    word_428D8 = 0;
    word_3A05F = 0;
    byte_44AC0 = 0;
    mSoundDisabled = false;
    Squad_Walk_Target_SetAll(0);
    mPhase_Completed_Timer = 0;
    mVideo_Draw_ColumnsMax = 0;
    word_428B6 = 0;
    word_428B8 = 0;
    word_428BA = 0;
    mHelicopterPosX = 0;
    mHelicopterPosY = 0;
    mBriefing_Helicopter_Off1 = 0;
    mBriefing_Helicopter_Off2 = 0;
    mBriefing_Helicopter_Off3 = 0;
    mBriefing_Helicopter_Off4 = 0;
    mBriefing_ParaHeli_Frame = 0;
    mBriefing_Helicopter_Moving = 0;
    mTroop_InRange_Callpad = 0;

    for (unsigned int x = 0; x < 0x18; ++x) {
        mMission_Save_Blocked[x] = 0;
    }

    mService_Promotion_Exit_Loop = 0;

    //      Field_0,                 X, Width, Y, Height, mMouseInsideFuncPtr
    mSidebar_OverlayButtons[0] = {
        &cFodder::GUI_Button_NoAction,
        0,      // X
        47,     // Width
        0,      // Y
        225,    // Height
        &cFodder::GUI_Button_NoAction2 };

    mSidebar_OverlayButtons[1] = { 0, 0, 0, 0, 0, 0 };

    mGUI_Handle_Button_SelectSquad_Array[0] = &cFodder::GUI_Handle_Button_SelectSquad_0;
    mGUI_Handle_Button_SelectSquad_Array[1] = &cFodder::GUI_Handle_Button_SelectSquad_1;
    mGUI_Handle_Button_SelectSquad_Array[2] = &cFodder::GUI_Handle_Button_SelectSquad_2;

    mDemo_ExitMenu = 0;

    mSurfaceMapTop = 0;
    mSurfaceMapLeft = 0;

    mSprite_SheetPtr = 0;

    Sprite_Clear_All();

    Phase_EngineReset();

    Sprite_Table_Setup();

}

cFodder::~cFodder() {

    delete mSurfaceMapOverview;
}

void cFodder::Squad_Walk_Target_SetAll(int16 pValue) {

    for (unsigned int x = 0; x < 10; ++x) {

        for (unsigned int y = 0; y < 30; ++y) {

            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
            mSquad_WalkTargets[x][y].asInt = pValue;
        }
    }

}

/**
 * Execute one cycle of the current phase
 *
 * -1 = Phase Try Again 
 *  0 = Phase Won
 *  1 = Phase Running
 */
int16 cFodder::Phase_Cycle() {

	// If demo playback is enabled, and a record resume cycle is set
	if (mStartParams->mDemoPlayback && mStartParams->mDemoRecordResumeCycle) {
		// See if we hit the tick count
		if (mGame_Data.mDemoRecorded.mTick >= mStartParams->mDemoRecordResumeCycle) {
			// Then resume recording
			mStartParams->mDemoPlayback = false;
			mStartParams->mDemoRecord = true;
			mStartParams->mDemoRecordResumeCycle = 0;
			mParams->mSleepDelta = 2;
			mParams->mDemoRecord = mStartParams->mDemoRecord;
			mParams->mDemoPlayback = mStartParams->mDemoPlayback;
			mStartParams->mDisableVideo = false;
			mStartParams->mDisableSound = false;
			Mouse_Setup();
		}
	}

	MapTile_UpdateFromCamera();
	MapTile_Update_Position();

	Game_Handle();
	++mMission_EngineTicks;

	if (mCamera_Start_Adjust) {
		Camera_SetTargetToStartPosition();
		mCamera_Start_Adjust = false;
		return 1;
	}

	//loc_1074E
	if (mGUI_Sidebar_Setup >= 0 && !mPhase_TryAgain)
		GUI_Sidebar_Setup();
	else {
		GUI_Sidebar_Draw();
	}

	//loc_10768
	Phase_Progress_Check();
	mHelicopterCallPadPressedCount = 0;
	if (word_3A9B8 >= 0)
		--word_3A9B8;

	Sprite_Find_HumanVehicles();

	// Cheat
	if (mDebug_PhaseSkip == -1) {
		mDebug_PhaseSkip = 0;
		mPhase_Complete = true;
	}
	else
		Phase_Goals_Check();

	//loc_1079C
	Squad_Walk_Steps_Decrease();
	Squad_Troops_Count();

	Mission_Sprites_Handle();
	Squad_Switch_Timer();
	if (!mStartParams->mDisableVideo)
		mGraphics->Sidebar_Copy_To_Surface();

	// Game Paused
	if (mPhase_Paused) {
		Phase_Paused();

		mSurface->Save();

		// Fade the background out, and the 'mission paused' message in
		mSurface->palette_FadeTowardNew();
		mSurface->palette_FadeTowardNew();
		mSurface->palette_FadeTowardNew();

		while (mPhase_Paused) {

			// Update mouse
			Mouse_Inputs_Get();
			Mouse_DrawCursor();
			// Draw surface
			mSurface->draw();

			// Copy the rendered surface of the 'mission paused' message over the top of the main surface
			mSurface->mergeSurfaceBuffer(mSurface2);

			mWindow->RenderAt(mSurface);
			mWindow->FrameEnd();
			Cycle_End();

			mSurface->Restore();
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

	Mouse_DrawCursor();

	if (mSurface->isPaletteAdjusting())
		mSurface->palette_FadeTowardNew();

	Camera_Update_Mouse_Position_For_Pan();

	if (mPhase_ShowMapOverview && mSurfaceMapOverview) {

		// Dont show the map while recording
		if (!mParams->mDemoRecord)
			Phase_Map_Overview_Show();

		mPhase_ShowMapOverview = 0;
	}

	if (mGame_Data.mGamePhase_Data.mIsComplete) {

		if (mPhase_Aborted)
			Sprite_Handle_Player_Destroy_Unk();
		else {
			if (!mPhase_TryAgain)
				return 0;

			Sprite_Handle_Player_DestroyAll();
		}
		return -1;
	}

	//loc_10841
	Sprite_Bullet_SetData();
	Squad_EnteredVehicle_TimerTick();
	Squad_Set_CurrentVehicle();

	// No squad is selected, so set count down timer
	if (mSquad_Selected < 0 && !mSquad_Select_Timer)
		mSquad_Select_Timer = 0x14;

	Sprite_HelicopterCallPad_Check();
	Mission_Final_Timer();

	Video_SurfaceRender();
	return 1;
}

void cFodder::Phase_Prepare() {

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

	MapTiles_Draw();
	Camera_Reset();

	Mouse_Inputs_Get();
	Sprite_Frame_Modifier_Update();

	mSound->Stop();
	Sprite_Aggression_Set();

	//seg000:05D1

	Phase_Goals_Set();

	Sprite_Bullet_SetData();
	Sprite_Handle_Loop();
	Sprite_Create_Rank();

	mCamera_Start_Adjust = true;
	mCamera_StartPosition_X = mSprites[0].field_0;
	mCamera_StartPosition_Y = mSprites[0].field_4;

	// Is map 17 x 12
	{
		if (mMapLoaded->getWidth() == 17) {
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

	Squad_Prepare_GrenadesAndRockets();

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

int16 cFodder::Phase_Loop() {
	int16 result = 1;

	// -1 = Phase Try Again 
	//  0 = Phase Won
	//  1 = Phase Running
    for (result = 1; result == 1; result = Phase_Cycle()) {
		
		Cycle_End();
    }

    return result;
}

void cFodder::Game_Handle() {

    // Input disabled or Mission not paused?
    if (!mInput_Enabled || !mPhase_Paused) {

        ++mGame_InputTicks;

        Mouse_Inputs_Get();

        if (mInput_Enabled) {
            Camera_Handle();
            Camera_Handle();
            Camera_Handle();

            if (!mPhase_Finished)
                Mouse_Inputs_Check();
        }
    }

    //loc_108AE
    if (mSquad_SwitchWeapon) {
        --mSquad_SwitchWeapon;
        Squad_Switch_Weapon();
    }

    if (mMouseCursor_Enabled)
        Mouse_DrawCursor();

    if (!mInput_Enabled)
        return;

    if (!mPhase_In_Progress)
        return;

    if (mPhase_Completed_Timer || mPhase_Complete || mPhase_TryAgain || mPhase_Aborted) {
        mPhase_Finished = true;
        return;
    }

    if (!mSoundEffectToPlay)
        return;

    Sound_Play(mSquad_Leader, mSoundEffectToPlay, 0);
    mSoundEffectToPlay = 0;
}

void cFodder::Camera_Handle() {

    if (mPhase_In_Progress) {
        Camera_Speed_Reset();
        Camera_Speed_MaxSet();
        Camera_Speed_Calculate();
        Camera_TileSpeedX_Set();
        Camera_Speed_Update_From_PanTarget();
        Camera_PanTarget_AdjustToward_SquadLeader();
        Camera_Acceleration_Set();
    }
}

void cFodder::Camera_PanTarget_AdjustToward_SquadLeader() {
    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return;

    int16 SquadLeaderX = mSquad_Leader->field_0 + 0x18;
    int16 SquadLeaderY = mSquad_Leader->field_4;
    int16 Data18 = SquadLeaderX;
    int16 Data1C = SquadLeaderY;


    mCamera_Scroll_Speed = CAMERA_TOWARD_SQUAD_SPEED;

    // Mouse near sidebar?
    if (mMouseX <= 0x0F) {

        if (mCamera_Panning_ToTarget) {

            if ((SquadLeaderX - mCamera_SquadLeaderX) > 0 || 
                (SquadLeaderY - mCamera_SquadLeaderY) > 0) {
                mCamera_PanTargetX = SquadLeaderX;
                mCamera_PanTargetY = SquadLeaderY;
            }

            // Original Logic
            //mCamera_PanTargetX += (SquadLeaderX - mCamera_SquadLeaderX);
            //mCamera_PanTargetY += (SquadLeaderY - mCamera_SquadLeaderY);

            mCamera_SquadLeaderX = SquadLeaderX;
            mCamera_SquadLeaderY = SquadLeaderY;
        } else {
            //loc_10A11
            mCamera_Panning_ToTarget = true;

            mCamera_PanTargetX = (mCameraX >> 16) + (getCameraWidth() / 2) - 8;
            mCamera_PanTargetY = (mCameraY >> 16) + (getCameraHeight() - 8) / 2;
        }
        //loc_10A3A
        mMouse_Locked = false;
        return;
    }

    mCamera_Panning_ToTarget = false;
    mCamera_SquadLeaderX = SquadLeaderX;
    mCamera_SquadLeaderY = SquadLeaderY;
    int16 Data0 = mMouseX + (mCameraX >> 16);
    int16 Data4 = mMouseY + (mCameraY >> 16);

    int16 Data0_Saved = Data0;
    int16 Data4_Saved = Data4;
    int16 Data8_Saved = SquadLeaderX;
    int16 DataC_Saved = SquadLeaderY;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, SquadLeaderX, SquadLeaderY);

    if (mSquad_CurrentVehicle) {
        if (Data0 >= 0x64)
            Data0 = 0x64;
    } else {
        if (Data0 >= 0x8C)
            Data0 = 0x8C;
    }
    //loc_10AAA
    int16 word_3ABFD = Data0;

    SquadLeaderX = Data8_Saved;
    SquadLeaderY = DataC_Saved;
    Data4 = Data4_Saved;    // MouseY
    Data0 = Data0_Saved;    // MouseX

    if (Direction_Between_Points(Data0, Data4, SquadLeaderX, SquadLeaderY) < 0)
        return;

    int32 DirectionX = mMap_Direction_Calculations[Data4 / 2];
    Data4 += 0x80;
    Data4 &= 0X1FE;

    int32 DirectionY = mMap_Direction_Calculations[Data4 / 2];

    DirectionX = (DirectionX * word_3ABFD) >> 16;
    DirectionY = (DirectionY * word_3ABFD) >> 16;

    Data18 += DirectionX;
    Data1C += DirectionY;
    Data18 -= 0x18;

    if (Data18 < 0)
        Data18 = 0;

    mCamera_PanTargetX = Data18;
    mCamera_PanTargetY = Data1C;
    mMouse_Locked = false;
}

void cFodder::GameData_Reset() {
    mDebug_PhaseSkip = 0;
    mInput_Enabled = false;
    mGame_InputTicks = 0;
    mMission_EngineTicks = 0;
    mRecruit_Mission_Restarting = false;

    for (unsigned int x = 0; x < 9; ++x) {
        mMission_Troops_SpritePtrs[x] = 0;
    }

    mPhase_TryingAgain = false;
    mGame_Data.Clear();
    mGame_Data.mGamePhase_Data.Clear();
}

void cFodder::GameData_Backup() {
    mGame_Data_Backup = mGame_Data;
}

void cFodder::GameData_Restore() {
    mGame_Data = mGame_Data_Backup;

    // Reset grave pointers
    mGame_Data.mGamePhase_Data.mHeroesCount = mGame_Data.mHeroes.size();
}

void cFodder::Phase_EngineReset() {
    // Clear memory 2454 to 3B58
    mPhase_EscapeKeyAbort = false;
    mPhase_Aborted2 = false;
    mButtonPressLeft = 0;
    mButtonPressRight = 0;
    mMouse_Button_Left_Toggle = 0;
    mMouse_Button_Right_Toggle = 0;
    mMouse_Button_LeftRight_Toggle = false;
    mMouse_Button_LeftRight_Toggle2 = false;

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
    for (uint8 x = 0; x < 3; ++x) {
        mSquad_Walk_Target_Indexes[x] = 0;
        mSquad_Walk_Target_Steps[x] = 0;
    }

    mSprite_Bumped_Into_SquadMember = 0;
    mSprite_Player_CheckWeapon = 0;
    mPhase_Paused = false;
    for (uint8 x = 0; x < 4; ++x) {
        mSprite_Projectile_Counters[x] = 0;
        mSprite_Missile_Projectile_Counters[x] = 0;
    }

    mMouseX_Offset = 0;
    mMouseY_Offset = 0;
    mMouseSpriteNew = 0;

    mCamera_Speed_Max = 0;

    mMouseSetToCursor = 0;
    mSprites_Found_Count = 0;

    for (uint16 x = 0; x < 3; ++x) {
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

    for (uint16 x = 0; x < 42; ++x) {
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

    for (uint16 x = 0; x < 3; ++x) {
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

    for (uint16 x = 0; x < 16; ++x) {
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
}

void cFodder::Phase_SquadPrepare() {
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

void cFodder::Squad_Set_Squad_Leader() {

    mSquad_Leader = &mSprites[0];
}

void cFodder::Sprite_Clear_All() {
	mSprites.resize(mParams->mSpritesMax);
	Squad_Set_Squad_Leader();

    for (auto& Sprite : mSprites) {
        Sprite.Clear();
    }
	mSprite_Spare.Clear();

    mSprites[mSprites.size() - 1].field_0 = -1;
    mSprite_SpareUsed = 0;
}

int16 cFodder::Tile_FindType(eTerrainFeature pType) {

    for (int16 TileID = 0; TileID < sizeof(mTile_Hit) / sizeof(int16); ++TileID) {

        int16 TerrainType = mTile_Hit[TileID];

        // Single Type Tile
        if (TerrainType >= 0) {

            // Match?
            if ((TerrainType & 0x0F) == pType) {

                return TileID;
            }
        }
        else {

            if (TerrainType < 0) {
                //loc_2A8D9
                // X
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

std::vector<int16> cFodder::Tile_FindType(const eTerrainFeature pType, const eTerrainFeature pType2) {
    std::vector<int16> Results;

    for (int16 TileID = 0; TileID < sizeof(mTile_Hit) / sizeof(int16); ++TileID) {

        int16 TerrainType = mTile_Hit[TileID];

        // Second Type?
        if (TerrainType < 0) {
            auto Type1 = (TerrainType & 0x0F);
            auto Type2 = (TerrainType >> 4) & 0x0F;

            if ((Type1 == pType && Type2 == pType2) || (Type1 == pType2 || Type2 == pType)) {

                Results.push_back(TileID);
            }

        }
    }

    return Results;
}

void cFodder::Map_Add_Structure(const sStructure& pStructure, int16 pTileX, int16 pTileY) {

    for (const auto& Piece : pStructure.mTiles) {

        MapTile_Set(pTileX + Piece.mX, pTileY + Piece.mY, Piece.mTileID);
    }

    // Add the sprites
    for (const auto& Sprite : pStructure.mSprites) {

        auto Sheet = Sprite_Get_Sheet(Sprite.mSpriteID, 0);

        // - 0x40 because no sidebar
        Sprite_Add(Sprite.mSpriteID, ((pTileX) * 16) + (Sprite.mX - Sheet->mModX) - 0x40,
            ((pTileY) * 16) + (Sprite.mY - Sheet->mModY));
    }
}

void cFodder::Map_Load_Sprites() {

    Sprite_Clear_All();

	mSprites = mMapLoaded->getSprites();

	if (mSprites.size() < mParams->mSpritesMax) {
		size_t start = mSprites.size();
		mSprites.resize(mParams->mSpritesMax);

		for (; start < mSprites.size(); ++start) {
			mSprites[start].Clear();
		}
	}

	Squad_Set_Squad_Leader();
    Map_Load_Sprites_Count();
}

void cFodder::Map_Load_Sprites_Count() {
    uint16 HumanCount = 0;
    int16 dword_37ABC = 0x0A;

    mTroops_Enemy_Count = 0;
    mHostage_Count = 0;

    for (auto& Sprite : mSprites) {

        if (Sprite.field_0 == -1 || Sprite.field_0 == -32768)
            continue;

        Sprite.field_8 = 0x7C;
        Sprite.field_32 = (HumanCount / 8);

        if (Sprite.field_18 == eSprite_Enemy_Leader || Sprite.field_18 == eSprite_Hostage) {

            ++mHostage_Count;
        }

        if (Sprite.field_18 == eSprite_Player) {

            ++HumanCount;
            Sprite.field_4A = 0;
        }
        else {

            if (Sprite.field_18 == eSprite_Enemy_Rocket) {
                Sprite.field_22 = eSprite_PersonType_AI;
                ++mTroops_Enemy_Count;

            }
            else {
                if (Sprite.field_18 == eSprite_Enemy)
                    ++mTroops_Enemy_Count;
            }

            dword_37ABC += 0x0A;
            if (Sprite.field_62 > 4)
                dword_37ABC = 0;

            Sprite.field_4A = dword_37ABC;
        }
    }
}

void cFodder::Phase_Soldiers_Count() {
    mGame_Data.mGamePhase_Data.mSoldiers_Required = 0;
    sSprite* Sprite = mSprites.data();

	// TODO: This counter needs fixing
    // How many player sprites are on this map
    for (int16 mTmpCount = 0x1D; mTmpCount > 0; --mTmpCount, ++Sprite) {
        if (Sprite->field_0 != -32768) {

            if (Sprite->field_18 == eSprite_Player)
                ++mGame_Data.mGamePhase_Data.mSoldiers_Required;
        }
    }

    // How many recruits have already been allocated to the mission
    mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count = 0;

    for( auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mRecruitID != -1) {
            --mGame_Data.mGamePhase_Data.mSoldiers_Required;
            ++mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count;
        }
    }

    // Do we still require troops?
    int16 ax = mGame_Data.mGamePhase_Data.mSoldiers_Required;
    if (mGame_Data.mGamePhase_Data.mSoldiers_Required >= 0) {

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

void cFodder::Phase_Soldiers_Prepare(const bool pPrebriefing) {
    Mission_Troops_Clear_Selected();

    if (!pPrebriefing) {

        // Set the sMission_Troop Sprites from mMission_Troops_SpritePtrs
        if (mGame_Data.mGamePhase_Data.mSoldiers_Prepare_SetFromSpritePtrs) {
            sSprite** Data24 = mMission_Troops_SpritePtrs;

            for (auto& Troop : mGame_Data.mSoldiers_Allocated) {
                Troop.mSprite = *Data24++;
            }
        }

        mGame_Data.mGamePhase_Data.mSoldiers_Prepare_SetFromSpritePtrs = true;

        sSprite** Data24 = mMission_Troops_SpritePtrs;
        for (auto& Troop : mGame_Data.mSoldiers_Allocated) {
            *Data24++ = Troop.mSprite;
        }

    }

    // Join recruits into the squad
    int16 Data1C = mGame_Data.mGamePhase_Data.mSoldiers_Required - 1;
    while (Data1C >= 0) {
        Mission_Troop_Prepare_Next_Recruits();
        --Data1C;
    }

    if (pPrebriefing)
        return;

    // Remove recruits which arn't needed for the map
    Data1C = mGame_Data.mGamePhase_Data.mSoldiers_Available;
    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mSprite == INVALID_SPRITE_PTR)
            continue;

        if (Data1C)
            --Data1C;
        else
            Troop.mSprite = INVALID_SPRITE_PTR;
    }

}

void cFodder::Mission_Troop_Prepare_Next_Recruits() {

    // Loop each troop
    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        // Does troop member have a recruit id?
        if (Troop.mRecruitID == -1) {

            if (mGame_Data.mRecruit_NextID >= 360)
                return;

            Troop.mRecruitID = mGame_Data.mRecruit_NextID;

            // All troops are equal during unit testing
            if (mParams->mUnitTesting) {
                Troop.mRank = 0;
            } else {
                // Demo sets static ranks
                if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set) {

                    Troop.mRank = (mGame_Data.mMission_Number - 1) >> 1;

                    // Jops
                    if (Troop.mRecruitID == 1)
                        Troop.mRank = 2;

                    // Jools
                    if (Troop.mRecruitID == 0)
                        Troop.mRank = 4;

                }
                else {
                    Troop.mRank = (mGame_Data.mMission_Number - 1) / 3;
                }
            }

            Troop.field_6 = 3;
            ++mGame_Data.mRecruit_NextID;

            return;
        }
    }
}

void cFodder::Phase_Soldiers_AttachToSprites() {

    int16 TroopsRemaining = mGame_Data.mGamePhase_Data.mSoldiers_Available;
    sSprite* Sprite = mSprites.data();
    sMission_Troop* Troop = mGame_Data.mSoldiers_Allocated;

	// TODO: This counter needs fixing
    // Loop the game sprites looking for 'player' sprite
    for (int16 Data18 = 0x1D; Data18 >= 0; --Data18, ++Sprite) {

        if (Sprite->field_0 == -32768)
            continue;

        if (Sprite->field_18 != eSprite_Player)
            continue;

        // 
        if (--TroopsRemaining < 0) {
            Troop->mSprite = INVALID_SPRITE_PTR;
            Sprite->field_0 = -32768;
            Sprite->field_18 = eSprite_Null;
            Sprite->field_8 = 0x7C;
            ++Troop;
        }
        else {
            // loc_1166B

            // Attach a Mission Troop to the sprite
            Sprite->field_46_mission_troop = Troop;

            Troop->mSprite = Sprite;
            Sprite->field_10 = 0x40;
            Sprite->field_22 = eSprite_PersonType_Human;

            ++Troop;
        }
    }
}

void cFodder::Camera_Speed_Update_From_PanTarget() {
	int16 Data4 = mCamera_PanTargetY - (getCameraHeight() - 8) / 2;
    if (Data4 < 0)
        Data4 = 0;

	int16 Data0 = mCamera_PanTargetX - (getCameraWidth() / 2) + 8;
    if (Data0 < 0)
        Data0 = 0;

    int16 Data8 = mCameraX >> 16;
    int16 DataC = mCameraY >> 16;

    int16 PanTargetX = Data0;
    int16 PanTargetY = Data4;
    int16 CameraX = Data8;
    int16 CameraY = DataC;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x10) {
        Data4 = mCamera_Scroll_Speed;

        if (Data4 <= (dword_39F36 >> 16)) {
            dword_39F36 = (Data4 << 16) | (dword_39F36 & 0xFFFF);
        }
        else {
            dword_39F36 += 0x8000;
        }
    }
    else {
        dword_39F36 = Data0 << 16;
    }

    DataC = CameraY;
    Data8 = CameraX;
    Data4 = PanTargetY;
    Data0 = PanTargetX;

    if (sub_119E1(Data0, Data4, Data8, DataC) >= 0) {

        Data0 &= 0x1FE;
        if (!dword_39F36)
            return;
        Data4 = Data0;

        Data8 = mMap_Direction_Calculations[Data4 / 2];
        Data4 += 0x80;
        Data4 &= 0x1FE;

        DataC = mMap_Direction_Calculations[Data4 / 2];
        Data8 >>= 2;
        DataC >>= 2;

        //seg000:197D
        int32 Dataa8 = (int32)Data8;
        int32 DataaC = (int32)DataC;

        Data4 = (dword_39F36 >> 16);
        Dataa8 *= Data4;
        DataaC *= Data4;

        mCamera_Speed_X = Dataa8;
        mCamera_Speed_Y = DataaC;

    }
    else {

        //loc_119C6
        dword_39F36 -= 0x8000;
        if (dword_39F36 < 0) {
            dword_39F36 = 0;
            mMouse_Locked = true;
        }
    }

}

int16 cFodder::sub_119E1(int16& pX1, int16& pY1, int16 pX2, int16 pY2) {

    int16 Data10 = 0, Data14 = 0;

    pX1 -= pX2;
    pY1 -= pY2;

    if (pX1 < 0)
        Data10 = 1;

    Data10 = -Data10;
    if (pX1 < 0)
        pX1 = -pX1;


    if (pY1 < 0)
        Data14 = 1;

    Data14 = -Data14;
    if (pY1 < 0)
        pY1 = -pY1;

    int16 count = 0;
    for (count = 0x0E; count >= 0; --count) {
        if (pX1 & (1 << count))
            break;
        if (pY1 & (1 << count))
            break;
    }

    count -= 4;
    if (count >= 0) {
        pX1 >>= count;
        pY1 >>= count;
    }
    pY1 <<= 5;
    pY1 |= pX1;

    pY1 = mMap_DirectionsBetweenPoints[pY1];
    if (pY1 < 0)
        return -1;

    pY1 <<= 1;
    if (Data10 < 0) {
    //loc_11AD2
        if (Data14 >= 0) {
            pX1 = 0x180;
            pX1 += pY1;
            pY1 = 0;
            return 0;
        }
        //loc_11AEF
        pX1 = 0x180;
        pX1 -= pY1;
        pY1 = 0;
        return 0;
    }

    if (Data14 < 0) {
    //loc_11ABC
        pX1 = 0x80;
        pX1 += pY1;
        pY1 = 0;
        return 0;
    }

    pX1 = 0x80;
    pX1 -= pY1;
    pY1 = 0;
    return 0;


}

void cFodder::Camera_Speed_Calculate() {

    mCamera_Speed_X += mCamera_AccelerationX;
    mCamera_Speed_Y += mCamera_AccelerationY;

    int32 Data0 = 1;

    if (mCamera_Speed_X < 0)
        Data0 = -1;

    if (Data0 != mCamera_MoveDirectionX) {
        mCamera_MoveDirectionX = Data0;
        mCamera_MovePauseX = 9;
    }

    if (mCamera_MovePauseX)
        mCamera_Speed_X = 0;

    Data0 = 1;
    if (mCamera_Speed_Y < 0)
        Data0 = -1;

    if (Data0 != mCamera_MoveDirectionY) {
        mCamera_MoveDirectionY = Data0;
        mCamera_MovePauseY = 9;
    }

    if (mCamera_MovePauseY)
        mCamera_Speed_Y = 0;

    //loc_11B9C
	// Calculate maximum right position of camera
    Data0 = mMapLoaded->getWidthPixels() - getCameraWidth();
    Data0 = (Data0 << 16) | (Data0 >> 16);

    int32 Data4 = mCameraX + mCamera_Speed_X;
    if (Data4 > Data0) {
        Data0 -= mCameraX;
        mCamera_Speed_X = Data0;
    }
    //loc_11BE8
    Data0 = mMapLoaded->getHeightPixels() - getCameraHeight();
    Data0 = (Data0 << 16) | (Data0 >> 16);

    Data4 = mCameraY + mCamera_Speed_Y;

    if (Data4 > Data0) {
        Data0 -= mCameraY;
        mCamera_Speed_Y = Data0;
    }

    Data0 = mCamera_Speed_X;
    if (Data0 < 0)
        Data0 = -Data0;

    if (Data0 > mCamera_Speed_Max) {
        Data0 = mCamera_Speed_Max;

        if ((mCamera_Speed_X >> 16) < 0)
            Data0 = -Data0;

        mCamera_Speed_X = Data0;
    }
    //loc_11C6D
    Data0 = mCamera_Speed_Y;
    if (Data0 < 0)
        Data0 = -Data0;

    if (Data0 <= mCamera_Speed_Max)
        return;

    Data0 = mCamera_Speed_Max;
    if ((mCamera_Speed_Y >> 16) < 0)
        Data0 = -Data0;

    mCamera_Speed_Y = Data0;
}

void cFodder::Camera_Prepare() {

    mCamera_TileX = mCameraX >> 16;
    mCamera_TileY = mCameraY >> 16;
    mCamera_Speed_Reset_X = false;
    mCamera_Speed_Reset_Y = false;
    mCamera_AccelerationX &= 0x0000FFFF;
    mCamera_AccelerationY &= 0x0000FFFF;
}

void cFodder::Camera_SetTargetToStartPosition() {
    int16 Data0 = mCameraX >> 16;
    int16 Data4 = mCameraY >> 16;

    int16 Data8 = mCamera_StartPosition_X;
    Data8 -= (getCameraWidth() / 2) - 8;
    if (Data8 < 0)
        Data8 = 0;

    int16 Data10 = mMapLoaded->getWidthPixels();
    Data10 -= (getCameraWidth() / 2) - 8;
    if (Data8 >= Data10)
        Data8 = Data10;

    int16 DataC = mCamera_StartPosition_Y;
    DataC -= (getCameraHeight() - 8) / 2;
    if (DataC < 0)
        DataC = 0;

    Data10 = mMapLoaded->getHeightPixels();
    Data10 -= (getCameraHeight() - 8) / 2;
    if (DataC >= Data10)
        DataC = Data10;

    if (!mPhase_MapIs17x12) {
        Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

        if (Data0 >= 0x8C)
            goto loc_11D8A;
    }
    //loc_11D62
    mCamera_PanTargetX = mCamera_StartPosition_X;
    mCamera_PanTargetY = mCamera_StartPosition_Y;

    Music_Play_Tileset();
    return;

loc_11D8A:;

    if (mPhase_Completed_Timer)
        goto loc_11E5B;

    mCamera_MovePauseX = 6;
    mCamera_MovePauseY = 6;
    mInput_Enabled = false;

    mCamera_PanTargetX = mCamera_StartPosition_X;
    mCamera_PanTargetY = mCamera_StartPosition_Y;
    mCamera_Speed_Reset_X = false;
    mCamera_AccelerationX &= 0x0000FFFF;
    mCamera_Speed_Reset_Y = false;
    mCamera_AccelerationY &= 0x0000FFFF;

    Image_FadeOut();

    if (mMap_Destroy_Tiles_Countdown)
        Map_Destroy_Tiles();

	// THis count is really meant as a backup to stop it looping forever
	for (int count = 0; count < 10000000; ++count) {
        Camera_Pan_To_Target();
        Camera_Pan_To_Target();

        if (!mCamera_Reached_Target)
            break;
    }

	if (mCamera_Reached_Target) {
		g_Debugger->Notice("Camera didnt reach target");
	}
    Mission_Sprites_Handle();
    mGraphics->Sidebar_Copy_To_Surface();
    Mouse_DrawCursor();
    Camera_Prepare();

    mInput_Enabled = true;

    mGraphics->PaletteSet();

loc_11E5B:;
    Music_Play_Tileset();
}

bool cFodder::Campaign_Load(std::string pName) {

    // If no campaign name was provided, use the default for the current version
    if (!pName.size()) {

        if (!mVersionCurrent->isCustom()) {
            pName = mVersionCurrent->mName;
        }
    }

    VersionSwitch(mVersions->GetForCampaign(pName, mParams->mDefaultPlatform));
    if (!mGame_Data.mCampaign.LoadCampaign(pName, pName != mVersionCurrent->mName)) {
        // TODO: But what?

        return false;
    }

    return true;
}

void cFodder::Map_Create(sMapParams pParams) {

    if (mVersionCurrent->isAmigaPower())
		pParams.mTileSub = eTileSub_1;

	mMapLoaded = std::make_shared<cRandomMap>(pParams);
	
	mMap = mMapLoaded->getData();

    mMapTile_Ptr = (int32)((0x60 - 8) - (mMapLoaded->getWidth() * 2));
    mMapTile_DrawX = 0;
    mMapTile_DrawY = 0;

    // Clear current sprites
    Sprite_Clear_All();

    // Load the map specific resources
    Map_Load_Resources();

}

void cFodder::Map_Load() {
	mMapLoaded = mGame_Data.mCampaign.getCMap(mGame_Data.mPhase_Current);
	mMap = mMapLoaded->getData();

    if (!mMap->size())
        return;

    Map_Load_Resources();
}

/**
 * Load the Base and Sub Tile BLK Files
 *
 * @return true if both files are loaded
 */
bool cFodder::Tiles_Load_Data() {
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

void cFodder::Map_Load_Resources() {
    std::string BaseBase, BaseSub, BaseBaseSet, BaseSubSet;

    // Is the tileset available?
    if (!Tiles_Load_Data()) {

        // Load the default version
        VersionSwitch(mVersionDefault);

        // Check the default version for the tileset
        if (!Tiles_Load_Data()) {

            // Not found, so lets go find it
            auto Version = mVersions->GetForTileset(mMapLoaded->getTileType(), mMapLoaded->getTileSub());

            // Load it
            if (Version) {
                VersionSwitch(Version);
                Tiles_Load_Data();
            }
            else {
				DataNotFound();
				return;
            }
        }

        mVersionReturnAfterPhase = true;
    }

    // jun
    BaseBaseSet.append(mMap->data(), mMap->data() + 3);

    // jun
    BaseSubSet.append(mMap->data() + 0x10, mMap->data() + 0x10 + 3);

    // junbase
    BaseBase.append(mMap->data(), mMap->data() + 7);

    // junsub0
    BaseSub.append(mMap->data() + 0x10, mMap->data() + 0x10 + 7);


    mFilenameCopt = BaseBaseSet + "copt";
    mFilenameArmy = BaseSubSet + "army";

    mFilenameBaseSwp = BaseBase + ".swp";
    mFilenameBaseHit = BaseBase + ".hit";
    mFilenameBaseBht = BaseBase + ".bht";
    mFilenameSubSwp = BaseSub + ".swp";
    mFilenameSubHit = BaseSub + ".hit";
    mFilenameSubBht = BaseSub + ".bht";
    mFilenameBasePal = BaseBase + ".pal";


    size_t Size = mResources->fileLoadTo(mFilenameBaseSwp, (uint8*)&mTile_Destroy_Swap[0]);
    tool_EndianSwap((uint8*)&mTile_Destroy_Swap[0], Size);

    Size = mResources->fileLoadTo(mFilenameSubSwp, (uint8*)&mTile_Destroy_Swap[240]);
    tool_EndianSwap((uint8*)&mTile_Destroy_Swap[240], Size);

	memset(mTile_Hit, 0, 512);

	for (int x = 0; x < 512; ++x)
		for (int y = 0; y < 8; ++y)
			mTile_BHit[x][y] = 0;

    Size = mResources->fileLoadTo(mFilenameBaseHit, (uint8*)&mTile_Hit[0]);
    tool_EndianSwap((uint8*)&mTile_Hit[0], Size);

    Size = mResources->fileLoadTo(mFilenameSubHit, (uint8*)&mTile_Hit[240]);
    tool_EndianSwap((uint8*)&mTile_Hit[240], Size);

    Size = mResources->fileLoadTo(mFilenameBaseBht, (uint8*)&mTile_BHit[0][0]);
    Size = mResources->fileLoadTo(mFilenameSubBht, (uint8*)&mTile_BHit[240][0]);

    mGraphics->Tile_Prepare_Gfx();
    mGraphics->Map_Load_Resources();
    mGraphics->PaletteSet();
}

void cFodder::Music_Play_Tileset() {
    if (!mStartParams->mDisableSound)
        mSound->Music_Play(mMapLoaded->getTileType() + 0x32);
}

void cFodder::Camera_Pan_To_Target() {
    Camera_Pan_Set_Speed();

    int32 Saved_dword_3A9FD = mCamera_Speed_Max;
    mCamera_Speed_Max = 0x100000;
    Camera_Speed_Calculate();
    mCamera_Speed_Max = Saved_dword_3A9FD;

    Camera_TileSpeedX_Set();

    MapTile_UpdateFromCamera();
    MapTile_Update_Position();
}

void cFodder::Camera_Pan_Set_Speed() {
    mCamera_Reached_Target = 2;
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;

    int16 Data0 = mCamera_PanTargetX;
    Data0 -= getCameraWidth() / 2;
    if (Data0 < 0)
        Data0 = 0;

    Data0 >>= 4;
    int16 Data4 = mCamera_PanTargetY;
    Data4 -= (getCameraHeight() - 8) / 2;
    if (Data4 < 0)
        Data4 = 0;

    Data4 >>= 4;

    int16 Data8 = mMapLoaded->getWidth();
	Data8 -= (getCameraWidth() >> 4) + 1;

    if (Data8 < 0)
        Data8 = 0;

    if (Data0 >= Data8)
        Data0 = Data8;

    Data8 = mMapLoaded->getHeight();
    Data8 -= (getCameraHeight() + 32) >> 4;
    if (Data8 < 0)
        Data8 = 0;

    if (Data4 >= Data8)
        Data4 = Data8;

    Data8 = mCameraX >> 16;
    Data8 >>= 4;
    int16 DataC = mCameraY >> 16;
    DataC >>= 4;

    if (Data8 == Data0) {
        --mCamera_Reached_Target;
    }
    else if (Data8 > Data0) {
        mCamera_Speed_X = (mCamera_Speed_X & 0xFFFF) | -CAMERA_PAN_TO_START_ACCELARATION; // (-8 << 16);
    }
    else
        mCamera_Speed_X = (mCamera_Speed_X & 0xFFFF) | CAMERA_PAN_TO_START_ACCELARATION;    // (8 << 16);

    //loc_11FAC
    if (DataC == Data4) {
        --mCamera_Reached_Target;
    }
    else if (DataC > Data4) {
        mCamera_Speed_Y = (mCamera_Speed_Y & 0xFFFF) | -CAMERA_PAN_TO_START_ACCELARATION; // (-8 << 16);
    }
    else
        mCamera_Speed_Y = (mCamera_Speed_Y & 0xFFFF) | CAMERA_PAN_TO_START_ACCELARATION;    // (8 << 16);
}

/**
 * Move the mouse position to follow the terrain as the camere pans
 */
void cFodder::Camera_Update_Mouse_Position_For_Pan() {

    if (!mMouse_Locked) {

        // Mouse in playfield?
        if (mMouseX > 0x0F) {
            mMouseX -= (mCameraX >> 16) - mCamera_TileX;
            mMouseY -= (mCameraY >> 16) - mCamera_TileY;
        }
    }

    //loc_12007
    mCamera_TileX = (mCameraX >> 16);
    mCamera_TileY = (mCameraY >> 16);
}

void cFodder::MapTile_UpdateFromCamera() {
    mMapTile_SpeedX = mCamera_TileSpeedX;
    mMapTile_SpeedY = mCamera_TileSpeedY;
    mMapTile_TargetX = mCameraX;
    mMapTile_TargetY = mCameraY;
    mMapTile_MoveDirectionX = mCamera_MoveDirectionX;
    mMapTile_MoveDirectionY = mCamera_MoveDirectionY;
}

void cFodder::Camera_Reset() {

    mCamera_TileSpeedX = 0;
    mCamera_TileSpeedY = 0;
    mCameraX = 0;
    mCameraY = 0;
    dword_39F36 &= 0x0000FFFF;
    mCamera_AccelerationX &= 0x0000FFFF;
    mCamera_AccelerationY &= 0x0000FFFF;
    mCamera_Speed_Reset_X = false;
    mCamera_Speed_Reset_Y = false;
    mCamera_TileX = 0;
    mCamera_TileY = 0;
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;
    mCamera_TileSpeed_Overflow = 0;
}

void cFodder::Camera_TileSpeedX_Set() {
    mCamera_TileSpeed_Overflow = 0;

    int32 dword_39F5A = mCameraX;

    mCameraX += mCamera_Speed_X;
    if (mCameraX < 0) {
        mCamera_Speed_X = dword_39F5A;
        if (mCamera_Speed_X)
            mCamera_Speed_X = -mCamera_Speed_X;

        mCameraX = 0;
    }
    //loc_12147
    mCamera_TileSpeedX += mCamera_Speed_X;

    if ((mCamera_Speed_X >> 16) < 0) {

        if ((mCamera_TileSpeedX >> 16) < 0) {

            mCamera_TileSpeedX += (getWindowWidth() << 16);
            mCamera_TileSpeed_Overflow = (0xFFFF << 16) | (mCamera_TileSpeed_Overflow & 0xFFFF);
        }
    }
    else {
        //loc_12181
        if ((mCamera_TileSpeedX >> 16) >= getWindowWidth()) {
            mCamera_TileSpeedX -= (getWindowWidth() << 16);
            mCamera_TileSpeed_Overflow = (1 << 16) | (mCamera_TileSpeed_Overflow & 0xFFFF);
        }
    }
    //loc_1219F
    Camera_TileSpeedY_Set();
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;
}

void cFodder::Camera_TileSpeedY_Set() {

    int32 dword_39F5A = mCameraY;

    mCameraY += mCamera_Speed_Y;
    if (mCameraY < 0) {
        mCamera_Speed_Y = dword_39F5A;

        if (mCamera_Speed_Y)
            mCamera_Speed_Y = -mCamera_Speed_Y;

        mCameraY = 0;
    }

    //loc_121F2

    mCamera_TileSpeedY += (mCamera_Speed_Y + mCamera_TileSpeed_Overflow);
    mCamera_TileSpeedY &= (0x00FF << 16) | 0xFFFF;
}

void cFodder::Camera_Speed_MaxSet() {

    mCamera_Speed_Max = 0x20000;
    if (!mPhase_Aborted && !mPhase_TryAgain && !mPhase_Complete)
        return;

    int16 Data0 = 0;

    mCamera_AccelerationX = (mCamera_AccelerationX & 0xFFFF) + (Data0 << 16);
    mCamera_AccelerationY = (mCamera_AccelerationY & 0xFFFF) + (Data0 << 16);
    mCamera_Speed_X = Data0;
    mCamera_Speed_Y = Data0;
    mCamera_Speed_Max = 0;
}

void cFodder::Camera_Speed_Reset() {

    if (mCamera_Speed_Reset_X)
        mCamera_Speed_X = 0;

    if (mCamera_Speed_Reset_Y)
        mCamera_Speed_Y = 0;
}

void cFodder::Camera_Acceleration_Set() {

    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return;

    int16 DistanceX = mSquad_Leader->field_0 + 0x40;
    int16 DistanceY = mSquad_Leader->field_4 + 0x10;

    DistanceX -= mCameraX >> 16;
    DistanceY -= mCameraY >> 16;

    if (mCamera_Speed_Reset_X && DistanceX > 0x90 && DistanceX < 0xCF) { // 90 / A2 = Amiga
        mCamera_Speed_Reset_X = false;
        mCamera_AccelerationX = 0;
    }

    if (mCamera_Speed_Reset_Y && DistanceY > 0x60 && DistanceY < 0x87) {    // 64 / 8F = Amiga
        mCamera_Speed_Reset_Y = false;
        mCamera_AccelerationY = 0;
    }
    //loc_1234F
    if (DistanceX <= 0x40) {
        mCamera_AccelerationX = (mCamera_AccelerationX & 0xFFFF) | -CAMERA_PAN_TO_SQUAD_ACCELERATION; // (-2 << 16);
        mCamera_Speed_Reset_X = true;
    }
    //loc_12362
    if (DistanceX >= getCameraWidth() + 64) {
        mCamera_AccelerationX = (mCamera_AccelerationX & 0xFFFF) | CAMERA_PAN_TO_SQUAD_ACCELERATION;  // (2 << 16);
        mCamera_Speed_Reset_X = true;
    }

    if (DistanceY <= 0x10) {
        mCamera_AccelerationY = (mCamera_AccelerationY & 0xFFFF) | -CAMERA_PAN_TO_SQUAD_ACCELERATION; // (-2 << 16);
        mCamera_Speed_Reset_Y = true;
    }

    if (DistanceY >= (getCameraHeight() - 9)) {
        mCamera_AccelerationY = (mCamera_AccelerationY & 0xFFFF) | CAMERA_PAN_TO_SQUAD_ACCELERATION;  // (2 << 16);
        mCamera_Speed_Reset_Y = true;
    }
}

void cFodder::Mission_Sprites_Handle() {

#ifndef _OFED
    Sprite_Frame_Modifier_Update();
    Sprite_Handle_Loop();
#endif

    Sprite_Sort_DrawList();

    MapTile_UpdateFromCamera();

    Map_Destroy_Tiles();
    Sprites_Draw();
}

void cFodder::Sprite_Sort_DrawList() {

    mSprite_DrawList_First.clear();
    mSprite_DrawList_Second.clear();
    mSprite_DrawList_Third.clear();
    for(sSprite& sprite : mSprites) {

        if (sprite.field_0 == -32768)
            continue; 

        if (sprite.field_2C == eSprite_Draw_First) {
            mSprite_DrawList_First.push_back(&sprite);
            continue;
        }

        if (sprite.field_2C == eSprite_Draw_Second) {
            mSprite_DrawList_Second.push_back(&sprite);
            continue;
        }

        mSprite_DrawList_Third.push_back(&sprite);
    }

    std::sort(mSprite_DrawList_Second.begin(), mSprite_DrawList_Second.end(), [](sSprite*& pLeft, sSprite*& pRight) {
        return pLeft->field_4 < pRight->field_4;
    });

    //loc_124AF
    mSprite_DrawList_Final.clear();
    for (auto spriteptr : mSprite_DrawList_First) {
        mSprite_DrawList_Final.push_back(spriteptr);
    }
    for (auto spriteptr : mSprite_DrawList_Second) {
        mSprite_DrawList_Final.push_back(spriteptr);
    }
    for (auto spriteptr : mSprite_DrawList_Third) {
        mSprite_DrawList_Final.push_back(spriteptr);
    }
}

void cFodder::Sprite_Bullet_SetData() {
    if (mSprite_TroopsAlive[0] == INVALID_SPRITE_PTR || mSprite_TroopsAlive[0] == 0)
        return;

    sSprite* Data20 = mSprite_TroopsAlive[0];
    int16 Data0 = 0;

    if (Data20->field_46_mission_troop) {
        Data0 = Data20->field_46_mission_troop->mRank;
        Data0 += 8;
        if (Data0 > 0x0F)
            Data0 = 0x0F;
    }

    mSprite_Weapon_Data = mSprite_Bullet_UnitData[Data0];
}

void cFodder::Phase_Goals_Check() {

    int16 Enemys = 0;
    int16 Buildings = 0;

	for (auto& Sprite : mSprites) {
		sSprite* Data20 = &Sprite;

        if (Data20->field_0 == -32768)
            continue;

        int16 Data10 = Data20->field_18;
        if (Data10 == eSprite_Computer_1 || Data10 == eSprite_Computer_2 || Data10 == eSprite_Computer_3 || Data10 == eSprite_BuildingDoor3 || Data10 == eSprite_BuildingDoor_Reinforced)
            goto loc_12620;

        if (Data10 == eSprite_BuildingDoor2 || Data10 == eSprite_BuildingDoor) {
            if (Data20->field_38 == eSprite_Anim_Die1)
                continue;

        loc_12620:;
            if (Data20->field_38 == eSprite_Anim_Die3)
                continue;

            ++Buildings;
            continue;
        }

        const int16* Data24 = mEnemy_Unit_Types;
        for (; *Data24 >= 0; ++Data24) {
            if (Data10 != *Data24)
                continue;

            ++Enemys;
        }
    }

    // The one demo just has two objectives for each map
    if (mVersionCurrent->isAmigaTheOne()) {

        if (Buildings || Enemys)
            return;

        mPhase_Complete = true;
        return;
    }

    mEnemy_BuildingCount = Buildings;
    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Destroy_Enemy_Buildings - 1]) {
        if (Buildings)
            return;
    }

    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Kill_All_Enemy - 1]) {
        if (Enemys)
            return;
    }

    if (!mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Kidnap_Leader - 1]) {
        if (!mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Rescue_Hostages - 1]) 
            if (!mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Rescue_Hostage - 1])
                goto loc_126A6;
    }

    if (mHostage_Count)
        return;

loc_126A6:;

    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Get_Civilian_Home - 1]) {
        if (!mSprite_Civilian_GotHome)
            return;
    }

    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Activate_All_Switches - 1]) {
        if (!mSwitchesActivated)
            return;
    }

    mPhase_Complete = true;
}

void cFodder::Phase_Goals_Set() {

    for (auto& Goal : mGame_Data.mGamePhase_Data.mGoals_Remaining)
        Goal = false;

    for (auto Goal : mGame_Data.mPhase_Current->mGoals) {

        mGame_Data.mGamePhase_Data.mGoals_Remaining[Goal - 1] = true;
    }
}

void cFodder::Phase_Progress_Check() {

    if (!mPhase_Complete)
        if (!mPhase_Aborted)
            if (!mPhase_TryAgain)
                return;

    if (mPhase_Completed_Timer < 0)
        return;

    if (mPhase_Completed_Timer)
        goto loc_1280A;

    mPhase_Completed_Timer = 0x64;

    if (mPhase_Aborted) {
        mPhase_Completed_Timer = 0x32;
        goto MissionTryAgain;
    }

    if (mPhase_Complete) {
        if (mPhase_TryAgain)
            goto MissionTryAgain;

        Phase_Show_Complete();
        goto loc_1280A;
    }
    //loc_127E1
    if (mGame_Data.mRecruits_Available_Count) {
    MissionTryAgain:;

        // Try Again sprites not availabe in 'The One'
        if(!mVersionCurrent->isAmigaTheOne())
            Phase_Show_TryAgain();

    }
    else {
        //loc_127FA
        Phase_GameOver();
    }
loc_1280A:;

    if (mPhase_Completed_Timer == 0x19) {
        mSurface->paletteNew_SetToBlack();
    }
    --mPhase_Completed_Timer;
    if (mPhase_Completed_Timer && !mParams->mUnitTesting)
        return;

    mPhase_Completed_Timer = -1;
    mGame_Data.mGamePhase_Data.mIsComplete = true;
}

void cFodder::Phase_Show_Complete() {
    Sprite_Destroy(&mSprites[40]);

    if (mGame_Data.mMission_Phases_Remaining == 1)
        Phase_TextSprite_Create_Mission(&mSprites[41]);
    else
        Phase_TextSprite_Create_Phase(&mSprites[41]);

    Phase_TextSprite_Create_Complete(&mSprites[42]);
}

void cFodder::Phase_TextSprite_Create_Mission(sSprite* pData2C) {
    Phase_TextSprite_Prepare(pData2C);

    pData2C->field_4 -= 0x14;
    pData2C->field_0 += 0x12;
    pData2C->field_8 = 0xA2;
    pData2C->field_18 = eSprite_Text_Mission;
    if (!mStartParams->mDisableSound)
        mSound->Music_Play(6);
}

void cFodder::Phase_TextSprite_Create_Phase(sSprite* pData2C) {
    Phase_TextSprite_Prepare(pData2C);

    pData2C->field_4 -= 0x14;
    pData2C->field_0 += 0x1B;
    pData2C->field_8 = 0xA1;
    pData2C->field_18 = eSprite_Text_Phase;
    
    if (!mStartParams->mDisableSound)
        mSound->Music_Play(0x0C);
}

void cFodder::Phase_TextSprite_Create_Complete(sSprite* pData2C) {
    Phase_TextSprite_Prepare(pData2C);

    pData2C->field_8 = 0xA0;
    pData2C->field_18 = eSprite_Text_Complete;
}

void cFodder::Phase_TextSprite_Prepare(sSprite* pData2C) {

    pData2C->field_0 = mMapTile_TargetX >> 16;
    pData2C->field_0 += (getCameraWidth() / 2) - 52;

    pData2C->field_4 = mMapTile_TargetY >> 16;
    pData2C->field_4 += getCameraHeight() + 10;

    pData2C->field_A = 0;
    pData2C->field_20 = 0;
    pData2C->field_52 = 0;
    pData2C->field_32 = -1;
    pData2C->field_2C = eSprite_Draw_OnTop;
}

void cFodder::Phase_Show_TryAgain() {

    Sprite_Destroy(&mSprites[40]);

    Phase_TextSprite_Create_Try(&mSprites[41]);
    Phase_TextSprite_Create_Again(&mSprites[42]);

    if (!mStartParams->mDisableSound)
        mSound->Music_Play(0x0F);
}

void cFodder::Phase_TextSprite_Create_Try(sSprite* pData2C) {
    Phase_TextSprite_Prepare(pData2C);
    pData2C->field_4 -= 0x14;
    pData2C->field_0 += 0x12;
    pData2C->field_8 = 0xCA;
    pData2C->field_18 = eSprite_Text_Try;
}

void cFodder::Phase_TextSprite_Create_Again(sSprite* pData2C) {
    Phase_TextSprite_Prepare(pData2C);

    pData2C->field_8 = 0xCB;
    pData2C->field_18 = eSprite_Text_Again;
}

void cFodder::Squad_Member_PhaseCount() {

    for( auto& Troop : mGame_Data.mSoldiers_Allocated ) {
        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        ++Troop.mPhaseCount;
    }
}

void cFodder::Sprite_Find_HumanVehicles() {

    if (mSprite_HumanVehicles_Found)
        return;

    mSprite_HumanVehicles_Found = true;

    for(auto& Sprite : mSprites) {

        if (Sprite.field_0 == -32768)
            continue;

        // Is not Human?
        if (Sprite.field_22 != eSprite_PersonType_Human)
            continue;

        // Not Enabled?
        if (!Sprite.field_65)
            continue;

        mSprites_HumanVehicles.push_back(&Sprite);
    }
}

void cFodder::Squad_Set_CurrentVehicle() {

    if (mSquad_Selected >= 0) {
        sSprite** Data20 = mSquads[mSquad_Selected];

        mSquad_CurrentVehicles[mSquad_Selected] = 0;

        int16 Data0 = 0;

        for (;;) {
            if (*Data20 != INVALID_SPRITE_PTR && *Data20 != 0) {
                sSprite* Data24 = *Data20++;

                // Sprite not in vehicle?
                if (!Data24->field_6E)
                    break;

                mSquad_CurrentVehicle = Data24->field_6A_sprite;
                Data0 = -1;
                continue;

            }
            else {
                //loc_12C2B
                if (Data0 == 0)
                    break;

                mSquad_CurrentVehicles[mSquad_Selected] = mSquad_CurrentVehicle;
                mSquad_Leader = mSquad_CurrentVehicle;
                return;
            }
        }

    }

    mSquad_CurrentVehicle = 0;
}

void cFodder::Squad_EnteredVehicle_TimerTick() {

    for (int16 Data0 = 0; Data0 < 3; ++Data0) {
        if (!mSquad_EnteredVehicleTimer[Data0])
            continue;

        mSquad_EnteredVehicleTimer[Data0]--;
        if (mSquad_EnteredVehicleTimer[Data0])
            continue;

        sSprite** Data2C = mSquads[Data0];

        for (;;) {
            if (*Data2C == INVALID_SPRITE_PTR)
                break;

            sSprite* Data30 = *Data2C++;
            Data30->field_38 = eSprite_Anim_Vehicle_Enter;
        }
    }
}

void cFodder::Phase_Map_Overview_Show() {

	// We allow the overview map to be shown on all versions in debug mode
#ifndef _DEBUG
    // Overview map is disabled for demos
    if (mVersionCurrent->isDemo() && !mVersionCurrent->isCustom())
        return;
#endif

    int16 word_3A016 = 0;
    mVideo_Draw_PosX = (mSquad_Leader->field_0) + (mSurfaceMapLeft * 16);
    mVideo_Draw_PosY = (mSquad_Leader->field_4 - 0x10) + (mSurfaceMapTop * 16);
	
	if (mVideo_Draw_PosX < 0)
		mVideo_Draw_PosX = 0;

	if (mVideo_Draw_PosY < 0)
		mVideo_Draw_PosY = 0;

    mVideo_Draw_PaletteIndex = 0xF0;

    mGraphics->PaletteSetOverview();
    mSurfaceMapOverview->Save();
    mGraphics->SetImage(mSurfaceMapOverview);

    do {
        if (mSquad_Leader != INVALID_SPRITE_PTR) {
            ++word_3A016;
            word_3A016 &= 0x1F; // Original Value: 0x3F;

            if (word_3A016 < 0x10) {
                mVideo_Draw_FrameDataPtr = mGraphics->GetSpriteData(eGFX_PSTUFF);
                mVideo_Draw_Rows = 0x10;

                mVideo_Draw_FrameDataPtr += PLATFORM_BASED(0x46B8, ((113 * 40) + 6));
                mVideo_Draw_Columns = PLATFORM_BASED(0x10, 0x02);

                mGraphics->Video_Draw_8();
            }
            else
                mSurfaceMapOverview->Restore();
        }

        Mouse_Inputs_Get();
        if (!mStartParams->mDisableVideo) {
            mWindow->RenderShrunk(mSurfaceMapOverview);
            mWindow->FrameEnd();
        }
        if (mPhase_Aborted)
            break;
        Cycle_End();

    } while (Mouse_Button_Left_Toggled() < 0);

    mSurfaceMapOverview->Restore();
    mGraphics->SetImageOriginal();

    mPhase_Aborted = false;
}

void cFodder::Map_Overview_Prepare() {

    if (mParams->mUnitTesting)
        return;

    delete mSurfaceMapOverview;
    size_t Size = mMapLoaded->getWidth() < mMapLoaded->getHeight() ? mMapLoaded->getHeight() : mMapLoaded->getWidth();
	if ((Size * 16) * (Size * 16) >= 0x7FFFFFFF)
		return;

    mSurfaceMapOverview = new cSurface(Size * 16, Size * 16);
    mSurfaceMapOverview->clearBuffer();

    int16* MapPtr = (int16*)(mMap->data() + 0x60);

    mSurfaceMapTop = mSurfaceMapLeft = 0;

    if (mMapLoaded->getHeight() < mMapLoaded->getWidth()) {
        mSurfaceMapTop = (mMapLoaded->getWidth() / 2) - (mMapLoaded->getHeight() / 2);
        if (mSurfaceMapTop < 0)
            mSurfaceMapTop = 0;
    }

    if (mMapLoaded->getWidth() < mMapLoaded->getHeight()) {
        mSurfaceMapLeft = (mMapLoaded->getHeight() / 2) - (mMapLoaded->getWidth() / 2);
        if (mSurfaceMapLeft < 0)
            mSurfaceMapLeft = 0;
    }

    for (uint16 dx = 0; dx < mMapLoaded->getHeight(); ++dx) {

        for (uint16 cx = 0; cx < mMapLoaded->getWidth(); ++cx, ++MapPtr) {
            
            if (MapPtr < (int16*) mMap->data() || MapPtr >= (int16*) (mMap->data() + mMap->size()))
                continue;

            mGraphics->MapOverview_Render_Tiles(*MapPtr & 0x1FF, cx + mSurfaceMapLeft, dx + mSurfaceMapTop);
        }
    }

}

void cFodder::eventProcess(const cEvent& pEvent) {
    switch (pEvent.mType) {

    case eEvent_KeyDown:
        keyProcess(pEvent.mButton, false);
        break;

    case eEvent_KeyUp:
        keyProcess(pEvent.mButton, true);
        break;

    case eEvent_MouseLeftDown:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastButtonsPressed |= 1;
        break;

    case eEvent_MouseRightDown:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastButtonsPressed |= 2;
        break;

    case eEvent_MouseLeftUp:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastButtonsPressed &= ~1;
        break;

    case eEvent_MouseRightUp:
        mMouse_EventLastPosition = pEvent.mPosition;
        mMouse_EventLastButtonsPressed &= ~2;
        break;

    case eEvent_MouseMove:
        mMouse_EventLastPosition = pEvent.mPosition;
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

void cFodder::eventsProcess() {

    mMouse_EventLastWheel.Clear();

    if (mParams->mDemoPlayback) {
        for (auto Event : mGame_Data.mDemoRecorded.GetEvents(mGame_Data.mDemoRecorded.mTick))
            eventProcess(Event);

    } else {
        for (auto Event : *mWindow->EventGet()) {
            if (mParams->mDemoRecord) {
                if(Event.mType != eEventType::eEvent_MouseMove)
                    mGame_Data.mDemoRecorded.AddEvent(mGame_Data.mDemoRecorded.mTick, Event);
            }
            eventProcess(Event);
        }
    }

    mWindow->EventGet()->clear();

    mGame_Data.mDemoRecorded.Tick();
}

void cFodder::keyProcess(uint8 pKeyCode, bool pPressed) {
    if (pPressed)
        mKeyCode = pKeyCode;
    else
        mKeyCode = 0;

    // Switch between platforms
    if (!mVersionPlatformSwitchDisabled) {
        mVersionPlatformSwitchDisabled = true;
        if ((mVersionDefault && mVersionDefault->isRetail()) || (mVersionCurrent && mVersionCurrent->isRetail())) {

            if (pKeyCode == SDL_SCANCODE_F1 && pPressed) {
                mVersionDefault = mVersions->GetForCampaign(mVersionCurrent->mName, ePlatform::Amiga);
                mParams->mDefaultPlatform = ePlatform::Amiga;
                VersionSwitch(mVersionDefault);
            }
            if (pKeyCode == SDL_SCANCODE_F2 && pPressed) {
                mVersionDefault = mVersions->GetForCampaign(mVersionCurrent->mName, ePlatform::PC); 
                mParams->mDefaultPlatform = ePlatform::PC;
                VersionSwitch(mVersionDefault);
            }
        }
        mVersionPlatformSwitchDisabled = false;
    }

    if (pKeyCode == SDL_SCANCODE_F3 && pPressed) {
        if (mParams->mDemoRecord) {
            mStartParams->mDemoRecordResumeCycle = mGame_Data.mDemoRecorded.mTick - 80;
            mGame_Data.mGamePhase_Data.mIsComplete = true;
            mPhase_TryAgain = true;
        }
    }

    if ((pKeyCode == SDL_SCANCODE_EQUALS && pPressed) || (pKeyCode == SDL_SCANCODE_KP_PLUS && pPressed))
        mWindow->WindowIncrease();

    if ((pKeyCode == SDL_SCANCODE_MINUS && pPressed) || (pKeyCode == SDL_SCANCODE_KP_MINUS && pPressed))
        mWindow->WindowDecrease();

    if (pKeyCode == SDL_SCANCODE_F11 && pPressed)
        mWindow->ToggleFullscreen();

    if (pKeyCode == SDL_SCANCODE_ESCAPE && pPressed && mPhase_Aborted)
        mPhase_Aborted2 = true;

    if (pKeyCode == SDL_SCANCODE_ESCAPE && pPressed) {
        mPhase_Aborted = true;
        mPhase_EscapeKeyAbort = true;
    }
    // In Mission and not on map overview
    if (mPhase_In_Progress && !mPhase_ShowMapOverview) {

        if (pKeyCode == SDL_SCANCODE_LCTRL || pKeyCode == SDL_SCANCODE_RCTRL) {
            if (pPressed)
                mKeyControlPressed = -1;
            else
                mKeyControlPressed = 0;
        }

        if (pKeyCode == SDL_SCANCODE_P && pPressed)
            mPhase_Paused = !mPhase_Paused;

        if (pKeyCode == SDL_SCANCODE_SPACE && pPressed)
            ++mSquad_SwitchWeapon;

        if (pKeyCode == SDL_SCANCODE_M && pPressed) {
            if (mPhase_Finished == false)
                mPhase_ShowMapOverview = -1;
        }

        if (pKeyCode == SDL_SCANCODE_1 && pPressed) {
            if(mSquads_TroopCount[0])
                Squad_Select(0, false);
        }

        if (pKeyCode == SDL_SCANCODE_2 && pPressed) {
            if(mSquads_TroopCount[1])
                Squad_Select(1, false);
        }
        if (pKeyCode == SDL_SCANCODE_3 && pPressed) {
            if(mSquads_TroopCount[2])
                Squad_Select(2, false);
        }

		if (mParams->mCheatsEnabled) {

			// Debug: Mission Complete
			if (pKeyCode == SDL_SCANCODE_F10 && pPressed) {
				mDebug_PhaseSkip = -1;
			}

			// Debug: Make current squad invincible
			if (pKeyCode == SDL_SCANCODE_F9 && pPressed) {
				if (mSquad_Selected >= 0) {
					sSprite** Data28 = mSquads[mSquad_Selected];
					for (; *Data28 != INVALID_SPRITE_PTR;) {

						sSprite* Data2C = *Data28++;
						Data2C->field_75 |= eSprite_Flag_Invincibility;
					}
				}
			}
		}
    }
}

void cFodder::Mouse_Setup() {

    mMouse_EventLastButtonsPressed = 0;
    mButtonPressLeft = 0;
    mButtonPressRight = 0;
    mMouseButtonStatus = 0;

    mMouseX = (getCameraWidth() / 2) - 9;
    mMouseY = (getCameraHeight() / 2) - 9;
}

void cFodder::Mouse_Cursor_Handle() {
	static bool WasClicked = false;
	static bool CursorGrabbed = false;
	const cPosition WindowPos = mWindow->GetWindowPosition();
	const cDimension ScreenSize = mWindow->GetScreenSize();
	const cDimension WindowSize = mWindow->GetWindowSize();
	const cDimension scale = mWindow->GetScale();

	mMouseButtonStatus = mMouse_EventLastButtonsPressed;

	if (!mWindow->hasFocusEvent() && CursorGrabbed)
		CursorGrabbed = false;

	if (mStartParams->mMouseAlternative) {
		mInputMouseX = (mMouse_EventLastPosition.mX / scale.getWidth()) + MOUSE_POSITION_X_ADJUST;
		mInputMouseY = (mMouse_EventLastPosition.mY / scale.getHeight()) + MOUSE_POSITION_Y_ADJUST;
		return;
	}

    // Check if the system mouse is grabbed
    if (!CursorGrabbed) {

        if (!mWindow->hasFocusEvent() || mWindow->isMouseInside()) {
            // Register mouse position even when not focused but cursor on window
            mInputMouseX = (mMouse_EventLastPosition.mX / scale.getWidth()) + MOUSE_POSITION_X_ADJUST;
            mInputMouseY = (mMouse_EventLastPosition.mY / scale.getHeight()) + MOUSE_POSITION_Y_ADJUST;
        }
        
        // Check if the system cursor x/y is inside our window
        // and ensure the mouse button has been released before we focus
        if (mWindow->hasFocusEvent() && mWindow->isMouseInside() && !mWindow->isMouseButtonPressed_Global()) {
            WasClicked = true;
            CursorGrabbed = true;

            if (!mWindow->isFullscreen()) {
                // Ensure X not too close to a border
                if (mInputMouseX <= MOUSE_POSITION_X_ADJUST)
                    mInputMouseX = MOUSE_POSITION_X_ADJUST + 1;
                else if (mInputMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1)
                    mInputMouseX = ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 2;

                // Ensure Y not too close to a border
                if (mInputMouseY <= MOUSE_POSITION_Y_ADJUST)
                    mInputMouseY = MOUSE_POSITION_Y_ADJUST + 1;
                else if (mInputMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1)
                    mInputMouseY = ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 2;
            }
        }
    } else {
        cPosition BorderMouse;

        if (!mWindow->isFullscreen()) {
            // Need to check if the game cursor is near an edge: top-left / top-right / bottom-left / bottom-right
            if ((mMouseX <= MOUSE_POSITION_X_ADJUST && mMouseY <= MOUSE_POSITION_Y_ADJUST) ||
                (mMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1 && mMouseY <= MOUSE_POSITION_Y_ADJUST) ||
                (mMouseX <= MOUSE_POSITION_X_ADJUST && mMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1) ||
                (mMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1 && mMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1)) {
                BorderMouse.mX = mMouseX <= MOUSE_POSITION_X_ADJUST ? WindowPos.mX - 1 : WindowPos.mX + WindowSize.getWidth() + 1;
                BorderMouse.mY = mMouseY <= MOUSE_POSITION_Y_ADJUST ? WindowPos.mY - 1 : WindowPos.mY + WindowSize.getHeight() + 1;
            }
            // Need to check if the game cursor is near a border on X axis
            else if (mMouseX <= MOUSE_POSITION_X_ADJUST || mMouseX >= ScreenSize.getWidth() + MOUSE_POSITION_X_ADJUST - 1) {
                BorderMouse.mX = mMouseX <= MOUSE_POSITION_X_ADJUST ? WindowPos.mX - 1 : WindowPos.mX + WindowSize.getWidth() + 1;
                BorderMouse.mY = WindowPos.mY + (mMouseY - MOUSE_POSITION_Y_ADJUST) * scale.getHeight();
            } // Need to check if the game cursor is near a border Y axis
            else if (mMouseY <= MOUSE_POSITION_Y_ADJUST || mMouseY >= ScreenSize.getHeight() + MOUSE_POSITION_Y_ADJUST - 1) {
                BorderMouse.mX = WindowPos.mX + (mMouseX - MOUSE_POSITION_X_ADJUST) * scale.getWidth();
                BorderMouse.mY = mMouseY <= MOUSE_POSITION_Y_ADJUST ? WindowPos.mY - 1 : WindowPos.mY + WindowSize.getHeight() + 1;
            }

            //  if yes set system cursor outside the border
            //  if yes set system cursor outside the border
            if (BorderMouse.mX || BorderMouse.mY) {
                CursorGrabbed = false;
                mWindow->SetMousePosition(BorderMouse);
                return;
            }
        }

        // hack to avoid moving cursor on window resizing
        if (mWindow->isResized()) {
            mWindow->ClearResized();
        } else {
            if (WasClicked) {
                if (!mWindow->isMouseButtonPressed_Global())
                    WasClicked = false;
            } else {
                // Calc the distance from the cursor to the centre of the window
                const cPosition Diff = (mMouse_EventLastPosition - WindowSize.getCentre());

                mInputMouseX = mMouseX + static_cast<int16>((Diff.mX / scale.getWidth()) * 1.5);
                mInputMouseY = mMouseY + static_cast<int16>((Diff.mY / scale.getHeight()) * 1.5);
            }
        }

        // Set system cursor back to centre of window
        mWindow->SetMousePosition(WindowSize.getCentre() + WindowPos);
    }
}

void cFodder::Mouse_Inputs_Get() {

	
    if (mParams->mDemoPlayback) {

       // Window_UpdateScreenSize();

        auto State = mGame_Data.mDemoRecorded.GetState(mGame_Data.mDemoRecorded.mTick);
        if (State) {
            mInputMouseX = State->mInputMouseX;
            mInputMouseY = State->mInputMouseY;

            mMouseButtonStatus = State->mMouseButtonStatus;
        }
        else {
            if(mGame_Data.mDemoRecorded.mTick > mGame_Data.mDemoRecorded.GetTotalTicks() + 100 )
                mPhase_Aborted = true;
        }
    }
    else {
        Mouse_Cursor_Handle();
    }

    if (mParams->mDemoRecord)

        mGame_Data.mDemoRecorded.AddState(mGame_Data.mDemoRecorded.mTick, cStateRecorded{ mInputMouseX, mInputMouseY, mMouseButtonStatus });

    Mouse_ButtonCheck();

    int16 Data4 = mInputMouseX;

    if (mSidebar_SmallMode == 0)
        goto loc_13B3A;

    if (Data4 >= MOUSE_POSITION_X_ADJUST + 16)
        goto loc_13B58;

    goto loc_13B41;

loc_13B3A:;
    if (Data4 >= MOUSE_POSITION_X_ADJUST)
        goto loc_13B58;

loc_13B41:;
    if (mSidebar_SmallMode)
        Data4 = MOUSE_POSITION_X_ADJUST + 16;
    else
        Data4 = MOUSE_POSITION_X_ADJUST;

    goto loc_13B66;

loc_13B58:;

    if (Data4 > (int16)mWindow->GetScreenSize().getWidth() + MOUSE_POSITION_X_ADJUST - 1)
        Data4 = (int16)mWindow->GetScreenSize().getWidth() + MOUSE_POSITION_X_ADJUST - 1;

loc_13B66:;
    mMouseX = Data4;

    int16 Data0 = mInputMouseY;

    if (Data0 < MOUSE_POSITION_Y_ADJUST)
        Data0 = MOUSE_POSITION_Y_ADJUST;
    else {

        if (Data0 > (int16)mWindow->GetScreenSize().getHeight() + MOUSE_POSITION_Y_ADJUST - 1)
            Data0 = (int16)mWindow->GetScreenSize().getHeight() + MOUSE_POSITION_Y_ADJUST - 1;
    }

    mMouseY = Data0;
}

void cFodder::Mouse_ButtonCheck() {

    mButtonPressLeft = 0;
    if (mMouseButtonStatus & 1) {
        mButtonPressLeft -= 1;
        if (mMouse_Button_Left_Toggle == 0) {
            mMouse_Button_Left_Toggle = -1;
            mMouse_Exit_Loop = true;

            if (mButtonPressRight) {
                mMouse_Button_LeftRight_Toggle = true;
                mMouse_Button_LeftRight_Toggle2 = true;
            }
        }

    }
    else {
        mMouse_Button_LeftRight_Toggle2 = false;
        mMouse_Button_Left_Toggle = 0;
    }

    mButtonPressRight = 0;
    if (mMouseButtonStatus & 2) {
        mButtonPressRight -= 1;
        if (mMouse_Button_Right_Toggle == 0) {
            mSquad_Member_Fire_CoolDown_Override = true;
            mMouse_Button_Right_Toggle = -1;
        }
    }
    else {
        mMouse_Button_Right_Toggle = 0;
    }

}

void cFodder::WindowTitleSet(bool pInMission) {
    std::stringstream Title;
    Title << mWindowTitle.str();

    if (pInMission && mGame_Data.mPhase_Current && mGame_Data.mMission_Current) {
        if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set ) {
            Title << " ( Mission: ";
            Title << mGame_Data.mPhase_Current->mName;
        }
        else {
            Title << " ( Mission: " << mGame_Data.mMission_Number;
            Title << " " << mGame_Data.mMission_Current->mName;

            Title << "  Phase: " << (mGame_Data.mMission_Phase) << " ";

            if (mGame_Data.mMission_Current->NumberOfPhases() > 1) {
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

void cFodder::WindowTitleBaseSetup() {

    mWindowTitle.str("");
    mWindowTitle << "Open Fodder";

    if (mVersionCurrent) {

        // Ensure we have a campaign
        if (mGame_Data.mCampaign.getName().size()) {
            if (mGame_Data.mCampaign.isCustom()) {
                mWindowTitle << ": Custom (" << mGame_Data.mCampaign.getName() << ")";
            } else {
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
void cFodder::VersionSwitch(const sGameVersion* pVersion) {
	const sGameVersion* VersionPrevious = mVersionCurrent;

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

	if(!mSound)
		mSound = mVersionCurrent->GetSound();

    if(!mResources) {
        std::cout << "Unknown Platform";
        exit(1);
    }

    mGUI_Select_File_ShownItems = PLATFORM_BASED(4, 5);

    Window_UpdateScreenSize();

    mGraphics->Load_Sprite_Font();
    mGraphics->Load_Hill_Data();
    mGraphics->Load_pStuff();

    if (mPhase_In_Progress) {
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);

        // Reload Map Data
        Map_Load_Resources();
        Map_Overview_Prepare();
        mGraphics->MapTiles_Draw();

        // Redraw sidebar
        mGUI_Sidebar_Setup = 0;
        while(mGUI_Sidebar_Setup>=0)
            GUI_Sidebar_Setup();

        mSurface->palette_FadeTowardNew();
        Music_Play_Tileset();
    }
    
    if(mRecruit_Screen_Active && mVersionCurrent->hasGfx(eGFX_HILL)) {
        Recruit_Prepare();

        mRecruit_RenderedNext = mRecruit_Rendereds.begin();
        for (int i = 0; i < mGame_Data.mGamePhase_Data.mSoldiers_Required - mRecruit_Truck_Troops_ToEnter_Count; ++i) {
            Recruit_Sidebar_Render_SquadName();
        }

        if (!mStartParams->mDisableSound)
            mSound->Music_Play(0);
    }

}

int16 cFodder::getWindowWidth() const {
	if (!mParams->mWindowColumns) {
		if (mVersionCurrent)
			return 320;

		return 352;
	}

	return (int16) mParams->mWindowColumns * 16;
}

cDimension cFodder::getSurfaceSize() const {
	if (!mParams->mWindowColumns || !mParams->mWindowRows) {
		if (mVersionCurrent)
			return mVersionCurrent->GetScreenSize();

		return { 352, 364 };
	}

	return { (unsigned int)(mParams->mWindowColumns + 2) * 16, (unsigned int)(mParams->mWindowRows + 2) * 16 };
}

cDimension cFodder::getWindowSize() const {

	if (!mParams->mWindowColumns || !mParams->mWindowRows) {
		if (mVersionCurrent)
			return mVersionCurrent->GetScreenSize();

		return { 336, 348 };
	}

	return { (unsigned int) mParams->mWindowColumns * 16, (unsigned int) mParams->mWindowRows * 16 };
}

int16 cFodder::getWindowRows() const {
	if (!mParams->mWindowRows) {
		return 16;
	}
	return (int16) mParams->mWindowRows;
}

int16 cFodder::getWindowColumns() const {
	if (!mParams->mWindowColumns) {
		if (mVersionCurrent->isAmiga())
			return 21;

		return 22;
	}
	return (int16) mParams->mWindowColumns;
}

int16 cFodder::getCameraWidth() const {

	return ((getWindowWidth() - SIDEBAR_WIDTH));
}

int16 cFodder::getCameraHeight() const {
	if (!mParams->mWindowRows) {
		if (mVersionCurrent)
			return mVersionCurrent->GetScreenSize().mHeight;

		return 364;
	}

	return (int16) mParams->mWindowRows * 16;
}

void cFodder::DataNotFound() {
	g_Debugger->Error("No game data could be found, including the demos, have you installed the data pack?");

	g_Debugger->Error("We are looking for the 'Data' directory in: ");
	for (auto path : g_ResourceMan->getAllPaths()) {
		g_Debugger->Error(path);
	}

	g_Debugger->Error("Press enter to quit");
	std::cin.get();
	exit(1);
}

void cFodder::Prepare(std::shared_ptr<sFodderParameters> pParams) {
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

    mSidebar_Back_Buffer = (uint16*) new uint8[mSidebar_Buffer_Size];
    mSidebar_Screen_Buffer = (uint16*) new uint8[mSidebar_Buffer_Size];
    mSidebar_Screen_BufferPtr = mSidebar_Screen_Buffer;

    mBriefing_Render_1_Mode = -1;

    mSurface = new cSurface( getSurfaceSize() );
    mSurface2 = new cSurface(getSurfaceSize() );

	Sprite_Clear_All();

	g_ScriptingEngine = std::make_shared<cScriptingEngine>();
}

void cFodder::Sprite_Count_HelicopterCallPads() {

    int16 Data0 = 0;
    for(auto& Sprite : mSprites) {

        if (Sprite.field_18 == eSprite_Helicopter_CallPad)
            ++Data0;
    }

    mHelicopterCallPadCount = Data0;
    mHelicopterCall_X = -1;
}

void cFodder::Mission_Set_Final_TimeRemaining() {
    mMission_Final_TimeRemain = 0x64;
    mMission_Final_TimeToDie_Ticker = 0x28;
}

void cFodder::Sprite_HelicopterCallPad_Check() {
    if (!mHelicopterCallPadCount)
        return;

    if (mHelicopterCallPadCount != mHelicopterCallPadPressedCount)
        return;

    if (mHelicopterCall_X >= 0)
        return;

    if (!mTroop_InRange_Callpad || mTroop_InRange_Callpad == INVALID_SPRITE_PTR)
        return;

    mHelicopterCall_X = mTroop_InRange_Callpad->field_0;
    mHelicopterCall_Y = mTroop_InRange_Callpad->field_4;

    mHelicopterCall_Y += 0x28;
}

void cFodder::Mission_Final_Timer() {

    if (mVersionCurrent->isRetail() && mVersionCurrent->isCannonFodder1()) {

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

int16 cFodder::Sprite_Create_RandomExplosion() {
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30)) {
        Data0 = -1;
        return -1;
    }

    Data2C->field_0 = mMapTile_TargetX >> 16;
    Data0 = tool_RandomGet() & 0xFF;
    Data0 += 0x0A;

    Data2C->field_0 += Data0;
    Data2C->field_4 = mMapTile_TargetY >> 16;

    Data0 = tool_RandomGet() & 0xFF;
    Data0 -= 0x14;

    Data2C->field_4 += Data0;
    Data2C->field_18 = eSprite_Explosion;
    return Data0;
}

void cFodder::Phase_Paused() {
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
        String_Print(mFont_Underlined_Width, 1, mGUI_Temp_X, 0x54,  "GAME PAUSED");

        mSurface2->draw();
        mString_GapCharID = 0;
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        mGraphics->SetImageOriginal();
    }
}

void cFodder::Phase_GameOver() {
    sSprite* Data24 = &mSprites[40];
    Sprite_Destroy(Data24);

    sSprite* Data2C = &mSprites[41];
    Phase_TextSprite_Create_GameOver(Data2C);
}

void cFodder::Phase_TextSprite_Create_GameOver(sSprite* pData2C) {
    Phase_TextSprite_Prepare(pData2C);
    pData2C->field_8 = 0xC1;
    pData2C->field_18 = eSprite_Text_GameOver;
    
    if (!mStartParams->mDisableSound)
        mSound->Music_Play(8);
}

void cFodder::Mouse_DrawCursor() {
    if (mParams->mDisableVideo)
        return;

    mVideo_Draw_PosX = (mMouseX + mMouseX_Offset) + SIDEBAR_WIDTH;
    mVideo_Draw_PosY = (mMouseY + mMouseY_Offset) + 12;

    if (mMouseSpriteNew >= 0) {
        mMouseSpriteCurrent = mMouseSpriteNew;
        mMouseSpriteNew = -1;
    }

    if (mGraphics)
        mGraphics->Mouse_DrawCursor();
}

void cFodder::Sprite_Draw_Frame(sSprite* pDi, int16 pSpriteType, int16 pFrame, cSurface *pDestination) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount - pDi->field_52;

#if defined(_OFED) || defined(_OFBOT)
    mMapTile_DrawY = (mMapTile_MovedVertical * 16) + mMapTile_RowOffset;
    mMapTile_DrawX = (mMapTile_MovedHorizontal * 16) + mMapTile_ColumnOffset;
    
    mVideo_Draw_PosX = (int16)((SheetData->mModX + pDi->field_0) - mMapTile_DrawX);
#else
    mVideo_Draw_PosX = (int16)((SheetData->mModX + pDi->field_0) - mMapTile_DrawX + 0x40);
#endif

    mVideo_Draw_PosY = (int16)((SheetData->mModY + pDi->field_4) - mVideo_Draw_Rows - pDi->field_20 - mMapTile_DrawY);
    mVideo_Draw_PosY += 0x10;

    if (Sprite_OnScreen_Check()) {
        pDi->field_5C = 1;
        if(!mStartParams->mDisableVideo)
            mGraphics->Video_Draw_8(pDestination);
    }
    else
        pDi->field_5C = 0;
}

bool cFodder::Sprite_OnScreen_Check() {

    return mGraphics->Sprite_OnScreen_Check();
}

void cFodder::Sprites_Draw() {

    for( auto& Sprite : mSprite_DrawList_Final) {

        if (Sprite->field_24) {
            Sprite->field_24 = 0;
            Sprite->field_0 = -32768;
            mSprite_SpareUsed = 0;
            mSprite_SpareUsed2 = 0;
        }
        else {
            int16 Data0 = Sprite->field_8;
            int16 Data4 = Sprite->field_A;
            Sprite_Draw_Frame(Sprite, Data0, Data4);
        }
    }
}

void cFodder::Sprite_Map_Sound_Play(int16 &pData0) {

    mSoundEffectToPlay_Set = pData0;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    goto loc_14D66;
    //seg002:05B0
    // UNUSED BLOCK

loc_14D66:;
    mSoundEffectToPlay = mSoundEffectToPlay_Set;
}

void cFodder::Sound_Play(sSprite* pSprite, int16 pSoundEffect, int16 pData8) {

    if (mSoundDisabled)
        return;

    //loc_14BD4
    pData8 = mCameraX >> 16;
    pData8 += (getCameraWidth() / 2) - 8;

    if (pSprite != INVALID_SPRITE_PTR)
        pData8 -= pSprite->field_0;

    int16 DataC = mCameraY >> 16;
    DataC += (getCameraHeight() - 8) / 2;

    if (pSprite != INVALID_SPRITE_PTR)
        DataC -= pSprite->field_4;
    
    int16 X = 0;
    int16 Y = 0;

    Map_Get_Distance_BetweenPoints_Within_640(X, Y, pData8, DataC);

    int Volume = 40;
    Volume -= (X / 16);

    if (Volume <= 0)
        return;

    if (!mStartParams->mDisableSound)
        mSound->Sound_Play(mMapLoaded->getTileType(), pSoundEffect, Volume);
}

void cFodder::Mission_Intro_Helicopter_Start() {
    mHelicopterPosX = 0x01500000;
    if (mVersionCurrent->isPC())
        mHelicopterPosY = 0x00260000;
    else
        mHelicopterPosY = 0x00300000;

    mBriefing_Helicopter_Off1 = mBriefing_Helicopter_Offsets[0];
    mBriefing_Helicopter_Off2 = mBriefing_Helicopter_Offsets[1];
    mBriefing_Helicopter_Off3 = mBriefing_Helicopter_Offsets[2];
    mBriefing_Helicopter_Off4 = &mBriefing_Helicopter_Offsets[3];
    mBriefing_ParaHeli_Frame = 0;
    mBriefing_Helicopter_Moving = -1;
    word_428D8 = -1;

    sub_1594F();
}

void cFodder::Briefing_Update_Helicopter() {
    word_428B6 &= 0x1FE;
    uint16 bx = word_428B6;

    int32 ax = mMap_Direction_Calculations[(bx / 2) & 0xFF];

    ax >>= 2;

    mHelicopterPosX += ax * word_428B8;

    bx += 0x80;
    bx &= 0x1FE;

    ax = mMap_Direction_Calculations[(bx / 2) & 0xFF];
    ax >>= 2;
    mHelicopterPosY += ax * word_428B8;

    bx = mBriefing_Helicopter_Off1 - word_428B6;
    bx >>= 5;
    --bx;
    bx ^= 0x0F;
    bx &= 0x0F;

    int16 al = mSprite_Direction_Frame_Unk[bx];
    al <<= 2;
    word_428B6 += al;
    word_428B6 &= 0x1FE;

    if (mBriefing_Helicopter_Off2 != word_428B8) {
        if (word_428B8 >= mBriefing_Helicopter_Off2)
            word_428B8 -= 4;
        else
            word_428B8 += 4;
    }

	if (mVersionCurrent->isCannonFodder1() && mVersionCurrent->isPC())
		mBriefing_ParaHeli_Frame += 1;

    if (mBriefing_ParaHeli_Frame == 4)
        mBriefing_ParaHeli_Frame = 0;

    --word_428BA;
    if (word_428BA <= 0)
        sub_1594F();

}

void cFodder::sub_1594F() {

    word_428B6 = mBriefing_Helicopter_Off1;
    word_428B8 = mBriefing_Helicopter_Off2;
    word_428BA = mBriefing_Helicopter_Off3;
    mBriefing_Helicopter_Off1 = *mBriefing_Helicopter_Off4++;
    mBriefing_Helicopter_Off2 = *mBriefing_Helicopter_Off4++;
    mBriefing_Helicopter_Off3 = *mBriefing_Helicopter_Off4++;

    if (word_428B6 == -1) {
        mBriefing_Helicopter_Moving = 0;
        word_428D8 = 0;

        mGraphics->mImageMissionIntro.CopyPalette(mGraphics->mPalette, 0x100, 0);
        mSurface->paletteNew_SetToBlack();
    }

}

void cFodder::Mission_Intro_Draw_OpenFodder() {
    // Draw OPEN FODDER
    {
        mString_GapCharID = 0x25;
        String_Print_Large("OPEN FODDER", true, 0x01);
    }

    // Draw Mission Name, or Map 
    {
        int16 DrawY = 0xB5;

        DrawY += PLATFORM_BASED(0, 0x16);
        //String_Print_Large("CAMPAIGNS", false, DrawY);
    }
}

void cFodder::Mission_Intro_Draw_Mission_Name() {

    if (mGame_Data.mMission_Number == 0)
        Mission_Intro_Draw_OpenFodder();
    else
        Briefing_Draw_Mission_Title(0xB5);
}

/**
 * Draw the Mission Name or the Map Name
 *  Drawing at 0xB5 will cause the Mission Name to be drawn.
 *
 * @param pDrawAtY
 */
void cFodder::Briefing_Draw_Mission_Title(int16 pDrawAtY) {

    // Draw MISSION xx
    {
        std::stringstream Mission;
        Mission << "MISSION ";
        mString_GapCharID = 0x25;
        Mission << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Number));

        String_Print_Large(Mission.str(), true, 0);
    }

    // Draw Mission Name, or Map 
    {
        std::string Title;

        if (pDrawAtY == 0xB5) {
            Title = mGame_Data.mMission_Current->GetName();

            pDrawAtY += PLATFORM_BASED(0, 0x16);

        }
        else {
            Title = mGame_Data.mPhase_Current->GetName();
        }

        String_Print_Large(Title, true, pDrawAtY);
    }
}

void cFodder::Campaign_Select_DrawMenu(const char* pTitle, const char* pSubTitle) {
    size_t YOffset = PLATFORM_BASED(0, 25);

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    GUI_Element_Reset();

    mString_GapCharID = 0x25;
    String_Print_Large(pTitle, true, 0x01);
    mString_GapCharID = 0x00;

    String_Print_Large(pSubTitle, false, 0x18);

    if (mGUI_Select_File_Count != mGUI_Select_File_ShownItems) {
        GUI_Button_Draw_Small("UP", 0x30);
        GUI_Button_Setup_Small(&cFodder::GUI_Button_Load_Up);

        GUI_Button_Draw_Small("DOWN", 0x99 + YOffset);
        GUI_Button_Setup_Small(&cFodder::GUI_Button_Load_Down);
    }

    GUI_Button_Draw_Small("EXIT", 0xB3 + YOffset);
    GUI_Button_Setup_Small(&cFodder::GUI_Button_Load_Exit);

    GUI_Button_Draw_SmallAt("ABOUT", 0xA, 0xB3 + YOffset);
    GUI_Button_Setup_Small(&cFodder::GUI_Button_Show_About);


    int16 ItemCount = 0;

    auto FileIT = mCampaignList.begin() + mGUI_Select_File_CurrentIndex;

    for (; ItemCount < mGUI_Select_File_ShownItems && FileIT != mCampaignList.end(); ++ItemCount) {

        GUI_Button_Draw_Small(FileIT->c_str(), 0x44 + (ItemCount * 0x15), 0xB2, 0xB3);
        GUI_Button_Setup_Small(&cFodder::GUI_Button_Filename);
        ++FileIT;
    }
}

// Campaign Selection screen tile data
static std::vector<unsigned char> mCampaignSelectMap_Jungle = {
    0x18, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0xd1, 0x00, 0x6a, 0x01, 0x30, 0x01, 0x6b, 0x01, 0x8b,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x08, 0x01, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x4d, 0x01, 0x4e, 0x01, 0x7c, 0x00, 0x19, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01,
    0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1b, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x10, 0x10, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x60, 0x01, 0x61, 0x01, 0x62, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0xf3, 0x00, 0x5a, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1b,
    0x01, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x74, 0x01, 0x75, 0x01, 0x76, 0x01, 0x77, 0x01, 0x7b, 0x00, 0x0a, 0x01, 0x5a, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1a, 0x01, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00,
    0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x88, 0x01, 0x89, 0x01, 0x8a, 0x01, 0x10, 0x10, 0x00, 0x00, 0x32, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf4, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b,
    0x00, 0x7b, 0x00, 0x7b, 0x00, 0xf0, 0x00, 0x05, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x06, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0xf1, 0x00, 0x8b, 0x01,
    0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf2, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x05, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7c, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0xf6, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x10, 0x10, 0xf1, 0x00, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x06, 0x01,
    0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x8c, 0x00, 0x0a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b,
    0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x6f, 0x01, 0x0a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1d, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x10, 0x10, 0x7b, 0x00, 0xf0, 0x00,
    0x05, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf2, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x36, 0x01, 0x37, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x68, 0x01, 0x69, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x69, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x08, 0x01,
    0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00
};

static std::vector<unsigned char> mCampaignSelectMap_AF = {
    0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5b, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x3d, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72,
    0x00, 0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x29, 0x00, 0x14, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x83, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x72, 0x00, 0x71, 0x00, 0x14, 0x00, 0x14, 0x00, 0x9a, 0x00, 0x9b, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5d, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x0c, 
    0x00, 0xae, 0x00, 0xaf, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5c, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x14, 0x00, 0x0b, 0x00, 0xc2, 0x00, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5c, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x28, 0x00, 0x14, 0x00, 0x14, 0x00, 0x07, 0x00, 0x07, 0x00, 0x04, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 
    0x00, 0x14, 0x00, 0x70, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x94, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x97, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x56, 0x00, 0x3d, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x52, 0x00, 0x53, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 
    0x00, 0x59, 0x00, 0x14, 0x10, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x14, 0x10, 0x14, 0x00, 
    0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 
    0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x73, 0x00, 0x73, 0x00, 0x73, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x80, 0x00, 0x14, 0x00, 0x0c, 0x00, 0x11, 0x00, 0x11, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x14, 0x00, 0x82, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x71, 0x00, 0x14, 0x00, 0x18, 0x00, 0x23, 0x00, 0x24, 0x00, 0x16, 0x00, 0x14, 0x00, 0x14, 0x00, 0x96, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 
    0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x94, 0x00, 0x14, 0x00, 0x0b, 0x00, 0x07, 0x00, 0x07, 0x00, 0x04, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x64, 0x00, 0x64, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
    0x10, 0x10, 0x0b, 0x00, 0x10, 0x10, 0x07, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
};

void cFodder::Campaign_Select_Setup() {
	mCampaignList.clear();

	mPhase_In_Progress = true;
	mPhase_Aborted = false;
	mGUI_SaveLoadAction = 0;

	{
		for (auto& Name : mVersions->GetCampaignNames()) {

			if (g_ResourceMan->isCampaignAvailable(Name) || Name == "Single Map" || Name == "Random Map")
				mCampaignList.push_back(Name);
		}
	}

	{
		auto Files = g_ResourceMan->GetCampaigns();

		// Append all custom campaigns to the list
		for (auto& File : Files) {
			size_t Pos = File.find_first_of(".");
			std::string FileName = File.substr(0, Pos);

			// Don't add known campaigns
			if (mVersions->isCampaignKnown(FileName))
				continue;

			mCampaignList.push_back(FileName);
		}
	}

	mGUI_Select_File_CurrentIndex = 0;
	mGUI_Select_File_Count = (int16)mCampaignList.size();



	// Create the title screen depending on which data is loaded
	if (mVersionCurrent->isRetail()) {
		sMapParams Params(0x15, 0x0F, eTileTypes_Jungle, eTileSub_0);
		Map_Create(Params);
		std::memcpy(mMap->data() + 0x60, mCampaignSelectMap_Jungle.data(), mMap->size() - 0x60);
	}
	else {
		sMapParams Params(0x15, 0x0F, eTileTypes_AFX, eTileSub_0);
		Map_Create(Params);
		std::memcpy(mMap->data() + 0x60, mCampaignSelectMap_AF.data(), mMap->size() - 0x60);
	}

	Campaign_Select_Sprite_Prepare();

	if (mGUI_SaveLoadAction != 3) {
		mSurface->palette_FadeTowardNew();
		Mouse_Setup();
	}
	mGUI_SaveLoadAction = 0;

	mGraphics->PaletteSet();
	mSurface->Save();

	mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
	mDemo_ExitMenu = 0;

	Camera_Reset();
}

std::string cFodder::Campaign_Select_File(const char* pTitle, const char* pSubTitle, const char* pPath, const char* pType, eDataType pData) {

	Campaign_Select_Setup();

    do {

        Campaign_Select_File_Loop(pTitle, pSubTitle);

    } while (mGUI_SaveLoadAction == 3);

    mPhase_In_Progress = false;

    if (mGUI_SaveLoadAction == 1)
        return "";

	return mCampaignList[mGUI_Select_File_CurrentIndex + mGUI_Select_File_SelectedFileIndex];
}



void cFodder::Campaign_Selection() {
    mPhase_Complete = false;

    Phase_EngineReset();
    GameData_Reset();
    mMap_Destroy_Tiles.clear();

    Image_FadeOut();

    mGraphics->PaletteSet();

    mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    mMouseX_Offset = -8;
    mMouseY_Offset = -8;

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    mCustom_Mode = eCustomMode_None;

    std::string CampaignFile = Campaign_Select_File("OPEN FODDER", "SELECT CAMPAIGN", "", "*.ofc", eDataType::eCampaign);

    // Exit Pressed?
    if (mGUI_SaveLoadAction == 1 || mGUI_SaveLoadAction == 4 || !CampaignFile.size()) {

        // Return to custom menu
        mDemo_ExitMenu = 1;
        mCustom_Mode = eCustomMode_None;

        return;
    }

    // Find a data version to use with this campaign
    // If no version is found, we use the currently loaded one
    {
        const sGameVersion* Version = mVersions->GetForCampaign(CampaignFile, mVersionCurrent->mPlatform);

        if (!Version) {
            Version = mVersions->GetForCampaign(CampaignFile);
        }

        // Load a new version?
        if (Version && Version != mVersionCurrent) {
            VersionSwitch(Version);
        }

        // Set the default/starting version
        mVersionDefault = mVersionCurrent;

        // Single Map Mode?
        if (CampaignFile == "Single Map" || CampaignFile == "Random Map") {
            
            mGame_Data.mCampaign.SetSingleMapCampaign();
            mCustom_Mode = eCustomMode_Map;
            return;

            // If no version, it must be a custom campaign
        }
        else if (!Version) {

			// If version is currently XMAS, it means no retail is available
			if (mVersionCurrent->isAmigaXmas()) {

				VersionSwitch(mVersions->GetForCampaign("Amiga Action"));

				// Set the default/starting version
				mVersionDefault = mVersions->GetForCampaign("Amiga Action");
			}

            mCustom_Mode = eCustomMode_Set;
        }
    }

    // Load the campaign
    if (Campaign_Load(CampaignFile) == true) {

        // 
        if (mVersionCurrent->isCustom()) {
            mDemo_ExitMenu = 1;
            mCustom_ExitMenu = 1;
        }

        WindowTitleBaseSetup();
        return;
    }

    mDemo_ExitMenu = 1;
    mCustom_Mode = eCustomMode_None;
}

void cFodder::Campaign_Select_Sprite_Prepare() {

    int16 x = 0;

    Sprite_Clear_All();

    Phase_SquadPrepare();

    mSquad_CurrentVehicle = &mSprites[x];
    mSprites[x].field_0 = 0xe6;
    mSprites[x].field_4 = 0xcd;
    mSprites[x].field_8 = 0xD2;
    mSprites[x].field_A = 5;
    mSprites[x].field_52 = 0;
    mSprites[x].field_20 = 0;
    mSprites[x].field_18 = eSprite_Turret_Missile_Human;
    mSprites[x++].field_6F = eVehicle_Turret_Missile;

    mSprites[x].field_0 = tool_RandomGet() & 0xFF;
    mSprites[x].field_4 = tool_RandomGet() & 0xff;
    mSprites[x].field_8 = 6;
    mSprites[x].field_A = 0;
    mSprites[x].field_52 = 0;
    mSprites[x].field_20 = 0;
    mSprites[x++].field_18 = eSprite_Civilian_Spear;

    mSprites[x].field_0 = 0xff;
    mSprites[x].field_4 = 16 + (tool_RandomGet() % 0x60);
    mSprites[x].field_8 = 2;
    mSprites[x].field_A = 0;
    mSprites[x].field_52 = 0;
    mSprites[x].field_20 = 0;
    mSprites[x++].field_18 = eSprite_Bird_Left;

    mSprites[x].field_0 = 0;
    mSprites[x].field_4 = 16 + (tool_RandomGet() % 0xc0);
    mSprites[x].field_8 = 2;
    mSprites[x].field_A = 0;
    mSprites[x].field_52 = 0;
    mSprites[x].field_20 = 0;
    mSprites[x++].field_18 = eSprite_Bird_Left;

    mSprites[x].field_0 = 185;
    mSprites[x].field_4 = 19;
    mSprites[x].field_8 = 2;
    mSprites[x].field_A = 0;
    mSprites[x].field_52 = 0;
    mSprites[x].field_20 = 0;
    mSprites[x++].field_18 = eSprite_Hostage;

    mSprites[x].field_0 = 100;
    mSprites[x].field_4 = 0xd0;
    mSprites[x].field_8 = 2;
    mSprites[x].field_A = 0;
    mSprites[x].field_52 = 0;
    mSprites[x].field_20 = 0;
    mSprites[x++].field_18 = eSprite_Floating_Dead_Soldier;

    mSprites[x].field_0 = 220;
    mSprites[x].field_4 = 60;
    mSprites[x].field_8 = 2;
    mSprites[x].field_A = 0;
    mSprites[x].field_52 = 0;
    mSprites[x].field_20 = 0;
    mSprites[x++].field_18 = eSprite_BoilingPot;

    mSprites[x].field_0 = 220;
    mSprites[x].field_4 = 60;
    mSprites[x].field_8 = 2;
    mSprites[x].field_A = 0;
    mSprites[x].field_52 = 0;
    mSprites[x].field_20 = 0;
    mSprites[x++].field_18 = eSprite_Null;

    mSquad_Leader = mSquad_CurrentVehicle;

    word_3AA1D = word_3BED5[0];

    if(mMapLoaded->getTileType() == eTileTypes_Jungle)
        Map_Add_Structure(mStructuresBarracksWithSoldier[mMapLoaded->getTileType()], 4, 2);

    if(mMapLoaded->getTileType() == eTileTypes_AFX)
        Map_Add_Structure(mStructuresBarracksWithSoldier[mMapLoaded->getTileType()], 2, 5);
}

void cFodder::Campaign_Select_File_Cycle(const char* pTitle, const char* pSubTitle) {
	static int16 Timedown = 0;

	mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
	Sprite_Frame_Modifier_Update();
	Mission_Sprites_Handle();

	mSurface->clearBuffer();
	MapTiles_Draw();
	Sprites_Draw();

	Campaign_Select_DrawMenu(pTitle, pSubTitle);


	if (mSurface->isPaletteAdjusting())
		mSurface->palette_FadeTowardNew();

	Mouse_Inputs_Get();
	Mouse_DrawCursor();

	if (Timedown)
		--Timedown;

	if (mMouse_Button_Left_Toggle && !Timedown) {
		Vehicle_Input_Handle();

		mMouse_Button_Left_Toggle = 0;
		mSprites[0].field_57 = -1;
		mSprites[0].field_2E = mSquad_Leader->field_26 + 10;
		mSprites[0].field_30 = mSquad_Leader->field_28 - 18;

		Timedown = 10;
	}

	if (mPhase_Aborted)
		GUI_Button_Load_Exit();

	if (mMouse_Button_Left_Toggle)
		GUI_Handle_Element_Mouse_Check(mGUI_Elements);

	GUI_Button_Load_MouseWheel();
	Video_SurfaceRender();
	Cycle_End();
}

void cFodder::Campaign_Select_File_Loop(const char* pTitle, const char* pSubTitle) {


    do {
		Campaign_Select_File_Cycle(pTitle, pSubTitle);

    } while (mGUI_SaveLoadAction <= 0);

    mPhase_Aborted = false;

    if (mGUI_SaveLoadAction == 3)
        return;

    Image_FadeOut();
}

/**
 * Display a list of custom maps
 */
void cFodder::Custom_ShowMapSelection() {
    // If demo data is loaded, we need to enture a retail release is loaded for the menu draw data
    if (mVersionCurrent->isDemo())
        VersionSwitch(mVersionDefault);

    Image_FadeOut();
    mGraphics->PaletteSet();

	std::string File = mParams->mSingleMap;

	if (!File.size()) {
		auto Maps = g_ResourceMan->GetMaps();
		File = GUI_Select_File("SELECT MAP", {}, Maps);
		// Exit Pressed?
		if (mGUI_SaveLoadAction == 1 || !File.size()) {

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

bool cFodder::Demo_Amiga_ShowMenu() {
    mSound->Music_Stop();

    if(mVersionCurrent->isAmigaNotVeryFestive() || mVersionCurrent->isAmigaAlienLevels()) 
        mGraphics->Load_And_Draw_Image("VMENU", 32);
    else
        mGraphics->Load_And_Draw_Image("apmenu.lbm", 32);

    // Amiga Demos have a different cursor palette
    GetGraphics<cGraphics_Amiga>()->SetCursorPalette(0x10);
    mWindow->SetScreenSize(mVersionCurrent->GetSecondScreenSize());

    mGUI_Mouse_Modifier_Y = 4;

    Menu_Loop(
        [this]() {
        if (mButtonPressLeft) {
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

void cFodder::Sprite_Handle_Player_Enter_Vehicle(sSprite* pSprite) {
    // Have a target vehicle?
    if (!pSprite->field_66)
        return;

    sSprite* Vehicle = pSprite->field_66;

    int16 VehicleX = Vehicle->field_0 + 0x10;

    if (Vehicle->field_6F == eVehicle_Turret_Cannon || Vehicle->field_6F == eVehicle_Turret_Missile)
        VehicleX -= 0x0C;

    int16 VehicleY = Vehicle->field_4 - 9;
    int16 SoldierX = pSprite->field_0 + 4;
    int16 SoldierY = pSprite->field_4;

    // Is the sprite close enough to enter the vehicle?
    int16 Distance = 0x1F;
    if (Map_Get_Distance_BetweenPoints(VehicleX, VehicleY, SoldierX, Distance, SoldierY) >= 0x0D)
        return;

    // Enter the vehicle
    pSprite->field_6A_sprite = Vehicle;
    pSprite->field_6E = -1;
    sSprite* Data20 = 0;

    sub_305D5(Data20);

    // First member of current player squad
    sSprite* eax = mSquads[pSprite->field_32][0];

    // Is this the first member of the squad
    if (eax != pSprite)
        return;

    mSquad_EnteredVehicleTimer[pSprite->field_32] = 0x190;
}

void cFodder::Mouse_Cursor_Update() {
    int16 Data0, Data4, Data8;

    // Sidebar
    if (mMouseX < 0x10) {
        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
        return;
    }

    if (!mButtonPressRight) {

        Data0 = mMouseX + (mCameraX >> 16);
        Data4 = mMouseY + (mCameraY >> 16);

        Data0 -= 0x0F;
        Data4 -= 3;

        //loc_30B36
        for (auto& Vehicle : mSprites_HumanVehicles) {

            if (Vehicle->field_0 < 0)
                continue;

            if (!Vehicle->field_65)
                continue;

            // If not human
            if (Vehicle->field_22 != eSprite_PersonType_Human)
                continue;

            Data8 = Vehicle->field_0;
            if (Vehicle->field_6F == eVehicle_Turret_Cannon)
                goto loc_30BB9;
            if (Vehicle->field_6F == eVehicle_Turret_Missile)
                goto loc_30BBE;

        loc_30BB9:;
            Data8 -= 8;
        loc_30BBE:;

            if (Data0 < Data8)
                continue;

            Data8 += mSprite_Width[Vehicle->field_18];
            if (Data0 > Data8)
                continue;

            Data8 = Vehicle->field_4;
            Data8 -= Vehicle->field_20;
            Data8 -= mSprite_Height_Top[Vehicle->field_18];
            Data8 -= 0x14;

            if (Data4 < Data8)
                continue;

            Data8 = Vehicle->field_4;
            Data8 -= Vehicle->field_20;
            if (Data4 > Data8)
                continue;

            mMouseSetToCursor = -1;

            // Is vehicle off ground
            if (Vehicle->field_20) {

                // And is current
                if (Vehicle != mSquad_CurrentVehicle)
                    return;

                if (Vehicle->field_8 == 0xA5)
                    return;

                // Show the helicopter land icon
                mMouseSpriteNew = eSprite_pStuff_Mouse_Helicopter;
                mMouseX_Offset = 0;
                mMouseY_Offset = 0;
                return;
            }

            // Is this the vehicle the current squad is in
            if (Vehicle == mSquad_CurrentVehicle)
                mMouseSpriteNew = eSprite_pStuff_Mouse_Arrow_DownRight;
            else
                mMouseSpriteNew = eSprite_pStuff_Mouse_Arrow_UpLeft;

            mMouseX_Offset = 0;
            mMouseY_Offset = 0;
            return;
        }
    }

    if (mMouseSetToCursor) {
        mMouseSetToCursor = 0;
        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
    }
}

void cFodder::sub_22C87(sSprite* pSprite) {

    int32 Data0 = pSprite->field_1A;

    pSprite->field_1E_Big += Data0;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
        Data0 = -Data0;
        Data0 >>= 1;
    }

    Data0 -= 0x20000;
    pSprite->field_1A = (int32)Data0;
}

void cFodder::sub_22CD7(sSprite* pSprite, int16& pData0, int16& pData4) {

    int16 Data1C;

    pSprite->field_3C = pSprite->field_10;

    Sprite_Direction_Between_Points(pSprite, pData0, pData4);

    mSprite_Field10_Saved = pSprite->field_10;
    pSprite->field_3E = 0;

    Sprite_SetDirectionMod(pSprite);

    pSprite->field_10 = pSprite->field_3C;

    pData0 = word_3B2F7;
    pData0 -= pSprite->field_36;

    pData0 *= mDirectionMod;
    int16 Data8 = 0;

    pData4 = pData0;
    if (pData4 < 0) {
        Data8 = -1;
        pData4 = -pData4;
    }

    if (pData4 > word_3B25D) {
        pData0 = word_3B25D;

        if (Data8 < 0)
            pData0 = -pData0;
    }
    //loc_22D7A
    pSprite->field_10 += pData0;
    pSprite->field_10 &= 0x1FE;

    if (pSprite->field_6F == eVehicle_Jeep)
        goto loc_22DC4;

    if (pSprite->field_6F == eVehicle_JeepRocket)
        goto loc_22DC4;

    if (pSprite->field_6F == eVehicle_Tank)
        goto loc_22DCF;

    if (pSprite->field_36 <= 6)
        return;

    if (!(mMission_EngineTicks & 1))
        return;

loc_22DC4:;
    if (pSprite->field_36 <= 8)
        return;

loc_22DCF:;
    pData0 = -pData0;
    if (!pData0) {
        Data1C = 0;
    }
    else if (pData0 >= 0) {
        Data1C = 1;
    }
    else {
        Data1C = -1;
    }

    Sprite_Vehicle_Direction_Update(pSprite, Data1C);
}

void cFodder::Sprite_Handle_Vehicle_Terrain_Check(sSprite* pSprite) {
    int16 Data4, Data0;
    int32 Dataa0;

    pSprite->field_50 = pSprite->field_52;
    if (!pSprite->field_50)
        pSprite->field_52 = 0;

    if (Map_Sprite_Check_Around_Position(pSprite))
        goto Computer_Vehicle_SoftTerrain;

    Data4 = 0x0F;
    Data0 = -10;
    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);

    pSprite->field_60 = static_cast<int8>(Data4);
    if (Data4 == eTerrainFeature_Rocky || Data4 == eTerrainFeature_Rocky2)
        goto loc_23056;

    if (Data4 == eTerrainFeature_Jump)
        goto loc_23100;

    if (Data4 == eTerrainFeature_Block)
        goto Computer_Vehicle_SoftTerrain;

    if (Data4 == eTerrainFeature_Drop || Data4 == 0x0A)
        goto loc_22F06;

    if (pSprite->field_56)
        pSprite->field_38 = eSprite_Anim_Die1;

    if (Data4 == eTerrainFeature_Snow)
        goto loc_22FA3;

    if (pSprite->field_22 == eSprite_PersonType_Human)
        goto Human_Vehicle;

    if (Data4 == eTerrainFeature_QuickSand || Data4 == eTerrainFeature_WaterEdge
        || Data4 == eTerrainFeature_Water || Data4 == eTerrainFeature_Sink)
        goto Computer_Vehicle_SoftTerrain;

    goto loc_22EEB;

Human_Vehicle:;

    if (Data4 == eTerrainFeature_QuickSand)
        goto Human_Vehicle_Quicksand;

    if (Data4 == eTerrainFeature_WaterEdge)
        goto Human_Vehicle_WaterEdge;

    if (Data4 == eTerrainFeature_Water || Data4 == eTerrainFeature_Sink)
        goto AnimDie3;

loc_22EEB:;
    pSprite->field_52 = 0;
    if (pSprite->field_50)
        goto loc_23056;

    return;

loc_22F06:;
    pSprite->field_56 += 4;
    Data0 = pSprite->field_56;
    pSprite->field_4 += Data0;
    return;

Computer_Vehicle_SoftTerrain:;

    if (pSprite->field_22 != eSprite_PersonType_Human) {
        pSprite->field_26 = pSprite->field_0;
        pSprite->field_28 = pSprite->field_4;
    }
    //loc_22F5F
    pSprite->field_36 = -pSprite->field_36;
    pSprite->field_36 >>= 1;
    Sprite_XY_Restore(pSprite);

    Data4 = 0x0F;
    Data0 = -10;
    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (Data4 == eTerrainFeature_Block)
        pSprite->field_38 = eSprite_Anim_Die1;

    Sprite_Animation_SlideOrDie(pSprite);
    return;

loc_22FA3:;

    if (!pSprite->field_36)
        return;

    pSprite->field_20 ^= 1;
    if (!pSprite->field_20)
        return;

    if (pSprite->field_36 <= 0x10)
        return;

    pSprite->field_36 -= 2;
    return;

Human_Vehicle_Quicksand:;
    pSprite->field_52 = 1;
    return;

Human_Vehicle_WaterEdge:;
    pSprite->field_52 = 3;
    return;

AnimDie3:;
    if (pSprite->field_56)
        pSprite->field_38 = eSprite_Anim_None;
    dword_3B24B = -1;

    Sprite_Handle_Vehicle_Sinking(pSprite);
    if (pSprite->field_52 < 0x50)
        goto loc_23033;

    pSprite->field_52 = 0x1E;
    pSprite->field_38 = eSprite_Anim_Die3;
    return;

loc_23033:;
    pSprite->field_52 += 1;
    pSprite->field_36 -= 0x0C;
    if (pSprite->field_36 >= 0)
        return;

    pSprite->field_36 = 0;
    return;

loc_23056:;
    if (pSprite->field_20)
        return;

    if (pSprite->field_36 >= 0x14)
        goto loc_230B0;

    if (!pSprite->field_36)
        return;

    Data0 = mGame_InputTicks;
    Data0 &= 1;
    Data0 += 3;
    pSprite->field_20 = Data0;
    Data0 = -Data0;
    pSprite->field_1A = (((int64)pSprite->field_1A & 0xFFFF) | Data0 << 16);
    return;

loc_230B0:;
    // Not Moving?
    if (!pSprite->field_36)
        return;

    pSprite->field_20 = 1;
    Dataa0 = pSprite->field_36 << 16;
    Dataa0 >>= 3;
    pSprite->field_1A = Dataa0;
    return;

loc_23100:;
    if (!pSprite->field_36)
        return;

    pSprite->field_20 = 1;
    Dataa0 = pSprite->field_36 << 16;
    Dataa0 >>= 2;
    pSprite->field_1A = Dataa0;
    pSprite->field_36 <<= 1;
    pSprite->field_36 <<= 1;

}

void cFodder::Sprite_Under_Vehicle(sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14, int16 pData18, int16 pData1C) {

    if (mPhase_Finished)
        return;

    sSprite* Sprite = mSprites.data();

	// TODO: Fix counter
    for (int16 Count = 0x1D; Count >= 0; --Count, ++Sprite) {
        if (Sprite->field_0 == -32768)
            continue;

        if (Sprite == pSprite)
            continue;

        if (!mSprite_Can_Be_RunOver[Sprite->field_18])
            continue;

        if (pSprite->field_22 == Sprite->field_22)
            continue;

        if (Sprite->field_18 == eSprite_Hostage)
            continue;

        if (Sprite->field_18 == eSprite_Enemy_Leader)
            continue;

        if (pData8 > Sprite->field_0)
            continue;

        if (pDataC < Sprite->field_0)
            continue;

        if (pData10 > Sprite->field_4)
            continue;

        if (pData14 < Sprite->field_4)
            continue;

        int16 Data0 = Sprite->field_20;
        if (pData1C < Data0)
            continue;
        Data0 += 0x0E;
        if (pData18 > Data0)
            continue;

        if (Sprite->field_18 != eSprite_Player) {
            if (Sprite->field_6E)
                continue;
        }

        // Run Over
        Sprite->field_38 = eSprite_Anim_Die1;
        if (!Sprite->field_65)
            return;

        pSprite->field_38 = eSprite_Anim_Die1;
        return;
    }
}

int16 cFodder::Sprite_Animation_SlideOrDie(sSprite* pSprite) {
    sSprite* Data24 = 0;

    // Die animation running?
    if (pSprite->field_38 == eSprite_Anim_Die1 || pSprite->field_38 == eSprite_Anim_Die3) {
        pSprite->field_18 = eSprite_Explosion;
        pSprite->field_26 = 0x1F56;
        pSprite->field_28 = -9;
        Data24 = pSprite + 1;

        Sprite_Destroy(Data24);
        if (pSprite->field_22 == eSprite_PersonType_Human)
            return -1;

        Data24 = pSprite + 2;
        Sprite_Destroy(Data24);
        return -1;
    }

    // Slide animation running?
    if (pSprite->field_38 == eSprite_Anim_Slide1) {

        pSprite->field_36 = -pSprite->field_36;
        pSprite->field_38 = eSprite_Anim_Slide2;

    }
    else if (pSprite->field_38 == eSprite_Anim_Slide2) {
        pSprite->field_36 += 6;

        if (pSprite->field_36 >= 0) {
            pSprite->field_38 = eSprite_Anim_None;
            pSprite->field_36 = 0;
        }

    }
    else {
        return 0;
    }

    Sprite_Movement_Calculate(pSprite);
    sub_243E9(pSprite);
    return -1;
}

int16 cFodder::Map_Sprite_Check_Around_Position(sSprite* pSprite) {
    int16 Data4 = 0x0A;
    int16 Data0 = -13;

    int16 ax = Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (ax)
        return ax;

    Data4 = 0x14;
    Data0 = -13;
    ax = Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (ax)
        return ax;

    Data4 = 0x0A;
    Data0 = -7;
    ax = Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (ax)
        return ax;

    Data4 = 0x14;
    Data0 = -7;
    ax = Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (ax)
        return ax;

    return 0;
}

void cFodder::Sprite_Handle_Vehicle_Human(sSprite* pSprite) {

    if (pSprite->field_38) {

        pSprite->field_22 = eSprite_PersonType_Human;
        Sprite_Handle_Vehicle(pSprite);
        pSprite->field_22 = eSprite_PersonType_Human;
        return;
    }

    if (pSprite->field_6F == eVehicle_JeepRocket)
        sub_245BF(pSprite);

    mSprite_Helicopter_DestroyLight = 0;
    pSprite->field_22 = eSprite_PersonType_Human;
    Sprite_Handle_Vehicle(pSprite);
    pSprite->field_22 = eSprite_PersonType_Human;
    mSprite_Helicopter_DestroyLight = 0;

    sSprite* Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;
}

int16 cFodder::Sprite_Create_Smoke(sSprite* pSprite, sSprite*& pData2C) {
    if (!pSprite->field_5C)
        return -1;

    int16 Data0 = 1;
    sSprite* Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return -1;

    pData2C->field_0 = pSprite->field_0;
    pData2C->field_2 = pSprite->field_2;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_6 = pSprite->field_6;

    if (dword_3B24B) {
        Data0 = tool_RandomGet() & 0x1F;
        Data0 -= 8;
        pData2C->field_0 += Data0;
        Data0 >>= 4;
        Data0 &= 0x0F;
        Data0 -= 8;
        pData2C->field_4 -= Data0;
        dword_3B24B = 0;
    }

    pData2C->field_20 = pSprite->field_20;
    pData2C->field_52 = 0;
    pData2C->field_8 = 0xCC;
    pData2C->field_A = 4;
    pData2C->field_2C = eSprite_Draw_OnTop;
    pData2C->field_18 = eSprite_Smoke;
    return 0;
}

void cFodder::sub_23525(sSprite* pSprite) {
    int16 Data0 = 2;
    int16 Data4 = pSprite->field_36;
    int16 Data8, DataC, Data10, Data14, Data18, Data1C;
    sSprite* Data2C = 0;

    if (!Data4) {
        if (pSprite->field_22 == eSprite_PersonType_Human) {
            if (pSprite != mSquad_CurrentVehicle)
                goto loc_2356B;
        }
    }

    Data4 += 0x10;
    if (Data4 > 0x20)
        Data4 = 0x20;

    Sprite_Map_Sound_Play(Data0);

loc_2356B:;
    pSprite->field_6F = eVehicle_Tank;
    pSprite->field_65 = -1;
    if (Sprite_Animation_SlideOrDie(pSprite))
        return;

    pSprite->field_8 = 0xD1;
    word_3B2F7 = pSprite->field_36 + 0x20;

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;

    sub_22CD7(pSprite, Data0, Data4);
    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;

    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 >= 0x1E)
        goto loc_2361A;

    if (Data0 >= pSprite->field_36)
        goto loc_23680;

    pSprite->field_36 = Data0;
    goto loc_23680;

loc_2361A:;
    dword_3B24B = -1;
    if (!Sprite_Create_Smoke(pSprite, Data2C))
        Data2C->field_2C = eSprite_Draw_First;

    if (!pSprite->field_36) {
        if (mDirectionMod) {
            pSprite->field_36 -= 4;
            goto loc_23680;
        }
    }

    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (!pSprite->field_2A) {
        ++pSprite->field_36;

        if (pSprite->field_36 >= 0x18)
            pSprite->field_36 = 0x18;
    }

loc_23680:;

    Sprite_XY_Store(pSprite);
    Sprite_Movement_Calculate(pSprite);
    Sprite_Handle_Vehicle_Terrain_Check(pSprite);

    pSprite->field_20 = 0;
    sub_236F7(pSprite);

    Data8 = pSprite->field_0;
    DataC = pSprite->field_0 + 0x1E;
    Data10 = pSprite->field_4 - 0x14;
    Data14 = pSprite->field_4;
    Data18 = pSprite->field_20;
    Data1C = Data18 + 0x0E;

    Sprite_Under_Vehicle(pSprite, Data8, DataC, Data10, Data14, Data18, Data1C);
}

void cFodder::sub_236F7(sSprite* pSprite) {
    sSprite* Data24 = pSprite + 1;
    int16 Data0, Data4, Data8, DataC;

    Data24->field_8 = 0xD2;
    Data24->field_0 = pSprite->field_0 + 0x0A;
    Data24->field_4 = pSprite->field_4 + 1;

    Data0 = pSprite->field_52;
    Data24->field_4 += Data0;
    Data24->field_20 = pSprite->field_20;
    Data24->field_20 += 9;
    Data0 = Data24->field_0 + 8;
    Data4 = Data24->field_4 - 5;

    Data4 -= Data24->field_20;
    Data24->field_52 = pSprite->field_52;

    if (pSprite->field_22 == eSprite_PersonType_Human)
        goto loc_23815;

    Data8 = pSprite->field_2E;
    if (Data8 >= 0)
        goto loc_23806;

    Data0 = mMission_EngineTicks & 0x0F;
    if (Data0)
        return;

    Data0 = tool_RandomGet() & 1;
    if (!Data0)
        Data0 -= 1;

    Data24->field_A += Data0;
    Data24->field_A &= 0x0F;
    return;

loc_23806:;
    DataC = pSprite->field_30;
    goto loc_23843;

loc_23815:;
    if (pSprite != mSquad_CurrentVehicle)
        return;

    Data8 = mMouseX + (mCameraX >> 16);
    Data8 -= 0x10;

    DataC = mMouseY + (mCameraY >> 16);
loc_23843:;
    Direction_Between_Points(Data0, Data4, Data8, DataC);

    Data4 -= 0x10;
    Data4 >>= 5;
    Data4 += 8;
    Data4 ^= 0x0F;
    Data4 &= 0x0F;
    Data24->field_A = Data4;
}

void cFodder::Sprite_Handle_Tank_FireMissile(sSprite* pSprite) {
    if (!pSprite->field_57)
        if (!pSprite->field_54)
            return;

    pSprite->field_54 = 0;
    sSprite* Data24 = pSprite + 1;

    Data24->field_2E = pSprite->field_2E;
    Data24->field_30 = pSprite->field_30;

    int16 Data24_Field_0 = Data24->field_0;
    int16 Data24_Field_4 = Data24->field_4;
    int16 Data0 = Data24->field_A << 1;

    Data24->field_0 += mSprite_Turret_Positions[Data0];
    Data24->field_4 += mSprite_Turret_Positions[Data0 + 1];

    mSprite_Missile_LaunchDistance_X = 0;
    mSprite_Missile_LaunchDistance_Y = 0;
    sSprite* Data2C = 0;

    if (!Sprite_Create_Missile(Data24, Data2C)) {
        Data24->field_4 = Data24_Field_4;
        Data24->field_0 = Data24_Field_0;
        pSprite->field_57 = -1;
        return;
    }

    Data2C->field_20 += 0x11;
    Data2C->field_36 = 0x3C;
    Sound_Play(Data24, 5, 0x1E);
    Sprite_Create_Smoke(Data24, Data2C);

    Data24->field_4 = Data24_Field_4;
    Data24->field_0 = Data24_Field_0;
    pSprite->field_57 = 0;
}

int16 cFodder::Sprite_Create_Missile(sSprite* pSprite, sSprite*& pData2C) {
    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Missile_Projectile_Counters[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    int16 Data0 = 2;
    sSprite* Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return 0;

    ++mSprite_Missile_Projectile_Counters[pSprite->field_22];
    Data30 = pData2C + 1;

    pData2C->field_0 = pSprite->field_0;
    pData2C->field_2 = pSprite->field_2;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_6 = pSprite->field_6;

    Data30->field_0 = pSprite->field_0;
    Data30->field_2 = pSprite->field_2;
    Data30->field_4 = pSprite->field_4;
    Data30->field_6 = pSprite->field_6;

    pData2C->field_0 += mSprite_Missile_LaunchDistance_X;
    Data30->field_0 += mSprite_Missile_LaunchDistance_X;
    if (Data30->field_0 < 0)
        return 0;

    pData2C->field_4 += mSprite_Missile_LaunchDistance_Y;
    Data30->field_4 += mSprite_Missile_LaunchDistance_Y;

    if (Data30->field_4 < 0)
        return 0;

    pData2C->field_32 = 0;
    if (pSprite->field_6F == eVehicle_Turret_Cannon)
        pData2C->field_32 = -1;

    pData2C->field_1E = pSprite->field_1E;
    pData2C->field_20 = pSprite->field_20;
    pData2C->field_20 -= 0x0D;
    pData2C->field_26 = pSprite->field_2E;
    pData2C->field_28 = pSprite->field_30;
    pData2C->field_28 += 0x10;

    if (pSprite->field_22 != eSprite_PersonType_Human) {
        Sprite_Enemy_Set_Target(pData2C);
    }
    else {
        pData2C->field_26 -= 1;
        pData2C->field_28 += 3;
    }

    pData2C->field_8 = 0xA3;
    Data30->field_8 = 0x7E;
    pData2C->field_A = 0;
    Data30->field_A = 1;
    pData2C->field_36 = pSprite->field_36;
    Data30->field_36 = pSprite->field_36;
    pData2C->field_3A = 1;
    Data30->field_1E = 0;
    Data30->field_20 = 0;
    pData2C->field_4 += 1;
    pData2C->field_0 += 3;
    pData2C->field_18 = eSprite_Missile;
    Data30->field_18 = eSprite_ShadowSmall;
    pData2C->field_52 = 0;
    Data30->field_52 = 0;
    pData2C->field_22 = pSprite->field_22;
    Data30->field_22 = pSprite->field_22;
    pData2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    pData2C->field_38 = eSprite_Anim_None;

    // HACK: Disable sound for Amiga Coverdisks (it crashes)
    if (!mVersionCurrent->isCoverDisk())
        Sound_Play(pSprite, eSound_Effect_Missile_Launch, 0x0F);

    return -1;
}

void cFodder::Sprite_Enemy_Set_Target(sSprite* pData2C) {

    int16 Data0 = tool_RandomGet();
    int16 Data4 = Data0;

    // Bigger X movement range for low aggression
    if (mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage < 5)
        Data0 &= 0x3F;
    else
        Data0 &= 0x1F;

    if (Data4 < 0)
        Data0 = -Data0;

    pData2C->field_26 += Data0;

    Data0 = tool_RandomGet();
    Data4 = Data0;

    // Bigger Y movement range for low aggression
    if (mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage < 5)
        Data0 &= 0x3F;
    else
        Data0 &= 0x1F;
    if (Data4 < 0)
        Data0 = -Data0;

    pData2C->field_28 += Data0;
}

void cFodder::Sprite_Handle_Helicopter_Human(sSprite* pSprite) {

    if (pSprite->field_38) {

        pSprite->field_22 = eSprite_PersonType_Human;
        Sprite_Handle_Helicopter(pSprite);
        pSprite->field_22 = eSprite_PersonType_Human;

        return;
    }

    Sprite_Handle_Helicopter_Human_Deploy_Weapon(pSprite);

    mSprite_Helicopter_DestroyLight = 0;
    pSprite->field_22 = eSprite_PersonType_Human;
    Sprite_Handle_Helicopter(pSprite);
    pSprite->field_22 = eSprite_PersonType_Human;
    mSprite_Helicopter_DestroyLight = 0;

    sSprite* Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;
    Data24 = pSprite + 2;
    Data24->field_18 = eSprite_Null;
}

int16 cFodder::Sprite_Handle_Helicopter_Terrain_Check(sSprite* pSprite) {
    int16 Data4 = 0x0F;
    int16 Data0 = -10;

    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    switch (Data4) {
    case eTerrainFeature_Rocky:
    case eTerrainFeature_QuickSand:
    case eTerrainFeature_WaterEdge:
    case eTerrainFeature_Water:
    case eTerrainFeature_Sink:
        Data0 = 0x0C;
        break;

    case eTerrainFeature_Block:
        Data0 = 0x14;
        break;
    case eTerrainFeature_Rocky2:
    case eTerrainFeature_Drop:
    case eTerrainFeature_Drop2:
        Data0 = 0x0E;
        break;

    default:
        return 0;   // Correct?
    }

    Data4 = pSprite->field_20;
    if (Data4 >= Data0)
        return 0;

    ++pSprite->field_20;
    return -1;
}

void cFodder::sub_23E01(sSprite* pSprite, int16& pData0, int16& pData4) {
    if (pSprite->field_44) {
        pSprite->field_44 -= 1;

        if (pSprite->field_44)
            goto loc_23E2F;
    }

    pSprite->field_3C = pSprite->field_10;
    Sprite_Direction_Between_Points(pSprite, pData0, pData4);

loc_23E2F:;
    Sprite_SetDirectionMod(pSprite);

    pSprite->field_10 = pSprite->field_3C;
    pData0 = mDirectionMod;
    pData0 <<= 3;

    if (pSprite->field_36 > 0x18)
        pData0 <<= 1;
    pSprite->field_10 = pData0;
    pSprite->field_10 &= 0x1FE;

    Sprite_Set_Direction_To_Next(pSprite);

    int16 Data8 = pSprite->field_3C;
    if (!pSprite->field_36)
        return;

    pSprite->field_A = Data8 >> 1;
}

void cFodder::Sprites_HumanVehicles_Remove(sSprite* pSprite) {

    auto it = remove_if(mSprites_HumanVehicles.begin(), mSprites_HumanVehicles.end(), 
        [pSprite](sSprite* pSpriteFind) { 
            return pSprite == pSpriteFind; 
    });

    mSprites_HumanVehicles.erase(it, mSprites_HumanVehicles.end());
}

void cFodder::Sprite_Handle_Turret(sSprite* pSprite) {
    sSprite* Data28 = 0;
    sSprite* Data34 = 0;
    int16 Data0, Data4, Data8, DataC, Data10;

    // Turrets in Moors / Interior can't be destroyed
    if (mVersionCurrent->isCannonFodder1()) {
        if (mMapLoaded->getTileType() == eTileTypes_Moors || mMapLoaded->getTileType() == eTileTypes_Int) {

            if (pSprite->field_38 == eSprite_Anim_Die1)
                pSprite->field_38 = eSprite_Anim_None;
        }
    }

    if (pSprite->field_38 == eSprite_Anim_Die3 || pSprite->field_38 == eSprite_Anim_Die1) {

        pSprite->field_18 = eSprite_Explosion;
        pSprite->field_26 = 0x1F45;
        pSprite->field_28 = -2;
        if (pSprite->field_22 == eSprite_PersonType_Human)
            return;

        Sprite_Destroy(pSprite + 1);
        return;
    }

    //loc_23F70
    pSprite->field_8 = 0xD2;
    pSprite->field_65 = -1;
    if (pSprite->field_22 == eSprite_PersonType_Human)
        goto loc_24075;

    mTurretFires_HomingMissile = 0;
    Data0 = eSprite_Player;
    Data4 = -1;
    Data8 = -1;
    DataC = -1;
    Data10 = -1;

    if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Get_Civilian_Home - 1]) {
        Data4 = eSprite_Civilian;
        Data8 = eSprite_Civilian2;
        DataC = eSprite_Civilian_Spear;
        Data10 = -1;
    }

    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        goto loc_240F3;

    if (mSprite_Find_Distance >= 0x28) {

        Data0 = tool_RandomGet();
        if (mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage < 5)
            Data0 &= 0x3F;
        else
            Data0 &= 0x1F;

        if (Data0 == 4)
            pSprite->field_54 = -1;
    }

    // Fire a homing missile
    mTurretFires_HomingMissile = -1;
    Data34 = Data28;

    Data0 = pSprite->field_0 + 8;
    Data4 = pSprite->field_4 - 5;
    Data8 = Data28->field_0;
    DataC = Data28->field_4;

    pSprite->field_2E = Data8;
    pSprite->field_30 = DataC;

    word_3B4ED[1] = Data28->field_20;
    goto loc_240C8;

loc_24075:;
    if (pSprite != mSquad_CurrentVehicle)
        return;

    Data0 = pSprite->field_0 + 8;
    Data4 = pSprite->field_4 - 5;
    Data8 = mMouseX + (mCameraX >> 16);
    Data8 -= 0x10;
    DataC = mMouseY + (mCameraY >> 16);

loc_240C8:;
    Direction_Between_Points(Data0, Data4, Data8, DataC);
    Data4 -= 0x10;
    Data4 >>= 5;
    Data4 += 8;
    Data4 ^= 0x0F;
    Data4 &= 0x0F;

    pSprite->field_A = Data4;
loc_240F3:;

    Sprite_Handle_Turret_Fire(pSprite, Data34);

    if (pSprite->field_22 == eSprite_PersonType_Human)
        return;

    sSprite* Data24 = pSprite + 1;

    Data24->field_18 = eSprite_Flashing_Light;
    Data4 = pSprite->field_0;
    Data4 += 5;
    Data24->field_0 = Data4;
    Data4 = pSprite->field_4;
    Data4 -= 0x0F;
    Data4 -= pSprite->field_52;
    Data24->field_4 = Data4;
    Data24->field_20 = pSprite->field_20;
}

void cFodder::Sprite_Handle_Turret_Fire(sSprite* pSprite, sSprite* pData34) {
    int16 ax;

    sSprite* Data2C = 0;

    if (!pSprite->field_57) {
        if (!pSprite->field_54)
            return;
    }

    pSprite->field_54 = 0;
    int16 Field_0 = pSprite->field_0;
    int16 Field_4 = pSprite->field_4;

    int16 Data0 = pSprite->field_A;
    Data0 <<= 1;

    int16 Data4 = mSprite_Turret_Positions[Data0];
    pSprite->field_0 += Data4;

    Data4 = mSprite_Turret_Positions[Data0 + 1];
    pSprite->field_4 += Data4;

    mSprite_Missile_LaunchDistance_X = 0;
    mSprite_Missile_LaunchDistance_Y = 0;

    if (pSprite->field_6F != eVehicle_Turret_Homing)
        goto loc_2421D;

    if (!mTurretFires_HomingMissile)
        goto loc_2421D;

    ax = Sprite_Create_MissileHoming(pSprite, Data2C, pData34);
    goto loc_24234;

loc_2421D:;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        if (word_3B4ED[1] > 9)
            goto loc_24275;

    ax = Sprite_Create_Missile(pSprite, Data2C);

loc_24234:;
    if (!ax)
        goto loc_2426C;

    Data2C->field_20 += 0x11;
    Data2C->field_36 = 0x3C;
    Sound_Play(pSprite, eSound_Effect_Turret_Fire, 0x1E);
    Sprite_Create_Smoke(pSprite, Data2C);
    pSprite->field_57 = 0;
    goto loc_24275;

loc_2426C:;
    pSprite->field_57 = -1;

loc_24275:;
    pSprite->field_4 = Field_4;
    pSprite->field_0 = Field_0;
}

int16 cFodder::Sprite_Find_By_Types(sSprite* pSprite, int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData10, sSprite*& pData28) {
    int16* Data2C = mSprite_Find_Types;

    mSprite_Find_Types[0] = pData0;
    mSprite_Find_Types[1] = pData4;
    mSprite_Find_Types[2] = pData8;
    mSprite_Find_Types[3] = pDataC;
    mSprite_Find_Types[4] = pData10;

	// Check if Sprite @ field_5E sprite is one of the types
	pData28 = &mSprites[pSprite->field_5E];

	// The original engine checks 1 sprite per cycle, and we have a max of 43 sprites to check
	// Meaning it takes 43 cycles to check all sprites, including empty sprite slots
	// this becomes problematic when you increase maxsprites, especially insane high values like
	//  1,000,000
	if (!mParams->isOriginalSpriteMax()) {
		bool Looped = false;

		// So if we arnt using default max sprites,
		// We counter the problem by searching all sprites in a single loop (we can do this, as we have faster CPUs than in 1993)
		// 
		do {
		NextSprite2:;
			pData28 = &mSprites[pSprite->field_5E];

			if (pSprite->field_5E >= (mParams->mSpritesMax - 2)) {
				pSprite->field_5E = 0;
				if (Looped == true)
					return -1;

				Looped = true;
				continue;
			}

			if (pData28->field_0 == -32768) {
				++pSprite->field_5E;
				continue;
			}

			Data2C = mSprite_Find_Types;
			do {
				pData0 = *Data2C++;
				if (pData0 < 0) {
					++pSprite->field_5E;
					goto NextSprite2;
				}
			} while (pData0 != pData28->field_18);
		} while (pData28->field_0 == -32768);

	} else {
		if (pData28->field_0 == -32768)
			goto NextSprite;

		do {
			pData0 = *Data2C++;
			if (pData0 < 0)
				goto NextSprite;
		} while (pData0 != pData28->field_18);
	}

    // Found a type match
    pData0 = pSprite->field_0;
    pData4 = pSprite->field_4;
    pData8 = pData28->field_0;
    pDataC = pData28->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(pData0, pData4, pData8, pDataC);
    mSprite_Find_Distance = pData0;

    if (pData0 >= 0xD2)
        goto NextSprite;

    if (pData0 <= 0x28)
        goto FoundSprite;

    pData0 = pSprite->field_0;
    pData4 = pSprite->field_4;
    pData8 = pData28->field_0;
    pDataC = pData28->field_4;

    if (Map_PathCheck_CalculateTo(pData0, pData4, pData8, pDataC))
        goto loc_2439F;

    pData0 = mSprite_Find_Distance;
    goto FoundSprite;

loc_2439F:;
    if (mSprite_Find_Distance < 0x40)
        goto FoundSprite;

NextSprite:;
    pSprite->field_5E++;
    if (pSprite->field_5E >= (mParams->mSpritesMax - 2))
        pSprite->field_5E = 0;

    goto loc_243DD;
FoundSprite:;
    pData4 = pData28->field_18;
    return 0;

loc_243DD:;
    return -1;
}

void cFodder::sub_243E9(sSprite* pSprite) {
    sSprite* Data24 = pSprite + 1;

    Data24->field_8 = 0x8D;
    int16 Data0 = pSprite->field_20;
    Data0 >>= 4;
    if (Data0 > 2)
        Data0 = 2;

    Data24->field_A = Data0;
    Data0 = pSprite->field_20;
    Data0 >>= 1;

    int16 Data4 = Data0;
    Data0 += 9;
    Data0 += pSprite->field_0;
    Data24->field_0 = Data0;
    Data4 -= 1;
    Data4 += pSprite->field_4;
    Data24->field_4 = Data4;
    if (!pSprite->field_52)
        return;

    Data24->field_A = 0;
    Data24->field_8 = 0x7C;
}

int16 cFodder::Sprite_Handle_Vehicle_Sinking(sSprite* pSprite) {

    Sound_Play(pSprite, eSound_Effect_Vehicle_Sinking, 0x0F);
    if (!pSprite->field_5C)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;

    if (dword_3B24B) {
        Data0 = tool_RandomGet() & 0x1F;
        Data0 -= 5;
        Data2C->field_0 += Data0;
        Data0 >>= 4;
        Data0 &= 0x0F;
        Data0 -= 0x0A;
        Data2C->field_4 -= Data0;
        dword_3B24B = 0;
    }
    //loc_24546
    Data2C->field_20 = pSprite->field_20;
    Data2C->field_52 = 0;
    Data2C->field_2C = eSprite_Draw_OnTop;

    Data0 = tool_RandomGet() & 1;
    if (!Data0) {
        Data2C->field_8 = 0xDE;
        Data2C->field_A = 4;
        Data2C->field_18 = eSprite_Vehicle_Sinking_1;
    }
    else {
        Data2C->field_8 = 0xDF;
        Data2C->field_A = 0;
        Data2C->field_18 = eSprite_Vehicle_Sinking_2;
    }
    return 0;
}

void cFodder::sub_245BF(sSprite* pSprite) {

    if (pSprite->field_22 == eSprite_PersonType_Human)
        goto loc_245DA;

    if (pSprite->field_54)
        goto loc_24617;

    return;

loc_245DA:;
    if (pSprite != mSquad_CurrentVehicle)
        return;

    if (!mButtonPressRight)
        return;

    if (pSprite->field_54)
        goto loc_24617;
    pSprite->field_54 = 0;
    pSprite->field_55 = ~pSprite->field_55;
    if (pSprite->field_55)
        return;

loc_24617:;
    pSprite->field_54 = 0;

    int16 Field_0 = pSprite->field_0;
    int16 Field_4 = pSprite->field_4;

    int16 Data0 = pSprite->field_A;

    // seg005:4F58 
    Data0 <<= 1;

    if (pSprite->field_5B < 0)
        Data0 += 2;

    int16 Data4 = mSprite_VehiclePosition_Mod[Data0];
    pSprite->field_0 += Data4;
    Data4 = mSprite_VehiclePosition_Mod[Data0 + 1];
    pSprite->field_4 += Data4;

    if (!Sprite_Create_Cannon(pSprite)) {
        pSprite->field_5B = ~pSprite->field_5B;
        Sound_Play(pSprite, 0x10, 0x1E);
    }

    //loc_246BC
    pSprite->field_4 = Field_4;
    pSprite->field_0 = Field_0;
}

int16 cFodder::Sprite_Create_Cannon(sSprite* pSprite) {
    if (mPhase_Completed_Timer)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    if (mSprite_Projectile_Counters[2] == 0x14)
        return -1;

    ++mSprite_Projectile_Counters[2];
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;
    Data2C->field_8 = 0x7F;
    Data2C->field_A = 3;
    Data2C->field_12 = 9;
    Data2C->field_18 = eSprite_Cannon;

    Data2C->field_1E_Big += 0x60000;

    Data2C->field_52 = pSprite->field_52;
    Data2C->field_22 = pSprite->field_22;
    Data2C->field_2C = eSprite_Draw_Second;
    Data0 = 0x321;

    Data0 += pSprite->field_62;
    Data2C->field_4A = Data0;
    Data0 = pSprite->field_36;
    Data0 += 0x50;
    Data2C->field_36 = Data0;
    Data2C->field_43 = 0;
    Data2C->field_44 = 0;
    Data0 = pSprite->field_A;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    Data0 <<= 5;
    Data2C->field_10 = Data0;
    Data2C->field_3A = 0;
    Data2C->field_1A_sprite = pSprite;
    Data2C->field_2A = 2;
    Data2C->field_16 = 0;
    Data2C->field_14 = 2;
    Data2C->field_34 = -1;
    Data2C->field_50 = 0;
    Data2C->field_59 = 0;

    int16 Data8 = 7;
    Data0 = tool_RandomGet();
    int16 Data4 = Data0;

    Data0 &= Data8;
    if (Data4 < 0)
        Data0 = -Data0;

    Data2C->field_10 += Data0;
    Data2C->field_64 = 0;
    return 0;
}

void cFodder::Sprite_Handle_Helicopter_Human_Deploy_Weapon(sSprite* pSprite) {
    sSprite* Data2C = 0, *Data34 = 0;

    if (!pSprite->field_57)
        if (!pSprite->field_54)
            return;

    pSprite->field_54 = 0;

    if (pSprite->field_20 <= 0x1F)
        return;

    if (pSprite->field_6F == eVehicle_Helicopter_Grenade)
        goto loc_24905;

    if (pSprite->field_6F == eVehicle_Helicopter_Missile)
        goto loc_24917;

    if (pSprite->field_6F == eVehicle_Helicopter_Homing)
        goto loc_24917;

loc_24905:;
    if (!Sprite_Create_Grenade2(pSprite))
        goto loc_2490D;
    goto loc_24942;

loc_2490D:;
    pSprite->field_57 = -1;
    return;

loc_24917:;
    mSprite_Missile_LaunchDistance_X = 0x0E;
    mSprite_Missile_LaunchDistance_Y = -12;

    // Homing Missle and can get lock on a target?
    if (pSprite->field_6F == eVehicle_Helicopter_Homing &&
        Sprite_Homing_LockInRange(pSprite, Data34)) {

        // Launch a homing missile
        if (!Sprite_Create_MissileHoming(pSprite, Data2C, Data34))
            goto loc_2490D;

    }
    else {
        // Launch a missile
        if (!Sprite_Create_Missile(pSprite, Data2C))
            goto loc_2490D;
    }

loc_24942:;
    pSprite->field_57 = 0;
}

int16 cFodder::Sprite_Create_Grenade2(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;

    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Projectile_Counters[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    Data0 = 2;
    sSprite* Data2C, *Data30;
    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return 0;

    ++mSprite_Projectile_Counters[pSprite->field_22];
    Data30 = Data2C + 1;
    Data2C->field_0 = pSprite->field_0;
    Data30->field_0 = pSprite->field_0;
    Data2C->field_4 = pSprite->field_4;
    Data30->field_4 = pSprite->field_4;
    Data2C->field_8 = 0x7D;
    Data30->field_8 = 0x7E;
    Data2C->field_A = 0;
    Data30->field_A = 0;

    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;
    DataC += 6;

    Data2C->field_26 = Data8;
    Data2C->field_28 = DataC;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        Sprite_Enemy_Set_Target(Data2C);

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 < 0x28)
        Data0 = 0x28;

    Data0 = Data0 / 5;
    if (Data0 > 0x64)
        Data0 = 0x64;

    Data2C->field_12 = Data0;
    Data2C->field_36 = 0x32;
    Data30->field_36 = 0x32;
    Data0 = pSprite->field_20;
    Data0 += 2;

    Data2C->field_1E = 0;
    Data2C->field_20 = Data0;
    Data30->field_1E = 0;
    Data30->field_20 = 0;

    Data2C->field_4 += 1;
    Data2C->field_0 += 3;

    int32 Dataa0 = Data2C->field_12 << 16;
    Dataa0 >>= 1;
    if (Dataa0 > 0xE0000)
        Dataa0 = 0xE0000;

    Data2C->field_1A = Dataa0;
    Data2C->field_1A = 0;
    Data2C->field_18 = eSprite_Grenade;
    Data30->field_18 = eSprite_ShadowSmall;

    Data2C->field_52 = 0;
    Data30->field_52 = 0;
    Data2C->field_50 = 0;

    Data2C->field_22 = pSprite->field_22;
    Data30->field_22 = pSprite->field_22;

    Data2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    Data2C->field_38 = eSprite_Anim_None;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        Data2C->field_12 += 0x0A;

    return -1;
}

int16 cFodder::Sprite_Create_MissileHoming(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData34) {

    if (mPhase_Completed_Timer)
        return 0;

    if (mSprite_Missile_Projectile_Counters[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    int16 Data0 = 2;
    sSprite *Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, pData2C, Data30))
        return 0;

    ++mSprite_Missile_Projectile_Counters[pSprite->field_22];

    Data30 = pData2C;
    ++Data30;

    //seg005:55E0
    pData2C->field_0 = pSprite->field_0;
    pData2C->field_2 = pSprite->field_2;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_6 = pSprite->field_6;

    Data30->field_0 = pSprite->field_0;
    Data30->field_2 = pSprite->field_2;
    Data30->field_4 = pSprite->field_4;
    Data30->field_6 = pSprite->field_6;

    pData2C->field_0 += mSprite_Missile_LaunchDistance_X;
    Data30->field_0 += mSprite_Missile_LaunchDistance_X;
    if (Data30->field_0 < 0)
        return 0;

    pData2C->field_4 += mSprite_Missile_LaunchDistance_Y;
    Data30->field_4 += mSprite_Missile_LaunchDistance_Y;

    if (Data30->field_0 < 0)
        return 0;

    //seg005:5672
    pData2C->field_1A_sprite = pData34;
    pData2C->field_1E = pSprite->field_1E;
    pData2C->field_20 = pSprite->field_20;
    pData2C->field_26 = pSprite->field_2E;
    pData2C->field_28 = pSprite->field_30;
    pData2C->field_28 += 0x10;
    pData2C->field_8 = 0xA3;
    Data30->field_8 = 0x7E;
    pData2C->field_A = 0;
    Data30->field_A = 1;
    pData2C->field_36 = 0;
    Data30->field_36 = 0;
    pData2C->field_6A = 0x10000;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        pData2C->field_6A = 0x400;

    Data30->field_1E = 0;
    Data30->field_20 = 0;
    pData2C->field_4 += 1;
    pData2C->field_0 += 3;
    pData2C->field_18 = eSprite_MissileHoming2;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        pData2C->field_18 = eSprite_MissileHoming;
    Data30->field_18 = eSprite_ShadowSmall;
    pData2C->field_52 = 0;
    Data30->field_52 = 0;
    pData2C->field_22 = pSprite->field_22;
    Data30->field_22 = pSprite->field_22;
    pData2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    pData2C->field_38 = eSprite_Anim_None;

    if (mVersionCurrent->isCoverDisk())
        Sound_Play(pSprite, 0x10, 0x0F);
    else
        Sound_Play(pSprite, eSound_Effect_Turret_Fire, 0x0F);
    return -1;
}

int16 cFodder::Sprite_Handle_Helicopter_Enemy_Weapon(sSprite* pSprite, sSprite*& pData30) {
    int16 Data0;
    sSprite* Data2C = 0, *Data34 = 0;

    if (pSprite->field_6F == eVehicle_Helicopter_Grenade)
        goto loc_25344;

    if (pSprite->field_6F == eVehicle_Helicopter_Missile)
        goto loc_253E7;

    if (pSprite->field_6F == eVehicle_Helicopter_Homing)
        goto loc_253E7;

loc_25344:;
    pSprite->field_26 = pData30->field_0;
    pSprite->field_26 += 0x14;
    pSprite->field_28 = pData30->field_4;
    pSprite->field_26 -= 0x1E;
    if (pSprite->field_26 < 0)
        pSprite->field_26 = 0;

    pSprite->field_2E = pData30->field_0;
    pSprite->field_30 = pData30->field_4;
    if (pSprite->field_20 <= 0x1F)
        goto loc_253D2;

    if (mMission_EngineTicks & 0x0F)
        goto loc_253D2;

    Sprite_Create_Grenade2(pSprite);
    Data0 = tool_RandomGet() & 0x0F;
    if (!Data0)
        goto loc_253DE;

loc_253D2:;
    return -1;

loc_253DE:;
    return 0;

loc_253E7:;
    pSprite->field_26 = pData30->field_0;
    pSprite->field_28 = pData30->field_4;
    pSprite->field_2E = pData30->field_0;
    pSprite->field_30 = pData30->field_4;

    Data0 = mSprite_DistanceTo_Squad0;
    if (Data0 <= 0x3C)
        goto loc_254C7;
    if (Data0 >= 0x8C)
        goto loc_254E7;

    if (pSprite->field_20 >= 0x19) {
        if (!(mMission_EngineTicks & 0x0F)) {
            mSprite_Missile_LaunchDistance_X = 0x0E;
            mSprite_Missile_LaunchDistance_Y = -12;
            if (pSprite->field_6F == eVehicle_Helicopter_Homing) {

                Data34 = pData30;
                if (Sprite_Create_MissileHoming(pSprite, Data2C, Data34)) {
                    Data2C->field_6A = 0x2000;
                    Data2C->field_36 = 0x14;
                }
            }
            else {
                Sprite_Create_Missile(pSprite, Data2C);
            }
        }
    }

    if (pSprite->field_36 < 0)
        goto loc_254E7;

    pSprite->field_36 -= 4;
    if (pSprite->field_36 < 0)
        pSprite->field_36 = 0;
    goto loc_254E7;

loc_254C7:;

    if (pSprite->field_36 >= 0)
        goto loc_254DE;

    if (pSprite->field_36 < -34)
        goto loc_254E7;
loc_254DE:;
    pSprite->field_36 -= 3;

loc_254E7:;
    Data0 = tool_RandomGet() & 0x0F;
    if (!Data0)
        return 0;

    return -1;
}

void cFodder::Sprite_Handle_Vehicle_Enemy(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data28 = 0;

    if (pSprite->field_38) {
        pSprite->field_22 = eSprite_PersonType_AI;
        Sprite_Handle_Vehicle(pSprite);
        pSprite->field_22 = eSprite_PersonType_AI;
        return;
    }

    if (pSprite->field_6F == eVehicle_DontTargetPlayer)
        goto loc_255DA;

    Data0 = eSprite_Player;
    Data4 = eSprite_Civilian;
    Data8 = 0x3E;
    DataC = 0x46;
    Data10 = -1;
    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        goto loc_255DA;

    Data0 = tool_RandomGet() & 3;
    if (!Data0)
        pSprite->field_54 = -1;

    Data0 = pSprite->field_0;
    Data0 += 8;
    Data4 = pSprite->field_4;
    Data4 -= 5;
    Data8 = Data28->field_0;
    pSprite->field_2E = Data8;
    pSprite->field_26 = Data8;
    DataC = Data28->field_4;
    pSprite->field_30 = DataC;
    pSprite->field_28 = DataC;

loc_255DA:;
    pSprite->field_22 = eSprite_PersonType_AI;
    Sprite_Handle_Vehicle(pSprite);

    if (pSprite->field_6F == eVehicle_JeepRocket)
        sub_245BF(pSprite);

    sSprite* Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;

    Data24 = pSprite + 2;
    Data24->field_18 = eSprite_Flashing_Light;
    Data4 = pSprite->field_0;
    Data4 += 0x0D;
    Data24->field_0 = Data4;

    Data4 = pSprite->field_4;
    Data4 -= 0x13;

    Data4 += pSprite->field_52;
    Data24->field_4 = Data4;
    Data24->field_20 = pSprite->field_20;
}

void cFodder::Sprite_Handle_Helicopter_Enemy(sSprite* pSprite) {
    int16 Data0;
    int16 Data4;
    int16 Data8;
    int16 DataC;
    int16 Data10, Data14, Data1C;
    sSprite* Data24 = 0, *Data30 = 0;

    // Bullets don't hit choppers
    if (pSprite->field_38 == eSprite_Anim_Hit)
        pSprite->field_38 = eSprite_Anim_None;

    if (pSprite->field_38) {
        mSprite_Helicopter_DestroyLight = -1;

        pSprite->field_22 = eSprite_PersonType_AI;
        Sprite_Handle_Helicopter(pSprite);
        pSprite->field_22 = eSprite_PersonType_AI;

        mSprite_Helicopter_DestroyLight = 0;
        goto loc_25288;
    }

    if (!pSprite->field_43) {
        pSprite->field_46 = (pSprite->field_0 << 16) | (pSprite->field_4 & 0xFFFF);
        pSprite->field_43 = -1;
    }

    if (pSprite->field_4C)
        --pSprite->field_4C;

    if (pSprite->field_20)
        goto loc_250D2;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = ((int64)pSprite->field_46) >> 16;
    DataC = ((int64)pSprite->field_46) & 0xFFFF;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x14)
        goto loc_250D2;

    Data8 = pSprite->field_62;
    if ((uint16)Data8 >= 0x1F4) {
        if (pSprite->field_18 == eSprite_Helicopter_Homing_Enemy2)
            if (mHelicopterCall_X < 0)
                goto loc_24FF1;

        if (!(tool_RandomGet() & 1))
            goto loc_2500F;
    }

loc_24FF1:;
    if (mVersionCurrent->isDemo()) {
        Data8 += 0x20;
        if (Data8)
            pSprite->field_62 = Data8;

    }
    else {
        Data8 += 1;
        Data8 += mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage;

        if (Data8 >= 0)
            pSprite->field_62 = Data8;
    }

    goto loc_25239;

loc_2500F:;
    if (pSprite->field_4C)
        goto loc_25239;

	Data0 = map_GetRandomX();
	Data0 += 4;
	if (Data0 > mMapLoaded->getWidth())
		goto loc_25239;

	Data8 = Data0;
	Data0 = map_GetRandomY();
	Data0 += 4;

    if (Data0 > mMapLoaded->getHeight())
        goto loc_25239;

    DataC = Data0;

    Data8 <<= 4;
    Data0 = tool_RandomGet() & 0x0F;
    Data8 += Data0;

	DataC <<= 4;
    Data0 = tool_RandomGet() & 0x0F;
    DataC += Data0;

    if (Map_Terrain_Get_Moveable_Wrapper(mTiles_NotFlyable, Data8, DataC, Data10, Data14))
        goto loc_25239;

    pSprite->field_26 = Data10;
    pSprite->field_28 = Data14;
    pSprite->field_4C = 0x5A;
    goto loc_25239;

loc_250D2:;

    if (pSprite->field_6F == eVehicle_Helicopter)
        goto loc_251D2;

    Data1C = pSprite->field_5E_Squad;

    if (mSquads[Data1C / 9] == (sSprite**)INVALID_SPRITE_PTR)
        goto loc_251B4;

    Data30 = mSquads[Data1C / 9][Data1C % 9];
    if (Data30 == INVALID_SPRITE_PTR)
        goto loc_251B4;

    if (Data30->field_52)
        goto loc_251B4;

    Data8 = Data30->field_0;
    Data8 += 8;
    DataC = Data30->field_4;
    DataC += -5;

    if (Map_Terrain_Get_Moveable_Wrapper(mTiles_NotDriveable, Data8, DataC, Data10, Data14))
        goto loc_251B4;

    Data0 = Data30->field_0;
    Data4 = Data30->field_4;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    mSprite_DistanceTo_Squad0 = Data0;
    if (Data0 < 0xFA)
        if (Sprite_Handle_Helicopter_Enemy_Weapon(pSprite, Data30))
            goto loc_251D2;

loc_251B4:;
    pSprite->field_5E_Squad += 1;
    if (pSprite->field_5E_Squad >= 0x1E)
        pSprite->field_5E_Squad = 0;

loc_251D2:;
    if (pSprite->field_62) {
        --pSprite->field_62;
    }
    else {
        pSprite->field_26 = ((int64)pSprite->field_46) >> 16;
        pSprite->field_28 = ((int64)pSprite->field_46) & 0xFFFF;
        goto loc_25239;
    }

    Data0 = tool_RandomGet() & 0x1F;
    if (!Data0)
        goto loc_2500F;

    if (!pSprite->field_20) {
        Data0 = tool_RandomGet() & 0x0F;
        if (!Data0) {
            sSprite* Data2C;
            Sprite_Create_Enemy(pSprite, Data2C);
        }
    }

loc_25239:;
    mSprite_Helicopter_DestroyLight = -1;
    pSprite->field_22 = eSprite_PersonType_AI;
    Sprite_Handle_Helicopter(pSprite);

    pSprite->field_22 = eSprite_PersonType_AI;
    mSprite_Helicopter_DestroyLight = 0;
    Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;

    ++Data24;
    Data24->field_18 = eSprite_Null;

loc_25288:;

    Data24 = pSprite + 3;
    Data24->field_18 = eSprite_Flashing_Light;
    Data0 = pSprite->field_A << 1;

    Data4 = pSprite->field_0;
    Data4 += mSprite_Helicopter_Light_Positions[Data0];
    Data24->field_0 = Data4;

    Data4 = pSprite->field_4;
    Data4 += mSprite_Helicopter_Light_Positions[Data0 + 1];
    Data24->field_4 = Data4;

    Data24->field_20 = pSprite->field_20;
}

void cFodder::Sprite_Handle_Civilian_Unk(sSprite* pSprite) {
    if (!pSprite->field_5C)
        return;

    if ((tool_RandomGet() & 0x3F) != 9)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    pSprite->field_2E = mSquad_Leader->field_0;
    pSprite->field_30 = mSquad_Leader->field_4;

    Sprite_Create_Civilian_Spear2(pSprite);
}

void cFodder::Sprite_Handle_Civilian_Movement(sSprite* pSprite) {

    // Destination change cool down
    if (pSprite->field_4C) {
        pSprite->field_4C -= 1;
        if (pSprite->field_4C)
            return;
    }

    if (pSprite->field_18 == eSprite_Hostage || pSprite->field_18 == eSprite_Enemy_Leader) {
        pSprite->field_26 = pSprite->field_0;
        pSprite->field_28 = pSprite->field_4;
    }
    else {
        if (Sprite_Handle_Civilian_RandomMovement(pSprite) < 0)
            return;
    }

    // Random time until destination change
    int16 Data0 = tool_RandomGet() & 0x3F;
    Data0 += 0x14;
    pSprite->field_4C = static_cast<int8>(Data0);
}

int16 cFodder::Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(sSprite* pSprite) {

    if (!mSprite_OpenCloseDoor_Ptr)
        return -1;

    if (pSprite->field_4C) {
        --pSprite->field_4C;
        return 0;
    }

    sSprite* Data28 = mSprite_OpenCloseDoor_Ptr;

    int16 Data0 = pSprite->field_0;
    int16 Data4 = pSprite->field_4;
    int16 Data8 = Data28->field_0;
    int16 DataC = Data28->field_4;

    pSprite->field_26 = Data8;
    pSprite->field_28 = DataC;

    mSprite_Civilian_Tmp_X = Data0;
    mSprite_Civilian_Tmp_Y = Data4;
    word_3B481 = Data8;
    word_3B483 = DataC;
    int16 Data10 = 0x20;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 <= 3) {
        Sprite_Destroy_Wrapper(pSprite);
        mSprite_Civilian_GotHome = -1;
        return 1;
    }

    if (Data0 < 6) {
        Data28->field_2A = 0x0A;
        return 0;
    }

    Data0 = mSprite_Civilian_Tmp_X;
    Data4 = mSprite_Civilian_Tmp_Y;
    Data8 = word_3B481;
    DataC = word_3B483;

    if (!Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC))
        return 0;

	// Move toward top left
    Data0 = tool_RandomGet() & 0x3F;
    if (Data0)
        return 0;

    pSprite->field_26 = pSprite->field_0;
    pSprite->field_28 = pSprite->field_4;
    pSprite->field_26 -= 0x10;
    pSprite->field_28 -= 0x20;
    pSprite->field_4C = 0x1E;

    return 0;
}

void cFodder::Sprite_Handle_Civilian_Death(sSprite* pSprite) {

    if (pSprite->field_8 != 0xD6) {
        word_3B2D1[2] = -1;

        if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Protect_Civilians - 1])
            mPhase_Aborted = true;

        if (mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Get_Civilian_Home - 1])
            mPhase_Aborted = true;

        pSprite->field_8 = 0xD6;
        pSprite->field_A = 0;

        int16 Data0 = tool_RandomGet() & 7;
        int16 Data4 = mSprite_Civilian_Sound_Death[Data0];

        Sound_Play(pSprite, Data4, 0x14);
    }
    else {
        //loc_2584A
        pSprite->field_A += 1;
        if (pSprite->field_A < 9)
            return;

        Sprite_Destroy_Wrapper(pSprite);
    }
}

void cFodder::sub_2593D(sSprite* pSprite) {
    Sprite_XY_Store(pSprite);

    if (pSprite->field_44)
        --pSprite->field_44;
    else {
        int16 Data0 = pSprite->field_26;
        int16 Data4 = pSprite->field_28;

        if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) < 0)
            mSprite_Reached_Target = -1;
    }

    if (pSprite->field_18 != eSprite_Seal) {
        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 0x0E;
        if (Data4 < 0)
            Data0 = -Data0;

        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
    }
    //loc_259BE
    if (!Sprite_Reached_Target(pSprite)) {
        Sprite_Movement_Speed_Update(pSprite);

        if (!word_3B25B)
            Sprite_Movement_Calculate(pSprite);
    }
    //loc_259D4
    Sprite_Reached_MapEdge(pSprite);

    int16 Data4 = 0;
    Sprite_Terrain_Check(pSprite, Data4);

    Data4 = pSprite->field_60;

    if (Data4 == 6) {
        Sprite_XY_Restore(pSprite);
        return;
    }

    if (Data4 != 4) {
        if (Data4 != 5)
            return;

        pSprite->field_52 = 5;
        return;
    }

    pSprite->field_52 = 3;
}

void cFodder::sub_25A31(sSprite* pSprite) {

    if (mSprite_Reached_Target || word_3B25B)
        return;

    pSprite->field_2A += 1;
    pSprite->field_2A &= 3;
    if (pSprite->field_2A)
        return;

    pSprite->field_A += 1;
    pSprite->field_A &= 1;
}

void cFodder::sub_25A66(sSprite* pSprite) {
    int16 Data0 = pSprite->field_10;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0E;

    int16 Data4 = pSprite->field_A;
    Data4 &= 1;
    Data0 |= Data4;
    pSprite->field_A = Data0;
}

int16 cFodder::Sprite_Handle_Civilian_RandomMovement(sSprite* pSprite) {
    int16 Data8 = map_GetRandomX();
	Data8 += 4;
    if (Data8 >= mMapLoaded->getWidth())
        return -1;

	int16 DataC = map_GetRandomY();
	DataC += 4;
    if (DataC >= mMapLoaded->getHeight())
        return -1;

    Data8 <<= 4;
    Data8 += tool_RandomGet() & 0x0F;

    DataC <<= 4;
    DataC += tool_RandomGet() & 0x0F;

    int16 X, Y;

    if (Map_Terrain_Get_Moveable_Wrapper(mTiles_NotFlyable, Data8, DataC, X, Y))
        return -1;

    pSprite->field_26 = X;
    pSprite->field_28 = Y;
    return 0;
}

int16 cFodder::Sprite_Create_Civilian_Spear2(sSprite* pSprite) {
    if (mPhase_Completed_Timer)
        return -1;

    if (!pSprite->field_2E)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;
    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    if (mSprite_Projectile_Counters[2] == 0x14)
        return -1;

    ++mSprite_Projectile_Counters[2];
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_8 = 0xD7;
    Data2C->field_A = 0;
    Data2C->field_12 = 9;
    Data0 = pSprite->field_62;

    Data0 >>= 3;
    Data0 += 8;
    if (Data0 > 0x10)
        Data0 = 0x10;

    Data2C->field_12 = Data0;
    Data2C->field_18 = eSprite_Civilian_Spear2;
    Data2C->field_1E_Big = pSprite->field_1E_Big;
    Data2C->field_1E_Big += 0x60000;

    Data2C->field_52 = pSprite->field_52;
    Data2C->field_22 = pSprite->field_22;
    Data2C->field_2C = eSprite_Draw_Second;
    Data0 = 0x32;
    Data0 += pSprite->field_62;

    Data2C->field_4A = Data0;
    Data2C->field_36 = 0x18;
    Data2C->field_43 = 0;
    Data2C->field_44 = 0;
    Data2C->field_2E = pSprite->field_2E;
    Data2C->field_30 = pSprite->field_30;
    Data2C->field_2E += 7;
    Data2C->field_52 = 0;
    Data2C->field_3A = 0;
    Data2C->field_1A_sprite = pSprite;
    Data2C->field_2A = 2;
    Data2C->field_16 = 0;
    Data2C->field_14 = 2;

    if (Data2C->field_A == 4)
        Data2C->field_14 = -1;

    Data2C->field_34 = -1;
    Data2C->field_50 = 0;
    Data2C->field_59 = 0;

    int16 Data8 = 7;
    Data0 = tool_RandomGet();
    int16 Data4 = Data0;
    Data0 &= Data8;

    if (Data4 < 0)
        Data0 = -Data0;

    Data2C->field_50 = Data0;

    if (mMapLoaded->getTileType() == eTileTypes_Moors) {
        Data0 = tool_RandomGet() & 1;
        if (Data0)
            goto loc_25D9D;
        Data4 = 0x11;
        goto loc_25DA3;

    loc_25D9D:;
        Data4 = 0x10;

    loc_25DA3:;
        Sound_Play(pSprite, Data4, 0);
    }

    //loc_25DB1
    Data2C->field_64 = 0;
    return 0;
}

int16 cFodder::sub_25DCF(sSprite* pSprite) {
    sSprite* Data2C = 0, *Data30 = 0;
    int16 Data0;

    if (pSprite->field_38 == eSprite_Anim_None)
        return 0;

    if (pSprite->field_38 == eSprite_Anim_Die4)
        goto loc_25E8F;

    if (!pSprite->field_2A) {
        pSprite->field_2A = 0x14;
        pSprite->field_A = 0;
    }
    pSprite->field_2A -= 1;
    if (!pSprite->field_2A)
        return Sprite_Destroy_Wrapper(pSprite);

    //loc_25E19
    if (pSprite->field_2A >= 0x0A)
        goto loc_25E54;

    pSprite->field_A = 0;
    if (pSprite->field_8 == 0xD5) {
        //loc_25E47
        pSprite->field_8 = 0x7C;
        return -1;
    }

    pSprite->field_8 = 0xD5;
    return -1;

loc_25E54:;
    if (Sprite_Create_BloodTrail(pSprite, Data2C, Data30))
        return -1;

    Data2C->field_4 += tool_RandomGet() & 3;
    Data2C->field_0 -= 4;
    Data2C->field_0 += tool_RandomGet() & 7;
    return -1;

loc_25E8F:;
    if (pSprite->field_8 != 0xE1) {
        pSprite->field_8 = 0xE1;
        pSprite->field_A = 0;
        pSprite->field_2A = 0;
        return -1;
    }

    pSprite->field_2A += 2;
    Data0 = mSprite_Seal_Frames[pSprite->field_2A / 2];

    if (Data0 >= 0) {

        pSprite->field_A = Data0;
        return -1;
    }

    pSprite->field_18 = eSprite_Explosion;
    pSprite->field_26 = 0x1F45;
    pSprite->field_28 = -3;
    return -1;
}

void cFodder::sub_25F2B(sSprite* pSprite) {

    if (word_3B2ED) {
        pSprite->field_8 = 0xD8;
        pSprite->field_A = 1;
        Sprite_XY_Restore(pSprite);
        return;
    }

    if (word_3B2D1[2] != pSprite->field_0 || word_3B2D1[3] != pSprite->field_2)
        return;

    if (word_3B2D1[4] != pSprite->field_4 || word_3B2D1[5] != pSprite->field_6)
        return;

    pSprite->field_8 = 0xDA;

    int16 Data0 = mMission_EngineTicks & 0x0F;
    if (!Data0) {
        Data0 = tool_RandomGet() & 0x06;
        pSprite->field_32 = mSprite_Hostage_Frames[Data0 / 2];
    }

    pSprite->field_A = pSprite->field_32;
}

void cFodder::Sprite_Handle_Hostage_Movement(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data28 = 0;

    if (pSprite->field_18 == eSprite_Enemy_Leader)
        goto CheckRescueTent;

    // No known rescue tent?
    if (!mHostage_Rescue_Tent)
        goto loc_2608B;

    if (pSprite->field_74) {
        pSprite->field_74 -= 1;
        goto loc_2608B;
    }
    //loc_2600B

    pSprite->field_74 = -56;

    Data8 = mHostage_Rescue_Tent->field_0 + 10;
    DataC = mHostage_Rescue_Tent->field_4 - 5;
    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data10 = 0x80;

    // Distance to rescue tent < 127?
    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data10 < 0x7F)
        pSprite->field_5E = (int16)(mHostage_Rescue_Tent - mSprites.data());

loc_2608B:;
    word_3B2ED = 0;
    Data0 = eSprite_Hostage_Rescue_Tent;
    Data4 = eSprite_Enemy;
    Data8 = eSprite_Player;
    DataC = -1;
    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        return;

    // Found Tent Nearby
    if (Data4 == eSprite_Hostage_Rescue_Tent) {

        pSprite->field_26 = Data28->field_0 + 10;
        pSprite->field_28 = Data28->field_4 - 5;

        Data0 = pSprite->field_0;
        Data4 = pSprite->field_4;
        Data8 = pSprite->field_26;
        DataC = pSprite->field_28;
        Data10 = 0x10;
        Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
        if (Data10 >= 0x10)
            goto loc_2620F;

        if (Data0 >= 3)
            return;

        mHostage_Count -= 1;
        Sprite_Destroy_Wrapper(pSprite);
        return;

        // Found Enemy near by
    }
    else if (Data4 == eSprite_Enemy) {
        Data28->field_70 = pSprite;
        pSprite->field_26 = pSprite->field_0;
        pSprite->field_28 = pSprite->field_4;
        word_3B2ED = -1;
        return;
    }

    pSprite->field_26 = Data28->field_0 + 4;
    pSprite->field_28 = Data28->field_4 - 6;

    // Not in vehicle?
    if (!Data28->field_6E)
        goto loc_2620F;

    // Vehicle Has Speed?
    if (Data28->field_6A_sprite->field_36 > 2)
        goto loc_2620F;

    // Vehicle has height?
    if (Data28->field_6A_sprite->field_20 > 3)
        goto loc_2620F;

    Data0 = Data28->field_6A_sprite->field_0;
    Data4 = Data28->field_6A_sprite->field_4;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    Data10 = 0x10;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 > 0x0A) {
    loc_2620F:;
        Data0 = tool_RandomGet() & 0x3F;
        if (Data0)
            return;

        sub_26490(pSprite);
        return;
    }

    pSprite->field_6E = -1;
    return;

CheckRescueTent:;
    word_3B2ED = 0;
    Data0 = eSprite_Player;
    Data4 = eSprite_Hostage_Rescue_Tent;
    Data8 = -1;
    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        return;

    if (Data4 == eSprite_Hostage_Rescue_Tent) {

        pSprite->field_26 = Data28->field_0 + 0x0A;
        pSprite->field_28 = Data28->field_4 - 0x05;

        Data0 = pSprite->field_0;
        Data4 = pSprite->field_4;
        Data8 = pSprite->field_26;
        DataC = pSprite->field_28;
        Data10 = 0x10;
        Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
        if (Data0 >= 3)
            return;
        mHostage_Count -= 1;

        Sprite_Destroy(pSprite + 1);
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    // Another Player
    word_3B2ED = -1;
    pSprite->field_8 = 0xD8;
    pSprite->field_26 = Data28->field_0 + 4;
    pSprite->field_28 = Data28->field_4 - 6;

    // Not in vehicle?
    if (!Data28->field_6E)
        goto loc_263E5;

    // Vehicle has speed?
    if (Data28->field_6A_sprite->field_36 > 2)
        goto loc_263E5;

    // Vehicle has height
    if (Data28->field_6A_sprite->field_20 > 3)
        goto loc_263E5;

    Data0 = Data28->field_6A_sprite->field_0;
    Data4 = Data28->field_6A_sprite->field_4;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    Data10 = 0x10;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 <= 0x0F) {
        pSprite->field_6E = -1;
        return;
    }

loc_263E5:;
    Data0 = tool_RandomGet() & 7;
    if (Data0)
        return;

    sub_26490(pSprite);
}

void cFodder::Sprite_Handle_Hostage_FrameUpdate2(sSprite* pSprite) {
    int16 Data0 = pSprite->field_4A;

    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (!pSprite->field_2A)
        Data0 += 2;

    Data0 &= 6;

    pSprite->field_4A = Data0;
    pSprite->field_A = mSprite_Hostage_Frames[Data0 / 2];

}

void cFodder::Sprite_Handle_Hostage_FrameUpdate(sSprite* pSprite) {
    int16 Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0E;

    int16 Data4 = Data0;
    Data4 >>= 1;
    Data4 += Data0;

    pSprite->field_A += Data4;
}

void cFodder::sub_26490(sSprite* pSprite) {
    ++pSprite->field_5E;
    if (pSprite->field_5E >= (mParams->mSpritesMax - 2))
		pSprite->field_5E = 0;
}

void cFodder::sub_264B0(sSprite* pSprite) {
    int16 Data0, Data4;

    if (!sub_222A3(pSprite))
        goto loc_264CF;

    if (!mGame_Data.mGamePhase_Data.mGoals_Remaining[eObjective_Protect_Civilians - 1])
        return;

    mPhase_Aborted = true;
    return;

loc_264CF:;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax) {
        pSprite->field_8 = 0x9B;
        return;
    }

    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_26580;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14 || pSprite->field_43 == 0x0A)
        goto loc_265B1;

    if (pSprite->field_43 >= 0) {
        pSprite->field_8 = 0x7C;
        return;
    }

    pSprite->field_8 = 0x9B;
    Sound_Play(pSprite, eSound_Effect_BuildingDoor2, 1);

    Data0 = tool_RandomGet() & 0x0F;
    Data4 = 0x14;
    Data4 -= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
    if (Data4 < 0)
        Data0 = 0;

    Data4 <<= 3;
    Data4 += Data0;
    pSprite->field_12 = Data4;

loc_26580:;
    pSprite->field_12 -= 1;
    if (pSprite->field_12 >= 0)
        return;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 += 6;
    pSprite->field_43 = static_cast<int8>(Data0);
    pSprite->field_8 = 0x7C;

loc_265B1:;
    sSprite* Data2C = 0, *Data30 = 0;

    if (!Sprite_Create_Native(pSprite, Data2C, Data30)) {
        Data2C->field_4 -= 4;
        return;
    }

    pSprite->field_12 = 0;
    pSprite->field_43 = -1;
}

int16 cFodder::Sprite_Handle_Helicopter_Callpad_InRange(sSprite* pSprite, sSprite*& pData2C) {
    sSprite** Data28 = mSprite_TroopsAlive;
    int16 Data0 = 0;

    do {
        if (*Data28 == INVALID_SPRITE_PTR || *Data28 == 0)
            return -1;

        pData2C = *Data28++;

        if (pData2C->field_6E)
            continue;
        if (pData2C->field_38)
            continue;

        Data0 = pSprite->field_0 - 2;
        int16 Data4 = pSprite->field_4 - 8;
        int16 Data8 = pData2C->field_0;
        int16 DataC = pData2C->field_4;
        int16 Data10 = 0x20;

        Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);

        if (Data0 < 9)
            break;

    } while (1);

    return 0;
}

void cFodder::Sprite_Handle_Helicopter_Human_CallCheck(sSprite* pSprite) {
    if (pSprite->field_75)
        return;

    pSprite->field_26 = pSprite->field_0;
    pSprite->field_28 = pSprite->field_4;
    if (mHelicopterCall_X < 0)
        return;

    pSprite->field_26 = mHelicopterCall_X;
    pSprite->field_28 = mHelicopterCall_Y;
    int16 Data0 = pSprite->field_0;
    int16 Data4 = pSprite->field_4;
    int16 Data8 = pSprite->field_26;
    int16 DataC = pSprite->field_28;
    int16 Data10 = 0x3F;
    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 >= 0x2C)
        return;

    pSprite->field_75 = -1;
    pSprite->field_6E = -1;
}

void cFodder::Sprite_Handle_Computer(sSprite* pSprite, int16 pData1C) {

    if (pSprite->field_38 == eSprite_Anim_Die3) {
        if (pSprite->field_74 >= pData1C) {
            pSprite->field_18 = eSprite_Explosion2;
            return;
        }
        pSprite->field_74 += 1;
        pSprite->field_38 = eSprite_Anim_None;
    }
    pSprite->field_A = 0;

    int32 Data0 = pSprite->field_74;
    Data0 += 0x28;
    Data0 <<= 8;
    bool of = false;

    if (((uint16)pSprite->field_5E) + Data0 > 0xFFFF)
        of = true;

    pSprite->field_5E += Data0;
    if (of)
        return;

    Data0 = pSprite->field_2A;
    Data0 += 2;
    if (Data0 >= 8)
        Data0 = 0;

    pSprite->field_2A = Data0;
    pSprite->field_8 = mSprite_Computer_Animation[Data0 / 2];
    pSprite->field_20 = mSprite_Computer_Frames[Data0 / 2];
}

int16 cFodder::Map_Get_Distance_BetweenSprites_Within_Window( const sSprite *pSprite, const sSprite *pSprite2 ) {

    auto X1 = pSprite->field_0;
    auto Y1 = pSprite->field_4;

    auto X2 = pSprite2->field_0;
    auto Y2 = pSprite2->field_4;

    return Map_Get_Distance_BetweenPoints_Within_Window(X1, Y1, X2, Y2);
}

int16 cFodder::Map_Get_Distance_BetweenPoints_Within_Window(int16& pX, int16 pY, int16& pX2, int16& pY2) {
    const int8* Data24 = mMap_Distance_Calculations;
    int16 Data10 = 0;

    pX2 -= pX;
    if (pX2 < 0)
        pX2 = -pX2;

    if (pX2 >= getWindowWidth())
        goto loc_29EBB;

    pY2 -= pY;
    if (pY2 < 0)
        pY2 = -pY2;

    if (pY2 >= getWindowWidth())
        goto loc_29EBB;


    for (;;) {
        if (pX2 <= 0x1F)
            if (pY2 <= 0x1F)
                break;

        pX2 >>= 1;
        pY2 >>= 1;
        ++Data10;
    }
    //loc_29E8C
    pY2 <<= 5;
    pY2 |= pX2;
    pX = 0;

    pX = Data24[pY2];
    pX <<= Data10;
    return pX;

loc_29EBB:;
    pX = 0x3E8;

    return 0x3E8;
}

int16 cFodder::Sprite_Create_Native(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30) {
    int16 Data4;

    if (!pSprite->field_5C)
        return -1;

    if (mPhase_Complete)
        return -1;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax)
        return -1;

    int16 Data0 = 1;

    Sprite_Get_Free_Max29(Data0, pData2C, pData30);
    if (Data0)
        return -1;

    pData2C->Clear();
    pData2C->field_18 = mSpawnSpriteType;
    pData2C->field_0 = pSprite->field_0;
    pData2C->field_0 -= 6;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_4 += 4;
    pData2C->field_8 = 0x7C;
    pData2C->field_4A = 0;
    pData2C->field_22 = eSprite_PersonType_Native;

    Data0 = tool_RandomGet();
    Data4 = Data0;
    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pData2C->field_10 = Data0;
    Data4 &= 0x0F;
    Data4 += 8;
    pData2C->field_44 = static_cast<int8>(Data4);
    return 0;
}

int16 cFodder::Direction_Between_Points(int16& pData0, int16& pData4, int16& pData8, int16& pDataC) {
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
    do {
        int32 eax = 1 << pData8;

        if (pData0 & eax)
            break;

        if (pData4 & eax)
            break;

    } while (--pData8 >= 0);

    pData8 -= 4;
    if (pData8 >= 0) {
        pData0 >>= pData8;
        pData4 >>= pData8;
    }

    pData4 <<= 5;
    pData4 |= pData0;
    //pData4 <<= 1;
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
    if (Data14 >= 0) {
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

int16 cFodder::map_GetRandomX() {

	return tool_RandomGet(1, mMapLoaded->getWidth() - 1);
}

int16 cFodder::map_GetRandomY() {

	return tool_RandomGet(1, mMapLoaded->getHeight() - 1);
}

uint16 cFodder::tool_RandomGet(size_t pMin, size_t pMax) {
	uint16 Rand = mRandom.getu();
	uint16 Mod = (uint16) (pMax - pMin + 1);

	return (uint16) ((Rand % Mod) + pMin);
}

int16 cFodder::tool_RandomGet() {

    return mRandom.get();
}

void cFodder::Sprite_Movement_Calculate(sSprite* pSprite) {
    mSprite_Bullet_Destroy = 0;
    if (!pSprite->field_36)
        return;

    //loc_2A10D
    pSprite->field_10 &= 0x1FE;
    int16 Data4 = pSprite->field_10;

    int16 Data8 = mMap_Direction_Calculations[Data4 / 2];

    Data4 += 0x80;
    Data4 &= 0x1FE;
    int16 DataC = mMap_Direction_Calculations[Data4 / 2];
    Data8 >>= 2;
    DataC >>= 2;
    //seg007:064F

    int32 DataaC = (int32)DataC;
    int32 Dataa8 = (int32)Data8;

    Data4 = pSprite->field_36;

    Dataa8 *= Data4;
    DataaC *= Data4;

    int32 tmp = (pSprite->field_2 & 0xFFFF) | pSprite->field_0 << 16;
    tmp += Dataa8;

    pSprite->field_2 = tmp & 0xFFFF;
    pSprite->field_0 = tmp >> 16;

    if (pSprite->field_0 < 0) {
        pSprite->field_0 = 0;
        pSprite->field_2 = 0;
        mSprite_Bullet_Destroy = -1;
    }

    tmp = (pSprite->field_6 & 0xFFFF) | pSprite->field_4 << 16;
    tmp += DataaC;
    pSprite->field_6 = tmp & 0xFFFF;
    pSprite->field_4 = tmp >> 16;

    if (pSprite->field_4 < 0) {
        pSprite->field_6 = 0;
        pSprite->field_4 = 0;
        mSprite_Bullet_Destroy = -1;
    }
}

int16 cFodder::Sprite_Direction_Between_Points(sSprite* pSprite, int16& pData0, int16& pData4) {
    pData0 -= pSprite->field_0;
    pData4 -= pSprite->field_4;

    int16 Data10 = 0;
    if (pData0 < 0)
        Data10 = 1;
    Data10 = -Data10;

    if (pData0 < 0)
        pData0 = -pData0;

    int16 Data14 = 0;
    if (pData4 < 0)
        Data14 = 1;
    Data14 = -Data14;

    if (pData4 < 0)
        pData4 = -pData4;

    int16 Data8 = 0x1F;
    int16 DataC = 0;
    if (pData0 <= Data8)
        if (pData4 <= Data8)
            goto loc_2A2F5;

    //seg007:0755
    ++DataC;
    Data8 <<= 1;
    ++Data8;

    //loc_2A283
    for (;;) {
        if (pData0 <= Data8)
            if (pData4 <= Data8)
                break;
        ++DataC;
        Data8 <<= 1;
        ++Data8;
        if (pData0 <= Data8)
            if (pData4 <= Data8)
                break;

        ++DataC;
        Data8 <<= 1;
        ++Data8;
        if (pData0 <= Data8)
            if (pData4 <= Data8)
                break;

        ++DataC;
        Data8 <<= 1;
        ++Data8;
    }
    //loc_2A2E5
    pData0 >>= DataC;
    pData4 >>= DataC;

loc_2A2F5:;
    pData4 <<= 5;
    pData4 |= pData0;
    //pData4 <<= 1;
    pData4 = mMap_DirectionsBetweenPoints[pData4];
    if (pData4 < 0) {
        word_3B25B = -1;
        return -1;
    }

    pData4 <<= 1;
    if (Data10 < 0) {
        //loc_2A376
        if (Data14 >= 0) {
            pData4 += 0x180;
            pData4 &= 0x1FE;
            pSprite->field_10 = pData4;

        }
        else {
            //loc_2A39F
            pData0 = 0x180;
            pData0 -= pData4;
            pData0 &= 0x1FE;
            pSprite->field_10 = pData0;
        }

        word_3B25B = 0;
        return 0;
    }

    //seg007:0804
    if (Data14 >= 0) {
        pData0 = 0x80;
        pData0 -= pData4;
        pData0 &= 0x1FE;
        pSprite->field_10 = pData0;
        word_3B25B = 0;
        return 0;
    }

    //loc_2A354
    pData4 += 0x80;
    pData4 &= 0x1FE;
    pSprite->field_10 = pData4;
    word_3B25B = 0;
    return 0;
}

void cFodder::sub_2A3D4(sSprite* pSprite) {

    mDirectionMod = 0;
    pSprite->field_3E -= 1;
    if (pSprite->field_3E >= 0)
        return;

    pSprite->field_3E = 1;
    int16 Data8 = pSprite->field_3C;
    int16 Data0 = pSprite->field_10;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0E;
    int16 Data4 = Data0;
    Data4 &= 0x0F;

    int16 DataC = Data8;
    DataC -= Data4;
    DataC &= 0x0F;
    //seg007:0920
    DataC = (int16)mSprite_Direction_Frame_Unk[DataC];
    DataC <<= 1;

    mDirectionMod = DataC;
}

void cFodder::Squad_Walk_Steps_Decrease() {

    for (int16 Data0 = 2; Data0 >= 0; --Data0) {

        if (mSquad_Walk_Target_Steps[Data0])
            --mSquad_Walk_Target_Steps[Data0];

    }
}

int16 cFodder::Map_PathCheck_CalculateTo(int16& pX1, int16& pY1, int16& pX2, int16& pY2) {
	pX1 >>= 4;
	pY1 >>= 4;
	pX2 >>= 4;
	pY2 >>= 4;

    int16 Data18 = 2;

    int16 RowBytes = mMapLoaded->getWidth();
	RowBytes <<= 1;

    mCheckPattern_Position.mX = pX1;
    mCheckPattern_Position.mY = pY1;

    Map_PathCheck_Generate(pX1, pY1, pX2, pY2, Data18, RowBytes);

    return Map_PathCheck_CanPass(pX1);
}

void cFodder::Map_PathCheck_Generate(int16& pX1, int16&  pY1, int16& pX2, int16& pY2, int16& pColumnWidth, int16& pRowWidth) {
    int16 X = pX2 - pX1;

    int16 Y = pY2 - pY1;

    int16 Data28 = pX2;

    int16 X_Move, Y_Move;

	// Target to the left?
    if (X < 0) {
        X_Move = -pColumnWidth;
		X = -X;
		pColumnWidth = -1;
    }
    else {
        X_Move = pColumnWidth;
		pColumnWidth = 1;
    }

    //loc_2A56A
	// If target position is above us
    if (Y < 0) {
        Y_Move = -pRowWidth;
		Y = -Y;
		pRowWidth = -1;
    } else {
        Y_Move = pRowWidth;
		pRowWidth = 1;
    }

    //loc_2A59D
	pX2 = 0;
    if (Y == 0)
		pX2 = 1;
    else
		pX2 = 0;

	pX2 = -pX2;

	mMap_PathToDest.clear();

loc_2A5BA:;

	// Reached target?
    if (Data28 == pX1 && pY2 == pY1)
        return;

	// Move up/down
    if (pX2 >= 0) {
		pY1 += pRowWidth;
		pX2 -= X;
		mMap_PathToDest.push_back(Y_Move);
        goto loc_2A5BA;
    }

	// Move left/right
    //loc_2A601
	pX1 += pColumnWidth;
	pX2 += Y;
	mMap_PathToDest.push_back(X_Move);
    goto loc_2A5BA;
}

int16 cFodder::Map_PathCheck_CanPass(int16& pTileHit) {

    int32 Data4 = mCheckPattern_Position.mY;

    uint8* MapTilePtr = mMap->data() + 0x60;
	uint16 TileID = 0;

    Data4 *= mMapLoaded->getWidth();
    Data4 += mCheckPattern_Position.mX;

    MapTilePtr += (Data4 << 1);
	
	for (size_t x = 0; x < mMap_PathToDest.size(); ++x) {
		int16 Movement = mMap_PathToDest[x];

        if (Movement == 0 || (x + 1) >= mMap_PathToDest.size())
            goto loc_2A728;

		if(mMap_PathToDest[x+1] == 0)
            goto loc_2A728;

        MapTilePtr += Movement;
		Movement = mMap_PathToDest[++x];

    loc_2A728:;
        MapTilePtr += Movement;

		//loc_2A6A1
		if (MapTilePtr > mMap->data() && MapTilePtr < mMap->data() + mMap->size()) {
			TileID = readLE<uint16>(MapTilePtr);
		} else
			TileID = 0;

		pTileHit = mTile_Hit[TileID & 0x1FF];

		// Tile has hit?
		if (pTileHit >= 0) {
			pTileHit &= 0x0F;

			// Check if tile is passable
			if (mTiles_NotWalkable[pTileHit]) {
				pTileHit = -1;
				return -1;
			}
		}
    }
	
	pTileHit = 0;
	return 0;
}

/**
 * Calculate the distance between two points on the map
 *
 * @return Distance between points
 */
int16 cFodder::Map_Get_Distance_BetweenPoints(int16& pPosX, int16& pPosY, int16& pPosX2, int16& pDistanceMax, int16& pPosY2) {
    const int8* Data24 = mMap_Distance_Calculations;

    pPosX2 -= pPosX;
    if (pPosX2 < 0)
        pPosX2 = -pPosX2;

    if (pPosX2 >= pDistanceMax)
        goto loc_2A7DB;

    pPosY2 -= pPosY;
    if (pPosY2 < 0)
        pPosY2 = -pPosY2;

    if (pPosY2 >= pDistanceMax)
        goto loc_2A7DB;

	pDistanceMax = 0;
    for (;;) {
        if (pPosX2 <= 0x1F)
            if (pPosY2 <= 0x1F)
                break;

        pPosX2 >>= 1;
        pPosY2 >>= 1;
        ++pDistanceMax;
    }
    //loc_2A7AD
    pPosY2 <<= 5;
    pPosY2 |= pPosX2;
    pPosX = 0;

    pPosX = Data24[pPosY2];
    pPosX <<= pDistanceMax;

    return pPosX;

loc_2A7DB:;
    pPosX = 0x3E8;
    return 0x3E8;
}

int32 cFodder::Map_Get_Distance_BetweenPositions(cPosition pPos1, cPosition pPos2, int32 pDistanceMax) {

	pPos2.mX -= pPos1.mX;
	if (pPos2.mX < 0)
		pPos2.mX = -pPos2.mX;

	if (pPos2.mX >= pDistanceMax)
		goto loc_2A7DB;

	pPos2.mY -= pPos1.mY;
	if (pPos2.mY < 0)
		pPos2.mY = -pPos2.mY;

	if (pPos2.mY >= pDistanceMax)
		goto loc_2A7DB;

	pDistanceMax = 0;
	for (;;) {
		if (pPos2.mX <= 0x1F)
			if (pPos2.mY <= 0x1F)
				break;

		pPos2.mX >>= 1;
		pPos2.mY >>= 1;
		++pDistanceMax;
	}
	//loc_2A7AD
	pPos2.mY <<= 5;
	pPos2.mY |= pPos2.mX;

	if (pPos2.mY >= sizeof(mMap_Distance_Calculations) / sizeof(mMap_Distance_Calculations[0]) - 1)
		goto loc_2A7DB;

	pPos1.mX = mMap_Distance_Calculations[pPos2.mY];
	pPos1.mX <<= pDistanceMax;

	return pPos1.mX;

loc_2A7DB:;
	return 0x3E8;
}

/**
 *
 * @param pY
 * @param pX        Returns Terrain Type
 *
 * @return False if can walk
 */
int16 cFodder::Map_Terrain_Get_Type_And_Walkable(int16& pY, int16& pX) {
    int16 Data10 = pY;
    int16 Data14 = pX;

    return Map_Terrain_Get(pY, pX, Data10, Data14);
}

/**
 *
 * @param pSprite
 * @param pY
 * @param pX        Returns Terrain Type
 *
 * @return False if can walk
 */
int16 cFodder::Map_Terrain_Get_Type_And_Walkable(sSprite* pSprite, int16& pY, int16& pX) {

    pY += pSprite->field_4;
    if (pY >= 0) {

        int16 Data14 = pY;
        pX += pSprite->field_0;
        if (pX >= 0) {
            int16 Data10 = pX;

            return Map_Terrain_Get(pY, pX, Data10, Data14);
        }
    }

    pY = -1;
    pX = 0;

    return 0;
}

int16 cFodder::Map_Terrain_Get(int16& pY, int16& pX, int16& pData10, int16& pData14) {

    if ((pY >> 4) > mMapLoaded->getHeight() || (pX >> 4) > mMapLoaded->getWidth())
        return 0;

    int32 MapPtr = (pY >> 4) * mMapLoaded->getWidth();
    MapPtr += (pX >> 4);
    MapPtr <<= 1;

    if (mMap->data() + (0x60 + MapPtr) >= mMap->data() + mMap->size())
        return 0;

    uint16 TileID = readLE<uint16>(mMap->data() + (0x60 + MapPtr)) & 0x1FF;

    // There is two tables, the HIT and the BHIT
    // HIT contains the type of terrain used by a tile, a value < 0 indicates
    // the tile contains two terrain types.
    // This is determined by looking up the terrain type and row
    //  then checking if a bit is set for the column. 
    //  The bit being set, means we use the upper 4 bits as the terrain type
    //  Not being set, means we use the lower 4 bits

    // eTerrainFeature
    int16 TerrainType = Tile_Terrain_Get(TileID, pData10, pData14);

    pY = mTiles_NotWalkable[TerrainType];
    pX = TerrainType;

    return mTiles_NotWalkable[TerrainType];
}

int16 cFodder::Map_Terrain_Get(int16 pX, int16 pY) {

    if ((pY >> 4) > mMapLoaded->getHeight() || (pX >> 4) > mMapLoaded->getWidth())
        return -1;

    int32 MapPtr = (pY >> 4) * mMapLoaded->getWidth();
    MapPtr += (pX >> 4);
    MapPtr <<= 1;

    if (mMap->data() + (0x60 + MapPtr) >= mMap->data() + mMap->size())
        return -1;

    uint16 TileID = readLE<uint16>(mMap->data() + (0x60 + MapPtr)) & 0x1FF;

    // eTerrainFeature
    return Tile_Terrain_Get(TileID, pX, pY);
}

int16 cFodder::Tile_Terrain_Get(const int16 pTileID, int16 pX, int16 pY) {
    // There is two tables, the HIT and the BHIT
    // HIT contains the type of terrain used by a tile, a value < 0 indicates
    // the tile contains two terrain types.
    // This is determined by looking up the terrain type and row
    //  then checking if a bit is set for the column. 
    //  The bit being set, means we use the upper 4 bits as the terrain type
    //  Not being set, means we use the lower 4 bits

    int16 TerrainType = mTile_Hit[pTileID];

    if (TerrainType < 0) {
        pX >>= 1;
        pX &= 0x07;

        int16 TilePixel = 7;
        TilePixel -= pX;

        // Y
        pY >>= 1;
        pY &= 0x07;

        int8 RowTerrainType = mTile_BHit[pTileID][pY];

        // If the bit for this X position is set, we use the UpperBits for the terrain type
        if (RowTerrainType & (1 << TilePixel))
            TerrainType >>= 4;
    }

    return (TerrainType & 0x0F);
}

void cFodder::Squad_Walk_Target_Set(int16 pTargetX, int16 pTargetY, int16 pSquadNumber, int16 pData10) {
    int16 Data1C;
    sSprite** Saved_Data24 = 0;

    if (pSquadNumber < 0 || pSquadNumber >= 3)
        return;

    pData10 = mSquad_Walk_Target_Indexes[pSquadNumber];

    sSprite** Data24 = mSquads[pSquadNumber];

    // Currently walking to a target?
    if (mSquad_Walk_Target_Steps[pSquadNumber]) {

        // Add this target to the queue
        mSquad_WalkTargets[pSquadNumber][pData10].mX = pTargetX;
        mSquad_WalkTargets[pSquadNumber][pData10].mY = pTargetY;

        mSquad_WalkTargets[pSquadNumber][pData10 + 1].asInt = -1;

        if (pData10 < (sizeof(mSquad_WalkTargets[pSquadNumber]) / sizeof(mSquad_WalkTargets[pSquadNumber][0])))   // 0x74
            pData10++;

        mSquad_Walk_Target_Indexes[pSquadNumber] = pData10;
        goto loc_2ABF8;
    }

    //loc_2AA4C
    pData10 = 0;
    Data1C = -1;

    Saved_Data24 = Data24;

    for (;;) {
        if (*Data24++ == INVALID_SPRITE_PTR)
            break;

        ++Data1C;
        pData10 += 1;
    }

    if (mSquad_WalkTargetX) {
        mSquad_WalkTargets[pSquadNumber][pData10].mX = mSquad_WalkTargetX;
        mSquad_WalkTargets[pSquadNumber][pData10].mY = mSquad_WalkTargetY;
        pData10 += 1;
    }
    //loc_2AAAE
    mSquad_WalkTargets[pSquadNumber][pData10].mX = pTargetX;
    mSquad_WalkTargets[pSquadNumber][pData10].mY = pTargetY;

    mSquad_WalkTargetX = pTargetX;
    mSquad_WalkTargetY = pTargetY;

    mSquad_WalkTargets[pSquadNumber][pData10 + 1].asInt = -1;

    Data24 = Saved_Data24;
    if (Data1C < 0)
        return;

    mSquad_Walk_Target_Indexes[pSquadNumber] = pData10;
    mSquad_Walk_Target_Indexes[pSquadNumber]++;

    for (;;) {
        sSprite* Data28 = *Data24++;

        if (Data28 == INVALID_SPRITE_PTR)
            break;

        if (Data28->field_5B)
            continue;

        if (Data28->field_6E)
            continue;

        Data28->field_4C = 0;
        Data28->field_4D = 0;
        Data28->field_45 = 0;
        Data28->field_43 = 0;
        Data28->field_40 = pData10;
        Data28->field_40++;
        pData10--;

        mSquad_WalkTargets[pSquadNumber][pData10].mX = Data28->field_0;
        mSquad_WalkTargets[pSquadNumber][pData10].mY = Data28->field_4;

        Data28->field_26 = pTargetX;
        Data28->field_28 = pTargetY;

        pTargetX = Data28->field_0;
        pTargetY = Data28->field_4;
    }

loc_2ABF8:;
    mSquad_Walk_Target_Steps[pSquadNumber] = 8;
}

int16 cFodder::Squad_Member_Sprite_Hit_In_Region(sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14) {
    int16 Data4;

    if (mPhase_Finished) {
        word_3AA45 = 0;
        return 0;
    }

    mSprites_Found_Count = 0;

    int16 Data0;

    for( auto& Troop : mGame_Data.mSoldiers_Allocated ) {

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        if (pData8 > (Troop.mSprite->field_0 + 9))
            continue;

        if (pDataC < Troop.mSprite->field_0)
            continue;

        if (pData10 > (Troop.mSprite->field_4 - 6))
            continue;

        if (pData14 < (Troop.mSprite->field_4 - 13))
            continue;

        if (Troop.mSprite->field_20 >= 0x0B)
            continue;

        if (!word_3AA45)
            goto loc_2AD3D;

        if (Troop.mSprite->field_6E)
            continue;

        if (pSprite->field_3A <= 4)
            goto loc_2AD25;

        if (Troop.mSprite->field_52 >= 5)
            goto loc_2AD2D;

        if (Troop.mSprite->field_4F)
            goto loc_2AD2D;


        Data0 = Troop.mSprite->field_32;
        if (word_3BED5[Data0]) {
        loc_2AD25:;
            Data0 = 0;
            goto loc_2AD36;
        }

    loc_2AD2D:;
        Data0 = tool_RandomGet() & 7;

    loc_2AD36:;
        if (!Data0)
            goto loc_2ADC3;
        continue;

    loc_2AD3D:;
        Data0 = pDataC;
        Data0 -= pData8;
        Data0 >>= 1;
        Data0 += pData8;

        Data4 = pData14;
        Data4 -= pData10;
        Data4 >>= 1;
        Data4 += pData10;

        Sprite_Direction_Between_Points(Troop.mSprite, Data0, Data4);

        Troop.mSprite->field_10 += 0x100;
        Troop.mSprite->field_10 &= 0x1FE;

    loc_2ADC3:; // Hit
        if (Troop.mSprite->field_18)
            goto loc_2ADFF;

        Troop.mSprite->field_10 = pSprite->field_10;
        Troop.mSprite->field_12 = pSprite->field_10;

    loc_2ADFF:;
        Troop.mSprite->field_38 = eSprite_Anim_Hit;
        Troop.mSprite->field_64 = -1;
        mSprites_Found_Count += 1;

        if (word_3AA45)
            break;
    }

    word_3AA45 = 0;
    return mSprites_Found_Count;
}

const sSpriteSheet* cFodder::Sprite_Get_Sheet(int16 pSpriteType, int16 pFrame) {
    const sSpriteSheet* Sheet = &mSprite_SheetPtr[pSpriteType][pFrame];

    return Sheet;
}

void cFodder::sub_2AEB6(int16 pColumns, int16 pRows, int16* pData8, int16* pDataC) {
    int32 Columns = pColumns * *pData8;
    int32 Rows = pRows * *pData8;

    // Return
    *pDataC = (int16)(Rows / 0x64);
    *pData8 = (int16)(Columns / 0x64);
}


void cFodder::Sprite_SetDirectionMod(sSprite* pSprite) {
    mDirectionMod = 0;
    int16 Data0 = pSprite->field_10;

    Data0 -= pSprite->field_3C;
    word_3B25D = Data0;
    if (Data0 < 0)
        word_3B25D = -word_3B25D;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    int16 Data4 = Data0;
    Data4 &= 0x0F;

    mDirectionMod = (int16)mSprite_Direction_Frame_Unk[Data4];
}

void cFodder::Sprite_Vehicle_Direction_Update(sSprite* pSprite, int16& pData1C) {
    int16 Data4 = mSprite_Field10_Saved;
    Data4 >>= 5;
    Data4 -= 1;
    Data4 ^= 0x0F;

    int16 Data0 = pSprite->field_A;

    Data0 -= Data4;
    Data4 = Data0;
    Data4 &= 0x0F;
    int16 DataC = mSprite_Direction_Frame_Unk[Data4];
    if (!DataC)
        return;

    if (DataC == pData1C)
        goto loc_2B21D;

    if (!pData1C)
        goto loc_2B21D;

    DataC = pData1C;

loc_2B21D:;
    pSprite->field_A += DataC;
    pSprite->field_A &= 0x0F;

}

int16 cFodder::Map_Terrain_Get_Moveable_Wrapper(const int8* pMovementData, int16& pX, int16& pY, int16& pData10, int16& pData14) {

    pData10 = pX;
    pData14 = pY;

    return Map_Terrain_Get_Moveable(pMovementData, pX, pY);
}

int16 cFodder::Map_Terrain_Get_Moveable(const int8* pMovementData, int16& pX, int16& pY) {
    uint32 DataC = pY;
    uint32 Data8 = pX;

    if (pY < 0)
        DataC = 0;
    if (pX < 0)
        Data8 = 0;

    DataC >>= 4;

    DataC *= mMapLoaded->getWidth();
    Data8 >>= 4;

    DataC += Data8;
    DataC <<= 1;

    if (mMap->data() + 0x60 + DataC >= mMap->data() + mMap->size())
        return pMovementData[0];

    int16 Data0 = readLE<uint16>(mMap->data() + 0x60 + DataC);
	Data0 &= 0x1FF;

    int16 Data4 = mTile_Hit[Data0];

    if (Data4 >= 0) {
        Data4 &= 0x0F;
        return pMovementData[Data4];
    }

    Data8 = Data4;
    Data4 &= 0x0F;

    Data0 = pMovementData[Data4];
    Data8 >>= 4;
    Data8 &= 0x0F;

    Data0 |= pMovementData[Data8];

    pX = Data8;
    pY = DataC;
    return Data0;
}

void cFodder::Map_Get_Distance_BetweenPoints_Within_640(int16& pX, int16& pY, int16& pX2, int16& pY2) {
    int16 Data10 = 0;

    pX2 -= pX;
    if (pX2 < 0)
        pX2 = -pX2;

    if (pX2 >= getWindowWidth() * 2)
        goto loc_2B403;

    pY2 -= pY;
    if (pY2 < 0)
        pY2 = -pY2;

    if (pY2 >= getWindowWidth() * 2)
        goto loc_2B403;

    for (;;) {
        if (pX2 <= 31)
            if (pY2 <= 31)
                break;

        pX2 >>= 1;
        pY2 >>= 1;
        Data10 += 1;
    }

    pY2 <<= 5;
    pY2 |= pX2;
    pX = 0;
    pX = mMap_Distance_Calculations[pY2];

    pX <<= Data10;
    return;

loc_2B403:;
    pX = getWindowWidth() * 2;
}

bool cFodder::MapTile_Update_Position() {

    MapTile_Update_Y();
    MapTile_Update_X();

    int16 TileColumns = mMapTile_DrawX - mMapTile_Column_CurrentScreen;
    int16 TileRows = mMapTile_DrawY - mMapTile_Row_CurrentScreen;

    mMapTile_Column_CurrentScreen = mMapTile_DrawX;
    mMapTile_Row_CurrentScreen = mMapTile_DrawY;

    if (TileColumns) {
        if (TileColumns < 0)
            MapTile_Move_Left(-TileColumns);
        else
            MapTile_Move_Right(TileColumns);
    }

    if (TileRows) {
        if (TileRows < 0)
            MapTile_Move_Up(-TileRows);
        else
            MapTile_Move_Down(TileRows);
    }

    if (TileColumns || TileRows) {
        if (!mStartParams->mDisableVideo)
            mGraphics->MapTiles_Draw();
        return true;
    }

    return false;
}

void cFodder::MapTile_Move_Right(int16 pPanTiles) {

    for (; pPanTiles > 0; --pPanTiles) {

        ++mMapTile_ColumnOffset;
        mMapTile_ColumnOffset &= 0x0F;
        if (!mMapTile_ColumnOffset) {
            mMapTile_Ptr += 2;
            ++mMapTile_MovedHorizontal;
        }
    }
}

void cFodder::MapTile_Move_Left(int16 pPanTiles) {

    for (; pPanTiles > 0; --pPanTiles) {

        --mMapTile_ColumnOffset;
        mMapTile_ColumnOffset &= 0x0F;
        if (mMapTile_ColumnOffset == 0x0F) {
            mMapTile_Ptr -= 2;
            --mMapTile_MovedHorizontal;
        }
    }
}

void cFodder::MapTile_Move_Down(int16 pPanTiles) {

    for (; pPanTiles > 0; --pPanTiles) {

        ++mMapTile_RowOffset;
        mMapTile_RowOffset &= 0x0F;
        if (!mMapTile_RowOffset) {
            mMapTile_Ptr += (mMapLoaded->getWidth() << 1);
            ++mMapTile_MovedVertical;
        }
    }
}

void cFodder::MapTile_Move_Up(int16 pPanTiles) {

    for (; pPanTiles > 0; --pPanTiles) {

        --mMapTile_RowOffset;
        mMapTile_RowOffset &= 0x0F;
        if (mMapTile_RowOffset == 0x0F) {
            mMapTile_Ptr -= (mMapLoaded->getWidth() << 1);
            --mMapTile_MovedVertical;
        }
    }
}

void cFodder::MapTile_Update_Y() {
    int16 Data0 = mMapTile_MoveDirectionY;

    if (Data0 < 0)
        Data0 = -1;
    else
        Data0 = 1;
    // Direction Change?
    if (Data0 != mMapTile_MoveDirectionY_Previous) {
        mMapTile_MoveDirectionY_Previous = Data0;
        Data0 = mMapTile_SpeedY >> 16;
        int16 Data4 = mMapTile_SpeedY_Previous;

        Data0 >>= 4;
        Data4 >>= 4;
        if (Data4 != Data0)
            return;
    }

    mCamera_MovePauseY = 0;
    mMapTile_DrawY = mMapTile_TargetY >> 16;
    mMapTile_SpeedY_Previous = mMapTile_SpeedY >> 16;
}

void cFodder::MapTile_Update_X() {
    int16 Data0 = mMapTile_MoveDirectionX;

    if (Data0 < 0)
        Data0 = -1;
    else
        Data0 = 1;

    if (Data0 != mMapTile_MoveDirectionX_Previous) {
        mMapTile_MoveDirectionX_Previous = Data0;
        Data0 = mMapTile_SpeedX >> 16;
        int16 Data4 = mMapTile_SpeedX_Previous;
        Data0 >>= 4;
        Data4 >>= 4;
        if (Data0 != Data4)
            return;
    }

    mCamera_MovePauseX = 0;
    mMapTile_DrawX = mMapTile_TargetX >> 16;
    mMapTile_SpeedX_Previous = mMapTile_SpeedX >> 16;
}

int32 cFodder::MapTile_Get(const size_t pTileX, const size_t pTileY) {

    if ((int32) pTileX > mMapLoaded->getWidth() || (int32) pTileY > mMapLoaded->getHeight())
        return -1;

    size_t Tile = (((pTileY * mMapLoaded->getWidth()) + pTileX)) + mMapLoaded->getWidth();

    uint8* CurrentMapPtr = mMap->data() + mMapTile_Ptr + (Tile * 2);
    if (CurrentMapPtr > mMap->data() + mMap->size())
        return -1;

    return readLE<int16>(CurrentMapPtr);
}

/**
 * Sets a map tile to a certain ID, positioned based on the current camera
 */
void cFodder::MapTile_Set(const size_t pTileX, const size_t pTileY, const size_t pTileID) {

    if ((int32) pTileX > mMapLoaded->getWidth() || (int32) pTileY > mMapLoaded->getHeight())
        return;

    size_t Tile = (((pTileY * mMapLoaded->getWidth()) + pTileX)) + mMapLoaded->getWidth();

    uint8* CurrentMapPtr = mMap->data() + mMapTile_Ptr + (Tile * 2);
    if (CurrentMapPtr > mMap->data() + mMap->size())
        return;

    writeLEWord(CurrentMapPtr, (uint16)pTileID);
}

sSprite* cFodder::Sprite_Add(size_t pSpriteID, int16 pSpriteX, int16 pSpriteY) {

    int16 Data0 = 3;

    sSprite* First = 0, *Second = 0, *Third = 0, *Fourth = 0;

    if (Sprite_Get_Free_Max42(Data0, First, Second))
        return First;

    Third = Second + 1;
    Fourth = Third + 1;

    First->field_18 = (int16)pSpriteID;
    First->field_0 = pSpriteX;
    First->field_4 = pSpriteY;
    First->field_26 = pSpriteX;
    First->field_28 = pSpriteY;

    switch (pSpriteID) {
    case eSprite_BoilingPot:                        // 1 Null
    case eSprite_Tank_Human:
    case eSprite_VehicleNoGun_Human:
    case eSprite_VehicleGun_Human:
    case eSprite_VehicleNoGun_Enemy:
    case eSprite_VehicleGun_Enemy:
    case eSprite_Vehicle_Unk_Enemy:
	case eSprite_Enemy_Leader:
        Second->field_18 = eSprite_Null;
        Second->field_0 = pSpriteX;
        Second->field_4 = pSpriteY;
        Second->field_26 = pSpriteX;
        Second->field_28 = pSpriteY;
        break;

    case eSprite_Helicopter_Grenade_Enemy:          // 3 Nulls
    case eSprite_Helicopter_Grenade2_Enemy:
    case eSprite_Helicopter_Missile_Enemy:
    case eSprite_Helicopter_Homing_Enemy:
    case eSprite_Helicopter_Homing_Enemy2:
        Second->field_18 = eSprite_Null;
        Second->field_0 = pSpriteX;
        Second->field_4 = pSpriteY;
        Second->field_26 = pSpriteX;
        Second->field_28 = pSpriteY;
        Third->field_18 = eSprite_Null;
        Third->field_0 = pSpriteX;
        Third->field_4 = pSpriteY;
        Third->field_26 = pSpriteX;
        Third->field_28 = pSpriteY;
        Fourth->field_18 = eSprite_Null;
        Fourth->field_0 = pSpriteX;
        Fourth->field_4 = pSpriteY;
        Fourth->field_26 = pSpriteX;
        Fourth->field_28 = pSpriteY;
        break;

        // Fall Through
    case eSprite_Helicopter_Grenade2_Human:         // 2 Nulls
    case eSprite_Helicopter_Grenade_Human:
    case eSprite_Helicopter_Missile_Human:
    case eSprite_Helicopter_Homing_Human:
    case eSprite_Helicopter_Grenade2_Human_Called:
    case eSprite_Helicopter_Grenade_Human_Called:
    case eSprite_Helicopter_Missile_Human_Called:
    case eSprite_Helicopter_Homing_Human_Called:
    case eSprite_Tank_Enemy:
        Second->field_18 = eSprite_Null;
        Second->field_0 = pSpriteX;
        Second->field_4 = pSpriteY;
        Second->field_26 = pSpriteX;
        Second->field_28 = pSpriteY;
        Third->field_18 = eSprite_Null;
        Third->field_0 = pSpriteX;
        Third->field_4 = pSpriteY;
        Third->field_26 = pSpriteX;
        Third->field_28 = pSpriteY;

        break;
    }
  
    (*this.*mSprite_Function[First->field_18])(First);

    if(Second && Second->field_0 != -32768)
        (*this.*mSprite_Function[Second->field_18])(Second);

    if(Third && Third->field_0 != -32768)
        (*this.*mSprite_Function[Third->field_18])(Third);

    if (Fourth && Fourth->field_0 != -32768)
        (*this.*mSprite_Function[Fourth->field_18])(Fourth);

    return First;
}

void cFodder::Squad_Troops_Count() {
    sSprite** Data34 = mSprite_TroopsAlive;

    mSquads_TroopCount[0] = 0;
    mSquads_TroopCount[1] = 0;
    mSquads_TroopCount[2] = 0;
    mSquads_TroopCount[3] = 0;

    int16 TotalTroops = 0;

    mSquad_0_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_1_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_2_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_3_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_4_Sprites[0] = INVALID_SPRITE_PTR;

    // Loop through all mission troops
    for( auto& Troop : mGame_Data.mSoldiers_Allocated ) {

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        sSprite* Sprite = Troop.mSprite;

        // Sprite is not invincible
        if (!(Sprite->field_75 & eSprite_Flag_Invincibility)) {

            // Is dying
            if (Sprite->field_38 < eSprite_Anim_Slide1) {
                if (Sprite->field_38) {

                    // Not in vehicle
                    if (!Sprite->field_6E)
                        continue;

                    Sprite->field_38 = eSprite_Anim_None;
                }
            }
        }
        //loc_2D165
        if (Sprite->field_32 < 0)
            continue;

        uint8 Data10 = mSquads_TroopCount[Sprite->field_32] & 0xFF;
        mSquads_TroopCount[Sprite->field_32] += 1;
        ++TotalTroops;

        sSprite** Data30 = mSquads[Sprite->field_32];
        //seg009:0151
        Data30[Data10] = Sprite;
        Data30[Data10 + 1] = INVALID_SPRITE_PTR;

        *Data34 = Sprite;
        ++Data34;
    }
    //seg009:019E

    if (!TotalTroops)
        mPhase_TryAgain = true;

    for (int16 Data0 = 2; Data0 >= 0; --Data0) {
        if (mSquads_TroopCount[Data0])
            continue;

        mSquad_Join_TargetSquad[Data0] = -1;
    }

    *Data34 = INVALID_SPRITE_PTR;
}

int16 cFodder::Mission_Troop_GetDeviatePotential(sMission_Troop* pData24) {
    int16 Data0 = 0;
    
    if (pData24)
        Data0 = pData24->mRank;

    Data0 += 8;

    if (Data0 > 0x0F)
        Data0 = 0x0F;

    return mSprite_Bullet_UnitData[Data0].mDeviatePotential;
}

void cFodder::Squad_Member_Rotate_Can_Fire() {
    if (mSquad_Member_Fire_CoolDown_Override)
        mSquad_Member_Fire_CoolDown_Override = false;
    else {
        mSquad_Member_Fire_CoolDown -= 2;
        if (mSquad_Member_Fire_CoolDown >= 0)
            return;
    }

    //loc_2D2EB
    sSprite* Data20 = mSquad_Leader;
    if (Data20 == INVALID_SPRITE_PTR || Data20 == 0)
        return;

    sMission_Troop* Dataa20 = Data20->field_46_mission_troop;
    if (Dataa20 == 0)
        return;

    mSquad_Member_Fire_CoolDown = mSprite_Weapon_Data.mCooldown;
    mSprite_Bullet_Time_Modifier = 0;
    mSprite_Bullet_Fire_Speed_Modifier = 0;

    // TODO: 0x0F is the max rank, so what was this code for?
    {
        if (Dataa20->mRank > 0x14) {
            if (mSquad_Selected >= 0) {
                if (mSquads_TroopCount[mSquad_Selected] == 1) {
                    mSquad_Member_Fire_CoolDown = 1;
                    mSprite_Bullet_Time_Modifier = -3;
                    mSprite_Bullet_Fire_Speed_Modifier = 0x0A;
                }
            }
        }
    }

    //loc_2D381
    if (mSquad_Selected < 0)
        return;

    int16 TroopsInSquad = (mSquads_TroopCount[mSquad_Selected] & 0xFF) - 1;
    int16 LastTroopNumber = TroopsInSquad;

    if (LastTroopNumber < 0)
        return;

    // Order based on number of troops in squad
    const std::vector<int16>& Rotate_Fire_Order = mSoldier_Squad_Fire_RotationOrder[TroopsInSquad];
    if (mTroop_Rotate_Next >= Rotate_Fire_Order.size())
        mTroop_Rotate_Next = 0;

    size_t Data0 = mTroop_Rotate_Next;

    // Squad Member Number
    int16 Data4 = Rotate_Fire_Order[Data0];

    // Past the last troop in squad?
    if (Data4 < 0 || Data4 > LastTroopNumber) {
        Data4 = 0;
        Data0 = 0;
    }

    // Next Troop in Squad
    Data0++;
    if (Rotate_Fire_Order[Data0] < 0)
        Data0 = 0;

    mTroop_Rotate_Next = Data0;

    // Can the squad member use their weapon
    Data20 = mSquads[mSquad_Selected][Data4];
    if (Data20->field_45)
        return;

    // Soldier fires weapon
    Data20->field_4A = -1;
}

int16 cFodder::Sprite_Find_In_Region(sSprite* pSprite, sSprite*& pData24, int16 pData8, int16 pDataC, int16 pData10, int16 pData14) {

    if (mPhase_Finished) {
        word_3AA45 = 0;
        return 0;
    }

    mSprites_Found_Count = 0;

    pData24 = mSprites.data();

    for (int32 Data1C = mParams->getSpritesMax() - 2; Data1C >= 0; --Data1C, ++pData24) {
        int16 Data4 = pData24->field_18;

        if (!mSprite_Can_Be_RunOver[Data4])
            continue;

        if (pData24->field_0 < 0)
            continue;

        if (pDataC < pData24->field_0)
            continue;

        int16 Data0 = pData24->field_0;
        Data0 += mSprite_Width[Data4];
        if (pData8 > Data0)
            continue;

        Data0 = pData24->field_4;
        Data0 -= mSprite_Height_Top[Data4];
        if (pData14 < Data0)
            continue;

        //seg009:050C
        Data0 = pData24->field_4;
        Data0 += mSprite_Height_Bottom[Data4];

        if (pData10 > Data0)
            continue;

        int16 Data18 = pSprite->field_20;
        Data0 = mSprite_Height_Top[Data4];
        Data0 += 8;

        Data0 += pData24->field_20;

        if (Data18 > Data0)
            continue;
        Data18 += 0x0B;

        if (Data18 < pData24->field_20)
            continue;

        if (pData24->field_18 != eSprite_Player)
            goto loc_2D5FA;

        if (pData24->field_38 != eSprite_Anim_Die5)
            continue;

    loc_2D5FA:;
        if (pSprite->field_18 == eSprite_Explosion2)
            goto AnimDie3;

        if (pSprite->field_18 == eSprite_Building_Explosion)
            goto loc_2D62C;

        if (pSprite->field_18 != eSprite_Explosion)
            goto loc_2D642;

        goto loc_2D62C;

    AnimDie3:;
        pData24->field_38 = eSprite_Anim_Die3;
        goto loc_2D636;

    loc_2D62C:;
        pData24->field_38 = eSprite_Anim_Die1;

    loc_2D636:;
        pData24->field_64 = -1;
        goto loc_2D6ED;

    loc_2D642:;
        if (mSprite_Can_Be_RunOver[Data4] == 2)
            goto loc_2D659;

        mSprites_Found_Count = 1;
        goto loc_2D705;
    loc_2D659:;

        pData24->field_38 = eSprite_Anim_Hit;
        pData24->field_64 = -1;
        Data0 = pSprite->field_10;
        Data4 = Data0;
        Data0 += 0x368;
        Data4 += 0x468;

        pData8 = pData24->field_10;
        pDataC = pData8;
        pData8 += 0x368;
        pDataC += 0x468;

        pData24->field_59 = 0;
        if (pData8 < Data0)
            goto loc_2D6D2;

        if (pDataC > Data0)
            goto loc_2D6D2;

        pData24->field_59 = -1;

    loc_2D6D2:;
        if (pData8 < Data4)
            goto loc_2D6ED;

        if (pDataC > Data4)
            goto loc_2D6ED;

        pData24->field_59 = -1;

    loc_2D6ED:;
        mSprites_Found_Count = -1;
        pData24->field_10 = pSprite->field_10;

    loc_2D705:;
        if (word_3AA45)
            break;
    }

    word_3AA45 = 0;
    return mSprites_Found_Count;
}

void cFodder::Sprite_Handle_Player_DestroyAll() {

    for(auto& Sprite : mSprites) {

        if (Sprite.field_0 == -32768)
            continue;

        if (Sprite.field_18 == eSprite_Player)
            Sprite_Troop_Dies(&Sprite);
    }
}

void cFodder::Sprite_Handle_Player_Destroy_Unk() {

    for (auto& Sprite : mSprites) {

        if (Sprite.field_0 == -32768)
            continue;

        if (Sprite.field_18 != eSprite_Player)
            continue;

        if (!Sprite.field_38)
            continue;

        if (Sprite.field_38 >= eSprite_Anim_Slide1)
            continue;

        Sprite_Troop_Dies(&Sprite);
    }
}

void cFodder::Squad_Prepare_GrenadesAndRockets() {
    mSquad_Grenades[0] = mGame_Data.mGamePhase_Data.mSoldiers_Available << 1;
    mSquad_Rockets[0] = mGame_Data.mGamePhase_Data.mSoldiers_Available;

    if (mVersionCurrent->isCannonFodder1()) {
        if (mVersionCurrent->isAmigaTheOne()) {
            mSquad_Grenades[0] += mGame_Data.mGamePhase_Data.mSoldiers_Available;

            if (mGame_Data.mMission_Number == 1 && mGame_Data.mMission_Phase < 3)
                mSquad_Grenades[0] = 0;

            mSquad_Rockets[0] = 0;
        }
        else {

            if (mGame_Data.mMission_Number < 4 || (mGame_Data.mMission_Number == 4 && mGame_Data.mMission_Phase < 2))       // Original CF: Map 5
                mSquad_Grenades[0] = 0;

            if (mGame_Data.mMission_Number < 5 || (mGame_Data.mMission_Number == 5 && mGame_Data.mMission_Phase < 2))       // Original CF: Map 10
                mSquad_Rockets[0] = 0;
        }
    }
    else {
        if (mGame_Data.mMission_Number < 3)       // Original CF2: Map 5
            mSquad_Grenades[0] = 0;

        if (mGame_Data.mMission_Number < 4 || (mGame_Data.mMission_Number == 4 && mGame_Data.mMission_Phase < 2))       // Original CF2: Map 10
            mSquad_Rockets[0] = 0;
    }

    // If grenades/rockets are provided in the campaign data, override with those
    if (mGame_Data.mPhase_Current->mGrenades != -1)
        mSquad_Grenades[0] = mGame_Data.mPhase_Current->mGrenades;

    if (mGame_Data.mPhase_Current->mRockets != -1)
        mSquad_Rockets[0] = mGame_Data.mPhase_Current->mRockets;
}

void cFodder::Sprite_Aggression_Set() {
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin = mGame_Data.mPhase_Current->mAggression.mMin;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax = mGame_Data.mPhase_Current->mAggression.mMax;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage = mGame_Data.mPhase_Current->mAggression.getAverage();

    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext = mGame_Data.mPhase_Current->mAggression.getAverage();
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement = 1;

    for(auto& Sprite : mSprites ) {

        if (Sprite.field_0 == -32768)
            continue;

        if (Sprite.field_18 == eSprite_Enemy_Rocket || Sprite.field_18 == eSprite_Enemy)
            Sprite_Handle_Enemy_Aggression_Set(&Sprite);
    }
}

/**
 * Set the aggression of a sprite, then calculate the next aggression rate
 *
 * This moves by 'Increment' in an up or down
 * direction inside the min/max range for this map
 */
void cFodder::Sprite_Handle_Enemy_Aggression_Set(sSprite* pSprite) {

    int16 Next = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext;
    pSprite->field_62 = Next;

    int16 Increment = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement;
    Next += Increment;

    if (Increment >= 0) {

        if (Next >= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax) {
            Next = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
            Increment = -Increment;
        }
    }
    else {
        if (Next < 0 || Next <= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin) {
            Next = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin;
            Increment = -Increment;
        }
    }
    //loc_2D90D
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement = Increment;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext = Next;
}

int16 cFodder::Sprite_Next_WalkTarget_Set(sSprite* pSprite) {

    int16 Data0 = mSquad_WalkTargets[pSprite->field_32][pSprite->field_40].mX;
    int16 Data4 = mSquad_WalkTargets[pSprite->field_32][pSprite->field_40].mY;

    // Do we have a valid walk target?
    if (Data0 >= 0) {
        pSprite->field_26 = Data0;
        pSprite->field_28 = Data4;
        pSprite->field_40++;
        pSprite->field_42 = -1;
        return 0;
    }

    //loc_2D9C0
    if (!pSprite->field_42)
        return -1;

    pSprite->field_42 = 0;

    return Squad_Join(pSprite);
}

int16 cFodder::Squad_Join(sSprite* pSprite) {
    int16 Data14 = pSprite->field_32;
    int16 Data0;

    int16 Data18 = mSquad_Join_TargetSquad[Data14];
    if (Data18 < 0)
        return -1;

    Data18 &= 0xFF;

    if (mSquads_TroopCount[Data14] > 8)
        return -1;

    sMission_Troop* Dataa24 = pSprite->field_46_mission_troop;
    Dataa24->mSelected &= 0xFE;

    sMapTarget* Data24 = mSquad_WalkTargets[Data18];
    Data0 = 0;

    for (;;) {

        sMapTarget eax = *Data24++;
        if (eax.asInt < 0)
            break;

        Data0++;
    }

    pSprite->field_40 = Data0;
    pSprite->field_32 = Data18;
    Squad_Troops_Count();

    mSquad_Selected = Data18;

    mSquad_Grenades[Data18] += mSquad_Grenades[Data14];
    mSquad_Rockets[Data18] += mSquad_Rockets[Data14];

    //seg010:0232
    mSquad_Grenades[Data14] = 0;
    mSquad_Rockets[Data14] = 0;

    sub_305D5(pSprite);
    mGUI_Sidebar_Setup = 0;

    return -1;
}

int16 cFodder::Squad_Join_Check(sSprite* pSprite) {

    if (mSquad_Join_TargetSquad[pSprite->field_32] < 0)
        return -1;

    sSprite* Dataa2C = mSquad_Join_TargetSprite[pSprite->field_32];
    if (Dataa2C->field_18 != eSprite_Player)
        return -1;

    sSprite** Data30 = mSquads[mSquad_Selected];
    int16 MaxDistance = 0;

    for (MaxDistance = 0x0C;;) {
        if (*Data30 == INVALID_SPRITE_PTR)
            break;

        sSprite* eax = *Data30++;

        if (pSprite == eax)
            break;
        MaxDistance += 8;
    }

    int16 Data0 = Dataa2C->field_0;
    int16 Data4 = Dataa2C->field_4;
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 <= MaxDistance)
        return Squad_Join(pSprite);

    return 0;
}

void cFodder::Squad_Walk_Target_Update(int16 pData0) {
    mSquad_WalkTargets[pData0]->asInt = -1;

    sSprite** Data24 = mSquads[pData0];

    for (;;) {

        if (*Data24 == INVALID_SPRITE_PTR)
            break;

        sSprite* Data28 = *Data24++;
        Data28->field_40 = 0;
        Data28->field_26 = Data28->field_0;
        Data28->field_28 = Data28->field_4;
    }

}

void cFodder::Sprite_Handle_Explosion_MapTiles(sSprite* pSprite) {

    int16 Data0 = pSprite->field_0 + 4;
    int16 Data4 = pSprite->field_4 - 8;

    const int16* Data24 = mSprite_Explosion_Positions;

    for (; *Data24 != -32768;) {

        Data0 += *Data24++;
        Data4 += *Data24++;

        // Dont use negative map x/y coorindates
        if (Data0 < 0 || Data4 < 0) {
            continue;
        }

        mMap_Destroy_Tiles.emplace_back( Data0, (pSprite->field_18 == eSprite_Explosion2) ? -Data4 : Data4 );
    }
}

void cFodder::Map_Destroy_Tiles() {
    uint8* MapPtr = 0;
    const int16* IndestructibleTypes = 0;
    int32 Data0, Data4, MapTile, TileType;
    sMapPosition LastTile;

    if (mMap_Destroy_Tiles_Countdown) {
        --mMap_Destroy_Tiles_Countdown;
        goto loc_2DFC7;
    }


    for (auto& Tile : mMap_Destroy_Tiles) {
        bool TriggerExplosion = false;

        LastTile = Tile;

        Data0 = Tile.mX;
        Data4 = Tile.mY;

        mMap_Destroy_Tile_X = Tile.mX;
        mMap_Destroy_Tile_Y = Tile.mY;

        if (mMap_Destroy_Tile_Y < 0) {
            TriggerExplosion = true;
            Data4 = -Data4;
            mMap_Destroy_Tile_Y = -Tile.mY;
        }
        //loc_2DE89
        Data4 >>= 4;
        Data4 *= mMapLoaded->getWidth();

        Data0 >>= 4;
        Data4 += Data0;
        Data4 <<= 1;

        // In some cases, tiles outside the map can be 'destroyed'. This continue prevents memory corruption
        if ((size_t)(0x60 + Data4) >= mMap->size())
            continue;

        MapPtr = mMap->data() + 0x60 + Data4;
        MapTile = readLE<uint16>(MapPtr);
        MapTile &= 0x1FF;
        TileType = MapTile;

        Data4 = mTile_Destroy_Swap[MapTile];
        if (Data4 < 0)
            continue;

        if (TriggerExplosion)
            goto loc_2DF7B;

        IndestructibleTypes = mTiles_Indestructible[mMapLoaded->getTileType()];

        int16 ax;
        do {
            if (*IndestructibleTypes < 0)
                goto loc_2DF7B;

            ax = *IndestructibleTypes;
            IndestructibleTypes++;

        } while (TileType != ax);
    }

    mMap_Destroy_Tiles.clear();
    return;

    //seg010:064C
        //UNUNSED BLOCK

loc_2DF7B:;
    Data0 = Sprite_Create_Building_Explosion_Wrapper(mMap_Destroy_Tile_X, mMap_Destroy_Tile_Y);
    if (Data0)
        return;

    mMap_Destroy_Tiles_Countdown = 1;
    writeLEWord(MapPtr, Data4);
    mMap_Destroy_Tile_LastTile = Data4;

    while (!(*mMap_Destroy_Tiles.begin() == LastTile))
        Map_Destroy_Tiles_Next();

    Map_Destroy_Tiles_Next();

loc_2DFC7:;
    int16 ax = (mMap_Destroy_Tile_X >> 4);
    ax -= mMapTile_MovedHorizontal;
    ax <<= 4;
    ax -= mMapTile_ColumnOffset;
    ax += 0x40;

    mVideo_Draw_PosX = ax;

    ax = (mMap_Destroy_Tile_Y >> 4);
    ax -= mMapTile_MovedVertical;
    ax <<= 4;
    ax -= mMapTile_RowOffset;
    ax += 0x10;

    mVideo_Draw_PosY = ax;

    mVideo_Draw_Columns = 0x10;
    mVideo_Draw_Rows = 0x10;
    if (!mStartParams->mDisableVideo)
        mGraphics->MapTiles_Draw();
}

void cFodder::Map_Destroy_Tiles_Next() {

    if (mMap_Destroy_Tiles.size())
        mMap_Destroy_Tiles.erase(mMap_Destroy_Tiles.begin());
}

void cFodder::Game_Save_Wrapper() {

    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);
    Game_Save();
}

void cFodder::Game_Save() {
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

    if (mGUI_SaveLoadAction != 2) {
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

void cFodder::String_Input_Print(int16 pPosY) {
    GUI_Input_CheckKey();

    GUI_Render_Text_Centred(mInput.c_str(), pPosY);

    if (mKeyCodeAscii == 0x0D && mInput.size())
        mGUI_SaveLoadAction = 2;

    if (mKeyCodeAscii == 8)
        goto loc_2E675;

    // Is Number
    if (mKeyCodeAscii >= 0x30 && mKeyCodeAscii <= 0x39)
        goto loc_2E636;

    if (mKeyCodeAscii < 0x41 || mKeyCodeAscii > 0x5A)
        goto loc_2E6A4;

loc_2E636:;
    if (mInput.size() >= 12)
        goto loc_2E6A4;

    mInput.push_back((char)mKeyCodeAscii);

    goto loc_2E6A4;

loc_2E675:;

    if (mInput.size()) {
        mInput.pop_back();
    }

loc_2E6A4:;
    String_Input_Check();
}

void cFodder::String_Input_Check() {

    if (mKeyCodeAscii != 0x0D) {
        byte_44AC0 &= 0x3F;
        if (byte_44AC0 < 0x20)
            goto loc_2E6EA;
    }
    return;

loc_2E6EA:;
}

void cFodder::Game_Load() {
	auto Files = g_ResourceMan->GetSaves();
	std::vector<sSavedGame> SaveFiles;

	SaveFiles = Game_Load_Filter(Files);
	Files.clear();


    const std::string File = GUI_Select_File("SELECT SAVED GAME", SaveFiles, Files);
    if (!File.size())
        return;

	auto SaveData = g_ResourceMan->FileReadStr(g_ResourceMan->GetSave(File));

    // Load the game data from the JSONstd::string((char*)SaveData->data(), SaveData->size()))
    if (!mGame_Data.FromJson(SaveData)) {
        return;
    }

    // If the game was saved on a different platform, lets look for it and attempt to switch
    if (mGame_Data.mSavedVersion.mPlatform != mVersionCurrent->mPlatform) {

        VersionSwitch( mVersions->GetForCampaign(mGame_Data.mCampaignName, mGame_Data.mSavedVersion.mPlatform) );
    }

    mMouse_Exit_Loop = false;

    for (int16 x = 0; x < 8; ++x)
        mMission_Troops_SpritePtrs[x] = INVALID_SPRITE_PTR;

    for (auto& Troop : mGame_Data.mSoldiers_Allocated)
        Troop.mSprite = INVALID_SPRITE_PTR;

    GameData_Backup();
    GameData_Restore();
}

std::vector<sSavedGame> cFodder::Game_Load_Filter(const std::vector<std::string>& pFiles) {
    std::vector<sSavedGame> Results;

    for (auto& CurrentFile : pFiles) {

		auto SaveData = g_ResourceMan->FileRead(g_ResourceMan->GetSave(CurrentFile));

        // Verify the savegame is for the current campaign
        try {
            sGameData NewData( std::string((char*)SaveData->data(), SaveData->size()) );

            // Ensure for this campaign
            if (NewData.mCampaignName != mGame_Data.mCampaign.getName())
                continue;

            // Ensure the game is the same
            if (NewData.mSavedVersion.mGame != mVersionCurrent->mGame)
                continue;

            Results.push_back({ CurrentFile, NewData.mSavedName });
        }
        catch (...) {
            continue;
        }
    }

    // Sort newest first
    std::sort(Results.begin(), Results.end(), [](sSavedGame& pLeft, sSavedGame& pRight) {
        return std::atoll(pLeft.mFileName.c_str()) > std::atoll(pRight.mFileName.c_str());
    });

    return Results;
}

void cFodder::Menu_Button_Reset() {
    mCustom_ExitMenu = 0;
    mPhase_Aborted = false;
    mDemo_ExitMenu = -1;
}

/**
 * Caller of Menu_Loop is responsible for cleaning up button states
 */
void cFodder::Menu_Loop(const std::function<void()> pButtonHandler) {

    Menu_Button_Reset();

    mSurface->Save();
    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();

    for (;;) {

        Cycle_End();

        if (Menu_Draw(pButtonHandler))
            break;
    }

    Image_FadeOut();
    mSurface->clearBuffer();
}

bool cFodder::Menu_Draw(const std::function<void()> pButtonHandler) {

    // Handle Mouse
    Mouse_Inputs_Get();
    Mouse_DrawCursor();

    // Button Pressed?
    if (mButtonPressLeft)
        pButtonHandler();

    // Exit Time?
    if (mDemo_ExitMenu > 0 || mPhase_Aborted || mCustom_ExitMenu)
        return true;

    if (mSurface->isPaletteAdjusting())
        mSurface->palette_FadeTowardNew();

    Video_SurfaceRender();
    return false;
}

void cFodder::Demo_Quiz_ShowScreen(const char* pFilename) {

    Image_FadeOut();

    mGraphics->Load_And_Draw_Image(pFilename, 32);
    mGraphics->PaletteSet();

    mSurface->Save();
    mSurface->palette_FadeTowardNew();

    for (;; ) {
        Cycle_End();

        Mouse_Inputs_Get();
        Mouse_DrawCursor();

        if (mButtonPressLeft || mPhase_Aborted)
            break;

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        Video_SurfaceRender();
    }

    Image_FadeOut();

    mGraphics->Load_And_Draw_Image("1.lbm", 32);
    mGraphics->PaletteSet();

    mSurface->Save();
    mSurface->palette_FadeTowardNew();

    Menu_Button_Reset();
}

void cFodder::Demo_Quiz() {
    Image_FadeOut();

    mGraphics->Load_And_Draw_Image("1.lbm", 32);
    Mouse_Setup();

    Menu_Loop(
        [this]() {
        if (mButtonPressLeft)
            GUI_Element_Mouse_Over(mPlusQuiz_Buttons);
    }
    );

    mGraphics->Load_And_Draw_Image("apmenu.lbm", 32);
    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();
    mSurface->Save();

    Menu_Button_Reset();
}

void cFodder::Squad_Select(int16 pData4, bool pCheckMouse) {

    if (pCheckMouse && Mouse_Button_Left_Toggled() < 0)
        return;

    mSquad_Selected = pData4;
    Mission_Troops_Clear_Selected();
    Squad_Troops_Count();

    sSprite* Data20 = 0;
    sub_305D5(Data20);

    if (Data20 == INVALID_SPRITE_PTR)
        return;

    mCamera_Panning_ToTarget = false;
    mCamera_StartPosition_X = Data20->field_0;
    mCamera_StartPosition_Y = Data20->field_4;
    mCamera_Start_Adjust = true;
    mSquad_Select_Timer = 1;
    mGUI_Sidebar_Setup = 0;
    mSquad_Grenade_SplitMode = eSquad_Weapon_Split_Half;
    mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;
    word_3AC4B = 0;
    word_3AC4D = 0;
}

void cFodder::Mission_Set_Initial_Weapon() {

    // segra: I don't believe this condition can ever occur
    if (mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] == eWeapon_Grenade) {

        if (mSquad_Grenades[mGUI_Loop_Squad_Current])
            return;
        if (!mSquad_Rockets[mGUI_Loop_Squad_Current])
            return;

        mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] = eWeapon_Rocket;
        return;
    }

    //loc_2EFF1
    if (mSquad_Rockets[mGUI_Loop_Squad_Current])
        return;

    if (!mSquad_Grenades[mGUI_Loop_Squad_Current])
        return;

    mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] = eWeapon_Grenade;
}

void cFodder::Service_Show() {
    if (mParams->mSkipService)
        return;

	mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());
    mVersionPlatformSwitchDisabled = true;

    WindowTitleSet(false);

    mGraphics->Load_Service_Data();
    mGraphics->SetActiveSpriteSheet(eGFX_SERVICE);
    mGraphics->PaletteSet();

    mSound->Music_Play(0);
    Service_KIA_Loop();

    if(!mVersionCurrent->isAmigaTheOne())
        Service_Promotion_Loop();

    Mouse_Setup();

    mGraphics->Load_pStuff();
    mVersionPlatformSwitchDisabled = false;
    mPhase_Aborted = false;
}

void cFodder::Service_KIA_Loop() {
    Mouse_Setup();
    mService_Promotion_Exit_Loop = 0;

    if (Service_KIA_Troop_Prepare() < 0)
        return;

    mSurface->clearBuffer();

    if (mVersionCurrent->isPC()) {
        GUI_Draw_Frame_8(5, 0, 0x34, 0);
        GUI_Draw_Frame_16(7, 0, 0, 0x31);
        GUI_Draw_Frame_16(7, 0, 0xF0, 0x31);
    }
    else {
        GetGraphics<cGraphics_Amiga>()->Service_Draw(8, 0x30, 0);
        GetGraphics<cGraphics_Amiga>()->Service_Draw(0, 0, 0x40);
        GetGraphics<cGraphics_Amiga>()->Service_Draw(4, 0xF0, 0x40);
    }

    mMouse_Exit_Loop = false;
    mService_ExitLoop = 0;
    mGraphics->PaletteSet();
    mSurface->Save();

    do {
        Mouse_Inputs_Get();

        if (mService_Promotion_Exit_Loop == -1 || mMouse_Exit_Loop) {
            mMouse_Exit_Loop = false;
            mSurface->paletteNew_SetToBlack();
            mService_ExitLoop = 1;
        }

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        Service_Draw_List();
        Cycle_End();
        Service_ScrollUp_DrawList();

        Video_SurfaceRender();

    } while (mSurface->isPaletteAdjusting() || mService_ExitLoop == 0);
}

void cFodder::Service_Promotion_Loop() {
    Mouse_Setup();
    mService_Promotion_Exit_Loop = 0;
    
    if (Service_Promotion_Prepare_Draw() < 0)
        goto loc_18001;

    mSurface->clearBuffer();
    if (mVersionCurrent->isPC()) {
        GUI_Draw_Frame_8(6, 0, 0x34, 0);
        GUI_Draw_Frame_16(8, 0, 0, 0x31);
        GUI_Draw_Frame_16(8, 0, 0xF0, 0x31);
    }
    else {
        GetGraphics<cGraphics_Amiga>()->Service_Draw(9, 0x30, 0);        // Heroes in Victory
		GetGraphics<cGraphics_Amiga>()->Service_Draw(3, 0, 0x40);        //  Left Symbol
		GetGraphics<cGraphics_Amiga>()->Service_Draw(7, 0xF0, 0x40);     //  Right Symbol
    }

    mService_ExitLoop = 0;
    mMouse_Exit_Loop = false;
    mGraphics->PaletteSet();
    mSurface->Save();

    do {

        Mouse_Inputs_Get();
        if (mService_Promotion_Exit_Loop == -1 || mMouse_Exit_Loop) {
            mMouse_Exit_Loop = false;
            mSurface->paletteNew_SetToBlack();
            mService_ExitLoop = 1;
        }

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        Service_Promotion_Check();
        Service_Draw_List();
        Cycle_End();
        //sub_14445();
        Service_ScrollUp_DrawList();

        Video_SurfaceRender();

    } while (mSurface->isPaletteAdjusting() || mService_ExitLoop == 0);

loc_18001:;

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {
        Troop.Promote();
    }
}

int16 cFodder::Service_KIA_Troop_Prepare() {
    mService_Draw_List.clear();

    mVideo_Draw_PosY = 0xE8;
    Service_Mission_Text_Prepare();
    mVideo_Draw_PosY += 0x40;

    if (mGame_Data.mHeroes.empty() || mGame_Data.mGamePhase_Data.mHeroesCount == mGame_Data.mHeroes.size())
        return -1;

   
    for (size_t i = mGame_Data.mGamePhase_Data.mHeroesCount; i < mGame_Data.mHeroes.size(); ++i) {

        auto& Hero = mGame_Data.mHeroes[i];

        Service_Draw_Troop_And_Rank(Hero.mRecruitID, Hero.mRank);
        mVideo_Draw_PosY += 0x40;
    }

    return 0;
}

int16 cFodder::Service_Promotion_Prepare_Draw() {
    mService_Draw_List.clear();

    mVideo_Draw_PosY = 0xE8;
    Service_Mission_Text_Prepare();
    mVideo_Draw_PosY += 0x40;
    size_t Drawn = mService_Draw_List.size();

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mSprite != INVALID_SPRITE_PTR) {
            int16 ax = Troop.mRecruitID;
            int8 bl = Troop.mRank;

            bl &= 0xFF;

            Service_Draw_Troop_And_Rank(ax, bl);

            mVideo_Draw_PosY += 0x40;
        }
    }

    // No names drawn?
    if (Drawn == mService_Draw_List.size())
        return -1;

    return 0;
}

void cFodder::Service_Draw_String(const std::string& pText, const bool pLarge, const size_t pY) {

    const uint8* FontWidth = mFont_Briefing_Width;
    if (pLarge)
        FontWidth = mFont_ServiceName_Width;

    String_CalculateWidth(320, FontWidth, pText.c_str());
    Service_Draw_String(pText, FontWidth, pLarge ? 3 : 0, mGUI_Temp_X, (int16) pY);

}

void cFodder::Service_Draw_Troop_And_Rank(int16 pRecruitID, int16 pRank) {

    mService_Draw_List.emplace_back(PLATFORM_BASED(9, 0), PLATFORM_BASED(0, 1), 0x60, mVideo_Draw_PosY);
    mService_Draw_List.emplace_back(2, pRank, 0x64, mVideo_Draw_PosY + 4);
    mService_Draw_List.emplace_back(2, pRank, 0xCC, mVideo_Draw_PosY + 4);

    //seg003:3237
    const char* si = mRecruits[pRecruitID].mName;
    std::stringstream tmpString;

    for (int16 cx = 5; cx >= 0; --cx) {
        // Break on space
        if (*si == 0x20)
            break;

        tmpString << *si++;
    }

    Service_Draw_String(tmpString.str(), true, (size_t) mVideo_Draw_PosY + 6);
}

void cFodder::Service_Promotion_Check() {
    auto* Troop = &mGame_Data.mSoldiers_Allocated[0];

    if (!mService_Draw_List.size())
        return;

    for( size_t x = 0; x < mService_Draw_List.size() - 1; ++x) {
        auto Draw = &mService_Draw_List[x];

        if (Draw->mSpriteType != 2)
            continue;

        int16 newRank = Troop->GetPromotedRank();
        if (Troop->mRecruitID == -1) {
            ++Troop;
            --x;
            continue;
        }
        ++Troop;
        ++x;

        if (Draw->mY >= (getCameraHeight() - 8) / 2)
            continue;

        (Draw+0)->mFrame = newRank;
        (Draw+1)->mFrame = newRank;
    }
}

void cFodder::Service_Draw_List() {

    for(auto& Draw : mService_Draw_List) {

        if (Draw.mY < 8)
            continue;

        Service_Sprite_Draw(Draw.mSpriteType, Draw.mFrame, Draw.mX, Draw.mY);
    }
}

void cFodder::Service_ScrollUp_DrawList() {
    mService_Promotion_Exit_Loop = -1;

    for (auto& Draw : mService_Draw_List) {

        Draw.mY -= 1;

        if(Draw.mY >= -4)
            mService_Promotion_Exit_Loop = 0;

    }

    //auto remove = std::remove_if(mService_Draw_List.begin(), mService_Draw_List.end(), [](auto pVal) 
    //{ return pVal.mY < -48; });

    //mService_Draw_List.erase(remove, mService_Draw_List.end());
}

void cFodder::Service_Draw_String(const std::string& pText, const uint8* pData28, int16 pData0, int16 pData8, int16 pDataC) {

    const char* Data20 = pText.c_str();

    for (;;) {
        int16 Data4 = *Data20++;
        if (Data4 == 0)
            break;

        int16 Data10 = Data4;

        if (mString_GapCharID) {

            if (Data4 == 0x20)
                goto loc_18229;

            Data4 = mString_GapCharID;
            goto loc_18259;
        }
        //loc_18222
        if (Data4 == 0x20)
            goto loc_1826D;
    loc_18229:;
        if (Data4 > 0x39)
            goto loc_1823C;

        Data4 -= 0x30;
        Data4 += 0x1A;
        goto loc_18259;
    loc_1823C:;
        if (Data4 >= 0x5A) {
            Data4 -= 0x61;
            Data4 += 0x39;
        }
        else {
            Data4 -= 0x41;
            Data4 += 0;
        }

    loc_18259:;
        mService_Draw_List.emplace_back(pData0, Data4, pData8, pDataC);

    loc_1826D:;

        Data10 = pData28[Data10];
        pData8 += Data10;
    }
}

int16 cFodder::Service_Sprite_Draw(int16& pSpriteType, int16& pFrame, int16& pX, int16& pY) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_PosX = pX + 0x10;
    mVideo_Draw_PosY = pY + 0x10;


    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount;

    if (!Service_Sprite_OnScreen_Check()) {
        if (mVersionCurrent->mPlatform == ePlatform::Amiga)
            mGraphics->Video_Draw_16(mFont_Service_PalleteIndex_Amiga);
        else
            mGraphics->Video_Draw_8(0, mFont_Service_PalleteIndex);

        return 1;
    }
    return 0;
}

int16 cFodder::Service_Sprite_OnScreen_Check() {
    int16 ax;

    if (mVideo_Draw_PosY < 0x2C) {
        ax = mVideo_Draw_PosY;
        ax += mVideo_Draw_Rows;
        --ax;
        if (ax < 0x2C)
            return -1;

        ax -= 0x2C;
        ax -= mVideo_Draw_Rows;
        ++ax;
        ax = -ax;
        mVideo_Draw_PosY += ax;
        mVideo_Draw_Rows -= ax;

        ax *= PLATFORM_BASED(160, 40);

        mVideo_Draw_FrameDataPtr += ax;
    }
    //loc_184FC

    ax = mVideo_Draw_PosY;
    ax += mVideo_Draw_Rows;
    --ax;

    if (ax <= 0xE7)
        return 0;

    if (mVideo_Draw_PosY > 0xE7)
        return -1;

    ax -= 0xE7;
    mVideo_Draw_Rows -= ax;
    return 0;
}

void cFodder::Service_Mission_Text_Prepare() {
    std::stringstream Mission;
    Mission << "MISSION ";

    Mission << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Number));

    String_CalculateWidth(320, mFont_Service_Width, Mission.str().c_str());

    Service_Draw_String(Mission.str(), mFont_Service_Width, 4, mGUI_Temp_X, mVideo_Draw_PosY);
}

void cFodder::Briefing_Draw_Phase() {
    const char* Str_Brief = "BRIEFING";
    const char* Str_Phase = "PHASE ";
    const char* Str_Of = " OF ";

    mString_GapCharID = 0x25;

    String_Print_Large(Str_Brief, true, 0x4E);

    Briefing_DrawBox(1, 0x49, 0x13E, 0x6B, 0xF3);
    Briefing_DrawBox(0, 0x48, 0x13E, 0x6B, 0xF2);

    mString_GapCharID = 0;

    std::stringstream Phase;

    Phase << Str_Phase << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Phase));
    Phase << Str_Of << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Current->NumberOfPhases()));

    String_Print_Small(Phase.str(), 0x1D);

    Phase_Goals_Set();

    int16 DataC = 0x84;
    bool* Goals = mGame_Data.mGamePhase_Data.mGoals_Remaining;

    for (const auto GoalName : mMissionGoal_Titles) {
        if (*Goals++) {

            String_Print_Small(GoalName, DataC - 0x12);
            DataC += 0x0C;
        }
    }
}

void cFodder::Briefing_Show_PreReady() {

    if (mVersionCurrent->isDemo() && !mGame_Data.mCampaign.isCustom() && !mGame_Data.mCampaign.isRandom())
        return;

    if (!mVersionCurrent->hasGfx(eGFX_BRIEFING) && !mGame_Data.mCampaign.isRandom())
        VersionSwitch(mVersionDefault);

	mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());
    mSurface->clearBuffer();
    mGraphics->PaletteSet();

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    Briefing_Draw_Mission_Title(0x2C);
    Briefing_Draw_Phase();

    Image_FadeIn();
}

void cFodder::Briefing_Show_Ready() {

    if (mVersionCurrent->isDemo() && !mGame_Data.mCampaign.isCustom() && !mGame_Data.mCampaign.isRandom())
        return;

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    Briefing_Draw_Mission_Title(0x2C);
    Briefing_Draw_Phase();
    Briefing_Draw_With();

    mMouse_Exit_Loop = false;

    do {
        Mouse_Inputs_Get();

        if (mPhase_Aborted) {
            mBriefing_Aborted = -1;
            mMouseButtonStatus = -1;
            break;
        }

        Video_SurfaceRender(false);
        Cycle_End();

    } while (!mMouse_Exit_Loop);

    mMouse_Exit_Loop = false;

    Image_FadeOut();
    Mouse_Setup();
}

void cFodder::Briefing_Draw_With() {
    std::stringstream With;

    With << "WITH ";
    With << tool_StripLeadingZero(std::to_string(mGame_Data.mGamePhase_Data.mSoldiers_Available));

    if (mGame_Data.mGamePhase_Data.mSoldiers_Available == 1) {
        With << " SOLDIER YOU MUST";
    }
    else {
        With << " SOLDIERS YOU MUST";
    }

    String_Print_Small(With.str(), 0x64);
    With.str("");

    if (!mGame_Data.mRecruits_Available_Count) {
        With << "THIS IS YOUR LAST CHANCE";
    }
    else {
        With << mGame_Data.mRecruits_Available_Count;
        if (mGame_Data.mRecruits_Available_Count == 1)
            With << " RECRUIT REMAINING";
        else
            With << " RECRUITS REMAINING";
    }

    String_Print_Small(With.str(), 0xA8);
    String_Print_Large("GO FOR IT", false, 0xB8);
}

void cFodder::Briefing_DrawBox(int16 pX, int16 pY, int16 pWidth, int16 pHeight, uint8 pColor) {
    pX += 0x10;
    pY += 0x10;

    // Top and Bottom
    Briefing_Draw_Horizontal_Line(pX, pX + pWidth, pY, pColor);

    Briefing_Draw_Horizontal_Line(pX, pX + pWidth, pY + pHeight, pColor);

    // Sides of box
    Briefing_Draw_Vertical_Line(pX, pY + pHeight, pY, pColor);

    Briefing_Draw_Vertical_Line(pX + pWidth, pY + pHeight, pY, pColor);
}

void cFodder::Briefing_Draw_Pixel(int16 pX, int16 pY, uint8 pColor) {
    uint8* di = mSurface->GetSurfaceBuffer();

    di += mSurface->GetWidth() * pY;
    di += pX;
    *di = pColor;
}

void cFodder::Briefing_Draw_Horizontal_Line(int16 pX, int16 pWidth, int16 pY, uint8 pColor) {

    do {
        Briefing_Draw_Pixel(pX, pY, pColor);
        ++pX;
    } while (pX <= pWidth);
}

void cFodder::Briefing_Draw_Vertical_Line(int16 pX, int16 pHeight, int16 pY, uint8 pColor) {

    do {
        Briefing_Draw_Pixel(pX, pY, pColor);
        ++pY;
    } while (pY <= pHeight);
}

void cFodder::Sprite_Frame_Modifier_Update() {

    mSprite_Frame_Modifier_Update_Countdown -= 1;
    if (mSprite_Frame_Modifier_Update_Countdown < 0) {
        mSprite_Frame_Modifier_Update_Countdown = 3;

        mSprite_Frame_1 += mSprite_Frame1_Modifier;
        if (mSprite_Frame_1 == 0 || mSprite_Frame_1 == 2)
            mSprite_Frame1_Modifier ^= -2;

        mSprite_Frame_2 += mSprite_Frame2_Modifier;
        if (mSprite_Frame2_Modifier == 0 || mSprite_Frame2_Modifier == 2)
            mSprite_Frame2_Modifier ^= -2;
    }

    //loc_18DA7
    mSprite_Frame3_ChangeCount -= 1;
    if (mSprite_Frame3_ChangeCount < 0) {
        mSprite_Frame3_ChangeCount = 1;

        mSprite_Frame_3 += mSprite_Frame_3_Modifier;
        if (mSprite_Frame_3 == 0 || mSprite_Frame_3 == 2)
            mSprite_Frame_3_Modifier ^= -2;
    }
}

void cFodder::Sprite_Handle_Loop() {
    sSprite* Data20 = mSprites.data();

    for (int32 Data1C = mParams->getSpritesMax() - 2; Data1C > 0; --Data1C, ++Data20) {

        if (Data20->field_0 == -32768)
            continue;

        (*this.*mSprite_Function[Data20->field_18])(Data20);
    }
}

void cFodder::Sprite_Handle_Null(sSprite* pSprite) {

    switch (pSprite->field_18) {
    case 4:
        return;

    default:
        std::cout << "Sprite Function not implemented: " << pSprite->field_18 << "\n";
        break;
    }

}

void cFodder::Sprite_Handle_Player(sSprite *pSprite) {
    int16 Data0, Data4, eax;
    sSprite* Data28 = 0;

    // player soldier in vehicle?
    if (pSprite->field_6E) {
        Sprite_Handle_Player_InVehicle(pSprite);
        return;
    }

    // If player is invincible
    if (pSprite->field_75 & eSprite_Flag_Invincibility) {

        // And dying
        if (pSprite->field_38 <= eSprite_Anim_Slide1)
            pSprite->field_38 = eSprite_Anim_None;

        // No longer Sinking
        pSprite->field_5B = 0;

        // Unknown
        pSprite->field_64 = 0;
    }

    if (Sprite_Handle_Soldier_Animation(pSprite))
        return;

    // Animation running?
    if (pSprite->field_38)
        return;

    if (Sprite_Handle_Player_MissionOver(pSprite))
        return;

    //loc_18E8D
    Sprite_Handle_Player_Enter_Vehicle(pSprite);
    mSprite_FaceWeaponTarget = 0;

    word_3AA1D = word_3BED5[pSprite->field_32];
    //seg004:0183

    if (mSquad_Join_TargetSquad[pSprite->field_32] < 0 && pSprite->field_32 != mSquad_Selected) {
        //loc_18F12
        mSprite_FaceWeaponTarget = -1;

		Data28 = &mSprites[pSprite->field_5E];

		// The original engine checked 1 sprite per cycle, this is problematic 
		// with max sprites gets bigger, especially at 100,000. So we check every sprite
		//  every cycle, if not using the original parameters.
		if (!mParams->isOriginalSpriteMax()) {
			bool looped = false;

			while (Data28->field_0 == -32768) {
				++pSprite->field_5E;
				if (pSprite->field_5E >= (mParams->mSpritesMax - 2)) {
					pSprite->field_5E = 0;
					if (looped)
						break;

					looped = true;
				}
				Data28 = &mSprites[pSprite->field_5E];
			};
		}

        if (Data28->field_0 != -32768) {

            if (Data28->field_18 == eSprite_Enemy) {
                if (Data28->field_38 == eSprite_Anim_None) {

                    //seg004:021A 
                    if (pSprite->field_4F == 0) {
                        //seg004:0228
                        Data0 = pSprite->field_0;
                        Data4 = pSprite->field_4;

                        int16 Data8 = Data28->field_0;
                        int16 DataC = Data28->field_4;

                        Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
                        if (Data0 < 0xD2) {

                            if (Data0 <= 0x28)
                                goto loc_1904A;

                            //seg004:0270
                            mSprite_Find_Distance = Data0;
                            Data0 = pSprite->field_0;
                            Data4 = pSprite->field_4;
                            Data8 = Data28->field_0;
                            DataC = Data28->field_4;

                            if (Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC) == 0) {
                                Data0 = mSprite_Find_Distance;
                                goto loc_1904A;
                            }
                            if (mSprite_Find_Distance < 0x40)
                                goto loc_1904A;
                        }
                    }
                }
            }
        }

    loc_1901C:;
        pSprite->field_4A = 0;
        pSprite->field_5E++;
        if (pSprite->field_5E >= (mParams->mSpritesMax - 2))
			pSprite->field_5E = 0;

        goto loc_191C3;

    loc_1904A:;
        Data0 = tool_RandomGet() & 0x1F;

        if (Data0 == 5)
            goto loc_1901C;

        pSprite->field_5A = -1;
        mTroop_Weapon_Grenade_Disabled = false;
        mTroop_Weapon_Bullet_Disabled = false;
        mTroop_Weapon_Rocket_Disabled = false;

        pSprite->field_2E = Data28->field_0;
        pSprite->field_30 = Data28->field_4 - 0x0E;

        // Fired Weapon?
        if (pSprite->field_4A <= 0) {
            //loc_190B9
            Data0 = tool_RandomGet() & 0x0F;
            ++Data0;
            pSprite->field_4A = Data0;

            Data0 = tool_RandomGet() & 0x3F;
            if (Data0 == 0x2A)
                goto loc_1918C;
        }

    HandleWeapon:;
        mTroop_Weapon_Grenade_Disabled = false;
        mTroop_Weapon_Bullet_Disabled = false;
        mTroop_Weapon_Rocket_Disabled = false;

        if (pSprite->field_4F) {
            mTroop_Weapon_Rocket_Disabled = true;
            mTroop_Weapon_Bullet_Disabled = true;
            mTroop_Weapon_Grenade_Disabled = true;
        }
        else {
            //loc_19118
            if (pSprite == mSquad_Leader && mMouse_Button_LeftRight_Toggle) {

                if (mSquad_CurrentWeapon[pSprite->field_32] == eWeapon_Rocket) {
                    mTroop_Weapon_Grenade_Disabled = true;
                    mTroop_Weapon_Bullet_Disabled = true;
                } else {
                    if (mSquad_CurrentWeapon[pSprite->field_32] == eWeapon_Grenade) {
                        mTroop_Weapon_Rocket_Disabled = true;
                        mTroop_Weapon_Bullet_Disabled = true;
                    }
                    else
                        goto loc_19198;
                }
            loc_1918C:;
                pSprite->field_4A = 1;
                goto loc_191BF;
            }

        loc_19198:;
            mTroop_Weapon_Grenade_Disabled = true;
            mTroop_Weapon_Rocket_Disabled = true;

            if(pSprite->field_46_mission_troop)
                Data0 = pSprite->field_46_mission_troop->field_6;

        loc_191BF:;
            Sprite_Handle_Troop_Weapon(pSprite);
        }
    }
    else {

        if (mSprite_Player_CheckWeapon) {
            goto HandleWeapon;

        }
    }

loc_191C3:;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        goto loc_1921E;

    // seg004:047F
    eax = mSquad_WalkTargets[pSprite->field_32][pSprite->field_40].mY;

    //seg004:04CC
    if (eax < 0) {
    loc_1921E:;
        if (mSprite_FaceWeaponTarget) {

            if (pSprite->field_0 == pSprite->field_26 &&
                pSprite->field_4 == pSprite->field_28)
                goto loc_1931E;
        }
    }
    //loc_1925C
    // Just Fired a bullet or grenade?
    if (pSprite->field_54 != 2 && pSprite->field_54 != 1)
        goto loc_19338;


    // Just Fired bullet or grenade
    //loc_19274
    if (word_3AA1D != 2)
        goto loc_1931E;

    mTmp_FrameNumber = pSprite->field_A;
    pSprite->field_5A = 0;

    if (mSprite_FaceWeaponTarget)
        Sprite_Handle_Troop_Direct_TowardWeaponTarget(pSprite);
    else
        Sprite_Handle_Troop_Direct_TowardMouse(pSprite);

    Data0 = pSprite->field_26;
    if (Data0 < 0)
        goto loc_1946D;

    Data4 = pSprite->field_28;
    if (Data4 < 0)
        goto loc_1946D;

    Sprite_XY_Store(pSprite);

    if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) >= 0) {
        Sprite_Handle_Player_Adjust_Movement_Speed(pSprite);
        Sprite_Draw_Row_Update(pSprite);
    }

    if (pSprite->field_0 == pSprite->field_26) {

        if (pSprite->field_4 == pSprite->field_28)
            Sprite_Next_WalkTarget_Set(pSprite);
    }

    //loc_19314
    if (mSprite_Reached_Target == 0)
        return;

loc_1931E:;

    if (pSprite->field_5A)
        goto loc_19403;

    Sprite_Handle_Troop_Direct_TowardMouse(pSprite);
    Sprite_Handle_Troop_FrameUnk(pSprite);
    return;

loc_19338:;
    pSprite->field_43 = 0;
    mTmp_FrameNumber = pSprite->field_A;
    mSprite_Bumped_Into_SquadMember = 0;

    sub_1FCF2(pSprite);
    Data0 = pSprite->field_26;
    if (Data0 < 0)
        goto loc_1946D;
    Data4 = pSprite->field_28;
    if (Data4 < 0)
        goto loc_1946D;

    if (pSprite->field_44) {
        pSprite->field_44 -= 1;
        goto loc_193D6;

    }

    // Reached near target?
    //loc_19392
    if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) >= 0)
        goto loc_193D6;

    // Exactly on target?
    //  Then move toward the next target
    if (pSprite->field_0 == pSprite->field_26) {
        if (pSprite->field_4 == pSprite->field_28)
            Sprite_Next_WalkTarget_Set(pSprite);
    }
    //loc_193D3
    goto loc_1946D;

loc_193D6:;
    Sprite_XY_Store(pSprite);
    sub_2A3D4(pSprite);

    if (!pSprite->field_5A) {
        if (!pSprite->field_45)
            goto loc_19424;

        pSprite->field_45 -= 1;
        return;
    }

loc_19403:;
    pSprite->field_5A = 0;
    sub_1FDE7(pSprite);
    return;

loc_19414:;
    pSprite->field_5A = 0;
    Sprite_Handle_Troop_Direct_TowardMouse(pSprite);
    return;

loc_19424:; // Soldier Walking
    Sprite_Draw_Row_Update(pSprite);

    if (pSprite->field_0 == pSprite->field_26) {
        if (pSprite->field_4 == pSprite->field_28)
            Sprite_Next_WalkTarget_Set(pSprite);
    }
    //loc_19463
    if (mSprite_Reached_Target)
        goto loc_19414;

    goto loc_1957A;

loc_1946D:;
    if (pSprite->field_5A)
        goto loc_19403;

    if (!pSprite->field_45)
        goto loc_194A0;

    pSprite->field_45 -= 1;
    goto loc_19414;

loc_19490:;
    pSprite->field_5A = 0;
    Sprite_Handle_Troop_Direct_TowardMouse(pSprite);
    return;

loc_194A0:;
    pSprite->field_43 = -1;

    if (!word_3AA1D) {
        mDirectionMod = 0;
        Sprite_Handle_Troop_FrameUnk(pSprite);
        pSprite->field_A = 0;
        return;
    }

    //loc_194CB
    if (mSquad_Selected == pSprite->field_32)
        goto loc_19490;

    Sprite_Handle_Troop_Direct_TowardMouse(pSprite);
    return;

loc_1957A:;
    pSprite->field_3A = 0;
    Sprite_Handle_Troop_FrameUnk(pSprite);

}

void cFodder::Sprite_Handle_Grenade(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data24;
    int16 a1;
    int32 Dataa4;

    if (pSprite->field_38)
        goto loc_1992D;

    if (!pSprite->field_12)
        goto loc_1992D;

    if (!pSprite->field_56)
        goto loc_19701;

    --pSprite->field_56;
    if (!pSprite->field_56) {
        // HACK: Disable grenade sound for cannon plus... it seems corrupted and causes a crash
        if (!mVersionCurrent->isCoverDisk())
            Sound_Play(pSprite, eSound_Effect_Grenade, 0x0F);
    }

    Data24 = pSprite->field_46_sprite;
    pSprite->field_0 = Data24->field_0;
    pSprite->field_2 = Data24->field_2;
    pSprite->field_4 = Data24->field_4;
    pSprite->field_6 = Data24->field_6;
    pSprite->field_4 += 1;
    pSprite->field_0 += 3;
    pSprite->field_8 = 0x7C;
    pSprite->field_A = 0;
    return;

loc_19701:;
    pSprite->field_8 = 0x7D;
    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 <= 1)
        pSprite->field_36 = 0;

    if (Data0 <= 4)
        pSprite->field_36 >>= 1;

    if (pSprite->field_36) {
        pSprite->field_36 -= 1;
        Sprite_XY_Store(pSprite);

        mTmp_FrameNumber = pSprite->field_A;
        Data0 = pSprite->field_26;
        Data4 = pSprite->field_28;

        Sprite_Direction_Between_Points(pSprite, Data0, Data4);
        Sprite_Movement_Calculate(pSprite);
        Sprite_Handle_Grenade_Terrain_Check(pSprite);

        (pSprite + 1)->field_10 = pSprite->field_10;
    }

    //loc_197C5
    if (!pSprite->field_50) {
        if (pSprite->field_52) {
            pSprite->field_36 = 0;
            goto loc_198D3;
        }
    }
    //loc_197EA
    a1 = (int16)(((int64)pSprite->field_1A) >> 16);

    if (a1 < 0)
        if (a1 < -8)
            pSprite->field_1A = (((int32)pSprite->field_1A & 0xFFFF) | -0x80000);

    Dataa4 = pSprite->field_1A;
    Dataa4 -= 0x18000;

    pSprite->field_1A = Dataa4;
    Dataa4 = pSprite->field_1A;

    if (!pSprite->field_50)
        goto loc_19877;

    if (Dataa4 >= 0)
        goto loc_19877;

    if (pSprite->field_20 >= 2)
        goto loc_19877;

    pSprite->field_1E = 0;
    pSprite->field_20 = 0;

loc_19855:;

    Dataa4 = (Dataa4 >> 16) | ((Dataa4 & 0xFFFF) << 16);
    if (pSprite->field_52 >= 0x0A)
        goto loc_198D3;

    pSprite->field_52 -= Dataa4;
    goto loc_198D3;

loc_19877:;
    pSprite->field_1E_Big += Dataa4;

    if (pSprite->field_20 < 0) {
        pSprite->field_1E_Big = 0;

        if (pSprite->field_50)
            goto loc_19855;

        Dataa4 = (int64)pSprite->field_1A;
        Dataa4 = -Dataa4;
        Dataa4 >>= 1;
        pSprite->field_1A = Dataa4;
    }

loc_198D3:;
    if (!pSprite->field_36)
        pSprite->field_20 = 0;

    Data4 = pSprite->field_20;
    Data4 >>= 4;
    if (Data4 > 3)
        Data4 = 3;

    pSprite->field_A = Data4;
    (pSprite + 1)->field_A = Data4;
    pSprite->field_12 -= 1;
    return;

loc_1992D:;

    if (pSprite->field_52 < 8) {
        pSprite->field_18 = eSprite_Explosion;
        Sprite_Projectile_Counters_Decrease(pSprite);
        Sprite_Destroy(pSprite + 1);
        return;
    }
    //loc_19957
    Sprite_Projectile_Counters_Decrease(pSprite);
    pSprite->field_0 = 0;
    pSprite->field_4 = 0;

    Sprite_Destroy(pSprite);
    Sprite_Destroy(pSprite + 1);
}

void cFodder::Sprite_Projectile_Counters_Decrease(sSprite* pSprite) {

    --mSprite_Projectile_Counters[pSprite->field_22];
}

void cFodder::Sprite_Handle_ShadowSmall(sSprite* pSprite) {
    sSprite* Data24 = pSprite - 1;

    pSprite->field_0 = Data24->field_0;
    pSprite->field_4 = Data24->field_4;
    pSprite->field_52 = Data24->field_52;

}

void cFodder::Sprite_Handle_Enemy(sSprite* pSprite) {

    if (Sprite_Handle_Soldier_Animation(pSprite))
        return;

    if (pSprite->field_38)
        return;

    mSprite_FaceWeaponTarget = 0;
    if (pSprite->field_44) {
        pSprite->field_44--;
    }
    else {
        sub_21CD1(pSprite);
        Sprite_Handle_Troop_Weapon(pSprite);

        mTmp_FrameNumber = pSprite->field_A;
        int16 Data0 = pSprite->field_26;
        if (Data0 < 0)
            goto loc_19A89;

        int16 Data4 = pSprite->field_28;
        if (Data4 < 0)
            goto loc_19A89;

        if (Sprite_Direction_Between_Points(pSprite, Data0, Data4) < 0)
            goto loc_19A89;
    }
    //loc_19A5D
    Sprite_XY_Store(pSprite);
    sub_2A3D4(pSprite);
    sub_1FCF2(pSprite);

    if (pSprite->field_45) {
        pSprite->field_45--;
        if (pSprite->field_45)
            return;
    }
    Sprite_Draw_Row_Update(pSprite);
    goto loc_19A96;

loc_19A89:;
    word_3ABB1 = 0;
    sub_1FDE7(pSprite);
    goto loc_19A9C;

loc_19A96:;
    word_3ABB1 = -1;

loc_19A9C:;
    pSprite->field_22 = eSprite_PersonType_AI;
    Sprite_Handle_Troop_FrameUnk(pSprite);

    if (word_3ABB1)
        return;
    if (mSprite_FaceWeaponTarget)
        return;

    pSprite->field_A = 1;
}

void cFodder::Sprite_Handle_Bullet(sSprite* pSprite) {
    int16 Data8, DataC, Data0, Data4;
    int8 al;

    pSprite->field_64 += 1;

    if (!pSprite->field_2A)
        goto loc_19BA8;

    Data0 = pSprite->field_1A_sprite->field_0;
    Data4 = pSprite->field_1A_sprite->field_4;

    Data0 += pSprite->field_16;
    Data4 += pSprite->field_14;
    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;

    pSprite->field_2A -= 1;
    if (pSprite->field_2A)
        return;

    pSprite->field_8 = 0x7F;
    pSprite->field_A = 0;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        pSprite->field_A = 3;

    Data0 = pSprite->field_1A_sprite->field_0;
    Data4 = pSprite->field_1A_sprite->field_4;
    Data0 += 7;
    Data4 += 5;
    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;

loc_19BA8:;
    if (pSprite->field_43)
        goto loc_19DCF;

    if (!pSprite->field_12)
        goto loc_19DCF;

    pSprite->field_12 -= 1;
    pSprite->field_3A += 1;

    if (pSprite->field_44)
        goto loc_19D24;

    Sprite_XY_Store(pSprite);

    if (pSprite->field_59)
        goto loc_19C6B;

    Data0 = pSprite->field_2E;
    Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);

    Data0 = pSprite->field_50;
    if (Data0) {
        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
        pSprite->field_59 = -1;
    }
    //loc_19C4D

    if (pSprite->field_34 < 0) {
        pSprite->field_34 = pSprite->field_10;
    }

loc_19C6B:;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto SpriteDestroy;
    pSprite->field_36 = pSprite->field_4A;

    if (pSprite->field_59)
        goto loc_19D3C;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    Data4 = pSprite->field_36;
    Data4 >>= 3;

    if (Data0 > Data4)
        goto loc_19D3C;

    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_0 = pSprite->field_2E;
    pSprite->field_4 = pSprite->field_30;
    pSprite->field_44 = -1;
    goto loc_19D3C;

loc_19D24:;
    pSprite->field_2C = eSprite_Draw_First;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto SpriteDestroy;

loc_19D3C:;
    if (pSprite->field_64 > 2) {
        Data0 = -9;
        Data4 = 0;
        if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
            goto loc_19DCF;
    }

    if (!Sprite_Projectile_Collision_Check(pSprite))
        return;

    if (mSprites_Found_Count == 1)
        goto loc_19DCF;

    pSprite->field_43 = 1;
    pSprite->field_2C = eSprite_Draw_OnTop;
    pSprite->field_0 -= 4;
    if (pSprite->field_0 < 0)
        goto loc_19E50;

    pSprite->field_4 -= 3;
    if (pSprite->field_4 < 0)
        goto loc_19E50;

    pSprite->field_8 = 0x96;
    pSprite->field_A = 0;
    return;

loc_19DB0:;
    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if (pSprite->field_A >= 4)
        goto SpriteDestroy;

    return;

loc_19DCF:;
    al = pSprite->field_43;
    if (al)
        goto loc_19E1C;

    Sprite_Projectile_Collision_Check(pSprite);
    pSprite->field_43 = -1;
    pSprite->field_2C = eSprite_Draw_First;
    pSprite->field_0 -= 4;
    if (pSprite->field_0 < 0)
        goto loc_19E50;

    pSprite->field_4 -= 3;
    if (pSprite->field_4 < 0)
        goto loc_19E50;

    pSprite->field_A = 3;
    al = pSprite->field_43;

loc_19E1C:;
    if (al >= 0)
        goto loc_19DB0;

    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if (pSprite->field_A < 8)
        return;

SpriteDestroy:;

    mSprite_Projectile_Counters[2] -= 1;
    Sprite_Destroy(pSprite);
    return;

loc_19E50:;
    pSprite->field_0 = 0;
    pSprite->field_4 = 0x1000;
    goto SpriteDestroy;
}

void cFodder::Sprite_Handle_Helicopter(sSprite* pSprite) {
    word_3B4ED[0] = 0;
    int16 Data0 = tool_RandomGet() & 0x0E;
    int16 Data4, Data8, DataC, Data10, Data14, Data18, Data1C;
    sSprite* Data24 = 0, *Data2C = 0;

    if (mVersionCurrent->isCoverDisk()) {
        static int FF = 0;

        if (++FF == 100) {
            FF = 0;
            //TODO: Fix chopper
            Data0 = 0x01;
        }
        else
            Data0 = 0;
    }
    else
        Data0 = mSprite_Helicopter_Sounds[Data0 / 2];

    if (pSprite->field_22 != eSprite_PersonType_Human) {
        if (pSprite->field_20 <= 1)
            goto loc_19EE5;
    }
    else {

        // Just off the ground?
        if (pSprite->field_20 < 2) {
            if (pSprite != mSquad_CurrentVehicle)
                goto loc_19EE5;
        }
    }

    word_3B4ED[0] = -1;
    Data4 = pSprite->field_20;
    if (Data4 < 0x10)
        Data4 = 0x10;

    Sprite_Map_Sound_Play(Data0);

loc_19EE5:;
    pSprite->field_65 = -1;

    if (pSprite->field_38 == eSprite_Anim_None)
        goto loc_1A149;

    if (pSprite->field_18 == eSprite_Helicopter_Homing_Enemy2) {

        if (mHelicopterCall_X >= 0) {

            if (pSprite->field_75 != 0x71) {
                pSprite->field_75 = 0x71;
                pSprite->field_74 = static_cast<int8>(pSprite->field_A);
            }

            pSprite->field_A += 1;
            pSprite->field_A &= 0x0F;

            if (pSprite->field_A == pSprite->field_74) {
                pSprite->field_75 = 0;
                pSprite->field_38 = eSprite_Anim_None;
            }

            Data24 = pSprite + 1;

            Data24->field_8 = 0x8C;
            Data24->field_0 = pSprite->field_0;
            Data24->field_4 = pSprite->field_4 + 1;
            Data24->field_20 = pSprite->field_20 + 0x0E;

            if (!word_3B4ED[0])
                if (!pSprite->field_1E && !pSprite->field_20)
                    return;

            if (pSprite->field_20 < 0x0C)
                if (mMission_EngineTicks & 1)
                    return;

            Data24->field_A += 1;
            Data24->field_A &= 3;
            return;
        }
    }

    if (pSprite->field_26 != 0x7171) {

        pSprite->field_26 = 0x7171;
        pSprite->field_1A = 0x10000;

        Data0 = tool_RandomGet() & 0x0F;
        Data0 += 0x13;

        pSprite->field_2A = Data0;
    }

    dword_3B24B = -1;

    Data2C = 0;
    Sprite_Create_Smoke(pSprite, Data2C);
    Sprite_Movement_Calculate(pSprite);

    pSprite->field_1A = (((int64)pSprite->field_1A) + 0x2000);
    Data0 = ((int64)pSprite->field_1A) >> 16;
    pSprite->field_A += Data0;
    pSprite->field_A &= 0x0F;
    pSprite->field_2A -= 1;
    if (pSprite->field_2A >= 0) {

        pSprite->field_1E_Big -= 0x18000;
        if (pSprite->field_1E_Big >= 0)
            goto loc_1A404;

        pSprite->field_1E_Big = 0;
    }

    pSprite->field_18 = eSprite_Explosion;
    pSprite->field_26 = 0x1F50;
    pSprite->field_28 = -9;
    (pSprite + 1)->field_18 = eSprite_Helicopter_PropCrash;

    Data0 = tool_RandomGet() & 0x1FE;
    (pSprite + 1)->field_10 = Data0;
    (pSprite + 1)->field_36 = 0x60;

    Sprite_Destroy(pSprite + 2);

    if (pSprite->field_22 == eSprite_PersonType_Human)
        Sprites_HumanVehicles_Remove(pSprite);

    if (mSprite_Helicopter_DestroyLight)
        Sprite_Destroy(pSprite + 3);

    return;

loc_1A149:;
    pSprite->field_8 = 0x8B;
    if (pSprite->field_44)
        goto loc_1A217;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_26;
    DataC = pSprite->field_28;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    dword_3B24B = Data0;

    if (Data0 >= 0x60)
        goto loc_1A217;

    Data0 >>= 1;

    if (Data0 > pSprite->field_36)
        goto loc_1A217;

    if (pSprite->field_22 != eSprite_PersonType_Human) {
        pSprite->field_36 = Data0;
        if (!Data0) {
            Data8 = pSprite->field_0;
            DataC = pSprite->field_4;

            if (Map_Terrain_Get_Moveable_Wrapper(mTiles_NotFlyable, Data8, DataC, Data10, Data14))
                goto loc_1A316;

            Data8 = pSprite->field_0;
            Data8 -= 0x10;
            DataC = pSprite->field_4;

            if (Map_Terrain_Get_Moveable_Wrapper(mTiles_NotFlyable, Data8, DataC, Data10, Data14))
                goto loc_1A316;

            Data8 = pSprite->field_0;
            Data8 += 0x10;
            DataC = pSprite->field_4;

            if (Map_Terrain_Get_Moveable_Wrapper(mTiles_NotFlyable, Data8, DataC, Data10, Data14))
                goto loc_1A316;

            if (pSprite->field_20) {
                pSprite->field_1E_Big -= 0x8000;
            }
            goto loc_1A316;
        }

        goto loc_1A316;
    }

    pSprite->field_36 = Data0;
    if (!pSprite->field_6E)
        goto loc_1A316;

    pSprite->field_1E_Big -= 0xC000;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
    }
    Sprite_Handle_Helicopter_Terrain_Check(pSprite);
    goto loc_1A316;

loc_1A217:;
    if (pSprite->field_20 < 0x20) {
        pSprite->field_20++;
        if (pSprite->field_20 < 0x12)
            goto loc_1A316;
    }
    //loc_1A239
    if (pSprite->field_36 >= 0x30)
        goto loc_1A316;

    pSprite->field_36 += 2;
    if (dword_3B24B >= 0x60)
        goto loc_1A316;

    if (!pSprite->field_6E)
        goto loc_1A316;

    pSprite->field_1E_Big -= 0xC000;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
    }
    goto loc_1A316;

loc_1A316:;
    if (pSprite->field_26 >= 0 && pSprite->field_28 >= 0) {

        if (pSprite->field_20)
            Sprite_Movement_Calculate(pSprite);
    }

    if (pSprite->field_22 == eSprite_PersonType_Human && !pSprite->field_36 &&
        pSprite->field_20 >= 0x20) {
        Data0 = mMouseX + (mCameraX >> 16);
        Data0 -= 0x10;

        Data4 = mMouseY + (mCameraY >> 16);
        Data4 += 0x20;
        pSprite->field_36 = 0x1E;
        sub_23E01(pSprite, Data0, Data4);
        pSprite->field_36 = 0;

        Data8 = pSprite->field_3C;
        if (pSprite->field_22 != eSprite_PersonType_Human) {
            if (!pSprite->field_36)
                goto loc_1A404;
        }
        pSprite->field_A = Data8;
    }
    else {

        word_3B2F7 = 0x3A;
        Data0 = pSprite->field_26;
        Data4 = pSprite->field_28;
        sub_22CD7(pSprite, Data0, Data4);
    }

loc_1A404:;
    Data24 = pSprite + 1;
    Data24->field_8 = 0x8C;
    Data24->field_0 = pSprite->field_0;
    Data24->field_4 = pSprite->field_4;
    Data24->field_4 += 1;
    Data0 = pSprite->field_20;
    Data0 += 0x0E;
    Data24->field_20 = Data0;

    if (!word_3B4ED[0])
        if (!pSprite->field_1E_Big)
            goto loc_1A49C;

    if (pSprite->field_20 < 0x0C)
        if (mMission_EngineTicks & 1)
            goto loc_1A49C;

    Data24->field_A += 1;
    Data24->field_A &= 3;

loc_1A49C:;
    Data24++;
    Data24->field_8 = 0x8D;

    Data0 = pSprite->field_20;
    Data0 >>= 4;
    if (Data0 > 2)
        Data0 = 2;

    Data24->field_A = Data0;
    Data24->field_2C = eSprite_Draw_First;

    Data0 = pSprite->field_20;
    Data0 >>= 1;
    Data4 = Data0;
    Data0 += 9;

    Data0 += pSprite->field_0;
    Data24->field_0 = Data0;
    Data4 -= 1;
    Data4 += pSprite->field_4;
    Data24->field_4 = Data4;

    if (pSprite->field_22 == eSprite_PersonType_Human) {
        if (pSprite->field_20) {

            Data8 = pSprite->field_0;
            DataC = pSprite->field_0;
            DataC += 0x1E;
            Data10 = pSprite->field_4;
            Data10 -= 0x14;
            Data14 = pSprite->field_4;
            Data18 = pSprite->field_20;
            Data1C = Data18;
            Data1C += 0x0E;

            Sprite_Under_Vehicle(pSprite, Data8, DataC, Data10, Data14, Data18, Data1C);
        }
    }

    pSprite->field_22 = eSprite_PersonType_Human;
}

void cFodder::Sprite_Handle_Explosion(sSprite* pSprite) {
    if (pSprite->field_8 == 0x7C) {
        Sprite_Destroy_Wrapper_2(pSprite);
        return;
    }

    if (pSprite->field_8 == 0xC0 || pSprite->field_8 == 0x8E) {
        //loc_1A8D1

        int16 X_Left = pSprite->field_0;
        X_Left += 8;

        if (pSprite->field_26 == 0x5F5F)
            X_Left += pSprite->field_28;

        int16 X_Right = pSprite->field_0;
        X_Right += 0x24;

        int16 Y_Top = pSprite->field_4;
        Y_Top -= 0x20;

        int16 Y_Bottom = pSprite->field_4;
        Y_Bottom -= 6;

        const int16* Explosion_Area_PerFrame = mSprite_Explosion_Area_PerFrame;
        int16 Frame = pSprite->field_A;

        if (pSprite->field_8 == 0xC0)
            Frame += 2;
        Frame -= 1;

        if (Frame >= 0) {

            do {
                X_Left -= *Explosion_Area_PerFrame;
                X_Right -= *(Explosion_Area_PerFrame + 1);
                Y_Top += *Explosion_Area_PerFrame;

                ++Explosion_Area_PerFrame;
                Y_Bottom += *Explosion_Area_PerFrame;
                ++Explosion_Area_PerFrame;

            } while (--Frame >= 0);
        }
        //loc_1A99D
        sSprite* Data24 = 0;

        pSprite->field_62 = ~pSprite->field_62;
        if (pSprite->field_62 >= 0)
            Squad_Member_Sprite_Hit_In_Region(pSprite, X_Left, X_Right, Y_Top, Y_Bottom);
        else
            Sprite_Find_In_Region(pSprite, Data24, X_Left, X_Right, Y_Top, Y_Bottom);

        pSprite->field_12 -= 1;
        if (pSprite->field_12 >= 0)
            return;

        pSprite->field_12 = 1;

        Explosion_Area_PerFrame = mSprite_Explosion_Area_PerFrame;
        int16 Data4 = pSprite->field_A;
        if (pSprite->field_8 == 0xC0)
            Data4 += 2;

        //seg004:1CC9
        pSprite->field_0 -= Explosion_Area_PerFrame[Data4];
        pSprite->field_4 += Explosion_Area_PerFrame[Data4 + 1];
        pSprite->field_A += 1;
        if (pSprite->field_8 != 0xC0)
            goto loc_1AA63;

        if (pSprite->field_A < 4)
            return;

        Sprite_Destroy_Wrapper(pSprite);
        return;

    loc_1AA63:;
        if (pSprite->field_20 && pSprite->field_A >= 2) {
            pSprite->field_8 = 0xC0;
            pSprite->field_A = 0;
        }
        if (pSprite->field_A >= 7)
            Sprite_Destroy_Wrapper(pSprite);

        return;
    }

    //loc_1AA9F
    if (pSprite->field_5C) {
        Sprite_Handle_Explosion_MapTiles(pSprite);
        int16 Data0 = tool_RandomGet() & 0x1FE;
        int16 Data18 = Data0;
        Sprite_Create_Sparks(pSprite, Data18);
        Data18 += 0x100;
        Data18 &= 0x1FE;
        Sprite_Create_Sparks(pSprite, Data18);
    }
    //loc_1AAFD
    pSprite->field_0 -= 6;
    if (pSprite->field_0 < 0) {
        Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
        return;
    }

    //loc_1AB0D
    pSprite->field_4 += 4;
    if (pSprite->field_4 < 0) {
        Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
        return;
    }

    int16 Data0 = pSprite->field_26;
    if (Data0 < 0) {
        Data0 &= 0x7FFF;
        pSprite->field_0 += Data0;
        if (pSprite->field_0 < 0) {
            Sprite_Destroy_Wrapper_At_TopLeft(pSprite);
            return;
        }
        pSprite->field_26 = 0;
    }
    //loc_1AB6F
    int16 Data4 = 5 + (mGame_InputTicks & 3);

    Sound_Play(pSprite, Data4, 0x1E);
    pSprite->field_8 = 0x8E;
    pSprite->field_A = 0;
    pSprite->field_12 = 1;
    pSprite->field_52 = 0;
    pSprite->field_22 = eSprite_PersonType_None;
    pSprite->field_32 = -1;
    pSprite->field_2C = eSprite_Draw_First;
}

void cFodder::Sprite_Handle_Shrub(sSprite* pSprite) {
    pSprite->field_8 = 0x8F;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_Tree(sSprite* pSprite) {
    pSprite->field_8 = 0x90;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_BuildingRoof(sSprite* pSprite) {
    pSprite->field_8 = 0x91;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_Snowman(sSprite* pSprite) {
    pSprite->field_8 = 0x92;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_Shrub2(sSprite* pSprite) {
    pSprite->field_8 = 0x93;
    Sprite_Handle_Exploidable(pSprite);
}

void cFodder::Sprite_Handle_Waterfall(sSprite* pSprite) {
    pSprite->field_8 = 0x94;
    pSprite->field_2C = eSprite_Draw_First;

    int16 Data0 = pSprite->field_12;
    Data0 ^= 1;
    Data0 &= 1;
    pSprite->field_12 = Data0;

    if (Data0)
        return;

    pSprite->field_A += 1;
    if (pSprite->field_A < 3)
        return;

    pSprite->field_A = 0;
}

void cFodder::Sprite_Handle_Bird2_Left(sSprite* pSprite) {

    pSprite->field_8 = 0x98;
    pSprite->field_2C = eSprite_Draw_OnTop;
    pSprite->field_12 += 1;
    pSprite->field_12 &= 1;
    if (!pSprite->field_12)
        pSprite->field_A ^= 1;

    pSprite->field_0 -= 2;
    if (pSprite->field_0 < -64)
        pSprite->field_0 = 0x7D0;
}

void cFodder::Sprite_Handle_BuildingDoor(sSprite* pSprite) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (Sprite_Handle_BuildingDoor_Explode(pSprite))
        return;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax) {
        pSprite->field_8 = 0x99;
        return;
    }

    //loc_1ACEC
    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_1AD86;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14 || pSprite->field_43 == 0x0A) {

        Sprite_Create_Enemy(pSprite, Data2C);
        return;
    }

    if (pSprite->field_43 >= 0) {
        pSprite->field_8 = 0x7C;
        return;
    }

    pSprite->field_8 = 0x99;
    Sound_Play(pSprite, eSound_Effect_BuildingDoor2, 0x01);
    Data0 = tool_RandomGet() & 0x0F;

    Data4 = 0x14 - mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
    if (Data4 < 0)
        Data4 = 0;

    Data4 <<= 3;

    Data4 += Data0;
    pSprite->field_12 = Data4;

loc_1AD86:;
    pSprite->field_12 -= 1;
    if (pSprite->field_12 >= 0)
        return;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 += 6;
    pSprite->field_43 = static_cast<int8>(Data0);
    pSprite->field_8 = 0x7C;

    if (!Sprite_Create_Enemy(pSprite, Data2C))
        return;

    pSprite->field_12 = -1;
    pSprite->field_43 = -1;
}

void cFodder::Sprite_Handle_Player_Rank(sSprite* pSprite) {
    sSprite* Data24 = mSquad_Leader;
    int16 Data4, Data0;

    if (mSquad_Selected < 0)
        goto loc_1AF63;

    if (Data24 == INVALID_SPRITE_PTR)
        goto loc_1AF63;

    if (!mPhase_Completed_Timer) {
        Data4 = mSquad_Selected;

        if (mSquad_Walk_Target_Steps[Data4])
            goto loc_1AF63;
    }

    // No rank for non human soldiers, or if they're sinking
    if (Data24->field_18 != eSprite_Player || Data24->field_5B)
        goto loc_1AE64;

    if (!Data24->field_38)
        goto loc_1AE76;

    if (Data24->field_38 >= eSprite_Anim_Slide1)
        goto loc_1AE76;

loc_1AE64:;
    sub_305D5(Data24);
    goto loc_1AF63;

loc_1AE76:;

    Data0 = Data24->field_0;
    if (Data0 < 0)
        goto loc_1AF63;

    Data0 += 3;
    Data4 = Data24->field_4;

    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;

    if (Data24->field_6E)
        goto loc_1AF63;

    pSprite->field_8 = 0x95;

    Data0 = Data24->field_46_mission_troop->mRank;

    pSprite->field_A = Data0;
    Data0 = pSprite->field_3A;
    Data0 += pSprite->field_10;

    if (!Data0)
        goto loc_1AF1F;

    if (Data0 < 6)
        goto loc_1AF27;

loc_1AF1F:;
    pSprite->field_10 = -pSprite->field_10;

loc_1AF27:;
    Data0 = 0;
    pSprite->field_20 = 1;
    pSprite->field_3A = Data0;

    Data0 += Data24->field_20;
    Data0 += 0x12;
    pSprite->field_20 = Data0;
    return;

loc_1AF63:;
    pSprite->field_8 = 0x7C;
    pSprite->field_A = 0;
}

void cFodder::Sprite_Handle_Player_Shadow(sSprite* pSprite) {
    sSprite* Data28 = pSprite->field_1A_sprite;
    int16 Data0;

    if (Data28 != pSprite->field_6A_sprite)
        goto loc_1B06D;

    Data0 = pSprite->field_62;

    if (Data0 != Data28->field_18)
        goto loc_1B06D;

    if (Data28->field_8 == 0x7C)
        goto loc_1B06D;

    if (Data28->field_52)
        goto loc_1B06D;

    pSprite->field_8 = 0x97;
    pSprite->field_A = 1;
    if (Data28->field_20 > 8)
        pSprite->field_A = 0;

    pSprite->field_0 = Data28->field_0;
    pSprite->field_4 = Data28->field_4;

    Data0 = Data28->field_20;
    Data0 >>= 2;
    pSprite->field_0 += Data0;
    pSprite->field_4 += Data0;
    pSprite->field_0 += 5;
    pSprite->field_4 -= 3;

    if (Data28->field_8 == 0x7C)
        pSprite->field_8 = 0x7C;
    return;

loc_1B06D:;
    Data28->field_58 = 0;
    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_BloodTrail(sSprite* pSprite) {

    if (pSprite->field_20)
        --pSprite->field_20;

    --pSprite->field_12;
    if (pSprite->field_12 >= 0)
        return;

    pSprite->field_12 = 1;
    pSprite->field_A += 1;
    if (pSprite->field_A < 4)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_GroundHole(sSprite* pSprite) {
    if (!mEnemy_BuildingCount)
        return;

    if (pSprite->field_2A) {
        //loc_1B194
        sSprite* Data2C = 0;

        if (!Sprite_Create_Enemy(pSprite, Data2C)) {
            Data2C->field_0 += 6;
            Data2C->field_4 -= 5;
            Data2C->field_52 = 0x0C;
        }

        pSprite->field_2A = 0;
        return;
    }

    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_43 < 0) {
        //loc_1B161
        pSprite->field_12 -= 1;
        if (pSprite->field_12 >= 0)
            return;

        int16 Data0 = tool_RandomGet() & 0x0F;
        Data0 += 6;
        pSprite->field_43 = static_cast<int8>(Data0);
        pSprite->field_2A = 0;
        return;
    }

    pSprite->field_43 -= 1;
    if (pSprite->field_43 != 0x14 && pSprite->field_43 != 0x0A) {

        if (pSprite->field_43 >= 0)
            return;
    }

    //loc_1B11A
    pSprite->field_2A = -1;
    pSprite->field_A = 0;

    int16 Data0 = tool_RandomGet() & 0xFF;
    int16 Data4 = 0x64;
    Data4 -= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage;
    Data0 += 0x0A;
    Data4 += mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage;

    pSprite->field_12 = Data0;
    return;
}

void cFodder::Sprite_Handle_BuildingDoor2(sSprite* pSprite) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (sub_222A3(pSprite))
        return;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax) {
        pSprite->field_8 = 0x9B;
        return;
    }

    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_1B285;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14)
        goto loc_1B2B6;

    if (pSprite->field_43 == 0x0A)
        goto loc_1B2B6;

    if (pSprite->field_43 >= 0) {
        pSprite->field_8 = 0x7C;
        return;
    }

    pSprite->field_8 = 0x9B;

    Sound_Play(pSprite, eSound_Effect_BuildingDoor2, 1);
    Data0 = tool_RandomGet() & 0x0F;
    Data4 = 0x14;

    Data4 -= mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
    if (Data4 < 0)
        Data0 = 0;

    Data4 <<= 3;
    Data4 += Data0;
    pSprite->field_12 = Data4;

loc_1B285:;
    pSprite->field_12 -= 1;
    if (pSprite->field_12 >= 0)
        return;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 += 6;
    pSprite->field_43 = static_cast<int8>(Data0);
    pSprite->field_8 = 0x7C;

loc_1B2B6:;

    if (!Sprite_Create_Enemy(pSprite, Data2C)) {
        Data2C->field_4 -= 4;
        return;
    }

    pSprite->field_12 = 0;
    pSprite->field_43 = -1;
}

void cFodder::Sprite_Handle_Floating_Dead_Soldier(sSprite* pSprite) {
    int16 Data0, Data4;

    pSprite->field_8 = 0x9E;

    if (!pSprite->field_12) {
        pSprite->field_12 = 0x10;
        pSprite->field_10 = 0x100;
    }

    if (pSprite->field_10 <= 0x100)
        goto loc_1B35A;

    if (pSprite->field_12 == 0x10)
        goto loc_1B350;

    pSprite->field_12 = 0x10;
    goto loc_1B35A;

loc_1B350:;
    pSprite->field_12 = -16;

loc_1B35A:;

    Sprite_XY_Store(pSprite);
    Sprite_Movement_Calculate(pSprite);
    Data0 = -3;
    Data4 = 8;
    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);
    if (Data4 < eTerrainFeature_QuickSand || Data4 > eTerrainFeature_Water) {
        pSprite->field_0 = mStoredSpriteX & 0xFFFF;
        pSprite->field_4 = mStoredSpriteY & 0xFFFF;
        pSprite->field_36 = 0;
        Data0 = pSprite->field_12;

        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
    }

    //loc_1B3CA
    if (pSprite->field_36 < 4)
        pSprite->field_36++;

    pSprite->field_2C = eSprite_Draw_Second;
}

void cFodder::Sprite_Handle_Text_Complete(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 65;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += getCameraHeight() / 2 + 7;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Mission(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 50;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Phase(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 41;

    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Vehicle(sSprite* pSprite) {
    int16 Data0 = 2;
    int16 Data4 = pSprite->field_36;
    int16 Data8, DataC;

    if (!Data4) {
        if (pSprite->field_22 == eSprite_PersonType_Human) {

            if (pSprite != mSquad_CurrentVehicle)
                goto loc_1B523;
        }
    }

    Data4 += 0x10;
    if (Data4 > 0x40)
        Data4 = 0x40;
    Data0 = Data4;

    Data0 >>= 4;
    if (Data0 > 3)
        Data0 = 3;
    Data0 += 0x38;

    Sprite_Map_Sound_Play(Data0);

loc_1B523:;
    pSprite->field_65 = -1;
    if (Sprite_Animation_SlideOrDie(pSprite))
        return;

    pSprite->field_8 = 0xA5;
    sub_22C87(pSprite);
    if (pSprite->field_20 < 3) {
        word_3B2F7 = 0x48;
        Data0 = pSprite->field_26;
        Data4 = pSprite->field_28;

        sub_22CD7(pSprite, Data0, Data4);
    }

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 >= 0x1E)
        goto loc_1B5D2;

    if (Data0 >= pSprite->field_36)
        goto loc_1B655;

    pSprite->field_36 = Data0;
    goto loc_1B655;

loc_1B5D2:;

    if (!pSprite->field_52) {
        dword_3B24B = -1;
        sSprite* Data2C = 0;

        if (!Sprite_Create_Smoke(pSprite, Data2C))
            Data2C->field_2C = eSprite_Draw_First;
    }

    if (!pSprite->field_36) {
        if (mDirectionMod)
            goto loc_1B655;
    }

    // Sinking? No, Then we can speed up
    if (pSprite->field_52 < 4) {

        pSprite->field_36 += 3;

        // This vehicle only moves at a single speed
        if (pSprite->field_18 == eSprite_Vehicle_Unk_Enemy) {

            pSprite->field_36 = 0x14;

        }
        else {
            if (pSprite->field_36 >= 0x32)
                pSprite->field_36 = 0x32;
        }

    }
loc_1B655:;
    // VEhicle in the air, increase speed
    if (pSprite->field_20 > 4)
        pSprite->field_36 += 0x1C;

    Sprite_XY_Store(pSprite);
    Sprite_Movement_Calculate(pSprite);

    if (pSprite->field_20 < 9) {
        Sprite_Handle_Vehicle_Terrain_Check(pSprite);

        if (pSprite->field_20 <= 4)
            pSprite->field_20 = 0;
    }
    else {
        pSprite->field_36 += 0x40;
    }

    sub_243E9(pSprite);

    Data8 = pSprite->field_0;
    DataC = pSprite->field_0;
    DataC += 0x1C;
    int16 Data10 = pSprite->field_4;
    Data10 -= 0x10;
    int16 Data14 = pSprite->field_4;

    int16 Data18 = pSprite->field_20;
    int16 Data1C = Data18;
    Data1C += 0x0E;
    Sprite_Under_Vehicle(pSprite, Data8, DataC, Data10, Data14, Data18, Data1C);

}

void cFodder::Sprite_Handle_Rocket(sSprite* pSprite) {
    sSprite* Data24 = 0;

    if (pSprite->field_56) {

        pSprite->field_56 -= 1;
        if (!pSprite->field_56) {
            if (mVersionCurrent->isCoverDisk())
                Sound_Play(pSprite, 0x10, 0x0F);
            else
                Sound_Play(pSprite, eSound_Effect_Rocket, 0x0F);

        }
        Data24 = pSprite->field_46_sprite;
        pSprite->field_0 = Data24->field_0;
        pSprite->field_2 = Data24->field_2;
        pSprite->field_4 = Data24->field_4;
        pSprite->field_6 = Data24->field_6;

        pSprite->field_4 += 1;
        pSprite->field_0 += 3;
        pSprite->field_8 = 0x7C;
        pSprite->field_A = 0;
        return;
    }

    pSprite->field_8 = 0xA3;
    int16 Data0 = pSprite->field_26;
    int16 Data4 = pSprite->field_28;
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 <= 7)
        goto loc_1B843;

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;
    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_A = Data0;

    Data0 = -9;
    Data4 = 2;

    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        return;

loc_1B843:;
    // Hit Target
    pSprite->field_18 = eSprite_Explosion;
    Sprite_Projectile_HitTarget(pSprite);
    Data24 = pSprite + 1;

    Sprite_Destroy(Data24);
}

void cFodder::Sprite_Handle_Text_GameOver(sSprite* pSprite) {

    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += 0x5B;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += 0x77;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Shadow(sSprite* pSprite) {
    sSprite* Data28 = pSprite->field_1A_sprite;

    if (Data28->field_18 == eSprite_Explosion) {
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    if (Data28->field_52) {
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    pSprite->field_8 = 0x8D;
    int16 Data0 = Data28->field_20;

    Data0 >>= 3;
    Data0 += 1;
    if (Data0 > 2)
        Data0 = 1;

    pSprite->field_A = Data0;
    pSprite->field_0 = Data28->field_0;
    pSprite->field_4 = Data28->field_4;
    Data0 = Data28->field_20;
    Data0 >>= 2;
    pSprite->field_0 += Data0;
    pSprite->field_4 += Data0;
    pSprite->field_0 += 5;
    pSprite->field_4 -= 3;

    if (Data28->field_8 == 0x7C)
        pSprite->field_8 = 0x7C;
}

void cFodder::Sprite_Handle_Enemy_Rocket(sSprite* pSprite) {

    if (Sprite_Handle_Soldier_Animation(pSprite))
        return;

    if (pSprite->field_38 != eSprite_Anim_None)
        return;

    mSprite_FaceWeaponTarget = 0;
    pSprite->field_43 = -1;

    sub_21CD1(pSprite);

    mTroop_Weapon_Bullet_Disabled = true;
    mTroop_Weapon_Grenade_Disabled = true;
    mTroop_Weapon_Rocket_Disabled = true;

    Sprite_Handle_Troop_Weapon(pSprite);
    sub_2A3D4(pSprite);

    mSprite_FaceWeaponTarget = 0;
    word_3ABB1 = 0;

    pSprite->field_54 = 3;
    pSprite->field_5A = -1;
    pSprite->field_55 = 0;
    pSprite->field_A = 0;

    sub_1FDE7(pSprite);
    pSprite->field_A = 2;
}

void cFodder::Sprite_Handle_GrenadeBox(sSprite* pSprite) {

    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xC2;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Grenades[mSquad_Selected] += 4;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_RocketBox(sSprite* pSprite) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xC3;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] += 4;

    // Plus uses homing missiles
    if (mVersionCurrent->isCoverDisk())
        mSquad_Leader->field_75 |= eSprite_Flag_HomingMissiles;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Building_Explosion(sSprite* pSprite) {
    int16 Data0, Data4, Data6, Data8, DataC, Data10, Data14;
    const int16* Data2C = mSprite_Explosion_Area_PerFrame;
    sSprite* Data24 = 0;

    if (pSprite->field_8 == 0x7C) {
        Sprite_Destroy_Wrapper_2(pSprite);
        return;
    }

    pSprite->field_62 = ~pSprite->field_62;
    if (pSprite->field_62 < 0)
        goto loc_1BC07;


    Data8 = pSprite->field_0;
    Data8 += 8;
    if (pSprite->field_26 == 0x5F5F)
        Data8 += pSprite->field_28;

    DataC = pSprite->field_0 + 0x24;

    Data10 = pSprite->field_4 - 0x20;
    Data14 = pSprite->field_4 - 6;

    Data0 = pSprite->field_A;

    if (pSprite->field_8 == 0xC0)
        Data0 += 2;

    Data0 -= 1;

    for (; Data0 >= 0; --Data0) {
        Data8 -= *Data2C;
        DataC -= *Data2C++;
        Data10 += *Data2C;
        Data14 += *Data2C++;
    }

    Squad_Member_Sprite_Hit_In_Region(pSprite, Data8, DataC, Data10, Data14);
    goto loc_1BC48;

loc_1BC07:;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_0 + 0x1E;
    Data10 = pSprite->field_4 - 30;
    Data14 = pSprite->field_4;

    Sprite_Find_In_Region(pSprite, Data24, Data8, DataC, Data10, Data14);
loc_1BC48:;

    Data2C = mSprite_Explosion_Area_PerFrame;
    Data4 = pSprite->field_A;
    if (pSprite->field_8 == 0xC0)
        Data4 += 2;

    Data4 <<= 1;
    Data2C += Data4;

    Data4 = *(Data2C + 1);
    Data6 = *Data2C;

    pSprite->field_0 -= Data4;
    pSprite->field_4 += Data6;

    pSprite->field_A += 1;
    if (pSprite->field_8 == 0xC0) {
        if (pSprite->field_A < 4)
            return;

        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

    if (!pSprite->field_20 || pSprite->field_A < 2)
        goto loc_1BD06;

    pSprite->field_8 = 0xC0;
    pSprite->field_A = 0;
loc_1BD06:;

    if (pSprite->field_A < 3)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Grenade_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Grenade;
    Sprite_Handle_Helicopter_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Flashing_Light(sSprite* pSprite) {

    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_8 = 0xC4;

    pSprite->field_43++;
    pSprite->field_43 &= 1;
    if (!pSprite->field_43) {
        pSprite->field_A++;
        pSprite->field_A &= 3;
    }

    pSprite->field_4 += 0x18;
    pSprite->field_20 += 0x18;
}

void cFodder::Sprite_Handle_Helicopter_Grenade2_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter;
    Sprite_Handle_Helicopter_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Missile_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Missile;
    Sprite_Handle_Helicopter_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Homing_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Homing;
    Sprite_Handle_Helicopter_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Missile(sSprite* pSprite) {

    dword_3B24B = -1;
    Sprite_Create_FireTrail(pSprite);

    pSprite->field_8 = 0xA3;
    int16 Data0 = pSprite->field_26;
    int16 Data4 = pSprite->field_28;
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;
    int16 Data10 = 0x10;

    Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
    if (Data0 <= 7)
        goto loc_1BECD;

    if (pSprite->field_20 > 4) {
        pSprite->field_1E_Big -= 0xA000;
    }

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    if (mSprite_Bullet_Destroy)
        goto loc_1BECD;

    Data0 = pSprite->field_10;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_A = Data0;

    if (pSprite->field_36 < 0x60)
        pSprite->field_36 += pSprite->field_3A;

    Data0 = mMission_EngineTicks;
    Data0 &= 3;

    if (!Data0)
        pSprite->field_3A <<= 1;

    if (!pSprite->field_32)
        return;

    Data0 = -9;
    Data4 = 2;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        return;

loc_1BECD:;

    pSprite->field_18 = eSprite_Explosion2;
    pSprite->field_4 -= 4;
    Sprite_Projectile_HitTarget(pSprite);

    Sprite_Destroy(pSprite + 1);
}

void cFodder::Sprite_Handle_MissileHoming(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data34 = 0;

    if (pSprite->field_38)
        goto MissileExplode;

    dword_3B24B = -1;
    Sprite_Create_FireTrail(pSprite);

    pSprite->field_8 = 0xA3;
    Data34 = pSprite->field_1A_sprite;

    if (!Data34 || Data34->field_0 == -32768) {
        pSprite->field_36 += 2;
        pSprite->field_20 -= 2;
        if (pSprite->field_20 >= 0) {
            Sprite_Movement_Calculate(pSprite);
            return;
        }

        pSprite->field_20 = 0;
        goto MissileExplode;
    }

    Data0 = Data34->field_0 + 8;
    Data4 = Data34->field_4 + 8;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    Data4 = pSprite->field_36 >> 4;
    Data4 += 1;

    if (Data0 <= Data4)
        goto MissileExplode;

    if (Data34->field_20 <= 0x10) {

        // Distance to target > 0x30?
        if (Data0 > 0x30) {
            Data0 = pSprite->field_0;
            Data4 = pSprite->field_4;
            Data8 = Data34->field_0;
            DataC = Data34->field_4;
            Data8 += 8;
            DataC += 8;

            if (!Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC)) {
                if (pSprite->field_20 > 8)
                    pSprite->field_1E_Big -= 0x12000;
            }
            else {

                if (pSprite->field_20 < 0x18)
                    pSprite->field_1E_Big += 0x28000;
            }

        }
        else {
            // Near Target
            Data4 = Data34->field_20;
            Data4 -= pSprite->field_20;
            Data4 >>= 3;
            pSprite->field_20 += Data4;
        }
    }
    else {

        Data4 = pSprite->field_20;
        if (Data4 != Data34->field_20) {

            if (Data4 > Data34->field_20)
                pSprite->field_1E_Big -= 0x8000;
            else
                pSprite->field_1E_Big += 0x8000;
        }
    }

    Data0 = Data34->field_0;
    Data0 += 8;
    Data4 = Data34->field_4;
    Data4 += 8;
    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Movement_Calculate(pSprite);

    Data4 = pSprite->field_A;
    Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_A = Data0;

    if (pSprite->field_22 == eSprite_PersonType_Human || Data4 == Data0) {
        if (pSprite->field_36 <= 0x3C) {

            Data0 = (int16)(pSprite->field_6A >> 16);
            pSprite->field_36 += Data0;
            pSprite->field_6A = pSprite->field_6A + 0x200;
        }
        else {
            pSprite->field_36 = 0x3C;
        }

        return;
    }

    if (pSprite->field_36 > 0x1E) {

        pSprite->field_6A = 0;
        pSprite->field_36 -= 2;
        if (pSprite->field_36 < 0)
            pSprite->field_36 = 0;
    }

    return;

MissileExplode:;
    pSprite->field_18 = eSprite_Explosion2;
    pSprite->field_4 -= 4;
    Sprite_Projectile_HitTarget(pSprite);
    Sprite_Destroy(pSprite + 1);

}

void cFodder::Sprite_Handle_Sparks(sSprite* pSprite) {
    Sprite_Movement_Calculate(pSprite);
    int32 Data0 = (int32)pSprite->field_1A;

    Data0 += pSprite->field_1E;

    pSprite->field_1E_Big = Data0;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
        Data0 = -Data0;
        Data0 >>= 1;
    }

    Data0 -= 0x22000;
    pSprite->field_1A = Data0;

    Data0 = Data0 >> 16;
    if (!pSprite->field_A)
        goto loc_1C248;

    if (pSprite->field_20)
        goto loc_1C262;

    if (Data0 > 1)
        goto loc_1C262;

loc_1C248:;
    pSprite->field_A++;
    if (pSprite->field_A >= 3) {
        Sprite_Destroy_Wrapper(pSprite);
        return;
    }

loc_1C262:;
    Sprite_Create_FireTrail(pSprite);
}

void cFodder::Sprite_Handle_FireTrail(sSprite* pSprite) {
    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (pSprite->field_2A)
        return;

    pSprite->field_A += 1;
    if (pSprite->field_A != 4)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Grenade_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter;
    Sprite_Handle_Helicopter_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Grenade2_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Grenade;
    Sprite_Handle_Helicopter_Human(pSprite);
}


void cFodder::Sprite_Handle_Helicopter_Missile_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Missile;
    Sprite_Handle_Helicopter_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Homing_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Helicopter_Homing;
    Sprite_Handle_Helicopter_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_PropCrash(sSprite* pSprite) {
    pSprite->field_A += 1;
    pSprite->field_A &= 3;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1C321;

    pSprite->field_36 -= 1;
    if (pSprite->field_36 < 0)
        pSprite->field_36 = 0;

    pSprite->field_1E_Big -= 0x8000;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
    }

loc_1C321:;
    pSprite->field_18 = eSprite_Explosion;
    Sprite_Create_FireTrail(pSprite);
}

void cFodder::Sprite_Handle_Mine(sSprite* pSprite) {
    pSprite->field_2C = eSprite_Draw_First;

    if (!pSprite->field_38) {
        int16 Data0;
        pSprite->field_8 = 0xC7;
        Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0);
    }
    else {
        pSprite->field_18 = eSprite_Explosion2;
    }
}

void cFodder::Sprite_Handle_Mine2(sSprite* pSprite) {
    int16 Data0;

    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_38)
        goto loc_1C406;

    if (pSprite->field_2A) {
        pSprite->field_2A += 1;
        if (pSprite->field_2A >= 0x0A)
            goto loc_1C406;
    }

    Data0 = 0;
    pSprite->field_8 = 0xC8;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    if (mSquad_Leader->field_38)
        return;

    if (!mSquad_Leader->field_36)
        return;

    if (mSquad_Leader->field_52)
        return;

    if (mSquad_Leader->field_20)
        return;

    mSquad_Leader->field_38 = eSprite_Anim_Slide1;
    mSquad_Leader->field_2A = 1;
    return;

loc_1C406:;
    pSprite->field_18 = eSprite_Explosion;
}

void cFodder::Sprite_Handle_Spike(sSprite* pSprite) {
    sSprite* Data28 = mSquad_Leader;
    int16 Data0 = 0;

    pSprite->field_8 = 0xC9;

    if (pSprite->field_A)
        goto loc_1C4AD;

    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    if (Data0 >= 6)
        return;

    if (Data0 == 5)
        goto loc_1C4AD;

    Data28->field_0 = pSprite->field_0;
    Data28->field_0 -= 3;
    Data28->field_4 = pSprite->field_4;
    Data28->field_4 += 1;
    Data28->field_38 = eSprite_Anim_Die4;
    Data28->field_64 = -1;
    Data28->field_20 = 7;

loc_1C4AD:;
    if (pSprite->field_A >= 2)
        return;

    pSprite->field_A += 1;
}

void cFodder::Sprite_Handle_Smoke(sSprite* pSprite) {
    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (pSprite->field_2A)
        return;

    pSprite->field_A -= 1;
    if (pSprite->field_A >= 0)
        return;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Text_Try(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 23;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += (getCameraHeight() / 2 - 13);

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Again(sSprite* pSprite) {
    pSprite->field_0 = mMapTile_TargetX >> 16;
    pSprite->field_0 += (getCameraWidth() / 2) - 49;
    pSprite->field_4 -= 0x20;

    int16 Data0 = mMapTile_TargetY >> 16;
    Data0 += getCameraHeight() / 2 + 5;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_BoilingPot(sSprite* pSprite) {
    pSprite->field_8 = 0xCD;
    pSprite->field_A += 1;
    pSprite->field_A &= 3;

    sSprite* Data24 = pSprite + 1;
    Data24->field_8 = 0xCE;
    Data24->field_2A += 1;
    Data24->field_2A &= 1;

    if (!Data24->field_2A) {
        Data24->field_A += 1;

        if (Data24->field_A >= 6)
            Data24->field_A = 0;
    }

    Data24->field_20 = 0x10;
}

void cFodder::Sprite_Handle_Civilian(sSprite* pSprite) {
    if (pSprite->field_38) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);

    if (ax) {
        if (ax >= 0)
            return;

        Sprite_Handle_Civilian_Movement(pSprite);
    }

    pSprite->field_36 = 6;
    sub_2593D(pSprite);
    sub_25A31(pSprite);
    sub_25A66(pSprite);
}

void cFodder::Sprite_Handle_Civilian2(sSprite* pSprite) {

    if (pSprite->field_38) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);
    if (ax > 0)
        return;
    if (ax < 0)
        Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->field_36 = 0x0A;
    sub_2593D(pSprite);
    sub_25A31(pSprite);
    sub_25A66(pSprite);
    if (!word_3B2D1[1])
        return;

    Sprite_Handle_Civilian_Unk(pSprite);
}

void cFodder::Sprite_Handle_VehicleNoGun_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_Jeep;
    Sprite_Handle_Vehicle_Human(pSprite);
}

void cFodder::Sprite_Handle_VehicleGun_Human(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_JeepRocket;
    Sprite_Handle_Vehicle_Human(pSprite);
}

void cFodder::Sprite_Handle_Tank_Human(sSprite* pSprite) {

    if (pSprite->field_38 == eSprite_Anim_Die1)
        pSprite->field_38 = eSprite_Anim_None;

    if (pSprite->field_38) {
        pSprite->field_22 = eSprite_PersonType_Human;

        sub_23525(pSprite);
        pSprite->field_22 = eSprite_PersonType_Human;
        return;
    }
    pSprite->field_22 = eSprite_PersonType_Human;
    sub_23525(pSprite);
    pSprite->field_22 = eSprite_PersonType_Human;
    Sprite_Handle_Tank_FireMissile(pSprite);

    sSprite* Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;
}

void cFodder::Sprite_Handle_Bird_Left(sSprite* pSprite) {

    pSprite->field_8 = 0xD3;
    pSprite->field_2C = eSprite_Draw_OnTop;

    if (!pSprite->field_12) {

        pSprite->field_57 = 8;
        pSprite->field_12 = 1;

        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 3;
        Data0 += 1;

        pSprite->field_A = Data0;
        if (Data4 < 0)
            pSprite->field_12 = -1;
    }

    int16 Data0 = pSprite->field_12;
    pSprite->field_A += Data0;

    if (pSprite->field_A) {
        if (pSprite->field_A < 5)
            goto loc_1C82D;
    }

    Data0 = -Data0;
    pSprite->field_12 = Data0;

loc_1C82D:;
    bool cf = false;

    if (pSprite->field_2 >= 0 && pSprite->field_2 - 32768 < 0)
        cf = true;

    pSprite->field_2 -= 32768;
    pSprite->field_0 -= 1 + (cf == true ? 1 : 0);


    if (Data0 < 0) {
        if (pSprite->field_2 >= 0 && pSprite->field_2 - 32768 < 0)
            cf = true;
        else
            cf = false;

        pSprite->field_2 -= 32768;
        pSprite->field_0 -= 0 + (cf == true ? 1 : 0);
    }

    if (pSprite->field_5C)
        goto loc_1C8C5;

    if (!pSprite->field_57)
        goto loc_1C87E;

    pSprite->field_57 -= 1;
    goto loc_1C8C5;

loc_1C87E:;

    pSprite->field_57 = 0x3F;

	Data0 = (tool_RandomGet() & 0x3F) + (mMapTile_TargetX >> 16) + getWindowWidth();
    pSprite->field_0 = Data0;

    Data0 = (tool_RandomGet() & 0xFF) + (mMapTile_TargetY >> 16);
    pSprite->field_4 = Data0;

loc_1C8C5:;

    if (mMapLoaded->getTileType() == eTileTypes_Jungle)
        Sprite_Native_Sound_Play(pSprite, 0x1A);

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX)
        Sprite_Native_Sound_Play(pSprite, 0x1F);

}

void cFodder::Sprite_Handle_Bird_Right(sSprite* pSprite) {
    pSprite->field_8 = 0xD4;
    pSprite->field_2C = eSprite_Draw_OnTop;

    if (!pSprite->field_12) {
        pSprite->field_57 = 8;
        pSprite->field_12 = 1;

        int16 Data0 = tool_RandomGet();
        int16 Data4 = Data0;
        Data0 &= 3;
        Data0 += 1;

        pSprite->field_A = Data0;
        if (Data4 < 0)
            pSprite->field_12 = -1;
    }

    //loc_1C950
    int16 Data0 = pSprite->field_12;
    pSprite->field_A += Data0;
    if (!pSprite->field_A || pSprite->field_A >= 5) {
        Data0 = -Data0;
        pSprite->field_12 = Data0;
    }

    bool cf = false;

    if (pSprite->field_2 < 0 && pSprite->field_2 + 32768 > 0)
        cf = true;

    pSprite->field_2 += 32768;
    pSprite->field_0 += 1 + (cf == true ? 1 : 0);

    if (Data0 < 0) {
        if (pSprite->field_2 < 0 && pSprite->field_2 + 32768 > 0)
            cf = true;
        else
            cf = false;

        pSprite->field_2 += 32768;
        pSprite->field_0 += 0 + (cf == true ? 1 : 0);
    }

    //loc_1C9B0
    if (pSprite->field_5C)
        goto loc_1CA20;

    if (pSprite->field_57) {
        pSprite->field_57 -= 1;
        goto loc_1CA20;
    }

    //loc_1C9D3
    pSprite->field_57 = 0x3F;
    Data0 = tool_RandomGet() & 0x3F;
    Data0 = -Data0;
    Data0 += mMapTile_TargetX >> 16;

    pSprite->field_0 = Data0;
    Data0 = tool_RandomGet() & 0xFF;
    Data0 += mMapTile_TargetY >> 16;
    pSprite->field_4 = Data0;

loc_1CA20:;
    if (mMapLoaded->getTileType() == eTileTypes_Jungle)
        Sprite_Native_Sound_Play(pSprite, 0x1A);

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX)
        Sprite_Native_Sound_Play(pSprite, 0x1F);
}

void cFodder::Sprite_Handle_Seal(sSprite* pSprite) {
    if (sub_25DCF(pSprite))
        return;

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD5;

    if (!pSprite->field_2E) {
        pSprite->field_2E = -1;
        pSprite->field_54 = tool_RandomGet() & 0x0F;
    }

    if (mMission_EngineTicks & 1)
        ++pSprite->field_54;

    int16 Data0 = pSprite->field_54 & 0x0F;

    Data0 = mSprite_Seal_AnimFrames[Data0];
    pSprite->field_A = Data0;

    if (mMapLoaded->getTileType() == eTileTypes_Moors) {
        Data0 = tool_RandomGet() & 3;
        if (Data0 != 3) {
            Data0 += 0x23;
            Sprite_Native_Sound_Play(pSprite, Data0);
        }
    }

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX) {
        Sprite_Native_Sound_Play(pSprite, 0x1E);
    }
}

void cFodder::Sprite_Handle_Tank_Enemy(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10, Data14;
    sSprite* Data24 = 0, *Data30 = 0;

    if (pSprite->field_38 == eSprite_Anim_Die1)
        pSprite->field_38 = eSprite_Anim_None;

    if (pSprite->field_38) {
        pSprite->field_22 = eSprite_PersonType_AI;
        sub_23525(pSprite);
        pSprite->field_22 = eSprite_PersonType_AI;

        Data24 = pSprite + 2;
        if (Data24->field_18 == eSprite_Flashing_Light)
            Sprite_Destroy(Data24);

        return;
    }

    if (pSprite->field_4C)
        pSprite->field_4C--;

    int16 Data1C = pSprite->field_5E_Squad;
    if (mSquads[Data1C / 9] == (sSprite**)INVALID_SPRITE_PTR)
        goto NextSquadMember;

    Data30 = mSquads[Data1C / 9][Data1C % 9];
    if (Data30 == INVALID_SPRITE_PTR)
        goto NextSquadMember;

    if (Data30->field_52)
        goto NextSquadMember;

    Data8 = Data30->field_0;
    Data8 += 8;

    DataC = Data30->field_4;
    DataC += -5;

    if (Map_Terrain_Get_Moveable_Wrapper(mTiles_NotDriveable, Data8, DataC, Data10, Data14))
        goto NextSquadMember;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = Data30->field_0;
    DataC = Data30->field_4;

    mSprite_Tank_SpriteX = Data0;
    mSprite_Tank_SpriteY = Data4;
    mSprite_Tank_Squad0_X = Data8;
    mSprite_Tank_DistanceTo_Squad0 = DataC;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    mSprite_DistanceTo_Squad0 = Data0;
    if (Data0 >= 250)
        goto NextSquadMember;

    Data0 = mSprite_Tank_SpriteX;
    Data4 = mSprite_Tank_SpriteY;
    Data8 = mSprite_Tank_Squad0_X;
    DataC = mSprite_Tank_DistanceTo_Squad0;
    Data8 += 0x0F;
    DataC -= 0x0A;

    if (Map_PathCheck_CalculateTo(Data0, Data4, Data8, DataC))
        goto NextSquadMember;

    pSprite->field_2E = Data30->field_0;
    pSprite->field_30 = Data30->field_4;
    pSprite->field_4C = 0x5A;

    // If we have reached our current target
    if (pSprite->field_0 == pSprite->field_26 && pSprite->field_4 == pSprite->field_28) {

        if (!(tool_RandomGet() & 0x3F)) {
            pSprite->field_26 = Data30->field_0;
            pSprite->field_28 = Data30->field_4;
        }
    }

    if (mSprite_DistanceTo_Squad0 <= 0x32)
        goto loc_1CDA3;

    Data0 = tool_RandomGet() & 0x1F;
    if (Data0 != 4)
        goto loc_1CDA3;

    pSprite->field_54 = -1;
    Sprite_Handle_Tank_FireMissile(pSprite);
    goto loc_1CDA3;

NextSquadMember:;
    pSprite->field_5E_Squad += 1;
    if (pSprite->field_5E_Squad >= 0x1E) {
        pSprite->field_5E_Squad = 0;
        pSprite->field_2E = -1;
    }

loc_1CDA3:;
    mSprite_Helicopter_DestroyLight = -1;
    pSprite->field_22 = eSprite_PersonType_AI;
    sub_23525(pSprite);
    pSprite->field_22 = eSprite_PersonType_AI;
    mSprite_Helicopter_DestroyLight = 0;

    if (mSprite_Reached_Target) {
        pSprite->field_4C = 0;
        pSprite->field_26 = pSprite->field_0;
        pSprite->field_28 = pSprite->field_4;
    }

    Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;

    Data24 = pSprite + 2;
    Data24->field_18 = eSprite_Flashing_Light;
    Data24->field_0 = pSprite->field_0 + 0x0F;
    Data24->field_4 = (pSprite->field_4 - 0x16) - pSprite->field_52;
    Data24->field_20 = pSprite->field_20;
}

void cFodder::Sprite_Handle_Civilian_Spear(sSprite* pSprite) {
    pSprite->field_22 = eSprite_PersonType_AI;
    if (pSprite->field_38) {
        Sprite_Handle_Civilian_Death(pSprite);
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    if (mMapLoaded->getTileType() == eTileTypes_Moors)
        Sprite_Native_Sound_Play(pSprite, 0x26);

    if (mMapLoaded->getTileType() == eTileTypes_Int)
        Sprite_Native_Sound_Play(pSprite, 0x1F);

    int16 ax = Sprite_Handle_Civilian_Within_Range_OpenCloseDoor(pSprite);
    if (ax > 0)
        return;
    if (ax)
        Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->field_36 = 0x0C;

    sub_2593D(pSprite);
    sub_25A31(pSprite);
    sub_25A66(pSprite);
    Sprite_Handle_Civilian_Unk(pSprite);
}

void cFodder::Sprite_Handle_Civilian_Spear2(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data1C;

    ++pSprite->field_64;

    if (!pSprite->field_12)
        goto loc_1D0F6;

    pSprite->field_12 -= 1;
    pSprite->field_3A += 1;

    if (pSprite->field_44)
        goto loc_1CFF7;

    Sprite_XY_Store(pSprite);
    Data1C = pSprite->field_10;

    if (pSprite->field_59)
        goto loc_1CF3E;

    Data0 = pSprite->field_2E;
    Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Data0 = pSprite->field_50;

    if (Data0) {
        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
        pSprite->field_59 = -1;
    }

    if (pSprite->field_34 < 0)
        pSprite->field_34 = pSprite->field_10;

loc_1CF3E:;
    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1D17A;

    pSprite->field_36 = pSprite->field_4A;
    if (pSprite->field_59)
        goto loc_1D00F;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    Data4 = pSprite->field_36;
    Data4 >>= 3;
    if (Data0 > Data4)
        goto loc_1D00F;

    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_0 = pSprite->field_2E;
    pSprite->field_4 = pSprite->field_30;
    pSprite->field_44 = -1;
    goto loc_1D00F;
loc_1CFF7:;
    pSprite->field_2C = eSprite_Draw_First;
    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1D17A;

loc_1D00F:;
    if (pSprite->field_64 > 2) {
        Data0 = -9;
        Data4 = 0;
        if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
            goto loc_1D0F6;
    }

    if (Sprite_Projectile_Collision_Check(pSprite))
        goto loc_1D07B;

    if (pSprite->field_8 != 0xD7)
        return;

    Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    Data0 >>= 1;
    pSprite->field_A = Data0;
    return;
loc_1D07B:;

    if (mSprites_Found_Count == 1)
        goto loc_1D0F6;

    pSprite->field_43 = 1;
    pSprite->field_2C = eSprite_Draw_OnTop;
    pSprite->field_4 -= 5;
    if (pSprite->field_4 < 0)
        goto loc_1D18D;
    pSprite->field_8 = 0x96;
    pSprite->field_A = 0;

    Sound_Play(pSprite, eSound_Effect_Spear, 0x0F);
    return;

loc_1D0CB:;
    pSprite->field_8 = 0x96;
    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if (pSprite->field_A >= 4)
        goto loc_1D17A;
    return;

loc_1D0F6:;
    if (pSprite->field_43)
        goto loc_1D14D;

    pSprite->field_43 = -1;
    pSprite->field_2C = eSprite_Draw_First;
    pSprite->field_4 -= 5;
    if (pSprite->field_4 < 0)
        goto loc_1D18D;

    pSprite->field_A = 3;
    Sound_Play(pSprite, eSound_Effect_Spear, 0x0F);
loc_1D14D:;
    if (pSprite->field_43 >= 0)
        goto loc_1D0CB;

    pSprite->field_12 = 0;
    pSprite->field_8 = 0x7F;
    pSprite->field_A += 1;
    if (pSprite->field_A >= 8)
        goto loc_1D17A;
    return;

loc_1D17A:;
    --mSprite_Projectile_Counters[2];
    Sprite_Destroy(pSprite);
    return;

loc_1D18D:;
    pSprite->field_0 = 0;
    pSprite->field_4 = 0x1000;
    goto loc_1D17A;
}

void cFodder::Sprite_Handle_Hostage(sSprite* pSprite) {
    int16 Data0, Data4;
    int16 Sprite_Field_10;
    sSprite* Data24 = 0;

    if (pSprite->field_6E)
        goto loc_1D349;

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD9;

    word_3B2D1[2] = pSprite->field_0;
    word_3B2D1[3] = pSprite->field_2;
    word_3B2D1[4] = pSprite->field_4;
    word_3B2D1[5] = pSprite->field_6;

    Sprite_Handle_Hostage_Movement(pSprite);
    Sprite_Handle_Civilian_Movement(pSprite);

    pSprite->field_36 = 0x0C;
    sub_2593D(pSprite);
    pSprite->field_38 = eSprite_Anim_None;

    Sprite_Handle_Hostage_FrameUpdate2(pSprite);
    //seg004:44B6

    Sprite_Field_10 = pSprite->field_10;
    pSprite->field_10 += 0x20;
    pSprite->field_10 &= 0x1C0;

    Sprite_Handle_Hostage_FrameUpdate(pSprite);
    pSprite->field_10 = Sprite_Field_10;
    sub_25F2B(pSprite);

    if (pSprite->field_18 != eSprite_Enemy_Leader)
        return;

    Data24 = pSprite + 1;

    //seg004:4505
    if (pSprite->field_5E) {
        Data24->field_18 = eSprite_Null;
        Data24->field_8 = 0x7C;
        Data24->field_A = 0;
    }

    Data24->field_18 = eSprite_Flashing_Light;

    Data4 = pSprite->field_0;
    Data4 += 6;

    Data24->field_0 = Data4;
    Data4 = pSprite->field_4 - 0x0B;
    Data4 += pSprite->field_52;
    Data24->field_4 = Data4;
    Data24->field_20 = pSprite->field_20;

    if (pSprite->field_6E) {
        Data24->field_18 = eSprite_Null;
        Data24->field_8 = 0x7C;
        Data24->field_A = 0;
    }

    Data0 = pSprite->field_0;
    if (Data0 == mStoredSpriteX) {
        Data0 = pSprite->field_4;
        if (Data0 == mStoredSpriteY)
            return;
    }
    pSprite->field_8 = 0xD8;
    return;

loc_1D349:;
    pSprite->field_38 = eSprite_Anim_None;
    sSprite* Data28 = &mSprites[pSprite->field_5E];
    if (Data28->field_6E)
        goto loc_1D44C;

    pSprite->field_0 = Data28->field_0;
    pSprite->field_4 = Data28->field_4;
    pSprite->field_0 += 8;
    pSprite->field_4 += 0x0A;
    mStoredSpriteY = pSprite->field_4;

loc_1D3C6:;
    Data0 = -3;
    Data4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_1D411;

    pSprite->field_4 -= 1;
    if (pSprite->field_4 >= 0)
        goto loc_1D3C6;

    pSprite->field_4 = mStoredSpriteY;
    pSprite->field_0 -= 1;
    if (pSprite->field_0 >= 0)
        goto loc_1D3C6;

    pSprite->field_0 = 0;
    pSprite->field_38 = eSprite_Anim_Die1;

loc_1D411:;
    Data4 = pSprite->field_4;
    Data0 = mMapLoaded->getHeightPixels();

    if (Data4 < Data0)
        goto loc_1D441;

    Data0 -= 1;
    pSprite->field_4 = Data0;

loc_1D441:;
    pSprite->field_6E = 0;
    return;

loc_1D44C:;
    pSprite->field_8 = 0x7C;
    pSprite->field_A = 0;
    pSprite->field_0 = Data28->field_0;
    pSprite->field_4 = Data28->field_4;
}

void cFodder::Sprite_Handle_Hostage_Rescue_Tent(sSprite* pSprite) {
    pSprite->field_8 = 0xDD;

    mHostage_Rescue_Tent = pSprite;
    if (pSprite->field_38 == eSprite_Anim_Die3)
        pSprite->field_18 = eSprite_Explosion2;
}

void cFodder::Sprite_Handle_Door_Civilian(sSprite* pSprite) {
    mSpawnSpriteType = 0x3D;

    sub_264B0(pSprite);
}

void cFodder::Sprite_Handle_Door2_Civilian(sSprite* pSprite) {
    mSpawnSpriteType = 0x3E;

    sub_264B0(pSprite);
}

void cFodder::Sprite_Handle_Door_Civilian_Spear(sSprite* pSprite) {
    mSpawnSpriteType = 0x46;

    sub_264B0(pSprite);
}

void cFodder::Sprite_Handle_Cannon(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data1C;
    ++pSprite->field_64;

    if (!pSprite->field_12)
        goto loc_1D65C;

    pSprite->field_12 -= 1;
    pSprite->field_3A++;

    if (pSprite->field_44)
        goto loc_1D5B3;

    Sprite_XY_Store(pSprite);
    Data1C = pSprite->field_10;

    Sprite_Movement_Calculate(pSprite);

    if (mSprite_Bullet_Destroy)
        goto loc_1D6CA;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);

    Data4 = pSprite->field_36;
    Data4 >>= 3;

    if (Data0 > Data4)
        goto loc_1D5CB;

    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_0 = pSprite->field_2E;
    pSprite->field_4 = pSprite->field_30;
    pSprite->field_44 = -1;
    goto loc_1D5CB;
loc_1D5B3:;
    pSprite->field_2C = eSprite_Draw_First;
    Sprite_Movement_Calculate(pSprite);

    if (mSprite_Bullet_Destroy)
        goto loc_1D6CA;

loc_1D5CB:;
    if (pSprite->field_64 > 3) {
        Data0 = -9;
        Data4 = 0;
        if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
            goto loc_1D65C;
    }

    if (!Sprite_Projectile_Collision_Check(pSprite))
        return;

    if (mSprites_Found_Count == 1)
        goto loc_1D65C;

    pSprite->field_43 = 1;
    pSprite->field_2C = eSprite_Draw_OnTop;
    pSprite->field_4 -= 5;
    if (pSprite->field_4 < 0)
        goto loc_1D6DD;
    pSprite->field_8 = 0x96;
    pSprite->field_A = 0;
    return;

loc_1D633:;
    pSprite->field_8 = 0x96;
    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if (pSprite->field_A >= 4)
        goto loc_1D6CA;

loc_1D65C:;

    if (pSprite->field_43)
        goto loc_1D69F;

    Sprite_Projectile_Collision_Check(pSprite);
    pSprite->field_43 = -1;
    pSprite->field_2C = eSprite_Draw_First;
    pSprite->field_4 -= 5;
    if (pSprite->field_4 < 0)
        goto loc_1D6DD;

    pSprite->field_A = 3;

loc_1D69F:;
    if (pSprite->field_43 >= 0)
        goto loc_1D633;

    pSprite->field_12 = 0;
    pSprite->field_8 = 0x7F;
    pSprite->field_A += 1;

    if (pSprite->field_8 >= 8)
        goto loc_1D6CA;

    return;

loc_1D6CA:;

    mSprite_Projectile_Counters[2] -= 1;
    Sprite_Destroy(pSprite);
    return;

loc_1D6DD:;
    pSprite->field_0 = 0;
    pSprite->field_4 = 0x1000;
    goto loc_1D6CA;

}

void cFodder::Sprite_Handle_VehicleNoGun_Enemy(sSprite* pSprite) {
    pSprite->field_6F = eVehicle_Jeep;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_VehicleGun_Enemy(sSprite* pSprite) {
    pSprite->field_6F = eVehicle_JeepRocket;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Vehicle_Unk_Enemy(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_DontTargetPlayer;
    
    pSprite->field_26 = pSprite->field_0;
    pSprite->field_26 += 0x28;

    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Civilian_Invisible(sSprite* pSprite) {

    if (!pSprite->field_38)
        return;

    Sprite_Handle_Civilian_Death(pSprite);

    /* Unused code block

     seg004:4A78 C4 36 20 00                       les     si, ds:20h
    seg004:4A7C 26 C7 44 22 02 00                 mov     word ptr es:[si+22h], 2
    seg004:4A82 C4 36 20 00                       les     si, ds:20h
    seg004:4A86 26 C7 44 08 D0 00                 mov     word ptr es:[si+8], 0D0h ; 'ð'
    */
}

void cFodder::Sprite_Handle_Turret_Missile_Enemy(sSprite* pSprite) {
    pSprite->field_22 = eSprite_PersonType_AI;
    pSprite->field_6F = eVehicle_Turret_Cannon;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile2_Enemy(sSprite* pSprite) {
    pSprite->field_22 = eSprite_PersonType_AI;
    pSprite->field_6F = eVehicle_Turret_Missile;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_HomingMissile_Enemy(sSprite* pSprite) {
    pSprite->field_22 = eSprite_PersonType_AI;
    pSprite->field_6F = eVehicle_Turret_Homing;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile_Human(sSprite* pSprite) {
    pSprite->field_22 = eSprite_PersonType_Human;
    pSprite->field_6F = eVehicle_Turret_Cannon;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile2_Human(sSprite* pSprite) {
    pSprite->field_22 = eSprite_PersonType_Human;
    pSprite->field_6F = eVehicle_Turret_Missile;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Vehicle_Sinking_1(sSprite* pSprite) {
    Sound_Play(pSprite, eSound_Effect_Vehicle_Sinking, 0x0F);
    pSprite->field_A -= 1;

    if (pSprite->field_A < 0)
        Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Vehicle_Sinking_2(sSprite* pSprite) {
    pSprite->field_A += 1;

    if (pSprite->field_A == 6)
        Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_BuildingDoor3(sSprite* pSprite) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (sub_1D92E(pSprite))
        return;

    if (mTroops_Enemy_Count >= mParams->mSpawnEnemyMax) {
        pSprite->field_8 = 0xE0;
        return;
    }

    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_1D8DC;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14 || pSprite->field_43 == 0x0A)
        goto loc_1D928;

    if (pSprite->field_43 >= 0) {
        pSprite->field_8 = 0x7C;
        return;
    }

    pSprite->field_8 = 0xE0;
    Sound_Play(pSprite, eSound_Effect_BuildingDoor2, 1);

    Data0 = tool_RandomGet() & 0x0F;
    Data4 = 0x14 - mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;

    if (Data4 < 0)
        Data0 = 0;

    Data4 <<= 3;
    Data4 += Data0;
    pSprite->field_12 = Data4;

loc_1D8DC:;

    pSprite->field_12 -= 1;
    if (pSprite->field_12 >= 0)
        return;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 += 6;
    pSprite->field_43 = static_cast<int8>(Data0);
    pSprite->field_8 = 0x7C;

    if (!Sprite_Create_Enemy(pSprite, Data2C))
        return;

    pSprite->field_12 = -1;
    pSprite->field_43 = -1;
    return;

loc_1D928:;
    Sprite_Create_Enemy(pSprite, Data2C);
}

void cFodder::Sprite_Handle_Explosion2(sSprite* pSprite) {

    Sprite_Handle_Explosion(pSprite);
}

void cFodder::Sprite_Handle_Door_Civilian_Rescue(sSprite* pSprite) {
    mSprite_OpenCloseDoor_Ptr = pSprite;

    if (sub_222A3(pSprite)) {
        mSprite_OpenCloseDoor_Ptr = 0;
        mPhase_Aborted = true;
        return;
    }
    pSprite->field_2C = eSprite_Draw_First;
    if (!pSprite->field_2A) {
        pSprite->field_8 = 0x9B;
        return;
    }

    pSprite->field_2A -= 1;
    if (!pSprite->field_2A)
        Sound_Play(pSprite, eSound_Effect_DoorCloseDoor, 1);

    if (pSprite->field_8 == 0x9B)
        Sound_Play(pSprite, eSound_Effect_DoorCloseDoor, 1);

    pSprite->field_8 = 0x7C;
}

void cFodder::Sprite_Handle_Seal_Mine(sSprite* pSprite) {
    Sprite_Handle_Seal(pSprite);

    if (pSprite->field_8 == 0xE1)
        return;

    if (pSprite->field_38)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    sSprite* Data24 = mSquad_Leader;
    int16 Data0 = Data24->field_0;
    int16 Data4 = Data24->field_4;
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x14)
        return;

    pSprite->field_38 = eSprite_Anim_Die4;
}

void cFodder::Sprite_Handle_Spider_Mine(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data14 = 0;

    if (pSprite->field_38)
        goto loc_1DC50;

    pSprite->field_8 = 0xE2;
    if (!pSprite->field_2E) {
        pSprite->field_2E = -1;
        Data0 = tool_RandomGet() & 0x1E;
        pSprite->field_2A = Data0;
    }

    Data0 = pSprite->field_2A;
    Data0 += 2;
    Data0 &= 0x1E;
    pSprite->field_2A = Data0;

    pSprite->field_A = mSprite_SpiderMine_Frames[Data0 / 2];
    Data14 = mSquad_Leader;

    if (Data14 == INVALID_SPRITE_PTR)
        return;

    Data0 = Data14->field_0;
    Data4 = Data14->field_4;
    Data8 = pSprite->field_0;
    Data8 += 4;
    DataC = pSprite->field_4;
    DataC -= 2;
    Data10 = 0x20;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 0x0A)
        return;

loc_1DC50:;
    pSprite->field_18 = eSprite_Explosion;
    pSprite->field_26 = -32763;
    pSprite->field_28 = -3;
}

void cFodder::Sprite_Handle_Bonus_RankToGeneral(sSprite* pSprite) {
    int16 Data0;

    // Blow up?
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0x95;
    pSprite->field_A = 0x0F;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mSquad_Leader->field_46_mission_troop->mRank = 0x0F;
    mGUI_Sidebar_Setup = 0;

    Sprite_Destroy_Wrapper(pSprite);
}


void cFodder::Sprite_Handle_Bonus_Rockets(sSprite* pSprite) {
    int16 Data0;

    // Blow up?
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0xE4;
    pSprite->field_A = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    // Has Homing missiles
    mSquad_Leader->field_75 |= eSprite_Flag_HomingMissiles;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 50;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Bonus_Armour(sSprite* pSprite) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0xE3;
    pSprite->field_A = 0;
    int16 Data0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    mSquad_Leader->field_75 |= eSprite_Flag_Invincibility;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Bonus_RankHomingInvin_SquadLeader(sSprite* pSprite) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xE5;
    pSprite->field_A = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    // Invincible + HomingMissiles
    mSquad_Leader->field_75 |= (eSprite_Flag_HomingMissiles | eSprite_Flag_Invincibility);
    mSquad_Leader->field_46_mission_troop->mRank = 0x0F;
    mGUI_Sidebar_Setup = 0;

    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 50;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_MissileHoming2(sSprite* pSprite) {

    Sprite_Handle_MissileHoming(pSprite);
}

void cFodder::Sprite_Handle_Bonus_RankHomingInvin_Squad(sSprite* pSprite) {

    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xE6;
    pSprite->field_A = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader(pSprite, Data0))
        return;

    if (mSquad_Selected < 0)
        return;

    sSprite** Data28 = mSquads[mSquad_Selected];
    for (; *Data28 != INVALID_SPRITE_PTR;) {

        sSprite* Data2C = *Data28++;
        Data2C->field_75 |= (eSprite_Flag_HomingMissiles | eSprite_Flag_Invincibility);

        Data2C->field_46_mission_troop->mRank = 0x0F;
    }

    mGUI_Sidebar_Setup = 0;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 0x32;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_CallPad(sSprite* pSprite) {
    sSprite* Data2C = 0;

    pSprite->field_8 = 0xE7;
    pSprite->field_2C = eSprite_Draw_First;

    // Find a troop inrange 
    if (Sprite_Handle_Helicopter_Callpad_InRange(pSprite, Data2C)) {
        pSprite->field_75 = 0;
    }
    else {
        ++mHelicopterCallPadPressedCount;

        if (!pSprite->field_75) {
            mTroop_InRange_Callpad = Data2C;
            pSprite->field_75 = -1;
        }
    }

    if (mHelicopterCall_X < 0) {
        pSprite->field_A = 0;
        return;
    }

    int16 Data0 = pSprite->field_32;
    Data0 += 2;
    Data0 &= 6;

    pSprite->field_A = mSprite_Helicopter_CallPad_Frames[Data0 / 2];
    pSprite->field_32 = Data0;
}

void cFodder::Sprite_Handle_BuildingDoor_Reinforced(sSprite* pSprite) {
    if (pSprite->field_38 != eSprite_Anim_Die3) {
        Sprite_Handle_BuildingDoor3(pSprite);
        return;
    }

    if (pSprite->field_74 >= 0x69) {
        Sprite_Handle_BuildingDoor3(pSprite);
        return;
    }

    pSprite->field_74 += 1;
    pSprite->field_38 = eSprite_Anim_None;
    Sprite_Handle_BuildingDoor3(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Grenade2_Human_Called(sSprite* pSprite) {
    Sprite_Handle_Helicopter_Human_CallCheck(pSprite);
    Sprite_Handle_Helicopter_Grenade2_Human(pSprite);

}

void cFodder::Sprite_Handle_Helicopter_Grenade_Human_Called(sSprite* pSprite) {
    Sprite_Handle_Helicopter_Human_CallCheck(pSprite);
    Sprite_Handle_Helicopter_Grenade_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Missile_Human_Called(sSprite* pSprite) {
    Sprite_Handle_Helicopter_Human_CallCheck(pSprite);
    Sprite_Handle_Helicopter_Missile_Human(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Homing_Human_Called(sSprite* pSprite) {
    Sprite_Handle_Helicopter_Human_CallCheck(pSprite);
    Sprite_Handle_Helicopter_Homing_Human(pSprite);
}

void cFodder::Sprite_Handle_Enemy_Leader(sSprite* pSprite) {

    Sprite_Handle_Hostage(pSprite);
}

void cFodder::Sprite_Handle_Helicopter_Homing_Enemy2(sSprite* pSprite) {

    Sprite_Handle_Helicopter_Homing_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Computer_1(sSprite* pSprite) {

    Sprite_Handle_Computer(pSprite, 0x46);
}

void cFodder::Sprite_Handle_Computer_2(sSprite* pSprite) {

    Sprite_Handle_Computer(pSprite, 0x69);
}

void cFodder::Sprite_Handle_Computer_3(sSprite* pSprite) {

    Sprite_Handle_Computer(pSprite, 0xAF);
}

void cFodder::Sprite_Handle_UFO_Callpad(sSprite* pSprite) {
    
    if (mHelicopterCall_X > 0) {

        mSwitchesActivated = true;
        pSprite->field_8 = 0xE8;
        pSprite->field_A = 0;

        sSprite* Data24 = pSprite + 1;
        Data24->field_8 = 0x8D;
        Data24->field_A = 0x01;
        if (pSprite->field_20 >= 0x14)
            pSprite->field_1E_Big -= 0xC000;
        
        int16 Data0 = mHelicopterCall_X;
        int16 Data4 = mHelicopterCall_Y - 0x28;

        Sprite_Direction_Between_Points(pSprite, Data0, Data4);

        Data0 = mHelicopterCall_X;
        Data4 = mHelicopterCall_Y - 0x28;
        int16 Data8 = pSprite->field_0;
        int16 DataC = pSprite->field_4;
        int16 Data10 = 0x60;

        Data0 = Map_Get_Distance_BetweenPoints(Data0, Data4, Data8, Data10, DataC);
        Data0 >>= 3;
        Data0 += 1;
        if (Data0 < 8)
            Data0 = 8;

        pSprite->field_36 = Data0;
        Sprite_Movement_Calculate(pSprite);
        Sprite_PositionNext_AdjustByHeight(pSprite);
    } else {
        // loc_2DBE8
        pSprite->field_8 = 0x7C;
        pSprite->field_A = 0;

        sSprite* Data24 = pSprite + 1;
        Data24->field_8 = 0x7C;
        Data24->field_A = 0;

        if (mSquad_Leader && mSquad_Leader != INVALID_SPRITE_PTR) {
 
            pSprite->field_0 = mSquad_Leader->field_0 + 0x190;
            pSprite->field_4 = mSquad_Leader->field_4;
        }

        pSprite->field_20 = 0x64;
        Sprite_PositionNext_AdjustByHeight(pSprite);
    }
}

void cFodder::Sprite_Handle_Turret_Cannon_Invulnerable(sSprite* pSprite) {
    
    if (pSprite->field_38 == eSprite_Anim_Die1)
        pSprite->field_38 = eSprite_Anim_None;

    pSprite->field_22 = eSprite_PersonType_AI;
    pSprite->field_6F = eVehicle_Turret_Cannon;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Turret_Missile_Invulnerable(sSprite* pSprite) {
    
    if (pSprite->field_38 == eSprite_Anim_Die1)
        pSprite->field_38 = eSprite_Anim_None;

    pSprite->field_22 = eSprite_PersonType_AI;
    pSprite->field_6F = eVehicle_Turret_Missile;

    Sprite_Handle_Turret(pSprite);
}

void cFodder::Sprite_Handle_Looping_Vehicle_Left(sSprite* pSprite) {
    
    pSprite->field_6F = eVehicle_DontTargetPlayer;

    if (pSprite->field_0 <= 6) {
        pSprite->field_0 = mMapLoaded->getWidthPixels() - 4;
        pSprite->field_75 = 0;
    }

    pSprite->field_26 = pSprite->field_0 - 0x28;
    Sprite_Handle_Vehicle_Enemy(pSprite);

    if (tool_RandomGet() & 0x0F)
        return;

    int16 Saved_F0 = pSprite->field_0;
    int16 Saved_F4 = pSprite->field_4;

    int16 Data0 = (tool_RandomGet() & 0x07) + 0x0A;
    pSprite->field_0 += Data0;
    pSprite->field_4 += -8;
    if (pSprite->field_4 < 0)
        pSprite->field_4 = 0;

    if (!Sprite_Create_Cannon(pSprite)) {
        Sound_Play(pSprite, 0x10, 0x1E);
    }

    pSprite->field_0 = Saved_F0;
    pSprite->field_4 = Saved_F4;
}

void cFodder::Sprite_Handle_Looping_Vehicle_Right(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_DontTargetPlayer;

    if (pSprite->field_0 >= mMapLoaded->getWidthPixels()) {
        pSprite->field_0 = 0;
        pSprite->field_75 = 0;
    }

    pSprite->field_26 = pSprite->field_0 + 0x28;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Looping_Vehicle_Up(sSprite* pSprite) {
    
    pSprite->field_6F = eVehicle_DontTargetPlayer;

    if (pSprite->field_4 <= 6) {
        pSprite->field_4 = mMapLoaded->getHeightPixels() - 4;
        pSprite->field_75 = 0;
    }

    pSprite->field_28 = pSprite->field_4 - 0x28;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_Handle_Looping_Vehicle_Down(sSprite* pSprite) {

    pSprite->field_6F = eVehicle_DontTargetPlayer;

    if (pSprite->field_4 >= mMapLoaded->getHeightPixels()) {
        pSprite->field_4 = 0;
        pSprite->field_75 = 0;
    }

    pSprite->field_28 = pSprite->field_4 + 0x28;
    Sprite_Handle_Vehicle_Enemy(pSprite);
}

void cFodder::Sprite_PositionNext_AdjustByHeight(sSprite* pSprite) {
    sSprite* Data24 = pSprite + 1;

    Data24->field_0 = pSprite->field_0 + 8;
    Data24->field_4 = pSprite->field_4;

    Data24->field_0 += (pSprite->field_20 >> 2);
    Data24->field_4 += (pSprite->field_20 >> 2);
    Data24->field_2C = eSprite_Draw_First;
}

int16 cFodder::sub_1D92E(sSprite* pSprite) {
    int16 Data0, Data4;

    if (pSprite->field_38 == eSprite_Anim_Die3)
        goto loc_1D95B;

    if (pSprite->field_38 == eSprite_Anim_Die2)
        goto loc_1D9C9;

    if (pSprite->field_58)
        goto loc_1D9C9;

    return 0;

loc_1D95B:;
    pSprite->field_38 = eSprite_Anim_Die2;
    pSprite->field_8 = 0xE0;

    Data0 = tool_RandomGet();
    Data4 = Data0;
    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pSprite->field_10 = Data0;

    Data4 &= 3;
    Data4 += 5;

    pSprite->field_44 = (int8)Data4;
    pSprite->field_36 = 0x78;
    pSprite->field_58 = -1;

loc_1D9C9:;
    pSprite->field_44 -= 1;
    if (!pSprite->field_44)
        goto loc_1DA13;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_1DA13;

    Data0 = -3;
    Data4 = 2;
    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_1DA13;

    if (pSprite->field_36 <= 0)
        goto loc_1DA13;

    return -1;

loc_1DA13:;
    pSprite->field_0 -= 8;
    if (pSprite->field_0 < 0)
        pSprite->field_0 = 0;

    pSprite->field_4 -= 8;
    Sprite_Turn_Into_Building_Explosion(pSprite);
    return -1;
}

void cFodder::Sprite_Native_Sound_Play(sSprite* pSprite, int16 pSoundID) {

    if (mPhase_Completed_Timer)
        return;

    if ((tool_RandomGet() & 0x7F) != 0x0E)
        return;

    int16 Data8 = 0;

    if (mMapLoaded->getTileType() == eTileTypes_Int || mMapLoaded->getTileType() == eTileTypes_Moors)
        Data8 = 0x7F;

    Sound_Play(pSprite, pSoundID, Data8);
}

int16 cFodder::Sprite_Handle_Soldier_Animation(sSprite* pSprite) {
    int32 Dataa4;
    int16 Data0, Data4, Data8;
    const int16* Data28;
    sSprite* Data20 = 0, *Data2C = 0, *Data30 = 0;
    int16 Field_52;
    int16 Field_0;
    int16 Field_4;

    if (pSprite->field_38 == eSprite_Anim_Hit3)
        goto loc_1E831;

    if (pSprite->field_38 == eSprite_Anim_Die2)
        goto loc_1EA48;

    if (pSprite->field_38 == eSprite_Anim_Hit2)
        goto loc_1EB87;

    Data0 = pSprite->field_4;
    if (Data0 >= mMapLoaded->getHeightPixels())
        pSprite->field_4 = mMapLoaded->getHeightPixels();

    //loc_1E0A4
    if (pSprite->field_56)
        goto loc_1E831;

    if (pSprite->field_5B == 1)
        goto loc_1E74C;

    if (pSprite->field_38 == eSprite_Anim_Die5)
        goto loc_1E2F4;

    if (pSprite->field_38 == eSprite_Anim_Slide1)
        goto loc_1ECA6;

    if (pSprite->field_38 == eSprite_Anim_Slide2)
        goto loc_1ED5B;

    if (pSprite->field_38 == eSprite_Anim_Slide3)
        goto loc_1EE59;

    if (pSprite->field_38 == eSprite_Anim_Vehicle_Enter)
        goto Sprite_EnteringVehicle;

    if (pSprite->field_38 == eSprite_Anim_Vehicle_Inside)
        goto Sprite_EnteredVehicle;

    //seg004:53BD
    // Is Sprite Dying
    if (!pSprite->field_38)
        goto loc_1EA3F;

    if (!pSprite->field_64)
        goto loc_1E3D2;

    pSprite->field_64 = 0;
    if (mPhase_TryAgain)
        pSprite->field_40 = -26506;

    if (!pSprite->field_58) {
        Sprite_Create_Player_Shadow(pSprite);
        pSprite->field_58 = -1;
    }

    //loc_1E15D
    Data8 = pSprite->field_10;
    Data8 >>= 5;
    Data8 -= 1;
    Data8 ^= 0x0F;
    Data8 &= 0x0E;
    Data0 = pSprite->field_22;

    Data28 = mSprite_AnimationPtrs[Data0];
    pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

    //loc_1E1E9
    pSprite->field_A = 0;
    pSprite->field_2A = 0;

    if (pSprite->field_1A < 0xB0000) {
        int32 Dataa0 = tool_RandomGet() & 0x07;
        Dataa0 += 2;
        Dataa0 = (Dataa0 << 16) | (Dataa0 >> 16);
        Dataa0 += pSprite->field_1A;

        pSprite->field_1A = Dataa0;
    }
    //loc_1E232
    pSprite->field_12 = 2;
    Data0 = tool_RandomGet();
    Data4 = 0;
    Data0 &= 7;
    Data4 = mSprite_Civilian_Sound_Death[Data0];
    //seg004:5508
    Sound_Play(pSprite, Data4, 0x14);

    if (pSprite->field_36 < 0x1E)
        pSprite->field_36 += 0x0F;

    pSprite->field_3E = pSprite->field_10;
    pSprite->field_3A = -1;
    Data0 = tool_RandomGet() & 0x01;
    if (!Data0)
        pSprite->field_3A = pSprite->field_10;

    Sprite_Create_BloodTrail(pSprite, Data2C, Data30);
    pSprite->field_26 = -1;
    if (pSprite->field_38 != eSprite_Anim_Die4)
        goto loc_1E3D2;

    // Die on spike
    pSprite->field_36 = 0;
    pSprite->field_38 = eSprite_Anim_Die5;
loc_1E2F4:;
    pSprite->field_3A -= 1;
    if (pSprite->field_3A < 0) {
        pSprite->field_38 = eSprite_Anim_Hit;
        return -1;
    }
    //loc_1E30C
    Data0 = 0x21;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        Data0 = 0x63;

    pSprite->field_8 = Data0;
    Data0 = pSprite->field_2A & 7;
    //seg004:55F3 

    if (!Data0) {
        sSprite* Data2C = 0;
        Data30 = 0;

        Sprite_Create_BloodTrail(pSprite, Data2C, Data30);

        if (pSprite->field_A)
            pSprite->field_A = 0;
        else
            pSprite->field_A = 1;
    }
    //loc_1E36C

    pSprite->field_2A -= 1;
    if (pSprite->field_2A >= 0)
        return -1;

    Data0 = tool_RandomGet() & 7;
    Data0 += 8;
    pSprite->field_2A = Data0;
    Data0 = tool_RandomGet() & 7;

    Data4 = mSprite_Civilian_Sound_Death[Data0];
    Data8 = 0;
    Sound_Play(pSprite, Data4, Data8);
    return -1;

loc_1E3D2:;

    if (pSprite->field_40 == -26506) {
        sSprite* Data2C = 0;
        Data30 = 0;

        pSprite->field_28 += 1;
        Sprite_Create_BloodTrail(pSprite, Data2C, Data30);

        if (!(pSprite->field_28 & 7)) {
            Data0 = tool_RandomGet() & 7;
            Data4 = mSprite_Civilian_Sound_Death[Data0];
            Data8 = 0x14;
            Sound_Play(pSprite, Data4, Data8);
        }
    }
    //loc_1E437

    if (pSprite->field_3A >= 0) {
        pSprite->field_3E = pSprite->field_3A;

        Data0 = pSprite->field_36 << 2;
        pSprite->field_3A += Data0;
        //seg004:5724
        pSprite->field_3A &= 0x1FE;
        Data8 = pSprite->field_3A;

        Data8 >>= 5;
        Data8 -= 1;
        Data8 ^= 0x0F;
        Data8 &= 0x0E;

        Data28 = mSprite_AnimationPtrs[pSprite->field_22];
        pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

        if (pSprite->field_59)
            pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

        // FIX: Added as only 2 frames exist for all sprites in mSprite_AnimationPtrs
        pSprite->field_A %= 2;
    }
    //loc_1E50A
    Sprite_XY_Store(pSprite);
    mTmp_FrameNumber = pSprite->field_A;
    Sprite_Movement_Calculate(pSprite);

    if (pSprite->field_20 < 0x0C) {

        int16 Field10 = pSprite->field_10;
        Sprite_Terrain_Check(pSprite, Data4);
        pSprite->field_10 = Field10;
    }
    else {
        //loc_1E542
        int16 Field0 = pSprite->field_0;
        int16 Field4 = pSprite->field_4;
        int16 Field10 = pSprite->field_10;

        Sprite_Terrain_Check(pSprite, Data4);

        pSprite->field_10 = Field10;
        pSprite->field_4 = Field4;
        pSprite->field_0 = Field0;
    }

    //loc_1E579
    if (!pSprite->field_20) {
        if (pSprite->field_4F || pSprite->field_50) {
            pSprite->field_52 = 6;
        }
    }
    //loc_1E5A7
    Dataa4 = (int64)pSprite->field_1A;

    pSprite->field_1E_Big += pSprite->field_1A;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;

        Dataa4 = -Dataa4;
        Dataa4 >>= 2;

        if (pSprite->field_52) {
            pSprite->field_36 = 0;
            Dataa4 = 0;
        }
    }
    //loc_1E619
    Dataa4 -= 0x18000;
    pSprite->field_1A = Dataa4;
    if (pSprite->field_36) {
        pSprite->field_36 -= 2;

        if (pSprite->field_36 < 0)
            pSprite->field_36 = 0;
    }
    //loc_1E653
    if (pSprite->field_26) {

        if (!pSprite->field_20) {
            sSprite* Data2C = 0;
            Data30 = 0;
            Sprite_Create_BloodTrail(pSprite, Data2C, Data30);
            pSprite->field_26 = 0;
        }
    }
    //loc_1E67A
    pSprite->field_12 -= 1;
    if (!pSprite->field_12) {

        pSprite->field_12 = 6;
        if (pSprite->field_A < 1)
            pSprite->field_A += 1;
    }
    //loc_1E6A3
    if (pSprite->field_2A <= 0x14) {

        pSprite->field_2A++;
        return -1;      // DOS version jumped to loc_1EB5D; here, which was different from Amiga and caused a bug
    }
    //loc_1E6BA
    if (pSprite->field_20)
        return -1;     // DOS version jumped to loc_1EB5D; here, which was different from Amiga and caused a bug

    //seg004:5978
    if (!pSprite->field_52) {
        if (!pSprite->field_50) {

            Data0 = tool_RandomGet() & 0x7F;
            if (!Data0) {

                pSprite->field_3A = 0x1F4;
                pSprite->field_38 = eSprite_Anim_Die5;
                mGUI_Sidebar_Setup = 0;
                return -1;
            }
        }
    }
    //loc_1E708
    pSprite->field_38 = eSprite_Anim_Die2;
    pSprite->field_10 = pSprite->field_3E;
    pSprite->field_12 = 0;
    pSprite->field_45 = 1;

    return -1;

loc_1E74C:;
    if (pSprite->field_52 < 5) {
        pSprite->field_52 = 5;
        Data4 = 0x0E;
        Data8 = 0x0A;

        Sound_Play(pSprite, Data4, Data8);
        pSprite->field_8 = 0x38;
        if (pSprite->field_22 != eSprite_PersonType_Human)
            pSprite->field_8 = 0x7A;

        pSprite->field_A = 0;
        pSprite->field_12 = 0x0A;

        return 1;
    }
    //loc_1E7AC
    pSprite->field_12 -= 1;
    if (!pSprite->field_12) {

        pSprite->field_52 += 1;
        if (pSprite->field_52 >= 0x0D) {

            pSprite->field_38 = eSprite_Anim_Die2;
            pSprite->field_10 = pSprite->field_3E;
            pSprite->field_12 = 0;
            pSprite->field_45 = 1;

            goto loc_1E825;
        }
        else {
            pSprite->field_12 = 0x0A;
        }
    }
    //loc_1E806
    if (!(pSprite->field_12 & 2) && pSprite->field_A < 5)
        pSprite->field_A += 1;
loc_1E825:;
    Data0 = -1;
    return -1;

loc_1E831:;
    if (pSprite->field_56 == 1) {
        pSprite->field_12 = 1;
        pSprite->field_28 += 2;
    }
    Data0 = pSprite->field_12;
    pSprite->field_12 += 1;

    if (pSprite->field_38 == eSprite_Anim_Hit3) {
        pSprite->field_52 += Data0;

        if (pSprite->field_52 < 0x0E)
            return -1;

        pSprite->field_52 = 0x0E;
        pSprite->field_12 = 0x14;
        Data0 = tool_RandomGet() & 0x07;
        Data4 = mSprite_Civilian_Sound_Death[Data0];
        Data8 = 0x0A;
        Sound_Play(pSprite, Data4, Data8);
        goto loc_1E9EC;
    }
    //loc_1E8D6
    pSprite->field_4 += Data0;
    Data0 = pSprite->field_4;
    if (Data0 >= mMapLoaded->getHeightPixels())
        pSprite->field_38 = eSprite_Anim_Hit2;

    mStoredSpriteY = pSprite->field_4;
    Data0 = 8;
    Data0 += pSprite->field_26;
    Data4 = -3;
    Data4 += pSprite->field_28;

    Map_Terrain_Get_Type_And_Walkable(Data0, Data4);

    if (Data4 == eTerrainFeature_Drop || Data4 == eTerrainFeature_Drop2) {
        Data0 = pSprite->field_12;
        pSprite->field_28 += Data0;
        Data8 = pSprite->field_28;

        sMapTarget* WalkTarget = mSquad_WalkTargets[pSprite->field_32];
        //seg004:5C49
        Data0 = pSprite->field_40;
        if (WalkTarget[Data0].mX >= 0)
            WalkTarget[Data0].mY = Data8;
    }
    //loc_1E9CD;
    Sprite_Terrain_Check(pSprite, Data4);
    if (pSprite->field_12 > 5)
        sub_223B2(pSprite);

    if (Data4 == 9)
        return -1;

loc_1E9EC:; // Troop Falling?
    if (pSprite->field_12 < 0x0C) {
        pSprite->field_3E = 0;
        pSprite->field_56 = 0;

        return 0;
    }

    pSprite->field_38 = eSprite_Anim_Die2;
    pSprite->field_10 = pSprite->field_3E;
    pSprite->field_12 = 0;
    pSprite->field_45 = 1;
    return -1;

loc_1EA3F:;
    Data0 = 0;
    return 0;

loc_1EA48:;
    pSprite->field_12 += 1;
    if (pSprite->field_12 >= 0x0F)
        return Sprite_Troop_Dies(pSprite);

    if (pSprite->field_12 >= 0x07 && pSprite->field_8 != 0x7C) {
        pSprite->field_8 = 0x7C;
        goto loc_1EB0E;
    }

loc_1EA82:;
    Data8 = pSprite->field_10;
    Data8 >>= 5;
    Data8 -= 1;
    Data8 ^= 0x0F;
    Data8 &= 0x0E;

    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

    // FIX: Added as only 2 frames exist for all sprites in mSprite_AnimationPtrs
    pSprite->field_A %= 2;
loc_1EB0E:;
    Field_52 = pSprite->field_52;
    Field_0 = pSprite->field_0;
    Field_4 = pSprite->field_4;
    Sprite_Terrain_Check(pSprite, Data4);
    pSprite->field_4 = Field_4;
    pSprite->field_0 = Field_0;
    pSprite->field_52 = Field_52;

    if (pSprite->field_4F || pSprite->field_50) {

        if (pSprite->field_52 >= 0x0D)
            pSprite->field_52 = 0x0B;

        pSprite->field_52 += 2;
    }

    return -1;

loc_1EB87:;
    pSprite->field_12 += 1;
    if (pSprite->field_12 >= 0x0F)
        return Sprite_Troop_Dies(pSprite);

    if (pSprite->field_12 < 0x07)
        goto loc_1EA82;

    if (pSprite->field_8 != 0x7C) {
        pSprite->field_8 = 0x7C;
        goto loc_1EB0E;
    }

    Data8 = pSprite->field_10;
    Data8 >>= 5;
    Data8 -= 1;
    Data8 ^= 0x0F;
    Data8 &= 0x0E;

    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

    pSprite->field_A = 0;
    //loc_1EC4F
    Field_52 = pSprite->field_52;
    Sprite_Terrain_Check(pSprite, Data4);
    pSprite->field_52 = Field_52;

    if (pSprite->field_4F || pSprite->field_50) {
        if (pSprite->field_52 >= 0x0D)
            pSprite->field_52 = 0x0B;
        pSprite->field_52 += 2;
    }

    //loc_1EC9A
    return -1;

loc_1ECA6:;
    if (pSprite->field_60 <= eTerrainFeature_Water && pSprite->field_60 >= eTerrainFeature_QuickSand) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    pSprite->field_38 = eSprite_Anim_Slide2;
    pSprite->field_36 = 0x24;

    Data0 = tool_RandomGet() & 6;
    Data0 = mSprite_Soldier_Unk[Data0];

    Data0 += pSprite->field_10;
    Data0 &= 0x1FE;
    pSprite->field_10 = Data0;
    pSprite->field_8 = 0xA4;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        pSprite->field_8 = 0xA7;

    Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction(pSprite);
    Data8 >>= 1;
    pSprite->field_A = Data8;
    return -1;

loc_1ED5B:;
    Sprite_XY_Store(pSprite);
    mTmp_FrameNumber = pSprite->field_A;

    Sprite_Movement_Calculate(pSprite);
    Sprite_Reached_MapEdge(pSprite);
    Sprite_Terrain_Check(pSprite, Data4);

    if (pSprite->field_38 != eSprite_Anim_Slide2)
        goto loc_1EE3E;

    if (pSprite->field_60 > eTerrainFeature_Water || pSprite->field_60 < eTerrainFeature_QuickSand) {
        pSprite->field_36 -= 5;
        if (pSprite->field_36) {
            if (pSprite->field_36 >= 0)
                return -1;

            pSprite->field_36 = 0;
        }
    }
    Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction(pSprite);

    //seg004:606C
    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    Data0 = Data28[(0x60 + Data8) / 2];

    if (Data0 != pSprite->field_8) {
        pSprite->field_8 = Data28[(0x60 + Data8) / 2];
        pSprite->field_A = 0;
        return -1;
    }
    //loc_1EE34
    pSprite->field_38 = eSprite_Anim_None;

loc_1EE3E:;
    pSprite->field_43 = 0;
    sub_305D5(Data20);
    return -1;

loc_1EE59:;
    if (pSprite->field_60 <= eTerrainFeature_Water && pSprite->field_60 >= eTerrainFeature_QuickSand) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    pSprite->field_38 = eSprite_Anim_Slide2;
    pSprite->field_36 = 0x2E;

    Data0 = tool_RandomGet() & 0x1F;

    pSprite->field_10 += Data0;
    pSprite->field_8 = 0xA4;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        pSprite->field_8 = 0xA7;

    Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction(pSprite);
    Data8 >>= 1;
    pSprite->field_A = Data8;
    return -1;

Sprite_EnteringVehicle:;

    // Not walking to a vehicle?
    if (!pSprite->field_66) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    pSprite->field_38 = eSprite_Anim_Vehicle_Inside;
    pSprite->field_8 = 0xCC;
    pSprite->field_A = 4;
    return -1;

Sprite_EnteredVehicle:;
    pSprite->field_A -= 1;
    if (pSprite->field_A < 0)
        return loc_1F043(pSprite);

    pSprite->field_36 = 0x30;
    Sprite_Draw_Row_Update(pSprite);
    return -1;

}

int16 cFodder::Sprite_Troop_Dies(sSprite* pSprite) {
    sMission_Troop* Troop = 0;

    // Is Player?
    if (pSprite->field_22 == eSprite_PersonType_Human) {
        //Yes, is player

        ++mGame_Data.mScore_Kills_Away;

        if (pSprite->field_46_mission_troop) {

            mGame_Data.Soldier_Died(pSprite->field_46_mission_troop);

            pSprite->field_46_mission_troop->mSprite = INVALID_SPRITE_PTR;
            pSprite->field_46_mission_troop->mRecruitID = -1;
            pSprite->field_46_mission_troop->mRank = 0;
        }

        if (mSquad_Selected < 0) {
            mGUI_Sidebar_Setup = 0;
            mSquad_Select_Timer = 0;
        }
        else {
            mGUI_Sidebar_Setup = 0;
        }

        //loc_1F03D
        return Sprite_Destroy_Wrapper(pSprite);
    }

    if (pSprite->field_18 == eSprite_Enemy_Rocket || pSprite->field_18 == eSprite_Enemy) {

        ++mGame_Data.mScore_Kills_Home;
        --mTroops_Enemy_Count;
        if (pSprite->field_5D) {
            pSprite->field_5D = 0;

            // Default to first human soldier
            Troop = mGame_Data.mSoldiers_Allocated;

            if (pSprite->field_5E_SoldierAllocated) {
                Troop = pSprite->field_5E_SoldierAllocated;
                goto loc_1F1E9;
            }
        }

        // Sprite didnt have a soldier who killed it attached
        if (mSquad_Selected != -1) {
            //loc_1F13E
            // use the first squad member
            if (mSquads[mSquad_Selected][0] != INVALID_SPRITE_PTR && mSquads[mSquad_Selected][0] != 0)
                Troop = mSquads[mSquad_Selected][0]->field_46_mission_troop;
        }
        loc_1F1E9:;

        if (Troop >= mGame_Data.mSoldiers_Allocated && Troop <= &mGame_Data.mSoldiers_Allocated[8]) {

            if (Troop->mNumberOfKills >= 999)
                Troop->mNumberOfKills = 998;

            ++Troop->mNumberOfKills;
        }
    }

    return Sprite_Destroy_Wrapper(pSprite);
}

int16 cFodder::loc_1F043(sSprite* pSprite) {
    if (!pSprite->field_66) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    loc_1F092(pSprite, (sSprite*)pSprite->field_66);
    pSprite->field_A = 0;
    pSprite->field_8 = 0x7C;
    pSprite->field_38 = eSprite_Anim_None;
    return 0;
}

void cFodder::loc_1F092(sSprite* pSprite, sSprite* pData24) {
    int16 Data0 = pData24->field_0;
    Data0 += 0x10;
    if (pData24->field_6F == eVehicle_Turret_Cannon)
        goto loc_1F0B9;

    if (pData24->field_6F == eVehicle_Turret_Missile)
        goto loc_1F0BE;
loc_1F0B9:;
    Data0 -= 0x0C;

loc_1F0BE:;

    int16 Data4 = pData24->field_4;
    Data4 -= 0x09;

    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;
}

int16 cFodder::Sprite_Handle_Player_MissionOver(sSprite* pSprite) {

    if (!mPhase_Aborted)
        if (!mPhase_Complete)
            return 0;

    int16 Data0 = -1;
    int16 Data4 = 8;

    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);

    if (Data4 == eTerrainFeature_Water) {
        Data0 = pSprite->field_20;
        Data0 -= 8;

        if (Data0 >= 0) {
            Data0 = 0;
        }
        else {
            Data0 = -Data0;
            if (Data0 > 8)
                Data0 = 8;
        }

        pSprite->field_52 = Data0;
    }
    //loc_1F283

    if (pSprite->field_8 == 0x41)
        goto loc_1F3C4;

    if (pSprite->field_8 == 0x9F)
        goto loc_1F340;

    if (pSprite->field_8 == 0x38)
        goto loc_1F3A2;

    pSprite->field_A = 0;
    pSprite->field_36 = 0;

    if (!mPhase_Aborted) {

        Data0 = tool_RandomGet() & 1;
        if (Data0 == 0)
            goto loc_1F32C;

        if (!pSprite->field_52) {
            Data0 = tool_RandomGet() & 1;

            if (!Data0) {
                //loc_1F2F9
                pSprite->field_8 = 0x41;
                Data0 = tool_RandomGet() & 3;
                pSprite->field_A = Data0;
                pSprite->field_2A = 1;
                Sprite_Create_Player_Shadow(pSprite);
                goto loc_1F3C4;
            }
        }
        else
            goto loc_1F32C;
    }

    //loc_1F2EC
    pSprite->field_8 = 0x38;
    goto loc_1F3A2;

loc_1F32C:;
    pSprite->field_8 = 0x9F;
    pSprite->field_2A = 0;

loc_1F340:;

    Data0 = pSprite->field_2A;

    pSprite->field_A = mSprite_Player_MissionOver_Frames[Data0];
    pSprite->field_36 ^= -32768;
    if (pSprite->field_36 < 0)
        goto loc_1F41D;

    pSprite->field_2A += 1;
    if (pSprite->field_2A < 7)
        goto loc_1F41D;

    pSprite->field_2A = 3;
    goto loc_1F41D;

loc_1F3A2:;
    pSprite->field_36 ^= -32768;
    if (pSprite->field_36 < 0)
        goto loc_1F3C2;

    if (pSprite->field_A >= 5)
        goto loc_1F41D;

    pSprite->field_A += 1;
loc_1F3C2:;
    goto loc_1F41D;

loc_1F3C4:;

    pSprite->field_A += pSprite->field_2A;
    if (!pSprite->field_A)
        goto loc_1F3E9;

    if (pSprite->field_A != 5)
        goto loc_1F3F1;

loc_1F3E9:;
    pSprite->field_2A = -pSprite->field_2A;

loc_1F3F1:;
    Data0 = pSprite->field_A;
    Data0 -= 1;
    if (Data0 < 0)
        Data0 = 0;

    Data0 <<= 3;
    pSprite->field_20 = Data0;

loc_1F41D:;
    Data0 = -1;

    return -1;
}

void cFodder::Sprite_Handle_Troop_Weapon(sSprite* pSprite) {

    if (pSprite->field_22 == eSprite_PersonType_Human) {

        // Fired Weapon?
        if (!pSprite->field_4A)
            return;

        // Delay before weapon fires?
        if (pSprite->field_4A >= 0) {

            pSprite->field_4A--;
            if (pSprite->field_4A)
                return;
        }

    }
    else {
        // No weapon firing?
        if (!pSprite->field_4A)
            return;

        if (pSprite->field_4A >= 0) {

            pSprite->field_4A -= 1;
            if (pSprite->field_4A)
                return;

            pSprite->field_4A = -1;
        }
    }

    // In water? no weapons
    if (pSprite->field_4F)
        return;

    // Are we enemy with rockets?
    if (pSprite->field_18 == eSprite_Enemy_Rocket) {
        mTroop_Weapon_Bullet_Disabled = true;
        mTroop_Weapon_Grenade_Disabled = true;
        mTroop_Weapon_Rocket_Disabled = true;

        // Only fire is unit is more than 24 away
        if (mSprite_Find_Distance > 0x18)
            mTroop_Weapon_Rocket_Disabled = false;

    }
    else {

        // Enemy, after 4th map (can throw grenades)
        //loc_1F4B0
        if (pSprite->field_18 == eSprite_Enemy) {

            // Retail CF didn't allow enemy troops to use grenades until mission 4
            if(!(mVersionCurrent->isRetail() && mGame_Data.mMission_Number < 4)) {

            // Use a grenade?
                if (!(tool_RandomGet() & 0x1F)) {

                    mTroop_Weapon_Bullet_Disabled = true;
                    mTroop_Weapon_Grenade_Disabled = false;
                }
            }
        }
    }

    // bullet weapon disabled?
    if (!mTroop_Weapon_Bullet_Disabled) {

        if (Sprite_Create_Bullet(pSprite))
            pSprite->field_4A = -1;
        else
            pSprite->field_4A = 0;

        if (word_3AA1D != 2)
            pSprite->field_45 = 0x0F;
    }
    mTroop_Weapon_Bullet_Disabled = false;

    // Grenade disabled?
    if (!mTroop_Weapon_Grenade_Disabled) {

        if (!Sprite_Create_Grenade(pSprite))
            pSprite->field_4A = 0;
        else
            pSprite->field_4A = -1;

        if (word_3AA1D != 2)
            pSprite->field_45 = 0x0C;

    }
    mTroop_Weapon_Grenade_Disabled = false;

    // Second weapon disabled?
    if (!mTroop_Weapon_Rocket_Disabled) {

        if (!Sprite_Create_Rocket(pSprite))
            pSprite->field_4A = 0;
        else
            pSprite->field_4A = -1;

        // 
        if (pSprite->field_45 < 0x0A) {

            pSprite->field_45 += 5;
            pSprite->field_44 = pSprite->field_45;
        }
    }
    mTroop_Weapon_Rocket_Disabled = false;
}

void cFodder::Sprite_Handle_Troop_Direct_TowardWeaponTarget(sSprite* pSprite) {

    int16 Data0 = pSprite->field_2E;
    int16 Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Set_Direction_To_Next(pSprite);
    Sprite_Handle_Troop_FrameUnk(pSprite);
}

void cFodder::Sprite_Handle_Troop_Direct_TowardMouse(sSprite* pSprite) {

    int16 Data0 = mMouseX;
    Data0 += mCameraX >> 16;
    Data0 -= 0x18;

    int16 Data4 = mMouseY;
    Data4 += mCameraY >> 16;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Set_Direction_To_Next(pSprite);

    mStoredSpriteFrame = pSprite->field_A;
    Sprite_Handle_Troop_FrameUnk(pSprite);

    if (!pSprite->field_43)
        return;

    pSprite->field_A = mStoredSpriteFrame;
}

void cFodder::Sprite_XY_Store(sSprite* pSprite) {
    mSprite_Reached_Target = 0;

    mStoredSpriteX = (pSprite->field_0 & 0xFFFF) | (pSprite->field_2 << 16);
    mStoredSpriteY = (pSprite->field_4 & 0xFFFF) | (pSprite->field_6 << 16);
}

void cFodder::Sprite_XY_Restore(sSprite* pSprite) {
    mSprite_Reached_Target = -1;

    pSprite->field_0 = mStoredSpriteX & 0xFFFF;
    pSprite->field_2 = mStoredSpriteX >> 16;

    pSprite->field_4 = mStoredSpriteY & 0xFFFF;
    pSprite->field_6 = mStoredSpriteY >> 16;
}

void cFodder::Sprite_Handle_Player_Adjust_Movement_Speed(sSprite* pSprite) {
    sub_1FCF2(pSprite);

    if (pSprite->field_4F)
        return;

    if (pSprite->field_50)
        return;

    int16 Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;

    int16 Data4 = 0;
    for (Data4 = 0; Data4 < 0x0E; Data4 += 2) {
        if (Data0 == pSprite->field_3C)
            break;

        Data0 += 2;
        Data0 &= 0x0E;
    }

    pSprite->field_36 = mSprite_Speed_Direction_Modifier[Data4 / 2];
}

void cFodder::Sprite_Draw_Row_Update(sSprite* pSprite) {
    int16 Data4 = 0;

    // Sprite still moving?
    if (!Sprite_Reached_Target(pSprite)) {
        Sprite_Movement_Speed_Update(pSprite);
        Sprite_Movement_Calculate(pSprite);
    }

    Squad_Join_Check(pSprite);
    Sprite_Reached_MapEdge(pSprite);

    // Check the terrain
    int8 PreviousTileType = pSprite->field_60;
    Sprite_Terrain_Check(pSprite, Data4);

    // Not leaving water?
    if (PreviousTileType != eTerrainFeature_Water)
        goto loc_1F75D;

    // Leaving water and into Quick sand?
    if (pSprite->field_60 == eTerrainFeature_QuickSand)
        goto loc_1F753;

    // Leaving water for the edge/bank
    if (pSprite->field_60 == eTerrainFeature_WaterEdge)
        goto loc_1F75D;

    pSprite->field_52 = 5;
    goto loc_1F75D;

loc_1F753:;
    pSprite->field_52 = 3;

loc_1F75D:;
    Sprite_Handle_Player_Close_To_SquadMember(pSprite);
}

void cFodder::Sprite_Handle_Troop_FrameUnk(sSprite* pSprite) {
    int16 Data0, Data8;
    sSprite* Dataa30 = 0;
    sMapTarget* Data30 = 0;
    const int16* Data28 = 0;
    sMission_Troop* Data24 = 0;

    // Is Human
    if (pSprite->field_22 == eSprite_PersonType_Human)
        goto AlterAnimation;

    // Hostage Following Sprite?
    if (!pSprite->field_70)
        goto AlterAnimation;

    // Hasnt Reached X Target
    if (pSprite->field_0 != pSprite->field_26)
        goto AlterAnimation;

    // Hasnt Reached Y Target
    if (pSprite->field_4 != pSprite->field_28)
        goto AlterAnimation;

    // Idle
    pSprite->field_8 = 0xDC;
    pSprite->field_A = 0;
    word_3ABB1 = -1;
    return;

AlterAnimation:;
    if (word_3AA1D == 2)
        goto loc_1F7FF;

    if (pSprite->field_5A)
        goto loc_1F7F9;

    if (!pSprite->field_43)
        goto loc_1F7FF;

    if (pSprite->field_4F)
        goto loc_1F7FF;

loc_1F7F9:;
    mSprite_Bumped_Into_SquadMember = -1;

loc_1F7FF:;
    Data8 = pSprite->field_3C;
    Data8 += mDirectionMod;
    Data8 &= 0x0E;

    pSprite->field_3C = Data8;
    Data8 /= 2;

    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    if (!mSprite_FaceWeaponTarget)
        goto loc_1F9C0;

    if (pSprite->field_22 == eSprite_PersonType_Human) {
        Data30 = mSquad_WalkTargets[pSprite->field_32];
        if (Data30[pSprite->field_40].mX >= 0)
            goto loc_1F9C0;

        //seg005:01CC
        Dataa30 = mSquads[pSprite->field_32][0];

        if (Dataa30 == INVALID_SPRITE_PTR || Dataa30 == 0)
            goto loc_1F9C0;

        //seg005:020C
        if (Dataa30 != pSprite) {

            Data0 = Dataa30->field_0;

            if (Data0 != Dataa30->field_26)
                goto loc_1F9C0;

            Data0 = Dataa30->field_4;
            if (Data0 != Dataa30->field_28)
                goto loc_1F9C0;

            if (!word_3B2F3)
                goto loc_1F9C0;
        }
        else {

            Data0 = pSprite->field_0;
            if (Data0 != pSprite->field_26)
                goto loc_1F9C0;

            //seg005:0246
            Data0 = pSprite->field_4;
            if (Data0 != pSprite->field_28)
                goto loc_1F9C0;
        }
    }

    if (pSprite->field_60 == eTerrainFeature_Water)
        goto loc_1F9C0;

    //seg005:02A6
    pSprite->field_8 = *(Data28 + Data8 + 0x30);
    pSprite->field_A = 0;
    mStoredSpriteFrame = 0;
    return;

loc_1F9C0:;
    if (pSprite->field_54 != 2)
        goto loc_1FA39;

    if (word_3AA1D) {
        if (word_3AA1D == 2) {
            pSprite->field_8 = *(Data28 + Data8);
            pSprite->field_A = mSprite_Frame_3;
            // FIX: Added as only 2 frames exist for all sprites in mSprite_AnimationPtrs
            pSprite->field_A %= 2;
        }
        else {
            //loc_1F9FF
            pSprite->field_8 = *(Data28 + Data8);
            pSprite->field_A = 0;
        }
    }
    //loc_1FA20
    --pSprite->field_57;
    if (pSprite->field_57)
        return;

    pSprite->field_54 = 0;
    goto loc_1FB00;

loc_1FA39:;
    if (pSprite->field_54 != 1)
        goto loc_1FB00;

    Data0 = *(Data28 + Data8 + 0x28);

    if (Data0 != pSprite->field_8) {
        pSprite->field_8 = *(Data28 + Data8 + 0x28);
        pSprite->field_55 = 0;
        pSprite->field_A = 0;
        return;
    }
    //loc_1FA93
    pSprite->field_55++;
    if (pSprite->field_55 != 2)
        return;
    pSprite->field_55 = 0;
    pSprite->field_A++;
    mStoredSpriteFrame = pSprite->field_A;

    if (pSprite->field_A < 3)
        return;
    mStoredSpriteFrame = 0;
    pSprite->field_A = 0;
    pSprite->field_54 = 0;
    pSprite->field_5A = 0;
    pSprite->field_55 = 0;

loc_1FB00:;
    if (pSprite->field_54 != 3)
        goto loc_1FBA4;

    Data0 = *(Data28 + Data8 + 0x38);
    if (Data0 != pSprite->field_8) {
        pSprite->field_8 = Data0;
        pSprite->field_55 = 0;
        pSprite->field_A = 0;
        return;
    }
    //loc_1FB5A
    pSprite->field_55++;
    if (pSprite->field_55 != 7)
        return;

    pSprite->field_55 = 0;
    pSprite->field_A = 0;
    pSprite->field_54 = 0;
    pSprite->field_5A = 0;
    pSprite->field_55 = 0;

loc_1FBA4:;
    // If in water
    if (pSprite->field_4F) {

        pSprite->field_8 = *(Data28 + Data8 + 0x20);
        Data0 = mSprite_Frame_1;

        // Reached another troop?
        if (pSprite->field_43)
            Data0 = mSprite_Frame_2;

        Data0 &= 1;
        pSprite->field_A = Data0;
        goto loc_1FCD7;
    }
    //loc_1FBF8
    if (!word_3AA1D) {
        pSprite->field_8 = *(Data28 + Data8 + 0x18);
        pSprite->field_A = mSprite_Frame_2;
        goto loc_1FCD7;
    }
    //loc_1FC29
    if (pSprite->field_8 == *(Data28 + Data8 + 0x18) && pSprite->field_A) {
        pSprite->field_A = 0;
        goto loc_1FCD7;
    }

    //loc_1FC61
    Data24 = pSprite->field_46_mission_troop;

    if (Data24 == 0)
        Data0 = 0x70;
    else
        Data0 = Data24->field_6;

    if (Data0 == 3)
        pSprite->field_8 = *(Data28 + Data8 + 0);
    else
        pSprite->field_8 = *(Data28 + Data8);

    //loc_1FCB4
    if (Data0 != 2)
        pSprite->field_A = mSprite_Frame_1;
    else
        pSprite->field_A = mSprite_Frame_3;

loc_1FCD7:;
    // Bumped into other troop
    if (!mSprite_Bumped_Into_SquadMember)
        return;

    // FIX: Added % as only 2 frames exist for all sprites in mSprite_AnimationPtrs
    // There is one case where mTmp_FrameNumber isnt set by the current sprite
    pSprite->field_A = mTmp_FrameNumber % 2;
    mSprite_Bumped_Into_SquadMember = 0;
}

void cFodder::sub_1FCF2(sSprite* pSprite) {
    int16 Data0;

    pSprite->field_36 = 0x10;

    if (pSprite->field_18 == eSprite_Enemy) {
        Data0 = 0x0C;
        Data0 += pSprite->field_62;
        if (Data0 > 0x1A)
            Data0 = 0x1A;
        pSprite->field_36 = Data0;
    }
    else {

        //loc_1FD36
        Data0 = word_3BED5[pSprite->field_32];
        if (!Data0)
            pSprite->field_36 >>= 1;
        else {
            if (Data0 != 1)
                pSprite->field_36 = 0x18;
        }
    }

    if (pSprite->field_4F)
        goto loc_1FDDC;

    Data0 = pSprite->field_52;
    if (Data0 == pSprite->field_50)
        goto loc_1FDD5;
    if (Data0 < pSprite->field_50)
        goto loc_1FDC3;

    Data0--;
    if (Data0 == pSprite->field_50)
        goto loc_1FDD5;
    Data0--;
    goto loc_1FDC8;

loc_1FDC3:;
    Data0++;
loc_1FDC8:;
    pSprite->field_52 = Data0;
loc_1FDD5:;
    if (!Data0)
        return;
loc_1FDDC:;
    pSprite->field_36 = 6;
}

void cFodder::sub_1FDE7(sSprite* pSprite) {

    int16 Data0 = pSprite->field_2E;
    int16 Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points(pSprite, Data0, Data4);
    Sprite_Set_Direction_To_Next(pSprite);

    mStoredSpriteFrame = pSprite->field_A;

    Sprite_Handle_Troop_FrameUnk(pSprite);
    if (!pSprite->field_43)
        return;

    pSprite->field_A = mStoredSpriteFrame;
}

void cFodder::Sprite_Create_Player_Shadow(sSprite* pSprite) {
    int16 Data0 = 0;

    sSprite* Data2C, *Data30;

    if (pSprite->field_5C == 0)
        return;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data2C->field_1A_sprite = pSprite;
    Data2C->field_6A_sprite = pSprite;

    Data2C->field_0 = pSprite->field_0;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_0 += 4;
    Data2C->field_8 = 0x8D;
    Data2C->field_A = 2;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_6 = pSprite->field_6;
    Data2C->field_18 = eSprite_Player_Shadow;
    Data2C->field_62 = pSprite->field_18;
    Data2C->field_20 = 0;
    Data2C->field_32 = -1;
    Data2C->field_2C = eSprite_Draw_First;
    Data2C->field_52 = 0;
}

int16 cFodder::Sprite_Create_BloodTrail(sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30) {
    int16 Data0;

    Data0 = 1;

    if (!Sprite_Get_Free_Max42(Data0, pData2C, pData30)) {
        pData2C->field_0 = pSprite->field_0;
        pData2C->field_2 = pSprite->field_2;
        pData2C->field_4 = pSprite->field_4;
        pData2C->field_6 = pSprite->field_6;

        pData2C->field_0 += 3;
        pData2C->field_4 -= 8;
        pData2C->field_8 = 0x96;
        pData2C->field_A = 0;
        pData2C->field_18 = eSprite_BloodTrail;
        pData2C->field_20 = pSprite->field_20;
        pData2C->field_32 = -1;
        pData2C->field_2C = eSprite_Draw_OnTop;
        pData2C->field_52 = 0;
        pData2C->field_12 = 1;
        Data0 = 0;
    }

    return Data0;
}

void cFodder::Sprite_Terrain_Check(sSprite* pSprite, int16& pData4) {
    int16 Data0 = -3;   // Y
    pData4 = 8; // X

    // pData4 becomes terrain type
    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, pData4))
        goto CheckFalling;

    pSprite->field_60 = pData4 & 0xFF;
    pSprite->field_50 = 0;
    pSprite->field_4F = 0;

    if (pData4 == eTerrainFeature_Drop2) {
        if (pSprite->field_18 == eSprite_Enemy)
            goto loc_20251;

        if (pSprite->field_22 == eSprite_PersonType_Native)
            goto loc_20251;

        if (!pSprite->field_38 || pSprite->field_38 >= eSprite_Anim_Slide1) {
            pSprite->field_38 = eSprite_Anim_Hit3;
            return;
        }
    }

    //loc_20044
    if (pData4 == eTerrainFeature_Drop) {

        if (pSprite->field_18 == eSprite_Enemy)
            goto loc_20251;

        if (pSprite->field_22 == eSprite_PersonType_Native)
            goto loc_20251;

        ++pSprite->field_56;
        return;
    }

    //loc_20072
    pSprite->field_56 = 0;
    if (pData4 == eTerrainFeature_QuickSandEdge)
        goto loc_201CC;

    if (pData4 == eTerrainFeature_Rocky)
        goto loc_20108;

    if (pData4 == eTerrainFeature_Rocky2)
        goto loc_2014D;

    if (!pSprite->field_61)
        goto loc_200C0;

    pSprite->field_20 -= 3;
    if (!pSprite->field_20)
        goto loc_200B7;
    if (pSprite->field_20 >= 0)
        goto loc_200C0;

    pSprite->field_20 = 0;
loc_200B7:;
    pSprite->field_61 = 0;

loc_200C0:;
    if (pData4 == eTerrainFeature_QuickSand)
        goto Soldier_InQuickSand;

    if (pData4 == eTerrainFeature_WaterEdge)
        goto Soldier_WaterEdge;

    if (pData4 != eTerrainFeature_Water)
        goto checkSinking;

    // Is a Native?
    if (pSprite->field_22 == eSprite_PersonType_Native)
        goto loc_20251;

    // In Water
    pSprite->field_4F = -1;
    pSprite->field_50 = 0;
    pSprite->field_52 = 0;
    return;

loc_20108:;
    pSprite->field_52 = 0;
    if (pSprite->field_61) {
        pSprite->field_20 = 2;
        return;
    }
    if (!pSprite->field_20) {
        pSprite->field_61 = -1;
        pSprite->field_20 = 1;
    }
    return;

loc_2014D:;
    pSprite->field_52 = 0;
    if (pSprite->field_61) {
        if (pSprite->field_20 >= 6)
            return;

        pSprite->field_20 += 2;
        return;
    }
    //loc_2017C
    if (pSprite->field_20)
        return;
    pSprite->field_61 = -1;
    pSprite->field_20 = 1;
    return;

Soldier_InQuickSand:;
    pSprite->field_50 = 3;
    return;

Soldier_WaterEdge:;
    pSprite->field_50 = 6;
    return;

checkSinking:;
    if (pData4 == eTerrainFeature_Sink)
        goto HumanSinking;

    pSprite->field_52 = 0;
    return;

loc_201CC:;
    if (pSprite->field_38)
        return;
    if (!pSprite->field_36)
        return;
    if (pSprite->field_52)
        return;
    if (pSprite->field_20)
        return;

    Data0 = tool_RandomGet() & 0x3F;
    if (Data0)
        return;

    if (pSprite->field_22 == eSprite_PersonType_Native)
        return;

    pSprite->field_38 = eSprite_Anim_Slide1;
    return;

HumanSinking:;
    pSprite->field_5B = 1;
    return;

CheckFalling:;
    if (pData4 == eTerrainFeature_Drop || pData4 == eTerrainFeature_Drop2) {
        pSprite->field_38 = eSprite_Anim_Hit2;
        return;
    }

loc_20251:;
    pSprite->field_43 = 1;
    mSprite_Reached_Target = -1;
    if (pSprite->field_22 == eSprite_PersonType_Human)
        goto loc_2035C;

    pSprite->field_0 = mStoredSpriteX & 0xFFFF;

    Data0 = -3;
    pData4 = 8;
    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, pData4))
        goto loc_202E5;

    if (pData4 == eTerrainFeature_Drop)
        goto loc_202E5;

    if (pData4 == eTerrainFeature_Drop2)
        goto loc_202E5;

    if (pSprite->field_22 == eSprite_PersonType_Native && pData4 == eTerrainFeature_Water)
        goto loc_202E5;

    Data0 = pSprite->field_10;
    if (Data0 <= 0x80 || Data0 > 0x180) {
        pSprite->field_10 = 0;
        goto loc_20307;
    }

    pSprite->field_10 = 0x100;
    goto loc_20307;

loc_202E5:;
    if (pSprite->field_10 > 0x100) {
        pSprite->field_10 = 0x180;
        goto loc_20307;
    }

    pSprite->field_10 = 0x80;
loc_20307:;

    Data0 = mGame_InputTicks;
    Data0 >>= 2;
    Data0 &= 0x3F;
    Data0 -= 0x20;
    Data0 &= 0x1FE;
    pSprite->field_10 += Data0;
    pSprite->field_4 = mStoredSpriteY;

    Data0 = mGame_InputTicks;
    Data0 &= 0x1F;
    Data0 += 0x0C;
    pSprite->field_44 = (int8)Data0;
    return;

loc_2035C:;

    int16 word_3A9E6 = pSprite->field_0;
    pSprite->field_0 = mStoredSpriteX & 0xFFFF;

    Data0 = -3;
    pData4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, pData4))
        return;

    pSprite->field_0 = word_3A9E6;
    pSprite->field_4 = mStoredSpriteY & 0xFFFF;

    Data0 = -3;
    pData4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, pData4))
        return;

    //loc_203BA
    pSprite->field_A = mTmp_FrameNumber;
    Sprite_XY_Restore(pSprite);
    pSprite->field_26 = pSprite->field_0;
    pSprite->field_28 = pSprite->field_4;

    sMapTarget* Data30 = mSquad_WalkTargets[pSprite->field_32];
    pData4 = 0;

    for (;;) {
        sMapTarget eax = *Data30++;

        if (eax.asInt < 0)
            break;

        pData4++;
    }

    pSprite->field_40 = pData4;
    pSprite->field_4C = -1;
}

void cFodder::String_Print_Small(std::string pText, const size_t pY) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    String_CalculateWidth(320, mFont_Briefing_Width, pText);
    String_Print(mFont_Briefing_Width, 0, mGUI_Temp_X, pY, pText);
}

void cFodder::String_Print_Small(std::string pText, const size_t pX, const size_t pY) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    String_CalculateWidth(320, mFont_Briefing_Width, pText);
    String_Print(mFont_Briefing_Width, 0, pX, pY, pText);
}

void cFodder::String_Print_Large(std::string pText, const bool pOverAndUnderLine, const size_t pY) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    String_CalculateWidth(320, mFont_Underlined_Width, pText);
    String_Print(mFont_Underlined_Width, pOverAndUnderLine == true ? 1 : 3, mGUI_Temp_X, pY, pText);
}

void cFodder::String_Print_Large(std::string pText, const bool pOverAndUnderLine, const size_t pX, const size_t pY) {
	std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

	String_CalculateWidth(320, mFont_Underlined_Width, pText);
	String_Print(mFont_Underlined_Width, pOverAndUnderLine == true ? 1 : 3, pX, pY, pText);
}

void cFodder::String_Print(const uint8* pWidths, int32 pFontSpriteID, size_t pParam08, size_t pParamC, const std::string& pText) {

    String_Print(pWidths, pFontSpriteID, pParam08, pParamC, pText.c_str());
}

void cFodder::String_Print(const uint8* pWidths, int32 pFontSpriteID, size_t pParam08, size_t pParamC, const char* pText) {
    const uint8* ptr = 0;
    uint8 al = 0;
    int32 unk14 = 0;

    mGUI_Temp_Y = (int16)pParamC;
    mGUI_Draw_LastHeight = 0;

    for (;;) {
    String_Print_Next:;
        uint8 NextChar = *pText++;
        uint8 NextChar10 = 0;

        //seg007:0170
        if (NextChar == 0xFD) {
            //sub_29DE2(0x10);

        }
        else if (NextChar == 0xFE) {
            //sub_29DE2(0x0F);

        }
        else if (NextChar == 0x00)
            break;

        else {
            //seg007:01A1
            NextChar10 = NextChar;

            if (!(NextChar & 0x80)) {

                //01AB
                if (mString_GapCharID) {
                    if (NextChar == 0x20) {
                        NextChar = (uint8)mString_GapCharID;
                        goto loc_29D71;
                    }
                }
                else {
                    //1C4
                    if (NextChar == 0x20)
                        goto loc_29DC7;
                }
                //1CD
                if (NextChar <= 0x39) {
                    if (NextChar >= 0x30) {
                        NextChar -= 0x30;
                        NextChar += 0x1A;
                        goto loc_29D71;
                    }
                }
                else {
                    //loc_29D07
                    if (NextChar > 0x5A) {
                        NextChar -= 0x61;
                        NextChar += PLATFORM_BASED(0x39, 0x28);

                        goto loc_29D71;
                    }
                    else {
                        if (NextChar >= 0x41) {
                            NextChar -= 0x41;
                            goto loc_29D71;
                        }
                    }
                }
            }

            // Must be a special character
            // 20D
            //loc_29D2D
            unk14 = -1;
            ptr = mGUI_Font_SpecialCharacters;
            do {

                // Reached end of table? then skip this character
                if (*ptr == 0xFF)
                    goto String_Print_Next;

                ++unk14;
                al = *ptr++;

                // Loop until we find this character in the table
            } while (al != NextChar);

            // Found the special character, lets use its index
            NextChar = 0x24 + unk14;

        loc_29D71:;

            if (mGUI_Print_String_To_Sidebar)
                mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(pFontSpriteID + NextChar, pParam08, pParamC);
            else
                GUI_Draw_Frame_8(pFontSpriteID, NextChar, pParam08, pParamC);

        }
    loc_29DC7:;

        NextChar10 = pWidths[NextChar10];

        pParam08 += NextChar10;
    }
}

void cFodder::Vehicle_Input_Handle() {

    if (!mButtonPressLeft) {
        mVehicle_Input_Disabled = false;
        return;
    }

    if (mVehicle_Input_Disabled)
        return;

    if (Mouse_Button_Left_Toggled() >= 0)
        if (sub_313CD() < 0)
            return;

    sSprite* Data20 = mSquad_Leader;

    if (Data20 == INVALID_SPRITE_PTR)
        return;
    int16 Data0 = mMouseX + (mCameraX >> 16);
    int16 Data4 = mMouseY + (mCameraY >> 16);

    Data0 -= 0x1C;

    if (Data0 < 0)
        Data0 = 0;
    Data4 += 6;
    if (Data4 < 0)
        Data4 = 0x14;

    if (Data4 < 0x14)
        Data4 = 0x14;

    if (Data20->field_6F >= eVehicle_Helicopter) {
        if (Data20->field_50 <= 8)
            Data4 += 0x20;
    }

    mCamera_PanTargetX = Data0;
    mCamera_PanTargetY = Data4;

    mMouse_Locked = false;

    Data20->field_26 = Data0;
    Data20->field_28 = Data4;
}

void cFodder::sub_311A7() {
    if (mSquad_Selected < 0)
        return;

    sSprite** Data24 = mSquads[mSquad_Selected];

    for (;;) {
        if (*Data24 == INVALID_SPRITE_PTR)
            break;
        sSprite* Data28 = *Data24++;
        Data28->field_66 = 0;
    }

    int16 Data0 = mMouseX;
    int16 Data4 = mMouseY;

    Data0 += mCameraX >> 16;
    Data4 += mCameraY >> 16;

    Data0 -= 0x0F;
    Data4 -= 3;

    for(auto& Sprite : mSprites) {

        if (Sprite.field_0 == -32768)
            continue;

        if (!Sprite.field_65)
            continue;

        if (Sprite.field_22 != eSprite_PersonType_Human)
            continue;

        if (Sprite.field_20)
            continue;

        int16 Data18 = Sprite.field_18;
        int16 Data8 = Sprite.field_0;

        if (Sprite.field_6F == eVehicle_Turret_Cannon ||
            Sprite.field_6F == eVehicle_Turret_Missile)
            Data8 -= 8;

        if (Data0 < Data8)
            continue;

        Data8 += mSprite_Width[Data18];
        if (Data0 > Data8)
            continue;

        Data8 = Sprite.field_4 - mSprite_Height_Top[Data18];
        Data8 -= 0x14;
        if (Data4 < Data8)
            continue;

        Data8 = Sprite.field_4 + mSprite_Height_Bottom[Data18];
        if (Data4 > Data8)
            continue;

        if (mSquad_Selected < 0)
            break;

        Data24 = mSquads[mSquad_Selected];
        for (;;) {
            if (*Data24 == INVALID_SPRITE_PTR)
                goto loc_313C6;

            sSprite* Data28 = *Data24++;
            Data28->field_66 = &Sprite;
        }
    }

loc_313C6:;
}

int16 cFodder::sub_313CD() {
    const int16* Data2C = mSprite_Width;
    const int16* Data30 = mSprite_Height_Top;

    int16 Data0 = mMouseX;
    int16 Data4 = mMouseY;
    int16 Data8, Data18;

    sSprite* Data20, *Dataa30;
    sSprite** Dataa2C;

    Data0 += mCameraX >> 16;
    Data4 += mCameraY >> 16;

    Data0 -= 0x0F;
    Data4 -= 3;

    if (!mSquad_CurrentVehicle)
        goto loc_31692;

    Data20 = mSquad_CurrentVehicle;
    if (Data20->field_0 == -32768)
        goto loc_31689;

    if (Data20->field_22 != eSprite_PersonType_Human)
        goto loc_31689;

    Data18 = Data20->field_18;
    Data8 = Data20->field_0;
    if (Data0 < Data8)
        goto loc_31689;

    Data8 += Data2C[Data18];
    if (Data0 > Data8)
        goto loc_31689;

    //seg011:29DA
    Data8 = Data20->field_4;
    Data8 -= Data20->field_20;

    Data8 -= Data30[Data18];
    Data8 -= 0x14;
    if (Data4 < Data8)
        goto loc_31689;

    Data8 = Data20->field_4;
    Data8 -= Data20->field_20;
    if (Data4 > Data8)
        goto loc_31689;

    if (!Data20->field_20)
        goto loc_31514;

    Data20->field_6E = -1;
    mVehicle_Input_Disabled = true;
    goto loc_3167D;

loc_31514:;
    Data0 = -3;
    Data4 = 8;

    if (Map_Terrain_Get_Type_And_Walkable(Data20, Data0, Data4))
        goto loc_31689;

    Data0 = mSquad_Selected;
    if (Data0 < 0)
        goto loc_31668;

    //seg011:2A84
    mSquad_EnteredVehicleTimer[Data0] = 0;
    Dataa2C = mSquads[Data0];

    //loc_31578
    for (; Data18 >= 0; --Data18) {

        if (*Dataa2C == INVALID_SPRITE_PTR)
            goto loc_31668;

        sSprite* Sprite = *Dataa2C++;

        if (!Sprite->field_6E)
            continue;

        //seg011:2AE9 

        Sprite->field_6E = 0;
        Dataa30 = Sprite->field_6A_sprite;
        Sprite->field_6A_sprite = 0;
        Sprite->field_66 = 0;

        Sprite->field_0 = Dataa30->field_0;
        Sprite->field_4 = Dataa30->field_4;

        if (Sprite->field_6F == eVehicle_Turret_Cannon)
            goto loc_3162B;

        if (Sprite->field_6F == eVehicle_Turret_Missile)
            goto loc_3162B;

        Sprite->field_0 += 0x0F;
        Sprite->field_4 += -10;

    loc_3162B:;
        Sprite->field_26 = Sprite->field_0;
        Sprite->field_28 = Sprite->field_4;
        Sprite->field_26 -= 6;
        Sprite->field_28 += 0x10;
    }

loc_31668:;
    mVehicle_Input_Disabled = true;
    Data20->field_6E = -1;
    mMouse_Button_LeftRight_Toggle = false;
loc_3167D:;
    Data0 = -1;
    return -1;

loc_31689:;
    Data20->field_6E = 0;
loc_31692:;
    Data0 = 0;
    return 0;
}

void cFodder::Vehicle_Target_Set() {

    if (Mouse_Button_Right_Toggled() < 0)
        return;

    sSprite* Vehicle = mSquad_CurrentVehicle;
    Vehicle->field_54 = -1;

    int16 PosX = mMouseX;
    int16 PosY = mMouseY;
    PosX += mCameraX >> 16;
    PosY += mCameraY >> 16;
    PosX -= 0x10;

    if (!PosX)
        PosX = 1;

    PosY -= 8;
    Vehicle->field_2E = PosX;
    Vehicle->field_30 = PosY;
}

void cFodder::String_CalculateWidth(int32 pPosX, const uint8* pWidths, const std::string& pString) {

    String_CalculateWidth(pPosX, pWidths, pString.c_str());
}

void cFodder::String_CalculateWidth(int32 pPosX, const uint8* pWidths, const char* pString) {
    int32 PositionX = 0;

    for (const char* Text = pString; *Text; ++Text) {
        uint8 Char = *Text;

        if (Char == 0xFE || Char == 0xFD || Char == 0xFF)
            break;

        PositionX += pWidths[Char];
    }

    pPosX -= PositionX;
    if (pPosX < 0) {
        pPosX = 0;
        PositionX = 318;
    }

    pPosX >>= 1;
    mGUI_Temp_X = pPosX;
    mGUI_Temp_Width = PositionX;
}

void cFodder::Intro_OpenFodder() {

	if (!mOpenFodder_Intro_Done && !mParams->mSkipIntro) {
		bool CF2 = false;

		mPhase_Aborted = false;
		mGame_Data.mMission_Number = 0;

		// Use a CF2 intro?
		if (mVersions->isCampaignAvailable("Cannon Fodder 2"))
			CF2 = (tool_RandomGet() & 2);
		if (CF2)
			VersionSwitch(mVersions->GetForCampaign("Cannon Fodder 2", mParams->mDefaultPlatform));

		// Random intro
		auto Tileset = static_cast<eTileTypes>(((uint8)tool_RandomGet()) % eTileTypes_Hid);
		Mission_Intro_Play(true, Tileset);
		mOpenFodder_Intro_Done = true;
		if (CF2)
			VersionSwitch(mVersions->GetForCampaign("Cannon Fodder", mParams->mDefaultPlatform));
	}
}

void cFodder::intro_LegionMessage() {
    int16 Duration = 325 / 4;
    bool DoBreak = false;

    mSurface->clearBuffer();
    mGraphics->PaletteSet();

    Intro_Print_String(&mVersionCurrent->getIntroData()->at(0).mText[0]);
	Intro_Print_String(&mVersionCurrent->getIntroData()->at(0).mText[1]);
	Intro_Print_String(&mVersionCurrent->getIntroData()->at(0).mText[2]);

    while (mSurface->isPaletteAdjusting() || DoBreak == false) {

        Mouse_Inputs_Get();

       if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        if (Duration > 1)
            --Duration;
        else {
            if (DoBreak == false) {
                mSurface->paletteNew_SetToBlack();
                Duration = 0;
                DoBreak = true;
            }
        }

        Video_SurfaceRender(false);
        Cycle_End();
    }
}

int16 cFodder::intro_Play() {

    mGraphics->Load_Sprite_Font();
    mGraphics->SetActiveSpriteSheet(eGFX_Types::eGFX_FONT);

    for (word_3B2CF = 1; mVersionCurrent->getIntroData()->at(word_3B2CF).mImageNumber != 0; ++word_3B2CF) {

        mIntro_PlayTextDuration = 0x288 / 5;

        mSurface->palette_SetToBlack();

        if (mVersionCurrent->getIntroData()->at(word_3B2CF).mImageNumber == 0 && mVersionCurrent->getIntroData()->at(word_3B2CF).mText == 0)
            break;

        if (mVersionCurrent->getIntroData()->at(word_3B2CF).mImageNumber != 0xFF) {

            std::stringstream ImageName;
            ImageName << (char)mVersionCurrent->getIntroData()->at(word_3B2CF).mImageNumber;

            mGraphics->Load_And_Draw_Image(ImageName.str(), 0xD0);
        }
        else {
            mIntro_PlayTextDuration = 0xAF / 2;
            mSurface->clearBuffer();
        }

        mGraphics->PaletteSet();
        const sIntroString* IntroString = mVersionCurrent->getIntroData()->at(word_3B2CF).mText;
        if (IntroString) {
            while (IntroString->mPosition) {

                Intro_Print_String(IntroString);
                ++IntroString;
            }
        }
        //loc_16710

        int16 Duration = mIntro_PlayTextDuration;
        bool DoBreak = false;

        while (mSurface->isPaletteAdjusting() || DoBreak == false) {
            --Duration;

            if (Duration) {
                mSurface->palette_FadeTowardNew();

                Mouse_Inputs_Get();
                if (mMouseButtonStatus) {

                    if (mVersionCurrent->getIntroData()->size() >= 2)
                        word_3B2CF = ((int16)mVersionCurrent->getIntroData()->size()) - 2;

                    mImage_Aborted = -1;
                    mSurface->paletteNew_SetToBlack();
                    DoBreak = true;
                }
            }
            else {
                mSurface->paletteNew_SetToBlack();
                DoBreak = true;
            }

            Video_SurfaceRender(false);
            Cycle_End();
        }


    }

    return mImage_Aborted;
}

void cFodder::Mission_Intro_Play(const bool pShowHelicopter, eTileTypes pTileset) {

    // Single maps
    if (mCustom_Mode == eCustomMode_Map)
        return;

    // If we don't have the briefing graphics, there is no point in switching here,
    //  As the correct background for the map intro won't exist
    if (!mVersionCurrent->hasGfx(eGFX_BRIEFING))
        return;

    mSurface->clearBuffer();
	mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());

    if (pTileset >= eTileTypes_Hid)
		pTileset = eTileTypes_Jungle;

    mGraphics->Mission_Intro_Load_Resources(pTileset);
    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    mMouse_Exit_Loop = false;
    mSound->Music_Play(0x07);
    Mission_Intro_Helicopter_Start();

    Mission_Intro_Draw_Mission_Name();
    mSurface->Save();

	// Prior to mission 4, the UFO is not shown on the mission intro
    bool ShowHelicopter = true;
    if (mVersionCurrent->isCannonFodder2() && mGame_Data.mMission_Number < 4 && !pShowHelicopter)
        ShowHelicopter = false;

	
    mVersionPlatformSwitchDisabled = true;
    mGraphics->Mission_Intro_Play(ShowHelicopter, pTileset);
    mVersionPlatformSwitchDisabled = false;
}

void cFodder::Intro_Print_String(const sIntroString* pString) {

    auto PosY = pString->mPosition + PLATFORM_BASED(-0x19, 9);

    String_CalculateWidth(320, mFont_Intro_Width, pString->mText);
    String_Print(mFont_Intro_Width, 0, mGUI_Temp_X, PosY, pString->mText);
}

void cFodder::Image_FadeIn() {
    mSurface->Save();
    mGraphics->PaletteSet();
    mGame_Data.mDemoRecorded.DisableTicks();
    while (mSurface->isPaletteAdjusting()) {
        mSurface->palette_FadeTowardNew();
        Mouse_Inputs_Get();

        Video_SurfaceRender();
        Cycle_End();
    }

    mGame_Data.mDemoRecorded.EnableTicks();
}

void cFodder::Image_FadeOut() {

    mSurface->Save();
    mSurface->paletteNew_SetToBlack();
#ifdef EMSCRIPTEN
	return;
#endif
    mGame_Data.mDemoRecorded.DisableTicks();
    while (mSurface->isPaletteAdjusting()) {

        Mouse_Inputs_Get();
        //Mouse_DrawCursor();

        // BUGFIX: Prevent a pallete bug when version switching
        if (mRecruit_Screen_Active) {
            Recruit_Cycle();
        } else 
            mSurface->palette_FadeTowardNew();

        Video_SurfaceRender();
        Cycle_End();

    }
    mGame_Data.mDemoRecorded.EnableTicks();

}

void cFodder::SetActiveSpriteSheetPtr(const sSpriteSheet** pSpriteSheet) {

    mSprite_SheetPtr = pSpriteSheet;
}

void cFodder::intro_Retail() {

    // Disabled: GOG CD Version doesn't require a manual check
    CopyProtection();
    mGraphics->Load_Sprite_Font();

	mSound->Music_Play(CANNON_BASED(16, 20));

    if (mVersionCurrent->isCannonFodder1())
        intro_LegionMessage();

    if (mVersionCurrent->isCannonFodder2())
        sleepLoop(1000);

    mPhase_Aborted = false;

    if (ShowImage_ForDuration("cftitle", CANNON_BASED(0x1F8 / 3, 0x280 / 3)))
        return;

    if (intro_Play())
        return;

    if (ShowImage_ForDuration("virgpres", 0x2D0 / 3))
        return;

    if (ShowImage_ForDuration("sensprod", 0x2D0 / 3))
        return;

    if (ShowImage_ForDuration("cftitle", 0x318 / 3))
        return;
}

void cFodder::intro_AmigaTheOne() {
    static bool ShownWarning = false;

    if (!ShownWarning) {
        ShownWarning = true;
        ShowImage_ForDuration("PRETENTIOUS1", 0x60, 3, false);
    }

    if (ShowImage_ForDuration("sensprod.lbm", 0x60))
        return;

    if (ShowImage_ForDuration("virgpres.lbm", 0x60))
        return;

    if (ShowImage_ForDuration("cftitle.lbm", 0x100))
        return;
}

int16 cFodder::ShowImage_ForDuration(const std::string& pFilename, uint16 pDuration, size_t pBackColor, bool pCanAbort) {
    bool DoBreak = false;

    mGraphics->Load_And_Draw_Image(pFilename, 0x100, pBackColor);
    mGraphics->PaletteSet();

    while (mSurface->isPaletteAdjusting() || DoBreak == false) {
        Mouse_Inputs_Get();
        --pDuration;

        if (pDuration) {
           if (mSurface->isPaletteAdjusting())
                mSurface->palette_FadeTowardNew();

            if (pCanAbort && (mMouseButtonStatus || mPhase_Aborted)) {
                mPhase_Aborted = false;
                mImage_Aborted = -1;
                mSurface->paletteNew_SetToBlack();
                DoBreak = true;
            }
        }
        else {
            mSurface->paletteNew_SetToBlack();
            DoBreak = true;
        }

        Video_SurfaceRender(false);
        Cycle_End();
    }

    return mImage_Aborted;
}

void cFodder::Video_SurfaceRender(const bool pRestoreSurface) {

    if (mStartParams->mDisableVideo)
        return;

    mSurface->draw();
    mWindow->RenderAt(mSurface);
    mWindow->FrameEnd();

    if(pRestoreSurface)
        mSurface->Restore();
}

void cFodder::Cycle_End() {
#ifndef _OFED
    if (mParams->mSleepDelta) {
        mTicksDiff = SDL_GetTicks() - mTicksDiff;
        mTicks = mTicksDiff * 40 / 1000;
        sleepLoop(mParams->mSleepDelta * 1000 / 40 - mTicksDiff);
        mTicksDiff = SDL_GetTicks();
    }
#endif

    // New Cycle begins
       
    mWindow->Cycle();
    eventsProcess();
}

void cFodder::sleepLoop(int64 pMilliseconds) {
#ifdef EMSCRIPTEN
	return;
#endif
    uint64 TimeStarted = SDL_GetTicks();
    uint64 TimeFinish = TimeStarted + pMilliseconds;

    do {
        mWindow->EventCheck();

        if (SDL_GetTicks() >= TimeFinish)
            break;

        SDL_Delay(1);

    } while (1);

}

void cFodder::WonGame() {
    
    if (mParams->mSinglePhase)
        return;

    mMouseX = -1;
    mMouseY = -1;
    
    mWindow->SetScreenSize(mVersionCurrent->GetSecondScreenSize());

    if (mVersionCurrent->isAmigaTheOne()) {
       // mGraphics->Load_And_Draw_Image("PRETENTIOUS2", 0x100);
        mGraphics->Load_And_Draw_Image("PRETENTIOUS3", 0x100);
    } else {
        mSound->Music_Play(17);
        mGraphics->Load_And_Draw_Image("WON", 0x100);
    }

    Image_FadeIn();

    for (int count = 500; count >= 0; --count) {
        Video_SurfaceRender();
        Cycle_End();
    }

    Image_FadeOut();

    mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());
}

int16 cFodder::Sprite_Handle_Troop_Get_Frame_For_Direction(sSprite* pSprite) {
    int16 FrameNumber = pSprite->field_10;

    FrameNumber >>= 5;
    FrameNumber -= 1;
    FrameNumber ^= 0x0F;
    FrameNumber &= 0x0E;

    return FrameNumber;
}

void cFodder::Sprite_Reached_MapEdge(sSprite* pSprite) {

    if (pSprite->field_4 < 0 || pSprite->field_4 <= 13) {
        pSprite->field_4 = mStoredSpriteY;
        mSprite_Reached_Target = -1;
    }

    if (pSprite->field_4 >= mMapLoaded->getHeightPixels()) {
        if (pSprite->field_38 == eSprite_Anim_None || pSprite->field_38 >= eSprite_Anim_Slide1) {
            pSprite->field_4 = mStoredSpriteY;
            mSprite_Reached_Target = -1;
        }
    }
    //loc_204E1
    if (pSprite->field_0 <= 0) {
        if (mStoredSpriteX <= 0)
            return;

        goto loc_20521;
    }

    if (pSprite->field_0 + 12 < mMapLoaded->getWidthPixels())
        return;

    if (mStoredSpriteX + 16 >= mMapLoaded->getWidthPixels())
        return;

loc_20521:;
    pSprite->field_0 = mStoredSpriteX;
    mSprite_Reached_Target = -1;
}

int16 cFodder::Sprite_Destroy_Wrapper(sSprite* pSprite) {

    return Sprite_Destroy(pSprite);
}

int16 cFodder::Sprite_Destroy(sSprite* pSprite) {

    pSprite->field_65 = 0;
    pSprite->field_8 = 0x7C;
    pSprite->field_18 = eSprite_Null;
    pSprite->field_A = 0;
    pSprite->field_24 = 1;

    return 1;
}

int16 cFodder::Sprite_Create_Bullet(sSprite* pSprite) {
    int16 Data0 = 1, Data8, Data4;
    sSprite* Data2C = 0, *Data30 = 0;

    if (mPhase_Completed_Timer)
        return -1;

    if (!pSprite->field_2E)
        return -1;

    if (pSprite == mSquad_Leader)
        if (pSprite->field_54 == 1)
            return -1;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    if (mSprite_Projectile_Counters[2] == 0x14)
        return -1;

    ++mSprite_Projectile_Counters[2];
    pSprite->field_54 = 2;
    pSprite->field_5A = -1;
    pSprite->field_57 = 8;
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;
    Data2C->field_8 = 0x9D;
    //seg005:1023 
    Data0 = pSprite->field_3C;
    Data0 >>= 1;
    Data0 &= 0x07;
    Data2C->field_A = Data0;
    Data2C->field_12 = 9;

    if (pSprite->field_18 != eSprite_Player) {

        Data0 = pSprite->field_62;
        Data0 >>= 3;
        Data0 += 8;
        if (Data0 > 0x10)
            Data0 = 0x10;
        Data2C->field_12 = Data0;
    }
    else {

        Data2C->field_5E_SoldierAllocated = pSprite->field_46_mission_troop;
        Data2C->field_5D = -1;

        // Bullet Travel time
        Data0 = mSprite_Weapon_Data.mAliveTime;
        Data0 += mSprite_Bullet_Time_Modifier;
        Data2C->field_12 = Data0;
    }

    Data2C->field_18 = eSprite_Bullet;
    Data2C->field_1E_Big = pSprite->field_1E_Big;
    Data2C->field_20 += 6;
    Data2C->field_52 = pSprite->field_52;
    Data2C->field_22 = pSprite->field_22;
    Data2C->field_32 = pSprite->field_32;
    Data2C->field_2C = eSprite_Draw_Second;
    if (pSprite->field_22 != eSprite_PersonType_Human)
        goto loc_2087D;

    Data0 = mSprite_Weapon_Data.mSpeed;
    Data0 += mSprite_Bullet_Fire_Speed_Modifier;
    Data2C->field_4A = Data0;

    Data0 = tool_RandomGet() & 0x0F;
    Data0 <<= 3;
    Data2C->field_36 = Data0;
    goto loc_208A6;

loc_2087D:;
    // AI Fire Speed
    Data0 = 0x3C;
    Data0 += pSprite->field_62;
    Data2C->field_4A = Data0;
    Data2C->field_36 = 0x18;

loc_208A6:;
    Data2C->field_43 = 0;
    Data2C->field_44 = 0;
    Data2C->field_2E = pSprite->field_2E;
    Data2C->field_30 = pSprite->field_30;
    if (pSprite->field_18 != eSprite_Player)
        Data2C->field_2E += 7;
    else
        Data2C->field_30 += 0x0F;

    Data2C->field_52 = 0;
    Data2C->field_3A = 0;

    Data2C->field_1A_sprite = pSprite;
    Data2C->field_2A = 2;
    Data2C->field_16 = 0;
    Data2C->field_14 = 2;

    if (Data2C->field_A == 4)
        Data2C->field_14 = -1;

    Data2C->field_34 = -1;
    Data2C->field_50 = 0;
    Data2C->field_59 = 0;
    Data8 = 7;

    // Is Human?
    if (pSprite->field_22 == eSprite_PersonType_Human) {

        if (pSprite == mSquad_Leader) {

            mSprite_Bullet_Deviate_Counter += 1;
            mSprite_Bullet_Deviate_Counter &= 3;

            if (!mSprite_Bullet_Deviate_Counter)
                goto loc_209F3;

            Data8 = mSprite_Weapon_Data.mDeviatePotential;
        }
        else {

            Data8 = Mission_Troop_GetDeviatePotential(pSprite->field_46_mission_troop);
        }
    }

    Data0 = tool_RandomGet();
    Data4 = Data0;

    Data0 &= Data8;
    if (Data4 < 0)
        Data0 = -Data0;

    Data2C->field_50 = Data0;

loc_209F3:;
    Data2C->field_64 = 0;
    Data0 = tool_RandomGet() & 1;
    if (!Data0)
        Data4 = 0x11;
    else
        Data4 = 0x10;

    Sound_Play(pSprite, Data4, 0);
    return 0;
}

int16 cFodder::Sprite_Create_Grenade(sSprite* pSprite) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data2C = 0, *Data30 = 0;

    if (mPhase_Completed_Timer)
        goto loc_20ADE;

    if (mSprite_Projectile_Counters[pSprite->field_22] == 2)
        goto loc_20ADE;

    if (!pSprite->field_2E)
        goto loc_20ADE;

    if (pSprite->field_18 != eSprite_Enemy)
        goto loc_20AC1;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;
    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 >= 0x82)
        goto loc_20ADE;

loc_20AC1:;

    Data0 = 2;

    if (!Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        goto loc_20B0A;

    return -1;

loc_20ADE:;
    if (pSprite == mSquad_Leader)
        mMouse_Button_LeftRight_Toggle = false;

    return -1;

loc_20B0A:;
    if (pSprite->field_18 != eSprite_Player)
        goto loc_20B6E;

    Data0 = pSprite->field_32;
    Data4 = Data0;
    if (!mSquad_Grenades[Data0])
        goto loc_20ADE;

    --mSquad_Grenades[Data0];
    mGUI_RefreshSquadGrenades[Data4] = -1;

loc_20B6E:;

    ++mSprite_Projectile_Counters[pSprite->field_22];
    pSprite->field_55 = 1;
    pSprite->field_5A = -1;
    pSprite->field_54 = 1;

    Data30 = Data2C;
    ++Data30;
    //seg005:14CE
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data30->field_0 = pSprite->field_0;
    Data30->field_2 = pSprite->field_2;
    //seg005:14F4
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;
    Data30->field_4 = pSprite->field_4;
    Data30->field_6 = pSprite->field_6;
    Data2C->field_8 = 0x7D;
    Data30->field_8 = 0x7E;
    Data2C->field_A = 0;
    Data30->field_A = 0;
    Data2C->field_12 = 0x10;
    Data0 = tool_RandomGet();
    Data0 &= 0x0F;
    Data2C->field_12 = Data0;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;
    DataC += 0x06;
    //seg005:158C
    Data2C->field_26 = Data8;
    Data2C->field_28 = DataC;
    Data0 = 0;
    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 < 0x14)
        Data0 = 0x14;

    Data0 /= 5;

    if (Data0 > 0x64)
        Data0 = 0x64;

    //loc_20CEA
    Data2C->field_12 = Data0;
    Data2C->field_36 = 0x32;
    Data30->field_36 = 0x32;
    Data2C->field_1E = 0x0000;
    Data2C->field_20 = 0x7;
    Data30->field_1E = 0;
    Data30->field_20 = 0;
    Data2C->field_4 += 1;
    Data2C->field_0 += 3;
    int32 Dataa0 = Data2C->field_12 << 16;

    Dataa0 >>= 1;
    if (Dataa0 > 0x0E0000)
        Dataa0 = 0x0E0000;

    Data2C->field_1A = Dataa0;
    Data2C->field_18 = eSprite_Grenade;
    Data30->field_18 = eSprite_ShadowSmall;
    Data2C->field_52 = 0;
    Data30->field_52 = 0;
    Data2C->field_50 = 0;
    Data2C->field_22 = pSprite->field_22;
    Data30->field_22 = pSprite->field_22;
    Data2C->field_32 = pSprite->field_32;
    Data2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    Data2C->field_38 = eSprite_Anim_None;
    if (pSprite->field_18 == eSprite_Enemy)
        Data2C->field_12 += 0x1C;
    Data2C->field_56 = 4;
    Data2C->field_46_sprite = pSprite;

    if (pSprite == mSquad_Leader)
        mMouse_Button_LeftRight_Toggle = false;

    return 0;
}

void cFodder::Sprite_Set_Direction_To_Next(sSprite* pSprite) {
    int16 Data0 = pSprite->field_10;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_3C = Data0;

    mDirectionMod = 0;
}

int16 cFodder::Sprite_Reached_Target(sSprite* pSprite) {

    // Sprite is moving?
    if (pSprite->field_36 <= 8)
        return 0;

    int16 Data0 = pSprite->field_26;
    int16 Data4 = pSprite->field_28;

    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
    if (Data0 > 2)
        return 0;

    pSprite->field_0 = pSprite->field_26;
    pSprite->field_4 = pSprite->field_28;

    mSprite_Reached_Target = -1;
    return -1;
}

void cFodder::Sprite_Movement_Speed_Update(sSprite* pSprite) {
    int16 Data0 = pSprite->field_10;

    if (pSprite->field_60 != eTerrainFeature_D)
        goto loc_20FBB;

    //seg005:1843

    if (Data0 < 0xD0)
        goto loc_20F63;

    if (Data0 < 0x130)
        goto loc_20FD1;

    if (Data0 <= 0x170)
        goto loc_20FD9;

    if (Data0 <= 0x190)
        return;

    if (Data0 <= 0x1C0)
        goto loc_20F9B;

    goto loc_20F7B;

loc_20F63:;
    if (Data0 >= 0x90)
        goto loc_20FD9;

    if (Data0 >= 0x70)
        return;

    if (Data0 >= 0x40)
        goto loc_20F9B;

loc_20F7B:;
    Data0 = pSprite->field_36;
    Data0 >>= 2;
    pSprite->field_36 += Data0;
    return;

loc_20F9B:;
    Data0 = pSprite->field_36;
    Data0 >>= 3;
    pSprite->field_36 += Data0;
    return;

loc_20FBB:;
    if (pSprite->field_60 == eTerrainFeature_Rocky)
        goto loc_20FD1;

    if (pSprite->field_60 != eTerrainFeature_Rocky2)
        goto loc_20FE3;

loc_20FD1:;
    pSprite->field_36 >>= 1;

loc_20FD9:;
    pSprite->field_36 >>= 1;
    return;
loc_20FE3:;
    if (pSprite->field_60 != eTerrainFeature_Snow)
        return;

    Data0 = pSprite->field_0;
    Data0 += pSprite->field_4;
    Data0 &= 3;
    if (Data0 == 0)
        return;
    Data0 = pSprite->field_36;
    Data0 >>= 2;
    pSprite->field_36 -= Data0;
}

void cFodder::Sprite_Handle_Player_Close_To_SquadMember(sSprite* pSprite) {

    // Not Human?
    if (pSprite->field_22 != eSprite_PersonType_Human) {
        pSprite->field_4D = 0;
        return;
    }

    word_3B2F3 = 0;

    // Find this sprite in the squad
    sSprite** PreviousMember = mSquads[pSprite->field_32];

    // Loop until we find this squad member
    for (;;) {
        if (*PreviousMember == INVALID_SPRITE_PTR)
            return;

        sSprite* eax = *PreviousMember;
        ++PreviousMember;

        // Found this troop?
        if (pSprite == eax)
            break;
    }

    // Get the previous squad member
    --PreviousMember;

    // If PreviousMember is Squad Leader, return
    if (mSquads[pSprite->field_32] == PreviousMember)
        return;

    // Get the previous members, previous member
    PreviousMember--;

    //seg005:1A13
    sSprite* Data24 = *PreviousMember;

    // Near the other member?
    if (pSprite->field_0 + 8 < Data24->field_0)
        return;

    if (Data24->field_0 + 8 < pSprite->field_0)
        return;

    if (Data24->field_4 + 2 < pSprite->field_4 - 2)
        return;

    if (pSprite->field_4 + 2 < Data24->field_4 - 2)
        return;

    // In Vehicle?
    if (Data24->field_6E)
        return;

    // Different Walk Target?
    int16 Data1C = pSprite->field_40;
    if (Data1C != Data24->field_40)
        return;

    // Restore the previous XY
    Sprite_XY_Restore(pSprite);
    mSprite_Bumped_Into_SquadMember = -1;

    pSprite->field_43 = 1;
    word_3B2F3 = -1;
}

int16 cFodder::Sprite_Get_Free_Max42(int16& pData0, sSprite*& pData2C, sSprite*& pData30) {

    // 
    if (!mSprite_SpareUsed) {

        // Looking for two sprites?
        if (pData0 == 3) {
            pData2C = mSprites.data();

            // Loop all sprites
             for (int32_t Data1C = mParams->getSpritesMax() - 5; Data1C >= 0; --Data1C, ++pData2C) {

                // Sprite free?
                if (pData2C->field_0 != -32768)
                    continue;

                if ((pData2C + 1)->field_0 != -32768)
                    continue;

                // Second sprite free?
                if ((pData2C + 2)->field_0 == -32768) {
                    pData30 = pData2C + 1;

                    pData2C->Clear();
					pData30->Clear();
					(pData30 + 1)->Clear();	// Yuck

                    pData0 = 0;
                    return 0;
                }
            }
        } else if (pData0 == 2) {
            pData2C = mSprites.data();

            // Loop all sprites
             for (int32_t Data1C = mParams->getSpritesMax() - 4; Data1C >= 0; --Data1C, ++pData2C) {

                // Sprite free?
                if (pData2C->field_0 != -32768)
                    continue;

                // Second sprite free?
                if ((pData2C + 1)->field_0 == -32768) {
                    pData30 = pData2C + 1;

                    pData2C->Clear();
					pData30->Clear();
                    pData0 = 0;
                    return 0;
                }
            }
        }
        else {
            // Only looking for 1 sprite
            pData2C = &mSprites[mParams->getSpritesMax() - 3];

            for (int32 Data1C = mParams->getSpritesMax() - 3; Data1C >= 0; --Data1C) {

                // Free?
                if (pData2C->field_0 == -32768) {
                    pData2C->Clear();
                    pData0 = 0;
                    return 0;
                }

                --pData2C;
            }
        }
    }

    //loc_211F0
    pData2C = &mSprite_Spare;
    pData30 = &mSprite_Spare;
    pData0 = -1;
    mSprite_SpareUsed = pData0;
    return -1;
}

int16 cFodder::Sprite_Get_Free_Max29(int16& pData0, sSprite*& pData2C, sSprite*& pData30) {
    if (mSprite_SpareUsed2)
        goto loc_21B4B;

    if (pData0 == 2)
        goto loc_21B91;

    pData2C = &mSprites[mParams->getSpritesMax() - 16];
     for (int32_t Data1C = mParams->getSpritesMax() - 16; Data1C >= 0; --Data1C, --pData2C) {

        if (pData2C->field_0 == -32768) {
            pData2C->Clear();
            pData0 = 0;
            return 0;
        }
    }

loc_21B4B:;
    pData2C = &mSprite_Spare;
    pData30 = &mSprite_Spare;
    pData0 = -1;
    mSprite_SpareUsed2 = pData0;
    return -1;

loc_21B91:;
    pData2C = mSprites.data();

     for (int32_t Data1C = mParams->getSpritesMax() - 17; Data1C >= 0; --Data1C, ++pData2C) {

        if (pData2C->field_0 != -32768)
            continue;

        if ((pData2C + 1)->field_0 == -32768) {
            pData30 = pData2C + 1;
            pData0 = 0;
        }
    }

    pData2C = &mSprite_Spare;
    pData30 = &mSprite_Spare;
    pData0 = -1;
    mSprite_SpareUsed2 = pData0;
    return -1;
}

void cFodder::Sprite_Handle_Exploidable(sSprite* pSprite) {
    int64 Dat0;
    int32 Dat4;
    int16 Data0, Data4;

    if (pSprite->field_5B)
        goto loc_2132A;

    if (pSprite->field_20)
        goto loc_2132A;

    if (pSprite->field_38 == eSprite_Anim_Die1)
        goto loc_213F7;

    if (pSprite->field_38 > eSprite_Anim_Die1)
        goto loc_2132A;

    pSprite->field_5B = 0;
    return;

loc_2132A:;

    Sprite_Movement_Calculate(pSprite);

    Dat0 = (int64)pSprite->field_1A;
    Dat4 = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);

    Dat0 -= 0x28000;
    Dat4 += (int32)Dat0;

    if (Dat4 < 0) {
        Dat4 = 0;
        Dat0 = -Dat0;
        Dat0 >>= 2;
    }

    pSprite->field_1A = (int32)Dat0;
    pSprite->field_1E_Big = Dat4;

    if (!(Dat4 >> 16)) {
        if (!(Dat0 >> 16))
            goto loc_21464;
    }

    Data0 = -3;
    Data4 = 0x0C;

    Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4);

    if (pSprite->field_20)
        return;

    if (Data4 == eTerrainFeature_WaterEdge)
        goto loc_21464;

    if (Data4 == eTerrainFeature_Water)
        goto loc_21464;

    if (Data4 == eTerrainFeature_Drop)
        goto loc_21464;

    if (Data4 == eTerrainFeature_Drop2)
        goto loc_21464;

    return;

loc_213F7:;

    if (mPhase_Completed_Timer)
        return;

    pSprite->field_38 = eSprite_Anim_Die2;
    pSprite->field_5B = -1;
    pSprite->field_1E = 0x0000;
    pSprite->field_20 = 0x1;

    Data0 = tool_RandomGet() & 0x1FE;
    pSprite->field_10 = Data0;

    Data0 = tool_RandomGet() & 0x1F;
    Data0 += 0x14;

    pSprite->field_36 = Data0;
    pSprite->field_1A = 0x1C0000;
    Sprite_Create_Shadow(pSprite);
    return;

loc_21464:;
    pSprite->field_18 = eSprite_Explosion;
    pSprite->field_26 = 0x5F5F;
    pSprite->field_28 = 8;
}

void cFodder::Sprite_Create_Shadow(sSprite* pSprite) {
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data2C->field_1A_sprite = pSprite;

    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;

    Data2C->field_0 += 0x0E;
    Data2C->field_4 += 0x10;
    Data2C->field_8 = 0x8D;
    Data2C->field_A = 0;
    Data2C->field_18 = eSprite_Shadow;
    Data2C->field_20 = 0;
    Data2C->field_32 = -1;
    Data2C->field_2C = eSprite_Draw_First;
    Data2C->field_52 = 0;
}

void cFodder::Sprite_Handle_Grenade_Terrain_Check(sSprite* pSprite) {
    int16 Data0 = -3;
    int16 Data4 = 2;

    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_21599;

    if (Data4 == eTerrainFeature_Drop || Data4 == eTerrainFeature_Drop2) {

        if (pSprite->field_20 <= 1) {
            pSprite->field_12 = 1;
            return;
        }
    }
    //loc_21561
    if (Data4 == eTerrainFeature_QuickSand || Data4 == eTerrainFeature_WaterEdge
        || Data4 == eTerrainFeature_Water) {

        if (pSprite->field_20)
            return;

        pSprite->field_52 = 6;
        (pSprite + 1)->field_52 = 6;
        return;
    }
    //loc_21597
    return;

loc_21599:;
    if (pSprite->field_20 >= 8)
        return;

    pSprite->field_0 = mStoredSpriteX >> 16;
    Data0 = -3;
    Data4 = 2;

    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4)) {
        Data0 = 0x100;
        Data0 -= pSprite->field_10;
        Data0 &= 0x1FE;
        pSprite->field_10 = Data0;
    }
    else {
        Data0 = 0x200;
        Data0 -= pSprite->field_10;
        Data0 &= 0x1FE;
        pSprite->field_10 = Data0;
    }
    Sprite_XY_Restore(pSprite);
}

int16 cFodder::Sprite_Projectile_Collision_Check(sSprite* pSprite) {

    // If the projectile doesn't belong to a human, check if it hit a squad member
    if (pSprite->field_22 != eSprite_PersonType_Human) {

        int16 Data8 = pSprite->field_0;
        int16 DataC = pSprite->field_0 + 6;
        int16 Data10 = pSprite->field_4 - 9;
        int16 Data14 = pSprite->field_4 - 4;
        word_3AA45 = 1;

        return Squad_Member_Sprite_Hit_In_Region(pSprite, Data8, DataC, Data10, Data14);
    }

    //loc_21673
    int16 Data8 = pSprite->field_0 - 6;
    int16 DataC = pSprite->field_0 + 10;
    int16 Data10 = pSprite->field_4 - 10;
    int16 Data14 = pSprite->field_4 + 6;

    if (mVersionCurrent->isAmigaTheOne()) {
        Data8 = pSprite->field_0;
        DataC = pSprite->field_0 + 4;
        Data10 = pSprite->field_4 - 4;
        Data14 = pSprite->field_4;
    }

    word_3AA45 = 1;
    sSprite* Data24 = 0;
    if (Sprite_Find_In_Region(pSprite, Data24, Data8, DataC, Data10, Data14) >= 0)
        return 0;

    if (Data24->field_18 == eSprite_Enemy) {

		Data24->field_5E_Squad = pSprite->field_5E_Squad;
        Data24->field_5E = pSprite->field_5E;
        Data24->field_5D = pSprite->field_5D;
    }

    return -1;
}

void cFodder::Sprite_Create_Sparks(sSprite* pSprite, int16 pData18) {
    int16 Data0 = 2;
    sSprite* Data2C, *Data30;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data30 = Data2C + 1;
    Data2C->field_0 = pSprite->field_0 + 2;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4 - 4;
    Data2C->field_6 = pSprite->field_6;

    Data30->field_0 = Data2C->field_0;
    Data30->field_4 = Data2C->field_4;

    Data2C->field_1E_Big = pSprite->field_1E_Big;

    Data30->field_20 = 0;
    Data2C->field_8 = 0xC5;
    Data30->field_8 = 0x7E;
    Data2C->field_A = 0;
    Data30->field_A = 0;
    Data2C->field_36 = 0x0C;
    Data30->field_36 = 0x0C;
    Data2C->field_18 = eSprite_Sparks;
    Data30->field_18 = eSprite_ShadowSmall;
    Data2C->field_52 = 0;
    Data30->field_52 = 0;
    Data2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    Data2C->field_38 = eSprite_Anim_None;

    Data2C->field_10 = pData18;
    Data2C->field_1A = 0x0A8000;

}

void cFodder::Sprite_Create_FireTrail(sSprite* pSprite) {

    if (!pSprite->field_5C)
        return;

    //  cmp     ds:word_3B1A9, 14h
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return;

    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;

    if (dword_3B24B == 0) {
        Data2C->field_4 -= 3;
    }
    else {
        Data2C->field_4 += 2;
        dword_3B24B = 0;
    }

    Data2C->field_20 = pSprite->field_20;
    Data2C->field_52 = 0;
    Data2C->field_8 = 0xC6;
    Data2C->field_A = 0;
    Data2C->field_18 = eSprite_FireTrail;
}

void cFodder::Sprite_Turn_Into_Building_Explosion(sSprite* pSprite) {
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;

    pSprite->Clear();
    Sprite_Create_Building_Explosion(pSprite, Data8, DataC);
}

int16 cFodder::Sprite_Create_Building_Explosion_Wrapper(int16& pX, int16& pY) {
    int16 Data0 = 1;
    sSprite* Data2C, *Data30;

    if (Sprite_Get_Free_Max42(Data0, Data2C, Data30))
        return -1;

    Data2C->Clear();

    pX &= -16;
    pY &= -16;

    return Sprite_Create_Building_Explosion(Data2C, pX, pY);
}

int16 cFodder::Sprite_Create_Building_Explosion(sSprite* pData2C, int16& pX, int16& pY) {

    pData2C->field_0 = pX;
    pData2C->field_4 = pY;
    pData2C->field_4 += 0x10;

    int16 Data4 = mGame_InputTicks;
    Data4 &= 3;
    Data4 += 5;

    Sound_Play(pData2C, Data4, 0x1E);

    pData2C->field_18 = eSprite_Building_Explosion;
    pData2C->field_8 = 0x8E;
    pData2C->field_A = 0;
    pData2C->field_12 = 1;
    pData2C->field_22 = eSprite_PersonType_None;
    pData2C->field_32 = -1;
    pData2C->field_2C = eSprite_Draw_OnTop;

    return 0;
}

int16 cFodder::Sprite_Create_Enemy(sSprite* pSprite, sSprite*& pData2C) {

    if (mPhase_Complete || mTroops_Enemy_Count >= mParams->mSpawnEnemyMax)
        return -1;
    int16 Data0 = 1;
    sSprite* Data30 = 0;

    if (Sprite_Get_Free_Max29(Data0, pData2C, Data30))
        return -1;

    //loc_21A1C:;
    pData2C->Clear();
    pData2C->field_18 = eSprite_Enemy;
    pData2C->field_0 = pSprite->field_0;
    pData2C->field_0 -= 6;
    pData2C->field_4 = pSprite->field_4;
    pData2C->field_4 += 4;
    pData2C->field_8 = 0x7C;
    pData2C->field_4A = 0;
    pData2C->field_22 = eSprite_PersonType_AI;

    Data0 = tool_RandomGet() & 0xFF;
    Data0 += 0x78;
    Data0 = -Data0;
    pData2C->field_5E = Data0;
    Data0 = tool_RandomGet();
    int16 Data4 = Data0;
    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pData2C->field_10 = Data0;
    Data4 &= 0x0F;
    Data4 += 8;
    pData2C->field_44 = (int8)Data4;
    Sprite_Enemy_Aggression_Update(pData2C);

    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionCreated_Count += 1;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionCreated_Count &= 0x0F;
    if (!mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionCreated_Count && mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax < 0x1E)
        ++mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;

    ++mTroops_Enemy_Count;
    return 0;
}

void cFodder::Sprite_Enemy_Aggression_Update(sSprite* pData2C) {
    int16 Data8 = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext;
    pData2C->field_62 = Data8;

    int16 Data4 = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement;
    Data8 += Data4;

    if (Data4 < 0)
        goto loc_21C42;

    if (Data8 < mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax)
        goto loc_21C5E;

    Data8 = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax;
    Data4 = -Data4;
    goto loc_21C5E;

loc_21C42:;
    if (Data8 < 0)
        goto loc_21C52;

    if (Data8 > mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin)
        goto loc_21C5E;

loc_21C52:;
    Data8 = mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin;
    Data4 = -Data4;

loc_21C5E:;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement = Data4;
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext = Data8;
}

void cFodder::Sprite_Create_Rank() {
    sSprite* Sprite = 0, *Data30 = 0;
    int16 Data0 = 1;

    if (Sprite_Get_Free_Max42(Data0, Sprite, Data30))
        return;

    Sprite->field_0 = 0;
    Sprite->field_4 = 0;
    Sprite->field_8 = 0x7C;
    Sprite->field_A = 0;
    Sprite->field_10 = 1;
    Sprite->field_18 = eSprite_Player_Rank;
    Sprite->field_22 = eSprite_PersonType_None;
    Sprite->field_3A = 0;
}

void cFodder::sub_21CD1(sSprite* pSprite) {
    sSprite* Data30 = 0;

    mTroop_Weapon_Bullet_Disabled = false;
    mTroop_Weapon_Grenade_Disabled = true;
    mTroop_Weapon_Rocket_Disabled = true;
    mDirectionMod = 0;
    mSprite_FaceWeaponTarget = -1;

    int16 Dataa0, Data4, Data8, DataC, Data10;

    sSprite* Data0, *Following = 0, *Squad0_Member = 0;

    if (pSprite->field_5E_Squad < 0) {

        pSprite->field_5E_Squad += 1;
        mSprite_FaceWeaponTarget = 0;

        Data0 = mSquad_Leader;
        if (mSquad_Leader == INVALID_SPRITE_PTR)
            goto loc_22125;

    }
    else {

        Squad0_Member = mSquad_0_Sprites[pSprite->field_5E_Squad];
        if (Squad0_Member == INVALID_SPRITE_PTR)
            goto loc_21E4A;

        if ((tool_RandomGet() & 0x3F) == 0)
            goto loc_21E4A;

        if (Squad0_Member->field_0 == -32768)
            goto loc_21E4A;

        if (Squad0_Member->field_18 != eSprite_Player)
            goto loc_21E4A;

        if (Squad0_Member->field_38 < eSprite_Anim_Slide1) {
            if (Squad0_Member->field_38)
                goto loc_21E4A;
        }

        if (Squad0_Member->field_20 >= 0x0D)
            goto loc_21E4A;

        Dataa0 = pSprite->field_0;
        Data4 = pSprite->field_4;

        Data8 = Squad0_Member->field_0;
        DataC = Squad0_Member->field_4;

        Map_Get_Distance_BetweenPoints_Within_Window(Dataa0, Data4, Data8, DataC);

        mSprite_Find_Distance = Dataa0;
        if (Dataa0 > 0xC8)
            goto loc_21E4A;

        if (Dataa0 <= 0x28)
            goto loc_21F77;

        Dataa0 = pSprite->field_0;
        Data4 = pSprite->field_4;
        Data8 = Squad0_Member->field_0;
        DataC = Squad0_Member->field_4;

        if (!Map_PathCheck_CalculateTo(Dataa0, Data4, Data8, DataC)) {
            Dataa0 = mSprite_Find_Distance;
            goto loc_21F77;
        }

        if (mSprite_Find_Distance < 0x40)
            goto loc_21F8A;

    loc_21E4A:;

        // Following a Sprite?
        if (pSprite->field_70) {

            Following = pSprite->field_70;

            if (Following->field_0 != -32768) {

                // Following a hostage?
                if (Following->field_18 == eSprite_Hostage) {

                    pSprite->field_5E_Squad += 1;
                    if (pSprite->field_5E_Squad >= 0x1E)
                        pSprite->field_5E_Squad = 0;

                    mSprite_FaceWeaponTarget = 0;
                    pSprite->field_26 = Following->field_0 + 0x0C;
                    pSprite->field_28 = Following->field_4;
                    return;
                }
            }

            pSprite->field_70 = 0;
        }

        if (!(tool_RandomGet() & 0x0F)) {
            Dataa0 = tool_RandomGet() & 0xFF;
            Dataa0 = (int16)Dataa0;
            pSprite->field_2E += Dataa0;

            Dataa0 = tool_RandomGet() & 0xFF;
            pSprite->field_30 += Dataa0;
        }

        pSprite->field_26 = pSprite->field_0;
        pSprite->field_28 = pSprite->field_4;
        pSprite->field_4A = 0;

        pSprite->field_5E_Squad += 1;
        if (pSprite->field_5E_Squad > 0x1E)
            pSprite->field_5E_Squad = 0;

        goto loc_22000;

    loc_21F77:;
        pSprite->field_70 = 0;
        mSprite_FaceWeaponTarget = 0;

    loc_21F8A:;
        if (Dataa0 > 0x87)
            goto loc_22000;

        //seg005:28A2
        // This is a strange segment, I can't reliably tell where data30 comes from,
        // in atleast one situation, its possible its not even pointing at an sSprite structure
        // or other times it might be pointing towards a completely irrelevant sprite from a previous use case.
        // i set it to spare, because that *could* happen if we are out of sprites and a previous sprite create had
        // failed... 
        Data30 = &mSprite_Spare;

        // Logic wise, it doesnt really make sense either... which makes me think perhaps Data30 should be Squad0_Member


        if (Data30->field_0 != Data30->field_26)
            goto loc_22000;

        if (Data30->field_4 != Data30->field_28)
            goto loc_22000;

        pSprite->field_2E = Squad0_Member->field_0;
        pSprite->field_30 += 7;                         // TODO: Is this an original bug and we should be adding to field_2E, code/logic wise it makes sense if it is

        pSprite->field_30 = Squad0_Member->field_4;
        pSprite->field_30 -= 0x0E;
        sub_2212A(pSprite);


    loc_22000:;
        if (mSprite_FaceWeaponTarget)
            goto loc_22125;

        if (mSquad_0_Sprites[pSprite->field_5E_Squad] == INVALID_SPRITE_PTR)
            goto loc_22125;

        Data0 = mSquad_0_Sprites[pSprite->field_5E_Squad];
    }

    // "Sort of" Random Movement Target
    // Depending on the sprite index, we add a factor to the X/Y target of a movement
    Data8 = (int16)(pSprite - mSprites.data());
    Data8 *= 0x76;
    Data8 &= 0x1FE;

    DataC = mMap_Direction_Calculations[Data8 / 2];
    Data8 += 0x80;
    Data8 &= 0x1FE;
    Data10 = mMap_Direction_Calculations[Data8 / 2];

    //seg005:29B2
    DataC >>= 8;
    Data10 >>= 8;
    DataC >>= 2;
    Data10 >>= 2;

    pSprite->field_2E = Data0->field_0;
    pSprite->field_26 = Data0->field_0 + DataC;      // Target X

    pSprite->field_30 = Data0->field_4;
    pSprite->field_28 = Data0->field_4 + Data10;      // Target Y

loc_22125:;
    sub_2212A(pSprite);
}

void cFodder::sub_2212A(sSprite* pSprite) {

    if (pSprite->field_4A)
        return;

    int16 Data0 = tool_RandomGet();

    int16 Data8 = pSprite->field_62;
    int16 DataC = Data8;
    DataC <<= 2;
    Data8 >>= 2;
    int16 Data4 = 0x1F;
    Data4 >>= Data8;
    Data0 &= Data4;
    Data4 = 0x32;
    Data4 -= DataC;
    if (Data4 < 0)
        Data4 = 0;

    Data0 += Data4;
    if (Data0 < 5)
        Data0 = 5;

    pSprite->field_4A = Data0;
}

int16 cFodder::Sprite_Handle_BuildingDoor_Explode(sSprite* pSprite) {
    int16 Data0, Data4;

    if (pSprite->field_38 == eSprite_Anim_Die3 || pSprite->field_38 == eSprite_Anim_Die1)
        goto loc_221D0;

    if (pSprite->field_38 == eSprite_Anim_Die2)
        goto loc_22235;

    return 0;
loc_221D0:; // Building Explodes
    pSprite->field_38 = eSprite_Anim_Die2;
    pSprite->field_8 = 0x99;

    Data0 = tool_RandomGet();
    Data4 = Data0;

    Data0 &= 0x1E;
    if (Data4 < 0)
        Data0 = -Data0;

    Data0 += 0x1C0;
    pSprite->field_10 = Data0;
    Data4 &= 3;
    Data4 += 5;
    pSprite->field_44 = (int8)Data4;
    pSprite->field_36 = 0x78;

loc_22235:; // Door moving
    pSprite->field_44 -= 1;
    if (!pSprite->field_44)
        goto loc_2227F;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_2227F;

    Data0 = -3;
    Data4 = 2;
    if (Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        goto loc_2227F;

    if (pSprite->field_36 <= 0)
        goto loc_2227F;

    return -1;

loc_2227F:; // Door Finished
    pSprite->field_0 -= 8;
    pSprite->field_4 -= 8;

    Sprite_Turn_Into_Building_Explosion(pSprite);

    return -1;
}

int16 cFodder::sub_222A3(sSprite* pSprite) {
    int16 Data0, Data4;

    if (pSprite->field_38 != eSprite_Anim_Die3 && pSprite->field_38 != eSprite_Anim_Die1) {

        if (pSprite->field_38 == eSprite_Anim_Die2)
            goto loc_22339;

        return 0;
    }

    //loc_222CD
    pSprite->field_38 = eSprite_Anim_Die2;
    pSprite->field_8 = 0x9B;

    Data0 = tool_RandomGet();
    Data4 = Data0;
    Data0 &= 0x1E;

    if (Data4 < 0)
        Data0 = -Data0;

    if (mMapLoaded->getTileType() == eTileTypes_Ice || mMapLoaded->getTileType() == eTileTypes_AFX)
        Data0 += 0x1C0;

    pSprite->field_10 = Data0;
    Data4 &= 3;
    Data4 += 5;

    pSprite->field_44 = (int8)Data4;
    pSprite->field_36 = 0x6E;

loc_22339:;
    pSprite->field_44 -= 1;
    if (pSprite->field_44 == 1)
        goto loc_22383;

    Sprite_Movement_Calculate(pSprite);
    if (mSprite_Bullet_Destroy)
        goto loc_22383;

    Data0 = -3;
    Data4 = 2;
    if (!Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
        if (pSprite->field_36 > 0)
            return -1;

loc_22383:;

    pSprite->field_0 -= 8;
    if (pSprite->field_0 < 0)
        pSprite->field_0 = 0;

    pSprite->field_4 -= 8;
    Sprite_Turn_Into_Building_Explosion(pSprite);
    return -1;
}

void cFodder::sub_223B2(sSprite* pSprite) {
    int16 Data8 = pSprite->field_3C;

    Data8 += mDirectionMod;
    Data8 &= 0x0E;

    pSprite->field_3C = Data8;

    const int16* Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    Data28 += ((0x80 + Data8) / 2);

    pSprite->field_8 = *Data28;
    pSprite->field_3E = pSprite->field_10;
    pSprite->field_A = 0;
}

/**
 * Check the distance between the provided sprite, and the squad leader
 *
 * @param pSprite
 * @param pData0
 *
 * @return 0 if within range, 1 if not
 */
int16 cFodder::Map_Get_Distance_Between_Sprite_And_Squadleader(sSprite* pSprite, int16& pData0) {
    int16 Data4, Data8, DataC, Data10;

    // Current Squad in vehicle?
    if (mSquad_CurrentVehicle)
        return 1;

    // No Squad Leader?
    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return 1;

    // Vehicle?
    if (mSquad_Leader->field_6E)
        return 1;

    // Anim playing?
    if (mSquad_Leader->field_38)
        return 1;

    pData0 = pSprite->field_0;

    Data4 = pSprite->field_4;
    Data8 = mSquad_Leader->field_0;
    DataC = mSquad_Leader->field_4;
    Data10 = 0x20;

    DataC += 2;

    Map_Get_Distance_BetweenPoints(pData0, Data4, Data8, Data10, DataC);
    if (pData0 >= 6)
        return 1;

    Data4 = 0;
    return 0;
}

int16 cFodder::Sprite_Create_Rocket(sSprite* pSprite) {
    sSprite* Data2C = 0, *Data30 = 0, *Data34 = 0;
    int16 Data0, Data4, Data8, DataC;

    if (mPhase_Completed_Timer) {
    loc_22592:;

        if (pSprite == mSquad_Leader)
            mMouse_Button_LeftRight_Toggle = false;

        return -1;
    }

    if (mSprite_Missile_Projectile_Counters[pSprite->field_22] == 2)
        goto loc_22592;

    if (!pSprite->field_2E)
        goto loc_22592;

    //seg005:2E39
    if (pSprite->field_18 == eSprite_Enemy) {
        Data0 = pSprite->field_0;
        Data4 = pSprite->field_4;
        Data8 = pSprite->field_2E;
        DataC = pSprite->field_30;

        // Don't let the AI fire further than 129
        Map_Get_Distance_BetweenPoints_Within_Window(Data0, Data4, Data8, DataC);
        if (Data0 >= 0x82)
            goto loc_22592;
    }

    Data0 = 2;
    Sprite_Get_Free_Max42(Data0, Data2C, Data30);

    // No Free sprites?
    if (Data0)
        return -1;

    if (pSprite->field_18 == eSprite_Player) {
        Data0 = pSprite->field_32;
        Data4 = Data0;

        if (!mSquad_Rockets[Data0])
            goto loc_22592;

        --mSquad_Rockets[Data0];
        mGUI_RefreshSquadRockets[Data4] = -1;
    }

    ++mSprite_Missile_Projectile_Counters[pSprite->field_22];
    pSprite->field_55 = 1;
    pSprite->field_5A = -1;
    pSprite->field_54 = 3;
    Data30 = Data2C;
    Data30++;

    //seg005:2F86 
    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data30->field_0 = pSprite->field_0;
    Data30->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;
    Data30->field_4 = pSprite->field_4;
    Data30->field_6 = pSprite->field_6;

    //seg005:2FD2
    Data2C->field_26 = pSprite->field_2E;
    Data2C->field_28 = pSprite->field_30;
    Data2C->field_28 += 0x10;
    Data2C->field_8 = 0x7C;
    Data30->field_8 = 0x7E;
    Data2C->field_10 = 0;
    Data30->field_10 = 1;
    Data2C->field_36 = 0x64;
    Data30->field_36 = 0x64;
    Data2C->field_1E = 0x0000;
    Data2C->field_20 = 0x0A;

    Data30->field_1E = 0;
    Data30->field_20 = 0;

    Data2C->field_4++;
    Data2C->field_0 += 3;

    // Amiga Power always has homing missiles
    if (pSprite->field_22 == eSprite_PersonType_Human &&
        (mVersionCurrent->isCoverDisk() || (pSprite->field_75 & eSprite_Flag_HomingMissiles))) {

        // Within lock on range?
        if (Sprite_Homing_LockInRange(pSprite, Data34)) {

            Data2C->field_1A_sprite = Data34;
            Data2C->field_18 = eSprite_MissileHoming2;
            Data2C->field_6A = 0x10000;
            Data2C->field_26 = pSprite->field_2E;
            Data2C->field_28 = pSprite->field_30;
            Data2C->field_28 += 0x10;
            Data2C->field_38 = eSprite_Anim_None;
            Data2C->field_36 = 0;
            Data30->field_36 = 0;

        }
        else {
            Data2C->field_32 = 0;
            Data2C->field_18 = eSprite_Missile;
        }

    }
    else {

        Data2C->field_18 = eSprite_Rocket;
        Data2C->field_46_sprite = pSprite;
    }

    Data30->field_18 = eSprite_ShadowSmall;
    Data2C->field_52 = 0;
    Data30->field_32 = 0;
    Data2C->field_22 = pSprite->field_22;
    Data30->field_22 = Data2C->field_22;
    Data2C->field_32 = pSprite->field_32;
    Data2C->field_2C = eSprite_Draw_Second;
    Data30->field_2C = eSprite_Draw_First;
    Data2C->field_56 = 6;

    if (pSprite == mSquad_Leader)
        mMouse_Button_LeftRight_Toggle = false;

    return 0;
}

int16 cFodder::Sprite_Homing_LockInRange(sSprite* pSprite, sSprite*& pFoundSprite) {

    int16 MouseX = mMouseX;
    int16 MouseY = mMouseY;

    MouseX += mCameraX >> 16;
    MouseY += mCameraY >> 16;

    MouseX -= 0x10;
    if (!MouseX)
        MouseX = 1;

    MouseY += 0x08;

	for( auto& Sprite : mSprites ) {
		pFoundSprite = &Sprite;

        if (pFoundSprite->field_0 == -32768)
            continue;

        if (!mSprite_Missile_CanLock[pFoundSprite->field_18])
            continue;

        if (pFoundSprite->field_22 == pSprite->field_22)
            continue;

        int16 Data0 = pFoundSprite->field_0 + 5;
        int16 Data4 = pFoundSprite->field_4 - 5;

        Data4 -= pFoundSprite->field_20;
        int16 Distance = 0x1F;
        int16 S_Data8 = MouseX;
        int16 S_DataC = MouseY;

        Map_Get_Distance_BetweenPoints(Data0, Data4, MouseX, Distance, MouseY);

        MouseX = S_Data8;
        MouseY = S_DataC;

        if (Data0 < 0x16) {
            Data0 = -1;
            return -1;
        }
    }

    return 0;
}

void cFodder::Sprite_Projectile_HitTarget(sSprite* pSprite) {

    mSprite_Missile_Projectile_Counters[pSprite->field_22] -= 1;
}

void cFodder::Sprite_Destroy_Wrapper_At_TopLeft(sSprite* pSprite) {
    pSprite->field_0 = 1;
    pSprite->field_4 = 1;

    Sprite_Destroy_Wrapper_2(pSprite);
}

int16 cFodder::Sprite_Destroy_Wrapper_2(sSprite* pSprite) {
    return Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Player_InVehicle(sSprite* pSprite) {
    pSprite->field_8 = 0x7C;
    pSprite->field_A = 0;
    pSprite->field_38 = eSprite_Anim_None;

    sSprite* Data24 = pSprite->field_6A_sprite;
    if (Data24->field_0 == -32768 || Data24->field_18 == eSprite_Explosion) {
        //loc_22AEE
        pSprite->field_6E = 0;
        pSprite->field_66 = 0;
        pSprite->field_6A_sprite = 0;
        pSprite->field_38 = eSprite_Anim_Hit;
        pSprite->field_64 = -1;

        pSprite->field_10 = tool_RandomGet() & 0x1FE;
    }

    //loc_22B3C

    pSprite->field_0 = Data24->field_0;
    pSprite->field_4 = Data24->field_4;
    pSprite->field_20 = Data24->field_20;
}

void cFodder::Game_Setup() {

    if (mParams->mMissionNumber < 1)
        mParams->mMissionNumber = 1;

    GameData_Reset();

    mIntroDone = false;
    mPhase_Complete = false;

    mGame_Data.mMission_Phases_Remaining = 1;
    mGame_Data.mMission_Number = (uint16) (mParams->mMissionNumber);
    mGame_Data.mMission_Phase = (uint16) (mParams->mPhaseNumber ? (mParams->mPhaseNumber) : 1);

    mGame_Data.Phase_Start();

    mPhase_TryAgain = true;
    mGraphics->Load_pStuff();
}

// This function is for viewing/iterating sprites
void cFodder::Playground() {
    //return;
    mGame_Data.Phase_Start();
    Map_Load();

    mGraphics->PaletteSet();

    Recruit_Truck_Anim_Prepare();
    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);
    //mGraphics->Recruit_Draw_Hill();

    Recruit_Copy_Sprites();

    if (mVersionCurrent->mPlatform == ePlatform::Amiga) {

        GetGraphics<cGraphics_Amiga>()->Hill_Prepare_Overlays();
    }

    // Load Icon
    mSurface->clearBuffer();
    mSurface->palette_FadeTowardNew();
    mSurface->Save();
    mString_GapCharID = 0x25;
    mGUI_Print_String_To_Sidebar = false;

    int32 SpriteID = 0x62;
    int32 Frame = 0;


    for (;; ) {
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

        //Mouse_Inputs_Get();
        //Mouse_DrawCursor();

       Video_SurfaceRender();
       Cycle_End();

        // Q
        if (mKeyCode == 0x14) {
            --SpriteID;
        }
        // W
        if (mKeyCode == 0x1A) {
            ++SpriteID;
        }

        // A
        if (mKeyCode == 0x04) {
            --Frame;
        }

        // S
        if (mKeyCode == 0x16) {
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

bool cFodder::Demo_Load() {

    std::ifstream DemoContent(mStartParams->mDemoFile, std::ios::binary);
    if (DemoContent.is_open()) {

        std::string SaveGameContent(
            (std::istreambuf_iterator<char>(DemoContent)),
            (std::istreambuf_iterator<char>())
        );

        mGame_Data.mDemoRecorded.FromJson(SaveGameContent);
        return true;
    }
    return false;
}

void cFodder::Window_UpdateScreenSize() {

	// We can override the window 
	mWindow->SetScreenSize(getWindowSize());
	mWindow->SetOriginalRes(getWindowSize());
	

    // This next section is done
    //
    // If we're playing back a demo
    if (mParams->mDemoPlayback && g_Fodder->mVersionCurrent) {
        // And the current platform, does not match the platform the demo was recorded with
        if (mGame_Data.mDemoRecorded.mRecordedPlatform != mVersionCurrent->mPlatform && mGame_Data.mDemoRecorded.mRecordedPlatform != ePlatform::Any ) {
            // Alter the screen size, to the other platforms
            switch (mGame_Data.mDemoRecorded.mRecordedPlatform) {
            case ePlatform::Amiga: // Viewing Amiga recording on PC Data
                mWindow->SetScreenSize({ 320,225 });
                break;
            case ePlatform::PC: // Viewing PC recording on Amiga Data
                mWindow->SetScreenSize({ 320,200 });
                break;
            case ePlatform::Any:
                break;
            }
        }
    }
}

void cFodder::About() {
    
    mService_Draw_List.clear();
    VersionSwitch(mVersions->GetRetail(mParams->mDefaultPlatform, mParams->mDefaultGame));
    if (!mVersionCurrent)
        VersionSwitch(mVersions->GetDemo());

    cAbout About;
    while (About.Cycle()) {


        mWindow->RenderAt(mSurface);
        mWindow->FrameEnd();
        Cycle_End();
    }

    g_Fodder->mPhase_Aborted = false;
}

void cFodder::CreateRandom(sMapParams pParams) {
	mGame_Data.mCampaign.CreateCustomCampaign();
	mGame_Data.mCampaign.setRandom(true);
	mGame_Data.mCampaign.setName("Random");

	VersionSwitch(mVersions->GetRetail(mParams->mDefaultPlatform, mParams->mDefaultGame));

	if (!mParams->mRandomFilename.size()) {
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

	if (g_ScriptingEngine->Run(mParams->mScriptRun)) {

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

void cFodder::Start() {

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
    VersionSwitch(mVersions->GetRetail( mParams->mDefaultPlatform, mParams->mDefaultGame ));

    if (!mVersionCurrent) {
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

	if (mParams->mPlayground) {
		Playground();
		return;
	}

    // Start a random map?
    if (mParams->mRandom || mParams->mSingleMap.size()) {

        mGame_Data.mCampaign.SetSingleMapCampaign();
        mCustom_Mode = eCustomMode_Map;

		if(mParams->mRandom)
	        VersionSwitch(mVersions->GetForCampaign("Random Map", mParams->mDefaultPlatform));
		else
			VersionSwitch(mVersions->GetForCampaign("Single Map", mParams->mDefaultPlatform));
	} else {

        // Select campaign menu
		if (!(mParams->mCampaignName.size() && Campaign_Load(mParams->mCampaignName))) {
			Campaign_Selection();
			// Exit pushed?
			if (mGUI_SaveLoadAction == 1)
				return;

			if (mGUI_SaveLoadAction == 4) {
				About();
				goto Start;
			}
		}
    }

	mVersionDefault = mVersionCurrent;
    Mouse_Setup();
    Mouse_Inputs_Get();

	if (Engine_Loop())
		goto Start;
}

/**
 * Main Engine Loop
 *
 * false = Exit
 * true  = Restart
 */
bool cFodder::Engine_Loop() {

	for (;;) {

		Game_Setup();

		if (Mission_Loop() == -1)
			return true;

		if (mParams->mSinglePhase)
			break;
	}

	return false;
}

bool cFodder::GameOverCheck() {

	if (!mParams->mUnitTesting) {
		// Mission completed?
		if (!mPhase_Aborted && !mPhase_TryAgain) {

			// Demo / Custom Mission restart
			if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set && !mVersionCurrent->isAmigaTheOne())
				return false;

			// Reached last map in this mission set?
			if (!mGame_Data.Phase_Next()) {

				mGame_Data.mGameWon = true;
				WonGame();
				return true;
			}
		}
	}

	// Double escape aborts out to OF selection, on Amiga the one
	if (mPhase_Aborted2 && mVersionCurrent->isAmigaTheOne()) {
		return true;
	}

	return false;
}

int16 cFodder::Briefing_Show() {

	if (mParams->mSkipBriefing)
		return 1;

	if (mVersionCurrent->isDemo() && mVersionDefault->isDemo())
		return 1;

	// Show the Briefing screen for Retail and Custom 
	if (mVersionCurrent->hasBriefingScreen() || mCustom_Mode != eCustomMode_None || mGame_Data.mCampaign.isRandom()) {

		Window_UpdateScreenSize();

		// Show the pre ready Briefing Screen
		Briefing_Show_PreReady();

		Map_Load();
		Map_Load_Sprites();
		Map_Overview_Prepare();

		// Prepare Squads
		Phase_Soldiers_Count();
		mGame_Data.Soldier_Sort();
		Phase_Soldiers_Prepare(false);
		Phase_Soldiers_AttachToSprites();

		mPhase_Aborted = false;

		// Needs to split into cycle function
		Briefing_Show_Ready();

		// Aborted?
		if (mBriefing_Aborted == -1) {

			if (mGame_Data.mCampaign.isRandom())
				return -1;// Return to version select

			GameData_Restore();

			mRecruit_Mission_Restarting = true;
			mGame_Data.mMission_Recruitment = -1;
			mPhase_Aborted = true;

			if (!mStartParams->mDisableSound)
				mSound->Music_Play(0);

			return 0;
		}
	}

	return 1;
}

int16 cFodder::Mission_Loop() {
  
    for (;;) {
        mGame_Data.mDemoRecorded.save();

		if (GameOverCheck())
			return -1;

        // Prepare the next mission
        Phase_EngineReset();
        Phase_SquadPrepare();
        
        mInput_Enabled = false;

		intro_main();

        // Prepare a new game?
        if (mGame_Data.mMission_Recruitment && !mParams->mSkipRecruit) {
            mGame_Data.mMission_Recruitment = 0;

            switch (Recruit_Show()) {
            case 0:     // Start Mission
                break;

            case -1:    // Return to version select
                return -1;

            case -2:    // Custom set mode
                return -2;

            case -3:    // Load/Save pressed
                continue;
            }
        }

        WindowTitleSet(true);

		switch (Briefing_Show()) {

			case -1:
				return -1;	// Return to version select

			case 0:			// Back to hill 
				continue;

			case 1:			// Continue to phase
				break;
		}

		Phase_Prepare();

        if (!Phase_Loop()) {
            mKeyCode = 0;
            mPhase_In_Progress = false;
            Squad_Member_PhaseCount();
            mPhase_TryingAgain = true;

        }
        else {
            mKeyCode = 0;
            mPhase_In_Progress = false;

            // Game over?
            if (!mGame_Data.mRecruits_Available_Count) {

                // Retail/Custom Set can do the service screen
                if (mVersionCurrent->isRetail() || mCustom_Mode == eCustomMode_Set) {

                    if (!mPhase_Aborted)
                        Service_Show();
                }
                break;
            }
        }

        // Single Phase?
        if (mParams->mSinglePhase) {
            return 0;
        }

        //loc_106F1
        if (mPhase_TryAgain) {
            mPhase_TryingAgain = true;
            continue;
        }

        // Switch back to the default data?
        if (mVersionReturnAfterPhase) {
            mVersionReturnAfterPhase = false;
            VersionSwitch(mVersionDefault);
        }

        // Demo/Single mission, but not set mode (its handled at the end of the loop)
        if (mVersionCurrent->isDemo() && mCustom_Mode != eCustomMode_Set) {

            // Custom can do the service screen, as it requires a retail release 
            if (!mPhase_Aborted && mVersionCurrent->isCustom())
                Service_Show();

            if(!mVersionCurrent->isAmigaTheOne())
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

void cFodder::intro_main() {
	if (!mIntroDone) {
		mImage_Aborted = 0;
		mVersionPlatformSwitchDisabled = true;

		if (!mParams->mSkipIntro) {
			mWindow->SetScreenSize(mVersionCurrent->GetSecondScreenSize());

			// Show the intro for retail releases (and the PC Format demo)
			if (mVersionCurrent->isRetail() || mVersionCurrent->isPCFormat()) {
				intro_Retail();
			}
			else {
				// Amiga The One has an intro too
				if (mVersionCurrent->isAmigaTheOne()) {
					intro_AmigaTheOne();
				}
			}
		}

		mGraphics->Load_pStuff();
		mVersionPlatformSwitchDisabled = false;
		mIntroDone = true;
	}
}

void cFodder::MapTiles_Draw() {

    mMapTile_ColumnOffset = 0;
    mMapTile_RowOffset = 0;

    // 0x60 - SidebarWidth - MapWidth
    mMapTile_Ptr = (0x60 - 8) - (mMapLoaded->getWidth() << 1);
    
    // No sidebar in OFED
#ifdef _OFED
    mMapTile_Ptr += 8;
#endif

    mMapTile_MovedHorizontal = 0;
    mMapTile_MovedVertical = 0;
    mMapTile_Column_CurrentScreen = 0;
    mMapTile_Row_CurrentScreen = 0;

    if (!mStartParams->mDisableVideo)
        mGraphics->MapTiles_Draw();
}

void cFodder::Exit(unsigned int pExitCode) {

    exit(pExitCode);
}

void cFodder::Squad_Split_Assets() {
    int16 Data8, DataC;

    if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_All) {
        mSquad_Grenades[mSquad_Selected] = mSquad_Grenades[mSquad_JoiningTo];
        mSquad_Grenades[mSquad_JoiningTo] = 0;
    }
    else {

        if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_Half) {
            Data8 = mSquad_Grenades[mSquad_JoiningTo];
            DataC = Data8;
            Data8 >>= 1;

            mSquad_Grenades[mSquad_JoiningTo] = Data8;
            DataC -= Data8;
            mSquad_Grenades[mSquad_Selected] = DataC;
        }
    }

    if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_All) {
        mSquad_Rockets[mSquad_Selected] = mSquad_Rockets[mSquad_JoiningTo];
        mSquad_Rockets[mSquad_JoiningTo] = 0;
    }
    else {

        if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_Half) {

            Data8 = mSquad_Rockets[mSquad_JoiningTo];
            DataC = Data8;
            Data8 >>= 1;

            mSquad_Rockets[mSquad_JoiningTo] = Data8;
            DataC -= Data8;
            mSquad_Rockets[mSquad_Selected] = DataC;
        }
    }

    mSquad_CurrentWeapon[mSquad_Selected] = mSquad_CurrentWeapon[mSquad_JoiningTo];
}

void cFodder::Squad_Member_Click_Check() {
    word_3A3BF = -1;

    if (mSquad_Selected < 0)
        return;

    int16 ClickedX = mMouseX;
    int16 ClickedY = mMouseY;

    ClickedX += mCameraX >> 16;
    ClickedY += mCameraY >> 16;
    ClickedX -= 0x0F;
    ClickedY -= 3;

    for (auto& SquadMember : mGame_Data.mSoldiers_Allocated) {

        if (SquadMember.mSprite == INVALID_SPRITE_PTR || SquadMember.mSprite == 0)
            continue;

        sSprite* SquadMemberSprite = SquadMember.mSprite;

        if (SquadMemberSprite->field_32 == mSquad_Selected)
            ++word_3A3BF;

        int16 SquadMemberX = SquadMemberSprite->field_0 + 4;
        int16 SquadMemberY = SquadMemberSprite->field_4 - SquadMemberSprite->field_20;

        // Clicked before/after troop?
        if (ClickedX < SquadMemberX)
            continue;
        if (ClickedX > SquadMemberX + 6)
            continue;

        // Clicked above/below troop?
        if (ClickedY < SquadMemberY - 13)
            continue;
        if (ClickedY > SquadMemberY)
            continue;

        // 
        if (mSquad_Selected == SquadMemberSprite->field_32) {
            mSquad_Member_Clicked_TroopInSameSquad = -1;
            mSquad_Member_Clicked_TroopPtr = SquadMemberSprite->field_46_mission_troop;
            return;
        }

        // Two squads joined will have more than 8 members?
        if (mSquads_TroopCount[SquadMemberSprite->field_32] + mSquads_TroopCount[mSquad_Selected] > 8)
            return;

        mSquad_Join_TargetSprite[mSquad_Selected] = SquadMemberSprite;
        mSquad_Join_TargetSquad[mSquad_Selected] = (int8)SquadMemberSprite->field_32;

        Squad_Walk_Target_Update(SquadMemberSprite->field_32);

        break;
    }
}

void cFodder::Mission_Troops_Clear_Selected() {

    for(auto& Troop : mGame_Data.mSoldiers_Allocated)
        Troop.mSelected &= 0;
}

void cFodder::sub_303AE() {
    GUI_Sidebar_Grenades_CurrentSquad_Draw();
    GUI_Sidebar_Squad_Split_Icon_Draw();
}

void cFodder::Squad_Switch_Weapon() {

    if (!mInput_Enabled || mPhase_Finished)
        return;

    if (mSquad_CurrentWeapon[mSquad_Selected] != eWeapon_Rocket) {
        if (mSquad_Rockets[mSquad_Selected])
            Squad_Select_Rockets();
    }
    else {
        if (mSquad_Grenades[mSquad_Selected])
            Squad_Select_Grenades();
    }
}

void cFodder::Mission_Final_TimeToDie() {

    // Retail CF1 only
    if (!mVersionCurrent->isRetail() || !mVersionCurrent->isCannonFodder1())
        return;

    if (!(mGame_Data.mMission_Number == 24 && mGame_Data.mMission_Phase == 6))
        return;

    ++mMission_Final_TimeToDie_Ticker;
    if (mMission_Final_TimeToDie_Ticker < 40)
        return;

    mMission_Final_TimeToDie_Ticker = 0;

    --mMission_Final_TimeRemain;
    if (mMission_Final_TimeRemain < 0)
        mMission_Final_TimeRemain = 0;

    if (mParams->mDisableVideo)
        return;

    for (unsigned int Y = 0x1000; Y < 0x1500; ++Y) {

        mSidebar_Screen_Buffer[Y] = 0;
    }

    mGUI_Sidebar_TroopList_Name_BreakOnSpace = 11;

    GUI_Sidebar_TroopList_Name_Draw(0, 0, 0xB7, "TIME TO DIE ");

    GUI_Sidebar_Number_Draw(mMission_Final_TimeRemain, 0, 0x30, 0xC0, 0xAF);

    mGUI_Sidebar_TroopList_Name_BreakOnSpace = 0x05;
}

int16 cFodder::sub_305D5(sSprite*& pData20) {
    sSprite** Data30 = 0;
    int16 Data0 = mSquad_Selected;

    if (Data0 < 0)
        goto loc_306AD;

    Data30 = mSquads[Data0];
    if (*Data30 == INVALID_SPRITE_PTR || *Data30 == 0)
        goto loc_306AD;

    do {
        if (*Data30 == INVALID_SPRITE_PTR || *Data30 == 0)
            goto loc_306AD;

        pData20 = *Data30;
        ++Data30;
        //seg011:1B61
        if (pData20->field_18 != eSprite_Player)
            continue;

        if (pData20->field_75 & eSprite_Flag_Invincibility)
            goto loc_3066E;

        if (pData20->field_38) {
            if (pData20->field_38 < eSprite_Anim_Slide1)
                goto loc_30662;
        }
        //loc_30656
        if (!pData20->field_5B)
            goto loc_30681;

    loc_30662:;
        if (!pData20->field_6E)
            continue;

    loc_3066E:;
        pData20->field_38 = eSprite_Anim_None;
        pData20->field_5B = 0;
    loc_30681:;

        if (!pData20->field_6E)
            break;

    } while (*Data30 != INVALID_SPRITE_PTR);
    //loc_3069A
    goto loc_306BE;

loc_306AD:;
    pData20 = INVALID_SPRITE_PTR;
    mSquad_Selected = -1;

loc_306BE:;
    if (mSquad_CurrentVehicle)
        return 1;

    mSquad_Leader = pData20;
    if (pData20 == INVALID_SPRITE_PTR || pData20 == 0)
        return -1;

    return 1;
}

void cFodder::Mouse_Inputs_Check() {
    int16 Data0 = 0;
    int16 Data4 = 0;

    if (mMouseDisabled)
        return;

    if (mPhase_In_Progress)
        Mouse_Cursor_Update();

    if (dword_3A030) {
        // TODO: Function pointer call, but appears not to be used
        //dword_3A030();
        return;
    }

    for (sGUI_Element* Loop_Element = mGUI_Elements;; ++Loop_Element) {

        if (Loop_Element->field_0 == 0)
            break;

        if ((*this.*Loop_Element->field_0)() < 0)
            return;

        Data0 = mGUI_Mouse_Modifier_X + Loop_Element->mX;

        int16 Data4 = mMouseX + 0x20;

        if (Data0 > Data4)
            continue;

        Data0 += Loop_Element->mWidth;
        if (Data0 < Data4)
            continue;

        Data0 = mGUI_Mouse_Modifier_Y;
        Data0 += Loop_Element->mY;
        if (Data0 > mMouseY)
            continue;

        Data0 += Loop_Element->mHeight;
        if (Data0 < mMouseY)
            continue;

        (*this.*Loop_Element->mMouseInsideFuncPtr)();
        return;
    }

    if (!mButtonPressRight)
        goto loc_30814;

    if (mSquad_Selected < 0)
        return;

    if (mMouseSpriteNew < 0) {
        mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
        mMouseX_Offset = -8;
        mMouseY_Offset = -8;
    }
    Squad_Member_Target_Set();
    if (!mSquad_CurrentVehicle)
        return;

loc_30814:;

    if (mSquad_CurrentVehicle) {
        Vehicle_Input_Handle();
        return;
    }

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    if (mMouse_Button_LeftRight_Toggle2)
        return;

    if (mMouseSpriteNew < 0) {
        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
    }

    if (mSquad_Selected < 0) {

        Squad_Member_Click_Check();
        return;
    }

    sSprite** Data24 = mSquads[mSquad_Selected];
    sSprite* Dataa24 = *Data24;

    if (Dataa24 == INVALID_SPRITE_PTR) {
        Squad_Member_Click_Check();
        return;
    }

    if (Dataa24->field_6E)
        return;

    if (word_3B2F1)
        word_3B2F1 = 0;
    else
        sub_311A7();

    mSquad_Member_Clicked_TroopInSameSquad = 0;
    Squad_Member_Click_Check();

    // If we clicked on a member of the current squad, nothing to do
    if (mSquad_Member_Clicked_TroopInSameSquad)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0)
        return;

    Data0 = mMouseX + (mCameraX >> 16);
    Data4 = mMouseY + (mCameraY >> 16);

    Data0 += -22;
    if (Data0 < 0)
        Data0 = 0;

    Data4 += -3;
    if (Data4 < 0)
        Data4 = 3;

    if (Data4 < 3)
        Data4 = 3;

    mCamera_PanTargetX = Data0;
    mCamera_PanTargetY = Data4;

    mMouse_Locked = false;

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        // Not in the squad?
        if (mSquad_Selected != Troop.mSprite->field_32)
            continue;

        Troop.mSprite->field_44 = 0;
        Troop.mSprite->field_2 = 0;
        Troop.mSprite->field_6 = 0;
    }

    int16 Data10 = mSquad_Leader->field_0;
    int16 Data8 = Data4;
    Data4 = Data0;

    Squad_Walk_Target_Set(Data4, Data8, mSquad_Leader->field_32, Data10);

    // Reset the join target
    for (auto& JoinTargetSquad : mSquad_Join_TargetSquad) {

        if (mSquad_Selected != JoinTargetSquad)
            continue;

        JoinTargetSquad = -1;
    }
}

void cFodder::Squad_Member_Target_Set() {
    sSprite** Data20 = 0;

    if (mSquad_CurrentVehicle) {
        Vehicle_Target_Set();
        return;
    }

    mSprite_Player_CheckWeapon = -1;
    if (word_3A9B8 < 0) {
        word_3A9B8 = 0x30;

        if (mSquad_Leader)
            mSquad_Leader->field_4A = 0;
    }

    int16 TargetX = mMouseX + (mCameraX >> 16);
    int16 TargetY = mMouseY + (mCameraY >> 16);

    TargetX -= 0x10;
    if (!TargetX)
        TargetX = 1;

    TargetY -= 8;
    if (mSquad_Selected < 0)
        goto loc_30E05;

    if ((mSquads_TroopCount[mSquad_Selected] - 1) < 0)
        goto loc_30E05;

    Data20 = mSquads[mSquad_Selected];

    for (;;) {
        if (*Data20 == INVALID_SPRITE_PTR || *Data20 == 0)
            break;

        sSprite* Data24 = *Data20++;

        Data24->field_2E = TargetX;
        Data24->field_30 = TargetY;
    }

loc_30E05:;
    Squad_Member_Rotate_Can_Fire();
}

int16 cFodder::Mouse_Button_Right_Toggled() {

    if (mMouse_Button_Right_Toggle < 0) {
        mMouse_Button_Right_Toggle = 1;
        return 1;
    }

    return -1;
}

void cFodder::Squad_Switch_Timer() {

    if (!mSquad_Select_Timer)
        return;

    --mSquad_Select_Timer;
    if (mSquad_Select_Timer)
        return;

    if (mSquad_Selected >= 0)
        return;

    for (int16 Data0 = 2; Data0 >= 0; --Data0) {

        if (mSquads_TroopCount[Data0]) {
            Squad_Switch_To(Data0);
            return;
        }

    }
}

void cFodder::Squad_Switch_To(int16 pData0) {

    if (!mSquads_TroopCount[pData0])
        return;

    mSquad_Selected = pData0;
    sSprite* Data20 = 0;

    if (sub_305D5(Data20) >= 0) {
        mCamera_StartPosition_X = Data20->field_0;
        mCamera_StartPosition_Y = Data20->field_4;
        mCamera_Start_Adjust = true;
        mMouse_Locked = false;
    }

    mGUI_Sidebar_Setup = 0;
}
