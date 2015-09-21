/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield <robcrossfield@gmail.com>
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


#include "Sprites.hpp"

struct sMouseData {
	uint16	anonymous_0;
	uint16	anonymous_1;
	uint16	anonymous_2;
	uint16	mColumn;
	uint16	mRow;
	uint16	anonymous_5;
	uint16	anonymous_6;
	uint16	anonymous_7;
	uint16	anonymous_8;
	uint16	anonymous_9;
	uint16	anonymous_10;
};

struct sSquad_Member {
	int16		mRecruitID;
	uint8		mRank;
	int8		field_3;
	sSprite_0*	field_4;
	uint16		field_6;
	int8		field_8;
	int8		field_9;
	uint16		mNumberOfKills; 
};

struct sSpriteSheet {
	uint16 field_0;
	uint16 field_2;
	int16 field_4;
	int16 field_6;
	int16 mColCount;
	int16 mRowCount;
	int16 field_C;
	int8 field_E;
	int8 field_F;
};

struct sHero {
	int8	mRecruitID;
	int16	mRank;
	int16	mKills;	
};

struct struct_2 {
	int16	field_0;
	int16	field_2;
	int16	field_4;
	int16	field_6;
};

struct struct_3 {
	int16	field_0;
	int16	field_2;
	int16	field_4;
	int16	field_6;
	int16*	field_8;
};

struct struct_4 {
	int16	field_0;
	int16	field_2;
	int16	field_4;
};

struct struct_5 {
	int16	field_0;
	int16	field_2;
	int16	field_4;
	int16	field_6;
	int16	field_8;
};

class cFodder;

struct struct_6 {
	int16	(cFodder::*field_0)();
	int16	field_4;
	int16	field_6;
	int16	field_8;
	int16	field_A;
	void	(cFodder::*mMouseInsideFuncPtr)();	// field_c
};

struct struct_7 {
	int16	field_0;
	int16	field_2;
};

union struct_8 {
	int32 asInt;

	struct {
		int16	field_0;
		int16	field_2;
	};
};

extern const struct_2 stru_44B50[209];

class cFodder : public cSingleton < cFodder > {
	public:
	std::vector<const sVersion*> mVersions;

	bool					mSkipIntro;
	Mix_Music*				mMusicPlaying;
	std::vector<cEvent>		mEvents;
	
	const sVersion*			mVersion;

	cGraphics*				mGraphics;
	cResources*				mResources;
	cWindow*				mWindow;

	sSprite_0				word_3B4F7;
	sSprite_0				mSprites[45];

	const sSpriteSheet**	mSpriteDataPtr;
	
	std::stringstream		mTitle;

	std::string		mFilenameCopt;
	std::string		mFilenameBaseSwp;
	std::string		mFilenameBaseHit;
	std::string		mFilenameBaseBht;
	std::string		mFilenameArmy;
	std::string		mFilenameSubSwp;
	std::string		mFilenameSubHit;
	std::string		mFilenameSubBht;
	std::string		mFilenameBasePal;

	cSurface*		mSurfaceMapOverview;
	cSurface*		mImage;

	cPosition		mMousePosition;
	uint32			mMouseButtons;

	uint16			mMapWidth;
	uint16			mMapHeight;
	uint8			mKeyCode;

	uint8*			mDataPStuff;
	uint8* 			mDataHillBits;
	uint8*			mDataArmy;
	uint8*			mMap;
	size_t			mMapSize;
	size_t			mDataBlkSize;
	uint8*			mDataBaseBlk;
	uint8*			mDataSubBlk;
	int32			mDataBaseBlkSize;
	int32			mDataSubBlkSize;

	int32*			dword_37AC0;
	uint32			dword_3901A;
	uint16			word_3901E;
	int16			word_39096;
	int16			word_3909A;
	uint16			word_390A4;
	int16			word_390A6;
	uint16			word_390AE;
	int16			word_390B0;
	uint16			word_390B8;
	int16			word_390BE;
	int16			word_390C0;
	int16			word_390C2;
	int16			word_390C4;
	int16			word_390C6;
	uint16			mMapNumber;
	uint16			mMissionNumber = 0;
	uint16			mMissionPhase = 0;
	uint16			word_390CE;
	uint16			word_390D0;
	uint16			word_390D2;
	int16			word_390D4;
	sSprite_0*		word_390D6[8];
	int16			word_390E6;
	uint16			word_390E8;
	int16			word_390EA;
	int16			word_390EC;
	uint16			mMissionPhaseRemain;
	uint16			mMissionPhases;
	uint16			word_390F4;
	int16			word_390F8;
	sSquad_Member	mSquad[9];

	int16			word_391D2[361];
	int16*			dword_394A4;
	int16*			dword_394A8;
	int16			word_394AC[361];
	int16*			dword_3977E;

	sHero			mHeroes[6];

	int16			word_397AC;
	int16			word_397AE;
	int16			mMapPlayerTroopCount;
	int16			mSquadMemberCount;
	int16			mTroopsAvailable;
	
	int16			mButtonPressLeft, mButtonPressRight;
	int16			mMouse_Button_Left_Toggle;
	int16			word_39F00;
	int16			word_39F02;
	int16			word_39F06;
	int16			word_39FA0;
	int16			word_39FA2;
	int16			word_39FA4;
	int16			word_39FA6;
	int16			mCamera_Column;
	int16			mCamera_Row;
	int16			word_39FAC;

	int16			word_39FAE;
	int16			word_39FBA;

	int16			word_39FBC;
	int16			word_39EFA;
	int16			word_39EFC;
	int16			word_39F04;
	int32			dword_39F18;
	int32			dword_39F1C;
	int32			dword_39F20;
	int32			dword_39F24;
	int32			dword_39F28;
	int32			mCamera_Adjust_Col;
	int32			mCamera_Adjust_Row;
	int32			dword_39F36;
	int16			word_39F3A;
	int16			word_39F3C;
	int16			word_39F3E;

	int32			dword_39F84;		// X : Unknown
	int32			dword_39F88;		// Y 
	int32			dword_39F8C;
	int32			dword_39F90;

	int16			word_39F34;
	int16			word_39F38;
	int16			word_39F40;
	int32			dword_39F4A;
	int32			dword_39F4E;
	int16			word_39F52;
	int16			word_39F54;
	int32			dword_39F42;
	int32			dword_39F46;
	int32			dword_39F56;
	int32			dword_39F5A;
	int16			word_39F5E;
	int16			word_39F60;
	int32			dword_39F7C;

