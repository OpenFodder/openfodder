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
#include "SpriteData.hpp"
#include "IntroData.hpp"
#include "Recruits.hpp"
#include "UnknownData.hpp"
#include "MissionNames.hpp"
#include "VocTable.hpp"

#define INVALID_SPRITE_PTR (sSprite_0*) -1

const char* mBinTable[] = { "rjnull.bin", 
							"rjnull.bin", 
							"player.bin", 
							"player.bin", 
							"player.bin", 
							"player.bin" };

std::string mMapTypes[] = {
	"jun",
	"des",
	"ice",
	"mor",
	"int",
	"hid"
};

void cFodder::Music_Play( const char* pFilename ) {

	std::string Filename = "Data\\WAV\\";
	Filename.append( pFilename );
	Filename.append( ".wav" );

	Mix_FreeMusic( mMusicPlaying );
	Sleep( 100 );

	mMusicPlaying = Mix_LoadMUS( Filename.c_str() );

	Mix_PlayMusic( mMusicPlaying, -1 );
}

void cFodder::Music_Stop() {

	Mix_FadeOutMusic(500);
}

cFodder::cFodder( bool pSkipIntro ) {
	
	mSkipIntro = pSkipIntro;
	mMusicPlaying = 0;

	mResources = new cResources();
	mWindow = new cWindow();
	mImage = 0;

	mTicksDiff = 0;
	mTicks = 0;

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
	mMouse_Button_Left_Toggle = word_39F02 = 0;
	word_39F06 = 0;
	word_39EFA = 0;
	word_39EFC = word_39F04 = 0;

	word_39FF2 = 0;
	word_39FF4 = 0;
	word_39FF6 = 0;
	word_39FF8 = 0;
	word_39FFA = 0;

	word_3A9B0 = 0;
	word_3A9B2 = 0;
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
	word_3B44F = 0;
	word_3B4F5 = 0;
	word_3E1B7 = 0;
	mCamera_Pan_RowCount = 0;

	mMouseSpriteCurrent = 0;
	word_40048 = 0;
	word_40054 = 0;
	word_42062 = 0;
	word_42066 = 0;
	mDrawSpritePositionX = 0;
	mDrawSpritePositionY = 0;
	word_4206C = 0;
	word_4206E = 0;
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
	mouseData0 = new sMouseData();
	mouseData1 = new sMouseData();

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
		word_3E0E5[x] = 0;
	}

	word_44475 = 0;

	stru_3DEDC[0] = { &cFodder::sub_2EAC2, 0, 47, 0, 225, &cFodder::sub_2EAC3 };
	stru_3DEDC[1] = { 0, 0, 0, 0, 0, 0 };

	mGUI_Handle_Button_SelectSquad_Array[0] = &cFodder::GUI_Handle_Button_SelectSquad_0;
	mGUI_Handle_Button_SelectSquad_Array[1] = &cFodder::GUI_Handle_Button_SelectSquad_1;
	mGUI_Handle_Button_SelectSquad_Array[2] = &cFodder::GUI_Handle_Button_SelectSquad_2;

	for (unsigned int x = 0; x < 0x3C; ++x) {
		dword_42320[x].mBuffer = 0;
		dword_42410[x].mBuffer = 0;
		dword_42500[x].mBuffer = 0;
		dword_425F0[x].mBuffer = 0;
		dword_426E0[x].mBuffer = 0;
	}

	word_42316[0] = dword_42320;
	word_42316[1] = dword_42410;
	word_42316[2] = dword_42500;
	word_42316[3] = dword_425F0;
	word_42316[4] = dword_426E0;
}

cFodder::~cFodder() {
	
	delete mSurfaceMapOverview;

	for (unsigned int x = 0; x < 0x3C; ++x) {

		delete[] dword_42320[x].mBuffer;
		delete[] dword_42410[x].mBuffer;
		delete[] dword_42500[x].mBuffer;
		delete[] dword_425F0[x].mBuffer;
		delete[] dword_426E0[x].mBuffer;
	}
}

int16 cFodder::Mission_Loop( ) {
	//mImage->clearBuffer();
	mImage->Save();

	for (;;) {
		videoSleep_50();

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
		mImage->Restore();

		sub_12018();
		Camera_Pan( );

		//mImage->Save();

		word_3A9FB = 0;
		Mouse_Handle();
		++word_390B0;
		word_39F06 = 0;
		word_3A9FB = -1;

		if (word_3AA1B >= 0) {
			sub_11CD6();
			word_3AA1B = -1;
			continue;
		}
		//loc_1074E
		if (word_3AC2B >= 0 && !word_3A9AA)
			sub_2F9B3();
		else
			sub_2F5ED();

		//loc_10768
		sub_12790();
		word_3B489 = 0;
		if (word_3A9B8 >= 0)
			--word_3A9B8;

		sub_12AEE();

		if (word_3FA21 == -1 && mKeyCode == 0x1C)
			mMissionComplete = -1;
		else
			Mission_Goals_Check();

		//loc_1079C
		sub_2A470();
		sub_2D06C();

		Mission_Sprites_Handle();
		sub_31033();
		sub_144A2();
		Mouse_DrawCursor();

		if (word_3A9D0 != 0) {
			//Mission_Paused();
			//video_?_0();

			while (word_3A9D0) {
				eventProcess();
			}
			mPaused = -1;

		}
		else {
			//video_?_0
		}

		if( mImageFaded == -1 )
			mImageFaded = mImage->paletteFade();

		sub_11FCD();
		if (mPaused == -1) {
			//video_Palette_?_0
			mPaused = 0;
		}

		//sub_14445();
		if (mMission_ShowMapOverview) {
			//sub_12D00();
			mMission_ShowMapOverview = 0;
		}

		if (word_3A3B9) {
			if (word_3A9B2)
				sub_2D767(); 

			if (!word_3A9AA)
				return 0;

			sub_2D725();

			return -1;
		}

		//loc_10841
		sub_124DB();
		sub_12C69();
		sub_12B6E();

		// No squad is selected, so set count down timer
		if (mSquad_Selected < 0 && !word_3AA47)
			word_3AA47 = 0x14;

		sub_13155();
		sub_131A2();
	}

	return 0;
}

void cFodder::Mouse_Handle( ) {
	sSprite_0* Sprite = 0;

	if (!word_390A6 || !word_3A9D0) {

		++word_39F06;
		word_390A4 = -1;
		++word_390AE;

		Mouse_Inputs_Get();

		if (word_390A6) {
			sub_10937();
			sub_10937();
			sub_10937();

			if (!word_3B4F1)
				Mouse_Inputs_Check();
		}
	}

	//loc_108AE
	if (word_3A9B4) {
		--word_3A9B4;
		sub_304D0();
	}

	if (word_3AA51)
		Mouse_DrawCursor();

	if (!word_390A6)
		return;
	if (!word_3B20F)
		return;

	if (word_3A9AE || word_3D469 || mMissionComplete || word_3A9AA || word_3A9B2 )
		goto loc_10930;

	int16 Data0 = word_390AE;
	Data0 &= 3;

	if (!word_3B44F)
		return;

	if ((byte_427E6 | byte_427EE))
		return;

	int16 Data4 = word_3B44F;
	int16 Data8 = 0;
	Sound_Voc_Play( Sprite, Data4, Data8 );
	word_3B44F = 0;
	return;

loc_10930:;
	word_3B4F1 = -1;

}

void cFodder::sub_10937() {
	
	if (word_3B20F == -1) {
		sub_1229C();
		sub_12245();
		sub_11B06();
		sub_120F6();
		Camera_Position_Update();
		sub_1096B();
		sub_122BD();
	}
}

void cFodder::sub_1096B() {
	sSprite_0* Data14 = word_39FCE;
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
	
	sub_29E30( Data0, Data4, Data8, DataC );
	
	if( dword_3B20B ) {
	
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
	

	if(sub_29EC2(Data0, Data4, Data8,DataC) < 0 )
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

void cFodder::sub_10BBC() {
	word_390A4 = 0;
	word_390A6 = 0;
	word_390AE = 0;
	word_390B0 = 0;
	word_390B8 = 0;
	word_390BE = 0;
	word_390C0 = 0;
	word_390C2 = 0;
	word_390C4 = 0;
	word_390C6 = 0;
	mMapNumber = 0;
	mMissionNumber = 0;
	mMissionPhase = 0;
	word_39096 = 0;
	word_3909A = 0;
	word_390CE = 0;
	word_390D0 = 0;
	word_390D2 = 0;
	word_390D4 = 0;

	for (unsigned int x = 0; x < 8; ++x) {
		word_390D6[x] = 0;
	}

	word_390E6 = 0;
	word_390E8 = 0;
	word_390EA = 0;
	word_390EC = 0;
	mMissionPhaseRemain = 0;
	mMissionPhases = 0;
	word_390F4 = 0;
	word_390F8 = 0;

	for (unsigned int x = 0; x < 9; ++x) {
		mSquad[x].mRecruitID = 0;
		mSquad[x].mRank = 0;
		mSquad[x].field_3 = 0;
		mSquad[x].field_4 = 0;
		mSquad[x].field_6 = 0;
		mSquad[x].field_8 = 0;
		mSquad[x].mNumberOfKills = 0;
	}

	for (unsigned int x = 0; x < 361; ++x)
		word_391D2[x] = 0;

	dword_394A4 = 0;
	dword_394A8 = 0;

	for (unsigned int x = 0; x < 361; ++x)
		word_394AC[x] = 0;

	dword_3977E = 0;

	for (unsigned int x = 0; x < 6; ++x) {
		mHeroes[x].mRecruitID = 0;
		mHeroes[x].mRank = 0;
		mHeroes[x].mKills = 0;
	}
	word_397AC = 0;
	word_397AE = 0;
	mMapPlayerTroopCount = 0;
	mSquadMemberCount = 0;
	mTroopsAvailable = 0;
}

void cFodder::Troops_Clear() {
	mSquad[8].mRecruitID = -1;
	mSquad[8].field_4 = INVALID_SPRITE_PTR;

	for (unsigned int x = 0; x < 8; ++x) {
		mSquad[x].field_4 = INVALID_SPRITE_PTR;
		mSquad[x].mRecruitID = -1;
		mSquad[x].mRank = 0;
		mSquad[x].field_3 = 0;
	}
}

void cFodder::Heroes_Clear() {
	
	for (unsigned int x = 0; x < 6; ++x) {
		mHeroes[x].mRecruitID = -1;
		mHeroes[x].mRank = -1;
		mHeroes[x].mKills = -1;
	}
}

void cFodder::sub_10CE7() {
	
}

void cFodder::sub_10D61() {
	// Clear memory 2454 to 3B58
	mButtonPressLeft = 0;
	mButtonPressRight = 0;
	mMouse_Button_Left_Toggle = 0;
	word_39EFA = 0;
	word_39EFC = 0;

	word_39F00 = 0;
	word_39F02 = 0;
	word_39F04 = 0;
	word_39F06 = 0;

	mCamera_Adjust_Row = 0;
	dword_39F36 = 0;
	word_39F3A = 0;
	word_39F3C = 0;
	word_39F3E = 0;
	word_39F40 = 0;

	dword_39F42 = 0;
	dword_39F46 = 0;
	dword_39F4A = 0;

	dword_39F4E = 0;
	word_39F52 = 0;
	word_39F54 = 0;
	dword_39F56 = 0;
	dword_39F5A = 0;
	word_39F5E = 0;
	word_39F60 = 0;

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

	word_39FCE = 0;
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

	dword_3A000 = 0;
	dword_3A004 = 0;
	dword_3A008 = 0;
	word_3A00C = 0;
	word_3A00E = 0;
	word_3A010 = 0;
	word_3A014 = 0;
	word_3A016 = 0;
	word_3A01A = 0;
	word_3A024 = 0;
	dword_3A030 = 0;
	mCamera_Position_Column = 0;
	mCamera_Position_Row = 0;
	word_3A067 = 0;

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
	word_3A399 = 0;
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
	word_3A9B2 = 0;
	word_3A9B4 = 0;
	word_3A9B8 = 0;
	for (uint8 x = 0; x < 3; ++x) {
		word_3A9BA[x] = 0;
		word_3A9C0[x] = 0;
	}

	word_3A9C6 = 0;
	dword_3A9C8 = 0;
	word_3A9CE = 0;
	word_3A9D0 = 0;
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
	word_3AA03 = 0;

	for (uint16 x = 0; x < 3; ++x) {
		mSquad_Grenades[x] = 0;
		mSquad_Rockets[x] = 0;
		word_3AA11[x] = 0;
	}

	mTroops_Enemy_Count = 0;
	word_3AA19 = 0;
	word_3AA1B = 0;
	word_3AA1D = 0;
	word_3AA1F = 0;
	word_3AA21 = 0;
	word_3AA41 = 0;
	word_3AA45 = 0;
	word_3AA47 = 0;
	word_3AA4B = 0;
	word_3AA4D = 0;
	word_3AA4F = 0;
	word_3AA51 = 0;
	word_3AA55 = 0;
	word_3AA67 = 0;
	word_3AA71 = 0;
	word_3AAC7 = 0;
	dword_3AAC9 = 0;
	word_3AACD = 0;
	word_3AACF = 0;
	byte_3ABA9 = 0;
	word_3ABB1 = 0;
	word_3ABB3 = 0;
	word_3ABB5 = 0;
	word_3ABB7 = 0;
	stru_3ABB9.field_0 = 0;
	stru_3ABB9.field_2 = 0;
	stru_3ABB9.field_4 = 0;
	stru_3ABB9.field_6 = 0;
	stru_3ABB9.field_8 = 0;

	word_3AAD1 = 0;
	word_3AB39 = 0;
	word_3ABAD = 0;
	word_3ABAF = 0;
	word_3ABC3 = 0;
	word_3ABC5 = 0;
	word_3ABC7 = 0;
	dword_3ABC9 = 0;

	for (uint16 x = 0; x < 9; ++x )
		word_3ABFF[x] = 0;

	dword_3AC11 = 0;

	word_3AC19 = 0;
	word_3AC1B = 0;
	word_3AC1D = 0;
	word_3AC1F = 0;
	word_3AC2D[0] = 0;
	word_3AC2D[1] = 0;
	word_3AC2D[2] = 0;

	word_3AC21 = 0;
	word_3AC29 = 0;
	word_3AC2B = 0;
	byte_3AC33[0] = 0;
	byte_3AC33[1] = 0;
	byte_3AC33[2] = 0;
	byte_3AC39[0] = 0;
	byte_3AC39[1] = 0;
	byte_3AC39[2] = 0;

	word_3AC3F[0] = 0;
	word_3AC3F[1] = 0;
	word_3AC3F[2] = 0;

	word_3AC45 = 0;
	word_3AC47 = 0;
	word_3AC49 = 0;
	word_3AC4B = 0;
	word_3AC4D = 0;
	word_3AC4F = 0;
	word_3AC51 = 0;

	word_3AF01 = 0;
	word_3AF03 = 0;
	word_3AF05 = 0;

	for (uint16 x = 0; x < 128; ++x) {
		stru_3AF0B[x].field_0 = 0;
		stru_3AF0B[x].field_2 = 0;
	}

	word_3AF07 = 0;
	dword_3B11B = 0;
	dword_3B11F = 0;

	for (uint16 x = 0; x < 42; ++x) {
		stru_3AC53[x].field_0 = 0;
		stru_3AC53[x].field_4 = 0;
		stru_3AC53[x].field_6 = 0;
		stru_3AC53[x].field_8 = 0;
		stru_3AC53[x].field_A = 0;
		stru_3AC53[x].mMouseInsideFuncPtr = 0;
	}

	dword_3AEF3 = 0;
	word_3AEFB = 0;
	dword_3B1CB = 0;

	word_3B1AB = 0;

	for (uint16 x = 0; x < 0x0F; ++x ) 
		word_3B1CF[x] = 0;

	word_3B1EF = 0;
	word_3B1F1 = 0;
	dword_3B1FB = 0;
	
	dword_3B20B = 0;
	word_3B20F = 0;
	word_3B211 = 0;
	for (uint16 x = 0; x < 15; ++x) {
		dword_3B213[x] = 0;
	}

	for (uint16 x = 0; x < 3; ++x) {
		dword_3B24F[x] = 0;
	}

	word_3B25B = 0;
	word_3B2CB = 0;
	word_3B2CD = 0;
	word_3B2CF = 0;
	word_3B2F1 = 0;
	word_3B2F3 = 0;
	word_3B2FD = 0;
	word_3B2FF = 0;

	word_3B301 = 0;
	word_3B303 = 0;
	word_3B305 = 0;
	word_3B307 = 0;
	word_3B447 = 0;

	for (uint16 x = 0; x < 3; ++x)
		word_3B461[x] = 0;

	for (uint16 x = 0; x < 8; ++x)
		mMapGoals[x] = 0;

	word_3B47B = 0;
	word_3B487 = 0;
	word_3B489 = 0;

	for (uint16 x = 0; x < 16; ++x) {
		dword_3B48B[x] = 0;
	}
	word_3B4CB = 0;
	word_3B4CD = 0;
	word_3B4D3 = 0;
	word_3B4D5 = 0;
	word_3B4D7 = 0;
	word_3B4D9 = 0;
	mMission_ShowMapOverview = 0;
	word_3B4DD = 0;
	word_3B4E9 = 0;
	word_3B4EB = 0;
	word_3B4F1 = 0;
	word_3B4F3 = 0;
}

void cFodder::sub_10D9F() {
	word_3BDE7 = 1;
	word_3BDE9 = 1;
	word_3BDEB = 5;
	// -1
	off_3BDEF[0] = dword_3BE03;
	off_3BDEF[1] = dword_3BE27;
	off_3BDEF[2] = dword_3BE4B;
	off_3BDEF[3] = dword_3BE6F;
	off_3BDEF[4] = dword_3BE93;
	
	for (unsigned int x = 0; x < 9; ++x)
		dword_3BE03[x] = INVALID_SPRITE_PTR;
	
	for (unsigned int x = 0; x < 9; ++x)
		dword_3BE27[x] = INVALID_SPRITE_PTR;
	
	for (unsigned int x = 0; x < 9; ++x)
		dword_3BE4B[x] = INVALID_SPRITE_PTR;
	
	for (unsigned int x = 0; x < 9; ++x)
		dword_3BE6F[x] = INVALID_SPRITE_PTR;
	
	for (unsigned int x = 0; x < 9; ++x)
		dword_3BE93[x] = INVALID_SPRITE_PTR;
	
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

	off_3BEF3[0] = stru_3A3FD;
	off_3BEF3[1] = stru_3A475;
	off_3BEF3[2] = stru_3A4ED;
	off_3BEF3[3] = stru_3A565;
	off_3BEF3[4] = stru_3A5DD;
	off_3BEF3[5] = stru_3A655;
	off_3BEF3[6] = stru_3A6CD;
	off_3BEF3[7] = stru_3A745;
	off_3BEF3[8] = stru_3A7BD;
	off_3BEF3[9] = stru_3A835;

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
	word_3A9AA = 0;
	mMissionComplete = 0;
	word_3A9AE = 0;
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

	word_3A067 = 0;
	word_3A069 = 0;
	word_3A06B = -1;
	word_3A28D = 0x0C;
	stru_3A3FD[0].field_0 = -1;
	stru_3A475[0].field_0 = -1;
	stru_3A4ED[0].field_0 = -1;
	stru_3A565[0].field_0 = -1;
	stru_3A5DD[0].field_0 = -1;
	stru_3A655[0].field_0 = -1;
	stru_3A6CD[0].field_0 = -1;
	stru_3A745[0].field_0 = -1;
	stru_3A7BD[0].field_0 = -1;
	stru_3A835[0].field_0 = -1;
}

void cFodder::sub_10EA4() {
	dword_39F98 = 0;
	word_39F9C = 0;
	word_39F9E = 0;
	mSprites[0].field_0 = -32768;
	word_39FCE = &mSprites[0];
}

void cFodder::sub_10EC3() {
	sub_10EE3();
	dword_3A000 = 0;
	dword_3A004 = 0;
	dword_3A008 = 0;

}

void cFodder::sub_10EE3() {

	sSprite_0* Data = mSprites;
	for( int16 count = 0x2C; count >= 0; --count,++Data ) {
		
		sub_212C4( Data );
	}

	mSprites[44].field_0 = -1;
}

void cFodder::map_Load_Spt() {
	
	std::string Filename_Spt = map_Filename_SptGet();

	mMapSptSize = g_Resource.fileLoadTo( Filename_Spt, (uint8*) mMapSptPtr );
	/*
	uint8 TmpSpt[] = { 0x00,0x7C,0x00,0x00,0x00,0x8B,0x00,0xD5,0x00,0x00,
					   0x00,0x7C,0x00,0x00,0x00,0x51,0x00,0x31,0x00,0x05 };
	mMapSptSize = sizeof( TmpSpt );

	memset( mMapSptPtr, 0, 0x258 * 16 );
	memcpy( mMapSptPtr, TmpSpt, sizeof( TmpSpt ) );
	*/
	tool_EndianSwap( (uint8*) mMapSptPtr, mMapSptSize );

	mTroops_Enemy_Count = 0;
	word_3AA19 = 0;
	
	uint16* dword_37AC0 = mMapSptPtr;
	sSprite_0* Data24 = mSprites;
	
	uint16* dword_37AC8 = dword_37AC0 + (mMapSptSize / 2);
	int16 dword_37ABC = 0x0A;

	for(uint16 mTmpCount = 0; dword_37AC0 != dword_37AC8; ++Data24 ) {
		++dword_37AC0;

		Data24->field_8 = 0x7C;
		
		uint16 ax = mTmpCount / 8;

		Data24->field_32 = ax;
		++dword_37AC0;
		ax = dword_37AC0[0];
		++dword_37AC0;
		
		ax += 0x10;
		Data24->field_0 = ax;
		Data24->field_26 = ax;
		
		ax = dword_37AC0[0];
		++dword_37AC0;
		Data24->field_4 = ax;
		Data24->field_28 = ax;
		
		ax = dword_37AC0[0];
		++dword_37AC0;
		Data24->field_18 = ax;
		
		if( Data24->field_18 == 0x6A || Data24->field_18 == 0x48 ) {
				
			++word_3AA19;
		}
		
		if(Data24->field_18 != 0 ) {
			//10EE
			if(Data24->field_18 == 0x24 ) {
				Data24->field_22 = 1;
				++mTroops_Enemy_Count;
				
			} else {
				if( Data24->field_18 == 5 )
					++mTroops_Enemy_Count;
			}
			
			dword_37ABC += 0x0A;
			if( Data24->field_62 > 4 )
				dword_37ABC = 0;
			
			Data24->field_4A = dword_37ABC;
			
		} else {
			// 113C
			++mTmpCount;
			Data24->field_4A = 0;
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
	sSprite_0* dword_37AC0 = mSprites;
	
	for( int16 mTmpCount = 0x1D; mTmpCount > 0; --mTmpCount, ++dword_37AC0 ) {
		if( dword_37AC0->field_0 != -32768 ) {
			
			if( dword_37AC0->field_18 == 0 )
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

		if( ax > word_390CE )
			ax = word_390CE;
		
		mMapPlayerTroopCount = ax;
		word_390CE -= ax;
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
				sSquad_Member* Data28 = Data20;

				*Data28 = *Data24;
				Data24->mRecruitID = -1;
				Data24->mRank = 0;
			}
		}
	}
	
	// Sort by kills

	sSquad_Member* Data20 = mSquad;
	//seg000:1347
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

void cFodder::sub_1142D() {
	sub_301F7();

	if (!word_3ABA7) {

		if (word_390D4) {
			sSquad_Member* Data20 = mSquad;
			sSprite_0** Data24 = word_390D6;

			for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
				Data20->field_4 = *Data24++;
			}
		}

		//seg000:1481                loc_11481:
		word_390D4 = -1;
		sSquad_Member* Data20 = mSquad;
		sSprite_0** Data24 = word_390D6;
		for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
			*Data24++ = Data20->field_4;
		}

	}
	//seg000:14C6                loc_114C6:
	int16 Data1C = mMapPlayerTroopCount - 1;
	while (Data1C >= 0) {
		sub_1152F();
		--Data1C;
	}

	if (word_3ABA7)
		return;

	Data1C = mTroopsAvailable;
	sSquad_Member* Data20 = mSquad;
	for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {

		if (Data20->field_4 == INVALID_SPRITE_PTR)
			continue;

		if (Data1C)
			--Data1C;
		else
			Data20->field_4 = INVALID_SPRITE_PTR;
	}

}

void cFodder::sub_1152F() {
	sSquad_Member* Data20 = mSquad;

	for (int16 Data0 = 7; Data0 >= 0; --Data0) {

		if (Data20->mRecruitID == -1) {

			if (word_390F4 >= 360)
				return;

			Data20->mRecruitID = word_390F4;

			sRecruit* Data24 = mRecruits;
			Data24 += word_390F4;

			Data20->mRank =  (mMissionNumber - 1) / 3;
			Data20->field_8 = 0;
			Data20->field_6 = 3;
			++word_390F4;

			return;
		}

		++Data20;
	}
}

void cFodder::sub_115F7() {

	word_3A016 = mTroopsAvailable;
	sSprite_0* Data20 = mSprites;
	sSquad_Member* Troop = mSquad;

	for (int16 Data18 = 0x1D; Data18 >= 0; --Data18, ++Data20 ) {

		if (Data20->field_0 == -32768)
			continue;

		if (Data20->field_18)
			continue;

		--word_3A016;
		if (word_3A016 < 0) {
			Troop->field_4 = INVALID_SPRITE_PTR;
			Data20->field_0 = -32768;
			Data20->field_18 = 4;
			Data20->field_8 = 0x7C;
			++Troop;
		}
		else {
			// loc_1166B
			Data20->field_46 = (int32*) Troop;

			Troop->field_4 = Data20;
			Data20->field_10 = 0x40;
			Data20->field_22 = 0;

			++Troop;
		}
	}
}

void cFodder::Camera_Position_Update() {
	int16 Data4 = mCamera_Position_Row;
	Data4 -= 108; // 108;

	if (Data4 < 0)
		Data4 = 0;

	int16 Data0 = mCamera_Position_Column;
	Data0 -= 128; // 128;

	if (Data0 < 0)
		Data0 = 0;
	
	//loc_118B0
	int16 Data8 = mCamera_Adjust_Col >> 16;
	int16 DataC = mCamera_Adjust_Row >> 16;

	int16 Data0_Saved = Data0;
	int16 Data4_Saved = Data4;
	int16 Data8_Saved = Data8;
	int16 DataC_Saved = DataC;

	sub_29E30( Data0, Data4, Data8, DataC );
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
		//loc_118FC
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

		dword_39F42 = Dataa8;
		dword_39F46 = DataaC;

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

void cFodder::sub_11B06() {
	
	dword_39F42 += dword_39F4A;
	dword_39F46 += dword_39F4E;

	int32 Data0 = 1;

	if (dword_39F42 < 0)
		Data0 = -1;

	if (Data0 != word_39FB6) {
		word_39FB6 = Data0;
		word_39FBA = 3;
	}

	if (word_39FBA)
		dword_39F42 = 0;

	Data0 = 1;
	if (dword_39F46 < 0)
		Data0 = -1;

	if (Data0 != word_39FB8) {
		word_39FB8 = Data0;
		word_39FBC = 3;
	}

	if (word_39FBC)
		dword_39F46 = 0;

	//loc_11B9C
	Data0 = word_3AA4D;
	Data0 -= 0x110;
	Data0 = (Data0 << 16) | (Data0 >> 16);

	int32 Data4 = mCamera_Adjust_Col;
	Data4 += dword_39F42;

	if (Data4 > Data0) {
		Data0 -= mCamera_Adjust_Col;
		dword_39F42 = Data0;
	}
	//loc_11BE8
	Data0 = word_3AA4F;
	Data0 -= 0xC8;
	Data0 = (Data0 << 16) | (Data0 >> 16);

	Data4 = mCamera_Adjust_Row;
	Data4 += dword_39F46;

	if (Data4 > Data0) {
		Data0 -= mCamera_Adjust_Row;
		dword_39F46 = Data0;
	}

	Data0 = dword_39F42;
	if (Data0 < 0)
		Data0 = -Data0;

	if (Data0 > dword_3A9FD) {
		Data0 = dword_3A9FD;

		if ((dword_39F42 >> 16) < 0)
			Data0 = -Data0;

		dword_39F42 = Data0;
	}
	//loc_11C6D
	Data0 = dword_39F46;
	if (Data0 < 0)
		Data0 = -Data0;

	if (Data0 <= dword_3A9FD)
		return;

	Data0 = dword_3A9FD;
	if ((dword_39F46 >> 16) < 0)
		Data0 = -Data0;

	dword_39F46 = Data0;
}

void cFodder::sub_11CAD() {

	word_39F5E = mCamera_Adjust_Col >> 16;
	word_39F60 = mCamera_Adjust_Row >> 16;
	word_39F52 = 0;
	word_39F54 = 0;
	dword_39F4A &= 0x0000FFFF;
	dword_39F4E &= 0x0000FFFF;
}

void cFodder::sub_11CD6( ) {
	int16 Data0 = mCamera_Adjust_Col >> 16;
	int16 Data4 = mCamera_Adjust_Row >> 16;

	int16 Data8 = word_3A067;
	Data8 -= 0x88;
	if (Data8 < 0)
		Data8 = 0;

	int16 Data10 = word_3AA4D;
	Data10 -= 0x88;
	if (Data8 >= Data10)
		Data8 = Data10;

	int16 DataC = word_3A069;
	DataC -= 0x6C;
	if (DataC < 0)
		DataC = 0;

	Data10 = word_3AA4F;
	Data10 -= 0x6C;
	if (DataC >= Data10)
		DataC = Data10;

	if (!word_3ABB7) {
		sub_29E30(Data0,Data4,Data8,DataC);

		if (Data0 >= 0x8C)
			goto loc_11D8A;
	}
	//loc_11D62
	mCamera_Position_Column = word_3A067;
	mCamera_Position_Row = word_3A069;

	//mImage->paletteFadeOut();
	sub_11E60();
	return;

loc_11D8A:;

	if (word_3A9AE)
		goto loc_11E5B;

	word_39FBA = 6;
	word_39FBC = 6;
	word_390A6 = 0;

	mCamera_Position_Column = word_3A067;
	mCamera_Position_Row = word_3A069;
	word_39F52 = 0;
	dword_39F4A &= 0x0000FFFF;
	word_39F54 = 0;
	dword_39F4E &= 0x0000FFFF;

	//mImage->paletteFadeOut();
	/*
	do {
		mImageFaded = mImage->paletteFade();

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

	} while (mImageFaded == -1);*/

	//sub_14445();
	//sub_14445();

	if (word_3AF07)
		sub_2DE2C();

	dword_39F7C = 0;

	for (;;) {
		sub_11E6C();
		sub_11E6C();

		if (!word_3AA1F)
			break;
	}

	Mission_Sprites_Handle();
	sub_144A2();
	Mouse_DrawCursor();
	sub_11CAD();

	word_390A6 = -1;

	//mImage->paletteFadeOut();

loc_11E5B:;
	sub_11E60();
}

void cFodder::map_Load_TileSet() {
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


	g_Resource.fileLoadTo( BaseName, mDataBaseBlk );
	paletteLoad( mDataBaseBlk + 0xFA00, 0x80, 0x00 );

	g_Resource.fileLoadTo( SubName, mDataSubBlk );

	mMapWidth = readBEWord( &mMap[0x54] );
	writeLEWord( &mMap[0x54], mMapWidth );

	mMapHeight = readBEWord( &mMap[0x56] );
	writeLEWord( &mMap[0x56], mMapHeight );

	delete mSurfaceMapOverview;
	mSurfaceMapOverview = new cSurface( mMapWidth * 16, mMapHeight * 16 );

	std::string JunCopt = sub_12AA1( BaseBaseSet, "copt.dat" );
	std::string JunBaseSwp = sub_12AA1( BaseBase, ".swp" );
	std::string JunBaseHit = sub_12AA1( BaseBase, ".hit" );
	std::string JunBaseBht = sub_12AA1( BaseBase, ".bht" );
	std::string JunArmy = sub_12AA1( BaseSubSet, "army.dat" );
	std::string JunSubSwp = sub_12AA1( BaseSub, ".swp" );
	std::string JunSubHit = sub_12AA1( BaseSub, ".hit" );
	std::string JunSubBht = sub_12AA1( BaseSub, ".bht" );

	g_Resource.fileLoadTo( JunCopt, mDataHillBits );
	paletteLoad( mDataHillBits + 0xD2A0, 0x40, 0xB0 );
	paletteLoad( mDataHillBits + 0xD360, 0x10, 0x90 );
	g_Resource.fileLoadTo( JunArmy, mDataArmy );
	paletteLoad( mDataArmy + 0xD200, 0x10, 0xA0 );

	size_t Size = g_Resource.fileLoadTo( JunBaseSwp, (uint8*) &word_3D03D[0] );
	tool_EndianSwap( (uint8*)&word_3D03D[0], Size );
	
	Size = g_Resource.fileLoadTo( JunSubSwp, (uint8*) &word_3D21D[0] );
	tool_EndianSwap( (uint8*)&word_3D21D[0], Size );

	Size = g_Resource.fileLoadTo( JunBaseHit, (uint8*) &word_3C09D[0] );
	tool_EndianSwap( (uint8*)&word_3C09D[0], Size );
	
	Size = g_Resource.fileLoadTo( JunSubHit, (uint8*) &graphicsSub0[0] );
	tool_EndianSwap( (uint8*)&graphicsSub0[0], Size );
	
	Size = g_Resource.fileLoadTo( JunBaseBht, (uint8*) &graphicsBaseBht[0] );
	tool_EndianSwap( (uint8*)&graphicsBaseBht[0], Size );

	Size = g_Resource.fileLoadTo( JunSubBht, (uint8*) &graphicsSub0Bht[0] );
	tool_EndianSwap( (uint8*)&graphicsSub0Bht[0], Size );
	
	Sprite_SetDataPtrToBase( off_32C0C );
}

void cFodder::sub_11E60() {
	int16 ax = mMap_TileSet + 0x32;

	Music_Unk(0x32);
}

void cFodder::sub_11E6C( ) {
	sub_11EC2();

	int32 Saved_dword_3A9FD = dword_3A9FD;
	dword_3A9FD = 0x100000;
	sub_11B06();
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

void cFodder::sub_11EC2() {
	word_3AA1F = 2;
	dword_39F42 = 0;
	dword_39F46 = 0;

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
		--word_3AA1F;
	} 
	else if (Data8 > Data0) {
		dword_39F42 = (dword_39F42 & 0xFFFF) | -8 << 16;
	} else 
		dword_39F42 = (dword_39F42 & 0xFFFF) | 8 << 16;

	//loc_11FAC
	if (DataC == Data4) {
		--word_3AA1F;
	} 
	else if (DataC > Data4) {
		dword_39F46 = (dword_39F46 & 0xFFFF) | -8 << 16;
	} else 
		dword_39F46 = (dword_39F46 & 0xFFFF) | 8 << 16;
}

void cFodder::sub_11FCD() {
	
	if (!word_3A054) {

		if (mMouseX > 0x0F) {
			int16 Data0 = mCamera_Adjust_Col >> 16;
			Data0 -= word_39F5E;

			mMouseX -= Data0;
			Data0 = mCamera_Adjust_Row >> 16;
			Data0 -= word_39F60;
			mMouseY -= Data0;

		}
	}
	//loc_12007
	word_39F5E = mCamera_Adjust_Col >> 16;
	word_39F60 = mCamera_Adjust_Row >> 16;
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

void cFodder::sub_12083() {
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
	word_39F5E = 0;
	word_39F60 = 0;
	dword_39F42 = 0;
	dword_39F46 = 0;
	dword_39F56 = 0;
}

void cFodder::sub_120F6() {
	dword_39F56 = 0;
	int32* Data20 = &dword_39F18;

	dword_39F5A = mCamera_Adjust_Col;
	int32 Data0 = dword_39F42;

	mCamera_Adjust_Col += Data0;
	if (mCamera_Adjust_Col < 0) {
		dword_39F42 = dword_39F5A;
		if (dword_39F42)
			dword_39F42 = -dword_39F42;

		mCamera_Adjust_Col = 0;
	}
	//loc_12147
	Data0 = dword_39F42;
	dword_39F24 += Data0;

	if ((dword_39F42 >> 16) < 0) {

		if ((dword_39F24 >> 16) < 0) {

			dword_39F24 += (0x140 << 16);
			dword_39F56 = (0xFFFF << 16) | dword_39F56 & 0xFFFF;
			word_39F94 -= 0x28;
			word_39F96 -= 1;
		}
	}
	else {
		//loc_12181
		if ((dword_39F24 >> 16) >= 0x140) {
			dword_39F24 -= (0x140 << 16);
			dword_39F56 = (1 << 16) | dword_39F56 & 0xFFFF;
			word_39F94 += 0x28;
			++word_39F96;
		}
	}
	//loc_1219F
	Camera_Adjust_Row( Data20 );
	dword_39F42 = 0;
	dword_39F46 = 0;
}

void cFodder::Camera_Adjust_Row( int32* pData20 ) {

	dword_39F5A = mCamera_Adjust_Row;

	int32 Data0 = dword_39F46;
	mCamera_Adjust_Row += Data0;

	if (mCamera_Adjust_Row < 0) {
		dword_39F46 = dword_39F5A;

		if (dword_39F46)
			dword_39F46 = -dword_39F46;

		mCamera_Adjust_Row = 0;
	}
	//loc_121F2
	Data0 = dword_39F46;
	Data0 += dword_39F56;

	dword_39F28 += Data0;
	dword_39F28 &= (0x00FF << 16) | 0xFFFF;

	Data0 = dword_39F28 >> 16;
	Data0 *= 0x28;
	*pData20 = Data0;
}

void cFodder::sub_12245() {
	
	dword_3A9FD = 0x20000;

	if (!word_3A9B2 && !word_3A9AA && !mMissionComplete)
		return;

	int16 Data0 = 0;

	dword_39F4A = (dword_39F4A & 0xFFFF) + (Data0 << 16);
	dword_39F4E = (dword_39F4E & 0xFFFF) + (Data0 << 16);
	dword_39F42 = Data0;
	dword_39F46 = Data0;
	dword_3A9FD = 0;
}

void cFodder::sub_1229C() {
	
	if (word_39F52)
		dword_39F42 = 0;

	if (word_39F54)
		dword_39F46 = 0;
}

void cFodder::sub_122BD() {

	if (word_39FCE == INVALID_SPRITE_PTR || word_39FCE == 0 )
		return;

	sSprite_0* Data20 = word_39FCE;

	int16 Data4 = Data20->field_0 + 0x40;
	int16 Data8 = Data20->field_4 + 0x10;

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

	sub_2DE2C();
	Sprite_Draw();
}

void cFodder::Sprite_Sort_DrawList() {
	sSprite_0* si = mSprites;
	sSprite_0** di = dword_3A071;
	sSprite_0** bx = dword_3A125;
	sSprite_0** bp = dword_3A1D9;

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

	sSprite_0** Si = dword_3A125 - 1;
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

		sSprite_0* eax = *di;
		*di = *Si;
		*Si = eax;
		goto loc_1248D;
	}
	//loc_124AF
	di = dword_3A291;
	Si = dword_3A071;

	for(;;) {
		sSprite_0* ax = *Si++;
		if (ax == INVALID_SPRITE_PTR)
			break;

		*di++ = ax;
	}

	Si = dword_3A125;
	for(;;) {
		sSprite_0* ax = *Si++;
		if (ax == INVALID_SPRITE_PTR)
			break;

		*di++ = ax;
	}
	Si = dword_3A1D9;
	for(;;) {
		sSprite_0* ax = *Si++;
		*di++ = ax;
		
		if (ax == INVALID_SPRITE_PTR)
			break;
	}
}

void cFodder::sub_124DB() {
	if (dword_3B48B[0] == INVALID_SPRITE_PTR || dword_3B48B[0] == 0)
		return;

	sSprite_0* Data20 = dword_3B48B[0];
	sSquad_Member* Dataa20 = (sSquad_Member*)Data20->field_46;
	int16 Data0 = 0;

	Data0 = Dataa20->mRank;
	Data0 += 8;
	if (Data0 > 0x0F)
		Data0 = 0x0F;

	//Data0 *= 0x0A
	const struct_5* Dataaa20 = &stru_3D35F[Data0];
	//seg000:254F
	stru_3ABB9 = *Dataaa20;
}

void cFodder::Mission_Goals_Check() {
	
	sSprite_0* Data20 = mSprites;
	int16 Data8 = 0;
	int16 DataC = 0;
	int16 Data0 = 0x2B;
	
	for(Data0 = 0x2B; Data0 >= 0; --Data0, ++Data20 ) {
		
		if( Data20->field_0 == -32768)
			continue;

		int16 Data10 = Data20->field_18;
		if(Data10 == 0x6C || Data10 == 0x6D || Data10 == 0x6E || Data10 == 0x58 || Data10 == 0x64 )
			goto loc_12620;
		
		if(Data10 == 0x19 )
			goto loc_12615;
		
		if(Data10 != 0x14 )
			goto loc_12632;
		
	loc_12615:;
		if(Data20->field_38 == 5 )
			continue;
		
	loc_12620:;
		if(Data20->field_38 == 7)
			continue;
		
		++DataC;
		continue;
		
		loc_12632:;
		const int16* Data24 = word_3D593;
		
		loc_1263E:;
		if(*Data24 < 0)
			continue;
		
		if( Data10 != *Data24++ )
			goto loc_1263E;
		
		++Data8;
	}
	
	word_3A9B0 = DataC;
	if(mMapGoals[1]) {
		if( DataC )
			return;
	}
	
	if(mMapGoals[0]) {
		if(Data8)
			return;
	}
	
	if(!mMapGoals[4]) {
		if(!mMapGoals[2])
			goto loc_126A6;
	}
	
	if( word_3AA19 )
		return;
	
	loc_126A6:;
	if(!mMapGoals[7] || word_3B47B ) 
		 mMissionComplete = -1;
}

void cFodder::sub_126BB() {
	dword_3B11B = stru_3AF0B;
	dword_3B11F = stru_3AF0B;

	stru_3AF0B[0].field_0 = -1;
	stru_3AF0B[0].field_2 = -1;
}

void cFodder::sub_126DD() {
	const int8* Data20 = off_3D5F1[mMapNumber];

	for (; *Data20 != -1; ++Data20) {

		int8 Data0 = *Data20;
		
		mMapGoals[Data0-1] = -1;
	}
}

void cFodder::sub_12790( ) {
	
	if (mMissionComplete >= 0)
		if (!word_3A9B2)
			if (!word_3A9AA)
				return;

	if (word_3A9AE < 0)
		return;
	if (word_3A9AE)
		goto loc_1280A;

	word_3A9AE = 0x64;
	if (word_3A9B2) {
		word_3A9AE = 0x32;
		goto loc_127E8;
	}

	if (mMissionComplete) {
		if (word_3A9AA)
			goto loc_127E8;

		sub_12838();
		word_390F8 = -1;
		goto loc_1280A;
	}
	//loc_127E1
	if (word_390CE) {
	loc_127E8:;

		sub_12952();
		word_3D465 = 0x0F;
		word_3D467 = 0x0A;
		goto loc_1280A;
	}
	//loc_127FA
	sub_13255();
	word_3D465 = 8;
	word_3D467 = 0x0A;

loc_1280A:;

	if (word_3A9AE == 0x19) {
		mImageFaded = -1;
		mImage->paletteFadeOut();
	}
	--word_3A9AE;
	if (word_3A9AE)
		return;

	word_3A9AE = -1;
	word_3A3B9 = -1;
}

void cFodder::sub_12838() {
	sSprite_0* Data24 = &mSprites[40];

	sub_2061C( Data24 );
	sSprite_0* Data2C = &mSprites[41];

	if (mMissionPhaseRemain == 1)
		sub_12877( Data2C );
	else
		sub_128A9( Data2C );
	
	Data2C = &mSprites[42];
	sub_128DB( Data2C );
}

void cFodder::sub_12877( sSprite_0* pData2C ) {

	sub_128F4( pData2C );

	pData2C->field_4 -= 0x14;
	pData2C->field_0 += 0x12;
	pData2C->field_8 = 0xA2;
	pData2C->field_18 = 0x1D;

	Music_Unk( 6 );
}

void cFodder::sub_128A9( sSprite_0* pData2C ) {
	sub_128F4( pData2C );

	pData2C->field_4 -= 0x14;
	pData2C->field_0 += 0x1B;
	pData2C->field_8 = 0xA1;
	pData2C->field_18 = 0x1E;

	Music_Unk( 0x0C );
}

void cFodder::sub_128DB( sSprite_0* pData2C ) {
	sub_128F4( pData2C );
	pData2C->field_8 = 0xA0;
	pData2C->field_18 = 0x1C;
}

void cFodder::sub_128F4( sSprite_0* pData2C ) {

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

void cFodder::sub_12952() {
	sSprite_0* Data24 = &mSprites[40];
	sub_2061C( Data24 );

	sSprite_0* Data2C = &mSprites[41];
	sub_1298C( Data2C );

	Data2C = &mSprites[42];
	sub_129B6( Data2C );

	//TODO
	Music_Unk(0x0F);
}

void cFodder::sub_1298C( sSprite_0* pData2C ) {
	sub_128F4( pData2C );
	pData2C->field_4 -= 0x14;
	pData2C->field_0 += 0x12;
	pData2C->field_8 = 0xCA;
	pData2C->field_18 = 0x3A;

}

void cFodder::sub_129B6( sSprite_0* pData2C ) {
	sub_128F4( pData2C );

	pData2C->field_8 = 0xCB;
	pData2C->field_18 = 0x3B;
}

std::string	cFodder::sub_12AA1( std::string pBase, const char* pFinish ) {
	std::string Final;

	Final.append( pBase );
	Final.append( pFinish );

	return Final;
}

void cFodder::sub_12A5F() {
	sSquad_Member* Data20 = mSquad;

	for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
		if (Data20->field_4 == INVALID_SPRITE_PTR || Data20->field_4 == 0 )
			continue;

		++Data20->field_3;
	}
}

void cFodder::sub_12AEE() {
	
	if(word_3B211)
		return;
	
	word_3B211 = -1;
	sSprite_0* Data20 = mSprites;
	sSprite_0** Data24 = dword_3B213;

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
		sSprite_0** Data20 = off_3BDEF[Data8];

		dword_3B24F[Data8] = 0;

		//Data34 = byte_3E75D;
		int16 Data0 = 0;
		int16 Data10 = 0;

		for (;;) {
			if (*Data20 != INVALID_SPRITE_PTR && *Data20 != 0 ) {
				sSprite_0* Data24 = *Data20++;
				if (!Data24->field_6E)
					goto loc_12C5F;

				dword_3B20B = (sSprite_0*) Data24->field_6A;
				Data0 = -1;
				continue;

			}
			else {
				//loc_12C2B
				if (Data0 == 0)
					goto loc_12C5F;

				sSprite_0* Dataa0 = dword_3B20B;
				dword_3B24F[Data8] = Dataa0;
				word_39FCE = Dataa0;
				return;
			}
		}

	}

loc_12C5F:;
	dword_3B20B = 0;
}

void cFodder::sub_12C69() {
	int16* Data24 = word_3B461;

	for (int16 Data0 = 0; Data0 < 3; ++Data0) {
		if (!word_3B461[Data0])
			continue;

		word_3B461[Data0]--;
		if (word_3B461[Data0])
			continue;

		sSprite_0** Data2C = off_3BDEF[Data0];

		for (;;) {
			if (*Data2C == INVALID_SPRITE_PTR )
				break;

			sSprite_0* Data30 = *Data2C++;
			Data30->field_38 = 0x5A;
		}
	}
}

void cFodder::Map_Overview_Prepare() {
	if (mMapNumber == word_3FA1F)
		return;

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

	//uint16 dx = readLEWord( &mMap[0x56] );
	//int32 Data14 = 0;
	//int16 Data16 = word_3F950;

	for (uint16 dx = 0; dx < mMapHeight; ++dx) {
		//uint16 cx = readLEWord( &mMap[0x54] );

		//int32 Data10 = 0;
		//int16 Data12 = word_3F94E;

		for (uint16 cx = 0; cx < mMapWidth; ++cx) {

			sub_2B04B( (uint8*) mGraphicBlkPtrs[ *Di & 0x1FF], cx, dx );

			++Di;
			//Data10 += dword_3F946;
		}

		//Data14 += dword_3F94A;
	}

}

void cFodder::map_SetTileType() {

	char Type[3];

	Type[0] = mMap[0];
	Type[1] = mMap[1];
	Type[2] = mMap[2];

	for (unsigned int x = 0; x < 6; ++x) {
		if (mMapTypes[x][0] != Type[0])
			continue;
	
		if (mMapTypes[x][1] != Type[1])
			continue;

		if (mMapTypes[x][2] != Type[2])
			continue;

		mMap_TileSet = x;
		return;
	}

	mMap_TileSet = 0;
}

void cFodder::sub_12AB1() {

	word_3E75B = -1;
	word_3EABD = 0;
}

void cFodder::Load_PlayerBin() {
	
}

bool cFodder::EventAdd( cEvent pEvent ) {
	
	mEvents.push_back( pEvent );

	return true;
}

void cFodder::Load_File( const std::string& pFilename ) {
	size_t FileSize = 0;

	uint8* Buffer = local_FileRead( pFilename, "", FileSize );

	word_3E7ED = readLEWord( Buffer );
	mEffectDriver = readLEWord( Buffer + 2 );
	mMusicDriver = readLEWord( Buffer + 4 );

	delete Buffer;
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

			case eEvent_Quit:

				break;
		}

	}

	mEvents.clear();
}

