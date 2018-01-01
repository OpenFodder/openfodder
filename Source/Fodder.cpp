/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2017 Robert Crossfield
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

#define INVALID_SPRITE_PTR (sSprite*) -1

#define ButtonToMouseVersion(x) case x: { Buttons[x].mMouseInsideFuncPtr = &cFodder::VersionSelect_##x; break; }


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

cFodder::cFodder( cWindow* pWindow, bool pSkipIntro ) {
    
    mVersion = 0;
    mVersionDefault = 0;

    mOpenFodder_Intro_Done = false;
    mCustom_Mode = eCustomMode_None;
    mSkipIntro = pSkipIntro;
    mResources = 0;
    mGraphics = 0;
    mSound = 0;
    mWindow = pWindow;
    mMapNumber = 0;
    mMissionNumber = 0;

    mTicksDiff = 0;
    mTicks = 0;

    mMouseX_Offset = 0;
    mMouseY_Offset = 0;

    mIntroDone = false;

    mMouseButtons = 0;

    mouse_Button_Status = 0;
    mouse_Pos_Column = 0;
    mouse_Pos_Row = 0;

    mButtonPressLeft = mButtonPressRight = 0;
    mMouse_Button_Left_Toggle = mMouse_Exit_Loop = 0;
    mMouse_Button_Right_Toggle = 0;
    mMouse_Button_LeftRight_Toggle = word_39F04 = 0;

    mSprite_Frame_1 = 0;
    word_39FF4 = 0;
    mSprite_Frame_2 = 0;
    mSprite_Frame3_ChangeCount = 0;
    mSprite_Frame_3 = 0;

    mEnemy_BuildingCount = 0;
    mMission_Aborted = false;
    mMouse_Button_LeftRight_Toggle2 = 0;

    word_3ABFD = 0;

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
    dword_3B30D = 0;

    mMapTile_Ptr = 0;

    mSoundEffectToPlay = 0;
    mBriefing_Aborted = 0;
    mGUI_Mouse_Modifier_X = 0;
    mGUI_Mouse_Modifier_Y = 0;
    mBriefing_Render_1_Mode = 0;

    mMouseSpriteCurrent = 0;
    mService_ExitLoop = 0;
    mVideo_Draw_FrameDataPtr = 0;
    word_42066 = 0;
    mVideo_Draw_PosX = 0;
    mVideo_Draw_PosY = 0;
    mVideo_Draw_Columns = 0;
    mVideo_Draw_Rows = 0;
    mVideo_Draw_PaletteIndex = 0;
    mDraw_Source_SkipPixelsPerRow = 0;
    mDraw_Dest_SkipPixelsPerRow = 0;
    mKeyCode = 0;

    dword_39F84 = 0;
    dword_39F88 = 0;
    dword_39F8C = 0;
    dword_39F90 = 0;

    mSurfaceMapOverview = 0;

    mMap = 0;

    mMission_IsFinished = 0;
    word_3A3BB = 0;
    mSidebar_Draw_Y = 0;
    word_3A3BF = 0;

    mMap_Overview_MapNumberRendered = -1;
    mDebug_MissionSkip = 0;
    mPaused = 0;

    byte_427E6 = 0;
    byte_427EE = 0;

    mRandom_0 = 0;
    mRandom_1 = 0;
    mRandom_2 = 0;
    mRandom_3 = 0;
    byte_44AC0 = 0;

    Squad_Walk_Target_SetAll(0);
	mMission_Completed_Timer = 0;

    for (unsigned int x = 0; x < 0x18; ++x) {
        mMission_Save_Availability[x] = 0;
    }

    word_44475 = 0;

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

    for (int16 x = 0; x < 45; ++x) {
        Sprite_Clear( &mSprites[x] );
    }
    Mission_Memory_Clear();

    Sprite_Table_Setup();
}

cFodder::~cFodder() {
    
    delete mSurfaceMapOverview;

    VersionCleanup();
}

void cFodder::Squad_Walk_Target_SetAll( int16 pValue ) {

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

int16 cFodder::Map_Loop( ) {

    mImage->Save();

    for (;;) {
        Video_Sleep();

        sub_12018();
        MapTile_Update_Position( );

        Game_Handle();
        ++mMission_EngineTicks;

        if (mCamera_Start_Adjust >= 0) {
            Camera_Refresh();
            mCamera_Start_Adjust = -1;
            continue;
        }

        //loc_1074E
        if (mGUI_Sidebar_Setup >= 0 && !mMission_TryAgain)
            GUI_Sidebar_Setup();
        else
            GUI_Sidebar_Draw();

        //loc_10768
        Mission_Progress_Check();
        mHelicopterCallPadPressedCount = 0;
        if (word_3A9B8 >= 0)
            --word_3A9B8;

        Sprite_Find_HumanVehicles();

        // Cheat
        if (mDebug_MissionSkip == -1)
            mMission_Complete = -1;
        else
            Mission_Phase_Goals_Check();

        //loc_1079C
        Squad_Walk_Steps_Decrease();
        Squad_Troops_Count();

        Mission_Sprites_Handle();
        Squad_Switch_Timer();
        mGraphics->Sidebar_Copy_To_Surface();
        Mouse_DrawCursor();

        // Game Paused
        if (mMission_Paused != 0) {
            Mission_Paused();

            while (mMission_Paused) {

                g_Window.RenderAt( mImage );
                g_Window.FrameEnd();
                eventProcess();
                Video_Sleep();
            }

            mGraphics->PaletteSet();
            mImageFaded = -1;
            mImage->palette_FadeTowardNew();
            mPaused = -1;
        }

        if( mImageFaded == -1 )
            mImageFaded = mImage->palette_FadeTowardNew();

        Camera_Update_From_Mouse();
        if (mPaused == -1) {
            mPaused = 0;
        }

        if (mMission_ShowMapOverview) {
            Mission_Map_Overview_Show();

            mMission_ShowMapOverview = 0;
        }

        if (mMission_IsFinished) {

            if (mMission_Aborted)
                Sprite_Handle_Player_Destroy_Unk(); 
            else {
                if (!mMission_TryAgain)
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

        g_Window.RenderAt( mImage );
        g_Window.FrameEnd();
        mImage->Restore();
    }

    return 0;
}

void cFodder::Game_Handle( ) {

    // Input disabled or Mission not paused?
    if (!mInput_Enabled || !mMission_Paused) {

        ++mGame_InputTicks;

        Mouse_Inputs_Get();

        if (mInput_Enabled) {
            Camera_Handle();
            Camera_Handle();
            Camera_Handle();

            if (!mMission_Finished)
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

    if (!mMission_In_Progress)
        return;

    if (mMission_Completed_Timer || mMission_Complete || mMission_TryAgain || mMission_Aborted) {
        mMission_Finished = -1;
        return;
    }

    if (!mSoundEffectToPlay)
        return;

    if ((byte_427E6 | byte_427EE))
        return;

    Sound_Play( mSquad_Leader, mSoundEffectToPlay, 0 );
    mSoundEffectToPlay = 0;
}

void cFodder::Camera_Handle() {
    
    if (mMission_In_Progress) {
        Camera_Speed_Reset();
        Camera_Mission_Over_Check();
        Camera_Calculate_Scroll();
        sub_120F6();
        Camera_Position_Update();
        Camera_Position_Toward_SquadLeader();
        Camera_Pan_SetSpeed();
    }
}

void cFodder::Camera_Position_Toward_SquadLeader() {
    if(mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0 )
        return;
    
    int16 SquadLeaderX = mSquad_Leader->field_0 + 0x18;
    
    int16 SquadLeaderY = mSquad_Leader->field_4;
    int16 Data18 = SquadLeaderX;
    int16 Data1C = SquadLeaderY;
    
    mCamera_Scroll_Speed = 0x14;

    if( mMouseX > 0x0F )
        goto Mouse_In_Playfield;
    
    int16 Data0;
    int16 Data4;

    if( word_39F40 ) {
        Data0 = SquadLeaderX;
        Data0 -= word_39F3C;
        mCamera_Position_Column += Data0;

        Data4 = SquadLeaderY;
        Data4 -= word_39F3E;
        mCamera_Position_Row += Data4;

        word_39F3C = SquadLeaderX;
        word_39F3E = SquadLeaderY;
        
    } else {
        //loc_10A11
        word_39F40 = -1;
        Data0 = mCamera_Adjust_Col >> 16;
        Data0 += 0x80;
        mCamera_Position_Column = Data0;

        Data4 = mCamera_Adjust_Row >> 16;
        Data4 += 0x6C;
        mCamera_Position_Row = Data4;
        
    }
    //loc_10A3A
    word_3A054 = 0;
    return;
    
Mouse_In_Playfield:;
    word_39F40 = 0;
    word_39F3C = SquadLeaderX;
    word_39F3E = SquadLeaderY;
    Data0 = mMouseX;
    Data0 += mCamera_Adjust_Col >> 16;
    Data4 = mMouseY;
    Data4 += mCamera_Adjust_Row >> 16;

    int16 Data0_Saved = Data0;
    int16 Data4_Saved = Data4;
    int16 Data8_Saved = SquadLeaderX;
    int16 DataC_Saved = SquadLeaderY;
    
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, SquadLeaderX, SquadLeaderY );
    
    if( mSquad_CurrentVehicle ) {
    
        if( Data0 >= 0x64 )
            Data0 = 0x64;
    } else {
        
        if( Data0 >= 0x8C )
            Data0 = 0x8C;
    }
    //loc_10AAA
    word_3ABFD = Data0;

    SquadLeaderX = Data8_Saved;
    SquadLeaderY = DataC_Saved;
    Data4 = Data4_Saved;
    Data0 = Data0_Saved;
    

    if(Direction_Between_Points(Data0, Data4, SquadLeaderX, SquadLeaderY ) < 0 )
        return;
    
    SquadLeaderX = mMap_Direction_Calculations[Data4/2];
    Data4 += 0x80;
    Data4 &= 0X1FE;

    SquadLeaderY = mMap_Direction_Calculations[Data4/2];
    
    int32 Dataa8 = SquadLeaderX * word_3ABFD;
    int32 DataaC = SquadLeaderY * word_3ABFD;
    
    SquadLeaderX = Dataa8 >> 16;
    SquadLeaderY = DataaC >> 16;
    
    Data18 += SquadLeaderX;
    Data1C += SquadLeaderY;
    Data18 -= 0x18;
    
    if(Data18 < 0 )
        Data18 = 0;

    mCamera_Position_Column = Data18;
    mCamera_Position_Row = Data1C;
    word_3A054 = 0;
}

void cFodder::Game_ClearVariables() {
    mInput_Enabled = 0;
    mGame_InputTicks = 0;
    mMission_EngineTicks = 0;
    mMission_Restart = 0;
    mSprite_Enemy_AggressionAverage = 0;
    mSprite_Enemy_AggressionMin = 0;
    mSprite_Enemy_AggressionMax = 0;
    mSprite_Enemy_AggressionNext = 0;
    mSprite_Enemy_AggressionIncrement = 0;
    mMapNumber = 0;
    mMissionNumber = 0;
    mMissionPhase = 0;
    mRecruits_Available_Count = 0;
    mSaved_MissionNumber = 0;
    mSaved_MapNumber = 0;
    Mission_Troop_Prepare_SetFromSpritePtrs = 0;

    for (unsigned int x = 0; x < 8; ++x) {
        mMission_Troops_SpritePtrs[x] = 0;
    }

    mSprite_Enemy_AggressionCreated_Count = 0;
    mMission_Recruits_AliveCount = 0;
    mMission_Recruitment = 0;
    mMission_TryingAgain = 0;
    mMission_Phases_Remaining = 0;
    mMission_Phases_Total = 0;
    mRecruit_NextID = 0;

    for (unsigned int x = 0; x < 9; ++x) {
        mMission_Troops[x].mRecruitID = 0;
        mMission_Troops[x].mRank = 0;
        mMission_Troops[x].mPhaseCount = 0;
        mMission_Troops[x].mSprite = 0;
        mMission_Troops[x].field_6 = 0;
        mMission_Troops[x].field_8 = 0;
        mMission_Troops[x].mNumberOfKills = 0;
    }

    for (unsigned int x = 0; x < 361; ++x)
        mGraveRanks[x] = 0;

    mGraveRankPtr = 0;
    mGraveRankPtr2 = 0;

    for (unsigned int x = 0; x < 361; ++x)
        mGraveRecruitID[x] = 0;

    mGraveRecruitIDPtr = 0;

    for (unsigned int x = 0; x < 6; ++x) {
        mHeroes[x].mRecruitID = 0;
        mHeroes[x].mRank = 0;
        mHeroes[x].mKills = 0;
    }
    mTroops_Away = 0;
    mTroops_Home = 0;
    mMission_Troops_Required = 0;
    mMission_Troop_Count = 0;
    mMission_Troops_Available = 0;
}

void cFodder::Squad_Clear() {
    mMission_Troops[8].mRecruitID = -1;
    mMission_Troops[8].mSprite = INVALID_SPRITE_PTR;

    for (unsigned int x = 0; x < 8; ++x) {
        mMission_Troops[x].mSprite = INVALID_SPRITE_PTR;
        mMission_Troops[x].mRecruitID = -1;
        mMission_Troops[x].mRank = 0;
        mMission_Troops[x].mPhaseCount = 0;
    }
}

void cFodder::Heroes_Clear() {
    
    for (unsigned int x = 0; x < 6; ++x) {
        mHeroes[x].mRecruitID = -1;
        mHeroes[x].mRank = -1;
        mHeroes[x].mKills = -1;
    }
}

void cFodder::Mission_Memory_Backup() {
    uint8* Start = (uint8*) &mMapNumber;
    uint8* End = (uint8*)&mButtonPressLeft;

    memcpy( mMission_Memory_Backup, &mMapNumber, End - Start );
}

void cFodder::Mission_Memory_Restore() {
    uint8* Start = (uint8*) &mMapNumber;
    uint8* End = (uint8*)&mButtonPressLeft;

    memcpy( &mMapNumber, mMission_Memory_Backup, End - Start );
}

void cFodder::Mission_Memory_Clear() {
    // Clear memory 2454 to 3B58
    mButtonPressLeft = 0;
    mButtonPressRight = 0;
    mMouse_Button_Left_Toggle = 0;
    mMouse_Button_Right_Toggle = 0;
    mMouse_Button_LeftRight_Toggle = 0;

    mVehicle_Input_Disabled = false;
    mMouse_Exit_Loop = 0;
    word_39F04 = 0;

    mCamera_Adjust_Row = 0;
    dword_39F36 = 0;
    mCamera_Scroll_Speed = 0;
    word_39F3C = 0;
    word_39F3E = 0;
    word_39F40 = 0;

    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;
    dword_39F4A = 0;

    dword_39F4E = 0;
    word_39F52 = 0;
    word_39F54 = 0;
    dword_39F56 = 0;
    dword_39F5A = 0;
    mCamera_Adjust_Col_High = 0;
    mCamera_Adjust_Row_High = 0;
    mKeyCodeAscii = 0;
    dword_39F84 = 0;
    dword_39F88 = 0;
    dword_39F8C = 0;
    dword_39F90 = 0;

    word_39FA0 = 0;
    word_39FA2 = 0;
    mMapTile_Column = 0;
    mMapTile_Row = 0;
    word_39FAC = 0;

    mSquad_Leader = 0;
    word_39FB2 = 0;
    word_39FB4 = 0;
    word_39FB6 = 0;
    word_39FB8 = 0;
    word_39FBA = 0;
    mSquad_Selected = 0;
    mSquad_JoiningTo = 0;

    mSprite_Frame_1 = 0;
    word_39FF4 = 0;
    mSprite_Frame_2 = 0;
    mSprite_Frame3_ChangeCount = 0;
    mSprite_Frame_3 = 0;

    mTroop_Weapon_Grenade_Disabled = false;
    mTroop_Weapon_Bullet_Disabled = false;
    mTroop_Weapon_Rocket_Disabled = false;
    word_3A024 = 0;
    dword_3A030 = 0;
    mCamera_Position_Column = 0;
    mCamera_Position_Row = 0;
    mCamera_Position_X = 0;

    for (int16 x = 0; x < 20; ++x)
        mSprite_DrawList_First[x] = 0;

    for (int16 x = 0; x < 45; ++x) {
        mSprite_DrawList_Second[x] = 0;
        mSprite_DrawList_Third[x] = 0;
    }

    for (int16 x = 0; x < 64; ++x) {
        mSprite_DrawList_Final[x] = 0;
    }

    mStoredSpriteX = 0;
    mStoredSpriteY = 0;
    mTmp_FrameNumber = 0;

    mMission_IsFinished = 0;
    word_3A3BB = 0;
    mSidebar_Draw_Y = 0;
    word_3A3BF = 0;
    mDirectionMod = 0;
    mMission_Aborted = false;
    mSquad_SwitchWeapon = 0;
    word_3A9B8 = 0;
    for (uint8 x = 0; x < 3; ++x) {
        mSquad_Walk_Target_Indexes[x] = 0;
        mSquad_Walk_Target_Steps[x] = 0;
    }

    mSprite_Bumped_Into_SquadMember = 0;
    word_3A9CE = 0;
    mMission_Paused = 0;
    for (uint8 x = 0; x < 4; ++x) {
        byte_3A9D2[x] = 0;
        byte_3A9D6[x] = 0;
    }
    for (uint8 x = 0; x < 10; ++x) {
        byte_3A9DA[x] = 0;
    }

    mMouseX_Offset = 0;
    mMouseY_Offset = 0;
    mMouseSpriteNew = 0;

    dword_3A9FD = 0;

    mMouseSetToCursor = 0;
    mSprites_Found_Count = 0;

    for (uint16 x = 0; x < 3; ++x) {
        mSquad_Grenades[x] = 0;
        mSquad_Rockets[x] = 0;
        mGUI_Squad_Icon[x] = 0;
    }

    mTroops_Enemy_Count = 0;
    mHostage_Count = 0;
    mCamera_Start_Adjust = 0;
    word_3AA1D = 0;
    mCamera_Reached_Target = 0;
    word_3AA21 = 0;
    mSprite_FaceWeaponTarget = 0;
    word_3AA45 = 0;
    mSquad_Select_Timer = 0;
    mSprite_Find_Distance = 0;
    mMapWidth_Pixels = 0;
    mMapHeight_Pixels = 0;
    mMouseCursor_Enabled = 0;
    word_3AA55 = 0;
    word_3AAC7 = 0;
    mRecruit_RenderedPtr = 0;
    mRecruit_Truck_Frame = 0;
    mRecruit_Truck_FrameAnimBufferPtr = 0;

    word_3ABB1 = 0;
    mSquad_Member_Fire_CoolDown = 0;
    mTroop_Rotate_Next = 0;
    word_3ABB7 = 0;
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

    for (uint16 x = 0; x < 9; ++x )
        mService_Troop_Promotions[x] = 0;

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

    for (uint16 x = 0; x < 128; ++x) {
        mMap_Destroy_Tiles[x].mX = 0;
        mMap_Destroy_Tiles[x].mY = 0;
    }

    mMap_Destroy_Tiles_Countdown = 0;
    mMap_Destroy_TilesPtr2 = 0;
    mMap_Destroy_TilesPtr = 0;

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
    word_3B1A9 = 0;
    mSprite_Bullet_Deviate_Counter = 0;
    dword_3B1CB = 0;

    for (uint16 x = 0; x < 0x0F; ++x ) 
        mRecruit_Hill_Positions_Use[x] = 0;

    mRecruit_Truck_Animation_Play = 0;
    mRecruit_Truck_Enter_Count = 0;

    mSquad_CurrentVehicle = 0;
    mMission_In_Progress = false;
    mSprite_HumanVehicles_Found = 0;
    for (uint16 x = 0; x < 14; ++x) {
        mSprites_HumanVehicles[x] = 0;
    }
    dword_3B24B = 0;

    for (uint16 x = 0; x < 3; ++x) {
        mSquad_CurrentVehicles[x] = 0;
    }

    word_3B25B = 0;
    word_3B25D = 0;
    word_3B2CB = 0;
    mGUI_SaveLoadAction = 0;
    word_3B2CF = 0;
    for (uint16 x = 0; x < 6; ++x)
        word_3B2D1[x] = 0;

    word_3B2DD[0] = 0;
    word_3B2DD[1] = 0;
    word_3B2DD[2] = 0;
    word_3B2DD[3] = 0;
    word_3B2DD[4] = 0;
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
    mInputString_Position = 0;
    mGUI_Select_File_CurrentIndex = 0;
    mGUI_Select_File_Count = 0;
    mGUI_Select_File_SelectedFileIndex = 0;
    mIntro_PlayTextDuration = 0;
    mSoundEffectToPlay_Set = 0;

    for (uint16 x = 0; x < 3; ++x)
        mSquad_EnteredVehicleTimer[x] = 0;

    for (uint16 x = 0; x < 8; ++x)
        mPhase_Goals[x] = 0;

    mSprite_OpenCloseDoor_Ptr = 0;
    mSprite_Civilian_GotHome = 0;
    mSprite_Indigenous_Tmp_X = 0;
    mSprite_Indigenous_Tmp_Y = 0;
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
    mMission_ShowMapOverview = 0;

    mTurretFires_HomingMissile = 0;
    word_3B4E9 = 0;
    word_3B4EB = 0;
    word_3B4ED[0] = 0;
    word_3B4ED[1] = 0;
    mMission_Finished = 0;
    mImage_Aborted = 0;
    mBriefing_Aborted = 0;
    mHostage_Rescue_Tent = 0;
}

void cFodder::Mission_Prepare_Squads() {
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
}

void cFodder::sub_10DEC() {
    word_3A054 = 0;

    mSquad_Selected = 0;
    m2A622_Unk_MapPosition.mX = 0;
    m2A622_Unk_MapPosition.mY = 0;
    mMission_TryAgain = 0;
    mMission_Complete = 0;
    mMission_Completed_Timer = 0;

    mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;

    mSquads_TroopCount[0] = 0;
    mSquads_TroopCount[1] = 0;
    mSquads_TroopCount[2] = 0;
    mSquads_TroopCount[3] = 0;

    byte_3A05E = 0;

    for (uint16 x = 0; x < 200; ++x )
        byte_3A8DE[x] = 0;

    mCamera_Position_X = 0;
    mCamera_Position_Y = 0;

    Squad_Walk_Target_SetAll( -1 );
}

void cFodder::Squad_Set_Squad_Leader() {

    mSprites[0].field_0 = -32768;
    mSquad_Leader = &mSprites[0];
}

void cFodder::Sprite_Clear_All() {

    sSprite* Data = mSprites;
    for( int16 count = 0x2C; count >= 0; --count,++Data ) {
        
        Sprite_Clear( Data );
    }

    mSprites[44].field_0 = -1;
}

void cFodder::Map_Save(const std::string pFilename) {

    std::ofstream outfile(pFilename, std::ofstream::binary);

    // The original game stores the maps in big endian
    tool_EndianSwap(mMap->data() + 0x60, mMap->size() - 0x60);
    outfile.write((const char*)mMap->data(), mMap->size());
    outfile.close();

    // Now we can swap it back to little endian
    tool_EndianSwap(mMap->data() + 0x60, mMap->size() - 0x60);

    Map_Save_Sprites(pFilename);
}

void cFodder::Map_Save_Sprites( const std::string pFilename ) {
    std::string SptFilename = pFilename;

    // Replace .map with .spt
    SptFilename.replace(pFilename.length() - 3, pFilename.length(), "spt");

    std::ofstream outfile(SptFilename, std::ofstream::binary);

    // Number of sprites in use
    size_t SpriteCount = std::count_if(std::begin(mSprites), std::end(mSprites), [](auto& l) {
        return l.field_0 != -32768 && l.field_0 != -1;
    });

    auto MapSpt = tSharedBuffer( new std::vector<uint8>() );
    MapSpt->resize(SpriteCount * 0x0A);

    uint8* SptPtr = MapSpt->data();

	// Cheap way of writing human players first
    for (const auto SpriteIT : mSprites) {

		if (SpriteIT.field_0 == -1 || SpriteIT.field_0 == -32768)
			continue;

		if (SpriteIT.field_18 != eSprite_Player)
			continue;

        writeBEWord(SptPtr, 0x7C);  SptPtr += 2;						// Direction
        writeBEWord(SptPtr, 0x00);  SptPtr += 2;						// Ignored
        writeBEWord(SptPtr, SpriteIT.field_0 - 0x10);  SptPtr += 2;    // X    
        writeBEWord(SptPtr, SpriteIT.field_4);  SptPtr += 2;			// Y    
        writeBEWord(SptPtr, SpriteIT.field_18); SptPtr += 2;			// Type 
    }


	for (const auto SpriteIT : mSprites) {

		if (SpriteIT.field_0 == -1 || SpriteIT.field_0 == -32768)
			continue;

		if (SpriteIT.field_18 == eSprite_Player)
			continue;

		writeBEWord(SptPtr, 0x7C);  SptPtr += 2;                // Direction
		writeBEWord(SptPtr, 0x00);  SptPtr += 2;                // Ignored
		writeBEWord(SptPtr, SpriteIT.field_0 - 0x10);  SptPtr += 2;    // X    
		writeBEWord(SptPtr, SpriteIT.field_4);  SptPtr += 2;    // Y    
		writeBEWord(SptPtr, SpriteIT.field_18); SptPtr += 2;    // Type 
	}
    outfile.write((const char*)MapSpt->data(), MapSpt->size());
    outfile.close();
}

#include "Utils//diamondsquare.hpp"

int16 cFodder::Tile_FindType(eTerrainType pType) {

	for (int16 TileID = 0; TileID < sizeof(mTile_Hit) / sizeof(int16); ++TileID) {
		
		// Single Type Tile
		if (mTile_Hit[TileID] >= 0) {

			// Match?
			if ((mTile_Hit[TileID] & 0x0F) == pType) {

				return TileID;
			}
		}
	}

	return -1;
}

void cFodder::Map_Randomise(const long pSeed) {
	int32 PowerOf = 0;
	int32 Size;

	if (mMapWidth < mMapHeight)
		Size = mMapHeight;
	else
		Size = mMapWidth;

	while (Size > 0) {
		PowerOf++;
		Size = Size >> 1;
	}

   cDiamondSquare DS(PowerOf, pSeed);
   auto HeightMap = DS.generate();

   int16* MapPtr = (int16*)(mMap->data() + 0x60);

   // Find the highest and lowest points in the height map
   double HeightMin = 0;
   double HeightMax = 0;
   for (auto Row : HeightMap) {
	   for (auto Column : Row) {

		   if (Column > HeightMax) HeightMax = Column;
		   if (Column < HeightMin) HeightMin = Column;
	   }
   }

   // Calcukate the difference between top/bottom
   double diff = HeightMax - HeightMin;
   double flood = 0.5;
   double mount = 0.85;

   // Calculate the flood/moutain levels
   flood *= diff;
   mount *= diff;

   int16 TileWater = Tile_FindType(eTerrainType_Water);
   int16 TileLand = Tile_FindType(eTerrainType_Land);
   int16 TileBounce = Tile_FindType(eTerrainType_BounceOff);

   int16 X = 0;
   int16 Y = 0;

   // Loop each tile row
   for (auto Row : HeightMap) {

	   X = 0;

	   // Loop each tile column
	   for (auto Column : Row) {

		   Column -= HeightMin;

		   // Dont set tile if its already set
		   if (*MapPtr == 0) {

			   if (Column < flood) {
				   *MapPtr = TileWater;
			   }
			   else if (Column > mount) {
				   *MapPtr = TileBounce;
			   }
			   else {
				   *MapPtr = TileLand;
			   }
		   }

		   ++MapPtr;

		   if(++X >= mMapWidth)
			   break;
	   }

	   if (++Y >= mMapHeight)
		   break;
   }


}

void cFodder::Map_Add_Structure(const sStructure& pStructure, int16 pTileX, int16 pTileY ) {

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

void cFodder::Map_Randomise_Structures( const size_t pCount ) {

	mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);

	int16 StructsCount = 0;

	// This is very lame :)
	while (StructsCount++ < pCount) {
		auto Struct = mStructuresBarracksWithSoldier[mMap_TileSet];

		int16 StartTileX = (((uint16)tool_RandomGet()) % (mMapWidth - Struct.MaxWidth() - 2)) + 2;
		int16 StartTileY = (((uint16)tool_RandomGet()) % (mMapHeight - Struct.MaxHeight() - 2)) + 2;

		// TODO: Check if we will overlap an existing structure,
		//		 or place on water

		Map_Add_Structure(Struct, StartTileX, StartTileY);

		// Add an enemy below each building
		Sprite_Add(eSprite_Enemy, StartTileX * 16, (StartTileY + 3) * 16);
	}
}

void cFodder::Map_Randomise_Sprites() {
	int16 Distance = 8;

	int16 StartTileX = (((uint16)tool_RandomGet()) % (mMapWidth - 2))  + 2;
	int16 StartTileY = (((uint16)tool_RandomGet()) % (mMapHeight - 2)) + 2;

	int16 MiddleX = StartTileX * 16;
	int16 MiddleY = StartTileY * 16;

	// Add atleast two sprites
	Sprite_Add(eSprite_Player, MiddleX - Distance, MiddleY );
	Sprite_Add(eSprite_Player, MiddleX + Distance, MiddleY );

	if( tool_RandomGet() % 1 )
		Sprite_Add(eSprite_Player, MiddleX, MiddleY + Distance);


	// Add some weapons
	Sprite_Add(eSprite_RocketBox, MiddleX, MiddleY + Distance);
	Sprite_Add(eSprite_GrenadeBox, MiddleX + Distance, MiddleY + Distance);


}

void cFodder::Map_Load_Sprites() {

    Sprite_Clear_All();

    auto MapSprites = mCampaign.getSprites(mMapNumber);
    tool_EndianSwap( (uint8*)MapSprites->data(), MapSprites->size());
    
    uint16* SptPtr = (uint16*) MapSprites->data();
    sSprite* Sprite = mSprites;
    
    uint16* SptFileEnd = SptPtr + (MapSprites->size() / 2);

    for(uint16 HumanCount = 0; SptPtr != SptFileEnd; ++Sprite ) {
        ++SptPtr;

        Sprite->field_8 = 0x7C;
        
        uint16 ax = HumanCount / 8;

        Sprite->field_32 = ax;
        ++SptPtr;
        ax = SptPtr[0];
        ++SptPtr;
        
        ax += 0x10;
        Sprite->field_0 = ax;
        Sprite->field_26 = ax;
        
        ax = SptPtr[0];
        ++SptPtr;
        Sprite->field_4 = ax;
        Sprite->field_28 = ax;
        
        ax = SptPtr[0];
        ++SptPtr;
        Sprite->field_18 = ax;
        
        // 114B
		
    }

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

		if (Sprite.field_18 == eSprite_Hostage_2 || Sprite.field_18 == eSprite_Hostage) {

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

void cFodder::Mission_Troop_Count() {
    mMission_Troops_Required = 0;
    sSprite* Sprite = mSprites;
    
    // How many player sprites are on this map
    for( int16 mTmpCount = 0x1D; mTmpCount > 0; --mTmpCount, ++Sprite ) {
        if( Sprite->field_0 != -32768 ) {
            
            if( Sprite->field_18 == eSprite_Player )
                ++mMission_Troops_Required;
        }
    }
    
    // How many troops have already been allocated to the mission
    mMission_Troop_Count = 0;
    for( int16 x = 7; x >= 0; --x ) {
        
        if( mMission_Troops[x].mRecruitID != -1 ) {
            --mMission_Troops_Required;
            ++mMission_Troop_Count;
        }
    }
    
    // Do we still require troops?
    int16 ax = mMission_Troops_Required;
    if( mMission_Troops_Required >= 0 ) {

        // Do we need more than available?
        if( ax > mRecruits_Available_Count )
            ax = mRecruits_Available_Count;
        
        // Set the Required amount
        mMission_Troops_Required = ax;

        // Reduce the available recruits by the number taken by the mission
        mRecruits_Available_Count -= ax;
    }
    
    mMission_Troops_Available = mMission_Troop_Count + ax;
}

void cFodder::Mission_Troop_Sort() {

    // Remove any 'dead' troops
    for (int16 Data1c = 7; Data1c >= 0; --Data1c) {
        sMission_Troop* Data20 = mMission_Troops;

        for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {

            if (Data20->mRecruitID == -1) {

                sMission_Troop* Data24 = Data20 + 1;

                *Data20 = *Data24;
                Data24->mRecruitID = -1;
                Data24->mRank = 0;
            }
        }
    }
    
    // Sort by kills

    sMission_Troop* Data20 = mMission_Troops;
    for (int16 Data1c = 7; Data1c >= 0; --Data1c, ++Data20) {
        sMission_Troop* Data24 = mMission_Troops;

        for (int16 Data18 = 7; Data18 >= 0; --Data18, ++Data24) {

            if (Data20 == Data24)
                continue;

            if (Data20->mRecruitID == -1 || Data24->mRecruitID == -1)
                continue;

            if (Data20->mRank != Data24->mRank)
                continue;

            if (Data20->mNumberOfKills <= Data24->mNumberOfKills)
                continue;

            sMission_Troop Spare = *Data20;

            *Data20 = *Data24;
            *Data24 = Spare;
        }
    }
}

void cFodder::Mission_Troop_Prepare(const bool pPrebriefing) {
    Mission_Troops_Clear_Selected();
    
    if (!pPrebriefing) {

        // Set the sMission_Troop Sprites from mMission_Troops_SpritePtrs
        if (Mission_Troop_Prepare_SetFromSpritePtrs) {
            sMission_Troop* MissionTroop = mMission_Troops;
            sSprite** Data24 = mMission_Troops_SpritePtrs;

            for (int16 Data0 = 7; Data0 >= 0; --Data0, ++MissionTroop) {
                MissionTroop->mSprite = *Data24++;
            }
        }

        Mission_Troop_Prepare_SetFromSpritePtrs = -1;

        sMission_Troop* MissionTroop = mMission_Troops;
        sSprite** Data24 = mMission_Troops_SpritePtrs;

        for (int16 Data0 = 7; Data0 >= 0; --Data0, ++MissionTroop) {
            *Data24++ = MissionTroop->mSprite;
        }

    }

    // Join recruits into the squad
    int16 Data1C = mMission_Troops_Required - 1;
    while (Data1C >= 0) {
        Mission_Troop_Prepare_Next_Recruits();
        --Data1C;
    }

    if (pPrebriefing)
        return;

    // Remove recruits which arn't needed for the map
    Data1C = mMission_Troops_Available;
    sMission_Troop* Data20 = mMission_Troops;
    for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {

        if (Data20->mSprite == INVALID_SPRITE_PTR)
            continue;

        if (Data1C)
            --Data1C;
        else
            Data20->mSprite = INVALID_SPRITE_PTR;
    }

}

void cFodder::Mission_Troop_Prepare_Next_Recruits() {
    sMission_Troop* Data20 = mMission_Troops;

    // Loop each troop member
    for (int16 Data0 = 7; Data0 >= 0; --Data0) {

        // Does troop member have a recruit id?
        if (Data20->mRecruitID == -1) {

            if (mRecruit_NextID >= 360)
                return;

            Data20->mRecruitID = mRecruit_NextID;

            const sRecruit* Data24 = &mRecruits[mRecruit_NextID];

            // Demo sets static ranks
            if (mVersion->mRelease == eRelease::Demo && mCustom_Mode != eCustomMode_Set) {

                Data20->mRank = (mMissionNumber - 1) >> 1;

                // Jops
                if (Data20->mRecruitID == 1)
                    Data20->mRank = 2;

                // Jools
                if (Data20->mRecruitID == 0)
                    Data20->mRank = 4;

            } else {
                Data20->mRank = (mMissionNumber - 1) / 3;
            }

            Data20->field_8 = 0;
            Data20->field_6 = 3;
            ++mRecruit_NextID;

            return;
        }

        ++Data20;
    }
}

void cFodder::Mission_Troop_Attach_Sprites() {

    int16 TroopsRemaining = mMission_Troops_Available;
    sSprite* Data20 = mSprites;
    sMission_Troop* Troop = mMission_Troops;

    for (int16 Data18 = 0x1D; Data18 >= 0; --Data18, ++Data20 ) {

        if (Data20->field_0 == -32768)
            continue;

        if (Data20->field_18 != eSprite_Player)
            continue;

        --TroopsRemaining;
        if (TroopsRemaining < 0) {
            Troop->mSprite = INVALID_SPRITE_PTR;
            Data20->field_0 = -32768;
            Data20->field_18 = eSprite_Null;
            Data20->field_8 = 0x7C;
            ++Troop;
        } else {
            // loc_1166B
            Data20->field_46_mission_troop =  Troop;

            Troop->mSprite = Data20;
            Data20->field_10 = 0x40;
            Data20->field_22 = eSprite_PersonType_Human;

            ++Troop;
        }
    }
}

void cFodder::Camera_Position_Update() {
    int16 Data4 = mCamera_Position_Row;
    Data4 -= 108;

    if (Data4 < 0)
        Data4 = 0;

    int16 Data0 = mCamera_Position_Column;
    Data0 -= 128;

    if (Data0 < 0)
        Data0 = 0;
    
    int16 Data8 = mCamera_Adjust_Col >> 16;
    int16 DataC = mCamera_Adjust_Row >> 16;

    int16 Data0_Saved = Data0;
    int16 Data4_Saved = Data4;
    int16 Data8_Saved = Data8;
    int16 DataC_Saved = DataC;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 > 0x10) {
        Data4 = mCamera_Scroll_Speed;

        if (Data4 <= (dword_39F36 >> 16)) {
            dword_39F36 = (Data4 << 16) | (dword_39F36 & 0xFFFF);
        } else {
            dword_39F36 += 0x8000;
        }
    } else {
        dword_39F36 = Data0 << 16;
    }

    DataC = DataC_Saved;
    Data8 = Data8_Saved;
    Data4 = Data4_Saved;
    Data0 = Data0_Saved;

    if (sub_119E1( Data0, Data4, Data8, DataC ) >= 0) {

        Data0 &= 0x1FE;
        word_39F34 = Data0;
        if (!dword_39F36)
            return;
        Data4 = word_39F34;

        Data8 = mMap_Direction_Calculations[Data4/2];
        Data4 += 0x80;
        Data4 &= 0x1FE;

        DataC = mMap_Direction_Calculations[Data4/2];
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

    } else {

        //loc_119C6
        dword_39F36 -= 0x8000;
        if (dword_39F36 < 0) {
            dword_39F36 = 0;
            word_3A054 = 1;
        }
    }

}

int16 cFodder::sub_119E1( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
    
    int16 Data10 = 0, Data14 = 0;

    pData0 -= pData8;
    pData4 -= pDataC;

    if (pData0 < 0)
        Data10 = 1;
    
    Data10 = -Data10;
    if (pData0 < 0 )
        pData0 = -pData0;


    if (pData4 < 0)
        Data14 = 1;

    Data14 = -Data14;
    if (pData4 < 0)
        pData4 = -pData4;

    pData8 = 0x0E;

    for (pData8 = 0x0E; pData8 >= 0; --pData8 ) {

        if (pData0 & (1 << pData8))
            break;

        if (pData4 & (1 << pData8))
            break;
    }

    pData8 -= 4;
    if (pData8 >= 0) {
        pData0 >>= pData8;
        pData4 >>= pData8;
    }
    pData4 <<= 5;
    pData4 |= pData0;

    pData4 = mMap_DirectionsBetweenPoints[pData4];
    if (pData4 < 0)
        return -1;

    pData4 <<= 1;
    if (Data10 < 0)
        goto loc_11AD2;

    if (Data14 < 0)
        goto loc_11ABC;

    pData0 = 0x80;
    pData0 -= pData4;
    pData4 = 0;
    return 0;

loc_11ABC:;
    pData0 = 0x80;
    pData0 += pData4;
    pData4 = 0;
    return 0;

loc_11AD2:;
    if (Data14 >= 0) {
        pData0 = 0x180;
        pData0 += pData4;
        pData4 = 0;
        return 0;
    }
//loc_11AEF
    pData0 = 0x180;
    pData0 -= pData4;
    pData4 = 0;
    return 0;
}

void cFodder::Camera_Calculate_Scroll() {
    
    mCamera_Speed_X += dword_39F4A;
    mCamera_Speed_Y += dword_39F4E;

    int32 Data0 = 1;

    if (mCamera_Speed_X < 0)
        Data0 = -1;

    if (Data0 != word_39FB6) {
        word_39FB6 = Data0;
        word_39FBA = 3;
    }

    if (word_39FBA)
        mCamera_Speed_X = 0;

    Data0 = 1;
    if (mCamera_Speed_Y < 0)
        Data0 = -1;

    if (Data0 != word_39FB8) {
        word_39FB8 = Data0;
        word_39FBC = 3;
    }

    if (word_39FBC)
        mCamera_Speed_Y = 0;

    //loc_11B9C
    Data0 = mMapWidth_Pixels;
    Data0 -= 0x110;
    Data0 = (Data0 << 16) | (Data0 >> 16);

    int32 Data4 = mCamera_Adjust_Col;
    Data4 += mCamera_Speed_X;

    if (Data4 > Data0) {
        Data0 -= mCamera_Adjust_Col;
        mCamera_Speed_X = Data0;
    }
    //loc_11BE8
    Data0 = mMapHeight_Pixels;
    Data0 -= mWindow->GetScreenSize().mHeight;

    Data0 = (Data0 << 16) | (Data0 >> 16);

    Data4 = mCamera_Adjust_Row + mCamera_Speed_Y;

    if (Data4 > Data0) {
        Data0 -= mCamera_Adjust_Row;
        mCamera_Speed_Y = Data0;
    }

    Data0 = mCamera_Speed_X;
    if (Data0 < 0)
        Data0 = -Data0;

    if (Data0 > dword_3A9FD) {
        Data0 = dword_3A9FD;

        if ((mCamera_Speed_X >> 16) < 0)
            Data0 = -Data0;

        mCamera_Speed_X = Data0;
    }
    //loc_11C6D
    Data0 = mCamera_Speed_Y;
    if (Data0 < 0)
        Data0 = -Data0;

    if (Data0 <= dword_3A9FD)
        return;

    Data0 = dword_3A9FD;
    if ((mCamera_Speed_Y >> 16) < 0)
        Data0 = -Data0;

    mCamera_Speed_Y = Data0;
}

void cFodder::sub_11CAD() {

    mCamera_Adjust_Col_High = mCamera_Adjust_Col >> 16;
    mCamera_Adjust_Row_High = mCamera_Adjust_Row >> 16;
    word_39F52 = 0;
    word_39F54 = 0;
    dword_39F4A &= 0x0000FFFF;
    dword_39F4E &= 0x0000FFFF;
}

void cFodder::Camera_Refresh( ) {
    int16 Data0 = mCamera_Adjust_Col >> 16;
    int16 Data4 = mCamera_Adjust_Row >> 16;

    int16 Data8 = mCamera_Position_X;
    Data8 -= 0x88;
    if (Data8 < 0)
        Data8 = 0;

    int16 Data10 = mMapWidth_Pixels;
    Data10 -= 0x88;
    if (Data8 >= Data10)
        Data8 = Data10;

    int16 DataC = mCamera_Position_Y;
    DataC -= 0x6C;
    if (DataC < 0)
        DataC = 0;

    Data10 = mMapHeight_Pixels;
    Data10 -= 0x6C;
    if (DataC >= Data10)
        DataC = Data10;

    if (!word_3ABB7) {
        Map_Get_Distance_BetweenPoints_Within_320(Data0,Data4,Data8,DataC);

        if (Data0 >= 0x8C)
            goto loc_11D8A;
    }
    //loc_11D62
    mCamera_Position_Column = mCamera_Position_X;
    mCamera_Position_Row = mCamera_Position_Y;

    Music_Play_Tileset();
    return;

loc_11D8A:;

    if (mMission_Completed_Timer)
        goto loc_11E5B;

    word_39FBA = 6;
    word_39FBC = 6;
    mInput_Enabled = 0;

    mCamera_Position_Column = mCamera_Position_X;
    mCamera_Position_Row = mCamera_Position_Y;
    word_39F52 = 0;
    dword_39F4A &= 0x0000FFFF;
    word_39F54 = 0;
    dword_39F4E &= 0x0000FFFF;
    
    Image_FadeOut();

    if (mMap_Destroy_Tiles_Countdown)
        Map_Destroy_Tiles();

    for (;;) {
        Camera_Pan_To_Target();
        Camera_Pan_To_Target();

        if (!mCamera_Reached_Target)
            break;
    }

    Mission_Sprites_Handle();
    mGraphics->Sidebar_Copy_To_Surface();
    Mouse_DrawCursor();
    sub_11CAD();

    mInput_Enabled = -1;

    mGraphics->PaletteSet();
    mImageFaded = -1;

loc_11E5B:;
    Music_Play_Tileset();
}

bool cFodder::Campaign_Load( std::string pName ) {
    
    // If no campaign name was provided, use the default for the current version
    if (!pName.size()) {

        if (mVersion->mVersion != eVersion::Custom) {
            pName = mVersion->mName;
        }
    }
    
    if (!mCampaign.LoadCampaign(pName, pName != mVersion->mName)) {
        // TODO

        return false;
    }

    return true;
}

void cFodder::Map_Create( const sTileType& pTileType, size_t pTileSub, const size_t pWidth, const size_t pHeight, const bool pRandomise) {
    uint8 TileID = (pTileType.mType == eTileTypes_Int) ? 4 : 0;

	if (mVersion->mVersion == AmigaPlus)
		pTileSub = 1;

	mMap = std::make_shared<std::vector<uint8_t>>();
    mMap->clear();
    mMap->resize(0x60 + ((pWidth * pHeight) * 2), TileID);
	
	mMapTile_Ptr = (0x60 - 8) - (pWidth * 2);
	mMapTile_Column = 0;
	mMapTile_Row = 0;

    uint16* Map = (uint16*) mMap->data();

    // Header
    {
        // Map Marker ('ofed')
        Map[0x28] = 'fo'; Map[0x29] = 'de';

        // Put the map size
        writeBEWord(&Map[0x2A], (uint16) pWidth);
        writeBEWord(&Map[0x2B], (uint16) pHeight);
    }

    // Tileset filenames
    {
        std::string mBaseName = pTileType.mName + "base.blk";
        std::string mSubName = pTileType.mName;

        // Only Jungle has a sub1
        if (pTileSub == 0 || pTileType.mType != eTileTypes_Jungle)
            mSubName.append("sub0.blk");
        else
            mSubName.append("sub1.blk");

        // Write the base/sub filenames
        std::memcpy(mMap->data(), mBaseName.c_str(), 11  );
        std::memcpy(mMap->data() + 16, mSubName.c_str(), 11);
    }

    // Clear current sprites
    Sprite_Clear_All();

    // Load the map specific resources
    Map_Load_Resources();

	if (pRandomise) {
		uint16 Seed = tool_RandomGet();

		// Lets store the seed for later
		Map[0x27] = Seed;

		Map_Randomise( Seed );
		Map_Randomise_Structures( 2 );
		Map_Randomise_Sprites();

#ifndef _OFED
		Map_Load_Sprites_Count();
#endif
	}
#ifdef _OFED
    // Draw the tiles
    MapTiles_Draw();

	Mission_Sprites_Handle();

    // Refresh the palette
    g_Graphics.PaletteSet(mImage);
    mImage->surfaceSetToPaletteNew();
#endif
}

void cFodder::Map_Load( ) {

	mMap = mCampaign.getMap(mMapNumber);

	if (!mMap->size())
		return;

	tool_EndianSwap(mMap->data() + 0x60, mMap->size() - 0x60);

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

    mTile_BaseBlk = g_Resource.fileGet(BaseName);
    mTile_SubBlk = g_Resource.fileGet(SubName);

    if (!mTile_BaseBlk->size() || !mTile_SubBlk->size())
        return false;

    return true;
}

void cFodder::Map_Load_Resources() {
    std::string BaseBase, BaseSub, BaseBaseSet, BaseSubSet;

    // Check Editor used
    switch (readBEDWord(mMap->data() + 0x50)) {
    default:        // Unknown Editor
    case 'cfed':    // Original Engine Map
        break;

    case 'ofed':    // Open Fodder Map
        break;
    }

    // Set the width/height in tiles
    mMapWidth = readBEWord(mMap->data() + 0x54);
    mMapHeight = readBEWord(mMap->data() + 0x56);

    // Calculate width/height in pixels
    mMapWidth_Pixels = (mMapWidth << 4);
    mMapHeight_Pixels = (mMapHeight << 4);

    // Map Tileset
    Map_SetTileType();

    // Was the tileset available?
    if (!Tiles_Load_Data()) {

        // Is the current version meant to have the required tileset?
        if (mVersion->hasTileset(mMap_TileSet)) {

            // TODO: Warn user about missing data?
        }
        
        // Load the default version
        VersionLoad(mVersionDefault);

        // Check the default version for the tileset
        if (!Tiles_Load_Data()) {

            // Not found, so lets go find it
            auto Version = FindAvailableVersionForTileset(mMap_TileSet);

            // Load it
            if (Version) {
                VersionLoad(Version);
                Tiles_Load_Data();
            } else {
                // TODO: Quit?
                std::cout << "Data not found\n";
            }
        }
        
    }

    // jun
    BaseBaseSet.append(mMap->data(), mMap->data() + 3);

    // jun
    BaseSubSet.append(mMap->data() + 0x10, mMap->data() + 0x10 + 3);

    // junbase
    BaseBase.append(mMap->data(), mMap->data() + 7);

    // junsub0
    BaseSub.append(mMap->data() + 0x10, mMap->data() + 0x10 + 7);


    mFilenameCopt = Filename_CreateFromBase( BaseBaseSet, "copt." );
    mFilenameBaseSwp = Filename_CreateFromBase( BaseBase, ".swp" );
    mFilenameBaseHit = Filename_CreateFromBase( BaseBase, ".hit" );
    mFilenameBaseBht = Filename_CreateFromBase( BaseBase, ".bht" );
    mFilenameArmy = Filename_CreateFromBase( BaseSubSet, "army." );
    mFilenameSubSwp = Filename_CreateFromBase( BaseSub, ".swp" );
    mFilenameSubHit = Filename_CreateFromBase( BaseSub, ".hit" );
    mFilenameSubBht = Filename_CreateFromBase( BaseSub, ".bht" );
    mFilenameBasePal  = Filename_CreateFromBase( BaseBase, ".pal" );


    size_t Size = g_Resource.fileLoadTo( mFilenameBaseSwp, (uint8*) &mTile_Destroy_Swap[0] );
    tool_EndianSwap( (uint8*)&mTile_Destroy_Swap[0], Size );
    
    Size = g_Resource.fileLoadTo( mFilenameSubSwp, (uint8*) &mTile_Destroy_Swap[240] );
    tool_EndianSwap( (uint8*)&mTile_Destroy_Swap[240], Size );

    Size = g_Resource.fileLoadTo( mFilenameBaseHit, (uint8*) &mTile_Hit[0] );
    tool_EndianSwap( (uint8*)&mTile_Hit[0], Size );
    
    Size = g_Resource.fileLoadTo( mFilenameSubHit, (uint8*) &mTile_Hit[240] );
    tool_EndianSwap( (uint8*)&mTile_Hit[240], Size );
    
    Size = g_Resource.fileLoadTo( mFilenameBaseBht, (uint8*) &mTile_BHit[0][0] );
    Size = g_Resource.fileLoadTo( mFilenameSubBht, (uint8*) &mTile_BHit[240][0] );

    mGraphics->Tile_Prepare_Gfx();
    mGraphics->Map_Load_Resources();
    mGraphics->PaletteSet();
}

void cFodder::Music_Play_Tileset() {

    mSound->Music_Play(mMap_TileSet + 0x32);
}

void cFodder::Camera_Pan_To_Target( ) {
    Camera_Pan_Set_Speed();

    int32 Saved_dword_3A9FD = dword_3A9FD;
    dword_3A9FD = 0x100000;
    Camera_Calculate_Scroll();
    dword_3A9FD = Saved_dword_3A9FD;

    sub_120F6();

    int16 Data0 = dword_39F24 >> 16;
    Data0 >>= 3;
    Data0 -= 2;

    if (Data0 < 0)
        Data0 += 0x28;

    Data0 &= 0x0FFFE;

    sub_12018();
    MapTile_Update_Position();
}

void cFodder::Camera_Pan_Set_Speed() {
    mCamera_Reached_Target = 2;
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;

    int16 Data0 = mCamera_Position_Column;
    Data0 -= 0x88;
    if (Data0 < 0)
        Data0 = 0;

    Data0 >>= 4;
    int16 Data4 = mCamera_Position_Row;
    Data4 -= 0x6C;
    if (Data4 < 0)
        Data4 = 0;

    Data4 >>= 4;

    int16 Data8 = mMapWidth;
    Data8 -= 0x12;
    if (Data8 < 0)
        Data8 = 0;

    if (Data0 >= Data8)
        Data0 = Data8;
    
    Data8 = mMapHeight;
    Data8 -= 0x10;
    if (Data8 < 0)
        Data8 = 0;

    if (Data4 >= Data8)
        Data4 = Data8;

    Data8 = mCamera_Adjust_Col >> 16;
    Data8 >>= 4;
    int16 DataC = mCamera_Adjust_Row >> 16;
    DataC >>= 4;

    if (Data8 == Data0) {
        --mCamera_Reached_Target;
    } 
    else if (Data8 > Data0) {
        mCamera_Speed_X = (mCamera_Speed_X & 0xFFFF) | (-8 << 16);
    } else 
        mCamera_Speed_X = (mCamera_Speed_X & 0xFFFF) | (8 << 16);

    //loc_11FAC
    if (DataC == Data4) {
        --mCamera_Reached_Target;
    } 
    else if (DataC > Data4) {
        mCamera_Speed_Y = (mCamera_Speed_Y & 0xFFFF) | (-8 << 16);
    } else 
        mCamera_Speed_Y = (mCamera_Speed_Y & 0xFFFF) | (8 << 16);
}

void cFodder::Camera_Update_From_Mouse() {
    
    if (!word_3A054) {

        // Mouse in playfield?
        if (mMouseX > 0x0F) {
            int16 Data0 = mCamera_Adjust_Col >> 16;
            Data0 -= mCamera_Adjust_Col_High;

            mMouseX -= Data0;
            Data0 = mCamera_Adjust_Row >> 16;
            Data0 -= mCamera_Adjust_Row_High;
            mMouseY -= Data0;
        }
    }

    //loc_12007
    mCamera_Adjust_Col_High = mCamera_Adjust_Col >> 16;
    mCamera_Adjust_Row_High = mCamera_Adjust_Row >> 16;
}

void cFodder::sub_12018() {
    int16 Data0 = mCamera_Adjust_Col >> 16;
    Data0 -= dword_39F84 >> 16;

    Data0 = mCamera_Adjust_Row >> 16;
    Data0 -= dword_39F88 >> 16;

    dword_39F8C = dword_39F24;
    dword_39F90 = dword_39F28;
    dword_39F84 = mCamera_Adjust_Col;
    dword_39F88 = mCamera_Adjust_Row;
    word_39FB2 = word_39FB6;
    word_39FB4 = word_39FB8;
}

void cFodder::Camera_Reset() {

    dword_39F24 = 0;
    dword_39F28 = 0;
    mCamera_Adjust_Col = 0;
    mCamera_Adjust_Row = 0;
    word_39F34 = 0;
    dword_39F36 &= 0x0000FFFF;
    dword_39F4A &= 0x0000FFFF;
    dword_39F4E &= 0x0000FFFF;
    word_39F52 = 0;
    word_39F54 = 0;
    mCamera_Adjust_Col_High = 0;
    mCamera_Adjust_Row_High = 0;
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;
    dword_39F56 = 0;
}

void cFodder::sub_120F6() {
    dword_39F56 = 0;

    dword_39F5A = mCamera_Adjust_Col;
    int32 Data0 = mCamera_Speed_X;

    mCamera_Adjust_Col += Data0;
    if (mCamera_Adjust_Col < 0) {
        mCamera_Speed_X = dword_39F5A;
        if (mCamera_Speed_X)
            mCamera_Speed_X = -mCamera_Speed_X;

        mCamera_Adjust_Col = 0;
    }
    //loc_12147
    dword_39F24 += mCamera_Speed_X;

    if ((mCamera_Speed_X >> 16) < 0) {

        if ((dword_39F24 >> 16) < 0) {

            dword_39F24 += (0x140 << 16);
            dword_39F56 = (0xFFFF << 16) | (dword_39F56 & 0xFFFF);
        }
    }
    else {
        //loc_12181
        if ((dword_39F24 >> 16) >= 0x140) {
            dword_39F24 -= (0x140 << 16);
            dword_39F56 = (1 << 16) | (dword_39F56 & 0xFFFF);
        }
    }
    //loc_1219F
    Camera_Adjust_Row( );
    mCamera_Speed_X = 0;
    mCamera_Speed_Y = 0;
}

void cFodder::Camera_Adjust_Row( ) {

    dword_39F5A = mCamera_Adjust_Row;

    int32 Data0 = mCamera_Speed_Y;
    mCamera_Adjust_Row += Data0;

    if (mCamera_Adjust_Row < 0) {
        mCamera_Speed_Y = dword_39F5A;

        if (mCamera_Speed_Y)
            mCamera_Speed_Y = -mCamera_Speed_Y;

        mCamera_Adjust_Row = 0;
    }

    //loc_121F2
    Data0 = mCamera_Speed_Y;
    Data0 += dword_39F56;

    dword_39F28 += Data0;
    dword_39F28 &= (0x00FF << 16) | 0xFFFF;
}

void cFodder::Camera_Mission_Over_Check() {
    
    dword_3A9FD = 0x20000;

    if (!mMission_Aborted && !mMission_TryAgain && !mMission_Complete)
        return;

    int16 Data0 = 0;

    dword_39F4A = (dword_39F4A & 0xFFFF) + (Data0 << 16);
    dword_39F4E = (dword_39F4E & 0xFFFF) + (Data0 << 16);
    mCamera_Speed_X = Data0;
    mCamera_Speed_Y = Data0;
    dword_3A9FD = 0;
}

void cFodder::Camera_Speed_Reset() {
    
    if (word_39F52)
        mCamera_Speed_X = 0;

    if (word_39F54)
        mCamera_Speed_Y = 0;
}

void cFodder::Camera_Pan_SetSpeed() {

    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0 )
        return;

    int16 Data4 = mSquad_Leader->field_0 + 0x40;
    int16 Data8 = mSquad_Leader->field_4 + 0x10;

    Data4 -= mCamera_Adjust_Col >> 16;
    Data8 -= mCamera_Adjust_Row >> 16;

    if (word_39F52 && Data4 > 0x90 && Data4 < 0xCF) {
        word_39F52 = 0;
        dword_39F4A = dword_39F4A & 0xFFFF;
    }

    if (word_39F54 && Data8 > 0x60 && Data8 < 0x87) {
        word_39F54 = 0;
        dword_39F4E &= 0xFFFF;
    }
    //loc_1234F
    if (Data4 <= 0x40) {
        dword_39F4A = (dword_39F4A & 0xFFFF) | (-2 << 16);
        word_39F52 = -1;
    }
    //loc_12362
    if (Data4 >= 0x150) {
        dword_39F4A = (dword_39F4A & 0xFFFF) | (2 << 16);
        word_39F52 = -1;
    }

    if (Data8 <= 0x10) {
        dword_39F4E = (dword_39F4E & 0xFFFF) | (-2 << 16);
        word_39F54 = -1;
    }

    if (Data8 >= 0xD8) {
        dword_39F4E = (dword_39F4E & 0xFFFF) | (2 << 16);
        word_39F54 = -1;
    }
}

void cFodder::Mission_Sprites_Handle( ) {

    
#ifndef _OFED
    Sprite_Frame_Modifier_Update();
    Sprite_Handle_Loop();
#endif

    Sprite_Sort_DrawList();

    int16 Data0 = mCamera_Adjust_Col >> 16;
    Data0 -= dword_39F84 >> 16;


    Data0 = mCamera_Adjust_Row >> 16;
    Data0 -= dword_39F88 >> 16;

    dword_39F84 = mCamera_Adjust_Col;
    dword_39F88 = mCamera_Adjust_Row;
    dword_39F8C = dword_39F24;
    dword_39F90 = dword_39F28;
    word_39FB2 = word_39FB6;
    word_39FB4 = word_39FB8;

    Map_Destroy_Tiles();
    Sprites_Draw();
}

void cFodder::Sprite_Sort_DrawList() {
    sSprite* si = mSprites;
    sSprite** di = mSprite_DrawList_First;
    sSprite** bx = mSprite_DrawList_Second;
    sSprite** bp = mSprite_DrawList_Third;

    for (int16 cx = 0x2B; cx > 0; --cx, ++si) {

        if (si->field_0 == -32768)
            continue;

        if (si->field_2C == eSprite_Draw_First) {
            *di++ = si;
            continue;
        }

        if (si->field_2C == eSprite_Draw_Second) {
            *bx++ = si;
            continue;
        }

        *bp++ = si;
    }

    *di = INVALID_SPRITE_PTR;
    *bx = INVALID_SPRITE_PTR;
    *bp = INVALID_SPRITE_PTR;

    sSprite** Si = mSprite_DrawList_Second - 1;
    //seg000:247C
    for (;;) {
        ++Si;
        if (*Si == INVALID_SPRITE_PTR)
            break;

        di = Si;
        --di;

    loc_1248D:;
        ++di;
        if (*di == INVALID_SPRITE_PTR)
            continue;

        int16 cx = (*di)->field_4;
        int16 dx = (*Si)->field_4;

        if (cx > dx)
            goto loc_1248D;

        sSprite* eax = *di;
        *di = *Si;
        *Si = eax;
        goto loc_1248D;
    }

    //loc_124AF
    di = mSprite_DrawList_Final;
    Si = mSprite_DrawList_First;

    for(;;) {
        sSprite* ax = *Si++;
        if (ax == INVALID_SPRITE_PTR)
            break;

        *di++ = ax;
    }

    Si = mSprite_DrawList_Second;
    for(;;) {
        sSprite* ax = *Si++;
        if (ax == INVALID_SPRITE_PTR)
            break;

        *di++ = ax;
    }

    Si = mSprite_DrawList_Third;
    for(;;) {
        sSprite* ax = *Si++;
        *di++ = ax;
        
        if (ax == INVALID_SPRITE_PTR)
            break;
    }
}

void cFodder::Sprite_Bullet_SetData() {
    if (mSprite_TroopsAlive[0] == INVALID_SPRITE_PTR || mSprite_TroopsAlive[0] == 0)
        return;

    sSprite* Data20 = mSprite_TroopsAlive[0];
    int16 Data0 = 0;

    Data0 = Data20->field_46_mission_troop->mRank;
    Data0 += 8;
    if (Data0 > 0x0F)
        Data0 = 0x0F;

    mSprite_Weapon_Data = mSprite_Bullet_UnitData[Data0];
}

void cFodder::Mission_Phase_Goals_Check() {

    sSprite* Data20 = mSprites;
    int16 Data8 = 0;
    int16 DataC = 0;
    int16 Data0 = 0x2B;
    
    for(Data0 = 0x2B; Data0 >= 0; --Data0, ++Data20 ) {
        
        if( Data20->field_0 == -32768)
            continue;

        int16 Data10 = Data20->field_18;
        if(Data10 == eSprite_Computer_1 || Data10 == eSprite_Computer_2 || Data10 == eSprite_Computer_3 || Data10 == eSprite_BuildingDoor3 || Data10 == 0x64 )
            goto loc_12620;
        
        if (Data10 == eSprite_BuildingDoor2 || Data10 == eSprite_BuildingDoor) {

            if (Data20->field_38 == eSprite_Anim_Die1)
                continue;

        loc_12620:;
            if (Data20->field_38 == eSprite_Anim_Die3)
                continue;

            ++DataC;
            continue;
        }

        const int16* Data24 = mEnemy_Unit_Types;
        
        for (; *Data24 >= 0; ++Data24){

            if (Data10 != *Data24)
                continue;

            ++Data8;
        }
    }
    
    mEnemy_BuildingCount = DataC;
    if(mPhase_Goals[ eGoal_Destroy_Enemy_Buildings - 1 ]) {
        if( DataC )
            return;
    }
    
    if(mPhase_Goals[ eGoal_Kill_All_Enemy - 1 ]) {
        if(Data8)
            return;
    }
    
    if(!mPhase_Goals[ eGoal_Kidnap_Leader - 1 ]) {
        if(!mPhase_Goals[ eGoal_Rescue_Hostages - 1 ])
            goto loc_126A6;
    }
    
    if( mHostage_Count )
        return;
    
    loc_126A6:;
    if(!mPhase_Goals[ eGoal_Get_Civilian_Home - 1 ] || mSprite_Civilian_GotHome ) 
         mMission_Complete = -1;
}

void cFodder::Map_Clear_Destroy_Tiles() {
    mMap_Destroy_TilesPtr2 = mMap_Destroy_Tiles;
    mMap_Destroy_TilesPtr = mMap_Destroy_Tiles;

    mMap_Destroy_Tiles[0].mX = -1;
    mMap_Destroy_Tiles[0].mY = -1;
}

void cFodder::Mission_Phase_Goals_Set() {

    for( auto Goal : mCampaign.getMapGoals( mMapNumber )) {

        mPhase_Goals[Goal -1] = -1;
    }
}

void cFodder::Mission_Progress_Check( ) {
    
    if (mMission_Complete >= 0)
        if (!mMission_Aborted)
            if (!mMission_TryAgain)
                return;

    if (mMission_Completed_Timer < 0)
        return;

    if (mMission_Completed_Timer)
        goto loc_1280A;

    mMission_Completed_Timer = 0x64;
    if (mMission_Aborted) {
        mMission_Completed_Timer = 0x32;
        goto MissionTryAgain;
    }

    if (mMission_Complete) {
        if (mMission_TryAgain)
            goto MissionTryAgain;

        Mission_Text_Completed();
        goto loc_1280A;
    }
    //loc_127E1
    if (mRecruits_Available_Count) {
    MissionTryAgain:;

        Mission_Text_TryAgain();

	} else {
		//loc_127FA
		Mission_GameOver();
	}
loc_1280A:;

    if (mMission_Completed_Timer == 0x19) {
        mImageFaded = -1;
        mImage->paletteNew_SetToBlack();
    }
    --mMission_Completed_Timer;
    if (mMission_Completed_Timer)
        return;

    mMission_Completed_Timer = -1;
    mMission_IsFinished = -1;
}

void cFodder::Mission_Text_Completed() {
    Sprite_Destroy( &mSprites[40] );

    if (mMission_Phases_Remaining == 1)
        Mission_Text_Sprite_Mission( &mSprites[41] );
    else
        Mission_Text_Sprite_Phase( &mSprites[41] );

    Mission_Text_Sprite_Complete(  &mSprites[42] );
}

void cFodder::Mission_Text_Sprite_Mission( sSprite* pData2C ) {
    Mission_Text_Prepare( pData2C );

    pData2C->field_4 -= 0x14;
    pData2C->field_0 += 0x12;
    pData2C->field_8 = 0xA2;
    pData2C->field_18 = eSprite_Text_Mission;

    mSound->Music_Play( 6 );
}

void cFodder::Mission_Text_Sprite_Phase( sSprite* pData2C ) {
    Mission_Text_Prepare( pData2C );

    pData2C->field_4 -= 0x14;
    pData2C->field_0 += 0x1B;
    pData2C->field_8 = 0xA1;
    pData2C->field_18 = eSprite_Text_Phase;

    mSound->Music_Play( 0x0C );
}

void cFodder::Mission_Text_Sprite_Complete( sSprite* pData2C ) {
    Mission_Text_Prepare( pData2C );

    pData2C->field_8 = 0xA0;
    pData2C->field_18 = eSprite_Text_Complete;
}

void cFodder::Mission_Text_Prepare( sSprite* pData2C ) {

    pData2C->field_0 = dword_39F84 >> 16;
    pData2C->field_0 += 0x54;

    pData2C->field_4 = dword_39F88 >> 16;
    pData2C->field_4 += 0xEB;

    pData2C->field_A = 0;
    pData2C->field_20 = 0;
    pData2C->field_52 = 0;
    pData2C->field_32 = -1;
    pData2C->field_2C = eSprite_Draw_Last;
}

void cFodder::Mission_Text_TryAgain() {

    Sprite_Destroy( &mSprites[40] );

    Mission_Text_Sprite_Try(  &mSprites[41] );
    Mission_Text_Sprite_Again(  &mSprites[42] );

    mSound->Music_Play(0x0F);
}

void cFodder::Mission_Text_Sprite_Try( sSprite* pData2C ) {
    Mission_Text_Prepare( pData2C );
    pData2C->field_4 -= 0x14;
    pData2C->field_0 += 0x12;
    pData2C->field_8 = 0xCA;
    pData2C->field_18 = eSprite_Text_Try;
}

void cFodder::Mission_Text_Sprite_Again( sSprite* pData2C ) {
    Mission_Text_Prepare( pData2C );

    pData2C->field_8 = 0xCB;
    pData2C->field_18 = eSprite_Text_Again;
}

std::string cFodder::Filename_CreateFromBase( const std::string& pBase, const char* pFinish ) {
    std::string Final;

    Final.append( pBase );
    Final.append( pFinish );

    return Final;
}

void cFodder::Squad_Member_PhaseCount() {
    sMission_Troop* Data20 = mMission_Troops;

    for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
        if (Data20->mSprite == INVALID_SPRITE_PTR || Data20->mSprite == 0 )
            continue;

        ++Data20->mPhaseCount;
    }
}

void cFodder::Sprite_Find_HumanVehicles() {
    
    if(mSprite_HumanVehicles_Found)
        return;
    
    mSprite_HumanVehicles_Found = -1;
    sSprite* Data20 = mSprites;
    sSprite** Data24 = mSprites_HumanVehicles;

    for( int16 Data1C = 0x1D; Data1C >= 0; --Data1C, ++Data20 ) {
        
        if( Data20->field_0 == -32768)
            continue;
        
        // Is not Human?
        if( Data20->field_22 != eSprite_PersonType_Human )
            continue;
        
        // Not Enabled?
        if( !Data20->field_65 )
            continue;
        
        *Data24 = Data20;
        ++Data24;
    }
    
    *Data24 = INVALID_SPRITE_PTR;
    ++Data24;

}

void cFodder::Squad_Set_CurrentVehicle() {

    if (mSquad_Selected >= 0) {
        sSprite** Data20 = mSquads[mSquad_Selected];

        mSquad_CurrentVehicles[mSquad_Selected] = 0;

        int16 Data0 = 0;

        for (;;) {
            if (*Data20 != INVALID_SPRITE_PTR && *Data20 != 0 ) {
                sSprite* Data24 = *Data20++;

                // Sprite not in vehicle?
                if (!Data24->field_6E)
                    break;

                mSquad_CurrentVehicle = Data24->field_6A_sprite;
                Data0 = -1;
                continue;

            } else {
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
            if (*Data2C == INVALID_SPRITE_PTR )
                break;

            sSprite* Data30 = *Data2C++;
            Data30->field_38 = eSprite_Anim_Vehicle_Enter;
        }
    }
}

void cFodder::Mission_Map_Overview_Show() {

    // Overview map is disabled for demos
    if ( mVersion->mRelease == eRelease::Demo && 
         mVersion->mVersion != eVersion::Custom)
        return;

    int16 word_3A016 = 0;


    mVideo_Draw_PosX =  (mSquad_Leader->field_0) + (mSurfaceMapLeft * 16);
    mVideo_Draw_PosY =  (mSquad_Leader->field_4 - 0x10) + (mSurfaceMapTop * 16);
    mVideo_Draw_PaletteIndex = 0xF0;

    mGraphics->PaletteSetOverview();
    mSurfaceMapOverview->Save();
    mGraphics->SetImage( mSurfaceMapOverview );

    do {
        if (mSquad_Leader != INVALID_SPRITE_PTR) {
            ++word_3A016;
            word_3A016 &= 0x3F;

            if (word_3A016 < 0x20) {
                mVideo_Draw_FrameDataPtr = mGraphics->GetSpriteData(eGFX_PSTUFF);
                mVideo_Draw_Rows = 0x10;

                if (mVersion->mPlatform == ePlatform::PC) {
                    mVideo_Draw_FrameDataPtr += 0x46B8;
                    mVideo_Draw_Columns = 0x10;
                } 
                else {
                    mVideo_Draw_FrameDataPtr += (113 * 40) + 6;
                    mVideo_Draw_Columns = 0x2;
                }

                mGraphics->Video_Draw_8();
            } 
            else
                mSurfaceMapOverview->Restore();
        }

        g_Window.RenderShrunk( mSurfaceMapOverview );
        g_Window.FrameEnd();
        
        Mouse_Inputs_Get();

        if (mMission_Aborted)
            break;

    } while (Mouse_Button_Left_Toggled() < 0);

    mSurfaceMapOverview->Restore();
    mGraphics->SetImageOriginal();

    mMission_Aborted = false;
}

void cFodder::Map_Overview_Prepare() {
    // Overview already rendered?
    //if (mMapNumber == mMap_Overview_MapNumberRendered)
    //  return;

    delete mSurfaceMapOverview;
    size_t Size = mMapWidth < mMapHeight ? mMapHeight : mMapWidth;

    mSurfaceMapOverview = new cSurface( Size * 16, Size * 16 );
    mSurfaceMapOverview->clearBuffer();

    mMap_Overview_MapNumberRendered = mMapNumber;

    int16* MapPtr = (int16*) (mMap->data() + 0x60);

    mSurfaceMapTop = mSurfaceMapLeft = 0;

    if (mMapHeight < mMapWidth) {
        mSurfaceMapTop = (mMapWidth / 2) - (mMapHeight/2);
        if (mSurfaceMapTop < 0)
            mSurfaceMapTop = 0;
    }

    if (mMapWidth < mMapHeight) {
        mSurfaceMapLeft = (mMapHeight / 2) - (mMapWidth/2);
        if (mSurfaceMapLeft < 0)
            mSurfaceMapLeft = 0;
    }

    for (uint16 dx = 0; dx < mMapHeight; ++dx) {

        for (uint16 cx = 0; cx < mMapWidth; ++cx) {

            mGraphics->MapOverview_Render_Tiles( *MapPtr & 0x1FF, cx + mSurfaceMapLeft, dx + mSurfaceMapTop );
            
            ++MapPtr;
        }
    }

}

void cFodder::Map_SetTileType() {

    for(auto& TileType : mTileTypes) {

        if (TileType.mName[0] != mMap->data()[0])
            continue;
    
        if (TileType.mName[1] != mMap->data()[1])
            continue;

        if (TileType.mName[2] != mMap->data()[2])
            continue;

        mMap_TileSet = TileType.mType;
        return;
    }

    // Fallback to Jungle
    mMap_TileSet = eTileTypes_Jungle;
}

void cFodder::Briefing_Set_Render_1_Mode_On() {

    mBriefing_Render_1_Mode = -1;
}

bool cFodder::EventAdd( cEvent pEvent ) {

    mEvents.push_back( pEvent );

    return true;
}

void cFodder::eventProcess() {
    g_Window.EventCheck();

    for (auto Event : mEvents) {

        switch (Event.mType) {
            
            case eEvent_KeyDown:
                keyProcess( Event.mButton, false );
                break;

            case eEvent_KeyUp:
                keyProcess( Event.mButton, true );
                break;

            case eEvent_MouseLeftDown:
                mMousePosition = Event.mPosition;
                mMouseButtons |= 1;
                break;

            case eEvent_MouseRightDown:
                mMousePosition = Event.mPosition;
                mMouseButtons |= 2;
                break;

            case eEvent_MouseLeftUp:
                mMousePosition = Event.mPosition;
                mMouseButtons &= ~1;
                break;

            case eEvent_MouseRightUp:
                mMousePosition = Event.mPosition;
                mMouseButtons &= ~2;
                break;

            case eEvent_MouseMove:
                mMousePosition = Event.mPosition;
                break;

            case eEvent_None:
            case eEvent_MouseWheelUp:
            case eEvent_MouseWheelDown:
                break;
            
            case eEvent_Quit:
                Exit( 0 );
                break;
        }

    }

    mEvents.clear();
}

void cFodder::keyProcess( uint8 pKeyCode, bool pPressed ) {
    if (pPressed)
        mKeyCode = pKeyCode;
    else
        mKeyCode = 0;

    if (pKeyCode == SDL_SCANCODE_EQUALS && pPressed)
        mWindow->WindowIncrease();

    if (pKeyCode == SDL_SCANCODE_MINUS && pPressed)
        mWindow->WindowDecrease();

    if (pKeyCode == SDL_SCANCODE_F11 && pPressed)
        mWindow->SetFullScreen();

    if (pKeyCode == SDL_SCANCODE_ESCAPE && pPressed)
        mMission_Aborted = true;

    // In Mission and not on map overview
    if (mMission_In_Progress && !mMission_ShowMapOverview) {

        if (pKeyCode == SDL_SCANCODE_LCTRL || pKeyCode == SDL_SCANCODE_RCTRL) {
            if (pPressed)
                mKeyControlPressed = -1;
            else
                mKeyControlPressed = 0;
        }

        if (pKeyCode == SDL_SCANCODE_P && pPressed)
            mMission_Paused = ~mMission_Paused;

        if (pKeyCode == SDL_SCANCODE_SPACE && pPressed)
            ++mSquad_SwitchWeapon;

        if (pKeyCode == SDL_SCANCODE_M && pPressed) {
            if (mMission_Finished == 0)
                mMission_ShowMapOverview = -1;
        }

        if (pKeyCode == SDL_SCANCODE_1)
            mKeyNumberPressed = 2;

        if (pKeyCode == SDL_SCANCODE_2)
            mKeyNumberPressed = 3;

        if (pKeyCode == SDL_SCANCODE_3)
            mKeyNumberPressed = 4;

        // Debug: Mission Complete
        if (pKeyCode == SDL_SCANCODE_F10 && pPressed ) {
            mDebug_MissionSkip = -1;
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

void cFodder::Mouse_Setup() {
    
    mMouseButtons = 0;
    mButtonPressLeft = 0;
    mButtonPressRight = 0;
    mouse_Button_Status = 0;

    mMouseX = 0x7F;
    mMouseY = 0x67;

    //g_Window.SetMousePosition( cPosition(g_Window.GetWindowSize().mWidth / 2, g_Window.GetWindowSize().mHeight / 2 ) );
}

void cFodder::Mouse_GetData() {
    float scaleX = ((float)mWindow->GetWindowSize().mWidth / mWindow->GetScreenSize().mWidth);
    float scaleY = ((float)mWindow->GetWindowSize().mHeight  / mWindow->GetScreenSize().mHeight);

    eventProcess();

    mouse_Pos_Column = (int16) (mMousePosition.mX / scaleX) - 32;
    mouse_Pos_Row = (int16) (mMousePosition.mY / scaleY) + 4;

    mouse_Button_Status = mMouseButtons;
}

void cFodder::Mouse_Inputs_Get() {
    
    Mouse_GetData();
    Mouse_ButtonCheck();

    int16 Data4 = mouse_Pos_Column;

    if( word_3A024 == 0 )
        goto loc_13B3A;
    
    if( Data4 >= -16 )
        goto loc_13B58;
    
    goto loc_13B41;
    
loc_13B3A:;
    if( Data4 >= -32 )
        goto loc_13B58;
    
loc_13B41:;
    if(word_3A024)
        Data4 = -16;
    else 
        Data4 = -32;
    
    goto loc_13B66;
    
loc_13B58:;

    if(Data4 > 287 )
        Data4 = 287;
    
loc_13B66:;
    mMouseX = Data4;
    
    int16 Data0 = mouse_Pos_Row;

    if (Data0 < 4)
        Data0 = 4;
    else {

        if (Data0 > (int16) mWindow->GetScreenSize().mHeight + 3)
            Data0 = (int16) mWindow->GetScreenSize().mHeight + 3;
    }

    mMouseY = Data0;
}

void cFodder::Mouse_ButtonCheck() {

    mButtonPressLeft = 0;
    if (mouse_Button_Status & 1) {
        mButtonPressLeft -= 1;
        if (mMouse_Button_Left_Toggle == 0) {
            mMouse_Button_Left_Toggle = -1;
            mMouse_Exit_Loop = -1;

            if (mButtonPressRight) {
                mMouse_Button_LeftRight_Toggle = -1;
                mMouse_Button_LeftRight_Toggle2 = -1;
            }
        }

    } else {
        mMouse_Button_LeftRight_Toggle2 = 0;
        mMouse_Button_Left_Toggle = 0;
    }

    mButtonPressRight = 0;
    if (mouse_Button_Status & 2) {
        mButtonPressRight -= 1;
        if (mMouse_Button_Right_Toggle == 0) {
            word_39F04 = -1;
            mMouse_Button_Right_Toggle = -1;
        }
    } else {
        mMouse_Button_Right_Toggle = 0;
    }

}

void cFodder::WindowTitleSet( bool pInMission ) {
    std::stringstream Title;
    Title << mTitle.str();

    if (pInMission) {
        if (mVersion->mRelease == eRelease::Demo) {

            Title << " ( Mission: ";

            Title << mCampaign.getMapName(mMapNumber);

        } else {
            Title << " ( Mission: " << mMissionNumber;
            Title << " " << mCampaign.getMissionName(mMissionNumber);

            Title << "  Phase: " << (mMissionPhase + 1) << " ";

            if (mMission_Phases_Total > 1) {
                Title << "of " << mMission_Phases_Total;
                Title << " " << mCampaign.getMapName(mMapNumber);
            }
            else
                Title << mCampaign.getMapName(mMapNumber);
        }

        Title << " )";
    }

    mWindow->SetWindowTitle( Title.str() );
}

void cFodder::VersionCleanup() {
    delete mGraphics;
    delete mResources;
    delete mSound;

    mGraphics = 0;
    mResources = 0;
    mSound = 0;
}

void cFodder::WindowTitleBaseSetup() {

    mTitle.str( "" );
    mTitle << "Open Fodder";

    if (mVersion) {

        // Ensure we have a campaign
        if (mCampaign.getName().size()) {
            if (mCampaign.isCustom()) {
                mTitle << ": Custom (" << mCampaign.getName() << ")";
            }
            else {

                mTitle << ": " << mCampaign.getName();
            }
        }
    }

    WindowTitleSet(false);
}

/**
 * This function loads a new version of the game, and is generally called on startup,
 * or AFTER a button on the version select screen is pushed
 *
 */
void cFodder::VersionLoad( const sVersion* pVersion ) {

    if (!pVersion)
        return;

    auto DataPath = pVersion->mDataPath;

    // Custom version?
    if (pVersion->mVersion == eVersion::Custom) {
        auto RetailRelease = FindAvailableRetail();

        // If a retail release is found, we use its data path
        if (RetailRelease != 0)
            DataPath = RetailRelease->mDataPath;
        else {
            std::cout << "Retail release not found";
            return;
        }
    }

    if (mVersion == pVersion)
        return;

    mVersion = pVersion;

    WindowTitleBaseSetup();

    Image_FadeOut();

    VersionCleanup();

    switch (mVersion->mPlatform) {
        case ePlatform::PC:
            mResources = new cResource_PC_CD( DataPath );
            mGraphics = new cGraphics_PC();

            if (mVersion->mGame == eGame::CF1)
                mSound = new cSound_PC();
            
            if (mVersion->mGame == eGame::CF2)
                mSound = new cSound_PC2();

            mWindow->SetScreenSize( cDimension( 320, 200 ) );
            mWindow->SetOriginalRes( cDimension( 320, 200 ) );

            break;

        case ePlatform::Amiga:
            mResources = new cResource_Amiga_File( DataPath );
            mGraphics = new cGraphics_Amiga();
            mSound = new cSound_Amiga();

            ((cGraphics_Amiga*)mGraphics)->SetCursorPalette( 0xE0 );

            mWindow->SetScreenSize( cDimension( 320, 225 ) );
            mWindow->SetOriginalRes( cDimension( 320, 225 ) );
            break;
    }

    mGraphics->SetActiveSpriteSheet( eGFX_IN_GAME );
    mGraphics->Load_pStuff(); 
    mGraphics->Load_Hill_Data(); 
}

void cFodder::Prepare( ) {
    FindFodderVersions();

    if (g_AvailableDataVersions.empty()) {
        std::cout << "No data files found\n";
        exit( 1 );
    }

    mWindow->InitWindow( "Open Fodder" );

    tool_RandomSeed();

    mTile_BaseBlk = tSharedBuffer();
    mTile_SubBlk = tSharedBuffer();

    mMap = tSharedBuffer();

    mSidebar_Back_Buffer = (uint16*) new uint8[0x4000];
    mSidebar_Screen_Buffer = (uint16*) new uint8[0x4000];
    mSidebar_Screen_BufferPtr = mSidebar_Screen_Buffer;

    uint8* Start = (uint8*) &mMapNumber;
    uint8* End = (uint8*)&mButtonPressLeft;

    mMission_Memory_Backup = new uint8[ End - Start ];
    Briefing_Set_Render_1_Mode_On();

    mImage = new cSurface( 352, 364 );
}

void cFodder::Sprite_Count_HelicopterCallPads() {
    
    sSprite* Data20 = mSprites;
    int16 Data0 = 0;
    for (int16 Data4 = 0x1D; Data4 >= 0; Data4-- ) {

        if (Data20->field_18 == eSprite_Helicopter_CallPad)
            ++Data0;

        ++Data20;
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

    sSprite* Data2C = mTroop_InRange_Callpad;
    if (!mTroop_InRange_Callpad || mTroop_InRange_Callpad == INVALID_SPRITE_PTR)
        return;

    mHelicopterCall_X = mTroop_InRange_Callpad->field_0;
    mHelicopterCall_Y = mTroop_InRange_Callpad->field_4;

    mHelicopterCall_Y += 0x28;
}

void cFodder::Mission_Final_Timer() {
    if (mMapNumber != 0x47)
        return;

    if (mMission_Final_TimeRemain)
        return;

    if (!mMission_Final_TimeToAbort)
        mMission_Final_TimeToAbort = 0x28;

    --mMission_Final_TimeToAbort;
    if (!mMission_Final_TimeToAbort)
        mMission_Aborted = true;

    if (!(mMission_EngineTicks & 3))
        Sprite_Create_RandomExplosion();
}

int16 cFodder::Sprite_Create_RandomExplosion() {
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free( Data0, Data2C, Data30 )) {
        Data0 = -1;
        return -1;
    }

    Data2C->field_0 = dword_39F84 >> 16;
    Data0 = tool_RandomGet() & 0xFF;
    Data0 += 0x0A;

    Data2C->field_0 += Data0;
    Data2C->field_4 = dword_39F88 >> 16;

    Data0 = tool_RandomGet() & 0xFF;
    Data0 -= 0x14;

    Data2C->field_4 += Data0;
    Data2C->field_18 = eSprite_Explosion;
    return Data0;
}

void cFodder::Mission_Paused() {

    mImage->paletteNew_SetToBlack();
    mImage->palette_FadeTowardNew();
    mImageFaded = -1;

    mGraphics->SetActiveSpriteSheet( eGFX_BRIEFING );
    
    mString_GapCharID = 0x25;
    String_Print_Large( "GAME PAUSED", true, 0x54 );
    
    mString_GapCharID = 0;
    mGraphics->SetActiveSpriteSheet( eGFX_IN_GAME );
}

void cFodder::Mission_GameOver() {
    sSprite* Data24 = &mSprites[40];
    Sprite_Destroy( Data24 );

    sSprite* Data2C = &mSprites[41];
    Mission_Text_GameOver( Data2C );
}

void cFodder::Mission_Text_GameOver( sSprite* pData2C ) {
    Mission_Text_Prepare( pData2C );
    pData2C->field_8 = 0xC1;
    pData2C->field_18 = eSprite_Text_GameOver;

    mSound->Music_Play(8);
}

void cFodder::Mouse_DrawCursor( ) {

    mVideo_Draw_PosX = (mMouseX + mMouseX_Offset) + 48;
    mVideo_Draw_PosY = (mMouseY + mMouseY_Offset) + 12;

    if (mMouseSpriteNew >= 0) {
        mMouseSpriteCurrent = mMouseSpriteNew;
        mMouseSpriteNew = -1;
    }

    if(mGraphics)
        mGraphics->Mouse_DrawCursor();
}

void cFodder::GUI_Draw_Frame_8( int32 pSpriteType, int32 pFrame, const size_t pPositionX, const size_t pPositionY ) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_PosX = (int16) (pPositionX + 0x10);
    mVideo_Draw_PosY = (int16) (pPositionY + 0x10);
    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount;

    if (SheetData->mRowCount > mGUI_Draw_LastHeight)
        mGUI_Draw_LastHeight = SheetData->mRowCount;
    
    if (Sprite_OnScreen_Check() )
        mGraphics->Video_Draw_8();

}

void cFodder::GUI_Draw_Frame_16( int16 pSpriteType, int16 pFrame, const size_t pPosX, const size_t pPosY ) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_PosX = (int16) (pPosX + 0x10);
    mVideo_Draw_PosY = (int16) (pPosY + 0x10);
    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount;
    mVideo_Draw_ColumnsMax = 0x140;

    if (Sprite_OnScreen_Check())
        mGraphics->Video_Draw_16();
}

void cFodder::Sprite_Draw_Frame( sSprite* pDi, int16 pSpriteType, int16 pFrame, cSurface *pDestination) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();
    
    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount - pDi->field_52;

#ifdef _OFED

    mMapTile_Row = (mMapTile_MovedVertical * 16) + mMapTile_RowOffset;
    mMapTile_Column = (mMapTile_MovedHorizontal * 16) + mMapTile_ColumnOffset;
#endif

    mVideo_Draw_PosX = (int16) (SheetData->mModX + pDi->field_0) - mMapTile_Column + 0x40;
    mVideo_Draw_PosY = (int16) (SheetData->mModY + pDi->field_4) - mVideo_Draw_Rows - pDi->field_20 - mMapTile_Row;
    mVideo_Draw_PosY += 0x10;

    if (Sprite_OnScreen_Check()) {
        pDi->field_5C = 1;
        mGraphics->Video_Draw_8(pDestination);
    } else 
        pDi->field_5C = 0;
}

bool cFodder::Sprite_OnScreen_Check() {

    return mGraphics->Sprite_OnScreen_Check();
}

void cFodder::Sprites_Draw( ) {
    sSprite** si = mSprite_DrawList_Final;

    for (;;) {
        sSprite* eax = *si++;
        if (eax == INVALID_SPRITE_PTR || eax == 0 )
            break;

        if (eax->field_24) {
            eax->field_24 = 0;
            eax->field_0 = -32768;
            mSprite_SpareUsed = 0;
            mSprite_SpareUsed2 = 0;
        }
        else {
            int16 Data0 = eax->field_8;
            int16 Data4 = eax->field_A;
            Sprite_Draw_Frame( eax, Data0, Data4 );
        }
    }
}

void cFodder::Sprite_Map_Sound_Play( int16 &pData0 ) {

    mSoundEffectToPlay_Set = pData0;

    if (mSquad_Leader == INVALID_SPRITE_PTR )
         return;

    goto loc_14D66;
    //seg002:05B0
    // UNUSED BLOCK

loc_14D66:;
    mSoundEffectToPlay = mSoundEffectToPlay_Set;
}

void cFodder::Sound_Play( sSprite* pSprite, int16 pSoundEffect, int16 pData8 ) {
    int16 Data0 = 0;

    int8 al = byte_3A9DA[0];
    if (pData8 >= al) {
        byte_3A9DA[0] = pData8 & 0xFF;
    } else {
        al = byte_427E6;
        al |= byte_427EE;
        if (al)
            return;

        al = pData8 & 0xFF;
        byte_3A9DA[0] = al;
    }
    //loc_14BD4
    pData8 = mCamera_Adjust_Col >> 16;
    pData8 += 0x88;

    if (pSprite != INVALID_SPRITE_PTR)
        pData8 -= pSprite->field_0;

    int16 DataC = mCamera_Adjust_Row >> 16;
    DataC += 0x6C;

    if (pSprite != INVALID_SPRITE_PTR)
        DataC -= pSprite->field_4;
    int16 Saved0 = Data0;
    int16 Saved4 = pSoundEffect;

    Data0 = 0;
    pSoundEffect = 0;

    Map_Get_Distance_BetweenPoints_Within_640( Data0, pSoundEffect, pData8, DataC );

    Data0 /= 0x10;
    int Volume = 0x28;
    Volume -= Data0;

    if (Volume <= 0)
        return;

    Data0 = Saved0;
    pSoundEffect = Saved4;
    mSound->Sound_Play( mMap_TileSet, pSoundEffect, Volume );
}

void cFodder::Mission_Intro_Helicopter_Start( ) {
    mHelicopterPosX = 0x01500000;
    mHelicopterPosY = 0x00260000;

    mBriefing_Helicopter_Off1 = mBriefing_Helicopter_Offsets[0];
    mBriefing_Helicopter_Off2 = mBriefing_Helicopter_Offsets[1];
    mBriefing_Helicopter_Off3 = mBriefing_Helicopter_Offsets[2];
    mBriefing_Helicopter_Off4 = &mBriefing_Helicopter_Offsets[3];
    mBriefing_ParaHeli_Frame = 0;
    mBriefing_Helicopter_Moving = -1;
    word_428D8 = -1;

    sub_1594F();
}

void cFodder::Briefing_Update_Helicopter( ) {
    word_428B6 &= 0x1FE;
    uint16 bx = word_428B6;

    int32 ax = mMap_Direction_Calculations[ (bx / 2) & 0xFF ];

    ax >>= 2;

    mHelicopterPosX += ax * word_428B8;

    bx += 0x80;
    bx &= 0x1FE;

    ax = mMap_Direction_Calculations[ (bx / 2) & 0xFF];
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

    mBriefing_ParaHeli_Frame += 1;
    if (mBriefing_ParaHeli_Frame == 4)
        mBriefing_ParaHeli_Frame = 0;

    --word_428BA;
    if (word_428BA <= 0)
        sub_1594F();

}

void cFodder::sub_1594F( ) {

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
        mImage->paletteNew_SetToBlack();
        mImageFaded = -1;
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

        if (mVersion->mPlatform == ePlatform::Amiga)
            DrawY += 0x16;

        //String_Print_Large("CAMPAIGNS", false, DrawY);
    }
}

void cFodder::Mission_Intro_Draw_Mission_Name( ) {

    if (mMissionNumber == 0)
        Mission_Intro_Draw_OpenFodder();
    else
        Briefing_Draw_Mission_Title( 0xB5 );
}

void cFodder::Briefing_Draw_Mission_Title( int16 pDrawAtY ) {
    
    // Draw MISSION xx
    {
        std::stringstream Mission;
        Mission << "MISSION ";
        mString_GapCharID = 0x25;
        Mission << tool_StripLeadingZero( tool_NumToString( mMissionNumber ) );

        String_Print_Large( Mission.str(), true, 0 );
    }

    // Draw Mission Name, or Map 
    {
        std::string Title;

        if (pDrawAtY == 0xB5) {
            Title = mCampaign.getMissionName( mMissionNumber );

            if (mVersion->mPlatform == ePlatform::Amiga)
                pDrawAtY += 0x16;

        } else {
            Title = mCampaign.getMapName(mMapNumber);
        }

        String_Print_Large( Title, true, pDrawAtY );
    }
}

void cFodder::CopyProtection() {

    // Only DOS CD had copy protection
    if (mVersion->mVersion != eVersion::Dos_CD)
        return;

    g_Graphics.SetActiveSpriteSheet( eGFX_Types::eGFX_RECRUIT);

    // 3 Attempts
    for (int16 Attempts = 0; Attempts < 3; ++Attempts) {

        int16 Data0;
        do {
            Data0 = tool_RandomGet() & 0x0F;
        }  while (Data0 == 0x0F);

        mImage->clearBuffer();
        mGraphics->PaletteSet();

        const sCopyProtection* word_44A1C = &mCopyProtection_Values[Data0];

        std::string Page = "PAGE " + tool_NumToString( word_44A1C->mPage );
        std::string Paragraph = "PARAGRAPH " + tool_NumToString( word_44A1C->mParagraph );
        std::string Line = "LINE " + tool_NumToString( word_44A1C->mLine );
        std::string Word = "WORD " + tool_NumToString( word_44A1C->mWord );

        Recruit_Render_Text( "ENTER WORD FROM", 0 );
        Recruit_Render_Text( "MANUAL AT", 0x14 );
        Recruit_Render_Text( Page.c_str(), 0x3C );
        Recruit_Render_Text( Paragraph.c_str(), 0x50 );
        Recruit_Render_Text( Line.c_str(), 0x64 );
        Recruit_Render_Text( Word.c_str(), 0x78 );

        mImageFaded = -1;
        int8 mCursorBlinkTimer = 0;
        bool mShow = false;

        mGUI_Temp_X = 5;
        mGUI_Temp_Width = 10;
        memset( mInputString, 0, sizeof( mInputString ) );
        mInputString_Position = 0;

        mImage->Save();
        mKeyCodeAscii = 0;

        while (mKeyCodeAscii != 0x0D) {
            
            if (mImageFaded) {
                mImageFaded = mImage->palette_FadeTowardNew();
            }

            String_Input_Print( 0xA0 );

            ++mCursorBlinkTimer;
            mCursorBlinkTimer &= 0x0F;
            if (!mCursorBlinkTimer)
                mShow = !mShow;

            if (mShow)
                GUI_Draw_Frame_8( 0x0F, 0x00, mGUI_Temp_X + mGUI_Temp_Width, 0xA0);

            g_Window.RenderAt( mImage );
            g_Window.FrameEnd();
            eventProcess();

            mImage->Restore();
        }

        Image_FadeOut();

        mInputString[mInputString_Position] = -1;

        CopyProtection_EncodeInput();

        const uint8* Answer = word_44A1C->mAnswer;
        bool Failed = false;

        for (int16 Pos = 0; *Answer != 0xFF; ++Pos) {

            if ((uint8) mInputString[Pos] != *Answer++) {
                Failed = true;
                break;
            }
        }

        if (!Failed)
            return;
    }

    Exit(1);
}

void cFodder::CopyProtection_EncodeInput() {
    int16 bx = mInputString[0] & 0xFF;
    int16 cx = 0;

    for (int16 Pos = 0; Pos < 21; ++Pos) {
        cx -= bx;

        if (mInputString[Pos] == -1)
            return;

    rollLoop:;

        bx = (bx << 8) | (bx >> 8);

        mInputString[Pos] ^= (bx & 0xFF);
        mInputString[Pos] ^= (cx & 0xFF);
        --mInputString[Pos];

        if (mInputString[Pos] == -1)
            goto rollLoop;
    }
}

void cFodder::Campaign_Select_DrawMenu(const char* pTitle, const char* pSubTitle) {
	size_t YOffset = (mVersion->mPlatform == ePlatform::PC ? 0 : 25);

	mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

	GUI_Element_Reset();
	MapTiles_Draw();

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

	int16 ItemCount = 0;

	auto FileIT = mCampaignList.begin() + mGUI_Select_File_CurrentIndex;

	for (; ItemCount < mGUI_Select_File_ShownItems && FileIT != mCampaignList.end(); ++ItemCount) {
		size_t Pos = FileIT->find_first_of(".");

		GUI_Button_Draw_Small(FileIT->c_str(), 0x44 + (ItemCount * 0x15), 0xB2, 0xB3);
		GUI_Button_Setup_Small(&cFodder::GUI_Button_Filename);
		++FileIT;
	}
}

std::string cFodder::Campaign_Select_File(const char* pTitle, const char* pSubTitle, const char* pPath, const char* pType, eDataType pData ) {
    mCampaignList = GetAvailableVersions(); 
    
    mMission_Aborted = false;
    mGUI_SaveLoadAction = 0;

    {
        auto Files = local_DirectoryList(local_PathGenerate("", pPath, pData), pType);

        // Sort files alphabetical
        std::sort(Files.begin(), Files.end(),[](std::string& pLeft, std::string& pRight) {
            return pLeft < pRight;
        });

        // Append all custom campaigns to the list
        for (auto& File : Files) {
            size_t Pos = File.find_first_of(".");
            std::string FileName = File.substr(0, Pos);

            // Don't add known campaigns
            if (isCampaignKnown(FileName))
                continue;

			mCampaignList.push_back(FileName);
        }
    }

    mGUI_Select_File_CurrentIndex = 0;
    mGUI_Select_File_Count = (int16)mCampaignList.size();
    mGUI_Select_File_ShownItems = (mVersion->mPlatform == ePlatform::PC ? 4 : 5);

	if(mVersion->isRetail())
		Map_Create(mTileTypes[eTileTypes_Jungle], 0, 0x15, 0x0F, false);
	else
		Map_Create(mTileTypes[eTileTypes_AFX], 0, 0x15, 0x0F, false);

	Campaign_Select_Sprite_Prepare();

    do {

		Campaign_Select_File_Loop(pTitle, pSubTitle);

    } while (mGUI_SaveLoadAction == 3);

    if (mGUI_SaveLoadAction == 1)
        return "";

    return mCampaignList[mGUI_Select_File_CurrentIndex + mGUI_Select_File_SelectedFileIndex];
}

std::string cFodder::GUI_Select_File( const char* pTitle, const char* pPath, const char* pType, eDataType pData ) {
    mMission_Aborted = false;
    mGUI_SaveLoadAction = 0;

    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);

    auto Files = local_DirectoryList( local_PathGenerate( "", pPath, pData ), pType );

    mGUI_Select_File_CurrentIndex = 0;
    mGUI_Select_File_Count = (int16)Files.size();
    mGUI_Select_File_ShownItems = (mVersion->mPlatform == ePlatform::PC ? 4 : 5);

    do {
        size_t YOffset = (mVersion->mPlatform == ePlatform::PC ? 0 : 25);

        GUI_Element_Reset();
        Recruit_Render_Text( pTitle, 0x0C );

        GUI_Button_Draw( "UP", 0x24 );
        GUI_Button_Setup( &cFodder::GUI_Button_Load_Up );

        GUI_Button_Draw( "DOWN", 0x99 + YOffset);
        GUI_Button_Setup( &cFodder::GUI_Button_Load_Down );

        GUI_Button_Draw( "CONTINUE GAME", 0xB3 + YOffset);
        GUI_Button_Setup( &cFodder::GUI_Button_Load_Exit );

        mImage->Save();

        int16 DataC = 0;

        auto FileIT = Files.begin() + mGUI_Select_File_CurrentIndex;

        for (; DataC < mGUI_Select_File_ShownItems && FileIT != Files.end(); ++DataC) {
            size_t Pos = FileIT->find_first_of( "." );

            GUI_Button_Draw(FileIT->substr(0, Pos), 0x3E + (DataC * 0x15), 0xB2, 0xB3 );
            GUI_Button_Setup( &cFodder::GUI_Button_Filename );
            ++FileIT;
        }

        GUI_Select_File_Loop( false );
        mImage->Restore();

    } while (mGUI_SaveLoadAction == 3);

    if (mGUI_SaveLoadAction == 1)
        return "";

    return Files[mGUI_Select_File_CurrentIndex + mGUI_Select_File_SelectedFileIndex];
}

void cFodder::Campaign_Selection() {
	mMission_Complete = 0;

	Mission_Memory_Clear();
	Game_ClearVariables();
	Map_Clear_Destroy_Tiles();

    Image_FadeOut();

    mGraphics->PaletteSet();

    mMouseSpriteNew = eSprite_pStuff_Mouse_Target;

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    mCustom_Mode = eCustomMode_None;

    std::string CampaignFile = Campaign_Select_File( "OPEN FODDER", "SELECT CAMPAIGN", "", "*.ofc", eDataType::eCampaign );

    // Exit Pressed?
    if (mGUI_SaveLoadAction == 1 || !CampaignFile.size()) {

        // Return to custom menu
        mDemo_ExitMenu = 1;
        mCustom_Mode = eCustomMode_None;

        return;
    }

    // Find a data version to use with this campaign
    // If no version is found, we use the currently loaded one
    {
        const sVersion* Version = FindAvailableVersionForCampaign(CampaignFile);

        // Load a new version?
        if (Version && Version != mVersion) {
            VersionLoad(Version);
        }

        // Set the default/starting version
        mVersionDefault = mVersion;

        // Single Map Mode?
        if (CampaignFile == "Single Map" || CampaignFile == "Random Map") {
            mCustom_Mode = eCustomMode_Map;
            return;
        
        // If no version, it must be a custom campaign
        } else if (!Version) {
            mCustom_Mode = eCustomMode_Set;
        }
    }

    // Load the campaign
    if (Campaign_Load(CampaignFile) == true) {

        // 
        if (mVersion->mVersion == eVersion::Custom) {
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

	Mission_Prepare_Squads();

	mSquad_CurrentVehicle = &mSprites[x];
	mSprites[x].field_0 = 0xf0;
	mSprites[x].field_4 = 0xce;
	mSprites[x].field_8 = 0xD2;
	mSprites[x].field_A = 5;
	mSprites[x].field_52 = 0;
	mSprites[x].field_20 = 0;
	mSprites[x].field_18 = eSprite_Turret_Missile_Human;
	mSprites[x++].field_6F = eVehicle_Turret_Missile;

	mSprites[x].field_0 = rand() & 0xFF;
	mSprites[x].field_4 = tool_RandomGet() & 0xff;
	mSprites[x].field_8 = 6;
	mSprites[x].field_A = 0;
	mSprites[x].field_52 = 0;
	mSprites[x].field_20 = 0;
	mSprites[x++].field_18 = eSprite_Indigenous_Spear;

	mSprites[x].field_0 = 0xff;
	mSprites[x].field_4 = 16 + (rand() % 0x60);
	mSprites[x].field_8 = 2;
	mSprites[x].field_A = 0;
	mSprites[x].field_52 = 0;
	mSprites[x].field_20 = 0;
	mSprites[x++].field_18 = eSprite_Bird_Left;

	mSprites[x].field_0 = 0;
	mSprites[x].field_4 = 16  + (rand() % 0xc0);
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

	mSprites[x].field_0 = 220;
	mSprites[x].field_4 = 60;
	mSprites[x].field_8 = 2;
	mSprites[x].field_A = 0;
	mSprites[x].field_52 = 0;
	mSprites[x].field_20 = 0;
	mSprites[x++].field_18 = eSprite_BoilingPot;

	mSquad_Leader = mSquad_CurrentVehicle;

	word_3AA1D = word_3BED5[0];

	Map_Add_Structure(mStructuresBarracksWithSoldier[mMap_TileSet], 4, 3);
}

void cFodder::Campaign_Select_File_Loop( const char* pTitle, const char* pSubTitle) {

	if (mGUI_SaveLoadAction != 3) {

		mImage->palette_FadeTowardNew();
		Mouse_Setup();
	}
	mGUI_SaveLoadAction = 0;

	mImageFaded = -1;
	mGraphics->PaletteSet();

	mImage->Save();

	mImageFaded = -1;
	mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
	mDemo_ExitMenu = 0;

	Camera_Reset();

	int16 Timedown = 0;
	do {
		Sprite_Frame_Modifier_Update();
		Mission_Sprites_Handle();

		Campaign_Select_DrawMenu(pTitle, pSubTitle);
		mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
		Sprites_Draw();

		if (mImageFaded == -1)
			mImageFaded = mImage->palette_FadeTowardNew();

		Mouse_Inputs_Get();
		Mouse_DrawCursor();

		if (Timedown)
			--Timedown;

		if (mMouse_Button_Left_Toggle && !Timedown) {
			Vehicle_Input_Handle();

			mMouse_Button_Left_Toggle = 0;
			mSprites[0].field_57 = -1;
			mSprites[0].field_2E = mSquad_Leader->field_26;
			mSprites[0].field_30 = mSquad_Leader->field_28;

			Timedown = 10;
		}

		if (mMission_Aborted)
			GUI_Button_Load_Exit();

		if (mMouse_Button_Left_Toggle)
			GUI_SaveLoad_MouseHandle(mGUI_Elements);

		if (dword_3B30D)
			(this->*dword_3B30D)(0x50);

		g_Window.RenderAt(mImage, cPosition());
		g_Window.FrameEnd();

		Video_Sleep();
		mImage->Restore();

	} while (mGUI_SaveLoadAction <= 0);

	mMission_Aborted = false;

	if (mGUI_SaveLoadAction == 3)
		return;

	Image_FadeOut();
}

/**
 * Display a list of custom maps
 */
void cFodder::Custom_ShowMapSelection() {

    Image_FadeOut();
    mGraphics->PaletteSet();

    const std::string File = GUI_Select_File( "SELECT MAP", "Custom/Maps", "*.map" );
    
    // Exit Pressed?
    if (mGUI_SaveLoadAction == 1 || !File.size()) {

        // Return to custom menu
        mDemo_ExitMenu = 1;
        mCustom_Mode = eCustomMode_None;

        return;
    }

    mCampaign.LoadCustomMap( File );

    mMapNumber = 0;
    mMissionNumber = 1;
    mMissionPhase = 0;
    mDemo_ExitMenu = 1;
    mCustom_ExitMenu = 1;
    mCustom_Mode = eCustomMode_Map;
}

bool cFodder::Demo_Amiga_ShowMenu() {

    mSound->Music_Stop();

    mGraphics->Load_And_Draw_Image( "apmenu.lbm", 32 );

	// Amiga Demos have a different cursor palette
    ((cGraphics_Amiga*)mGraphics)->SetCursorPalette( 0x10 );
    mWindow->SetScreenSize( cDimension( 320, 260) );

    mGUI_Mouse_Modifier_Y = 4;

    Menu_Loop(
        [this]() {
            if (mButtonPressLeft) {
                if (mVersion->mVersion == eVersion::AmigaFormat)
                    GUI_Element_Mouse_Over( mAfx_Buttons );

                if (mVersion->mVersion == eVersion::AmigaPlus)
                    GUI_Element_Mouse_Over( mPlus_Buttons );
            }
    } );

    ((cGraphics_Amiga*)mGraphics)->SetCursorPalette( 0xE0 );

	mWindow->SetScreenSize(cDimension(320, 225));

    return mMission_Aborted;
}

bool cFodder::Recruit_Loop() {

    mImage->clearBuffer();

    // If demo data is loaded, we need to load
    if (mVersion->mRelease == eRelease::Demo) {

        if (mCustom_Mode == eCustomMode_Set) {
            VersionLoad(mVersionDefault);
        }

		if (mVersion->mRelease == eRelease::Demo)
			return 0;
    }

    Mouse_Setup();
    Sidebar_Clear_ScreenBufferPtr();

    mMission_Aborted = false;

    Recruit_Truck_Anim_Prepare();
    sub_16C6C();
    Recruit_Render_LeftMenu();

	mGraphics->Load_Hill_Data();
    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);
    mGraphics->Recruit_Draw_Hill();

    Recruit_Copy_Sprites();
    
    if (mVersion->mPlatform == ePlatform::Amiga) {
        
        ((cGraphics_Amiga*)mGraphics)->Hill_Prepare_Overlays();
    }
    
    mGraphics->SetActiveSpriteSheet( eGFX_HILL );
    
    Recruit_Draw_Graves();
    
    mGraphics->PaletteSet();
    mImage->palette_FadeTowardNew();
    mImage->Save();

    mGUI_Mouse_Modifier_X = 0;
    mGUI_Mouse_Modifier_Y = 0x1D;

    Recruit_Render_Names_UnusedSlots();

    mRecruit_RenderedPtr = mRecruit_Rendered;

    for (int16 ax = mMission_Troop_Count - 1; ax >= 0;--ax )
        Recruit_Sidebar_Render_SquadName();
    
    mMouseCursor_Enabled = -1;

    sub_1787C();

    Recruit_Draw_Actors();
    Recruit_Draw_Actors();
    
    mMouse_Exit_Loop = 0;
    
    for( ;; ) {
        if( mMouse_Exit_Loop ) {
            mMouse_Exit_Loop = 0;
            
            if( Recruit_Check_Buttons_SaveLoad() )
                break;
        }

        if (mMission_Aborted)
            break;

        Recruit_Draw();
    }
    
    mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
    mMouseCursor_Enabled = 0;
    
    mImage->paletteNew_SetToBlack();
    
    while( mImage->GetFaded() == false ) {
        Recruit_Draw();
    }

    return mMission_Aborted;
}

void cFodder::Recruit_Draw_String(  int32 pParam0, size_t pParam8, size_t pParamC, const std::string& pString ) {

    for ( std::string::const_iterator Text = pString.begin(); Text != pString.end(); ++Text, pParam8 += 0x0C) {
        char NextChar = *Text;

        if (NextChar == 0x20)
            continue;

        if (NextChar <= 0x39) {
            NextChar -= 0x30;
            NextChar += 0x1A;
        }
        else
            NextChar -= 0x41;

        GUI_Draw_Frame_8( pParam0, NextChar, pParam8, pParamC);
    }
}

void cFodder::Recruit_Truck_Anim_Prepare() {
    uint16* di = mRecruit_Truck_FramesPlay;
    int16  count;
    
    // Do any troops need to enter the truck?
    if(mMission_Troops_Required > 0 ) {
        
        // Welcome Troops
        Recruit_Truck_Anim_CopyFrames( &di, mRecruit_Truck_Anim_Welcome );
        
        count = mMission_Troops_Required;
        count -= 2;

        // Wave Troop Past
        while( count >= 0 ) {
            Recruit_Truck_Anim_CopyFrames( &di, mRecruit_Truck_Anim_PassTroop );
            --count;
        }
        
        // Swing Arm to stop Troops
        Recruit_Truck_Anim_CopyFrames( &di, mRecruit_Truck_Anim_SwingArm );

        // Close the truck door
        Recruit_Truck_Anim_CopyFrames( &di, mRecruit_Truck_Anim_CloseDoor );
    }

    // Set end markers
    *di++ = 0;
    *di = -1;
}

void cFodder::Recruit_Truck_Anim_CopyFrames( uint16** pDi, const int16* pSource ) {
    int16 ax;
    
    for(;;) {
        ax = *pSource++;
        
        if( ax < 0 )
            break;
        
        **pDi = ax;
        (*pDi)++;
    }   
}

void cFodder::sub_16C6C() {
    int16 *si = mGraveRanks;
    
    while( *si != -1 ) {
        uint8* si1 = (uint8*) si;
        
        *(si1+1) += 1;
        ++si;
    }
}

void cFodder::Recruit_Draw_Grave( int16 pSpriteType, const size_t pPosX, const size_t pPosY ) {
    pSpriteType >>= 1;
    if (pSpriteType > 8)
        pSpriteType = 8;

    int32 Frame = (int32) pPosY;
    Frame -= 0x14;
    Frame >>= 5;
    if (Frame >= 5)
        Frame = 4;

    GUI_Draw_Frame_8( pSpriteType, Frame, pPosX, pPosY - 8);
}

void cFodder::Recruit_Draw_Graves( ) {
    int32 Data1C = -1;
    int16* GraveRank = mGraveRanks;
    const int16* GravePosition = &mGravePositions[720];

    for(; *GraveRank >= 0 ; ++GraveRank) {
        GravePosition -= 2;
        Data1C++;
    }

    if (GraveRank == mGraveRanks)
        return;

    --GraveRank;

    do {
        int16 RankSprite = *GraveRank & 0xFF;
        if (RankSprite < 0)
            return;

        --GraveRank;
        int16 PosX = *GravePosition++;
        int16 PosY = *GravePosition++;

        Recruit_Draw_Grave( RankSprite, PosX, PosY );

        --Data1C;
    } while (Data1C >= 0);

}

void cFodder::Recruit_Render_LeftMenu() {
    int16 Data0 = 0xAD;
    int16 Data8 = 0;
    int16 DataC = 0x18;
    
    // Draw Heroes Heading
    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC );
    
    int16 Data14 = 0x0E;
    Data0 = 4;
    
    for( sHero* Data20 = mHeroes; Data0 >= 0; --Data0, ++Data20  ) {
        if( Data20->mRecruitID == -1 )
            break;
        
        Data14 += 0x0C;
    }

    // Draw Empty Hero Slots
    DataC = 0x0E;
    do {
        
        Data0 = 0xA9;
        if( DataC >= Data14 )
            Data0 = 0xAB;
        
        Data8 = 0;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC + 0x18 );
        DataC += 0x0C;
        
    } while( DataC < 0x4A );

    //seg003:1E89

    // Draw Squad Heading
    Data0 = 0xAE;
    Data8 = 0;
    DataC = 0x4A + 0x18;
    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC );
    
    Data14 = mMission_Troops_Required + mMission_Troop_Count;
    Data14 *= 0x0C;
    DataC = 0x58;
    Data14 += DataC;

    int16 Final = 0;
    if (mVersion->mPlatform == ePlatform::PC)
        Final = 0xA0;
    else
        Final = 0xB8;

    // Draw Used Slot
    do {
        Data0 = 0xA9;
        if( DataC >= Data14 )
            Data0 = 0xAB;

        Data8 = 0;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC + 0x18 );
        DataC += 0x0C;
        
    } while( DataC < Final );

    word_3AA55 = 0x0F;
    word_3AAC7 = -1;

    Recruit_Render_Squad_RankKills();
    Recruit_Render_Squad_Names();
    
    word_3AAC7 = 0;
    word_3AA55 = -59;
    
    Recruit_Render_HeroList();
    word_3AA55 = 0;
}

void cFodder::Recruit_Render_Squad_Names() {

    word_3A3BB = 7;
    mSidebar_Draw_Y = 0;
    
    for( uint16 x = 0; x < 8; ++x, --word_3A3BB ) {
        sMission_Troop* Member = &mMission_Troops[x];
        if (Member->mRecruitID == -1)
            continue;

        if( Member->mSprite == INVALID_SPRITE_PTR ||  Member->mSprite == 0 )
            continue;
        
        if( mSquad_Selected != Member->mSprite->field_32)
            continue;
            
        const sRecruit* Data28 = &mRecruits[Member->mRecruitID];
        int16 Data14;

        for( Data14 = 0; Data14 <= 5; ++Data14 ) {
            
            if( Data28->mName[Data14] == 0x20 )
                break;
        } 
        
        Data14 <<= 2;
        int16 word_3A05F = (0x30 - Data14) >> 1;

        if( word_3AAC7 )
            word_3A05F -= 1;
        
        // Draw Troop name to list
        for( Data14 = 0; Data14 <= 5; ++Data14 ) {
            
            if( Data28->mName[Data14] != 0x20 ) {
                int16 Data0 = Data28->mName[Data14];
                Data0 -= 0x41;
                Data0 += 0x29;
                
                int16 X = Data14;
                X <<= 2;
                X += word_3A05F;

                int16 Y = 0x4B;
                Y += mSidebar_Draw_Y;
                Y += word_3AA55;

                mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, X, Y + 0x18 );
            }
        }

        mSidebar_Draw_Y += 0x0C;
    }

}

void cFodder::Recruit_Render_Squad_RankKills() {
    int16 Data0;

    int16 word_3A061 = 7;
    mSidebar_Draw_Y = 0;
    
    int16 Data4;
    
    for( uint16 x = 0; x < 8; ++x, --word_3A061 ) {
        sMission_Troop* Member = &mMission_Troops[x];
        if (Member->mRecruitID == -1)
            continue;

        
        if( Member->mSprite == INVALID_SPRITE_PTR || Member->mSprite == 0 )
            continue;
        
        Data4 = mSquad_Selected;
        sSprite* Data34 = Member->mSprite;
        
        if( Data4 != Data34->field_32  )
            continue;
        
        int16 Data8 = 0;
        int16 DataC = mSidebar_Draw_Y - 1;
        
        if( !word_3AAC7 ) {
            Data0 = Member->mRank + 9;
            Data8 = 0x23;

            DataC += 0x4A;
            DataC += word_3AA55;

            mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC + 0x18 );
            
        } else {
            // Draw Kills
            Recruit_Render_Number( Member->mNumberOfKills, 0x43 );
            
        }
        
        //Draw Rank
        Data0 = Member->mRank + 9;

        DataC += 0x4A;
        DataC += word_3AA55;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, 0, DataC + 0x18 );
        mSidebar_Draw_Y += 0x0C;
    }
}

void cFodder::Recruit_Render_Number( int16 pNumber, int16 pData10 ) {
    
    pData10 -= 0x30;
    std::string Data20 = tool_StripLeadingZero(tool_NumToString( pNumber ));
    uint16 Data0 = (uint16) Data20.length() * 4;

    int16 Data8 = 0x30 - Data0;
    int16 DataC = 0x4B + mSidebar_Draw_Y + word_3AA55;

    for (std::string::iterator CharIT = Data20.begin(); CharIT != Data20.end(); ++CharIT) {

        Data0 = (*CharIT & 0xFF) + pData10;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC + 0x18);
        Data8 += 4;
    }
}

void cFodder::Recruit_Render_HeroList() {
    const sHero* Hero = mHeroes;

    word_3A3BB = 4;
    mSidebar_Draw_Y = 0;

    for (word_3A3BB = 4; word_3A3BB >= 0; --word_3A3BB, ++Hero) {

        if (Hero->mRecruitID < 0)
            continue;

        int16 Data8 = 0;
        int16 DataC = mSidebar_Draw_Y - 1;
        DataC += 0x4A + word_3AA55 + 0x18;

        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Hero->mRank + 9, Data8, DataC );

        const sRecruit* Troop = &mRecruits[ Hero->mRecruitID ];

        int16 Position;
        for( Position = 0; Position <= 5; ++Position ) {
            
            if( Troop->mName[Position] == 0x20 )
                break;
        } 

        Position <<= 2;
        word_3A05F = (0x30 - Position) >> 1;
        --word_3A05F;

        for (Position = 0; Position <= 5; ++Position) {

            uint8 Character = Troop->mName[Position];
            if (Character == 0x20)
                continue;

            Character -= 0x41;
            Character += 0x29;
            Data8 = Position;
            Data8 <<= 2;
            Data8 += word_3A05F;
            DataC = 0x4B;
            DataC += mSidebar_Draw_Y + word_3AA55;

            mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Character, Data8, DataC  + 0x18);
        }

        Recruit_Render_Number( Hero->mKills, 0x67 );
        mSidebar_Draw_Y += 0x0C;
    }
}

void cFodder::Recruit_Render_Names_UnusedSlots() {
    uint32* Data20 = (uint32*) mGraphics->mImageTemporary.mData->data();

    sRecruitRendered* Data24 = mRecruit_Rendered;
    int16 Data0 = 0x58;
    
    if (mVersion->mPlatform == ePlatform::PC)
        Data0 = 0x58;
    else
        Data0 = 0x44;
    
    // Squad Troop Names
    for ( ; Data0 < 0xA0; Data0 += 0x0C) {
        Data24->mDataPtr = Data20;
        Data24->mPosition = Data0;

        mGraphics->Sidebar_Copy_ScreenBuffer( Data0, 0x0C, 0, Data20 );
        ++Data24;
    }

    //seg003:2532
    Data24->mDataPtr = 0;
    Data24->mPosition = -1;

    int16 DataC = 0x58;
    int16 Data4 = 0;
    
    // Empty Slots
    if (mVersion->mPlatform == ePlatform::PC)
        Data4 = 5;
    else
        Data4 = 7;

    for (; Data4 >= 0; --Data4) {
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( 0xAC, 0, DataC + 0x18 );

        DataC += 0x0C;
    }
}

void cFodder::Recruit_Sidebar_Render_SquadName() {
    sRecruitRendered* Data24 = mRecruit_RenderedPtr ;
    if (Data24->mPosition < 0)
        return;

    ++mRecruit_RenderedPtr;
    mGraphics->Sidebar_Copy_ScreenBuffer( (uint16) Data24->mPosition, 0x0C, -1, Data24->mDataPtr );
}

void cFodder::Recruit_Draw_Actors( ) {

    Recruit_Draw_Truck();
    Recruit_Draw_Troops();
}

void cFodder::sub_175C0() {
    sRecruit_Screen_Pos* Data20 = 0;

    if (mRecruit_Truck_Enter_Count > 0) {

        sRecruit_Screen_Pos* Data20 = &mRecruit_Screen_Positions[293];
        do {
            if (Data20 == &mRecruit_Screen_Positions[0])
                return;

            --Data20;

        } while (Data20->mFrame == 0 || Data20->mFrame < 4);

        int16 Data0 = Data20->mFrame;
        Data20->mFrame = 0;
        Data0 ^= 1;


        if ((Data20 + 1)->mX >= 0) {
            (Data20 + 1)->mFrame = Data0;
        } else {
            --mRecruit_Truck_Enter_Count;
            Recruit_Sidebar_Render_SquadName();
        }
    }
    //loc_17652

    uint16 aa = ~(dword_3B1CB >> 16);

    dword_3B1CB = (aa << 16) | (dword_3B1CB & 0xFFFF);

    Data20 = &mRecruit_Screen_Positions[293];
    sRecruit_Screen_Pos* Data28 = &mRecruit_Screen_Positions[271];

loc_17686:;
    for (; Data20 != &mRecruit_Screen_Positions[0];) {
        --Data20;

        if (Data20->mFrame == 0)
            continue;

        if (Data20->mFrame < 4)
            continue;

        if (Data20 >= Data28)
            continue;

        int16 Data8 = 3;
        sRecruit_Screen_Pos* Data30 = Data20;
        //loc_176CE

        do {
            ++Data30;
            if (Data30->mFrame) {
                if (Data30->mFrame > 3)
                    goto loc_17686;

                ++Data30;
            }
            --Data8;
        } while (Data8 >= 0);

        // Animate the frame
        int16 Data0 = Data20->mFrame;
        Data20->mFrame = 0;

        if ((dword_3B1CB >> 16) < 0)
            Data0 ^= 1;

        if ((Data20 + 1)->mFrame) {
            (Data20 + 2)->mFrame = Data0;
        }
        else {
            (Data20 + 1)->mFrame = Data0;
        }
    }

}

void cFodder::Recruit_Draw_Troops() {

    if (mRecruit_Truck_Animation_Play) {
        sub_175C0();
    } else {
        Recruit_Frame_Check();
        Recruit_Position_Troops();
    }

    sRecruit_Screen_Pos* Data20 = mRecruit_Screen_Positions;
    sRecruit_Screen_Pos* dword_3B1C7;

    for ( ; ;Data20 = dword_3B1C7) {

        dword_3B1C7 = Data20;

        if (Data20->mX < 0)
            break;

        int16 Data0;
        int16 Data8 = Data20->mX;
        int16 DataC = Data20->mY;
        int16 Data4 = Data20->mFrame;
        ++Data20;
        dword_3B1C7 = Data20;

        if (Data4 == 0)
            continue;

        --Data4;
        if (Data4 >= 3) {
            //loc_177E3
            Data4 -= 3;
            Data0 = 0x0A;
            Data0 += Data4;

            // Sprite reached end of last curve
            if (DataC >= 0xB7)
                Data0 += 4;

            // Sprite on last curve
            else if (DataC > 0x7C )
                Data0 += 2;

            // First dip of mountain is frame 0
            Data4 = 0;

            // Second Dip is frame 1
            if (Data20 > &mRecruit_Screen_Positions[0x46]) {
                Data4 = 1;

                // Last leg of mountain
                if (Data20 > &mRecruit_Screen_Positions[0x7A])
                    Data4 = 2;
            }
        }
        else
            Data0 = 9;  // Hill Piece

        //loc_1784C
        if (mVersion->mPlatform == ePlatform::PC) {
            DataC -= 8;

            // Hill pieces are 16bit graphics
            if (Data0 != 9)
                GUI_Draw_Frame_8( Data0, Data4, Data8, DataC);
            else {
                ++Data8;
                --DataC;
                GUI_Draw_Frame_16( Data0, Data4, Data8, DataC );
            }

        } else {
            GUI_Draw_Frame_8( Data0, Data4, Data8, DataC);
        }
    }
}

void cFodder::sub_1787C() {
    
    if (mMission_Restart)
        return;
    
    mRecruit_Truck_Enter_Count = mMission_Troops_Required;
    mRecruit_Truck_Animation_Play = 0;

    int16 Data0 = 0x1E;
    int16* Data20 = mRecruit_Hill_Positions_Use;
    const int16* Data24 = mRecruit_Hill_Position_Gaps;

    // Loop the 15 positions
    for ( int16 Data4 = 0x0E; Data4 >= 0; --Data4 ) {

        *Data20++ = Data0;
        Data0 -= *Data24++;

    }
}

void cFodder::Recruit_Frame_Check() {
    sRecruit_Screen_Pos* Data20 = mRecruit_Screen_Positions;

    for (; Data20->mX >= 0; ++Data20) {
        
        if (Data20->mFrame >= 4)
            Data20->mFrame = 0;
    }
}

void cFodder::Recruit_Position_Troops() {
    const int16*    Recruit_Next_Shirt = mRecruit_Shirt_Colors;

    uint64 Data8 = (uint64) mGraveRankPtr;
    int16 Recruits, Data4;

    // Calculate number of dead troops
    Data8 -= (uint64) mGraveRanks;
    Data8 >>= 2;
    Data8 &= 0xF;

    if (mMission_Restart)
        Recruits = mRecruits_Available_Count;
    else {
        Recruits = mMission_Recruits_AliveCount;
        if (!Recruits)
            goto loc_179B2;
    }

    // loc_1795E
    --Recruits;
    Data4 = 0x110;

    // Set the recruit position frames
    for (; Recruits >= 0; --Recruits) {

        if (Data4 >= 0) {
            if (mRecruit_Screen_Positions[Data4].mFrame)
                --Data4;

            mRecruit_Screen_Positions[Data4].mFrame = mRecruit_Shirt_Colors[Data8];
        }

        Data8 += 1;
        Data8 &= 0xF;
        Data4 -= 4;
    }

loc_179B2:;
    // If the mission is restarting, troops are already over the hill
    if (mMission_Restart)
        return;

    mRecruit_Truck_Reached = 0;
    int16 Data0 = mRecruit_Hill_Positions_Use[0];
    Data8 = 5;

    for ( ; ; Data0++ ) {

        Data4 = mRecruit_Screen_Positions[Data0].mFrame;
        if (Data4) {
            if (Data4 > 3) {
                mRecruit_Truck_Reached = -1;
                break;
            }
            else
                continue;

        } else {
            if (Data0 == 0x110) {
                mRecruit_Truck_Reached = -1;
                break;
            }
            --Data8;
            if (!Data8)
                break;
        }
    }
    //seg003:2B6D
    if (!mRecruit_Truck_Reached) {
        dword_3B1CB += 0x8000;
        dword_3B1CB &= 0x1FFFF;
    }
    else
        dword_3B1CB = 0x10000;

    int16* Data20 = mRecruit_Hill_Positions_Use;

    // 14 moving recruits
    for (int16 DData8 = 0x0E; DData8 >= 0; --DData8) {

        // Get the next position
        Data0 = *Data20;

        if (Data0 >= 0) {

            // Frame
            Data4 = (dword_3B1CB >> 16);
            Data4 += *Recruit_Next_Shirt++;

            if (mRecruit_Screen_Positions[Data0].mX < 0)
                Data0 = 0;

            if (mRecruit_Screen_Positions[Data0].mFrame)
                ++Data0;

            mRecruit_Screen_Positions[Data0].mFrame = Data4;
        }
        // 17A9D

        if (!mRecruit_Truck_Reached)
            Data0 ++;

        //loc_17AA9
        *Data20 = Data0;
        ++Data20;
    }

    if (mRecruit_Truck_Reached)
        mRecruit_Truck_Animation_Play = -1;
}

void cFodder::Recruit_Draw_Truck( ) {
    int16 DataC = 0xB6;

    if (mVersion->mPlatform == ePlatform::Amiga)
        DataC = 0xBE;

    GUI_Draw_Frame_8( 0x22, mRecruit_Truck_Frame, 0x31, DataC);

    // If no troop has reached the truck, don't do animation
    if (!mRecruit_Truck_Animation_Play)
        return;

    int16 Data0 = *(mRecruit_Truck_FramesPlay + mRecruit_Truck_FrameAnimBufferPtr);
    if (Data0 < 0)
        return;

    ++mRecruit_Truck_FrameAnimBufferPtr;
    mRecruit_Truck_Frame = Data0;
}

void cFodder::Recruit_Copy_Sprites() {

    const sRecruit_Sprites* stru = mRecruitSprite;

    for (; stru->mSpriteType != -1;) {
        int16 word_3B19F = stru->mSpriteType;
        int16 word_3B1A1 = stru->mFrame;
        int16 word_3B1A3 = stru->field_4;
        int16 word_3B1A5 = stru->field_6;
        int16* Data34 = stru->field_8;

        for (;;) {
            auto SpriteSheet = Sprite_Get_Sheet(stru->mSpriteType,stru->mFrame);

            int16 Columns = SpriteSheet->mColCount;
            int16 Rows = SpriteSheet->mRowCount;
            int16 Data8 = *Data34++;
            int16 DataC = 0;

            if (Data8 < 0) {
                ++stru;
                break;
            }

            // Originally Inside sub_A094C
            if (mVersion->mPlatform == ePlatform::Amiga) {
                Columns -= 1;
                Columns <<= 4;
            }

            // Multiply 
            sub_2AEB6( Columns, Rows, &Data8, &DataC );

            int16 Data10 = word_3B1A3 + 0x08;
            int16 Data14 = word_3B1A5;
            mGraphics->Recruit_Sprite_Draw( Columns, Rows,
                                            Data8, Data10, 
                                            Data14, DataC, SpriteSheet->GetGraphicsPtr() );
            word_3B1A3 += 0x10;
        }
    }
}

void cFodder::Recruit_Draw() {
    Mouse_Inputs_Get();
    
    Recruit_Draw_Actors();
    mGraphics->Sidebar_Copy_To_Surface(0x18);

    if (mVersion->mPlatform == ePlatform::PC)
        mGraphics->Recruit_Draw_HomeAway();

    Mouse_DrawCursor();
    Video_Sleep_Wrapper();

    if (mImage->GetFaded() == false )
        mImage->palette_FadeTowardNew();

    mVideo_Draw_PosX = 0x40;
    mVideo_Draw_PosY = 0x28;
    mVideo_Draw_Columns = 0x110;
    mVideo_Draw_Rows = 0xB0;

    g_Window.RenderAt( mImage );
    g_Window.FrameEnd();
    mImage->Restore();
}

bool cFodder::Recruit_Check_Buttons_SaveLoad() {
    mRecruit_Button_Load_Pressed = 0;
    mRecruit_Button_Save_Pressed = 0;

    if (mMouseX <= -16 && mMouseY <= 26) {
        mRecruit_Button_Load_Pressed = -1;
        return true;
    }

    if (mMouseX >= 0x10F) {
        if (mMouseY > 0x1A)
            return true;

        if (mMission_Save_Availability[mMissionNumber-1])
            return false;

        mRecruit_Button_Save_Pressed = -1;
        return true;
    }

    if (mMouseY < 0x1A)
        return false;

    return true;
}

void cFodder::Sprite_Handle_Player_Enter_Vehicle( sSprite* pSprite ) {
    
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
    if (Map_Get_Distance_BetweenPoints( VehicleX, VehicleY, SoldierX, Distance, SoldierY ) >= 0x0D)
        return;

    // Enter the vehicle
    pSprite->field_6A_sprite = Vehicle;
    pSprite->field_6E = -1;
    sSprite* Data20 = 0;

    sub_305D5( Data20 );

    // First member of current player squad
    sSprite* eax = mSquads[pSprite->field_32][0];

    // Is this the first member of the squad
    if (eax != pSprite)
        return;

    mSquad_EnteredVehicleTimer[pSprite->field_32] = 0x190;
}

void cFodder::GUI_Handle_Button_ShowOverview() {

    if (mMission_Finished)
        return;

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    mMission_ShowMapOverview = -1;
}

void cFodder::sub_3037A( ) {
    
    if (GUI_Sidebar_SelectedTroops_Count()) {

        if (word_3AC4B < 0)
            return;

        word_3AC4B = -1;
        sub_303AE();
        return;
    }

    if (!word_3AC4B)
        return;

    word_3AC4B = 0;
    mSquad_Grenade_SplitMode = 1;

    GUI_Sidebar_Grenades_CurrentSquad_Draw();
    sub_303B7();
}

void cFodder::sub_3049B( ) {

    if (GUI_Sidebar_SelectedTroops_Count()) {

        if (word_3AC4D < 0)
            return;

        word_3AC4D = -1;
        GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper();
        return;
    }

    if (word_3AC4D) {
        word_3AC4D = 0;
        mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;

        GUI_Sidebar_Rockets_CurrentSquad_Draw();
    }
}


void cFodder::GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper( ) {
    GUI_Sidebar_Rockets_CurrentSquad_Draw();
}

void cFodder::Mouse_Cursor_Update() {
    int16 Data0, Data4, Data8;
    sSprite* Vehicle = 0;
    sSprite** HumanVehicles = 0;

	// Sidebar
    if (mMouseX < 0x10) {
        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
        return;
    }

    if (mButtonPressRight)
        goto loc_30CBC;

    Data0 = mMouseX + (mCamera_Adjust_Col >> 16);
    Data4 = mMouseY + (mCamera_Adjust_Row >> 16);

    Data0 -= 0x0F;
    Data4 -= 3;
    
    HumanVehicles = mSprites_HumanVehicles;

    //loc_30B36
    for (int16 Data1C = 4; Data1C >= 0; --Data1C) {

        if (*HumanVehicles == INVALID_SPRITE_PTR )
            goto loc_30CBC;

        if (*HumanVehicles == 0)
            continue;

        Vehicle = *HumanVehicles++;
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

        if (!Vehicle->field_20)
            goto loc_30C7C;

        if (Vehicle != mSquad_CurrentVehicle)
            return;

        if (Vehicle->field_8 == 0xA5)
            return;

        mMouseSpriteNew = eSprite_pStuff_Mouse_Helicopter;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
        return;

    loc_30C7C:;

        if (Vehicle == mSquad_CurrentVehicle) 
            mMouseSpriteNew = eSprite_pStuff_Mouse_Arrow_DownRight;
        else
            mMouseSpriteNew = eSprite_pStuff_Mouse_Arrow_UpLeft;

        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
        return;
    }

loc_30CBC:;

    if (mMouseSetToCursor) {
        mMouseSetToCursor = 0;
        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;
        mMouseX_Offset = 0;
        mMouseY_Offset = 0;
    }
}

void cFodder::sub_22C87( sSprite* pSprite ) {
    
    int32 Data0 = pSprite->field_1A;

    pSprite->field_1E_Big += Data0;

    if (pSprite->field_1E_Big < 0) {
        pSprite->field_1E_Big = 0;
        Data0 = -Data0;
        Data0 >>= 1;
    }

    Data0 -= 0x20000;
    pSprite->field_1A = (int32) Data0;
}

void cFodder::sub_22CD7( sSprite* pSprite, int16& pData0, int16& pData4 ) {
    
    int16 Data1C;

    pSprite->field_3C = pSprite->field_10;

    Sprite_Direction_Between_Points( pSprite, pData0, pData4 );

    mSprite_Field10_Saved = pSprite->field_10;
    pSprite->field_3E = 0;

    sub_2B12E( pSprite );

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

    if (!mMission_EngineTicks & 1)
        return;

loc_22DC4:;
    if (pSprite->field_36 <= 8)
        return;

loc_22DCF:;
    pData0 = -pData0;
    if (!pData0) {
        Data1C = 0;
    } else if (pData0 >= 0) {
        Data1C = 1;
    } else {
        Data1C = -1;
    }

    Sprite_Vehicle_Direction_Update( pSprite, Data1C );
}

void cFodder::Sprite_Handle_Vehicle_Terrain_Check( sSprite* pSprite ) {
    int16 Data4, Data0;
    int32 Dataa0;

    pSprite->field_50 = pSprite->field_52;
    if (!pSprite->field_50)
        pSprite->field_52 = 0;

    if (Map_Sprite_Check_Around_Position( pSprite ))
        goto Computer_Vehicle_SoftTerrain;

    Data4 = 0x0F;
    Data0 = -10;
    Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );

    pSprite->field_60 = static_cast<int8>(Data4);
    if (Data4 == eTerrainType_Rocky || Data4 == eTerrainType_Rocky2)
        goto loc_23056;

    if (Data4 == eTerrainType_Jump)
        goto loc_23100;

    if (Data4 == eTerrainType_BounceOff)
        goto Computer_Vehicle_SoftTerrain;

    if (Data4 == eTerrainType_Drop || Data4 == 0x0A)
        goto loc_22F06;

    if (pSprite->field_56)
        pSprite->field_38 = eSprite_Anim_Die1;

    if (Data4 == eTerrainType_Snow)
        goto loc_22FA3;

    if (pSprite->field_22 == eSprite_PersonType_Human)
        goto Human_Vehicle;

    if (    Data4 == eTerrainType_QuickSand || Data4 == eTerrainType_WaterEdge 
         || Data4 == eTerrainType_Water || Data4 == eTerrainType_Sink)
        goto Computer_Vehicle_SoftTerrain;
    
    goto loc_22EEB;

Human_Vehicle:;

    if (Data4 == eTerrainType_QuickSand)
        goto Human_Vehicle_Quicksand;

    if (Data4 == eTerrainType_WaterEdge)
        goto Human_Vehicle_WaterEdge;

    if (Data4 == eTerrainType_Water || Data4 == eTerrainType_Sink)
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
    Sprite_XY_Restore( pSprite );

    Data4 = 0x0F;
    Data0 = -10;
    Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
    if (Data4 == eTerrainType_BounceOff)
        pSprite->field_38 = eSprite_Anim_Die1;

    Sprite_Animation_SlideOrDie( pSprite );
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

    Sprite_Handle_Vehicle_Sinking( pSprite );
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

void cFodder::Sprite_Under_Vehicle( sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14, int16 pData18, int16 pData1C ) {

    if (mMission_Finished)
        return;

    sSprite* Sprite = mSprites;

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

        if (Sprite->field_18 == eSprite_Hostage_2)
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

int16 cFodder::Sprite_Animation_SlideOrDie( sSprite* pSprite ) {
    sSprite* Data24 = 0;

    // Die animation running?
    if (pSprite->field_38 == eSprite_Anim_Die1 || pSprite->field_38 == eSprite_Anim_Die3) {
        pSprite->field_18 = eSprite_Explosion;
        pSprite->field_26 = 0x1F56;
        pSprite->field_28 = -9;
        Data24 = pSprite + 1;

        Sprite_Destroy( Data24 );
        if (pSprite->field_22 == eSprite_PersonType_Human)
            return -1;

        Data24 = pSprite + 2;
        Sprite_Destroy( Data24 );
        return -1;
    }

    // Slide animation running?
    if (pSprite->field_38 == eSprite_Anim_Slide1) {

        pSprite->field_36 = -pSprite->field_36;
        pSprite->field_38 = eSprite_Anim_Slide2;

    } else if (pSprite->field_38 == eSprite_Anim_Slide2) {
        pSprite->field_36 += 6;
        
        if (pSprite->field_36 >= 0) {
            pSprite->field_38 = eSprite_Anim_None;
            pSprite->field_36 = 0;
        }

    } else {
        return 0;
    }

    Sprite_Movement_Calculate( pSprite );
    sub_243E9( pSprite );
    return -1;
}

int16 cFodder::Map_Sprite_Check_Around_Position( sSprite* pSprite ) {
    int16 Data4 = 0x0A;
    int16 Data0 = -13;

    int16 ax = Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
    if (ax)
        return ax;

    Data4 = 0x14;
    Data0 = -13;
    ax = Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
    if (ax)
        return ax;

    Data4 = 0x0A;
    Data0 = -7;
    ax = Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
    if (ax)
        return ax;

    Data4 = 0x14;
    Data0 = -7;
    ax = Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
    if (ax)
        return ax;

    return 0;
}

void cFodder::Sprite_Handle_Vehicle_Human( sSprite* pSprite ) {

    if (pSprite->field_38) {

        pSprite->field_22 = eSprite_PersonType_Human;
        Sprite_Handle_Vehicle( pSprite );
        pSprite->field_22 = eSprite_PersonType_Human;
        return;
    }

    if (pSprite->field_6F == eVehicle_JeepRocket)
        sub_245BF( pSprite );

    mSprite_Helicopter_DestroyLight = 0;
    pSprite->field_22 = eSprite_PersonType_Human;
    Sprite_Handle_Vehicle( pSprite );
    pSprite->field_22 = eSprite_PersonType_Human;
    mSprite_Helicopter_DestroyLight = 0;

    sSprite* Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;
}

int16 cFodder::Sprite_Create_Smoke( sSprite* pSprite, sSprite*& pData2C ) {
    if (!pSprite->field_5C)
        return -1;

    int16 Data0 = 1;
    sSprite* Data30 = 0;

    if (Sprite_Get_Free( Data0, pData2C, Data30 ))
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
    pData2C->field_2C = eSprite_Draw_Last;
    pData2C->field_18 = eSprite_Smoke;
    return 0;
}

void cFodder::sub_23525( sSprite* pSprite ) {
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

    Sprite_Map_Sound_Play( Data0 );

loc_2356B:;
    pSprite->field_6F = eVehicle_Tank;
    pSprite->field_65 = -1;
    if (Sprite_Animation_SlideOrDie( pSprite ))
        return;

    pSprite->field_8 = 0xD1;
    word_3B2F7 = pSprite->field_36 + 0x20;

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;

    sub_22CD7( pSprite, Data0, Data4 );
    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;

    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 >= 0x1E)
        goto loc_2361A;

    if (Data0 >= pSprite->field_36)
        goto loc_23680;

    pSprite->field_36 = Data0;
    goto loc_23680;

loc_2361A:;
    dword_3B24B = -1;
    if (!Sprite_Create_Smoke( pSprite, Data2C))
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

    Sprite_XY_Store( pSprite );
    Sprite_Movement_Calculate( pSprite );
    Sprite_Handle_Vehicle_Terrain_Check( pSprite );

    pSprite->field_20 = 0;
    sub_236F7( pSprite );

    Data8 = pSprite->field_0;
    DataC = pSprite->field_0 + 0x1E;
    Data10 = pSprite->field_4 - 0x14;
    Data14 = pSprite->field_4;
    Data18 = pSprite->field_20;
    Data1C = Data18 + 0x0E;

    Sprite_Under_Vehicle( pSprite, Data8, DataC, Data10, Data14, Data18, Data1C );
}

void cFodder::sub_236F7( sSprite* pSprite ) {
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

    Data8 = mMouseX + (mCamera_Adjust_Col >> 16);
    Data8 -= 0x10;

    DataC = mMouseY + (mCamera_Adjust_Row >> 16);
loc_23843:;
    Direction_Between_Points( Data0, Data4, Data8, DataC );

    Data4 -= 0x10;
    Data4 >>= 5;
    Data4 += 8;
    Data4 ^= 0x0F;
    Data4 &= 0x0F;
    Data24->field_A = Data4;
}

void cFodder::Sprite_Handle_Tank_FireMissile( sSprite* pSprite ) {
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

    if (!Sprite_Create_Missile( Data24, Data2C )) {
        Data24->field_4 = Data24_Field_4;
        Data24->field_0 = Data24_Field_0;
        pSprite->field_57 = -1;
        return;
    }

    Data2C->field_20 += 0x11;
    Data2C->field_36 = 0x3C;
    Sound_Play( Data24, 5, 0x1E );
    Sprite_Create_Smoke( Data24, Data2C );
    
    Data24->field_4 = Data24_Field_4;
    Data24->field_0 = Data24_Field_0;
    pSprite->field_57 = 0;
}

int16 cFodder::Sprite_Create_Missile( sSprite* pSprite, sSprite*& pData2C ) {
    if (mMission_Completed_Timer)
        return 0;

    if (byte_3A9D6[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    int16 Data0 = 2;
    sSprite* Data30 = 0;
    if (Sprite_Get_Free( Data0, pData2C, Data30 ))
        return 0;

    ++byte_3A9D6[pSprite->field_22];
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
        Sprite_Enemy_Set_Target( pData2C );
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

    // HACK: Disable sound for Amiga Plus
    if (mVersion->mVersion != eVersion::AmigaPlus)
        Sound_Play( pSprite, eSound_Effect_Missile_Launch, 0x0F );

    return -1;
}

void cFodder::Sprite_Enemy_Set_Target( sSprite* pData2C ) {
    
    int16 Data0 = tool_RandomGet();
    int16 Data4 = Data0;

    // Bigger X movement range for low aggression
    if (mSprite_Enemy_AggressionAverage < 5)
        Data0 &= 0x3F;
    else
        Data0 &= 0x1F;

    if (Data4 < 0)
        Data0 = -Data0;

    pData2C->field_26 += Data0;

    Data0 = tool_RandomGet();
    Data4 = Data0;

    // Bigger Y movement range for low aggression
    if (mSprite_Enemy_AggressionAverage < 5)
        Data0 &= 0x3F;
    else
        Data0 &= 0x1F;
    if (Data4 < 0)
        Data0 = -Data0;

    pData2C->field_28 += Data0;
}

void cFodder::Sprite_Handle_Helicopter_Human( sSprite* pSprite ) {
    
    if (pSprite->field_38) {

        pSprite->field_22 = eSprite_PersonType_Human;
        Sprite_Handle_Helicopter( pSprite );
        pSprite->field_22 = eSprite_PersonType_Human;

        return;
    }

    Sprite_Handle_Helicopter_Human_Deploy_Weapon( pSprite );

    mSprite_Helicopter_DestroyLight = 0;
    pSprite->field_22 = eSprite_PersonType_Human;
    Sprite_Handle_Helicopter( pSprite );
    pSprite->field_22 = eSprite_PersonType_Human;
    mSprite_Helicopter_DestroyLight = 0;

    sSprite* Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;
    Data24 = pSprite + 2;
    Data24->field_18 = eSprite_Null;
}

int16 cFodder::Sprite_Handle_Helicopter_Terrain_Check( sSprite* pSprite ) {
    int16 Data4 = 0x0F;
    int16 Data0 = -10;

    Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
    switch (Data4) {
        case eTerrainType_Rocky:
        case eTerrainType_QuickSand:
        case eTerrainType_WaterEdge:
        case eTerrainType_Water:
        case eTerrainType_Sink:
            Data0 = 0x0C;
            break;

        case eTerrainType_BounceOff:
            Data0 = 0x14;
            break;
        case eTerrainType_Rocky2:
        case eTerrainType_Drop:
        case eTerrainType_Drop2:
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

void cFodder::sub_23E01( sSprite* pSprite, int16& pData0, int16& pData4 ) {
    if (pSprite->field_44) {
        pSprite->field_44 -= 1;

        if (pSprite->field_44)
            goto loc_23E2F;
    }

    pSprite->field_3C = pSprite->field_10;
    Sprite_Direction_Between_Points( pSprite, pData0, pData4 );

loc_23E2F:;
    sub_2B12E( pSprite );

    pSprite->field_10 = pSprite->field_3C;
    pData0 = mDirectionMod;
    pData0 <<= 3;

    if (pSprite->field_36 > 0x18)
        pData0 <<= 1;
    pSprite->field_10 = pData0;
    pSprite->field_10 &= 0x1FE;

    Sprite_Set_Direction_To_Next( pSprite );

    int16 Data8 = pSprite->field_3C;
    if (!pSprite->field_36)
        return;

    pSprite->field_A = Data8 >> 1;
}

void cFodder::Sprites_HumanVehicles_Remove( sSprite* pSprite ) {
    
    for (sSprite** Data24 = mSprites_HumanVehicles; *Data24 != INVALID_SPRITE_PTR; ++Data24) {

        if (*Data24 != pSprite)
            continue;

        do {
            sSprite* eax = *(Data24+1);
            *Data24 = eax;

            ++Data24;
        } while (*Data24 != INVALID_SPRITE_PTR);

        return;
    }
}

void cFodder::Sprite_Handle_Turret( sSprite* pSprite ) {
    sSprite* Data28 = 0;
    sSprite* Data34 = 0;
    int16 Data0, Data4, Data8, DataC, Data10;

    // Turrets in Moors / Interior can't be destroyed
    if (mMap_TileSet == eTileTypes_Moors || mMap_TileSet == eTileTypes_Int) {
        if (pSprite->field_38 == eSprite_Anim_Die1)
            pSprite->field_38 = eSprite_Anim_None;
    }

    if (pSprite->field_38 == eSprite_Anim_Die3 || pSprite->field_38 == eSprite_Anim_Die1) {

        pSprite->field_18 = eSprite_Explosion;
        pSprite->field_26 = 0x1F45;
        pSprite->field_28 = -2;
        if (pSprite->field_22 == eSprite_PersonType_Human)
            return;

        Sprite_Destroy( pSprite + 1 );
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

    if (mPhase_Goals[ eGoal_Get_Civilian_Home - 1 ]) {
        Data4 = eSprite_Indigenous;
        Data8 = eSprite_Indigenous2;
        DataC = eSprite_Indigenous_Spear;
        Data10 = -1;
    }

    if (Sprite_Find_By_Types(pSprite, Data0, Data4, Data8, DataC, Data10, Data28))
        goto loc_240F3;

    if (mSprite_Find_Distance >= 0x28) {

        Data0 = tool_RandomGet();
        if (mSprite_Enemy_AggressionAverage < 5)
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
    Data8 = mMouseX + (mCamera_Adjust_Col >> 16);
    Data8 -= 0x10;
    DataC = mMouseY + (mCamera_Adjust_Row >> 16);

loc_240C8:;
    Direction_Between_Points( Data0, Data4, Data8, DataC );
    Data4 -= 0x10;
    Data4 >>= 5;
    Data4 += 8;
    Data4 ^= 0x0F;
    Data4 &= 0x0F;

    pSprite->field_A = Data4;
loc_240F3:;

    Sprite_Handle_Turret_Fire( pSprite, Data34 );

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

void cFodder::Sprite_Handle_Turret_Fire( sSprite* pSprite, sSprite* pData34 ) {
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

    ax = Sprite_Create_MissileHoming( pSprite, Data2C, pData34 );
    goto loc_24234;

loc_2421D:;

    if (pSprite->field_22 != eSprite_PersonType_Human)
        if (word_3B4ED[1] > 9)
            goto loc_24275;

    ax = Sprite_Create_Missile( pSprite, Data2C );

loc_24234:;
    if (!ax)
        goto loc_2426C;

    Data2C->field_20 += 0x11;
    Data2C->field_36 = 0x3C;
    Sound_Play( pSprite, eSound_Effect_Turret_Fire, 0x1E );
    Sprite_Create_Smoke( pSprite, Data2C );
    pSprite->field_57 = 0;
    goto loc_24275;

loc_2426C:;
    pSprite->field_57 = -1;

loc_24275:;
    pSprite->field_4 = Field_4;
    pSprite->field_0 = Field_0;
}

int16 cFodder::Sprite_Find_By_Types( sSprite* pSprite, int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData10, sSprite*& pData28 ) {
    int16* Data2C = word_3B2DD;

    word_3B2DD[0] = pData0;
    word_3B2DD[1] = pData4;
    word_3B2DD[2] = pData8;
    word_3B2DD[3] = pDataC;
    word_3B2DD[4] = pData10;

    int16 Data1C = pSprite->field_5E;

    pData28 = &mSprites[Data1C];
    
    if (pData28->field_0 == -32768)
        goto loc_243A6;

    do {
        pData0 = *Data2C++;
        if (pData0 < 0)
            goto loc_243A6;

    } while (pData0 != pData28->field_18);

    pData0 = pSprite->field_0;
    pData4 = pSprite->field_4;
    pData8 = pData28->field_0;
    pDataC = pData28->field_4;

    Map_Get_Distance_BetweenPoints_Within_320( pData0, pData4, pData8, pDataC );

    mSprite_Find_Distance = pData0;

    if (pData0 >= 0xD2)
        goto loc_243A6;

    if (pData0 <= 0x28)
        goto loc_243C7;

    pData0 = pSprite->field_0;
    pData4 = pSprite->field_4;
    pData8 = pData28->field_0;
    pDataC = pData28->field_4;

    if (sub_2A4A2( pData0, pData4, pData8, pDataC ))
        goto loc_2439F;

    pData0 = mSprite_Find_Distance;
    goto loc_243C7;

loc_2439F:;
    if (mSprite_Find_Distance < 0x40)
        goto loc_243C7;

loc_243A6:;
    pSprite->field_5E++; //+= 0x76; This should be a sprite counter
    if (pSprite->field_5E >= 43)
        pSprite->field_5E = 0;

    goto loc_243DD;
loc_243C7:;
    pData4 = pData28->field_18;
    return 0;

loc_243DD:;
    return -1;
}

void cFodder::sub_243E9( sSprite* pSprite ) {
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

int16 cFodder::Sprite_Handle_Vehicle_Sinking( sSprite* pSprite ) {
    
    Sound_Play( pSprite, eSound_Effect_Vehicle_Sinking, 0x0F );
    if (!pSprite->field_5C)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;
    
    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
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
    Data2C->field_2C = eSprite_Draw_Last;

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

void cFodder::sub_245BF( sSprite* pSprite ) {
    
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

    if (!Sprite_Create_Cannon( pSprite )) {
        pSprite->field_5B = ~pSprite->field_5B;
        Sound_Play( pSprite, 0x10, 0x1E );
    }

    //loc_246BC
    pSprite->field_4 = Field_4;
    pSprite->field_0 = Field_0;
}

int16 cFodder::Sprite_Create_Cannon( sSprite* pSprite ) {
    if (mMission_Completed_Timer)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
        return -1;

    if (byte_3A9D2[2] == 0x14)
        return -1;

    ++byte_3A9D2[2];
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

void cFodder::Sprite_Handle_Helicopter_Human_Deploy_Weapon( sSprite* pSprite ) {
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
    if (!Sprite_Create_Grenade2( pSprite ))
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

    } else {
        // Launch a missile
        if (!Sprite_Create_Missile(pSprite, Data2C))
            goto loc_2490D;
    }

loc_24942:;
    pSprite->field_57 = 0;
}

int16 cFodder::Sprite_Create_Grenade2( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC;

    if (mMission_Completed_Timer)
        return 0;

    if (byte_3A9D2[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    Data0 = 2;
    sSprite* Data2C, *Data30;
    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
        return 0;

    ++byte_3A9D2[pSprite->field_22];
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
        Sprite_Enemy_Set_Target( Data2C );

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
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

int16 cFodder::Sprite_Create_MissileHoming( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData34 ) {
    
    if (mMission_Completed_Timer)
        return 0;

    if (byte_3A9D6[pSprite->field_22] == 2)
        return 0;

    if (!pSprite->field_2E)
        return 0;

    int16 Data0 = 2;
    sSprite *Data30 = 0;
    if (Sprite_Get_Free( Data0, pData2C, Data30 ))
        return 0;

    ++byte_3A9D6[pSprite->field_22];

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

    if (mVersion->mVersion == eVersion::AmigaPlus)
        Sound_Play( pSprite, 0x10, 0x0F );
    else
        Sound_Play( pSprite, eSound_Effect_Turret_Fire, 0x0F );
    return -1;
}

int16 cFodder::sub_2531F( sSprite* pSprite, sSprite*& pData30 ) {
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

void cFodder::Sprite_Handle_Vehicle_Enemy( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data28 = 0;

    if (pSprite->field_38) {
        pSprite->field_22 = eSprite_PersonType_AI;
        Sprite_Handle_Vehicle( pSprite );
        pSprite->field_22 = eSprite_PersonType_AI;
        return;
    }

    if (pSprite->field_6F == eVehicle_Unknown)
        goto loc_255DA;

    Data0 = eSprite_Player;
    Data4 = eSprite_Indigenous;
    Data8 = 0x3E;
    DataC = 0x46;
    Data10 = -1;
    if (Sprite_Find_By_Types( pSprite, Data0, Data4, Data8, DataC, Data10, Data28 ))
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
    Sprite_Handle_Vehicle( pSprite );

    if (pSprite->field_6F == eVehicle_JeepRocket)
        sub_245BF( pSprite );

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

void cFodder::Sprite_Handle_Helicopter_Enemy( sSprite* pSprite ) {
    int16 Data0;
    int16 Data4;
    int16 Data8;
    int16 DataC;
    int16 Data10, Data14, Data1C;
    sSprite* Data24 = 0, *Data30 = 0;
    sSprite** Dataa30 = 0;

    // Bullets don't hit choppers
    if (pSprite->field_38 == eSprite_Anim_Hit)
        pSprite->field_38 = eSprite_Anim_None;

    if (pSprite->field_38) {
        mSprite_Helicopter_DestroyLight = -1;

        pSprite->field_22 = eSprite_PersonType_AI;
        Sprite_Handle_Helicopter( pSprite );
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
    Data8 = ((int64) pSprite->field_46) >> 16;
    DataC = ((int64) pSprite->field_46) & 0xFFFF;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 > 0x14)
        goto loc_250D2;

    Data8 = pSprite->field_62;
    if ((uint16) Data8 >= 0x1F4) {
        if (pSprite->field_18 == eSprite_Helicopter_Homing_Enemy2)
            if (mHelicopterCall_X < 0)
                goto loc_24FF1;

        if (!(tool_RandomGet() & 1))
            goto loc_2500F;
    }

loc_24FF1:;
    if (mVersion->mRelease == eRelease::Demo) {
        Data8 += 0x20;
        if (Data8)
            pSprite->field_62 = Data8;

    } else {
        Data8 += 1;
        Data8 += mSprite_Enemy_AggressionAverage;

        if (Data8 >= 0)
            pSprite->field_62 = Data8;
    }

    goto loc_25239;

loc_2500F:;
    if (pSprite->field_4C)
        goto loc_25239;

    Data0 = tool_RandomGet() & 0x7F;
    Data0 += 4;
    if (Data0 > mMapWidth)
        goto loc_25239;

    Data8 = Data0;
    Data0 = tool_RandomGet() & 0x3F;
    Data0 += 4;

    if (Data0 > mMapHeight)
        goto loc_25239;

    DataC = Data0;
    Data8 <<= 4;
    Data0 = tool_RandomGet() & 0x0F;
    Data8 += Data0;

    Data0 = tool_RandomGet() & 0x0F;
    DataC += Data0;

    if (Map_Terrain_Get_Moveable_Wrapper( mTiles_NotFlyable, Data8, DataC, Data10, Data14 ))
        goto loc_25239;

    pSprite->field_26 = Data10;
    pSprite->field_28 = Data14;
    pSprite->field_4C = 0x5A;
    goto loc_25239;

loc_250D2:;

    if (pSprite->field_6F == eVehicle_Helicopter)
        goto loc_251D2;

    Data1C = pSprite->field_5E;

    Dataa30 = mSquad_0_Sprites;
    Data30 = Dataa30[Data1C];
    if (Data30 == INVALID_SPRITE_PTR)
        goto loc_251B4;

    if (Data30->field_52)
        goto loc_251B4;

    Data8 = Data30->field_0;
    Data8 += 8;
    DataC = Data30->field_4;
    DataC += -5;

    if (Map_Terrain_Get_Moveable_Wrapper( mTiles_NotDriveable, Data8, DataC, Data10, Data14 ))
        goto loc_251B4;
    
    Data0 = Data30->field_0;
    Data4 = Data30->field_4;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );

    mSprite_DistanceTo_Squad0 = Data0;
    if (Data0 < 0xFA)
        if (sub_2531F( pSprite, Data30 ))
            goto loc_251D2;

loc_251B4:;
    pSprite->field_5E += 1;
    if (pSprite->field_5E >= 0x1E)
        pSprite->field_5E = 0;

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
            Sprite_Create_Enemy( pSprite, Data2C );
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

void cFodder::Sprite_Handle_Indigenous_Unk( sSprite* pSprite ) {
    if (!pSprite->field_5C)
        return;

    if ((tool_RandomGet() & 0x3F) != 9)
        return;

    if (mSquad_Leader == INVALID_SPRITE_PTR)
        return;

    pSprite->field_2E = mSquad_Leader->field_0;
    pSprite->field_30 = mSquad_Leader->field_4;

    Sprite_Create_Indigenous_Spear2( pSprite );
}

void cFodder::Sprite_Handle_Indigenous_Movement( sSprite* pSprite ) {
    
    // Destination change cool down
    if (pSprite->field_4C) {
        pSprite->field_4C -= 1;
        if (pSprite->field_4C)
            return;
    }

    if (pSprite->field_18 == eSprite_Hostage || pSprite->field_18 == eSprite_Hostage_2) {
        pSprite->field_26 = pSprite->field_0;
        pSprite->field_28 = pSprite->field_4;
    }
    else {
        if (Sprite_Handle_Indigenous_RandomMovement( pSprite ) < 0)
            return;
    }

    // Random time until destination change
    int16 Data0 = tool_RandomGet() & 0x3F;
    Data0 += 0x14;
    pSprite->field_4C = static_cast<int8>(Data0);
}

int16 cFodder::Sprite_Handle_Indigenous_Within_Range_OpenCloseDoor( sSprite* pSprite ) {

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

    mSprite_Indigenous_Tmp_X = Data0;
    mSprite_Indigenous_Tmp_Y = Data4;
    word_3B481 = Data8;
    word_3B483 = DataC;
    int16 Data10 = 0x20;

    Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
    if (Data0 <= 3) {
        Sprite_Destroy_Wrapper( pSprite );
        mSprite_Civilian_GotHome = -1;
        return 1;
    }

    if (Data0 < 6) {
        Data28->field_2A = 0x0A;
        return 0;
    }

    Data0 = mSprite_Indigenous_Tmp_X;
    Data4 = mSprite_Indigenous_Tmp_Y;
    Data8 = word_3B481;
    DataC = word_3B483;

    if (!sub_2A4A2( Data0, Data4, Data8, DataC ))
        return 0;

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

void cFodder::Sprite_Handle_Indigenous_Death( sSprite* pSprite ) {
    
    if (pSprite->field_8 != 0xD6) {
        word_3B2D1[2] = -1;

        if (mPhase_Goals[ eGoal_Protect_Civilians - 1 ])
            mMission_Aborted = true;

        if (mPhase_Goals[ eGoal_Get_Civilian_Home - 1 ])
            mMission_Aborted = true;

        pSprite->field_8 = 0xD6;
        pSprite->field_A = 0;
        
        int16 Data0 = tool_RandomGet() & 7;
        int16 Data4 = mSprite_Indigenous_Sound_Death[Data0];

        Sound_Play( pSprite, Data4, 0x14 );
    }
    else {
        //loc_2584A
        pSprite->field_A += 1;
        if (pSprite->field_A < 9)
            return;

        Sprite_Destroy_Wrapper( pSprite );
    }
}

void cFodder::sub_2593D( sSprite* pSprite ) {
    Sprite_XY_Store( pSprite );

    if (pSprite->field_44)
        --pSprite->field_44;
    else {
        int16 Data0 = pSprite->field_26;
        int16 Data4 = pSprite->field_28;

        if (Sprite_Direction_Between_Points( pSprite, Data0, Data4 ) < 0)
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
    if (!Sprite_Reached_Target( pSprite )) {
        Sprite_Movement_Speed_Update( pSprite );

        if (!word_3B25B)
            Sprite_Movement_Calculate( pSprite );
    }
    //loc_259D4
    Sprite_Reached_MapEdge( pSprite );

    int16 Data4 = 0;
    Sprite_Terrain_Check( pSprite, Data4 );

    Data4 = pSprite->field_60;

    if (Data4 == 6) {
        Sprite_XY_Restore( pSprite );
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

void cFodder::sub_25A31( sSprite* pSprite ) {
    
    if (mSprite_Reached_Target || word_3B25B)
        return;

    pSprite->field_2A += 1;
    pSprite->field_2A &= 3;
    if (pSprite->field_2A)
        return;

    pSprite->field_A += 1;
    pSprite->field_A &= 1;
}

void cFodder::sub_25A66( sSprite* pSprite ) {
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

int16 cFodder::Sprite_Handle_Indigenous_RandomMovement( sSprite* pSprite ) {
    int16 Data0 = tool_RandomGet() & 0x7F;
    Data0 += 4;

    // Map Width
    if (Data0 >= mMapWidth)
        return -1;

    int16 Data8 = Data0;
    Data0 = tool_RandomGet() & 0x3F;
    Data0 += 4;

    // Map Height
    if (Data0 >= mMapHeight)
        return -1;

    int16 DataC = Data0;
    Data8 <<= 4;

    Data0 = tool_RandomGet() & 0x0F;
    Data8 += Data0;
    DataC <<= 4;

    Data0 = tool_RandomGet() & 0x0F;
    DataC += Data0;

    int16 Data10, Data14;

    if (Map_Terrain_Get_Moveable_Wrapper( mTiles_NotFlyable, Data8, DataC, Data10, Data14 ))
        return -1;

    pSprite->field_26 = Data10;
    pSprite->field_28 = Data14;
    return 0;
}

int16 cFodder::Sprite_Create_Indigenous_Spear2( sSprite* pSprite ) {
    if (mMission_Completed_Timer)
        return -1;

    if (!pSprite->field_2E)
        return -1;

    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;
    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
        return -1;

    if (byte_3A9D2[2] == 0x14)
        return -1;
    
    ++byte_3A9D2[2];
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
    Data2C->field_18 = eSprite_Indigenous_Spear2;
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

    if (mMap_TileSet == eTileTypes_Moors) {
        Data0 = tool_RandomGet() & 1;
        if (Data0)
            goto loc_25D9D;
        Data4 = 0x11;
        goto loc_25DA3;

    loc_25D9D:;
        Data4 = 0x10;

    loc_25DA3:;
        Sound_Play( pSprite, Data4, 0 );
    }

    //loc_25DB1
    Data2C->field_64 = 0;
    return 0;
}

int16 cFodder::sub_25DCF( sSprite* pSprite ) {
    sSprite* Data2C = 0, *Data30 = 0;
    int16 Data0;

    if (!pSprite->field_38)
        return 0;

    if (pSprite->field_38 == eSprite_Anim_Die4)
        goto loc_25E8F;

    if (!pSprite->field_2A) {
        pSprite->field_2A = 0x14;
        pSprite->field_A = 0;
    }
    pSprite->field_2A -= 1;
    if (!pSprite->field_2A)
        return Sprite_Destroy_Wrapper( pSprite );

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
    if (Sprite_Create_BloodTrail( pSprite, Data2C, Data30 ))
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

void cFodder::sub_25F2B( sSprite* pSprite ) {
    
    if (word_3B2ED) {
        pSprite->field_8 = 0xD8;
        pSprite->field_A = 1;
        Sprite_XY_Restore( pSprite );
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

void cFodder::Sprite_Handle_Hostage_Movement( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC, Data10;
    sSprite* Data28 = 0;

    if (pSprite->field_18 == eSprite_Hostage_2)
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
    Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
    if (Data10 < 0x7F)
        pSprite->field_5E = (int16) (mHostage_Rescue_Tent - mSprites);

loc_2608B:;
    word_3B2ED = 0;
    Data0 = eSprite_Hostage_Rescue_Tent;
    Data4 = eSprite_Enemy;
    Data8 = eSprite_Player;
    DataC = -1;
    if (Sprite_Find_By_Types( pSprite, Data0, Data4, Data8, DataC, Data10, Data28 ))
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
	} else if (Data4 == eSprite_Enemy) {
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

    Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
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
    if (Sprite_Find_By_Types( pSprite, Data0, Data4, Data8, DataC, Data10, Data28 ))
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

    Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
    if (Data0 <= 0x0F) {
        pSprite->field_6E = -1;
        return;
    }

loc_263E5:;
    Data0 = tool_RandomGet() & 7;
    if (Data0)
        return;

    sub_26490( pSprite );
}

void cFodder::Sprite_Handle_Hostage_FrameUpdate2( sSprite* pSprite ) {
    int16 Data0 = pSprite->field_4A;

    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (!pSprite->field_2A)
        Data0 += 2;

    Data0 &= 6;

    pSprite->field_4A = Data0;
    pSprite->field_A = mSprite_Hostage_Frames[Data0 / 2];

}

void cFodder::Sprite_Handle_Hostage_FrameUpdate( sSprite* pSprite ) {
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

void cFodder::sub_26490( sSprite* pSprite ) {
    ++pSprite->field_5E;
    if (pSprite->field_5E < 43)
        return;

    pSprite->field_5E = 0;
}

void cFodder::sub_264B0( sSprite* pSprite ) {
    int16 Data0, Data4;

    if (!sub_222A3( pSprite ))
        goto loc_264CF;

    if (!mPhase_Goals[ eGoal_Protect_Civilians - 1 ])
        return;

    mMission_Aborted = true;
    return;

loc_264CF:;

    if (mTroops_Enemy_Count >= 0x0A) {
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
    Sound_Play( pSprite, eSound_Effect_BuildingDoor2, 1 );

    Data0 = tool_RandomGet() & 0x0F;
    Data4 = 0x14;
    Data4 -= mSprite_Enemy_AggressionMax;
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

    if (!Sprite_Create_Native( pSprite, Data2C, Data30 )) {
        Data2C->field_4 -= 4;
        return;
    }
        
    pSprite->field_12 = 0;
    pSprite->field_43 = -1;
}

int16 cFodder::Sprite_Handle_Helicopter_Callpad_InRange( sSprite* pSprite, sSprite*& pData2C ) {
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

        Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );

        if (Data0 < 9)
            break;

    } while (1);

    return 0;
}

void cFodder::Sprite_Handle_Helicopter_Human_CallCheck( sSprite* pSprite ) {
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
    Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
    if (Data0 >= 0x2C)
        return;

    pSprite->field_75 = -1;
    pSprite->field_6E = -1;
}

void cFodder::Sprite_Handle_Computer( sSprite* pSprite, int16 pData1C ) {
    
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
    pSprite->field_8 = mSprite_ComputerAnimation[Data0 / 2];
    pSprite->field_20 = mSprite_Computer_Unk[Data0 / 2];
}

int16 cFodder::Map_Get_Distance_BetweenPoints_Within_320( int16& pX, int16& pY, int16& pX2, int16& pY2 ) {
    const int8* Data24 = mMap_Distance_Calculations;
    int16 Data10 = 0;

    pX2 -= pX;
    if (pX2 < 0)
        pX2 = -pX2;

    if (pX2 >= 0x140)
        goto loc_29EBB;

    pY2 -= pY;
    if (pY2 < 0)
        pY2 = -pY2;

    if (pY2 >= 0x140)
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

int16 cFodder::Sprite_Create_Native( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30 ) {
    int16 Data4;

    if (!pSprite->field_5C)
        return -1;

    if (word_3B1A9 >= 0x14)
        return -1;

    if (mMission_Complete)
        return -1;

    if (mTroops_Enemy_Count >= 0x0A)
        return -1;

    int16 Data0 = 1;

    Sprite_Get_Free2( Data0, pData2C, pData30 );
    if (Data0)
        return -1;

    Sprite_Clear( pData2C );
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

int16 cFodder::Direction_Between_Points( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
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

void cFodder::tool_RandomSeed() {
    const time_t now = time(0);
    tm* ltm;

#ifndef _WIN32
    ltm = localtime( &now );
#else
    ltm = new tm();
    localtime_s( ltm, &now );
#endif

    uint16 ax = tool_DecimalToBinaryCodedDecimal( ltm->tm_sec );
    ax |= tool_DecimalToBinaryCodedDecimal(ltm->tm_min) << 8;
    ax += 0x40B;
#ifdef _WIN32
    delete ltm;
#endif
    mRandom_1 = -ax;
    mRandom_0 = ax;
    mRandom_2 = 1;
    mRandom_3 = 0;
}

int16 cFodder::tool_RandomGet() {
    int16 Data0 = mRandom_0;
    int16 Data2 = mRandom_1;
    int16 Data4 = mRandom_2;
    int16 Data6 = mRandom_3;
    
    uint32 Dat4 = Data4 | (Data6 << 16);
    //seg007:053F
    uint8 CF = Data4 & 1;
    uint32 Data8 = Data0 | (Data2 << 16);

    uint8 CF2 = Data8 & 1;
    Data8 >>= 1;

    if (CF)
        Data8 |= 0x80000000;

    Dat4 += CF2;
    Data4 = Dat4 & 0xFFFF;
    Data6 = Dat4 >> 16;

    for (uint16 cx = 0x0C; cx > 0; --cx) {
        CF = 0;

        if (Data0 & 0x8000)
            CF = 1;
        Data0 <<= 1;
        Data2 <<= 1;
        if (CF)
            Data2 |= 1;
    }

    int16 DataA = Data8 >> 16;

    //seg007:0575
    Data0 ^= Data8 & 0xFFFF;
    Data2 ^= DataA;
    Data8 = Data0;
    DataA = Data2;

    //seg007:058F
    int16 ax = Data8;
    int16 bx = DataA;
    Data8 = bx;
    DataA = ax;

    Data8 >>= 4;
    Data0 ^= Data8;

    mRandom_0 = Data0;
    mRandom_1 = Data2;
    mRandom_2 = Data4;
    mRandom_3 = Data6;

    Data2 = 0;
    return Data0;
}

void cFodder::GUI_Sidebar_MapButton_Render() {

    if (mGUI_Sidebar_MapButton_Prepared)
        return;

    sGUI_Element* Element = mGUI_NextFreeElement;
    Element->field_0 = &cFodder::GUI_Button_NoAction;
    Element->mX = 0;
    Element->mWidth = 0x2F;
    Element->mHeight = 0x0B;
    Element->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_ShowOverview;

    if (mVersion->mPlatform == ePlatform::PC) {
        Element->mY = 0xBD;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( 0xD0, 0, 0xBD );
    }
    else {
        Element->mY = 214;
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( 0xD0, 0, 214 );
    }
    ++Element;

    GUI_ClearElement( Element );
    mGUI_Sidebar_MapButton_Prepared = -1;

}

void cFodder::Sprite_Movement_Calculate( sSprite* pSprite ) {
    mSprite_Bullet_Destroy = 0;
    if (!pSprite->field_36)
        return;

    //loc_2A10D
    pSprite->field_10 &= 0x1FE;
    int16 Data4 = pSprite->field_10;

    int16 Data8 = mMap_Direction_Calculations[Data4/2];

    Data4 += 0x80;
    Data4 &= 0x1FE;
    int16 DataC = mMap_Direction_Calculations[Data4/2];
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

int16 cFodder::Sprite_Direction_Between_Points( sSprite* pSprite, int16& pData0, int16& pData4 ) {
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

void cFodder::sub_2A3D4( sSprite* pSprite ) {
    
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
    DataC = (int16) mSprite_Direction_Frame_Unk[DataC];
    DataC <<= 1;

    mDirectionMod = DataC;
}

void cFodder::Squad_Walk_Steps_Decrease() {

    for (int16 Data0 = 2; Data0 >= 0; --Data0) {

        if (mSquad_Walk_Target_Steps[Data0])
            --mSquad_Walk_Target_Steps[Data0];
        
    }
}

int16 cFodder::sub_2A4A2( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
    pData0 >>= 4;
    pData4 >>= 4;
    pData8 >>= 4;
    pDataC >>= 4;

    int16 Data18 = 2;

    int16 Data1C = mMapWidth;
    Data1C <<= 1;

    m2A622_Unk_MapPosition.mX = pData0;
    m2A622_Unk_MapPosition.mY = pData4;

    sub_2A4FD( pData0, pData4, pData8, pDataC, Data18, Data1C );
    
    return sub_2A622( pData0 );
}

void cFodder::sub_2A4FD( int16& pData0, int16&  pData4, int16& pData8, int16& pDataC, int16& pData18, int16& pData1C ) {
    uint8* Data20 = byte_3A8DE;

    int16 Data10 = pData8;
    Data10 -= pData0;

    int16 Data14 = pDataC;
    Data14 -= pData4;
    
    int16 Data28 = pData8;

    int16 WriteFinalValue, WriteValue;

    if (Data10 < 0) {
        pData18 = -pData18;
        WriteFinalValue = pData18;
        Data10 = -Data10;
        pData18 = -1;
    }
    else {
        WriteFinalValue = pData18;
        pData18 = 1;
    }
    //loc_2A56A

    if (Data14 < 0) {
        pData1C = -pData1C;
        WriteValue = pData1C;
        Data14 = -Data14;
        pData1C = -1;
    }
    else {
        WriteValue = pData1C;
        pData1C = 1;
    }
    //loc_2A59D
    pData8 = 0;
    if (Data14 == 0)
        pData8 = 1;
    else
        pData8 = 0;

    pData8 = -pData8;

loc_2A5BA:;
    if (Data28 == pData0 && pDataC == pData4){
        Data20[0] = -1;
        Data20[1] = -1;
        Data20[2] = -1;
        Data20[3] = -1;
        return;
    }

    if (pData8 >= 0) {
        pData4 += pData1C;
        pData8 -= Data10;
        writeLEWord( Data20, WriteValue );
        Data20 += 2;
        goto loc_2A5BA;
    }
    //loc_2A601
    pData0 += pData18;
    pData8 += Data14;
    writeLEWord( Data20, WriteFinalValue );
    Data20 += 2;
    goto loc_2A5BA;
}

int16 cFodder::sub_2A622( int16& pData0 ) {
    
    int32 Data4 = m2A622_Unk_MapPosition.mY;

    uint8* MapTilePtr = mMap->data() + 0x60;

    Data4 *= mMapWidth;
    Data4 += m2A622_Unk_MapPosition.mX;
    
    MapTilePtr += (Data4 << 1);
    //seg007:0B48

    uint8* Data28 = byte_3A8DE;
    word_3B2CB = -1;

    for (;;) {
        if (word_3B2CB) {
            word_3B2CB = 0;
            goto loc_2A6D7;
        }
        //loc_2A6A1
        pData0 = readLEWord( MapTilePtr );
        pData0 = mTile_Hit[pData0];

        // Tile has hit?
        if (pData0 >= 0) {
            pData0 &= 0x0F;

            // Check if tile is passable
            if (mTiles_NotWalkable[pData0]) {
                pData0 = -1;
                return -1;
            }
        }

    loc_2A6D7:;
        if (readLEDWord( Data28 ) == -1) {
            pData0 = 0;
            return 0;
        }
        pData0 = readLEWord( Data28 );
        Data28 += 2;
        if (pData0 == 0)
            goto loc_2A728;

        if (readLEDWord( Data28 ) == -1)
            goto loc_2A728;

        if (readLEWord( Data28 ) == 0)
            goto loc_2A728;

        MapTilePtr += pData0;
        pData0 = readLEWord( Data28 );
        Data28 += 2;

    loc_2A728:;
        MapTilePtr += pData0;

    }
}

/**
 * Calculate the distance between two points on the map
 *
 * @return Distance between points
 */
int16 cFodder::Map_Get_Distance_BetweenPoints( int16& pPosX, int16& pPosY, int16& pPosX2, int16& pDistance, int16& pPosY2 ) {
    const int8* Data24 = mMap_Distance_Calculations;

    pPosX2 -= pPosX;
    if (pPosX2 < 0)
        pPosX2 = -pPosX2;

    if (pPosX2 >= pDistance)
        goto loc_2A7DB;

    pPosY2 -= pPosY;
    if (pPosY2 < 0)
        pPosY2 = -pPosY2;

    if (pPosY2 >= pDistance)
        goto loc_2A7DB;

    pDistance = 0;
    for (;;) {
        if (pPosX2 <= 0x1F)
            if (pPosY2 <= 0x1F)
                break;

        pPosX2 >>= 1;
        pPosY2 >>= 1;
        ++pDistance;
    }
    //loc_2A7AD
    pPosY2 <<= 5;
    pPosY2 |= pPosX2;
    pPosX = 0;

    pPosX = Data24[pPosY2];
    pPosX <<= pDistance;

    return pPosX;

loc_2A7DB:;
    pPosX = 0x3E8;
    return 0x3E8;
}

/**
 *
 * @param pY
 * @param pX        Returns Terrain Type
 *
 * @return False if can walk
 */
int16 cFodder::Map_Terrain_Get_Type_And_Walkable( int16& pY, int16& pX ) {
    int16 Data10 = pY;
    int16 Data14 = pX;

    return Map_Terrain_Get( pY, pX, Data10, Data14 );
}

/**
 *
 * @param pSprite
 * @param pY         
 * @param pX        Returns Terrain Type
 *
 * @return False if can walk
 */
int16 cFodder::Map_Terrain_Get_Type_And_Walkable( sSprite* pSprite, int16& pY, int16& pX ) {

    pY += pSprite->field_4;
    if (pY >= 0) {
        
        int16 Data14 = pY;
        pX += pSprite->field_0;
        if (pX >= 0) {
            int16 Data10 = pX;

            return Map_Terrain_Get( pY, pX, Data10, Data14 );
        }
    }

    pY = -1;
    pX = 0;
        
    return 0;
}

int16 cFodder::Map_Terrain_Get( int16& pY, int16& pX, int16& pData10, int16& pData14 ) {

	if ((pY >>4) > mMapHeight || (pX >> 4) > mMapWidth)
		return 0;

    int32 MapPtr = (pY >> 4) * mMapWidth;
    MapPtr += (pX >> 4);
    MapPtr <<= 1;

    uint16 TileID = readLEWord( mMap->data() + (0x60 + MapPtr)) & 0x1FF;

    // There is two tables, the HIT and the BHIT
    // HIT contains the type of terrain used by a tile, a value < 0 indicates
    // the tile contains two terrain types.
    // This is determined by looking up the terrain type and row
    //  then checking if a bit is set for the column. 
    //  The bit being set, means we use the upper 4 bits as the terrain type
    //  Not being set, means we use the lower 4 bits

    // eTerrainType
    int16 TerrainType = mTile_Hit[TileID];

    // Tile contains two Terrain Types?
    if (TerrainType < 0) {
        //loc_2A8D9
        // X
        pData10 >>= 1;
        pData10 &= 0x07;

        int16 TilePixel = 7;
        TilePixel -= pData10;

        // Y
        pData14 >>= 1;
        pData14 &= 0x07;

        int8 RowTerrainType = mTile_BHit[TileID][pData14];

        // If the bit for this X position is set, we use the UpperBits for the terrain type
        if (RowTerrainType & (1 << TilePixel))
            TerrainType >>= 4;
    }

    TerrainType &= 0x0F;
    pY = mTiles_NotWalkable[TerrainType];
    pX = TerrainType;

    return mTiles_NotWalkable[TerrainType];
}

void cFodder::Squad_Walk_Target_Set( int16 pTargetX, int16 pTargetY, int16 pSquadNumber, int16 pData10 ) {
    int16 Data1C;
    sSprite** Saved_Data24 = 0;

    if (pSquadNumber < 0 || pSquadNumber >= 3)
        return;

    int16 Data0 = pSquadNumber;

    pData10 = mSquad_Walk_Target_Indexes[Data0];

    sSprite** Data24 = mSquads[Data0];

    // Currently walking to a target?
    if (mSquad_Walk_Target_Steps[Data0]) {

        // Add this target to the queue
        mSquad_WalkTargets[Data0][pData10].mX = pTargetX;
        mSquad_WalkTargets[Data0][pData10].mY = pTargetY;

        mSquad_WalkTargets[Data0][pData10 + 1].asInt = -1;

        if (pData10 < (sizeof( mSquad_WalkTargets[Data0] ) / sizeof( mSquad_WalkTargets[Data0][0] )))   // 0x74
            pData10++;

        mSquad_Walk_Target_Indexes[Data0] = pData10;
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
        mSquad_WalkTargets[Data0][pData10].mX = mSquad_WalkTargetX;
        mSquad_WalkTargets[Data0][pData10].mY = mSquad_WalkTargetY;
        pData10 += 1;
    }
    //loc_2AAAE
    mSquad_WalkTargets[Data0][pData10].mX = pTargetX;
    mSquad_WalkTargets[Data0][pData10].mY = pTargetY;

    mSquad_WalkTargetX = pTargetX;
    mSquad_WalkTargetY = pTargetY;

    mSquad_WalkTargets[Data0][pData10 + 1].asInt = -1;

    Data24 = Saved_Data24;
    if (Data1C < 0)
        return;

    mSquad_Walk_Target_Indexes[Data0] = pData10;
    mSquad_Walk_Target_Indexes[Data0]++;

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

        mSquad_WalkTargets[Data0][pData10].mX = Data28->field_0;
        mSquad_WalkTargets[Data0][pData10].mY = Data28->field_4;

        Data28->field_26 = pTargetX;
        Data28->field_28 = pTargetY;

        pTargetX = Data28->field_0;
        pTargetY = Data28->field_4;
    }

loc_2ABF8:;
    mSquad_Walk_Target_Steps[Data0] = 8;
}

int16 cFodder::Squad_Member_Sprite_Hit_In_Region( sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 ) {
    int16 Data4;

    if (mMission_Finished) {
        word_3AA45 = 0;
        return 0;
    }

    mSprites_Found_Count = 0;

    sMission_Troop* Data28 = mMission_Troops;

    for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Data28 ) {

        sSprite* Data2C = Data28->mSprite;

        if (Data2C == INVALID_SPRITE_PTR || Data2C == 0 )
            continue;

        int16 Data0 = Data2C->field_0;
        Data0 += 9;

        if (pData8 > Data0)
            continue;

        if (pDataC < Data2C->field_0)
            continue;

        Data0 = Data2C->field_4;
        Data0 -= 6;
        if (pData10 > Data0)
            continue;

        Data0 = Data2C->field_4;
        Data0 -= 0x0D;
        if (pData14 < Data0)
            continue;

        if (Data2C->field_20 >= 0x0B)
            continue;

        if (!word_3AA45)
            goto loc_2AD3D;

        if (Data2C->field_6E)
            continue;

        if (pSprite->field_3A <= 4)
            goto loc_2AD25;

        if (Data2C->field_52 >= 5)
            goto loc_2AD2D;

        if (Data2C->field_4F)
            goto loc_2AD2D;


        Data0 = Data2C->field_32;
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

        Sprite_Direction_Between_Points( Data2C, Data0, Data4 );
        
        Data2C->field_10 += 0x100;
        Data2C->field_10 &= 0x1FE;

    loc_2ADC3:; // Hit
        if (Data2C->field_18)
            goto loc_2ADFF;

        Data2C->field_10 = pSprite->field_10;
        Data2C->field_12 = pSprite->field_10;

    loc_2ADFF:;
        Data2C->field_38 = eSprite_Anim_Hit;
        Data2C->field_64 = -1;
        mSprites_Found_Count += 1;

        if (word_3AA45)
            break;
    }

    word_3AA45 = 0;
    return mSprites_Found_Count;
}

const sSpriteSheet* cFodder::Sprite_Get_Sheet(int16 pSpriteType, int16 pFrame) {
    const sSpriteSheet* Sheet = &mSpriteSheetPtr[pSpriteType][pFrame];

    return Sheet;
}

void cFodder::sub_2AEB6( int16 pColumns, int16 pRows, int16* pData8, int16* pDataC ) {
    int32 Columns   = pColumns  * *pData8;
    int32 Rows      = pRows     * *pData8;

    // Return
    *pDataC = (int16) (Rows / 0x64);
    *pData8 = (int16) (Columns / 0x64);
}


void cFodder::sub_2B12E( sSprite* pSprite ) {
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

    mDirectionMod = (int16) mSprite_Direction_Frame_Unk[Data4];
}

void cFodder::Sprite_Vehicle_Direction_Update( sSprite* pSprite, int16& pData1C ) {
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

int16 cFodder::Map_Terrain_Get_Moveable_Wrapper( const int8* pMovementData, int16& pX, int16& pY, int16& pData10, int16& pData14 ) {
    
    pData10 = pX;
    pData14 = pY;

    return Map_Terrain_Get_Moveable( pMovementData, pX, pY, pData10, pData14 );
}

int16 cFodder::Map_Terrain_Get_Moveable( const int8* pMovementData, int16& pX, int16& pY, int16& pData10, int16& pData14 ) {
    uint32 DataC = pY;
    uint32 Data8 = pX;

    if (pY < 0)
        DataC = 0;
    if (pX < 0)
        Data8 = 0;

    DataC >>= 4;

    DataC *= mMapWidth;
    Data8 >>= 4;

    DataC += Data8;
    DataC <<= 1;

    int16 Data0 = readLEWord( mMap->data() + 0x60 + DataC );
    Data0 &= 0xFF;

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

void cFodder::Map_Get_Distance_BetweenPoints_Within_640( int16& pX, int16& pY, int16& pX2, int16& pY2 ) {
    int16 Data10 = 0;

    pX2 -= pX;
    if (pX2 < 0)
        pX2 = -pX2;

    if (pX2 >= 0x280)
        goto loc_2B403;

    pY2 -= pY;
    if (pY2 < 0)
        pY2 = -pY2;

    if (pY2 >= 0x280)
        goto loc_2B403;

    for (;;) {
        if (pX2 <= 0x1F)
            if (pY2 <= 0x1F)
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
    pX = 0x280;
}

void cFodder::MapTile_Update_Position() {
    
    MapTile_Update_Row();
    MapTile_Update_Column();

    int16 TileColumns = mMapTile_Column - mMapTile_Column_CurrentScreen;
    int16 TileRows = mMapTile_Row - mMapTile_Row_CurrentScreen;
    
    mMapTile_Column_CurrentScreen = mMapTile_Column;
    mMapTile_Row_CurrentScreen = mMapTile_Row;

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

    if(TileColumns || TileRows)
        g_Graphics.MapTiles_Draw();
}

void cFodder::MapTile_Move_Right( int16 pPanTiles ) {

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

void cFodder::MapTile_Move_Down( int16 pPanTiles) {

    for (; pPanTiles > 0; --pPanTiles) {

        ++mMapTile_RowOffset;
        mMapTile_RowOffset &= 0x0F;
        if (!mMapTile_RowOffset) {
            mMapTile_Ptr += (mMapWidth << 1);
            ++mMapTile_MovedVertical;
        }
    }
}

void cFodder::MapTile_Move_Up(int16 pPanTiles) {

    for (; pPanTiles > 0; --pPanTiles) {

        --mMapTile_RowOffset;
        mMapTile_RowOffset &= 0x0F;
        if (mMapTile_RowOffset == 0x0F) {
            mMapTile_Ptr -= (mMapWidth << 1);
            --mMapTile_MovedVertical;
        }
    }
}

void cFodder::MapTile_Update_Row() {
    int16 Data0 = word_39FB4;

    if (Data0 < 0)
        Data0 = -1;
    else
        Data0 = 1;

    if (Data0 != word_39FA2) {
        word_39FA2 = Data0;
        Data0 = dword_39F90 >> 16;
        int16 Data4 = word_39FAE;

        Data0 >>= 4;
        Data4 >>= 4;
        if (Data4 != Data0)
            return;
    }

    word_39FBC = 0;
    mMapTile_Row = dword_39F88 >> 16;
    word_39FAE = dword_39F90 >> 16;
}

void cFodder::MapTile_Update_Column() {
    int16 Data0 = word_39FB2;

    if (Data0 < 0)
        Data0 = -1;
    else
        Data0 = 1;

    if (Data0 != word_39FA0) {
        word_39FA0 = Data0;
        Data0 = dword_39F8C >> 16;
        int16 Data4 = word_39FAC;
        Data0 >>= 4;
        Data4 >>= 4;
        if (Data0 != Data4)
            return;
    }

    word_39FBA = 0;
    mMapTile_Column = dword_39F84 >> 16;
    word_39FAC = dword_39F8C >> 16;
}

void cFodder::MapTile_Set(const size_t pTileX, const size_t pTileY, const size_t pTileID) {
    
    if (pTileX > mMapWidth || pTileY > mMapHeight)
        return;

    size_t Tile = (((pTileY * mMapWidth) + pTileX));

    uint8* CurrentMapPtr = mMap->data() + mMapTile_Ptr + (Tile * 2);

    writeLEWord(CurrentMapPtr, (uint16) pTileID);
}

void cFodder::Sprite_Add(size_t pSpriteID, int16 pTileX, int16 pTileY) {

    int16 Data0 = 0;

    sSprite* Data2C, *Data30;

    if (Sprite_Get_Free(Data0, Data2C, Data30))
        return;

	Data0 = 0;
    Data2C->field_18 = (int16) pSpriteID;
    Data2C->field_0 = pTileX;
    Data2C->field_4 = pTileY;
	Data2C->field_26 = pTileX;
	Data2C->field_28 = pTileY;

    switch (pSpriteID) {
        case eSprite_BoilingPot:                        // 1 Null
            if (Sprite_Get_Free(Data0, Data2C, Data30))
                return;

            Data2C->field_18 = eSprite_Null;
            Data2C->field_0 = pTileX;
            Data2C->field_4 = pTileY;
			Data2C->field_26 = pTileX;
			Data2C->field_28 = pTileY;

            break;

        case eSprite_Helicopter_Grenade_Enemy:          // 3 Nulls
        case eSprite_Helicopter_Grenade2_Enemy:
        case eSprite_Helicopter_Missile_Enemy:
        case eSprite_Helicopter_Homing_Enemy:
        case eSprite_Helicopter_Homing_Enemy2:
            if (Sprite_Get_Free(Data0, Data2C, Data30))
                return;

            Data2C->field_18 = eSprite_Null;
            Data2C->field_0 = pTileX;
            Data2C->field_4 = pTileY;
			Data2C->field_26 = pTileX;
			Data2C->field_28 = pTileY;

            // Fall Through
        case eSprite_Helicopter_Grenade2_Human:         // 2 Nulls
        case eSprite_Helicopter_Grenade_Human:
        case eSprite_Helicopter_Missile_Human:
        case eSprite_Helicopter_Homing_Human:
        case eSprite_Helicopter_Grenade2_Human_Called:
        case eSprite_Helicopter_Grenade_Human_Called:
        case eSprite_Helicopter_Missile_Human_Called:
        case eSprite_Helicopter_Homing_Human_Called:
            Data0 = 2;
            if (Sprite_Get_Free(Data0, Data2C, Data30))
                return;

            Data2C->field_18 = eSprite_Null;
            Data2C->field_0 = pTileX;
            Data2C->field_4 = pTileY;
			Data2C->field_26 = pTileX;
			Data2C->field_28 = pTileY;

            Data30->field_18 = eSprite_Null;
            Data30->field_0 = pTileX;
            Data30->field_4 = pTileY;
			Data30->field_26 = pTileX;
			Data30->field_28 = pTileY;

            break;

        case eSprite_Tank_Enemy:                        // 2 Nulls
            if (Sprite_Get_Free(Data0, Data2C, Data30))
                return;

            Data2C->field_18 = eSprite_Null;
            Data2C->field_0 = pTileX;
            Data2C->field_4 = pTileY;
			Data2C->field_26 = pTileX;
			Data2C->field_28 = pTileY;
        case eSprite_Tank_Human:
			Data0 = 0;
            if (Sprite_Get_Free(Data0, Data2C, Data30))
                return;

            Data2C->field_18 = eSprite_Null;
            Data2C->field_0 = pTileX;
            Data2C->field_4 = pTileY;
			Data2C->field_26 = pTileX;
			Data2C->field_28 = pTileY;
            break;

        case eSprite_VehicleNoGun_Human:
        case eSprite_VehicleGun_Human:
        case eSprite_VehicleNoGun_Enemy:
        case eSprite_VehicleGun_Enemy:
        case eSprite_Vehicle_Unk_Enemy:
            if (Sprite_Get_Free(Data0, Data2C, Data30))
                return;

            Data2C->field_18 = eSprite_Null;
            Data2C->field_0 = pTileX;
            Data2C->field_4 = pTileY;
			Data2C->field_26 = pTileX;
			Data2C->field_28 = pTileY;
            break;
        }

    Sprite_Handle_Loop();
}

void cFodder::Squad_Troops_Count() {
    sSprite** Data34 = mSprite_TroopsAlive;

    mSquads_TroopCount[0] = 0;
    mSquads_TroopCount[1] = 0;
    mSquads_TroopCount[2] = 0;
    mSquads_TroopCount[3] = 0;
    byte_3A05E = 0;

    int16 TotalTroops = 0;

    mSquad_0_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_1_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_2_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_3_Sprites[0] = INVALID_SPRITE_PTR;
    mSquad_4_Sprites[0] = INVALID_SPRITE_PTR;

    // Loop through all mission troops
    for (int8 Data1C = 0; Data1C < 8; ++Data1C) {
        sMission_Troop* Data2C = &mMission_Troops[Data1C];

        if (Data2C->mSprite == INVALID_SPRITE_PTR || Data2C->mSprite == 0 )
            continue;

        sSprite* Sprite = Data2C->mSprite;

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

        uint8 Data10 = mSquads_TroopCount[ Sprite->field_32 ] & 0xFF;
        mSquads_TroopCount[ Sprite->field_32 ] += 1;
        ++TotalTroops;

        sSprite** Data30 = mSquads[ Sprite->field_32 ];
        //seg009:0151
        Data30[Data10] = Sprite;
        Data30[Data10 + 1] = INVALID_SPRITE_PTR;

        *Data34 = Sprite;
        ++Data34;
    }
    //seg009:019E

    if (!TotalTroops)
        mMission_TryAgain = -1;

    byte_3A8DE[1] = byte_3A05E;

    for (int16 Data0 = 2; Data0 >= 0; --Data0 ) {

        int8 al = mSquads_TroopCount[Data0];
        if (al)
            continue;

        mSquad_Join_TargetSquad[Data0] = -1;
    }

    *Data34 = INVALID_SPRITE_PTR;
}

int16 cFodder::Mission_Troop_GetDeviatePotential( sMission_Troop* pData24 ) {

    int16 Data0 = pData24->mRank;
    Data0 += 8;

    if (Data0 > 0x0F)
        Data0 = 0x0F;

    return mSprite_Bullet_UnitData[Data0].mDeviatePotential;
}

void cFodder::Squad_Member_Rotate_Can_Fire() {
    if (word_39F04) 
        word_39F04 = 0;
    else {
        mSquad_Member_Fire_CoolDown -= 2;
        if (mSquad_Member_Fire_CoolDown >= 0)
            return;
    }

    //loc_2D2EB
    sSprite* Data20 = mSquad_Leader;
    if (Data20 == INVALID_SPRITE_PTR || Data20 == 0 )
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
    const int16* Rotate_Fire_Order = mSoldier_Squad_Fire_RotationOrder[TroopsInSquad];

    int16 Data0 = mTroop_Rotate_Next;

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

int16 cFodder::Sprite_Find_In_Region( sSprite* pSprite, sSprite*& pData24, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 ) {

    if (mMission_Finished) {
        word_3AA45 = 0;
        return 0;
    }

    mSprites_Found_Count = 0;

    pData24 = mSprites;

    for (int16 Data1C = 0x2B; Data1C >= 0; --Data1C, ++pData24) {
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

        if (pData24->field_38 != 0x0A)
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
    sSprite* Data20 = mSprites;
    
    for (int16 Data0 = 0x2B; Data0 >= 0; --Data0, ++Data20 ) {

        if (Data20->field_0 == -32768)
            continue;

        if (Data20->field_18 == eSprite_Player )
            Sprite_Troop_Dies( Data20 );
    }
}

void cFodder::Sprite_Handle_Player_Destroy_Unk() {
    sSprite* Data20 = mSprites;

    for (int16 Data0 = 0x2B; Data0 >= 0; --Data0, ++Data20) {

        if (Data20->field_0 == -32768)
            continue;

        if (Data20->field_18 != eSprite_Player )
            continue;

        if (!Data20->field_38)
            continue;

        if (Data20->field_38 >= eSprite_Anim_Slide1)
            continue;

        Sprite_Troop_Dies( Data20 );
    }
}

void cFodder::Squad_Prepare_GrenadesAndRockets() {

    mSquad_Grenades[0] = mMission_Troops_Available << 1;
    mSquad_Rockets[0] = mMission_Troops_Available;

    if (mMapNumber < 5)
        mSquad_Grenades[0] = 0;

    if (mMapNumber < 10)
        mSquad_Rockets[0] = 0;
}

void cFodder::Sprite_Aggression_Set() {
    int16 Data0 = mMapNumber;
    Data0 <<= 1;


    mSprite_Enemy_AggressionMin = mCampaign.getMapAggression( mMapNumber ).mMin;
    mSprite_Enemy_AggressionMax = mCampaign.getMapAggression( mMapNumber ).mMax;
    mSprite_Enemy_AggressionAverage = mCampaign.getMapAggression( mMapNumber ).getAverage();

    mSprite_Enemy_AggressionNext = mCampaign.getMapAggression( mMapNumber ).getAverage();
    mSprite_Enemy_AggressionIncrement = 1;

    sSprite* Sprite = mSprites;
    for (int16 Data1C = 29; Data1C >= 0; --Data1C, ++Sprite ) {

        if (Sprite->field_0 == -32768)
            continue;

        if (Sprite->field_18 == eSprite_Enemy_Rocket || Sprite->field_18 == eSprite_Enemy)
            Sprite_Handle_Enemy_Aggression_Set( Sprite );
    }
}

/**
 * Set the aggression of a sprite, then calculate the next aggression rate
 *
 * This moves by 'Increment' in an up or down 
 * direction inside the min/max range for this map
 */
void cFodder::Sprite_Handle_Enemy_Aggression_Set( sSprite* pSprite ) {
    
    int16 Data8 = mSprite_Enemy_AggressionNext;
    pSprite->field_62 = Data8;

    int16 Data4 = mSprite_Enemy_AggressionIncrement;
    Data8 += Data4;

    if (Data4 >= 0) {

        if (Data8 >= mSprite_Enemy_AggressionMax) {
            Data8 = mSprite_Enemy_AggressionMax;
            Data4 = -Data4;
        }
    }
    else {
        if (Data8 < 0 || Data8 <= mSprite_Enemy_AggressionMin) {
            Data8 = mSprite_Enemy_AggressionMin;
            Data4 = -Data4;
        }

    }
    //loc_2D90D
    mSprite_Enemy_AggressionIncrement = Data4;
    mSprite_Enemy_AggressionNext = Data8;
}

int16 cFodder::Sprite_Next_WalkTarget_Set( sSprite* pSprite ) {

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

    return Squad_Join( pSprite );
}

int16 cFodder::Squad_Join( sSprite* pSprite ) {
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

    sub_305D5( pSprite );
    mGUI_Sidebar_Setup = 0;

    return -1;
}

int16 cFodder::Squad_Join_Check( sSprite* pSprite ) {

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
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 <= MaxDistance)
        return Squad_Join(pSprite);

    return 0;
}

void cFodder::Squad_Walk_Target_Update( int16 pData0 ) {
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

void cFodder::Sprite_Handle_Explosion_MapTiles( sSprite* pSprite ) {
    word_3B4E9 = 0;
    if (pSprite->field_18 == eSprite_Explosion2)
        word_3B4E9 = -1;

    sMapPosition* Data28 = mMap_Destroy_TilesPtr2;

    int16 Data0 = pSprite->field_0 + 4;
    int16 Data4 = pSprite->field_4 - 8;

    const int16* Data24 = mSprite_Explosion_Positions;

    for (;;) {
        if (*Data24 == -32768)
            break;

        Data0 += *Data24++;
        Data4 += *Data24++;

        Data28->mX = Data0;
        Data28->mY = Data4;
        ++Data28;

        if (word_3B4E9)
            (Data28 - 1)->mY = -Data4;

        if (Data28 >= &mMap_Destroy_Tiles[128])
            Data28 = mMap_Destroy_Tiles;
    }

    Data28->mX = -1;
    Data28->mY = -1;

    mMap_Destroy_TilesPtr2 = Data28;
}

void cFodder::Map_Destroy_Tiles( ) {
    int16* Data20 = 0; 
    uint8* MapPtr = 0;
    const int16* IndestructibleTypes = 0;
    int32 Data0, Data4, MapTile, TileType;

    if (mMap_Destroy_Tiles_Countdown) {
        --mMap_Destroy_Tiles_Countdown;
        goto loc_2DFC7;
    }

loc_2DE3C:;
    if (!mMap_Destroy_TilesPtr)
        return;

    Data20 = (int16*) mMap_Destroy_TilesPtr;

    Data0 = *Data20;
    Data4 = *(Data20 + 1);

    if (Data0 < 0)
        return;

    mMap_Destroy_Tile_X = Data0;
    word_3B4EB = 0;
    mMap_Destroy_Tile_Y = Data4;
    if (mMap_Destroy_Tile_Y < 0) {
        word_3B4EB = -1;
        Data4 = -Data4;
        mMap_Destroy_Tile_Y = Data4;
    }
    //loc_2DE89
    Data4 >>= 4;
    Data4 *= mMapWidth; // * Width

    Data0 >>= 4;
    Data4 += Data0;
    Data4 <<= 1;
    
    // In some cases, tiles outside the map can be 'destroyed'. This prevents memory corruption
    if (0x60 + Data4 >= mMap->size())
        goto loc_2DF55;

    MapPtr = mMap->data() + 0x60 + Data4;

    MapTile = readLEWord( MapPtr );
    MapTile &= 0x1FF;
    TileType = MapTile;

    Data4 = mTile_Destroy_Swap[MapTile];
    if (Data4 < 0)
        goto loc_2DF55;

    if (word_3B4EB)
        goto loc_2DF7B;

    IndestructibleTypes = mTiles_Indestructible[mMap_TileSet];

    int16 ax;
    do {
        if (*IndestructibleTypes < 0)
            goto loc_2DF7B;

        ax = *IndestructibleTypes;
        IndestructibleTypes++;

    } while (TileType != ax);

loc_2DF55:;
    Map_Destroy_Tiles_Next();
    goto loc_2DE3C;

//seg010:064C
    //UNUNSED BLOCK

loc_2DF7B:;

    Data0 = Sprite_Create_Building_Explosion_Wrapper( mMap_Destroy_Tile_X, mMap_Destroy_Tile_Y );

    if (Data0)
        return;

    mMap_Destroy_Tiles_Countdown = 1;
    writeLEWord( MapPtr, Data4 );
    mMap_Destroy_Tile_LastTile = Data4;
    Map_Destroy_Tiles_Next();

loc_2DFC7:;
    ax = mMap_Destroy_Tile_X >> 4;
    ax -= mMapTile_MovedHorizontal;
    ax <<= 4;
    ax -= mMapTile_ColumnOffset;
    ax += 0x40;

    mVideo_Draw_PosX = ax;

    ax = mMap_Destroy_Tile_Y >> 4;
    ax -= mMapTile_MovedVertical;
    ax <<= 4;
    ax -= mMapTile_RowOffset;
    ax += 0x10;

    mVideo_Draw_PosY = ax;

    mVideo_Draw_Columns = 0x10;
    mVideo_Draw_Rows = 0x10;

    g_Graphics.MapTiles_Draw();
}

void cFodder::Map_Destroy_Tiles_Next() {
    
    sMapPosition* Data20 = mMap_Destroy_TilesPtr;
    ++Data20;
    
    if( Data20 >= &mMap_Destroy_Tiles[128] ) 
        Data20 = mMap_Destroy_Tiles;
    
    mMap_Destroy_TilesPtr = Data20;
}

void cFodder::Game_Save_Wrapper2() {
    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);

    Game_Save_Wrapper();
}

void cFodder::GUI_Element_Reset() {
    mImage->clearBuffer();

    mGUI_NextFreeElement = mGUI_Elements;

    for (uint16 x = 0; x < 42; ++x) {
        mGUI_Elements[x].field_0 = 0;
        mGUI_Elements[x].mX = 0;
        mGUI_Elements[x].mWidth = 0;
        mGUI_Elements[x].mY = 0;
        mGUI_Elements[x].mHeight = 0;
        mGUI_Elements[x].mMouseInsideFuncPtr = 0;
    }
}

void cFodder::Recruit_Render_Text( const char* pText, const size_t pPosY ) {

    String_CalculateWidth( 320, mFont_Recruit_Width, pText );
    String_Print( mFont_Recruit_Width, 0x0D, mGUI_Temp_X, pPosY, pText );
}

void cFodder::GUI_Button_Draw_Small(const std::string pText, const size_t pY, const size_t pColorShadow, const size_t pColorPrimary) {
    String_Print_Small(pText, pY);

    GUI_Box_Draw(pColorShadow, pColorPrimary);
}

void cFodder::GUI_Button_Draw( std::string pText, const size_t pY, const size_t pColorShadow, const size_t pColorPrimary ) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    Recruit_Render_Text( pText.c_str(), pY );

    GUI_Box_Draw( pColorShadow, pColorPrimary );
}

void cFodder::GUI_Box_Draw( const size_t pColorShadow, const size_t pColorPrimary ) {

    int16 X = mGUI_Temp_X;
    int16 Y = mGUI_Temp_Y;
    int16 Width = mGUI_Temp_Width;
    int16 Height = mGUI_Draw_LastHeight;

    Briefing_DrawBox( X - 2, Y - 2, Width + 4, Height + 5, (uint8) pColorPrimary );
    Briefing_DrawBox( X - 3, Y - 3, Width + 4, Height + 5, (uint8) pColorShadow );
}

void cFodder::GUI_Select_File_Loop( bool pShowCursor ) {
    int8 byte_44B49 = 0;
    if (mGUI_SaveLoadAction != 3) {

        mImage->palette_FadeTowardNew();
        Mouse_Setup();
    }
    mGUI_SaveLoadAction = 0;

    mImageFaded = -1;
    mGraphics->PaletteSet();

    mImage->Save();
    bool mShow = false;

    do {
        if (mImageFaded == -1)
            mImageFaded = mImage->palette_FadeTowardNew();

        ++byte_44B49;
        byte_44B49 &= 0x0F;
        if (!byte_44B49) {
            if (pShowCursor)
                mShow = !mShow;
            mMouse_Button_Left_Toggle = 0;
        }

        if (mShow) {
            if (mVersion->mPlatform == ePlatform::Amiga)
                GUI_Draw_Frame_8( 0x18, 0x00, mGUI_Temp_X + mGUI_Temp_Width, 0x50 );
            else
                GUI_Draw_Frame_8( 0x0F, 0x00, mGUI_Temp_X + mGUI_Temp_Width, 0x50 );
        }

        Mouse_Inputs_Get();
        Mouse_DrawCursor();

        if (mMission_Aborted)
            GUI_Button_Load_Exit();

        ++byte_44AC0;
        if (Mouse_Button_Left_Toggled() >= 0)
            GUI_SaveLoad_MouseHandle( mGUI_Elements );

        if (dword_3B30D)
            (this->*dword_3B30D)(0x50);

        g_Window.RenderAt( mImage, cPosition() );
        g_Window.FrameEnd();
        mImage->Restore();

    } while (mGUI_SaveLoadAction <= 0);

    mMission_Aborted = false;

    if (mGUI_SaveLoadAction == 3)
        return;

    Image_FadeOut();
}

void cFodder::Game_Save_Wrapper() {

    Game_Save();
}

void cFodder::GUI_Button_Setup( void(cFodder::*pFunction)(void) ) {
    sGUI_Element* Element = mGUI_NextFreeElement;

    Element->field_0 = &cFodder::GUI_Button_NoAction;

    Element->mX         = mGUI_Temp_X - 3;
    Element->mWidth     = mGUI_Temp_Width + 4;
    Element->mY         = mGUI_Temp_Y;
    Element->mHeight    = mGUI_Draw_LastHeight + 5;
    Element->mMouseInsideFuncPtr = pFunction;

    ++Element;
    Element->field_0 = 0;
    mGUI_NextFreeElement = Element;
}

void cFodder::GUI_Button_Setup_Small(void(cFodder::*pFunction)(void)) {
    sGUI_Element* Element = mGUI_NextFreeElement;

    Element->field_0 = &cFodder::GUI_Button_NoAction;

    Element->mX = mGUI_Temp_X - 3;
    Element->mWidth = mGUI_Temp_Width + 4;
    Element->mY = mGUI_Temp_Y - 5;
    Element->mHeight = mGUI_Draw_LastHeight + 5;
    Element->mMouseInsideFuncPtr = pFunction;

    ++Element;
    Element->field_0 = 0;
    mGUI_NextFreeElement = Element;
}

void cFodder::Game_Save() {
    memset( mInputString, 0, 0x13 );
    dword_3B30D = 0;
    mInputString[0] = -1;

    mInputString_Position = 0;

    GUI_Element_Reset();

    Recruit_Render_Text( "TYPE A FILENAME IN", 0x32 );

    GUI_Button_Draw( "EXIT", 0xA0 );
    GUI_Button_Setup( &cFodder::GUI_Button_Load_Exit );

    dword_3B30D = &cFodder::String_Input_Print;

    GUI_Select_File_Loop(true);
    dword_3B30D = 0;
    if (mGUI_SaveLoadAction != 2) {
        mGUI_SaveLoadAction = 1;
        return;
    }
        
    mInputString[mInputString_Position + 0] = '.';
    mInputString[mInputString_Position + 1] = 'o';
    mInputString[mInputString_Position + 2] = 'f';
    mInputString[mInputString_Position + 3] = (mVersion->mGame == eGame::CF1) ? 0 : '2';
    mInputString[mInputString_Position + 4] = 0;

    std::string Filename = local_PathGenerate( mInputString, "", eDataType::eSave );
    
    std::ofstream outfile (Filename,std::ofstream::binary);

    // Copy from mMapNumber to mSaved_MissionNumber
    uint8* Start = (uint8*) &mMapNumber;
    uint8* End = (uint8*)&mSaved_MissionNumber;
    
    int16 SavegameVersion = 1;

    // Hack
    outfile << "OF";
    outfile.write( (char*)&SavegameVersion, sizeof( int16 ) );

    outfile.write ((const char*) Start, End - Start );
    outfile.close();
    mMouse_Exit_Loop = 0;
}

void cFodder::GUI_SaveLoad_MouseHandle( sGUI_Element* pData20 ) {
    mGUI_Loop_Element = pData20;

    for (; mGUI_Loop_Element->field_0; ++mGUI_Loop_Element) {

        pData20 = mGUI_Loop_Element;

        int16 Data0 = pData20->mX;
        int16 Data4 = mMouseX;
        Data4 += 0x20;

        if (Data0 > Data4)
            continue;

        Data0 += pData20->mWidth;
        if (Data0 < Data4)
            continue;

        Data0 = pData20->mY;
        if (Data0 > mMouseY)
            continue;

        Data0 += pData20->mHeight;
        if (Data0 < mMouseY)
            continue;

        (*this.*pData20->mMouseInsideFuncPtr)();
        return;
    }
}

void cFodder::GUI_Button_Load_Exit() {
    dword_3B30D = 0;
    mGUI_SaveLoadAction = 1;
}

void cFodder::String_Input_Print( int16 pPosY ) {
    GUI_Input_CheckKey();
    int16 Data4;
    char* Data24;

    Recruit_Render_Text( mInputString, pPosY );

    int16 Data0 = mKeyCodeAscii;
    if (Data0 == 0x0D && mInputString_Position)
        mGUI_SaveLoadAction = 2;

    if (Data0 == 8)
        goto loc_2E675;

    if (Data0 < 0x30)
        goto loc_2E628;

    if (Data0 <= 0x39)
        goto loc_2E636;

loc_2E628:;
    if (Data0 < 0x41 || Data0 > 0x5A)
        goto loc_2E6A4;

loc_2E636:;
    Data4 = mInputString_Position;
    if (Data4 >= 8)
        goto loc_2E6A4;

    Data24 = mInputString;
    Data24 += Data4;
    *Data24 = (char)Data0;

    *(Data24 + 1) = -1;
    ++mInputString_Position;
    goto loc_2E6A4;

loc_2E675:;
    Data4 = mInputString_Position;
    if (Data4) {
        --Data4;
        Data24 = mInputString;
        mInputString_Position = Data4;
        Data24 += Data4;
        *Data24 = -1;
    }

loc_2E6A4:;
    String_Input_Check();
}

void cFodder::String_Input_Check() {
    char* Data20 = mInputString;

    int16 Data0 = mInputString_Position;
    if (mKeyCodeAscii != 0x0D) {
        byte_44AC0 &= 0x3F;
        if (byte_44AC0 < 0x20)
            goto loc_2E6EA;
    }
    Data20[Data0] = -3;
    Data20[Data0 + 1] = -1;
    return;

loc_2E6EA:;
    Data20[Data0] = -2;
    Data20[Data0 + 1] = -1;
}

void cFodder::GUI_Input_CheckKey() {

    if (mKeyCode != mInput_LastKey) {
        mInput_LastKey = mKeyCode;

        if (!(mInput_LastKey & 0x80)) {
        
            if (mKeyCode >= SDL_SCANCODE_A && mKeyCode <= SDL_SCANCODE_Z)
                mKeyCodeAscii = 'A' + (mKeyCode - 4);
        
            if (mKeyCode >= SDL_SCANCODE_1 && mKeyCode <= SDL_SCANCODE_9)
                mKeyCodeAscii = '1' + (mKeyCode - 30);

            if (mKeyCode == SDL_SCANCODE_0 )
                mKeyCodeAscii = '0';

            if (mKeyCode == SDL_SCANCODE_RETURN)
                mKeyCodeAscii = 0x0D;

            if (mKeyCode == SDL_SCANCODE_BACKSPACE)
                mKeyCodeAscii = 8;

            return;
        }
    }

    mKeyCodeAscii = 0;
}
void cFodder::Game_Load() {

    const std::string File = GUI_Select_File( "SELECT FILE", "", mVersion->mGame == eGame::CF1 ? ".of" : ".of2", eDataType::eSave);
    if (!File.size())
        return;

    std::string Filename = local_PathGenerate( File, "", eDataType::eSave);

    std::ifstream SaveFile( Filename, std::ios::binary );
    if (SaveFile.is_open()) {

        std::vector<char> SaveGameContent( 
            (std::istreambuf_iterator<char>( SaveFile )),
            (std::istreambuf_iterator<char>()) 
        );

        // Valid save game?
        if (SaveGameContent[0] != 'O' || SaveGameContent[1] != 'F')
            return;

        // Skip OF, Save version
        std::memcpy( &mMapNumber, SaveGameContent.data() + 4, SaveGameContent.size() - 4);
        
        mMouse_Exit_Loop = 0;

        // Reset grave pointers
        mGraveRankPtr = mGraveRanks;
        mGraveRankPtr2 = mGraveRankPtr;
        mGraveRecruitIDPtr = mGraveRecruitID;

        // Update grave pointers to next free
        for (int16 x = 0; x < 361; ++x) {
            if (*mGraveRankPtr != -1) {
                ++mGraveRankPtr;
                ++mGraveRankPtr2;
            }
            if (*mGraveRecruitIDPtr != -1) {
                ++mGraveRecruitIDPtr;
            }
        }

        for (int16 x = 0; x < 8; ++x)
            mMission_Troops_SpritePtrs[x] = INVALID_SPRITE_PTR;

        for (int16 x = 0; x < 9; ++x)
            mMission_Troops[x].mSprite = INVALID_SPRITE_PTR;

        Mission_Memory_Backup();
    }
}

void cFodder::GUI_Button_Load_Up() {
    mGUI_Select_File_CurrentIndex -= (mGUI_Select_File_ShownItems - 1);
    if (mGUI_Select_File_CurrentIndex < 0)
        mGUI_Select_File_CurrentIndex = 0;

    mGUI_SaveLoadAction = 3;
}

void cFodder::GUI_Button_Load_Down() {
    mGUI_Select_File_CurrentIndex += (mGUI_Select_File_ShownItems-1);

    int16 Data0 = mGUI_Select_File_CurrentIndex;
    Data0 += mGUI_Select_File_ShownItems;

    if (Data0 >= mGUI_Select_File_Count) {
        Data0 = mGUI_Select_File_Count;
        Data0 -= mGUI_Select_File_ShownItems;
        if (Data0 < 0)
            Data0 = 0;

        mGUI_Select_File_CurrentIndex = Data0;
    }

    mGUI_SaveLoadAction = 3;
}

void cFodder::GUI_Button_Filename() {
    int16 Data0 = mMouseY;

    Data0 -= 0x3E;
    Data0 /= 0x15;

    mGUI_Select_File_SelectedFileIndex = Data0;
    mGUI_SaveLoadAction = 2;
}

int16 cFodder::GUI_Button_NoAction() {
    return 0;
}

void cFodder::GUI_Button_NoAction2() {
    return;
}

void cFodder::GUI_Sidebar_Prepare_Squads() {

    mMouseDisabled = -1;
    Sidebar_Render_To_BackBuffer();

    mGUI_NextFreeElement = mGUI_Elements;

    Sidebar_Clear_ScreenBufferPtr();
    int16 Data4 = 0;
    {
        // Squad 1
        mGUI_Squad_NextDraw_Y = 0;
        GUI_Sidebar_Prepare(0, Data4);
    }

    {
        // Squad 2
        Data4 = mGUI_Squad_NextDraw_Y;
        if (Data4)
            Data4 += 5;
        GUI_Sidebar_Prepare(1, Data4);
    }

    {
        // Squad 3
        Data4 = mGUI_Squad_NextDraw_Y;
        if (Data4)
            Data4 += 5;
        GUI_Sidebar_Prepare(2, Data4);
    }

    GUI_Sidebar_Add_Sidebar_Overlay();

    mMouseDisabled = 0;

    Sidebar_Render_To_ScreenBuffer();
    mMouse_Button_LeftRight_Toggle = 0;
}

void cFodder::GUI_Element_Mouse_Over( const sGUI_Element *pElement ) {
    for (;; ++pElement) {
        const sGUI_Element* Data20 = pElement;

        if (Data20->field_0 == 0)
            break;

        if ((*this.*Data20->field_0)() < 0)
            return;

        int16 Data0 = Data20->mX;

        int16 Data4 = mMouseX + 0x20;

        if (Data0 > Data4)
            continue;

        Data0 += Data20->mWidth;
        if (Data0 < Data4)
            continue;

        Data0 = mGUI_Mouse_Modifier_Y;
        Data0 += Data20->mY;
        if (Data0 > mMouseY)
            continue;

        Data0 += Data20->mHeight;
        if (Data0 < mMouseY)
            continue;

        (*this.*Data20->mMouseInsideFuncPtr)();
        return;
    }
}

void cFodder::GUI_Button_SelectMap0() {
    mMapNumber = 0;
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_SelectMap1() {
    mMapNumber = 1;
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_SelectQuiz() {
    Demo_Quiz();
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_SelectMap2() {
    mMapNumber = 2;
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_SelectMap3() {
    mMapNumber = 3;
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_ExitMenu() {
    mDemo_ExitMenu = 1;
}

void cFodder::GUI_Button_Quiz_2() {

    Demo_Quiz_ShowScreen( "2.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_3() {

    Demo_Quiz_ShowScreen( "3.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_4() {

    Demo_Quiz_ShowScreen( "4.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_5() {
    
    Demo_Quiz_ShowScreen( "5.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_6() {
    
    Demo_Quiz_ShowScreen( "6.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_7() {
    
    Demo_Quiz_ShowScreen( "7.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_8() {

    Demo_Quiz_ShowScreen( "8.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_9() {
    Demo_Quiz_ShowScreen( "9.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_10() {
    Demo_Quiz_ShowScreen( "10.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::GUI_Button_Quiz_11() {
    Demo_Quiz_ShowScreen( "11.lbm" );
    mDemo_ExitMenu = -1;
}

void cFodder::Menu_Loop( const std::function<void()> pButtonHandler ) {

    mCustom_ExitMenu = 0;
    mMission_Aborted = false;
    mDemo_ExitMenu = -1;

    mImage->Save();
    mGraphics->PaletteSet();
    mImage->palette_FadeTowardNew();

    for (;;) {

        Video_Sleep_Wrapper();

        if (Menu_Draw( pButtonHandler ))
            break;
    }

    Image_FadeOut();
    mImage->clearBuffer();
}

bool cFodder::Menu_Draw( const std::function<void()> pButtonHandler ) {

    // Handle Mouse
    Mouse_Inputs_Get();
    Mouse_DrawCursor();

    // Button Pressed?
    if (mButtonPressLeft)
        pButtonHandler();

    // Exit Time?
    if (mDemo_ExitMenu > 0 || mMission_Aborted || mCustom_ExitMenu)
        return true;

    if (mImage->GetFaded() == false)
        mImage->palette_FadeTowardNew();

    g_Window.RenderAt( mImage );
    g_Window.FrameEnd();
    mImage->Restore();
    return false;
}

void cFodder::Demo_Quiz_ShowScreen( const char* pFilename ) {
        
    Image_FadeOut();

    mGraphics->Load_And_Draw_Image( pFilename, 32 );
    mGraphics->PaletteSet();

    mImage->Save();
    mImage->palette_FadeTowardNew();
    
    for( ;; ) {
        Video_Sleep_Wrapper();

        Mouse_Inputs_Get();
        Mouse_DrawCursor();

        if (mButtonPressLeft)
            break;

        if (mImage->GetFaded() == false )
            mImage->palette_FadeTowardNew();

        g_Window.RenderAt( mImage );
        g_Window.FrameEnd();
        mImage->Restore(); 
    }
        
    Image_FadeOut();

    mGraphics->Load_And_Draw_Image( "1.lbm", 32 );
    mGraphics->PaletteSet();

    mImage->Save();
    mImage->palette_FadeTowardNew();
}

void cFodder::Demo_Quiz() {
    Image_FadeOut();

    mGraphics->Load_And_Draw_Image( "1.lbm", 32 );
    Mouse_Setup();

    Menu_Loop(
        []() {
        if (g_Fodder.mButtonPressLeft)
            g_Fodder.GUI_Element_Mouse_Over( mPlusQuiz_Buttons );
    }
    );


    mGraphics->Load_And_Draw_Image( "apmenu.lbm", 32 );
    mGraphics->PaletteSet();
    mImage->palette_FadeTowardNew();
    mImage->Save();
}

void cFodder::GUI_Loop_Draw_Prepare( int16& pData0, int16& pData4 ) {
    mGUI_Loop_Squad_Current = pData0;
    mGUI_Loop_Draw_Y = pData4;
    mGUI_Loop_Is_CurrentSquad = 0;

    if (pData0 == mSquad_Selected)
        mGUI_Loop_Is_CurrentSquad = -1;
}

void cFodder::GUI_Sidebar_Prepare( int16 pSquadNumber, int16 pData4 ) {
    
    GUI_Loop_Draw_Prepare( pSquadNumber, pData4 );

    word_3AC2D[pSquadNumber] = pData4;
    Squad_Troops_Count();
    
    if (!mSquads_TroopCount[mGUI_Loop_Squad_Current])
        return;

    GUI_Sidebar_SplitButton_Draw();
    GUI_Prepare_Button_Squad();

    Mission_Set_Initial_Weapon();

    GUI_Sidebar_Grenades_Draw();
    GUI_Prepare_Button_Grenade();

    GUI_Sidebar_Rockets_Draw();
    GUI_Prepare_Button_Rockets();

    GUI_Sidebar_SquadIcon_Update();

    GUI_Sidebar_TroopList_Draw();
    GUI_Prepare_Button_TroopName();
}

void cFodder::GUI_Sidebar_Add_Sidebar_Overlay() {

    GUI_SetElementsFrom(mGUI_NextFreeElement, mSidebar_OverlayButtons );
}

void cFodder::GUI_SetElementsFrom( sGUI_Element* pTo, sGUI_Element* pFrom ) {
    
    while (pFrom->field_0 != 0) {
        *pTo = *pFrom;

        ++pTo;
        ++pFrom;
    }

    GUI_ClearElement(pTo);

}

void cFodder::GUI_ClearElement( sGUI_Element *pData20 ) {

    pData20->field_0 = 0;
    mGUI_NextFreeElement = pData20;
}

void cFodder::GUI_Sidebar_SplitButton_Draw() {

    int16 Data0 = mGUI_Squad_Icons[mGUI_Loop_Squad_Current];
    int16 Data8 = 0;
    int16 DataC = mGUI_Loop_Draw_Y;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC );
}

void cFodder::GUI_Prepare_Button_Squad() {
    sGUI_Element* Element = mGUI_NextFreeElement;

    Element->field_0 = &cFodder::GUI_Button_NoAction;
    Element->mX = 0;
    Element->mY = mGUI_Loop_Draw_Y;

    Element->mWidth = 0x2F;
    Element->mHeight = 0x0E;

    if (mGUI_Loop_Squad_Current != mSquad_Selected) {
        Element->mMouseInsideFuncPtr = mGUI_Handle_Button_SelectSquad_Array[mGUI_Loop_Squad_Current];
    }
    else {
        Element->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_SplitSquad;
    }

    ++Element;
    GUI_ClearElement( Element );
}

void cFodder::GUI_Handle_Button_SelectSquad_0() {
    Squad_Select( 0 );
}

void cFodder::GUI_Handle_Button_SelectSquad_1() {
    Squad_Select( 1 );
}

void cFodder::GUI_Handle_Button_SelectSquad_2() {
    Squad_Select( 2 );
}

void cFodder::Squad_Select( int16 pData4 ) {
    
    if (Mouse_Button_Left_Toggled() < 0)
        return;

    mSquad_Selected = pData4;
    Mission_Troops_Clear_Selected();
    Squad_Troops_Count();
    
    sSprite* Data20 = 0;
    sub_305D5( Data20 );

    if (Data20 == INVALID_SPRITE_PTR)
        return;

    mCamera_Position_X = Data20->field_0;
    mCamera_Position_Y = Data20->field_4;
    mCamera_Start_Adjust = 1;
    mSquad_Select_Timer = 1;
    mGUI_Sidebar_Setup = 0;
    mSquad_Grenade_SplitMode = eSquad_Weapon_Split_Half;
    mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;
    word_3AC4B = 0;
    word_3AC4D = 0;
}

void cFodder::GUI_Handle_Button_SplitSquad() {

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    if (!sub_2FF41())
        return;

    Squad_Split_Assets();

    mSquad_Grenade_SplitMode = eSquad_Weapon_Split_Half;
    mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;
    word_3AC4B = 0;
    word_3AC4D = 0;

    Squad_Walk_Target_Update( mSquad_Selected );
    Squad_Troops_Count();

    sSprite* Data20 = 0;
    sub_305D5( Data20 );

    mSquad_Select_Timer = 1;
    mGUI_Sidebar_Setup = 0;
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

    //if (mVersion->mRelease == eRelease::Demo) 
    //  return;

    WindowTitleSet( false );

    mGraphics->Load_Service_Data();
    mGraphics->SetActiveSpriteSheet( eGFX_SERVICE );
    mGraphics->PaletteSet();

    mSound->Music_Play( 0 );
    Service_KIA_Loop();
    Service_Promotion_Loop();
    Mouse_Setup();

    mGraphics->Load_pStuff();
}

void cFodder::Service_KIA_Loop() {
    Mouse_Setup();
    word_44475 = 0;

    if (Service_KIA_Troop_Prepare() < 0)
        return;

    mImage->clearBuffer();

    if (mVersion->mPlatform == ePlatform::PC) {
        GUI_Draw_Frame_8 (  5, 0, 0x34, 0 );
        GUI_Draw_Frame_16(  7, 0, 0,    0x31 );
        GUI_Draw_Frame_16(  7, 0, 0xF0, 0x31 );
    }
    else {
        ((cGraphics_Amiga*)mGraphics)->Service_Draw( 8, 0x30,   0 );
        ((cGraphics_Amiga*)mGraphics)->Service_Draw( 0, 0,      0x40 );
        ((cGraphics_Amiga*)mGraphics)->Service_Draw( 4, 0xF0,   0x40 );
    }

    mImageFaded = -1;
    mMouse_Exit_Loop = 0;
    mService_ExitLoop = 0;
    mGraphics->PaletteSet();
    mImage->Save();

    do {
        Mouse_Inputs_Get();

        if (word_44475 == -1 || mMouse_Exit_Loop ) {
            mMouse_Exit_Loop = 0;
            mImage->paletteNew_SetToBlack();
            mImageFaded = -1;
            mService_ExitLoop = 1;
        }

        if (mImageFaded == -1)
            mImageFaded = mImage->palette_FadeTowardNew();

        sub_18149();
        Video_Sleep_Wrapper();
        sub_181BD();

        g_Window.RenderAt( mImage );
        g_Window.FrameEnd();
        mImage->Restore();

    } while (mImageFaded == -1 || mService_ExitLoop == 0);
}

void cFodder::Service_Promotion_Loop() {
    Mouse_Setup();
    word_44475 = 0;
    Service_Promotion_Prepare();

    if (Service_Promotion_Prepare_Draw() < 0)
        goto loc_18001;

    mImage->clearBuffer();
    if (mVersion->mPlatform == ePlatform::PC) {
        GUI_Draw_Frame_8( 6, 0, 0x34, 0 );
        GUI_Draw_Frame_16( 8, 0, 0, 0x31 );
        GUI_Draw_Frame_16( 8, 0, 0xF0, 0x31 );
    }
    else {
        ((cGraphics_Amiga*)mGraphics)->Service_Draw( 9, 0x30,   0 );        // Heroes in Victory
        ((cGraphics_Amiga*)mGraphics)->Service_Draw( 3, 0,      0x40 );     //  Left Symbol
        ((cGraphics_Amiga*)mGraphics)->Service_Draw( 7, 0xF0,   0x40 );     //  Right Symbol
    }

    mImageFaded = -1;
    mService_ExitLoop = 0;
    mMouse_Exit_Loop = 0;
    mGraphics->PaletteSet();
    mImage->Save();

    do {

        Mouse_Inputs_Get();
        if (word_44475 == -1 || mMouse_Exit_Loop ) {
            mMouse_Exit_Loop = 0;
            mImage->paletteNew_SetToBlack();
            mImageFaded = -1;
            mService_ExitLoop = 1;
        }

        if (mImageFaded == -1)
            mImageFaded = mImage->palette_FadeTowardNew();

        Service_Promotion_Check();
        sub_18149();
        Video_Sleep_Wrapper();
        //sub_14445();
        sub_181BD();

        g_Window.RenderAt( mImage, cPosition() );
        g_Window.FrameEnd();
        mImage->Restore();

    } while (mImageFaded == -1 || mService_ExitLoop == 0);

loc_18001:;
    
    Service_Promotion_SetNewRanks();
}

int16 cFodder::Service_KIA_Troop_Prepare() {
    uint16* di = (uint16*) mGraphics->mImagePStuff.mData->data();

    mVideo_Draw_PosY = 0xE8;
    Service_Mission_Text_Prepare( di );
    mVideo_Draw_PosY += 0x40;

    int16* si = mGraveRecruitID;

    if (*si == -1)
        return -1;

    for (;;) {
        int16 ax = *si++;
        if (ax == -1 )
            break;

        int16* bp = mGraveRankPtr2;
        int8 bl = *bp & 0xFF;
        ++bp;
        mGraveRankPtr2 = bp;

        Service_Draw_Troop_And_Rank( di, ax, bl );
        mVideo_Draw_PosY += 0x40;
    }

    *di++ = -1;
    return 0;
}

int16 cFodder::Service_Promotion_Prepare_Draw() {
    uint16* di = (uint16*)mGraphics->mImagePStuff.mData->data();

    mVideo_Draw_PosY = 0xE8;
    Service_Mission_Text_Prepare( di );
    mVideo_Draw_PosY += 0x40;

    uint16* di2 = di;
    sMission_Troop* si = mMission_Troops;

    for (int16 word_3ABE1 = 7; word_3ABE1 >= 0; --word_3ABE1, ++si) {

        if (si->mSprite != INVALID_SPRITE_PTR ) {
            int16 ax = si->mRecruitID;
            int8 bl = si->mRank;

            bl &= 0xFF;

            Service_Draw_Troop_And_Rank( di, ax, bl );

            mVideo_Draw_PosY += 0x40;
        }
    }
    if (di == di2)
        return -1;

    *di++ = -1;

    return 0;
}

void cFodder::Service_Draw_Troop_And_Rank( uint16*& pDi, int16 pRecruitID, int16 pRank ) {

    if (mVersion->mPlatform == ePlatform::PC) {
        *pDi++ = 9;
        *pDi++ = 0;
    } else {
        *pDi++ = 0;
        *pDi++ = 1;
    }
    *pDi++ = 0x60;
    *pDi++ = mVideo_Draw_PosY;

    *pDi++ = 2;
    *pDi++ = pRank;
    *pDi++ = 0x64;
    *pDi++ = mVideo_Draw_PosY + 4;

    *pDi++ = 2;
    *pDi++ = pRank;
    *pDi++ = 0xCC;
    *pDi++ = mVideo_Draw_PosY + 4;

    //seg003:3237
    int16 cx = 5;
    const char* si = mRecruits[pRecruitID].mName;
    std::stringstream tmpString;

    for (cx = 5; cx >= 0; --cx) {
        if (*si == 0x20)
            break;

        tmpString << *si++;
    }

    String_CalculateWidth( 0x140, mFont_ServiceName_Width, tmpString.str().c_str() );
    sub_181E6( pDi, tmpString.str().c_str(), mFont_ServiceName_Width, 3, mGUI_Temp_X, mVideo_Draw_PosY + 6 );
}

void cFodder::sub_18149() {

    int16 *di = (int16*)mGraphics->mImagePStuff.mData->data();

    for (;;) {
        if (*di < 0)
            break;

        int16 Data0 = *di++;
        int16 Data4 = *di++;
        int16 Data8 = *di++;
        int16 DataC = *di++;

        sub_1828A( Data0, Data4, Data8, DataC );
    }
}

void cFodder::sub_181BD() {
    word_44475 = -1;

    int16* es = (int16*)mGraphics->mImagePStuff.mData->data();

    for (;;) {
        if (*es == -1)
            break;
        
        *(es+3) -= 1;
        if (*(es + 3) >= -48)
            word_44475 = 0;

        es += 4;
    }
}

void cFodder::sub_181E6( uint16*& pDi, const std::string& pText, const uint8* pData28, int16 pData0, int16 pData8, int16 pDataC ) {

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
        *pDi++ = pData0;
        *pDi++ = Data4;
        *pDi++ = pData8;
        *pDi++ = pDataC;

    loc_1826D:;

        Data10 = pData28[Data10];
        pData8 += Data10;
    }
}

int16 cFodder::sub_1828A( int16& pSpriteType, int16& pFrame, int16& pData8, int16& pDataC ) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_PosX = pData8 + 0x10;
    mVideo_Draw_PosY = pDataC + 0x10;

    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount;

    if (!sub_184C7()) {
        if (mVersion->mPlatform == ePlatform::Amiga)
            mGraphics->Video_Draw_16();
        else
            sub_182EA();

        return 1;
    }
    return 0;
}

void cFodder::sub_182EA() {
    const uint8* bp = &mFont_Intro_Width[0x80];

    uint8* di = mImage->GetSurfaceBuffer();
    
    di += (mImage->GetWidth() * mVideo_Draw_PosY);
    di += mVideo_Draw_PosX;

    word_42066 = di;

    uint8* si = mVideo_Draw_FrameDataPtr;

    mVideo_Draw_Columns >>= 1;
    mDraw_Source_SkipPixelsPerRow = 0xA0 - mVideo_Draw_Columns;
    mVideo_Draw_Columns >>= 1;

    mDraw_Dest_SkipPixelsPerRow = mImage->GetWidth() - (mVideo_Draw_Columns*4);
    uint8 Plane = 0;

    for (int16 dx = 0; dx < mVideo_Draw_Rows; ++dx ) {

        int16 bx = mVideo_Draw_PosY;
        bx += dx;

        uint8 bl = *(bp + bx);

        for (int16 cx = mVideo_Draw_Columns; cx > 0; --cx) {
            uint8 al = *si;
            al >>= 4;
            if (al) {
                al |= bl;
                *di = al;
            }
            
            di += 4;
            si += 2;
        }

        si += mDraw_Source_SkipPixelsPerRow;
        di += mDraw_Dest_SkipPixelsPerRow;
    }

    ++Plane;
    if (Plane == 4) {
        Plane = 0;
        ++word_42066;
    }

    si = mVideo_Draw_FrameDataPtr;
    di = word_42066;
    di += Plane;
    for (int16 dx = 0; dx < mVideo_Draw_Rows; ++dx ) {

        int16 bx = mVideo_Draw_PosY;
        bx += dx;

        uint8 bl = *(bp + bx);

        for (int16 cx = mVideo_Draw_Columns; cx > 0; --cx) {
            uint8 al = *si;
            al &= 0x0F;
            if (al) {
                al |= bl;
                *di = al;
            }
            
            di += 4;
            si += 2;
        }

        si += mDraw_Source_SkipPixelsPerRow;
        di += mDraw_Dest_SkipPixelsPerRow;
    }

    ++Plane;
    if (Plane == 4) {
        Plane = 0;
        ++word_42066;
    }

    ++mVideo_Draw_FrameDataPtr;
    si = mVideo_Draw_FrameDataPtr;
    di = word_42066;
    di += Plane;

    for (int16 dx = 0; dx < mVideo_Draw_Rows; ++dx ) {

        int16 bx = mVideo_Draw_PosY;
        bx += dx;

        uint8 bl = *(bp + bx);

        for (int16 cx = mVideo_Draw_Columns; cx > 0; --cx) {
            uint8 al = *si;
            al >>= 4;
            if (al) {
                al |= bl;
                *di = al;
            }
            
            di += 4;
            si += 2;
        }

        si += mDraw_Source_SkipPixelsPerRow;
        di += mDraw_Dest_SkipPixelsPerRow;
    }
    
    ++Plane;
    if (Plane == 4) {
        Plane = 0;
        ++word_42066;
    }

    si = mVideo_Draw_FrameDataPtr;
    di = word_42066;
    di += Plane;
    for (int16 dx = 0; dx < mVideo_Draw_Rows; ++dx ) {

        int16 bx = mVideo_Draw_PosY;
        bx += dx;

        uint8 bl = *(bp + bx);

        for (int16 cx = mVideo_Draw_Columns; cx > 0; --cx) {
            uint8 al = *si;
            al &= 0x0F;
            if (al) {
                al |= bl;
                *di = al;
            }
            
            di += 4;
            si += 2;
        }

        si += mDraw_Source_SkipPixelsPerRow;
        di += mDraw_Dest_SkipPixelsPerRow;
    }
}

int16 cFodder::sub_184C7() {
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

        if (mVersion->mPlatform == ePlatform::PC)
            ax *= 160;
        else if (mVersion->mPlatform == ePlatform::Amiga)
            ax *= 40;

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

void cFodder::Service_Mission_Text_Prepare( uint16*& pTarget ) {
    std::stringstream Mission;
    Mission << "MISSION ";

    Mission << tool_StripLeadingZero( tool_NumToString( mMissionNumber ) );

    String_CalculateWidth( 0x140, mFont_Service_Width, Mission.str().c_str() );

    sub_181E6( pTarget, Mission.str(), mFont_Service_Width, 4, mGUI_Temp_X, mVideo_Draw_PosY );
}

void cFodder::Service_Promotion_Prepare() {
    int16* Data28 = mService_Troop_Promotions;

    sMission_Troop* Data20 = mMission_Troops;

    for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
        if (Data20->mRecruitID == -1)
            continue;

        if (Data20->mSprite == INVALID_SPRITE_PTR )
            continue;

        int16 Data4 = Data20->mPhaseCount;
        Data4 += Data20->mRank;

        if (Data4 > 0x0F)
            Data4 = 0x0F;

        *Data28++ = Data4;
    }

    *Data28 = -1;
}

void cFodder::Service_Promotion_Check() {
    int16* si = mService_Troop_Promotions;
    int16* es = (int16*)mGraphics->mImagePStuff.mData->data();

    for (;;es+=4) {
        if (*es == -1)
            break;

        if (*es != 2)
            continue;

        int16 ax = *si++;
        es += 4;
        if (*(es + 3) >= 0x6C)
            continue;

        *(es - 3) = ax;
        *(es + 1) = ax;
    }
}

void cFodder::Service_Promotion_SetNewRanks() {
    int16* Data28 = mService_Troop_Promotions;
    sMission_Troop* Data20 = mMission_Troops;

    for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
        
        if (Data20->mRecruitID == -1)
            continue;

        int16 ax = *Data28++;
        if (ax < 0)
            return;

        Data20->mRank = (int8) ax;
    }
}

void cFodder::Sidebar_Clear_ScreenBufferPtr() {
    
    for (uint16 cx = 0; cx < 0x2000; ++cx )
        mSidebar_Screen_BufferPtr[cx] = 0;
}

void cFodder::Briefing_Draw_Phase( ) {
    const char* Str_Brief = "BRIEFING";
    const char* Str_Phase = "PHASE ";
    const char* Str_Of = " OF ";

    mString_GapCharID = 0x25;

    String_Print_Large( Str_Brief, true, 0x4E );
    
    Briefing_DrawBox( 1, 0x49, 0x13E, 0x6B, 0xF3 );
    Briefing_DrawBox( 0, 0x48, 0x13E, 0x6B, 0xF2 );
    
    mString_GapCharID = 0;

    std::stringstream Phase;

    Phase << Str_Phase  << tool_StripLeadingZero( tool_NumToString( mMissionPhase + 1 ) );
    Phase << Str_Of     << tool_StripLeadingZero( tool_NumToString( mMission_Phases_Total ));

    String_Print_Small( Phase.str(), 0x1D );
    
    Mission_Phase_Goals_Set();

    int16 DataC = 0x84;
    int16* Goals = mPhase_Goals;

    for (const auto GoalName : mMissionGoal_Titles) {
        if (*Goals++) {

            String_Print_Small( GoalName, DataC - 0x12 );
            DataC += 0x0C;
        }
    }
}

void cFodder::Briefing_Show_PreReady() {

    if (mVersion->isDemo() && !mCampaign.isCustom() && !mCampaign.isRandom())
        return;

    if (!mVersion->hasGfx(eGFX_BRIEFING) && !mCampaign.isRandom())
        VersionLoad(mVersionDefault);

    mImage->clearBuffer();
    mGraphics->PaletteSet();

    mGraphics->SetActiveSpriteSheet( eGFX_BRIEFING );

    Briefing_Draw_Mission_Title( 0x2C );
    Briefing_Draw_Phase();

    Image_FadeIn();
}

void cFodder::Briefing_Show_Ready() {

    if (mVersion->isDemo() && !mCampaign.isCustom() && !mCampaign.isRandom())
        return;

    mGraphics->SetActiveSpriteSheet( eGFX_BRIEFING );

    Briefing_Draw_Mission_Title( 0x2C );
    Briefing_Draw_Phase( );
    Briefing_Draw_With( );

    mMouse_Exit_Loop = 0;

    do {
        Video_Sleep();
        eventProcess();
        Mouse_Inputs_Get();

        if (mMission_Aborted) {
            mBriefing_Aborted = -1;
            mouse_Button_Status = -1;
            break;
        }

        g_Window.RenderAt( mImage );
        g_Window.FrameEnd();

    } while (!mMouse_Exit_Loop);

    mMouse_Exit_Loop = 0;

    Image_FadeOut();
    Mouse_Setup();
}

void cFodder::Briefing_Draw_With( ) {
    std::stringstream With;
    
    With << "WITH ";
    With << tool_StripLeadingZero( tool_NumToString( mMission_Troops_Available ) );

    if (mMission_Troops_Available == 1) {
        With << " SOLDIER YOU MUST";
    }
    else {
        With << " SOLDIERS YOU MUST";
    }

    String_Print_Small( With.str(), 0x64 );
    With.str("");

    if (!mRecruits_Available_Count) {
        With << "THIS IS YOUR LAST CHANCE";
    }
    else {
        With << mRecruits_Available_Count;
        if (mRecruits_Available_Count == 1)
            With << " RECRUIT REMAINING";
        else
            With << " RECRUITS REMAINING";
    }

    String_Print_Small( With.str(), 0xA8 );
    String_Print_Large( "GO FOR IT", false, 0xB8 );
}

void cFodder::Briefing_DrawBox( int16 pX, int16 pY, int16 pWidth, int16 pHeight, uint8 pColor ) {
    pX += 0x10;
    pY += 0x10;

    // Top and Bottom
    Briefing_Draw_Horizontal_Line( pX, pX + pWidth, pY, pColor );

    Briefing_Draw_Horizontal_Line( pX, pX + pWidth, pY + pHeight, pColor );

    // Sides of box
    Briefing_Draw_Vertical_Line( pX, pY + pHeight, pY, pColor );
    
    Briefing_Draw_Vertical_Line( pX + pWidth, pY + pHeight, pY, pColor );
}

void cFodder::Briefing_Draw_Pixel( int16 pX, int16 pY, uint8 pColor ) {
    uint8* di = mImage->GetSurfaceBuffer();

    di += mImage->GetWidth() * pY;
    di += pX;
    *di = pColor;
}

void cFodder::Briefing_Draw_Horizontal_Line( int16 pX, int16 pWidth, int16 pY, uint8 pColor ) {
    
    do {
        Briefing_Draw_Pixel( pX, pY, pColor );
        ++pX;
    } while (pX <= pWidth);
}

void cFodder::Briefing_Draw_Vertical_Line(  int16 pX, int16 pHeight, int16 pY, uint8 pColor ) {
    
    do {
        Briefing_Draw_Pixel( pX, pY, pColor );
        ++pY;
    } while (pY <= pHeight);
}

void cFodder::Sprite_Frame_Modifier_Update() {

    word_39FF4 -= 1;
    if (word_39FF4 < 0) {
        word_39FF4 = 3;

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
    sSprite* Data20 = mSprites;

    for (int16 Data1C = 0x2B; Data1C > 0; --Data1C, ++Data20) {

        if (Data20->field_0 == -32768)
            continue;

        int16 Data4 = Data20->field_18;

        (*this.*mSprite_Function[Data4])( Data20 );
    }
}

void cFodder::Sprite_Handle_Null( sSprite* pSprite ) {
    
    switch (pSprite->field_18) {
    case 4:
        return;

    default:
        std::cout << "Sprite Function not implemented: " << pSprite->field_18 << "\n";
        break;
    }

}

void cFodder::Sprite_Handle_Player( sSprite *pSprite ) {
    int16 Data0, Data4, eax;
    sSprite* Data28 = 0;

    // player soldier in vehicle?
    if (pSprite->field_6E) {
        Sprite_Handle_Player_InVehicle( pSprite );
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

    if (Sprite_Handle_Soldier_Animation( pSprite ))
        return;

    // Animation running?
    if (pSprite->field_38)
        return;

    if (Sprite_Handle_Player_MissionOver( pSprite ))
        return;

    //loc_18E8D
    Sprite_Handle_Player_Enter_Vehicle( pSprite );
    mSprite_FaceWeaponTarget = 0;

    word_3AA1D = word_3BED5[pSprite->field_32];
    //seg004:0183

    if (mSquad_Join_TargetSquad[pSprite->field_32] < 0 && pSprite->field_32 != mSquad_Selected ) {
        //loc_18F12
        mSprite_FaceWeaponTarget = -1;

        Data28 = &mSprites[pSprite->field_5E];
        
        if( Data28->field_0 != -32768 ) {
            
            if( Data28->field_18 == eSprite_Enemy ) {
                if( Data28->field_38 == eSprite_Anim_None) {
                
                    //seg004:021A 
                    if( pSprite->field_4F == 0 ) {
                        //seg004:0228
                        int16 Data0 = pSprite->field_0;
                        int16 Data4 = pSprite->field_4;
                        
                        int16 Data8 = Data28->field_0;
                        int16 DataC = Data28->field_4;
                        
                        Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
                        if (Data0 < 0xD2) {

                            if (Data0 <= 0x28)
                                goto loc_1904A;

                            //seg004:0270
                            mSprite_Find_Distance = Data0;
                            Data0 = pSprite->field_0;
                            Data4 = pSprite->field_4;
                            Data8 = Data28->field_0;
                            DataC = Data28->field_4;

                            if (sub_2A4A2( Data0, Data4, Data8, DataC ) == 0) {
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
        if (pSprite->field_5E < 43)
            goto loc_191C3;

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
        
    loc_190E8:;
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
            if (pSprite == mSquad_Leader && mMouse_Button_LeftRight_Toggle ) {

                if (mSquad_CurrentWeapon[pSprite->field_32] == eWeapon_Rocket) {
                    mTroop_Weapon_Grenade_Disabled = true;
                    mTroop_Weapon_Bullet_Disabled = true;

                } else {
                    if (mSquad_CurrentWeapon[pSprite->field_32] == eWeapon_Grenade) {
                        mTroop_Weapon_Rocket_Disabled = true;
                        mTroop_Weapon_Bullet_Disabled = true;
                    } else
                        goto loc_19198;
                }
            loc_1918C:;
                pSprite->field_4A = 1;
                goto loc_191BF;
            }

        loc_19198:;
            mTroop_Weapon_Grenade_Disabled = true;
            mTroop_Weapon_Rocket_Disabled = true;

            Data0 =  pSprite->field_46_mission_troop->field_6;

        loc_191BF:;
            Sprite_Handle_Troop_Weapon( pSprite );
        }
    }
    else {

        if (word_3A9CE) {
            goto loc_190E8;
            
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

            if ( pSprite->field_0 == pSprite->field_26 &&
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
        Sprite_Handle_Troop_Direct_TowardWeaponTarget( pSprite );
    else
        Sprite_Handle_Troop_Direct_TowardMouse( pSprite );

    Data0 = pSprite->field_26;
    if( Data0 < 0 )
        goto loc_1946D;
    
    Data4 = pSprite->field_28;
    if( Data4 < 0 )
        goto loc_1946D;
    
    Sprite_XY_Store( pSprite );
    
    if( Sprite_Direction_Between_Points( pSprite, Data0, Data4 ) >= 0 ) {
        Sprite_Handle_Player_Adjust_Movement_Speed( pSprite );
        Sprite_Draw_Row_Update( pSprite );
    }

    if(pSprite->field_0 == pSprite->field_26 ) {
        
        if(pSprite->field_4 == pSprite->field_28 )
            Sprite_Next_WalkTarget_Set( pSprite );
    }

    //loc_19314
    if( mSprite_Reached_Target == 0 )
        return;
    
    loc_1931E:;
    
    if( pSprite->field_5A )
        goto loc_19403;
    
    Sprite_Handle_Troop_Direct_TowardMouse( pSprite );
    Sprite_Handle_Troop_FrameUnk( pSprite );
    return;
    
    loc_19338:;
    pSprite->field_43 = 0;
    mTmp_FrameNumber = pSprite->field_A;
    mSprite_Bumped_Into_SquadMember = 0;
    
    sub_1FCF2( pSprite );
    Data0 = pSprite->field_26;
    if( Data0 < 0 )
        goto loc_1946D;
    Data4 = pSprite->field_28;
    if( Data4 < 0 )
        goto loc_1946D;
    
    if( pSprite->field_44 ) {
        pSprite->field_44 -= 1;
        goto loc_193D6;
        
    } 
    
    // Reached near target?
    //loc_19392
    if( Sprite_Direction_Between_Points( pSprite, Data0, Data4 ) >= 0 )
        goto loc_193D6;
    
    // Exactly on target?
    //  Then move toward the next target
    if( pSprite->field_0 == pSprite->field_26 ) {
        if( pSprite->field_4 == pSprite->field_28 )
            Sprite_Next_WalkTarget_Set( pSprite );
    }
    //loc_193D3
    goto loc_1946D;
    
loc_193D6:;
    Sprite_XY_Store( pSprite );
    sub_2A3D4( pSprite );
    
    if( !pSprite->field_5A ) {
        if( !pSprite->field_45 )
            goto loc_19424;
        
        pSprite->field_45 -= 1;
        return;
    }
    
loc_19403:;
    pSprite->field_5A = 0;
    sub_1FDE7( pSprite );
    return;
    
loc_19414:;
    pSprite->field_5A = 0;
    Sprite_Handle_Troop_Direct_TowardMouse( pSprite );
    return;
    
loc_19424:; // Soldier Walking
    Sprite_Draw_Row_Update( pSprite );
    
    if( pSprite->field_0 == pSprite->field_26 ) {
        
        if( pSprite->field_4 == pSprite->field_28 )
            Sprite_Next_WalkTarget_Set( pSprite );
        
    }
    //loc_19463
    if( mSprite_Reached_Target )
        goto loc_19414;
    
    goto loc_1957A;
    
loc_1946D:;
    if( pSprite->field_5A )
        goto loc_19403;
    
    if( !pSprite->field_45 )
        goto loc_194A0;
    
    pSprite->field_45 -= 1;
    goto loc_19414;

loc_19490:;
    pSprite->field_5A = 0;
    Sprite_Handle_Troop_Direct_TowardMouse( pSprite );
    return;

loc_194A0:;
    pSprite->field_43 = -1;
    
    if( !word_3AA1D ) {
        mDirectionMod = 0;
        Sprite_Handle_Troop_FrameUnk( pSprite );
        pSprite->field_A = 0;
        return; 
    }

    //loc_194CB
    if( mSquad_Selected == pSprite->field_32 )
        goto loc_19490;
    
    Sprite_Handle_Troop_Direct_TowardMouse( pSprite );
    return;

loc_1957A:;
    pSprite->field_3A = 0;
    Sprite_Handle_Troop_FrameUnk( pSprite );

}

void cFodder::Sprite_Handle_Grenade( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data24;
    int32 tmp = 0;
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
        if(mVersion->mVersion != eVersion::AmigaPlus)
            Sound_Play( pSprite, eSound_Effect_Grenade, 0x0F );
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
    
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 <= 1)
        pSprite->field_36 = 0;

    if (Data0 <= 4)
        pSprite->field_36 >>= 1;

    if (pSprite->field_36) {
        pSprite->field_36 -= 1;
        Sprite_XY_Store( pSprite );

        mTmp_FrameNumber = pSprite->field_A;
        Data0 = pSprite->field_26;
        Data4 = pSprite->field_28;

        Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
        Sprite_Movement_Calculate( pSprite );
        Sprite_Handle_Grenade_Terrain_Check( pSprite );

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
    a1 = (int16) (((int64)pSprite->field_1A) >> 16);

    if (a1 < 0)
        if (a1 < -8)
            pSprite->field_1A = (((int32)pSprite->field_1A & 0xFFFF) | (-8 << 16));

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

        Dataa4 = (int64) pSprite->field_1A;
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
        sub_1998C( pSprite );
        Sprite_Destroy( pSprite + 1 );
        return;
    }
    //loc_19957
    sub_1998C( pSprite );
    pSprite->field_0 = 0;
    pSprite->field_4 = 0;

    Sprite_Destroy( pSprite );
    Sprite_Destroy( pSprite + 1 );
}

void cFodder::sub_1998C( sSprite* pSprite ) {
    int16 Data0 = pSprite->field_22;

    int8* Data24 = byte_3A9D2;
    --Data24[Data0];
}

void cFodder::Sprite_Handle_ShadowSmall( sSprite* pSprite ) {
    sSprite* Data24 = pSprite - 1;

    pSprite->field_0 = Data24->field_0;
    pSprite->field_4 = Data24->field_4;
    pSprite->field_52 = Data24->field_52;

}

void cFodder::Sprite_Handle_Enemy( sSprite* pSprite ) {

    if (Sprite_Handle_Soldier_Animation( pSprite ))
        return;

    if (pSprite->field_38)
        return;

    mSprite_FaceWeaponTarget = 0;
    if (pSprite->field_44) {
        pSprite->field_44--;
    }
    else {
        sub_21CD1( pSprite );
        Sprite_Handle_Troop_Weapon( pSprite );

        mTmp_FrameNumber = pSprite->field_A;
        int16 Data0 = pSprite->field_26;
        if (Data0 < 0)
            goto loc_19A89;

        int16 Data4 = pSprite->field_28;
        if (Data4 < 0)
            goto loc_19A89;

        if (Sprite_Direction_Between_Points( pSprite, Data0, Data4 ) < 0)
            goto loc_19A89;
    }
    //loc_19A5D
    Sprite_XY_Store( pSprite );
    sub_2A3D4( pSprite );
    sub_1FCF2( pSprite );

    if (pSprite->field_45) {
        pSprite->field_45--;
        if (pSprite->field_45)
            return;
    }
    Sprite_Draw_Row_Update( pSprite );
    goto loc_19A96;

loc_19A89:;
    word_3ABB1 = 0;
    sub_1FDE7( pSprite );
    goto loc_19A9C;

loc_19A96:;
    word_3ABB1 = -1;

loc_19A9C:;
    pSprite->field_22 = eSprite_PersonType_AI;
    Sprite_Handle_Troop_FrameUnk( pSprite );

    if (word_3ABB1)
        return;
    if (mSprite_FaceWeaponTarget)
        return;

    pSprite->field_A = 1;
}

void cFodder::Sprite_Handle_Bullet( sSprite* pSprite ) {
    int16 Data8, DataC, Data0, Data4;
    int8 al;
    sSprite* Data28;

    pSprite->field_64 += 1;
    
    if(!pSprite->field_2A)
        goto loc_19BA8;
    
    Data28 = pSprite->field_1A_sprite;
    
    Data0 = Data28->field_0;
    Data4 = Data28->field_4;
    
    Data0 += pSprite->field_16;
    Data4 += pSprite->field_14;
    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;
    
    pSprite->field_2A -= 1;
    if( pSprite->field_2A )
        return;
    
    pSprite->field_8 = 0x7F;
    pSprite->field_A = 0;
    
    if(pSprite->field_22 != eSprite_PersonType_Human)
        pSprite->field_A = 3;
    
    Data0 = Data28->field_0;
    Data4 = Data28->field_4;
    Data0 += 7;
    Data4 += 5;
    pSprite->field_0 = Data0;
    pSprite->field_4 = Data4;
    
loc_19BA8:;
    if(pSprite->field_43)
        goto loc_19DCF;
    
    if(!pSprite->field_12)
        goto loc_19DCF;
    
    pSprite->field_12 -= 1;
    pSprite->field_3A += 1;
    
    if(pSprite->field_44)
        goto loc_19D24;
    
    Sprite_XY_Store( pSprite );
    
    if(pSprite->field_59)
        goto loc_19C6B;
    
    Data0 = pSprite->field_2E;
    Data4 = pSprite->field_30;
    
    Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
    
    Data0 = pSprite->field_50;
    if( Data0 ) {
        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
        pSprite->field_59 = -1;
    }
    //loc_19C4D
    
    if(pSprite->field_34 < 0) {
        pSprite->field_34 = pSprite->field_10;
    }
    
    loc_19C6B:;

    Sprite_Movement_Calculate(pSprite);
    if(mSprite_Bullet_Destroy)
        goto SpriteDestroy;
    pSprite->field_36 = pSprite->field_4A;
    
    if(pSprite->field_59)
        goto loc_19D3C;
    
    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;
    
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    
    Data4 = pSprite->field_36;
    Data4 >>= 3;
    
    if( Data0 > Data4 )
        goto loc_19D3C;
    
    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_0 = pSprite->field_2E;
    pSprite->field_4 = pSprite->field_30;
    pSprite->field_44 = -1;
    goto loc_19D3C;
    
loc_19D24:;
    pSprite->field_2C = eSprite_Draw_First;
    
    Sprite_Movement_Calculate(pSprite);
    if( mSprite_Bullet_Destroy )
        goto SpriteDestroy;
    
    loc_19D3C:;
    if( pSprite->field_64 > 2 ) {
        Data0 = -9;
        Data4 = 0;
        if(Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
            goto loc_19DCF;
    }

    if( !Sprite_Projectile_Collision_Check( pSprite ) )
        return;
    
    if(mSprites_Found_Count == 1 )
        goto loc_19DCF;
    
    pSprite->field_43 = 1;
    pSprite->field_2C = eSprite_Draw_Last;
    pSprite->field_0 -= 4;
    if(pSprite->field_0 < 0 )
        goto loc_19E50;
    
    pSprite->field_4 -= 3;
    if(pSprite->field_4 < 0 )
        goto loc_19E50;
    
    pSprite->field_8 = 0x96;
    pSprite->field_A = 0;
    return;
    
loc_19DB0:;
    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if( pSprite->field_A >= 4 )
        goto SpriteDestroy;
    
    return;
    
loc_19DCF:;
    al = pSprite->field_43;
    if(al)
        goto loc_19E1C;
    
    Sprite_Projectile_Collision_Check( pSprite );
    pSprite->field_43 = -1;
    pSprite->field_2C = eSprite_Draw_First;
    pSprite->field_0 -= 4;
    if(pSprite->field_0 < 0 )
        goto loc_19E50;
    
    pSprite->field_4 -= 3;
    if( pSprite->field_4 < 0 )
        goto loc_19E50;
    
    pSprite->field_A = 3;
    al = pSprite->field_43;
    
loc_19E1C:;
    if( al >= 0 )
        goto loc_19DB0;
    
    pSprite->field_12 = 0;
    pSprite->field_A += 1;
    if(pSprite->field_A < 8 )
        return;
    
SpriteDestroy:;
    
    byte_3A9D2[2] -= 1;
    Sprite_Destroy( pSprite );
    return;
    
loc_19E50:;
    pSprite->field_0 = 0;
    pSprite->field_4 = 0x1000;
    goto SpriteDestroy;
}

void cFodder::Sprite_Handle_Helicopter( sSprite* pSprite ) {
    word_3B4ED[0] = 0;
    int16 Data0 = tool_RandomGet() & 0x0E;
    int16 Data4, Data8, DataC, Data10, Data14, Data18, Data1C;
    sSprite* Data24 = 0, *Data2C = 0;

    if (mVersion->mVersion == eVersion::AmigaPlus) {
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
    } else {

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

    Sprite_Map_Sound_Play( Data0 );

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
    Sprite_Create_Smoke( pSprite, Data2C );
    Sprite_Movement_Calculate( pSprite );

    pSprite->field_1A = (((int64)pSprite->field_1A ) + 0x2000);
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

    Sprite_Destroy( pSprite + 2 );

    if (pSprite->field_22 == eSprite_PersonType_Human)
        Sprites_HumanVehicles_Remove(pSprite);

    if (mSprite_Helicopter_DestroyLight)
        Sprite_Destroy( pSprite + 3 );

    return;

loc_1A149:;
    pSprite->field_8 = 0x8B;
    if (pSprite->field_44)
        goto loc_1A217;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_26;
    DataC = pSprite->field_28;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
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
    Sprite_Handle_Helicopter_Terrain_Check( pSprite );
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
        Data0 = mMouseX + (mCamera_Adjust_Col >> 16);
        Data0 -= 0x10;

        Data4 = mMouseY + (mCamera_Adjust_Row >> 16);
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

void cFodder::Sprite_Handle_Explosion( sSprite* pSprite ) {
    if (pSprite->field_8 == 0x7C) {
        Sprite_Destroy_Wrapper_2( pSprite );
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
            Squad_Member_Sprite_Hit_In_Region( pSprite, X_Left, X_Right, Y_Top, Y_Bottom );
        else
            Sprite_Find_In_Region( pSprite, Data24, X_Left, X_Right, Y_Top, Y_Bottom );

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

        Sprite_Destroy_Wrapper( pSprite );
        return;

    loc_1AA63:;
        if (pSprite->field_20 && pSprite->field_A >= 2) {
            pSprite->field_8 = 0xC0;
            pSprite->field_A = 0;
        }
        if (pSprite->field_A >= 7)
            Sprite_Destroy_Wrapper( pSprite );

        return;
    }

    //loc_1AA9F
    if (pSprite->field_5C) {
        Sprite_Handle_Explosion_MapTiles( pSprite );
        int16 Data0 = tool_RandomGet() & 0x1FE;
        int16 Data18 = Data0;
        Sprite_Create_Sparks( pSprite, Data18 );
        Data18 += 0x100;
        Data18 &= 0x1FE;
        Sprite_Create_Sparks( pSprite, Data18 );
    }
    //loc_1AAFD
    pSprite->field_0 -= 6;
    if (pSprite->field_0 < 0) {
        Sprite_Destroy_Wrapper_At_TopLeft( pSprite );
        return;
    }

    //loc_1AB0D
    pSprite->field_4 += 4;
    if (pSprite->field_4 < 0) {
        Sprite_Destroy_Wrapper_At_TopLeft( pSprite );
        return;
    }

    int16 Data0 = pSprite->field_26;
    if (Data0 < 0) {
        Data0 &= 0x7FFF;
        pSprite->field_0 += Data0;
        if (pSprite->field_0 < 0) {
            Sprite_Destroy_Wrapper_At_TopLeft( pSprite );
            return;
        }
        pSprite->field_26 = 0;
    }
    //loc_1AB6F
    int16 Data4 = mGame_InputTicks;
    Data4 &= 3;
    Data4 += 5;

    Sound_Play( pSprite, Data4, 0x1E );
    pSprite->field_8 = 0x8E;
    pSprite->field_A = 0;
    pSprite->field_12 = 1;
    pSprite->field_52 = 0;
    pSprite->field_22 = eSprite_PersonType_None;
    pSprite->field_32 = -1;
    pSprite->field_2C = eSprite_Draw_First;
}

void cFodder::Sprite_Handle_Shrub( sSprite* pSprite ) {
    pSprite->field_8 = 0x8F;
    Sprite_Handle_Exploidable( pSprite );
}

void cFodder::Sprite_Handle_Tree( sSprite* pSprite ) {
    pSprite->field_8 = 0x90;
    Sprite_Handle_Exploidable( pSprite );
}

void cFodder::Sprite_Handle_BuildingRoof( sSprite* pSprite ) {
    pSprite->field_8 = 0x91;
    Sprite_Handle_Exploidable( pSprite );
}

void cFodder::Sprite_Handle_Snowman( sSprite* pSprite ) {
    pSprite->field_8 = 0x92;
    Sprite_Handle_Exploidable( pSprite );
}

void cFodder::Sprite_Handle_Shrub2( sSprite* pSprite ) {
    pSprite->field_8 = 0x93;
    Sprite_Handle_Exploidable( pSprite );
}

void cFodder::Sprite_Handle_Waterfall( sSprite* pSprite ) {
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

void cFodder::Sprite_Handle_Bird2_Left( sSprite* pSprite ) {

    pSprite->field_8 = 0x98;
    pSprite->field_2C = eSprite_Draw_Last;
    pSprite->field_12 += 1;
    pSprite->field_12 &= 1;
    if (!pSprite->field_12)
        pSprite->field_A ^= 1;

    pSprite->field_0 -= 2;
    if (pSprite->field_0 < -64)
        pSprite->field_0 = 0x7D0;
}

void cFodder::Sprite_Handle_BuildingDoor( sSprite* pSprite ) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (Sprite_Handle_BuildingDoor_Explode( pSprite ))
        return;

    if (mTroops_Enemy_Count >= 0x0A) {
        pSprite->field_8 = 0x99;
        return;
    }

    //loc_1ACEC
    pSprite->field_2C = eSprite_Draw_First;
    if (pSprite->field_43 < 0)
        goto loc_1AD86;

    pSprite->field_43 -= 1;
    if (pSprite->field_43 == 0x14 || pSprite->field_43 == 0x0A) {

        Sprite_Create_Enemy(pSprite, Data2C );
        return;
    }

    if (pSprite->field_43 >= 0) {
        pSprite->field_8 = 0x7C;
        return;
    }

    pSprite->field_8 = 0x99;
    Sound_Play( pSprite, eSound_Effect_BuildingDoor2, 0x01 );
    Data0 = tool_RandomGet() & 0x0F;

    Data4 = 0x14 - mSprite_Enemy_AggressionMax;
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

void cFodder::Sprite_Handle_Player_Rank( sSprite* pSprite ) {
    sSprite* Data24 = mSquad_Leader;
    int16 Data4, Data0;
    
    if (mSquad_Selected < 0)
        goto loc_1AF63;

    if (Data24 == INVALID_SPRITE_PTR)
        goto loc_1AF63;

    if (!mMission_Completed_Timer) {
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
    sub_305D5( Data24 );
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

void cFodder::Sprite_Handle_Player_Shadow( sSprite* pSprite ) {
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
    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_BloodTrail( sSprite* pSprite ) {
    
    if (pSprite->field_20)
        --pSprite->field_20;

    --pSprite->field_12;
    if (pSprite->field_12 >= 0)
        return;

    pSprite->field_12 = 1;
    pSprite->field_A += 1;
    if (pSprite->field_A < 4)
        return;

    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_GroundHole( sSprite* pSprite ) {
    if (!mEnemy_BuildingCount)
        return;

    if (pSprite->field_2A) {
        //loc_1B194
        sSprite* Data2C = 0;

        if (!Sprite_Create_Enemy( pSprite, Data2C )) {
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
    Data4 -= mSprite_Enemy_AggressionAverage;
    Data0 += 0x0A;
    Data4 += mSprite_Enemy_AggressionAverage;

    pSprite->field_12 = Data0;
    return;
}

void cFodder::Sprite_Handle_BuildingDoor2( sSprite* pSprite ) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (sub_222A3(pSprite))
        return;

    if (mTroops_Enemy_Count >= 0x0A) {
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

    Sound_Play( pSprite, eSound_Effect_BuildingDoor2, 1 );
    Data0 = tool_RandomGet() & 0x0F;
    Data4 = 0x14;

    Data4 -= mSprite_Enemy_AggressionMax;
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
    
    if (!Sprite_Create_Enemy( pSprite, Data2C )) {
        Data2C->field_4 -= 4;
        return;
    }

    pSprite->field_12 = 0;
    pSprite->field_43 = -1;
}

void cFodder::Sprite_Handle_Floating_Dead_Soldier( sSprite* pSprite ) {
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

    Sprite_XY_Store( pSprite );
    Sprite_Movement_Calculate( pSprite );
    Data0 = -3;
    Data4 = 8;
    Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
    if (Data4 < eTerrainType_QuickSand || Data4 > eTerrainType_Water) {
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

void cFodder::Sprite_Handle_Text_Complete( sSprite* pSprite ) {
    pSprite->field_0 = dword_39F84 >> 16;
    pSprite->field_0 += 0x47;
    pSprite->field_4 -= 0x20;

    int16 Data0 = dword_39F88 >> 16;
    Data0 += 0x77;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}
    
void cFodder::Sprite_Handle_Text_Mission( sSprite* pSprite ) {
    pSprite->field_0 = dword_39F84 >> 16;
    pSprite->field_0 += 0x56;
    pSprite->field_4 -= 0x20;

    int16 Data0 = dword_39F88 >> 16;
    Data0 += 0x63;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Phase( sSprite* pSprite ) {
    pSprite->field_0 = dword_39F84 >> 16;
    pSprite->field_0 += 0x5F;
    pSprite->field_4 -= 0x20;

    int16 Data0 = dword_39F88 >> 16;
    Data0 += 0x63;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Vehicle( sSprite* pSprite ) {
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

    Sprite_Map_Sound_Play( Data0 );

loc_1B523:;
    pSprite->field_65 = -1;
    if (Sprite_Animation_SlideOrDie(pSprite))
        return;

    pSprite->field_8 = 0xA5;
    sub_22C87( pSprite );
    if (pSprite->field_20 < 3) {
        word_3B2F7 = 0x48;
        Data0 = pSprite->field_26;
        Data4 = pSprite->field_28;

        sub_22CD7( pSprite, Data0, Data4 );
    }

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_4;
    
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
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

        if (!Sprite_Create_Smoke( pSprite, Data2C))
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

        } else {
            if (pSprite->field_36 >= 0x32)
                pSprite->field_36 = 0x32;
        }
            
    }
loc_1B655:;
    // VEhicle in the air, increase speed
    if (pSprite->field_20 > 4)
        pSprite->field_36 += 0x1C;

    Sprite_XY_Store( pSprite );
    Sprite_Movement_Calculate( pSprite );

    if (pSprite->field_20 < 9) {
        Sprite_Handle_Vehicle_Terrain_Check(pSprite);

        if (pSprite->field_20 <= 4)
            pSprite->field_20 = 0;
    } else {
        pSprite->field_36 += 0x40;
    }

    sub_243E9( pSprite );

    Data8 = pSprite->field_0;
    DataC = pSprite->field_0;
    DataC += 0x1C;
    int16 Data10 = pSprite->field_4;
    Data10 -= 0x10;
    int16 Data14 = pSprite->field_4;

    int16 Data18 = pSprite->field_20;
    int16 Data1C = Data18;
    Data1C += 0x0E;
    Sprite_Under_Vehicle( pSprite, Data8, DataC, Data10, Data14, Data18, Data1C );

}

void cFodder::Sprite_Handle_Rocket( sSprite* pSprite ) {
    sSprite* Data24 = 0;

    if (pSprite->field_56) {

        pSprite->field_56 -= 1;
        if (!pSprite->field_56) {
            if (mVersion->mVersion == eVersion::AmigaPlus)
                Sound_Play( pSprite, 0x10, 0x0F );
            else
                Sound_Play( pSprite, eSound_Effect_Rocket, 0x0F );

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

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 <= 7)
        goto loc_1B843;

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;
    Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
    Sprite_Movement_Calculate( pSprite );

    Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_A = Data0;

    Data0 = -9;
    Data4 = 2;
    
    if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
        return;

loc_1B843:;
    // Hit Target
    pSprite->field_18 = eSprite_Explosion;
    Sprite_Projectile_HitTarget( pSprite );
    Data24 = pSprite + 1;

    Sprite_Destroy( Data24 );
}

void cFodder::Sprite_Handle_Text_GameOver( sSprite* pSprite ) {

    pSprite->field_0 = dword_39F84 >> 16;
    pSprite->field_0 += 0x5B;
    pSprite->field_4 -= 0x20;

    int16 Data0 = dword_39F88 >> 16;
    Data0 += 0x77;
    
    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Shadow( sSprite* pSprite ) {
    sSprite* Data28 = pSprite->field_1A_sprite;

    if (Data28->field_18 == eSprite_Explosion) {
        Sprite_Destroy_Wrapper( pSprite );
        return;
    }

    if (Data28->field_52) {
        Sprite_Destroy_Wrapper( pSprite );
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

void cFodder::Sprite_Handle_Enemy_Rocket( sSprite* pSprite ) {

    if (Sprite_Handle_Soldier_Animation( pSprite))
        return;

    if (pSprite->field_38 != eSprite_Anim_None)
        return;

    mSprite_FaceWeaponTarget = 0;
    pSprite->field_43 = -1;

    sub_21CD1( pSprite );

    mTroop_Weapon_Bullet_Disabled = true;
    mTroop_Weapon_Grenade_Disabled = true;
    mTroop_Weapon_Rocket_Disabled = true;

    Sprite_Handle_Troop_Weapon( pSprite );
    sub_2A3D4( pSprite );

    mSprite_FaceWeaponTarget = 0;
    word_3ABB1 = 0;

    pSprite->field_54 = 3;
    pSprite->field_5A = -1;
    pSprite->field_55 = 0;
    pSprite->field_A = 0;

    sub_1FDE7( pSprite );
    pSprite->field_A = 2;
}

void cFodder::Sprite_Handle_GrenadeBox( sSprite* pSprite ) {
    
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xC2;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
        return;

    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Grenades[mSquad_Selected] += 4;
    
    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_RocketBox( sSprite* pSprite ) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xC3;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
        return;

    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] += 4;

    // Plus uses homing missiles
    if (mVersion->mVersion == eVersion::AmigaPlus)
        mSquad_Leader->field_75 |= eSprite_Flag_HomingMissiles;

    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Building_Explosion( sSprite* pSprite ) {
    int16 Data0, Data4, Data6, Data8, DataC, Data10, Data14;
    const int16* Data2C = mSprite_Explosion_Area_PerFrame;
    sSprite* Data24 = 0;

    if (pSprite->field_8 == 0x7C) {
        Sprite_Destroy_Wrapper_2( pSprite );
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

    Squad_Member_Sprite_Hit_In_Region( pSprite, Data8, DataC, Data10, Data14 );
    goto loc_1BC48;

loc_1BC07:;
    Data8 = pSprite->field_0;
    DataC = pSprite->field_0 + 0x1E;
    Data10 = pSprite->field_4 - 30;
    Data14 = pSprite->field_4;

    Sprite_Find_In_Region( pSprite, Data24, Data8, DataC, Data10, Data14 );
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

        Sprite_Destroy_Wrapper( pSprite );
        return;
    }

    if (!pSprite->field_20 || pSprite->field_A < 2)
        goto loc_1BD06;

    pSprite->field_8 = 0xC0;
    pSprite->field_A = 0;
loc_1BD06:;

    if (pSprite->field_A < 3)
        return;

    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Grenade_Enemy( sSprite* pSprite ) {

    pSprite->field_6F = eVehicle_Helicopter_Grenade;
    Sprite_Handle_Helicopter_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Flashing_Light( sSprite* pSprite ) {
    
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

void cFodder::Sprite_Handle_Helicopter_Grenade2_Enemy( sSprite* pSprite ) {

    pSprite->field_6F = eVehicle_Helicopter;
    Sprite_Handle_Helicopter_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Missile_Enemy( sSprite* pSprite ) {
    
    pSprite->field_6F = eVehicle_Helicopter_Missile;
    Sprite_Handle_Helicopter_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Homing_Enemy( sSprite* pSprite ) {
    
    pSprite->field_6F = eVehicle_Helicopter_Homing;
    Sprite_Handle_Helicopter_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Missile( sSprite* pSprite ) {
    
    dword_3B24B = -1;
    Sprite_Create_FireTrail( pSprite );

    pSprite->field_8 = 0xA3;
    int16 Data0 = pSprite->field_26;
    int16 Data4 = pSprite->field_28;
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;
    int16 Data10 = 0x10;

    Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
    if (Data0 <= 7)
        goto loc_1BECD;

    if (pSprite->field_20 > 4) {
        pSprite->field_1E_Big -= 0xA000;
    }

    Data0 = pSprite->field_26;
    Data4 = pSprite->field_28;

    Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
    Sprite_Movement_Calculate( pSprite );

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
    if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
        return;

loc_1BECD:;

    pSprite->field_18 = eSprite_Explosion2;
    pSprite->field_4 -= 4;
    Sprite_Projectile_HitTarget( pSprite );

    Sprite_Destroy( pSprite + 1 );
}

void cFodder::Sprite_Handle_MissileHoming( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data34 = 0;

    if (pSprite->field_38)
        goto MissileExplode;

    dword_3B24B = -1;
    Sprite_Create_FireTrail( pSprite );

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
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    
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

            if (!sub_2A4A2(Data0, Data4, Data8, DataC)) {
                if (pSprite->field_20 > 8)
                    pSprite->field_1E_Big -= 0x12000;
            } else {

                if (pSprite->field_20 < 0x18)
                    pSprite->field_1E_Big += 0x28000;
            }

        } else {
            // Near Target
            Data4 = Data34->field_20;
            Data4 -= pSprite->field_20;
            Data4 >>= 3;
            pSprite->field_20 += Data4;
        }
    } else {

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
    Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
    Sprite_Movement_Calculate( pSprite );

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
        } else {
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
    Sprite_Projectile_HitTarget( pSprite );
    Sprite_Destroy( pSprite + 1 );

}

void cFodder::Sprite_Handle_Sparks( sSprite* pSprite ) {
    Sprite_Movement_Calculate( pSprite );
    int32 Data0 = (int32) pSprite->field_1A;

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
        Sprite_Destroy_Wrapper( pSprite );
        return;
    }

loc_1C262:;
    Sprite_Create_FireTrail( pSprite );
}

void cFodder::Sprite_Handle_FireTrail( sSprite* pSprite ) {
    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (pSprite->field_2A)
        return;

    pSprite->field_A += 1;
    if (pSprite->field_A != 4)
        return;

    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Grenade_Human( sSprite* pSprite ) {

    pSprite->field_6F = eVehicle_Helicopter;
    Sprite_Handle_Helicopter_Human( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Grenade2_Human( sSprite* pSprite ) {

    pSprite->field_6F = eVehicle_Helicopter_Grenade;
    Sprite_Handle_Helicopter_Human( pSprite );
}


void cFodder::Sprite_Handle_Helicopter_Missile_Human( sSprite* pSprite ) {

    pSprite->field_6F = eVehicle_Helicopter_Missile;
    Sprite_Handle_Helicopter_Human( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Homing_Human( sSprite* pSprite ) {

    pSprite->field_6F = eVehicle_Helicopter_Homing;
    Sprite_Handle_Helicopter_Human( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_PropCrash( sSprite* pSprite ) {
    pSprite->field_A += 1;
    pSprite->field_A &= 3;

    Sprite_Movement_Calculate( pSprite );
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
    Sprite_Create_FireTrail( pSprite );
}

void cFodder::Sprite_Handle_Mine( sSprite* pSprite ) {
    pSprite->field_2C = eSprite_Draw_First;

    if (!pSprite->field_38) {
        int16 Data0;
        pSprite->field_8 = 0xC7;
        Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 );
    }
    else {
        pSprite->field_18 = eSprite_Explosion2;
    }
}

void cFodder::Sprite_Handle_Mine2( sSprite* pSprite ) {
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
    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
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

void cFodder::Sprite_Handle_Spike( sSprite* pSprite ) {
    sSprite* Data28 = mSquad_Leader;
    int16 Data0 = 0;

    pSprite->field_8 = 0xC9;

    if (pSprite->field_A)
        goto loc_1C4AD;

    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
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

void cFodder::Sprite_Handle_Smoke( sSprite* pSprite ) {
    pSprite->field_2A += 1;
    pSprite->field_2A &= 1;
    if (pSprite->field_2A)
        return;

    pSprite->field_A -= 1;
    if (pSprite->field_A >= 0)
        return;

    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Text_Try( sSprite* pSprite ) {
    pSprite->field_0 = dword_39F84 >> 16;
    pSprite->field_0 += 0x71;
    pSprite->field_4 -= 0x20;

    int16 Data0 = dword_39F88 >> 16;
    Data0 += 0x63;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Again( sSprite* pSprite ) {
    pSprite->field_0 = dword_39F84 >> 16;
    pSprite->field_0 += 0x57;
    pSprite->field_4 -= 0x20;

    int16 Data0 = dword_39F88 >> 16;
    Data0 += 0x75;

    if (Data0 < pSprite->field_4)
        return;

    pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_BoilingPot( sSprite* pSprite ) {
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

void cFodder::Sprite_Handle_Indigenous( sSprite* pSprite ) {
    if (pSprite->field_38) {
        Sprite_Handle_Indigenous_Death( pSprite );
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    int16 ax = Sprite_Handle_Indigenous_Within_Range_OpenCloseDoor( pSprite );

    if (ax) {
        if (ax >= 0)
            return;

        Sprite_Handle_Indigenous_Movement( pSprite );
    }

    pSprite->field_36 = 6;
    sub_2593D( pSprite );
    sub_25A31( pSprite );
    sub_25A66( pSprite );
}

void cFodder::Sprite_Handle_Indigenous2( sSprite* pSprite ) {
    
    if (pSprite->field_38) {
        Sprite_Handle_Indigenous_Death( pSprite );
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    int16 ax = Sprite_Handle_Indigenous_Within_Range_OpenCloseDoor( pSprite );
    if (ax > 0)
        return;
    if (ax < 0)
        Sprite_Handle_Indigenous_Movement( pSprite );

    pSprite->field_36 = 0x0A;
    sub_2593D( pSprite );
    sub_25A31( pSprite );
    sub_25A66( pSprite );
    if (!word_3B2D1[1])
        return;

    Sprite_Handle_Indigenous_Unk( pSprite );
}

void cFodder::Sprite_Handle_VehicleNoGun_Human( sSprite* pSprite ) {

    pSprite->field_6F = eVehicle_Jeep;
    Sprite_Handle_Vehicle_Human( pSprite );
}

void cFodder::Sprite_Handle_VehicleGun_Human( sSprite* pSprite ) {

    pSprite->field_6F = eVehicle_JeepRocket;
    Sprite_Handle_Vehicle_Human( pSprite );
}

void cFodder::Sprite_Handle_Tank_Human( sSprite* pSprite ) {
    
    if (pSprite->field_38 == eSprite_Anim_Die1)
        pSprite->field_38 = eSprite_Anim_None;

    if (pSprite->field_38) {
        pSprite->field_22 = eSprite_PersonType_Human;

        sub_23525( pSprite );
        pSprite->field_22 = eSprite_PersonType_Human;
        return;
    }
    pSprite->field_22 = eSprite_PersonType_Human;
    sub_23525( pSprite );
    pSprite->field_22 = eSprite_PersonType_Human;
    Sprite_Handle_Tank_FireMissile( pSprite );

    sSprite* Data24 = pSprite + 1;
    Data24->field_18 = eSprite_Null;
}

void cFodder::Sprite_Handle_Bird_Left( sSprite* pSprite ) {
    
    pSprite->field_8 = 0xD3;
    pSprite->field_2C = eSprite_Draw_Last;

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
    Data0 = tool_RandomGet() & 0x3F;

    Data0 += dword_39F84 >> 16;
    Data0 += 0x140;
    pSprite->field_0 = Data0;

    Data0 = tool_RandomGet() & 0xFF;
    Data0 += dword_39F88 >> 16;
    pSprite->field_4 = Data0;

loc_1C8C5:;
    
    if (mMap_TileSet == eTileTypes_Jungle)
        Sprite_Native_Sound_Play(pSprite, 0x1A);

    if (mMap_TileSet == eTileTypes_Ice || mMap_TileSet == eTileTypes_AFX)
        Sprite_Native_Sound_Play(pSprite, 0x1F);

}

void cFodder::Sprite_Handle_Bird_Right( sSprite* pSprite ) {
    pSprite->field_8 = 0xD4;
    pSprite->field_2C = eSprite_Draw_Last;

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
    Data0 += dword_39F84 >> 16;

    pSprite->field_0 = Data0;
    Data0 = tool_RandomGet() & 0xFF;
    Data0 += dword_39F88 >> 16;
    pSprite->field_4 = Data0;

loc_1CA20:;
    if (mMap_TileSet == eTileTypes_Jungle)
        Sprite_Native_Sound_Play(pSprite, 0x1A);

    if (mMap_TileSet == eTileTypes_Ice || mMap_TileSet == eTileTypes_AFX)
        Sprite_Native_Sound_Play(pSprite, 0x1F);
}

void cFodder::Sprite_Handle_Seal( sSprite* pSprite ) {
    if (sub_25DCF( pSprite ))
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

    if (mMap_TileSet == eTileTypes_Moors) {
        Data0 = tool_RandomGet() & 3;
        if (Data0 != 3) {
            Data0 += 0x23;
            Sprite_Native_Sound_Play( pSprite, Data0 );
        }
    }

    if (mMap_TileSet == eTileTypes_Ice || mMap_TileSet == eTileTypes_AFX) {
        Sprite_Native_Sound_Play( pSprite, 0x1E );
    }
}

void cFodder::Sprite_Handle_Tank_Enemy( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC, Data10, Data14;
    sSprite* Data24 = 0, *Data30 = 0;

    if (pSprite->field_38 == eSprite_Anim_Die1)
        pSprite->field_38 = eSprite_Anim_None;

    if (pSprite->field_38) {
        pSprite->field_22 = eSprite_PersonType_AI;
        sub_23525( pSprite );
        pSprite->field_22 = eSprite_PersonType_AI;

        Data24 = pSprite + 2;
        if (Data24->field_18 == eSprite_Flashing_Light)
            Sprite_Destroy( Data24 );

        return;
    }

    if (pSprite->field_4C)
        pSprite->field_4C--;

    int16 Data1C = pSprite->field_5E;
    if (mSquad_0_Sprites[Data1C] == INVALID_SPRITE_PTR)
        goto loc_1CD7B;
    
    Data30 = mSquad_0_Sprites[Data1C];
    if (Data30->field_52)
        goto loc_1CD7B;

    Data8 = Data30->field_0;
    Data8 += 8;

    DataC = Data30->field_4;
    DataC += -5;

    if (Map_Terrain_Get_Moveable_Wrapper( mTiles_NotDriveable, Data8, DataC, Data10, Data14 ))
        goto loc_1CD7B;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = Data30->field_0;
    DataC = Data30->field_4;

    mSprite_Tank_SpriteX = Data0;
    mSprite_Tank_SpriteY = Data4;
    mSprite_Tank_Squad0_X = Data8;
    mSprite_Tank_DistanceTo_Squad0 = DataC;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    mSprite_DistanceTo_Squad0 = Data0;
    if (Data0 >= 250)
        goto loc_1CD7B;

    Data0 = mSprite_Tank_SpriteX;
    Data4 = mSprite_Tank_SpriteY;
    Data8 = mSprite_Tank_Squad0_X;
    DataC = mSprite_Tank_DistanceTo_Squad0;
    Data8 += 0x0F;
    DataC -= 0x0A;

    if (sub_2A4A2( Data0, Data4, Data8, DataC ))
        goto loc_1CD7B;
    
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
    Sprite_Handle_Tank_FireMissile( pSprite );
    goto loc_1CDA3;

loc_1CD7B:;
    pSprite->field_5E += 1;
    if (pSprite->field_5E >= 0x1E) {
        pSprite->field_5E = 0;
        pSprite->field_2E = -1;
    }

loc_1CDA3:;
    mSprite_Helicopter_DestroyLight = -1;
    pSprite->field_22 = eSprite_PersonType_AI;
    sub_23525( pSprite );
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

void cFodder::Sprite_Handle_Indigenous_Spear( sSprite* pSprite ) {
    pSprite->field_22 = eSprite_PersonType_AI;
    if (pSprite->field_38) {
        Sprite_Handle_Indigenous_Death( pSprite );
        return;
    }

    pSprite->field_22 = eSprite_PersonType_Native;
    pSprite->field_8 = 0xD0;

    if (mMap_TileSet == eTileTypes_Moors)
        Sprite_Native_Sound_Play( pSprite, 0x26 );
    
    if (mMap_TileSet == eTileTypes_Int)
        Sprite_Native_Sound_Play( pSprite, 0x1F );

    int16 ax = Sprite_Handle_Indigenous_Within_Range_OpenCloseDoor( pSprite );
    if (ax > 0)
        return;
    if (ax)
        Sprite_Handle_Indigenous_Movement( pSprite );

    pSprite->field_36 = 0x0C;

    sub_2593D( pSprite );
    sub_25A31( pSprite );
    sub_25A66( pSprite );
    Sprite_Handle_Indigenous_Unk( pSprite );
}

void cFodder::Sprite_Handle_Indigenous_Spear2( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC, Data1C;

    ++pSprite->field_64;

    if (!pSprite->field_12)
        goto loc_1D0F6;

    pSprite->field_12 -= 1;
    pSprite->field_3A += 1;

    if (pSprite->field_44)
        goto loc_1CFF7;

    Sprite_XY_Store( pSprite );
    Data1C = pSprite->field_10;

    if (pSprite->field_59)
        goto loc_1CF3E;

    Data0 = pSprite->field_2E;
    Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
    Data0 = pSprite->field_50;

    if (Data0) {
        pSprite->field_10 += Data0;
        pSprite->field_10 &= 0x1FE;
        pSprite->field_59 = -1;
    }

    if (pSprite->field_34 < 0)
        pSprite->field_34 = pSprite->field_10;

loc_1CF3E:;
    Sprite_Movement_Calculate( pSprite );
    if (mSprite_Bullet_Destroy)
        goto loc_1D17A;

    pSprite->field_36 = pSprite->field_4A;
    if (pSprite->field_59)
        goto loc_1D00F;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
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
    Sprite_Movement_Calculate( pSprite );
    if (mSprite_Bullet_Destroy)
        goto loc_1D17A; 

loc_1D00F:;
    if (pSprite->field_64 > 2) {
        Data0 = -9;
        Data4 = 0;
        if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
            goto loc_1D0F6;
    }

    if (Sprite_Projectile_Collision_Check( pSprite ))
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
    pSprite->field_2C = eSprite_Draw_Last;
    pSprite->field_4 -= 5;
    if (pSprite->field_4 < 0)
        goto loc_1D18D;
    pSprite->field_8 = 0x96;
    pSprite->field_A = 0;

    Sound_Play( pSprite, eSound_Effect_Spear, 0x0F );
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
    Sound_Play( pSprite, eSound_Effect_Spear, 0x0F );
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
    writeLEWord( &byte_3A9D2[2], readLEWord( &byte_3A9D2[2] - 1 ));
    Sprite_Destroy( pSprite );
    return;

loc_1D18D:;
    pSprite->field_0 = 0;
    pSprite->field_4 = 0x1000;
    goto loc_1D17A;
}

void cFodder::Sprite_Handle_Hostage( sSprite* pSprite ) {
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

    Sprite_Handle_Hostage_Movement( pSprite );
    Sprite_Handle_Indigenous_Movement( pSprite );

    pSprite->field_36 = 0x0C;
    sub_2593D( pSprite );
    pSprite->field_38 = eSprite_Anim_None;

    Sprite_Handle_Hostage_FrameUpdate2( pSprite );
    //seg004:44B6

    Sprite_Field_10 = pSprite->field_10;
    pSprite->field_10 += 0x20;
    pSprite->field_10 &= 0x1C0;

    Sprite_Handle_Hostage_FrameUpdate( pSprite );
    pSprite->field_10 = Sprite_Field_10;
    sub_25F2B( pSprite );

    if (pSprite->field_18 != eSprite_Hostage_2)
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
    if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
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
    Data0 = mMapHeight_Pixels;

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

void cFodder::Sprite_Handle_Hostage_Rescue_Tent( sSprite* pSprite ) {
    pSprite->field_8 = 0xDD;

    mHostage_Rescue_Tent = pSprite;
    if (pSprite->field_38 == eSprite_Anim_Die3)
        pSprite->field_18 = eSprite_Explosion2;
}

void cFodder::Sprite_Handle_Door_Indigenous( sSprite* pSprite ) {
    mSpawnSpriteType = 0x3D;

    sub_264B0(pSprite);
}

void cFodder::Sprite_Handle_Door2_Indigenous( sSprite* pSprite ) {
    mSpawnSpriteType = 0x3E;

    sub_264B0(pSprite);
}

void cFodder::Sprite_Handle_Door_Indigenous_Spear( sSprite* pSprite ) {
    mSpawnSpriteType = 0x46;

    sub_264B0(pSprite);
}

void cFodder::Sprite_Handle_Cannon( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC, Data1C;
    ++pSprite->field_64;

    if (!pSprite->field_12)
        goto loc_1D65C;

    pSprite->field_12 -= 1;
    pSprite->field_3A++;

    if (pSprite->field_44)
        goto loc_1D5B3;

    Sprite_XY_Store( pSprite );
    Data1C = pSprite->field_10;

    Sprite_Movement_Calculate( pSprite );

    if (mSprite_Bullet_Destroy)
        goto loc_1D6CA;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );

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
    Sprite_Movement_Calculate( pSprite );

    if (mSprite_Bullet_Destroy)
        goto loc_1D6CA;

loc_1D5CB:;
    if (pSprite->field_64 > 3) {
        Data0 = -9;
        Data4 = 0;
        if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
            goto loc_1D65C;
    }

    if (!Sprite_Projectile_Collision_Check( pSprite ))
        return;

    if (mSprites_Found_Count == 1)
        goto loc_1D65C;

    pSprite->field_43 = 1;
    pSprite->field_2C = eSprite_Draw_Last;
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

    Sprite_Projectile_Collision_Check( pSprite );
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

    byte_3A9D2[2] -= 1;
    Sprite_Destroy( pSprite );
    return;

loc_1D6DD:;
    pSprite->field_0 = 0;
    pSprite->field_4 = 0x1000;
    goto loc_1D6CA;

}

void cFodder::Sprite_Handle_VehicleNoGun_Enemy( sSprite* pSprite ) {
    pSprite->field_6F = eVehicle_Jeep;
    Sprite_Handle_Vehicle_Enemy( pSprite );
}

void cFodder::Sprite_Handle_VehicleGun_Enemy( sSprite* pSprite ) {
    pSprite->field_6F = eVehicle_JeepRocket;
    Sprite_Handle_Vehicle_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Vehicle_Unk_Enemy( sSprite* pSprite ) {
    pSprite->field_6F = eVehicle_Unknown;
    pSprite->field_26 = pSprite->field_0;
    pSprite->field_26 += 0x28;

    Sprite_Handle_Vehicle_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Indigenous_Invisible( sSprite* pSprite ) {

    if (!pSprite->field_38)
        return;

    Sprite_Handle_Indigenous_Death( pSprite );

    /* Unused code block

     seg004:4A78 C4 36 20 00                       les     si, ds:20h
    seg004:4A7C 26 C7 44 22 02 00                 mov     word ptr es:[si+22h], 2
    seg004:4A82 C4 36 20 00                       les     si, ds:20h
    seg004:4A86 26 C7 44 08 D0 00                 mov     word ptr es:[si+8], 0D0h ; 'ð'
    */
}

void cFodder::Sprite_Handle_Turret_Missile_Enemy( sSprite* pSprite ) {
    pSprite->field_22 = eSprite_PersonType_AI;
    pSprite->field_6F = eVehicle_Turret_Cannon;

    Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Turret_Missile2_Enemy( sSprite* pSprite ) {
    pSprite->field_22 = eSprite_PersonType_AI;
    pSprite->field_6F = eVehicle_Turret_Missile;

    Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Turret_HomingMissile_Enemy( sSprite* pSprite ) {
    pSprite->field_22 = eSprite_PersonType_AI;
    pSprite->field_6F = eVehicle_Turret_Homing;

    Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Turret_Missile_Human( sSprite* pSprite ) {
    pSprite->field_22 = eSprite_PersonType_Human;
    pSprite->field_6F = eVehicle_Turret_Cannon;

    Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Turret_Missile2_Human( sSprite* pSprite ) {
    pSprite->field_22 = eSprite_PersonType_Human;
    pSprite->field_6F = eVehicle_Turret_Missile;

    Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Vehicle_Sinking_1( sSprite* pSprite ) {
    Sound_Play( pSprite, eSound_Effect_Vehicle_Sinking, 0x0F );
    pSprite->field_A -= 1;

    if (pSprite->field_A < 0)
        Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Vehicle_Sinking_2( sSprite* pSprite ) {
    pSprite->field_A += 1;

    if (pSprite->field_A == 6)
        Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_BuildingDoor3( sSprite* pSprite ) {
    int16 Data0, Data4;
    sSprite* Data2C = 0;

    if (sub_1D92E( pSprite ))
        return;

    if (mTroops_Enemy_Count >= 0x0A) {
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
    Sound_Play( pSprite, eSound_Effect_BuildingDoor2, 1 );

    Data0 = tool_RandomGet() & 0x0F;
    Data4 = 0x14 - mSprite_Enemy_AggressionMax;

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

    if (!Sprite_Create_Enemy( pSprite, Data2C ))
        return;

    pSprite->field_12 = -1;
    pSprite->field_43 = -1;
    return;

loc_1D928:;
    Sprite_Create_Enemy( pSprite, Data2C );
}

void cFodder::Sprite_Handle_Explosion2( sSprite* pSprite ) {

    Sprite_Handle_Explosion( pSprite );
}

void cFodder::Sprite_Handle_OpenCloseDoor( sSprite* pSprite ) {
    mSprite_OpenCloseDoor_Ptr = pSprite;

    if (sub_222A3( pSprite )) {
        mSprite_OpenCloseDoor_Ptr = 0;
        mMission_Aborted = true;
        return;
    }
    pSprite->field_2C = eSprite_Draw_First;
    if (!pSprite->field_2A) {
        pSprite->field_8 = 0x9B;
        return;
    }

    pSprite->field_2A -= 1;
    if (!pSprite->field_2A) 
        Sound_Play( pSprite, eSound_Effect_DoorCloseDoor, 1 );
    
    if (pSprite->field_8 == 0x9B)
        Sound_Play( pSprite, eSound_Effect_DoorCloseDoor, 1 );

    pSprite->field_8 = 0x7C;
}

void cFodder::Sprite_Handle_Seal_Mine( sSprite* pSprite ) {
    Sprite_Handle_Seal( pSprite );

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

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 > 0x14)
        return;

    pSprite->field_38 = eSprite_Anim_Die4;
}

void cFodder::Sprite_Handle_Spider_Mine( sSprite* pSprite ) {
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
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 > 0x0A)
        return;

loc_1DC50:;
    pSprite->field_18 = eSprite_Explosion;
    pSprite->field_26 = -32763;
    pSprite->field_28 = -3;
}

void cFodder::Sprite_Handle_Bonus_RankToGeneral( sSprite* pSprite ) {
    int16 Data0;

    // Blow up?
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0x95;
    pSprite->field_A = 0x0F;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
        return;

    mSquad_Leader->field_46_mission_troop->mRank = 0x0F;
    mGUI_Sidebar_Setup = 0;

    Sprite_Destroy_Wrapper( pSprite );
}


void cFodder::Sprite_Handle_Bonus_Rockets( sSprite* pSprite ) {
    int16 Data0;

    // Blow up?
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0xE4;
    pSprite->field_A = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
        return;

    // Has Homing missiles
    mSquad_Leader->field_75 |= eSprite_Flag_HomingMissiles;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 50;

    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Bonus_Armour( sSprite* pSprite ) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    pSprite->field_8 = 0xE3;
    pSprite->field_A = 0;
    int16 Data0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
         return;
    
    mSquad_Leader->field_75 |= eSprite_Flag_Invincibility;

    Sprite_Destroy_Wrapper(pSprite);
}

void cFodder::Sprite_Handle_Bonus_RankHomingInvin_SquadLeader( sSprite* pSprite ) {
    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xE5;
    pSprite->field_A = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
        return;

    // Invincible + HomingMissiles
    mSquad_Leader->field_75 |= (eSprite_Flag_HomingMissiles | eSprite_Flag_Invincibility);
    mSquad_Leader->field_46_mission_troop->mRank = 0x0F;
    mGUI_Sidebar_Setup = 0;
    
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 50;

    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_MissileHoming2( sSprite* pSprite ) {

    Sprite_Handle_MissileHoming( pSprite );
}

void cFodder::Sprite_Handle_Bonus_RankHomingInvin_Squad( sSprite* pSprite ) {

    if (pSprite->field_38) {
        pSprite->field_18 = eSprite_Explosion;
        return;
    }

    int16 Data0 = 0;
    pSprite->field_8 = 0xE6;
    pSprite->field_A = 0;
    if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
        return;

    if (mSquad_Selected < 0)
        return;

    sSprite** Data28 = mSquads[mSquad_Selected];
    for (;*Data28 != INVALID_SPRITE_PTR;) {
        
        sSprite* Data2C = *Data28++;
        Data2C->field_75 |= (eSprite_Flag_HomingMissiles | eSprite_Flag_Invincibility);

        Data2C->field_46_mission_troop->mRank = 0x0F;
    }

    mGUI_Sidebar_Setup = 0;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mSquad_Rockets[mSquad_Selected] = 0x32;

    Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_CallPad( sSprite* pSprite ) {
    sSprite* Data2C = 0;

    pSprite->field_8 = 0xE7;
    pSprite->field_2C = eSprite_Draw_First;

    // Find a troop inrange 
    if (Sprite_Handle_Helicopter_Callpad_InRange( pSprite, Data2C )) {
        pSprite->field_75 = 0;
    } else {
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

void cFodder::Sprite_Handle_BuildingDoor_Reinforced( sSprite* pSprite ) {
    if (pSprite->field_38 != eSprite_Anim_Die3) {
        Sprite_Handle_BuildingDoor3( pSprite );
        return;
    }

    if (pSprite->field_74 >= 0x69) {
        Sprite_Handle_BuildingDoor3( pSprite );
        return;
    }

    pSprite->field_74 += 1;
    pSprite->field_38 = eSprite_Anim_None;
    Sprite_Handle_BuildingDoor3( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Grenade2_Human_Called( sSprite* pSprite ) {
    Sprite_Handle_Helicopter_Human_CallCheck( pSprite );
    Sprite_Handle_Helicopter_Grenade2_Human( pSprite );

}

void cFodder::Sprite_Handle_Helicopter_Grenade_Human_Called( sSprite* pSprite ) {
    Sprite_Handle_Helicopter_Human_CallCheck( pSprite );
    Sprite_Handle_Helicopter_Grenade_Human( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Missile_Human_Called( sSprite* pSprite ) {
    Sprite_Handle_Helicopter_Human_CallCheck( pSprite );
    Sprite_Handle_Helicopter_Missile_Human( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Homing_Human_Called( sSprite* pSprite ) {
    Sprite_Handle_Helicopter_Human_CallCheck( pSprite );
    Sprite_Handle_Helicopter_Homing_Human( pSprite );
}

void cFodder::Sprite_Handle_Hostage_2( sSprite* pSprite ) {

    Sprite_Handle_Hostage( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Homing_Enemy2( sSprite* pSprite ) {

    Sprite_Handle_Helicopter_Homing_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Computer_1( sSprite* pSprite ) {

    Sprite_Handle_Computer( pSprite, 0x46 );
}

void cFodder::Sprite_Handle_Computer_2( sSprite* pSprite ) {
    
    Sprite_Handle_Computer( pSprite, 0x69 );
}

void cFodder::Sprite_Handle_Computer_3( sSprite* pSprite ) {

    Sprite_Handle_Computer( pSprite, 0xAF );
}

int16 cFodder::sub_1D92E( sSprite* pSprite ) {
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

    pSprite->field_44 = (int8) Data4;
    pSprite->field_36 = 0x78;
    pSprite->field_58 = -1;

loc_1D9C9:;
    pSprite->field_44 -= 1;
    if (!pSprite->field_44)
        goto loc_1DA13;

    Sprite_Movement_Calculate( pSprite );
    if (mSprite_Bullet_Destroy)
        goto loc_1DA13;

    Data0 = -3;
    Data4 = 2;
    if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
        goto loc_1DA13;

    if (pSprite->field_36 <= 0)
        goto loc_1DA13;

    return -1;

loc_1DA13:;
    pSprite->field_0 -= 8;
    if (pSprite->field_0 < 0)
        pSprite->field_0 = 0;

    pSprite->field_4 -= 8;
    Sprite_Turn_Into_Building_Explosion( pSprite );
    return -1;
}

void cFodder::Sprite_Native_Sound_Play( sSprite* pSprite, int16 pSoundID ) {

    if (mMission_Completed_Timer)
        return;

    if ((tool_RandomGet() & 0x7F) != 0x0E)
        return;

    int16 Data8 = 0;

    if (mMap_TileSet == eTileTypes_Int || mMap_TileSet == eTileTypes_Moors)
        Data8 = 0x7F;

    Sound_Play( pSprite, pSoundID, Data8 );
}

int16 cFodder::Sprite_Handle_Soldier_Animation( sSprite* pSprite ) {
    int32 Dataa4;
    int16 Data0, Data4, Data8;
    const int16* Data28;
    sSprite* Data20 =0, *Data2C = 0, *Data30 = 0;
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
    if (Data0 >= mMapHeight_Pixels)
        pSprite->field_4 = mMapHeight_Pixels;

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
    if (mMission_TryAgain)
        pSprite->field_40 = -26506;

    if (!pSprite->field_58) {
        Sprite_Create_Player_Shadow( pSprite );
        pSprite->field_58 = -1;
    }

    //loc_1E15D
    Data8 = pSprite->field_10;
    Data8 >>= 5;
    Data8 -= 1;
    Data8 ^= 0x0F;
    Data8 &= 0x0E;
    Data0 = pSprite->field_22;

    //Data0 <<= 2;
    Data28 = mSprite_AnimationPtrs[Data0];
    pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

    //loc_1E1E9
    pSprite->field_A = 0;
    pSprite->field_2A = 0;

    if (((int64)pSprite->field_1A) < 0xB0000) {
        int32 Dataa0 = tool_RandomGet() & 0x07;
        Dataa0 += 2;
        Dataa0 = (Dataa0 << 16) | (Dataa0 >> 16);
        Dataa0 += (int64) pSprite->field_1A;

        pSprite->field_1A = Dataa0;
    }
    //loc_1E232
    pSprite->field_12 = 2;
    Data0 = tool_RandomGet();
    Data4 = 0;
    Data0 &= 7;
    Data4 = mSprite_Indigenous_Sound_Death[Data0];
    //seg004:5508
    Sound_Play( pSprite, Data4, 0x14 );

    if (pSprite->field_36 < 0x1E)
        pSprite->field_36 += 0x0F;

    pSprite->field_3E = pSprite->field_10;
    pSprite->field_3A = -1;
    Data0 = tool_RandomGet() & 0x01;
    if (!Data0)
        pSprite->field_3A = pSprite->field_10;

    Sprite_Create_BloodTrail(pSprite, Data2C, Data30 );
    pSprite->field_26 = -1;
    if (pSprite->field_38 != eSprite_Anim_Die4)
        goto loc_1E3D2;

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
        sSprite* Data2C = 0, *Data30 = 0;

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

    Data4 = mSprite_Indigenous_Sound_Death[Data0];
    Data8 = 0;
    Sound_Play( pSprite, Data4, Data8 );
    return -1;

loc_1E3D2:;

    if (pSprite->field_40 == -26506) {
        sSprite* Data2C = 0, *Data30 = 0;

        pSprite->field_28 += 1;
        Sprite_Create_BloodTrail(pSprite, Data2C, Data30);

        if (!(pSprite->field_28 & 7)) {
            Data0 = tool_RandomGet() & 7;
            Data4 = mSprite_Indigenous_Sound_Death[Data0];
            Data8 = 0x14;
            Sound_Play( pSprite, Data4, Data8 );
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
        pSprite->field_8 = Data28[ (Data8 + 0x20) / 2 ];

        if (pSprite->field_59) {

            pSprite->field_8 = Data28[ (Data8 + 0x10) / 2];
        }

    }
    //loc_1E50A
    Sprite_XY_Store( pSprite );
    mTmp_FrameNumber = pSprite->field_A;
    Sprite_Movement_Calculate( pSprite );

    if (pSprite->field_20 < 0x0C) {

        int16 Field10 = pSprite->field_10;
        Sprite_Terrain_Check( pSprite, Data4 );
        pSprite->field_10 = Field10;
    }
    else {
        //loc_1E542
        int16 Field0 = pSprite->field_0;
        int16 Field4 = pSprite->field_4;
        int16 Field10 = pSprite->field_10;

        Sprite_Terrain_Check( pSprite, Data4 );

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
            sSprite* Data2C = 0, *Data30 = 0;
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
        goto loc_1EB5D;
    }
    //loc_1E6BA
    if (pSprite->field_20)
        goto loc_1EB5D;

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

        Sound_Play( pSprite, Data4, Data8 );
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
        Data4 = mSprite_Indigenous_Sound_Death[Data0];
        Data8 = 0x0A;
        Sound_Play( pSprite, Data4, Data8 );
        goto loc_1E9EC;
    }
    //loc_1E8D6
    pSprite->field_4 += Data0;
    Data0 = pSprite->field_4;
    if (Data0 >= mMapHeight_Pixels)
        pSprite->field_38 = eSprite_Anim_Hit2;

    mStoredSpriteY = pSprite->field_4;
    Data0 = 8;
    Data0 += pSprite->field_26;
    Data4 = -3;
    Data4 += pSprite->field_28;

    Map_Terrain_Get_Type_And_Walkable( Data0, Data4 );

    if (Data4 == eTerrainType_Drop || Data4 == eTerrainType_Drop2) {
        Data0 = pSprite->field_12;
        pSprite->field_28 += Data0;
        Data8 = pSprite->field_28;

        sMapTarget* Data30 = mSquad_WalkTargets[pSprite->field_32];
        //seg004:5C49
        Data0 = pSprite->field_40;
        if (Data30[Data0].mX >= 0)
            Data30[Data0].mY = Data8;
    }
    //loc_1E9CD;
    Sprite_Terrain_Check( pSprite, Data4 );
    if (pSprite->field_12 > 5)
        sub_223B2( pSprite );

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
    pSprite->field_8 = Data28[ (Data8 + 0x20) / 2 ];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[ (Data8 + 0x10) / 2 ];
    
loc_1EB0E:;
    Field_52 = pSprite->field_52;
    Field_0 = pSprite->field_0;
    Field_4 = pSprite->field_4;
    Sprite_Terrain_Check( pSprite, Data4 );
    pSprite->field_4 = Field_4;
    pSprite->field_0 = Field_0;
    pSprite->field_52 = Field_52;

    if ( pSprite->field_4F || pSprite->field_50 ) {

    loc_1EB5D:;
        if (pSprite->field_52 >= 0x0D)
            pSprite->field_52 = 0x0B;

        pSprite->field_52 += 2;
    }

    return -1;

loc_1EB87:;
    pSprite->field_12 += 1;
    if (pSprite->field_12 >= 0x0F)
        return Sprite_Troop_Dies( pSprite );

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
    pSprite->field_8 = Data28[ (Data8 + 0x20) / 2 ];

    if (pSprite->field_59)
        pSprite->field_8 = Data28[ (Data8 + 0x10) / 2 ];
    
    //loc_1EC4F
    Field_52 = pSprite->field_52;
    Sprite_Terrain_Check( pSprite, Data4 );
    pSprite->field_52 = Field_52;

    if (pSprite->field_4F || pSprite->field_50) {
        if (pSprite->field_52 >= 0x0D)
            pSprite->field_52 = 0x0B;
        pSprite->field_52 += 2;
    }

    //loc_1EC9A
    return -1;

loc_1ECA6:;
    if (pSprite->field_60 <= 0x06 && pSprite->field_60 >= 0x04) {
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

	Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction( pSprite );
    Data8 >>= 1;
    pSprite->field_A = Data8;
    return -1;

loc_1ED5B:;
    Sprite_XY_Store( pSprite );
    mTmp_FrameNumber = pSprite->field_A;

    Sprite_Movement_Calculate( pSprite );
    Sprite_Reached_MapEdge( pSprite );
    Sprite_Terrain_Check( pSprite, Data4 );

    if (pSprite->field_38 != eSprite_Anim_Slide2)
        goto loc_1EE3E;

    if (pSprite->field_60 > 6 || pSprite->field_60 < 4) {
        pSprite->field_36 -= 5;
        if (pSprite->field_36) {
            if (pSprite->field_36 >= 0)
                return -1;

            pSprite->field_36 = 0;
        }
    }
	Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction( pSprite );

    //seg004:606C
    Data28 = mSprite_AnimationPtrs[pSprite->field_22];
    Data0 = Data28[ (0x60 + Data8) / 2 ];

    if (Data0 != pSprite->field_8) {
        pSprite->field_8 = Data28[ (0x60 + Data8) / 2 ];
        pSprite->field_A = 0;
        return -1;
    }
    //loc_1EE34
    pSprite->field_38 = eSprite_Anim_None;

loc_1EE3E:;
    pSprite->field_43 = 0;
    sub_305D5( Data20 );
    return -1;

loc_1EE59:;
    if (pSprite->field_60 <= 6 && pSprite->field_60 >= 4 ) {
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

	Data8 = Sprite_Handle_Troop_Get_Frame_For_Direction( pSprite );
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
    Sprite_Draw_Row_Update( pSprite );
    return - 1;

}

int16 cFodder::Sprite_Troop_Dies( sSprite* pSprite ) {
    uint8* DataFinal = 0;
    int16 Data0;
    sSprite* eax = 0;
    int8* Data28 = 0;
    sMission_Troop* SquadMember = 0;

    // Is Player?
    if (pSprite->field_22 != eSprite_PersonType_Human)
        goto loc_1F0EA;

    //Yes, is player

    ++mTroops_Away;

    SquadMember = pSprite->field_46_mission_troop;

    Hero_Add( SquadMember );

    *mGraveRankPtr++ = SquadMember->mRank;
    *mGraveRankPtr = -1;

    *mGraveRecruitIDPtr++ = SquadMember->mRecruitID;
    *mGraveRecruitIDPtr = -1;

    SquadMember->mSprite = INVALID_SPRITE_PTR;
    SquadMember->mRecruitID = -1;
    SquadMember->mRank = 0;
    
    if (mSquad_Selected < 0) {
        mGUI_Sidebar_Setup = 0;
        mSquad_Select_Timer = 0;
    }
    else {
        mGUI_Sidebar_Setup = 0;
    }

    //loc_1F03D
    return Sprite_Destroy_Wrapper( pSprite );

loc_1F0EA:;

    if (pSprite->field_18 != eSprite_Enemy_Rocket)
        if (pSprite->field_18 != eSprite_Enemy)
            goto loc_1F218;

    ++mTroops_Home;
    --mTroops_Enemy_Count;
    if (pSprite->field_5D) {
        pSprite->field_5D = 0;
        DataFinal = (uint8*) mMission_Troops;
        Data0 = pSprite->field_5E;

        if (pSprite->field_5E >= 0)
            goto loc_1F19A;
    }
    if (mSquad_Selected == -1)
        goto loc_1F218;

    //loc_1F13E
    eax = mSquads[mSquad_Selected][0];
    if (eax == INVALID_SPRITE_PTR || eax == 0 )
        goto loc_1F218;

    DataFinal = (uint8*)eax->field_46_mission_troop;
    goto loc_1F1E9;

loc_1F19A:;
    DataFinal = (uint8*) &mMission_Troops[Data0];

loc_1F1E9:;

    if (DataFinal >= (uint8*)mMission_Troops && DataFinal <= (uint8*)&mMission_Troops[8]) {
        sMission_Troop* Member = (sMission_Troop*)DataFinal;
        if (Member->mNumberOfKills >= 999)
            Member->mNumberOfKills = 998;

        ++Member->mNumberOfKills;
    }
loc_1F218:;

    return Sprite_Destroy_Wrapper( pSprite );
}

int16 cFodder::loc_1F043( sSprite* pSprite ) {
    if (!pSprite->field_66) {
        pSprite->field_38 = eSprite_Anim_None;
        return 0;
    }

    loc_1F092( pSprite,  (sSprite*) pSprite->field_66 );
    pSprite->field_A = 0;
    pSprite->field_8 = 0x7C;
    pSprite->field_38 = eSprite_Anim_None;
    return 0;
}

void cFodder::loc_1F092( sSprite* pSprite, sSprite* pData24 ) {
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

int16 cFodder::Sprite_Handle_Player_MissionOver( sSprite* pSprite ) {

    if( !mMission_Aborted )
        if( !mMission_Complete )
            return 0;
        
    int16 Data0 = -1;
    int16 Data4 = 8;
    
    Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
    
    if( Data4 == eTerrainType_Water) {
        Data0 = pSprite->field_20;
        Data0 -= 8;
        
        if( Data0 >= 0 ) {
            Data0 = 0;
        } else {
            Data0 = -Data0;
            if( Data0 > 8 )
                Data0 = 8;
        }
        
        pSprite->field_52 = Data0;
    }
    //loc_1F283
    
    if( pSprite->field_8 == 0x41 )
        goto loc_1F3C4;
    
    if( pSprite->field_8 == 0x9F )
        goto loc_1F340;
    
    if( pSprite->field_8 == 0x38 )
        goto loc_1F3A2;
    
    pSprite->field_A = 0;
    pSprite->field_36 = 0;
    
    if( !mMission_Aborted ) {
        
        Data0 = tool_RandomGet() & 1;
        if(Data0 == 0)
            goto loc_1F32C;
    
        if( !pSprite->field_52 ) {
            Data0 = tool_RandomGet() & 1;
            
            if( !Data0 ) {
                //loc_1F2F9
                pSprite->field_8 = 0x41;
                Data0 = tool_RandomGet() & 3;
                pSprite->field_A = Data0;
                pSprite->field_2A = 1;
                Sprite_Create_Player_Shadow( pSprite );
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
    
    pSprite->field_A = mSprite_Player_MissionOver_Frames[ Data0 ];
    pSprite->field_36 ^= -32768;
    if(pSprite->field_36 < 0)
        goto loc_1F41D;
    
    pSprite->field_2A += 1;
    if( pSprite->field_2A < 7)
        goto loc_1F41D;
    
    pSprite->field_2A = 3;
    goto loc_1F41D;
    
    loc_1F3A2:;
    pSprite->field_36 ^= -32768;
    if( pSprite->field_36 < 0 )
        goto loc_1F3C2;
    
    if( pSprite->field_A >= 5 )
        goto loc_1F41D;
    
    pSprite->field_A += 1;
    loc_1F3C2:;
    goto loc_1F41D;
    
    loc_1F3C4:;

    pSprite->field_A += pSprite->field_2A;
    if( !pSprite->field_A )
        goto loc_1F3E9;
    
    if( pSprite->field_A != 5 )
        goto loc_1F3F1;
    
    loc_1F3E9:;
    pSprite->field_2A = -pSprite->field_2A;
    
    loc_1F3F1:;
    Data0 = pSprite->field_A;
    Data0 -= 1;
    if( Data0 < 0)
        Data0 = 0;
    
    Data0 <<= 3;
    pSprite->field_20 = Data0;
    
    loc_1F41D:;
    Data0 = -1;
    
    return -1;
}

void cFodder::Sprite_Handle_Troop_Weapon( sSprite* pSprite ) {

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

	} else {
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

	} else {

		// Enemy, after 4th map (can throw grenades)
		//loc_1F4B0
		if (pSprite->field_18 == eSprite_Enemy && mMapNumber > 4) {

			// Use a grenade?
			if (!(tool_RandomGet() & 0x1F)) {

				mTroop_Weapon_Bullet_Disabled = true;
				mTroop_Weapon_Grenade_Disabled = false;
			}
		}
	}

	// bullet weapon disabled?
    if (!mTroop_Weapon_Bullet_Disabled) {

        if (Sprite_Create_Bullet( pSprite ))
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

void cFodder::Sprite_Handle_Troop_Direct_TowardWeaponTarget( sSprite* pSprite ) {

    int16 Data0 = pSprite->field_2E;
    int16 Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
    Sprite_Set_Direction_To_Next( pSprite );
    Sprite_Handle_Troop_FrameUnk( pSprite );
}

void cFodder::Sprite_Handle_Troop_Direct_TowardMouse( sSprite* pSprite ) {

    int16 Data0 = mMouseX;
    Data0 += mCamera_Adjust_Col >> 16;
    Data0 -= 0x18;

    int16 Data4 = mMouseY;
    Data4 += mCamera_Adjust_Row >> 16;

    Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
    Sprite_Set_Direction_To_Next( pSprite );

    mStoredSpriteFrame = pSprite->field_A;
    Sprite_Handle_Troop_FrameUnk( pSprite );

    if (!pSprite->field_43)
        return;

    pSprite->field_A = mStoredSpriteFrame;
}

void cFodder::Sprite_XY_Store( sSprite* pSprite ) {
    mSprite_Reached_Target = 0;
    
    mStoredSpriteX = (pSprite->field_0 & 0xFFFF) | (pSprite->field_2 << 16);
    mStoredSpriteY = (pSprite->field_4 & 0xFFFF) | (pSprite->field_6 << 16);
}

void cFodder::Sprite_XY_Restore( sSprite* pSprite ) {
    mSprite_Reached_Target = -1;

    pSprite->field_0 = mStoredSpriteX & 0xFFFF;
    pSprite->field_2 = mStoredSpriteX >> 16;

    pSprite->field_4 = mStoredSpriteY & 0xFFFF;
    pSprite->field_6 = mStoredSpriteY >> 16;
}

void cFodder::Sprite_Handle_Player_Adjust_Movement_Speed( sSprite* pSprite ) {
    sub_1FCF2( pSprite );

    if (pSprite->field_4F)
        return;

    if (pSprite->field_50)
        return;

    int16 Data0 = pSprite->field_10;
    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;

    int16 Data4 = 0;
    for (Data4 = 0; Data4 < 0x0E; Data4+=2) {
        if (Data0 == pSprite->field_3C)
            break;

        Data0 += 2;
        Data0 &= 0x0E;
    }

    pSprite->field_36 = mSprite_Speed_Direction_Modifier[Data4 / 2];
}

void cFodder::Sprite_Draw_Row_Update( sSprite* pSprite ) {
    int16 Data4 = 0;

    // Sprite still moving?
    if (!Sprite_Reached_Target( pSprite )) {
        Sprite_Movement_Speed_Update( pSprite );
        Sprite_Movement_Calculate( pSprite );
    }

    Squad_Join_Check( pSprite );
    Sprite_Reached_MapEdge( pSprite );

    // Check the terrain
    int8 PreviousTileType = pSprite->field_60;
    Sprite_Terrain_Check( pSprite, Data4 );

    // Not leaving water?
    if (PreviousTileType != eTerrainType_Water)
        goto loc_1F75D;

    // Leaving water and into Quick sand?
    if (pSprite->field_60 == eTerrainType_QuickSand)
        goto loc_1F753;

    // Leaving water for the edge/bank
    if (pSprite->field_60 == eTerrainType_WaterEdge)
        goto loc_1F75D;

    pSprite->field_52 = 5;
    goto loc_1F75D;

loc_1F753:;
    pSprite->field_52 = 3;

loc_1F75D:;
    Sprite_Handle_Player_Close_To_SquadMember( pSprite );
}

void cFodder::Sprite_Handle_Troop_FrameUnk( sSprite* pSprite ) {
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

    if (pSprite->field_60 == 6)
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
        } else {
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

    if (Data0 != pSprite->field_8 ) {
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

    pSprite->field_A = mTmp_FrameNumber;
    mSprite_Bumped_Into_SquadMember = 0;
}

void cFodder::sub_1FCF2( sSprite* pSprite ) {
    int16 Data0;

    pSprite->field_36 = 0x10;

    if (pSprite->field_18 == eSprite_Enemy) {
        Data0 = 0x0C;
        Data0 += pSprite->field_62;
        if (Data0 > 0x1A)
            Data0 = 0x1A;
        pSprite->field_36 = Data0;
	} else {

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

void cFodder::sub_1FDE7( sSprite* pSprite ) {
    
    int16 Data0 = pSprite->field_2E;
    int16 Data4 = pSprite->field_30;

    Sprite_Direction_Between_Points( pSprite, Data0, Data4 );
    Sprite_Set_Direction_To_Next( pSprite );

    mStoredSpriteFrame = pSprite->field_A;

    Sprite_Handle_Troop_FrameUnk( pSprite );
    if (!pSprite->field_43)
        return;

    pSprite->field_A = mStoredSpriteFrame;
}

void cFodder::Sprite_Create_Player_Shadow( sSprite* pSprite ) {
    int16 Data0 = 0;

    sSprite* Data2C, *Data30;

    if (pSprite->field_5C == 0)
        return;

    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
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

int16 cFodder::Sprite_Create_BloodTrail( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30 ) {
    int16 Data0;

    Data0 = 1;
    
    if (!Sprite_Get_Free( Data0, pData2C, pData30 )) {
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
        pData2C->field_2C = eSprite_Draw_Last;
        pData2C->field_52 = 0;
        pData2C->field_12 = 1;
        Data0 = 0;
    }

    return Data0;
}

void cFodder::Sprite_Terrain_Check( sSprite* pSprite, int16& pData4 ) {
    int16 Data0 = -3;
    pData4 = 8;

    if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, pData4 ))
        goto CheckFalling;

    pSprite->field_60 = pData4 & 0xFF;
    pSprite->field_50 = 0;
    pSprite->field_4F = 0;

    if (pData4 == eTerrainType_Drop2) {
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
    if (pData4 == eTerrainType_Drop) {

        if (pSprite->field_18 == eSprite_Enemy)
            goto loc_20251;

        if (pSprite->field_22 == eSprite_PersonType_Native)
            goto loc_20251;

        ++pSprite->field_56;
        return;
    }

    //loc_20072
    pSprite->field_56 = 0;
    if (pData4 == eTerrainType_QuickSandEdge)
        goto loc_201CC;

    if (pData4 == eTerrainType_Rocky)
        goto loc_20108;

    if (pData4 == eTerrainType_Rocky2)
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
    if (pData4 == eTerrainType_QuickSand)
        goto Soldier_InQuickSand;

    if (pData4 == eTerrainType_WaterEdge)
        goto Soldier_WaterEdge;

    if (pData4 != eTerrainType_Water)
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
    if (pData4 == eTerrainType_Sink)
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
    if (pData4 == eTerrainType_Drop || pData4 == eTerrainType_Drop2) {
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
    if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, pData4 ))
        goto loc_202E5;

    if (pData4 == eTerrainType_Drop)
        goto loc_202E5;

    if (pData4 == eTerrainType_Drop2)
        goto loc_202E5;

    if (pSprite->field_22 == eSprite_PersonType_Native && pData4 == eTerrainType_Water)
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
    pSprite->field_44 = (int8) Data0;
    return;

loc_2035C:;

    int16 word_3A9E6 = pSprite->field_0;
    pSprite->field_0 = mStoredSpriteX & 0xFFFF;

    Data0 = -3;
    pData4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, pData4 ))
        return;

    pSprite->field_0 = word_3A9E6;
    pSprite->field_4 = mStoredSpriteY & 0xFFFF;

    Data0 = -3;
    pData4 = 8;
    if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, pData4 ))
        return;

    //loc_203BA
    pSprite->field_A = mTmp_FrameNumber;
    Sprite_XY_Restore( pSprite );
    pSprite->field_26 = pSprite->field_0;
    pSprite->field_28 = pSprite->field_4;

    sMapTarget* Data30 = mSquad_WalkTargets[ pSprite->field_32 ];
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

void cFodder::String_Print_Small( std::string pText, const size_t pY ) {
    std::transform( pText.begin(), pText.end(), pText.begin(), ::toupper );

    String_CalculateWidth( 320, mFont_Briefing_Width, pText );
    String_Print( mFont_Briefing_Width, 0, mGUI_Temp_X, pY, pText );
}

void cFodder::String_Print_Large( std::string pText, const bool pOverAndUnderLine, const uint16 pY ) {
    std::transform( pText.begin(), pText.end(), pText.begin(), ::toupper );

    String_CalculateWidth( 320, mFont_Underlined_Width, pText );
    String_Print( mFont_Underlined_Width, pOverAndUnderLine == true ? 1 : 3, mGUI_Temp_X, pY, pText );
}

void cFodder::String_Print( const uint8* pWidths, int32 pFontSpriteID, size_t pParam08, size_t pParamC, const std::string& pText ) {

    String_Print( pWidths, pFontSpriteID, pParam08, pParamC, pText.c_str() );
}

void cFodder::String_Print(  const uint8* pWidths, int32 pFontSpriteID, size_t pParam08, size_t pParamC, const char* pText ) {
    const uint8* ptr = 0;
    uint8 al = 0;
    int32 unk14 = 0;

    mGUI_Temp_Y = (int16) pParamC;
    mGUI_Draw_LastHeight = 0;

    for (;;) {
    String_Print_Next:;
        uint8 NextChar = *pText++;
        uint8 NextChar10 = 0;

        //seg007:0170
        if (NextChar == 0xFD) {
            //sub_29DE2(0x10);

        } else if (NextChar == 0xFE) {
            //sub_29DE2(0x0F);

        } else if (NextChar == 0x00)
            break;

        else {
            //seg007:01A1
            NextChar10 = NextChar;

            if (!(NextChar & 0x80)) {

                //01AB
                if (mString_GapCharID) {
                    if (NextChar == 0x20) {
                        NextChar = (uint8) mString_GapCharID;
                        goto loc_29D71;
                    }
                } else {
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
                        if (mVersion->mPlatform == ePlatform::Amiga)
                            NextChar += 0x28;
                        else
                            NextChar += 0x39;
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
            } while(al != NextChar);
            
            // Found the special character, lets use its index
            NextChar = 0x24 + unk14;

            loc_29D71:;

            if (mGUI_Print_String_To_Sidebar)
                mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(pFontSpriteID + NextChar, pParam08, pParamC );
            else
                GUI_Draw_Frame_8( pFontSpriteID, NextChar, pParam08, pParamC);

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
    int16 Data0 = mMouseX;
    int16 Data4 = mMouseY;
    Data0 += mCamera_Adjust_Col >> 16;
    Data4 += mCamera_Adjust_Row >> 16;
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

    mCamera_Position_Column = Data0;
    mCamera_Position_Row = Data4;
    word_3A054 = 0;

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

    Data0 += mCamera_Adjust_Col >> 16;
    Data4 += mCamera_Adjust_Row >> 16;

    Data0 -= 0x0F;
    Data4 -= 3;

    sSprite* Data20 = mSprites;

    for (int16 Data1C = 0x2B; Data1C >= 0; --Data1C, ++Data20) {

        if (Data20->field_0 == -32768)
            continue;

        if (!Data20->field_65)
            continue;

        if (Data20->field_22 != eSprite_PersonType_Human)
            continue;

        if (Data20->field_20)
            continue;

        int16 Data18 = Data20->field_18;
        int16 Data8 = Data20->field_0;

        if (Data20->field_6F == eVehicle_Turret_Cannon || 
            Data20->field_6F == eVehicle_Turret_Missile)
            Data8 -= 8;

        if (Data0 < Data8)
            continue;

        Data8 += mSprite_Width[Data18];
        if (Data0 > Data8)
            continue;

        Data8 = Data20->field_4 - mSprite_Height_Top[Data18];
        Data8 -= 0x14;
        if (Data4 < Data8)
            continue;

        Data8 = Data20->field_4 + mSprite_Height_Bottom[Data18];
        if (Data4 > Data8)
            continue;

        if (mSquad_Selected < 0)
            break;

        Data24 = mSquads[mSquad_Selected];
        for (;;) {
            if (*Data24 == INVALID_SPRITE_PTR)
                goto loc_313C6;

            sSprite* Data28 = *Data24++;
            Data28->field_66 = Data20;
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

    Data0 += mCamera_Adjust_Col >> 16;
    Data4 += mCamera_Adjust_Row >> 16;

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

    if (Map_Terrain_Get_Type_And_Walkable(Data20, Data0,Data4))
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
    mMouse_Button_LeftRight_Toggle = 0;
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
    PosX += mCamera_Adjust_Col >> 16;
    PosY += mCamera_Adjust_Row >> 16;
    PosX -= 0x10;

    if (!PosX)
        PosX = 1;

    PosY -= 8;
    Vehicle->field_2E = PosX;
    Vehicle->field_30 = PosY;
}

void cFodder::String_CalculateWidth( int32 pPosX, const uint8* pWidths, const std::string& pString ) {

    String_CalculateWidth( pPosX, pWidths, pString.c_str() );
}

void cFodder::String_CalculateWidth( int32 pPosX, const uint8* pWidths, const char* pString ) {
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

void cFodder::intro_LegionMessage() {
    int16 Duration = 300;
    bool DoBreak = false;

    mImage->clearBuffer();
    mGraphics->PaletteSet();

    mImageFaded = -1;

    Intro_Print_String( mVersion->mIntroData[0].mText[0].mPosition, &mVersion->mIntroData[0].mText[0] );
    Intro_Print_String( mVersion->mIntroData[0].mText[1].mPosition, &mVersion->mIntroData[0].mText[1] );
    Intro_Print_String( mVersion->mIntroData[0].mText[2].mPosition, &mVersion->mIntroData[0].mText[2] );

    while( mImageFaded == -1 || DoBreak == false  ) {
        
        if (mImageFaded)
            mImageFaded = mImage->palette_FadeTowardNew();

        if (Duration > 1)
            --Duration;
        else {
            if (DoBreak == false) {
                mImage->paletteNew_SetToBlack();
                mImageFaded = -1;
                Duration = 0;
                DoBreak = true;
            }
        }
    
        eventProcess();
        g_Window.RenderAt( mImage );
        g_Window.FrameEnd();
    }
}

int16 cFodder::intro_Play() {

    mGraphics->Load_Sprite_Font();
    mGraphics->SetActiveSpriteSheet( eGFX_Types::eGFX_FONT );

    for ( word_3B2CF = 1; mVersion->mIntroData[word_3B2CF].mImageNumber != 0; ++word_3B2CF ) {

        mIntro_PlayTextDuration = 0x288;
        
        mImage->palette_SetToBlack();

        if (mVersion->mIntroData[word_3B2CF].mImageNumber == 0 && mVersion->mIntroData[word_3B2CF].mText == 0 )
            break;

        if (mVersion->mIntroData[word_3B2CF].mImageNumber != 0xFF) {

            std::stringstream ImageName;
            ImageName << (char) mVersion->mIntroData[word_3B2CF].mImageNumber;

            mGraphics->Load_And_Draw_Image( ImageName.str(), 0xD0 );
        }
        else {
            mIntro_PlayTextDuration = 0xAF;
            mImage->clearBuffer();
        }

        mGraphics->PaletteSet();
        const sIntroString* IntroString = mVersion->mIntroData[word_3B2CF].mText;
        if (IntroString) {
            while (IntroString->mPosition) {

                Intro_Print_String( IntroString->mPosition, IntroString );
                ++IntroString;
            }
        }
        //loc_16710
        
        int16 Duration = mIntro_PlayTextDuration;
        int16 Fade = -1;
        bool DoBreak = false;

        while( Fade == -1 || DoBreak == false  ) {
            --Duration;

            if (Duration) {
                if (Fade)
                    Fade = mImage->palette_FadeTowardNew();

                Mouse_GetData();
                if (mouse_Button_Status) {
                    word_3B2CF = 16;
                    mImage_Aborted = -1;
                    mImage->paletteNew_SetToBlack();
                    Fade = -1;
                    DoBreak = true;
                }
            }
            else {
                mImage->paletteNew_SetToBlack();
                Fade = -1;
                DoBreak = true;
            }

            g_Window.RenderAt( mImage );
            g_Window.FrameEnd();
        }

        
    }

    return mImage_Aborted;
}

void cFodder::Mission_Intro_Play() {

    // Single maps
    if (mCustom_Mode == eCustomMode_Map)
        return;

    // If we don't have the briefing graphics, there is no point in switching here,
    //  As the correct background for the map intro won't exist
    if (!mVersion->hasGfx(eGFX_BRIEFING))
        return;

    mImage->clearBuffer();

    mGraphics->Mission_Intro_Load_Resources();
    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    
    mMouse_Exit_Loop = 0;
    mSound->Music_Play(0x07);
    Mission_Intro_Helicopter_Start();

    mGraphics->Mission_Intro_Play();
}

void cFodder::Intro_Print_String( int32 pPosY, const sIntroString* pString ) {

    if (mVersion->mPlatform == ePlatform::PC)
        pPosY -= 0x19;
    else
        pPosY += 0x9;

    String_CalculateWidth( 320, mFont_Intro_Width, pString->mText );
    String_Print( mFont_Intro_Width, 0, mGUI_Temp_X, pPosY, pString->mText );
}

void cFodder::Image_FadeIn() {
    mImage->Save();
    mGraphics->PaletteSet();

    while (mImage->palette_FadeTowardNew() == -1) {

        Mouse_Inputs_Get();
        //Mouse_DrawCursor();

        g_Window.RenderAt( mImage, cPosition() );
        g_Window.FrameEnd();
        mImage->Restore();
    }

    g_Window.RenderAt( mImage, cPosition() );
    g_Window.FrameEnd();
}

void cFodder::Image_FadeOut() {
    mImage->Save();
    mImage->paletteNew_SetToBlack();

    while (mImage->GetFaded() == false) {

        Mouse_Inputs_Get();
        //Mouse_DrawCursor();

        mImage->palette_FadeTowardNew();

        g_Window.RenderAt( mImage );
        g_Window.FrameEnd();
        mImage->Restore();
    }
}

void cFodder::SetActiveSpriteSheetPtr( const sSpriteSheet** pSpriteSheet ) {

    mSpriteSheetPtr = pSpriteSheet;
}

void cFodder::intro() {
    
    if (mVersion->mPlatform == ePlatform::Amiga)
        mWindow->SetScreenSize(cDimension(320, 260));

    // Disabled: GOG CD Version doesn't require a manual check
    //  CopyProtection();

    mImage_Aborted = 0;
    mGraphics->Load_Sprite_Font();
    
    if (mSkipIntro)
        goto introDone;

    intro_Music_Play();

    if (mVersion->mGame == eGame::CF1)
        intro_LegionMessage();

    if (ShowImage_ForDuration( "cftitle", 0x1F8 ))
        goto introDone;
    
    if (intro_Play())
        goto introDone;

    if (ShowImage_ForDuration( "virgpres", 0x2D0 ))
        goto introDone;
    
    if (ShowImage_ForDuration( "sensprod", 0x2D0 ))
        goto introDone;
    
    if (ShowImage_ForDuration( "cftitle", 0x318 ))
        goto introDone;

introDone:;
    mIntroDone = true;
    mSound->Music_Stop();

    mGraphics->Load_pStuff();
    mSound->Music_Play( 0 );

	if (mVersion->mPlatform == ePlatform::Amiga)
		mWindow->SetScreenSize(cDimension(320, 225));
}

void cFodder::intro_Music_Play() {

    mSound->Music_Play( 16 );
}

int16 cFodder::ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration ) {
    bool DoBreak = false;

    g_Graphics.Load_And_Draw_Image( pFilename, 0x100 );
    mGraphics->PaletteSet();

    mImageFaded = -1;
    while( mImageFaded == -1 || DoBreak == false  ) {
        Mouse_GetData();
        --pDuration;

        if (pDuration) {
            if (mImageFaded)
                mImageFaded = mImage->palette_FadeTowardNew();

            if (mouse_Button_Status) {
                mImage_Aborted = -1;
                mImage->paletteNew_SetToBlack();
                mImageFaded = -1;
                DoBreak = true;
            }
        }
        else {
            mImage->paletteNew_SetToBlack();
            mImageFaded = -1;
            DoBreak = true;
        }

        g_Window.RenderAt( mImage );
        g_Window.FrameEnd();
    }

    return mImage_Aborted;
}

void cFodder::Mission_Phase_Next() {

    // Next Phase
    ++mMissionPhase;
    --mMission_Phases_Remaining;

    // Still got phases to complete?
    if (mMission_Phases_Remaining)
        return;

    // Mission Complete
    for (unsigned int x = 0; x < 8; ++x) {
        mMission_Troops[x].mPhaseCount = 0;
    }

    mSaved_MissionNumber = mMissionNumber;
    mSaved_MapNumber = mMapNumber;
    --mSaved_MapNumber;

    ++mMissionNumber;
    mMission_Phases_Remaining = mMission_Phases_Total = mCampaign.getNumberOfPhases(mMissionNumber);
    mMissionPhase = 0;
    mRecruits_Available_Count += 0x0F;

    mMission_Recruits_AliveCount = mRecruits_Available_Count;
    mMission_Recruits_AliveCount -= 0x0F;
    mMission_Recruitment = -1;
    Mission_Troop_Prepare_SetFromSpritePtrs = 0;

    mGraveRecruitIDPtr = mGraveRecruitID;
    mGraveRecruitID[0] = -1;
    mGraveRankPtr2 = mGraveRankPtr;
}

void cFodder::Video_Sleep_Wrapper() {

    Video_Sleep();
}

void cFodder::Video_Sleep() {
    static uint64_t delta = 2;

    mTicksDiff = SDL_GetTicks() - mTicksDiff;

    mTicks = mTicksDiff * 40 / 1000;

    sleepLoop(delta * 1000 / 40 - mTicksDiff);

    mTicksDiff = SDL_GetTicks();
}

void cFodder::sleepLoop( int64 pMilliseconds ) {

    uint64 TimeStarted = SDL_GetTicks();
    uint64 TimeFinish = TimeStarted + pMilliseconds;

    do {
        mWindow->EventCheck();

        if (SDL_GetTicks() >= TimeFinish)
            break;

        SDL_Delay( 10 );

    } while (1);

}

void cFodder::WonGame() {
    mSound->Music_Play( 17 );
    mMouseX = -1;
    mMouseY = -1;

    mGraphics->Load_And_Draw_Image( "won", 0x100 );

    Image_FadeIn();

    for (int count = 500; count >= 0; --count) {

        eventProcess();
        Video_Sleep();
    }

    Image_FadeOut();
}

int16 cFodder::Sprite_Handle_Troop_Get_Frame_For_Direction( sSprite* pSprite ) {
    int16 FrameNumber = pSprite->field_10;

	FrameNumber >>= 5;
	FrameNumber -= 1;
	FrameNumber ^= 0x0F;
	FrameNumber &= 0x0E;

	return FrameNumber;
}

void cFodder::Sprite_Reached_MapEdge( sSprite* pSprite ) {

    if (pSprite->field_4 < 0 || pSprite->field_4 <= 13) {
        pSprite->field_4 = mStoredSpriteY;
        mSprite_Reached_Target = -1;
    }

    if (pSprite->field_4 >= mMapHeight_Pixels) {
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

    if (pSprite->field_0 + 12 < mMapWidth_Pixels)
        return;

    if (mStoredSpriteX + 16 >= mMapWidth_Pixels)
        return;

loc_20521:;
    pSprite->field_0 = mStoredSpriteX;
    mSprite_Reached_Target = -1;
}

void cFodder::Hero_Add( sMission_Troop* pTroop ) {
    sHero* Hero = &mHeroes[4];
    int16 Data4;

    for (Data4 = 4; Data4 >= 0; --Data4, --Hero ) {
        if (!pTroop->mNumberOfKills)
            break;

        if (pTroop->mNumberOfKills < Hero->mKills)
            break;

        if (pTroop->mNumberOfKills > Hero->mKills)
            continue;

        if (pTroop->mRank <= Hero->mRank)
            break;
    }
    if (Data4 == 4)
        return;

    // Move the heros down the list one, until we reach the slot
    int16 X = 4;
    do {
        mHeroes[X + 1] = mHeroes[X];
        --X;
    } while (++Data4 < 4);

    // 
    mHeroes[X + 1].mRecruitID = (int8)pTroop->mRecruitID;
    mHeroes[X + 1].mRank = pTroop->mRank;
    mHeroes[X + 1].mKills = pTroop->mNumberOfKills;
}

int16 cFodder::Sprite_Destroy_Wrapper( sSprite* pSprite ) {

    return Sprite_Destroy( pSprite );
}

int16 cFodder::Sprite_Destroy( sSprite* pSprite ) {
    pSprite->field_65 = 0;
    pSprite->field_8 = 0x7C;
    pSprite->field_18 = eSprite_Null;
    pSprite->field_A = 0;
    pSprite->field_24 = 1;

    return 1;
}

int16 cFodder::Sprite_Create_Bullet( sSprite* pSprite ) {
    int16 Data0 = 1, Data8, Data4;
    sSprite* Data2C = 0, *Data30 = 0;

    if (mMission_Completed_Timer)
        return -1;

    if (!pSprite->field_2E)
        return -1;

    if (pSprite == mSquad_Leader)
        if (pSprite->field_54 == 1)
            return -1;

    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
        return -1;

    if (byte_3A9D2[2] == 0x14)
        return -1;

    ++byte_3A9D2[2];
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
    } else {

        Data2C->field_5E = (int16) (pSprite->field_46_mission_troop - mMission_Troops);
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

    Sound_Play( pSprite, Data4, 0 );
    return 0;
}

int16 cFodder::Sprite_Create_Grenade( sSprite* pSprite ) {
    int16 Data0, Data4, Data8, DataC;
    sSprite* Data2C = 0, *Data30 =0;

    if (mMission_Completed_Timer)
        goto loc_20ADE;
    
    if (byte_3A9D2[pSprite->field_22] == 2)
        goto loc_20ADE;

    if (!pSprite->field_2E)
        goto loc_20ADE;

    if (pSprite->field_18 != eSprite_Enemy)
        goto loc_20AC1;

    Data0 = pSprite->field_0;
    Data4 = pSprite->field_4;
    Data8 = pSprite->field_2E;
    DataC = pSprite->field_30;
    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 >= 0x82)
        goto loc_20ADE;

loc_20AC1:;

    Data0 = 2;

    if (!Sprite_Get_Free( Data0, Data2C, Data30 ))
        goto loc_20B0A;
    
    return -1;

loc_20ADE:;
    if (pSprite == mSquad_Leader)
        mMouse_Button_LeftRight_Toggle = 0;

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

    ++byte_3A9D2[pSprite->field_22];
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

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
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
        mMouse_Button_LeftRight_Toggle = 0;

    return 0;
}

void cFodder::Sprite_Set_Direction_To_Next( sSprite* pSprite ) {
    int16 Data0 = pSprite->field_10;

    Data0 >>= 5;
    Data0 -= 1;
    Data0 ^= 0x0F;
    Data0 &= 0x0F;
    pSprite->field_3C = Data0;

    mDirectionMod = 0;
}

int16 cFodder::Sprite_Reached_Target( sSprite* pSprite ) {

    // Sprite is moving?
    if (pSprite->field_36 <= 8)
        return 0;

    int16 Data0 = pSprite->field_26;
    int16 Data4 = pSprite->field_28;

    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;

    Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
    if (Data0 > 2)
        return 0;

    pSprite->field_0 = pSprite->field_26;
    pSprite->field_4 = pSprite->field_28;

    mSprite_Reached_Target = -1;
    return -1;
}

void cFodder::Sprite_Movement_Speed_Update( sSprite* pSprite ) {
    int16 Data0 = pSprite->field_10;

    if (pSprite->field_60 != eTerrainType_D)
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
    if (pSprite->field_60 == eTerrainType_Rocky)
        goto loc_20FD1;

    if (pSprite->field_60 != eTerrainType_Rocky2)
        goto loc_20FE3;

loc_20FD1:;
    pSprite->field_36 >>= 1;

loc_20FD9:;
    pSprite->field_36 >>= 1;
    return;
loc_20FE3:;
    if (pSprite->field_60 != eTerrainType_Snow)
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

void cFodder::Sprite_Handle_Player_Close_To_SquadMember( sSprite* pSprite ) {
    
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
        if (*PreviousMember == INVALID_SPRITE_PTR )
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

    if (pSprite->field_4 + 2 < Data24->field_4 + 2)
        return;

    // In Vehicle?
    if (Data24->field_6E)
        return;

    // Different Walk Target?
    int16 Data1C = pSprite->field_40;
    if (Data1C != Data24->field_40)
        return;

    // Restore the previous XY
    Sprite_XY_Restore( pSprite );
    mSprite_Bumped_Into_SquadMember = -1;

    pSprite->field_43 = 1;
    word_3B2F3 = -1;
}

int16 cFodder::Sprite_Get_Free( int16& pData0, sSprite*& pData2C, sSprite*& pData30 ) {

    // 
    if (!mSprite_SpareUsed) {

        // Looking for two sprites?
        if (pData0 == 2) {
            pData2C = mSprites;

            // Loop all sprites
            for (int16 Data1C = 0x29; Data1C >= 0; --Data1C, ++pData2C) {

                // Sprite free?
                if (pData2C->field_0 != -32768)
                    continue;

                // Second sprite free?
                if ((pData2C + 1)->field_0 == -32768) {
                    pData30 = pData2C + 1;

                    Sprite_Clear( pData2C );
                    Sprite_Clear( pData30 );
                    pData0 = 0;
                    return 0;
                }
            }

        } else {
            // Only looking for 1 sprite
            pData2C = &mSprites[42];

            for (int16 Data1C = 0x2A; Data1C >= 0; --Data1C) {

                // Free?
                if (pData2C->field_0 == -32768) {
                    Sprite_Clear( pData2C );
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

void cFodder::Sprite_Clear( sSprite* pSprite ) {
    pSprite->field_0 = -32768;
    pSprite->field_2 = 0;
    pSprite->field_4 = 0;
    pSprite->field_6 = 0;
    pSprite->field_8 = 0;
    pSprite->field_A = 0;
    pSprite->field_C = 0;
    pSprite->field_E = 0;
    pSprite->field_10 = 0;
    pSprite->field_12 = 0;
    pSprite->field_14 = 0;
    pSprite->field_16 = 0;
    pSprite->field_18 = 0;
    pSprite->field_1A_sprite = 0;
    pSprite->field_1E = 0;
    pSprite->field_20 = 0;
    pSprite->field_22 = eSprite_PersonType_Human;
    pSprite->field_24 = 0;
    pSprite->field_26 = 0;
    pSprite->field_28 = 0;
    pSprite->field_2A = 0;
    pSprite->field_2C = eSprite_Draw_Second;
    pSprite->field_2E = 0;
    pSprite->field_30 = 0;
    pSprite->field_32 = 0;
    pSprite->field_34 = 0;
    pSprite->field_36 = 0;
    pSprite->field_38 = eSprite_Anim_None;
    pSprite->field_3A = 0;
    pSprite->field_3C = 0;
    pSprite->field_3E = 0;
    pSprite->field_40 = 0;
    pSprite->field_42 = 0;
    pSprite->field_43 = 0;
    pSprite->field_44 = 0;
    pSprite->field_45 = 0;
    pSprite->field_46_sprite = 0;
    pSprite->field_4A = 0;
    pSprite->field_4C = 0;
    pSprite->field_4D = 0;
    pSprite->field_4E = 0;
    pSprite->field_4F = 0;
    pSprite->field_50 = 0;
    pSprite->field_52 = 0;
    pSprite->field_54 = 0;
    pSprite->field_55 = 0;
    pSprite->field_56 = 0;
    pSprite->field_58 = 0;
    pSprite->field_59 = 0;
    pSprite->field_5A = 0;
    pSprite->field_5B = 0;
    pSprite->field_5C = 0;
    pSprite->field_5D = 0;
    pSprite->field_5E = 0;
    pSprite->field_60 = 0;
    pSprite->field_61 = 0;
    pSprite->field_62 = 0;
    pSprite->field_64 = 0;
    pSprite->field_65 = 0;
    pSprite->field_66 = 0;
    pSprite->field_6A_sprite = 0;
    pSprite->field_6E = 0;
    pSprite->field_6F = 0;
    pSprite->field_70 = 0;
    pSprite->field_74 = 0;
    pSprite->field_75 = 0;
}

void cFodder::Sprite_Handle_Exploidable( sSprite* pSprite ) {
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

    Sprite_Movement_Calculate( pSprite );

    Dat0 = (int64) pSprite->field_1A;
    Dat4 = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);

    Dat0 -= 0x28000;
    Dat4 += (int32) Dat0;

    if (Dat4 < 0) {
        Dat4 = 0;
        Dat0 = -Dat0;
        Dat0 >>= 2;
    }

    pSprite->field_1A = (int32) Dat0;
    pSprite->field_1E_Big = Dat4;

    if (!(Dat4 >> 16)) {
        if (!(Dat0 >> 16))
            goto loc_21464;
    }

    Data0 = -3;
    Data4 = 0x0C;

    Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );

    if (pSprite->field_20)
        return;

    if (Data4 == eTerrainType_WaterEdge)
        goto loc_21464;

    if (Data4 == eTerrainType_Water)
        goto loc_21464;

    if (Data4 == eTerrainType_Drop)
        goto loc_21464;

    if (Data4 == eTerrainType_Drop2)
        goto loc_21464;

    return;

loc_213F7:;

    if (mMission_Completed_Timer)
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
    Sprite_Create_Shadow( pSprite );
    return;

loc_21464:;
    pSprite->field_18 = eSprite_Explosion;
    pSprite->field_26 = 0x5F5F;
    pSprite->field_28 = 8;
}

void cFodder::Sprite_Create_Shadow( sSprite* pSprite ) {
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;

    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
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

void cFodder::Sprite_Handle_Grenade_Terrain_Check( sSprite* pSprite ) {
    int16 Data0 = -3;
    int16 Data4 = 2;

    if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
        goto loc_21599;

    if (Data4 == eTerrainType_Drop || Data4 == eTerrainType_Drop2) {

        if (pSprite->field_20 <= 1) {
            pSprite->field_12 = 1;
            return;
        }
    }
    //loc_21561
    if (    Data4 == eTerrainType_QuickSand || Data4 == eTerrainType_WaterEdge 
         || Data4 == eTerrainType_Water) {

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

    if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 )) {
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
    Sprite_XY_Restore( pSprite );
}

int16 cFodder::Sprite_Projectile_Collision_Check( sSprite* pSprite ) {

	// If the projectile doesn't belong to a human, check if it hit a squad member
    if (pSprite->field_22 != eSprite_PersonType_Human) {

        int16 Data8 = pSprite->field_0;
        int16 DataC = pSprite->field_0 + 6;
        int16 Data10 = pSprite->field_4 - 9;
        int16 Data14 = pSprite->field_4 - 4;
        word_3AA45 = 1;

        return Squad_Member_Sprite_Hit_In_Region( pSprite, Data8, DataC, Data10, Data14 );
    }

    //loc_21673
    int16 Data8 = pSprite->field_0 - 6;
    int16 DataC = pSprite->field_0 + 10;
    int16 Data10 = pSprite->field_4 - 10;
    int16 Data14 = pSprite->field_4 + 6;

    word_3AA45 = 1;
    sSprite* Data24 = 0;
    if (Sprite_Find_In_Region(pSprite, Data24, Data8, DataC, Data10, Data14 ) >= 0)
        return 0;

    if (Data24->field_18 == eSprite_Enemy) {

        Data24->field_5E = pSprite->field_5E;
        Data24->field_5D = pSprite->field_5D;
    }

    return -1;
}

void cFodder::Sprite_Create_Sparks( sSprite* pSprite, int16 pData18 ) {
    int16 Data0 = 2;
    sSprite* Data2C, *Data30;

    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
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

void cFodder::Sprite_Create_FireTrail( sSprite* pSprite ) {

    if (!pSprite->field_5C)
        return;

    //  cmp     ds:word_3B1A9, 14h
    int16 Data0 = 1;
    sSprite* Data2C = 0, *Data30 = 0;
    
    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
        return;

    Data2C->field_0 = pSprite->field_0;
    Data2C->field_2 = pSprite->field_2;
    Data2C->field_4 = pSprite->field_4;
    Data2C->field_6 = pSprite->field_6;

    if (dword_3B24B == 0) {
        Data2C->field_4 -= 3;
    } else {
        Data2C->field_4 += 2;
        dword_3B24B = 0;
    }

    Data2C->field_20 = pSprite->field_20;
    Data2C->field_52 = 0;
    Data2C->field_8 = 0xC6;
    Data2C->field_A = 0;
    Data2C->field_18 = eSprite_FireTrail;
}

void cFodder::Sprite_Turn_Into_Building_Explosion( sSprite* pSprite ) {
    int16 Data8 = pSprite->field_0;
    int16 DataC = pSprite->field_4;
    
    Sprite_Clear( pSprite );
    Sprite_Create_Building_Explosion( pSprite, Data8, DataC );
}

int16 cFodder::Sprite_Create_Building_Explosion_Wrapper( int16& pX, int16& pY ) {
    int16 Data0 = 1;
    sSprite* Data2C, *Data30;

    if (Sprite_Get_Free( Data0, Data2C, Data30 ))
        return -1;

    Sprite_Clear(Data2C);

    pX &= -16;
    pY &= -16;

    return Sprite_Create_Building_Explosion( Data2C, pX, pY );
}

int16 cFodder::Sprite_Create_Building_Explosion( sSprite* pData2C, int16& pX, int16& pY ) {

    pData2C->field_0 = pX;
    pData2C->field_4 = pY;
    pData2C->field_4 += 0x10;

    int16 Data4 = mGame_InputTicks;
    Data4 &= 3;
    Data4 += 5;

    Sound_Play( pData2C, Data4, 0x1E );

    pData2C->field_18 = eSprite_Building_Explosion;
    pData2C->field_8 = 0x8E;
    pData2C->field_A = 0;
    pData2C->field_12 = 1;
    pData2C->field_22 = eSprite_PersonType_None;
    pData2C->field_32 = -1;
    pData2C->field_2C = eSprite_Draw_Last;

    return 0;
}

int16 cFodder::Sprite_Create_Enemy( sSprite* pSprite, sSprite*& pData2C ) {
    
    if (mMission_Complete || mTroops_Enemy_Count >= 0x0A)
        return -1;
    int16 Data0 = 1;
    sSprite* Data30 = 0;

    if (Sprite_Get_Free2( Data0, pData2C, Data30 ))
        return -1;

//loc_21A1C:;
    Sprite_Clear( pData2C );
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
    pData2C->field_44 = (int8) Data4;
    Sprite_Enemy_Aggression_Update(pData2C);

    mSprite_Enemy_AggressionCreated_Count += 1;
    mSprite_Enemy_AggressionCreated_Count &= 0x0F;
    if (!mSprite_Enemy_AggressionCreated_Count && mSprite_Enemy_AggressionMax < 0x1E)
        ++mSprite_Enemy_AggressionMax;

    ++mTroops_Enemy_Count;
    return 0;
}

int16 cFodder::Sprite_Get_Free2( int16& pData0, sSprite*& pData2C, sSprite*& pData30 ) {
    if (mSprite_SpareUsed2)
        goto loc_21B4B;

    if (pData0 == 2)
        goto loc_21B91;

    pData2C = &mSprites[29];
    for (int16 Data1C = 29; Data1C >= 0; --Data1C, --pData2C ) {

        if (pData2C->field_0 == -32768) {
            Sprite_Clear( pData2C );
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
    pData2C = mSprites;

    for( int16 Data1C = 0x1C; Data1C >= 0; --Data1C, ++pData2C ) {
        
        if( pData2C->field_0 != -32768 )
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

void cFodder::Sprite_Enemy_Aggression_Update( sSprite* pData2C ) {
    int16 Data8 = mSprite_Enemy_AggressionNext;
    pData2C->field_62 = Data8;

    int16 Data4 = mSprite_Enemy_AggressionIncrement;
    Data8 += Data4;

    if (Data4 < 0)
        goto loc_21C42;

    if (Data8 < mSprite_Enemy_AggressionMax)
        goto loc_21C5E;

    Data8 = mSprite_Enemy_AggressionMax;
    Data4 = -Data4;
    goto loc_21C5E;

loc_21C42:;
    if (Data8 < 0)
        goto loc_21C52;

    if (Data8 > mSprite_Enemy_AggressionMin)
        goto loc_21C5E;

loc_21C52:;
    Data8 = mSprite_Enemy_AggressionMin;
    Data4 = -Data4;

loc_21C5E:;
    mSprite_Enemy_AggressionIncrement = Data4;
    mSprite_Enemy_AggressionNext = Data8;
}

void cFodder::Sprite_Create_Rank( ) {
    sSprite* Sprite = 0, *Data30 = 0;
    int16 Data0 = 1;

    if (Sprite_Get_Free( Data0, Sprite, Data30 ))
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

void cFodder::sub_21CD1( sSprite* pSprite ) {
	sSprite* Data30 = 0;

    mTroop_Weapon_Bullet_Disabled = false;
    mTroop_Weapon_Grenade_Disabled = true;
    mTroop_Weapon_Rocket_Disabled = true;
    mDirectionMod = 0;
    mSprite_FaceWeaponTarget = -1;

    int16 Dataa0, Data4, Data8, DataC, Data10;

    sSprite* Data0, *Following= 0, *Squad0_Member = 0;

    if (pSprite->field_5E < 0) {

        pSprite->field_5E += 1;
        mSprite_FaceWeaponTarget = 0;

        Data0 = mSquad_Leader;
        if (mSquad_Leader == INVALID_SPRITE_PTR)
            goto loc_22125;

	} else {

		Squad0_Member = mSquad_0_Sprites[pSprite->field_5E];
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

		Map_Get_Distance_BetweenPoints_Within_320(Dataa0, Data4, Data8, DataC);

		mSprite_Find_Distance = Dataa0;
		if (Dataa0 > 0xC8)
			goto loc_21E4A;

		if (Dataa0 <= 0x28)
			goto loc_21F77;

		Dataa0 = pSprite->field_0;
		Data4 = pSprite->field_4;
		Data8 = Squad0_Member->field_0;
		DataC = Squad0_Member->field_4;

		if (!sub_2A4A2(Dataa0, Data4, Data8, DataC)) {
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

					pSprite->field_5E += 1;
					if (pSprite->field_5E >= 0x1E)
						pSprite->field_5E = 0;

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

		pSprite->field_5E += 1;
		if (pSprite->field_5E > 0x1E)
			pSprite->field_5E = 0;

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
		pSprite->field_30 += 7;							// TODO: Is this an original bug and we should be adding to field_2E, code/logic wise it makes sense if it is

		pSprite->field_30 = Squad0_Member->field_4;
		pSprite->field_30 -= 0x0E;
		sub_2212A(pSprite);


	loc_22000:;
		if (mSprite_FaceWeaponTarget)
			goto loc_22125;

		if (mSquad_0_Sprites[pSprite->field_5E] == INVALID_SPRITE_PTR)
			goto loc_22125;

		Data0 = mSquad_0_Sprites[pSprite->field_5E];
	}

	// "Sort of" Random Movement Target
	// Depending on the sprite index, we add a factor to the X/Y target of a movement
    Data8 = (int16) (pSprite - mSprites);
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
    sub_2212A( pSprite );
}

void cFodder::sub_2212A( sSprite* pSprite ) {
    
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

int16 cFodder::Sprite_Handle_BuildingDoor_Explode( sSprite* pSprite ) {
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
    pSprite->field_44 = (int8) Data4;
    pSprite->field_36 = 0x78;

loc_22235:; // Door moving
    pSprite->field_44 -= 1;
    if (!pSprite->field_44)
        goto loc_2227F;

    Sprite_Movement_Calculate( pSprite );
    if (mSprite_Bullet_Destroy)
        goto loc_2227F;

    Data0 = -3;
    Data4 = 2;
    if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
        goto loc_2227F;

    if (pSprite->field_36 <= 0)
        goto loc_2227F;

    return -1;

loc_2227F:; // Door Finished
    pSprite->field_0 -= 8;
    pSprite->field_4 -= 8;
    
    Sprite_Turn_Into_Building_Explosion( pSprite );

    return -1;
}

int16 cFodder::sub_222A3( sSprite* pSprite ) {
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

    if (mMap_TileSet == eTileTypes_Ice || mMap_TileSet == eTileTypes_AFX)
        Data0 += 0x1C0;

    pSprite->field_10 = Data0;
    Data4 &= 3;
    Data4 += 5;

    pSprite->field_44 = (int8) Data4;
    pSprite->field_36 = 0x6E;

loc_22339:;
    pSprite->field_44 -= 1;
    if (pSprite->field_44 == 1)
        goto loc_22383;

    Sprite_Movement_Calculate( pSprite );
    if (mSprite_Bullet_Destroy)
        goto loc_22383;

    Data0 = -3;
    Data4 = 2;
    if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
        if (pSprite->field_36 > 0)
            return -1;

loc_22383:;

    pSprite->field_0 -= 8;
    if (pSprite->field_0 < 0)
        pSprite->field_0 = 0;

    pSprite->field_4 -= 8;
    Sprite_Turn_Into_Building_Explosion( pSprite );
    return -1;
}

void cFodder::sub_223B2( sSprite* pSprite ) {
    int16 Data8 = pSprite->field_3C;
    
    Data8 += mDirectionMod;
    Data8 &= 0x0E;

    pSprite->field_3C = Data8;

    const int16* Data28 = mSprite_AnimationPtrs[ pSprite->field_22 ];
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
int16 cFodder::Map_Get_Distance_Between_Sprite_And_Squadleader( sSprite* pSprite, int16& pData0 ) {
    int16 Data4, Data8, DataC, Data10;

    // Current Squad in vehicle?
    if (mSquad_CurrentVehicle)
        return 1;

    // No Squad Leader?
    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0 )
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

    Map_Get_Distance_BetweenPoints( pData0, Data4, Data8, Data10, DataC );
    if (pData0 >= 6)
        return 1;

    Data4 = 0;
    return 0;
}

int16 cFodder::Sprite_Create_Rocket( sSprite* pSprite ) {
    sSprite* Data2C = 0, *Data30 = 0, *Data34 = 0;
    int16 Data0, Data4, Data8, DataC;

    if (mMission_Completed_Timer) {
    loc_22592:;

        if (pSprite == mSquad_Leader)
            mMouse_Button_LeftRight_Toggle = 0;

        return -1;
    }

    if (byte_3A9D6[pSprite->field_22] == 2)
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
        Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
        if (Data0 >= 0x82)
            goto loc_22592;
    }

    Data0 = 2;
    Sprite_Get_Free(Data0,Data2C,Data30);

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

    ++byte_3A9D6[pSprite->field_22];
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

    // Amiga Plus always has homing missiles
    if (pSprite->field_22 == eSprite_PersonType_Human &&
        (mVersion->mVersion == eVersion::AmigaPlus || (pSprite->field_75 & eSprite_Flag_HomingMissiles)) ) {

        // Within lock on range?
        if (Sprite_Homing_LockInRange( pSprite, Data34 )) {

            Data2C->field_1A_sprite = Data34;
            Data2C->field_18 = eSprite_MissileHoming2;
            Data2C->field_6A = 0x10000;
            Data2C->field_26 = pSprite->field_2E;
            Data2C->field_28 = pSprite->field_30;
            Data2C->field_28 += 0x10;
            Data2C->field_38 = eSprite_Anim_None;
            Data2C->field_36 = 0;
            Data30->field_36 = 0;

        } else {
            Data2C->field_32 = 0;
            Data2C->field_18 = eSprite_Missile;
        }

    } else {

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
        mMouse_Button_LeftRight_Toggle = 0;

    return 0;
}

int16 cFodder::Sprite_Homing_LockInRange( sSprite* pSprite, sSprite*& pFoundSprite ) {

    int16 MouseX = mMouseX;
    int16 MouseY = mMouseY;

    MouseX += mCamera_Adjust_Col >> 16;
    MouseY += mCamera_Adjust_Row >> 16;

    MouseX -= 0x10;
    if (!MouseX)
        MouseX = 1;

    MouseY += 0x08;
    
    pFoundSprite = mSprites;
    for (int16 Data1C = 0x2B; Data1C >= 0; --Data1C, ++pFoundSprite) {

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
        
        Map_Get_Distance_BetweenPoints( Data0, Data4, MouseX, Distance, MouseY );

        MouseX = S_Data8;
        MouseY = S_DataC;

        if (Data0 < 0x16) {
            Data0 = -1;
            return -1;
        }
    }
    
    return 0;
}

void cFodder::Sprite_Projectile_HitTarget( sSprite* pSprite ) {

    byte_3A9D6[pSprite->field_22] -= 1;
}

void cFodder::Sprite_Destroy_Wrapper_At_TopLeft( sSprite* pSprite ) {
    pSprite->field_0 = 1;
    pSprite->field_4 = 1;

    Sprite_Destroy_Wrapper_2( pSprite );
}

int16 cFodder::Sprite_Destroy_Wrapper_2( sSprite* pSprite ) {
    return Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Player_InVehicle( sSprite* pSprite ) {
    pSprite->field_8 = 0x7C;
    pSprite->field_A = 0;
    pSprite->field_38 = eSprite_Anim_None;
    
    sSprite* Data24 = pSprite->field_6A_sprite;
    if( Data24->field_0 == -32768 || Data24->field_18 == eSprite_Explosion ) {
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

void cFodder::Recruit_CheckLoadSaveButtons() {
    
    if( mRecruit_Button_Load_Pressed ) {
        Game_Load();
        return;
    } 

    if( mRecruit_Button_Save_Pressed ) {
        Game_Save_Wrapper2();
    }
}

void cFodder::Game_Setup( int16 pStartMap ) {
    Game_ClearVariables();

    mIntroDone = false;
    mMission_Complete = 0;
    mMapNumber = pStartMap;

    Squad_Clear();
    Heroes_Clear();

    mGraveRankPtr = mGraveRanks;
    mGraveRanks[0] = -1;

    mMission_Phases_Remaining = 1;
    mMissionNumber = 0;

    Mission_Phase_Next();

    mMission_TryAgain = -1;

    mGraphics->Load_pStuff();
}

void cFodder::Playground() {
    //return;
    mImageFaded = -1;
    Map_Load();

    mGraphics->PaletteSet();

    Recruit_Truck_Anim_Prepare();
    sub_16C6C();

    mGraphics->SetActiveSpriteSheet(eGFX_RECRUIT);
    //mGraphics->Recruit_Draw_Hill();

    Recruit_Copy_Sprites();

    if (mVersion->mPlatform == ePlatform::Amiga) {

        ((cGraphics_Amiga*)mGraphics)->Hill_Prepare_Overlays();
    }

    // Load Icon

    mImage->palette_FadeTowardNew();
    mImage->Save();
    mString_GapCharID = 0x25;
    mGUI_Print_String_To_Sidebar = false;

    int32 SpriteID = 0xB;
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
            mGraphics->SetActiveSpriteSheet(eGFX_HILL);
            GUI_Draw_Frame_8(SpriteID, Frame, 65, 65);
        }

        if (mImageFaded)
            mImageFaded = mImage->palette_FadeTowardNew();

        Mouse_Inputs_Get();
        Mouse_DrawCursor();

        // Q
        if (mKeyCode == 0x14) {
            --SpriteID;
        }
        // W
        if (mKeyCode == 0x1A) {
            ++SpriteID;
        }

        // A
        if (mKeyCode == 0x04 ) {
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

        g_Window.RenderAt(mImage);
        g_Window.FrameEnd();
        mImage->Restore();
    }
}

int16 cFodder::Recruit_Show() {

    if (mCustom_Mode != eCustomMode_Map) {
        Map_Load();
        Map_Load_Sprites();

        Mission_Troop_Count();
        Mission_Troop_Sort();
        Mission_Troop_Prepare(true);
        Mission_Troop_Attach_Sprites();

    } else {
		if (mVersion->mName == "Random Map") {

			Map_Create(mTileTypes[0], 0, 28, 22, true);
			Map_Save("Data/Custom/Maps/random.map");

			mCampaign.LoadCustomMap("random.map");

			mCampaign.setRandom(true);
			mCampaign.setAggression();
			mCampaign.setGoals({ eGoal_Kill_All_Enemy, eGoal_Destroy_Enemy_Buildings });
		} 
		else {
			Custom_ShowMapSelection();
		}

        if (mCustom_Mode == eCustomMode_None)
            return -1;
    }

    WindowTitleSet(false);

    // Single Map does not have a recruit screen
    if (mCustom_Mode != eCustomMode_Map) {

        // Retail / Custom set show the Recruitment Hill
        if (mVersion->mRelease == eRelease::Retail || mCustom_Mode == eCustomMode_Set) {

            // Recruit Screen
            if (Recruit_Loop())
                return -1;

            Recruit_CheckLoadSaveButtons();

            // Did we just load/save a game?
            if (mRecruit_Button_Load_Pressed || mRecruit_Button_Save_Pressed) {
                mMission_Restart = -1;
                mMission_Recruitment = -1;
                mMission_Aborted = true;
                return -3;
            }
        } else {

            // Amiga demos have a menu
            if (mVersion->mPlatform == ePlatform::Amiga) {

                // But not custom games
                if (mCustom_Mode == eCustomMode_None) {

                    if (Demo_Amiga_ShowMenu())
                        return -1;
                }
            }
        }
    }

    mMission_Restart = 0;
    Mission_Memory_Backup();

    // Retail or Custom Mode
    if (mVersion->mRelease == eRelease::Retail || 
        mCustom_Mode != eCustomMode_None) {
        Map_Load();

        // Show the intro for the briefing screen
        Mission_Intro_Play();
    }
    
    mGraphics->Load_pStuff();

    return 0;
}

void cFodder::Start(int16 pStartMap) {

Start:;
	mCampaign.Clear();
    mVersionDefault = 0;
    mVersion = 0;
    VersionLoad(g_AvailableDataVersions[0]);

    //Playground();

    // Play the intro
    if (!mOpenFodder_Intro_Done) {

        mMissionNumber = 0;

        // Random intro
        mMap_TileSet = static_cast<eTileTypes>(((uint8)tool_RandomGet()) % eTileTypes_Hid);

        Mission_Intro_Play();
        mOpenFodder_Intro_Done = true;
    }

    // Select campaign menu
    Campaign_Selection();

    // Exit pushed?
    if (mGUI_SaveLoadAction == 1)
        return;

    if (mVersion->mRelease == eRelease::Demo)
        pStartMap = 0;

    Mouse_Setup();
    Mouse_Inputs_Get();

    // Game Loop
    for (;;) {

        Game_Setup(pStartMap);

        if (Mission_Loop() == -1)
            goto Start;
    }
}

int16 cFodder::Mission_Loop() {

    //loc_1042E:;
    for (;;) {

        // Mission completed?
        if (!mMission_Aborted && !mMission_TryAgain) {

            // Demo / Custom Mission restart
            if (mVersion->mRelease == eRelease::Demo && mCustom_Mode != eCustomMode_Set)
                break;

            // Reached last map in this mission set?
            if (++mMapNumber == mCampaign.getMapCount()) {
                WonGame();
                return -1;
            }

            Mission_Phase_Next();
        }

        // loc_1045F
        // Prepare the next mission
        Mission_Memory_Clear();
        Mission_Prepare_Squads();
        sub_10DEC();

        mInput_Enabled = 0;

        // Show the intro for retail releases
        if (mVersion->mRelease != eRelease::Demo) {
            if (!mIntroDone)
                intro();
        }

        //loc_10496
        Squad_Set_Squad_Leader();
        Sprite_Clear_All();

        // Prepare a new game?
        if (mMission_Recruitment) {
            mMission_Recruitment = 0;
                
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

        WindowTitleSet( true );

        Map_Load();
        Map_Load_Sprites();
        Map_Overview_Prepare();

        //loc_10513
        // Show the pre ready Briefing Screen
        Briefing_Show_PreReady();

        // Prepare Squads
        Mission_Troop_Count();
        Mission_Troop_Sort();
        Mission_Troop_Prepare(false);
        Mission_Troop_Attach_Sprites();

        mMission_Aborted = false;


        // Show the Briefing screen for Retail and Custom 
        if (mVersion->mRelease == eRelease::Retail || mCustom_Mode == eCustomMode_Set || mCampaign.isRandom()) {
            Briefing_Show_Ready();

            // Aborted?
            if (mBriefing_Aborted == -1) {

				if (mCampaign.isRandom())
					return -1;

                Mission_Memory_Restore();

                mMission_Restart = -1;
                mMission_Recruitment = -1;
                mMission_Aborted = true;

                mSound->Music_Play( 0 );
                continue;
            }
        }

        Map_Load();
        mGraphics->SetActiveSpriteSheet( eGFX_IN_GAME );
    
        MapTiles_Draw();
        Camera_Reset();
            
        Mouse_Inputs_Get();
        Sprite_Frame_Modifier_Update();

        mSound->Stop();

        Sprite_Handle_Loop();
        //seg000:05D1

        Sprite_Bullet_SetData();
        Sprite_Create_Rank( );

        mCamera_Start_Adjust = 1;
        mCamera_Position_X = mSprites[0].field_0;
        mCamera_Position_Y = mSprites[0].field_4;

        // Is map 17 x 12
        {
            if (mMapWidth == 17) {
                if (mMapHeight == 12)
                    word_3ABB7 = -1;
            }
        }

        mGUI_Elements[0].field_0 = 0;
        mInput_Enabled = -1;
        sub_11CAD();

        mGUI_Mouse_Modifier_X = 0;
        mGUI_Mouse_Modifier_Y = 4;
        mCamera_Start_Adjust = 1;

        Squad_Prepare_GrenadesAndRockets();
        Sprite_Aggression_Set();
        Mission_Phase_Goals_Set();

        g_Graphics.PaletteSet();

        mImageFaded = -1;

        GUI_Sidebar_Prepare_Squads();
        Squad_Select_Grenades();
        Map_Clear_Destroy_Tiles();
        Sprite_Count_HelicopterCallPads();
        Mission_Set_Final_TimeRemaining();

        mMouseSpriteNew = eSprite_pStuff_Mouse_Cursor;

        mMission_Aborted = false;
        mMission_Paused = 0;
        mMission_In_Progress = true;
        mMission_Finished = 0;
        mMission_ShowMapOverview = 0;

        if (!Map_Loop()) {
            mKeyCode = 0;
            mMission_In_Progress = false;
            Squad_Member_PhaseCount();
            mMission_TryingAgain = -1;
        } else {
            mKeyCode = 0;
            mMission_In_Progress = false;
            if (!mRecruits_Available_Count) {

                Service_Show();
                break;
            }
        }

        //loc_106F1
        if (mMission_TryAgain) {
            mMission_TryingAgain = -1;
            continue;
        }

        // Demo/Single mission 
        if (mVersion->mRelease == eRelease::Demo && mCustom_Mode != eCustomMode_Set) {

            // Custom can do the service screen
            if(!mMission_Aborted && mVersion->mVersion == eVersion::Custom)
                Service_Show();

            break;
        }

        if (mMission_Aborted)
            continue;

        if (mMission_Phases_Remaining > 1)
            continue;

        Service_Show();
    }

    return 0;
}

void cFodder::MapTiles_Draw() {

    mMapTile_ColumnOffset = 0;
    mMapTile_RowOffset = 0;

    mMapTile_Ptr = (0x60 - 8) - (mMapWidth << 1);
    mMapTile_MovedHorizontal = 0;
    mMapTile_MovedVertical = 0;
    mMapTile_Column_CurrentScreen = 0;
    mMapTile_Row_CurrentScreen = 0;

    g_Graphics.MapTiles_Draw();
}

void cFodder::Exit( unsigned int pExitCode ) {

    exit( pExitCode );
}

void cFodder::GUI_Sidebar_Grenades_Draw( ) {
    int16 Data0 = mGUI_Loop_Squad_Current;
    int16 Data8, DataC, Data4, Data10;

    if (!mSquad_Grenades[mGUI_Loop_Squad_Current])
        goto loc_2ED63;

    if (mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] != eWeapon_Grenade)
        goto loc_2ED63;

    if (!mGUI_Loop_Is_CurrentSquad) {
    loc_2ED63:;
        Data0 = 0xC4;
    }
    else {
        Data0 = 0xC3;
    }

    // Draw Grenade Icon
    Data8 = 0;
    DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC );
    
    Data0 = mSquad_Grenades[mGUI_Loop_Squad_Current];
    if (!Data0)
        return;

    Data4 = 0;
    Data8 = 0x0D;
    DataC = mGUI_Loop_Draw_Y;
    DataC += 0x19;
    Data10 = 0xAF;

    if (mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] != eWeapon_Grenade)
        goto loc_2EDF7;

    if (mGUI_Loop_Is_CurrentSquad)
        goto loc_2EDFD;

loc_2EDF7:;
    Data10 = 0xB9;

loc_2EDFD:;
    GUI_Sidebar_Number_Draw( Data0, Data4, Data8, DataC, Data10 );

}

void cFodder::GUI_Prepare_Button_Grenade() {
    if (!mGUI_Loop_Is_CurrentSquad)
        return;

    sGUI_Element* NewElement = mGUI_NextFreeElement;
    NewElement->field_0 = &cFodder::GUI_Button_NoAction;
    NewElement->mX = 0;
    NewElement->mWidth = 0x0C;
    NewElement->mY =  mGUI_Loop_Draw_Y + 0x0E;
    NewElement->mHeight = 0x14;
    NewElement->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_Grenades;

    ++NewElement;
    GUI_ClearElement( NewElement );
}

void cFodder::GUI_Handle_Button_Grenades() {
    
    if (Mouse_Button_Left_Toggled() < 0)
        return;

    if (!GUI_Sidebar_SelectedTroops_Count()) {
        Squad_Select_Grenades();
        return;
    }

    ++mSquad_Grenade_SplitMode;
    if (mSquad_Grenade_SplitMode >= (eSquad_Weapon_Split_None+1))
        mSquad_Grenade_SplitMode = eSquad_Weapon_Split_All;

    sub_303AE();

}

void cFodder::Squad_Select_Grenades() {

    if (mSquad_CurrentWeapon[mSquad_Selected] == eWeapon_Grenade)
        return;

    mSquad_CurrentWeapon[mSquad_Selected] = eWeapon_Grenade;
    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
}

void cFodder::GUI_Sidebar_TroopList_Draw() {

    int16 Data0 = 0;
    int8 Data4 = mSquads_TroopCount[mGUI_Loop_Squad_Current];
    int16 DataC = mGUI_Loop_Draw_Y;

    int8 Data4_Saved = Data4;

    DataC += 0x22;
    Data4 -= 1;
    if (Data4 < 0)
        goto loc_2F1BC;

    for (;Data4 >= 0;--Data4) {

        if (!mGUI_Loop_Is_CurrentSquad)
            Data0 = 0xAB;
        else
            Data0 = 0xA9;

        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, 0, DataC );
        DataC += 0x0C;
    }
loc_2F1BC:;

    mGUI_Squad_NextDraw_Y = DataC;
    Data4 = Data4_Saved;
    if (Data4 < 0)
        return;

    sMission_Troop* Data38 = mMission_Troops;

    mSidebar_Draw_Y = mGUI_Loop_Draw_Y + 0x23;

    // Loop the squad
    for (int16 word_3A061 = 7; word_3A061 >= 0; --word_3A061, ++Data38 ) {
        
        sSprite* Data34 = Data38->mSprite;

        if (Data34 == INVALID_SPRITE_PTR || Data34 == 0)
            continue;

        if (mGUI_Loop_Squad_Current != Data34->field_32)
            continue;

        // If sliding, or entering a vehicle, or invincible
        if (Data34->field_38 >= eSprite_Anim_Slide1 || (Data34->field_75 & eSprite_Flag_Invincibility))
            goto loc_2F25C;

        // If hit/dying, no longer render the troop name
        if (Data34->field_38 != eSprite_Anim_None)
            continue;

    loc_2F25C:;

        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data38->mRank + 9, 0x00, mSidebar_Draw_Y );
        mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data38->mRank + 9, 0x23, mSidebar_Draw_Y );

        const sRecruit* Data28 = &mRecruits[Data38->mRecruitID];
        if (!mGUI_Loop_Is_CurrentSquad)
            Data0 = 2;
        else
            Data0 = Data38->mSelected & 1;
        
        //loc_2F318
        GUI_Sidebar_TroopList_Name_Draw( Data0, 3, 0x23, mSidebar_Draw_Y + 2, Data28->mName );
        mSidebar_Draw_Y += 0x0C;
    }
}

void cFodder::GUI_Sidebar_TroopList_Name_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, const char* pData28 ) {

    word_3AA21 =  mGUI_Sidebar_TroopList_Sprite_Modifier[pData0];

    int16 Data14;

    for (Data14 = 0; Data14 <= mGUI_Sidebar_TroopList_Name_BreakOnSpace; ++Data14) {

        if (mGUI_Sidebar_TroopList_Name_BreakOnSpace == 5) {

            if (pData28[Data14] == 0x20)
                break;
        }

        if (pData28[Data14] == 0)
            break;

    }

    Data14 <<= 2;

    int16 Data18 = 0x30;
    Data18 -= Data14;
    Data18 >>= 1;
    word_3A05F = Data18;

    for( Data14 = 0; Data14 <= mGUI_Sidebar_TroopList_Name_BreakOnSpace; ++Data14 ) {
            
        if( pData28[Data14] != 0x20 ) {
            pData0 = pData28[Data14];
            pData0 -= 0x41;
            pData0 += word_3AA21;
                
            pData8 = Data14;
            pData8 <<= 2;
            pData8 += word_3A05F;

            mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( pData0, pData8, pDataC );
        }
    }
}

void cFodder::GUI_Sidebar_SquadIcon_Update() {

    GUI_Sidebar_SquadIcon_Set();
    GUI_Sidebar_SquadIcon_Current_Draw();
}

void cFodder::GUI_Sidebar_SquadIcon_Current_Draw() {
    
    int16 Data0 = mGUI_Squad_Icon[mGUI_Loop_Squad_Current];

    const int16* Data20 = mGUI_Squad_Active_Icons;

    if (!mGUI_Loop_Is_CurrentSquad) 
        Data20 = mGUI_Squad_Inactive_Icons;

    Data0 = Data20[Data0];
    int16 Data8 = 0x0C;
    int16 DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC );
}

int16 cFodder::GUI_Sidebar_SquadIcon_Set() {
    int16 Data1C = 0;
    sSprite* Vehicle = 0;
    const sGUI_SquadVehicleIcon* SquadIcon = mGUI_Squad_Vehicle_Icons;

    // No Troops in Squad?
    if (!mSquads_TroopCount[mGUI_Loop_Squad_Current])
        return 0;

    // More than 1 troop in squad?
    if (mSquads_TroopCount[mGUI_Loop_Squad_Current] != 1)
        Data1C = 1;

    // Squad not in vehicle?
    if (!mSquad_CurrentVehicles[mGUI_Loop_Squad_Current])
        goto loc_2F593;


    Vehicle = mSquad_CurrentVehicles[mGUI_Loop_Squad_Current];

    for (;; ++SquadIcon) {
        if (SquadIcon->mVehicleType < 0)
            goto loc_2F593;

        if (Vehicle->field_6F == SquadIcon->mVehicleType)
            goto loc_2F586;
    }
loc_2F586:;
    Data1C = SquadIcon->mIconID;

loc_2F593:;

    // has status changed?
    if (Data1C == mGUI_Squad_Icon[mGUI_Loop_Squad_Current])
        return 0;

    mGUI_Squad_Icon[mGUI_Loop_Squad_Current] = Data1C;
    return -1;
}

void cFodder::GUI_Sidebar_Draw() {
    Mission_Final_TimeToDie();
    GUI_Sidebar_SquadIcons_Draw();
    
    int16 Data0 = 2;
    int16 Data4 = 2;
    
    int8* Data20 = &mGUI_RefreshSquadGrenades[2]; //byte_3AC35; 
    int8* Data24 = &mGUI_RefreshSquadRockets[2]; //byte_3AC3B;
    int16* Data28 = mSquad_Grenades;
    int16* Data2C = mSquad_Rockets;
    int16* Data30 = mSquad_CurrentWeapon;
    
    for( Data0 = 2; Data0 >= 0; --Data0 ) {
        
        if( ! *Data20 )
            goto loc_2F67A;
        
        if( *(Data28 + Data4) )
            goto loc_2F67A;
        
        if( *(Data30 + Data4) != 1 )
            goto loc_2F67A;
        
        *(Data30 + Data4) = 3;
        *Data24 = -1;
        
        loc_2F67A:;
        if(! *Data24 )
            goto loc_2F6B7;
        
        if( *(Data2C + Data4 ))
            goto loc_2F6B7;
        
        if( *(Data30 + Data4) != 3 )
            goto loc_2F6B7;
        
        *(Data30 + Data4) = 1;
        *(Data20) = -1;
        
    loc_2F6B7:;
        --Data20;
        --Data24;
        Data4--;
    }

    Data20 = &mGUI_RefreshSquadGrenades[2];
    Data24 = &mGUI_RefreshSquadRockets[2];

    for (int16 Data0 = 2; Data0 >= 0; --Data0, --Data20, --Data24) {

        if (!*Data20)
            goto loc_2F707;

        *Data20 = 0;

        GUI_Sidebar_Grenades_Draw( Data0 );

    loc_2F707:;

        if (!*Data24)
            continue;

        *Data24 = 0;

        GUI_Sidebar_Rockets_Draw( Data0 );
    }
}

void cFodder::GUI_Sidebar_SquadIcons_Draw() {
    
    mGUI_Loop_Squad_Current = 0;

    for (mGUI_Loop_Squad_Current = 0; mGUI_Loop_Squad_Current < 3; ++mGUI_Loop_Squad_Current) {

        mGUI_Loop_Draw_Y = word_3AC2D[mGUI_Loop_Squad_Current];

        if (!mSquads_TroopCount[mGUI_Loop_Draw_Y])
            continue;

        mGUI_Loop_Is_CurrentSquad = 0;

        if (mGUI_Loop_Squad_Current == mSquad_Selected)
            mGUI_Loop_Is_CurrentSquad = -1;

        if (GUI_Sidebar_SquadIcon_Set())
            GUI_Sidebar_SquadIcon_Current_Draw();
    }
}

void cFodder::GUI_Sidebar_Grenades_CurrentSquad_Draw( ) {
    GUI_Sidebar_Grenades_Draw( mSquad_Selected );
}

void cFodder::GUI_Sidebar_Grenades_Draw(  int16 pData0 ) {

    int16 Data4 = word_3AC2D[pData0];

    GUI_Loop_Draw_Prepare(pData0, Data4);
    GUI_Sidebar_Grenades_Draw();

    if (!mSquad_Grenades[mGUI_Loop_Squad_Current])
        return;

    if (!mGUI_Loop_Is_CurrentSquad)
        return;

    if (!GUI_Sidebar_SelectedTroops_Count())
        return;

    // Grenades not selected
    if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_None)
        return;

    int16 Data0 = 0xC7;
    Data0 += mSquad_Grenade_SplitMode;

    int16 Data8 = 0;
    int16 DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC );
}

void cFodder::GUI_Sidebar_Rockets_CurrentSquad_Draw( ) {

    GUI_Sidebar_Rockets_Draw( mSquad_Selected );
}

void cFodder::GUI_Sidebar_Rockets_Draw(  int16 pData0 ) {

    int16 Data4 = word_3AC2D[pData0];
    GUI_Loop_Draw_Prepare( pData0, Data4 );
    GUI_Sidebar_Rockets_Draw();

    if (!mSquad_Rockets[mGUI_Loop_Squad_Current])
        return;

    if (!mGUI_Loop_Is_CurrentSquad)
        return;

    if (!GUI_Sidebar_SelectedTroops_Count())
        return;

    if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_None)
        return;

    pData0 = 0xC7;
    pData0 += mSquad_Rocket_SplitMode;

    int16 Data8 = 0x24;
    int16 DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( pData0, Data8, DataC );
}

int16 cFodder::GUI_Sidebar_SelectedTroops_Count() {
    sMission_Troop* Data38 = mMission_Troops;

    int16 Data10 = 0;

    for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Data38) {

        if (Data38->mSprite == INVALID_SPRITE_PTR || Data38->mSprite == 0 )
            continue;

        if (mSquad_Selected != Data38->mSprite->field_32)
            continue;

        if (!(Data38->mSelected & 1))
            continue;

        Data10++;
    }

    int16 Data0 = mSquads_TroopCount[mSquad_Selected];
    if (Data10 == Data0)
            return 0;

    if (Data10)
        return Data10;

    Data0 = 0;
    return 0;
}

void cFodder::GUI_Sidebar_Setup() {
    
    int16 Data0 = mGUI_Sidebar_Setup++;

    switch (Data0) {
        case 0: //loc_2FA05
            sub_2FA05();
            break;

        case 1: //loc_2FA1D
            GUI_Sidebar_MapButton_Prepare();
            break;

        case 2: //loc_2FA44
            GUI_Sidebar_Squad0_Prepare();
            break;

        case 3: //loc_2FA5B
            GUI_Sidebar_Squad1_Prepare();
            break;

        case 4: //loc_2FA75
            GUI_Sidebar_Squad2_Prepare();
            break;

        case 5: //loc_2FA8F
            GUI_Sidebar_Ready();
            break;

        case 6:
            mGUI_Sidebar_Setup = -1;
            return;

        default:
            std::cout << "GUI_Sidebar_Setup(): Missing Function " << Data0;
            break;
    }
}

void cFodder::sub_2FA05() {
    mMouseDisabled = -1;

    if (word_3AC51)
        return; 

    Sidebar_Render_To_BackBuffer();
    word_3AC51 = -1;
}

void cFodder::GUI_Sidebar_MapButton_Prepare() {

    mGUI_NextFreeElement = mGUI_Elements;

    Sidebar_Clear_ScreenBufferPtr();

    GUI_Sidebar_MapButton_RenderWrapper();
}

void cFodder::GUI_Sidebar_Squad0_Prepare( ) {
    mGUI_Squad_NextDraw_Y = 0;

    GUI_Sidebar_Prepare( 0, 0 );
}

void cFodder::GUI_Sidebar_Squad1_Prepare( ) {
    int16 Data4 = mGUI_Squad_NextDraw_Y;
    if (Data4) 
        Data4 += 5;
    
    GUI_Sidebar_Prepare( 1, Data4 );
}

void cFodder::GUI_Sidebar_Squad2_Prepare( ) {
    int16 Data4 = mGUI_Squad_NextDraw_Y;

    if (Data4)
        Data4 += 5;

    GUI_Sidebar_Prepare( 2, Data4 );
}

void cFodder::GUI_Sidebar_Ready() {
    GUI_Sidebar_Add_Sidebar_Overlay();
    Sidebar_Render_To_ScreenBuffer();

    word_3AC51 = 0;
    mMouseDisabled = 0;
    mMouse_Button_LeftRight_Toggle = 0;
}

void cFodder::GUI_Sidebar_Rockets_Draw( ) {
    int16 Data0, Data8, DataC, Data4, Data14, Data10;

    if (!mSquad_Rockets[mGUI_Loop_Squad_Current])
        goto loc_2FAF6;

    if (mSquad_CurrentWeapon[mGUI_Loop_Squad_Current] != eWeapon_Rocket)
        goto loc_2FAF6;


    if (!mGUI_Loop_Is_CurrentSquad) {
        loc_2FAF6:;
        Data0 = 0xC6;
    }
    else {
        Data0 = 0xC5;
    }

    //loc_2FB04
    Data8 = 0x20;
    DataC = mGUI_Loop_Draw_Y;
    DataC += 0x0E;
    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( Data0, Data8, DataC );

    Data0 = mSquad_Rockets[mGUI_Loop_Squad_Current];
    if (!Data0)
        return;

    Data4 = 0x24;
    Data8 = 0x0D;
    DataC = mGUI_Loop_Draw_Y;
    DataC += 0x19;
    Data14 = mGUI_Loop_Squad_Current;

    if (mSquad_CurrentWeapon[Data14] != eWeapon_Rocket)
        goto loc_2FB8A;

    Data10 = 0xAF;
    if (!mGUI_Loop_Is_CurrentSquad) {
    loc_2FB8A:;
        Data10 = 0xB9;
    }

    GUI_Sidebar_Number_Draw( Data0, Data4, Data8, DataC, Data10 );
}

void cFodder::GUI_Prepare_Button_Rockets() {

    if (!mGUI_Loop_Is_CurrentSquad)
        return;

    sGUI_Element* NewElement = mGUI_NextFreeElement;
    NewElement->field_0 = &cFodder::GUI_Button_NoAction;
    NewElement->mX = 0x24;
    NewElement->mWidth = 0x0C;
    NewElement->mY = mGUI_Loop_Draw_Y + 0x0E;
    NewElement->mHeight = 0x14;
    NewElement->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_Rockets;
    ++NewElement;

    GUI_ClearElement( NewElement );
}

void cFodder::GUI_Handle_Button_Rockets() {

    if (Mouse_Button_Left_Toggled() < 0)
        return;

    if (!GUI_Sidebar_SelectedTroops_Count()) {
        Squad_Select_Rockets();
        return;
    }

    ++mSquad_Rocket_SplitMode;

    if (mSquad_Rocket_SplitMode >= (eSquad_Weapon_Split_None + 1))
        mSquad_Rocket_SplitMode = eSquad_Weapon_Split_All;

    GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper();
}

void cFodder::Squad_Select_Rockets() {
    
    if (mSquad_CurrentWeapon[mSquad_Selected] == eWeapon_Rocket)
        return;

    mSquad_CurrentWeapon[mSquad_Selected] = eWeapon_Rocket;
    mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
    mGUI_RefreshSquadGrenades[mSquad_Selected] = -1;
}

void cFodder::GUI_Prepare_Button_TroopName() {
    
    if (mSquad_Selected != mGUI_Loop_Squad_Current)
        return;

    for (int16 Data4 = 2; Data4 >= 0; --Data4) {
        if (!mSquads_TroopCount[Data4])
            goto loc_2FCF4;
    }

    return;

loc_2FCF4:;
    mGUI_TroopName_DrawOffset = mGUI_Loop_Draw_Y;
    sGUI_Element* Element = mGUI_NextFreeElement;

    Element->field_0 = &cFodder::GUI_Button_NoAction;
    Element->mX = 0;
    Element->mWidth = 0x2F;
    Element->mY = mGUI_Loop_Draw_Y + 0x22;

    Element->mHeight = mSquads_TroopCount[mSquad_Selected] * 0x0C;
    Element->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_TroopName;

    ++Element;
    GUI_ClearElement( Element );
}

void cFodder::GUI_Handle_Button_TroopName() {

    if (Mouse_Button_Left_Toggled() < 0) {
        if (Mouse_Button_Right_Toggled() < 0)
            return;
    }

    if (mSquad_Selected < 0)
        return;

    int16 Data0 = mMouseY;
    Data0 -= mGUI_Mouse_Modifier_Y;
    Data0 -= mGUI_TroopName_DrawOffset;
    Data0 -= 0x22;
    Data0 /= 0x0C;

    int16 Data4 = mSquads_TroopCount[mSquad_Selected];

    if (Data0 >= Data4)
        return;

    word_3A3BF = Data0;

    sMission_Troop* Data38 = mMission_Troops;
    Data4 = mSquad_Selected;
    int16 Data8 = word_3A3BF;

    for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Data38) {

        sSprite* Data34 = Data38->mSprite;

        if (Data34 == INVALID_SPRITE_PTR || Data34 == 0 )
            continue;

        if (Data4 != Data34->field_32)
            continue;

        --Data8;
        if (Data8 < 0)
            break;
    }

    Data38->mSelected ^= 1;

    const sRecruit* Data28 = &mRecruits[Data38->mRecruitID];
    Data0 = Data38->mSelected & 1;
    Data4 = 3;
    Data8 = word_3A3BF;
    Data8 *= 0x0C;

    Data8 += mGUI_TroopName_DrawOffset;
    Data8 += 0x25;

    int16 DataC = Data8;

    GUI_Sidebar_TroopList_Name_Draw( Data0, Data4, Data8, DataC, Data28->mName );
    sub_3049B();
    sub_3037A();
}

int16 cFodder::sub_2FF41() {
    
    if (!GUI_Sidebar_SelectedTroops_Count())
        return 0;

    int16 Data0;
    for (Data0 = 0; Data0 < 3; ++Data0) {
        if (!mSquads_TroopCount[Data0])
            goto loc_2FF79;
    }
    return 0;

loc_2FF79:;
    int16 Data14 = Data0;

    mSquad_JoiningTo = mSquad_Selected;
    int16 Data10 = Data14;

    sMapTarget* Dataa34 = mSquad_WalkTargets[Data10];
    Dataa34->asInt = -1;

    sMission_Troop* Member = mMission_Troops;

    for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Member ) {
        sSprite* Data34 = Member->mSprite;

        if (Data34 == INVALID_SPRITE_PTR || Data34 == 0)
            continue;

        if (mSquad_Selected != Data34->field_32)
            continue;

        if (!(Member->mSelected & 1))
            continue;

        Data34->field_32 = Data14;
        Data34->field_40 = 0;
        Data34->field_42 = 0;
        Data34->field_44 = 0;
        Data34->field_28 += 4;
    }

    mSquad_Selected = Data14;
    Mission_Troops_Clear_Selected();

    return -1;
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

void cFodder::GUI_Sidebar_MapButton_RenderWrapper() {

    // Don't display the map button on the final map
    if (mVersion->mRelease == eRelease::Retail && mMapNumber == 0x47)
        return;

    // No the map overview button in the Demo versions
    if (mVersion->mRelease == eRelease::Demo && mVersion->mVersion != eVersion::Custom)
        return;

    mGUI_Sidebar_MapButton_Prepared = 0;
    GUI_Sidebar_MapButton_Render();
}

void cFodder::GUI_Sidebar_Number_Draw( int16 pNumber, int16 pX, int16 pData8, int16 pY, int16 pData10 ) {
    pData10 -= 0x1A;

    std::stringstream Tmp;
    Tmp << tool_StripLeadingZero( tool_NumToString( pNumber ) );

    String_CalculateWidth( pData8, mFont_Sidebar_Width, Tmp.str() );
    mGUI_Print_String_To_Sidebar = -1;
    String_Print(  mFont_Sidebar_Width, pData10, mGUI_Temp_X + pX, pY, Tmp.str() );
    
    mGUI_Print_String_To_Sidebar = 0;
}

int16 cFodder::Mouse_Button_Left_Toggled() {
    if (mMouse_Button_Left_Toggle >= 0)
        return -1; 
    
    mMouse_Button_Left_Toggle = 1;
    return 1;
}

void cFodder::Squad_Member_Click_Check() {
    word_3A3BF = -1;
    int16 Data1C = 0x07;
    sMission_Troop* SquadMember = mMission_Troops;

    if (mSquad_Selected < 0)
        return;

    int16 ClickedX = mMouseX;
    int16 ClickedY = mMouseY;

    ClickedX += mCamera_Adjust_Col >> 16;
    ClickedY += mCamera_Adjust_Row >> 16;
    ClickedX -= 0x0F;
    ClickedY -= 3;

    for (auto& SquadMember : mMission_Troops) {

        if (SquadMember.mSprite == INVALID_SPRITE_PTR || SquadMember.mSprite == 0 )
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

        Squad_Walk_Target_Update( SquadMemberSprite->field_32 );

        break;
    }
}

void cFodder::Mission_Troops_Clear_Selected() {
    sMission_Troop* SquadMember = mMission_Troops;
    
    for (int16 Data1c = 7; Data1c >= 0; --Data1c, ++SquadMember) {
        SquadMember->mSelected &= 0;
    }
}

void cFodder::sub_303AE() {
    GUI_Sidebar_Grenades_CurrentSquad_Draw();
    GUI_Sidebar_Squad_Split_Icon_Draw();
}

void cFodder::sub_303B7() {
    if (mSquad_Selected < 0)
        return;

    mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr( mSquad_Selected, 0, mGUI_Loop_Draw_Y );
}

void cFodder::GUI_Sidebar_Squad_Split_Icon_Draw() {
    int16 EmptySquadID;

    // Loop each squad
    for (EmptySquadID = 0; EmptySquadID < 3; ++EmptySquadID) {

        // Find an empty squad
        if (!mSquads_TroopCount[EmptySquadID]) {

            if (mSquad_Selected < 0)
                return;

            // 3 Icons per Squad Slot
            int16 SquadIcon = (mSquad_Selected * 3);

            // Split into squad
            SquadIcon += EmptySquadID;

            int16 SplitIconID = mGUI_Squad_Split_Icons[SquadIcon];

            mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(SplitIconID, 0, mGUI_Loop_Draw_Y);
            return;
        }
    }
}

void cFodder::Sidebar_Render_To_BackBuffer() {

    for (int16 cx = 0; cx < 0x2000; ++cx){
        mSidebar_Back_Buffer[cx] = mSidebar_Screen_Buffer[cx];
    }

    mSidebar_Screen_BufferPtr = mSidebar_Back_Buffer;
}

/**
 * Copy 'mSidebar_Back_Buffer' to the Screen Buffer, and set the Ptr to the Screen Buffer
 */
void cFodder::Sidebar_Render_To_ScreenBuffer() {

    for (int16 cx = 0; cx < 0x2000; ++cx){
        mSidebar_Screen_Buffer[cx] = mSidebar_Back_Buffer[cx];
    }

    mSidebar_Screen_BufferPtr = mSidebar_Screen_Buffer;
}

void cFodder::Squad_Switch_Weapon() {
    
    if (!mInput_Enabled || mMission_Finished)
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
    if (mVersion->mRelease != eRelease::Retail || mVersion->mGame != eGame::CF1 )
        return;

    if (mMapNumber != 0x47)
        return;

    ++mMission_Final_TimeToDie_Ticker;
    if (mMission_Final_TimeToDie_Ticker < 40)
        return;

    mMission_Final_TimeToDie_Ticker = 0;

    --mMission_Final_TimeRemain;
    if (mMission_Final_TimeRemain < 0)
        mMission_Final_TimeRemain = 0;

    mGUI_Sidebar_TroopList_Name_BreakOnSpace = 0x0A;

    GUI_Sidebar_TroopList_Name_Draw( 0, 0, 0, 0xB7, "TIME TO DIE" );

    int16 di = 0x900;
    for (int16 dx = 4; dx > 0; --dx) {
        int16 di2 = di / 2;

        // Clear the sidebar buffer
        for (int16 cx = 0x12; cx > 0; --cx) {
            mSidebar_Screen_Buffer[di2] = 0;
            mSidebar_Screen_Buffer[di2+1] = 0;
            di2+=2;
        }
        di += 0x960;
    }

    GUI_Sidebar_Number_Draw( mMission_Final_TimeRemain, 0, 0x30, 0xC0, 0xAF );
    mGUI_Sidebar_TroopList_Name_BreakOnSpace = 5;
}

int16 cFodder::sub_305D5( sSprite*& pData20 ) {
    sSprite** Data30 = 0;
    int16 Data0 = mSquad_Selected;

    if (Data0 < 0)
        goto loc_306AD;

    Data30 = mSquads[Data0];
    if (*Data30 == INVALID_SPRITE_PTR || *Data30 == 0)
        goto loc_306AD;

    do {
        if (*Data30 ==  INVALID_SPRITE_PTR || *Data30 == 0 )
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

    } while (*Data30 != INVALID_SPRITE_PTR );
    //loc_3069A
    goto loc_306BE;

loc_306AD:;
    pData20 = INVALID_SPRITE_PTR;
    mSquad_Selected = -1;

loc_306BE:;
    if (mSquad_CurrentVehicle)
        return 1;

    mSquad_Leader = pData20;
    if (pData20 == INVALID_SPRITE_PTR || pData20 == 0 )
        return -1;

    return 1;
}

void cFodder::Mouse_Inputs_Check() {
    int16 Data0 = 0;
    int16 Data4 = 0;

    if (mMouseDisabled)
        return;

    if (mMission_In_Progress)
        Mouse_Cursor_Update();

    if (dword_3A030) {
        // TODO: Function pointer call, but appears not to be used
        //dword_3A030();
        return;
    }

    mGUI_Loop_Element = mGUI_Elements;

    for (;; ++mGUI_Loop_Element) {
        sGUI_Element* Data20 = mGUI_Loop_Element;

        if (Data20->field_0 == 0)
            break;

        if ((*this.*Data20->field_0)() < 0)
            return;

        Data20 = mGUI_Loop_Element;
        Data0 = mGUI_Mouse_Modifier_X + Data20->mX;

        int16 Data4 = mMouseX + 0x20;

        if (Data0 > Data4)
            continue;

        Data0 += Data20->mWidth;
        if (Data0 < Data4)
            continue;

        Data0 = mGUI_Mouse_Modifier_Y;
        Data0 += Data20->mY;
        if (Data0 > mMouseY)
            continue;

        Data0 += Data20->mHeight;
        if (Data0 < mMouseY)
            continue;

        (*this.*Data20->mMouseInsideFuncPtr)();
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

    if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0 )
        return;

    Data0 = mMouseX;
    Data4 = mMouseY;
    Data0 += mCamera_Adjust_Col >> 16;
    Data4 += mCamera_Adjust_Row >> 16;

    Data0 += -22;
    if (Data0 < 0)
        Data0 = 0;

    Data4 += -3;
    if (Data4 < 0)
        Data4 = 3;

    if (Data4 < 3)
        Data4 = 3;

    mCamera_Position_Column = Data0;
    mCamera_Position_Row = Data4;
    word_3A054 = 0;

    sMission_Troop* SquadMember = mMission_Troops;

    int8 Data14 = 0;

    for( auto& SquadMember : mMission_Troops) {

        if (SquadMember.mSprite == INVALID_SPRITE_PTR || SquadMember.mSprite == 0 )
            continue;
        
        // Not in the squad?
        if (mSquad_Selected != SquadMember.mSprite->field_32)
            continue;

        SquadMember.mSprite->field_44 = Data14;
        SquadMember.mSprite->field_2 = 0;
        SquadMember.mSprite->field_6 = 0;
    }

    int16 Data10 = mSquad_Leader->field_0;
    int16 DataC = mSquad_Leader->field_32;
    int16 Data8 = Data4;
    Data4 = Data0;

    Squad_Walk_Target_Set( Data4, Data8, mSquad_Leader->field_32, Data10 );

    // Reset the join target
    for( auto& JoinTargetSquad : mSquad_Join_TargetSquad) {

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

    word_3A9CE = -1;
    if (word_3A9B8 < 0) {
        word_3A9B8 = 0x30;

        if (mSquad_Leader)
            mSquad_Leader->field_4A = 0;
    }

    int16 TargetX = mMouseX;
    int16 TargetY = mMouseY;
    TargetX += mCamera_Adjust_Col >> 16;
    TargetY += mCamera_Adjust_Row >> 16;

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

    for (int16 Data0 = 2; Data0 >= 0; --Data0 ) {

        if (mSquads_TroopCount[Data0]) {
            Squad_Switch_To(Data0);
            return;
        }

    }
}

void cFodder::Squad_Switch_To( int16 pData0 ) {
    
    if (!mSquads_TroopCount[pData0])
        return;

    mSquad_Selected = pData0;
    sSprite* Data20 = 0;

    if (sub_305D5( Data20 ) >= 0) {
        mCamera_Position_X = Data20->field_0;
        mCamera_Position_Y = Data20->field_4;
        mCamera_Start_Adjust = 1;
        word_3A054 = 0;
    }

    mGUI_Sidebar_Setup = 0;
}