	int16			word_39F94;
	int16			word_39F96;
	int16			dword_39F98;
	int16			word_39F9C;
	int16			word_39F9E;
	int16			word_39FB2;
	int16			word_39FB4;
	int16			word_39FB6;
	int16			word_39FB8;
	sSprite_0*		word_39FCE;

	int16			mSquad_Selected;
	int16			word_39FD2;
	int16			word_39FD4;

	int16			word_39FF2;
	int16			word_39FF4;
	int16			word_39FF6;
	int16			word_39FF8;
	int16			word_39FFA;

	int16			dword_3A000;
	int16			dword_3A004;
	int16			dword_3A008;
	int16			word_3A00C;
	int16			word_3A00E;
	int16			word_3A010;
	int16			word_3A014;
	int16			word_3A016;
	int16			word_3A01A;
	uint16			word_3A024;
	struct_6*		dword_3A02A;
	int32			dword_3A030;
	uint16			word_3A054;
	int8			mSquads_TroopCount[4];
	uint8			byte_3A05E;
	uint16			word_3A05F;
	int16			mCamera_Position_Column;
	int16			mCamera_Position_Row;
	int16			word_3A067;
	int16			word_3A069;
	int16			word_3A06B;
	sSprite_0*		dword_3A071[45];
	sSprite_0*		dword_3A125[45];
	sSprite_0*		dword_3A1D9[45];
	int16			word_3A28D;

	int16			word_3A8B1;
	int16			word_3A8D5;
	int16			word_3A8D7;
	int16			word_3A8D9;
	int32			dword_3A8DB;
	uint8			byte_3A8DE[200];

	sSprite_0*		dword_3A291[64];
	int32			dword_3A391;
	uint32			dword_3A395;
	int16			word_3A399;
	uint32			dword_3A39D;
	int16			word_3A3AB;
	int16			word_3A3AD;
	int16			word_3A3AF;
	int16			word_3A3A1;
	int16			word_3A3A7;
	int16			word_3A3A9;
	int16			word_3A3B9;
	int16			word_3A3BB;
	int16			word_3A3BD;
	int16			word_3A3BF;
	struct_8*		dword_3A3F9;
	struct_8		stru_3A3FD[30];
	struct_8		stru_3A475[30];
	struct_8		stru_3A4ED[30];
	struct_8		stru_3A565[30];
	struct_8		stru_3A5DD[30];
	struct_8		stru_3A655[30];
	struct_8		stru_3A6CD[30];
	struct_8		stru_3A745[30];
	struct_8		stru_3A7BD[30];
	struct_8		stru_3A835[30];
	int16			word_3A8CF;

	int16			word_3A9A6[2];
	int16			word_3A9AA;
	int16			mMissionComplete;
	int16			word_3A9AE;
	int16			word_3A9B0;
	int16			word_3A9B2;
	int16			word_3A9B4;
	int16			word_3A9B8;
	int16			word_3A9BA[3];	// values here, seem to be a byte count originally.. now its an index
	int16			word_3A9C0[3];
	int16			word_3A9C6;
	sSprite_0**		dword_3A9C8;
	int16			word_3A9CE;
	int16			word_3A9D0;
	int8			byte_3A9D2[4];
	int8			byte_3A9D6[4];
	int8			byte_3A9DA[10];
	int16			word_3A9E4;
	int16			word_3A9E6;
	int16			word_3A9F3;
	int16			word_3A9F5;
	int16			mMouseSpriteNew;
	int16			word_3A9FB;
	int32			dword_3A9FD;
	int16			word_3AA01;
	int16			word_3AA03;
	int16			mSquad_Grenades[3];
	int16			mSquad_Rockets[3];
	int16			word_3AA11[3];
	int16			mTroops_Enemy_Count;
	int16			word_3AA19;
	int16			word_3AA1B;
	int16			word_3AA1D;
	int16			word_3AA1F;
	int16			word_3AA21;
	int16			word_3AA41;
	int16			word_3AA45;
	int16			word_3AA47;
	int16			word_3AA4B;
	int16			word_3AA4D;
	int16			word_3AA4F;
	int16			word_3AA51;
	int16			word_3AA55;
	int16			word_3AA67;
	int16			word_3AA71;
	int16			word_3AAC7;
	uint8*			dword_3AAC9;
	int16			word_3AACD;
	int16			word_3AACF;
	int16			word_3AAD1;

	int16			word_3AA43;
	int16			word_3AB39;
	int16			word_3ABA7;
	int8			byte_3ABA9;
	int16			word_3ABAD;
	int16			word_3ABAF;
	int16			word_3ABB1;
	int16			word_3ABB3;
	int16			word_3ABB5;
	int16			word_3ABB7;
	struct_5		stru_3ABB9;
	int16			word_3ABC3;
	int16			word_3ABC5;
	int16			word_3ABC7;
	sSquad_Member*	dword_3ABC9;
	int16			word_3ABE7;
	int16			word_3ABE9;
	int16			word_3ABEB;
	int16			word_3ABFB;
	int16			word_3ABFD;
	int16			word_3ABFF[9];
	int16*			dword_3AC11;
	int16			word_3AC19;
	int16			word_3AC1B;
	int16			word_3AC1D;
	int16			word_3AC1F;
	int16			word_3AC2D[3];
	int16			word_3AC21;
	int16			word_3AC29;
	int16			word_3AC2B;
	int8			byte_3AC33[3];
	int8			byte_3AC39[3];
	int16			word_3AC3F[3];
	int16			word_3AC45;
	int16			word_3AC47;
	int16			word_3AC49;
	int16			word_3AC4B;
	int16			word_3AC4D;
	int16			word_3AC4F;
	int16			word_3AC51;
	struct_6		stru_3AC53[42]; //Correct?
	struct_6*		dword_3AEF3;
	int16			word_3AEFB;
	int16			word_3AF01;
	int16			word_3AF03;
	int16			word_3AF05;
	int16			word_3AF07;
	struct_7		stru_3AF0B[128];