void cFodder::keyProcess( uint8 pKeyCode, bool pPressed ) {
	
	if (pKeyCode == SDL_SCANCODE_LCTRL || pKeyCode == SDL_SCANCODE_RCTRL) {
		if (pPressed)
			mKeyControlPressed = -1;
		else
			mKeyControlPressed = 0;
	}
	
	if (pKeyCode == SDL_SCANCODE_P)
		word_3A9D0 = ~word_3A9D0;
	
	if (pKeyCode == SDL_SCANCODE_ESCAPE)
		word_3A9B2 = -1;

	if (pKeyCode == SDL_SCANCODE_SPACE)
		++word_3A9B4;

	if (pKeyCode == SDL_SCANCODE_M) {
		if (word_3B4F1 == 0)
			mMission_ShowMapOverview = -1;
	}

	if (pKeyCode == SDL_SCANCODE_1)
		mKeyNumberPressed = 2;
	
	if (pKeyCode == SDL_SCANCODE_2)
		mKeyNumberPressed = 3;

	if (pKeyCode == SDL_SCANCODE_3)
		mKeyNumberPressed = 4;
}

void cFodder::mouse_Setup() {
	
	mMouseX = 0x7F;
	mMouseY = 0x67;
	word_3BDB3 = 0;
	word_3BDB5 = 0;

	mouseData1->anonymous_0 = 0;
	mouseData1->anonymous_1 = 0;
	mouseData1->anonymous_2 = 2;
	mouseData1->mColumn = 160;
	mouseData1->mRow = 100;
	mouseData1->anonymous_5 = 0x10;
	mouseData1->anonymous_6 = 0x10;
	mouseData1->anonymous_7 = 0x10;
	mouseData1->anonymous_8 = 0x10;
	mouseData1->anonymous_9 = 0x30;
	mouseData1->anonymous_10 = 0xC8;

	mouseData0->anonymous_0 = 0;
	mouseData0->anonymous_1 = 0;
	mouseData0->anonymous_2 = 2;
	mouseData0->mColumn = 160;
	mouseData0->mRow = 100;
	mouseData0->anonymous_5 = 0x10;
	mouseData0->anonymous_6 = 0x10;
	mouseData0->anonymous_7 = 0x10;
	mouseData0->anonymous_8 = 0x10;
	mouseData0->anonymous_9 = 0x30;
	mouseData0->anonymous_10 = 0xC8;

	g_Window.SetMousePosition( cPosition(g_Window.GetWindowSize().mWidth / 2, g_Window.GetWindowSize().mHeight / 2 ) );
}

void cFodder::mouse_GetData() {
	eventProcess();

	mouse_Pos_Column = (int16) mMousePosition.mX - 48;
	mouse_Pos_Row = (int16) mMousePosition.mY - 12;
	mouse_Button_Status = mMouseButtons;
}

void cFodder::Mouse_Inputs_Get() {
	
	mouse_GetData();
	mouse_ButtonCheck();

	//g_Window.SetMousePosition( cPosition( 0xA0, 0x64 ) );

	int16 Data4 = mouse_Pos_Column;// -0x70;
	word_3BDB3 = Data4;

	//Data4 += mMouseX;
	
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
	
	int16 Data0 = mouse_Pos_Row;// - 88;
	word_3BDB5 = Data0;
	//Data0 += mMouseY;

	if (Data0 < 4)
		Data0 = 4;
	else {

		if (Data0 > 203)
			Data0 = 203;
	}

	mMouseY = Data0;
}

void cFodder::mouse_ButtonCheck() {

	mButtonPressLeft = 0;
	if (mouse_Button_Status & 1) {
		mButtonPressLeft -= 1;
		if (mMouse_Button_Left_Toggle == 0) {
			mMouse_Button_Left_Toggle = -1;
			word_39F02 = -1;

			if (mButtonPressRight) {
				word_39EFC = -1;
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
		if (word_39EFA == 0) {
			word_39F04 = -1;
			word_39EFA = -1;
		}
	} else {
		word_39EFA = 0;
	}

}

void cFodder::MixerChannelFinished( int32 pChannel ) {

	for (std::vector<sVocPlaying>::iterator ChannelIT = mMixerChunks.begin(); ChannelIT != mMixerChunks.end(); ++ChannelIT) {
		
		if (ChannelIT->mChannel == pChannel) {
			Mix_FreeChunk( ChannelIT->mCurrentChunk );

			mMixerChunks.erase( ChannelIT );
			return;
		}
	}
}

void cFodder::Prepare() {

	mWindow->InitWindow( "Open Fodder" );
	tool_RandomSeed();

	mDataBlkSize = 0xFD0 * 16;

	mDataPStuff = new uint8[0xA03 * 16];
	mDataBaseBlk = new uint8[mDataBlkSize];
	mDataSubBlk = new uint8[mDataBlkSize];
	mMap = new uint8[0x346 * 16];
	mDataHillBits = new uint8[0xD5A * 16];
	mDataArmy = new uint8[0xD50 * 16];
	word_3BDAD = (uint16*) new uint8[0x258 * 16];
	mMapSptPtr = (uint16*) new uint8[0x258 * 16];
	word_3D5B7 = mMapSptPtr;

	Load_File("setup.dat");
	sub_12AB1();
	//Resource_Data_Open();
	//Load_EffectDriver();
	//Load_MusicDriver();
	memory_XMS_Detect();
}

void cFodder::sub_13102() {
	
	sSprite_0* Data20 = mSprites;
	int16 Data0 = 0;
	for (int16 Data4 = 0x1D; Data4 >= 0; Data4 -= 4 ) {

		if (Data20->field_18 == 0x63) {
			++Data0;
		}

		++Data20;
	}

	word_3B487 = Data0;
	word_3B4CB = -1;
}

void cFodder::sub_13148() {
	word_3B4D5 = 0x64;
	word_3B4D3 = 0x28;
}

void cFodder::sub_13155() {
	int16 Data0 = word_3B487;
	if (!Data0)
		return;

	if (Data0 != word_3B489)
		return;

	if (word_3B4CB >= 0)
		return;

	sSprite_0* Data2C = dword_3B4CF;
	if (!Data2C)
		return;

	word_3B4CB = Data2C->field_0;
	word_3B4CD = Data2C->field_4;

	word_3B4CD += 0x28;
}

void cFodder::sub_131A2() {
	if (mMapNumber != 0x47)
		return;

	if (word_3B4D5)
		return;


	if (!word_3B4D7)
		word_3B4D7 = 0x28;

	--word_3B4D7;
	if (!word_3B4D7)
		word_3A9B2 = -1;

	int16 Data0 = word_390B0;
	Data0 &= 3;
	if (!Data0)
		sub_131DE();
}

int16 cFodder::sub_131DE() {
	int16 Data0 = 1;
	sSprite_0* Data2C = 0, *Data30 = 0;

	sub_211BA( Data0, Data2C, Data30 );
	if (Data0) {
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
	Data2C->field_18 = 0x0C;
	return Data0;
}

void cFodder::sub_136D0() {
	
	word_40056 = 0;
	word_40058 = 0;
	word_4005A = 0x5280;
	word_4005C = 0xA500;
	word_40054 = 0;
}

void cFodder::sub_13800() {

	videoSleep();
}

void cFodder::sub_13255() {
	sSprite_0* Data24 = &mSprites[40];
	sub_2061C( Data24 );

	sSprite_0* Data2C = &mSprites[41];
	sub_13277( Data2C );
}

void cFodder::sub_13277( sSprite_0* pData2C ) {
	sub_128F4( pData2C );
	pData2C->field_8 = 0xC1;
	pData2C->field_18 = 0x22;

	//TODO
	Music_Unk(8);
}

void cFodder::paletteLoad( uint8* pBuffer, uint16 pColors, uint16 pColorID ) {
		size_t colorStartID = pColorID;

	if( pColors >= g_MaxColors )
		pColors = g_MaxColors-1;

	for(; pColorID < pColors + colorStartID; pColorID++) {
		
		// Get the next color values
		mPalette[ pColorID ].mRed =		*pBuffer++;
		mPalette[ pColorID ].mGreen =	*pBuffer++;
		mPalette[ pColorID ].mBlue =	*pBuffer++;
	}
}

void cFodder::Mouse_DrawCursor( ) {
	int16 cx = mMouseX + word_3A9F3;
	int16 dx = mMouseY + word_3A9F5;

	cx += 48;
	dx += 12;

	mouseData1->mColumn = cx;
	mouseData1->mRow = dx;

	mDrawSpritePositionX = cx;
	mDrawSpritePositionY = dx;

	if (mMouseSpriteNew >= 0) {
		mMouseSpriteCurrent = mMouseSpriteNew;
		mMouseSpriteNew = -1;
	}

	const struct_2* di = &stru_44B50[mMouseSpriteCurrent];
	mouseData1->anonymous_5 = di->field_4;
	mouseData1->anonymous_6 = di->field_6;

	word_4206C = di->field_4;
	word_4206E = di->field_6;

	int16 ax = di->field_2 * 160;
	int16 bx = di->field_0 >> 1;

	word_42062 = mDataPStuff + (ax +bx);
	byte_42070 = 0xF0;
	video_Draw_Sprite_();
}

void cFodder::sub_13C1C( int32 pParam00, int32 pParam0C, int32 pParam04, int32 pParam08 ) {

	uint16 bx = mSpriteDataPtr[pParam00][pParam04].field_0;

	word_42062 = GetSpriteData( mSpriteDataPtr[pParam00][pParam04].field_2 ) + bx;

	mDrawSpritePositionX = (pParam08 + 0x10);
	mDrawSpritePositionY = pParam0C + 0x10;
	word_4206C = mSpriteDataPtr[pParam00][pParam04].mColCount;
	word_4206E = mSpriteDataPtr[pParam00][pParam04].mRowCount;
	if (word_3B307 > pParam00) 
		word_3B307 = pParam00;
	
	byte_42070 = (uint8) mSpriteDataPtr[pParam00][pParam04].field_C;
	
	if (Sprite_OnScreen_Check() )
		video_Draw_Sprite_();

}

void cFodder::sub_13C8A( int16 pData0, int16 pData4, int16 pPosX, int16 pPosY ) {
	uint16 bx = mSpriteDataPtr[pData0][pData4].field_0;

	word_42062 = GetSpriteData( mSpriteDataPtr[pData0][pData4].field_2 ) + bx;
	
	mDrawSpritePositionX = (pPosX + 0x10);
	mDrawSpritePositionY = pPosY + 0x10;
	word_4206C = mSpriteDataPtr[pData0][pData4].mColCount;
	word_4206E = mSpriteDataPtr[pData0][pData4].mRowCount;
	byte_42070 = mSpriteDataPtr[pData0][pData4].field_C & 0xFF;
	word_42078 = 0x140;

	if (Sprite_OnScreen_Check())
		video_Draw_Linear_To_Planar();
}

void cFodder::sub_13CF0( sSprite_0* pDi, int16 pData0, int16 pData4 ) {

	word_42062 = GetSpriteData( mSpriteDataPtr[pData0][pData4].field_2 ) + mSpriteDataPtr[pData0][pData4].field_0;
	
	word_4206C = mSpriteDataPtr[pData0][pData4].mColCount;
	word_4206E = mSpriteDataPtr[pData0][pData4].mRowCount - pDi->field_52;

	mDrawSpritePositionX = (mSpriteDataPtr[pData0][pData4].field_E + pDi->field_0) - mCamera_Column + 0x40;
	mDrawSpritePositionY = (mSpriteDataPtr[pData0][pData4].field_F + pDi->field_4) - word_4206E - pDi->field_20 - mCamera_Row;
	mDrawSpritePositionY += 0x10;

	byte_42070 = mSpriteDataPtr[pData0][pData4].field_C;
	
	++word_42072;
	if (Sprite_OnScreen_Check()) {
		pDi->field_5C = 1;
		video_Draw_Sprite_();
	} else 
		pDi->field_5C = 0;
}

void cFodder::video_Draw_Sprite_( ) {
	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = word_42062;
	int16	ax, cx;
	
	di += 352 * mDrawSpritePositionY;

	ax = mDrawSpritePositionX;
	ax += word_40054;
	//ax >>= 2;
	
	di += ax;
	word_42066 = di;
	cx = mDrawSpritePositionX;
	cx += word_40054;
	cx &= 3;

	uint8 Plane = 0;

	byte_42071 = 1 << cx;
	byte bl = byte_42070;
	
	word_4206C >>= 1;
	word_42074 = 160 - word_4206C;
	word_4206C >>= 1;
	
	word_42076 = 352 - (word_4206C*4);

	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = 0; cx < word_4206C; ++cx ) {
			byte al = (*si) >> 4;
			if(al)
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}

	si = word_42062;
	di = word_42066;
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = word_4206C; cx > 0; --cx ) {
			byte al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}
	
	++word_42062;
	si = word_42062;
	di = word_42066;
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = word_4206C; cx > 0; --cx ) {
			
			byte al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			di+=4;
			
		}
		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}

	si = word_42062;
	di = word_42066;
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = word_4206C; cx > 0; --cx ) {
			
			byte al = (*si) & 0x0F;
			if( al ) 
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += word_42074;
		di += word_42076;
	}
}

void cFodder::video_Draw_Linear_To_Planar( ) {
	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = word_42062;
	int16	ax, cx;
	
	di += 352 * mDrawSpritePositionY;

	ax = mDrawSpritePositionX;
	ax += word_40054;
	//ax >>= 2;
	
	di += ax;
	word_42066 = di;
	cx = mDrawSpritePositionX;
	cx += word_40054;
	cx &= 3;

	uint8 Plane = 0;

	byte_42071 = 1 << cx;
	word_42074 = word_42078 - word_4206C;
		                 
	//word_4206C >>= 1;
	word_42076 = 352 - word_4206C;

	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = word_4206C; cx > 0; --cx ) {
			byte al = *si;
			if(al)
				*di = al;
			
			si ++;
			di ++;
		}

		si += word_42074;
		di += word_42076;
	}
}

bool cFodder::Sprite_OnScreen_Check() {
	int16 ax;
	
	if( mDrawSpritePositionY < 0 ) {
		ax = mDrawSpritePositionY + word_4206E;
		--ax;
		if( ax < 0 )
			return false;
		
		ax -= 0;
		ax -= word_4206E;
		++ax;
		ax = -ax;
		mDrawSpritePositionY += ax;
		word_4206E -= ax;
		ax *= 0xA0;
		word_42062 += ax;
	}
	
	ax = mDrawSpritePositionY + word_4206E;
	--ax;
	if( ax > 231 ) {
		if( mDrawSpritePositionY > 231 )
			return false;
		
		ax -= 231;
		word_4206E -= ax;
		
	}

	if( mDrawSpritePositionX < 0 ) {
		ax = mDrawSpritePositionX + word_4206C;
		--ax;
		if( ax < 0 )
			return false;
		
		ax -= 0;
		ax -= word_4206C;
		++ax;
		ax = -ax;
		--ax;
		
		do {
			++ax;
		} while (ax & 3);

		mDrawSpritePositionX += ax;
		word_4206C -= ax;
		ax >>= 1;
		word_42062 += ax;
	}

	ax = mDrawSpritePositionX + word_4206C;
	--ax;
	
	if( ax > 351 ) {
		if( mDrawSpritePositionX > 351 )
			return false;
		
		ax -= 351;
		--ax;
		
		do {
			++ax;
		} while (ax & 3);
		
		word_4206C -= ax;
	}

	if( word_4206C <= 0 )
		return false;
	
	if( word_4206E <= 0 )
		return false;
	
	return true;
}

