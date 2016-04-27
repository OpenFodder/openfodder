/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield
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
#include "UnknownData.hpp"

#define INVALID_SPRITE_PTR (sSprite*) -1

cFodder::cFodder( bool pSkipIntro ) {
	
	mSkipIntro = pSkipIntro;
	mResources = 0;
	mGraphics = 0;
	mSound = 0;
	mWindow = new cWindow();
	mMapNumber = 0;

	mTicksDiff = 0;
	mTicks = 0;

	word_3A9F3 = 0;
	word_3A9F5 = 0;
	word_3D465 = 0;
	word_3D467 = 0;
	word_3D469 = 0;
	mIntroDone = 0;
	mErrorCode = 0;
	word_427D4 = -1;

	mMouseButtons = 0;

	mouse_Button_Status = 0;
	mouse_Pos_Column = 0;
	mouse_Pos_Row = 0;

	mButtonPressLeft = mButtonPressRight = 0;
	mMouse_Button_Left_Toggle = mMouse_Exit_Loop = 0;
	word_39F06 = 0;
	mMouse_Button_Right_Toggle = 0;
	mMouse_Button_LeftRight_Toggle = word_39F04 = 0;

	word_39FF2 = 0;
	word_39FF4 = 0;
	word_39FF6 = 0;
	word_39FF8 = 0;
	word_39FFA = 0;

	mEnemy_BuildingCount = 0;
	mMission_Aborted = 0;
	word_3AA43 = 0;
	word_3ABA7 = 0;
	word_3ABE7 = 0;
	word_3ABE9 = 0;
	word_3ABEB = 0;
	word_3ABFD = 0;
	word_3ABFB = 0;
	word_3B15D = 0;
	word_3B15F = 0;
	word_3B161 = 0;
	word_3B163 = 0;
	word_3B173 = 0;
	word_3B175 = 0;
	word_3B177 = 0;
	word_3B179 = 0;
	word_3B17B = 0;
	word_3B17D = 0;
	word_3B17F = 0;
	word_3B189 = 0;
	word_3B18B = 0;
	word_3B60E = 0;
	word_3B610 = 0;
	dword_3B30D = 0;

	mMapTilePtr = 0;

	mSoundEffectToPlay = 0;
	word_3B4F5 = 0;
	word_3BEC1 = 0;
	word_3BEC3 = 0;
	word_3E1B7 = 0;
	word_3E75B = 0;
	mCamera_Pan_RowCount = 0;

	mMouseSpriteCurrent = 0;
	word_40048 = 0;
	word_40054 = 0;
	mDrawSpriteFrameDataPtr = 0;
	word_42066 = 0;
	mDrawSpritePositionX = 0;
	mDrawSpritePositionY = 0;
	mDrawSpriteColumns = 0;
	mDrawSpriteRows = 0;
	byte_42070 = 0;
	byte_42071 = 0;
	word_42074 = 0;
	word_42076 = 0;
	mKeyCode = 0;

	dword_39F84 = 0;
	dword_39F88 = 0;
	dword_39F8C = 0;
	dword_39F90 = 0;

	mSurfaceMapOverview = 0;

	mMap = 0;

	word_3A3B9 = 0;
	word_3A3BB = 0;
	word_3A3BD = 0;
	word_3A3BF = 0;
	dword_3A3F9 = 0;
	word_3FA1F = -1;
	word_3FA21 = 0;
	mPaused = 0;

	word_42072 = 0;
	byte_427E6 = 0;
	byte_427EE = 0;

	word_44A2E = 0;
	word_44A30 = 0;
	word_44A32 = 0;
	word_44A34 = 0;
	byte_44AC0 = 0;

	for (unsigned int x = 0; x < 30; ++x) {
		stru_3A3FD[x].asInt = 0;
		stru_3A475[x].asInt = 0;
		stru_3A4ED[x].asInt = 0;
		stru_3A565[x].asInt = 0;
		stru_3A5DD[x].asInt = 0;
		stru_3A655[x].asInt = 0;
		stru_3A6CD[x].asInt = 0;
		stru_3A745[x].asInt = 0;
		stru_3A7BD[x].asInt = 0;
		stru_3A835[x].asInt = 0;
	}

	for (unsigned int x = 0; x < 0x18; ++x) {
		mMission_Save_Availability[x] = 0;
	}

	word_44475 = 0;

	stru_3DEDC[0] = { &cFodder::GUI_Button_NoAction, 0, 47, 0, 225, &cFodder::GUI_Button_NoAction2 };
	stru_3DEDC[1] = { 0, 0, 0, 0, 0, 0 };

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

	delete mResources;
	delete mGraphics;
	delete mSound;
}

int16 cFodder::Mission_Loop( ) {
	mImage->Save();

	for (;;) {
		Video_Sleep();

		sub_12018();
		Camera_Pan( );

		word_3A9FB = 0;
		Game_Handle();
		++word_390B0;
		word_39F06 = 0;
		word_3A9FB = -1;

		if (mCamera_Start_Adjust >= 0) {
			Camera_Refresh();
			mCamera_Start_Adjust = -1;
			continue;
		}

		//loc_1074E
		if (mGUI_Sidebar_Setup >= 0 && !mMission_TryAgain)
			GUI_Sidebar_Setup();
		else
			GUI_Sidebar_Refresh();

		//loc_10768
		Mission_Progress_Check();
		mHelicopterCallPadPressedCount = 0;
		if (word_3A9B8 >= 0)
			--word_3A9B8;

		sub_12AEE();

		if (word_3FA21 == -1 && mKeyCode == 0x1C) {
			mMission_Complete = -1;
			mKeyCode = 0;
		} else
			Mission_Phase_Goals_Check();

		//loc_1079C
		sub_2A470();
		Squad_Troops_Count();

		Mission_Sprites_Handle();
		Squad_Switch_Timer();
		mGraphics->sub_144A2();
		Mouse_DrawCursor();

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
			mImage->paletteFade();
			mPaused = -1;
		}

		if( mImageFaded == -1 )
			mImageFaded = mImage->paletteFade();

		sub_11FCD();
		if (mPaused == -1) {
			mPaused = 0;
		}

		if (mMission_ShowMapOverview) {
			if (mVersion->mRelease == eRelease::Retail)
				Mission_Map_Overview_Show();

			mMission_ShowMapOverview = 0;
		}

		if (word_3A3B9) {

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
		sub_12C69();
		sub_12B6E();

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

	if (!word_390A6 || !mMission_Paused) {

		++word_39F06;
		word_390A4 = -1;
		++word_390AE;

		Mouse_Inputs_Get();

		if (word_390A6) {
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

	if (!word_390A6)
		return;

	if (!mMission_In_Progress)
		return;

	if (mMission_Completed_Timer || word_3D469 || mMission_Complete || mMission_TryAgain || mMission_Aborted) {
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
	
	if (mMission_In_Progress == -1) {
		Camera_Speed_Reset();
		sub_12245();
		Camera_Calculate_Scroll();
		sub_120F6();
		Camera_Position_Update();
		sub_1096B();
		Camera_Pan_Toward_SquadLeader();
	}
}

void cFodder::sub_1096B() {
	sSprite* Data14 = mSquad_Leader;
	if(Data14 == INVALID_SPRITE_PTR || Data14 == 0 )
		return;
	
	int16 Data8 = Data14->field_0;
	Data8 += 0x18;
	
	int16 DataC = Data14->field_4;
	int16 Data18 = Data8;
	int16 Data1C = DataC;
	
	word_39F3A = 0x14;
	if(word_3ABFB) {
		word_3ABFB = 0;
		dword_39F36 = 0;
	}

	if( mMouseX > 0x0F )
		goto Mouse_In_Playfield;
	
	int16 Data0;
	int16 Data4;

	if( word_39F40 ) {
		Data0 = Data8;
		Data0 -= word_39F3C;
		mCamera_Position_Column += Data0;

		Data4 = DataC;
		Data4 -= word_39F3E;
		
		mCamera_Position_Row += Data4;
		word_39F3C = Data8;
		word_39F3E = DataC;
		
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
	word_39F3C = Data8;
	word_39F3E = DataC;
	Data0 = mMouseX;
	Data0 += mCamera_Adjust_Col >> 16;
	Data4 = mMouseY;
	Data4 += mCamera_Adjust_Row >> 16;

	int16 Data0_Saved = Data0;
	int16 Data4_Saved = Data4;
	int16 Data8_Saved = Data8;
	int16 DataC_Saved = DataC;
	
	Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
	
	if( mSquad_CurrentVehicle ) {
	
		if( Data0 >= 0x64 )
			Data0 = 0x64;
	} else {
		
		if( Data0 >= 0x8C )
			Data0 = 0x8C;
	}
	//loc_10AAA
	word_3ABFD = Data0;
	
	DataC = DataC_Saved;
	Data8 = Data8_Saved;
	Data4 = Data4_Saved;
	Data0 = Data0_Saved;
	

	if(Direction_Calculate(Data0, Data4, Data8,DataC) < 0 )
		return;
	
	Data8 = word_3EABF[Data4/2];
	Data4 += 0x80;
	Data4 &= 0X1FE;

	DataC = word_3EABF[Data4/2];
	
	int32 Dataa8 = Data8 * word_3ABFD;
	int32 DataaC = DataC * word_3ABFD;
	
	Data8 = Dataa8 >> 16;
	DataC = DataaC >> 16;
	
	Data18 += Data8;
	Data1C += DataC;
	Data18 -= 0x18;
	
	if(Data18 < 0 )
		Data18 = 0;

	mCamera_Position_Column = Data18;
	mCamera_Position_Row = Data1C;
	word_3A054 = 0;
}

void cFodder::Game_ClearVariables() {
	word_390A4 = 0;
	word_390A6 = 0;
	word_390AE = 0;
	word_390B0 = 0;
	word_390B8 = 0;
	mSprite_Enemy_Aggression_HalfOfMinPlusMax = 0;
	mSprite_Enemy_AggressionMin = 0;
	mSprite_Enemy_AggressionMax = 0;
	mSprite_Enemy_AggressionNext = 0;
	mSprite_Enemy_AggressionIncrement = 0;
	mMapNumber = 0;
	mMissionNumber = 0;
	mMissionPhase = 0;
	word_39096 = 0;
	word_3909A = 0;
	mSquad_AliveCount = 0;
	mSaved_MissionNumber = 0;
	mSaved_MapNumber = 0;
	word_390D4 = 0;

	for (unsigned int x = 0; x < 8; ++x) {
		mSquad_SpritePtrs[x] = 0;
	}

	mSprite_Enemy_AggressionCreated_Count = 0;
	word_390E8 = 0;
	mGame_Prepare = 0;
	word_390EC = 0;
	mMissionPhaseRemain = 0;
	mMissionPhases = 0;
	mRecruit_NextID = 0;
	word_390F8 = 0;

	for (unsigned int x = 0; x < 9; ++x) {
		mSquad[x].mRecruitID = 0;
		mSquad[x].mRank = 0;
		mSquad[x].mPhaseCount = 0;
		mSquad[x].mSprite = 0;
		mSquad[x].field_6 = 0;
		mSquad[x].field_8 = 0;
		mSquad[x].mNumberOfKills = 0;
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
	mMapPlayerTroopCount = 0;
	mSquadMemberCount = 0;
	mTroopsAvailable = 0;
}

void cFodder::Squad_Clear() {
	mSquad[8].mRecruitID = -1;
	mSquad[8].mSprite = INVALID_SPRITE_PTR;

	for (unsigned int x = 0; x < 8; ++x) {
		mSquad[x].mSprite = INVALID_SPRITE_PTR;
		mSquad[x].mRecruitID = -1;
		mSquad[x].mRank = 0;
		mSquad[x].mPhaseCount = 0;
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

	memcpy( word_397D8, &mMapNumber, End - Start );
}

void cFodder::Mission_Memory_Restore() {
	uint8* Start = (uint8*) &mMapNumber;
	uint8* End = (uint8*)&mButtonPressLeft;

	memcpy( &mMapNumber, word_397D8, End - Start );
}

void cFodder::Mission_Memory_Clear() {
	// Clear memory 2454 to 3B58
	mButtonPressLeft = 0;
	mButtonPressRight = 0;
	mMouse_Button_Left_Toggle = 0;
	mMouse_Button_Right_Toggle = 0;
	mMouse_Button_LeftRight_Toggle = 0;

	word_39F00 = 0;
	mMouse_Exit_Loop = 0;
	word_39F04 = 0;
	word_39F06 = 0;

	mCamera_Adjust_Row = 0;
	dword_39F36 = 0;
	word_39F3A = 0;
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
	word_39F66 = 0;
	dword_39F84 = 0;
	dword_39F88 = 0;
	dword_39F8C = 0;
	dword_39F90 = 0;

	word_39F94 = 0;
	word_39F96 = 0;
	dword_39F98 = 0;
	word_39F9C = 0;
	word_39F9E = 0;
	word_39FA0 = 0;
	word_39FA2 = 0;
	word_39FA4 = 0;
	word_39FA6 = 0;
	mCamera_Column = 0;
	mCamera_Row = 0;
	word_39FAC = 0;

	mSquad_Leader = 0;
	word_39FB2 = 0;
	word_39FB4 = 0;
	word_39FB6 = 0;
	word_39FB8 = 0;
	word_39FBA = 0;
	mSquad_Selected = 0;
	word_39FD2 = 0;
	word_39FD4 = 0;

	word_39FF2 = 0;
	word_39FF4 = 0;
	word_39FF6 = 0;
	word_39FF8 = 0;
	word_39FFA = 0;
/*
	dword_3A000 = 0;
	dword_3A004 = 0;
	dword_3A008 = 0;
*/
	mTroop_Cannot_Throw_Grenade = 0;
	mTroop_Cannot_Fire_Bullet = 0;
	word_3A010 = 0;
	word_3A016 = 0;
	word_3A01A = 0;
	word_3A024 = 0;
	dword_3A030 = 0;
	mCamera_Position_Column = 0;
	mCamera_Position_Row = 0;
	mCamera_Position_X = 0;

	for (int16 x = 0; x < 20; ++x)
		dword_3A071[x] = 0;

	for (int16 x = 0; x < 45; ++x) {
		dword_3A125[x] = 0;
		dword_3A1D9[x] = 0;
	}

	for (int16 x = 0; x < 64; ++x) {
		dword_3A291[x] = 0;
	}

	dword_3A391 = 0;
	dword_3A395 = 0;
	mTmp_FrameNumber = 0;
	word_3A3AB = 0;
	word_3A3AD = 0;
	word_3A3AF = 0;
	word_3A3A1 = 0;
	word_3A3A7 = 0;
	word_3A3A9 = 0;
	word_3A3B9 = 0;
	word_3A3BB = 0;
	word_3A3BD = 0;
	word_3A3BF = 0;
	word_3A8CF = 0;
	mMission_Aborted = 0;
	mSquad_SwitchWeapon = 0;
	word_3A9B8 = 0;
	for (uint8 x = 0; x < 3; ++x) {
		word_3A9BA[x] = 0;
		word_3A9C0[x] = 0;
	}

	word_3A9C6 = 0;
	dword_3A9C8 = 0;
	word_3A9CE = 0;
	mMission_Paused = 0;
	for (uint8 x = 0; x < 4; ++x) {
		byte_3A9D2[x] = 0;
		byte_3A9D6[x] = 0;
	}
	for (uint8 x = 0; x < 10; ++x) {
		byte_3A9DA[x] = 0;
	}

	word_3A9E4 = 0;
	word_3A9E6 = 0;
	word_3A9F3 = 0;
	word_3A9F5 = 0;
	mMouseSpriteNew = 0;
	word_3A9FB = 0;
	dword_3A9FD = 0;

	word_3AA01 = 0;
	mSprites_Found_Count = 0;

	for (uint16 x = 0; x < 3; ++x) {
		mSquad_Grenades[x] = 0;
		mSquad_Rockets[x] = 0;
		word_3AA11[x] = 0;
	}

	mTroops_Enemy_Count = 0;
	mHostage_Count = 0;
	mCamera_Start_Adjust = 0;
	word_3AA1D = 0;
	mCamera_Reached_Target = 0;
	word_3AA21 = 0;
	word_3AA41 = 0;
	word_3AA45 = 0;
	mSquad_Select_Timer = 0;
	word_3AA4B = 0;
	mMapWidth_Shifted = 0;
	mMapHeight_Shifted = 0;
	mMouseCursor_Enabled = 0;
	word_3AA55 = 0;
	word_3AA67 = 0;
	word_3AA71 = 0;
	word_3AAC7 = 0;
	dword_3AAC9 = 0;
	word_3AACD = 0;
	word_3AACF = 0;
	byte_3ABA9 = 0;
	word_3ABB1 = 0;
	mSquad_Member_Fire_CoolDown = 0;
	word_3ABB5 = 0;
	word_3ABB7 = 0;
	mSprite_Weapon_Data.mSpeed = 0;
	mSprite_Weapon_Data.mAliveTime = 0;
	mSprite_Weapon_Data.mCooldown = 0;
	mSprite_Weapon_Data.mDeviatePotential = 0;
	mSprite_Weapon_Data.field_8 = 0;

	word_3AAD1 = 0;
	word_3AB39 = 0;
	word_3ABAD = 0;
	word_3ABAF = 0;
	mSprite_Bullet_Time_Modifier = 0;
	mSprite_Bullet_Fire_Speed_Modifier = 0;
	word_3ABC7 = 0;
	dword_3ABC9 = 0;

	for (uint16 x = 0; x < 9; ++x )
		word_3ABFF[x] = 0;

	dword_3AC11 = 0;

	word_3AC19 = 0;
	mGUI_Loop_Squad_Current = 0;
	word_3AC1D = 0;
	mGUI_Squad_Current = 0;
	word_3AC2D[0] = 0;
	word_3AC2D[1] = 0;
	word_3AC2D[2] = 0;

	word_3AC21 = 0;
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
	word_3AC47 = 0;
	word_3AC49 = 0;
	word_3AC4B = 0;
	word_3AC4D = 0;
	word_3AC4F = 0;
	word_3AC51 = 0;

	word_3AF01 = 0;
	mMap_Destroy_Tile_X = 0;
	mMap_Destroy_Tile_Y = 0;

	for (uint16 x = 0; x < 128; ++x) {
		mMap_Destroy_Tiles[x].mX = 0;
		mMap_Destroy_Tiles[x].mY = 0;
	}

	word_3AF07 = 0;
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
	word_3AEFB = 0;

	word_3B15D = 0;
	word_3B161 = 0;
	word_3B163 = 0;
	word_3B173 = 0;
	word_3B175 = 0;
	word_3B1A9 = 0;
	word_3B1AB = 0;
	dword_3B1CB = 0;

	for (uint16 x = 0; x < 0x0F; ++x ) 
		word_3B1CF[x] = 0;

	mRecruit_Reached_Truck = 0;
	word_3B1F1 = 0;
	dword_3B1FB = 0;
	
	mSquad_CurrentVehicle = 0;
	mMission_In_Progress = 0;
	word_3B211 = 0;
	for (uint16 x = 0; x < 15; ++x) {
		dword_3B213[x] = 0;
	}

	for (uint16 x = 0; x < 3; ++x) {
		dword_3B24F[x] = 0;
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
	word_3B2F5 = 0;
	word_3B2F7 = 0;
	mGame_Load = 0;
	mGame_Save = 0;

	mGUI_Temp_X = 0;
	mGUI_Temp_Width = 0;
	mGUI_Temp_Y = 0;
	mGUI_Temp_Height = 0;
	mInputString_Position = 0;
	word_3B335 = 0;
	word_3B33D = 0;
	word_3B33F = 0;
	word_3B447 = 0;
	word_3B449 = 0;

	for (uint16 x = 0; x < 3; ++x)
		word_3B461[x] = 0;

	for (uint16 x = 0; x < 8; ++x)
		mPhase_Goals[x] = 0;

	dword_3B477 = 0;
	word_3B47B = 0;
	word_3B47D = 0;
	word_3B47F = 0;
	word_3B481 = 0;
	word_3B483 = 0;
	mHelicopterCallPadCount = 0;
	mHelicopterCallPadPressedCount = 0;

	for (uint16 x = 0; x < 16; ++x) {
		mSprite_TroopsAlive[x] = 0;
	}
	mHelicopterCall_X = 0;
	mHelicopterCall_Y = 0;
	word_3B4D3 = 0;
	word_3B4D5 = 0;
	word_3B4D7 = 0;
	word_3B4D9 = 0;
	mMission_ShowMapOverview = 0;
	word_3B4DD = 0;
	word_3B4DF = 0;
	word_3B4E9 = 0;
	word_3B4EB = 0;
	word_3B4ED[0] = 0;
	word_3B4ED[1] = 0;
	mMission_Finished = 0;
	word_3B4F3 = 0;
	word_3B4F5 = 0;
	dword_3B5F5 = 0;
}

void cFodder::Mission_Prepare_Squads() {
	mSquad_Grenade_SplitMode = eSquad_Weapon_Split_Half;
	mSquad_Rocket_SplitMode = eSquad_Weapon_Split_Half;
	word_3BDEB = 5;
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
	
	word_3BEB9 = 1;
	word_3BEBB = 1;
	word_3BEBD = 1;
	word_3BEC1 = 0x1A;
	word_3BEC3 = 0x12;
	word_3BEC9 = 0xC8;
	word_3BED5[0] = 2;
	word_3BED5[1] = 2;
	word_3BED5[2] = 2;
	word_3BED5[3] = 2;
	word_3BED5[4] = 2;
	word_3BEDF[0] = 1;
	word_3BEDF[1] = 1;
	word_3BEDF[2] = 1;
	word_3BEDF[3] = 1;
	word_3BEDF[4] = 1;
	word_3BEDF[5] = 0;
	word_3BEDF[6] = 0;
	word_3BEDF[7] = 0;
	word_3BEDF[8] = 0;
	word_3BEDF[9] = 0;

	mSprite_WalkTargets[0] = stru_3A3FD;
	mSprite_WalkTargets[1] = stru_3A475;
	mSprite_WalkTargets[2] = stru_3A4ED;
	mSprite_WalkTargets[3] = stru_3A565;
	mSprite_WalkTargets[4] = stru_3A5DD;
	mSprite_WalkTargets[5] = stru_3A655;
	mSprite_WalkTargets[6] = stru_3A6CD;
	mSprite_WalkTargets[7] = stru_3A745;
	mSprite_WalkTargets[8] = stru_3A7BD;
	mSprite_WalkTargets[9] = stru_3A835;

	byte_3BF1B[0] = -1;
	byte_3BF1B[1] = -1;
	byte_3BF1B[2] = -1;

	word_3BF1E[0] = 0;
	word_3BF1E[1] = 0;
	word_3BF1E[2] = 0;
	word_3BF1E[3] = 0;
	word_3BF1E[4] = 0;
	word_3BF1E[5] = 0;
}

void cFodder::sub_10DEC() {
	word_39FD4 = 0;
	word_3A054 = 0;
	dword_3A39D = 0;
	mSquad_Selected = 0;
	word_3A9A6[0] = 0;
	word_3A9A6[1] = 0;
	mMission_TryAgain = 0;
	mMission_Complete = 0;
	mMission_Completed_Timer = 0;
	dword_39F7C = 0;
	mMouseSpriteNew = 0x23;

	mSquads_TroopCount[0] = 0;
	mSquads_TroopCount[1] = 0;
	mSquads_TroopCount[2] = 0;
	mSquads_TroopCount[3] = 0;

	byte_3A05E = 0;
	word_3A8D5 = 0;
	word_3A8D7 = 0;
	word_3A8D9 = 0;
	dword_3A8DB = 0;
	
	for (uint16 x = 0; x < 200; ++x )
		byte_3A8DE[x] = 0;

	mCamera_Position_X = 0;
	mCamera_Position_Y = 0;
	word_3A06B = -1;
	word_3A28D = 0x0C;
	stru_3A3FD[0].asInt = -1;
	stru_3A475[0].asInt = -1;
	stru_3A4ED[0].asInt = -1;
	stru_3A565[0].asInt = -1;
	stru_3A5DD[0].asInt = -1;
	stru_3A655[0].asInt = -1;
	stru_3A6CD[0].asInt = -1;
	stru_3A745[0].asInt = -1;
	stru_3A7BD[0].asInt = -1;
	stru_3A835[0].asInt = -1;
}

void cFodder::Squad_Set_Squad_Leader() {
	dword_39F98 = 0;
	word_39F9C = 0;
	word_39F9E = 0;
	mSprites[0].field_0 = -32768;
	mSquad_Leader = &mSprites[0];
}

void cFodder::Sprite_Clear_All_Wrapper() {
	Sprite_Clear_All();

/*	dword_3A000 = 0;
	dword_3A004 = 0;
	dword_3A008 = 0;*/

}

void cFodder::Sprite_Clear_All() {

	sSprite* Data = mSprites;
	for( int16 count = 0x2C; count >= 0; --count,++Data ) {
		
		Sprite_Clear( Data );
	}

	mSprites[44].field_0 = -1;
}

void cFodder::Map_Load_Sprites() {
	
	std::string Filename_Spt = map_Filename_SptGet();

	mMapSptSize = g_Resource.fileLoadTo( Filename_Spt, (uint8*) mMapSptPtr );
	tool_EndianSwap( (uint8*) mMapSptPtr, mMapSptSize );

	mTroops_Enemy_Count = 0;
	mHostage_Count = 0;
	
	uint16* SptPtr = mMapSptPtr;
	sSprite* Sprite = mSprites;
	
	uint16* SptFileEnd = SptPtr + (mMapSptSize / 2);
	int16 dword_37ABC = 0x0A;

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
		
		if( Sprite->field_18 == eSprite_Hostage_2 || Sprite->field_18 == eSprite_Hostage ) {
				
			++mHostage_Count;
		}
		
		if (Sprite->field_18 == eSprite_Player) {

			++HumanCount;
			Sprite->field_4A = 0;
		} else {

			if (Sprite->field_18 == eSprite_Enemy_Rocket) {
				Sprite->field_22 = 1;
				++mTroops_Enemy_Count;
				
			} else {
				if (Sprite->field_18 == eSprite_Enemy)
					++mTroops_Enemy_Count;
			}
			
			dword_37ABC += 0x0A;
			if( Sprite->field_62 > 4 )
				dword_37ABC = 0;
			
			Sprite->field_4A = dword_37ABC;
			
		}
		// 114B
	}
}

std::string cFodder::map_Filename_Get() {
	std::stringstream	filename;
	
	filename << "mapm";
	filename << (mMapNumber + 1);

	return filename.str();
}

std::string cFodder::map_Filename_MapGet() {
	std::string	filename = map_Filename_Get();

	filename.append(".map");

	return filename;
}

std::string cFodder::map_Filename_SptGet() {
	std::string	filename = map_Filename_Get();

	filename.append(".spt");

	return filename;
}

void cFodder::Squad_Member_Count() {
	mMapPlayerTroopCount = 0;
	sSprite* Sprite = mSprites;
	
	for( int16 mTmpCount = 0x1D; mTmpCount > 0; --mTmpCount, ++Sprite ) {
		if( Sprite->field_0 != -32768 ) {
			
			if( Sprite->field_18 == eSprite_Player )
				++mMapPlayerTroopCount;
		}
	}
	
	mSquadMemberCount = 0;
	
	for( int16 x = 7; x >= 0; --x ) {
		
		if( mSquad[x].mRecruitID != -1 ) {
			--mMapPlayerTroopCount;
			++mSquadMemberCount;
		}
	}
	
	int16 ax = mMapPlayerTroopCount;
	
	if( mMapPlayerTroopCount >= 0 ) {

		if( ax > mSquad_AliveCount )
			ax = mSquad_AliveCount;
		
		mMapPlayerTroopCount = ax;
		mSquad_AliveCount -= ax;
	}
	
	ax += mSquadMemberCount;
	mTroopsAvailable = ax;
}

void cFodder::Squad_Member_Sort() {

	// Remove any 'dead' troops
	for (int16 Data1c = 7; Data1c >= 0; --Data1c) {
		sSquad_Member* Data20 = mSquad;

		for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {

			if (Data20->mRecruitID == -1) {

				sSquad_Member* Data24 = Data20 + 1;

				*Data20 = *Data24;
				Data24->mRecruitID = -1;
				Data24->mRank = 0;
			}
		}
	}
	
	// Sort by kills

	sSquad_Member* Data20 = mSquad;
	for (int16 Data1c = 7; Data1c >= 0; --Data1c, ++Data20) {
		sSquad_Member* Data24 = mSquad;

		for (int16 Data18 = 7; Data18 >= 0; --Data18, ++Data24) {

			if (Data20 == Data24)
				continue;

			if (Data20->mRecruitID == -1 || Data24->mRecruitID == -1)
				continue;

			if (Data20->mRank != Data24->mRank)
				continue;

			if (Data20->mNumberOfKills <= Data24->mNumberOfKills)
				continue;

			sSquad_Member Spare = *Data20;

			*Data20 = *Data24;
			*Data24 = Spare;
		}
	}
}

void cFodder::Squad_Prepare() {
	Squad_Clear_Selected();
	
	// This next chunk appears to have no effect and could be removed
	if (!word_3ABA7) {

		if (word_390D4) {
			sSquad_Member* Data20 = mSquad;
			sSprite** Data24 = mSquad_SpritePtrs;

			for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
				Data20->mSprite = *Data24++;
			}
		}

		word_390D4 = -1;
		sSquad_Member* Data20 = mSquad;
		sSprite** Data24 = mSquad_SpritePtrs;
		for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
			*Data24++ = Data20->mSprite;
		}

	}
	// End Chunk

	int16 Data1C = mMapPlayerTroopCount - 1;
	while (Data1C >= 0) {
		Squad_Prepare_NextRecruit();
		--Data1C;
	}

	if (word_3ABA7)
		return;

	Data1C = mTroopsAvailable;
	sSquad_Member* Data20 = mSquad;
	for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {

		if (Data20->mSprite == INVALID_SPRITE_PTR)
			continue;

		if (Data1C)
			--Data1C;
		else
			Data20->mSprite = INVALID_SPRITE_PTR;
	}

}

void cFodder::Squad_Prepare_NextRecruit() {
	sSquad_Member* Data20 = mSquad;

	for (int16 Data0 = 7; Data0 >= 0; --Data0) {

		if (Data20->mRecruitID == -1) {

			if (mRecruit_NextID >= 360)
				return;

			Data20->mRecruitID = mRecruit_NextID;

			const sRecruit* Data24 = &mRecruits[mRecruit_NextID];

			// Demo sets static ranks
			if (mVersion->mRelease == eRelease::Demo) {

				Data20->mRank = (mMissionNumber - 1) >> 1;

				// Jops
				if (Data20->mRecruitID == 1)
					Data20->mRank = 2;

				// Jools
				if (Data20->mRecruitID == 0)
					Data20->mRank = 4;
			}
			else {
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

void cFodder::Squad_Prepare_Sprites() {

	word_3A016 = mTroopsAvailable;
	sSprite* Data20 = mSprites;
	sSquad_Member* Troop = mSquad;

	for (int16 Data18 = 0x1D; Data18 >= 0; --Data18, ++Data20 ) {

		if (Data20->field_0 == -32768)
			continue;

		if (Data20->field_18 != eSprite_Player)
			continue;

		--word_3A016;
		if (word_3A016 < 0) {
			Troop->mSprite = INVALID_SPRITE_PTR;
			Data20->field_0 = -32768;
			Data20->field_18 = eSprite_Null;
			Data20->field_8 = 0x7C;
			++Troop;
		}
		else {
			// loc_1166B
			Data20->field_46 = (int32*) Troop;

			Troop->mSprite = Data20;
			Data20->field_10 = 0x40;
			Data20->field_22 = 0;

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
		Data4 = word_39F3A;

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

		Data8 = word_3EABF[Data4/2];
		Data4 += 0x80;
		Data4 &= 0x1FE;

		DataC = word_3EABF[Data4/2];
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
		int32 eax = 1 << pData8;
		if (pData0 & eax)
			break;

		if (pData4 & eax)
			break;
	}

	pData8 -= 4;
	if (pData8 >= 0) {
		pData0 >>= pData8;
		pData4 >>= pData8;
	}
	pData4 <<= 5;
	pData4 |= pData0;
	//pData4 <<= 1;

	pData4 = word_3F0C1[pData4];
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
	Data0 = mMapWidth_Shifted;
	Data0 -= 0x110;
	Data0 = (Data0 << 16) | (Data0 >> 16);

	int32 Data4 = mCamera_Adjust_Col;
	Data4 += mCamera_Speed_X;

	if (Data4 > Data0) {
		Data0 -= mCamera_Adjust_Col;
		mCamera_Speed_X = Data0;
	}
	//loc_11BE8
	Data0 = mMapHeight_Shifted;
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

	int16 Data10 = mMapWidth_Shifted;
	Data10 -= 0x88;
	if (Data8 >= Data10)
		Data8 = Data10;

	int16 DataC = mCamera_Position_Y;
	DataC -= 0x6C;
	if (DataC < 0)
		DataC = 0;

	Data10 = mMapHeight_Shifted;
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
	word_390A6 = 0;

	mCamera_Position_Column = mCamera_Position_X;
	mCamera_Position_Row = mCamera_Position_Y;
	word_39F52 = 0;
	dword_39F4A &= 0x0000FFFF;
	word_39F54 = 0;
	dword_39F4E &= 0x0000FFFF;
	
	mImage->paletteFadeOut();
	
	do {
		eventProcess();
		mImageFaded = mImage->paletteFade();

		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();

	} while (mImageFaded == -1);

	if (word_3AF07)
		Map_Destroy_Tiles();

	dword_39F7C = 0;

	for (;;) {
		Camera_Pan_To_Target();
		Camera_Pan_To_Target();

		if (!mCamera_Reached_Target)
			break;
	}

	Mission_Sprites_Handle();
	mGraphics->sub_144A2();
	Mouse_DrawCursor();
	sub_11CAD();

	word_390A6 = -1;

	mGraphics->PaletteSet();
	mImageFaded = -1;

loc_11E5B:;
	Music_Play_Tileset();
}

void cFodder::map_Load_Resources() {
	std::string MapName = map_Filename_MapGet();

	mMapSize = g_Resource.fileLoadTo( MapName, mMap );
	tool_EndianSwap( mMap + 0x60, mMapSize - 0x60 );

	std::string BaseName,SubName, BaseBase, BaseSub, BaseBaseSet, BaseSubSet;
	BaseName.append( mMap, mMap + 11 );
	SubName.append( mMap + 0x10, mMap + 0x10 + 11 );

	BaseBaseSet.append( mMap, mMap + 3 );
	BaseSubSet.append( mMap + 0x10, mMap + 0x10 + 3 );

	BaseBase.append( mMap, mMap + 7 );
	BaseSub.append( mMap + 0x10, mMap + 0x10 + 7 );

	mDataBaseBlkSize = g_Resource.fileLoadTo( BaseName, mDataBaseBlk );
	mDataSubBlkSize = g_Resource.fileLoadTo( SubName, mDataSubBlk );

	mMapWidth = readBEWord( &mMap[0x54] );
	writeLEWord( &mMap[0x54], mMapWidth );

	mMapHeight = readBEWord( &mMap[0x56] );
	writeLEWord( &mMap[0x56], mMapHeight );

	mFilenameCopt = Filename_CreateFromBase( BaseBaseSet, "copt." );
	mFilenameBaseSwp = Filename_CreateFromBase( BaseBase, ".swp" );
	mFilenameBaseHit = Filename_CreateFromBase( BaseBase, ".hit" );
	mFilenameBaseBht = Filename_CreateFromBase( BaseBase, ".bht" );
	mFilenameArmy = Filename_CreateFromBase( BaseSubSet, "army." );
	mFilenameSubSwp = Filename_CreateFromBase( BaseSub, ".swp" );
	mFilenameSubHit = Filename_CreateFromBase( BaseSub, ".hit" );
	mFilenameSubBht = Filename_CreateFromBase( BaseSub, ".bht" );
	mFilenameBasePal  = Filename_CreateFromBase( BaseBase, ".pal" );

	size_t Size = g_Resource.fileLoadTo( mFilenameBaseSwp, (uint8*) &mMap_Base_Swp[0] );
	tool_EndianSwap( (uint8*)&mMap_Base_Swp[0], Size );
	
	Size = g_Resource.fileLoadTo( mFilenameSubSwp, (uint8*) &mMap_Sub_Swp[0] );
	tool_EndianSwap( (uint8*)&mMap_Sub_Swp[0], Size );

	Size = g_Resource.fileLoadTo( mFilenameBaseHit, (uint8*) &graphicsBaseHit[0] );
	tool_EndianSwap( (uint8*)&graphicsBaseHit[0], Size );
	
	Size = g_Resource.fileLoadTo( mFilenameSubHit, (uint8*) &graphicsSub0[0] );
	tool_EndianSwap( (uint8*)&graphicsSub0[0], Size );
	
	Size = g_Resource.fileLoadTo( mFilenameBaseBht, (uint8*) &graphicsBaseBht[0] );
	Size = g_Resource.fileLoadTo( mFilenameSubBht, (uint8*) &graphicsSub0Bht[0] );

	mGraphics->map_Load_Resources();
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

	dword_39F20 = Data0;

	sub_12018();
	Camera_Pan();
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

	int16 Data8 = readLEWord( &mMap[0x54] );
	Data8 -= 0x12;
	if (Data8 < 0)
		Data8 = 0;

	if (Data0 >= Data8)
		Data0 = Data8;
	
	Data8 = readLEWord( &mMap[0x56] );
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

void cFodder::sub_11FCD() {
	
	if (!word_3A054) {

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

	word_39F9C = Data0;

	Data0 = mCamera_Adjust_Row >> 16;
	Data0 -= dword_39F88 >> 16;

	word_39F9E = Data0;

	dword_39F8C = dword_39F24;
	dword_39F90 = dword_39F28;
	dword_39F84 = mCamera_Adjust_Col;
	dword_39F88 = mCamera_Adjust_Row;
	word_39FB2 = word_39FB6;
	word_39FB4 = word_39FB8;
}

void cFodder::Camera_Reset() {
	dword_39F18 = 0;
	dword_39F1C = 0;
	dword_39F20 = 0;
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
	int32* Data20 = &dword_39F18;

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
	Data0 = mCamera_Speed_X;
	dword_39F24 += Data0;

	if ((mCamera_Speed_X >> 16) < 0) {

		if ((dword_39F24 >> 16) < 0) {

			dword_39F24 += (0x140 << 16);
			dword_39F56 = (0xFFFF << 16) | (dword_39F56 & 0xFFFF);
			word_39F94 -= 0x28;
			word_39F96 -= 1;
		}
	}
	else {
		//loc_12181
		if ((dword_39F24 >> 16) >= 0x140) {
			dword_39F24 -= (0x140 << 16);
			dword_39F56 = (1 << 16) | (dword_39F56 & 0xFFFF);
			word_39F94 += 0x28;
			++word_39F96;
		}
	}
	//loc_1219F
	Camera_Adjust_Row( Data20 );
	mCamera_Speed_X = 0;
	mCamera_Speed_Y = 0;
}

void cFodder::Camera_Adjust_Row( int32* pData20 ) {

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

	Data0 = dword_39F28 >> 16;
	Data0 *= 0x28;
	*pData20 = Data0;
}

void cFodder::sub_12245() {
	
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

void cFodder::Camera_Pan_Toward_SquadLeader() {

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

	sub_18D5E();
	Sprite_Handle_Loop();

	Sprite_Sort_DrawList();

	int16 Data0 = mCamera_Adjust_Col >> 16;
	Data0 -= dword_39F84 >> 16;

	word_39F9C = Data0;

	Data0 = mCamera_Adjust_Row >> 16;
	Data0 -= dword_39F88 >> 16;

	word_39F9E = Data0;
	dword_39F84 = mCamera_Adjust_Col;
	dword_39F88 = mCamera_Adjust_Row;
	dword_39F8C = dword_39F24;
	dword_39F90 = dword_39F28;
	word_39FB2 = word_39FB6;
	word_39FB4 = word_39FB8;

	Map_Destroy_Tiles();
	Sprite_Draw();
}

void cFodder::Sprite_Sort_DrawList() {
	sSprite* si = mSprites;
	sSprite** di = dword_3A071;
	sSprite** bx = dword_3A125;
	sSprite** bp = dword_3A1D9;

	for (int16 cx = 0x2B; cx > 0; --cx, ++si) {

		if (si->field_0 == -32768)
			continue;

		if (si->field_2C == -1) {
			*di++ = si;
			continue;
		}

		if (si->field_2C == 0) {
			*bx++ = si;
			continue;
		}

		*bp++ = si;
	}

	*di = INVALID_SPRITE_PTR;
	*bx = INVALID_SPRITE_PTR;
	*bp = INVALID_SPRITE_PTR;

	sSprite** Si = dword_3A125 - 1;
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
	di = dword_3A291;
	Si = dword_3A071;

	for(;;) {
		sSprite* ax = *Si++;
		if (ax == INVALID_SPRITE_PTR)
			break;

		*di++ = ax;
	}

	Si = dword_3A125;
	for(;;) {
		sSprite* ax = *Si++;
		if (ax == INVALID_SPRITE_PTR)
			break;

		*di++ = ax;
	}
	Si = dword_3A1D9;
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
	sSquad_Member* Dataa20 = (sSquad_Member*)Data20->field_46;
	int16 Data0 = 0;

	Data0 = Dataa20->mRank;
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

			if (Data20->field_38 == 5)
				continue;

		loc_12620:;
			if (Data20->field_38 == 7)
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
	if(!mPhase_Goals[ eGoal_Get_Civilian_Home - 1 ] || word_3B47B ) 
		 mMission_Complete = -1;
}

void cFodder::Mission_Clear_Destroy_Tiles() {
	mMap_Destroy_TilesPtr2 = mMap_Destroy_Tiles;
	mMap_Destroy_TilesPtr = mMap_Destroy_Tiles;

	mMap_Destroy_Tiles[0].mX = -1;
	mMap_Destroy_Tiles[0].mY = -1;
}

void cFodder::Mission_Phase_Goals_Set() {
	const int8* Data20 = mVersion->mMissionData->mMapGoals[mMapNumber];

	for (; *Data20 != -1; ++Data20) {

		int8 Data0 = *Data20;
		
		mPhase_Goals[Data0-1] = -1;
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
		goto loc_127E8;
	}

	if (mMission_Complete) {
		if (mMission_TryAgain)
			goto loc_127E8;

		Mission_Text_Completed();
		word_390F8 = -1;
		goto loc_1280A;
	}
	//loc_127E1
	if (mSquad_AliveCount) {
	loc_127E8:;

		Mission_Text_TryAgain();
		word_3D465 = 0x0F;
		word_3D467 = 0x0A;
		goto loc_1280A;
	}
	//loc_127FA
	Mission_GameOver();
	word_3D465 = 8;
	word_3D467 = 0x0A;

loc_1280A:;

	if (mMission_Completed_Timer == 0x19) {
		mImageFaded = -1;
		mImage->paletteFadeOut();
	}
	--mMission_Completed_Timer;
	if (mMission_Completed_Timer)
		return;

	mMission_Completed_Timer = -1;
	word_3A3B9 = -1;
}

void cFodder::Mission_Text_Completed() {
	Sprite_Destroy( &mSprites[40] );

	if (mMissionPhaseRemain == 1)
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
	pData2C->field_2C = 1;
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

std::string	cFodder::Filename_CreateFromBase( const std::string& pBase, const char* pFinish ) {
	std::string Final;

	Final.append( pBase );
	Final.append( pFinish );

	return Final;
}

void cFodder::Squad_Member_PhaseCount() {
	sSquad_Member* Data20 = mSquad;

	for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
		if (Data20->mSprite == INVALID_SPRITE_PTR || Data20->mSprite == 0 )
			continue;

		++Data20->mPhaseCount;
	}
}

void cFodder::sub_12AEE() {
	
	if(word_3B211)
		return;
	
	word_3B211 = -1;
	sSprite* Data20 = mSprites;
	sSprite** Data24 = dword_3B213;

	for( int16 Data1C = 0x1D; Data1C >= 0; --Data1C, ++Data20 ) {
		
		if( Data20->field_0 == -32768)
			continue;
		
		if( Data20->field_22 )
			continue;
		
		if( !Data20->field_65 )
			continue;
		
		*Data24 = Data20;
		++Data24;
	}
	
	*Data24 = INVALID_SPRITE_PTR;
	++Data24;

}

void cFodder::sub_12B6E() {
	int16 Data8 = mSquad_Selected;
	if (Data8 >= 0) {
		sSprite** Data20 = mSquads[Data8];

		dword_3B24F[Data8] = 0;

		int16 Data0 = 0;

		for (;;) {
			if (*Data20 != INVALID_SPRITE_PTR && *Data20 != 0 ) {
				sSprite* Data24 = *Data20++;
				if (!Data24->field_6E)
					goto loc_12C5F;

				mSquad_CurrentVehicle = (sSprite*) Data24->field_6A;
				Data0 = -1;
				continue;

			}
			else {
				//loc_12C2B
				if (Data0 == 0)
					goto loc_12C5F;

				dword_3B24F[Data8] = mSquad_CurrentVehicle;
				mSquad_Leader = mSquad_CurrentVehicle;
				return;
			}
		}

	}

loc_12C5F:;
	mSquad_CurrentVehicle = 0;
}

void cFodder::sub_12C69() {

	for (int16 Data0 = 0; Data0 < 3; ++Data0) {
		if (!word_3B461[Data0])
			continue;

		word_3B461[Data0]--;
		if (word_3B461[Data0])
			continue;

		sSprite** Data2C = mSquads[Data0];

		for (;;) {
			if (*Data2C == INVALID_SPRITE_PTR )
				break;

			sSprite* Data30 = *Data2C++;
			Data30->field_38 = 0x5A;
		}
	}
}

void cFodder::Mission_Map_Overview_Show() {
	word_3A016 = 0;

	int16 word_3F952;
	int16 word_3F954;
	int32 eax = dword_3F946;

	eax *= mSquad_Leader->field_0 & 0xFFFF;
	eax >>= 14;
	eax += 0x40;
	eax += word_3F94E;
	eax -= 8;
	word_3F952 = eax & 0xFFFF;
	if (word_3F952 < 0x40)
		word_3F952 = 0x40;

	eax = dword_3F94A;
	eax *= mSquad_Leader->field_4 & 0xFFFF;
	eax >>= 14;
	eax += 0x10;
	eax += word_3F950;
	eax -= 8;
	word_3F954 = eax & 0xFFFF;

	mDrawSpritePositionX =  (mSquad_Leader->field_0) + (mSurfaceMapLeft * 16);
	mDrawSpritePositionY =  (mSquad_Leader->field_4 - 0x10) + (mSurfaceMapTop * 16);
	byte_42070 = 0xF0;

	mGraphics->PaletteSetOverview();
	mSurfaceMapOverview->Save();
	mGraphics->SetImage( mSurfaceMapOverview );

	do {
		if (mSquad_Leader != INVALID_SPRITE_PTR) {
			++word_3A016;
			word_3A016 &= 0x3F;

			if (word_3A016 < 0x20) {
				if (mVersion->mPlatform == ePlatform::PC) {
					mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( 0x4307 ) + 0x46B8;
					mDrawSpriteColumns = 0x10;
					mDrawSpriteRows = 0x10;
				} 
				else {
					mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( 2 ) + (113 * 40) + 6;
					mDrawSpriteColumns = 0x2;
					mDrawSpriteRows = 0x10;
				}

				mGraphics->video_Draw_Sprite();
			} 
			else
				mSurfaceMapOverview->Restore();
		}

		g_Window.RenderShrunk( mSurfaceMapOverview );
		g_Window.FrameEnd();
		
		Mouse_Inputs_Get();

		if (mMission_Aborted == -1)
			break;

	} while (Mouse_Button_Left_Toggled() < 0);

	mSurfaceMapOverview->Restore();
	mGraphics->SetImageOriginal();

	mMission_Aborted = 0;
}

void cFodder::Map_Overview_Prepare() {
	if (mMapNumber == word_3FA1F)
		return;

	delete mSurfaceMapOverview;
	size_t Size = mMapWidth < mMapHeight ? mMapHeight : mMapWidth;

	mSurfaceMapOverview = new cSurface( Size * 16, Size * 16 );
	mSurfaceMapOverview->clearBuffer();

	word_3FA1F = mMapNumber;

	for (int16 cx = 0x45; cx >= 0; --cx) {
		dword_3E9A3[cx] = 0;
	}

	int16* Di = (int16*) (mMap + 0x60);

	int16 Data8;
	int16 DataC;

	if (readLEWord( &mMap[0x56] ) <= readLEWord( &mMap[0x54] ) ) {
		int32 eax = 0x1100000;
		int32 ebx = readLEWord( &mMap[0x54] );
		ebx &= 0xFFFF;

		eax /= ebx;
		dword_3F946 = eax;
		eax >>= 0x10;
		Data8 = eax;

		eax = 0x0C80000;
		ebx = readLEWord( &mMap[0x54] );
		ebx &= 0xFFFF;
		eax /= ebx;
		dword_3F94A = eax;
		eax >>= 0x10;
		DataC = eax;
	}
	else {
		//loc_12FDD
		int32 eax = 0x1100000;
		int32 ebx = readLEWord( &mMap[0x56] );
		ebx &= 0xFFFF;

		eax /= ebx;
		dword_3F946 = eax;
		eax >>= 0x10;
		Data8 = eax;

		eax = 0x0C80000;
		ebx = readLEWord( &mMap[0x56] );
		ebx &= 0xFFFF;
		eax /= ebx;
		dword_3F94A = eax;
		eax >>= 0x10;
		DataC = eax;
	}

	int32 eax = dword_3F946;
	int32 ebx = readLEWord( &mMap[0x54] ) & 0xFFFF;
	eax *= ebx;
	eax >>= 0x11;
	eax -= 0x88;
	eax = -eax;
	word_3F94E = eax;

	eax = dword_3F94A;
	ebx = readLEWord( &mMap[0x56] ) & 0xFFFF;
	eax *= ebx;
	eax >>= 0x11;
	eax -= 0x64;
	eax = -eax;
	word_3F950 = eax;
	int16 Data0 = 0x10;
	int16 Data4 = 0x10;

	eax = Data8 + 1;
	eax <<= 0x10;
	eax /= Data0;
	dword_44A42 = eax;

	eax = DataC + 1;
	eax >>= 0x10;
	eax /= Data4;
	dword_44A46 = eax;

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

			mGraphics->sub_2B04B( *Di & 0x1FF, cx + mSurfaceMapLeft, dx + mSurfaceMapTop );
			
			++Di;
		}
	}

}

void cFodder::map_SetTileType() {

	for (unsigned int x = 0; x < 7; ++x) {
		if (mTileType_Names[x][0] != mMap[0])
			continue;
	
		if (mTileType_Names[x][1] != mMap[1])
			continue;

		if (mTileType_Names[x][2] != mMap[2])
			continue;

		mMap_TileSet = x;
		return;
	}

	// Fallback to Jungle
	mMap_TileSet = eTileTypes_Jungle;
}

void cFodder::sub_12AB1() {

	word_3E75B = -1;
	word_3EABD = 0;
}

bool cFodder::EventAdd( cEvent pEvent ) {
	
	mEvents.push_back( pEvent );

	return true;
}

void cFodder::Load_SetupData( const std::string& pFilename ) {
	/*
	size_t FileSize = 0;
	uint8* Buffer = local_FileRead( pFilename, "..", FileSize );

	word_3E7ED = readLEWord( Buffer );
	mEffectDriver = readLEWord( Buffer + 2 );
	mMusicDriver = readLEWord( Buffer + 4 );
	delete Buffer;
	*/

	word_3E7ED = 1;
	mEffectDriver = 6;
	mMusicDriver = 4;
}

void cFodder::memory_XMS_Detect() {

	word_427D4 = 1;
	mErrorCode = 0;
}

void cFodder::eventProcess() {
	g_Window.EventCheck();

	for (std::vector<cEvent>::iterator EventIT = mEvents.begin(); EventIT != mEvents.end(); ++EventIT) {

		switch (EventIT->mType) {
			
			case eEvent_KeyDown:
				keyProcess( EventIT->mButton, false );
				break;

			case eEvent_KeyUp:
				keyProcess( EventIT->mButton, true );
				break;

			case eEvent_MouseLeftDown:
				mMousePosition = EventIT->mPosition;
				mMouseButtons |= 1;
				break;

			case eEvent_MouseRightDown:
				mMousePosition = EventIT->mPosition;
				mMouseButtons |= 2;
				break;

			case eEvent_MouseLeftUp:
				mMousePosition = EventIT->mPosition;
				mMouseButtons &= ~1;
				break;

			case eEvent_MouseRightUp:
				mMousePosition = EventIT->mPosition;
				mMouseButtons &= ~2;
				break;

			case eEvent_MouseMove:
				mMousePosition = EventIT->mPosition;
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
		mMission_Aborted = -1;

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
			word_3FA21 = -1;
			mKeyCode = 0x1C;
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
	word_3BDB3 = 0;
	word_3BDB5 = 0;

	g_Window.SetMousePosition( cPosition(g_Window.GetWindowSize().mWidth / 2, g_Window.GetWindowSize().mHeight / 2 ) );
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
	word_3BDB3 = Data4;

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
	word_3BDB5 = Data0;

	if (Data0 < 4)
		Data0 = 4;
	else {

		if (Data0 > mWindow->GetScreenSize().mHeight + 3)
			Data0 = mWindow->GetScreenSize().mHeight + 3;
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
				word_3AA43 = -1;
			}
		}

	} else {
		word_3AA43 = 0;
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

void cFodder::VersionSelect_0() {
	
	VersionLoad( mVersions[0] );
	mDemo_ExitMenu = 1;
}

void cFodder::VersionSelect_1() {

	VersionLoad( mVersions[1] );
	mDemo_ExitMenu = 1;
}

void cFodder::VersionSelect_2() {

	VersionLoad( mVersions[2] );
	mDemo_ExitMenu = 1;
}

void cFodder::VersionSelect_3() {
	
	VersionLoad( mVersions[3] );
	mDemo_ExitMenu = 1;
}

void cFodder::VersionSelect_4() {
	
	VersionLoad( mVersions[4] );
	mDemo_ExitMenu = 1;
}

void cFodder::VersionSelect() {
	sGUI_Element *Buttons = new sGUI_Element[mVersions.size() + 1];

	sSprite* Sprite, *Sprite2, *Sprite3;

	Sprite_Clear_All();

	Sprite = &mSprites[0];
	Sprite2 = Sprite + 1;
	Sprite3 = Sprite + 2;

	Sprite->field_0 = 0x10;
	Sprite->field_4 = 0x14;
	Sprite->field_8 = 6;
	Sprite->field_A = 0;
	Sprite->field_52 = 0;
	Sprite->field_20 = 0;
	Sprite->field_18 = eSprite_Player;

	Sprite2->field_0 = 0xbf;
	Sprite2->field_4 = 0x14;
	Sprite2->field_8 = 2;
	Sprite2->field_A = 0;
	Sprite2->field_52 = 0;
	Sprite2->field_20 = 0;
	Sprite2->field_18 = eSprite_Player;

	Sprite3->field_0 = 0xf0;
	Sprite3->field_4 = 0xce;
	Sprite3->field_8 = 0xD2;
	Sprite3->field_A = 5;
	Sprite3->field_52 = 0;
	Sprite3->field_20 = 0;
	Sprite3->field_18 = eSprite_Turret_Missile_Human;

	mGraphics->LoadpStuff();

	map_Load_Resources();
	mGraphics->PaletteSet();
	Mouse_Setup();

	word_3AC21 = 0;
	word_3BEC3 = 0;

	mSound->Music_Stop();
	mImage->clearBuffer();
	mWindow->SetScreenSize( cDimension( 320, 200 ) );

	mGraphics->SetSpritePtr( eSPRITE_BRIEFING );
	int16 Pos = 0x1;
	int Count = 0;

	std::string Name = "OPEN FODDER";
	std::transform(Name.begin(), Name.end(),Name.begin(), ::toupper);

	word_3AC19 = 0x25;
	String_CalculateWidth( 320, mFont_Underlined_Width, Name.c_str() );
	String_Print( mFont_Underlined_Width, 1, mGUI_Temp_X, Pos , Name.c_str() );

	Name = "SELECT A GAME";

	String_CalculateWidth( 320, mFont_Underlined_Width, Name.c_str() );
	String_Print( mFont_Underlined_Width, 3, mGUI_Temp_X, 0x1A , Name.c_str() );

	word_3AC19 = 0;

	Pos += 0x40;
	for (std::vector<const sVersion*>::const_iterator VersionIT = mVersions.begin(); VersionIT != mVersions.end(); ++VersionIT) {
		std::string Name = (*VersionIT)->mName;
		std::transform(Name.begin(), Name.end(),Name.begin(), ::toupper);

		String_CalculateWidth( 320, mFont_Briefing_Width, Name.c_str() );
		String_Print( mFont_Briefing_Width, 0, mGUI_Temp_X, Pos , Name.c_str() );

		Buttons[Count].field_0 = &cFodder::GUI_Button_NoAction;
		Buttons[Count].mX = mGUI_Temp_X - 6;
		Buttons[Count].mWidth = mGUI_Temp_Width;
		Buttons[Count].mY = Pos - 2;
		Buttons[Count].mHeight = 5;

		switch (Count) {
			case 0:
				Buttons[Count].mMouseInsideFuncPtr = &cFodder::VersionSelect_0;
				break;

			case 1:
				Buttons[Count].mMouseInsideFuncPtr = &cFodder::VersionSelect_1;
				break;

			case 2:
				Buttons[Count].mMouseInsideFuncPtr = &cFodder::VersionSelect_2;
				break;

			case 3:
				Buttons[Count].mMouseInsideFuncPtr = &cFodder::VersionSelect_3;
				break;

			case 4:
				Buttons[Count].mMouseInsideFuncPtr = &cFodder::VersionSelect_4;
				break;
		}
		Pos += 30;
		++Count;
	}

	Buttons[Count].field_0 = 0;

	mImage->Save();

	mGraphics->SetSpritePtr( eSPRITE_IN_GAME );

	mImageFaded = -1;
	mMouseSpriteNew = 0x24;
	eventProcess();
	Mouse_Setup();
	mDemo_ExitMenu = 0;

	mSquad_CurrentVehicle = Sprite3;

	Camera_Reset();

	for( ;; ) {

		Sprite_Handle_Turret_Missile_Human(Sprite3);

		Sprite_Sort_DrawList();
		Sprite_Draw();

		if (mImageFaded)
			mImageFaded = mImage->paletteFade();

		Mouse_Inputs_Get();
		Mouse_DrawCursor();
		if (mButtonPressLeft)
			GUI_Element_Mouse_Over( Buttons );

		if (mDemo_ExitMenu)
			break;

		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->Restore();
	}

	mButtonPressLeft = 0;

	mSquad_CurrentVehicle = 0;
	delete[] Buttons;
}

void cFodder::WindowTitleSet( bool pInMission ) {
	std::stringstream Title;
	Title << mTitle.str();

	if (pInMission) {
		if (mVersion->mRelease == eRelease::Demo) {

			Title << " ( Mission:";
			Title << " " << mVersion->mMissionData->mMissionPhaseNames[mMapNumber];

		} else {
			Title << " ( Mission: " << mMissionNumber;
			Title << " " << mVersion->mMissionData->mMissionNames[mMissionNumber - 1];

			Title << "  Phase: " << (mMissionPhase + 1) << " ";

			if (mMissionPhases > 1) {
				Title << "of " << mMissionPhases;
				Title << " " << mVersion->mMissionData->mMissionPhaseNames[mMapNumber];
			}
			else
				Title << mVersion->mMissionData->mMissionPhaseNames[mMapNumber];
		}

		Title << " )";
	}

	mWindow->SetWindowTitle( Title.str() );
}

/**
 * This function loads a new version of the game, and is generally called on startup,
 * or AFTER a button on the version select screen is pushed
 *
 */
void cFodder::VersionLoad( const sVersion* pVersion ) {
	mTitle.str( "" );
	mTitle << "Open Fodder";
	if (mVersion || mVersions.size() == 1 )
		if (strlen( pVersion->mName ))
			mTitle << ": " << pVersion->mName;
	
	mVersion = pVersion;

	mWindow->SetWindowTitle( mTitle.str() );

	delete mGraphics;
	delete mResources;
	delete mSound;

	mImage->paletteFadeOut();
	
	while( mImage->GetFaded() == false ) {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->paletteFade();
	}

	switch (mVersion->mPlatform) {
		case ePlatform::PC:
			mResources = new cResource_PC_CD( mVersion->mDataPath );
			mGraphics = new cGraphics_PC();

			if (mVersion->mGame == eGame::CF1)
				mSound = new cSound_PC();
			
			if (mVersion->mGame == eGame::CF2)
				mSound = new cSound_PC2();

			mWindow->SetScreenSize( cDimension( 320, 200 ) );
			mWindow->SetOriginalRes( cDimension( 320, 200 ) );

			break;

		case ePlatform::Amiga:
			mResources = new cResource_Amiga_File( mVersion->mDataPath );
			mGraphics = new cGraphics_Amiga();
			mSound = new cSound_Amiga();

			((cGraphics_Amiga*)mGraphics)->SetCursorPalette( 0xE0 );

			mWindow->SetScreenSize( cDimension( 320, 260 ) );
			mWindow->SetOriginalRes( cDimension( 320, 225 ) );
			break;
	}
		
	mGraphics->SetSpritePtr( eSPRITE_IN_GAME );
	mGraphics->LoadpStuff();
	map_Load_Resources();
	mGraphics->graphicsBlkPtrsPrepare();
	mGraphics->PaletteSet();
}

void cFodder::Prepare( ) {
	mVersions = FindFodderVersions();
	if (mVersions.empty()) {
		std::cout << "No data files found\n";
		exit( 1 );
	}

	mWindow->InitWindow( "Open Fodder" );

	tool_RandomSeed();

	mDataBlkSize = 0xFD00 * 16;

	mDataPStuff = new uint8[0xA03 * 16];
	mDataBaseBlk = new uint8[mDataBlkSize];
	mDataSubBlk = new uint8[mDataBlkSize];

	mMap = new uint8[0x346 * 16];
	mDataHillBits = new uint8[0xD5A * 16];
	mDataArmy = new uint8[0xD50 * 16];
	word_3BDAD = (uint16*) new uint8[0x400 * 16];
	mMapSptPtr = (uint16*) new uint8[0x400 * 16];
	word_3D5B7 = mMapSptPtr;

	uint8* Start = (uint8*) &mMapNumber;
	uint8* End = (uint8*)&mButtonPressLeft;

	word_397D8 = new uint8[ End - Start ];
	Load_SetupData("setup.dat");
	sub_12AB1();

	memory_XMS_Detect();

	memset(mDataBaseBlk, 0, mDataBlkSize );
	memset(mDataSubBlk, 0, mDataBlkSize );

	mImage = new cSurface( 352, 300 );
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

void cFodder::sub_13148() {
	word_3B4D5 = 0x64;
	word_3B4D3 = 0x28;
}

void cFodder::Sprite_HelicopterCallPad_Check() {
	if (!mHelicopterCallPadCount)
		return;

	if (mHelicopterCallPadCount != mHelicopterCallPadPressedCount)
		return;

	if (mHelicopterCall_X >= 0)
		return;

	sSprite* Data2C = dword_3B4CF;
	if (!dword_3B4CF || dword_3B4CF == INVALID_SPRITE_PTR)
		return;

	mHelicopterCall_X = dword_3B4CF->field_0;
	mHelicopterCall_Y = dword_3B4CF->field_4;

	mHelicopterCall_Y += 0x28;
}

void cFodder::Mission_Final_Timer() {
	if (mMapNumber != 0x47)
		return;

	if (word_3B4D5)
		return;


	if (!word_3B4D7)
		word_3B4D7 = 0x28;

	--word_3B4D7;
	if (!word_3B4D7)
		mMission_Aborted = -1;

	int16 Data0 = word_390B0;
	Data0 &= 3;
	if (!Data0)
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
	const char *PausedStr = "GAME PAUSED";

	mGraphics->PaletteSet();
	mImage->paletteFadeOut();
	mImage->paletteFade();
	mImageFaded = -1;

	mGraphics->SetSpritePtr( eSPRITE_BRIEFING );
	
	word_3AC19 = 0x25;
	String_CalculateWidth( 0x170, mFont_Underlined_Width, PausedStr );
	String_Print( mFont_Underlined_Width, 1, mGUI_Temp_X, 0x54, PausedStr );

	word_3AC19 = 0;
	mGraphics->SetSpritePtr( eSPRITE_IN_GAME );
}

void cFodder::sub_136D0() {
	
	word_40056 = 0;
	word_40058 = 0;
	word_4005A = 0x5280;
	word_4005C = 0xA500;
	word_40054 = 0;
}

void cFodder::Video_Sleep_Wrapper() {

	Video_Sleep();
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
	int16 cx = mMouseX + word_3A9F3;
	int16 dx = mMouseY + word_3A9F5;

	cx += 48;
	dx += 12;

	mDrawSpritePositionX = cx;
	mDrawSpritePositionY = dx;

	if (mMouseSpriteNew >= 0) {
		mMouseSpriteCurrent = mMouseSpriteNew;
		mMouseSpriteNew = -1;
	}

	mGraphics->Mouse_DrawCursor();
}

void cFodder::Sprite_Draw_Frame( int32 pSpriteType, int32 pPositionY, int32 pFrame, int32 pPositionX ) {
	const sSpriteSheet* SheetData = &mSpriteDataPtr[pSpriteType][pFrame];

	byte_42070 = SheetData->field_C & 0xFF;
	mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( SheetData->field_2 );
	mDrawSpriteFrameDataPtr += SheetData->field_0;

	mDrawSpritePositionX = (pPositionX + 0x10);
	mDrawSpritePositionY = (pPositionY + 0x10);
	mDrawSpriteColumns = SheetData->mColCount;
	mDrawSpriteRows = SheetData->mRowCount;
	if (pSpriteType > mGUI_Temp_Height)
		mGUI_Temp_Height = pSpriteType;
	
	if (Sprite_OnScreen_Check() )
		mGraphics->video_Draw_Sprite();

}

void cFodder::sub_13C8A( int16 pData0, int16 pData4, int16 pPosX, int16 pPosY ) {

	byte_42070 = mSpriteDataPtr[pData0][pData4].field_C & 0xFF;
	mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( mSpriteDataPtr[pData0][pData4].field_2 );
	mDrawSpriteFrameDataPtr += mSpriteDataPtr[pData0][pData4].field_0;

	mDrawSpritePositionX = (pPosX + 0x10);
	mDrawSpritePositionY = (pPosY + 0x10);
	mDrawSpriteColumns = mSpriteDataPtr[pData0][pData4].mColCount;
	mDrawSpriteRows = mSpriteDataPtr[pData0][pData4].mRowCount;
	word_42078 = 0x140;

	if (Sprite_OnScreen_Check())
		mGraphics->video_Draw_Linear();
}

void cFodder::sub_13CF0( sSprite* pDi, int16 pData0, int16 pData4 ) {
	
	byte_42070 = mSpriteDataPtr[pData0][pData4].field_C & 0xFF;
	mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( mSpriteDataPtr[pData0][pData4].field_2 );
	mDrawSpriteFrameDataPtr += mSpriteDataPtr[pData0][pData4].field_0;
	
	mDrawSpriteColumns = mSpriteDataPtr[pData0][pData4].mColCount;
	mDrawSpriteRows = mSpriteDataPtr[pData0][pData4].mRowCount - pDi->field_52;

	mDrawSpritePositionX = (mSpriteDataPtr[pData0][pData4].field_E + pDi->field_0) - mCamera_Column + 0x40;
	mDrawSpritePositionY = (mSpriteDataPtr[pData0][pData4].field_F + pDi->field_4) - mDrawSpriteRows - pDi->field_20 - mCamera_Row;
	mDrawSpritePositionY += 0x10;

	++word_42072;
	if (Sprite_OnScreen_Check()) {
		pDi->field_5C = 1;
		mGraphics->video_Draw_Sprite();
	} else 
		pDi->field_5C = 0;
}

bool cFodder::Sprite_OnScreen_Check() {
	int16 ax;
	
	if( mDrawSpritePositionY < 0 ) {
		ax = mDrawSpritePositionY + mDrawSpriteRows;
		--ax;
		if( ax < 0 )
			return false;
		
		ax -= 0;
		ax -= mDrawSpriteRows;
		++ax;
		ax = -ax;
		mDrawSpritePositionY += ax;
		mDrawSpriteRows -= ax;

		if (mVersion->mPlatform == ePlatform::PC)
			ax *= 0xA0;

		if (mVersion->mPlatform == ePlatform::Amiga)
			ax *= 40;

		mDrawSpriteFrameDataPtr += ax;
	}
	
	ax = mDrawSpritePositionY + mDrawSpriteRows;
	--ax;
	if (mVersion->mPlatform == ePlatform::PC) {
		if (ax > 231) {
			if (mDrawSpritePositionY > 231)
				return false;

			ax -= 231;
			mDrawSpriteRows -= ax;

		}
	}
	if (mVersion->mPlatform == ePlatform::Amiga) {
		if (ax > 256) {
			if (mDrawSpritePositionY > 256)
				return false;

			ax -= 256;
			mDrawSpriteRows -= ax;

		}
	}

	if( mDrawSpritePositionX < 0 ) {
		ax = mDrawSpritePositionX + mDrawSpriteColumns;
		--ax;
		if( ax < 0 )
			return false;
		
		ax -= 0;
		ax -= mDrawSpriteColumns;
		++ax;
		ax = -ax;
		--ax;
		
		do {
			++ax;
		} while (ax & 3);

		mDrawSpritePositionX += ax;
		mDrawSpriteColumns -= ax;
		ax >>= 1;
		mDrawSpriteFrameDataPtr += ax;
	}

	ax = mDrawSpritePositionX + mDrawSpriteColumns;
	--ax;
	
	if( ax > 351 ) {
		if( mDrawSpritePositionX > 351 )
			return false;
		
		ax -= 351;
		--ax;
		
		do {
			++ax;
		} while (ax & 3);
		
		mDrawSpriteColumns -= ax;
	}

	if( mDrawSpriteColumns <= 0 )
		return false;
	
	if( mDrawSpriteRows <= 0 )
		return false;
	
	return true;
}

void cFodder::Sprite_Draw( ) {
	sSprite** si = dword_3A291;
	word_42072 = 2;

	for (;;) {
		sSprite* eax = *si++;
		if (eax == INVALID_SPRITE_PTR || eax == 0 )
			break;

		if (eax->field_24) {
			eax->field_24 = 0;
			eax->field_0 = -32768;
			word_3B15D = 0;
			word_3B15F = 0;
		}
		else {
			int16 Data0 = eax->field_8;
			int16 Data4 = eax->field_A;
			sub_13CF0( eax, Data0, Data4 );
		}
	}
}

void cFodder::Sprite_Map_Sound_Play( int16 &pData0 ) {

	word_3B449 = pData0;

	if (mSquad_Leader == INVALID_SPRITE_PTR )
		 return;

	goto loc_14D66;
	//seg002:05B0
	// UNUSED BLOLCK

loc_14D66:;
	mSoundEffectToPlay = word_3B449;
}

void cFodder::Sound_Play( sSprite* pSprite, int16 pSoundEffect, int16 pData8 ) {
	int16 Data0 = 0;

	int8 al = byte_3A9DA[0];
	if (pData8 >= al) {
		byte_3A9DA[0] = pData8 & 0xFF;
	}
	else {
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

void cFodder::Briefing_Intro_Jungle( ) {
	
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mDrawSpriteFrameDataPtr = (uint8*) word_4286B;

	byte_42070 = 0xE0;


	sub_1590B();

	mGraphics->PaletteLoad( (word_42861 + word_4286D) - 0x300, 0x100, 0 );

	mImage->paletteSet( mPalette );

	mImageFaded = -1;

	do {
		if (word_428D6 == -1)
			sub_159A6();

		if( mImageFaded == -1 )
			mImageFaded = mImage->paletteFade();

		// Clouds
		word_42859 = 0x30;
		word_4285B = 0x0C64 * 4;
		sub_15B86( word_42867, word_42875 );

		word_42859 = 0x38;
		word_4285B = 0x102C * 4;
		sub_15A36( word_42865, word_42873 );

		word_42859 = 0x12;
		word_4285B = 0x1D3C * 4;
		sub_15A36( word_42863 , word_42871 );

		// Trees (Main)
		word_42859 = 0x5C;
		word_4285B = 0x236C * 4;
		sub_15B86( word_42869, word_42871 );

		mDrawSpriteFrameDataPtr = ((uint8*) word_4286B) + word_428CE[word_428CC / 2];

		mDrawSpritePositionX = mHelicopterPosX >> 16;		// X
		mDrawSpritePositionY = mHelicopterPosY >> 16;		// Y 
		mDrawSpriteColumns = 0x40;
		mDrawSpriteRows = 0x18;
		if (Sprite_OnScreen_Check())
			mGraphics->video_Draw_Sprite();

		word_42859 = 0x2D;
		word_4285B = 0x33EC * 4;
		sub_15A36( word_42861, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		eventProcess();
		Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		Mouse_GetData();
		if (mouse_Button_Status) {
			word_428D8 = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
		}
	} while (word_428D8 || mImageFaded != 0);
}

void cFodder::Briefing_Intro_Desert() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mDrawSpriteFrameDataPtr = (uint8*) word_4286B;

	byte_42070 = 0xE0;


	sub_1590B();

	mGraphics->PaletteLoad( (word_42861 + word_4286D) - 0x300, 0x100, 0 );

	mImage->paletteSet( mPalette );

	mImageFaded = -1;

	do {
		if (word_428D6 == -1)
			sub_159A6();

		if( mImageFaded == -1 )
			mImageFaded = mImage->paletteFade();

		// Clouds
		word_42859 = 0x3A;
		word_4285B = 0x0C64 * 4;
		sub_15B86( word_42867, word_42875 );

		word_42859 = 0x4C;
		word_4285B = 0x139C * 4;
		sub_15A36( word_42865, word_42873 );

		word_42859 = 0x30;
		word_4285B = 0x1CE4 * 4;
		sub_15A36( word_42863 , word_42871 );

		// Trees (Main)
		word_42859 = 0x40;
		word_4285B = 0x2D64 * 4;
		sub_15B86( word_42869, word_42871 );

		mDrawSpriteFrameDataPtr = ((uint8*) word_4286B) + word_428CE[word_428CC / 2];

		mDrawSpritePositionX = mHelicopterPosX >> 16;		// X
		mDrawSpritePositionY = mHelicopterPosY >> 16;		// Y 
		mDrawSpriteColumns = 0x40;
		mDrawSpriteRows = 0x18;
		if (Sprite_OnScreen_Check())
			mGraphics->video_Draw_Sprite();

		word_42859 = 0x30;
		word_4285B = 0x32E4 * 4;
		sub_15A36( word_42861, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		Mouse_GetData();
		if (mouse_Button_Status) {
			word_428D8 = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
		}
	} while (word_428D8 || mImageFaded != 0);
}

void cFodder::Briefing_Intro_Ice() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mDrawSpriteFrameDataPtr = (uint8*) word_4286B;

	byte_42070 = 0xE0;


	sub_1590B();

	mGraphics->PaletteLoad( (word_42861 + word_4286D) - 0x300, 0x100, 0 );

	mImage->paletteSet( mPalette );

	mImageFaded = -1;

	do {
		if (word_428D6 == -1)
			sub_159A6();

		if( mImageFaded == -1 )
			mImageFaded = mImage->paletteFade();

		// Clouds
		word_42859 = 0x24;
		word_4285B = 0x0C64 * 4;
		sub_15B86( word_42867, word_42875 );

		word_42859 = 0x42;
		word_4285B = 0x102C * 4;
		sub_15A36( word_42865, word_42873 );

		// Ice Caps
		word_42859 = 0x18;
		word_4285B = 0x1CE4 * 4;
		sub_15A36( word_42863 , word_42871 );

		// Ice Mountains
		word_42859 = 0x58;
		word_4285B = 0x2524 * 4;
		sub_15B86( word_42869, word_42871 );

		mDrawSpriteFrameDataPtr = ((uint8*) word_4286B) + word_428CE[word_428CC / 2];

		mDrawSpritePositionX = mHelicopterPosX >> 16;		// X
		mDrawSpritePositionY = mHelicopterPosY >> 16;		// Y 
		mDrawSpriteColumns = 0x40;
		mDrawSpriteRows = 0x18;
		if (Sprite_OnScreen_Check())
			mGraphics->video_Draw_Sprite();

		word_42859 = 0x2E;
		word_4285B = 0x3394 * 4;
		// Trees
		sub_15A36( word_42861, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		Mouse_GetData();
		if (mouse_Button_Status) {
			word_428D8 = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
		}
	} while (word_428D8 || mImageFaded != 0);
}

void cFodder::Briefing_Intro_Mor() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mDrawSpriteFrameDataPtr = (uint8*) word_4286B;

	byte_42070 = 0xE0;

	sub_1590B();

	mGraphics->PaletteLoad( (word_42861 + word_4286D) - 0x300, 0x100, 0 );

	mImage->paletteSet( mPalette );

	mImageFaded = -1;

	do {
		if (word_428D6 == -1)
			sub_159A6();

		if( mImageFaded == -1 )
			mImageFaded = mImage->paletteFade();

		// Clouds
		word_42859 = 0x1D;
		word_4285B = 0x0C64 * 4;
		sub_15B86( word_42867, word_42875 );

		word_42859 = 0x40;
		word_4285B = 0x1134 * 4;
		sub_15A36( word_42865, word_42873 );

		word_42859 = 0x6;
		word_4285B = 0x2524 * 4;
		sub_15A36( word_42863 , word_42871 );

		// Trees (Main)
		word_42859 = 0x52;
		word_4285B = 0x2734 * 4;
		sub_15B86( word_42869, word_42871 );

		mDrawSpriteFrameDataPtr = ((uint8*) word_4286B) + word_428CE[word_428CC / 2];

		mDrawSpritePositionX = mHelicopterPosX >> 16;		// X
		mDrawSpritePositionY = mHelicopterPosY >> 16;		// Y 
		mDrawSpriteColumns = 0x40;
		mDrawSpriteRows = 0x18;
		if (Sprite_OnScreen_Check())
			mGraphics->video_Draw_Sprite();

		word_42859 = 0x30;
		word_4285B = 0x32E4 * 4;
		sub_15A36( word_42861, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		Mouse_GetData();
		if (mouse_Button_Status) {
			word_428D8 = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
		}
	} while (word_428D8 || mImageFaded != 0);
}

void cFodder::Briefing_Intro_Int() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mDrawSpriteFrameDataPtr = (uint8*) word_4286B;

	byte_42070 = 0xE0;


	sub_1590B();

	mGraphics->PaletteLoad( (word_42861 + word_4286D) - 0x300, 0x100, 0 );

	mImage->paletteSet( mPalette );

	mImageFaded = -1;

	do {
		if (word_428D6 == -1)
			sub_159A6();

		if( mImageFaded == -1 )
			mImageFaded = mImage->paletteFade();

		// Clouds
		word_42859 = 0x40;
		word_4285B = 0x0C64 * 4;
		sub_15B86( word_42867, word_42875 );

		word_42859 = 0x2F;
		word_4285B = 0x16B4 * 4;
		sub_15A36( word_42865, word_42873 );

		word_42859 = 0x22;
		word_4285B = 0x1B2C * 4;
		sub_15A36( word_42863 , word_42871 );

		// Trees (Main)
		word_42859 = 0x53;
		word_4285B = 0x26DC * 4;
		sub_15B86( word_42869, word_42871 );

		mDrawSpriteFrameDataPtr = ((uint8*) word_4286B) + word_428CE[word_428CC / 2];

		mDrawSpritePositionX = mHelicopterPosX >> 16;		// X
		mDrawSpritePositionY = mHelicopterPosY >> 16;		// Y 
		mDrawSpriteColumns = 0x40;
		mDrawSpriteRows = 0x18;
		if (Sprite_OnScreen_Check())
			mGraphics->video_Draw_Sprite();

		word_42859 = 0x23;
		word_4285B = 0x375C * 4;
		sub_15A36( word_42861, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		Mouse_GetData();
		if (mouse_Button_Status) {
			word_428D8 = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
		}
	} while (word_428D8 || mImageFaded != 0);
}

void cFodder::sub_1590B( ) {
	mHelicopterPosX = 0x01500000;
	mHelicopterPosY = 0x00260000;

	word_428C6 = word_428DC[0];
	word_428C4 = word_428DC[1];
	word_428C8 = word_428DC[2];
	word_428CA = &word_428DC[3];
	word_428CC = 0;
	word_428D6 = -1;
	word_428D8 = -1;

	sub_1594F();
}

void cFodder::sub_159A6( ) {
	word_428B6 &= 0x1FE;
	uint16 bx = word_428B6;

	int32 ax = word_3EABF[ (bx / 2) & 0xFF ];

	ax >>= 2;

	mHelicopterPosX += ax* word_428B8;

	bx += 0x80;
	bx &= 0x1FE;

	ax = word_3EABF[ (bx / 2) & 0xFF];
	ax >>= 2;
	mHelicopterPosY += ax * word_428B8;

	bx = word_428C6 - word_428B6;
	bx >>= 5;
	--bx;
	bx ^= 0x0F;
	bx &= 0x0F;

	int16 al = byte_3E98F[bx];
	al <<= 2;
	word_428B6 += al;
	word_428B6 &= 0x1FE;

	if (word_428C4 != word_428B8) {
		if (word_428B8 >= word_428C4)
			word_428B8 -= 4;
		else
			word_428B8 += 4;
	}

	word_428CC += 2;
	if (word_428CC == 8)
		word_428CC = 0;

	--word_428BA;
	if (word_428BA <= 0)
		sub_1594F();

}

void cFodder::sub_1594F( ) {

	word_428B6 = word_428C6;
	word_428B8 = word_428C4;
	word_428BA = word_428C8;
	word_428C6 = *word_428CA++;
	word_428C4 = *word_428CA++;
	word_428C8 = *word_428CA++;

	if (word_428B6 == -1) {
		word_428D6 = 0;
		word_428D8 = 0;

		mGraphics->PaletteLoad( (word_42861 + word_4286D) - 0x300, 0x100, 0 );
		mImage->paletteFadeOut();
		mImageFaded = -1;
	}

}

void cFodder::sub_15A36( uint8* pDs, int16 pCx ) {
	int16 ax = pCx >> 2;
	int16 dx = ax;

	ax -= 0x50;
	word_4285F = -ax;

	word_4285D = mImage->GetSurfaceBuffer() + word_4285B + (dx*4);
	pCx &= 3;

	++dx;
	
	uint8* di = word_4285D;

	
	for (uint8 Plane = 0; Plane < 4; ++Plane) {
		di = word_4285D + Plane;

		for (int16 bx = word_42859; bx > 0; --bx) {
			int16 cx;
			for (cx = word_4285F; cx > 0; --cx) {
				uint8 al = *pDs++;
				if (al)
					*di = al;

				di += 4;
			}

			di -= 0x51 * 4;
			--pDs;
			for (cx = dx; cx > 0; --cx) {
				uint8 al = *pDs++;
				if (al)
					*di = al;

				di += 4;
			}

			di += 0x58 * 4;
		}
	}
}

void cFodder::sub_15B86(  uint8* pDs, int16 pCx ) {

	if (word_3E75B != 0)
		sub_15B98( pDs, pCx );
	else
		sub_15CE8( pDs, pCx );
}

void cFodder::sub_15B98(  uint8* pDsSi, int16 pCx ) {
	int16 ax = pCx >> 2;
	int16 dx = ax;

	ax -= 0x50;
	word_4285F = -ax;

	word_4285D = mImage->GetSurfaceBuffer() + word_4285B + (dx*4);
	pCx &= 3;

	++dx;
	
	uint8* di = word_4285D;

	for (uint8 Plane = 0; Plane < 4; ++Plane) {
		di = word_4285D + Plane;

		for (int16 bx = word_42859; bx > 0; --bx) {
			int16 cx = word_4285F;

			if (cx & 1) {
				*di = *pDsSi++;
				di += 4;
			}

			cx >>= 1;
			while (cx > 0) {
				*di = *pDsSi++;
				di += 4;

				*di = *pDsSi++;
				di += 4;

				--cx;
			}

			cx = dx;
			di -= 0x51 * 4;
			--pDsSi;
			if (cx & 1) {
				*di = *pDsSi++;
				di += 4;
			}

			cx >>= 1;
			while (cx > 0) {
				*di = *pDsSi++;
				di += 4;

				*di = *pDsSi++;
				di += 4;

				--cx;
			}
			di += 0x58 * 4;
		}
	}
}

void cFodder::sub_15CE8(  uint8* pDs, int16 pCx ) {
	//todo
}

void cFodder::Briefing_Intro() {
	mImage->clearBuffer();

	mGraphics->Briefing_Load_Resources();
	mGraphics->SetSpritePtr( eSPRITE_BRIEFING );

	mSound->Music_Play( 0x07 );

	if (mVersion->mPlatform == ePlatform::Amiga) {
		//TODO
		mDrawSpritePositionX = 16;

		mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( 5 );
		mDrawSpritePositionY = 40;
		mGraphics->video_Draw_Linear();

		mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( 6 );
		mDrawSpritePositionY = 60;
		mGraphics->video_Draw_Linear();

		mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( 7 );
		mDrawSpritePositionY = 100;
		mGraphics->video_Draw_Linear();

		mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( 8 );
		mDrawSpritePositionY = 163;
		mGraphics->video_Draw_Linear();

		((cGraphics_Amiga*)mGraphics)->PaletteBriefingSet();
		mImage->paletteFade();
		mImageFaded = -1;

		sub_136D0();
		Briefing_Draw_Mission_Name();
		mImage->Save();
		sub_1590B();
		do {
			if (word_428D6 == -1)
				sub_159A6();

			Mouse_Inputs_Get();

			/*mDrawSpriteFrameDataPtr = 0;
			mDrawSpritePositionX = mHelicopterPosX >> 16;		// X
			mDrawSpritePositionY = mHelicopterPosY >> 16;		// Y
			mDrawSpriteColumns = 0x40;
			mDrawSpriteRows = 0x18;
			if (Sprite_OnScreen_Check())
			mGraphics->video_Draw_Sprite();*/

			if (mImageFaded)
				mImageFaded = mImage->paletteFade();

			eventProcess();
			Video_Sleep();
			g_Window.RenderAt( mImage, cPosition() );
			g_Window.FrameEnd();

			Mouse_GetData();

			if (mMouse_Exit_Loop) {
				word_428D8 = 0;
				mImage->paletteFadeOut();
				mImageFaded = -1;
				mMouse_Exit_Loop = 0;
			}
		} while (word_428D8 || mImageFaded != 0);

		mMouse_Exit_Loop = 0;
	} 
	else {
		
		sub_136D0();
		Briefing_Draw_Mission_Name();

		switch (mMap_TileSet) {
		case eTileTypes_Jungle:
			Briefing_Intro_Jungle();
			break;

		case eTileTypes_Desert:
			Briefing_Intro_Desert();
			break;

		case eTileTypes_Ice:
			Briefing_Intro_Ice();
			break;

		case eTileTypes_Moors:
			Briefing_Intro_Mor();
			break;

		case eTileTypes_Int:
			Briefing_Intro_Int();
			break;
		}
	}

	mGraphics->LoadpStuff();
}

void cFodder::Briefing_Draw_Mission_Name( ) {

	word_3A01A = 0xB5;
	Briefing_Draw_Mission_Title();
}

void cFodder::Briefing_Draw_Mission_Title( ) {
	
	std::stringstream Mission;
	Mission << "MISSION ";

	word_3AC19 = 0x25;
	Mission << tool_StripLeadingZero( tool_NumToString( mMissionNumber ) );

	String_CalculateWidth( 0x140, mFont_Underlined_Width, Mission.str().c_str() );
	String_Print( mFont_Underlined_Width, 1, mGUI_Temp_X, 0, Mission.str().c_str() );
	
	int16 Data0 = mMissionNumber;
	const char** Data20 = mVersion->mMissionData->mMissionNames;

	if (word_3A01A != 0xB5) {
		Data20 = mVersion->mMissionData->mMissionPhaseNames;
		Data0 = mMapNumber + 1;
	}

	Data0 -= 1;
	Data20 += Data0;

	String_CalculateWidth( 0x140, mFont_Underlined_Width, *Data20 );
	Data0 = mMissionNumber;
	Data20 = mVersion->mMissionData->mMissionNames; 
	if (word_3A01A != 0xB5) {
		Data20 = mVersion->mMissionData->mMissionPhaseNames;
		Data0 = mMapNumber + 1;
	}
	else {
		if (mVersion->mPlatform == ePlatform::Amiga)
			word_3A01A += 0x16;
	}
	Data0 -= 1;
	Data20 += Data0;
	String_Print( mFont_Underlined_Width, 1, mGUI_Temp_X, word_3A01A, *Data20 );
}

void cFodder::CopyProtection() {

	if (mVersion->mVersion != eVersion::Dos_CD)
		return;

	g_Graphics.Load_Hill_Bits();
	g_Graphics.SetSpritePtr( eSpriteType::eSPRITE_HILL );

	int16 CopyProtectionAttempts = 0;

	for (; CopyProtectionAttempts < 3; ++CopyProtectionAttempts) {
		sub_136D0();

		int16 Data0;
		do {
			Data0 = tool_RandomGet() & 0x0F;
		}  while (Data0 == 0x0F);

		mImage->clearBuffer();
		mGraphics->PaletteSet();

		const sCopyProtection* word_44A1C = &mCopyProtection_Values[8];

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
		int8 byte_44B49 = 0;
		bool mShow = false;

		mGUI_Temp_X = 5;
		mGUI_Temp_Width = 10;
		memset( mInputString, 0, sizeof( mInputString ) );
		mInputString_Position = 0;

		mImage->Save();
		word_39F66 = 0;

		while (word_39F66 != 0x0D) {
			
			if (mImageFaded) {
				mImageFaded = mImage->paletteFade();
			}

			String_Print_Input( 0xA0 );

			++byte_44B49;
			byte_44B49 &= 0x0F;
			if (!byte_44B49)
				mShow = !mShow;

			if (mShow)
				Sprite_Draw_Frame( 0x0F, 0xA0, 0x00, mGUI_Temp_X + mGUI_Temp_Width );

			g_Window.RenderAt( mImage );
			g_Window.FrameEnd();
			eventProcess();

			mImage->Restore();
		}

		mImage->paletteFadeOut();

		while (mImage->GetFaded() == false) {
			g_Window.RenderAt( mImage );
			g_Window.FrameEnd();
			mImage->paletteFade();
		}

		mInputString[mInputString_Position] = 0xFF;

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

bool cFodder::Demo_ShowMenu() {

	mSound->Music_Stop();

	mGraphics->imageLoad( "apmenu.lbm", 32 );
	mGraphics->PaletteSet();

	((cGraphics_Amiga*)mGraphics)->SetCursorPalette( 0x10 );
	mWindow->SetScreenSize( cDimension( 320, 260) );

	mImage->Save();
	mImage->paletteFade();

	mDemo_ExitMenu = -1;
	word_3BEC3 = 4;

	for( ;; ) {
		Video_Sleep_Wrapper();

		Mouse_Inputs_Get();
		Mouse_DrawCursor();

		if (mButtonPressLeft) {
			if (mVersion->mVersion == eVersion::AmigaFormat)
				GUI_Element_Mouse_Over( mAfx_Buttons );

			if (mVersion->mVersion == eVersion::AmigaPlus)
				GUI_Element_Mouse_Over( mPlus_Buttons );
		}

		if (mDemo_ExitMenu > 0 || mMission_Aborted)
			break;

		if (mImage->GetFaded() == false )
			mImage->paletteFade();

		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->Restore(); 
	}
		
	mImage->paletteFadeOut();
	
	while( mImage->GetFaded() == false ) {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->paletteFade();
	}

	mImage->clearBuffer();
	((cGraphics_Amiga*)mGraphics)->SetCursorPalette( 0xE0 );
	if (mVersion->mPlatform == ePlatform::Amiga)
		mWindow->SetScreenSize( cDimension( 320, 225 ));

	if (mMission_Aborted)
		return true;

	return false;
}

bool cFodder::Recruit_Show() {
	mImage->clearBuffer();

	if (mVersion->mPlatform == ePlatform::Amiga)
		mWindow->SetScreenSize( cDimension( 320, 225 ));

	Mouse_Setup();
	map_ClearSpt();

	mMission_Aborted = 0;
	word_3E1B7 = mDataBaseBlk;

	mGraphics->Load_Hill_Data();

	dword_3B1FB = mRecruit_Screen_Positions;
	word_3AAD1 = -1;
	word_3AB39 = -1;

	sub_16BC3();
	sub_16C6C();
	Recruit_Render_LeftMenu();
	mGraphics->Recruit_Draw_Hill();

	if (mVersion->mPlatform == ePlatform::Amiga) {
		mGraphics->Recruit_Draw_HomeAway();

		//sub_A5A7E
		uint8* a0 = word_3E1B7 + (29 * 40);
		uint8* a1 = mDataBaseBlk + 0x3E8;

		for (int16 d1 = 0xB7; d1 >= 0; --d1) {
			uint8* a2 = a0 + 6;
			uint8* a3 = a1 + 6;

			for (int16 d0 = 0x10; d0 >= 0; --d0) {

				/*writeBEWord( a3 , readBEWord( a2 ) );
				writeBEWord( a3 + 0x2828, readBEWord( a2 + 0x2800 ) );
				writeBEWord( a3 + 0x5050, readBEWord( a2 + 0x5000 ) );
				writeBEWord( a3 + 0x7878, readBEWord( a2 + 0x7800 ) );*/

				writeBEWord( a2, 0 );
				writeBEWord( a2 + 0x2800, 0 );
				writeBEWord( a2 + 0x5000, 0 );
				writeBEWord( a2 + 0x7800, 0 );
				a2 += 2;
				a3 += 2;
			}

			a0 += 0x28;
			a1 += 0x28;
		}
	}

	sub_17B64();
	
	if (mVersion->mPlatform == ePlatform::Amiga) {
		
		((cGraphics_Amiga*)mGraphics)->sub_A5B46();
	}
	
	mGraphics->SetSpritePtr( eSPRITE_HILL_UNK );
	
	Recruit_Draw_Graves();
	
	mGraphics->PaletteSet();
	mImage->paletteFade();
	mImage->Save();

	word_3BEC1 = 0;
	word_3BEC3 = 0x1D;
	//word_39020 = 0;
	
	Recruit_Render_Names_UnusedSlots();

	dword_3AAC9 = mRecruit_Rendered;
;

	for (int16 ax = mSquadMemberCount - 1; ax >= 0;--ax )
		sub_17429();
	
	mMouseCursor_Enabled = -1;
	word_3A016 = 0;
	word_3AA67 = 0;
	word_3AA71 = mMapPlayerTroopCount;
	
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
	
	mMouseSpriteNew = 0x23;
	mMouseCursor_Enabled = 0;
	
	mImage->paletteFadeOut();
	
	while( mImage->GetFaded() == false ) {
		Recruit_Draw();
	}

	if (mMission_Aborted)
		return true;

	return false;
}

void cFodder::Recruit_Draw_String(  int16 pParam0, int16 pParam8, int16 pParamC, const std::string& pString ) {

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

		Sprite_Draw_Frame( pParam0, pParamC, NextChar, pParam8 );
	}
}

void cFodder::sub_16BC3() {
	uint16* di = word_3BDAD;	// 20h
	int16  ax = mMapPlayerTroopCount;
	int16  count;
	
	if( ax > 0 ) {
		
		sub_16C45( &di, word_3E115 );
		count = mMapPlayerTroopCount;
		count -= 2;

		while( count >= 0 ) {
			sub_16C45( &di, word_3E135 );
			--count;
		}
		
		sub_16C45( &di, word_3E12B );
		sub_16C45( &di, word_3E15D );
	}

	*di++ = 0;
	*di = -1;
}

void cFodder::sub_16C45( uint16** pDi, int16* pSource ) {
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

void cFodder::Recruit_Draw_Grave( int16 pSpriteType, int16 pPosX, int16 pPosY ) {
	pSpriteType >>= 1;
	if (pSpriteType > 8)
		pSpriteType = 8;

	int16 Frame = pPosY;
	Frame -= 0x14;
	Frame >>= 5;
	if (Frame >= 5)
		Frame = 4;

	pPosY -= 8;
	Sprite_Draw_Frame( pSpriteType, pPosY, Frame, pPosX );
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
	int16 DataC	= 0x18;
	
	// Draw Heroes Heading
	mGraphics->sub_145AF( Data0, Data8, DataC );
	
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
		mGraphics->sub_145AF( Data0, Data8, DataC + 0x18 );
		DataC += 0x0C;
		
	} while( DataC < 0x4A );

	//seg003:1E89

	// Draw Squad Heading
	Data0 = 0xAE;
	Data8 = 0;
	DataC = 0x4A + 0x18;
	mGraphics->sub_145AF( Data0, Data8, DataC );
	
	Data14 = mMapPlayerTroopCount + mSquadMemberCount;
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
		mGraphics->sub_145AF( Data0, Data8, DataC + 0x18 );
		DataC += 0x0C;
		
	} while( DataC < Final );

	word_3BEC9 = 0xB8;
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
	word_3A3BD = 0;
	
	for( uint16 x = 0; x < 8; ++x, --word_3A3BB ) {
		sSquad_Member* Member = &mSquad[x];
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
				
				int16 Data8 = Data14;
				Data8 <<= 2;
				Data8 += word_3A05F;
				int16 DataC = 0x4B;
				DataC += word_3A3BD;
				DataC += word_3AA55;

				mGraphics->sub_145AF( Data0, Data8, DataC + 0x19 );
			}
		}

		word_3A3BD += 0x0C;
	}

}

void cFodder::Recruit_Render_Squad_RankKills() {
	int16 Data0;

	int16 word_3A061 = 7;
	word_3A3BD = 0;
	
	int16 Data4;
	
	for( uint16 x = 0; x < 8; ++x, --word_3A061 ) {
		sSquad_Member* Member = &mSquad[x];
		if (Member->mRecruitID == -1)
			continue;

		
		if( Member->mSprite == INVALID_SPRITE_PTR || Member->mSprite == 0 )
			continue;
		
		Data4 = mSquad_Selected;
		sSprite* Data34 = Member->mSprite;
		
		if( Data4 != Data34->field_32  )
			continue;
		
		int16 Data8 = 0;
		int16 DataC = word_3A3BD;
		DataC += 0x4A;
		DataC += word_3AA55;
		
		if( !word_3AAC7 ) {
			Data0 = 0;
			Data0 = Member->mRank;
			Data0 += 9;
			Data8 = 0x23;
			DataC = word_3A3BD;
			DataC -= 1;
			DataC += 0x4A;
			DataC += word_3AA55;
			DataC += 0x19;
			
			mGraphics->sub_145AF( Data0, Data8, DataC );
			
			DataC -= 0x19;
			
		} else {
			// Draw Kills
			Recruit_Render_Number( Member->mNumberOfKills, 0x43 );
			
		}
		
		//Draw Rank
		Data0 = Member->mRank + 9;
		DataC = word_3A3BD - 1;

		DataC += 0x4A;
		DataC += word_3AA55;
		DataC += 0x19;
		mGraphics->sub_145AF( Data0, 0, DataC );
		DataC -= 0x19;
		word_3A3BD += 0x0C;
	}
}

void cFodder::Recruit_Render_Number( int16 pNumber, int16 pData10 ) {
	
	pData10 -= 0x30;
	std::string Data20 = tool_StripLeadingZero(tool_NumToString( pNumber ));
	uint16 Data0 = (uint16) Data20.length() * 4;

	int16 Data8 = 0x30 - Data0;
	int16 DataC = 0x4B + word_3A3BD + word_3AA55;

	for (std::string::iterator CharIT = Data20.begin(); CharIT != Data20.end(); ++CharIT) {

		Data0 = (*CharIT & 0xFF) + pData10;
		mGraphics->sub_145AF( Data0, Data8, DataC + 0x19 );
		Data8 += 4;
	}
}

void cFodder::Recruit_Render_HeroList() {
	const sHero* Hero = mHeroes;

	word_3A3BB = 4;
	word_3A3BD = 0;

	for (word_3A3BB = 4; word_3A3BB >= 0; --word_3A3BB, ++Hero) {

		if (Hero->mRecruitID < 0)
			continue;

		int16 Data8 = 0;
		int16 DataC = word_3A3BD - 1;
		DataC += 0x4A + word_3AA55+ 0x19;

		mGraphics->sub_145AF( Hero->mRank + 9, Data8, DataC );

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
			DataC += word_3A3BD + word_3AA55;

			mGraphics->sub_145AF( Character, Data8, DataC  + 0x19 );
		}

		Recruit_Render_Number( Hero->mKills, 0x67 );
		word_3A3BD += 0x0C;
	}
}

void cFodder::Recruit_Render_Names_UnusedSlots() {
	uint32* Data20 = (uint32*) mDataArmy;

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

		mGraphics->sub_17480( Data0, 0x0C, 0, Data20 );
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
		mGraphics->sub_145AF( 0xAC, 0, DataC + 0x18 );

		DataC += 0x0C;
	}
}

void cFodder::sub_17429() {
	//uint8* Data24 = dword_3AAC9;
	sRecruitRendered* Data24 = dword_3AAC9 ;
	if (Data24->mPosition < 0)
		return;

	++dword_3AAC9;
	mGraphics->sub_17480( (uint16) Data24->mPosition, 0x0C, -1, Data24->mDataPtr );
}

void cFodder::Recruit_Draw_Actors( ) {
	word_42072 = 2;

	if (mVersion->mPlatform == ePlatform::Amiga)
		((cGraphics_Amiga*)mGraphics)->mBMHDHill.mHeight = 0x101;

	Recruit_Draw_Truck();
	Recruit_Draw_Troops();

	if (mVersion->mPlatform == ePlatform::Amiga)
		((cGraphics_Amiga*)mGraphics)->mBMHDHill.mHeight = 0x100;
}

void cFodder::sub_175C0() {
	sRecruit_Screen_Pos* Data20 = 0;

	if (word_3B1F1 > 0) {

		sRecruit_Screen_Pos* Data20 = &mRecruit_Screen_Positions[293];
		do {
			if (Data20 == &mRecruit_Screen_Positions[0])
				return;

			--Data20;

		} while (Data20->field_4 == 0 || Data20->field_4 < 4);

		int16 Data0 = Data20->field_4;
		Data20->field_4 = 0;
		Data0 ^= 1;

		if ((Data20 + 1)->field_0 >= 0) {
			(Data20 + 1)->field_4 = Data0;
		} else {
			--word_3B1F1;
			sub_17429();
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

		if (Data20->field_4 == 0)
			continue;

		if (Data20->field_4 < 4)
			continue;

		if (Data20 >= Data28)
			continue;

		int16 Data8 = 3;
		sRecruit_Screen_Pos* Data30 = Data20;
		//loc_176CE

		do {
			++Data30;
			if (Data30->field_4) {
				if (Data30->field_4 > 3)
					goto loc_17686;

				++Data30;
			}
			--Data8;
		} while (Data8 >= 0);

		int16 Data0 = Data20->field_4;
		Data20->field_4 = 0;
		if ((dword_3B1CB >> 16) < 0)
			Data0 ^= 1;

		if ((Data20 + 1)->field_4) {
			(Data20 + 2)->field_4 = Data0;
		}
		else {
			(Data20 + 1)->field_4 = Data0;
		}
	}

}

void cFodder::Recruit_Draw_Troops() {

	if (mRecruit_Reached_Truck) {
		sub_175C0();
	}
	else {
		sub_178DD();
		sub_17911();
	}

	sRecruit_Screen_Pos* Data20 = mRecruit_Screen_Positions;
	sRecruit_Screen_Pos* dword_3B1C7;

	for ( ; ;Data20 = dword_3B1C7) {

		dword_3B1C7 = Data20;

		if (Data20->field_0 < 0)
			break;

		int16 Data0;
		int16 Data8 = Data20->field_0;
		int16 DataC = Data20->field_2;
		int16 Data4 = Data20->field_4;
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

			if (DataC >= 0xB7)
				Data0 += 4;
			else if (DataC > 0x7C )
				Data0 += 2;

			Data4 = 0;

			if (Data20 > &mRecruit_Screen_Positions[0x46]) {
				Data4 = 1;

				if (Data20 > &mRecruit_Screen_Positions[0x7A])
					Data4 = 2;
			}
		}
		else
			Data0 = 9;

		//loc_1784C
		if (mVersion->mPlatform == ePlatform::PC) {
			DataC -= 8;

			if (Data0 != 9)
				Sprite_Draw_Frame( Data0, DataC, Data4, Data8 );
			else {
				++Data8;
				--DataC;
				sub_13C8A( Data0, Data4, Data8, DataC );
			}
		}
		else {
			Sprite_Draw_Frame( Data0, DataC, Data4, Data8 );
		}
	}
}

void cFodder::sub_1787C() {
	
	if (word_390B8)
		return;
	
	word_3B1F1 = mMapPlayerTroopCount;
	mRecruit_Reached_Truck = 0;

	int16 Data0 = 0xB4;
	int16* Data20 = word_3B1CF;
	const int16* Data24 = word_3E177;

	for ( int16 Data4 = 0x0E; Data4 >= 0; --Data4 ) {

		*Data20++ = Data0;
		Data0 -= *Data24++;

	}
}

void cFodder::sub_178DD() {
	sRecruit_Screen_Pos* Data20 = mRecruit_Screen_Positions;

	for (; Data20->field_0 >= 0; ++Data20) {
		
		if (Data20->field_4 >= 4)
			Data20->field_4 = 0;
	}
}

void cFodder::sub_17911() {
	sRecruit_Screen_Pos* Data24 = mRecruit_Screen_Positions;
	const int16*    Data2C = word_3E197;

	uint64 Data8 = (uint64) mGraveRankPtr;
	int16 Data0, Data4;

	Data8 -= (uint64) mGraveRanks;
	Data8 >>= 1;
	Data8 &= 0x1E;

	if (word_390B8)
		Data0 = mSquad_AliveCount;
	else {
		Data0 = word_390E8;
		if (!Data0)
			goto loc_179B2;
	}

	// loc_1795E
	--Data0;
	Data4 = 0x660;

	//loc_17968
	for (; Data0 >= 0; --Data0) {

		if (Data4 >= 0) {
			if ((Data24 + (Data4/6))->field_4)
				Data4 -= 6;

			(Data24 + (Data4/6))->field_4 = *(Data2C + (Data8/2));
		}

		Data8 += 2;
		Data8 &= 0x1F;
		Data4 -= 0x18;
	}

loc_179B2:;
	if (word_390B8)
		return;

	word_3B1ED = 0;
	Data0 = word_3B1CF[0];
	Data8 = 5;

	for ( ; ; Data0+=6 ) {

		Data4 = (Data24 + (Data0/6))->field_4;
		if (Data4) {
			if (Data4 > 3) {
				word_3B1ED = -1;
				break;
			}
			else
				continue;
		}
		else {
			if (Data0 == 0x660) {
				word_3B1ED = -1;
				break;
			}
			--Data8;
			if (!Data8)
				break;
		}
	}
	//seg003:2B6D
	if (!word_3B1ED) {
		dword_3B1CB += 0x8000;
		dword_3B1CB &= 0x1FFFF;
	}
	else
		dword_3B1CB = 0x10000;

	int16* Data20 = word_3B1CF;

	for (int16 DData8 = 0x0E; DData8 >= 0; --DData8) {

		Data0 = *Data20;
		if (Data0 >= 0) {
			Data4 = (dword_3B1CB >> 16);

			Data4 += *Data2C++;
			if ((Data24 + (Data0/6))->field_0 < 0)
				Data0 = 0;

			if ((Data24 + (Data0/6))->field_4)
				Data0 += 6;

			(Data24 + (Data0/6))->field_4 = Data4;
		}
		// 17A9D

		if (!word_3B1ED)
			Data0 += 6;

		//loc_17AA9
		*Data20 = Data0;
		++Data20;
	}

	if (word_3B1ED)
		mRecruit_Reached_Truck = -1;
}

void cFodder::Recruit_Draw_Truck( ) {
	int16 Data0 = 0x22;
	int16 Data4 = word_3AACD;
	int16 Data8 = 0x31;
	int16 DataC = 0xB6;

	if (mVersion->mPlatform == ePlatform::Amiga)
		DataC = 0xBE;

	Sprite_Draw_Frame(  Data0, DataC, Data4, Data8 );

	if (!mRecruit_Reached_Truck)
		return;

	uint16* Data20 = word_3BDAD;

	Data0 = *(Data20 + word_3AACF);
	if (Data0 < 0)
		return;

	++word_3AACF;
	word_3AACD = Data0;
}

void cFodder::sub_17B64() {

	const sRecruitSpritePos* stru = mRecruitSprite;

	for (; stru->field_0 != -1;) {
		int16 word_3B19F = stru->field_0;
		int16 word_3B1A1 = stru->field_2;
		int16 word_3B1A3 = stru->field_4;
		int16 word_3B1A5 = stru->field_6;
		int16* Data34 = stru->field_8;

		for (;;) {
			int16 Data0 = word_3B19F;
			int16 Data4 = word_3B1A1;
			int16 Data8 = word_3B1A3;
			int16 DataC = word_3B1A5;

			uint8* Data20 = Sprite_Get_Gfx_Ptr( Data0, Data4 );

			Data8 = *Data34++;
			if (Data8 < 0) {
				++stru;
				break;
			}

			// Originally Inside sub_A094C
			if (mVersion->mPlatform == ePlatform::Amiga) {
				Data0 -= 1;
				Data0 <<= 4;
			}

			sub_2AEB6( Data0, Data4, &Data8, &DataC );
			int16 Data10 = word_3B1A3 + 0x08;
			int16 Data14 = word_3B1A5;
			mGraphics->sub_2AF19( Data0, Data4, Data8, Data10, Data14, DataC, Data20 );
			word_3B1A3 += 0x10;
		}
	}
}

void cFodder::Recruit_Draw() {
	Mouse_Inputs_Get();
	
	Recruit_Draw_Actors();
	mGraphics->sub_144A2(0x18);

	if (mVersion->mPlatform == ePlatform::PC)
		mGraphics->Recruit_Draw_HomeAway();

	Mouse_DrawCursor();
	Video_Sleep_Wrapper();

	if (mImage->GetFaded() == false )
		mImage->paletteFade();

	mDrawSpritePositionX = 0x40;
	mDrawSpritePositionY = 0x28;
	mDrawSpriteColumns = 0x110;
	mDrawSpriteRows = 0xB0;

	//sub_14367();

	g_Window.RenderAt( mImage );
	g_Window.FrameEnd();
	mImage->Restore();
}

bool cFodder::Recruit_Check_Buttons_SaveLoad() {
	mGame_Load = 0;
	mGame_Save = 0;

	if (mMouseX <= -16 && mMouseY <= 26) {
		mGame_Load = -1;
		return true;
	}

	if (mMouseX >= 0x10F) {
		if (mMouseY > 0x1A)
			return true;

		if (mMission_Save_Availability[mMissionNumber-1])
			return false;

		mGame_Save = -1;
		return true;
	}

	if (mMouseY < 0x1A)
		return false;

	return true;
}

void cFodder::Sprite_Handle_Player_Unk( sSprite* pSprite ) {
	
	if (!pSprite->field_66)
		return;

	sSprite* Data28 = pSprite->field_66;
	
	int16 Data0 = Data28->field_0;
	Data0 += 0x10;

	if (Data28->field_6F || Data28->field_6F == 1)
		Data0 -= 0x0C;

	int16 Data4 = Data28->field_4;
	Data4 -= 9;
	int16 Data8 = pSprite->field_0;
	Data8 += 4;
	int16 DataC = pSprite->field_4;

	int16 Data10 = 0x1F;
	Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
	
	if (Data0 >= 0x0D)
		return;

	pSprite->field_6A = Data28;
	pSprite->field_6E = -1;
	sSprite* Data20 = 0;

	sub_305D5( Data20 );

	sSprite** Data24 = mSquads[pSprite->field_32];
	sSprite* eax = Data24[0];

	if (eax != pSprite)
		return;

	word_3B461[pSprite->field_32] = 0x190;
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

	GUI_Sidebar_Grenades_Refresh_CurrentSquad();
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

		GUI_Sidebar_Rockets_Refresh_CurrentSquad();
	}
}


void cFodder::GUI_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper( ) {
	GUI_Sidebar_Rockets_Refresh_CurrentSquad();
}

void cFodder::sub_30AB0() {
	int16 Data0, Data4, Data8, Data18;
	sSprite* Data20 = 0;
	sSprite** Data24 = 0;

	if (mMouseX < 0x10) {
		mMouseSpriteNew = 0x23;
		word_3A9F3 = 0;
		word_3A9F5 = 0;
		return;
	}

	if (mButtonPressRight)
		goto loc_30CBC;

	Data0 = mMouseX;
	Data4 = mMouseY;

	Data0 += mCamera_Adjust_Col >> 16;
	Data4 += mCamera_Adjust_Row >> 16;

	Data0 -= 0x0F;
	Data4 -= 3;
	
	Data24 = dword_3B213;

	//loc_30B36
	for (int16 Data1C = 4; Data1C >= 0; --Data1C) {

		if (*Data24 == INVALID_SPRITE_PTR )
			goto loc_30CBC;

		if (*Data24 == 0)
			continue;

		Data20 = *Data24++;
		if (Data20->field_0 < 0)
			continue;

		if (!Data20->field_65)
			continue;

		if (Data20->field_22)
			continue;

		Data18 = Data20->field_18;
		Data8 = Data20->field_0;
		if (Data20->field_6F == 0)
			goto loc_30BB9;
		if (Data20->field_6F == 1)
			goto loc_30BBE;

	loc_30BB9:;
		Data8 -= 8;
	loc_30BBE:;

		if (Data0 < Data8)
			continue;

		Data8 += mSprite_Width[Data18];
		if (Data0 > Data8)
			continue;

		Data8 = Data20->field_4;
		Data8 -= Data20->field_20;
		Data8 -= mSprite_Height_Top[Data18];
		Data8 -= 0x14;

		if (Data4 < Data8)
			continue;

		Data8 = Data20->field_4;
		Data8 -= Data20->field_20;
		if (Data4 > Data8)
			continue;

		word_3AA01 = -1;

		if (!Data20->field_20)
			goto loc_30C7C;

		if (Data20 != mSquad_CurrentVehicle)
			return;

		if (Data20->field_8 == 0xA5)
			return;

		mMouseSpriteNew = 0x27;
		word_3A9F3 = 0;
		word_3A9F5 = 0;
		return;

	loc_30C7C:;

		if (Data20 == mSquad_CurrentVehicle) {
			mMouseSpriteNew = 0x25;
			word_3A9F3 = 0;
			word_3A9F5 = 0;
			return;
		}

		mMouseSpriteNew = 0x26;
		word_3A9F3 = 0;
		word_3A9F5 = 0;
		return;
	}

loc_30CBC:;

	if (word_3AA01) {
		word_3AA01 = 0;
		mMouseSpriteNew = 0x23;
		word_3A9F3 = 0;
		word_3A9F5 = 0;
	}
}

void cFodder::sub_22C87( sSprite* pSprite ) {
	
	int64 Data0 = (int64) pSprite->field_1A;
	int64 tmp = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	tmp += Data0;

	pSprite->field_1E = (tmp & 0xFFFF);
	pSprite->field_20 = (tmp >> 16);
	if (tmp < 0) {
		pSprite->field_1E = 0;
		pSprite->field_20 = 0;
		Data0 = -Data0;
		Data0 >>= 1;
	}

	Data0 -= 0x20000;
	pSprite->field_1A = (int32*) Data0;
}

void cFodder::sub_22CD7( sSprite* pSprite, int16& pData0, int16& pData4 ) {
	
	int16 Data1C;

	pSprite->field_3C = pSprite->field_10;

	sub_2A1F0( pSprite, pData0, pData4 );

	word_3B2F5 = pSprite->field_10;
	pSprite->field_3E = 0;

	sub_2B12E( pSprite );

	pSprite->field_10 = pSprite->field_3C;

	pData0 = word_3B2F7;
	pData0 -= pSprite->field_36;

	pData0 *= word_3A8CF;
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

	if (pSprite->field_6F == 2)
		goto loc_22DC4;

	if (pSprite->field_6F == 3)
		goto loc_22DC4;

	if (pSprite->field_6F == 4)
		goto loc_22DCF;

	if (pSprite->field_36 <= 6)
		return;

	if (!word_390B0 & 1)
		return;

loc_22DC4:;
	if (pSprite->field_36 <= 8)
		return;

loc_22DCF:;
	pData0 = -pData0;
	if (!pData0)
		goto loc_22DE2;

	if (pData0 >= 0)
		goto loc_22DED;

	Data1C = -1;
	goto loc_22DF6;

loc_22DE2:;
	Data1C = 0;
	goto loc_22DF6;

loc_22DED:;
	Data1C = 1;

loc_22DF6:;
	Sprite_Vehicle_Direction_Update( pSprite, Data1C );

}

void cFodder::sub_22DFC( sSprite* pSprite ) {
	int16 Data4, Data0;
	int32 Dataa0;

	pSprite->field_50 = pSprite->field_52;
	if (!pSprite->field_50)
		pSprite->field_52 = 0;

	if (Map_Sprite_Check_Around_Position( pSprite ))
		goto loc_22F30;

	Data4 = 0x0F;
	Data0 = -10;
	Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );

	pSprite->field_60 = Data4;
	if (Data4 == 1 || Data4 == 2)
		goto loc_23056;

	if (Data4 == 0x0E)
		goto loc_23100;

	if (Data4 == 3)
		goto loc_22F30;

	if (Data4 == 9 || Data4 == 0x0A)
		goto loc_22F06;

	if (pSprite->field_56)
		pSprite->field_38 = 5;

	if (Data4 == 7)
		goto loc_22FA3;

	if (!pSprite->field_22)
		goto loc_22EC7;

	if (Data4 == 4 || Data4 == 5 || Data4 == 6 || Data4 == 0x0B)
		goto loc_22F30;
	
	goto loc_22EEB;

loc_22EC7:;

	if (Data4 == 4)
		goto loc_22FD7;

	if (Data4 == 5)
		goto loc_22FE4;

	if (Data4 == 6 || Data4 == 0x0B)
		goto loc_22FF1;

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

loc_22F30:;
	
	if (pSprite->field_22) {
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
	if (Data4 == 3)
		pSprite->field_38 = 5;

	sub_2329D( pSprite );
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

loc_22FD7:;
	pSprite->field_52 = 1;
	return;

loc_22FE4:;
	pSprite->field_52 = 3;
	return;

loc_22FF1:;
	if (pSprite->field_56)
		pSprite->field_38 = 0;
	dword_3B213[0x0E] = INVALID_SPRITE_PTR;

	sub_2449E( pSprite );
	if (pSprite->field_52 < 0x50)
		goto loc_23033;

	pSprite->field_52 = 0x1E;
	pSprite->field_38 = 7;
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

	Data0 = word_390AE;
	Data0 &= 1;
	Data0 += 3;
	pSprite->field_20 = Data0;
	Data0 = -Data0;
	pSprite->field_1A = (int32*) (((int64)pSprite->field_1A & 0xFFFF) | Data0 << 16);
	return;

loc_230B0:;
	if (!pSprite->field_36)
		return;

	pSprite->field_20 = 1;
	Dataa0 = pSprite->field_36 << 16;
	Dataa0 >>= 3;
	pSprite->field_1A = (int32*)Dataa0;
	return;

loc_23100:;
	if (!pSprite->field_36)
		return;

	pSprite->field_20 = 1;
	Dataa0 = pSprite->field_36 << 16;
	Dataa0 >>= 2;
	pSprite->field_1A = (int32*)Dataa0;
	pSprite->field_36 <<= 1;
	pSprite->field_36 <<= 1;

}

void cFodder::Sprite_Under_Vehicle( sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14, int16 pData18, int16 pData1C ) {

	if (mMission_Finished)
		return;

	int16 Data28 = pSprite->field_22;
	const int16* Data30 = mSprite_Can_Be_RunOver;
	sSprite* Data24 = mSprites;

	for (int16 Data4 = 0x1D; Data4 >= 0; --Data4, ++Data24) {
		if (Data24->field_0 == -32768)
			continue;

		if (Data24 == pSprite)
			continue;

		int16 Data0 = Data24->field_18;
		if (!Data30[Data0])
			continue;

		if (Data28 == Data24->field_22)
			continue;

		if (Data24->field_18 == eSprite_Hostage)
			continue;

		if (Data24->field_18 == eSprite_Hostage_2)
			continue;

		if (pData8 > Data24->field_0)
			continue;

		if (pDataC < Data24->field_0)
			continue;

		if (pData10 > Data24->field_4)
			continue;

		if (pData14 < Data24->field_4)
			continue;

		Data0 = Data24->field_20;
		if (pData1C < Data0)
			continue;
		Data0 += 0x0E;
		if (pData18 > Data0)
			continue;

		if (!Data24->field_18) {
			if (Data24->field_6E)
				continue;
		}

		// Run Over
		Data24->field_38 = 5;
		if (!Data24->field_65)
			return;

		pSprite->field_38 = 5;
		return;
	}
}

int16 cFodder::sub_2329D( sSprite* pSprite ) {
	sSprite* Data24 = 0;

	if (pSprite->field_38 == 5)
		goto loc_232DE;

	if (pSprite->field_38 == 7)
		goto loc_232DE;

	if (pSprite->field_38 == 0x32)
		goto loc_23328;

	if (pSprite->field_38 == 0x33)
		goto loc_2333C;

	return 0;

loc_232D2:;
	return -1;

loc_232DE:;
	pSprite->field_18 = eSprite_Explosion;
	pSprite->field_26 = 0x1F56;
	pSprite->field_28 = -9;
	Data24 = pSprite + 1;

	Sprite_Destroy( Data24 );
	if (!pSprite->field_22)
		goto loc_232D2;

	Data24 = pSprite + 2;
	Sprite_Destroy( Data24 );
	goto loc_232D2;

loc_23328:;
	pSprite->field_36 = -pSprite->field_36;
	pSprite->field_38 = 0x33;
	goto loc_2335B;

loc_2333C:;
	pSprite->field_36 += 6;
	if (pSprite->field_36 >= 0) {
		pSprite->field_38 = 0;
		pSprite->field_36 = 0;
	}
loc_2335B:;
	Sprite_Movement_Calculate( pSprite );
	sub_243E9( pSprite );
	goto loc_232D2;

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
		pSprite->field_22 = 0;
		sub_1B4BB( pSprite );
		pSprite->field_22 = 0;
		return;
	}

	if (pSprite->field_6F == 3)
		sub_245BF( pSprite );

	word_3B175 = 0;
	pSprite->field_22 = 0;
	sub_1B4BB( pSprite );
	pSprite->field_22 = 0;
	word_3B175 = 0;

	sSprite* Data24 = pSprite + 1;
	Data24->field_18 = eSprite_Null;
}

int16 cFodder::sub_23444( sSprite* pSprite, sSprite*& pData2C ) {
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
	if (!dword_3B213[0xE])
		goto loc_234D8;

	Data0 = tool_RandomGet() & 0x1F;
	Data0 -= 8;
	pData2C->field_0 += Data0;
	Data0 >>= 4;
	Data0 &= 0x0F;
	Data0 -= 8;
	pData2C->field_4 -= Data0;
	dword_3B213[0xE] = 0;

loc_234D8:;
	pData2C->field_20 = pSprite->field_20;
	pData2C->field_52 = 0;
	pData2C->field_8 = 0xCC;
	pData2C->field_A = 4;
	pData2C->field_2C = 1;
	pData2C->field_18 = 0x39;
	return 0;
}

void cFodder::sub_23525( sSprite* pSprite ) {
	int16 Data0 = 2;
	int16 Data4 = pSprite->field_36;
	int16 Data8, DataC, Data10, Data14, Data18, Data1C;
	sSprite* Data2C = 0;

	if (!Data4) {
		if (!pSprite->field_22) {
			if (pSprite != mSquad_CurrentVehicle)
				goto loc_2356B;
		}
	}

	Data4 += 0x10;
	if (Data4 > 0x20)
		Data4 = 0x20;

	Sprite_Map_Sound_Play( Data0 );

loc_2356B:;
	pSprite->field_6F = 4;
	pSprite->field_65 = -1;
	if (sub_2329D( pSprite ))
		return;

	pSprite->field_8 = 0xD1;
	word_3B2F7 = pSprite->field_36;
	word_3B2F7 += 0x20;

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
	dword_3B213[0x0E] = INVALID_SPRITE_PTR;
	if (!sub_23444( pSprite, Data2C))
		Data2C->field_2C = -1;

	if (!pSprite->field_36) {
		if (word_3A8CF) {
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
	sub_22DFC( pSprite );

	pSprite->field_20 = 0;
	sub_236F7( pSprite );

	Data8 = pSprite->field_0;
	DataC = pSprite->field_0;
	DataC += 0x1E;
	Data10 = pSprite->field_4;
	Data10 -= 0x14;
	Data14 = pSprite->field_4;
	Data18 = pSprite->field_20;
	Data1C = Data18;
	Data1C += 0x0E;

	Sprite_Under_Vehicle( pSprite, Data8, DataC, Data10, Data14, Data18, Data1C );
}

void cFodder::sub_236F7( sSprite* pSprite ) {
	sSprite* Data24 = pSprite + 1;
	int16 Data0, Data4, Data8, DataC;

	Data24->field_8 = 0xD2;
	Data24->field_0 = pSprite->field_0;
	Data24->field_0 += 0x0A;
	Data24->field_4 = pSprite->field_4;
	Data24->field_4 += 1;

	Data0 = pSprite->field_52;
	Data24->field_4 += Data0;
	Data24->field_20 = pSprite->field_20;
	Data24->field_20 += 9;
	Data0 = Data24->field_0 + 8;
	Data4 = Data24->field_4 - 5;

	Data4 -= Data24->field_20;
	Data24->field_52 = pSprite->field_52;

	if (!pSprite->field_22)
		goto loc_23815;

	Data8 = pSprite->field_2E;
	if (Data8 >= 0)
		goto loc_23806;

	Data0 = word_390B0 & 0x0F;
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
	Direction_Calculate( Data0, Data4, Data8, DataC );

	Data4 -= 0x10;
	Data4 >>= 5;
	Data4 += 8;
	Data4 ^= 0x0F;
	Data4 &= 0x0F;
	Data24->field_A = Data4;
}

void cFodder::sub_23879( sSprite* pSprite ) {
	if (!pSprite->field_57)
		if (!pSprite->field_54)
			return;

	pSprite->field_54 = 0;
	sSprite* Data24 = pSprite + 1;

	Data24->field_2E = pSprite->field_2E;
	Data24->field_30 = pSprite->field_30;

	int16 Data24_Field_0 = Data24->field_0;
	int16 Data24_Field_4 = Data24->field_4;
	sSprite* Data20 = pSprite;
	pSprite = Data24;

	int16 Data0 = pSprite->field_A << 1;

	pSprite->field_0 += mSprite_Turret_Positions[Data0];
	pSprite->field_4 += mSprite_Turret_Positions[Data0 + 1];

	word_3B189 = 0;
	word_3B18B = 0;
	sSprite* Data2C = 0;

	if (!Sprite_Create_Missile( pSprite, Data2C )) {
		pSprite = Data20;
		Data24->field_4 = Data24_Field_4;
		Data24->field_0 = Data24_Field_0;
		pSprite->field_57 = -1;
		return;
	}

	Data2C->field_20 += 0x11;
	Data2C->field_36 = 0x3C;
	Sound_Play( pSprite, 5, 0x1E );
	sub_23444( pSprite, Data2C );
	
	pSprite = Data20;
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

	pData2C->field_0 += word_3B189;
	Data30->field_0 += word_3B189;
	if (Data30->field_0 < 0)
		return 0;

	pData2C->field_4 += word_3B18B;
	Data30->field_4 += word_3B18B;

	if (Data30->field_4 < 0)
		return 0;

	pData2C->field_32 = 0;
	if (!pSprite->field_6F)
		pData2C->field_32 = -1;

	pData2C->field_1E = pSprite->field_1E;
	pData2C->field_20 = pSprite->field_20;
	pData2C->field_20 -= 0x0D;
	pData2C->field_26 = pSprite->field_2E;
	pData2C->field_28 = pSprite->field_30;
	pData2C->field_28 += 0x10;

	if (pSprite->field_22) {
		sub_23C70( pData2C );
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
	pData2C->field_2C = 0;
	Data30->field_2C = -1;
	pData2C->field_38 = 0;

	if (mVersion->mVersion != eVersion::AmigaPlus)
		Sound_Play( pSprite, 0x2D, 0x0F );

	return -1;
}

void cFodder::sub_23C70( sSprite* pData2C ) {
	
	int16 Data0 = tool_RandomGet();
	int16 Data4 = Data0;

	if (mSprite_Enemy_Aggression_HalfOfMinPlusMax < 5)
		Data0 &= 0x3F;
	else
		Data0 &= 0x1F;

	if (Data4 < 0)
		Data0 = -Data0;

	pData2C->field_26 += Data0;

	Data0 = tool_RandomGet();
	Data4 = Data0;

	if (mSprite_Enemy_Aggression_HalfOfMinPlusMax < 5)
		Data0 &= 0x3F;
	else
		Data0 &= 0x1F;
	if (Data4 < 0)
		Data0 = -Data0;

	pData2C->field_28 += Data0;
}

void cFodder::Sprite_Handle_Helicopter_Human( sSprite* pSprite ) {
	
	if (pSprite->field_38) {
		pSprite->field_22 = 0;
		Sprite_Handle_Helicopter( pSprite );
		pSprite->field_22 = 0;

		return;
	}

	Sprite_Handle_Helicopter_Human_Deploy_Weapon( pSprite );

	word_3B175 = 0;
	pSprite->field_22 = 0;
	Sprite_Handle_Helicopter( pSprite );
	pSprite->field_22 = 0;
	word_3B175 = 0;

	sSprite* Data24 = pSprite + 1;
	Data24->field_18 = eSprite_Null;
	Data24 = pSprite + 2;
	Data24->field_18 = eSprite_Null;
}

int16 cFodder::sub_23D57( sSprite* pSprite ) {
	int16 Data4 = 0x0F;
	int16 Data0 = -10;

	Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
	switch (Data4) {
		case 1:
		case 4:
		case 5:
		case 6:
		case 0x0B:
			Data0 = 0x0C;
			break;

		case 3:
			Data0 = 0x14;
			break;
		case 2:
		case 9:
		case 0x0A:
			Data0 = 0x0E;
			break;

		default:
			return 0;	// Correct?
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
	sub_2A1F0( pSprite, pData0, pData4 );

loc_23E2F:;
	sub_2B12E( pSprite );

	pSprite->field_10 = pSprite->field_3C;
	pData0 = word_3A8CF;
	pData0 <<= 3;

	if (pSprite->field_36 > 0x18)
		pData0 <<= 1;
	pSprite->field_10 = pData0;
	pSprite->field_10 &= 0x1FE;

	sub_20E5C( pSprite );

	int16 Data8 = pSprite->field_3C;
	if (!pSprite->field_36)
		return;

	pSprite->field_A = Data8 >> 1;
}

void cFodder::sub_23EA6( sSprite* pSprite ) {
	
	for (sSprite** Data24 = dword_3B213; *Data24 != INVALID_SPRITE_PTR; ++Data24) {

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

	if (mMap_TileSet == eTileTypes_Moors || mMap_TileSet == eTileTypes_Int)
		if (pSprite->field_38 == 5)
			pSprite->field_38 = 0;

	if (pSprite->field_38 == 7 || pSprite->field_38 == 5) {

		pSprite->field_18 = eSprite_Explosion;
		pSprite->field_26 = 0x1F45;
		pSprite->field_28 = -2;
		if (!pSprite->field_22)
			return;

		Sprite_Destroy( pSprite + 1 );
		return;
	}

	//loc_23F70
	pSprite->field_8 = 0xD2;
	pSprite->field_65 = -1;
	if (!pSprite->field_22)
		goto loc_24075;

	word_3B4DF = 0;
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

	if (word_3AA4B >= 0x28) {

		Data0 = tool_RandomGet();
		if (mSprite_Enemy_Aggression_HalfOfMinPlusMax < 5)
			Data0 &= 0x3F;
		else
			Data0 &= 0x1F;

		if (Data0 == 4)
			pSprite->field_54 = -1;
	}

	word_3B4DF = -1;
	Data34 = Data28;

	Data0 = pSprite->field_0 + 8;
	Data4 = pSprite->field_4 - 5;
	Data8 = Data28->field_0;
	pSprite->field_2E = Data8;
	DataC = Data28->field_4;
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
	Direction_Calculate( Data0, Data4, Data8, DataC );
	Data4 -= 0x10;
	Data4 >>= 5;
	Data4 += 8;
	Data4 ^= 0x0F;
	Data4 &= 0x0F;

	pSprite->field_A = Data4;
loc_240F3:;

	Sprite_Handle_Turret_Fire( pSprite, Data34 );

	if (!pSprite->field_22)
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

	word_3B189 = 0;
	word_3B18B = 0;

	if (pSprite->field_6F != 9)
		goto loc_2421D;

	if (!word_3B4DF)
		goto loc_2421D;

	ax = Sprite_Create_MissileHoming( pSprite, Data2C, pData34 );
	goto loc_24234;

loc_2421D:;

	if (pSprite->field_22)
		if (word_3B4ED[1] > 9)
			goto loc_24275;

	ax = Sprite_Create_Missile( pSprite, Data2C );

loc_24234:;
	if (!ax)
		goto loc_2426C;

	Data2C->field_20 += 0x11;
	Data2C->field_36 = 0x3C;
	Sound_Play( pSprite, 0x2C, 0x1E );
	sub_23444( pSprite, Data2C );
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

	word_3AA4B = pData0;

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

	pData0 = word_3AA4B;
	goto loc_243C7;

loc_2439F:;
	if (word_3AA4B < 0x40)
		goto loc_243C7;

loc_243A6:;
	pSprite->field_5E++; //+= 0x76;	This should be a sprite counter
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

int16 cFodder::sub_2449E( sSprite* pSprite ) {
	
	Sound_Play( pSprite, 0x2B, 0x0F );
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

	if (dword_3B213[0x0E]) {
		Data0 = tool_RandomGet() & 0x1F;
		Data0 -= 5;
		Data2C->field_0 += Data0;
		Data0 >>= 4;
		Data0 &= 0x0F;
		Data0 -= 0x0A;
		Data2C->field_4 -= Data0;
		dword_3B213[0x0E] = 0;
	}
	//loc_24546
	Data2C->field_20 = pSprite->field_20;
	Data2C->field_52 = 0;
	Data2C->field_2C = 1;

	Data0 = tool_RandomGet() & 1;
	if (!Data0) {
		Data2C->field_8 = 0xDE;
		Data2C->field_A = 4;
		Data2C->field_18 = 0x56;
	}
	else {
		Data2C->field_8 = 0xDF;
		Data2C->field_A = 0;
		Data2C->field_18 = 0x57;
	}
	return 0;
}

void cFodder::sub_245BF( sSprite* pSprite ) {
	
	if (!pSprite->field_22)
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

	int16 Data4 = word_3E856[Data0];
	pSprite->field_0 += Data4;
	Data4 = word_3E856[Data0 + 1];
	pSprite->field_4 += Data4;

	if (!sub_246CC( pSprite )) {
		pSprite->field_5B = ~pSprite->field_5B;
		Sound_Play( pSprite, 0x10, 0x1E );
	}

	//loc_246BC
	pSprite->field_4 = Field_4;
	pSprite->field_0 = Field_0;
}

int16 cFodder::sub_246CC( sSprite* pSprite ) {
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
	Data2C->field_18 = 0x4D;

	int32 Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	Field_1E += 0x60000;
	Data2C->field_1E = Field_1E & 0xFFFF;
	Data2C->field_20 = Field_1E >> 16;

	Data2C->field_52 = pSprite->field_52;
	Data2C->field_22 = pSprite->field_22;
	Data2C->field_2C = 0;
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
	Data2C->field_1A = (int32*) pSprite;
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

	if (pSprite->field_6F == 6)
		goto loc_24905;

	if (pSprite->field_6F == 7)
		goto loc_24917;

	if (pSprite->field_6F == 8)
		goto loc_24917;

loc_24905:;
	if (!Sprite_Create_Grenade2( pSprite ))
		goto loc_2490D;
	goto loc_24942;

loc_2490D:;
	pSprite->field_57 = -1;
	return;

loc_24917:;
	word_3B189 = 0x0E;
	word_3B18B = -12;

	if (pSprite->field_6F == 8)
		goto loc_24936;

loc_2492E:;
	if (!Sprite_Create_Missile( pSprite, Data2C ))
		goto loc_2490D;

	goto loc_24942;

loc_24936:;
	if (!Sprite_Homing_LockInRange( pSprite, Data34 ))
		goto loc_2492E;

	if (!Sprite_Create_MissileHoming( pSprite, Data2C, Data34 ))
		goto loc_2490D;

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

	if (pSprite->field_22)
		sub_23C70( Data2C );

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

	Data2C->field_1A = (int32*) Dataa0;
	Data2C->field_1A = 0;
	Data2C->field_18 = eSprite_Grenade;
	Data30->field_18 = eSprite_ShadowSmall;

	Data2C->field_52 = 0;
	Data30->field_52 = 0;
	Data2C->field_50 = 0;

	Data2C->field_22 = pSprite->field_22;
	Data30->field_22 = pSprite->field_22;

	Data2C->field_2C = 0;
	Data30->field_2C = -1;
	Data2C->field_38 = 0;
	
	if (pSprite->field_22)
		Data2C->field_12 += 0x0A;

	return -1;
}

int16 cFodder::Sprite_Create_MissileHoming( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData34 ) {
	
	if (mMission_Completed_Timer)
		return 0;

	int16 Data0 = pSprite->field_22;
	int8* Data24 = byte_3A9D6;

	if (Data24[Data0] == 2)
		return 0;

	if (!pSprite->field_2E)
		return 0;

	Data0 = 2;
	sSprite *Data30 = 0;
	if (Sprite_Get_Free( Data0, pData2C, Data30 ))
		return 0;

	Data0 = pSprite->field_22;
	Data24 = byte_3A9D6;
	++Data24[Data0];

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

	Data0 = word_3B189;
	pData2C->field_0 += Data0;
	Data30->field_0 += Data0;
	if (Data30->field_0 < 0)
		return 0;

	pData2C->field_4 += word_3B18B;
	Data30->field_4 += word_3B18B;

	if (Data30->field_0 < 0)
		return 0;

	//seg005:5672
	pData2C->field_1A = (int32*) pData34;
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
	pData2C->field_6A = (sSprite*) 0x10000;
	if (pSprite->field_22)
		pData2C->field_6A = (sSprite*) 0x400;

	Data30->field_1E = 0;
	Data30->field_20 = 0;
	pData2C->field_4 += 1;
	pData2C->field_0 += 3;
	pData2C->field_18 = eSprite_MissileHoming2;
	if (pSprite->field_22)
		pData2C->field_18 = eSprite_MissileHoming;
	Data30->field_18 = eSprite_ShadowSmall;
	pData2C->field_52 = 0;
	Data30->field_52 = 0;
	pData2C->field_22 = pSprite->field_22;
	Data30->field_22 = pSprite->field_22;
	pData2C->field_2C = 0;
	Data30->field_2C = -1;
	pData2C->field_38 = 0;

	if (mVersion->mVersion == eVersion::AmigaPlus)
		Sound_Play( pSprite, 0x10, 0x0F );
	else
		Sound_Play( pSprite, 0x2C, 0x0F );
	return -1;
}

int16 cFodder::sub_2531F( sSprite* pSprite, sSprite*& pData30 ) {
	int16 Data0;
	sSprite* Data2C = 0, *Data34 = 0;

	if (pSprite->field_6F == 6)
		goto loc_25344;

	if (pSprite->field_6F == 7)
		goto loc_253E7;

	if (pSprite->field_6F == 8)
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

	Data0 = word_390B0;
	Data0 &= 0x0F;
	if (Data0)
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

	Data0 = word_3B177;
	if (Data0 <= 0x3C)
		goto loc_254C7;
	if (Data0 >= 0x8C)
		goto loc_254E7;

	if (pSprite->field_20 < 0x19)
		goto loc_254A4;

	Data0 = word_390B0;
	Data0 &= 0x0F;
	if (Data0)
		goto loc_254A4;

	word_3B189 = 0x0E;
	word_3B18B = -12;
	if (pSprite->field_6F == 8)
		goto loc_2547F;

	Sprite_Create_Missile( pSprite, Data2C );
	goto loc_254A4;

loc_2547F:;
	Data34 = pData30;
	if (!Sprite_Create_MissileHoming( pSprite, Data2C, Data34 ))
		goto loc_254A4;

	Data2C->field_6A = (sSprite*) 0x2000;
	Data2C->field_36 = 0x14;

loc_254A4:;
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
		pSprite->field_22 = 1;
		sub_1B4BB( pSprite );
		pSprite->field_22 = 1;
		return;
	}

	if (pSprite->field_6F == 0x0A)
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
	pSprite->field_22 = 1;
	sub_1B4BB( pSprite );

	if (pSprite->field_6F == 3)
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

	if (pSprite->field_38 == 1)
		pSprite->field_38 = 0;

	if (pSprite->field_38) {
		word_3B175 = -1;

		pSprite->field_22 = 1;
		Sprite_Handle_Helicopter( pSprite );
		pSprite->field_22 = 1;

		word_3B175 = 0;
		goto loc_25288;
	}

	if (!pSprite->field_43) {
		pSprite->field_46 = (int32*) ((pSprite->field_0 << 16) | (pSprite->field_4 & 0xFFFF));
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
		if (pSprite->field_18 == 0x6B)
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
		Data8 += mSprite_Enemy_Aggression_HalfOfMinPlusMax;

		if (Data8 >= 0)
			pSprite->field_62 = Data8;
	}

	goto loc_25239;

loc_2500F:;
	if (pSprite->field_4C)
		goto loc_25239;

	Data0 = tool_RandomGet() & 0x7F;
	Data0 += 4;
	if (Data0 > readLEWord(&mMap[0x54]))
		goto loc_25239;

	Data8 = Data0;
	Data0 = tool_RandomGet() & 0x3F;
	Data0 += 4;

	if (Data0 > readLEWord(&mMap[0x56]))
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

	if (pSprite->field_6F == 5)
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

	word_3B177 = Data0;
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
	word_3B175 = -1;
	pSprite->field_22 = 1;
	Sprite_Handle_Helicopter(pSprite);

	pSprite->field_22 = 1;
	word_3B175 = 0;
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

void cFodder::sub_25863( sSprite* pSprite ) {
	if (!pSprite->field_5C)
		return;

	int16 Data0 = tool_RandomGet() & 0x3F;
	if (Data0 != 9)
		return;

	if (mSquad_Leader == INVALID_SPRITE_PTR)
		return;

	pSprite->field_2E = mSquad_Leader->field_0;
	pSprite->field_30 = mSquad_Leader->field_4;

	sub_25B6B( pSprite );
}

void cFodder::sub_258C6( sSprite* pSprite ) {
	
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
		if (sub_25AAE( pSprite ) < 0)
			return;
	}

	int16 Data0 = tool_RandomGet() & 0x3F;
	Data0 += 0x14;
	pSprite->field_4C = Data0;
}

int16 cFodder::sub_25680( sSprite* pSprite ) {

	if (!dword_3B477)
		return -1;

	if (pSprite->field_4C) {
		--pSprite->field_4C;
		return 0;
	}

	sSprite* Data28 = dword_3B477;

	int16 Data0 = pSprite->field_0;
	int16 Data4 = pSprite->field_4;
	int16 Data8 = Data28->field_0;
	int16 DataC = Data28->field_4;

	pSprite->field_26 = Data8;
	pSprite->field_28 = DataC;

	word_3B47D = Data0;
	word_3B47F = Data4;
	word_3B481 = Data8;
	word_3B483 = DataC;
	int16 Data10 = 0x20;

	Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
	if (Data0 <= 3) {
		Sprite_Destroy_Wrapper( pSprite );
		word_3B47B = -1;
		return 1;
	}

	if (Data0 < 6) {
		Data28->field_2A = 0x0A;
		return 0;
	}

	Data0 = word_3B47D;
	Data4 = word_3B47F;
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
			mMission_Aborted = -1;

		if (mPhase_Goals[ eGoal_Get_Civilian_Home - 1 ])
			mMission_Aborted = -1;

		pSprite->field_8 = 0xD6;
		pSprite->field_A = 0;
		
		int16 Data0 = tool_RandomGet() & 7;
		int16 Data4 = mSound_Indigenous_Death[Data0];

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

		if (sub_2A1F0( pSprite, Data0, Data4 ) < 0)
			word_3ABAD = -1;
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
	if (!sub_20E91( pSprite )) {
		sub_20F19( pSprite );

		if (!word_3B25B)
			Sprite_Movement_Calculate( pSprite );
	}
	//loc_259D4
	sub_20478( pSprite );
	byte_3ABA9 = pSprite->field_60;

	int16 Data4 = 0;
	sub_1FFC6( pSprite, Data4 );

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
	
	if (word_3ABAD || word_3B25B)
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

int16 cFodder::sub_25AAE( sSprite* pSprite ) {
	int16 Data0 = tool_RandomGet() & 0x7F;
	Data0 += 4;

	// Map Width
	if (Data0 >= readLEWord( &mMap[0x54] ))
		return -1;

	int16 Data8 = Data0;
	Data0 = tool_RandomGet() & 0x3F;
	Data0 += 4;

	// Map Height
	if (Data0 >= readLEWord( &mMap[0x56] ))
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

int16 cFodder::sub_25B6B( sSprite* pSprite ) {
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
	Data2C->field_18 = 0x47;
	Data2C->field_1E = pSprite->field_1E;
	Data2C->field_20 = pSprite->field_20;

	int32 tmp = Data2C->field_1E | Data2C->field_20 << 16;
	tmp += 0x60000;

	Data2C->field_1E = tmp & 0xFFFF;
	Data2C->field_20 = tmp >> 16;

	Data2C->field_52 = pSprite->field_52;
	Data2C->field_22 = pSprite->field_22;
	Data2C->field_2C = 0;
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
	Data2C->field_1A = (int32*) pSprite;
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

	if (pSprite->field_38 == 9)
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
	if (sub_1FF1A( pSprite, Data2C, Data30 ))
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
	Data0 = word_3E926[pSprite->field_2A / 2];

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

	int16 Data0 = word_390B0 & 0x0F;
	if (!Data0) {
		Data0 = tool_RandomGet() & 0x06;
		pSprite->field_32 = mSprite_Hostage_Frames[Data0 / 2];
	}

	pSprite->field_A = pSprite->field_32;
}

void cFodder::sub_25FDA( sSprite* pSprite ) {
	int16 Data0, Data4, Data8, DataC, Data10;
	sSprite* Data24 = 0, *Data28 = 0, *Data2C = 0;

	if (pSprite->field_18 == eSprite_Hostage_2)
		goto loc_2625B;

	if (!dword_3B5F5)
		goto loc_2608B;

	if (pSprite->field_74) {
		pSprite->field_74 -= 1;
		goto loc_2608B;
	}
	//loc_2600B

	pSprite->field_74 = -56;
	Data24 = dword_3B5F5;

	Data8 = Data24->field_0;
	Data8 += 0x0A;
	DataC = Data24->field_4;
	DataC -= 5;
	Data0 = pSprite->field_0;
	Data4 = pSprite->field_4;
	Data10 = 0x80;

	Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
	if (Data10 < 0x7F)
		pSprite->field_5E = (dword_3B5F5 - mSprites);

loc_2608B:;
	word_3B2ED = 0;
	Data0 = eSprite_Hostage_Rescue_Tent;
	Data4 = eSprite_Enemy;
	Data8 = eSprite_Player;
	DataC = -1;
	if (Sprite_Find_By_Types( pSprite, Data0, Data4, Data8, DataC, Data10, Data28 ))
		return; 

	if (Data4 != eSprite_Hostage_Rescue_Tent)
		goto loc_2614F;

	pSprite->field_26 = Data28->field_0;
	pSprite->field_26 += 0x0A;
	pSprite->field_28 = Data28->field_4;
	pSprite->field_28 -= 5;

	Data0 = pSprite->field_0;
	Data4 = pSprite->field_4;
	Data8 = pSprite->field_26;
	DataC = pSprite->field_28;
	Data10 = 0x10;
	Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
	if (Data10 >= 0x10)
		goto loc_2620F;

	if (Data0 >= 3)
		return;

	mHostage_Count -= 1;
	Sprite_Destroy_Wrapper( pSprite );
	return;

loc_2614F:;
	if (Data4 == 5)
		goto loc_26221;

	pSprite->field_26 = Data28->field_0;
	pSprite->field_26 += 4;
	pSprite->field_28 = Data28->field_4;
	pSprite->field_28 -= 6;

	if (!Data28->field_6E)
		goto loc_2620F;

	Data2C = Data28->field_6A;
	if (Data2C->field_36 > 2)
		goto loc_2620F;
	if (Data2C->field_20 > 3)
		goto loc_2620F;

	Data0 = Data2C->field_0;
	Data4 = Data2C->field_4;
	Data8 = pSprite->field_0;
	DataC = pSprite->field_4;
	Data10 = 0x10;

	Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
	if (Data0 > 0x0A)
		goto loc_2620F;
	pSprite->field_6E = -1;
	return;

loc_2620F:;
	Data0 = tool_RandomGet() & 0x3F;
	if (Data0)
		return;

	sub_26490(pSprite);
	return;

loc_26221:;
	Data28->field_70 = pSprite;
	pSprite->field_26 = pSprite->field_0;
	pSprite->field_28 = pSprite->field_4;
	word_3B2ED = -1;
	return;

loc_2625B:;
	word_3B2ED = 0;
	Data0 = eSprite_Player;
	Data4 = eSprite_Hostage_Rescue_Tent;
	Data8 = -1;
	if (Sprite_Find_By_Types( pSprite, Data0, Data4, Data8, DataC, Data10, Data28 ))
		return;

	if (Data4 != eSprite_Hostage_Rescue_Tent)
		goto loc_2631F;

	pSprite->field_26 = Data28->field_0 + 0x0A;
	pSprite->field_28 = Data28->field_4 - 0x05;

	Data0 = pSprite->field_0;
	Data4 = pSprite->field_4;
	Data8 = pSprite->field_26;
	DataC = pSprite->field_28;
	Data10 = 0x10;
	Map_Get_Distance_BetweenPoints( Data0, Data4, Data8, Data10, DataC );
	if (Data0 >= 3)
		return;
	mHostage_Count -= 1;

	Sprite_Destroy( pSprite + 1 );
	Sprite_Destroy_Wrapper( pSprite );
	return;

loc_2631F:;
	word_3B2ED = -1;
	pSprite->field_8 = 0xD8;
	pSprite->field_26 = Data28->field_0 + 4;
	pSprite->field_28 = Data28->field_4 - 6;
	if (!Data28->field_6E)
		goto loc_263E5;

	Data2C = Data28->field_6A;
	if (Data2C->field_36 > 2)
		goto loc_263E5;
	if (Data2C->field_20 > 3)
		goto loc_263E5;

	Data0 = Data2C->field_0;
	Data4 = Data2C->field_4;
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

	mMission_Aborted = -1;
	return;

loc_264CF:;

	if (mTroops_Enemy_Count >= 0x0A) {
		pSprite->field_8 = 0x9B;
		return;
	}

	pSprite->field_2C = -1;
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
	Sound_Play( pSprite, 0x10, 1 );

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
	pSprite->field_43 = Data0;
	pSprite->field_8 = 0x7C;

loc_265B1:;
	sSprite* Data2C = 0, *Data30 = 0;

	if (!sub_265D6( pSprite, Data2C, Data30 )) {
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
	
	if (pSprite->field_38 == 7) {
		if (pSprite->field_74 >= pData1C) {
			pSprite->field_18 = eSprite_Explosion2;
			return;
		}
		pSprite->field_74 += 1;
		pSprite->field_38 = 0;
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
	pSprite->field_8 = word_3E97A[Data0 / 2];
	pSprite->field_20 = word_3E982[Data0 / 2];
}

void cFodder::Map_Get_Distance_BetweenPoints_Within_320( int16& pX, int16& pY, int16& pX2, int16& pY2 ) {
	const int8* Data24 = byte_3ECC0;
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
	return;

loc_29EBB:;
	pX = 0x3E8;
}

int16 cFodder::sub_265D6( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30 ) {
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
	pData2C->field_22 = 2;

	Data0 = tool_RandomGet();
	Data4 = Data0;
	Data0 &= 0x1E;
	if (Data4 < 0)
		Data0 = -Data0;

	Data0 += 0x1C0;
	pData2C->field_10 = Data0;
	Data4 &= 0x0F;
	Data4 += 8;
	pData2C->field_44 = Data4;
	return 0;
}

int16 cFodder::Direction_Calculate( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
	const int16* Data24 = word_3F0C1;
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
	pData4 = Data24[pData4];
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
	word_44A30 = -ax;
	word_44A2E = ax;
	word_44A32 = 1;
	word_44A34 = 0;
}

int16 cFodder::tool_RandomGet() {
	int16 Data0 = word_44A2E;
	int16 Data2 = word_44A30;
	int16 Data4 = word_44A32;
	int16 Data6 = word_44A34;
	
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

	word_44A2E = Data0;
	word_44A30 = Data2;
	word_44A32 = Data4;
	word_44A34 = Data6;

	Data2 = 0;
	return Data0;
}

void cFodder::GUI_Sidebar_MapButton_Render() {

	if (word_3B4D9)
		return;

	sGUI_Element* Element = mGUI_NextFreeElement;
	Element->field_0 = &cFodder::GUI_Button_NoAction;
	Element->mX = 0;
	Element->mWidth = 0x2F;
	Element->mHeight = 0x0B;
	Element->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_ShowOverview;

	if (mVersion->mPlatform == ePlatform::PC) {
		Element->mY = 0xBD;
		mGraphics->sub_145AF( 0xD0, 0, 0xBD );
	}
	else {
		Element->mY = 214;
		mGraphics->sub_145AF( 0xD0, 0, 214 );
	}
	++Element;

	GUI_ClearElement( Element );
	word_3B4D9 = -1;

}

void cFodder::Sprite_Movement_Calculate( sSprite* pSprite ) {
	word_3B173 = 0;
	if (!pSprite->field_36)
		return;

	//loc_2A10D
	const int16* Data24 = word_3EABF;
	pSprite->field_10 &= 0x1FE;
	int16 Data4 = pSprite->field_10;

	int16 Data8 = *(Data24 + (Data4/2));

	Data4 += 0x80;
	Data4 &= 0x1FE;
	int16 DataC = *(Data24 + (Data4/2));
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
		word_3B173 = -1;
	}

	tmp = (pSprite->field_6 & 0xFFFF) | pSprite->field_4 << 16;
	tmp += DataaC;
	pSprite->field_6 = tmp & 0xFFFF;
	pSprite->field_4 = tmp >> 16;

	if (pSprite->field_4 < 0) {
		pSprite->field_6 = 0;
		pSprite->field_4 = 0;
		word_3B173 = -1;
	}
}

int16 cFodder::sub_2A1F0( sSprite* pSprite, int16& pData0, int16& pData4 ) {
	const int16 *Data24 = word_3F0C1;

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

	word_3A8D5 = pData0;
	word_3A8D7 = pData4;
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
	pData4 = Data24[pData4];
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
	
	word_3A8CF = 0;
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
	DataC = (int16) byte_3E98F[DataC];
	DataC <<= 1;

	word_3A8CF = DataC;
}

void cFodder::sub_2A470() {
	int16* Data34 = word_3A9C0;

	for (int16 Data0 = 2; Data0 >= 0; --Data0) {
		int16 ax = *Data34++;

		if (ax) 
			*(Data34 - 1) -= 1;
		
	}
}

int16 cFodder::sub_2A4A2( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
	pData0 >>= 4;
	pData4 >>= 4;
	pData8 >>= 4;
	pDataC >>= 4;

	int16 Data18 = 2;

	int16 Data1C = readLEWord( &mMap[0x54] );
	Data1C <<= 1;

	word_3A9A6[0] = pData0;
	word_3A9A6[1] = pData4;

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
	int16 Data2C = pDataC;

	int16 Data24, Data38;

	if (Data10 < 0) {
		pData18 = -pData18;
		Data24 = pData18;
		Data10 = -Data10;
		pData18 = -1;
	}
	else {
		Data24 = pData18;
		pData18 = 1;
	}
	//loc_2A56A

	if (Data14 < 0) {
		pData1C = -pData1C;
		Data38 = pData1C;
		Data14 = -Data14;
		pData1C = -1;
	}
	else {
		Data38 = pData1C;
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
	if (Data28 == pData0 && Data2C == pData4){
		Data20[0] = -1;
		Data20[1] = -1;
		Data20[2] = -1;
		Data20[3] = -1;
		return;
	}

	if (pData8 >= 0) {
		pData4 += pData1C;
		pData8 -= Data10;
		writeLEWord( Data20, Data38 );
		Data20 += 2;
		goto loc_2A5BA;
	}
	//loc_2A601
	pData0 += pData18;
	pData8 += Data14;
	writeLEWord( Data20, Data24 );
	Data20 += 2;
	goto loc_2A5BA;
}

int16 cFodder::sub_2A622( int16& pData0 ) {
	
	pData0 = word_3A9A6[0];
	int32 Data4 = word_3A9A6[1];

	uint8* Data20 = &mMap[0x60];

	Data4 *= readLEWord( &mMap[0x54] );
	Data4 += pData0;
	Data4 <<= 1;

	Data20 += Data4;
	//seg007:0B48

	uint8* Data28 = byte_3A8DE;
	word_3B2CB = -1;

	for (;;) {
		if (word_3B2CB) {
			word_3B2CB = 0;
			goto loc_2A6D7;
		}
		//loc_2A6A1
		pData0 = readLEWord( Data20 );
		pData0 = graphicsBaseHit[pData0];
		if (pData0 >= 0) {
			pData0 &= 0x0F;

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

		Data20 += pData0;
		pData0 = readLEWord( Data28 );
		Data28 += 2;

	loc_2A728:;
		Data20 += pData0;

	}
}

int16 cFodder::Map_Get_Distance_BetweenPoints( int16& pPosX, int16& pPosY, int16& pPosX2, int16& pDistance, int16& pPosY2 ) {
	const int8* Data24 = byte_3ECC0;

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
 * @param pX		Returns Terrain Type
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
 * @param pX		Returns Terrain Type
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
	int16 Data0 = (pY >> 4);
	int16 Data4 = (pX >> 4);

	Data0 *= readLEWord( &mMap[0x54] );

	Data0 += Data4;
	Data0 <<= 1;

	Data0 = readLEWord( &mMap[0x60 + Data0]) & 0x1FF;

	Data4 = graphicsBaseHit[Data0];

	if (Data4 < 0) {
		//loc_2A8D9
		pData10 >>= 1;
		pData10 &= 0x07;

		int16 Data8 = 7;
		Data8 -= pData10;

		pData14 >>= 1;
		pData14 &= 0x07;

		Data0 <<= 3;
		uint8* Data28 = (uint8*) graphicsBaseBht;
		pData14 += Data0;

		uint8 al = 1 << Data8;
		Data28 += pData14;
		if ((*Data28 & 0xFF) & al)
			Data4 >>= 4;
	}

	Data4 &= 0x0F;
	Data0 = mTiles_NotWalkable[Data4];
	pY = Data0;
	pX = Data4;

	return Data0;
}

void cFodder::Squad_Walk_Target_Set( int16 pData4, int16 pData8, int16 pDataC, int16 pData10 ) {
	int16 Data1C;
	sSprite** Saved_Data24 = 0;

	if (pDataC < 0 || pDataC >= 3)
		return;

	int16 Data0 = pDataC;

	pData10 = word_3A9BA[Data0];

	sMapTarget* Data38 = mSprite_WalkTargets[Data0];

	sSprite** Data24 = mSquads[Data0];
	int16* Data34 = word_3A9C0;
	int16 Data14 = word_3A9C0[Data0];

	if (Data14) {
		Data38[pData10].field_0 = pData4;
		Data38[pData10].field_2 = pData8;
		Data38[pData10 + 1].asInt = -1;

		if (pData10 < 29)	// 0x74
			pData10++;

		word_3A9BA[Data0] = pData10;
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

	if (word_3B161) {
		Data38[pData10].field_0 = word_3B161;
		Data38[pData10].field_2 = word_3B163;
		pData10 += 1;
	}
	//loc_2AAAE
	Data38[pData10].field_0 = pData4;
	Data38[pData10].field_2 = pData8;

	word_3B161 = pData4;
	word_3B163 = pData8;

	Data38[pData10 + 1].asInt = -1;

	Data24 = Saved_Data24;
	if (Data1C < 0)
		return;

	word_3A9BA[Data0] = pData10;
	word_3A9BA[Data0]++;

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

		Data38[pData10].field_0 = Data28->field_0;
		Data38[pData10].field_2 = Data28->field_4;

		Data28->field_26 = pData4;
		Data28->field_28 = pData8;

		pData4 = Data28->field_0;
		pData8 = Data28->field_4;
	}

loc_2ABF8:;
	Data34[Data0] = 8;
}

int16 cFodder::Squad_Member_Sprite_Hit_In_Region( sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 ) {
	int16 Data4;

	if (mMission_Finished) {
		word_3AA45 = 0;
		return 0;
	}

	mSprites_Found_Count = 0;

	sSquad_Member* Data28 = mSquad;

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

		sub_2A1F0( Data2C, Data0, Data4 );
		
		Data2C->field_10 += 0x100;
		Data2C->field_10 &= 0x1FE;

	loc_2ADC3:;	// Hit
		if (Data2C->field_18)
			goto loc_2ADFF;

		Data2C->field_10 = pSprite->field_10;
		Data2C->field_12 = pSprite->field_10;

	loc_2ADFF:;
		Data2C->field_38 = 1;
		Data2C->field_64 = -1;
		mSprites_Found_Count += 1;

		if (word_3AA45)
			break;
	}

	word_3AA45 = 0;
	return mSprites_Found_Count;
}

uint8* cFodder::Sprite_Get_Gfx_Ptr( int16& pData0, int16& pData4 ) {
	const sSpriteSheet* Sheet = &mSpriteDataPtr[pData0][pData4];

	pData0 = Sheet->mColCount;
	pData4 = Sheet->mRowCount;

	return mGraphics->GetSpriteData( Sheet->field_2 ) + Sheet->field_0;
}

uint8 cFodder::sub_2AFF5( uint8* pSi, int16 pBx, int16 pCx ) {

	pSi += 0xA0 * pBx;
	pSi += (pCx >> 1);
	if (pCx & 1)
		return (*pSi) & 0x0F;

	return (*pSi) >> 4;
}

void cFodder::sub_2AEB6( int16 pData0, int16 pData4, int16* pData8, int16* pDataC ) {
	int32	Data = pData0 * *pData8;
	pData0 = (int16) Data / 0x64;
	
	int16 Final8 = pData0;
	
	Data = pData4 * *pData8;
	*pDataC = (int16) Data / 0x64;

	*pData8 = Final8;
}

void cFodder::sub_2B016( uint8* pDi, uint8 pAl ) {
	
	pDi += 0xA0 * (dword_44A3A >> 16); 
	pDi += (dword_44A36 >> 16) >> 1;

	if ((dword_44A36 >> 16) & 1) {
		*pDi &= 0xF0;
		*pDi |= pAl & 0x0F;
		return;
	}

	*pDi &= 0x0F;
	*pDi |= pAl << 4;
}

void cFodder::sub_2B12E( sSprite* pSprite ) {
	word_3A8CF = 0;
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
	int16 DataC = (int16) byte_3E98F[Data4];
	word_3A8CF = DataC;
}

void cFodder::Sprite_Vehicle_Direction_Update( sSprite* pSprite, int16& pData1C ) {
	int16 Data4 = word_3B2F5;
	Data4 >>= 5;
	Data4 -= 1;
	Data4 ^= 0x0F;

	int16 Data0 = pSprite->field_A;

	Data0 -= Data4;
	Data4 = Data0;
	Data4 &= 0x0F;
	int16 DataC = (int16) byte_3E98F[Data4];
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
	uint16 DataC = pY;
	uint16 Data8 = pX;

	DataC >>= 4;

	DataC *= readLEWord( &mMap[0x54] );
	Data8 >>= 4;

	DataC += Data8;
	DataC <<= 1;

	int16 Data0 = readLEWord( &mMap[0x60 + DataC] );
	Data0 &= 0xFF;

	int16 Data4 = graphicsBaseHit[Data0];

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
	pX = byte_3ECC0[pY2];

	pX <<= Data10;
	return;

loc_2B403:;
	pX = 0x280;
}

void cFodder::Camera_Pan() {
	
	Camera_Update_Row();
	Camera_Update_Column();

	int16 bx = mCamera_Column_Previous;
	int16 cx = mCamera_Column;
	mCamera_Column_Previous = cx;

	cx -= bx;

	if (cx) {
		if (cx < 0) {
			mCamera_Pan_RowCount = -cx;
			Camera_Pan_Left( );
		}
		else {
			mCamera_Pan_RowCount = cx;
			Camera_Pan_Right( );
		}
	}

	bx = mCamera_Row_Previous;
	cx = mCamera_Row;
	mCamera_Row_Previous = cx;
	cx -= bx;

	if (!cx)
		return;

	if (cx < 0) {
		mCamera_Pan_RowCount = -cx;
		Camera_Pan_Up( );
	}
	else {
		mCamera_Pan_RowCount = cx;
		Camera_Pan_Down( );
	}

}

void cFodder::Camera_Pan_Right() {

	for (int16 cx = mCamera_Pan_RowCount; cx > 0; --cx) {

		++word_3B60E;
		word_3B60E &= 0x0F;
		if (!word_3B60E) {
			mMapTilePtr += 2;
			++word_3B612;
		}

		//++word_40054;
		word_40054 &= 3;
		if (!word_40054) {
			++word_40056;
			++word_40058;
			++word_4005A;
			++word_4005C;
		}
	}

	g_Graphics.map_Tiles_Draw();
}

void cFodder::Camera_Pan_Left() {

	for (int16 cx = mCamera_Pan_RowCount; cx > 0; --cx) {
		
		--word_3B60E;
		word_3B60E &= 0x0F;
		if (word_3B60E == 0x0F) {
			mMapTilePtr -= 2;
			--word_3B612;
		}
		
		//--word_40054;
		word_40054 &= 3;
		if (word_40054 == 3) {
			--word_40056;
			--word_40058;
			--word_4005A;
			--word_4005C;
		}

	}

	g_Graphics.map_Tiles_Draw();
}

void cFodder::Camera_Pan_Down( ) {

	int16 ax = word_3B60E;
	ax &= 3;

	for (int16 cx = mCamera_Pan_RowCount; cx > 0; --cx) {

		++word_3B610;
		word_3B610 &= 0x0F;
		if (!word_3B610) {
			mMapTilePtr += mMapWidth << 1;
			++word_3B614;
		}

		word_40056 += 0x58;
		word_40058 += 0x58;
		word_4005A += 0x58;
		word_4005C += 0x58;
	}
	
	g_Graphics.map_Tiles_Draw();
}

void cFodder::Camera_Pan_Up() {

	for (int16 cx = mCamera_Pan_RowCount; cx > 0; --cx) {

		--word_3B610;
		word_3B610 &= 0x0F;
		if (word_3B610 == 0x0F) {
			mMapTilePtr -= mMapWidth << 1;
			--word_3B614;
		}

		word_40056 -= 0x58;
		word_40058 -= 0x58;
		word_4005A -= 0x58;
		word_4005C -= 0x58;


	}

	g_Graphics.map_Tiles_Draw();
}

void cFodder::Camera_Update_Row() {
	int16 Data0 = word_39FB4;

	if (Data0 < 0)
		Data0 = -1;
	else
		Data0 = 1;

	if (Data0 != word_39FA2)
		goto loc_2CFBE;

loc_2CF93:;
	word_39FBC = 0;
	mCamera_Row = dword_39F88 >> 16;
	word_39FAE = dword_39F90 >> 16;
	word_39FA6 = 0xF0;

	if (word_39FB4 < 0)
		word_39FA6 = -16;
	return;

loc_2CFBE:;
	word_39FA2 = Data0;
	Data0 = dword_39F90 >> 16;
	int16 Data4 = word_39FAE;

	Data0 >>= 4;
	Data4 >>= 4;
	if (Data4 == Data0)
		goto loc_2CF93;
}

void cFodder::Camera_Update_Column() {
	int16 Data0 = word_39FB2;

	if (Data0 < 0)
		Data0 = -1;
	else
		Data0 = 1;

	if (Data0 != word_39FA0)
		goto loc_2D03B;

loc_2D010:;
	word_39FBA = 0;
	mCamera_Column = dword_39F84 >> 16;
	word_39FAC = dword_39F8C >> 16;
	word_39FA4 = 0x130;

	if (word_39FB2 < 0)
		word_39FA4 = -1;
	return;

loc_2D03B:;
	word_39FA0 = Data0;
	Data0 = dword_39F8C >> 16;
	int16 Data4 = word_39FAC;
	Data0 >>= 4;
	Data4 >>= 4;
	if (Data0 == Data4)
		goto loc_2D010;
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

	// Loop through all squad members
	for (int8 Data1C = 0; Data1C < 8; ++Data1C) {
		sSquad_Member* Data2C = &mSquad[Data1C];

		if (Data2C->mSprite == INVALID_SPRITE_PTR || Data2C->mSprite == 0 )
			continue;

		sSprite* Sprite = Data2C->mSprite;

		if (!(Sprite->field_75 & 2)) {
			if (Sprite->field_38 < 32) {
				if (Sprite->field_38) {
					if (!Sprite->field_6E)
						continue;

					Sprite->field_38 = 0;
				}
			}
		}
		//loc_2D165

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

	dword_3A8DB = readLEDWord( &mSquads_TroopCount );
	byte_3A8DE[1] = byte_3A05E;

	for (int16 Data0 = 2; Data0 >= 0; --Data0 ) {

		int8 al = mSquads_TroopCount[Data0];
		if (al)
			continue;

		byte_3BF1B[Data0] = -1;
	}

	*Data34 = INVALID_SPRITE_PTR;
}

int16 cFodder::Squad_Member_GetDeviatePotential( sSquad_Member* pData24 ) {

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

	sSquad_Member* Dataa20 = (sSquad_Member*) Data20->field_46;
	if (Dataa20 == 0)
		return;

	mSquad_Member_Fire_CoolDown = mSprite_Weapon_Data.mCooldown;
	mSprite_Bullet_Time_Modifier = 0;
	mSprite_Bullet_Fire_Speed_Modifier = 0;

	if (Dataa20->mRank > 0x14) {

		if (mSquad_Selected >= 0) {

			if (mSquads_TroopCount[mSquad_Selected] == 1) {
				mSquad_Member_Fire_CoolDown = 1;
				mSprite_Bullet_Time_Modifier = -3;
				mSprite_Bullet_Fire_Speed_Modifier = 0x0A;
			}
		}
	}
	//loc_2D381
	int16 Data0 = mSquad_Selected;
	if (Data0 < 0)
		return;

	int16 Data8 = Data0;
	Data0 = mSquads_TroopCount[Data0];
	Data0 &= 0xFF;
	--Data0;
	int16 DataC = Data0;

	if (DataC < 0)
		return;

	const int16* Dataaa20 = off_3D4B5[Data0];
	Data0 = word_3ABB5;

	int16 Data4 = Dataaa20[Data0];
	if (Data4 < 0 || Data4 > DataC) {
		Data4 = 0;
		Data0 = 0;
	}

	Data0++;
	if (Dataaa20[Data0] < 0)
		Data0 = 0;

	word_3ABB5 = Data0;
	sSprite** Dat20 = mSquads[Data8];
	Data20 = Dat20[Data4];

	if (Data20->field_45)
		return;

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
			goto loc_2D620;

		if (pSprite->field_18 == eSprite_Building_Explosion)
			goto loc_2D62C;

		if (pSprite->field_18 != eSprite_Explosion)
			goto loc_2D642;

		goto loc_2D62C;

	loc_2D620:;
		pData24->field_38 = 7;
		goto loc_2D636;

	loc_2D62C:;
		pData24->field_38 = 5;

	loc_2D636:;
		pData24->field_64 = -1;
		goto loc_2D6ED;

	loc_2D642:;
		if (mSprite_Can_Be_RunOver[Data4] == 2)
			goto loc_2D659;

		mSprites_Found_Count = 1;
		goto loc_2D705;
	loc_2D659:;

		pData24->field_38 = 1;
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

		if (Data20->field_38 >= 0x32)
			continue;

		Sprite_Troop_Dies( Data20 );
	}
}

void cFodder::Squad_Prepare_GrenadesAndRockets() {

	mSquad_Grenades[0] = mTroopsAvailable << 1;
	mSquad_Rockets[0] = mTroopsAvailable;

	if (mMapNumber < 5)
		mSquad_Grenades[0] = 0;

	if (mMapNumber < 0x0A)
		mSquad_Rockets[0] = 0;
}

void cFodder::Sprite_Aggression_Set() {
	int16 Data0 = mMapNumber;
	Data0 <<= 1;

	int16 Data4 = mVersion->mMissionData->mEnemyAggression[Data0];
	int16 Data8 = mVersion->mMissionData->mEnemyAggression[Data0 + 1];

	mSprite_Enemy_AggressionMin = Data4;
	mSprite_Enemy_AggressionMax = Data8;

	Data8 += Data4;

	Data8 >>= 1;

	mSprite_Enemy_Aggression_HalfOfMinPlusMax = Data8;
	mSprite_Enemy_AggressionNext = Data8;
	mSprite_Enemy_AggressionIncrement = 1;

	sSprite* Sprite = mSprites;
	for (int16 Data1C = 29; Data1C >= 0; --Data1C, ++Sprite ) {

		if (Sprite->field_0 == -32768)
			continue;

		if (Sprite->field_18 == eSprite_Enemy_Rocket || Sprite->field_18 == eSprite_Enemy)
			Sprite_Handle_Enemy_Aggression_Set( Sprite );
	}
}

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

	sMapTarget* Data24 = mSprite_WalkTargets[ pSprite->field_32 ];

	int16 Data0 = Data24[pSprite->field_40].field_0;
	int16 Data4 = Data24[pSprite->field_40].field_2;

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

	int16 Data18 = byte_3BF1B[Data14];
	if (Data18 < 0)
		return -1;

	Data18 &= 0xFF;

	if (mSquads_TroopCount[Data14] > 8)
		return -1;

	sSquad_Member* Dataa24 = (sSquad_Member*) pSprite->field_46;
	Dataa24->mSelected &= 0xFE;

	sMapTarget* Data24 = mSprite_WalkTargets[Data18];
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

	word_3BEDF[Data14] = 0;
	mSquad_Selected = Data18;
		
	mSquad_Grenades[Data18] += mSquad_Grenades[Data14];
	mSquad_Rockets[Data18] += mSquad_Rockets[Data14];

	//seg010:0232
	mSquad_Grenades[Data14] = 0;
	mSquad_Rockets[Data14] = 0;

	sub_305D5( pSprite );
	mGUI_Sidebar_Setup = 0;
	word_3A8D9 = 0;

	return -1;
}

int16 cFodder::Squad_Join_Check( sSprite* pSprite ) {

	if (byte_3BF1B[pSprite->field_32] < 0)
		return -1;

	sSprite* Dataa2C = word_3BF1E[pSprite->field_32];
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

void cFodder::sub_2DCB0( int16 pData0 ) {
	sMapTarget* Dataa24 = mSprite_WalkTargets[pData0];

	Dataa24->asInt = -1;
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
	uint8* Data24 = 0;
	const int16* IndestructibleTypes = 0;
	int16 Data0, Data4, Data10, TileType;

	if (word_3AF07) {
		--word_3AF07;
		goto loc_2DFC7;
	}

loc_2DE3C:;
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
	Data4 *= readLEWord( &mMap[0x54] );	// * Width

	Data0 >>= 4;
	Data4 += Data0;
	Data4 <<= 1;

	Data24 = &mMap[0x60 + Data4];

	Data10 = readLEWord( Data24 );
	Data10 &= 0x1FF;
	TileType = Data10;

	Data4 = mMap_Base_Swp[Data10];
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

	word_3AF07 = 1;
	writeLEWord( Data24, Data4 );
	word_3AF01 = Data4;
	Map_Destroy_Tiles_Next();

loc_2DFC7:;
	ax = mMap_Destroy_Tile_X;
	ax >>= 4;
	ax -= word_3B612;
	ax <<= 4;
	ax -= word_3B60E;
	ax += 0x40;

	mDrawSpritePositionX = ax;

	ax = mMap_Destroy_Tile_Y;
	ax >>= 4;
	ax -= word_3B614;
	ax <<= 4;
	ax -= word_3B610;
	ax += 0x10;

	mDrawSpritePositionY = ax;

	mDrawSpriteColumns = 0x10;
	mDrawSpriteRows = 0x10;

	g_Graphics.map_Tiles_Draw();
}

void cFodder::Map_Destroy_Tiles_Next() {
	
	sMapPosition* Data20 = mMap_Destroy_TilesPtr;
	++Data20;
	
	if( Data20 >= &mMap_Destroy_Tiles[128] ) 
		Data20 = mMap_Destroy_Tiles;
	
	mMap_Destroy_TilesPtr = Data20;
}

void cFodder::Game_Save_Wrapper2() {
	mGraphics->Load_Hill_Bits();

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

void cFodder::Recruit_Render_Text( const char* pText, int16 pPosY ) {

	String_CalculateWidth( 320, mFont_Recruit_Width, pText );
	String_Print( mFont_Recruit_Width, 0x0D, mGUI_Temp_X, pPosY, pText );
}

void cFodder::GUI_Button_Draw( const char* pText, int16 pY, int16 pColorShadow = 0xBF, int16 pColorPrimary = 0xBC ) {

	Recruit_Render_Text( pText, pY );

	GUI_Box_Draw( pColorShadow, pColorPrimary );
}

void cFodder::GUI_Box_Draw( int16 pColorShadow, int16 pColorPrimary ) {

	int16 X = mGUI_Temp_X;
	int16 Y = mGUI_Temp_Y;
	int16 Width = mGUI_Temp_Width;
	int16 Height = mGUI_Temp_Height;

	Briefing_DrawBox( X - 2, Y - 2, Width + 4, Height + 5, (uint8) pColorPrimary );
	Briefing_DrawBox( X - 3, Y - 3, Width + 4, Height + 5, (uint8) pColorShadow );
}

void cFodder::GUI_SaveLoad( bool pShowCursor ) {
	int8 byte_44B49 = 0;
	if (mGUI_SaveLoadAction != 3) {

		mImage->paletteFade();
		Mouse_Setup();
	}
	mGUI_SaveLoadAction = 0;

	mImageFaded = -1;
	mGraphics->PaletteSet();

	mImage->Save();
	bool mShow = false;

	do {
		if (mImageFaded == -1)
			mImageFaded = mImage->paletteFade();

		++byte_44B49;
		byte_44B49 &= 0x0F;
		if (!byte_44B49) {
			if (pShowCursor)
				mShow = !mShow;
			mMouse_Button_Left_Toggle = 0;
		}

		if (mShow)
			Sprite_Draw_Frame( 0x0F, 0x50, 0x00, mGUI_Temp_X + mGUI_Temp_Width );

		Mouse_Inputs_Get();
		Mouse_DrawCursor();

		++byte_44AC0;
		if (Mouse_Button_Left_Toggled() >= 0)
			GUI_SaveLoad_MouseHandle( mGUI_Elements );

		if (dword_3B30D)
			(this->*dword_3B30D)(0x50);

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
		mImage->Restore();

	} while (mGUI_SaveLoadAction <= 0);

	if (mGUI_SaveLoadAction == 3)
		return;

	mImage->paletteFadeOut();

	do {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
	} while (mImage->paletteFade() == -1);
}

void cFodder::Game_Save_Wrapper() {

	Game_Save();
}

void cFodder::GUI_Button_Setup( void(cFodder::*pFunction)(void) ) {
	sGUI_Element* Element = mGUI_NextFreeElement;

	Element->field_0 = &cFodder::GUI_Button_NoAction;

	Element->mX			= mGUI_Temp_X - 3;
	Element->mWidth		= mGUI_Temp_Width + 4;
	Element->mY			= mGUI_Temp_Y;
	Element->mHeight	= mGUI_Temp_Height + 5;
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

	dword_3B30D = &cFodder::String_Print_Input;

	GUI_SaveLoad(true);
	dword_3B30D = 0;
	if (mGUI_SaveLoadAction != 2) {
		mGUI_SaveLoadAction = 1;
		return;
	}
		
	mInputString[mInputString_Position + 0] = '.';
	mInputString[mInputString_Position + 1] = 'c';
	mInputString[mInputString_Position + 2] = 'f';
	mInputString[mInputString_Position + 3] = (mVersion->mGame == eGame::CF1) ? 0 : '2' ;
	mInputString[mInputString_Position + 4] = 0;

	std::string Filename = local_PathGenerate( mInputString, "", false );
	
	std::ofstream outfile (Filename,std::ofstream::binary);
	uint8* Start = (uint8*) &mMapNumber;
	uint8* End = (uint8*)&mButtonPressLeft;
	
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

void cFodder::String_Print_Input( int16 pPosY ) {
	GUI_Input_CheckKey();
	int16 Data4;
	char* Data24;

	Recruit_Render_Text( mInputString, pPosY );

	int16 Data0 = word_39F66;
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
	sub_2E6A9();
}

void cFodder::sub_2E6A9() {
	char* Data20 = mInputString;

	int16 Data0 = mInputString_Position;
	if (word_39F66 != 0x0D) {
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
				word_39F66 = 'A' + (mKeyCode - 4);
		
			if (mKeyCode >= SDL_SCANCODE_1 && mKeyCode <= SDL_SCANCODE_9)
				word_39F66 = '1' + (mKeyCode - 30);

			if (mKeyCode == SDL_SCANCODE_0 )
				word_39F66 = '0';

			if (mKeyCode == SDL_SCANCODE_RETURN)
				word_39F66 = 0x0D;

			if (mKeyCode == SDL_SCANCODE_BACKSPACE)
				word_39F66 = 8;

			return;
		}
	}

	word_39F66 = 0;
}

void cFodder::Game_Load() {
	
	mGUI_SaveLoadAction = 0;

	mGraphics->Load_Hill_Bits();

	std::vector<std::string> Files = local_DirectoryList( local_PathGenerate(  "", "", false  ), mVersion->mGame == eGame::CF1 ? ".cf" : ".cf2" );

	word_3B335 = 0;
	word_3B33D = (int16) Files.size();
	
	do {
		GUI_Element_Reset();
		Recruit_Render_Text( "SELECT FILE", 0x0C );

		GUI_Button_Draw( "UP", 0x24 );
		GUI_Button_Setup( &cFodder::GUI_Button_Load_Up );

		GUI_Button_Draw( "DOWN", 0x99 );
		GUI_Button_Setup( &cFodder::GUI_Button_Load_Down );

		GUI_Button_Draw( "EXIT", 0xB3 );
		GUI_Button_Setup( &cFodder::GUI_Button_Load_Exit );

		sGUI_Element* dword_3AEF7 = mGUI_NextFreeElement;
		mImage->Save();

		int16 DataC = 0;
		mGUI_NextFreeElement = dword_3AEF7;

		std::vector<std::string>::iterator FileIT = Files.begin() + word_3B335;

		for (; DataC < 4 && FileIT != Files.end(); ++DataC) {
			size_t Pos = FileIT->find_first_of( "." );

			memcpy( mInputString, FileIT->c_str(), Pos );
			mInputString[Pos] = 0x00;

			GUI_Button_Draw( mInputString, 0x3E + (DataC * 0x15), 0xB2, 0xB3 );
			GUI_Button_Setup( &cFodder::GUI_Button_Filename );
			++FileIT;
		}

		GUI_SaveLoad(false);
		mImage->Restore();

	} while (mGUI_SaveLoadAction == 3);

	if (mGUI_SaveLoadAction == 1)
		return;

	std::string Filename = local_PathGenerate( Files[word_3B335 + word_3B33F], "", false );
	
	std::ifstream infile (Filename,std::ofstream::binary);
	uint8* Start = (uint8*) &mMapNumber;
	uint8* End = (uint8*)&mButtonPressLeft;
	
	infile.read ((char*) Start, End - Start );
	infile.close();
	mMouse_Exit_Loop = 0;

	mGraveRankPtr = mGraveRanks;
	mGraveRankPtr2 = mGraveRankPtr;
	mGraveRecruitIDPtr = mGraveRecruitID;

	for (int16 x = 0; x < 361; ++x) {
		if (*mGraveRankPtr == -1)
			break;

		++mGraveRankPtr;
		++mGraveRankPtr2;
	}
	
	for (int16 x = 0; x < 361; ++x) {
		if (*mGraveRecruitIDPtr == -1)
			break;

		++mGraveRecruitIDPtr;
	}

	for (int16 x = 0; x < 8; ++x)
		mSquad_SpritePtrs[x] = INVALID_SPRITE_PTR;

	for (int16 x = 0; x < 9; ++x)
		mSquad[x].mSprite = INVALID_SPRITE_PTR;

	Mission_Memory_Backup();
}

void cFodder::GUI_Button_Load_Up() {
	word_3B335 -= 3;
	if (word_3B335 < 0)
		word_3B335 = 0;

	mGUI_SaveLoadAction = 3;
}

void cFodder::GUI_Button_Load_Down() {
	word_3B335 += 3;

	int16 Data0 = word_3B335;
	Data0 += 4;

	if (Data0 >= word_3B33D) {
		Data0 = word_3B33D;
		Data0 -= 4;
		if (Data0 < 0)
			Data0 = 0;

		word_3B335 = Data0;
	}

	mGUI_SaveLoadAction = 3;
}

void cFodder::GUI_Button_Filename() {
	int16 Data0 = mMouseY;

	Data0 -= 0x3E;
	Data0 /= 0x15;

	word_3B33F = Data0;
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
	sub_30465();

	mGUI_NextFreeElement = mGUI_Elements;

	map_ClearSpt();

	mGUI_Squad_NextDraw_Y = 0;
	int16 Data4 = 0;
	GUI_Sidebar_Prepare(0, Data4);

	Data4 = mGUI_Squad_NextDraw_Y;
	if (Data4)
		Data4 += 5;
	GUI_Sidebar_Prepare(1, Data4);

	Data4 = mGUI_Squad_NextDraw_Y;
	if (Data4)
		Data4 += 5;
	GUI_Sidebar_Prepare(2, Data4);
	sub_2EBC4();

	mMouseDisabled = 0;

	sub_30480();
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

		Data0 = word_3BEC3;
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

void cFodder::Demo_Quiz_ShowScreen( const char* pFilename ) {
		
	mImage->paletteFadeOut();
	
	while( mImage->GetFaded() == false ) {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->paletteFade();
	}

	mGraphics->imageLoad( pFilename, 32 );
	mGraphics->PaletteSet();

	mImage->Save();
	mImage->paletteFade();
	
	for( ;; ) {
		Video_Sleep_Wrapper();

		Mouse_Inputs_Get();
		Mouse_DrawCursor();

		if (mButtonPressLeft)
			break;

		if (mImage->GetFaded() == false )
			mImage->paletteFade();

		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->Restore(); 
	}
		
	mImage->paletteFadeOut();
	
	while( mImage->GetFaded() == false ) {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->paletteFade();
	}

	mGraphics->imageLoad( "1.lbm", 32 );
	mGraphics->PaletteSet();

	mImage->Save();
	mImage->paletteFade();
}

void cFodder::Demo_Quiz() {
	mImage->paletteFadeOut();
	
	while( mImage->GetFaded() == false ) {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->paletteFade();
	}

	mGraphics->imageLoad( "1.lbm", 32 );
	mGraphics->PaletteSet();

	mImage->Save();
	mImage->paletteFade();
	Mouse_Setup();

	for( ;; ) {
		Video_Sleep_Wrapper();

		Mouse_Inputs_Get();
		Mouse_DrawCursor();
		if (mButtonPressLeft) {
			GUI_Element_Mouse_Over( mPlusQuiz_Buttons );
		}

		if (mDemo_ExitMenu >= 0)
			break;

		if (mImage->GetFaded() == false )
			mImage->paletteFade();

		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->Restore(); 
	}
		
	mImage->paletteFadeOut();
	
	while( mImage->GetFaded() == false ) {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->paletteFade();
	}

	mImage->clearBuffer();

	mGraphics->imageLoad( "apmenu.lbm", 32 );
	mGraphics->PaletteSet();
	mImage->paletteFade();
	mImage->Save();
}

void cFodder::sub_2EBE0( int16& pData0, int16& pData4 ) {
	mGUI_Loop_Squad_Current = pData0;
	word_3AC1D = pData4;
	word_3AC47 = 0;

	if (pData0 == mSquad_Selected)
		word_3AC47 = -1;

	//pData0 <<= 1;
	mGUI_Squad_Current = pData0;
}

void cFodder::GUI_Sidebar_Prepare( int16 pSquadNumber, int16 pData4 ) {
	
	sub_2EBE0( pSquadNumber, pData4 );

	word_3AC2D[pSquadNumber] = pData4;
	Squad_Troops_Count();
	
	pSquadNumber = mGUI_Loop_Squad_Current;

	if (!mSquads_TroopCount[pSquadNumber])
		return;

	GUI_Sidebar_SplitButton_Draw();
	GUI_Prepare_Button_Squad();

	Mission_Set_Initial_Weapon();

	GUI_Sidebar_Grenades_Draw();
	GUI_Prepare_Button_Grenade();

	GUI_Sidebar_Rockets_Draw();
	GUI_Prepare_Button_Rockets();

	sub_2F452();

	GUI_Sidebar_TroopList_Draw();
	GUI_Prepare_Button_TroopName();
}

void cFodder::sub_2EBC4() {
	sGUI_Element* Data20 = mGUI_NextFreeElement;

	GUI_SetElementsFrom( Data20, stru_3DEDC );
}

void cFodder::GUI_SetElementsFrom( sGUI_Element* pData20, sGUI_Element* pData24 ) {
	
	while (pData24->field_0 != 0) {

		pData20->field_0 = pData24->field_0;
		pData20->mX		 = pData24->mX;
		pData20->mWidth  = pData24->mWidth;
		pData20->mY		 = pData24->mY;
		pData20->mHeight = pData24->mHeight;
		pData20->mMouseInsideFuncPtr = pData24->mMouseInsideFuncPtr;

		++pData20;
		++pData24;
	}

	GUI_ClearElement( pData20 );

}

void cFodder::GUI_ClearElement( sGUI_Element *pData20 ) {

	pData20->field_0 = 0;
	mGUI_NextFreeElement = pData20;
}

void cFodder::GUI_Sidebar_SplitButton_Draw() {

	int16 Data0 = word_3DEF0[mGUI_Squad_Current];
	int16 Data8 = 0;
	int16 DataC = word_3AC1D;

	mGraphics->sub_145AF( Data0, Data8, DataC );
}

void cFodder::GUI_Prepare_Button_Squad() {
	sGUI_Element* Element = mGUI_NextFreeElement;

	Element->field_0 = &cFodder::GUI_Button_NoAction;
	Element->mX = 0;
	Element->mWidth = 0x2F;
	Element->mY = word_3AC1D;
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
	Squad_Clear_Selected();
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

	sub_2DCB0( mSquad_Selected );
	Squad_Troops_Count();

	sSprite* Data20 = 0;
	sub_305D5( Data20 );

	mSquad_Select_Timer = 1;
	mGUI_Sidebar_Setup = 0;
}

void cFodder::Mission_Set_Initial_Weapon() {

	// segra: I don't believe this condition can ever occur
	if (mSquad_CurrentWeapon[mGUI_Squad_Current] == eWeapon_Grenade) {

		if (mSquad_Grenades[mGUI_Squad_Current])
			return;
		if (!mSquad_Rockets[mGUI_Squad_Current])
			return;

		mSquad_CurrentWeapon[mGUI_Squad_Current] = eWeapon_Rocket;
		return;
	}

	//loc_2EFF1
	if (mSquad_Rockets[mGUI_Squad_Current])
		return;

	if (!mSquad_Grenades[mGUI_Squad_Current])
		return;

	mSquad_CurrentWeapon[mGUI_Squad_Current] = eWeapon_Grenade;
}

void cFodder::Service_Show() {

	if (mVersion->mRelease == eRelease::Demo) 
		return;

	WindowTitleSet( false );

	mGraphics->Load_Service_Data();
	mGraphics->SetSpritePtr( eSPRITE_SERVICE );
	mGraphics->PaletteSet();

	mSound->Music_Play( 0 );
	Service_KIA_Loop();
	Service_Promotion_Loop();
	Mouse_Setup();

	mGraphics->LoadpStuff();
}

void cFodder::Service_KIA_Loop() {
	sub_136D0();
	Mouse_Setup();
	word_44475 = 0;

	if (Service_KIA_Troop_Prepare() < 0)
		return;

	mImage->clearBuffer();

	if (mVersion->mPlatform == ePlatform::PC) {
		Sprite_Draw_Frame(  5, 0, 0, 0x34 );
		sub_13C8A(  7, 0, 0, 0x31 );
		sub_13C8A(  7, 0, 0xF0, 0x31 );
	}
	else {
		((cGraphics_Amiga*)mGraphics)->Service_Draw( 8, 0x30,	0, 1 );
		((cGraphics_Amiga*)mGraphics)->Service_Draw( 0, 0,		0x40, 0 );
		((cGraphics_Amiga*)mGraphics)->Service_Draw( 4, 0xF0,	0x40, 0 );
	}

	mImageFaded = -1;
	mMouse_Exit_Loop = 0;
	word_40048 = 0;
	mGraphics->PaletteSet();
	mImage->Save();

	do {
		Mouse_Inputs_Get();

		if (word_44475 == -1 || mMouse_Exit_Loop ) {
			mMouse_Exit_Loop = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
			word_40048 = 1;
		}

		if (mImageFaded == -1)
			mImageFaded = mImage->paletteFade();

		sub_18149();
		Video_Sleep_Wrapper();
		//sub_14445();
		sub_181BD();

		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
		mImage->Restore();

	} while (mImageFaded == -1 || word_40048 == 0);
}

void cFodder::Service_Promotion_Loop() {
	sub_136D0();
	Mouse_Setup();
	word_44475 = 0;
	Service_Promotion_Prepare();

	if (Service_Promotion_Prepare_Draw() < 0)
		goto loc_18001;

	mImage->clearBuffer();
	if (mVersion->mPlatform == ePlatform::PC) {
		Sprite_Draw_Frame( 6, 0, 0, 0x34 );
		sub_13C8A( 8, 0, 0, 0x31 );
		sub_13C8A( 8, 0, 0xF0, 0x31 );
	}
	else {
		((cGraphics_Amiga*)mGraphics)->Service_Draw( 9, 0x30,	0,	  1 );
		((cGraphics_Amiga*)mGraphics)->Service_Draw( 3, 0,		0x40, 0 );
		((cGraphics_Amiga*)mGraphics)->Service_Draw( 7, 0xF0,	0x40, 0 );
	}

	mImageFaded = -1;
	word_40048 = 0;
	mMouse_Exit_Loop = 0;
	mGraphics->PaletteSet();
	mImage->Save();

	do {

		Mouse_Inputs_Get();
		if (word_44475 == -1 || mMouse_Exit_Loop ) {
			mMouse_Exit_Loop = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
			word_40048 = 1;
		}

		if (mImageFaded == -1)
			mImageFaded = mImage->paletteFade();

		Service_Promotion_Check();
		sub_18149();
		Video_Sleep_Wrapper();
		//sub_14445();
		sub_181BD();

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
		mImage->Restore();

	} while (mImageFaded == -1 || word_40048 == 0);

loc_18001:;
	
	Service_Promotion_SetNewRanks();
}

int16 cFodder::Service_KIA_Troop_Prepare() {
	uint16* di = (uint16*) mDataPStuff;

	mDrawSpritePositionY = 0xE8;
	Service_Mission_Text_Prepare( di );
	mDrawSpritePositionY += 0x40;

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
		mDrawSpritePositionY += 0x40;
	}

	*di++ = -1;
	return 0;
}

int16 cFodder::Service_Promotion_Prepare_Draw() {
	uint16* di = (uint16*) mDataPStuff;

	mDrawSpritePositionY = 0xE8;
	Service_Mission_Text_Prepare( di );
	mDrawSpritePositionY += 0x40;

	uint16* di2 = di;
	sSquad_Member* si = mSquad;

	for (int16 word_3ABE1 = 7; word_3ABE1 >= 0; --word_3ABE1, ++si) {

		if (si->mSprite != INVALID_SPRITE_PTR ) {
			int16 ax = si->mRecruitID;
			int8 bl = si->mRank;

			bl &= 0xFF;

			Service_Draw_Troop_And_Rank( di, ax, bl );

			mDrawSpritePositionY += 0x40;
		}
	}
	if (di == di2)
		return -1;

	*di++ = -1;

	return 0;
}

void cFodder::Service_Draw_Troop_And_Rank( uint16*& pDi, int16 pRecruitID, int16 pRank ) {
	word_3A016 = pRank;

	if (mVersion->mPlatform == ePlatform::PC) {
		*pDi++ = 9;
		*pDi++ = 0;
	}
	else {
		*pDi++ = 0;
		*pDi++ = 1;
	}
	*pDi++ = 0x60;
	*pDi++ = mDrawSpritePositionY;

	*pDi++ = 2;
	*pDi++ = pRank;
	*pDi++ = 0x64;
	*pDi++ = mDrawSpritePositionY + 4;

	*pDi++ = 2;
	*pDi++ = pRank;
	*pDi++ = 0xCC;
	*pDi++ = mDrawSpritePositionY + 4;

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
	sub_181E6( pDi, tmpString.str().c_str(), mFont_ServiceName_Width, 3, mGUI_Temp_X, mDrawSpritePositionY + 6 );
}

void cFodder::sub_18149() {

	word_42072 = 0;
	int16 *di = (int16*) mDataPStuff;

	for (;;) {
		if (*di < 0)
			break;

		int16 Data0 = *di++;
		int16 Data4 = *di++;
		int16 Data8 = *di++;
		int16 DataC = *di++;

//		sub_13C8A( Data0, Data4, Data8, DataC );
		sub_1828A( Data0, Data4, Data8, DataC );
		//sub_17B2A();
	}
}

void cFodder::sub_181BD() {
	word_44475 = -1;

	int16* es = (int16*)mDataPStuff;

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

		if (word_3AC19) {

			if (Data4 == 0x20)
				goto loc_18229;

			Data4 = word_3AC19;
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

int16 cFodder::sub_1828A( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
	byte_42070 = mSpriteDataPtr[pData0][pData4].field_C & 0xFF;

	mDrawSpriteFrameDataPtr = mGraphics->GetSpriteData( mSpriteDataPtr[pData0][pData4].field_2 ) + mSpriteDataPtr[pData0][pData4].field_0;

	mDrawSpritePositionX = pData8 + 0x10;
	mDrawSpritePositionY = pDataC + 0x10;

	mDrawSpriteColumns = mSpriteDataPtr[pData0][pData4].mColCount;
	mDrawSpriteRows = mSpriteDataPtr[pData0][pData4].mRowCount;

	if (!sub_184C7()) {
		if (mVersion->mPlatform == ePlatform::Amiga)
			mGraphics->video_Draw_Linear();
		else
			sub_182EA();

		return 1;
	}
	return 0;
}

void cFodder::sub_182EA() {
	const uint8* bp = &mFont_Intro_Width[0x80];

	uint8* di = mImage->GetSurfaceBuffer();
	
	di += (mImage->GetWidth() * mDrawSpritePositionY);

	di += (mDrawSpritePositionX + word_40054);
	word_42066 = di;

	uint8* si = mDrawSpriteFrameDataPtr;

	mDrawSpriteColumns >>= 1;
	word_42074 = 0xA0 - mDrawSpriteColumns;
	mDrawSpriteColumns >>= 1;

	word_42076 = mImage->GetWidth() - (mDrawSpriteColumns*4);
	uint8 Plane = 0;

	for (int16 dx = 0; dx < mDrawSpriteRows; ++dx ) {

		int16 bx = mDrawSpritePositionY;
		bx += dx;

		uint8 bl = *(bp + bx);

		for (int16 cx = mDrawSpriteColumns; cx > 0; --cx) {
			uint8 al = *si;
			al >>= 4;
			if (al) {
				al |= bl;
				*di = al;
			}
			
			di += 4;
			si += 2;
		}

		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}

	si = mDrawSpriteFrameDataPtr;
	di = word_42066;
	di += Plane;
	for (int16 dx = 0; dx < mDrawSpriteRows; ++dx ) {

		int16 bx = mDrawSpritePositionY;
		bx += dx;

		uint8 bl = *(bp + bx);

		for (int16 cx = mDrawSpriteColumns; cx > 0; --cx) {
			uint8 al = *si;
			al &= 0x0F;
			if (al) {
				al |= bl;
				*di = al;
			}
			
			di += 4;
			si += 2;
		}

		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}

	++mDrawSpriteFrameDataPtr;
	si = mDrawSpriteFrameDataPtr;
	di = word_42066;
	di += Plane;

	for (int16 dx = 0; dx < mDrawSpriteRows; ++dx ) {

		int16 bx = mDrawSpritePositionY;
		bx += dx;

		uint8 bl = *(bp + bx);

		for (int16 cx = mDrawSpriteColumns; cx > 0; --cx) {
			uint8 al = *si;
			al >>= 4;
			if (al) {
				al |= bl;
				*di = al;
			}
			
			di += 4;
			si += 2;
		}

		si += word_42074;
		di += word_42076;
	}
	
	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}

	si = mDrawSpriteFrameDataPtr;
	di = word_42066;
	di += Plane;
	for (int16 dx = 0; dx < mDrawSpriteRows; ++dx ) {

		int16 bx = mDrawSpritePositionY;
		bx += dx;

		uint8 bl = *(bp + bx);

		for (int16 cx = mDrawSpriteColumns; cx > 0; --cx) {
			uint8 al = *si;
			al &= 0x0F;
			if (al) {
				al |= bl;
				*di = al;
			}
			
			di += 4;
			si += 2;
		}

		si += word_42074;
		di += word_42076;
	}
}

int16 cFodder::sub_184C7() {
	int16 ax;

	if (mDrawSpritePositionY < 0x2C) {
		ax = mDrawSpritePositionY;
		ax += mDrawSpriteRows;
		--ax;
		if (ax < 0x2C)
			return -1;

		ax -= 0x2C;
		ax -= mDrawSpriteRows;
		++ax;
		ax = -ax;
		mDrawSpritePositionY += ax;
		mDrawSpriteRows -= ax;

		if (mVersion->mPlatform == ePlatform::PC)
			ax *= 160;
		else if (mVersion->mPlatform == ePlatform::Amiga)
			ax *= 40;

		mDrawSpriteFrameDataPtr += ax;
	}
	//loc_184FC

	ax = mDrawSpritePositionY;
	ax += mDrawSpriteRows;
	--ax;

	if (ax <= 0xE7)
		return 0;

	if (mDrawSpritePositionY > 0xE7)
		return -1;

	ax -= 0xE7;
	mDrawSpriteRows -= ax;
	return 0;
}

void cFodder::Service_Mission_Text_Prepare( uint16*& pTarget ) {
	std::stringstream Mission;
	Mission << "MISSION ";

	Mission << tool_StripLeadingZero( tool_NumToString( mMissionNumber ) );

	String_CalculateWidth( 0x140, mFont_Service_Width, Mission.str().c_str() );

	sub_181E6( pTarget, Mission.str(), mFont_Service_Width, 4, mGUI_Temp_X, mDrawSpritePositionY );
}

void cFodder::Service_Promotion_Prepare() {
	int16* Data28 = word_3ABFF;
	dword_3AC11 = Data28;

	sSquad_Member* Data20 = mSquad;

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
	int16* si = word_3ABFF;
	int16* es = (int16*)mDataPStuff;

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
	int16* Data28 = word_3ABFF;
	sSquad_Member* Data20 = mSquad;

	for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
		
		if (Data20->mRecruitID == -1)
			continue;

		int16 ax = *Data28++;
		if (ax < 0)
			return;

		Data20->mRank = (int8) ax;
	}
}

void cFodder::map_ClearSpt() {
	
	for (uint16 cx = 0; cx < 0x2000; ++cx )
		word_3D5B7[cx] = 0;
}

void cFodder::Briefing_Show( ) {
	const char* Str_Brief = "BRIEFING";
	const char* Str_Phase = "PHASE ";
	const char* Str_Of = " OF ";

	word_3AC19 = 0x25;

	String_CalculateWidth( 320, mFont_Underlined_Width, Str_Brief );
	String_Print( mFont_Underlined_Width, 0x03, mGUI_Temp_X, 0x4E, Str_Brief );
	
	Briefing_DrawBox( 1, 0x49, 0x13E, 0x6B, 0xF3 );
	Briefing_DrawBox( 0, 0x48, 0x13E, 0x6B, 0xF2 );
	
	word_3AC19 = 0;

	std::stringstream Phase;

	Phase << Str_Phase	<< tool_StripLeadingZero( tool_NumToString( mMissionPhase + 1 ) );
	Phase << Str_Of		<< tool_StripLeadingZero( tool_NumToString( mMissionPhases ));

	String_CalculateWidth( 320, mFont_Briefing_Width, Phase.str().c_str() );
	String_Print( mFont_Briefing_Width, 0, mGUI_Temp_X, 0x1D, Phase.str().c_str() );
	
	Mission_Phase_Goals_Set();

	int16 DataC = 0x84;
	const char* Data20 = 0;
	const char** Data28 = mMissionGoals;
	int16* Goals = mPhase_Goals;

	for (int16 Data0 = 7 ;Data0>=0; ++Data28, --Data0 ) {
		if (*Goals++) {

			Data20 = *Data28;
			String_CalculateWidth( 0x140, mFont_Briefing_Width, Data20 );
			String_Print( mFont_Briefing_Width, 0, mGUI_Temp_X, DataC - 0x12, Data20 );
			DataC += 0x0C;
		}
	}
}

void cFodder::Briefing_Prepare() {
	mImage->clearBuffer();
	mGraphics->PaletteSet();

	sub_136D0();
	mGraphics->SetSpritePtr( eSPRITE_BRIEFING );
	word_3A01A = 0x2C;

	Briefing_Draw_Mission_Title( );
	Briefing_Show();
	mImage->paletteFade();

	do {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
	} while (mImage->paletteFade() == -1);

}

void cFodder::Briefing_Wait() {
	mGraphics->SetSpritePtr( eSPRITE_BRIEFING );

	word_3A01A = 0x2C;
	Briefing_Draw_Mission_Title( );
	Briefing_Show( );
	Briefing_Draw_With( );

	do {
		Video_Sleep();
		eventProcess();
		Mouse_Inputs_Get();

		if (mMission_Aborted == -1) {
			word_3B4F5 = -1;
			mouse_Button_Status = -1;
			break;
		}

		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();

	} while (!mMouse_Exit_Loop);

	mMouse_Exit_Loop = 0;

	mImage->paletteFadeOut();
	do {
		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
	} while (mImage->paletteFade() == -1);

	Mouse_Setup();
}

void cFodder::Briefing_Draw_With( ) {
	std::stringstream With;
	
	With << "WITH ";
	With << tool_StripLeadingZero( tool_NumToString( mTroopsAvailable ) );

	if (mTroopsAvailable == 1) {
		With << " SOLDIER YOU MUST";
	}
	else {
		With << " SOLDIERS YOU MUST";
	}

	String_CalculateWidth( 0x140, mFont_Briefing_Width, With.str().c_str() );
	String_Print( mFont_Briefing_Width, 0, mGUI_Temp_X, 0x64, With.str().c_str() );
	With.str("");

	if (!mSquad_AliveCount) {
		With << "THIS IS YOUR LAST CHANCE";
	}
	else {
		With << mSquad_AliveCount;
		if (mSquad_AliveCount == 1)
			With << " RECRUIT REMAINING";
		else
			With << " RECRUITS REMAINING";
	}

	String_CalculateWidth( 0x140, mFont_Briefing_Width, With.str().c_str() );
	String_Print(  mFont_Briefing_Width, 0, mGUI_Temp_X, 0xA8, With.str().c_str() );
	
	String_CalculateWidth( 0x140, mFont_Underlined_Width, "GO FOR IT" );
	String_Print(  mFont_Underlined_Width, 3, mGUI_Temp_X, 0xB8, "GO FOR IT" );
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

void cFodder::Intro_Print_String( int32 pPosY,  const sIntroString* pString ) {

	if (mVersion->mPlatform == ePlatform::PC)
		pPosY -= 0x19;
	else
		pPosY += 0x9;
	String_CalculateWidth( 320, mFont_Intro_Width, pString->mText );
	String_Print(  mFont_Intro_Width, 0, mGUI_Temp_X, pPosY, pString->mText );
}

void cFodder::Briefing_Draw_Vertical_Line(  int16 pX, int16 pHeight, int16 pY, uint8 pColor ) {
	
	do {
		Briefing_Draw_Pixel( pX, pY, pColor );
		++pY;
	} while (pY <= pHeight);
}

void cFodder::sub_18D5E() {

	word_39FF4 -= 1;
	if (word_39FF4 < 0) {
		word_39FF4 = 3;

		word_39FF2 += word_3BEB9;
		if (word_39FF2 == 0 || word_39FF2 == 2)
			word_3BEB9 ^= -2;

		word_39FF6 += word_3BEBB;
		if (word_3BEBB == 0 || word_3BEBB == 2)
			word_3BEBB ^= -2;
	}

	//loc_18DA7
	word_39FF8 -= 1;
	if (word_39FF8 < 0) {
		word_39FF8 = 1;

		word_39FFA += word_3BEBD;
		if (word_39FFA == 0 || word_39FFA == 2)
			word_3BEBD ^= -2;
	}
}

void cFodder::Sprite_Handle_Loop() {
	sSprite* Data20 = mSprites;

	for (int16 Data1C = 0x2B; Data1C > 0; --Data1C, ++Data20) {

		if (Data20->field_0 == -32768)
			continue;

		word_44A20 = Data20->field_18;
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

	if (pSprite->field_6E) {
		sub_22AA9( pSprite );
		return;
	}

	if (pSprite->field_75 & 2) {

		if (pSprite->field_38 <= 0x32)
			pSprite->field_38 = 0;

		pSprite->field_5B = 0;
		pSprite->field_64 = 0;
	}

	if (sub_1E05A( pSprite ))
		return;

	if (pSprite->field_38)
		return;

	if (sub_1F21E( pSprite ))
		return;

	//loc_18E8D
	Sprite_Handle_Player_Unk( pSprite );
	word_3AA41 = 0;

	word_3AA1D = word_3BED5[pSprite->field_32];
	//seg004:0183

	if (byte_3BF1B[pSprite->field_32] < 0 && pSprite->field_32 != mSquad_Selected ) {
		//loc_18F12
		//seg004:01C2
		word_3AA41 = -1;
		//Data0 <<= 1;   UNUSED?

		Data28 = &mSprites[pSprite->field_5E];
		
		if( Data28->field_0 != -32768 ) {
			
			if( Data28->field_18 == eSprite_Enemy ) {
				if( Data28->field_38 == 0x00 ) {
				
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
							word_3AA4B = Data0;
							Data0 = pSprite->field_0;
							Data4 = pSprite->field_4;
							Data8 = Data28->field_0;
							DataC = Data28->field_4;

							if (sub_2A4A2( Data0, Data4, Data8, DataC ) == 0) {
								Data0 = word_3AA4B;
								goto loc_1904A;
							}
							if (word_3AA4B < 0x40)
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
		mTroop_Cannot_Throw_Grenade = 0;
		mTroop_Cannot_Fire_Bullet = 0;
		word_3A010 = 0;

		pSprite->field_2E = Data28->field_0;

		// WHAT THE?
		//seg004:0337
		pSprite->field_30 += 0x07;

		pSprite->field_30 = Data28->field_4;
		pSprite->field_30 -= 0x0E;

		if (pSprite->field_4A <= 0) {
			//loc_190B9
			Data0 = tool_RandomGet();
			Data0 &= 0x0F;
			++Data0;
			pSprite->field_4A = Data0;
			Data0 = tool_RandomGet();
			Data0 &= 0x3F;
			if (Data0 == 0x2A)
				goto loc_1918C;
		}
		
	loc_190E8:;
		mTroop_Cannot_Throw_Grenade = 0;
		mTroop_Cannot_Fire_Bullet = 0;
		word_3A010 = 0;

		if (pSprite->field_4F) {
			word_3A010 = -1;
			mTroop_Cannot_Fire_Bullet = -1;
			mTroop_Cannot_Throw_Grenade = -1;
		}
		else {
			//loc_19118
			if (pSprite == mSquad_Leader && mMouse_Button_LeftRight_Toggle ) {

				Data0 = mSquad_CurrentWeapon[ pSprite->field_32 ];
				if (Data0 == eWeapon_Rocket) {
					mTroop_Cannot_Throw_Grenade = -1;
					mTroop_Cannot_Fire_Bullet = -1;
				} else {
					if (Data0 == eWeapon_Grenade) {
						word_3A010 = -1;
						mTroop_Cannot_Fire_Bullet = -1;
					}
					else
						goto loc_19198;
				}
			loc_1918C:;
				pSprite->field_4A = 1;
				goto loc_191BF;
			}

		loc_19198:;
			mTroop_Cannot_Throw_Grenade = -1;
			word_3A010 = -1;

			Data0 =  ((sSquad_Member*) pSprite->field_46)->field_6;

		loc_191BF:;
			Sprite_Handle_Troop( pSprite );
		}
	}
	else {

		if (word_3A9CE) {
			goto loc_190E8;
			
		}
	}

loc_191C3:;
	if (pSprite->field_22)
		goto loc_1921E;

	// seg004:047F
	eax = mSprite_WalkTargets[pSprite->field_32][pSprite->field_40].field_2;

	//seg004:04CC
	if (eax < 0) {
	loc_1921E:;
		if (word_3AA41) {

			if ( pSprite->field_0 == pSprite->field_26 &&
				 pSprite->field_4 == pSprite->field_28)
				 goto loc_1931E;
		}
	}
	//loc_1925C
	if (pSprite->field_54 != 2 && pSprite->field_54 != 1)
		goto loc_19338;

	//loc_19274
	if (word_3AA1D != 2)
		goto loc_1931E;

	mTmp_FrameNumber = pSprite->field_A;
	pSprite->field_5A = 0;

	if (word_3AA41)
		sub_1F5A0( pSprite );
	else
		sub_1F5CA( pSprite );

	Data0 = pSprite->field_26;
	if( Data0 < 0 )
		goto loc_1946D;
	
	Data4 = pSprite->field_28;
	if( Data4 < 0 )
		goto loc_1946D;
	
	Sprite_XY_Store( pSprite );
	
	if( sub_2A1F0( pSprite, Data0, Data4 ) >= 0 ) {
		sub_1F66F( pSprite );
		sub_1F6F4( pSprite );
	}
		
	Data0 = pSprite->field_0;
	if( Data0 == pSprite->field_26 ) {
		Data0 = pSprite->field_4;
		
		if( Data0 == pSprite->field_28 )
			Sprite_Next_WalkTarget_Set( pSprite );
	}
	//loc_19314
	if( word_3ABAD == 0 )
		return;
	
	loc_1931E:;
	
	if( pSprite->field_5A )
		goto loc_19403;
	
	sub_1F5CA( pSprite );
	sub_1F762( pSprite );
	return;
	
	loc_19338:;
	pSprite->field_43 = 0;
	mTmp_FrameNumber = pSprite->field_A;
	word_3A9C6 = 0;
	
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
	
	//loc_19392
	if( sub_2A1F0( pSprite, Data0, Data4 ) >= 0 )
		goto loc_193D6;
	

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
	sub_1F5CA( pSprite );
	return;
	
loc_19424:;	// Soldier Walking
	sub_1F6F4( pSprite );
	
	if( pSprite->field_0 == pSprite->field_26 ) {
		
		if( pSprite->field_4 == pSprite->field_28 )
			Sprite_Next_WalkTarget_Set( pSprite );
		
	}
	//loc_19463
	if( word_3ABAD )
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
	sub_1F5CA( pSprite );
	return;

loc_194A0:;
	pSprite->field_43 = -1;
	
	if( !word_3AA1D ) {
		word_3A8CF = 0;
		sub_1F762( pSprite );
		pSprite->field_A = 0;
		return;
		
	}
	//loc_194CB
	
	if( mSquad_Selected == pSprite->field_32 )
		goto loc_19490;
	
	sub_1F5CA( pSprite );
	return;

loc_1957A:;
	pSprite->field_3A = 0;
	sub_1F762( pSprite );

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
			Sound_Play( pSprite, 0x0F, 0x0F );
	}
	
	Data24 = (sSprite*) pSprite->field_46;
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

		sub_2A1F0( pSprite, Data0, Data4 );
		Sprite_Movement_Calculate( pSprite );
		sub_21525( pSprite );

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
			pSprite->field_1A = (int32*)(((int64)pSprite->field_1A & 0xFFFF) | (-8 << 16));
	Dataa4 = (int32) ((int64)pSprite->field_1A );
	Dataa4 -= 0x18000;

	pSprite->field_1A = (int32*)Dataa4;
	Dataa4 = (int32) ((int64) pSprite->field_1A);

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
	tmp = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	tmp += Dataa4;

	pSprite->field_1E = tmp & 0xFFFF;
	pSprite->field_20 = tmp >> 16;

	if (pSprite->field_20 < 0) {
		pSprite->field_1E = 0;
		pSprite->field_20 = 0;

		if (pSprite->field_50)
			goto loc_19855;

		Dataa4 = (int64) pSprite->field_1A;
		Dataa4 = -Dataa4;
		Dataa4 >>= 1;
		pSprite->field_1A = (int32*) Dataa4;
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

	if (sub_1E05A( pSprite ))
		return;

	if (pSprite->field_38)
		return;

	word_3AA41 = 0;
	if (pSprite->field_44) {
		pSprite->field_44--;
	}
	else {
		sub_21CD1( pSprite );
		Sprite_Handle_Troop( pSprite );

		mTmp_FrameNumber = pSprite->field_A;
		int16 Data0 = pSprite->field_26;
		if (Data0 < 0)
			goto loc_19A89;

		int16 Data4 = pSprite->field_28;
		if (Data4 < 0)
			goto loc_19A89;

		if (sub_2A1F0( pSprite, Data0, Data4 ) < 0)
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
	sub_1F6F4( pSprite );
	goto loc_19A96;

loc_19A89:;
	word_3ABB1 = 0;
	sub_1FDE7( pSprite );
	goto loc_19A9C;

loc_19A96:;
	word_3ABB1 = -1;

loc_19A9C:;
	pSprite->field_22 = 1;
	sub_1F762( pSprite );

	if (word_3ABB1)
		return;
	if (word_3AA41)
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
	
	Data28 = (sSprite*) pSprite->field_1A;
	
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
	
	if(pSprite->field_22)
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
	
	sub_2A1F0( pSprite, Data0, Data4 );
	
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
	if(word_3B173)
		goto loc_19E3D;
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
	
	pSprite->field_2C = 0;
	pSprite->field_0 = pSprite->field_2E;
	pSprite->field_4 = pSprite->field_30;
	pSprite->field_44 = -1;
	goto loc_19D3C;
	
loc_19D24:;
	pSprite->field_2C = -1;
	
	Sprite_Movement_Calculate(pSprite);
	if( word_3B173 )
		goto loc_19E3D;
	
	loc_19D3C:;
	if( pSprite->field_64 > 2 ) {
		Data0 = -9;
		Data4 = 0;
		if(Map_Terrain_Get_Type_And_Walkable(pSprite, Data0, Data4))
			goto loc_19DCF;
	}

	if( !sub_21618( pSprite ) )
		return;
	
	if(mSprites_Found_Count == 1 )
		goto loc_19DCF;
	
	pSprite->field_43 = 1;
	pSprite->field_2C = 1;
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
		goto loc_19E3D;
	
	return;
	
loc_19DCF:;
	al = pSprite->field_43;
	if(al)
		goto loc_19E1C;
	
	sub_21618( pSprite );
	pSprite->field_43 = -1;
	pSprite->field_2C = -1;
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
	
loc_19E3D:;
	
	byte_3A9D2[2] -= 1;
	Sprite_Destroy( pSprite );
	return;
	
loc_19E50:;
	pSprite->field_0 = 0;
	pSprite->field_4 = 0x1000;
	goto loc_19E3D;
}

void cFodder::Sprite_Handle_Helicopter( sSprite* pSprite ) {
	word_3B4ED[0] = 0;
	int16 Data0 = tool_RandomGet() & 0x0E;
	int16 Data4, Data8, DataC, Data10, Data14, Data18, Data1C;
	sSprite* Data24 = 0, *Data2C = 0;
	int32 Field_1E;

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

	if (pSprite->field_22)
		goto loc_19EB5;

	if (pSprite->field_20 >= 2)
		goto loc_19EC0;

	if (pSprite != mSquad_CurrentVehicle)
		goto loc_19EE5;

	goto loc_19EC0;

loc_19EB5:;
	if (pSprite->field_20 <= 1)
		goto loc_19EE5;

loc_19EC0:;
	word_3B4ED[0] = -1;
	Data4 = pSprite->field_20;
	if (Data4 < 0x10)
		Data4 = 0x10;

	Sprite_Map_Sound_Play( Data0 );
loc_19EE5:;
	pSprite->field_65 = -1;

	if (!pSprite->field_38)
		goto loc_1A149;

	if (pSprite->field_18 == 0x6B)
		if (mHelicopterCall_X >= 0)
			goto loc_1A042;
	
	if (pSprite->field_26 == 0x7171)
		goto loc_19F50;

	pSprite->field_26 = 0x7171;
	pSprite->field_1A = (int32*) 0x10000;

	Data0 = tool_RandomGet() & 0x0F;
	Data0 += 0x13;

	pSprite->field_2A = Data0;
loc_19F50:;
	dword_3B213[0x0E] = INVALID_SPRITE_PTR;

	Data2C = 0;
	sub_23444( pSprite, Data2C );
	Sprite_Movement_Calculate( pSprite );

	pSprite->field_1A = (int32*) (((int64)pSprite->field_1A ) + 0x2000);
	Data0 = ((int64)pSprite->field_1A) >> 16;
	pSprite->field_A += Data0;
	pSprite->field_A &= 0x0F;
	pSprite->field_2A -= 1;
	if (pSprite->field_2A < 0)
		goto loc_19FBC;

	Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	Field_1E -= 0x18000;
	pSprite->field_1E = Field_1E & 0xFFFF;
	pSprite->field_20 = Field_1E >> 16;
	if (Field_1E >= 0)
		goto loc_1A404;

	pSprite->field_1E = 0;
	pSprite->field_20 = 0;

loc_19FBC:;
	pSprite->field_18 = eSprite_Explosion;
	pSprite->field_26 = 0x1F50;
	pSprite->field_28 = -9;
	(pSprite + 1)->field_18 = 0x35;

	Data0 = tool_RandomGet() & 0x1FE;
	(pSprite + 1)->field_10 = Data0;
	(pSprite + 1)->field_36 = 0x60;

	Sprite_Destroy( pSprite + 2 );

	if (!pSprite->field_22)
		sub_23EA6(pSprite);

	if (word_3B175)
		Sprite_Destroy( pSprite + 3 );

	return;

loc_1A042:;
	if (pSprite->field_75 != 0x71) {
		pSprite->field_75 = 0x71;
		Data0 = pSprite->field_A;
		pSprite->field_74 = Data0;
	}

	pSprite->field_A += 1;
	pSprite->field_A &= 0x0F;

	Data0 = pSprite->field_A;
	if (Data0 == pSprite->field_74) {
		pSprite->field_75 = 0;
		pSprite->field_38 = 0;
	}

	Data24 = pSprite + 1;

	Data24->field_8 = 0x8C;
	Data24->field_0 = pSprite->field_0;
	Data24->field_4 = pSprite->field_4;
	Data24->field_4 += 1;
	Data24->field_20 = pSprite->field_20 + 0x0E;

	if (!word_3B4ED[0])
		if (!pSprite->field_1E && !pSprite->field_20)
			return;
	
	if (pSprite->field_20 < 0x0C)
		if (word_390B0 & 1)
			return;

	Data24->field_A += 1;
	Data24->field_A &= 3;
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
	dword_3B213[0x0E] = (sSprite*) Data0;

	if (Data0 >= 0x60)
		goto loc_1A217;

	Data0 >>= 1;

	if (Data0 > pSprite->field_36)
		goto loc_1A217;

	if (pSprite->field_22)
		goto loc_1A205;

	pSprite->field_36 = Data0;
	if (!pSprite->field_6E)
		goto loc_1A316;

	Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	Field_1E -= 0xC000;
	pSprite->field_1E = Field_1E & 0xFFFF;
	pSprite->field_20 = Field_1E >> 16;
	if (Field_1E < 0) {
		pSprite->field_1E = 0;
		pSprite->field_20 = 0;
	}
	sub_23D57( pSprite );
	goto loc_1A316;

loc_1A205:;
	pSprite->field_36 = Data0;
	if (!Data0)
		goto loc_1A287;
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
	if (dword_3B213[0x0E] >= (sSprite*)0x60)
		goto loc_1A316;

	if (!pSprite->field_6E)
		goto loc_1A316;

	Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	Field_1E -= 0xC000;
	pSprite->field_1E = Field_1E & 0xFFFF;
	pSprite->field_20 = Field_1E >> 16;
	if (Field_1E < 0) {
		pSprite->field_1E = 0;
		pSprite->field_20 = 0;
	}
	goto loc_1A316;

loc_1A287:;
	Data8 = pSprite->field_0;
	DataC = pSprite->field_4;

	if (Map_Terrain_Get_Moveable_Wrapper( mTiles_NotFlyable, Data8, DataC, Data10, Data14 ))
		goto loc_1A316;
	
	Data8 = pSprite->field_0;
	Data8 -= 0x10;
	DataC = pSprite->field_4;

	if (Map_Terrain_Get_Moveable_Wrapper( mTiles_NotFlyable, Data8, DataC, Data10, Data14 ))
		goto loc_1A316;

	Data8 = pSprite->field_0;
	Data8 += 0x10;
	DataC = pSprite->field_4;

	if (Map_Terrain_Get_Moveable_Wrapper( mTiles_NotFlyable, Data8, DataC, Data10, Data14 ))
		goto loc_1A316;

	if (pSprite->field_20) {
		Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
		Field_1E -= 0x8000;
		pSprite->field_1E = Field_1E & 0xFFFF;
		pSprite->field_20 = Field_1E >> 16;
	}

loc_1A316:;
	Data0 = pSprite->field_26;
	if (Data0 < 0)
		goto loc_1A345;

	Data4 = pSprite->field_28;
	if (Data4 < 0)
		goto loc_1A345;

	if (!pSprite->field_20)
		goto loc_1A345;

	Sprite_Movement_Calculate( pSprite );

loc_1A345:;

	if (pSprite->field_22)
		goto loc_1A3AB;

	if (pSprite->field_36)
		goto loc_1A3AB;

	if (pSprite->field_20 < 0x20)
		goto loc_1A3AB;

	Data0 = mMouseX + (mCamera_Adjust_Col >> 16);
	Data0 -= 0x10;

	Data4 = mMouseY + (mCamera_Adjust_Row >> 16);
	Data4 += 0x20;
	pSprite->field_36 = 0x1E;
	sub_23E01( pSprite, Data0, Data4 );
	pSprite->field_36 = 0;
	goto loc_1A3D2;

loc_1A3AB:;
	word_3B2F7 = 0x3A;
	Data0 = pSprite->field_26;
	Data4 = pSprite->field_28;
	sub_22CD7( pSprite, Data0, Data4 );
	goto loc_1A404;

loc_1A3D2:;

	Data8 = pSprite->field_3C;
	if (pSprite->field_22) {
		if (!pSprite->field_36)
			goto loc_1A404;
	}
	pSprite->field_A = Data8;

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
		if (!((pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16)))
			goto loc_1A49C;

	if (pSprite->field_20 < 0x0C)
		if (word_390B0 & 1)
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
	Data24->field_2C = -1;
	Data0 = pSprite->field_20;
	Data0 >>= 1;
	Data4 = Data0;
	Data0 += 9;

	Data0 += pSprite->field_0;
	Data24->field_0 = Data0;
	Data4 -= 1;
	Data4 += pSprite->field_4;
	Data24->field_4 = Data4;

	if (pSprite->field_22)
		goto loc_1A5A7;

	if (!pSprite->field_20)
		goto loc_1A5A7;

	Data8 = pSprite->field_0;
	DataC = pSprite->field_0;
	DataC += 0x1E;
	Data10 = pSprite->field_4;
	Data10 -= 0x14;
	Data14 = pSprite->field_4;
	Data18 = pSprite->field_20;
	Data1C = Data18;
	Data1C += 0x0E;

	Sprite_Under_Vehicle( pSprite, Data8, DataC, Data10, Data14, Data18, Data1C );

loc_1A5A7:;

	pSprite->field_22 = 0;
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
		sub_21702( pSprite, Data18 );
		Data18 += 0x100;
		Data18 &= 0x1FE;
		sub_21702( pSprite, Data18 );

		word_3A3AB = -1;
		word_3A3AD = 0;
		word_3A3AF = 0;
		word_3A3A1 = 9;
		word_3A3A7 = -1;
		word_3A3A9 = -1;
	}
	//loc_1AAFD
	pSprite->field_0 -= 6;
	if (pSprite->field_0 < 0) {
		sub_22A3D( pSprite );
		return;
	}

	//loc_1AB0D
	pSprite->field_4 += 4;
	if (pSprite->field_4 < 0) {
		sub_22A3D( pSprite );
		return;
	}

	int16 Data0 = pSprite->field_26;
	if (Data0 < 0) {
		Data0 &= 0x7FFF;
		pSprite->field_0 += Data0;
		if (pSprite->field_0 < 0) {
			sub_22A3D( pSprite );
			return;
		}
		pSprite->field_26 = 0;
	}
	//loc_1AB6F
	int16 Data4 = word_390AE;
	Data4 &= 3;
	Data4 += 5;

	Sound_Play( pSprite, Data4, 0x1E );
	pSprite->field_8 = 0x8E;
	pSprite->field_A = 0;
	pSprite->field_12 = 1;
	pSprite->field_52 = 0;
	pSprite->field_22 = -1;
	pSprite->field_32 = -1;
	pSprite->field_2C = -1;
}

void cFodder::Sprite_Handle_Shrub( sSprite* pSprite ) {
	pSprite->field_8 = 0x8F;
	Sprite_Handle_Explodable( pSprite );
}

void cFodder::Sprite_Handle_Tree( sSprite* pSprite ) {
	pSprite->field_8 = 0x90;
	Sprite_Handle_Explodable( pSprite );
}

void cFodder::Sprite_Handle_BuildingRoof( sSprite* pSprite ) {
	pSprite->field_8 = 0x91;
	Sprite_Handle_Explodable( pSprite );
}

void cFodder::sub_1AC03( sSprite* pSprite ) {
	pSprite->field_8 = 0x92;
	Sprite_Handle_Explodable( pSprite );
}

void cFodder::Sprite_Handle_Shrub2( sSprite* pSprite ) {
	pSprite->field_8 = 0x93;
	Sprite_Handle_Explodable( pSprite );
}

void cFodder::Sprite_Handle_Waterfall( sSprite* pSprite ) {
	pSprite->field_8 = 0x94;
	pSprite->field_2C = -1;

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
	pSprite->field_2C = 1;
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
	pSprite->field_2C = -1;
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
	Sound_Play( pSprite, 0x10, 0x01 );
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
	pSprite->field_43 = Data0;
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

		if (word_3A9C0[Data4])
			goto loc_1AF63;
	}
	//loc_1AE1D
	if (Data24->field_18 || Data24->field_5B)
		goto loc_1AE64;

	if (!Data24->field_38)
		goto loc_1AE76;

	if (Data24->field_38 >= 0x32)
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

	Data0 = ((sSquad_Member*) Data24->field_46)->mRank;

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

void cFodder::sub_1AF78( sSprite* pSprite ) {
	sSprite* Data28 = (sSprite*) pSprite->field_1A;
	int16 Data0;

	if (Data28 != pSprite->field_6A)
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

void cFodder::sub_1B07C( sSprite* pSprite ) {
	
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

void cFodder::sub_1B0C0( sSprite* pSprite ) {
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

	pSprite->field_2C = -1;
	if (pSprite->field_43 < 0) {
		//loc_1B161
		pSprite->field_12 -= 1;
		if (pSprite->field_12 >= 0)
			return;

		int16 Data0 = tool_RandomGet() & 0x0F;
		Data0 += 6;
		pSprite->field_43 = Data0;
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
	Data4 -= mSprite_Enemy_Aggression_HalfOfMinPlusMax;
	Data0 += 0x0A;
	Data4 += mSprite_Enemy_Aggression_HalfOfMinPlusMax;

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

	pSprite->field_2C = -1;
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

	Sound_Play( pSprite, 0x10, 1 );
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
	pSprite->field_43 = Data0;
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
	if (Data4 < 4 || Data4 > 6) {
		pSprite->field_0 = dword_3A391 & 0xFFFF;
		pSprite->field_4 = dword_3A395 & 0xFFFF;
		pSprite->field_36 = 0;
		Data0 = pSprite->field_12;

		pSprite->field_10 += Data0;
		pSprite->field_10 &= 0x1FE;
	}

	//loc_1B3CA
	if (pSprite->field_36 < 4)
		pSprite->field_36++;

	pSprite->field_2C = 0;
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

void cFodder::sub_1B4BB( sSprite* pSprite ) {
	int16 Data0 = 2;
	int16 Data4 = pSprite->field_36;
	int16 Data8, DataC;

	if (!Data4) {
		if (!pSprite->field_22) {

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
	if (sub_2329D(pSprite))
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
		dword_3B213[0x0E] = INVALID_SPRITE_PTR;
		sSprite* Data2C = 0;

		if (!sub_23444( pSprite, Data2C))
			Data2C->field_2C = -1;
	}

	if (!pSprite->field_36) {
		if (word_3A8CF)
			goto loc_1B655;
	}

	if (pSprite->field_52 >= 4)
		goto loc_1B655;

	pSprite->field_36 += 3;
	if (pSprite->field_18 != 0x52)
		goto loc_1B640;

	pSprite->field_36 = 0x14;
	/*
	seg004:28E2 7C 21                          jl      short loc_1B655
	seg004:28E4 C4 36 20 00                    les     si, ds:20h
	seg004:28E8 26 C7 44 36 14+                mov     word ptr es:[si+36h], 14h
	*/
	goto loc_1B655;
loc_1B640:;
	if (pSprite->field_36 >= 0x32)
		pSprite->field_36 = 0x32;

loc_1B655:;
	if (pSprite->field_20 > 4)
		pSprite->field_36 += 0x1C;

	Sprite_XY_Store( pSprite );
	Sprite_Movement_Calculate( pSprite );

	if (pSprite->field_20 < 9)
		goto loc_1B688;
	pSprite->field_36 += 0x40;
	goto loc_1B6A2;

loc_1B688:;
	sub_22DFC( pSprite );

	if (pSprite->field_20 <= 4)
		pSprite->field_20 = 0;

loc_1B6A2:;

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
				Sound_Play( pSprite, 0x2E, 0x0F );

		}
		Data24 = (sSprite*)pSprite->field_46;
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
	sub_2A1F0( pSprite, Data0, Data4 );
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
	sub_229C9( pSprite );
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
	sSprite* Data28 = (sSprite*) pSprite->field_1A;

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
	
	if (sub_1E05A( pSprite ))
		return;

	if (pSprite->field_38)
		return;

	word_3AA41 = 0;
	pSprite->field_43 = -1;

	sub_21CD1( pSprite );

	mTroop_Cannot_Fire_Bullet = -1;
	mTroop_Cannot_Throw_Grenade = -1;
	word_3A010 = -1;

	Sprite_Handle_Troop( pSprite );
	sub_2A3D4( pSprite );

	word_3AA41 = 0;
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
		mSquad_Leader->field_75 |= 1;

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
	
	DataC = pSprite->field_0;
	DataC += 0x24;

	Data10 = pSprite->field_4;
	Data10 -= 0x20;
	Data14 = pSprite->field_4;
	Data14 -= 6;

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
	DataC = pSprite->field_0;
	DataC += 0x1E;
	Data10 = pSprite->field_4;
	Data10 += -30;
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

	pSprite->field_6F = 6;
	Sprite_Handle_Helicopter_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Flashing_Light( sSprite* pSprite ) {
	
	pSprite->field_2C = 0;
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

	pSprite->field_6F = 5;
	Sprite_Handle_Helicopter_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Missile_Enemy( sSprite* pSprite ) {
	
	pSprite->field_6F = 7;
	Sprite_Handle_Helicopter_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Homing_Enemy( sSprite* pSprite ) {
	
	pSprite->field_6F = 8;
	Sprite_Handle_Helicopter_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Missile( sSprite* pSprite ) {
	
	dword_3B213[0x0E] = INVALID_SPRITE_PTR;
	sub_2183B( pSprite );

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
		int32 tmp = (pSprite->field_1E & 0xFFFF) | pSprite->field_20 << 16;
		tmp -= 0xA000;

		pSprite->field_1E = tmp & 0xFFFF;
		pSprite->field_20 = tmp >> 16;
	}

	Data0 = pSprite->field_26;
	Data4 = pSprite->field_28;

	sub_2A1F0( pSprite, Data0, Data4 );
	Sprite_Movement_Calculate( pSprite );

	if (word_3B173)
		goto loc_1BECD;

	Data0 = pSprite->field_10;

	Data0 >>= 5;
	Data0 -= 1;
	Data0 ^= 0x0F;
	Data0 &= 0x0F;
	pSprite->field_A = Data0;

	if (pSprite->field_36 < 0x60)
		pSprite->field_36 += pSprite->field_3A;

	Data0 = word_390B0;
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
	sub_229C9( pSprite );

	Sprite_Destroy( pSprite + 1 );
}

void cFodder::Sprite_Handle_MissileHoming( sSprite* pSprite ) {
	int16 Data0, Data4, Data8, DataC;
	int32 Field_1E;
	sSprite* Data34 = 0;

	if (pSprite->field_38)
		goto loc_1C197;

	dword_3B213[0x0E] = INVALID_SPRITE_PTR;
	sub_2183B( pSprite );

	pSprite->field_8 = 0xA3;
	Data34 = (sSprite*) pSprite->field_1A;
	Data0 = Data34->field_0;
	if (Data0 == -32768)
		goto loc_1C170;

	Data0 += 8;
	Data4 = Data34->field_4 + 8;
	Data8 = pSprite->field_0;
	DataC = pSprite->field_4;
	Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
	Data4 = pSprite->field_36;
	Data4 >>= 4;
	Data4 += 1;

	if (Data0 <= Data4)
		goto loc_1C197;

	if (Data34->field_20 <= 0x10)
		goto loc_1BFE0;

	Data4 = pSprite->field_20;
	if (Data4 == Data34->field_20)
		goto loc_1C087;
	if (Data4 > Data34->field_20)
		goto loc_1BFD0;

	Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	Field_1E += 0x8000;
	pSprite->field_1E = Field_1E & 0xFFFF;
	pSprite->field_20 = Field_1E >> 16;

	goto loc_1C087;
loc_1BFD0:;

	Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	Field_1E -= 0x8000;
	pSprite->field_1E = Field_1E & 0xFFFF;
	pSprite->field_20 = Field_1E >> 16;
	goto loc_1C087;

loc_1BFE0:;
	if (Data0 > 0x30)
		goto loc_1C012;

	Data4 = Data34->field_20;
	Data4 -= pSprite->field_20;
	Data4 >>= 3;
	pSprite->field_20 += Data4;
	goto loc_1C087;

loc_1C012:;

	Data0 = pSprite->field_0;
	Data4 = pSprite->field_4;
	Data8 = Data34->field_0;
	DataC = Data34->field_4;
	Data8 += 8;
	DataC += 8;

	if (!sub_2A4A2( Data0, Data4, Data8, DataC ))
		goto loc_1C06F;

	if (pSprite->field_20 >= 0x18)
		goto loc_1C087;

	Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	Field_1E += 0x28000;
	pSprite->field_1E = Field_1E & 0xFFFF;
	pSprite->field_20 = Field_1E >> 16;
	goto loc_1C087;

loc_1C06F:;
	if (pSprite->field_20 > 8) {
		Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
		Field_1E -= 0x12000;
		pSprite->field_1E = Field_1E & 0xFFFF;
		pSprite->field_20 = Field_1E >> 16;
	}
loc_1C087:;

	Data0 = Data34->field_0;
	Data0 += 8;
	Data4 = Data34->field_4;
	Data4 += 8;
	sub_2A1F0( pSprite, Data0, Data4 );
	Sprite_Movement_Calculate( pSprite );

	Data4 = pSprite->field_A;
	Data0 = pSprite->field_10;
	Data0 >>= 5;
	Data0 -= 1;
	Data0 ^= 0x0F;
	Data0 &= 0x0F;
	pSprite->field_A = Data0;

	if (!pSprite->field_22)
		goto loc_1C133;

	if (Data4 == Data0)
		goto loc_1C133;

	if (pSprite->field_36 <= 0x1E)
		goto loc_1C131;

	pSprite->field_6A = 0;
	pSprite->field_36 -= 2;
	if (pSprite->field_36 < 0)
		pSprite->field_36 = 0;
loc_1C131:;
	return;

loc_1C133:;
	if (pSprite->field_36 <= 0x3C)
		goto loc_1C14A;

	pSprite->field_36 = 0x3C;
	return;

loc_1C14A:;
	Data0 = ((int64)pSprite->field_6A) >> 16;
	pSprite->field_36 += Data0;
	pSprite->field_6A = (sSprite*) (((int64)pSprite->field_6A) + 0x200);
	return;

loc_1C170:;
	pSprite->field_36 += 2;
	pSprite->field_20 -= 2;
	if (pSprite->field_20 >= 0)
		goto loc_1C190;

	pSprite->field_20 = 0;
	goto loc_1C197;

loc_1C190:;
	Sprite_Movement_Calculate( pSprite );
	return;

loc_1C197:;
	pSprite->field_18 = eSprite_Explosion2;
	pSprite->field_4 -= 4;
	sub_229C9( pSprite );
	Sprite_Destroy( pSprite + 1 );

}

void cFodder::sub_1C1C0( sSprite* pSprite ) {
	Sprite_Movement_Calculate( pSprite );
	int64 Data0 = (int64) pSprite->field_1A;

	Data0 += pSprite->field_1E;

	pSprite->field_1E = Data0;
	pSprite->field_20 = Data0 >> 16;

	if (Data0 < 0) {
		pSprite->field_1E = 0;
		pSprite->field_20 = 0;
		Data0 = -Data0;
		Data0 >>= 1;
	}

	Data0 -= 0x22000;
	pSprite->field_1A = (int32*) Data0;

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
	sub_2183B( pSprite );
}

void cFodder::sub_1C268( sSprite* pSprite ) {
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

	pSprite->field_6F = 5;
	Sprite_Handle_Helicopter_Human( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Grenade2_Human( sSprite* pSprite ) {

	pSprite->field_6F = 6;
	Sprite_Handle_Helicopter_Human( pSprite );
}


void cFodder::Sprite_Handle_Helicopter_Missile_Human( sSprite* pSprite ) {

	pSprite->field_6F = 7;
	Sprite_Handle_Helicopter_Human( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_Homing_Human( sSprite* pSprite ) {

	pSprite->field_6F = 8;
	Sprite_Handle_Helicopter_Human( pSprite );
}

void cFodder::sub_1C2D2( sSprite* pSprite ) {
	pSprite->field_A += 1;
	pSprite->field_A &= 3;
	int32 Field_1E;

	Sprite_Movement_Calculate( pSprite );
	if (word_3B173)
		goto loc_1C321;

	pSprite->field_36 -= 1;
	if (pSprite->field_36 < 0)
		pSprite->field_36 = 0;

	Field_1E = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);
	Field_1E -= 0x8000;
	pSprite->field_1E = Field_1E & 0xFFFF;
	pSprite->field_20 = Field_1E >> 16;
	if (Field_1E < 0) {
		pSprite->field_1E = 0;
		pSprite->field_20 = 0;
	}

loc_1C321:;
	pSprite->field_18 = eSprite_Explosion;
	sub_2183B( pSprite );
}

void cFodder::Sprite_Handle_Mine( sSprite* pSprite ) {
	pSprite->field_2C = -1;

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

	pSprite->field_2C = -1;
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

	mSquad_Leader->field_38 = 0x32;
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
	Data28->field_38 = 9;
	Data28->field_64 = -1;
	Data28->field_20 = 7;

loc_1C4AD:;
	if (pSprite->field_A >= 2)
		return;

	pSprite->field_A += 1;
}

void cFodder::sub_1C4C2( sSprite* pSprite ) {
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

	pSprite->field_22 = 2;
	pSprite->field_8 = 0xD0;

	int16 ax = sub_25680( pSprite );

	if (ax) {
		if (ax >= 0)
			return;

		sub_258C6( pSprite );
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

	pSprite->field_22 = 2;
	pSprite->field_8 = 0xD0;

	int16 ax = sub_25680( pSprite );
	if (ax > 0)
		return;
	if (ax < 0)
		sub_258C6( pSprite );

	pSprite->field_36 = 0x0A;
	sub_2593D( pSprite );
	sub_25A31( pSprite );
	sub_25A66( pSprite );
	if (!word_3B2D1[1])
		return;

	sub_25863( pSprite );
}

void cFodder::Sprite_Handle_VehicleNoGun_Human( sSprite* pSprite ) {

	pSprite->field_6F = 2;
	Sprite_Handle_Vehicle_Human( pSprite );
}

void cFodder::Sprite_Handle_VehicleGun_Human( sSprite* pSprite ) {

	pSprite->field_6F = 3;
	Sprite_Handle_Vehicle_Human( pSprite );
}

void cFodder::Sprite_Handle_Tank_Human( sSprite* pSprite ) {
	
	if (pSprite->field_38 == 5)
		pSprite->field_38 = 0;

	if (pSprite->field_38) {
		pSprite->field_22 = 0;

		sub_23525( pSprite );
		pSprite->field_22 = 0;
		return;
	}
	pSprite->field_22 = 0;
	sub_23525( pSprite );
	pSprite->field_22 = 0;
	sub_23879( pSprite );

	sSprite* Data24 = pSprite + 1;
	Data24->field_18 = eSprite_Null;
}

void cFodder::Sprite_Handle_Bird_Left( sSprite* pSprite ) {
	
	pSprite->field_8 = 0xD3;
	pSprite->field_2C = 1;

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
		sub_14D6D(pSprite, 0x1A);

	if (mMap_TileSet == eTileTypes_Ice || mMap_TileSet == eTileTypes_AFX)
		sub_14D6D(pSprite, 0x1F);

}

void cFodder::Sprite_Handle_Bird_Right( sSprite* pSprite ) {
	pSprite->field_8 = 0xD4;
	pSprite->field_2C = 1;

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
		sub_14D6D(pSprite, 0x1A);

	if (mMap_TileSet == eTileTypes_Ice || mMap_TileSet == eTileTypes_AFX)
		sub_14D6D(pSprite, 0x1F);
}

void cFodder::Sprite_Handle_Seal( sSprite* pSprite ) {
	if (sub_25DCF( pSprite ))
		return;

	pSprite->field_22 = 2;
	pSprite->field_8 = 0xD5;

	if (!pSprite->field_2E) {
		pSprite->field_2E = -1;
		pSprite->field_54 = tool_RandomGet() & 0x0F;
	}

	if (word_390B0 & 1)
		++pSprite->field_54;

	int16 Data0 = pSprite->field_54 & 0x0F;

	Data0 = byte_3E916[Data0];
	pSprite->field_A = Data0;

	if (mMap_TileSet == eTileTypes_Moors) {
		Data0 = tool_RandomGet() & 3;
		if (Data0 != 3) {
			Data0 += 0x23;
			sub_14D6D( pSprite, Data0 );
		}
	}

	if (mMap_TileSet == eTileTypes_Ice || mMap_TileSet == eTileTypes_AFX) {
		sub_14D6D( pSprite, 0x1E );
	}
}

void cFodder::Sprite_Handle_Tank_Enemy( sSprite* pSprite ) {
	int16 Data0, Data4, Data8, DataC, Data10, Data14;
	sSprite* Data24 = 0, *Data30 = 0;

	if (pSprite->field_38 == 5)
		pSprite->field_38 = 0;

	if (pSprite->field_38) {
		pSprite->field_22 = 1;
		sub_23525( pSprite );
		pSprite->field_22 = 1;

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

	word_3B179 = Data0;
	word_3B17B = Data4;
	word_3B17D = Data8;
	word_3B17F = DataC;

	Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
	word_3B17F = Data0;
	if (Data0 >= 0xFA)
		goto loc_1CD7B;

	Data0 = word_3B179;
	Data4 = word_3B17B;
	Data8 = word_3B17D;
	DataC = word_3B17F;
	Data8 += 0x0F;
	DataC -= 0x0A;

	if (sub_2A4A2( Data0, Data4, Data8, DataC ))
		goto loc_1CD7B;
	
	pSprite->field_2E = Data30->field_0;
	pSprite->field_30 = Data30->field_4;
	pSprite->field_4C = 0x5A;

	if (pSprite->field_0 != pSprite->field_26)
		goto loc_1CD53;

	if (pSprite->field_4 != pSprite->field_28)
		goto loc_1CD53;

	Data0 = tool_RandomGet() & 0x3F;
	if (!Data0) {
		pSprite->field_26 = Data30->field_0;
		pSprite->field_28 = Data30->field_4;
	}

loc_1CD53:;
	if (word_3B177 <= 0x32)
		goto loc_1CDA3;

	Data0 = tool_RandomGet() & 0x1F;
	if (Data0 != 4)
		goto loc_1CDA3;

	pSprite->field_54 = -1;
	sub_23879( pSprite );
	goto loc_1CDA3;

loc_1CD7B:;
	pSprite->field_5E += 1;
	if (pSprite->field_5E >= 0x1E) {
		pSprite->field_5E = 0;
		pSprite->field_2E = -1;
	}

loc_1CDA3:;
	word_3B175 = -1;
	pSprite->field_22 = 1;
	sub_23525( pSprite );
	pSprite->field_22 = 1;
	word_3B175 = 0;

	if (word_3ABAD) {
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
	pSprite->field_22 = 1;
	if (pSprite->field_38) {
		Sprite_Handle_Indigenous_Death( pSprite );
		return;
	}

	pSprite->field_22 = 2;
	pSprite->field_8 = 0xD0;

	if (mMap_TileSet == eTileTypes_Moors)
		sub_14D6D( pSprite, 0x26 );
	
	if (mMap_TileSet == eTileTypes_Int)
		sub_14D6D( pSprite, 0x1F );

	int16 ax = sub_25680( pSprite );
	if (ax > 0)
		return;
	if (ax)
		sub_258C6( pSprite );

	pSprite->field_36 = 0x0C;

	sub_2593D( pSprite );
	sub_25A31( pSprite );
	sub_25A66( pSprite );
	sub_25863( pSprite );
}

void cFodder::sub_1CE80( sSprite* pSprite ) {
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

	sub_2A1F0( pSprite, Data0, Data4 );
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
	if (word_3B173)
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

	pSprite->field_2C = 0;
	pSprite->field_0 = pSprite->field_2E;
	pSprite->field_4 = pSprite->field_30;
	pSprite->field_44 = -1;
	goto loc_1D00F;
loc_1CFF7:;
	pSprite->field_2C = -1;
	Sprite_Movement_Calculate( pSprite );
	if (word_3B173)
		goto loc_1D17A; 

loc_1D00F:;
	if (pSprite->field_64 > 2) {
		Data0 = -9;
		Data4 = 0;
		if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
			goto loc_1D0F6;
	}

	if (sub_21618( pSprite ))
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
	pSprite->field_2C = 1;
	pSprite->field_4 -= 5;
	if (pSprite->field_4 < 0)
		goto loc_1D18D;
	pSprite->field_8 = 0x96;
	pSprite->field_A = 0;

	Sound_Play( pSprite, 0x2A, 0x0F );
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
	pSprite->field_2C = -1;
	pSprite->field_4 -= 5;
	if (pSprite->field_4 < 0)
		goto loc_1D18D;

	pSprite->field_A = 3;
	Sound_Play( pSprite, 0x2A, 0x0F );
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

	pSprite->field_22 = 2;
	pSprite->field_8 = 0xD9;

	word_3B2D1[2] = pSprite->field_0;
	word_3B2D1[3] = pSprite->field_2;
	word_3B2D1[4] = pSprite->field_4;
	word_3B2D1[5] = pSprite->field_6;

	sub_25FDA( pSprite );
	sub_258C6( pSprite );

	pSprite->field_36 = 0x0C;
	sub_2593D( pSprite );
	pSprite->field_38 = 0;

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
	if (Data0 == dword_3A391) {
		Data0 = pSprite->field_4;
		if (Data0 == dword_3A395)
			return;
	}
	pSprite->field_8 = 0xD8;
	return;

loc_1D349:;
	pSprite->field_38 = 0;
	sSprite* Data28 = &mSprites[pSprite->field_5E];
	if (Data28->field_6E)
		goto loc_1D44C;

	pSprite->field_0 = Data28->field_0;
	pSprite->field_4 = Data28->field_4;
	pSprite->field_0 += 8;
	pSprite->field_4 += 0x0A;
	dword_3A395 = pSprite->field_4;

loc_1D3C6:;
	Data0 = -3;
	Data4 = 8;
	if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
		goto loc_1D411;

	pSprite->field_4 -= 1;
	if (pSprite->field_4 >= 0)
		goto loc_1D3C6;

	pSprite->field_4 = dword_3A395;
	pSprite->field_0 -= 1;
	if (pSprite->field_0 >= 0)
		goto loc_1D3C6;

	pSprite->field_0 = 0;
	pSprite->field_38 = 5;

loc_1D411:;
	Data4 = pSprite->field_4;
	Data0 = mMapHeight_Shifted;

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

	dword_3B5F5 = pSprite;
	if (pSprite->field_38 == 7)
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

void cFodder::sub_1D4D2( sSprite* pSprite ) {
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

	if (word_3B173)
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

	pSprite->field_2C = 0;
	pSprite->field_0 = pSprite->field_2E;
	pSprite->field_4 = pSprite->field_30;
	pSprite->field_44 = -1;
	goto loc_1D5CB;
loc_1D5B3:;
	pSprite->field_2C = -1;
	Sprite_Movement_Calculate( pSprite );

	if (word_3B173)
		goto loc_1D6CA;

loc_1D5CB:;
	if (pSprite->field_64 > 3) {
		Data0 = -9;
		Data4 = 0;
		if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
			goto loc_1D65C;
	}

	if (!sub_21618( pSprite ))
		return;

	if (mSprites_Found_Count == 1)
		goto loc_1D65C;

	pSprite->field_43 = 1;
	pSprite->field_2C = 1;
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

	sub_21618( pSprite );
	pSprite->field_43 = -1;
	pSprite->field_2C = -1;
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
	pSprite->field_6F = 2;
	Sprite_Handle_Vehicle_Enemy( pSprite );
}

void cFodder::Sprite_Handle_VehicleGun_Enemy( sSprite* pSprite ) {
	pSprite->field_6F = 3;
	Sprite_Handle_Vehicle_Enemy( pSprite );
}

void cFodder::Sprite_Handle_Vehicle_Unk_Enemy( sSprite* pSprite ) {
	pSprite->field_6F = 0x0A;
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
	pSprite->field_22 = 1;
	pSprite->field_6F = 0;

	Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Turret_Missile2_Enemy( sSprite* pSprite ) {
	pSprite->field_22 = 1;
	pSprite->field_6F = 1;

	Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Turret_HomingMissile_Enemy( sSprite* pSprite ) {
	pSprite->field_22 = 1;
	pSprite->field_6F = 9;

	Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Turret_Missile_Human( sSprite* pSprite ) {
	pSprite->field_22 = 0;
	pSprite->field_6F = 0;

	Sprite_Handle_Turret( pSprite );
}

void cFodder::Sprite_Handle_Turret_Missile2_Human( sSprite* pSprite ) {
	pSprite->field_22 = 0;
	pSprite->field_6F = 1;

	Sprite_Handle_Turret( pSprite );
}

void cFodder::sub_1D7DD( sSprite* pSprite ) {
	Sound_Play( pSprite, 0x2B, 0x0F );
	pSprite->field_A -= 1;

	if (pSprite->field_A < 0)
		Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::sub_1D802( sSprite* pSprite ) {
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

	pSprite->field_2C = -1;
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
	Sound_Play( pSprite, 0x10, 1 );

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
	pSprite->field_43 = Data0;
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

void cFodder::sub_1DA48( sSprite* pSprite ) {
	dword_3B477 = pSprite;

	if (sub_222A3( pSprite )) {
		dword_3B477 = 0;
		mMission_Aborted = -1;
		return;
	}
	pSprite->field_2C = -1;
	if (!pSprite->field_2A) {
		pSprite->field_8 = 0x9B;
		return;
	}

	pSprite->field_2A -= 1;
	if (!pSprite->field_2A) 
		Sound_Play( pSprite, 0x29, 1 );
	
	if (pSprite->field_8 == 0x9B)
		Sound_Play( pSprite, 0x29, 1 );

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

	pSprite->field_38 = 9;
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

	sSquad_Member* Data24 = (sSquad_Member*) mSquad_Leader->field_46;
	Data24->mRank = 0x0F;
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

	mSquad_Leader->field_75 |= 1;
	mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
	mSquad_Rockets[mSquad_Selected] = 50;

	Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Player_Rocket( sSprite* pSprite ) {
	if (pSprite->field_38) {
		pSprite->field_18 = eSprite_Explosion;
		return;
	}

	pSprite->field_8 = 0x3E;
	pSprite->field_A = 0;
	int16 Data0;
	if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
		 return;
	
	mSquad_Leader->field_75 |= 2;
}

void cFodder::Sprite_Handle_Bonus_RocketsAndGeneral( sSprite* pSprite ) {
	if (pSprite->field_38) {
		pSprite->field_18 = eSprite_Explosion;
		return;
	}

	int16 Data0 = 0;
	pSprite->field_8 = 0xE5;
	pSprite->field_A = 0;
	if (Map_Get_Distance_Between_Sprite_And_Squadleader( pSprite, Data0 ))
		return;

	mSquad_Leader->field_75 |= 3;
	sSquad_Member* Member = (sSquad_Member*) mSquad_Leader->field_46;
	Member->mRank = 0x0F;
	mGUI_Sidebar_Setup = 0;
	
	mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
	mSquad_Rockets[mSquad_Selected] = 50;

	Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_MissileHoming2( sSprite* pSprite ) {

	Sprite_Handle_MissileHoming( pSprite );
}

void cFodder::Sprite_Handle_Bonus_SquadGeneralRockets( sSprite* pSprite ) {

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
		Data2C->field_75 |= 3;

		sSquad_Member* Data24 = (sSquad_Member*) Data2C->field_46;
		Data24->mRank = 0x0F;
	}

	mGUI_Sidebar_Setup = 0;
	mGUI_RefreshSquadRockets[mSquad_Selected] = -1;
	mSquad_Rockets[mSquad_Selected] = 0x32;

	Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::Sprite_Handle_Helicopter_CallPad( sSprite* pSprite ) {
	sSprite* Data2C = 0;

	pSprite->field_8 = 0xE7;
	pSprite->field_2C = -1;

	if (Sprite_Handle_Helicopter_Callpad_InRange( pSprite, Data2C )) {
		pSprite->field_75 = 0;
	} else {
		++mHelicopterCallPadPressedCount;

		if (!pSprite->field_75) {
			dword_3B4CF = Data2C;
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

void cFodder::sub_1DFD2( sSprite* pSprite ) {
	if (pSprite->field_38 != 7) {
		Sprite_Handle_BuildingDoor3( pSprite );
		return;
	}

	if (pSprite->field_74 >= 0x69) {
		Sprite_Handle_BuildingDoor3( pSprite );
		return;
	}

	pSprite->field_74 += 1;
	pSprite->field_38 = 0;
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

	if (pSprite->field_38 == 7)
		goto loc_1D95B;

	if (pSprite->field_38 == 6)
		goto loc_1D9C9;

	if (pSprite->field_58)
		goto loc_1D9C9;

	return 0;

loc_1D95B:;
	pSprite->field_38 = 6;
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

	pSprite->field_44 = Data4;
	pSprite->field_36 = 0x78;
	pSprite->field_58 = -1;

loc_1D9C9:;
	pSprite->field_44 -= 1;
	if (!pSprite->field_44)
		goto loc_1DA13;

	Sprite_Movement_Calculate( pSprite );
	if (word_3B173)
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
	sub_218E2( pSprite );
	return -1;
}

void cFodder::sub_14D6D( sSprite* pSprite, int16 pData4 ) {
	word_3B4DD = pData4;

	if (mMission_Completed_Timer)
		return;

	int16 Data0 = tool_RandomGet() & 0x7F;
	if (Data0 != 0x0E)
		return;

	pData4 = word_3B4DD;
	int16 Data8 = 0;

	if (mMap_TileSet == eTileTypes_Int || mMap_TileSet == eTileTypes_Moors)
		Data8 = 0x7F;

	Sound_Play( pSprite, pData4, Data8 );
}

int16 cFodder::sub_1E05A( sSprite* pSprite ) {
	int64 Dataa0;
	int32 Dataa4;
	int16 Data0, Data4, Data8;
	int16* Data28;
	sSprite* Data20 =0, *Data2C = 0, *Data30 = 0;
	int16 Field_52;
	int16 Field_0;
	int16 Field_4;

	if (pSprite->field_38 == 3)
		goto loc_1E831;

	if (pSprite->field_38 == 6)
		goto loc_1EA48;

	if (pSprite->field_38 == 2)
		goto loc_1EB87;

	Data0 = pSprite->field_4;
	if (Data0 >= mMapHeight_Shifted)
		pSprite->field_4 = mMapHeight_Shifted;

	//loc_1E0A4
	if (pSprite->field_56)
		goto loc_1E831;

	if (pSprite->field_5B == 1)
		goto loc_1E74C;

	if (pSprite->field_38 == 0x0A)
		goto loc_1E2F4;

	if (pSprite->field_38 == 0x32)
		goto loc_1ECA6;

	if (pSprite->field_38 == 0x33)
		goto loc_1ED5B;

	if (pSprite->field_38 == 0x34)
		goto loc_1EE59;

	if (pSprite->field_38 == 0x5A)
		goto loc_1EEEC;

	if (pSprite->field_38 == 0x5B)
		goto loc_1EF28;

	//seg004:53BD
	if (!pSprite->field_38)
		goto loc_1EA3F;

	if (!pSprite->field_64)
		goto loc_1E3D2;

	pSprite->field_64 = 0;
	if (mMission_TryAgain)
		pSprite->field_40 = -26506;

	if (!pSprite->field_58) {
		sub_1FE35( pSprite );
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
	Data28 = off_32AE4[Data0];
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

		pSprite->field_1A = (int32*) Dataa0;
	}
	//loc_1E232
	pSprite->field_12 = 2;
	Data0 = tool_RandomGet();
	Data4 = 0;
	Data0 &= 7;
	Data4 = mSound_Indigenous_Death[Data0];
	//seg004:5508
	Data8 = 0x14;
	Sound_Play( pSprite, Data4, Data8 );

	if (pSprite->field_36 < 0x1E)
		pSprite->field_36 += 0x0F;

	pSprite->field_3E = pSprite->field_10;
	pSprite->field_3A = -1;
	Data0 = tool_RandomGet() & 0x01;
	if (!Data0)
		pSprite->field_3A = pSprite->field_10;

	sub_1FF1A(pSprite, Data2C, Data30 );
	pSprite->field_26 = -1;
	if (pSprite->field_38 != 9)
		goto loc_1E3D2;

	pSprite->field_36 = 0;
	pSprite->field_38 = 0x0A;
loc_1E2F4:;
	pSprite->field_3A -= 1;
	if (pSprite->field_3A < 0) {
		pSprite->field_38 = 1;
		return -1;
	}
	//loc_1E30C
	Data0 = 0x21;
	if (pSprite->field_22)
		Data0 = 0x63;

	pSprite->field_8 = Data0;
	Data0 = pSprite->field_2A & 7;
	//seg004:55F3 

	if (!Data0) {
		sSprite* Data2C = 0, *Data30 = 0;

		sub_1FF1A(pSprite, Data2C, Data30);

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

	Data4 = mSound_Indigenous_Death[Data0];
	Data8 = 0;
	Sound_Play( pSprite, Data4, Data8 );
	return -1;

loc_1E3D2:;

	if (pSprite->field_40 == -26506) {
		sSprite* Data2C = 0, *Data30 = 0;

		pSprite->field_28 += 1;
		sub_1FF1A(pSprite, Data2C, Data30);

		if (!(pSprite->field_28 & 7)) {
			Data0 = tool_RandomGet() & 7;
			Data4 = mSound_Indigenous_Death[Data0];
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

		Data28 = off_32AE4[pSprite->field_22];
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
		sub_1FFC6( pSprite, Data4 );
		pSprite->field_10 = Field10;
	}
	else {
		//loc_1E542
		int16 Field0 = pSprite->field_0;
		int16 Field4 = pSprite->field_4;
		int16 Field10 = pSprite->field_10;

		sub_1FFC6( pSprite, Data4 );

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
	Dataa0 = (int64) pSprite->field_1A;
	Dataa4 = Dataa0;
	Dataa0 += ((pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16));

	pSprite->field_1E = Dataa0 & 0xFFFF;
	pSprite->field_20 = Dataa0 >> 16;
	if (Dataa0 < 0) {
		pSprite->field_1E = 0;
		pSprite->field_20 = 0;
		Dataa4 = -Dataa4;
		Dataa4 >>= 2;

		if (pSprite->field_52) {
			pSprite->field_36 = 0;
			Dataa4 = 0;
		}
	}
	//loc_1E619
	Dataa4 -= 0x18000;
	pSprite->field_1A = (int32*) ((int64)Dataa4);
	if (pSprite->field_36) {
		pSprite->field_36 -= 2;

		if (pSprite->field_36 < 0)
			pSprite->field_36 = 0;
	}
	//loc_1E653
	if (pSprite->field_26) {

		if (!pSprite->field_20) {
			sSprite* Data2C = 0, *Data30 = 0;
			sub_1FF1A(pSprite, Data2C, Data30);
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
				pSprite->field_38 = 0x0A;
				mGUI_Sidebar_Setup = 0;
				return -1;
			}
		}
	}
	//loc_1E708
	pSprite->field_38 = 6;
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
		if (pSprite->field_22)
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

			pSprite->field_38 = 6;
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

	if (pSprite->field_38 == 3) {
		pSprite->field_52 += Data0;

		if (pSprite->field_52 < 0x0E)
			return -1;

		pSprite->field_52 = 0x0E;
		pSprite->field_12 = 0x14;
		Data0 = tool_RandomGet() & 0x07;
		Data4 = mSound_Indigenous_Death[Data0];
		Data8 = 0x0A;
		Sound_Play( pSprite, Data4, Data8 );
		goto loc_1E9EC;
	}
	//loc_1E8D6
	pSprite->field_4 += Data0;
	Data0 = pSprite->field_4;
	if (Data0 >= mMapHeight_Shifted)
		pSprite->field_38 = 2;

	dword_3A395 = pSprite->field_4;
	Data0 = 8;
	Data0 += pSprite->field_26;
	Data4 = -3;
	Data4 += pSprite->field_28;

	Map_Terrain_Get_Type_And_Walkable( Data0, Data4 );

	if (Data4 == 9 || Data4 == 0x0A) {
		Data0 = pSprite->field_12;
		pSprite->field_28 += Data0;
		Data8 = pSprite->field_28;

		sMapTarget* Data30 = mSprite_WalkTargets[pSprite->field_32];
		//seg004:5C49
		Data0 = pSprite->field_40;
		if (Data30[Data0].field_0 >= 0)
			Data30[Data0].field_2 = Data8;
	}
	//loc_1E9CD;
	sub_1FFC6( pSprite, Data4 );
	if (pSprite->field_12 > 5)
		sub_223B2( pSprite );

	if (Data4 == 9)
		return -1;

loc_1E9EC:;	// Troop Falling?
	if (pSprite->field_12 < 0x0C) {
		pSprite->field_3E = 0;
		pSprite->field_56 = 0;

		return 0;
	}
	
	pSprite->field_38 = 6;
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

	Data28 = off_32AE4[pSprite->field_22];
	pSprite->field_8 = Data28[ (Data8 + 0x20) / 2 ];

	if (pSprite->field_59)
		pSprite->field_8 = Data28[ (Data8 + 0x10) / 2 ];
	
loc_1EB0E:;
	Field_52 = pSprite->field_52;
	Field_0 = pSprite->field_0;
	Field_4 = pSprite->field_4;
	sub_1FFC6( pSprite, Data4 );
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

	Data28 = off_32AE4[pSprite->field_22];
	pSprite->field_8 = Data28[ (Data8 + 0x20) / 2 ];

	if (pSprite->field_59)
		pSprite->field_8 = Data28[ (Data8 + 0x10) / 2 ];
	
	//loc_1EC4F
	Field_52 = pSprite->field_52;
	sub_1FFC6( pSprite, Data4 );
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
		pSprite->field_38 = 0;
		return 0;
	}

	pSprite->field_38 = 0x33;
	pSprite->field_36 = 0x24;

	Data0 = tool_RandomGet() & 6;
	Data0 = byte_3D47F[Data0];

	Data0 += pSprite->field_10;
	Data0 &= 0x1FE;
	pSprite->field_10 = Data0;
	pSprite->field_8 = 0xA4;

	if (pSprite->field_22)
		pSprite->field_8 = 0xA7;

	sub_20456( pSprite, Data8 );
	Data8 >>= 1;
	pSprite->field_A = Data8;
	return -1;

loc_1ED5B:;
	Sprite_XY_Store( pSprite );
	mTmp_FrameNumber = pSprite->field_A;

	Sprite_Movement_Calculate( pSprite );
	sub_20478( pSprite );
	sub_1FFC6( pSprite, Data4 );

	if (pSprite->field_38 != 0x33)
		goto loc_1EE3E;

	if (pSprite->field_60 > 6 || pSprite->field_60 < 4) {
		pSprite->field_36 -= 5;
		if (pSprite->field_36) {
			if (pSprite->field_36 >= 0)
				return -1;

			pSprite->field_36 = 0;
		}
	}
	sub_20456( pSprite, Data8 );

	//seg004:606C
	Data28 = off_32AE4[pSprite->field_22];
	Data0 = Data28[ (0x60 + Data8) / 2 ];

	if (Data0 != pSprite->field_8) {
		pSprite->field_8 = Data28[ (0x60 + Data8) / 2 ];
		pSprite->field_A = 0;
		return -1;
	}
	//loc_1EE34
	pSprite->field_38 = 0;

loc_1EE3E:;
	pSprite->field_43 = 0;
	sub_305D5( Data20 );
	return -1;

loc_1EE59:;
	if (pSprite->field_60 <= 6 && pSprite->field_60 >= 4 ) {
		pSprite->field_38 = 0;
		return 0;
	}

	pSprite->field_38 = 0x33;
	pSprite->field_36 = 0x2E;

	Data0 = tool_RandomGet() & 0x1F;

	pSprite->field_10 += Data0;
	pSprite->field_8 = 0xA4;
	if (pSprite->field_22)
		pSprite->field_8 = 0xA7;

	sub_20456( pSprite, Data8 );
	Data8 >>= 1;
	pSprite->field_A = Data8;
	return -1;

loc_1EEEC:;

	if (!pSprite->field_66) {
		pSprite->field_38 = 0;
		return 0;
	}

	pSprite->field_38 = 0x5B;
	pSprite->field_8 = 0xCC;
	pSprite->field_A = 4;
	return -1;

loc_1EF28:;
	pSprite->field_A -= 1;
	if (pSprite->field_A < 0)
		return loc_1F043(pSprite);

	pSprite->field_36 = 0x30;
	sub_1F6F4( pSprite );
	return - 1;

}

int16 cFodder::Sprite_Troop_Dies( sSprite* pSprite ) {
	uint8* DataFinal = 0;
	int16 Data0;
	sSprite* eax = 0;
	int8* Data28 = 0;
	sSquad_Member* SquadMember = 0;

	// Is Player?
	if (pSprite->field_22)
		goto loc_1F0EA;

	//Yes, is player

	++mTroops_Away;

	SquadMember = (sSquad_Member*) pSprite->field_46;

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
		DataFinal = (uint8*) mSquad;
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

	DataFinal = (uint8*)eax->field_46;
	goto loc_1F1E9;

loc_1F19A:;
	DataFinal = (uint8*) &mSquad[Data0];

loc_1F1E9:;

	if (DataFinal >= (uint8*)mSquad && DataFinal <= (uint8*)&mSquad[8]) {
		sSquad_Member* Member = (sSquad_Member*)DataFinal;
		if (Member->mNumberOfKills >= 999)
			Member->mNumberOfKills = 998;

		++Member->mNumberOfKills;
	}
loc_1F218:;

	return Sprite_Destroy_Wrapper( pSprite );
}

int16 cFodder::loc_1F043( sSprite* pSprite ) {
	if (!pSprite->field_66) {
		pSprite->field_38 = 0;
		return 0;
	}

	loc_1F092( pSprite,  (sSprite*) pSprite->field_66 );
	pSprite->field_A = 0;
	pSprite->field_8 = 0x7C;
	pSprite->field_38 = 0;
	return 0;
}

void cFodder::loc_1F092( sSprite* pSprite, sSprite* pData24 ) {
	int16 Data0 = pData24->field_0;
	Data0 += 0x10;
	if (pData24->field_6F == 0)
		goto loc_1F0B9;

	if (pData24->field_6F == 1)
		goto loc_1F0BE;
loc_1F0B9:;
	Data0 -= 0x0C;

loc_1F0BE:;

	int16 Data4 = pData24->field_4;
	Data4 -= 0x09;

	pSprite->field_0 = Data0;
	pSprite->field_4 = Data4;
}

int16 cFodder::sub_1F21E( sSprite* pSprite ) {

	if( !mMission_Aborted )
		if( !mMission_Complete )
			return 0;
		
	int16 Data0 = -1;
	int16 Data4 = 8;
	
	Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );
	
	if( Data4 == 6 ) {
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
				sub_1FE35( pSprite );
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
	
	pSprite->field_A = word_3D487[ Data0 ];
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

void cFodder::Sprite_Handle_Troop( sSprite* pSprite ) {

	if (!pSprite->field_22)
		goto loc_1F45B;

	if (!pSprite->field_4A)
		return;
	if (pSprite->field_4A < 0)
		goto loc_1F477;

	pSprite->field_4A -= 1;
	if (pSprite->field_4A)
		return;

	pSprite->field_4A = -1;
	goto loc_1F477;

loc_1F45B:;
	if (!pSprite->field_4A)
		return;

	if (pSprite->field_4A >= 0) {

		pSprite->field_4A--;
		if (pSprite->field_4A)
			return;
	}
loc_1F477:;
	if (pSprite->field_4F)
		return;

	if (pSprite->field_18 == eSprite_Enemy_Rocket) {
		mTroop_Cannot_Fire_Bullet = -1;
		mTroop_Cannot_Throw_Grenade = -1;
		word_3A010 = -1;
		if (word_3AA4B > 0x18)
			word_3A010 = 0;
		goto loc_1F4DB;

	}
	//loc_1F4B0
	if (pSprite->field_18 == eSprite_Enemy && mMapNumber > 4) {
		if (!(tool_RandomGet() & 0x1F)) {

			mTroop_Cannot_Fire_Bullet = -1;
			mTroop_Cannot_Throw_Grenade = 0;
		}
	}

loc_1F4DB:;

	if (!mTroop_Cannot_Fire_Bullet) {

		if (Sprite_Create_Bullet( pSprite ))
			pSprite->field_4A = -1;
		else
			pSprite->field_4A = 0;

		if (word_3AA1D != 2)
			pSprite->field_45 = 0x0F;
	}

	mTroop_Cannot_Fire_Bullet  = 0;
	if (mTroop_Cannot_Throw_Grenade)
		goto loc_1F549;

	if (!Sprite_Create_Grenade( pSprite ))
		goto loc_1F52F;

	pSprite->field_4A = -1;
	goto loc_1F539;

loc_1F52F:;
	pSprite->field_4A = 0;

loc_1F539:;
	if (word_3AA1D != 2)
		pSprite->field_45 = 0x0C;
loc_1F549:;
	mTroop_Cannot_Throw_Grenade = 0;
	if (word_3A010)
		goto loc_1F599;

	if (!sub_224ED(pSprite))
		goto loc_1F569;

	pSprite->field_4A = -1;
	goto loc_1F573;

loc_1F569:;
	pSprite->field_4A = 0;

loc_1F573:;
	if (pSprite->field_45 >= 0x0A)
		goto loc_1F599;

	pSprite->field_45 += 5;
	pSprite->field_44 = pSprite->field_45;
loc_1F599:;
	word_3A010 = 0;
}

void cFodder::sub_1F5A0( sSprite* pSprite ) {

	int16 Data0 = pSprite->field_2E;
	int16 Data4 = pSprite->field_30;

	sub_2A1F0( pSprite, Data0, Data4 );
	sub_20E5C( pSprite );
	sub_1F762( pSprite );
}

void cFodder::sub_1F5CA( sSprite* pSprite ) {

	int16 Data0 = mMouseX;
	Data0 += mCamera_Adjust_Col >> 16;

	Data0 -= 0x18;
	int16 Data4 = mMouseY;
	Data4 += mCamera_Adjust_Row >> 16;

	sub_2A1F0( pSprite, Data0, Data4 );
	sub_20E5C( pSprite );

	word_3ABAF = pSprite->field_A;
	sub_1F762( pSprite );

	if (!pSprite->field_43)
		return;

	pSprite->field_A = word_3ABAF;
}

void cFodder::Sprite_XY_Store( sSprite* pSprite ) {
	word_3ABAD = 0;
	
	dword_3A391 = (pSprite->field_0 & 0xFFFF) | (pSprite->field_2 << 16);
	dword_3A395 = (pSprite->field_4 & 0xFFFF) | (pSprite->field_6 << 16);
}

void cFodder::Sprite_XY_Restore( sSprite* pSprite ) {
	word_3ABAD = -1;

	pSprite->field_0 = dword_3A391 & 0xFFFF;
	pSprite->field_2 = dword_3A391 >> 16;

	pSprite->field_4 = dword_3A395 & 0xFFFF;
	pSprite->field_6 = dword_3A395 >> 16;
}

void cFodder::sub_1F66F( sSprite* pSprite ) {
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
	for (Data4 = 0; Data4 < 0x0E; Data4++) {
		if (Data0 == pSprite->field_3C)
			break;

		Data0 += 2;
		Data0 &= 0x0E;
	}

	pSprite->field_36 = word_3D495[Data4 / 2];
}

void cFodder::sub_1F6F4( sSprite* pSprite ) {
	int16 Data4 = 0;

	if (!sub_20E91( pSprite )) {
		sub_20F19( pSprite );
		Sprite_Movement_Calculate( pSprite );
	}

	Squad_Join_Check( pSprite );
	sub_20478( pSprite );

	byte_3ABA9 = pSprite->field_60;
	sub_1FFC6( pSprite, Data4 );

	int16 Data0 = byte_3ABA9;
	Data4 = pSprite->field_60;
	if (Data0 != 6)
		goto loc_1F75D;

	if (Data4 == 4)
		goto loc_1F753;

	if (Data4 == 5)
		goto loc_1F75D;

	pSprite->field_52 = 5;
	goto loc_1F75D;

loc_1F753:;
	pSprite->field_52 = 3;

loc_1F75D:;
	sub_21041( pSprite );
}

void cFodder::sub_1F762( sSprite* pSprite ) {
	int16 Data0, Data8;
	sSprite* Dataa30 = 0;
	sMapTarget* Data30 = 0;
	int16* Data28 = 0;
	sSquad_Member* Data24 = 0;

	if (!pSprite->field_22)
		goto loc_1F7CE;

	if (!pSprite->field_70)
		goto loc_1F7CE;

	if (pSprite->field_0 != pSprite->field_26)
		goto loc_1F7CE;

	if (pSprite->field_4 != pSprite->field_28)
		goto loc_1F7CE;

	pSprite->field_8 = 0xDC;
	pSprite->field_A = 0;
	word_3ABB1 = -1;
	return;

loc_1F7CE:;
	if (word_3AA1D == 2)
		goto loc_1F7FF;

	if (pSprite->field_5A)
		goto loc_1F7F9;

	if (!pSprite->field_43)
		goto loc_1F7FF;

	if (pSprite->field_4F)
		goto loc_1F7FF;

loc_1F7F9:;
	word_3A9C6 = -1;

loc_1F7FF:;
	Data8 = pSprite->field_3C;
	Data8 += word_3A8CF;
	Data8 &= 0x0E;

	pSprite->field_3C = Data8;
	Data8 /= 2;
	Data0 = pSprite->field_22;

	Data28 = off_32AE4[pSprite->field_22];
	if (!word_3AA41)
		goto loc_1F9C0;

	if (pSprite->field_22)
		goto loc_1F98B;

	Data30 = mSprite_WalkTargets[pSprite->field_32];
	if (Data30[pSprite->field_40].field_0 >= 0)
		goto loc_1F9C0;

	//seg005:01CC
	Dataa30 = mSquads[pSprite->field_32][0];

	if (Dataa30 == INVALID_SPRITE_PTR || Dataa30 == 0 )
		goto loc_1F9C0;

	//seg005:020C
	if (Dataa30 != pSprite )
		goto loc_1F94F;

	Data0 = pSprite->field_0;
	if (Data0 != pSprite->field_26)
		goto loc_1F9C0;

	//seg005:0246
	Data0 = pSprite->field_4;
	if (Data0 != pSprite->field_28)
		goto loc_1F9C0;

	goto loc_1F98B;

loc_1F94F:;
	Data0 = Dataa30->field_0;

	if (Data0 != Dataa30->field_26)
		goto loc_1F9C0;

	Data0 = Dataa30->field_4;
	if (Data0 != Dataa30->field_28)
		goto loc_1F9C0;

	if (!word_3B2F3)
		goto loc_1F9C0;

loc_1F98B:;
	if (pSprite->field_60 == 6)
		goto loc_1F9C0;

	//seg005:02A6
	pSprite->field_8 = *(Data28 + Data8 + 0x30);
	pSprite->field_A = 0;
	word_3ABAF = 0;
	return;

loc_1F9C0:;
	if (pSprite->field_54 != 2)
		goto loc_1FA39;

	if (word_3AA1D) {
		if (word_3AA1D == 2) {
			pSprite->field_8 = *(Data28 + Data8);
			pSprite->field_A = word_39FFA;
		}
		else {
			//loc_1F9FF
			pSprite->field_8 = *(Data28 + Data8);
			pSprite->field_A = 0;
		}
	}
	//loc_1FA20
	--pSprite->field_57;
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
	word_3ABAF = pSprite->field_A;

	if (pSprite->field_A < 3)
		return;
	word_3ABAF = 0;
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
	if (pSprite->field_4F) {
	
		pSprite->field_8 = *(Data28 + Data8 + 0x20);
		Data0 = word_39FF2;
		if (pSprite->field_43) {
			Data0 = word_39FF6;
		}
		Data0 &= 1;
		pSprite->field_A = Data0;
		goto loc_1FCD7;
	}
	//loc_1FBF8
	Data0 = word_3AA1D;
	if (!Data0) {
		pSprite->field_8 = *(Data28 + Data8 + 0x18);
		pSprite->field_A = word_39FF6;
		goto loc_1FCD7;
	}
	//loc_1FC29
	Data0 = pSprite->field_8;
	if (Data0 == *(Data28 + Data8 + 0x18) && pSprite->field_A) {
		pSprite->field_A = 0;
		goto loc_1FCD7;
	}

	//loc_1FC61
	Data24 = (sSquad_Member*) pSprite->field_46;

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
		pSprite->field_A = word_39FF2;
	else
		pSprite->field_A = word_39FFA;

loc_1FCD7:;
	if (!word_3A9C6)
		return;

	pSprite->field_A = mTmp_FrameNumber;
	word_3A9C6 = 0;
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
		goto loc_1FD80;
	}

	//loc_1FD36
	Data0 =  word_3BED5[pSprite->field_32];
	if (!Data0)
		pSprite->field_36 >>= 1;
	else {
		if (Data0 != 1)
			pSprite->field_36 = 0x18;
	}
loc_1FD80:;
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

	sub_2A1F0( pSprite, Data0, Data4 );
	sub_20E5C( pSprite );

	word_3ABAF = pSprite->field_A;

	sub_1F762( pSprite );
	if (!pSprite->field_43)
		return;

	pSprite->field_A = word_3ABAF;
}

void cFodder::sub_1FE35( sSprite* pSprite ) {
	int16 Data0 = 0;

	sSprite* Data2C, *Data30;

	if (pSprite->field_5C == 0)
		return;

	if (Sprite_Get_Free( Data0, Data2C, Data30 ))
		return;

	Data2C->field_1A = (int32*) pSprite;
	Data2C->field_6A = pSprite;

	Data2C->field_0 = pSprite->field_0;
	Data2C->field_4 = pSprite->field_4;
	Data2C->field_0 += 4;
	Data2C->field_8 = 0x8D;
	Data2C->field_A = 2;
	Data2C->field_2 = pSprite->field_2;
	Data2C->field_6 = pSprite->field_6;
	Data2C->field_18 = 0x16;
	Data2C->field_62 = pSprite->field_18;
	Data2C->field_20 = 0;
	Data2C->field_32 = -1;
	Data2C->field_2C = -1;
	Data2C->field_52 = 0;
}

int16 cFodder::sub_1FF1A( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30 ) {
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
		pData2C->field_18 = 0x17;
		pData2C->field_20 = pSprite->field_20;
		pData2C->field_32 = -1;
		pData2C->field_2C = 1;
		pData2C->field_52 = 0;
		pData2C->field_12 = 1;
		Data0 = 0;
	}

	return Data0;
}

void cFodder::sub_1FFC6( sSprite* pSprite, int16& pData4 ) {
	int16 Data0 = -3;
	pData4 = 8;

	if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, pData4 ))
		goto loc_20236;

	pSprite->field_60 = pData4 & 0xFF;
	pSprite->field_50 = 0;
	pSprite->field_4F = 0;

	if (pData4 == 0x0A) {
		if (pSprite->field_18 == eSprite_Enemy)
			goto loc_20251;

		if (pSprite->field_22 == 2)
			goto loc_20251;

		if (!pSprite->field_38 || pSprite->field_38 >= 0x32) {
			pSprite->field_38 = 3;
			return;
		}
	}

	//loc_20044
	if (pData4 == 9) {

		if (pSprite->field_18 == eSprite_Enemy)
			goto loc_20251;

		if (pSprite->field_22 == 2)
			goto loc_20251;

		++pSprite->field_56;
		return;
	}

	//loc_20072
	pSprite->field_56 = 0;
	if (pData4 == 8)
		goto loc_201CC;

	if (pData4 == 1)
		goto loc_20108;

	if (pData4 == 2)
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
	if (pData4 == 4)
		goto loc_2019E;

	if (pData4 == 5)
		goto loc_201AB;

	if (pData4 != 6)
		goto loc_201B8;

	if (pSprite->field_22 == 2)
		goto loc_20251;
	
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

loc_2019E:;
	pSprite->field_50 = 3;
	return;

loc_201AB:;
	pSprite->field_50 = 6;
	return;

loc_201B8:;
	if (pData4 == 0x0B)
		goto loc_2022C;

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

	if (pSprite->field_22 == 2)
		return;

	pSprite->field_38 = 0x32;
	return;

loc_2022C:;
	pSprite->field_5B = 1;
	return;

loc_20236:;
	if (pData4 == 9 || pData4 == 0x0A) {
		pSprite->field_38 = 2;
		return;
	}

loc_20251:;
	pSprite->field_43 = 1;
	word_3ABAD = -1;
	if (!pSprite->field_22)
		goto loc_2035C;

	pSprite->field_0 = dword_3A391 & 0xFFFF;

	Data0 = -3;
	pData4 = 8;
	if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, pData4 ))
		goto loc_202E5;

	if (pData4 == 9)
		goto loc_202E5;

	if (pData4 == 0x0A)
		goto loc_202E5;

	if (pSprite->field_22 == 2 && pData4 == 6)
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

	Data0 = word_390AE;
	Data0 >>= 2;
	Data0 &= 0x3F;
	Data0 -= 0x20;
	Data0 &= 0x1FE;
	pSprite->field_10 += Data0;
	pSprite->field_4 = dword_3A395;

	Data0 = word_390AE;
	Data0 &= 0x1F;
	Data0 += 0x0C;
	pSprite->field_44 = (int8) Data0;
	return;

loc_2035C:;

	word_3A9E6 = pSprite->field_0;
	pSprite->field_0 = dword_3A391 & 0xFFFF;

	Data0 = -3;
	pData4 = 8;
	if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, pData4 ))
		return;

	pSprite->field_0 = word_3A9E6;
	pSprite->field_4 = dword_3A395 & 0xFFFF;

	Data0 = -3;
	pData4 = 8;
	if (!Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, pData4 ))
		return;

	//loc_203BA
	pSprite->field_A = mTmp_FrameNumber;
	Sprite_XY_Restore( pSprite );
	pSprite->field_26 = pSprite->field_0;
	pSprite->field_28 = pSprite->field_4;

	sMapTarget* Data30 = mSprite_WalkTargets[ pSprite->field_32 ];
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

void cFodder::String_Print(  const uint8* pWidths, int32 pParam0, int32 pParam08, int32 pParamC, const char* pText ) {
	uint8* ptr = 0;
	uint8 al = 0;
	int32 unk14 = 0;

	mGUI_Temp_Y = pParamC;
	mGUI_Temp_Height = 0;

	for (;;) {
	loc_29C7A:;
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
				if (word_3AC19) {
					if (NextChar == 0x20) {
						NextChar = (uint8) word_3AC19;
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

			// 20D
			//loc_29D2D
			unk14 = -1;
			ptr = mUnkStringModifier;
			do {
				if (*ptr == 0xFF)
					goto loc_29C7A;
				++unk14;

				al = *ptr++;

			} while(al != NextChar);

			NextChar = unk14;
			NextChar += 0x24;

			loc_29D71:;

			if (word_3AC21) {
				mGraphics->sub_145AF( pParam0 + NextChar, pParam08, pParamC );
			}
			else			         //0	// C     // 4	   // 8
				Sprite_Draw_Frame(  pParam0, pParamC, NextChar, pParam08 );

		}
		loc_29DC7:;
		//seg007:02A7
		NextChar10 = pWidths[NextChar10];

		pParam08 += NextChar10;
	}
}

void cFodder::Vehicle_Input_Handle() {
	
	if (!mButtonPressLeft) {
		word_39F00 = 0;
		return;
	}

	if (word_39F00)
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

	if (Data20->field_6F >= 5) {
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

	const int16* Data2C = mSprite_Width;
	const int16* Data30 = mSprite_Height_Top;
	const int16* Data34 = mSprite_Height_Bottom;

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

		if (Data20->field_22)
			continue;

		if (Data20->field_20)
			continue;

		int16 Data18 = Data20->field_18;
		int16 Data8 = Data20->field_0;

		if (!Data20->field_6F || Data20->field_6F == 1)
			Data8 -= 8;

		if (Data0 < Data8)
			continue;

		Data8 += Data2C[Data18];
		if (Data0 > Data8)
			continue;

		Data8 = Data20->field_4;
		Data8 -= Data30[Data18];
		Data8 -= 0x14;
		if (Data4 < Data8)
			continue;

		Data8 = Data20->field_4;
		Data8 += Data34[Data18];
		if (Data4 > Data8)
			continue;

		Data0 = mSquad_Selected;
		if (Data0 < 0)
			break;

		Data24 = mSquads[Data0];
		for (;;) {
			if (*Data24 == INVALID_SPRITE_PTR)
				goto loc_313C6;

			sSprite* Data28 = *Data24++;
			Data28->field_66 = Data20;
		}
	}

loc_313C6:;
	Data0 = 0;
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

	if (Data20->field_22)
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
	word_39F00 = -1;
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
	word_3B461[Data0] = 0;
	Dataa2C = mSquads[Data0];

	//loc_31578
	for (; Data18 >= 0; --Data18) {

		if (*Dataa2C == INVALID_SPRITE_PTR)
			goto loc_31668;

		sSprite* Data24 = *Dataa2C++;

		if (!Data24->field_6E)
			continue;

		//seg011:2AE9 

		Data24->field_6E = 0;
		Dataa30 = (sSprite*) Data24->field_6A;
		Data24->field_6A = 0;
		Data24->field_66 = 0;

		Data24->field_0 = Dataa30->field_0;
		Data24->field_4 = Dataa30->field_4;
		if (!Data24->field_6F)
			goto loc_3162B;

		if (Data24->field_6F == 1)
			goto loc_3162B;

		Data24->field_0 += 0x0F;
		Data24->field_4 += -10;

	loc_3162B:;
		Data24->field_26 = Data24->field_0;
		Data24->field_28 = Data24->field_4;
		Data24->field_26 -= 6;
		Data24->field_28 += 0x10;
	}

loc_31668:;
	word_39F00 = -1;
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
			mImageFaded = mImage->paletteFade();

		if (Duration > 1)
			--Duration;
		else {
			if (DoBreak == false) {
				mImage->paletteFadeOut();
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

int16 cFodder::introPlayText() {

	mGraphics->Load_Sprite_Font();
	mGraphics->SetSpritePtr( eSpriteType::eSPRITE_FONT );

	for ( word_3B2CF = 1; mVersion->mIntroData[word_3B2CF].mImageNumber != 0; ++word_3B2CF ) {

		word_3B447 = 0x288;
		
		mImage->paletteClear();

		if (mVersion->mIntroData[word_3B2CF].mImageNumber == 0 && mVersion->mIntroData[word_3B2CF].mText == 0 )
			break;

		if (mVersion->mIntroData[word_3B2CF].mImageNumber != 0xFF) {

			std::stringstream ImageName;
			ImageName << (char) mVersion->mIntroData[word_3B2CF].mImageNumber;

			mGraphics->imageLoad( ImageName.str(), 0xD0 );
		}
		else {
			word_3B447 = 0xAF;
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
		
		int16 Duration = word_3B447;
		int16 Fade = -1;
		bool DoBreak = false;

		while( Fade == -1 || DoBreak == false  ) {
			--Duration;

			if (Duration) {
				if (Fade)
					Fade = mImage->paletteFade();

				Mouse_GetData();
				if (mouse_Button_Status) {
					word_3B2CF = 16;
					word_3B4F3 = -1;
					mImage->paletteFadeOut();
					Fade = -1;
					DoBreak = true;
				}
			}
			else {
				mImage->paletteFadeOut();
				Fade = -1;
				DoBreak = true;
			}

			g_Window.RenderAt( mImage );
			g_Window.FrameEnd();
		}

		
	}

	return word_3B4F3;
}

void cFodder::Sprite_SetDataPtrToBase( const sSpriteSheet** pSpriteSheet ) {

	mSpriteDataPtr = pSpriteSheet;
}

void cFodder::intro() {
	
	// Disabled: GOG CD Version doesn't require a manual check
	//  CopyProtection();

	word_42851 = 0;
	sub_136D0();

	word_3B4F3 = 0;
	mGraphics->Load_Sprite_Font();
	
	if (mSkipIntro)
		goto introDone;

	intro_Music_Play();

	if (mVersion->mGame == eGame::CF1)
		intro_LegionMessage();

	if (ShowImage_ForDuration( "cftitle", 0x1F8 ))
		goto introDone;
	
	if (introPlayText())
		goto introDone;

	if (ShowImage_ForDuration( "virgpres", 0x2D0 ))
		goto introDone;
	
	if (ShowImage_ForDuration( "sensprod", 0x2D0 ))
		goto introDone;
	
	word_42851 = -1;
	if (ShowImage_ForDuration( "cftitle", 0x318 ))
		goto introDone;

introDone:;
	mIntroDone = -1;
	mSound->Music_Stop();

	mGraphics->LoadpStuff();
	mSound->Music_Play( 0 );
}

void cFodder::intro_Music_Play() {

	mSound->Music_Play( 16 );
}

int16 cFodder::ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration ) {
	bool DoBreak = false;

	g_Graphics.imageLoad( pFilename, 0x100 );
	mGraphics->PaletteSet();

	mImageFaded = -1;
	while( mImageFaded == -1 || DoBreak == false  ) {
		Mouse_GetData();
		--pDuration;

		if (pDuration) {
			if (mImageFaded)
				mImageFaded = mImage->paletteFade();

			if (mouse_Button_Status) {
				word_3B4F3 = -1;
				mImage->paletteFadeOut();
				mImageFaded = -1;
				DoBreak = true;
			}
		}
		else {
			mImage->paletteFadeOut();
			mImageFaded = -1;
			DoBreak = true;
		}

		g_Window.RenderAt( mImage );
		g_Window.FrameEnd();
	}

	return word_3B4F3;
}

void cFodder::Mission_Phase_Next() {
	mMissionPhase += 1;
	mMissionPhaseRemain -= 1;
	if (mMissionPhaseRemain)
		return;

	for (unsigned int x = 0; x < 8; ++x) {
		mSquad[x].mPhaseCount = 0;
	}

	mSaved_MissionNumber = mMissionNumber;
	mSaved_MapNumber = mMapNumber;
	--mSaved_MapNumber;

	mMissionPhaseRemain = mMissionPhases = mVersion->mMissionData->mMissionPhases[mMissionNumber];
	mMissionPhase = 0;
	++mMissionNumber;
	mSquad_AliveCount += 0x0F;

	word_390E8 = mSquad_AliveCount;
	word_390E8 -= 0x0F;
	mGame_Prepare = -1;
	word_390D4 = 0;

	mGraveRecruitIDPtr = mGraveRecruitID;
	mGraveRecruitID[0] = -1;
	mGraveRankPtr2 = mGraveRankPtr;
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

	if (mVersion->mPlatform == ePlatform::Amiga) {
		mGraphics->imageLoad( "won.raw", 32 );
	}
	else {
		mGraphics->imageLoad( "won.dat", 0x100 );
	}

	mGraphics->PaletteSet();

	while(mImage->paletteFade()) {

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
	}

	while (1) {

		eventProcess();
	}
}

void cFodder::sub_20456( sSprite* pSprite, int16& pData8 ) {
	pData8 = pSprite->field_10;

	pData8 >>= 5;
	pData8 -= 1;
	pData8 ^= 0x0F;
	pData8 &= 0x0E;
}

void cFodder::sub_20478( sSprite* pSprite ) {
	int16 Data0 = pSprite->field_4;

	if (Data0 < 0 || Data0 <= 0x0D) {
		pSprite->field_4 = dword_3A395;
		word_3ABAD = -1;
	}

	Data0 = pSprite->field_4;

	if (Data0 >= mMapHeight_Shifted) {
		if (!pSprite->field_38 || pSprite->field_38 >= 0x32) {
			pSprite->field_4 = dword_3A395;
			word_3ABAD = -1;
		}
	}
	//loc_204E1
	Data0 = pSprite->field_0;
	if (Data0 <= 0) {
		if (dword_3A391 <= 0)
			return;

		goto loc_20521;
	}

	Data0 += 0x0C;
	if (Data0 < mMapWidth_Shifted)
		return;

	Data0 = dword_3A391 + 0x10 ;
	if (Data0 >= mMapWidth_Shifted)
		return;

loc_20521:;
	pSprite->field_0 = dword_3A391;
	word_3ABAD = -1;
}

void cFodder::Hero_Add( sSquad_Member* pSquadMember ) {
	sHero* Hero = &mHeroes[4];
	int16 Data4;

	for (Data4 = 4; Data4 >= 0; --Data4, --Hero ) {
		if (!pSquadMember->mNumberOfKills)
			break;

		if (pSquadMember->mNumberOfKills < Hero->mKills)
			break;

		if (pSquadMember->mNumberOfKills > Hero->mKills)
			continue;

		if (pSquadMember->mRank <= Hero->mRank)
			break;
	}
	if (Data4 == 4)
		return;

	int16 X = 4;

	do {
		mHeroes[X + 1] = mHeroes[X];
		--X;
	} while (++Data4 < 4);

	//seg005:0EDD
	mHeroes[X + 1].mRecruitID = pSquadMember->mRecruitID;
	mHeroes[X + 1].mRank = pSquadMember->mRank;
	mHeroes[X + 1].mKills = pSquadMember->mNumberOfKills;
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

  	if (word_3A9E4)
		--word_3A9E4;

	if (mMission_Completed_Timer)
		goto loc_20A2D;

	if (!pSprite->field_2E)
		goto loc_20A2D;

	if (pSprite == mSquad_Leader)
		if (pSprite->field_54 == 1)
			goto loc_20A2D;

	if (Sprite_Get_Free( Data0, Data2C, Data30 ))
		goto loc_20A2D;

	if (byte_3A9D2[2] == 0x14)
		goto loc_20A2D;

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

		Data2C->field_5E = ((sSquad_Member*)pSprite->field_46) - mSquad;
		Data2C->field_5D = -1;

		// Bullet Travel time
		Data0 = mSprite_Weapon_Data.mAliveTime;
		Data0 += mSprite_Bullet_Time_Modifier;
		Data2C->field_12 = Data0;
	}

	Data2C->field_18 = eSprite_Bullet;
	Data2C->field_1E = pSprite->field_1E;
	Data2C->field_20 = pSprite->field_20;
	Data2C->field_20 += 6;
	Data2C->field_52 = pSprite->field_52;
	Data2C->field_22 = pSprite->field_22;
	Data2C->field_32 = pSprite->field_32;
	Data2C->field_2C = 0;
	if (pSprite->field_22)
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

	Data2C->field_1A = (int32*) pSprite;
	Data2C->field_2A = 2;
	Data2C->field_16 = 0;
	Data2C->field_14 = 2;

	if (Data2C->field_A == 4)
		Data2C->field_14 = -1;

	Data2C->field_34 = -1;
	Data2C->field_50 = 0;
	Data2C->field_59 = 0;
	Data8 = 7;

	if (pSprite->field_22)
		goto loc_209C7;

	if (pSprite == mSquad_Leader)
		goto loc_209B3;

	Data8 = Squad_Member_GetDeviatePotential( (sSquad_Member*) pSprite->field_46 );
	goto loc_209C7;

loc_209B3:;
	word_3B1AB += 1;
	word_3B1AB &= 3;
	if (!word_3B1AB)
		goto loc_209F3;

	Data8 = mSprite_Weapon_Data.mDeviatePotential;

loc_209C7:;
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

loc_20A2D:;
	return -1;
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
	Data2C->field_1E = 0x0000;	// This maybe incorrect
	Data2C->field_20 = 0x7;
	Data30->field_1E = 0;
	Data30->field_20 = 0;
	Data2C->field_4 += 1;
	Data2C->field_0 += 3;
	int32 Dataa0 = Data2C->field_12 << 16;

	Dataa0 >>= 1;
	if (Dataa0 > 0x0E0000)
		Dataa0 = 0x0E0000;

	Data2C->field_1A = (int32*) Dataa0;
	Data2C->field_18 = eSprite_Grenade;
	Data30->field_18 = eSprite_ShadowSmall;
	Data2C->field_52 = 0;
	Data30->field_52 = 0;
	Data2C->field_50 = 0;
	Data2C->field_22 = pSprite->field_22;
	Data30->field_22 = pSprite->field_22;
	Data2C->field_32 = pSprite->field_32;
	Data2C->field_2C = 0;
	Data30->field_2C = -1;
	Data2C->field_38 = 0;
	if (pSprite->field_18 == eSprite_Enemy)
		Data2C->field_12 += 0x1C;
	Data2C->field_56 = 4;
	Data2C->field_46 = (int32*)pSprite;

	if (pSprite == mSquad_Leader)
		mMouse_Button_LeftRight_Toggle = 0;

	return 0;
}

void cFodder::sub_20E5C( sSprite* pSprite ) {
	int16 Data0 = pSprite->field_10;

	Data0 >>= 5;
	Data0 -= 1;
	Data0 ^= 0x0F;
	Data0 &= 0x0F;
	pSprite->field_3C = Data0;

	word_3A8CF = 0;
}

int16 cFodder::sub_20E91( sSprite* pSprite ) {

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

	word_3ABAD = -1;
	Data0 = -1;
	return -1;
}

void cFodder::sub_20F19( sSprite* pSprite ) {
	int16 Data0 = pSprite->field_10;

	if (pSprite->field_60 != 0x0D)
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
	if (pSprite->field_60 == 1)
		goto loc_20FD1;

	if (pSprite->field_60 != 2)
		goto loc_20FE3;

loc_20FD1:;
	pSprite->field_36 >>= 1;

loc_20FD9:;
	pSprite->field_36 >>= 1;
	return;
loc_20FE3:;
	if (pSprite->field_60 != 7)
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

void cFodder::sub_21041( sSprite* pSprite ) {
	
	if (pSprite->field_22) {
		pSprite->field_4D = 0;
		return;
	}

	word_3B2F3 = 0;
	sSprite** Data30 = mSquads[pSprite->field_32];
	sSprite** Data34 = Data30;

	for (;;) {
		if (*Data30 == INVALID_SPRITE_PTR )
			return;

		sSprite* eax = *Data30;
		++Data30;
		if (pSprite == eax)
			break;
	}
	--Data30;
	if (Data34 == Data30)
		return;

	Data30--;
	int16 Data0 = pSprite->field_0;
	int16 Data4 = Data0;
	Data4 += 8;
	int16 Data8 = pSprite->field_4;
	int16 DataC = Data8;
	Data8 -= 2;
	DataC+= 2;
	//seg005:1A13
	dword_3A9C8 = Data30;
	sSprite* Data24 = *Data30;
	++Data30;

	int16 Data14 = Data24->field_0;
	if (Data4 < Data14)
		return;

	Data14 += 8;
	if (Data14 < Data0)
		return;

	Data14 = Data24->field_4;
	Data14 += 2;
	if (Data14 < Data8)
		return;

	if (DataC < Data14)
		return;

	if (Data24->field_6E)
		return;


	int16 Data1C = pSprite->field_40;
	if (Data1C != Data24->field_40)
		return;

	Sprite_XY_Restore( pSprite );
	word_3A9C6 = -1;
	pSprite->field_43 = 1;
	word_3B2F3 = -1;
}

int16 cFodder::Sprite_Get_Free( int16& pData0, sSprite*& pData2C, sSprite*& pData30 ) {
	if (!word_3B15D) {

		if (pData0 == 2)
			goto loc_21234;

		pData2C = &mSprites[42];

		for (int16 Data1C = 0x2A; Data1C >= 0; --Data1C) {
			if (pData2C->field_0 == -32768)
				goto loc_21217;

			pData2C--;
		}
	}
	//loc_211F0
	pData2C = &mSprite_Spare;
	pData30 = &mSprite_Spare;
	pData0 = -1;
	word_3B15D = pData0;
	return -1;

loc_21217:;

	Sprite_Clear( pData2C );
	pData0 = 0;
	return 0;

loc_21234:;
	pData2C = mSprites;

	for( int16 Data1C = 0x29; Data1C >= 0; --Data1C, ++pData2C ) {
		
		if( pData2C->field_0 != -32768 )
			continue;
		
		if( (pData2C+1)->field_0 == -32768 )
			goto loc_2128F;
	}
	
	pData2C = &mSprite_Spare;
	pData30 = &mSprite_Spare;
	pData0 = -1;
	word_3B15D = pData0;
	return -1;

loc_2128F:;
	pData30 = pData2C + 1;

	Sprite_Clear( pData2C );
	Sprite_Clear( pData30 );
	pData0 = 0;
	return 0;
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
	pSprite->field_1A = 0;
	pSprite->field_1E = 0;
	pSprite->field_20 = 0;
	pSprite->field_22 = 0;
	pSprite->field_24 = 0;
	pSprite->field_26 = 0;
	pSprite->field_28 = 0;
	pSprite->field_2A = 0;
	pSprite->field_2C = 0;
	pSprite->field_2E = 0;
	pSprite->field_30 = 0;
	pSprite->field_32 = 0;
	pSprite->field_34 = 0;
	pSprite->field_36 = 0;
	pSprite->field_38 = 0;
	pSprite->field_3A = 0;
	pSprite->field_3C = 0;
	pSprite->field_3E = 0;
	pSprite->field_40 = 0;
	pSprite->field_42 = 0;
	pSprite->field_43 = 0;
	pSprite->field_44 = 0;
	pSprite->field_45 = 0;
	pSprite->field_46 = 0;
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
	pSprite->field_6A = 0;
	pSprite->field_6E = 0;
	pSprite->field_6F = 0;
	pSprite->field_70 = 0;
	pSprite->field_74 = 0;
	pSprite->field_75 = 0;
}

void cFodder::Sprite_Handle_Explodable( sSprite* pSprite ) {
	int64 Dat0;
	int32 Dat4;
	int16 Data0, Data4;

	if (pSprite->field_5B)
		goto loc_2132A;

	if (pSprite->field_20)
		goto loc_2132A;

	if (pSprite->field_38 == 5)
		goto loc_213F7;

	if (pSprite->field_38 > 5)
		goto loc_2132A;

	pSprite->field_5B = 0;
	return;

loc_2132A:;

	Sprite_Movement_Calculate( pSprite );

	Dat0 = (int64) pSprite->field_1A;
	Dat4 = (pSprite->field_1E & 0xFFFF) | (pSprite->field_20 << 16);

	Dat0 -= 0x28000;

	Dat4 += Dat0;
	if (Dat4 < 0) {
		Dat4 = 0;
		Dat0 = -Dat0;
		Dat0 >>= 2;
	}

	pSprite->field_1A = (int32*) Dat0;
	pSprite->field_1E = Dat4 & 0xFFFF;
	pSprite->field_20 = Dat4 >> 16;


	if (!(Dat4 >> 16)) {
		if (!(Dat0 >> 16))
			goto loc_21464;
	}

	Data0 = -3;
	Data4 = 0x0C;

	Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 );

	if (pSprite->field_20)
		return;

	if (Data4 == 5)
		goto loc_21464;

	if (Data4 == 6)
		goto loc_21464;

	if (Data4 == 9)
		goto loc_21464;

	if (Data4 == 0x0A)
		goto loc_21464;

	return;

loc_213F7:;

	if (mMission_Completed_Timer)
		return;

	pSprite->field_38 = 6;
	pSprite->field_5B = -1;
	pSprite->field_1E = 0x0000;
	pSprite->field_20 = 0x1;

	Data0 = tool_RandomGet() & 0x1FE;
	pSprite->field_10 = Data0;

	Data0 = tool_RandomGet() & 0x1F;
	Data0 += 0x14;

	pSprite->field_36 = Data0;
	pSprite->field_1A = (int32*) 0x1C0000;
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

	Data2C->field_1A = (int32*) pSprite;

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
	Data2C->field_2C = -1;
	Data2C->field_52 = 0;
}

void cFodder::sub_21525( sSprite* pSprite ) {
	int16 Data0 = -3;
	int16 Data4 = 2;

	if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
		goto loc_21599;

	if (Data4 == 9 || Data4 == 0x0A) {

		if (pSprite->field_20 <= 1) {
			pSprite->field_12 = 1;
			return;
		}
	}
	//loc_21561
	if (Data4 == 4 || Data4 == 5 || Data4 == 6) {

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

	pSprite->field_0 = dword_3A391 >> 16;
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

int16 cFodder::sub_21618( sSprite* pSprite ) {

	if (pSprite->field_22) {

		int16 Data8 = pSprite->field_0;
		int16 DataC = pSprite->field_0;
		DataC += 6;
		int16 Data10 = pSprite->field_4;
		Data10 -= 9;
		int16 Data14 = pSprite->field_4;
		Data14 -= 4;
		word_3AA45 = 1;

		return Squad_Member_Sprite_Hit_In_Region( pSprite, Data8, DataC, Data10, Data14 );
	}
	//loc_21673
	int16 Data8 = pSprite->field_0;
	Data8 -= 6;

	int16 DataC = pSprite->field_0;
	DataC += 0x0A;

	int16 Data10 = pSprite->field_4;
	Data10 -= 0x0A;
	int16 Data14 = pSprite->field_4;
	Data14 += 6;

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

void cFodder::sub_21702( sSprite* pSprite, int16 pData18 ) {
	int16 Data0 = 2;
	sSprite* Data2C, *Data30;

	if (Sprite_Get_Free( Data0, Data2C, Data30 ))
		return;

	Data30 = Data2C + 1;
	Data2C->field_0 = pSprite->field_0;
	Data2C->field_2 = pSprite->field_2;
	Data2C->field_0 += 2;
	Data2C->field_4 = pSprite->field_4;
	Data2C->field_6 = pSprite->field_6;
	Data2C->field_4 -= 4;

	Data30->field_0 = Data2C->field_0;
	Data30->field_4 = Data2C->field_4;
	
	Data2C->field_1E = pSprite->field_1E;
	Data2C->field_20 = pSprite->field_20;
	Data30->field_20 = 0;
	Data2C->field_8 = 0xC5;
	Data30->field_8 = 0x7E;
	Data2C->field_A = 0;
	Data30->field_A = 0;
	Data2C->field_36 = 0x0C;
	Data30->field_36 = 0x0C;
	Data2C->field_18 = 0x2F;
	Data30->field_18 = eSprite_ShadowSmall;
	Data2C->field_52 = 0;
	Data30->field_52 = 0;
	Data2C->field_2C = 0;
	Data30->field_2C = -1;
	Data2C->field_38 = 0;

	Data2C->field_10 = pData18;
	Data2C->field_1A = (int32*)0x0A8000;

}

void cFodder::sub_2183B( sSprite* pSprite ) {

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

	if (dword_3B213[0x0E] == 0) {
		Data2C->field_4 -= 3;
	}
	else {
		Data2C->field_4 += 2;
		dword_3B213[0x0E] = 0;
	}

	Data2C->field_20 = pSprite->field_20;
	Data2C->field_52 = 0;
	Data2C->field_8 = 0xC6;
	Data2C->field_A = 0;
	Data2C->field_18 = 0x30;
}

void cFodder::sub_218E2( sSprite* pSprite ) {
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

	sSprite* Data20 = pData2C;

	int16 Data4 = word_390AE;
	Data4 &= 3;
	Data4 += 5;

	Sound_Play( Data20, Data4, 0x1E );

	pData2C->field_18 = eSprite_Building_Explosion;
	pData2C->field_8 = 0x8E;
	pData2C->field_A = 0;
	pData2C->field_12 = 1;
	pData2C->field_22 = -1;
	pData2C->field_32 = -1;
	pData2C->field_2C = 1;

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
	pData2C->field_22 = 1;

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
	pData2C->field_44 = Data4;
	sub_21C00(pData2C);

	mSprite_Enemy_AggressionCreated_Count += 1;
	mSprite_Enemy_AggressionCreated_Count &= 0x0F;
	if (!mSprite_Enemy_AggressionCreated_Count && mSprite_Enemy_AggressionMax < 0x1E)
		++mSprite_Enemy_AggressionMax;

	++mTroops_Enemy_Count;
	return 0;
}

int16 cFodder::Sprite_Get_Free2( int16& pData0, sSprite*& pData2C, sSprite*& pData30 ) {
	if (word_3B15F)
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
	word_3B15F = pData0;
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
	word_3B15F = pData0;
	return -1;
}

void cFodder::sub_21C00( sSprite* pData2C ) {
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

	Sprite->field_8 = 0x7C;
	Sprite->field_A = 0;
	Sprite->field_18 = eSprite_Player_Rank;
	Sprite->field_0 = 0;
	Sprite->field_4 = 0;
	Sprite->field_22 = -1;
	Sprite->field_10 = 1;
	Sprite->field_3A = 0;
}

void cFodder::sub_21CD1( sSprite* pSprite ) {
	mTroop_Cannot_Fire_Bullet = 0;
	mTroop_Cannot_Throw_Grenade = -1;
	word_3A010 = -1;
	word_3A8CF = 0;
	word_3AA41 = -1;

	int16 Dataa0, Data4, Data8, DataC, Data10;
	int16 Data1C = pSprite->field_5E;
	sSprite* Data0, *Data24= 0, *Data28 = 0;

	if (Data1C < 0) {

		pSprite->field_5E += 1;
		word_3AA41 = 0;

		Data0 = mSquad_Leader;
		if (mSquad_Leader == INVALID_SPRITE_PTR)
			goto loc_22125;

		goto loc_22053;
	}

	Data28 = mSquad_0_Sprites[Data1C];
	if (Data28 == INVALID_SPRITE_PTR)
		goto loc_21E4A;

	Dataa0 = tool_RandomGet() & 0x3F;
	if (Dataa0 == 0)
		goto loc_21E4A;

	if (Data28->field_0 == -32768)
		goto loc_21E4A;

	if (Data28->field_18 != 0)
		goto loc_21E4A;

	if (Data28->field_38 < 0x32) {
		if (Data28->field_38)
			goto loc_21E4A;
	}

	if (Data28->field_20 >= 0x0D)
		goto loc_21E4A;

	Dataa0 = pSprite->field_0;
	Data4 = pSprite->field_4;

	Data8 = Data28->field_0;
	DataC = Data28->field_4;

	Map_Get_Distance_BetweenPoints_Within_320( Dataa0, Data4, Data8, DataC );

	word_3AA4B = Dataa0;
	if (Dataa0 > 0xC8)
		goto loc_21E4A;

	if (Dataa0 <= 0x28)
		goto loc_21F77;

	Dataa0 = pSprite->field_0;
	Data4 = pSprite->field_4;
	Data8 = Data28->field_0;
	DataC = Data28->field_4;

	if (!sub_2A4A2( Dataa0, Data4, Data8, DataC )) {
		Dataa0 = word_3AA4B;
		goto loc_21F77;
	}

	if (word_3AA4B < 0x40)
		goto loc_21F8A;

loc_21E4A:;

	if (!pSprite->field_70)
		goto loc_21EE1;

	Data24 = pSprite->field_70;
	if (Data24->field_0 == -32768)
		goto loc_21E81;

	if (Data24->field_18 == eSprite_Hostage)
		goto loc_21E90;

loc_21E81:;
	pSprite->field_70 = 0;
	goto loc_21EE1;

loc_21E90:;
	pSprite->field_5E += 1;
	if (pSprite->field_5E >= 0x1E)
		pSprite->field_5E = 0;

	word_3AA41 = 0;
	pSprite->field_26 = Data24->field_0;
	pSprite->field_26 += 0x0C;
	pSprite->field_28 = Data24->field_4;
	return;

loc_21EE1:;

	Dataa0 = tool_RandomGet() & 0x0F;
	if (!Dataa0) {
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
	if (pSprite->field_5E <= 0x1E)
		goto loc_22000;

	pSprite->field_5E = 0;
	goto loc_22000;

loc_21F77:;
	pSprite->field_70 = 0;
	word_3AA41 = 0;

loc_21F8A:;
	if (Dataa0 > 0x87)
		goto loc_22000;

	//seg005:28A2
	// TODO: Does this bit even get used in the original
	//std::cout << "TODO: Where does Data30 come from?\n";

loc_22000:;
	if (word_3AA41)
		goto loc_22125;

	Dataa0 = pSprite->field_5E;

	if (mSquad_0_Sprites[Dataa0] == INVALID_SPRITE_PTR)
		goto loc_22125;

	Data0 = mSquad_0_Sprites[Dataa0];

loc_22053:;		// Movement Target?

	Data8 = (int16) (pSprite - mSprites);
	Data8 *= 0x76;
	Data8 &= 0x1FE;

	DataC = word_3EABF[Data8 / 2];
	Data8 += 0x80;
	Data8 &= 0x1FE;
	Data10 = word_3EABF[Data8 / 2];

	//seg005:29B2
	DataC >>= 8;
	Data10 >>= 8;
	DataC >>= 2;
	Data10 >>= 2;

	Data4 = Data0->field_0;
	pSprite->field_2E = Data4;

	Data4 += DataC;
	pSprite->field_26 = Data4;

	Data4 = Data0->field_4;
	pSprite->field_30 = Data4;

	Data4 += Data10;
	pSprite->field_28 = Data4;

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

	if (pSprite->field_38 == 7 || pSprite->field_38 == 5)
		goto loc_221D0;

	if (pSprite->field_38 == 6)
		goto loc_22235;

	return 0;
loc_221D0:;	// Building Explodes
	pSprite->field_38 = 6;
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
	pSprite->field_44 = Data4;
	pSprite->field_36 = 0x78;

loc_22235:;	// Door moving
	pSprite->field_44 -= 1;
	if (!pSprite->field_44)
		goto loc_2227F;

	Sprite_Movement_Calculate( pSprite );
	if (word_3B173)
		goto loc_2227F;

	Data0 = -3;
	Data4 = 2;
	if (Map_Terrain_Get_Type_And_Walkable( pSprite, Data0, Data4 ))
		goto loc_2227F;

	if (pSprite->field_36 <= 0)
		goto loc_2227F;

	return -1;

loc_2227F:;	// Door Finished
	pSprite->field_0 -= 8;
	pSprite->field_4 -= 8;
	
	sub_218E2( pSprite );

	return -1;
}

int16 cFodder::sub_222A3( sSprite* pSprite ) {
	int16 Data0, Data4;

	if (pSprite->field_38 != 7 && pSprite->field_38 != 5) {

		if (pSprite->field_38 == 6)
			goto loc_22339;

		return 0;
	}

	//loc_222CD
	pSprite->field_38 = 6;
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

	pSprite->field_44 = Data4;
	pSprite->field_36 = 0x6E;

loc_22339:;
	pSprite->field_44 -= 1;
	if (pSprite->field_44 == 1)
		goto loc_22383;

	Sprite_Movement_Calculate( pSprite );
	if (word_3B173)
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
	sub_218E2( pSprite );
	return -1;
}

void cFodder::sub_223B2( sSprite* pSprite ) {
	int16 Data8 = pSprite->field_3C;
	
	Data8 += word_3A8CF;
	Data8 &= 0x0E;

	pSprite->field_3C = Data8;

	int16* Data28 = off_32AE4[ pSprite->field_22 ];
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

	if (mSquad_CurrentVehicle)
		return 1;

	if (mSquad_Leader == INVALID_SPRITE_PTR || mSquad_Leader == 0 )
		return 1;

	if (mSquad_Leader->field_6E)
		return 1;

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

int16 cFodder::sub_224ED( sSprite* pSprite ) {
	sSprite* Data2C = 0, *Data30 = 0, *Data34 = 0;
	int16 Data0, Data4, Data8, DataC;

	if (mMission_Completed_Timer)
		goto loc_22592;

	if (byte_3A9D6[pSprite->field_22] == 2)
		goto loc_22592;

	if (!pSprite->field_2E)
		goto loc_22592;

	//seg005:2E39
	if (pSprite->field_18 != eSprite_Enemy)
		goto loc_22575;

	Data0 = pSprite->field_0;
	Data4 = pSprite->field_4;
	Data8 = pSprite->field_2E;
	DataC = pSprite->field_30;
	Map_Get_Distance_BetweenPoints_Within_320( Data0, Data4, Data8, DataC );
	if (Data0 >= 0x82)
		goto loc_22592;

loc_22575:;
	Data0 = 2;
	Sprite_Get_Free(Data0,Data2C,Data30);
	if (!Data0)
		goto loc_225BE;

	Data0 = -1;
	return -1;

loc_22592:;

	if (pSprite == mSquad_Leader)
		mMouse_Button_LeftRight_Toggle = 0;
	Data0 = -1;
	return -1;

loc_225BE:;
	if (pSprite->field_18 != 0)
		goto loc_22622;

	Data0 = pSprite->field_32;
	Data4 = Data0;

	if (!mSquad_Rockets[Data0])
		goto loc_22592;

	--mSquad_Rockets[Data0];
	mGUI_RefreshSquadRockets[Data4] = -1;
loc_22622:;
	Data0 = pSprite->field_22;

	++byte_3A9D6[Data0];
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
	if (pSprite->field_22)
		goto loc_22801;

	if (mVersion->mVersion != eVersion::AmigaPlus) {
		if (!(pSprite->field_75 & 1))
			goto loc_22801;
	}

	if (!Sprite_Homing_LockInRange(pSprite, Data34))
		goto loc_227EB;

	Data2C->field_1A = (int32*)Data34;
	Data2C->field_18 = eSprite_MissileHoming2;
	Data2C->field_6A = (sSprite*) 0x10000;
	Data2C->field_26 = pSprite->field_2E;
	Data2C->field_28 = pSprite->field_30;
	Data2C->field_28 += 0x10;
	Data2C->field_38 = 0;
	Data2C->field_36 = 0;
	Data30->field_36 = 0;
	goto loc_2281B;

loc_227EB:;
	Data2C->field_32 = 0;
	Data2C->field_18 = eSprite_Missile;
	goto loc_2281B;

loc_22801:;
	Data2C->field_18 = eSprite_Rocket;
	Data2C->field_46 = (int32*) pSprite;

loc_2281B:;
	Data30->field_18 = eSprite_ShadowSmall;
	Data2C->field_52 = 0;
	Data30->field_32 = 0;
	Data2C->field_22 = pSprite->field_22;
	Data30->field_22 = Data2C->field_22;
	Data2C->field_32 = pSprite->field_32;
	Data2C->field_2C = 0;
	Data30->field_2C = -1;
	Data2C->field_56 = 6;

	if (pSprite == mSquad_Leader)
		mMouse_Button_LeftRight_Toggle = 0;
	Data0 = 0;
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

void cFodder::sub_229C9( sSprite* pSprite ) {

	byte_3A9D6[pSprite->field_22] -= 1;
}

void cFodder::sub_22A3D( sSprite* pSprite ) {
	pSprite->field_0 = 1;
	pSprite->field_4 = 1;

	Sprite_Destroy_Wrapper_2( pSprite );
}

int16 cFodder::Sprite_Destroy_Wrapper_2( sSprite* pSprite ) {
	return Sprite_Destroy_Wrapper( pSprite );
}

void cFodder::sub_22AA9( sSprite* pSprite ) {
	pSprite->field_8 = 0x7C;
	pSprite->field_A = 0;
	pSprite->field_38 = 0;
	
	sSprite* Data24 = pSprite->field_6A;
	if( Data24->field_0 == -32768 || Data24->field_18 == eSprite_Explosion ) {
		//loc_22AEE
		pSprite->field_6E = 0;
		pSprite->field_66 = 0;
		pSprite->field_6A = 0;
		pSprite->field_38 = 1;
		pSprite->field_64 = -1;

		pSprite->field_10 = tool_RandomGet() & 0x1FE;
	}
	
	//loc_22B3C

	pSprite->field_0 = Data24->field_0;
	pSprite->field_4 = Data24->field_4;
	pSprite->field_20 = Data24->field_20;
}

void cFodder::Game_CheckLoadSave() {
	
	if( mGame_Load ) {
		Game_Load();
		return;
	} 

	if( mGame_Save ) {
		Game_Save_Wrapper2();
	}
}

void cFodder::Game_Setup( int16 pStartMap ) {
	Game_ClearVariables();

	mIntroDone = 0;
	mMission_Complete = 0;
	mMapNumber = pStartMap;
	word_3901E = 0x3333;

	Squad_Clear();
	Heroes_Clear();

	mGraveRankPtr = mGraveRanks;
	mGraveRanks[0] = -1;

	mMissionPhaseRemain = 1;
	mMissionNumber = 0;

	Mission_Phase_Next();

	mMission_TryAgain = -1;
	word_39096 = -1;

	mGraphics->LoadpStuff();
}

void cFodder::ExtractData() {
	
	std::vector< cResource_File >* Files = ((cResource_PC_CD*)mResources)->filesGet();
	for (std::vector< cResource_File >::iterator FileIT = Files->begin(); FileIT != Files->end(); ++FileIT) {
		size_t size;

		uint8* FileData = mResources->fileGet( FileIT->mName, size );
		std::string Filename = local_PathGenerate( FileIT->mName, "ExtractedData", true );
	
		std::ofstream outfile (Filename,std::ofstream::binary);
		outfile.write ((const char*) FileData, size );
		outfile.close();
	}

}

void cFodder::Start( int16 pStartMap ) {

Start:;
	mVersion = 0;
	VersionLoad( mVersions[0] );

	// Show the version selection, if we have more than 1 option
	if (mVersions.size() > 1)
		VersionSelect( );

	if (mVersion->mRelease == eRelease::Demo)
		pStartMap = 0;

	Mouse_Setup();
	Mouse_Inputs_Get();

	for (;;) {
		
		Game_Setup( pStartMap );

		//loc_1042E:;
		for (;;) {

			if (!mMission_Aborted && !mMission_TryAgain) {

				if (mVersion->mRelease == eRelease::Demo)
						break;

				if (mMapNumber == 71) {
					WonGame();
					return;
				}

				++mMapNumber;
				Mission_Phase_Next();
				word_3901E = 0x3333;
			}

			// loc_1045F
			Mission_Memory_Clear();
			Mission_Prepare_Squads();
			sub_10DEC();

			word_390A6 = 0;

			if (mVersion->mRelease != eRelease::Demo) {
				if (!mIntroDone)
					intro();
			}
			
			if (dword_3901A != 0x12345678)
				dword_3901A = 0x12345678;

			//loc_10496
			Squad_Set_Squad_Leader();
			Sprite_Clear_All_Wrapper();
			if (mGame_Prepare) {
				mGame_Prepare = 0;

				Map_Load_Sprites();
				word_3ABA7 = -1;

				Squad_Member_Count();
				Squad_Member_Sort();
				Squad_Prepare();
				Squad_Prepare_Sprites();

				word_3ABA7 = 0;
				WindowTitleSet( false );

				if (mVersion->mRelease == eRelease::Retail) {
					if (Recruit_Show()) {
						goto Start;
					}

					Game_CheckLoadSave();

					if (mGame_Load || mGame_Save) {
						word_390B8 = -1;
						mGame_Prepare = -1;
						mMission_Aborted = -1;
						continue;
					}
				} else {
					if (Demo_ShowMenu())
						goto Start;
				}

				word_390B8 = 0;
				Mission_Memory_Backup();
				word_3ABE9 = 0;
				word_3ABEB = 0;

				if (mVersion->mRelease == eRelease::Retail) 
					Briefing_Intro();
				else
					mGraphics->LoadpStuff();			
			}

			//loc_10513
			if (mVersion->mRelease == eRelease::Retail)
				Briefing_Prepare();
			
			WindowTitleSet( true );

			map_Load_Resources();
			Map_Load_Sprites();

			mMapWidth_Shifted = readLEWord( &mMap[0x54] ) << 4;
			mMapHeight_Shifted = readLEWord( &mMap[0x56] ) << 4;

			Squad_Member_Count();
			Squad_Member_Sort();
			Squad_Prepare();
			Squad_Prepare_Sprites();
			g_Graphics.graphicsBlkPtrsPrepare();
			mMission_Aborted = 0;

			if (mVersion->mRelease == eRelease::Retail) {
				Map_Overview_Prepare();
				Briefing_Wait();
			}

			if (word_3B4F5 == -1) {

				Mission_Memory_Restore();
				
				word_390B8 = -1;
				mGame_Prepare = -1;
				mMission_Aborted = -1;
				word_3901E = -1;

				mSound->Music_Play( 0 );
				continue;
			}

			Camera_Reset();
			mGraphics->SetSpritePtr( eSPRITE_IN_GAME );
	
			map_Tiles_Draw();
			Camera_Reset();
			map_SetTileType();
			Mouse_Inputs_Get();
			sub_18D5E();

			mSound->Music_Stop();
			mSound->Sound_Stop();

			Sprite_Handle_Loop();
			//seg000:05D1

			Sprite_Bullet_SetData();
			word_39F06 = 0;
			Sprite_Create_Rank( );
			mCamera_Start_Adjust = 1;
			mCamera_Position_X = mSprites[0].field_0;
			mCamera_Position_Y = mSprites[0].field_4;

			word_3909A = -1;
			word_3909A = 0;

			int16 Data0 = 0x0D;
			word_3ABE9 = 0;
			word_3ABEB = 0;
			word_3ABE7 = 0;

			Data0 = readLEWord( &mMap[0x54] );
			if (Data0 == 0x11) {
				Data0 = readLEWord( &mMap[0x56] );
				if (Data0 == 0x0C)
					word_3ABB7 = -1;
			}

			mGUI_Elements[0].field_0 = 0;
			word_390A6 = -1;
			sub_11CAD();

			word_3BEC1 = 0;
			word_3BEC3 = 4;
			mCamera_Start_Adjust = 1;

			Squad_Prepare_GrenadesAndRockets();
			Sprite_Aggression_Set();
			Mission_Phase_Goals_Set();

			word_3BEC9 = 0xE0;
			g_Graphics.PaletteSet();

			mImageFaded = -1;

			GUI_Sidebar_Prepare_Squads();
			Squad_Select_Grenades();
			Mission_Clear_Destroy_Tiles();
			Sprite_Count_HelicopterCallPads();
			sub_13148();

			mMission_Aborted = 0;
			mMission_Paused = 0;
			mMouseSpriteNew = 0x23;
			mMission_In_Progress = -1;
			mMission_Finished = 0;
			mMission_ShowMapOverview = 0;
			word_3EABD = 0x40BC;
			
			if (!Mission_Loop()) {
				mKeyCode = 0;
				mMission_In_Progress = 0;
				Squad_Member_PhaseCount();
				word_390EC = -1;
			}
			else {
				mKeyCode = 0;
				mMission_In_Progress = 0;
				if (!mSquad_AliveCount) {

					Service_Show();
					break;
				}
			}

			if (mVersion->mRelease == eRelease::Demo) 
				break;

			//loc_106F1
			if (mMission_TryAgain) {
				word_390EC = -1;
				continue;
			}

			if (mMission_Aborted)
				continue;

			if (mMissionPhaseRemain > 1)
				continue;

			Service_Show();
		}
	}
}

void cFodder::map_Tiles_Draw() {
	word_40054 = 0;
	word_3B60E = 0;
	word_3B610 = 0;

	mMapTilePtr = (0x60 - 8) - (readLEWord( &mMap[0x54] ) << 1);
	word_3B612 = 0;
	word_3B614 = 0;
	mCamera_Column_Previous = 0;
	mCamera_Row_Previous = 0;

	g_Graphics.map_Tiles_Draw();
}

void cFodder::Exit( unsigned int pExitCode ) {

	exit( pExitCode );
}

void cFodder::GUI_Sidebar_Grenades_Draw( ) {
	int16 Data0 = mGUI_Squad_Current;
	int16 Data8, DataC, Data4, Data10;

	if (!mSquad_Grenades[mGUI_Squad_Current])
		goto loc_2ED63;

	if (mSquad_CurrentWeapon[mGUI_Squad_Current] != eWeapon_Grenade)
		goto loc_2ED63;

	if (!word_3AC47) {
	loc_2ED63:;
		Data0 = 0xC4;
	}
	else {
		Data0 = 0xC3;
	}

	// Draw Grenade Icon
	Data8 = 0;
	DataC = word_3AC1D;
	DataC += 0x0E;

	mGraphics->sub_145AF( Data0, Data8, DataC );
	
	Data0 = mSquad_Grenades[mGUI_Squad_Current];
	if (!Data0)
		return;

	Data4 = 0;
	Data8 = 0x0D;
	DataC = word_3AC1D;
	DataC += 0x19;
	Data10 = 0xAF;

	if (mSquad_CurrentWeapon[mGUI_Squad_Current] != eWeapon_Grenade)
		goto loc_2EDF7;

	if (word_3AC47)
		goto loc_2EDFD;

loc_2EDF7:;
	Data10 = 0xB9;

loc_2EDFD:;
	GUI_Sidebar_Number_Draw( Data0, Data4, Data8, DataC, Data10 );

}

void cFodder::GUI_Prepare_Button_Grenade() {
	if (!word_3AC47)
		return;

	sGUI_Element* NewElement = mGUI_NextFreeElement;
	NewElement->field_0 = &cFodder::GUI_Button_NoAction;
	NewElement->mX = 0;
	NewElement->mWidth = 0x0C;
	NewElement->mY =  word_3AC1D + 0x0E;
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

	word_3AC49 = mSquad_Grenade_SplitMode;
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
	int16 DataC = word_3AC1D;

	int8 Data4_Saved = Data4;

	DataC += 0x22;
	Data4 -= 1;
	if (Data4 < 0)
		goto loc_2F1BC;

	for (;Data4 >= 0;--Data4) {

		if (!word_3AC47)
			Data0 = 0xAB;
		else
			Data0 = 0xA9;

		mGraphics->sub_145AF( Data0, 0, DataC );
		DataC += 0x0C;
	}
loc_2F1BC:;

	mGUI_Squad_NextDraw_Y = DataC;
	Data4 = Data4_Saved;
	if (Data4 < 0)
		return;

	sSquad_Member* Data38 = mSquad;

	DataC = word_3AC1D;
	DataC += 0x23;
	word_3A3BD = DataC;

	for (int16 word_3A061 = 7; word_3A061 >= 0; --word_3A061, ++Data38 ) {
		
		sSprite* Data34 = Data38->mSprite;

		if (Data34 == INVALID_SPRITE_PTR || Data34 == 0)
			continue;

		if (mGUI_Loop_Squad_Current != Data34->field_32)
			continue;

		if (Data34->field_38 >= 0x32)
			goto loc_2F25C;

		if ((Data34->field_75 & 2))
			goto loc_2F25C;

		if (Data34->field_38)
			continue;

	loc_2F25C:;
		Data0 = 0;
		Data0 = Data38->mRank;
		Data0 += 9;
		int16 Data8 = 0;
		int16 DataC = word_3A3BD;

		mGraphics->sub_145AF( Data0, Data8, DataC );
		Data0 = Data38->mRank + 9;
		Data8 = 0x23;
		DataC = word_3A3BD;

		mGraphics->sub_145AF( Data0, Data8, DataC );
		Data0 = 0;

		const sRecruit* Data28 = &mRecruits[Data38->mRecruitID];
		if (!word_3AC47) {
			Data0 = 2;
		}
		else {
			Data0 = 0;
			Data0 = Data38->mSelected;
			Data0 &= 1;
		}
		//loc_2F318
		Data4 = 3;
		word_3A8B1 = 6;
		DataC = word_3A3BD;
		DataC += 2;

		GUI_Sidebar_TroopList_Name_Draw( Data0, Data4, Data8, DataC, Data28->mName );
		word_3A3BD += 0x0C;
	}
}

void cFodder::GUI_Sidebar_TroopList_Name_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, const char* pData28 ) {

	word_3AA21 =  byte_3DF02[pData0];

	int16 Data14;

	for (Data14 = 0; Data14 <= word_3BDEB; ++Data14) {
		if (word_3BDEB == 5) {

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

	for( Data14 = 0; Data14 <= word_3BDEB; ++Data14 ) {
			
		if( pData28[Data14] != 0x20 ) {
			pData0 = pData28[Data14];
			pData0 -= 0x41;
			pData0 += word_3AA21;
				
			pData8 = Data14;
			pData8 <<= 2;
			pData8 += word_3A05F;

			mGraphics->sub_145AF( pData0, pData8, pDataC );
		}
	}
}

void cFodder::sub_2F452() {

	sub_2F4CB();
	GUI_Sidebar_SquadIcon_Draw();
}

void cFodder::GUI_Sidebar_SquadIcon_Draw() {
	
	int16 Data0 = word_3AA11[mGUI_Squad_Current];

	const int16* Data20 = word_3DF05;

	if (!word_3AC47) 
		Data20 = word_3DF1B;

	Data0 = Data20[Data0];
	int16 Data8 = 0x0C;
	int16 DataC = word_3AC1D;
	DataC += 0x0E;

	mGraphics->sub_145AF( Data0, Data8, DataC );
}

int16 cFodder::sub_2F4CB() {
	int16 Data1C = 0;
	sSprite* Data2C = 0;
	const int16* Dataa2C = word_3DF31;
	int16 Data8 = mSquads_TroopCount[ mGUI_Loop_Squad_Current ];
	if (!Data8)
		return 0;

	if (Data8 != 1)
		Data1C = 1;

	int16 DataC = mGUI_Squad_Current;

	if (!dword_3B24F[DataC])
		goto loc_2F593;

	Data2C = dword_3B24F[DataC];
	DataC = Data2C->field_6F;

	for (;; Dataa2C += 2) {
		if (*Dataa2C < 0)
			goto loc_2F593;

		if (DataC == *Dataa2C)
			goto loc_2F586;
	}
loc_2F586:;
	Data1C = *(Dataa2C + 1);

loc_2F593:;
	if (Data1C == word_3AA11[mGUI_Squad_Current])
		return 0;

	word_3AA11[mGUI_Squad_Current] = Data1C;
	return -1;
}

void cFodder::GUI_Sidebar_Refresh() {
	Mission_Final_TimeToDie();
	GUI_Sidebar_SquadIcon_Refresh();
	
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

		GUI_Sidebar_Grenades_Refresh( Data0 );

	loc_2F707:;

		if (!*Data24)
			continue;

		*Data24 = 0;

		GUI_Sidebar_Rockets_Refresh( Data0 );
	}
}

void cFodder::GUI_Sidebar_SquadIcon_Refresh() {
	
	mGUI_Loop_Squad_Current = 0;
	mGUI_Squad_Current = 0;
	
	for (mGUI_Loop_Squad_Current = 0; mGUI_Loop_Squad_Current < 3; ++mGUI_Loop_Squad_Current, ++mGUI_Squad_Current) {

		word_3AC1D = word_3AC2D[mGUI_Squad_Current];

		if (!mSquads_TroopCount[word_3AC1D])
			continue;

		word_3AC47 = 0;

		if (mGUI_Squad_Current == mSquad_Selected)
			word_3AC47 = -1;

		if (sub_2F4CB())
			GUI_Sidebar_SquadIcon_Draw();
	}
}

void cFodder::GUI_Sidebar_Grenades_Refresh_CurrentSquad( ) {
	GUI_Sidebar_Grenades_Refresh( mSquad_Selected );
}

void cFodder::GUI_Sidebar_Grenades_Refresh(  int16 pData0 ) {

	int16 Data4 = word_3AC2D[pData0];

	sub_2EBE0(pData0, Data4);
	GUI_Sidebar_Grenades_Draw();

	if (!mSquad_Grenades[mGUI_Squad_Current])
		return;

	if (!word_3AC47)
		return;

	if (!GUI_Sidebar_SelectedTroops_Count())
		return;

	// Grenades not selected
	if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_None)
		return;

	int16 Data0 = 0xC7;
	Data0 += mSquad_Grenade_SplitMode;

	int16 Data8 = 0;
	int16 DataC = word_3AC1D;
	DataC += 0x0E;

	mGraphics->sub_145AF( Data0, Data8, DataC );
}

void cFodder::GUI_Sidebar_Rockets_Refresh_CurrentSquad( ) {

	GUI_Sidebar_Rockets_Refresh( mSquad_Selected );
}

void cFodder::GUI_Sidebar_Rockets_Refresh(  int16 pData0 ) {

	int16 Data4 = word_3AC2D[pData0];
	sub_2EBE0( pData0, Data4 );
	GUI_Sidebar_Rockets_Draw();

	if (!mSquad_Rockets[mGUI_Squad_Current])
		return;

	if (!word_3AC47)
		return;

	if (!GUI_Sidebar_SelectedTroops_Count())
		return;

	if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_None)
		return;

	pData0 = 0xC7;
	pData0 += mSquad_Rocket_SplitMode;

	int16 Data8 = 0x24;
	int16 DataC = word_3AC1D;
	DataC += 0x0E;

	mGraphics->sub_145AF( pData0, Data8, DataC );
}

int16 cFodder::GUI_Sidebar_SelectedTroops_Count() {
	sSquad_Member* Data38 = mSquad;

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
		return Data0;

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
			sub_2FA8F();
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

	sub_30465();
	word_3AC51 = -1;
}

void cFodder::GUI_Sidebar_MapButton_Prepare() {

	mGUI_NextFreeElement = mGUI_Elements;

	map_ClearSpt();

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

void cFodder::sub_2FA8F() {
	sub_2EBC4();
	sub_30480();

	word_3AC51 = 0;
	mMouseDisabled = 0;
	mMouse_Button_LeftRight_Toggle = 0;
}

void cFodder::GUI_Sidebar_Rockets_Draw( ) {
	int16 Data0, Data8, DataC, Data4, Data14, Data10;

	if (!mSquad_Rockets[mGUI_Squad_Current])
		goto loc_2FAF6;

	if (mSquad_CurrentWeapon[mGUI_Squad_Current] != eWeapon_Rocket)
		goto loc_2FAF6;


	if (!word_3AC47) {
		loc_2FAF6:;
		Data0 = 0xC6;
	}
	else {
		Data0 = 0xC5;
	}

	//loc_2FB04
	Data8 = 0x20;
	DataC = word_3AC1D;
	DataC += 0x0E;
	mGraphics->sub_145AF( Data0, Data8, DataC );

	Data0 = mSquad_Rockets[mGUI_Squad_Current];
	if (!Data0)
		return;

	Data4 = 0x24;
	Data8 = 0x0D;
	DataC = word_3AC1D;
	DataC += 0x19;
	Data14 = mGUI_Squad_Current;

	if (mSquad_CurrentWeapon[Data14] != eWeapon_Rocket)
		goto loc_2FB8A;

	Data10 = 0xAF;
	if (!word_3AC47) {
	loc_2FB8A:;
		Data10 = 0xB9;
	}

	GUI_Sidebar_Number_Draw( Data0, Data4, Data8, DataC, Data10 );
}

void cFodder::GUI_Prepare_Button_Rockets() {

	if (!word_3AC47)
		return;

	sGUI_Element* NewElement = mGUI_NextFreeElement;
	NewElement->field_0 = &cFodder::GUI_Button_NoAction;
	NewElement->mX = 0x24;
	NewElement->mWidth = 0x0C;
	NewElement->mY = word_3AC1D + 0x0E;
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

	word_3AC4F = mSquad_Rocket_SplitMode;
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
	word_3AEFB = word_3AC1D;
	sGUI_Element* Element = mGUI_NextFreeElement;

	Element->field_0 = &cFodder::GUI_Button_NoAction;
	Element->mX = 0;
	Element->mWidth = 0x2F;
	Element->mY = word_3AC1D + 0x22;

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
	Data0 -= word_3BEC3;
	Data0 -= word_3AEFB;
	Data0 -= 0x22;
	Data0 /= 0x0C;

	int16 Data4 = mSquads_TroopCount[mSquad_Selected];

	if (Data0 >= Data4)
		return;

	word_3A3BF = Data0;

	sSquad_Member* Data38 = mSquad;
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

	Data8 += word_3AEFB;
	Data8 += 0x25;
	word_3A8B1 = 6;
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
	int16 Data18 = mSquad_Selected;

	word_39FD2 = Data18;
	int16 Data10 = Data14;

	sMapTarget* Dataa34 = mSprite_WalkTargets[Data10];
	Dataa34->asInt = -1;

	sSquad_Member* Member = mSquad;

	for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Member ) {
		sSprite* Data34 = Member->mSprite;

		if (Data34 == INVALID_SPRITE_PTR || Data34 == 0)
			continue;

		if (Data18 != Data34->field_32)
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
	Squad_Clear_Selected();

	return -1;
}

void cFodder::Squad_Split_Assets() {
	int16 Data8, DataC;

	if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_All) {
		mSquad_Grenades[mSquad_Selected] = mSquad_Grenades[word_39FD2];
		mSquad_Grenades[word_39FD2] = 0;
	}
	else {

		if (mSquad_Grenade_SplitMode == eSquad_Weapon_Split_Half) {
			Data8 = mSquad_Grenades[word_39FD2];
			DataC = Data8;
			Data8 >>= 1;

			mSquad_Grenades[word_39FD2] = Data8;
			DataC -= Data8;
			mSquad_Grenades[mSquad_Selected] = DataC;
		}
	}

	if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_All) {
		mSquad_Rockets[mSquad_Selected] = mSquad_Rockets[word_39FD2];
		mSquad_Rockets[word_39FD2] = 0;
	}
	else {

		if (mSquad_Rocket_SplitMode == eSquad_Weapon_Split_Half) {

			Data8 = mSquad_Rockets[word_39FD2];
			DataC = Data8;
			Data8 >>= 1;

			mSquad_Rockets[word_39FD2] = Data8;
			DataC -= Data8;
			mSquad_Rockets[mSquad_Selected] = DataC;
		}
	}

	mSquad_CurrentWeapon[mSquad_Selected] = mSquad_CurrentWeapon[word_39FD2];
}

void cFodder::GUI_Sidebar_MapButton_RenderWrapper() {

	if (mMapNumber == 0x47)
		return;

	// No the map overview button in the Demo versions
	if (mVersion->mRelease == eRelease::Demo)
		return;

	word_3B4D9 = 0;
	GUI_Sidebar_MapButton_Render();
}

void cFodder::GUI_Sidebar_Number_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, int16 pData10 ) {
	pData10 -= 0x1A;
	int16 Data10_Saved = pData10;

	int16 word_3AC23 = pData4;
	int16 word_3AC27 = pDataC;

	std::stringstream Tmp;
	Tmp << tool_StripLeadingZero( tool_NumToString( pData0 ) );

	String_CalculateWidth( pData8, mFont_Sidebar_Width, Tmp.str().c_str() );
	
	pData8 = mGUI_Temp_X + word_3AC23;
	int16 Data0 = Data10_Saved;
	word_3AC21 = -1;
	pDataC = word_3AC27;

	String_Print(  mFont_Sidebar_Width, Data0, pData8, pDataC, Tmp.str().c_str() );
	
	word_3AC21 = 0;
}

int16 cFodder::Mouse_Button_Left_Toggled() {
	if (mMouse_Button_Left_Toggle >= 0) {
		//pData0 = -1;
		return -1; 
	}
	mMouse_Button_Left_Toggle = 1;
	return 1;
}

void cFodder::sub_30E49() {
	word_3A3BF = -1;
	int16 Data1C = 0x07;
	sSquad_Member* Data24 = mSquad;

	int16 Data0 = mMouseX;
	int16 Data4 = mMouseY;

	Data0 += mCamera_Adjust_Col >> 16;
	Data4 += mCamera_Adjust_Row >> 16;
	Data0 -= 0x0F;
	Data4 -= 3;

	for (; Data1C >= 0; --Data1C, ++Data24 ) {

		if ( Data24->mSprite == INVALID_SPRITE_PTR || Data24->mSprite == 0 )
			continue;

		sSprite* Data20 = Data24->mSprite;
		int16 Data10 = Data20->field_32;

		if (Data10 == mSquad_Selected)
			++word_3A3BF;

		Data10 = Data20->field_0 + 4;

		int16 Data14 = Data20->field_4;
		Data14 -= Data20->field_20;
		Data14 -= 0x0D;
		if (Data0 < Data10)
			continue;

		Data10 += 6;
		if (Data0 > Data10)
			continue;

		if (Data4 < Data14)
			continue;

		Data14 += 0x0D;
		if (Data4 > Data14)
			continue;

		Data10 = Data20->field_32;
		Data14 = mSquad_Selected;

		if (Data14 < 0) {
			Data0 = Data10;
			return;
		}

		if (Data14 == Data10) {
			word_3ABC7 = -1;
			dword_3ABC9 = (sSquad_Member*) Data20->field_46;
			return;
		}

		Data0 = mSquads_TroopCount[Data10] + mSquads_TroopCount[Data14];
		if (Data0 > 8)
			return;

		word_3BF1E[Data14] = Data20;
		byte_3BF1B[Data14] = Data10;
		Data0 = Data10;

		sub_2DCB0( Data0 );

		break;
	}
}

void cFodder::Squad_Clear_Selected() {
	sSquad_Member* Data38 = mSquad;
	
	for (int16 Data1c = 7; Data1c >= 0; --Data1c, ++Data38) {
		Data38->mSelected &= 0;
	}
}

void cFodder::sub_303AE() {
	GUI_Sidebar_Grenades_Refresh_CurrentSquad();
	sub_303DA();
}

void cFodder::sub_303B7() {
	if (mSquad_Selected < 0)
		return;

	mGraphics->sub_145AF( mSquad_Selected, 0, word_3AC1D );
}

void cFodder::sub_303DA() {
	int16 Data0;

	for (Data0 = 0; Data0 < 3; ++Data0) {
		if (!mSquads_TroopCount[Data0])
			goto loc_30409;

	}
	return;

loc_30409:;

	int16 Data4 = mSquad_Selected;
	if (Data4 < 0)
		return;

	int16 Data8 = Data4;
	Data4 += Data8;
	Data4 += Data0;

	Data0 = word_3DF73[Data4];
	int16 DataC = word_3AC1D;

	mGraphics->sub_145AF( Data0, Data8, DataC );
}

void cFodder::sub_30465() {

	for (int16 cx = 0; cx < 0x2000; ++cx){
		word_3BDAD[cx] = mMapSptPtr[cx];
	}

	word_3D5B7 = word_3BDAD;
}

void cFodder::sub_30480() {

	for (int16 cx = 0; cx < 0x2000; ++cx){
		mMapSptPtr[cx] = word_3BDAD[cx];
	}

	word_3D5B7 = mMapSptPtr;
}

void cFodder::Squad_Switch_Weapon() {
	
	if (!word_390A6 || mMission_Finished)
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
	if (mMapNumber != 0x47)
		return;

	++word_3B4D3;
	if (word_3B4D3 < 0x28)
		return;

	word_3B4D3 = 0;

	--word_3B4D5;
	if (word_3B4D5 < 0)
		word_3B4D5 = 0;

	word_3BDEB = 0x0A;

	GUI_Sidebar_TroopList_Name_Draw( 0, 0, 0, 0xB7, "TIME TO DIE" );

	int16 di = 0x900;
	for (int16 dx = 4; dx > 0; --dx) {
		int16 di2 = di / 2;

		for (int16 cx = 0x12; cx > 0; --cx) {
			mMapSptPtr[di2] = 0;
			mMapSptPtr[di2+1] = 0;
			di2+=2;
		}
		di += 0x960;
	}

	GUI_Sidebar_Number_Draw( word_3B4D5, 0, 0x30, 0xC0, 0xAF );
	word_3BDEB = 5;
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

		if (pData20->field_75 & 2)
			goto loc_3066E;

		if (pData20->field_38) {
			if (pData20->field_38 < 0x32)
				goto loc_30662;
		}
		//loc_30656
		if (!pData20->field_5B)
			goto loc_30681;

	loc_30662:;
		if (!pData20->field_6E)
			continue;

	loc_3066E:;
		pData20->field_38 = 0;
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
		sub_30AB0();

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
		Data0 = word_3BEC1 + Data20->mX;

		int16 Data4 = mMouseX + 0x20;

		if (Data0 > Data4)
			continue;

		Data0 += Data20->mWidth;
		if (Data0 < Data4)
			continue;

		Data0 = word_3BEC3;
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
		mMouseSpriteNew = 0x24;
		word_3A9F3 = -8;
		word_3A9F5 = -8;
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

	if (word_3AA43)
		return;

	if (mMouseSpriteNew < 0) {
		mMouseSpriteNew = 0x23;
		word_3A9F3 = 0;
		word_3A9F5 = 0;
	}

	if (mSquad_Selected < 0) {

		sub_30E49();
		return;
	}

	sSprite** Data24 = mSquads[mSquad_Selected];
	sSprite* Dataa24 = *Data24;

	if (Dataa24 == INVALID_SPRITE_PTR) {
		sub_30E49();
		return;
	}

	if (Dataa24->field_6E)
		return;

	if (word_3B2F1)
		word_3B2F1 = 0;
	else
		sub_311A7();

	word_3ABC7 = 0;
	sub_30E49();

	if (word_3ABC7) {

		int8* Data20 = mSquads_TroopCount;
		int8 al;

		//TODO 
		// Why is this here??
		for (Data0 = 2; Data0 >= 0; --Data0) {
			al = *Data20++;
			if (!al)
				return;
		}
		return;
	}

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

	dword_3A3F9 = mSprite_WalkTargets[mSquad_Selected];

	sSquad_Member* SquadMember = mSquad;

	int8 Data14 = 0;

	for (int16 Data18 = 7; Data18 >= 0; --Data18, ++SquadMember) {

		if (SquadMember->mSprite == INVALID_SPRITE_PTR || SquadMember->mSprite == 0 )
			continue;
		
		sSprite* tmp = SquadMember->mSprite;
		if (mSquad_Selected != tmp->field_32)
			continue;

		tmp->field_44 = Data14;
		tmp->field_2 = 0;
		tmp->field_6 = 0;
	}

	int16 Data10 = mSquad_Leader->field_0;
	int16 DataC = mSquad_Leader->field_32;
	int16 Data8 = Data4;
	Data4 = Data0;

	Squad_Walk_Target_Set( Data4, Data8, DataC, Data10 );

	for (Data0 = 2; Data0 >= 0; --Data0) {

		if (mSquad_Selected != byte_3BF1B[Data0])
			continue;

		byte_3BF1B[Data0] = -1;
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