	struct_7*		dword_3B11B;
	struct_7*		dword_3B11F;
	int16			word_3B15D;
	int16			word_3B15F;
	int16			word_3B161;
	int16			word_3B163;
	int16			word_3B173;
	int16			word_3B175;
	int16			word_3B177;
	int16			word_3B179;
	int16			word_3B17B;
	int16			word_3B17D;
	int16			word_3B17F;
	int16			word_3B189;
	int16			word_3B18B;
	int16			word_3B1A9;
	int16			word_3B1AB;
	int32			dword_3B1CB;
	int16			word_3B1CF[0x0F];
	int16			word_3B1ED;
	int16			word_3B1EF;
	int16			word_3B1F1;

	const struct_4*		dword_3B1FB;
	
	int16			mMap_TileSet;
	sSprite_0*		dword_3B20B;
	int16			word_3B20F;
	int16			word_3B211;
	sSprite_0*		dword_3B213[15];
	sSprite_0*		dword_3B24F[3];
	int16			word_3B25B;
	int16			word_3B25D;
	int16			word_3B2CB;
	int16			word_3B2CD;
	int16			word_3B2CF;
	int16			word_3B2D1[6];
	int16			word_3B2DD[2];
	int16			word_3B2E1;
	int16			word_3B2E3;
	int16			word_3B2E5;
	int16			word_3B2ED;
	int16			word_3B2EF;
	int16			word_3B2F1;
	int16			word_3B2F3;
	int16			word_3B2F5;
	int16			word_3B2F7;
	int16			word_3B2FD;
	int16			word_3B2FF;
	int16			word_3B301;
	int16			word_3B303;

	int16			word_3B305;
	int16			word_3B307;
	
	int16			mMapGoals[8];
	int16			word_3B447;
	int16			word_3B449;
	int16			word_3B44F;

	int16			word_3B461[3];
	sSprite_0*		dword_3B477;
	int16			word_3B47B;
	int16			word_3B47D;
	int16			word_3B47F;
	int16			word_3B481;
	int16			word_3B483;
	int16			word_3B487;
	int16			word_3B489;
	sSprite_0*		dword_3B48B[16];
	int16			word_3B4CB;
	int16			word_3B4CD;
	sSprite_0*		dword_3B4CF;
	int16			word_3B4D3;
	int16			word_3B4D5;
	int16			word_3B4D7;
	int16			word_3B4D9;
	int16			mMission_ShowMapOverview;
	int16			word_3B4DD;
	int16			word_3B4DF;
	int16			word_3B4E9;
	int16			word_3B4EB;
	int16			word_3B4ED[2];
	int16			word_3B4F1;
	int16			word_3B4F3;
	int16			word_3B4F5;

	sSprite_0*		dword_3B5F5;
	int16			mMapTilePtr;
	int16			word_3B60E;
	int16			word_3B610;
	int16			word_3B612;
	int16			word_3B614;
	int16			mCamera_Pan_RowCount;	// Number of rows to move the camera

	uint16*			mMapSptPtr;
	size_t			mMapSptSize;

	uint16*			word_3BDAD;
	int16			mMouseX;
	int16			mMouseY;
	uint16			word_3BDB3;
	uint16			word_3BDB5;
	

	uint16			word_3BDE7;
	uint16			word_3BDE9;
	uint16			word_3BDEB;
	
	sSprite_0*		dword_3BE03[9];
	sSprite_0*		dword_3BE27[9];
	sSprite_0*		dword_3BE4B[9];
	sSprite_0*		dword_3BE6F[9];
	sSprite_0*		dword_3BE93[9];
	
	sSprite_0**		off_3BDEF[5];
	int16			word_3BEB9;
	int16			word_3BEBB;
	int16			word_3BEBD;
	int16			word_3BEC1;
	int16			word_3BEC3;
	uint16			word_3BEC9;
	uint16			word_3BED5[5];
	uint16			word_3BEDF[10];
	
	struct_8*		off_3BEF3[10];
	int8			byte_3BF1B[3];
	sSprite_0*		word_3BF1E[6];

	uint16			mIntroDone;

	int16			word_3C09D[240];
	int16			graphicsSub0[160];
	int16			graphicsBaseBht[960];
	int16			graphicsSub0Bht[640];
	int16			word_3D03D[240];
	int16			word_3D21D[161];

	int16			word_3D465;
	int16			word_3D467;
	int16			word_3D469;
	int16			mCamera_Column_Previous;
	int16			mCamera_Row_Previous;
	uint16*			word_3D5B7;

	uint16			byte_3DDA2[0x30];
	struct_6		stru_3DEDC[2];
	int16			word_3E0E5[0x18];

	uint8*			word_3E1B7;
	size_t			word_3E1B7_size;
	
	int16			word_3E75B;

		/* Configuration Settings (SETUP.DAT) */
	uint16			word_3E7ED;
	uint16			mEffectDriver;
	uint16			mMusicDriver;
	/* End Configuration Settings */

	int16			word_3EABD;

	int32			dword_3F946;
	int32			dword_3F94A;
	int16			word_3F94E;
	int16			word_3F950;

	int32			dword_3E9A3[70];
	int16			word_3FA1F;
	int16			word_3FA21;
	int16			mPaused;
	int16			mKeyControlPressed;

	int16			mImageFaded;
	int16			mMouseSpriteCurrent;
	int16			word_40048;
	uint16			word_40054;
	uint16			word_40056;
	uint16			word_40058;
	uint16			word_4005A;
	uint16			word_4005C;
	uint8*			word_42062;
	uint8*			word_42066;
	int16			mDrawSpritePositionX;
	int16			mDrawSpritePositionY;
	int16			word_4206C;
	int16			word_4206E;
	uint8			byte_42070;
	uint8			byte_42071;
	uint16			word_42074;
	uint16			word_42076;
	uint16			word_42078;

	struct sVocLoaded {
		uint8*		mBuffer;
		size_t		mSize;
	};
	struct sVocPlaying {
		int32		mChannel;
		Mix_Chunk*	mCurrentChunk;
	};
	std::vector<sVocPlaying> mMixerChunks;

	sVocLoaded*		word_42316[7];
	sVocLoaded		dword_42320[0x3C];
	sVocLoaded		dword_42410[0x3C];
	sVocLoaded		dword_42500[0x3C];
	sVocLoaded		dword_425F0[0x3C];
	sVocLoaded		dword_426E0[0x3C];