void cFodder::Sprite_Draw( ) {
	sSprite_0** si = dword_3A291;
	word_42072 = 2;

	for (;;) {
		sSprite_0* eax = *si++;
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
	//seg001:0D5E
	// mousedata... ? TODO??
}

void cFodder::sub_144A2( ) {
	
	uint8*	Buffer = mImage->GetSurfaceBuffer();
	uint8* 	si = (uint8*) mMapSptPtr;

	//TODO: BUG?
	Buffer += (16 * 352) + 16; // 0x584
	byte_42071 = 1 << word_40054;

	word_42066 = Buffer;
	for (unsigned int Plane = 0; Plane < 4; Plane++) {

		Buffer = word_42066;

		for (unsigned int Y = 0; Y < 200; ++Y) {

			for (unsigned int X = Plane; X < 0x30; X += 4) {

				Buffer[X] = *si++;
			}
			
			Buffer += 352;
		}
	}
}

void cFodder::sub_145AF( int16 pData0, int16 pData8, int16 pDataC ) {
	const struct_2* str2 = &stru_44B50[pData0];
	
	int16 cx = str2->field_4;
	int16 dx = str2->field_6;
	word_4206C = cx;
	word_4206E = dx;
	
	uint16 ax = 0xA0 * str2->field_2;
	uint16 bx = str2->field_0 >> 1;
	ax += bx;
	
	word_42062 = mDataPStuff + ax;
	
	byte_42070 = 0xF0;
	
	uint16 w42066 = 0x0C * pDataC;
	w42066 += pData8 >> 2;
	
	ax = pData8 & 3;
	ax *= 0x960;
	w42066 += ax;
	
	uint8* di = ((uint8*)word_3D5B7) + w42066;
	uint8* si = word_42062;
	
	byte bl = byte_42070;
	word_4206C >>= 1;
	
	word_42074 = 0xA0 - word_4206C;
	word_4206C >>= 1;
	word_42076 = 0x0C - word_4206C;
	
	for( uint16 dx = word_4206E; dx > 0; --dx ) {
		
		for( uint16 cx = word_4206C; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
			
		si += word_42074;
		di += word_42076;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	si = word_42062;
	di =  ((uint8*)word_3D5B7) + w42066;
	
	for( uint16 dx = word_4206E; dx > 0; --dx ) {
		
		for( uint16 cx = word_4206C; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
		
		si += word_42074;
		di += word_42076;
	}
		
	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	++word_42062;
	si = word_42062;
	di =  ((uint8*)word_3D5B7) + w42066;
	
	for( uint16 dx = word_4206E; dx > 0; --dx ) {
		
		for( uint16 cx = word_4206C; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
			
		si += word_42074;
		di += word_42076;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;

	si = word_42062;
	di =  ((uint8*)word_3D5B7) + w42066;
	
	for( uint16 dx = word_4206E; dx > 0; --dx ) {
		
		for( uint16 cx = word_4206C; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
		
		si += word_42074;
		di += word_42076;
	}

}

void cFodder::Sound_Voc_Load() {
	if (mEffectDriver < 5)
		return;

	for (unsigned int x = 0; x < 0x3C; ++x) {
		dword_42320[x].mSize = 0;
		dword_42410[x].mSize = 0;
		dword_42500[x].mSize = 0;
		dword_425F0[x].mSize = 0;
	}

	struct_Voc* Voc = mVocTable;

	for (; Voc->field_0 != 0xFF; ++Voc) {
		size_t bx = 0;

		uint8* VocFile = g_Resource.fileGet( Voc->mFilename, bx );
		//bx += 8;
		//bx &= 0x0FFF8;

		if (Voc->field_0 != 9) {
			sVocLoaded* eax = word_42316[Voc->field_0];
			eax[Voc->field_1].mBuffer = VocFile;
			eax[Voc->field_1].mSize = bx;
		} else {

			dword_42320[Voc->field_1].mBuffer = VocFile;
			dword_42320[Voc->field_1].mSize = bx;
			dword_42410[Voc->field_1].mBuffer = VocFile;
			dword_42410[Voc->field_1].mSize = bx;
			dword_42500[Voc->field_1].mBuffer = VocFile;
			dword_42500[Voc->field_1].mSize = bx;
			dword_425F0[Voc->field_1].mBuffer = VocFile;
			dword_425F0[Voc->field_1].mSize = bx;
			dword_426E0[Voc->field_1].mBuffer = VocFile;
			dword_426E0[Voc->field_1].mSize = bx;
		}
	}
}

void cFodder::Music_Unk( int16 pTrack ) {
	
	const char* Tracks[] = {
		"rjp.JON(1)",
		"",
		"",
		"",
		"",
		"",
		"rjp.JON(7)",
		"rjp.JON(8)",
		"",
		"",
		"",
		"",
		"rjp.JON(13)",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"rjp.JUNBASE(2)",
	/*		"des",
			"ice",
			"mor",
			"int",
			"hid"*/
	};

	Music_Play( Tracks[pTrack] );

}

void cFodder::Sound_Voc_Play( sSprite_0* pSprite, int16 pData4, int16 pData8 ) {
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

	pData8 -= pSprite->field_0;
	int16 DataC = mCamera_Adjust_Row >> 16;
	DataC += 0x6C;

	DataC -= pSprite->field_4;
	int16 Saved0 = Data0;
	int16 Saved4 = pData4;

	sub_2B378( Data0, pData4, pData8, DataC );

	Data0 /= 0x10;
	pData8 = 0x28;
	pData8 -= Data0;

	//TODO: Correct?
	if (pData8 <= 0)
		pData8 = 4;

	Data0 = Saved0;
	pData4 = Saved4;

	int16 bx = mMap_TileSet;
	
	sVocLoaded* eax = &word_42316[bx][pData4];
	if (eax->mSize == 0)
		return;

	SDL_RWops *rw = SDL_RWFromMem( eax->mBuffer, eax->mSize );

	sVocPlaying Playing;
	Playing.mCurrentChunk = Mix_LoadWAV_RW( rw, 1 );
	Playing.mChannel = Mix_PlayChannel( -1, Playing.mCurrentChunk , 0 );

	if (Playing.mChannel == -1) {
		Mix_FreeChunk( Playing.mCurrentChunk );
		return;
	}

	mMixerChunks.push_back( Playing );
}

void cFodder::Briefing_Intro_Jungle( ) {
	
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	word_42062 = (uint8*) word_4286B;

	byte_42070 = 0xE0;


	sub_1590B();

	paletteLoad( (word_42861 + word_4286D) - 0x300, 0x100, 0 );

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

		word_42062 = ((uint8*) word_4286B) + word_428CE[word_428CC / 2];

		mDrawSpritePositionX = mHelicopterPosX >> 16;		// X
		mDrawSpritePositionY = mHelicopterPosY >> 16;		// Y 
		word_4206C = 0x40;
		word_4206E = 0x18;
		if (Sprite_OnScreen_Check())
			video_Draw_Sprite_();

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

		videoSleep_50();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		mouse_GetData();
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

		paletteLoad( (word_42861 + word_4286D) - 0x300, 0x100, 0 );
		mImage->paletteFadeOut();
		mImageFaded = -1;
	}

}

void cFodder::sub_15A36( uint8* pDs, int16 pCx ) {
	int16 ax = pCx >> 2;
	int16 dx = ax;

	ax -= 0x50;
	word_4285F = -ax;

	word_4285D = mImage->GetSurfaceBuffer() + word_4285B + (dx*4) + 4;
	pCx &= 3;
	int8 ah = 1;
	ah <<= pCx;

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
	int8 ah = 1;
	ah <<= pCx;

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

void cFodder::Mission_Brief() {

	map_Load_Resources();
	Sprite_SetDataPtrToBase( off_42918 );
	Music_Unk( 0x07 );
	sub_136D0();

	sub_15DF0();
	//video_unk_0_1();
	mImage->clearBuffer();
	sub_15DF0();

	switch (mMap_TileSet) {
	case 0:
		Briefing_Intro_Jungle();
		break;

	case 1:
		//sub_15397();
		break;

	case 2:
		//sub_151C6();
		break;

	case 3:
		//sub_15568();
		break;

	case 4:
		//sub_15739();
		break;
	}

	g_Resource.fileLoadTo( "pstuff.dat", mDataPStuff );
}

void cFodder::map_Load_Resources() {
	std::string MapName = map_Filename_MapGet();
	std::string JunData1 = "p1.dat";
	std::string JunData2 = "p2.dat";
	std::string JunData3 = "p3.dat";
	std::string JunData4 = "p4.dat";
	std::string JunData5 = "p5.dat";

	g_Resource.fileLoadTo( MapName, mMap );

	map_SetTileType();

	JunData1.insert( 0, mMapTypes[mMap_TileSet] );
	JunData2.insert( 0, mMapTypes[mMap_TileSet] );
	JunData3.insert( 0, mMapTypes[mMap_TileSet] );
	JunData4.insert( 0, mMapTypes[mMap_TileSet] );
	JunData5.insert( 0, mMapTypes[mMap_TileSet] );

	size_t DataBaseBlkSize = g_Resource.fileLoadTo( JunData1, mDataBaseBlk );
	word_42861 = mDataBaseBlk;
	word_4286D = DataBaseBlkSize;

	g_Resource.fileLoadTo( JunData2, mDataSubBlk );
	word_42863 = mDataSubBlk;
	
	g_Resource.fileLoadTo( JunData3, mDataHillBits );
	word_42865 = mDataHillBits;

	g_Resource.fileLoadTo( JunData4, mDataArmy );
	word_42867 = mDataArmy;

	g_Resource.fileLoadTo( JunData5, mDataPStuff );
	word_42869 = mDataPStuff;

	g_Resource.fileLoadTo( "paraheli.dat", (uint8*) mMapSptPtr );
	word_4286B = mMapSptPtr;

	uint8* si = ((uint8*)mMapSptPtr) + 0xF00;
	si += 0x30 * mMap_TileSet;

	memcpy( (word_42861 + DataBaseBlkSize) - 0x60, si, 0x30 );
	memcpy( (word_42861 + DataBaseBlkSize) - 0x30, mDataPStuff + 0xA000, 0x30 );
}

void cFodder::sub_15DF0( ) {

	word_3A01A = 0xB5;
	Briefing_Draw_MissionName();
}

void cFodder::Briefing_Draw_MissionName( ) {
	
	std::stringstream Mission;
	Mission << "MISSION ";

	word_3AC19 = 0x25;
	Mission << tool_StripLeadingZero( tool_NumToString( mMissionNumber ) );

	String_CalculateWidth( 0x140, byte_4382F, Mission.str().c_str() );
	String_Print( byte_4382F, 1, word_3B301, 0, Mission.str().c_str() );
	
	int16 Data0 = mMissionNumber;
	const char** Data20 = mMapNames;

	if (word_3A01A != 0xB5) {
		Data20 = mMapPhaseNames;
		Data0 = mMapNumber + 1;
	}

	Data0 -= 1;
	Data20 += Data0;

	String_CalculateWidth( 0x140, byte_4382F, *Data20 );
	Data0 = mMissionNumber;
	Data20 = mMapNames; 
	if (word_3A01A != 0xB5) {
		Data20 = mMapPhaseNames;
		Data0 = mMapNumber + 1;
	}
	Data0 -= 1;
	Data20 += Data0;
	String_Print( byte_4382F, 1, word_3B301, word_3A01A, *Data20 );
}

void cFodder::Recruit_Show() {
	mImage->clearBuffer();

	mouse_Setup();
	map_ClearSpt();
	
	word_3E1B7 = mDataBaseBlk;
	g_Resource.fileLoadTo( "hill.dat", mDataBaseBlk );
	paletteLoad( word_3E1B7 + 0xFA00, 0x50, 0x00 );

	g_Resource.fileLoadTo( "hillbits.dat", mDataHillBits );
	paletteLoad( mDataHillBits + 0x6900, 0x10, 0xB0 );

	dword_3B1FB = stru_373BA;
	word_3AAD1 = -1;
	word_3AB39 = -1;
	
	Sprite_SetDataPtrToBase( mHillBitsSpriteSheetPtr );

	sub_16BC3();
	sub_16C6C();
	Recruit_Render_LeftMenu();
	Recruit_Draw_Hill();
	
	sub_17B64();
	
	Sprite_SetDataPtrToBase( off_35E42 );
	
	sub_17CD3();

	mImage->Save();
	mImage->paletteSet( mPalette );
	mImage->paletteFade();
	word_3BEC1 = 0;
	word_3BEC3 = 0x1D;
	//word_39020 = 0;
	
	sub_17368();
	
	dword_3AAC9 = mDataSubBlk;
	
	for (int16 ax = mSquadMemberCount - 1; ax >= 0;--ax )
		sub_17429();
	
	word_3AA51 = -1;
	word_3A016 = 0;
	word_3AA67 = 0;
	word_3AA71 = mMapPlayerTroopCount;
	
	sub_1787C();

	Recruit_Draw_Actors();
	Recruit_Draw_Actors();
	
	word_39F02 = 0;
	
	for( ;; ) {
		if( word_39F02 ) {
			word_39F02 = 0;
			
			if( sub_18C7A() )
				break;
		}

		Recruit_Draw();
	}
	
	mMouseSpriteNew = 0x23;
	word_3AA51 = 0;
	
	mImage->paletteFadeOut();
	
	while( mImage->GetFaded() == false ) {
		Recruit_Draw();
	}
}

void cFodder::Recruit_Draw_Hill( ) {
	word_42062 = word_3E1B7 + 0xA00;
	
	mDrawSpritePositionX = 0x40;
	mDrawSpritePositionY = 0x28;
	word_4206C = 0x110;
	word_4206E = 0xB0;
	word_42078 = 0x140;
	
	video_Draw_Linear_To_Planar();
	
	for( uint32 x = 0; x < 0xA000; ++x) {
		word_3E1B7[x] = 0;
	}
}

void cFodder::Recruit_Draw_HomeAway( ) {
	const char* strHomeAndAway = "HOME                AWAY";
	
	Sprite_SetDataPtrToBase( mHillBitsSpriteSheetPtr );
	
	sub_13C1C( 0x18, 0, 0, 0 );
	
	int16 Data4 = word_3E0E5[ (mMissionNumber - 1) ];

	sub_13C1C( 0x19, 0, Data4, 0x130 );
	
	String_CalculateWidth( 320, mUnkStringModifier_Recruit, strHomeAndAway );
	String_Print( mUnkStringModifier_Recruit, 0x0D, word_3B301, 0x0A, strHomeAndAway );
	
	sub_13C1C( 0x0E, 0x0A, 0, 0x9B );
	
	std::string Home = tool_StripLeadingZero(tool_NumToString( word_397AE ));
	sub_16B55( 0x0D, 0x9A - (Home.length() * 0x0C), 0x0A, Home );

	std::string Away = tool_StripLeadingZero(tool_NumToString( word_397AC ));
	sub_16B55( 0x0D, 0xAA, 0x0A, Away );

	Sprite_SetDataPtrToBase( off_35E42 );
}

void cFodder::sub_16B55(  int16 pParam0, int16 pParam8, int16 pParamC, const std::string& pString ) {

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

		sub_13C1C( pParam0, pParamC, NextChar, pParam8 );
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
	int16 *si = word_391D2;
	
	while( *si != -1 ) {
		uint8* si1 = (uint8*) si;
		
		*(si1+1) += 1;
		++si;
	}
}

void cFodder::sub_17C91( cSurface *mImage, int16 pData0, int16 pData8, int16 pDataC ) {
	pData0 >>= 1;
	if (pData0 > 8)
		pData0 = 0;

	int16 Data4 = pDataC;
	Data4 -= 0x14;
	Data4 >>= 5;
	if (Data4 >= 5)
		Data4 = 4;

	pDataC -= 8;
	sub_13C1C( pData0, pDataC, Data4, pData8 );
}

void cFodder::sub_17CD3( ) {
	int16 Data0 = 1440;
	int32 Data1C = -1;
	int16* Data24 = word_391D2;

	for(; *Data24 >= 0 ; ++Data24) {
		Data0 -= 4;
		Data1C++;
	}

	if (Data24 == word_391D2)
		return;

	--Data24;
	int16* Data20 = word_3E1B9 + (Data0 / 2);

	do {
		Data0 = *Data24;
		if (Data0 < 0)
			return;

		--Data24;
		int16 Data8 = *Data20++;
		int16 DataC = *Data20++;

		sub_17C91( mImage, Data0, Data8, DataC );

		--Data1C;
	} while (Data1C >= 0);

}

void cFodder::Recruit_Render_LeftMenu() {
	int16 Data0 = 0xAD;
	int16 Data8 = 0;
	int16 DataC	= 0x18;
	
	// Draw Heroes Heading
	sub_145AF( Data0, Data8, DataC );
	
	sHero* Data20 = mHeroes;
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
		sub_145AF( Data0, Data8, DataC + 0x18 );
		DataC += 0x0C;
		
	} while( DataC < 0x4A );

	//seg003:1E89

	// Draw Squad Heading
	Data0 = 0xAE;
	Data8 = 0;
	DataC = 0x4A + 0x18;
	sub_145AF( Data0, Data8, DataC );
	
	Data14 = mMapPlayerTroopCount + mSquadMemberCount;
	Data14 /= 0x0C;
	DataC = 0x58;
	Data14 += DataC;

	// Draw Used Slot into Temporary Area
	do {
		Data0 = 0xA9;
		if( DataC >= Data14 )
			Data0 = 0xAB;

		Data8 = 0;
		sub_145AF( Data0, Data8, DataC + 0x18 );
		DataC += 0x0C;
		
	} while( DataC < 0xA0 );

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
		sSquad_Member* Data2C = &mSquad[x];
		
		if( Data2C->field_4 == INVALID_SPRITE_PTR ||  Data2C->field_4 == 0 )
			continue;
		
		if( mSquad_Selected != Data2C->field_4->field_32)
			continue;

		sRecruit* Data28 = &mRecruits[Data2C->mRecruitID];
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

				sub_145AF( Data0, Data8, DataC + 0x19 );
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
		sSquad_Member* Data38 = &mSquad[x];

		if( Data38->field_4 == INVALID_SPRITE_PTR || Data38->field_4 == 0 )
			continue;
		
		Data4 = mSquad_Selected;
		sSprite_0* Data34 = Data38->field_4;
		
		if( Data4 != Data34->field_32  )
			continue;
		
		int16 Data8 = 0;
		int16 DataC = word_3A3BD;
		DataC += 0x4A;
		DataC += word_3AA55;
		
		if( !word_3AAC7 ) {
			Data0 = 0;
			Data0 = Data38->mRank;
			Data0 += 9;
			Data8 = 0x23;
			DataC = word_3A3BD;
			DataC -= 1;
			DataC += 0x4A;
			DataC += word_3AA55;
			DataC += 0x19;
			
			sub_145AF( Data0, Data8, DataC );
			
			DataC -= 0x19;
			
		} else {
			// Draw Kills
			sub_170A4( Data38->mNumberOfKills, 0x43 );
			
		}
		
		//Draw Rank
		Data0 = Data38->mRank + 9;
		DataC = word_3A3BD - 1;

		DataC += 0x4A;
		DataC += word_3AA55;
		DataC += 0x19;
		sub_145AF( Data0, 0, DataC );
		DataC -= 0x19;
		word_3A3BD += 0x0C;
	}
}

void cFodder::sub_170A4( int16 pData4, int16 pData10 ) {
	
	pData10 -= 0x30;
	std::string Data20 = tool_StripLeadingZero(tool_NumToString( pData4 ));
	uint16 Data0 = (uint16) Data20.length() * 4;

	int16 Data8 = 0x30 - Data0;
	int16 DataC = 0x4B + word_3A3BD + word_3AA55;

	for (std::string::iterator CharIT = Data20.begin(); CharIT != Data20.end(); ++CharIT) {

		Data0 = (*CharIT & 0xFF) + pData10;
		sub_145AF( Data0, Data8, DataC + 0x19 );
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

		sub_145AF( Hero->mRecruitID + 9, Data8, DataC );

		sRecruit* Troop = &mRecruits[ Hero->mRecruitID ];

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

			sub_145AF( Character, Data8, DataC  + 0x19 );
		}

		sub_170A4( Hero->mKills, 0x67 );
		word_3A3BD += 0x0C;
	}
}

void cFodder::sub_17368() {
	uint32* Data20 = (uint32*) mDataArmy;

	uint16* Data24 = (uint16*) mDataSubBlk;

	for ( int16 Data0 = 0x58; Data0 < 0xA0; Data0 += 0x0C) {
		*((uint32*)Data24) = (uint32) Data20;
		Data24 += 2;

		*Data24++ = Data0;

		sub_17480( Data0, 0x0C, 0, Data20 );
	}

	//seg003:2532
	*Data24 = -1;
	int16 DataC = 0x58;

	for (int16 Data4 = 5; Data4 >= 0; --Data4) {
		sub_145AF( 0xAC, 0, DataC + 0x18 );

		DataC += 0x0C;
	}
}

void cFodder::sub_17429() {
	uint8* Data24 = dword_3AAC9;

	if (*((uint32*)Data24) < 0)
		return;

	uint8* Data20 = (uint8*) *((uint32*)Data24);
	Data24 += 4;

	uint16 Data0 = *((uint16*)Data24);
	Data24 += 2;

	dword_3AAC9 = Data24;
	uint32* Dataa20 =  (uint32*) Data20;
	sub_17480( Data0, 0x0C, -1, Dataa20 );
}

void cFodder::sub_17480( uint16 pData0, int16 pData4, int16 pData8, uint32*& pData20 ) {
	pData0 += 0x18;

	uint8* SptPtr = (uint8*)mMapSptPtr;

	if (pData8 == 0) {
		uint32* esi = (uint32*)(SptPtr + (0x0C * pData0));

		for (int16 cx = pData4; cx > 0; --cx) {
			*pData20++ = *esi;
			*pData20++ = *(esi + 0x258);
			*pData20++ = *(esi + 0x4B0);
			*pData20++ = *(esi + 0x708);
			++esi;

			*pData20++ = *esi;
			*pData20++ = *(esi + 0x258);
			*pData20++ = *(esi + 0x4B0);
			*pData20++ = *(esi + 0x708);
			++esi;

			*pData20++ = *esi;
			*pData20++ = *(esi + 0x258);
			*pData20++ = *(esi + 0x4B0);
			*pData20++ = *(esi + 0x708);
			++esi;
		}
	}
	else {
		uint32* edi = (uint32*)(SptPtr + (0x0C * pData0));

		for (int16 cx = pData4; cx > 0; --cx) {
			*edi = *pData20++;
			*(edi+0x258) = *pData20++;
			*(edi+0x4B0) = *pData20++;
			*(edi+0x708) = *pData20++;
			++edi;
			*edi = *pData20++;
			*(edi+0x258) = *pData20++;
			*(edi+0x4B0) = *pData20++;
			*(edi+0x708) = *pData20++;
			++edi;
			*edi = *pData20++;
			*(edi+0x258) = *pData20++;
			*(edi+0x4B0) = *pData20++;
			*(edi+0x708) = *pData20++;
			++edi;
		}
	}
}

void cFodder::Recruit_Draw_Actors( ) {
	sMouseData* bp = mouseData1 + 1;

	word_42072 = 2;

	Recruit_Draw_Truck();
	Recruit_Draw_Troops();

	bp = mouseData1;
	bp->anonymous_0 = 0;
	bp->anonymous_2 = word_42072;
}

void cFodder::sub_175C0() {
	struct_4* Data20 = 0;

	if (word_3B1F1 > 0) {

		struct_4* Data20 = &stru_373BA[293];
		do {
			if (Data20 == &stru_373BA[0])
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

	Data20 = &stru_373BA[293];
	struct_4* Data28 = &stru_373BA[271];

loc_17686:;
	for (; Data20 != &stru_373BA[0];) {
		--Data20;

		if (Data20->field_4 == 0)
			continue;

		if (Data20->field_4 < 4)
			continue;

		if (Data20 >= Data28)
			continue;

		int16 Data8 = 3;
		struct_4* Data30 = Data20;
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

	if (word_3B1EF) {
		sub_175C0();
	}
	else {
		sub_178DD();
		sub_17911();
	}

	struct_4* Data20 = stru_373BA;
	struct_4* dword_3B1C7;

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

			if (Data20 > &stru_373BA[0x46]) {
				Data4 = 1;

				if (Data20 > &stru_373BA[0x7A])
					Data4 = 2;
			}
		}
		else
			Data0 = 9;

		//loc_1784C
		DataC -= 8;
		if (Data0 != 9)
			sub_13C1C( Data0, DataC, Data4, Data8 );
		else {
			++Data8;
			--DataC;
			sub_13C8A(  Data0, Data4, Data8, DataC );
		}
	}
}

void cFodder::sub_1787C() {
	
	if (word_390B8)
		return;
	
	word_3B1F1 = mMapPlayerTroopCount;
	word_3B1EF = 0;

	int16 Data0 = 0xB4;
	int16* Data20 = word_3B1CF;
	const int16* Data24 = word_3E177;

	for ( int16 Data4 = 0x0E; Data4 >= 0; --Data4 ) {

		*Data20++ = Data0;
		Data0 -= *Data24++;

	}
}

void cFodder::sub_178DD() {
	struct_4* Data20 = stru_373BA;

	for (; Data20->field_0 >= 0; ++Data20) {
		
		if (Data20->field_4 >= 4)
			Data20->field_4 = 0;
	}
}

void cFodder::sub_17911() {
	struct_4* Data24 = stru_373BA;
	int16*    Data2C = word_3E197;

	uint32 Data8 = (uint32) dword_394A4;
	int16 Data0;

	Data8 -= (uint32) word_391D2;
	Data8 >>= 1;
	Data8 &= 0x1E;

	if (word_390B8)
		Data0 = word_390CE;
	else {
		Data0 = word_390E8;
		if (!Data0)
			goto loc_179B2;
	}

	// loc_1795E
	--Data0;
	int16 Data4 = 0x660;

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
		word_3B1EF = -1;
}

void cFodder::Recruit_Draw_Truck( ) {
	int16 Data0 = 0x22;
	int16 Data4 = word_3AACD;
	int16 Data8 = 0x31;
	int16 DataC = 0xB6;

	sub_13C1C(  Data0, DataC, Data4, Data8 );

	if (!word_3B1EF)
		return;

	uint16* Data20 = word_3BDAD;

	Data0 = *(Data20 + word_3AACF);
	if (Data0 < 0)
		return;

	++word_3AACF;
	word_3AACD = Data0;
}

void cFodder::sub_17B64() {

	const struct_3* stru = stru_35C70;

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

			uint8* Data20 = sub_2AE81( &Data0, &Data4 );

			Data8 = *Data34++;
			if (Data8 < 0) {
				++stru;
				break;
			}

			sub_2AEB6( Data0, Data4, &Data8, &DataC );
			int16 Data10 = word_3B1A3 + 0x08;
			int16 Data14 = word_3B1A5;
			sub_2AF19( Data0, Data4, Data8, Data10, Data14, DataC, Data20 );
			word_3B1A3 += 0x10;
		}
	}

}

void cFodder::Recruit_Draw() {
	Mouse_Inputs_Get();

	Recruit_Draw_Actors();
	sub_144A2();
	Recruit_Draw_HomeAway();
	Mouse_DrawCursor();
	sub_13800();

	if (mImage->GetFaded() == false )
		mImage->paletteFade();

	mDrawSpritePositionX = 0x40;
	mDrawSpritePositionY = 0x28;
	word_4206C = 0x110;
	word_4206E = 0xB0;

	//sub_14367();

	g_Window.RenderAt( mImage, cPosition() );
	g_Window.FrameEnd();
	mImage->Restore();
}

uint8* cFodder::GetSpriteData( uint16 pSegment ) {
	
	switch ( pSegment ) {
		case 0x3B68:
			return mDataBaseBlk;
			break;

		case 0x4307:
			return mDataPStuff;
			break;

		case 0x4309:
			return mDataHillBits;
			break;

		case 0x430B:
			return mDataArmy;
			break;

		case 0x6717:
			return word_3E1B7;
			break;

		default:
			std::cout << "Invalid Sprite\n";
			break;
	}
	return 0;
}

bool cFodder::sub_18C7A() {
	word_3B2FD = 0;
	word_3B2FF = 0;

	if (mMouseX <= -16 && mMouseY <= 26) {
		word_3B2FD = -1;
		return true;
	}

	if (mMouseX >= 0x10F) {
		if (mMouseY > 0x1A)
			return true;

		if (word_3E0E5[mMissionNumber-1])
			return true;

		return false;
	}

	if (mMouseY < 0x1A)
		return false;

	return true;
}

void cFodder::sub_22B71( sSprite_0* pSprite ) {
	
	if (!pSprite->field_66)
		return;

	sSprite_0* Data28 = pSprite->field_66;
	
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
	sub_2A74F( Data0, Data4, Data8, Data10, DataC );
	
	if (Data0 >= 0x0D)
		return;

	pSprite->field_6A = Data28;
	pSprite->field_6E = -1;
	sSprite_0* Data20 = 0;

	sub_305D5( Data20 );

	sSprite_0** Data24 = off_3BDEF[pSprite->field_32];
	sSprite_0* eax = Data24[0];

	if (eax != pSprite)
		return;

	word_3B461[pSprite->field_32] = 0x190;
}

void cFodder::GUI_Handle_Button_ShowOverview() {

	if (word_3B4F1)
		return;

	if (sub_30E0B() < 0)
		return;

	mMission_ShowMapOverview = -1;
}

void cFodder::sub_3037A( ) {
	
	if (sub_2F90B()) {

		if (word_3AC4B < 0)
			return;

		word_3AC4B = -1;
		sub_303AE();
		return;
	}

	if (!word_3AC4B)
		return;

	word_3AC4B = 0;
	word_3BDE7 = 1;

	sub_2F7D7();
	sub_2F7D7();
}

void cFodder::sub_3049B( ) {

	if (sub_2F90B()) {

		if (word_3AC4D < 0)
			return;

		word_3AC4D = -1;
		sub_304CB();
		return;
	}

	if (word_3AC4D) {
		word_3AC4D = 0;
		word_3BDE9 = 1;

		sub_2F871();
	}
}


void cFodder::sub_304CB( ) {
	sub_2F871();
}

void cFodder::sub_30AB0() {

	if (mMouseX < 0x10) {
		mMouseSpriteNew = 0x23;
		word_3A9F3 = 0;
		word_3A9F5 = 0;
		return;
	}

	if (mButtonPressRight)
		goto loc_30CBC;

	const int16* Data2C = word_4547E;
	const int16* Data30 = word_4555C;
	const int16* Data34 = word_4563A;

	int16 Data0 = mMouseX;
	int16 Data4 = mMouseY;

	Data0 += mCamera_Adjust_Col >> 16;
	Data4 += mCamera_Adjust_Row >> 16;

	Data0 -= 0x0F;
	Data4 -= 3;
	
	sSprite_0** Data24 = dword_3B213;

	//loc_30B36
	for (int16 Data1C = 4; Data1C >= 0; --Data1C) {

		if (*Data24 == INVALID_SPRITE_PTR )
			goto loc_30CBC;

		if (*Data24 == 0)
			continue;

		sSprite_0* Data20 = *Data24++;
		if (Data20->field_0 < 0)
			continue;

		if (!Data20->field_65)
			continue;

		if (Data20->field_22)
			continue;

		int16 Data18 = Data20->field_18;
		int16 Data8 = Data20->field_0;
		if (Data20->field_6F == 0)
			goto loc_30BB9;
		if (Data20->field_6F == 1)
			goto loc_30BBE;

	loc_30BB9:;
		Data8 -= 8;
	loc_30BBE:;

		if (Data0 < Data8)
			continue;

		Data8 += Data2C[Data18];
		Data8 -= Data20->field_20;
		Data8 -= Data30[Data18];
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

		if (Data20 != dword_3B20B)
			return;

		if (Data20->field_8 == 0xA5)
			return;

		mMouseSpriteNew = 0x27;
		word_3A9F3 = 0;
		word_3A9F5 = 0;
		return;

	loc_30C7C:;

		if (Data20 == dword_3B20B) {
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

void cFodder::sub_29E30( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
	const int8* Data24 = byte_3ECC0;

	pData8 -= pData0;
	if (pData8 < 0)
		pData8 = -pData8;

	if (pData8 >= 0x140)
		goto loc_29EBB;

	pDataC -= pData4;
	if (pDataC < 0)
		pDataC = -pDataC;

	if (pDataC >= 0x140)
		goto loc_29EBB;

	int16 Data10 = 0;
	for (;;) {
		if (pData8 <= 0x1F)
			if (pDataC <= 0x1F)
				break;

		pData8 >>= 1;
		pDataC >>= 1;
		++Data10;
	}
	//loc_29E8C
	pDataC <<= 5;
	pDataC |= pData8;
	pData0 = 0;

	pData0 = Data24[pDataC];
	pData0 <<= Data10;
	return;

loc_29EBB:;
	pData0 = 0x3E8;
}

int16 cFodder::sub_29EC2( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
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
	time_t now = time(0);

	tm ltm;
	
	localtime_s( &ltm, &now );

	uint16 ax = tool_DecimalToBinaryCodedDecimal( ltm.tm_sec );
	ax |= tool_DecimalToBinaryCodedDecimal(ltm.tm_min) << 8;
	ax += 0x40B;

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

void cFodder::Mission_Sidebar_MapButton_Render() {
	
	if (word_3B4D9)
		return;

	sub_145AF( 0xD0, 0, 0xBD );

	struct_6* Data20 = dword_3AEF3;
	Data20->field_0 = &cFodder::sub_2EAC2;
	Data20->field_4 = 0;
	Data20->field_6 = 0x2F;
	Data20->field_8 = 0xBD;
	Data20->field_A = 0x0B;
	Data20->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_ShowOverview;
	++Data20;

	sub_2ECC7( Data20 );
	word_3B4D9 = -1;

}

void cFodder::Sprite_Movement_Calculate( sSprite_0* pSprite ) {
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

	int32 tmp = (pSprite->field_2 & 0xFFFF | pSprite->field_0 << 16);
	tmp += Dataa8;

	pSprite->field_2 = tmp & 0xFFFF; 
	pSprite->field_0 = tmp >> 16;

	if (pSprite->field_0 < 0) {
		pSprite->field_0 = 0;
		pSprite->field_2 = 0;
		word_3B173 = -1;
	}

	tmp = (pSprite->field_6 & 0xFFFF | pSprite->field_4 << 16);
	tmp += DataaC;
	pSprite->field_6 = tmp & 0xFFFF;
	pSprite->field_4 = tmp >> 16;

	if (pSprite->field_4 < 0) {
		pSprite->field_6 = 0;
		pSprite->field_4 = 0;
		word_3B173 = -1;
	}
}

int16 cFodder::sub_2A1F0( sSprite_0* pSprite, int16& pData0, int16& pData4 ) {
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

void cFodder::sub_2A3D4( sSprite_0* pSprite ) {
	
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

	uint8* es = mMap;

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
	int16 *Data2C = word_3C09D;
	uint8* Data30 = byte_3D4A5;
	word_3B2CB = -1;

	for (;;) {
		if (word_3B2CB) {
			word_3B2CB = 0;
			goto loc_2A6D7;
		}
		//loc_2A6A1
		pData0 = readLEWord( Data20 );
		pData0 = Data2C[pData0];
		if (pData0 >= 0) {
			pData0 &= 0x0F;

			if (Data30[pData0]) {
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

void cFodder::sub_2A74F( int16& pData0, int16& pData4, int16& pData8, int16& pData10, int16& pDataC ) {
	const int8* Data24 = byte_3ECC0;

	pData8 -= pData0;
	if (pData8 < 0)
		pData8 = -pData8;

	if (pData8 >= pData10)
		goto loc_2A7DB;

	pDataC -= pData4;
	if (pDataC < 0)
		pDataC = -pDataC;

	if (pDataC >= pData10)
		goto loc_2A7DB;

	pData10 = 0;
	for (;;) {
		if (pData8 <= 0x1F)
			if (pDataC <= 0x1F)
				break;

		pData8 >>= 1;
		pDataC >>= 1;
		++pData10;
	}
	//loc_2A7AD
	pDataC <<= 5;
	pDataC |= pData8;
	pData0 = 0;

	pData0 = Data24[pDataC];
	pData0 <<= pData10;

	return;

loc_2A7DB:;
	pData0 = 0x3E8;
}

int16 cFodder::sub_2A7E2( int16& pData0, int16& pData4 ) {
	int16 Data10 = pData0;
	int16 Data14 = pData4;

	return Map_Terrain_Check( pData0, pData4, Data10, Data14 );
}

int16 cFodder::Map_Sprite_Check_Position( sSprite_0* pSprite, int16& pData0, int16& pData4 ) {

	pData0 += pSprite->field_4;
	if (pData0 >= 0) {
		
		int16 Data14 = pData0;
		pData4 += pSprite->field_0;
		if (pData4 >= 0) {
			int16 Data10 = pData4;

			return Map_Terrain_Check( pData0, pData4, Data10, Data14 );
		}
	}

	pData0 = -1;
	pData4 = 0;
		
	return 0;
}

int16 cFodder::Map_Terrain_Check( int16& pData0, int16& pData4, int16& pData10, int16& pData14 ) {
	pData0 >>= 4;

	uint8* es = mMap;

	pData0 *= readLEWord( &es[0x54] );
	pData4 >>= 4;

	pData0 += pData4;
	pData0 <<= 1;

	pData0 = readLEWord( &es[0x60 + pData0]) & 0x1FF;
	//Data0 <<= 1;

	pData4 = word_3C09D[pData0];

	if (pData4 < 0) {
		//loc_2A8D9
		pData10 >>= 1;
		pData10 &= 0x07;

		int16 Data8 = 7;
		Data8 -= pData10;

		pData14 >>= 1;
		pData14 &= 0x07;

		pData0 <<= 3;
		int8* Data28 = (int8*) graphicsBaseBht;
		pData14 += pData0;

		byte al = 1 << Data8;
		Data28 += pData14;
		if (*((int16*)Data28) & al)
			pData4 >>= 4;
	}

	pData4 &= 0x0F;
	pData0 = byte_3D4A5[pData4];

	return pData0;
}

void cFodder::sub_2A932( int16 pData4, int16 pData8, int16 pDataC, int16 pData10 ) {
	
	if (pDataC >= 3)
		return;

	int16 Data0 = pDataC;

	pData10 = word_3A9BA[Data0];

	struct_8* Data38 = off_3BEF3[Data0];
	int16 Data18 = 3 - word_3BED5[Data0];

	sSprite_0** Data24 = off_3BDEF[Data0];
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
	int16 Data1C = -1;

	sSprite_0** Saved_Data24 = Data24;

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
		sSprite_0* Data28 = *Data24++;

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

int16 cFodder::SquadMember_Sprite_Find_In_Region( sSprite_0* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 ) {
	
	if (word_3B4F1)
		goto loc_2AE33;

	word_3AA03 = 0;

	sSquad_Member* Data28 = mSquad;

	for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Data28 ) {

		sSprite_0* Data2C = Data28->field_4;

		if (Data2C == INVALID_SPRITE_PTR)
			continue;

		int16 Data0 = Data2C->field_0;
		Data0 += 9;

		if (pData8 > Data0)
			continue;

		if (pData8 < Data2C->field_0)
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

		int16 Data4 = pData14;
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
		word_3AA03 += 1;

		if (word_3AA45)
			break;
	}

loc_2AE33:;
	word_3AA45 = 0;
	return word_3AA03;
}

uint8* cFodder::sub_2AE81( int16 *pData0, int16 *pData4 ) {
	const sSpriteSheet* Sheet = &mSpriteDataPtr[*pData0][*pData4];

	*pData0 = Sheet->mColCount;
	*pData4 = Sheet->mRowCount;

	return GetSpriteData( Sheet->field_2 ) + Sheet->field_0;
}
 
void cFodder::sub_2AF19( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 ) {
	pData0 &= 0xFFFF;
	pData4 &= 0xFFFF;

	uint8 *si = pData20;
	uint8* es = word_3E1B7;

	dword_44A36 = (pData10 - (pData8 >> 1)) << 16;
	dword_44A3E = dword_44A36;

	dword_44A3A = (pData14 - (pDataC >> 1)) << 16;
	int32 eax = (pData8 << 0x10);
	if (eax <= 0)
		return;

	dword_44A42 = eax / pData0;
	eax = pDataC << 0x10;
	if (eax <= 0)
		return;

	dword_44A46 = eax / pData4;
	for (int16 bx = 0; bx != pData4; ++bx) {
		dword_44A36 = dword_44A3E;

		for (int16 cx = 0; cx != pData0; ++cx) {
			uint8 al = sub_2AFF5( si, bx, cx );
			sub_2B016( es, al );
			dword_44A36 += dword_44A42;
		}

		dword_44A3A += dword_44A46;
   	}

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

void cFodder::sub_2B04B( uint8* pTileGraphicPtr, uint16 pDestX, uint16 pDestY ) {
	uint8* Target = mSurfaceMapOverview->GetSurfaceBuffer();

	pDestX *= 16;

	Target += (pDestY * 16) * (mMapWidth*16);
	Target += pDestX;

	for (uint16 i = 0; i < 16; ++i) {

		memcpy( Target, pTileGraphicPtr, 16 );
		pTileGraphicPtr += 0x140;
		Target += (mMapWidth*16);
	}
}

void cFodder::sub_2B378( int16& pData0, int16& pData4, int16& pData8, int16& pDataC ) {
	const int8* Data24 = byte_3ECC0;

	pData8 -= pData0;
	if (pData8 < 0)
		pData8 = -pData8;

	if (pData8 >= 0x280)
		goto loc_2B403;

	pDataC -= pData4;
	if (pDataC < 0)
		pDataC = -pDataC;

	if (pDataC >= 0x280)
		goto loc_2B403;

	int16 Data10 = 0;
	for (;;) {
		if (pData8 <= 0x1F)
			if (pDataC <= 0x1F)
				break;

		pData8 >>= 1;
		pDataC >>= 1;
		Data10 += 1;
	}

	pDataC <<= 5;
	pDataC |= pData8;
	pData0 = 0;
	pData0 = Data24[pDataC];

	pData0 <<= Data10;
	return;

loc_2B403:;
	pData0 = 0x280;
}

void cFodder::Camera_Pan() {
	
	Camera_Update_Row();
	Camera_Update_Column();

	int16 bx = mCamera_Column_Previous;
	int16 cx = mCamera_Column;
	mCamera_Column_Previous = cx;

	cx -= bx;

	mouseData1->anonymous_0 -= cx;
	mouseData0->anonymous_0 -= cx;

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
	mouseData1->anonymous_1 -= cx;
	mouseData0->anonymous_1 -= cx;

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

	map_Tiles_Draw_();
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

	map_Tiles_Draw_();
}

void cFodder::Camera_Pan_Down( ) {

	int16 ax = word_3B60E;
	ax &= 3;

	int16 word_39707 = 0;
	int16 word_39709 = 0;

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
	
	map_Tiles_Draw_();
}

void cFodder::Camera_Pan_Up() {
	int16 word_39707 = 0;
	int16 word_39709 = 0;

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

	map_Tiles_Draw_();
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

void cFodder::sub_2D06C() {
	sSprite_0** Data34 = dword_3B48B;
	int8* Data24 = mSquads_TroopCount;

	Data24[0] = 0;
	Data24[1] = 0;
	Data24[2] = 0;
	Data24[3] = 0;
	byte_3A05E = 0;

	int16 Data14 = 0;

	sSprite_0*** Data28 = off_3BDEF;
	int16 Data0 = -32768;
	sSprite_0* Data4 = INVALID_SPRITE_PTR;

	dword_3BE03[0] = INVALID_SPRITE_PTR;
	dword_3BE27[0] = INVALID_SPRITE_PTR;
	dword_3BE4B[0] = INVALID_SPRITE_PTR;
	dword_3BE6F[0] = INVALID_SPRITE_PTR;
	dword_3BE93[0] = INVALID_SPRITE_PTR;

	for (int8 Data1C = 0; Data1C < 8; ++Data1C) {
		sSquad_Member* Data2C = &mSquad[Data1C];

		if (Data2C->field_4 == INVALID_SPRITE_PTR || Data2C->field_4 == 0 )
			continue;

		sSprite_0* Sprite = Data2C->field_4;

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

		uint8 Data10 = Data24[ Sprite->field_32 ] & 0xFF;
		Data24[ Sprite->field_32 ] += 1;
		++Data14;

		sSprite_0** Data30 = Data28[ Sprite->field_32 ];
		//seg009:0151
		Data30[Data10] = Sprite;
		Data30[Data10 + 1] = Data4;
		*Data34 = Sprite;
		++Data34;
	}
	//seg009:019E

	if (!Data14)
		word_3A9AA = -1;

	dword_3A8DB = readLEDWord( &mSquads_TroopCount );
	byte_3A8DE[1] = byte_3A05E;

	Data0 = 2;
	int8* Data2CC = mSquads_TroopCount;
	int8* Data30 = byte_3BF1B;

	for (Data0 = 2; Data0 >= 0; --Data0 ) {

		int8 al = Data2CC[Data0];
		if (al)
			continue;

		Data30[Data0] = -1;
	}

	*Data34 = INVALID_SPRITE_PTR;
}

void cFodder::sub_2D26A( sSquad_Member* pData24, int16& pData8 ) {

	int16 Data0 = pData24->mRank;
	Data0 += 8;

	if (Data0 > 0x0F)
		Data0 = 0x0F;

	//Data0 *= 0x0A;

	pData8 = stru_3D35F[Data0].field_6;

}

void cFodder::Squad_Member_Rotate_Can_Fire() {
	if (word_39F04) 
		word_39F04 = 0;
	else {
		word_3ABB3 -= 2;
		if (word_3ABB3 >= 0)
			return;
	}

	//loc_2D2EB
	sSprite_0* Data20 = word_39FCE;
	if (Data20 == INVALID_SPRITE_PTR || Data20 == 0 )
		return;

	sSquad_Member* Dataa20 = (sSquad_Member*) Data20->field_46;

	word_3ABB3 = stru_3ABB9.field_4;
	word_3ABC3 = 0;
	word_3ABC5 = 0;

	if (Dataa20->mRank > 0x14) {

		if (mSquad_Selected >= 0) {

			if (mSquads_TroopCount[mSquad_Selected] == 1) {
				word_3ABB3 = 1;
				word_3ABC3 = -3;
				word_3ABC5 = 0x0A;
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
	sSprite_0** Dat20 = off_3BDEF[Data8];
	Data20 = Dat20[Data4];

	if (Data20->field_45)
		return;

	Data20->field_4A = -1;
}

int16 cFodder::Sprite_Find_In_Region( sSprite_0* pSprite, sSprite_0*& pData24, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 ) {

	if (word_3B4F1)
		goto loc_2D719;

	word_3AA03 = 0;

	pData24 = mSprites;
	const int16* Data28 = word_453A0;
	const int16* Data2C = word_4547E;
	const int16* Data30 = word_4555C;
	const int16* Data34 = word_4563A;

	for (int16 Data1C = 0x2B; Data1C >= 0; --Data1C, ++pData24) {
		int16 Data4 = pData24->field_18;

		if (!Data28[Data4])
			continue;

		if (pData24->field_0 < 0)
			continue;

		if (pDataC < pData24->field_0)
			continue;

		int16 Data0 = pData24->field_0;
		Data0 += Data2C[Data4];
		if (pData8 > Data0)
			continue;

		Data0 = pData24->field_4;
		Data0 -= Data30[Data4];
		if (pData14 < Data0)
			continue;

		//seg009:050C
		Data0 = pData24->field_4;
		Data0 += Data34[Data4];

		if (pData10 > Data0)
			continue;

		int16 Data18 = pSprite->field_20;
		Data0 = Data30[Data4];
		Data0 += 8;

		Data0 += pData24->field_20;

		if (Data18 > Data0)
			continue;
		Data18 += 0x0B;

		if (Data18 < pData24->field_20)
			continue;

		if (pData24->field_18 != 0)
			goto loc_2D5FA;

		if (pData24->field_38 != 0x0A)
			continue;

	loc_2D5FA:;
		if (pSprite->field_18 == 0x59)
			goto loc_2D620;

		if (pSprite->field_18 == 0x27)
			goto loc_2D62C;

		if (pSprite->field_18 != 0x0C)
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
		if (Data28[Data4] == 2)
			goto loc_2D659;

		word_3AA03 = 1;
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

		pData24->field_58 = 0;
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
		word_3AA03 = -1;
		pData24->field_10 = pSprite->field_10;

	loc_2D705:;
		if (word_3AA45)
			goto loc_2D719;
	}

loc_2D719:;
	word_3AA45 = 0;
	return word_3AA03;
}

void cFodder::sub_2D725() {
	sSprite_0* Data20 = mSprites;
	
	for (int16 Data0 = 0x2B; Data0 >= 0; --Data0, ++Data20 ) {

		if (Data20->field_0 == -32768)
			continue;

		if (Data20->field_18 == 0)
			sub_1EF47( Data20 );
	}
}

void cFodder::sub_2D767() {
	sSprite_0* Data20 = mSprites;

	for (int16 Data0 = 0x2B; Data0 >= 0; --Data0, ++Data20) {

		if (Data20->field_0 == -32768)
			continue;

		if (Data20->field_18 != 0)
			continue;

		if (!Data20->field_38)
			continue;

		if (Data20->field_38 >= 0x32)
			continue;

		sub_1EF47( Data20 );
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

void cFodder::sub_2D7FF() {
	int16 Data0 = mMapNumber;
	Data0 <<= 1;

	int16 Data4 = word_3DFC5[Data0];
	int16 Data8 = word_3DFC5[Data0 + 1];

	word_390C0 = Data4;
	word_390C2 = Data8;

	Data8 += Data4;

	Data8 >>= 1;

	word_390BE = Data8;
	word_390C4 = Data8;
	word_390C6 = 1;

	sSprite_0* Sprite = mSprites;
	for (int16 Data1C = 29; Data1C >= 0; --Data1C, ++Sprite ) {

		if (Sprite->field_0 == -32768)
			continue;

		if (Sprite->field_18 == 0x24 || Sprite->field_18 == 5)
			sub_2D8AF( Sprite );
	}
}

void cFodder::sub_2D8AF( sSprite_0* pSprite ) {
	
	int16 Data8 = word_390C4;
	pSprite->field_62 = Data8;

	int16 Data4 = word_390C6;
	Data8 += Data4;

	if (Data4 >= 0) {

		if (Data8 >= word_390C2) {
			Data8 = word_390C2;
			Data4 = -Data4;
		}
	}
	else {
		if (Data8 < 0 || Data8 <= word_390C0) {
			Data8 = word_390C0;
			Data4 = -Data4;
		}

	}
	//loc_2D90D
	word_390C6 = Data4;
	word_390C4 = Data8;
}

int16 cFodder::sub_2D91E( sSprite_0* pSprite ) {

	struct_8* Data24 = off_3BEF3[ pSprite->field_32 ];

	int16 Data0 = Data24[pSprite->field_40].field_0;
	int16 Data4 = Data24[pSprite->field_40].field_2;

	if (Data0 >= 0) {
		pSprite->field_26 = Data0;
		pSprite->field_28 = Data4;
		pSprite->field_40++;
		pSprite->field_42 = -1;
		return 0;

	loc_2D9B4:;
		Data0 = -1;
		return -1;
	}

	//loc_2D9C0
	if (!pSprite->field_42)
		goto loc_2D9B4;

	pSprite->field_42 = 0;

	return loc_2D9D5( pSprite );
}

int16 cFodder::loc_2D9D5( sSprite_0* pSprite ) {
	int16 Data14 = pSprite->field_32;
	int16 Data0;

	int16 Data18 = byte_3BF1B[Data14];
	if (Data18 < 0) {
		//goto loc_2D9B4;
		Data0 = -1;
		return -1;
	}

	Data18 &= 0xFF;
	int8* Data2C = mSquads_TroopCount + Data14;

	if (*Data2C > 8) {
		//goto loc_2D9B4;
		Data0 = -1;
		return -1;
	}
	uint8* Dataa24 = (uint8*) pSprite->field_46;
	*(Dataa24 + 9) &= 0xFE;

	struct_8* Data24 = off_3BEF3[Data18];
	Data0 = 0;

	for (;;) {

		struct_8 eax = *Data24++;
		if (eax.asInt < 0)
			break;

		Data0++;
	}
	//loc_2DA86
	pSprite->field_40 = Data0;
	pSprite->field_32 = Data18;
	sub_2D06C();

	word_3BEDF[Data14] = 0;
	mSquad_Selected = Data18;
		
	mSquad_Grenades[Data18] += mSquad_Grenades[Data14];

	//seg010:0232
	mSquad_Grenades[Data14] = 0;

	mSquad_Rockets[Data18] += mSquad_Rockets[Data14];
	mSquad_Rockets[Data14] = 0;

	sub_305D5( pSprite );
	word_3AC2B = 0;
	word_3A8D9 = 0;
	//goto loc_2D9B4;
	Data0 = -1;
	return -1;
}

int16 cFodder::sub_2DBA3( sSprite_0* pSprite ) {
	int8 Data2C = byte_3BF1B[pSprite->field_32];
	int16 Data0;

	if (Data2C < 0) {
		//goto loc_2D9B4;
		Data0 = -1;
		return -1;
	}

	int16 Data8 = pSprite->field_32;

	sSprite_0* Dataa2C = word_3BF1E[Data8];
	if (Dataa2C->field_18 != 0) {
		//goto loc_2D9B4;
		Data0 = -1;
		return -1;
	}
	Data8 = mSquad_Selected;

	sSprite_0** Data30 = off_3BDEF[Data8];
	int16 Data1C = 0;

	for (Data1C = 0x0C;;) {
		if (*Data30 < 0)
			break;

		sSprite_0* eax = *Data30;
		++Data30;
		if (pSprite == eax)
			break;
		Data1C += 8;
	}

	Data0 = Dataa2C->field_0;
	int16 Data4 = Dataa2C->field_4;
	Data8 = pSprite->field_0;
	int16 DataC = pSprite->field_4;
	sub_29E30( Data0, Data4, Data8, DataC );
	if (Data0 <= Data1C)
		goto loc_2DCAD;
	//goto loc_2D9AB;
	return 0;
loc_2DCAD:;
	return loc_2D9D5(pSprite);
}

void cFodder::sub_2DCB0( int16 pData0 ) {
	struct_8* Dataa24 = off_3BEF3[pData0];

	Dataa24->asInt = -1;
	sSprite_0** Data24 = off_3BDEF[pData0];

	for (;;) {

		if (*Data24 == INVALID_SPRITE_PTR)
			break;

		sSprite_0* Data28 = *Data24++;
		Data28->field_40 = 0;
		Data28->field_26 = Data28->field_0;
		Data28->field_28 = Data28->field_4;
	}

}

void cFodder::sub_2DD50( sSprite_0* pSprite ) {
	word_3B4E9 = 0;
	if (pSprite->field_18 == 0x59)
		word_3B4E9 = -1;

	struct_7* Data28 = dword_3B11B;

	int16 Data0 = pSprite->field_0;
	Data0 += 4;
	int16 Data4 = pSprite->field_4;

	Data4 -= 8;
	const int16* Data24 = word_3DDD2;

	for (;;) {
		if (*Data24 == -32768)
			break;

		Data0 += *Data24++;
		Data4 += *Data24++;

		Data28->field_0 = Data0;
		Data28->field_2 = Data4;
		++Data28;

		if (word_3B4E9)
			(Data28 - 1)->field_2 = -Data4;

		if (Data28 >= &stru_3AF0B[128])
			Data28 = stru_3AF0B;
	}

	Data28->field_0 = -1;
	Data28->field_2 = -1;

	dword_3B11B = Data28;
}

void cFodder::sub_2DE2C( ) {
	
	if (word_3AF07) {
		--word_3AF07;
		goto loc_2DFC7;
	}

loc_2DE3C:;
	int16* Data20 = (int16*) dword_3B11F;

	int16 Data0 = *Data20;
	int16 Data4 = *(Data20 + 1);

	if (Data0 < 0)
		return;

	word_3AF03 = Data0;
	word_3B4EB = 0;
	word_3AF05 = Data4;
	if (word_3AF05 < 0) {
		word_3B4EB = -1;
		Data4 = -Data4;
		word_3AF05 = Data4;
	}
	//loc_2DE89
	Data4 >>= 4;
	Data4 *= readLEWord( &mMap[0x54] );

	Data0 >>= 4;
	Data4 += Data0;
	Data4 <<= 1;

	uint8* Data24 = &mMap[0x60 + Data4];

	int16 Data10 = readLEWord( Data24 );
	Data10 &= 0x1FF;
	int16 DataC = Data10;

	Data4 = word_3D03D[Data10];
	if (Data4 < 0)
		goto loc_2DF55;

	if (word_3B4EB)
		goto loc_2DF7B;

	//seg010:0608 
	const int16* Data30 = off_3DE38[mMap_TileSet];

	int16 ax;
	do {
		if (*Data30 < 0)
			goto loc_2DF7B;

		ax = *Data30;
		Data30++;

	} while (DataC != ax);

loc_2DF55:;
	sub_2E01C();
	goto loc_2DE3C;

//seg010:064C
	//UNUNSED BLOCK

loc_2DF7B:;

	int16 Data8 = word_3AF03;
	DataC = word_3AF05;
	Data0 = sub_21914(Data8, DataC);

	if (Data0)
		return;

	word_3AF07 = 1;
	writeLEWord( Data24, Data4 );
	word_3AF01 = Data4;
	sub_2E01C();

loc_2DFC7:;
	ax = word_3AF03;
	ax >>= 4;
	ax -= word_3B612;
	ax <<= 4;
	ax -= word_3B60E;
	ax += 0x40;

	mDrawSpritePositionX = ax;

	ax = word_3AF05;
	ax >>= 4;
	ax -= word_3B614;
	ax <<= 4;
	ax -= word_3B610;
	ax += 0x10;

	mDrawSpritePositionY = ax;

	word_42062 = (uint8*) mGraphicBlkPtrs[word_3AF01];
	word_4206C = 0x10;
	word_4206E = 0x10;

	map_Tiles_Draw_();
	//mImage->Save();

	//sub_140F1();
}

void cFodder::sub_2E01C() {
	
	struct_7* Data20 = dword_3B11F;
	++Data20;
	
	if( Data20 >= &stru_3AF0B[128] ) 
		Data20 = stru_3AF0B;
	
	dword_3B11F = Data20;
}

int16 cFodder::sub_2EAC2() {
	return 0;
}

void cFodder::sub_2EAC3() {
	int16 Data0 = -1;
	return;
}

void cFodder::sub_2EACA() {
	word_3AC45 = -1;
	sub_30465();

	dword_3AEF3 = stru_3AC53;
	int16 Data8 = -1;
	int16 DataC = -1;

	map_ClearSpt();

	word_3AC29 = 0;
	int16 Data0 = 0;
	int16 Data4 = 0;
	Mission_Sidebar_Prepare(Data0, Data4);

	Data0 = 1;
	Data4 = word_3AC29;
	if (Data4)
		Data4 += 5;
	Mission_Sidebar_Prepare(Data0, Data4);

	Data0 = 2;
	Data4 = word_3AC29;
	if (Data4)
		Data4 += 5;

	Mission_Sidebar_Prepare(Data0, Data4);
	sub_2EBC4();

	word_3AC45 = 0;

	sub_30480();
	word_39EFC = 0;
}

void cFodder::sub_2EBE0( int16& pData0, int16& pData4 ) {
	word_3AC1B = pData0;
	word_3AC1D = pData4;
	word_3AC47 = 0;

	if (pData0 == mSquad_Selected)
		word_3AC47 = -1;

	//pData0 <<= 1;
	word_3AC1F = pData0;
}

void cFodder::Mission_Sidebar_Prepare( int16 pData0, int16 pData4 ) {
	
	sub_2EBE0( pData0, pData4 );

	word_3AC2D[pData0] = pData4;
	sub_2D06C();
	
	pData0 = word_3AC1B;

	if (!mSquads_TroopCount[pData0])
		return;

	Mission_Sidebar_SplitButton_Draw();
	GUI_Prepare_Button_Squad();

	sub_2EF8A();

	Mission_Sidebar_Grenades_Draw();
	GUI_Prepare_Button_Grenade();

	Mission_Sidebar_Rockets_Draw();
	GUI_Prepare_Button_Rockets();

	sub_2F452();

	Mission_Sidebar_TroopList_Draw();
	GUI_Prepare_Button_TroopName();
}

void cFodder::sub_2EBC4() {
	struct_6* Data20 = dword_3AEF3;

	sub_2EC0E( Data20, stru_3DEDC );
}

void cFodder::sub_2EC0E( struct_6* pData20, struct_6* pData24 ) {
	
	while (pData24->field_0 != 0) {

		pData20->field_0 = pData24->field_0;
		pData20->field_4 = pData24->field_4;
		pData20->field_6 = pData24->field_6;
		pData20->field_8 = pData24->field_8;
		pData20->field_A = pData24->field_A;
		pData20->mMouseInsideFuncPtr = pData24->mMouseInsideFuncPtr;

		++pData20;
		++pData24;
	}

	sub_2ECC7( pData20 );

}

void cFodder::sub_2ECC7( struct_6 *pData20 ) {

	pData20->field_0 = 0;
	dword_3AEF3 = pData20;
}

void cFodder::Mission_Sidebar_SplitButton_Draw() {

	int16 Data0 = word_3DEF0[word_3AC1F];
	int16 Data8 = 0;
	int16 DataC = word_3AC1D;

	sub_145AF( Data0, Data8, DataC );
}

void cFodder::GUI_Prepare_Button_Squad() {
	struct_6* Data20 = dword_3AEF3;

	Data20->field_0 = &cFodder::sub_2EAC2;
	Data20->field_4 = 0;
	Data20->field_6 = 0x2F;
	Data20->field_8 = word_3AC1D;
	Data20->field_A = 0x0E;

	if (word_3AC1B != mSquad_Selected) {
		Data20->mMouseInsideFuncPtr = mGUI_Handle_Button_SelectSquad_Array[word_3AC1B];
	}
	else {
		Data20->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_SplitSquad;
	}

	++Data20;
	sub_2ECC7( Data20 );
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
	
	if (sub_30E0B() < 0)
		return;

	mSquad_Selected = pData4;
	sub_301F7();
	sub_2D06C();
	
	sSprite_0* Data20 = 0;
	sub_305D5( Data20 );

	word_3A067 = Data20->field_0;
	word_3A069 = Data20->field_4;
	word_3AA1B = 1;
	word_3AA47 = 1;
	word_3AC2B = 0;
	word_3BDE7 = 1;
	word_3BDE9 = 1;
	word_3AC4B = 0;
	word_3AC4D = 0;
}

void cFodder::GUI_Handle_Button_SplitSquad() {

	if (sub_30E0B() < 0)
		return;

	if (!sub_2FF41())
		return;

	sub_30082();

	word_3BDE7 = 1;
	word_3BDE9 = 1;
	word_3AC4B = 0;
	word_3AC4D = 0;

	sub_2DCB0( mSquad_Selected );
	sub_2D06C();

	sSprite_0* Data20 = 0;
	sub_305D5( Data20 );

	word_3AA47 = 1;
	word_3AC2B = 0;
}

void cFodder::sub_2EF8A() {
	int16 Data0 = word_3AC1F;

	int16* Data20 = word_3AC3F;
	int16* Data24 = mSquad_Grenades;
	int16* Data28 = mSquad_Rockets;

	if (Data20[Data0] == 1) {

		if (Data24[Data0])
			return;
		if (!Data28[Data0])
			return;

		Data20[Data0] = 3;
		return;
	}

	//loc_2EFF1
	if (Data28[Data0])
		return;

	if (!Data24[Data0])
		return;

	Data20[Data0] = 1;
}

void cFodder::sub_17DB3() {

	g_Resource.fileLoadTo( "rankfont.dat", mDataHillBits );
	paletteLoad( mDataHillBits + 0xA000, 0x80, 0x40 );

	g_Resource.fileLoadTo( "morphbig.dat", mDataBaseBlk );
	paletteLoad( mDataBaseBlk + 0xFA00, 0x40, 0x00 );

	mImage->paletteSet(mPalette);

	Sprite_SetDataPtrToBase( off_43963 );

	Music_Unk( 0 );
	Service_KIA_Loop();
	Service_Promotion_Loop();
	mouse_Setup();

	g_Resource.fileLoadTo( "pstuff.dat", mDataPStuff );
}

void cFodder::Service_KIA_Loop() {
	sub_136D0();
	mouse_Setup();
	word_44475 = 0;

	if (Service_KIA_Troop_Prepare() < 0)
		return;

	mImage->clearBuffer();

	sub_13C1C(  5, 0, 0, 0x34 );
	sub_13C8A(  7, 0, 0, 0x31 );
	sub_13C8A(  7, 0, 0xF0, 0x31 );

	video_Draw_Unk_2();

	mImageFaded = -1;
	word_39F02 = 0;
	mImage->Save();

	do {
		Mouse_Inputs_Get();

		if (word_44475 == -1 || word_39F02 ) {
			word_39F02 = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
			word_40048 = 1;
		}

		if (mImageFaded == -1)
			mImageFaded = mImage->paletteFade();

		sub_18149();
		sub_13800();
		//sub_14445();
		sub_181BD();

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
		mImage->Restore();

	} while (mImageFaded == -1 || word_40048 == 0);
}

void cFodder::Service_Promotion_Loop() {
	sub_136D0();
	mouse_Setup();
	word_44475 = 0;
	Service_Promotion_Prepare();

	if (sub_1804C() < 0)
		goto loc_18001;

	mImage->clearBuffer();

	sub_13C1C(  6, 0, 0, 0x34 );
	sub_13C8A(  8, 0, 0, 0x31 );
	sub_13C8A(  8, 0, 0xF0, 0x31 );

	video_Draw_Unk_2();

	mImageFaded = -1;
	word_40048 = 0;
	word_39F02 = 0;
	mImage->paletteSet( mPalette );
	mImage->Save();

	do {

		Mouse_Inputs_Get();
		if (word_44475 == -1 || word_39F02 ) {
			word_39F02 = 0;
			mImage->paletteFadeOut();
			mImageFaded = -1;
			word_40048 = 1;
		}

		if (mImageFaded == -1)
			mImageFaded = mImage->paletteFade();

		Service_Promotion_Check();
		sub_18149();
		sub_13800();
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

	int16* si = word_394AC;

	if (*si == -1)
		return -1;

	for (;;) {
		int16 ax = *si++;
		if (ax == -1 )
			break;

		int16* bp = dword_394A8;
		int8 bl = *bp & 0xFF;
		++bp;
		dword_394A8 = bp;

		sub_18099( di, ax, bl );
		mDrawSpritePositionY += 0x40;
	}

	*di++ = -1;
	return 0;
}

int16 cFodder::sub_1804C() {
	uint16* di = (uint16*) mDataPStuff;

	
	mDrawSpritePositionY = 0xE8;
	Service_Mission_Text_Prepare( di );
	mDrawSpritePositionY += 0x40;

	sSquad_Member* si = mSquad;

	for (int16 word_3ABE1 = 7; word_3ABE1 >= 0; --word_3ABE1, ++si) {

		if (si->field_4 != INVALID_SPRITE_PTR ) {
			int16 ax = si->mRecruitID;
			int8 bl = si->mRank;

			bl &= 0xFF;

			sub_18099( di, ax, bl );

			mDrawSpritePositionY += 0x40;
		}
	}
	if (di == (uint16*) mDataPStuff)
		return -1;

	*di++ = -1;

	return 0;
}

void cFodder::sub_18099( uint16*& pDi, int16 ax, int16 bx) {
	word_3A014 = ax;
	word_3A016 = bx;

	*pDi++ = 9;
	*pDi++ = 0;
	*pDi++ = 0x60;
	*pDi++ = mDrawSpritePositionY;
	*pDi++ = 2;
	*pDi++ = word_3A016;
	*pDi++ = 0x64;
	*pDi++ = mDrawSpritePositionY + 4;
	*pDi++ = 2;
	*pDi++ = word_3A016;
	*pDi++ = 0xCC;
	*pDi++ = mDrawSpritePositionY + 4;

	//seg003:3237
	int16 cx = 5;
	char* si = mRecruits[word_3A014].mName;
	std::stringstream tmpString;

	for (cx = 5; cx >= 0; --cx) {
		if (*si == 0x20)
			break;

		tmpString << *si++;
	}

	String_CalculateWidth( 0x140, byte_4422D, tmpString.str().c_str() );
	sub_181E6( pDi, tmpString.str().c_str(), byte_4422D, 3, word_3B301, mDrawSpritePositionY + 6 );
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

void cFodder::sub_181E6( uint16*& pDi, const std::string& pText, uint8* pData28, int16 pData0, int16 pData8, int16 pDataC ) {

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
	
	word_42062 = GetSpriteData( mSpriteDataPtr[pData0][pData4].field_2 ) + mSpriteDataPtr[pData0][pData4].field_0;

	mDrawSpritePositionX = pData8 + 0x10;
	mDrawSpritePositionY = pDataC + 0x10;

	word_4206C = mSpriteDataPtr[pData0][pData4].mColCount;
	word_4206E = mSpriteDataPtr[pData0][pData4].mRowCount;
	byte_42070 = mSpriteDataPtr[pData0][pData4].field_C;

	if (!sub_184C7()) {
		sub_182EA();
		return 1;
	}
	return 0;
}

void cFodder::sub_182EA() {
	uint8* bp = &mFontWidths[0x80];

	uint8* di = mImage->GetSurfaceBuffer();
	
	di += (352 * mDrawSpritePositionY);

	di += (mDrawSpritePositionX + word_40054);
	word_42066 = di;

	uint8* si = word_42062;

	word_4206C >>= 1;
	word_42074 = 0xA0 - word_4206C;
	word_4206C >>= 1;

	word_42076 = 352 - (word_4206C*4);
	uint8 Plane = 0;

	for (int16 dx = 0; dx < word_4206E; ++dx ) {

		int16 bx = mDrawSpritePositionY;
		bx += dx;

		uint8 bl = *(bp + bx);

		for (int16 cx = word_4206C; cx > 0; --cx) {
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

	si = word_42062;
	di = word_42066;
	di += Plane;
	for (int16 dx = 0; dx < word_4206E; ++dx ) {

		int16 bx = mDrawSpritePositionY;
		bx += dx;

		uint8 bl = *(bp + bx);

		for (int16 cx = word_4206C; cx > 0; --cx) {
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

	++word_42062;
	si = word_42062;
	di = word_42066;
	di += Plane;

	for (int16 dx = 0; dx < word_4206E; ++dx ) {

		int16 bx = mDrawSpritePositionY;
		bx += dx;

		uint8 bl = *(bp + bx);

		for (int16 cx = word_4206C; cx > 0; --cx) {
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

	si = word_42062;
	di = word_42066;
	di += Plane;
	for (int16 dx = 0; dx < word_4206E; ++dx ) {

		int16 bx = mDrawSpritePositionY;
		bx += dx;

		uint8 bl = *(bp + bx);

		for (int16 cx = word_4206C; cx > 0; --cx) {
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
		ax += word_4206E;
		--ax;
		if (ax < 0x2C)
			return -1;

		ax -= 0x2C;
		ax -= word_4206E;
		++ax;
		ax = -ax;
		mDrawSpritePositionY += ax;
		word_4206E -= ax;

		ax *= 0xA0;
		word_42062 += ax;
	}
	//loc_184FC

	ax = mDrawSpritePositionY;
	ax += word_4206E;
	--ax;

	if (ax <= 0xE7)
		return 0;

	if (mDrawSpritePositionY > 0xE7)
		return -1;

	ax -= 0xE7;
	word_4206E -= ax;
	return 0;
}

void cFodder::Service_Mission_Text_Prepare( uint16*& pTarget ) {
	std::stringstream Mission;
	Mission << "MISSION ";

	Mission << tool_StripLeadingZero( tool_NumToString( mMissionNumber ) );

	String_CalculateWidth( 0x140, byte_4428D, Mission.str().c_str() );

	sub_181E6( pTarget, Mission.str(), byte_4428D, 4, word_3B301, mDrawSpritePositionY );
}

void cFodder::Service_Promotion_Prepare() {
	int16* Data28 = word_3ABFF;
	dword_3AC11 = Data28;

	int16 Data8 = mMissionPhaseTable[ mMissionNumber - 1 ];

	sSquad_Member* Data20 = mSquad;

	for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
		if (Data20->mRecruitID == -1)
			continue;

		if (Data20->field_4 == INVALID_SPRITE_PTR )
			continue;

		int16 Data4 = Data20->field_3;
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

void cFodder::video_Draw_Unk_2( ) {
	
}

void cFodder::map_ClearSpt() {
	
	for (uint16 cx = 0; cx < 4800; ++cx )
		word_3D5B7[cx] = 0;
}

void cFodder::Briefing_Show( ) {
	const char* Brief = "BRIEFING";

	word_3AC19 = 0x25;

	String_CalculateWidth( 320, byte_4382F, Brief );
	String_Print( byte_4382F, 0x03, word_3B301, 0x4E, Brief );
	
	Briefing_DrawBox( 1, 0x47, 0x13B, 0x6B, 0xF3 );
	Briefing_DrawBox( 0, 0x46, 0x13D, 0x6B, 0xF2 );
	
	word_3AC19 = 0;

	std::stringstream Phase;

	Phase << "PHASE " << tool_StripLeadingZero( tool_NumToString( mMissionPhase + 1 ) );
	Phase << " OF " << tool_StripLeadingZero( tool_NumToString( mMissionPhases ));

	String_CalculateWidth( 320, byte_4388F, Phase.str().c_str() );
	String_Print( byte_4388F, 0, word_3B301, 0x1D, Phase.str().c_str() );
	
	sub_126DD();

	int16 DataC = 0x84;
	const char* Data20 = 0;
	const char** Data28 = mMissionGoals;
	int16* Data24 = mMapGoals;

	for (int16 Data0 = 7 ;Data0>=0; ++Data28, --Data0 ) {
		if (*Data24++) {

			Data20 = *Data28;
			String_CalculateWidth( 0x140, byte_4388F, Data20 );
			String_Print( byte_4388F, 0, word_3B301, DataC - 0x12, Data20 );
			DataC += 0x0C;
		}
	}
}

void cFodder::sub_18908() {
	mImage->clearBuffer();
	mImage->paletteSet( mPalette );

	sub_136D0();
	Sprite_SetDataPtrToBase( off_42918 );
	word_3A01A = 0x2C;

	Briefing_Draw_MissionName( );
	Briefing_Show();
	mImage->paletteFade();

	do {
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
	} while (mImage->paletteFade() == -1);

}

void cFodder::Briefing_Wait() {
	mImage->paletteSet( mPalette, 0, true );

	Sprite_SetDataPtrToBase( off_42918 );

	word_3A01A = 0x2C;
	Briefing_Draw_MissionName( );
	Briefing_Show( );
	Briefing_Draw_With( );

	g_Window.RenderAt( mImage, cPosition() );
	g_Window.FrameEnd();

	do {
		eventProcess();
		Mouse_Inputs_Get();

		if (word_3A9B2 == -1) {
			word_3B4F5 = -1;
			break;
		}

	} while (!word_39F02);

	word_39F02 = 0;

	mImage->paletteFadeOut();
	do {
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
	} while (mImage->paletteFade() == -1);

	while (!mouse_Button_Status) {
		mouse_GetData();
	}

	mouse_Setup();
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

	String_CalculateWidth( 0x140, byte_4388F, With.str().c_str() );
	String_Print( byte_4388F, 0, word_3B301, 0x64, With.str().c_str() );
	With.str("");

	if (!word_390CE) {
		With << "THIS IS YOUR LAST CHANCE";
	}
	else {
		With << word_390CE;
		if (word_390CE == 1)
			With << " RECRUIT REMAINING";
		else
			With << " RECRUITS REMAINING";
	}

	String_CalculateWidth( 0x140, byte_4388F, With.str().c_str() );
	String_Print(  byte_4388F, 0, word_3B301, 0xA8, With.str().c_str() );
	
	String_CalculateWidth( 0x140, byte_4382F, "GO FOR IT" );
	String_Print(  byte_4382F, 3, word_3B301, 0xB8, "GO FOR IT" );
}

void cFodder::Briefing_DrawBox( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, uint8 pData10 ) {
	pData0 += 0x10;
	pData4 += 0x10;

	int16 bx = pData0;
	int16 cx = pData4;

	int16 dx = bx + pData8;

	// Top and Bottom
	Brief_Draw_Horizontal_Line( pData0, pData0 + pData8, pData4, pData10 );

	Brief_Draw_Horizontal_Line( pData0, pData0 + pData8, pData4 + pDataC, pData10 );

	// Sides of box
	Brief_Draw_Vertical_Line( pData0 * 4, pData4 + pDataC, pData4, pData10 );
	
	Brief_Draw_Vertical_Line( (pData0 + pData8 + 2) * 4, pData4 + pDataC, pData4, pData10 );
}

void cFodder::sub_18BDF( int16 pBx, int16 pCx, uint8 pSi ) {
	uint8* di = mImage->GetSurfaceBuffer();

	di += 352 * pCx;
	di += pBx >> 2;
	pBx &= 3;

	di += pBx;
	*di = pSi;
}

void cFodder::Brief_Draw_Horizontal_Line( int16 pBx, int16 pDx, int16 pCx, uint8 pSi ) {
	
	do {
		sub_18BDF( pBx, pCx, pSi );
		++pBx;
	} while ( pBx <= pDx * 4 );
}

void cFodder::sub_18C45( int32 pPosY,  const sIntroString* pString ) {

	String_CalculateWidth( 320, mFontWidths, pString->mText );
	String_Print(  mFontWidths, 0, word_3B301, pPosY, pString->mText );
}

void cFodder::Brief_Draw_Vertical_Line(  int16 pBx, int16 pDx, int16 pCx, uint8 pSi ) {
	
	do {
		sub_18BDF( pBx, pCx, pSi );
		++pCx;
	} while (pCx <= pDx );
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
	sSprite_0* Data20 = mSprites;

	for (int16 Data1C = 0x2B; Data1C > 0; --Data1C, ++Data20) {
		
		if (Data20->field_0 == -32768)
			continue;

		word_44A20 = Data20->field_18;
		int16 Data4 = Data20->field_18;

		switch (Data4) {
		case 0:
			Sprite_Handle_Player( Data20 );
			break;

		case 2:
			sub_19658( Data20 );
			break;

		case 3:
			sub_199B2( Data20 );
			break;

		case 4:	// nullsub_3
			break;

		case 5:
			Sprite_Handle_Enemy( Data20 );
			break;

		case 6:
			Sprite_Handle_Bullet( Data20 );
			break;
			
		case 12:
			sub_1A8A5( Data20 );
			break;

		case 13:
			sub_1ABD3( Data20 );
			break;

		case 14:
			Sprite_Handle_Tree( Data20 );
			break;

		case 15:
			Sprite_Handle_BuildingRoof( Data20 );
			break;

		case 20:
			Sprite_Handle_BuildingDoor( Data20 );
			break;

		case 21:
			Sprite_Handle_Player_Rank( Data20 );
			break;

		case 22:
			sub_1AF78( Data20 );
			break;

		case 23:
			sub_1B07C( Data20 );
			break;

		case 27:
			Sprite_Handle_Floating_Dead_Soldier( Data20 );
			break;

		case 28:
			Sprite_Handle_Text_Complete( Data20 );
			break;

		case 29:
			Sprite_Handle_Text_Mission( Data20 );
			break;

		case 30:
			Sprite_Handle_Text_Phase( Data20 );
			break;

		case 34:
			Sprite_Handle_Text_GameOver( Data20 );
			break;

		case 35:
			sub_1B8A9( Data20 );
			break;

		case 37:
			Sprite_Handle_GrenadeBox( Data20 );
			break;

		case 39:
			sub_1BB11( Data20 );
			break;

		case 47:
			sub_1C1C0( Data20 );
			break;

		case 48:
			sub_1C268( Data20 );
			break;

		case 58:
			sub_1C4E7( Data20 );
			break;

		case 59:
			sub_1C52D( Data20 );
			break;

		case 66:
			Sprite_Handle_Bird_Left( Data20 );
			break;

		case 67:
			Sprite_Handle_Bird_Right( Data20 );
			break;

		default:
			std::cout << "Sprite Function not implemented: " << Data4 << "\n";
			break;
		}
		
	}
}

void cFodder::Sprite_Handle_Player( sSprite_0 *pData20 ) {
	sSprite_0* Sprite = (sSprite_0*)pData20;
	sSprite_0* Data30 = 0;

	if (Sprite->field_6E) {
		sub_22AA9( Sprite );
		return;
	}

	if (Sprite->field_75 & 2) {

		if (Sprite->field_38 <= 0x32)
			Sprite->field_38 = 0;

		Sprite->field_5B = 0;
		Sprite->field_64 = 0;
	}

	if (sub_1E05A( Sprite ))
		return;

	if (Sprite->field_38)
		return;

	if (sub_1F21E( Sprite ))
		return;

	//loc_18E8D
	sub_22B71( Sprite );
	word_3AA41 = 0;

	word_3AA1D = word_3BED5[Sprite->field_32];
	//seg004:0183

	if (byte_3BF1B[Sprite->field_32] < 0 && Sprite->field_32 != mSquad_Selected ) {
		//loc_18F12
		//seg004:01C2
		word_3AA41 = -1;
		//Data0 <<= 1;   UNUSED?

		sSprite_0* Data28 = &mSprites[Sprite->field_5E];
		
		if( Data28->field_0 != -32768 ) {
			
			if( Data28->field_18 == 0x05 ) {
				if( Data28->field_38 == 0x00 ) {
				
					//seg004:021A 
					if( Sprite->field_4F == 0 ) {
						//seg004:0228
						int16 Data0 = Sprite->field_0;
						int16 Data4 = Sprite->field_4;
						
						int16 Data8 = Data28->field_0;
						int16 DataC = Data28->field_2;
						
						sub_29E30( Data0, Data4, Data8, DataC );
						if (Data0 < 0xD2) {

							if (Data0 <= 0x28)
								goto loc_1904A;

							//seg004:0270
							word_3AA4B = Data0;
							Data0 = Sprite->field_0;
							Data4 = Sprite->field_4;
							Data8 = Data28->field_0;
							DataC = Data28->field_2;

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
		Sprite->field_4A = 0;
		Sprite->field_5E++;
		if (Sprite->field_5E < 43)
			goto loc_191C3;

		Sprite->field_5E = 0;
		goto loc_191C3;

	loc_1904A:;
		int16 Data0 = tool_RandomGet() & 0x1F;

		if (Data0 == 5)
			goto loc_1901C;

		Sprite->field_5A = -1;
		word_3A00C = 0;
		word_3A00E = 0;
		word_3A010 = 0;

		Sprite->field_2E = Data28->field_0;

		// WHAT THE?
		//seg004:0337
		Sprite->field_30 += 0x07;

		Sprite->field_30 = Data28->field_4;
		Sprite->field_30 -= 0x0E;

		if (Sprite->field_4A <= 0) {
			//loc_190B9
			Data0 = tool_RandomGet();
			Data0 &= 0x0F;
			++Data0;
			Sprite->field_4A = Data0;
			Data0 = tool_RandomGet();
			Data0 &= 0x3F;
			if (Data0 == 0x2A)
				goto loc_1918C;
		}
		
	loc_190E8:;
		word_3A00C = 0;
		word_3A00E = 0;
		word_3A010 = 0;

		if (Sprite->field_4F) {
			word_3A010 = -1;
			word_3A00E = -1;
			word_3A00C = -1;
		}
		else {
			//loc_19118
			if (Sprite == word_39FCE && word_39EFC ) {

				Data0 = word_3AC3F[ Sprite->field_32 ];
				if (Data0 == 3) {
					word_3A00C = -1;
					word_3A00E = -1;
				} else {
					if (Data0 == 1) {
						word_3A010 = -1;
						word_3A00E = -1;
					}
					else
						goto loc_19198;
				}
			loc_1918C:;
				Sprite->field_4A = 1;
				goto loc_191BF;
			}

		loc_19198:;
			word_3A00C = -1;
			word_3A010 = -1;

			sSquad_Member* eax = (sSquad_Member*) Sprite->field_46;
			Data0 = eax->field_6;

		loc_191BF:;
			sub_1F429( Sprite );
		}
	}
	else {

		if (word_3A9CE) {
			goto loc_190E8;
			
		}
	}

loc_191C3:;
	if (Sprite->field_22)
		goto loc_1921E;

	//TODO: Check this
	// seg004:047F
	struct_8* Dataa30 = off_3BEF3[Sprite->field_32];
	int32  eax = Dataa30[Sprite->field_40].field_2 | Dataa30[Sprite->field_40 + 1].field_0 << 16;

	//seg004:04CC
	if (eax < 0) {
	loc_1921E:;
		if (word_3AA41) {

			if (Sprite->field_0 == Sprite->field_26 &&
				 Sprite->field_4 == Sprite->field_28)
				 goto loc_1931E;
		}
	}
	//loc_1925C
	if (Sprite->field_54 != 2 && Sprite->field_54 != 1)
		goto loc_19338;

	//loc_19274
	if (word_3AA1D != 2)
		goto loc_1931E;

	word_3A399 = Sprite->field_A;
	Sprite->field_5A = 0;

	if (word_3AA41)
		sub_1F5A0( Sprite );
	else
		sub_1F5CA( Sprite );

	int16 Data0 = Sprite->field_26;
	if( Data0 < 0 )
		goto loc_1946D;
	
	int16 Data4 = Sprite->field_28;
	if( Data4 < 0 )
		goto loc_1946D;
	
	sub_1F623( Sprite );
	
	if( sub_2A1F0( Sprite, Data0, Data4 ) >= 0 ) {
		sub_1F66F( Sprite );
		sub_1F6F4( Sprite );
	}
		
	Data0 = Sprite->field_0;
	if( Data0 == Sprite->field_26 ) {
		Data0 = Sprite->field_4;
		
		if( Data0 == Sprite->field_28 )
			sub_2D91E( Sprite );
	}
	//loc_19314
	if( word_3ABAD == 0 )
		return;
	
	loc_1931E:;
	
	if( Sprite->field_5A )
		goto loc_19403;
	
	sub_1F5CA( Sprite );
	sub_1F762( Sprite );
	return;
	
	loc_19338:;
	Sprite->field_43 = 0;
	word_3A399 = Sprite->field_A;
	word_3A9C6 = 0;
	
	sub_1FCF2( Sprite );
	Data0 = Sprite->field_26;
	if( Data0 < 0 )
		goto loc_1946D;
	Data4 = Sprite->field_28;
	if( Data4 < 0 )
		goto loc_1946D;
	
	if( Sprite->field_44 ) {
		Sprite->field_44 -= 1;
		goto loc_193D6;
		
	} 
	
	//loc_19392
	if( sub_2A1F0( Sprite, Data0, Data4 ) >= 0 )
		goto loc_193D6;
	

	if( Sprite->field_0 == Sprite->field_26 ) {
		if( Sprite->field_4 == Sprite->field_28 )
			sub_2D91E( Sprite );
	}
	//loc_193D3
	goto loc_1946D;
	
loc_193D6:;
	sub_1F623( Sprite );
	sub_2A3D4( Sprite );
	
	if( !Sprite->field_5A ) {
		if( !Sprite->field_45 )
			goto loc_19424;
		
		Sprite->field_45 -= 1;
		return;
	}
	
loc_19403:;
	Sprite->field_5A = 0;
	sub_1FDE7( Sprite );
	return;
	
loc_19414:;
	Sprite->field_5A = 0;
	sub_1F5CA( Sprite );
	return;
	
loc_19424:;
	sub_1F6F4( Sprite );
	
	if( Sprite->field_0 == Sprite->field_26 ) {
		
		if( Sprite->field_4 == Sprite->field_28 )
			sub_2D91E( Sprite );
		
	}
	//loc_19463
	if( word_3ABAD )
		goto loc_19414;
	
	goto loc_1957A;
	
loc_1946D:;
	if( Sprite->field_5A )
		goto loc_19403;
	
	if( !Sprite->field_45 )
		goto loc_194A0;
	
	Sprite->field_45 -= 1;
	goto loc_19414;

loc_19490:;
	Sprite->field_5A = 0;
	sub_1F5CA( Sprite );
	return;

loc_194A0:;
	Sprite->field_43 = -1;
	
	if( !word_3AA1D ) {
		word_3A8CF = 0;
		sub_1F762( Sprite );
		Sprite->field_A = 0;
		return;
		
	}
	//loc_194CB
	
	if( mSquad_Selected == Sprite->field_32 )
		goto loc_19490;
	
	sub_1F5CA( Sprite );
	return;

loc_1957A:;
	Sprite->field_3A = 0;
	sub_1F762( Sprite );

}

void cFodder::sub_19658( sSprite_0* pSprite ) {

	if (pSprite->field_38)
		goto loc_1992D;

	if (!pSprite->field_12)
		goto loc_1992D;

	if (!pSprite->field_56)
		goto loc_19701;

	--pSprite->field_56;
	if (!pSprite->field_56)
		Sound_Voc_Play( pSprite, 0x0F, 0x0F );
	
	sSprite_0* Data24 = (sSprite_0*) pSprite->field_46;
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
	int16 Data0 = pSprite->field_26;
	int16 Data4 = pSprite->field_28;
	int16 Data8 = pSprite->field_0;
	int16 DataC = pSprite->field_4;
	
	sub_29E30( Data0, Data4, Data8, DataC );
	if (Data0 <= 1)
		pSprite->field_36 = 0;

	if (Data0 <= 4)
		pSprite->field_36 >>= 1;

	if (pSprite->field_36) {
		pSprite->field_36 -= 1;
		sub_1F623( pSprite );

		word_3A399 = pSprite->field_A;
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
	int32 a1 = (int32) pSprite->field_1A;

	if ((a1 >> 16) < 0)
		if ((a1 >> 16) < -8)
			pSprite->field_1A = (int32*) ((a1 & 0xFFFF) | -8);

	pSprite->field_1A -= 0x18000;
	Data4 = (int32) pSprite->field_1A;

	if (!pSprite->field_50)
		goto loc_19877;

	if (Data4 >= 0)
		goto loc_19877;

	if (pSprite->field_20 >= 2)
		goto loc_19877;
	
	pSprite->field_1E = 0;

loc_19855:;

	Data4 = (Data4 >> 16) | ((Data4 & 0xFFFF) << 16);
	if (pSprite->field_52 >= 0x0A)
		goto loc_198D3;

	pSprite->field_52 -= Data4;
	goto loc_198D3;

loc_19877:;
	int32 tmp = pSprite->field_1E | pSprite->field_20 << 16;
	tmp += Data4;

	pSprite->field_1E = tmp;
	pSprite->field_20 = tmp >> 16;

	if (pSprite->field_20 < 0) {
		pSprite->field_1E = 0;
		pSprite->field_20 = 0;

		if (pSprite->field_50)
			goto loc_19855;

		Data4 = (int32) pSprite->field_1A;
		Data4 = -Data4;
		Data4 >>= 1;
		pSprite->field_1A = (int32*) Data4;
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
		pSprite->field_18 = 0x0C;
		sub_1998C( pSprite );
		sub_2061C( pSprite + 1 );
		return;
	}
	//loc_19957
	sub_1998C( pSprite );
	pSprite->field_0 = 0;
	pSprite->field_4 = 0;

	sub_2061C( pSprite );
	sub_2061C( pSprite + 1 );
}

void cFodder::sub_1998C( sSprite_0* pSprite ) {
	int16 Data0 = pSprite->field_22;

	int8* Data24 = byte_3A9D2;
	--Data24[Data0];
}

void cFodder::sub_199B2( sSprite_0* pSprite ) {
	sSprite_0* Data24 = pSprite - 1;

	pSprite->field_0 = Data24->field_0;
	pSprite->field_4 = Data24->field_4;
	pSprite->field_52 = Data24->field_52;

}

void cFodder::Sprite_Handle_Enemy( sSprite_0* pSprite ) {

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
		sub_1F429( pSprite );

		word_3A399 = pSprite->field_A;
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
	sub_1F623( pSprite );
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

void cFodder::Sprite_Handle_Bullet( sSprite_0* pSprite ) {

	pSprite->field_64 += 1;
	
	if(!pSprite->field_2A)
		goto loc_19BA8;
	
	sSprite_0* Data28 = (sSprite_0*) pSprite->field_1A;
	
	int16 Data0 = Data28->field_0;
	int16 Data4 = Data28->field_4;
	
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
	
	sub_1F623( pSprite );
	
	int16 Data1C = pSprite->field_10;
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
	int16 Data8 = pSprite->field_2E;
	int16 DataC = pSprite->field_30;
	
	sub_29E30( Data0, Data4, Data8, DataC );
	
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
		if(Map_Sprite_Check_Position(pSprite, Data0, Data4))
			goto loc_19DCF;
	}

	if( sub_21618( pSprite ) )
		goto loc_19D68;
	
	return;
	
loc_19D68:;
	
	if(word_3AA03 == 1 )
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
	int8 al = pSprite->field_43;
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
	sub_2061C( pSprite );
	return;
	
loc_19E50:;
	pSprite->field_0 = 0;
	pSprite->field_4 = 0x1000;
	goto loc_19E3D;
}

void cFodder::sub_1A8A5( sSprite_0* pSprite ) {
	if (pSprite->field_8 == 0x7C) {
		sub_22A55( pSprite );
		return;
	}

	if (pSprite->field_8 == 0xC0 || pSprite->field_8 == 0x8E) {
		//loc_1A8D1

		int16 Data8 = pSprite->field_0;
		Data8 += 8;

		if (pSprite->field_26 == 0x5F5F)
			Data8 += pSprite->field_28;

		int16 DataC = pSprite->field_0;
		DataC += 0x24;
		int16 Data10 = pSprite->field_4;
		Data10 -= 0x20;
		int16 Data14 = pSprite->field_4;
		Data14 -= 6;

		const int16* Data2C = word_3D5B9;
		int16 Data0 = pSprite->field_A;
		if (pSprite->field_8 == 0xC0)
			Data0 += 2;
		Data0 -= 1;
		if (Data0 >= 0) {

			do {
				Data8 -= *Data2C;
				DataC -= *(Data2C + 1);
				Data10 += *Data2C;

				++Data2C;
				Data14 += *Data2C;
				++Data2C;

			} while (--Data0 >= 0);
		}
		//loc_1A99D
		sSprite_0* Data24 = 0;

		pSprite->field_62 = ~pSprite->field_62;
		if (pSprite->field_62 >= 0)
			SquadMember_Sprite_Find_In_Region( pSprite, Data8, DataC, Data10, Data14 );
		else
			Sprite_Find_In_Region( pSprite, Data24, Data8, DataC, Data10, Data14 );

		pSprite->field_12 -= 1;
		if (pSprite->field_12 >= 0)
			return;

		pSprite->field_12 = 1;

		Data2C = word_3D5B9;
		int16 Data4 = pSprite->field_A;
		if (pSprite->field_8 == 0xC0)
			Data4 += 2;
		
		//seg004:1CC9
		pSprite->field_0 -= Data2C[Data4];
		pSprite->field_4 += Data2C[Data4 + 1];
		pSprite->field_A += 1;
		if (pSprite->field_8 != 0xC0)
			goto loc_1AA63;

		if (pSprite->field_A < 4)
			return;

		sub_2060F( pSprite );
		return;

	loc_1AA63:;
		if (pSprite->field_20 && pSprite->field_A >= 2) {
			pSprite->field_8 = 0xC0;
			pSprite->field_A = 0;
		}
		if (pSprite->field_A >= 7)
			sub_2060F( pSprite );

		return;
	}

	//loc_1AA9F
	if (pSprite->field_5C) {
		sub_2DD50( pSprite );
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

	Sound_Voc_Play( pSprite, Data4, 0x1E );
	pSprite->field_8 = 0x8E;
	pSprite->field_A = 0;
	pSprite->field_12 = 1;
	pSprite->field_52 = 0;
	pSprite->field_22 = -1;
	pSprite->field_32 = -1;
	pSprite->field_2C = -1;
}

void cFodder::sub_1ABD3( sSprite_0* pSprite ) {
	pSprite->field_8 = 0x8F;
	sub_212F9( pSprite );
}

void cFodder::Sprite_Handle_Tree( sSprite_0* pSprite ) {
	pSprite->field_8 = 0x90;
	sub_212F9( pSprite );
}

void cFodder::Sprite_Handle_BuildingRoof( sSprite_0* pSprite ) {
	pSprite->field_8 = 0x91;
	sub_212F9( pSprite );
}

void cFodder::Sprite_Handle_BuildingDoor( sSprite_0* pSprite ) {

	if (sub_221A6( pSprite ))
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
		Troop_Deploy(pSprite);
		return;
	}

	if (pSprite->field_43 >= 0) {
		pSprite->field_8 = 0x7C;
		return;
	}

	pSprite->field_8 = 0x99;
	Sound_Voc_Play( pSprite, 0x10, 0x01 );
	int16 Data0 = tool_RandomGet() & 0x0F;

	int16 Data4 = 0x14 - word_390C2;
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

	if (!Troop_Deploy(pSprite))
		return;

	pSprite->field_12 = -1;
	pSprite->field_43 = -1;
}

void cFodder::Sprite_Handle_Player_Rank( sSprite_0* pSprite ) {
	
	if (mSquad_Selected < 0)
		goto loc_1AF63;

	sSprite_0* Data24 = word_39FCE;
	int16 Data4;

	if (Data24 == INVALID_SPRITE_PTR)
		goto loc_1AF63;

	if (!word_3A9AE) {
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

	int16 Data0 = Data24->field_0;
	if (Data0 < 0)
		goto loc_1AF63;


	Data0 += 3;
	Data4 = Data24->field_4;

	pSprite->field_0 = Data0;
	pSprite->field_4 = Data4;

	if (Data24->field_6E)
		goto loc_1AF63;

	pSprite->field_8 = 0x95;

	sSquad_Member* Data28 = (sSquad_Member*) Data24->field_46;
	Data0 = Data28->mRank;

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

void cFodder::sub_1AF78( sSprite_0* pSprite ) {
	sSprite_0* Data28 = (sSprite_0*) pSprite->field_1A;

	if (Data28 != pSprite->field_6A)
		goto loc_1B06D;

	int16 Data0 = pSprite->field_62;

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
	sub_2060F( pSprite );
}

void cFodder::sub_1B07C( sSprite_0* pSprite ) {
	
	if (pSprite->field_20)
		--pSprite->field_20;

	--pSprite->field_12;
	if (pSprite->field_12 >= 0)
		return;

	pSprite->field_12 = 1;
	pSprite->field_A += 1;
	if (pSprite->field_A < 4)
		return;

	sub_2060F( pSprite );
}

void cFodder::Sprite_Handle_Floating_Dead_Soldier( sSprite_0* pSprite ) {

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

	sub_1F623( pSprite );
	Sprite_Movement_Calculate( pSprite );
	int16 Data0 = -3;
	int16 Data4 = 8;
	Map_Sprite_Check_Position( pSprite, Data0, Data4 );
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

void cFodder::Sprite_Handle_Text_Complete( sSprite_0* pSprite ) {
	pSprite->field_0 = dword_39F84 >> 16;
	pSprite->field_0 += 0x47;
	pSprite->field_4 -= 0x20;

	int16 Data0 = dword_39F88 >> 16;
	Data0 += 0x77;

	if (Data0 < pSprite->field_4)
		return;

	pSprite->field_4 = Data0;
}
	
void cFodder::Sprite_Handle_Text_Mission( sSprite_0* pSprite ) {
	pSprite->field_0 = dword_39F84 >> 16;
	pSprite->field_0 += 0x56;
	pSprite->field_4 -= 0x20;

	int16 Data0 = dword_39F88 >> 16;
	Data0 += 0x63;

	if (Data0 < pSprite->field_4)
		return;

	pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_Phase( sSprite_0* pSprite ) {
	pSprite->field_0 = dword_39F84 >> 16;
	pSprite->field_0 += 0x5F;
	pSprite->field_4 -= 0x20;

	int16 Data0 = dword_39F88 >> 16;
	Data0 += 0x63;

	if (Data0 < pSprite->field_4)
		return;

	pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Text_GameOver( sSprite_0* pSprite ) {

	pSprite->field_0 = dword_39F84 >> 16;
	pSprite->field_0 += 0x5B;
	pSprite->field_4 -= 0x20;

	int16 Data0 = dword_39F88 >> 16;
	Data0 += 0x77;
	
	if (Data0 < pSprite->field_4)
		return;

	pSprite->field_4 = Data0;
}

void cFodder::sub_1B8A9( sSprite_0* pSprite ) {
	sSprite_0* Data28 = (sSprite_0*) pSprite->field_1A;

	if (Data28->field_18 == 0x0C) {
		sub_2060F( pSprite );
		return;
	}

	if (Data28->field_52) {
		sub_2060F( pSprite );
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

void cFodder::Sprite_Handle_GrenadeBox( sSprite_0* pSprite ) {
	
	if (pSprite->field_38) {
		pSprite->field_18 = 0x0C;
		return;
	}

	pSprite->field_8 = 0xC2;
	if (sub_2244B( pSprite ))
		return;

	byte_3AC33[mSquad_Selected] = -1;
	byte_3AC39[mSquad_Selected] = -1;
	mSquad_Grenades[mSquad_Selected] += 4;
	
	sub_2060F( pSprite );
}

void cFodder::sub_1BB11( sSprite_0* pSprite ) {

	if (pSprite->field_8 == 0x7C) {
		sub_22A55( pSprite );
		return;
	}

	pSprite->field_62 = ~pSprite->field_62;
	if (pSprite->field_62 < 0)
		goto loc_1BC07;


	int16 Data8 = pSprite->field_0;
	Data8 += 8;
	if (pSprite->field_26 == 0x5F5F)
		Data8 += pSprite->field_28;
	
	int16 DataC = pSprite->field_0;
	DataC += 0x24;

	int16 Data10 = pSprite->field_4;
	Data10 -= 0x20;
	int16 Data14 = pSprite->field_4;
	Data14 -= 6;

	const int16* Data2C = word_3D5B9;
	int16 Data0 = pSprite->field_A;

	if (pSprite->field_8 == 0xC0)
		Data0 += 2;
	Data0 -= 1;

	for (; Data0 >= 0; --Data0) {
		Data8 -= *Data2C;
		DataC -= *Data2C++;
		Data10 += *Data2C;
		Data14 += *Data2C++;
	}

	SquadMember_Sprite_Find_In_Region( pSprite, Data8, DataC, Data10, Data14 );
	goto loc_1BC48;

loc_1BC07:;
	Data8 = pSprite->field_0;
	DataC = pSprite->field_0;
	DataC += 0x1E;
	Data10 = pSprite->field_4;
	Data10 += -30;
	Data14 = pSprite->field_4;

	sSprite_0* Data24 = 0;
	Sprite_Find_In_Region( pSprite, Data24, Data8, DataC, Data10, Data14 );
loc_1BC48:;

	Data2C = word_3D5B9;
	int16 Data4 = pSprite->field_A;
	if (pSprite->field_8 == 0xC0)
		Data4 += 2;

	Data4 <<= 1;
	Data2C += Data4;
	
	Data4 = *(Data2C + 1);
	int16 Data6 = *Data2C;

	pSprite->field_0 -= Data4;
	pSprite->field_4 += Data6;

	pSprite->field_A += 1;
	if (pSprite->field_8 == 0xC0) {
		if (pSprite->field_A < 4)
			return;

		sub_2060F( pSprite );
		return;
	}

	if (!pSprite->field_20 || pSprite->field_A < 2)
		goto loc_1BD06;

	pSprite->field_8 = 0xC0;
	pSprite->field_A = 0;
loc_1BD06:;

	if (pSprite->field_A < 3)
		return;

	sub_2060F( pSprite );
}

void cFodder::sub_1C1C0( sSprite_0* pSprite ) {
	Sprite_Movement_Calculate( pSprite );
	int32 Data0 = (int32) pSprite->field_1A;

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
		sub_2060F( pSprite );
		return;
	}

loc_1C262:;
	sub_2183B( pSprite );
}

void cFodder::sub_1C268( sSprite_0* pSprite ) {
	pSprite->field_2A += 1;
	pSprite->field_2A &= 1;
	if (pSprite->field_2A)
		return;

	pSprite->field_A += 1;
	if (pSprite->field_A != 4)
		return;

	sub_2060F( pSprite );
}

void cFodder::sub_1C4E7( sSprite_0* pSprite ) {
	pSprite->field_0 = dword_39F84 >> 16;
	pSprite->field_0 += 0x71;
	pSprite->field_4 -= 0x20;

	int16 Data0 = dword_39F88 >> 16;
	Data0 += 0x63;

	if (Data0 < pSprite->field_4)
		return;

	pSprite->field_4 = Data0;
}

void cFodder::sub_1C52D( sSprite_0* pSprite ) {
	pSprite->field_0 = dword_39F84 >> 16;
	pSprite->field_0 += 0x57;
	pSprite->field_4 -= 0x20;

	int16 Data0 = dword_39F88 >> 16;
	Data0 += 0x75;

	if (Data0 < pSprite->field_4)
		return;

	pSprite->field_4 = Data0;
}

void cFodder::Sprite_Handle_Bird_Left( sSprite_0* pSprite ) {
	
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
	
	if (mMap_TileSet == 0)
		sub_14D6D(pSprite, 0x1A);

	if (mMap_TileSet == 2)
		sub_14D6D(pSprite, 0x1F);

}

void cFodder::Sprite_Handle_Bird_Right( sSprite_0* pSprite ) {
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
	if (mMap_TileSet == 0)
		sub_14D6D(pSprite, 0x1A);

	if (mMap_TileSet == 2)
		sub_14D6D(pSprite, 0x1F);
}

void cFodder::sub_14D6D( sSprite_0* pSprite, int16 pData4 ) {
	word_3B4DD = pData4;

	if (word_3A9AE)
		return;

	int16 Data0 = tool_RandomGet() & 0x7F;
	if (Data0 != 0x0E)
		return;

	pData4 = word_3B4DD;
	int16 Data8 = 0;

	if (mMap_TileSet == 4 || mMap_TileSet == 3)
		Data8 = 0x7F;

	Sound_Voc_Play( pSprite, pData4, Data8 );
}

int16 cFodder::sub_1E05A( sSprite_0* pSprite ) {

	if (pSprite->field_38 == 3)
		goto loc_1E831;

	if (pSprite->field_38 == 6)
		goto loc_1EA48;

	if (pSprite->field_38 == 2)
		goto loc_1EB87;

	int16 Data0 = pSprite->field_4;
	if (Data0 >= word_3AA4F)
		pSprite->field_4 = word_3AA4F;

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
	if (word_3A9AA)
		pSprite->field_40 = -26506;

	if (!pSprite->field_58) {
		sub_1FE35( pSprite );
		pSprite->field_58 = -1;
	}

	//loc_1E15D
	int16 Data8 = pSprite->field_10;
	Data8 >>= 5;
	Data8 -= 1;
	Data8 ^= 0x0F;
	Data8 &= 0x0E;
	Data0 = pSprite->field_22;

	//Data0 <<= 2;
	int16* Data28 = off_32AE4[Data0];
	pSprite->field_8 = Data28[(Data8 + 0x20) / 2];

	if (pSprite->field_59)
		pSprite->field_8 = Data28[(Data8 + 0x10) / 2];

	//loc_1E1E9
	pSprite->field_A = 0;
	pSprite->field_2A = 0;

	// TODO: is this even close to correct?
	if (pSprite->field_1A < (int32*) &mSprites[0]) {
		int32 Dataa0 = tool_RandomGet() & 0x07;
		Dataa0 += 2;
		Dataa0 = (Data0 << 16);

		// lol?
		pSprite->field_1A = (int32*) Dataa0;
	}
	//loc_1E232
	pSprite->field_12 = 2;
	Data0 = tool_RandomGet();
	int16 Data4 = 0;
	Data0 &= 7;
	Data4 = byte_3D477[Data0];
	//seg004:5508
	Data8 = 0x14;
	Sound_Voc_Play( pSprite, Data4, Data8 );

	if (pSprite->field_36 < 0x1E)
		pSprite->field_36 += 0x0F;

	pSprite->field_3E = pSprite->field_10;
	pSprite->field_3A = -1;
	Data0 = tool_RandomGet() & 0x01;
	if (!Data0)
		pSprite->field_3A = pSprite->field_10;

	sub_1FF1A(pSprite);
	pSprite->field_26 = -1;
	if (pSprite->field_38 != 9)
		goto loc_1E3D2;

	pSprite->field_36 = 0;
	pSprite->field_38 = 0x0A;
loc_1E2F4:;
	pSprite->field_3A -= 1;
	if (pSprite->field_3A < 0) {
		pSprite->field_38 = 1;
		goto loc_1E737;
	}
	//loc_1E30C
	Data0 = 0x21;
	if (pSprite->field_22)
		Data0 = 0x63;

	pSprite->field_8 = Data0;
	Data0 = pSprite->field_2A & 7;
	//seg004:55F3 

	if (!Data0) {
		sub_1FF1A(pSprite);

		if (pSprite->field_A)
			pSprite->field_A = 0;
		else
			pSprite->field_A = 1;
	}
	//loc_1E36C

	pSprite->field_2A -= 1;
	if (pSprite->field_2A >= 0)
		goto loc_1E737;

	Data0 = tool_RandomGet() & 7;
	Data0 += 8;
	pSprite->field_2A = Data0;
	Data0 = tool_RandomGet() & 7;

	Data4 = byte_3D477[Data0];
	Data8 = 0;
	Sound_Voc_Play( pSprite, Data4, Data8 );
	goto loc_1E737;

loc_1E3D2:;

	if (pSprite->field_40 == -26506) {
		pSprite->field_28 += 1;
		sub_1FF1A(pSprite);

		if (!(pSprite->field_28 & 7)) {
			Data0 = tool_RandomGet() & 7;
			Data4 = byte_3D477[Data0];
			Data8 = 0x14;
			Sound_Voc_Play( pSprite, Data4, Data8 );
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
	sub_1F623( pSprite );
	word_3A399 = pSprite->field_A;
	Sprite_Movement_Calculate( pSprite );

	if (pSprite->field_20 < 0x0C) {

		int16 Field10 = pSprite->field_10;
		sub_1FFC6( pSprite );
		pSprite->field_10 = Field10;
	}
	else {
		//loc_1E542
		int16 Field0 = pSprite->field_0;
		int16 Field4 = pSprite->field_4;
		int16 Field10 = pSprite->field_10;

		sub_1FFC6( pSprite );

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
	int32 Dataa0 = (int32) pSprite->field_1A;
	int32 Dataa4 = Dataa0;
	Dataa0 += (pSprite->field_1E | (pSprite->field_20 << 16));

	// Probably going to be issues here
	pSprite->field_1E = Dataa4;
	pSprite->field_20 = Dataa4 >> 16;
	if (pSprite->field_1E < 0) {
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
	pSprite->field_1A = (int32*) Dataa4;
	if (pSprite->field_36) {
		pSprite->field_36 -= 2;

		if (pSprite->field_36 < 0)
			pSprite->field_36 = 0;
	}
	//loc_1E653
	if (pSprite->field_26) {

		if (!pSprite->field_20) {
			sub_1FF1A(pSprite);
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
				word_3AC2B = 0;
				goto loc_1E737;
			}
		}
	}
	//loc_1E708
	pSprite->field_38 = 6;
	pSprite->field_10 = pSprite->field_3E;
	pSprite->field_12 = 0;
	pSprite->field_45 = 1;

loc_1E737:;
	Data0 = -1;
	return -1;

loc_1E743:;
	Data0 = 0;
	return 0;

loc_1E74C:;
	if (pSprite->field_52 < 5) {
		pSprite->field_52 = 5;
		Data4 = 0x0E;
		Data8 = 0x0A;

		Sound_Voc_Play( pSprite, Data4, Data8 );
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
			goto loc_1E737;

		pSprite->field_52 = 0x0E;
		pSprite->field_12 = 0x14;
		Data0 = tool_RandomGet() & 0x07;
		Data4 = byte_3D477[Data0];
		Data8 = 0x0A;
		Sound_Voc_Play( pSprite, Data4, Data8 );
		goto loc_1E9EC;
	}
	//loc_1E8D6
	pSprite->field_4 += Data0;
	Data0 = pSprite->field_4;
	if (Data0 >= word_3AA4F)
		pSprite->field_38 = 2;

	dword_3A395 = pSprite->field_4;
	Data0 = 8;
	Data0 += pSprite->field_26;
	Data4 = -3;
	Data4 += pSprite->field_28;

	sub_2A7E2( Data0, Data4 );

	if (Data4 == 9 || Data4 == 0x0A) {
		Data0 = pSprite->field_12;
		pSprite->field_28 += Data0;
		Data8 = pSprite->field_28;

		struct_8* Data30 = off_3BEF3[pSprite->field_32];
		//seg004:5C49
		Data0 = pSprite->field_40;
		if (Data30[Data0].field_0 >= 0)
			Data30[Data0].field_2 = Data8;
	}
	//loc_1E9CD;
	sub_1FFC6( pSprite );
	if (pSprite->field_12 > 5)
		sub_223B2( pSprite );

	if (Data4 == 9)
		goto loc_1E737;

loc_1E9EC:;	// Troop Falling?
	if (pSprite->field_12 < 0x0C) {
		pSprite->field_3E = 0;
		pSprite->field_56 = 0;

		goto loc_1E743;
	}
	
	pSprite->field_38 = 6;
	pSprite->field_10 = pSprite->field_3E;
	pSprite->field_12 = 0;
	pSprite->field_45 = 1;
	goto loc_1E737;

loc_1EA3F:;
	Data0 = 0;
	return 0;

loc_1EA48:;
	pSprite->field_12 += 1;
	if (pSprite->field_12 >= 0x0F)
		return sub_1EF47(pSprite);

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
	int16 Field_52 = pSprite->field_52;
	int16 Field_0 = pSprite->field_0;
	int16 Field_4 = pSprite->field_4;
	sub_1FFC6( pSprite );
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
		return sub_1EF47( pSprite );

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
	sub_1FFC6( pSprite );
	pSprite->field_52 = Field_52;

	if (pSprite->field_4F || pSprite->field_50) {
		if (pSprite->field_52 >= 0x0D)
			pSprite->field_52 = 0x0B;
		pSprite->field_52 += 2;
	}
	//loc_1EC9A
	Data0 = -1;
	return -1;

loc_1ECA6:;
	if (pSprite->field_60 <= 0x06 && pSprite->field_60 >= 0x04) {
		pSprite->field_38 = 0;
		goto loc_1E743;
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
	goto loc_1E737;

loc_1ED5B:;
	sub_1F623( pSprite );
	word_3A399 = pSprite->field_A;

	Sprite_Movement_Calculate( pSprite );
	sub_20478( pSprite );
	sub_1FFC6( pSprite );

	if (pSprite->field_38 != 0x33)
		goto loc_1EE3E;

	if (pSprite->field_60 > 6 || pSprite->field_60 < 4) {
		pSprite->field_36 -= 5;
		if (pSprite->field_36) {
			if (pSprite->field_36 >= 0)
				goto loc_1EE31;

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

	loc_1EE31:;
		goto loc_1E737;
	}
	//loc_1EE34
	pSprite->field_38 = 0;

loc_1EE3E:;
	pSprite->field_43 = 0;
	sSprite_0* Data20 = 0;
	sub_305D5( Data20 );
	goto loc_1E737;

loc_1EE59:;
	if (pSprite->field_60 <= 6 && pSprite->field_60 >= 4 ) {
		pSprite->field_38 = 0;
		goto loc_1E743;
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
	goto loc_1E737;

loc_1EEEC:;

	if (!pSprite->field_66) {
		pSprite->field_38 = 0;
		goto loc_1E743;
	}

	pSprite->field_38 = 0x5B;
	pSprite->field_8 = 0xCC;
	pSprite->field_A = 4;
	goto loc_1E737;

loc_1EF28:;
	pSprite->field_A -= 1;
	if (pSprite->field_A < 0)
		return loc_1F043(pSprite);

	pSprite->field_36 = 0x30;
	sub_1F6F4( pSprite );
	goto loc_1E737;

}

int16 cFodder::sub_1EF47( sSprite_0* pSprite ) {
	
	if (pSprite->field_22)
		goto loc_1F0EA;

	++word_397AC;
	//TODO: Fix this messy function
	sSquad_Member* Data24 = (sSquad_Member*) pSprite->field_46;

	Hero_Add( Data24 );

	int8* Data28 = (int8*) dword_394A4;

	Data28[0] = Data24->mRank;
	++Data28;
	Data28[0] = 0;
	++Data28;
	Data28[0] = -1; 
	Data28[1] = -1;
	dword_394A4 = (int16*) Data28;

	Data28 = (int8*) dword_3977E;
	writeLEWord( Data28, Data24->mRecruitID );

	Data28 += 2;
	//seg004:627D
	Data28[0] = -1;
	Data28[1] = -1;
	dword_3977E = (int16*) Data28;

	Data24->field_4 = INVALID_SPRITE_PTR;
	Data24->mRecruitID = -1;
	Data24->mRank = 0;
	
	if (mSquad_Selected < 0) {
		word_3AC2B = 0;
		word_3AA47 = 0;
	}
	else {
		word_3AC2B = 0;
	}

	//loc_1F03D
	return sub_2060F( pSprite );

loc_1F043:;
	return loc_1F043( pSprite );

loc_1F0EA:;

	if (pSprite->field_18 != 0x24)
		if (pSprite->field_18 != 5)
			goto loc_1F218;

	uint8* DataFinal = 0;
	int16 Data0;

	++word_397AE;
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
	sSprite_0** Dataa24 = off_3BDEF[mSquad_Selected];
	sSprite_0* eax = Dataa24[0];
	if (eax == INVALID_SPRITE_PTR || eax == 0 )
		goto loc_1F218;

	// TODO: Correct?
	DataFinal = (uint8*)eax->field_46;
	goto loc_1F1E9;

loc_1F19A:;
	if (!Data0)
		goto loc_1F1DD;

	Data0 &= 0xFFF;
	Data0 /= 0x0C;
	Data0 &= 0xFFFF;
	Data0 *= 0x0C;

	if (Data0 >= 0x60)
		Data0 = 0;
loc_1F1DD:;
	DataFinal = (uint8*) (DataFinal + Data0);

loc_1F1E9:;

	if (DataFinal >= (uint8*)mSquad && DataFinal <= (uint8*)&mSquad[8]) {
		sSquad_Member* Member = (sSquad_Member*)DataFinal;
		if (Member->mNumberOfKills >= 999)
			Member->mNumberOfKills = 998;

		++Member->mNumberOfKills;
	}
loc_1F218:;

	return sub_2060F( pSprite );
}

int16 cFodder::loc_1F043( sSprite_0* pSprite ) {
	if (!pSprite->field_66) {
		pSprite->field_38 = 0;
		return 0;
	}

	loc_1F092( pSprite,  (sSprite_0*) pSprite->field_66 );
	pSprite->field_A = 0;
	pSprite->field_8 = 0x7C;
	pSprite->field_38 = 0;
	return 0;
}

void cFodder::loc_1F092( sSprite_0* pSprite, sSprite_0* pData24 ) {
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

int16 cFodder::sub_1F21E( sSprite_0* pSprite ) {

	if( !word_3A9B2 )
		if( !mMissionComplete )
			return 0;
		
	int16 Data0 = -1;
	int16 Data4 = 8;
	
	Map_Sprite_Check_Position( pSprite, Data0, Data4 );
	
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
	
	if( !word_3A9B2 ) {
		
		tool_RandomGet();
		Data0 &= 1;
		if(Data0 == 0)
			goto loc_1F32C;
	
		if( !pSprite->field_52 ) {
			tool_RandomGet();
			Data0 &= 1;
			
			if( !Data0 ) {
				//loc_1F2F9
				pSprite->field_8 = 0x41;
				tool_RandomGet();
				Data0 &= 3;
				pSprite->field_A = Data0;
				pSprite->field_2A = 1;
				sub_1FE35( pSprite );
				goto loc_1F3C4;
			}
		}
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

void cFodder::sub_1F429( sSprite_0* pSprite ) {
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

	if (pSprite->field_18 == 0x24) {
		word_3A00E = -1;
		word_3A00C = -1;
		word_3A010 = -1;
		if (word_3AA4B > 0x18)
			word_3A010 = 0;
		goto loc_1F4DB;

	}
	//loc_1F4B0
	if (pSprite->field_18 != 5)
		goto loc_1F4DB;
	if (mMapNumber <= 4)
		goto loc_1F4DB;

	int16 Data0 = tool_RandomGet() & 0x1F;
	if (Data0)
		goto loc_1F4DB;

	word_3A00E = -1;
	word_3A00C = 0;
loc_1F4DB:;

	Data0 = word_3A00E;
	if (Data0)
		goto loc_1F50F;

	if (!Troop_Fire_Bullet( pSprite ))
		goto loc_1F4F5;

	pSprite->field_4A = -1;
	goto loc_1F4FF;
loc_1F4F5:;
	pSprite->field_4A = 0;

loc_1F4FF:;
	if (word_3AA1D == 2)
		goto loc_1F50F;
	pSprite->field_45 = 0x0F;

loc_1F50F:;
	word_3A00E  = 0;
	if (word_3A00C)
		goto loc_1F549;

	if (!Troop_Throw_Grenade( pSprite ))
		goto loc_1F52F;

	pSprite->field_4A = -1;
	goto loc_1F539;

loc_1F52F:;
	pSprite->field_4A = 0;

loc_1F539:;
	if (word_3AA1D != 2)
		pSprite->field_45 = 0x0C;
loc_1F549:;
	word_3A00C = 0;
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

void cFodder::sub_1F5A0( sSprite_0* pSprite ) {

	int16 Data0 = pSprite->field_2E;
	int16 Data4 = pSprite->field_30;

	sub_2A1F0( pSprite, Data0, Data4 );
	sub_20E5C( pSprite );
	sub_1F762( pSprite );
}

void cFodder::sub_1F5CA( sSprite_0* pSprite ) {

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

void cFodder::sub_1F623( sSprite_0* pSprite ) {
	word_3ABAD = 0;
	
	dword_3A391 = pSprite->field_0 & 0xFFFF | pSprite->field_2 << 16;
	dword_3A395 = pSprite->field_4 & 0xFFFF | pSprite->field_6 << 16;
}

void cFodder::sub_1F649( sSprite_0* pSprite ) {
	word_3ABAD = -1;
	//TODO: Fix
	pSprite->field_0 = dword_3A391 & 0xFFFF;
	pSprite->field_2 = dword_3A391 >> 16;

	pSprite->field_4 = dword_3A395 & 0xFFFF;
	pSprite->field_6 = dword_3A395 >> 16;
}

void cFodder::sub_1F66F( sSprite_0* pSprite ) {
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

void cFodder::sub_1F6F4( sSprite_0* pSprite ) {
	
	if (!sub_20E91( pSprite )) {
		sub_20F19( pSprite );
		Sprite_Movement_Calculate( pSprite );
	}

	sub_2DBA3( pSprite );
	sub_20478( pSprite );

	byte_3ABA9 = pSprite->field_60;
	sub_1FFC6( pSprite );

	int16 Data0 = byte_3ABA9;
	int16 Data4 = pSprite->field_60;
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

void cFodder::sub_1F762( sSprite_0* pSprite ) {
	if (!pSprite->field_22)
		goto loc_1F7CE;

	if (!pSprite->field_70)
		goto loc_1F7CE;

	int16 Data0 = pSprite->field_0;
	if (Data0 != pSprite->field_26)
		goto loc_1F7CE;

	Data0 = pSprite->field_4;
	if (Data0 != pSprite->field_28)
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
	int16 Data8 = pSprite->field_3C;
	Data8 += word_3A8CF;
	Data8 &= 0x0E;

	pSprite->field_3C = Data8;
	Data8 /= 2;
	Data0 = pSprite->field_22;

	int16* Data28 = off_32AE4[pSprite->field_22];
	if (!word_3AA41)
		goto loc_1F9C0;

	if (pSprite->field_22)
		goto loc_1F98B;

	struct_8* Data30 = off_3BEF3[pSprite->field_32];
	if (Data30[pSprite->field_40].field_0 >= 0)
		goto loc_1F9C0;

	//seg005:01CC
	sSprite_0* Dataa30 = off_3BDEF[pSprite->field_32][0];

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

	Data0 = Dataa30->field_28;
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
	//TODO: Fix?
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
	sSquad_Member* Data24 = (sSquad_Member*) pSprite->field_46;

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

	pSprite->field_A = word_3A399;
	word_3A9C6 = 0;
}

void cFodder::sub_1FCF2( sSprite_0* pSprite ) {
	int16 Data0;

	pSprite->field_36 = 0x10;

	if (pSprite->field_18 == 5) {
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

void cFodder::sub_1FDE7( sSprite_0* pSprite ) {
	
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

void cFodder::sub_1FE35( sSprite_0* pSprite ) {
	int16 Data0 = 0;

	sSprite_0* Data2C, *Data30;

	if (pSprite->field_5C == 0)
		return;


	sub_211BA( Data0, Data2C, Data30 );
	if (Data0)
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

void cFodder::sub_1FF1A( sSprite_0* pSprite ) {
	int16 Data0;
	sSprite_0* Data2C, *Data30 = 0;

	Data0 = 1;
	
	sub_211BA( Data0, Data2C, Data30 );

	if (!Data0) {
		Data2C->field_0 = pSprite->field_0;
		Data2C->field_2 = pSprite->field_2;
		Data2C->field_4 = pSprite->field_4;
		Data2C->field_6 = pSprite->field_6;

		Data2C->field_0 += 3;
		Data2C->field_4 -= 8;
		Data2C->field_8 = 0x96;
		Data2C->field_A = 0;
		Data2C->field_18 = 0x17;
		Data2C->field_20 = pSprite->field_20;
		Data2C->field_32 = -1;
		Data2C->field_2C = 1;
		Data2C->field_52 = 0;
		Data2C->field_12 = 1;
		Data0 = 0;

	}
}

void cFodder::sub_1FFC6( sSprite_0* pSprite ) {
	int16 Data0 = -3;
	int16 Data4 = 8;

	if (Map_Sprite_Check_Position( pSprite, Data0, Data4 ))
		goto loc_20236;

	pSprite->field_60 = Data4 & 0xFF;
	pSprite->field_50 = 0;
	pSprite->field_4F = 0;

	if (Data4 == 0x0A) {
		if (pSprite->field_18 == 5)
			goto loc_20251;

		if (pSprite->field_22 == 2)
			goto loc_20251;

		if (!pSprite->field_38 || pSprite->field_38 >= 0x32) {
			pSprite->field_38 = 3;
			return;
		}
	}

	//loc_20044
	if (Data4 == 9) {

		if (pSprite->field_18 == 5)
			goto loc_20251;

		if (pSprite->field_22 == 2)
			goto loc_20251;

		++pSprite->field_56;
		return;
	}

	//loc_20072
	pSprite->field_56 = 0;
	if (Data4 == 8)
		goto loc_201CC;

	if (Data4 == 1)
		goto loc_20108;

	if (Data4 == 2)
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
	if (Data4 == 4)
		goto loc_2019E;

	if (Data4 == 5)
		goto loc_201AB;

	if (Data4 != 6)
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
	if (Data4 == 0x0B)
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
	if (Data4 == 9 || Data4 == 0x0A) {
		pSprite->field_38 = 2;
		return;
	}

loc_20251:;
	pSprite->field_43 = 1;
	word_3ABAD = -1;
	if (!pSprite->field_22)
		goto loc_2035C;

	// TODO: Check
	pSprite->field_0 = dword_3A391;

	Data0 = -3;
	Data4 = 8;
	if (Map_Sprite_Check_Position( pSprite, Data0, Data4 ))
		goto loc_202E5;

	if (Data4 == 9)
		goto loc_202E5;

	if (Data4 == 0x0A)
		goto loc_202E5;

	if (pSprite->field_22 == 2 && Data4 == 6)
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
	pSprite->field_0 = dword_3A391;

	Data0 = -3;
	Data4 = 8;
	if (!Map_Sprite_Check_Position( pSprite, Data0, Data4 ))
		return;

	pSprite->field_0 = word_3A9E6;
	pSprite->field_4 = dword_3A395;

	Data0 = -3;
	Data4 = 8;
	if (!Map_Sprite_Check_Position( pSprite, Data0, Data4 ))
		return;

	//loc_203BA
	pSprite->field_A = word_3A399;
	sub_1F649( pSprite );
	pSprite->field_26 = pSprite->field_0;
	pSprite->field_28 = pSprite->field_4;

	struct_8* Data30 = off_3BEF3[ pSprite->field_32 ];
	Data4 = 0;

	for (;;) {
		struct_8 eax = *Data30++;

		if (eax.asInt < 0)
			break;

		Data4++;
	}

	pSprite->field_40 = Data4;
	pSprite->field_4C = -1;
}

void cFodder::String_Print(  const uint8* pWidths, int32 pParam0, int32 pParam08, int32 pParamC, const char* pText ) {

	word_3B305 = pParamC;
	word_3B307 = 0;

	for (;;) {
	loc_29C7A:;
		uint8 NextChar = *pText++;
		uint8 NextChar10;

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
			int32 unk14 = -1;
			uint8 al;
			uint8* ptr = mUnkStringModifier;
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
				sub_145AF( pParam0 + NextChar, pParam08, pParamC );
			}
			else			         //0	// C     // 4	   // 8
				sub_13C1C(  pParam0, pParamC, NextChar, pParam08 );

		}
		loc_29DC7:;
		//seg007:02A7
		NextChar10 = pWidths[NextChar10];

		pParam08 += NextChar10;
	}
}

void cFodder::sub_310CB() {
	
	if (!mButtonPressLeft) {
		word_39F00 = 0;
		return;
	}

	if (word_39F00)
		return;

	if (sub_30E0B() >= 0)
		if (sub_313CD() < 0)
			return;

	sSprite_0* Data20 = word_39FCE;

	if (Data20 < 0)
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

	sSprite_0** Data24 = off_3BDEF[mSquad_Selected];

	for (;;) {

		if (*Data24 == INVALID_SPRITE_PTR)
			break;

		sSprite_0* Data28 = *Data24++;
		Data28->field_66 = 0;
	}

	const int16* Data2C = word_4547E;
	const int16* Data30 = word_4555C;
	const int16* Data34 = word_4563A;

	int16 Data0 = mMouseX;
	int16 Data4 = mMouseY;

	Data0 += mCamera_Adjust_Col >> 16;
	Data4 += mCamera_Adjust_Row >> 16;

	Data0 -= 0x0F;
	Data4 -= 3;

	sSprite_0* Data20 = mSprites;

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

		Data24 = off_3BDEF[Data0];
		for (;;) {
			if (*Data24 == INVALID_SPRITE_PTR)
				goto loc_313C6;

			sSprite_0* Data28 = *Data24++;
			Data28->field_66 = Data20;
		}
	}

loc_313C6:;
	Data0 = 0;
}

int16 cFodder::sub_313CD() {
	const int16* Data2C = word_4547E;
	const int16* Data30 = word_4555C;
	const int16* Data34 = word_4563A;

	int16 Data0 = mMouseX;
	int16 Data4 = mMouseY;

	Data0 += mCamera_Adjust_Col >> 16;
	Data4 += mCamera_Adjust_Row >> 16;

	Data0 -= 0x0F;
	Data4 -= 3;

	if (!dword_3B20B)
		goto loc_31692;

	sSprite_0* Data20 = dword_3B20B;
	if (Data20->field_0 == -32768)
		goto loc_31689;

	if (Data20->field_22)
		goto loc_31689;

	int16 Data18 = Data20->field_18;
	int16 Data8 = Data20->field_0;
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

	if (Map_Sprite_Check_Position(Data20, Data0,Data4))
		goto loc_31689;

	sSprite_0*** Data28 = off_3BDEF;
	Data0 = mSquad_Selected;
	if (Data0 < 0)
		goto loc_31668;

//seg011:2A84
	word_3B461[Data0] = 0;
	sSprite_0** Dataa2C = Data28[Data0];

	//loc_31578
	for (; Data18 >= 0; --Data18) {
		sSprite_0* eax = *Dataa2C;

		if (eax < 0)
			goto loc_31668;

		++Dataa2C;
		sSprite_0* Data24 = eax;

		++Dataa2C;
		if (!Data24->field_6E)
			continue;

		//seg011:2AE9 

		Data24->field_6E = 0;
		sSprite_0* Dataa30 = (sSprite_0*) Data24->field_6A;
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
	word_39EFC = 0;
loc_3167D:;
	Data0 = -1;
	return -1;

loc_31689:;
	Data20->field_6E = 0;
loc_31692:;
	Data0 = 0;
	return 0;
}

void cFodder::sub_3169B() {

	if (sub_30E2A() < 0)
		return;

	sSprite_0* Data20 = dword_3B20B;
	Data20->field_54 = -1;

	int16 Data8 = mMouseX;
	int16 DataC = mMouseY;
	Data8 += mCamera_Adjust_Col >> 16;
	DataC += mCamera_Adjust_Row >> 16;
	Data8 -= 0x10;

	if (!Data8)
		Data8 = 1;

	DataC -= 8;
	Data20->field_2E = Data8;
	Data20->field_30 = DataC;
}

void cFodder::String_CalculateWidth( int32 pPosX, const uint8* pWidths, const char* pString ) {
	int32 PositionX = 0;

	for (const char* Text = pString; *Text; ++Text) {

		PositionX += pWidths[*Text];
	}

	pPosX -= PositionX;
	if (pPosX < 0) {
		pPosX = 0;
		PositionX = 318;
	}

	pPosX >>= 1;
	word_3B301 = pPosX;
	word_3B303 = PositionX;
}

void cFodder::intro_LegionMessage() {
	int16 Duration = 300;
	bool DoBreak = false;

	mImage->paletteSet( mPalette, 0xD0 );

	mImageFaded = -1;

	sub_18C45( mIntro_0[0].mPosition - 0x19, &mIntro_0[0] );
	sub_18C45( mIntro_0[1].mPosition - 0x19, &mIntro_0[1] );
	sub_18C45( mIntro_0[2].mPosition - 0x19, &mIntro_0[2] );

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

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
	}
}

int16 cFodder::introPlayText() {

	Sprite_SetDataPtrToBase( mFontSpriteSheetPtr );

	for ( word_3B2CF = 0; mIntroText[word_3B2CF].mImageNumber != 0; ++word_3B2CF) {

		word_3B447 = 0x288;
		
		if (mIntroText[word_3B2CF].mImageNumber != 0xFF) {

			std::stringstream ImageName;
			size_t ImageSize = 0;
			ImageName << mIntroText[word_3B2CF].mImageNumber;
			ImageName << ".dat";

			g_Resource.image4PlaneLoad( mImage, ImageName.str(), 0xD0 );
			mImage->paletteSet( mPalette, 0xD0 );
		}
		else {
			mImage->paletteSet( mPalette, 0xD0 );
			word_3B447 = 0xAF;
			
		}

		const sIntroString* IntroString = mIntroText[word_3B2CF].mText;
		while (IntroString->mPosition) {

			sub_18C45( IntroString->mPosition - 0x19, IntroString );
			++IntroString;
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

				mouse_GetData();
				if (mouse_Button_Status) {
					word_3B2CF = 15;
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

			g_Window.RenderAt( mImage, cPosition() );
			g_Window.FrameEnd();
		}
	}

	return word_3B4F3;
}

void cFodder::Sprite_SetDataPtrToBase( const sSpriteSheet** pSpriteSheet ) {

	mSpriteDataPtr = pSpriteSheet;
}

void cFodder::Load_Sprite_Font() {
	
	g_Resource.fileLoadTo( "font.dat", mDataPStuff );

	paletteLoad( mDataPStuff + 0xA000, 0x10, 0xD0 );
	Sprite_SetDataPtrToBase( mFontSpriteSheetPtr );

	Sound_Voc_Load();
}

void cFodder::intro() {

	//copyprotection();
	word_42851 = 0;
	sub_136D0();

	word_3B4F3 = 0;
	Load_Sprite_Font();
	
	if (mSkipIntro)
		goto introDone;

	intro_Music_Play();

	intro_LegionMessage();

	if (ShowImage_ForDuration( "cftitle.dat", 0x1F8 ))
		goto introDone;

	if (introPlayText())
		goto introDone;

	if (ShowImage_ForDuration( "virgpres.dat", 0x2D0 ))
		goto introDone;
	
	if (ShowImage_ForDuration( "sensprod.dat", 0x2D0 ))
		goto introDone;
	
	word_42851 = -1;
	if (ShowImage_ForDuration( "cftitle.dat", 0x318 ))
		goto introDone;

introDone:;
	mIntroDone = -1;
	//sub_1645F();
	Music_Stop();

	g_Resource.fileLoadTo( "pstuff.dat", mDataPStuff );
	paletteLoad( mDataPStuff + 0xA000, 0x10, 0xF0 );
	//Sound_Unk();
	Music_Unk( 0 );
}

void cFodder::intro_Music_Play() {

	Music_Play( "rjp.WARX4(1)" );
}

int16 cFodder::ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration ) {
	bool DoBreak = false;
	int16 Fade = -1;
	g_Resource.image4PlaneLoad( mImage, pFilename, 0x100 );
	
	while( Fade == -1 || DoBreak == false  ) {
		--pDuration;

		if (pDuration) {
			if (Fade)
				Fade = mImage->paletteFade();

			mouse_GetData();
			if (mouse_Button_Status) {
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

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
	}

	return word_3B4F3;
}

void cFodder::Mission_PhaseNext() {
	mMissionPhase += 1;
	mMissionPhaseRemain -= 1;
	if (mMissionPhaseRemain)
		return;

	for (unsigned int x = 0; x < 8; ++x) {
		mSquad[x].field_3 = 0;
	}

	word_390D0 = mMissionNumber;
	word_390D2 = mMapNumber;
	--word_390D2;

	mMissionPhaseRemain = mMissionPhases = mMissionPhaseTable[mMissionNumber];
	mMissionPhase = 0;
	++mMissionNumber;
	word_390CE += 0x0F;

	word_390E8 = word_390CE;
	word_390E8 -= 0x0F;
	word_390EA = -1;
	word_390D4 = 0;

	dword_3977E = word_394AC;
	word_394AC[0] = -1;
	dword_394A8 = dword_394A4;
}

void cFodder::videoSleep() {
	static uint64_t delta = 2;

	mTicksDiff = GetTickCount() - mTicksDiff;

	mTicks = mTicksDiff * 60 / 1000;

	sleepLoop(delta * 1000 / 60 - mTicksDiff);

	mTicksDiff = GetTickCount();
}

void cFodder::videoSleep_50() {
	static uint64_t delta = 1;

	mTicksDiff = GetTickCount() - mTicksDiff;

	mTicks = mTicksDiff * 50 / 1000;

	sleepLoop(delta * 1000 / 50 - mTicksDiff);

	mTicksDiff = GetTickCount();
}

void cFodder::sleepLoop( int64 pMilliseconds ) {

	uint64 TimeStarted = GetTickCount();
	uint64 TimeFinish = TimeStarted + pMilliseconds;

	do {
		//_system->updateScreen();
		mWindow->EventCheck();

		if (GetTickCount() >= TimeFinish)
			break;

		Sleep( 10 );

	} while (1);

}

void cFodder::WonGame() {
	//seg003:3E6D
	g_Resource.image4PlaneLoad( mImage, "won.dat", 0x100 );

	while(mImage->paletteFade()) {

		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();
	}

	while (1) {
		g_Window.EventCheck();
	}
}

void cFodder::sub_20456( sSprite_0* pSprite, int16& pData8 ) {
	pData8 = pSprite->field_10;

	pData8 >>= 5;
	pData8 -= 1;
	pData8 ^= 0x0F;
	pData8 &= 0x0E;
}

void cFodder::sub_20478( sSprite_0* pSprite ) {
	int16 Data0 = pSprite->field_4;

	if (Data0 < 0 || Data0 <= 0x0D) {
		pSprite->field_4 = dword_3A395;
		word_3ABAD = -1;
	}

	Data0 = pSprite->field_4;

	if (Data0 >= word_3AA4F) {
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
	if (Data0 < word_3AA4D)
		return;

	Data0 = dword_3A391 + 0x10 ;
	if (Data0 >= word_3AA4D)
		return;

loc_20521:;
	pSprite->field_0 = dword_3A391;
	word_3ABAD = -1;
}

void cFodder::Hero_Add( sSquad_Member* pSquadMember ) {
	sHero* Hero = &mHeroes[4];
	int16 Data4;

	for (Data4 = 4; Data4 >= 0; --Data4, --Hero ) {
		int16 Data0 = pSquadMember->mNumberOfKills;
		if (!Data0)
			break;

		if (Data0 < Hero->mKills)
			break;

		if (Data0 > Hero->mKills)
			continue;

		Data0 = pSquadMember->mRecruitID;
		if (Data0 <= Hero->mRecruitID)
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

int16 cFodder::sub_2060F( sSprite_0* pSprite ) {

	return sub_2061C( pSprite );
}

int16 cFodder::sub_2061C( sSprite_0* pSprite ) {
	pSprite->field_65 = 0;
	pSprite->field_8 = 0x7C;
	pSprite->field_18 = 4;
	pSprite->field_A = 0;
	pSprite->field_24 = 1;

	return 1;
}

int16 cFodder::Troop_Fire_Bullet( sSprite_0* pSprite ) {
  	if (word_3A9E4)
		--word_3A9E4;

	if (word_3A9AE)
		goto loc_20A2D;

	if (!pSprite->field_2E)
		goto loc_20A2D;

	if (pSprite == word_39FCE)
		if (pSprite->field_54 == 1)
			goto loc_20A2D;

	int16 Data0 = 1;
	sSprite_0* Data2C = 0, *Data30 = 0;
	sub_211BA( Data0, Data2C, Data30 );

	if (Data0)
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
	if (!pSprite->field_18)
		goto loc_20777;

	Data0 = pSprite->field_62;
	Data0 >>= 3;
	Data0 += 8;
	if (Data0 > 0x10)
		Data0 = 0x10;
	Data2C->field_12 = Data0;
	goto loc_207CF;

loc_20777:;

	Data2C->field_5E = (sSquad_Member*) pSprite->field_46 - mSquad;
	Data2C->field_5D = -1;
	sSquad_Member* Data24 = (sSquad_Member*) pSprite->field_46;

	Data0 = stru_3ABB9.field_2;
	Data0 += word_3ABC3;
	Data2C->field_12 = Data0;

loc_207CF:;
	Data2C->field_18 = 6;
	Data2C->field_1E = pSprite->field_1E;
	Data2C->field_20 = pSprite->field_20;
	Data2C->field_20 += 6;
	Data2C->field_52 = pSprite->field_52;
	Data2C->field_22 = pSprite->field_22;
	Data2C->field_32 = pSprite->field_32;
	Data2C->field_2C = 0;
	if (pSprite->field_22)
		goto loc_2087D;

	Data0 = stru_3ABB9.field_0;
	Data0 += word_3ABC5;
	Data2C->field_4A = Data0;

	Data0 = tool_RandomGet() & 0x0F;
	Data0 <<= 3;
	Data2C->field_36 = Data0;
	goto loc_208A6;

loc_2087D:;
	Data0 = 0x3C;
	Data0 += pSprite->field_62;
	Data2C->field_4A = Data0;
	Data2C->field_36 = 0x18;

loc_208A6:;
	Data2C->field_43 = 0;
	Data2C->field_44 = 0;
	Data2C->field_2E = pSprite->field_2E;
	Data2C->field_30 = pSprite->field_30;
	if (pSprite->field_18)
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
	int16 Data8 = 7;

	if (pSprite->field_22)
		goto loc_209C7;

	if (pSprite == word_39FCE)
		goto loc_209B3;

	sub_2D26A( (sSquad_Member*) pSprite->field_46, Data8 );
	goto loc_209C7;

loc_209B3:;
	word_3B1AB += 1;
	word_3B1AB &= 3;
	if (!word_3B1AB)
		goto loc_209F3;

	Data8 = stru_3ABB9.field_6;
loc_209C7:;
	Data0 = tool_RandomGet();
	int16 Data4 = Data0;

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

	Sound_Voc_Play( pSprite, Data4, 0 );
	return 0;

loc_20A2D:;
	return -1;
}

int16 cFodder::Troop_Throw_Grenade( sSprite_0* pSprite ) {

	if (word_3A9AE)
		goto loc_20ADE;
	
	if (byte_3A9D2[pSprite->field_22] == 2)
		goto loc_20ADE;

	if (!pSprite->field_2E)
		goto loc_20ADE;

	if (pSprite->field_18 != 5)
		goto loc_20AC1;

	int16 Data0 = pSprite->field_0;
	int16 Data4 = pSprite->field_4;
	int16 Data8 = pSprite->field_2E;
	int16 DataC = pSprite->field_30;
	sub_29E30( Data0, Data4, Data8, DataC );
	if (Data0 >= 0x82)
		goto loc_20ADE;

loc_20AC1:;

	Data0 = 2;
	sSprite_0* Data2C = 0, *Data30 =0;

	sub_211BA( Data0, Data2C, Data30 );
	if (!Data0)
		goto loc_20B0A;
	
	return -1;

loc_20ADE:;
	if (pSprite == word_39FCE)
		word_39EFC = 0;

	return -1;

loc_20B0A:;
	if (pSprite->field_18 != 0)
		goto loc_20B6E;

	Data0 = pSprite->field_32;
	Data4 = Data0;
	if (!mSquad_Grenades[Data0])
		goto loc_20ADE;

	--mSquad_Grenades[Data0];
	byte_3AC33[Data4] = -1;

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
	int16 Data18 = Data0;
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

	sub_29E30( Data0, Data4, Data8, DataC );
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
	Data2C->field_18 = 2;
	Data30->field_18 = 3;
	Data2C->field_52 = 0;
	Data30->field_52 = 0;
	Data2C->field_50 = 0;
	Data2C->field_22 = pSprite->field_22;
	Data30->field_22 = pSprite->field_22;
	Data2C->field_32 = pSprite->field_32;
	Data2C->field_2C = 0;
	Data30->field_2C = -1;
	Data2C->field_38 = 0;
	if (pSprite->field_18 == 5)
		Data2C->field_12 += 0x1C;
	Data2C->field_56 = 4;
	Data2C->field_46 = (int32*)pSprite;

	if (pSprite == word_39FCE)
		word_39EFC = 0;

	return 0;
}

void cFodder::sub_20E5C( sSprite_0* pSprite ) {
	int16 Data0 = pSprite->field_10;

	Data0 >>= 5;
	Data0 -= 1;
	Data0 ^= 0x0F;
	Data0 &= 0x0F;
	pSprite->field_3C = Data0;

	word_3A8CF = 0;
}

int16 cFodder::sub_20E91( sSprite_0* pSprite ) {

	if (pSprite->field_36 <= 8)
		return 0;

	int16 Data0 = pSprite->field_26;
	int16 Data4 = pSprite->field_28;

	int16 Data8 = pSprite->field_0;
	int16 DataC = pSprite->field_4;

	sub_29E30( Data0, Data4, Data8, DataC );
	if (Data0 > 2)
		return 0;

	pSprite->field_0 = pSprite->field_26;
	pSprite->field_4 = pSprite->field_28;

	word_3ABAD = -1;
	Data0 = -1;
	return -1;
}

void cFodder::sub_20F19( sSprite_0* pSprite ) {

	if (pSprite->field_60 != 0x0D)
		goto loc_20FBB;

	int16 Data0 = pSprite->field_10;
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
	int16 Data4 = pSprite->field_36;
	pSprite->field_36 -= Data0;
}

void cFodder::sub_21041( sSprite_0* pSprite ) {
	
	if (pSprite->field_22) {
		pSprite->field_4D = 0;
		return;
	}

	word_3B2F3 = 0;
	sSprite_0** Data30 = off_3BDEF[pSprite->field_32];
	sSprite_0** Data34 = Data30;

	for (;;) {
		if (*Data30 == INVALID_SPRITE_PTR )
			return;

		sSprite_0* eax = *Data30;
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
	sSprite_0** eax = Data30;
	sSprite_0* Data24 = *Data30;
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

	sub_1F649( pSprite );
	word_3A9C6 = -1;
	pSprite->field_43 = 1;
	word_3B2F3 = -1;
}

int16 cFodder::sub_211BA( int16& pData0, sSprite_0*& pData2C, sSprite_0*& pData30 ) {
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
	pData2C = &word_3B4F7;
	pData30 = &word_3B4F7;
	pData0 = -1;
	word_3B15D = pData0;
	return -1;

loc_21217:;

	sub_212C4( pData2C );
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
	
	pData2C = &word_3B4F7;
	pData30 = &word_3B4F7;
	pData0 = -1;
	word_3B15D = pData0;
	return -1;

loc_2128F:;
	pData30 = pData2C + 1;

	sub_212C4( pData2C );
	sub_212C4( pData30 );
	pData0 = 0;
	return 0;
}

void cFodder::sub_212C4( sSprite_0* pSprite ) {
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

void cFodder::sub_212F9( sSprite_0* pSprite ) {
	
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

	int32 Dat0 = (int32) pSprite->field_1A;
	int32 Dat4 = pSprite->field_1E | pSprite->field_20 << 16;

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

	int16 Data0 = -3;
	int16 Data4 = 0x0C;

	Map_Sprite_Check_Position( pSprite, Data0, Data4 );

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

	if (word_3A9AE)
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
	sub_21483( pSprite );
	return;

loc_21464:;
	pSprite->field_18 = 0x0C;
	pSprite->field_26 = 0x5F5F;
	pSprite->field_28 = 8;
}

void cFodder::sub_21483( sSprite_0* pSprite ) {
	int16 Data0 = 1;
	sSprite_0* Data2C = 0, *Data30 = 0;

	sub_211BA( Data0, Data2C, Data30 );
	if (Data0)
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
	Data2C->field_18 = 0x23;
	Data2C->field_20 = 0;
	Data2C->field_32 = -1;
	Data2C->field_2C = -1;
	Data2C->field_52 = 0;
}

void cFodder::sub_21525( sSprite_0* pSprite ) {
	int16 Data0 = -3;
	int16 Data4 = 2;

	if (Map_Sprite_Check_Position( pSprite, Data0, Data4 ))
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

	if (!Map_Sprite_Check_Position( pSprite, Data0, Data4 )) {
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
	sub_1F649( pSprite );
}

int16 cFodder::sub_21618( sSprite_0* pSprite ) {

	if (pSprite->field_22) {

		int16 Data8 = pSprite->field_0;
		int16 DataC = pSprite->field_0;
		DataC += 6;
		int16 Data10 = pSprite->field_4;
		Data10 -= 9;
		int16 Data14 = pSprite->field_4;
		Data14 -= 4;
		word_3AA45 = 1;

		SquadMember_Sprite_Find_In_Region( pSprite, Data8, DataC, Data10, Data14 );

		return 0;
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
	sSprite_0* Data24 = 0;
	if (Sprite_Find_In_Region(pSprite, Data24, Data8, DataC, Data10, Data14 ) >= 0)
		return 0;

	if (Data24->field_18 == 5) {

		Data24->field_5E = pSprite->field_5E;
		Data24->field_5D = pSprite->field_5D;

		return -1;
	}

	return 0;
}

void cFodder::sub_21702( sSprite_0* pSprite, int16 pData18 ) {
	int16 Data0 = 2;
	sSprite_0* Data2C, *Data30;

	sub_211BA( Data0, Data2C, Data30 );
	if (Data0)
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
	Data30->field_18 = 0x03;
	Data2C->field_52 = 0;
	Data30->field_52 = 0;
	Data2C->field_2C = 0;
	Data30->field_2C = -1;
	Data2C->field_38 = 0;

	Data2C->field_10 = pData18;
	Data2C->field_1A = (int32*)0x0A8000;

}

void cFodder::sub_2183B( sSprite_0* pSprite ) {

	if (!pSprite->field_5C)
		return;

	//  cmp     ds:word_3B1A9, 14h
	int16 Data0 = 1;
	sSprite_0* Data2C = 0, *Data30 = 0;
	
	if (sub_211BA( Data0, Data2C, Data30 ))
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

void cFodder::sub_218E2( sSprite_0* pSprite ) {
	int16 Data8 = pSprite->field_0;
	int16 DataC = pSprite->field_4;
	
	sub_212C4( pSprite );
	sub_2194E( pSprite, Data8, DataC );
}

int16 cFodder::sub_21914( int16& pData8, int16& pDataC ) {
	int16 Data0 = 1;
	sSprite_0* Data2C, *Data30;

	sub_211BA(Data0, Data2C, Data30);

	if (Data0)
		return -1;

	sub_212C4(Data2C);

	pData8 &= -16;
	pDataC &= -16;

	return sub_2194E( Data2C, pData8, pDataC );
}

int16 cFodder::sub_2194E( sSprite_0* pData2C, int16& pData8, int16& pDataC ) {
	pData2C->field_0 = pData8;
	pData2C->field_4 = pDataC;

	pData2C->field_4 += 0x10;

	sSprite_0* Data20 = pData2C;

	int16 Data4 = word_390AE;
	Data4 &= 3;
	Data4 += 5;

	int16 Data8 = 0x1E;
	Sound_Voc_Play( Data20, Data4, Data8 );

	pData2C->field_18 = 0x27;
	pData2C->field_8 = 0x8E;
	pData2C->field_A = 0;
	pData2C->field_12 = 1;
	pData2C->field_22 = -1;
	pData2C->field_32 = -1;
	pData2C->field_2C = 1;

	return 0;
}

int16 cFodder::Troop_Deploy( sSprite_0* pSprite ) {
	
	if (mMissionComplete || mTroops_Enemy_Count >= 0x0A)
		return -1;
	int16 Data0 = 1;
	sSprite_0* Data2C = 0, *Data30 = 0;

	sub_21B15( Data0, Data2C, Data30 );
	if (Data0)
		return -1;

//loc_21A1C:;
	sub_212C4( Data2C );
	Data2C->field_18 = 5;
	Data2C->field_0 = pSprite->field_0;
	Data2C->field_0 -= 6;
	Data2C->field_4 = pSprite->field_4;
	Data2C->field_4 += 4;
	Data2C->field_8 = 0x7C;
	Data2C->field_4A = 0;
	Data2C->field_22 = 1;

	Data0 = tool_RandomGet() & 0xFF;
	Data0 += 0x78;
	Data0 = -Data0;
	Data2C->field_5E = Data0;
	Data0 = tool_RandomGet();
	int16 Data4 = Data0;
	Data0 &= 0x1E;
	if (Data4 < 0)
		Data0 = -Data0;

	Data0 += 0x1C0;
	Data2C->field_10 = Data0;
	Data4 &= 0x0F;
	Data4 += 8;
	Data2C->field_44 = Data4;
	sub_21C00(Data2C);

	word_390E6 += 1;
	word_390E6 &= 0x0F;
	if (!word_390E6 && word_390C2 < 0x1E)
		++word_390C2;

	++mTroops_Enemy_Count;
	return 0;
}

void cFodder::sub_21B15( int16& pData0, sSprite_0*& pData2C, sSprite_0*& pData30 ) {
	if (word_3B15F)
		goto loc_21B4B;

	if (pData0 == 2)
		goto loc_21B91;

	int16 Data1C = 29;
	pData2C = &mSprites[29];
	for (; Data1C >= 0; --Data1C, --pData2C ) {

		if (pData2C->field_0 == -32768)
			goto loc_21B72;
	}

loc_21B4B:;
	pData2C = &word_3B4F7;
	pData30 = &word_3B4F7;
	pData0 = -1;
	word_3B15F = pData0;
	return;

loc_21B72:;
	sub_212C4( pData2C );
	pData0 = 0;
	return;

loc_21B91:;
	pData2C = mSprites;

	for( int16 Data1C = 0x1C; Data1C >= 0; --Data1C, ++pData2C ) {
		
		if( pData2C->field_0 != -32768 )
			continue;
		
		if( (pData2C+1)->field_0 == -32768 )
			goto loc_21BEC;
	}
	
	pData2C = &word_3B4F7;
	pData30 = &word_3B4F7;
	pData0 = -1;
	word_3B15F = pData0;
	return;

loc_21BEC:;
	pData30 = pData2C + 1;
	pData0 = 0;
	return;
}

void cFodder::sub_21C00( sSprite_0* pData2C ) {
	int16 Data8 = word_390C4;
	pData2C->field_62 = Data8;

	int16 Data4 = word_390C6;
	Data8 += Data4;

	if (Data4 < 0)
		goto loc_21C42;

	if (Data8 < word_390C2)
		goto loc_21C5E;

	Data8 = word_390C2;
	Data4 = -Data4;
	goto loc_21C5E;

loc_21C42:;
	if (Data8 < 0)
		goto loc_21C52;

	if (Data8 > word_390C0)
		goto loc_21C5E;

loc_21C52:;
	Data8 = word_390C0;
	Data4 = -Data4;

loc_21C5E:;
	word_390C6 = Data4;
	word_390C4 = Data8;
}

void cFodder::sub_21C6F( ) {
	sSprite_0* Data2C = 0, *Data30 = 0;
	int16 Data0 = 1;
	sub_211BA( Data0, Data2C, Data30 );

	if (Data0)
		return;

	Data2C->field_8 = 0x7C;
	Data2C->field_A = 0;
	Data2C->field_18 = 0x15;
	Data2C->field_0 = 0;
	Data2C->field_4 = 0;
	Data2C->field_22 = -1;
	Data2C->field_10 = 1;
	Data2C->field_3A = 0;
}

void cFodder::sub_21CD1( sSprite_0* pSprite ) {
	word_3A00E = 0;
	word_3A00C = -1;
	word_3A010 = -1;
	word_3A8CF = 0;
	word_3AA41 = -1;

	int16 Data1C = pSprite->field_5E;
	sSprite_0* Data0;

	if (Data1C < 0) {

		pSprite->field_5E += 1;
		word_3AA41 = 0;

		Data0 = word_39FCE;
		if (Data0 == INVALID_SPRITE_PTR)
			goto loc_22125;

		goto loc_22053;
	}

	sSprite_0* Data28 = dword_3BE03[Data1C];
	if (Data28 == INVALID_SPRITE_PTR)
		goto loc_21E4A;

	int16 Dataa0 = tool_RandomGet() & 0x3F;
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
	int16 Data4 = pSprite->field_4;

	int16 Data8 = Data28->field_0;
	int16 DataC = Data28->field_4;

	sub_29E30( Dataa0, Data4, Data8, DataC );

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

	sSprite_0* Data24 = pSprite->field_70;
	if (Data24->field_0 == -32768)
		goto loc_21E81;

	if (Data24->field_18 == 0x48)
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
	const uint8* Data30 = byte_3D4A5;
	// TODO: Does this bit even get used in the original
	//std::cout << "TODO: Where does Data30 come from?\n";

loc_22000:;
	if (word_3AA41)
		goto loc_22125;

	Dataa0 = pSprite->field_5E;

	if (dword_3BE03[Dataa0] == INVALID_SPRITE_PTR)
		goto loc_22125;

	Data0 = dword_3BE03[Dataa0];

loc_22053:;		// Movement Target?

	Data8 = (pSprite - mSprites);
	Data8 *= 0x76;
	Data8 &= 0x1FE;

	DataC = word_3EABF[Data8 / 2];
	Data8 += 0x80;
	Data8 &= 0x1FE;
	int16 Data10 = word_3EABF[Data8 / 2];

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

void cFodder::sub_2212A( sSprite_0* pSprite ) {
	
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

int16 cFodder::sub_221A6( sSprite_0* pSprite ) {
	if (pSprite->field_38 == 7 || pSprite->field_38 == 5)
		goto loc_221D0;

	if (pSprite->field_38 == 6)
		goto loc_22235;

	return 0;
loc_221D0:;	// Building Explodes
	pSprite->field_38 = 6;
	pSprite->field_8 = 0x99;

	int16 Data0 = tool_RandomGet();
	int16 Data4 = Data0;

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
	if (Map_Sprite_Check_Position( pSprite, Data0, Data4 ))
		goto loc_2227F;

	if (pSprite->field_36 <= 0)
		goto loc_2227F;

	return -1;

loc_2227F:;	// Door Finished
	pSprite->field_0 = 8;
	pSprite->field_4 = 8;
	
	sub_218E2( pSprite );

	return -1;
}

void cFodder::sub_223B2( sSprite_0* pSprite ) {
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

int16 cFodder::sub_2244B( sSprite_0* pSprite ) {
	if (dword_3B20B)
		return 1;

	if (word_39FCE == INVALID_SPRITE_PTR || word_39FCE == 0 )
		return 1;

	if (word_39FCE->field_6E)
		return 1;

	if (word_39FCE->field_38)
		return 1;

	int16 Data0 = pSprite->field_0;
	int16 Data4 = pSprite->field_4;

	int16 Data8 = word_39FCE->field_0;
	int16 DataC = word_39FCE->field_4;
	int16 Data10 = 0x20;

	DataC += 2;

	sub_2A74F( Data0, Data4, Data8, Data10, DataC );
	if (Data0 >= 6)
		return 1;

	Data4 = 0;
	return 0;
}

int16 cFodder::sub_224ED( sSprite_0* pSprite ) {
	if (word_3A9AE)
		goto loc_22592;

	if (byte_3A9D6[pSprite->field_22] == 2)
		goto loc_22592;

	if (!pSprite->field_2E)
		goto loc_22592;

	//seg005:2E39
	if (pSprite->field_18 != 5)
		goto loc_22575;

	int16 Data0 = pSprite->field_0;
	int16 Data4 = pSprite->field_4;
	int16 Data8 = pSprite->field_2E;
	int16 DataC = pSprite->field_30;
	sub_29E30( Data0, Data4, Data8, DataC );
	if (Data0 >= 0x82)
		goto loc_22592;

loc_22575:;
	sSprite_0* Data2C = 0, *Data30 = 0;
	Data0 = 2;
	sub_211BA(Data0,Data2C,Data30);
	if (!Data0)
		goto loc_225BE;

	Data0 = -1;
	return -1;

loc_22592:;

	if (pSprite == word_39FCE)
		word_39EFC = 0;
	Data0 = -1;
	return -1;

loc_225BE:;
	if (pSprite->field_18 != 0)
		goto loc_22622;

	int16* Data24 = mSquad_Rockets;
	Data0 = pSprite->field_32;
	Data4 = Data0;

	if (!Data24[Data0])
		goto loc_22592;

	--Data24[Data0];
	byte_3AC39[Data4] = -1;
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
	Data30->field_0 = pSprite->field_30;
	Data30->field_2 = pSprite->field_30;
	Data2C->field_4 = pSprite->field_4;
	Data2C->field_6 = pSprite->field_6;
	Data30->field_4 = pSprite->field_4;
	Data30->field_6 = pSprite->field_4;

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
	if (!pSprite->field_22)
		goto loc_22801;

	if (!(pSprite->field_75 & 1))
		goto loc_22801;

	sSprite_0* Data34 = 0;

	if (!sub_228B5(pSprite, Data34))
		goto loc_227EB;

	Data2C->field_1A = (int32*)Data34;
	Data2C->field_18 = 0x61;
	Data2C->field_6A = (sSprite_0*) 0x10000;
	Data2C->field_26 = pSprite->field_2E;
	Data2C->field_28 = pSprite->field_30;
	Data2C->field_28 += 0x10;
	Data2C->field_38 = 0;
	Data2C->field_36 = 0;
	Data30->field_36 = 0;
	goto loc_2281B;

loc_227EB:;
	Data2C->field_32 = 0;
	Data2C->field_18 = 0x2D;
	goto loc_2281B;

loc_22801:;
	Data2C->field_18 = 0x21;
	Data2C->field_46 = (int32*) pSprite;

loc_2281B:;
	Data30->field_18 = 3;
	Data2C->field_52 = 0;
	Data30->field_32 = 0;
	Data2C->field_22 = pSprite->field_22;
	Data30->field_22 = Data2C->field_22;
	Data2C->field_32 = pSprite->field_32;
	Data2C->field_2C = 0;
	Data30->field_2C = -1;
	Data2C->field_56 = 6;

	if (pSprite == word_39FCE)
		word_39EFC = 0;
	Data0 = 0;
	return 0;
}

int16 cFodder::sub_228B5( sSprite_0* pSprite, sSprite_0*& pData34 ) {
	const int16* Data30 = word_45718;

	int16 Data8 = mMouseX;
	int16 DataC = mMouseY;

	Data8 += mCamera_Adjust_Col >> 16;
	DataC += mCamera_Adjust_Row >> 16;

	Data8 -= 0x10;
	if (!Data8)
		Data8 = 1;

	DataC += 0x08;
	
	pData34 = mSprites;
	for (int16 Data1C = 0x2B; Data1C >= 0; --Data1C, ++pData34) {

		if (pData34->field_0 == -32768)
			continue;

		if (!Data30[pData34->field_18])
			continue;

		if ( pData34->field_22 == pSprite->field_22)
			continue;

		int16 Data0 = pData34->field_0;
		Data0 += 5;

		int16 Data4 = pData34->field_4;
		Data4 -= 5;

		Data4 -= pData34->field_20;
		int16 Data10 = 0x1F;
		sub_2A74F( Data0, Data4, Data8, Data10, DataC );

		if (Data0 < 0x16) {
			Data0 = -1;
			return -1;
		}

	}
	
	return 0;
}

void cFodder::sub_22A3D( sSprite_0* pSprite ) {
	pSprite->field_0 = 1;
	pSprite->field_4 = 1;

	sub_22A55( pSprite );
}

int16 cFodder::sub_22A55( sSprite_0* pSprite ) {
	return sub_2060F( pSprite );
}

void cFodder::sub_22AA9( sSprite_0* pSprite ) {
	pSprite->field_8 = 0x7C;
	pSprite->field_A = 0;
	pSprite->field_38 = 0;
	
	sSprite_0* Data24 = pSprite->field_6A;
	if( Data24->field_0 == -32768 || Data24->field_18 == 0x0C ) {
		//loc_22AEE
		pSprite->field_6E = 0;
		pSprite->field_66 = 0;
		pSprite->field_6A = 0;
		pSprite->field_38 = 1;
		pSprite->field_64 = -1;
		
		int16 Data0 = tool_RandomGet() & 0x1FE;
		pSprite->field_10 = Data0;
	}
	
	//loc_22B3C

	pSprite->field_0 = Data24->field_0;
	pSprite->field_4 = Data24->field_4;
	pSprite->field_20 = Data24->field_20;
}

void cFodder::sub_2E04C() {
	//TODO
	/*
	if( word_3B2FD ) {
		sub_2E72B();
		return;
	} 

	if( word_3B2FF ) {
		sub_2E064();
	}*/
}

void cFodder::Start( int16 pStartMap ) {
	mImage = new cSurface( 352, 250 );

	mouse_Setup();
	Mouse_Inputs_Get();

loc_103BF:;
	for (;;) {
		sub_10BBC();

		mMissionComplete = 0;
		mMapNumber = pStartMap;
		word_3901E = 0x3333;

		Troops_Clear();
		Heroes_Clear();

		dword_394A4 = word_391D2;
		word_391D2[0] = -1;

		mMissionPhaseRemain = 1;
		mMissionNumber = 0;

		Mission_PhaseNext();

		word_3A9AA = -1;
		word_39096 = -1;

		size_t StuffSize = 0;
		g_Resource.fileLoadTo( "pstuff.dat", mDataPStuff );

		for (unsigned int x = 0; x < 0x30; ++x)
			byte_3DDA2[x] = mDataPStuff[0xA000 + x];

		//loc_1042E:;
		for (;;) {
			//video_?_1();

			if (!word_3A9B2 && !word_3A9AA) {

				if (mMapNumber == 71)
					WonGame();

				++mMapNumber;
				Mission_PhaseNext();
				word_3901E = 0x3333;
			}

			// loc_1045F
			sub_10D61();
			sub_10D9F();
			sub_10DEC();
			//video_?_1();
			word_390A6 = 0;

			if (!mIntroDone)
				intro();

			if (dword_3901A != 0x12345678)
				dword_3901A = 0x12345678;

			//loc_10496
			sub_10EA4();
			sub_10EC3();
			if (word_390EA) {
				word_390EA = 0;

				map_Load_Spt();
				word_3ABA7 = -1;

				Squad_Member_Count();
				Squad_Member_Sort();
				sub_1142D();
				sub_115F7();

				word_3ABA7 = 0;
				Recruit_Show();
				sub_2E04C();
				
				if (!word_3B2FD) {
					if (word_3B2FF) {
						word_390B8 = -1;
						word_390EA = -1;
						word_3A9B2 = -1;
						continue;	// goto loc_1042E;
					}
					word_390B8 = 0;
					//sub_10CE7(); //TODO: This backs up a heap of mission variables
					word_3ABE9 = 0;
					word_3ABEB = 0;

					Mission_Brief();
				}
			}

			//loc_10513
			sub_18908();
			
			map_Load_TileSet();
			map_Load_Spt();

			word_3AA4D = readLEWord( &mMap[0x54] ) << 4;
			word_3AA4F = readLEWord( &mMap[0x56] ) << 4;

			Squad_Member_Count();
			Squad_Member_Sort();
			sub_1142D();
			sub_115F7();
			graphicsBlkPtrsPrepare();

			Map_Overview_Prepare();
			Briefing_Wait();
			if (word_3B4F5 == -1) {

				//TODO
				//sub_10D24();
				
				word_390B8 = -1;
				word_390EA = -1;
				word_3A9B2 = -1;
				word_3901E = -1;

				Music_Unk( 0 );
				continue;
			}

			sub_12083();
			Sprite_SetDataPtrToBase( off_32C0C );
			
			map_Tiles_Draw();
			sub_12083();
			map_SetTileType();
			Mouse_Inputs_Get();
			sub_18D5E();
			Sprite_Handle_Loop();
			//seg000:05D1

			sub_124DB();
			word_39F06 = 0;
			sub_21C6F( );
			word_3AA1B = 1;
			word_3A067 = mSprites[0].field_0;
			word_3A069 = mSprites[0].field_4;

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
			stru_3AC53[0].field_0 = 0;
			word_390A6 = -1;
			sub_11CAD();
			word_3BEC1 = 0;
			word_3BEC3 = 4;
			word_3AA1B = 1;
			Squad_Prepare_GrenadesAndRockets();
			sub_2D7FF();
			sub_126DD();
//			nullsub_1();

			word_3BEC9 = 0xE0;
			mImage->paletteSet( mPalette );
			mImageFaded = -1;

			sub_2EACA();
			sub_2F0D7();
			sub_126BB();
			sub_13102();
			sub_13148();
			word_3A9B2 = 0;
			word_3A9D0 = 0;
			mMouseSpriteNew = 0x23;
			word_3B20F = -1;
			word_3B4F1 = 0;
			mMission_ShowMapOverview = 0;
			word_3EABD = 0x40BC;
			
			if (!Mission_Loop()) {
				word_3B20F = 0;
				sub_12A5F();
				word_390EC = -1;
			}
			else {
				word_3B20F = 0;
				if (!word_390CE) {
					sub_17DB3();
					goto loc_103BF;
				}
			}
		//loc_106F1
			if (word_3A9AA) {
				word_390EC = -1;
				continue;
			}

			if (word_3A9B2)
				continue;

			if (mMissionPhaseRemain > 1)
				continue;

			sub_17DB3();
		}
	}
}

void cFodder::graphicsBlkPtrsPrepare() {
	uint16 bx = 0, dx = 0;

	for (uint16 cx = 0; cx < 240; ++cx) {

		mGraphicBlkPtrs[cx + 0x00] = mDataBaseBlk + bx;
		mGraphicBlkPtrs[cx + 0xF0] = mDataSubBlk + bx;

		++dx;
		bx += 0x10;
		if (dx % 0x14 == 0) {
			dx = 0;
			bx += 0x12C0;
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

	map_Tiles_Draw_();
}

void cFodder::map_Tiles_Draw_() {

	uint8* Target = mImage->GetSurfaceBuffer();

	uint8* CurrentMapPtr = &mMap[mMapTilePtr];

	// Y
	for (uint16 cx = 0; cx < 0x0F; ++cx) {
		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = word_3B610;
		else
			StartY = 0;

		// X
		for (uint16 cx2 = 0; cx2 < 0x16; ++cx2) {
			uint8* TargetTmp = TargetRow;

			uint16 Tile = readLEWord( MapPtr ) & 0x1FF;
			if (Tile > 0x1E0)
				Tile = 0;

			uint8* TilePtr = mGraphicBlkPtrs[Tile];
			uint16 StartX = 0;

			TilePtr += StartY * 0x140;
			
			if (cx2 == 0)
				StartX = word_3B60E;
			else
				StartX = 0;

			// Each Tile Row
			for (uint16 i = StartX; i < 16; ++i) {

				memcpy( TargetTmp, TilePtr + StartX, 16 - StartX );
				TilePtr += 0x140;
				TargetTmp += mImage->GetWidth();
			}

			MapPtr += 2;
			TargetRow += (16-StartX);
		}

		Target += mImage->GetWidth() * (16-StartY);
		CurrentMapPtr += mMapWidth << 1;
	}

	mImage->Save();
}

void cFodder::Exit( unsigned int pExitCode ) {

}

void cFodder::Mission_Sidebar_Grenades_Draw( ) {
	int16 Data0 = word_3AC1F;

	if (!mSquad_Grenades[word_3AC1F])
		goto loc_2ED63;

	int16 Data14 = word_3AC1F;

	if (word_3AC3F[word_3AC1F] != 1)
		goto loc_2ED63;

	if (!word_3AC47) {
	loc_2ED63:;
		Data0 = 0xC4;
	}
	else {
		Data0 = 0xC3;
	}

	// Draw Grenade Icon
	int16 Data8 = 0;
	int16 DataC = word_3AC1D;
	DataC += 0x0E;

	sub_145AF( Data0, Data8, DataC );
	
	Data0 = mSquad_Grenades[word_3AC1F];
	if (!Data0)
		return;

	int16 Data4 = 0;
	Data8 = 0x0D;
	DataC = word_3AC1D;
	DataC += 0x19;
	int16 Data10 = 0xAF;
	Data14 = word_3AC1F;

	if (word_3AC3F[Data14] != 1)
		goto loc_2EDF7;

	if (word_3AC47)
		goto loc_2EDFD;

loc_2EDF7:;
	Data10 = 0xB9;

loc_2EDFD:;
	sub_302DE( Data0, Data4, Data8, DataC, Data10 );

}

void cFodder::GUI_Prepare_Button_Grenade() {
	if (!word_3AC47)
		return;

	struct_6* Data20 = dword_3AEF3;
	Data20->field_0 = &cFodder::sub_2EAC2;
	Data20->field_4 = 0;
	Data20->field_6 = 0x0C;
	Data20->field_8 =  word_3AC1D + 0x0E;
	Data20->field_A = 0x14;
	Data20->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_Grenades;

	++Data20;
	sub_2ECC7( Data20 );
}

void cFodder::GUI_Handle_Button_Grenades() {
	
	if (sub_30E0B() < 0)
		return;

	if (!sub_2F90B()) {
		sub_2F0D7();
		return;
	}

	word_3AC49 = word_3BDE7;
	++word_3BDE7;
	if (word_3BDE7 >= 3)
		word_3BDE7 = 0;

	sub_303AE();

}

void cFodder::sub_2F0D7() {

	if (word_3AC3F[mSquad_Selected] == 1)
		return;

	word_3AC3F[mSquad_Selected] = 1;
	byte_3AC33[mSquad_Selected] = -1;
	byte_3AC39[mSquad_Selected] = -1;
}

void cFodder::Mission_Sidebar_TroopList_Draw() {

	int16 Data0 = 0;
	int8 Data4 = mSquads_TroopCount[word_3AC1B];
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

		sub_145AF( Data0, 0, DataC );
		DataC += 0x0C;
	}
loc_2F1BC:;

	word_3AC29 = DataC;
	Data4 = Data4_Saved;
	if (Data4 < 0)
		return;

	sSquad_Member* Data38 = mSquad;

	DataC = word_3AC1D;
	DataC += 0x23;
	word_3A3BD = DataC;

	for (int16 word_3A061 = 7; word_3A061 >= 0; --word_3A061, ++Data38 ) {
		
		sSprite_0* Data34 = Data38->field_4;

		if (Data34 == INVALID_SPRITE_PTR || Data34 == 0)
			continue;

		if (word_3AC1B != Data34->field_32)
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

		sub_145AF( Data0, Data8, DataC );
		Data0 = Data38->mRank + 9;
		Data8 = 0x23;
		DataC = word_3A3BD;

		sub_145AF( Data0, Data8, DataC );
		Data0 = 0;

		sRecruit* Data28 = &mRecruits[Data38->mRecruitID];
		if (!word_3AC47) {
			Data0 = 2;
		}
		else {
			Data0 = 0;
			Data0 = Data38->field_9;
			Data0 &= 1;
		}
		//loc_2F318
		Data4 = 3;
		word_3A8B1 = 6;
		DataC = word_3A3BD;
		DataC += 2;

		Mission_Sidebar_TroopList_Name_Draw( Data0, Data4, Data8, DataC, Data28 );
		word_3A3BD += 0x0C;
	}
}

void cFodder::Mission_Sidebar_TroopList_Name_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, sRecruit* pData28 ) {

	const int8* Data20 = byte_3DF02;

	word_3AA21 =  Data20[pData0];

	int16 Data14;

	for (Data14 = 0; Data14 <= word_3BDEB; ++Data14) {
		if (word_3BDEB == 5) {

			if (pData28->mName[Data14] == 0x20)
				break;
		}

		if (pData28->mName[Data14] == 0)
			break;

	}

	Data14 <<= 2;

	int16 Data18 = 0x30;
	Data18 -= Data14;
	Data18 >>= 1;
	word_3A05F = Data18;

	for( Data14 = 0; Data14 <= word_3BDEB; ++Data14 ) {
			
		if( pData28->mName[Data14] != 0x20 ) {
			pData0 = pData28->mName[Data14];
			pData0 -= 0x41;
			pData0 += word_3AA21;
				
			pData8 = Data14;
			pData8 <<= 2;
			pData8 += word_3A05F;

			sub_145AF( pData0, pData8, pDataC );
		}
	}
}

void cFodder::sub_2F452() {

	sub_2F4CB();
	Mission_Sidebar_SquadIcon_Draw();
}

void cFodder::Mission_Sidebar_SquadIcon_Draw() {
	
	int16 Data0 = word_3AA11[word_3AC1F];

	const int16* Data20 = word_3DF05;

	if (!word_3AC47) 
		Data20 = word_3DF1B;

	Data0 = Data20[Data0];
	int16 Data8 = 0x0C;
	int16 DataC = word_3AC1D;
	DataC += 0x0E;

	sub_145AF( Data0, Data8, DataC );
}

int16 cFodder::sub_2F4CB() {
	int16 Data1C = 0;

	int16 Data8 = mSquads_TroopCount[ word_3AC1B ];
	if (!Data8)
		return 0;

	if (Data8 != 1)
		Data1C = 1;

	int16 DataC = word_3AC1F;

	if (!dword_3B24F[DataC])
		goto loc_2F593;

	sSprite_0* Data2C = dword_3B24F[DataC];
	DataC = Data2C->field_6F;
	const int16* Dataa2C = word_3DF31;

	for (;; Dataa2C += 2) {
		if (*Dataa2C < 0)
			goto loc_2F593;

		if (DataC == *Dataa2C)
			goto loc_2F586;
	}
loc_2F586:;
	Data1C = *(Dataa2C + 1);

loc_2F593:;
	if (Data1C == word_3AA11[word_3AC1F])
		return 0;

	word_3AA11[word_3AC1F] = Data1C;
	return -1;
}

void cFodder::sub_2F5ED() {
	//map_CheckFinal();
	sub_2F757();
	
	int16 Data0 = 2;
	int16 Data4 = 2;
	
	int8* Data20 = &byte_3AC33[2]; //byte_3AC35; 
	int8* Data24 = &byte_3AC39[2]; //byte_3AC3B;
	int16* Data28 = mSquad_Grenades;
	int16* Data2C = mSquad_Rockets;
	int16* Data30 = word_3AC3F;
	
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

	Data20 = &byte_3AC33[2];
	
	for (int16 Data0 = 2; Data0 >= 0; --Data0, --Data20) {

		if (!*Data20)
			goto loc_2F707;

		*Data20 = 0;

		sub_2F7E4( Data0 );

	loc_2F707:;
		--Data20;
		--Data0;

		if (Data0 < 0) {
			Data0 = 2;
			Data20 = &byte_3AC39[2];
		}

		if (!*Data20)
			continue;

		*Data20 = 0;

		sub_2F87E( Data0 );
	}
}

void cFodder::sub_2F757() {
	
	word_3AC1B = 0;
	word_3AC1F = 0;
	
	for (word_3AC1B = 0; word_3AC1B < 3; ++word_3AC1B, ++word_3AC1F) {

		word_3AC1D = word_3AC2D[word_3AC1F];

		if (!mSquads_TroopCount[word_3AC1D])
			continue;

		word_3AC47 = 0;

		if (word_3AC1F == mSquad_Selected)
			word_3AC47 = -1;

		if (sub_2F4CB())
			Mission_Sidebar_SquadIcon_Draw();
	}
}

void cFodder::sub_2F7D7( ) {
	sub_2F7E4( mSquad_Selected );
}

void cFodder::sub_2F7E4(  int16 pData0 ) {

	int16 Data4 = word_3AC2D[pData0];

	sub_2EBE0(pData0, Data4);
	Mission_Sidebar_Grenades_Draw();

	if (!mSquad_Grenades[word_3AC1F])
		return;

	if (!word_3AC47)
		return;

	if (!sub_2F90B())
		return;

	if (word_3BDE7 == 2)
		return;

	int16 Data0 = 0xC7;
	Data0 += word_3BDE7;

	int16 Data8 = 0;
	int16 DataC = word_3AC1D;
	DataC += 0x0E;

	sub_145AF( Data0, Data8, DataC );
}

void cFodder::sub_2F871( ) {

	sub_2F87E( mSquad_Selected );
}

void cFodder::sub_2F87E(  int16 pData0 ) {

	int16 Data4 = word_3AC2D[pData0];
	sub_2EBE0( pData0, Data4 );
	Mission_Sidebar_Rockets_Draw();

	if (!mSquad_Rockets[word_3AC1F])
		return;

	if (!word_3AC47)
		return;

	if (!sub_2F90B())
		return;

	if (word_3BDE9 == 2)
		return;

	pData0 = 0xC7;
	pData0 += word_3BDE9;

	int16 Data8 = 0x24;
	int16 DataC = word_3AC1D;
	DataC += 0x0E;

	sub_145AF( pData0, Data8, DataC );
}

int16 cFodder::sub_2F90B() {
	sSquad_Member* Data38 = mSquad;

	sSprite_0* Data34 = mSprites;
	int16 Data18 = mSquad_Selected;

	int16 Data10 = 0;

	for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Data38) {

		if (Data38->field_4 == INVALID_SPRITE_PTR || Data38->field_4 == 0 )
			continue;

		if (Data18 != Data38->field_4->field_32)
			continue;

		if (!(Data38->field_9 & 1))
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

void cFodder::sub_2F9B3() {
	
	int16 Data0 = word_3AC2B;
	int16 Data4 = Data0;

	Data4++;
	word_3AC2B = Data4;

	//TODO 
	switch (Data0) {
		case 0: //loc_2FA05
			sub_2FA05();
			break;

		case 1: //loc_2FA1D
			Mission_Sidebar_MapButton_Prepare();
			break;

		case 2: //loc_2FA44
			sub_2FA44();
			break;

		case 3: //loc_2FA5B
			sub_2FA5B();
			break;

		case 4: //loc_2FA75
			sub_2FA75();
			break;

		case 5: //loc_2FA8F
			sub_2FA8F();
			break;

		case 6:
			word_3AC2B = -1;
			return;

		default:
			std::cout << "sub_2F9B3(): Missing Function " << Data0;
			break;
	}
}

void cFodder::sub_2FA05() {
	word_3AC45 = -1;

	if (word_3AC51)
		return; 

	sub_30465();
	word_3AC51 = -1;
}

void cFodder::Mission_Sidebar_MapButton_Prepare() {

	dword_3AEF3 = stru_3AC53;

	map_ClearSpt();

	Mission_Sidebar_MapButton_RenderWrapper();
}

void cFodder::sub_2FA44( ) {
	word_3AC29 = 0;

	Mission_Sidebar_Prepare( 0, 0 );
}

void cFodder::sub_2FA5B( ) {
	int16 Data0 = 1;
	int16 Data4 = word_3AC29;
	if (Data4) 
		Data4 += 5;
	
	Mission_Sidebar_Prepare( Data0, Data4 );
}

void cFodder::sub_2FA75( ) {
	int16 Data0 = 2;
	int16 Data4 = word_3AC29;

	if (Data4)
		Data4 += 5;

	Mission_Sidebar_Prepare( Data0, Data4 );
}

void cFodder::sub_2FA8F() {
	sub_2EBC4();
	sub_30480();

	word_3AC51 = 0;
	word_3AC45 = 0;
	word_39EFC = 0;
}

void cFodder::Mission_Sidebar_Rockets_Draw( ) {
	
	if (!mSquad_Rockets[word_3AC1F])
		goto loc_2FAF6;

	if (word_3AC3F[word_3AC1F] != 3)
		goto loc_2FAF6;

	int16 Data0;

	if (!word_3AC47) {
		loc_2FAF6:;
		Data0 = 0xC6;
	}
	else {
		Data0 = 0xC5;
	}

	//loc_2FB04
	int16 Data8 = 0x20;
	int16 DataC = word_3AC1D;
	DataC += 0x0E;
	sub_145AF( Data0, Data8, DataC );

	Data0 = mSquad_Rockets[word_3AC1F];
	if (!Data0)
		return;

	int16 Data4 = 0x24;
	Data8 = 0x0D;
	DataC = word_3AC1D;
	DataC += 0x19;
	int16 Data14 = word_3AC1F;

	if (word_3AC3F[Data14] != 3)
		goto loc_2FB8A;

	int16 Data10 = 0xAF;
	if (!word_3AC47) {
	loc_2FB8A:;
		Data10 = 0xB8;
	}

	sub_302DE( Data0, Data4, Data8, DataC, Data10 );
}

void cFodder::GUI_Prepare_Button_Rockets() {

	if (!word_3AC47)
		return;

	struct_6* Data20 = dword_3AEF3;
	Data20->field_0 = &cFodder::sub_2EAC2;
	Data20->field_4 = 0x24;
	Data20->field_6 = 0x0C;
	Data20->field_8 = word_3AC1D + 0x0E;
	Data20->field_A = 0x14;
	Data20->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_Rockets;
	++Data20;

	sub_2ECC7( Data20 );
}

void cFodder::GUI_Handle_Button_Rockets() {

	if (sub_30E0B() < 0)
		return;

	if (!sub_2F90B()) {
		sub_2FC4F();
		return;
	}

	word_3AC4F = word_3BDE9;
	++word_3BDE9;

	if (word_3BDE9 >= 3)
		word_3BDE9 = 0;

	sub_304CB();
}

void cFodder::sub_2FC4F() {
	
	if (word_3AC3F[mSquad_Selected] == 3)
		return;

	word_3AC3F[mSquad_Selected] = 3;
	byte_3AC39[mSquad_Selected] = -1;
	byte_3AC33[mSquad_Selected] = -1;
}

void cFodder::GUI_Prepare_Button_TroopName() {
	
	if (mSquad_Selected != word_3AC1B)
		return;

	for (int16 Data4 = 2; Data4 >= 0; --Data4) {
		if (!mSquads_TroopCount[Data4])
			goto loc_2FCF4;
	}

	return;

loc_2FCF4:;
	word_3AEFB = word_3AC1D;
	struct_6* Data20 = dword_3AEF3;

	Data20->field_0 = &cFodder::sub_2EAC2;
	Data20->field_4 = 0;
	Data20->field_6 = 0x2F;
	Data20->field_8 = word_3AC1D + 0x22;

	Data20->field_A = mSquads_TroopCount[mSquad_Selected] * 0x0C;
	Data20->mMouseInsideFuncPtr = &cFodder::GUI_Handle_Button_TroopName;

	++Data20;
	sub_2ECC7( Data20 );
}

void cFodder::GUI_Handle_Button_TroopName() {

	if (sub_30E0B() < 0) {
		if (sub_30E2A() < 0)
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

		sSprite_0* Data34 = Data38->field_4;

		if (Data34 == INVALID_SPRITE_PTR || Data34 == 0 )
			continue;

		if (Data4 != Data34->field_32)
			continue;

		--Data8;
		if (Data8 < 0)
			break;
	}

	Data38->field_9 ^= 1;

	sRecruit* Data28 = &mRecruits[Data38->mRecruitID];
	Data0 = Data38->field_9 & 1;
	Data4 = 3;
	Data8 = word_3A3BF;
	Data8 *= 0x0C;

	Data8 += word_3AEFB;
	Data8 += 0x25;
	word_3A8B1 = 6;
	int16 DataC = Data8;

	Mission_Sidebar_TroopList_Name_Draw( Data0, Data4, Data8, DataC, Data28 );
	sub_3049B();
	sub_3037A();
}

int16 cFodder::sub_2FF41() {
	
	if (!sub_2F90B())
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

	struct_8* Dataa34 = off_3BEF3[Data10];
	Dataa34->asInt = -1;

	sSquad_Member* Data38 = mSquad;
	

	for (int16 Data1C = 7; Data1C >= 0; --Data1C, ++Data38 ) {
		sSprite_0* Data34 = Data38->field_4;

		if (Data34 == INVALID_SPRITE_PTR || Data34 == 0)
			continue;

		if (Data18 != Data34->field_32)
			continue;

		if (!(Data38->field_9 & 1))
			continue;

		Data34->field_32 = Data14;
		Data34->field_40 = 0;
		Data34->field_42 = 0;
		Data34->field_44 = 0;
		Data34->field_28 += 4;
	}

	mSquad_Selected = Data14;
	sub_301F7();

	return -1;
}

void cFodder::sub_30082() {
	int16* Data20 = mSquad_Grenades;

	int16 Data0 = word_39FD2;
	int16 Data4 = mSquad_Selected;

	if (!word_3BDE7)
		goto loc_300F9;

	if (word_3BDE7 != 1)
		goto loc_3011E;

	int16 Data8 = Data20[Data0];
	int16 DataC = Data8;
	Data8 >>= 1;

	Data20[Data0] = Data8;
	DataC -= Data8;
	Data20[Data4] = DataC;
	goto loc_3011E;

loc_300F9:;

	Data20[Data4] = Data20[Data0];
	Data20[Data0] = 0;

loc_3011E:;

	Data20 = mSquad_Rockets;
	Data0 = word_39FD2;
	Data4 = mSquad_Selected;

	if (!word_3BDE9)
		goto loc_30195;

	if (word_3BDE9 != 1)
		goto loc_301BA;

	Data8 = Data20[Data0];
	DataC = Data8;
	Data8 >>= 1;

	Data20[Data0] = Data8;
	DataC -= Data8;
	Data20[Data4] = DataC;

	goto loc_301BA;

loc_30195:;
	Data20[Data4] = Data20[Data0];
	Data20[Data0] = 0;

loc_301BA:;
	Data20 = word_3AC3F;
	Data0 = word_39FD2;
	Data4 = mSquad_Selected;

	Data20[Data4] = Data20[Data0];
}

void cFodder::Mission_Sidebar_MapButton_RenderWrapper() {

	if (mMapNumber == 0x47)
		return;

	word_3B4D9 = 0;
	Mission_Sidebar_MapButton_Render();
}

void cFodder::sub_302DE( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, int16 pData10 ) {
	pData10 -= 0x1A;
	int16 Data10_Saved = pData10;

	int16 word_3AC23 = pData4;
	int16 word_3AC25 = pData8;
	int16 word_3AC27 = pDataC;

	std::stringstream Tmp;
	Tmp << tool_StripLeadingZero( tool_NumToString( pData0 ) );

	String_CalculateWidth( pData8, byte_3DF85, Tmp.str().c_str() );
	
	pData8 = word_3B301 + word_3AC23;
	int16 Data0 = Data10_Saved;
	word_3AC21 = -1;
	pDataC = word_3AC27;

	String_Print(  byte_3DF85, Data0, pData8, pDataC, Tmp.str().c_str() );
	
	word_3AC21 = 0;
}

int16 cFodder::sub_30E0B() {
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

		if ( Data24->field_4 == INVALID_SPRITE_PTR || Data24->field_4 == 0 )
			continue;

		sSprite_0* Data20 = Data24->field_4;
		int16 Data10 = Data20->field_32;

		if (Data10 == mSquad_Selected)
			++word_3A3BF;

		Data10 = Data20->field_0;
		Data10 += 4;
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

		Data0 = mSquads_TroopCount[Data10];
		Data0 += mSquads_TroopCount[Data14];
		if (Data0 > 8)
			return;

		word_3BF1E[Data14] = Data20;
		//seg011:251C
		//TODO: Check whats going on here

		byte_3BF1B[Data14] = Data10;
		Data0 = Data10;

		sub_2DCB0( Data0 );

		break;
	}
}

void cFodder::sub_301F7() {
	sSquad_Member* Data38 = mSquad;
	
	for (int16 Data1c = 7; Data1c >= 0; --Data1c, ++Data38) {

		Data38->field_9 &= 0;

	}
}

void cFodder::sub_303AE() {
	sub_2F7D7();
	sub_303DA();
}

void cFodder::sub_303DA() {
	int8* Data20 = mSquads_TroopCount;
	int16 Data0;

	for (Data0 = 0; Data0 < 3; ++Data0) {
		if (!Data20[Data0])
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

	sub_145AF( Data0, Data8, DataC );
}

void cFodder::sub_30465() {

	for (int16 cx = 0; cx < 0x12C0; ++cx){
		word_3BDAD[cx] = mMapSptPtr[cx];
	}

	word_3D5B7 = word_3BDAD;
}

void cFodder::sub_30480() {

	for (int16 cx = 0; cx < 0x12C0; ++cx){
		mMapSptPtr[cx] = word_3BDAD[cx];
	}

	word_3D5B7 = mMapSptPtr;
}

void cFodder::sub_304D0() {
	
	if (!word_390A6 || word_3B4F1)
		return;

	if (word_3AC3F[mSquad_Selected] != 3) {
		if (mSquad_Rockets[mSquad_Selected])
			sub_2FC4F();
	}
	else {
		if (mSquad_Grenades[mSquad_Selected])
			sub_2F0D7();
	}
}

int16 cFodder::sub_305D5( sSprite_0*& pData20 ) {
	
	int16 Data0 = mSquad_Selected;

	if (Data0 < 0)
		goto loc_306AD;

	sSprite_0** Data30 = off_3BDEF[Data0];
	if (*Data30 == INVALID_SPRITE_PTR || *Data30 == 0)
		goto loc_306AD;

	do {
		if (*Data30 ==  INVALID_SPRITE_PTR || *Data30 == 0 )
			goto loc_306AD;

		pData20 = *Data30;
		++Data30;
		//seg011:1B61
		if (pData20->field_18)
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

	} while (*Data30 >= 0);
	//loc_3069A
	goto loc_306BE;

loc_306AD:;
	pData20 = INVALID_SPRITE_PTR;
	mSquad_Selected = -1;

loc_306BE:;
	if (dword_3B20B)
		return 1;

	word_39FCE = pData20;
	if (pData20 == INVALID_SPRITE_PTR || pData20 == 0 )
		return -1;

	return 1;
}

void cFodder::Mouse_Inputs_Check() {
	int16 Data0 = 0;
	int16 Data4 = 0;

	if (word_3AC45)
		return;

	if (word_3B20F)
		sub_30AB0();

	if (dword_3A030) {
		// TODO: Function pointer call, but appears not to be used
		//dword_3A030();
		return;
	}

	dword_3A02A = stru_3AC53;

	for (;;++dword_3A02A) {
		struct_6* Data20 = dword_3A02A;

		if (Data20->field_0 == 0)
			break;

		if ( (*this.*Data20->field_0)() < 0 )
			return;

		Data20 = dword_3A02A;
		Data0 = word_3BEC1 + Data20->field_4;

		int16 Data4 = mMouseX;
		Data4 += 0x20;

		if (Data0 > Data4)
			continue;

		Data0 += Data20->field_6;
		if (Data0 < Data4)
			continue;

		Data0 = word_3BEC3;
		Data0 += Data20->field_8;
		if (Data0 > mMouseY)
			continue;

		Data0 += Data20->field_A;
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
	if (!dword_3B20B)
		return;

loc_30814:;

	if (dword_3B20B) {
		sub_310CB();
		return;
	}

	if (sub_30E0B() < 0)
		return;

	if (word_3AA43)
		return;

	if (mMouseSpriteNew < 0) {
		mMouseSpriteNew = 0x23;
		word_3A9F3 = 0;
		word_3A9F5 = 0;
	}

	// segra: I've moved this as it looks like a bug
	if (mSquad_Selected < 0) {

		sub_30E49();
		return;
	}

	sSprite_0** Data24 = off_3BDEF[mSquad_Selected];
	sSprite_0* Dataa24 = *Data24;

	if (Dataa24 == INVALID_SPRITE_PTR) {
		sub_30E49();
		return;
	}

	if (Dataa24->field_6E)
		return;

	if (word_3B2F1) {
		word_3B2F1 = 0;
	}
	else {
		sub_311A7();
	}

	word_3ABC7 = 0;
	sub_30E49();

	if (!word_3ABC7)
		goto loc_308F6;

	int8* Data20 = mSquads_TroopCount;
	int8 al;

	//TODO 
	// Why is this here??
	for (Data0 = 2; Data0 >= 0; --Data0) {
		al = *Data20;
		if (!al)
			return;
	}
	return;

loc_308F6:;
	sSprite_0* Dataa20 = word_39FCE;

	if (Dataa20 == INVALID_SPRITE_PTR || Dataa20 == 0 )
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

	dword_3A3F9 = off_3BEF3[mSquad_Selected];

	sSquad_Member* SquadMember = mSquad;

	int8 Data14 = 0;

	for (int16 Data18 = 7; Data18 >= 0; --Data18, ++SquadMember) {

		if (SquadMember->field_4 == INVALID_SPRITE_PTR || SquadMember->field_4 == 0 )
			continue;
		
		sSprite_0* tmp = SquadMember->field_4;
		if (mSquad_Selected != tmp->field_32)
			continue;

		tmp->field_44 = Data14;
		tmp->field_2 = 0;
		tmp->field_6 = 0;
	}

	sSprite_0* Saved_Data20 = Dataa20;

	int16 Data10 = Dataa20->field_0;
	int16 DataC = Dataa20->field_32;
	int16 Data8 = Data4;
	Data4 = Data0;

	sub_2A932( Data4, Data8, DataC, Data10 );
	Dataa20 = Saved_Data20;

	for (Data0 = 2; Data0 >= 0; --Data0) {

		if (mSquad_Selected != byte_3BF1B[Data0])
			continue;

		byte_3BF1B[Data0] = -1;
	}
}

void cFodder::Squad_Member_Target_Set() {
	
	if (dword_3B20B) {
		sub_3169B();
		return;
	}

	word_3A9CE = -1;
	if (word_3A9B8 < 0) {
		word_3A9B8 = 0x30;

		if (word_39FCE)
			word_39FCE->field_4A = 0;
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

	int16 Data14 = mSquads_TroopCount[mSquad_Selected];
	--Data14;
	if (Data14 < 0)
		goto loc_30E05;

	sSprite_0** Data20 = off_3BDEF[mSquad_Selected];

	for (;;) {
		if (*Data20 == INVALID_SPRITE_PTR || *Data20 == 0)
			break;

		sSprite_0* Data24 = *Data20++;

		Data24->field_2E = TargetX;
		Data24->field_30 = TargetY;
	}

loc_30E05:;
	Squad_Member_Rotate_Can_Fire();
}

int16 cFodder::sub_30E2A() {

	if (word_39EFA < 0) {
		word_39EFA = 1;
		return 1;
	}

	return -1;
}

void cFodder::sub_31033() {

	if (!word_3AA47)
		return;

	--word_3AA47;
	if (word_3AA47)
		return;

	if (mSquad_Selected >= 0)
		return;

	for (int16 Data0 = 2; Data0 >= 0; --Data0 ) {

		if (mSquads_TroopCount[Data0]) {
			sub_31075(Data0);
			return;
		}

	}
}

void cFodder::sub_31075( int16 pData0 ) {
	
	if (!mSquads_TroopCount[pData0])
		return;

	mSquad_Selected = pData0;
	sSprite_0* Data20 = 0;

	if (sub_305D5( Data20 ) >= 0) {
		word_3A067 = mSquads_TroopCount[0];
		word_3A069 = mSquads_TroopCount[4];
		word_3AA1B = 1;
		word_3A054 = 0;
	}

	word_3AC2B = 0;
}