	int16			word_42851;
	uint16			word_42859;
	uint16			word_4285B;
	uint8*			word_4285D;
	int16			word_4285F;

	uint8*			word_42861;
	uint8*			word_42863;
	uint8*			word_42865;
	int16			word_428B6;
	uint8*			word_42867;
	int16			word_428B8;
	uint8*			word_42869;
	uint16			word_428BA;
	uint16*			word_4286B;

	uint16			word_4286D;
	uint32			mHelicopterPosX;
	uint32			mHelicopterPosY;
	int16			word_428C6;
	int16			word_428C4;
	int16			word_428C8;
	const int16*	word_428CA;
	uint16			word_428CC;
	int16			word_428D6;
	int16			word_428D8;

	uint8			mErrorCode;
	int16			mKeyNumberPressed;

	int16			mouse_Button_Status;
	int16			mouse_Pos_Column;
	int16			mouse_Pos_Row;

	sMouseData*		mouseData0;
	sMouseData*		mouseData1;

	int16			word_42072;
	uint16			word_427D4;
	int8			byte_427E6;
	int8			byte_427EE;
	int16			word_44475;
	int16			word_44A20;
	int16			word_44A2E;
	int16			word_44A30;
	int16			word_44A32;
	int16			word_44A34;

	int32			dword_44A36;
	int32			dword_44A3E;
	int32			dword_44A3A;
	int32			dword_44A42;
	int32			dword_44A46;

	int16			word_82132;

	int64			mTicks;
	int64			mTicksDiff;

	cPalette		mPalette[ g_MaxColors ];

	void(cFodder::*mGUI_Handle_Button_SelectSquad_Array[3])(void);

public:
	void			AFX_Show();
	void			Music_PlayFile( const char* pFilename );
	void			Music_Stop();

	int16			Mission_Loop( );

	void			Mouse_Handle( );
	void			sub_10937();
	void			sub_1096B();
	void			sub_10BBC();
	void			Squad_Clear();
	void			Heroes_Clear();
	void			sub_10CE7();
	void			sub_10D61();
	void			sub_10D9F();
	void			sub_10DEC();
	void			sub_10EA4();
	void			sub_10EC3();
	void			sub_10EE3();

	void			map_Load_Spt();
	void			Squad_Member_Count();
	void			Squad_Member_Sort();
	void			sub_1142D();
	void			Squad_PrepareMember();
	void			sub_115F7();
	void			Camera_Position_Update();
	int16			sub_119E1( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );

	void			sub_11B06();
	void			sub_11CAD();
	void			sub_11CD6( );
	void			map_Load_Resources();
	void			sub_11E60();
	void			sub_11E6C( );
	void			sub_11EC2();
	void			sub_11FCD();

	void			sub_12018();
	void			sub_12083();
	void			sub_120F6();
	void			Camera_Adjust_Row( int32* pData20 );
	void			sub_12245();
	void			sub_1229C();
	void			sub_122BD();
	void			Mission_Sprites_Handle( );
	void			Sprite_Sort_DrawList();
	void			sub_124DB();
	void			Mission_Goals_Check();
	void			sub_126BB();
	void			Mission_Goals_Set();
	void			sub_12790( );
	void			Mission_Show_Complete();
	void			sub_12877( sSprite_0* pData2C );
	void			sub_128A9( sSprite_0* pData2C );
	void			sub_128DB( sSprite_0* pData2C );
	void			sub_128F4( sSprite_0* pData2C );
	void			Mission_Show_TryAgain();
	void			sub_1298C( sSprite_0* pData2C );
	void			sub_129B6( sSprite_0* pData2C );

	std::string		sub_12AA1( const std::string& pBase, const char* pFinish );
	void			sub_12A5F();
	void			sub_12AEE();
	void			sub_12B6E();
	void			sub_12C69();
	void			sub_12D00();

	void			Map_Overview_Prepare();
	void			map_SetTileType();

	void			sub_13102();
	void			sub_13148();
	void			sub_13155();
	void			sub_131A2();
	int16			sub_131DE();
	void			sub_13255();
	void			sub_13277( sSprite_0* pData2C );
	void			sub_136D0();
	void			sub_13800();
	void			Mouse_DrawCursor( );
	void			sub_13C1C(  int32 pParam00, int32 pParam0C, int32 pParam04, int32 pParam08 );
	void			sub_13C8A(  int16 pData0, int16 pData4, int16 pPosX, int16 pPosY );
	void			sub_13CF0(  sSprite_0* pDi, int16 pData0, int16 pData4 );

	bool			Sprite_OnScreen_Check();
	void			Sprite_Draw( );
	void			sub_14CCB( int16& pData0 );

	void			Sound_Voc_Load();
	void			Sound_Voc_Play( sSprite_0* pSprite, int16 pData4, int16 pData8 );
	void			Music_Play( int16 pTrack );

	// 14EAC
	void			Briefing_Intro();

	void			Briefing_Intro_Jungle();


	void			sub_1590B( );
	void			sub_1594F( );
	void			sub_159A6( );
	void			sub_15A36(  uint8* pDs, int16 pCx );
	void			sub_15B86(  uint8* pDs, int16 pCx );
	void			sub_15B98(  uint8* pDs, int16 pCx );

	void			sub_15CE8(  uint8* pDs, int16 pCx );

	void			sub_15DF0( );
	void			Briefing_Draw_MissionName( );
	
	void			Briefing_Load_Resources();

	/* Recruitment */
	void			Recruit_Show();
	void			Recruit_Draw_HomeAway();
	void			sub_16B55(  int16 pParam0, int16 pParam8, int16 pParamC, const std::string& pString );
	void			sub_16BC3();
	void			sub_16C45( uint16** pDi, int16* pSource );
	void			sub_16C6C();
	void			Recruit_Render_LeftMenu();
	void			Recruit_Render_Squad_Names();
	void			Recruit_Render_Squad_RankKills();
	void			sub_170A4( int16 pData4, int16 pData10 );
	void			Recruit_Render_HeroList();
	void			sub_17368();
	void			sub_17429();
	void			sub_17480( uint16 Data0, int16 Data4, int16 Data8, uint32*& Data20 );
	void			Recruit_Draw_Actors( );
	void			sub_175C0();
	void			Recruit_Draw_Troops();
	void			sub_1787C();
	void			sub_178DD();
	void			sub_17911();
	void			Recruit_Draw_Truck( );
	void			sub_17B64();
	void			Recruit_Draw();
	void			sub_17CD3( );
	void			sub_17C91( cSurface *pImage, int16 Data0, int16 Data8, int16 DataC );
	void			Service_Show();

	/* End Recruitment */

	/* Promotion / Heroes */
	void			Service_KIA_Loop();
	void			Service_Promotion_Loop();
	int16			Service_KIA_Troop_Prepare();
	int16			sub_1804C();
	void			sub_18099( uint16*& pDi,int16 ax, int16 bx );
	void			sub_18149();
	void			sub_181BD();
	void			sub_181E6( uint16*& pDi, const std::string& pText, uint8* pData28, int16 pData0, int16 pData8, int16 pDataC );
	int16			sub_1828A( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );
	void			sub_182EA();
	int16			sub_184C7();
	void			Service_Mission_Text_Prepare( uint16*& pTarget );
	void			Service_Promotion_Prepare();
	void			Service_Promotion_Check();
	void			Service_Promotion_SetNewRanks();

	void			video_Draw_Unk_2( );
	/* End Promotion / Heroes */

	int16			loc_2D9D5( sSprite_0* pSprite );

	void			Briefing_Show( );
	void			Briefing_Prepare();
	void			Briefing_Wait();
	void			Briefing_Draw_With( );
	void			Briefing_DrawBox( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, uint8 pData10 );

	void			sub_18BDF( int16 pBx, int16 pCx, uint8 pSi );
	void			Brief_Draw_Horizontal_Line(  int16 pBx, int16 pDx, int16 pCx, uint8 pSi );

	void			sub_18C45( int32 pPosX, const sIntroString* pString );
	bool			sub_18C7A();
	void			Brief_Draw_Vertical_Line(  int16 pBx, int16 pDx, int16 pCx, uint8 pSi );
	void			sub_18D5E();
	void			Sprite_Handle_Loop();

	void			Sprite_Handle_Player( sSprite_0* pSprite );					// 0
	void			Sprite_Handle_Grenade( sSprite_0* pSprite );				// 2
	void			sub_1998C( sSprite_0* pSprite );
	void			sub_199B2( sSprite_0* pSprite );							// 3
	void			Sprite_Handle_Enemy( sSprite_0* pSprite );					// 5
	void			Sprite_Handle_Bullet( sSprite_0* pSprite );					// 6
	void			sub_19E65( sSprite_0* pSprite );
	void			sub_1A8A5( sSprite_0* pSprite );							// 12
	void			sub_1ABD3( sSprite_0* pSprite );							// 13
	void			Sprite_Handle_Tree( sSprite_0* pSprite );					// 14
	void			Sprite_Handle_BuildingRoof( sSprite_0* pSprite );			// 15
	void			sub_1AC03( sSprite_0* pSprite );							// 16
	void			sub_1AC13( sSprite_0* pSprite );							// 17
	void			sub_1AC23( sSprite_0* pSprite );							// 18
	void			sub_1AC7C( sSprite_0* pSprite );							// 19

	void			Sprite_Handle_BuildingDoor( sSprite_0* pSprite );			// 20
	void			Sprite_Handle_Player_Rank( sSprite_0* pSprite );			// 21
	void			sub_1AF78( sSprite_0* pSprite );							// 22
	void			sub_1B07C( sSprite_0* pSprite );							// 23
	void			sub_1B0C0( sSprite_0* pSprite );							// 24
	void			sub_1B1C1( sSprite_0* pSprite );							// 25
	void			Sprite_Handle_Floating_Dead_Soldier( sSprite_0* pSprite );	// 27
	void			Sprite_Handle_Text_Complete( sSprite_0* pSprite );			// 28
	void			Sprite_Handle_Text_Mission( sSprite_0* pSprite );			// 29

	void			Sprite_Handle_Text_Phase( sSprite_0* pSprite );				// 30
	void			sub_1B4BB( sSprite_0* pSprite );
	void			Sprite_Handle_Rocket( sSprite_0* pSprite );					// 33
	void			Sprite_Handle_Text_GameOver( sSprite_0* pSprite );			// 34
	void			sub_1B8A9( sSprite_0* pSprite );							// 35
	void			Sprite_Handle_Enemy_Rocket( sSprite_0* pSprite );			// 36
	void			Sprite_Handle_GrenadeBox( sSprite_0* pSprite );				// 37
	void			Sprite_Handle_RocketBox( sSprite_0* pSprite );				// 38
	void			sub_1BB11( sSprite_0* pSprite );							// 39

	void			Sprite_Handle_Helicopter( sSprite_0* pSprite );				// 40
	void			sub_1BD54( sSprite_0* pSprite );							// 41
	void			sub_1BD27( sSprite_0* pSprite );							// 42
	void			sub_1BD36( sSprite_0* pSprite );							// 43
	void			sub_1BD45( sSprite_0* pSprite );							// 44
	void			Sprite_Handle_Missile( sSprite_0* pSprite );				// 45
	void			sub_1BEFB( sSprite_0* pSprite );							// 46
	void			sub_1C1C0( sSprite_0* pSprite );							// 47
	void			sub_1C268( sSprite_0* pSprite );							// 48
	void			sub_1C2A5( sSprite_0* pSprite );							// 49

	void			sub_1C296( sSprite_0* pSprite );							// 50
	void			sub_1C2B4( sSprite_0* pSprite );							// 51
	void			sub_1C2C3( sSprite_0* pSprite );							// 52
	void			sub_1C2D2( sSprite_0* pSprite );							// 53
	void			sub_1C331( sSprite_0* pSprite );							// 54
	void			sub_1C364( sSprite_0* pSprite );							// 55
	void			sub_1C411( sSprite_0* pSprite );							// 56
	void			sub_1C4C2( sSprite_0* pSprite );							// 57
	void			Sprite_Handle_Text_Try( sSprite_0* pSprite );				// 58
	void			Sprite_Handle_Text_Again( sSprite_0* pSprite );				// 59

	void			Sprite_Handle_BoilingPot( sSprite_0* pSprite );				// 60
	void			Sprite_Handle_Indigenous( sSprite_0* pSprite );				// 61
	void			sub_1C62F( sSprite_0* pSprite );							// 62
	void			Sprite_Handle_Skidu( sSprite_0* pSprite );					// 63
	void			Sprite_Handle_Truck( sSprite_0* pSprite );					// 64
	void			Sprite_Handle_Tank( sSprite_0* pSprite );					// 65
	void			Sprite_Handle_Bird_Left( sSprite_0* pSprite );				// 66
	void			Sprite_Handle_Bird_Right( sSprite_0* pSprite );				// 67
	void			Sprite_Handle_Seal( sSprite_0* pSprite );					// 68
	void			sub_1CB1F( sSprite_0* pSprite );							// 69

	void			sub_1C689( sSprite_0* pSprite );							// 70
	void			sub_1CE80( sSprite_0* pSprite );							// 71
	void			sub_1D1A2( sSprite_0* pSprite );							// 72
	void			sub_1D483( sSprite_0* pSprite );							// 73
	void			sub_1D4AE( sSprite_0* pSprite );							// 74
	void			sub_1D4BA( sSprite_0* pSprite );							// 75
	void			sub_1D4C6( sSprite_0* pSprite );							// 76
	void			sub_1D4D2( sSprite_0* pSprite );							// 77
	void			Sprite_Handle_Turret( sSprite_0* pSprite );					// 78
	void			sub_1D756( sSprite_0* pSprite );							// 79

	void			sub_1D76F( sSprite_0* pSprite );							// 80
	void			sub_1D77E( sSprite_0* pSprite );							// 81
	void			sub_1D78D( sSprite_0* pSprite );							// 82
	void			sub_1D7B6( sSprite_0* pSprite );							// 83
	void			sub_1D6F2( sSprite_0* pSprite );							// 84
	void			sub_1D70B( sSprite_0* pSprite );							// 85
	void			sub_1D7DD( sSprite_0* pSprite );							// 86
	void			sub_1D802( sSprite_0* pSprite );							// 87
	void			sub_1D81C( sSprite_0* pSprite );							// 88
	void			sub_1DA43( sSprite_0* pSprite );							// 89

	void			sub_1DA48( sSprite_0* pSprite );							// 90
	void			sub_1DACF( sSprite_0* pSprite );							// 91
	void			sub_1DB58( sSprite_0* pSprite );							// 92
	void			Sprite_Handle_RankToGeneral( sSprite_0* pSprite );			// 93
	void			Sprite_Handle_Set50Rockets( sSprite_0* pSprite );			// 94
	void			sub_1DD4C( sSprite_0* pSprite );							// 95
	void			sub_1DD91( sSprite_0* pSprite );							// 96
	void			sub_1BEF6( sSprite_0* pSprite );							// 97
	void			sub_1DE38( sSprite_0* pSprite );							// 98
	void			sub_1DF2C( sSprite_0* pSprite );							// 99

	void			sub_1DFD2( sSprite_0* pSprite );							// 100
	void			sub_1E004( sSprite_0* pSprite );							// 101
	void			sub_1E00E( sSprite_0* pSprite );							// 102
	void			sub_1E018( sSprite_0* pSprite );							// 103
	void			sub_1E022( sSprite_0* pSprite );							// 104
	void			sub_1D724( sSprite_0* pSprite );							// 105
	void			sub_1E02C( sSprite_0* pSprite );							// 106
	void			sub_1E031( sSprite_0* pSprite );							// 107
	void			sub_1E036( sSprite_0* pSprite );							// 108
	void			sub_1E042( sSprite_0* pSprite );							// 109
	void			sub_1E04E( sSprite_0* pSprite );							// 110

	int16			sub_1D92E( sSprite_0* pSprite );
	void			sub_14D6D( sSprite_0* pSprite, int16 pData4 );
	int16			sub_1E05A( sSprite_0* pSprite );
	int16			sub_1EF47( sSprite_0* pSprite );
	int16			loc_1F043( sSprite_0* pSprite );
	void			loc_1F092( sSprite_0* pSprite, sSprite_0* pData24 );

	int16			sub_1F21E( sSprite_0* pSprite );
	void			sub_1F429( sSprite_0* pSprite );
	void			sub_1F5A0( sSprite_0* pSprite );
	void			sub_1F5CA( sSprite_0* pSprite );
	void			sub_1F623( sSprite_0* pSprite );
	void			sub_1F649( sSprite_0* pSprite );

	void			sub_1F66F( sSprite_0* pSprite );
	void			sub_1F6F4( sSprite_0* pSprite );
	void			sub_1F762( sSprite_0* pSprite );
	void			sub_1FCF2( sSprite_0* pSprite );
	void			sub_1FDE7( sSprite_0* pSprite );
	void			sub_1FE35( sSprite_0* pSprite );
	int16			sub_1FF1A( sSprite_0* pSprite, sSprite_0*& pData2C, sSprite_0*& pData30 );
	void			sub_1FFC6( sSprite_0* pSprite, int16& pData4 );

	void			sub_20456( sSprite_0* pSprite, int16& pData8 );
	void			sub_20478( sSprite_0* pSprite );
	void			Hero_Add( sSquad_Member* pSquadMember );

	int16			sub_2060F( sSprite_0* pSprite );
	int16			sub_2061C( sSprite_0* pSprite );
	int16			Troop_Fire_Bullet( sSprite_0* pSprite );
	int16			Troop_Throw_Grenade( sSprite_0* pSprite );
	void			sub_20E5C( sSprite_0* pSprite );
	int16			sub_20E91( sSprite_0* pSprite );
	void			sub_20F19( sSprite_0* pSprite );

	void			sub_21041( sSprite_0* pSprite );
	int16			sub_211BA( int16& pData0, sSprite_0*& pData2C, sSprite_0*& pData30 );
	void			sub_212C4( sSprite_0* pSprite );
	void			sub_212F9( sSprite_0* pSprite );
	void			sub_21483( sSprite_0* pSprite );
	void			sub_21525( sSprite_0* pSprite );
	int16			sub_21618( sSprite_0* pSprite );
	void			sub_21702( sSprite_0* pSprite, int16 pData18 );
	void			sub_2183B( sSprite_0* pSprite );
	void			sub_218E2( sSprite_0* pSprite );
	int16			sub_21914( int16& pData8, int16& pDataC );
	int16			sub_2194E( sSprite_0* pData2C, int16& pData8, int16& pDataC );
	int16			Troop_Deploy( sSprite_0* pSprite, sSprite_0*& pData2C );
	void			sub_21B15( int16& pData0, sSprite_0*& pData2C, sSprite_0*& pData30 );
	void			sub_21C00( sSprite_0* pData2C );
	void			sub_21C6F( );
	void			sub_21CD1( sSprite_0* pSprite );

	void			sub_2212A( sSprite_0* pSprite );
	int16			sub_221A6( sSprite_0* pSprite );
	int16			sub_222A3( sSprite_0* pSprite );
	void			sub_223B2( sSprite_0* pSprite );
	int16			sub_2244B( sSprite_0* pSprite, int16& pData0 );
	int16			sub_224ED( sSprite_0* pSprite );
	int16			sub_228B5( sSprite_0* pSprite, sSprite_0*& pData34 );
	void			sub_229C9( sSprite_0* pSprite );
	void			sub_22A3D( sSprite_0* pSprite );
	int16			sub_22A55( sSprite_0* pSprite );
	void			sub_22AA9( sSprite_0* pSprite );
	void			sub_22B71( sSprite_0* pSprite );
	void			sub_22C87( sSprite_0* pSprite );
	void			sub_22CD7( sSprite_0* pSprite, int16& Data0, int16& Data4 );
	void			sub_22DFC( sSprite_0* pSprite );
	void			sub_2315D( sSprite_0* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14, int16 pData18, int16 pData1C );
	int16			sub_2329D( sSprite_0* pSprite );
	int16			sub_23367( sSprite_0* pSprite );
	void			sub_233D4( sSprite_0* pSprite );
	int16			sub_23444( sSprite_0* pSprite, sSprite_0*& pData2C  );
	void			sub_23525( sSprite_0* pSprite );
	void			sub_236F7( sSprite_0* pSprite );
	void			sub_23879( sSprite_0* pSprite );
	int16			sub_239C9( sSprite_0* pSprite, sSprite_0*& pData2C );
	void			sub_23C70( sSprite_0* pData2C );
	void			sub_23CDD( sSprite_0* pSprite );
	int16			sub_23D57( sSprite_0* pSprite );
	void			sub_23E01( sSprite_0* pSprite, int16& pData0, int16& pData4  );
	void			sub_23EA6( sSprite_0* pSprite );
	void			sub_23EFD( sSprite_0* pSprite );
	void			sub_24173( sSprite_0* pSprite, sSprite_0* pData34 );
	int16			sub_24285( sSprite_0* pSprite, int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData10, sSprite_0*& pData28 );
	void			sub_243E9( sSprite_0* pSprite );
	int16			sub_2449E( sSprite_0* pSprite );
	void			sub_245BF( sSprite_0* pSprite );
	int16			sub_246CC( sSprite_0* pSprite );
	void			sub_248B7( sSprite_0* pSprite );
	int16			sub_2494C( sSprite_0* pSprite );
	int16			sub_24C47( sSprite_0* pSprite, sSprite_0*& pData2C, sSprite_0*& pData34 );
	int16			sub_2531F( sSprite_0* pSprite, sSprite_0*& pData30 );
	void			sub_254F9( sSprite_0* pSprite );
	void			sub_24ED7( sSprite_0* pSprite );
	void			sub_25863( sSprite_0* pSprite );
	void			sub_258C6( sSprite_0* pSprite );
	int16			sub_25680( sSprite_0* pSprite );
	void			sub_257D1( sSprite_0* pSprite );
	void			sub_2593D( sSprite_0* pSprite );
	void			sub_25A31( sSprite_0* pSprite );
	void			sub_25A66( sSprite_0* pSprite );
	int16			sub_25AAE( sSprite_0* pSprite );
	int16			sub_25B6B( sSprite_0* pSprite );
	int16			sub_25DCF( sSprite_0* pSprite );
	void			sub_25F2B( sSprite_0* pSprite );
	void			sub_25FDA( sSprite_0* pSprite );
	void			sub_263F6( sSprite_0* pSprite );
	void			sub_26450( sSprite_0* pSprite );
	void			sub_26490( sSprite_0* pSprite );
	void			sub_264B0( sSprite_0* pSprite );
	int16			sub_265D6( sSprite_0* pSprite, sSprite_0*& pData2C, sSprite_0*& pData30 );
	int16			sub_266CE( sSprite_0* pSprite, sSprite_0*& pData2C );
	void			sub_26781( sSprite_0* pSprite );
	void			sub_2682B( sSprite_0* pSprite, int16 pData1C );

	void			sub_29E30( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );
	int16			sub_29EC2( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );

	void			tool_RandomSeed();
	int16			tool_RandomGet();
	void			Sprite_Movement_Calculate( sSprite_0* pSprite );
	int16			sub_2A1F0( sSprite_0* pSprite, int16& pData0, int16& pData4 );
	void			sub_2A3D4( sSprite_0* pSprite );
	void			sub_2A470();
	int16			sub_2A4A2( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );
	void			sub_2A4FD( int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData18, int16& pData1C );
	int16			sub_2A622( int16& pData0 );
	void			sub_2A74F( int16& pData0, int16& pData4, int16& pData8, int16& pData10, int16& pDataC );
	int16			sub_2A7E2( int16& pData0, int16& pData4 );
	int16			Map_Sprite_Check_Position( sSprite_0* pSprite, int16& pData0, int16& pData4 );
	int16			Map_Terrain_Check( int16& pData0, int16& pData4, int16& pData10, int16& pData14 );
	void			sub_2A932( int16 pData4, int16 pData8, int16 pDataC, int16 pData10 );
	int16			SquadMember_Sprite_Find_In_Region( sSprite_0* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 );

	uint8*			sub_2AE81( int16 *pData0, int16 *pData4 );
	void			sub_2AEB6( int16 pData0, int16 pData4, int16 *pData8, int16* pDataC );
	void			sub_2AF19( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 );
	uint8			sub_2AFF5( uint8* pSi, int16 pBx, int16 pCx );

	void			sub_2B016( uint8* pDi, uint8 pAl );
	void			sub_2B12E( sSprite_0* pSprite );
	void			Sprite_Skidu_Direction_Update( sSprite_0* pSprite, int16& pData1C );
	int16			sub_2B232( const int8* pData28, int16& pData8, int16& pDataC, int16& pData10, int16& pData14 );
	int16			sub_2B286( const int8* pData28, int16& pData8, int16& pDataC, int16& pData10, int16& pData14 );

	void			sub_2B378( int16& Data0, int16& Data4, int16& Data8, int16& DataC );

	void			Camera_Pan();

	void			Camera_Pan_Right();
	void			Camera_Pan_Left();
	void			Camera_Pan_Down();
	void			Camera_Pan_Up();
	void			Camera_Update_Row();
	void			Camera_Update_Column();

	void			sub_2D06C();
	void			sub_2D26A( sSquad_Member* pData24, int16& pData8 );
	void			Squad_Member_Rotate_Can_Fire();
	int16			Sprite_Find_In_Region( sSprite_0* pSprite, sSprite_0*& pData24, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 );
	void			sub_2D725();
	void			sub_2D767();
	void			Squad_Prepare_GrenadesAndRockets();
	void			sub_2D7FF();
	void			sub_2D8AF( sSprite_0* pSprite );
	int16			Sprite_Next_WalkTarget_Set( sSprite_0* pSprite );
	int16			sub_2DBA3( sSprite_0* pSprite );
	void			sub_2DCB0( int16 pData0 );
	void			sub_2DD50( sSprite_0* pSprite );
	void			sub_2DE2C( );
	void			sub_2E01C();
	void			sub_2E72B();

	int16			sub_2EAC2();
	void			sub_2EAC3();
	void			sub_2EACA();
	
	void			sub_9B94E( const struct_6 *pA0 );
	void			sub_A03EE();
	void			sub_A0400();

	void			sub_2EBE0( int16& pData0, int16& pData4 );
	void			Mission_Sidebar_Prepare( int16 pData0, int16 pData4 );
	void			sub_2EBC4();
	void			sub_2EC0E( struct_6* pData20, struct_6* pData24 );
	void			sub_2ECC7( struct_6 *pData20 );
	void			Mission_Sidebar_SplitButton_Draw();
	void			GUI_Prepare_Button_Squad();
	void			GUI_Handle_Button_SelectSquad_0();
	void			GUI_Handle_Button_SelectSquad_1();
	void			GUI_Handle_Button_SelectSquad_2();
	void			Squad_Select( int16 pData4 );
	void			GUI_Handle_Button_SplitSquad();
	void			sub_2EF8A();

	void			map_Tiles_Draw();

	void			sub_2E04C();
	void			Mission_Sidebar_Grenades_Draw( );

	void			GUI_Prepare_Button_Grenade();
	void			GUI_Handle_Button_Grenades();
	void			sub_2F0D7();
	void			Mission_Sidebar_TroopList_Draw();
	void			Mission_Sidebar_TroopList_Name_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, sRecruit* pData28 );
	void			sub_2F452();
	void			Mission_Sidebar_SquadIcon_Draw();
	int16			sub_2F4CB();
	void			sub_2F5ED();
	void			sub_2F757();
	void			sub_2F7D7( );
	void			sub_2F7E4(  int16 pData0 );
	void			sub_2F871( );
	void			sub_2F87E(  int16 pData0 );
	int16			sub_2F90B();
	void			sub_2F9B3( );
	void			sub_2FA05();
	void			Mission_Sidebar_MapButton_Prepare();
	void			sub_2FA44( );
	void			sub_2FA5B( );
	void			sub_2FA75( );
	void			sub_2FA8F();
	void			Mission_Sidebar_Rockets_Draw( );
	void			GUI_Prepare_Button_Rockets();
	void			GUI_Handle_Button_Rockets();
	void			sub_2FC4F();
	void			GUI_Prepare_Button_TroopName();
	void			GUI_Handle_Button_TroopName();
	int16			sub_2FF41();

	void			sub_30082();

	void			Mission_Sidebar_MapButton_RenderWrapper();
	void			sub_302DE( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, int16 pData10 );
	void			Mission_Sidebar_MapButton_Render();
	void			GUI_Handle_Button_ShowOverview();
	void			sub_3037A( );
	void			sub_3049B( );
	void			sub_304CB( );
	void			sub_30AB0();
	int16			sub_30E0B();
	void			sub_30E49();
	void			sub_301F7();
	void			sub_303AE();
	void			sub_303DA();
	void			sub_30465();
	void			sub_30480();
	void			sub_304D0();
	int16			sub_305D5( sSprite_0*& pData20 );
	void			Mouse_Inputs_Check();
	void			Squad_Member_Target_Set();
	int16			sub_30E2A();
	void			sub_31033();
	void			sub_31075( int16 pData0 );
	void			Vehicle_Input_Handle();
	void			sub_311A7();
	int16			sub_313CD();
	void			sub_3169B();

	void			String_CalculateWidth( int32 pPosX, const uint8* pWidths, const char* pString );
	void			String_Print(  const uint8* pWidths, int32 pParam0, int32 pParam08, int32 pParamC, const char* pText );

	std::string		map_Filename_Get();
	std::string		map_Filename_MapGet();
	std::string		map_Filename_SptGet();

	void			intro_LegionMessage();
	int16			introPlayText();
	void			intro();
	void			intro_Music_Play();

	void			map_ClearSpt();
	void			Mission_PhaseNext();

	void			sub_12AB1();

	void			WonGame();

	void			Load_File( const std::string& pFilename );
	void			Load_PlayerBin();
	
	void			videoSleep();

	void			sleepLoop( int64 pMilliseconds );
	int16			ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration );

	void			memory_XMS_Detect();
	void			mouse_Setup();
	void			Mouse_Inputs_Get();
	void			mouse_ButtonCheck();
	void			mouse_GetData();

	void			eventProcess();
	void			keyProcess( uint8 pKeyCode, bool pPressed );

public:

					cFodder( bool pSkipIntro );
					~cFodder();

	void			Sprite_SetDataPtrToBase( const sSpriteSheet** pSpriteSheet );

	bool			EventAdd( cEvent pEvent );
	void			MixerChannelFinished( int32 pChannel );
	void			Prepare();
	void			Start( int16 pStartMap );
	void			Exit( unsigned int pExitCode );

	void			WindowTitleSet( bool pInMission );

	void			VersionLoad( const sVersion* pVersion );
	void			VersionSelect();
	void			VersionSelect_0();
	void			VersionSelect_1();
	void			VersionSelect_2();

};
