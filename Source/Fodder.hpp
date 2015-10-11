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

struct sSquad_Member {
	int16		mRecruitID;
	uint8		mRank;
	int8		mPhaseCount;		// Number of phases completed this Mission
	sSprite*	mSprite;
	uint16		field_6;
	int8		field_8;
	int8		mSelected;
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

struct sRecruitSpritePos {
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

struct sGUI_Element {
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
	char					mInputString[0x14];

	std::vector<const sVersion*> mVersions;

	bool					mSkipIntro;
	Mix_Music*				mMusicPlaying;
	std::vector<cEvent>		mEvents;
	
	const sVersion*			mVersion;

	cGraphics*				mGraphics;
	cResources*				mResources;
	cWindow*				mWindow;

	uint8*					word_397D8;

	sSprite					word_3B4F7;
	sSprite					mSprites[45];

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
	size_t			mDataBaseBlkSize;
	size_t			mDataSubBlkSize;

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
	
	/************** Save Game Region *********** */
	uint16			mMapNumber;
	int16			word_390BE;
	int16			word_390C0;
	int16			word_390C2;
	int16			word_390C4;
	int16			word_390C6;
	uint16			mMissionNumber;
	uint16			mMissionPhase;
	uint16			word_390CE;
	uint16			word_390D0;
	uint16			word_390D2;
	int16			word_390D4;
	sSprite*		word_390D6[8];
	int16			word_390E6;
	uint16			word_390E8;
	int16			word_390EA;
	int16			word_390EC;
	uint16			mMissionPhaseRemain;
	uint16			mMissionPhases;
	uint16			word_390F4;
	int16			word_390F8;
	sSquad_Member	mSquad[9];

	int16			mGraveRanks[361];
	int16*			mGraveRankPtr;
	int16*			mGraveRankPtr2;
	int16			mGraveRecruitID[361];
	int16*			mGraveRecruitIDPtr;

	sHero			mHeroes[6];

	int16			word_397AC;
	int16			word_397AE;
	int16			mMapPlayerTroopCount;
	int16			mSquadMemberCount;
	int16			mTroopsAvailable;

	/************** End Save Game Region *********** */

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
	int16			word_39F66;
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
	sSprite*		word_39FCE;

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
	sGUI_Element*		dword_3A02A;
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
	sSprite*		dword_3A071[45];
	sSprite*		dword_3A125[45];
	sSprite*		dword_3A1D9[45];
	int16			word_3A28D;

	int16			word_3A8B1;
	int16			word_3A8D5;
	int16			word_3A8D7;
	int16			word_3A8D9;
	int32			dword_3A8DB;
	uint8			byte_3A8DE[200];

	sSprite*		dword_3A291[64];
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
	sSprite**		dword_3A9C8;
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
	int8			mGUI_RefreshSquadGrenades[3];
	int8			mGUI_RefreshSquadRockets[3];
	int16			word_3AC3F[3];
	int16			word_3AC45;
	int16			word_3AC47;
	int16			word_3AC49;
	int16			word_3AC4B;
	int16			word_3AC4D;
	int16			word_3AC4F;
	int16			word_3AC51;
	sGUI_Element	mGUI_Elements[42];
	sGUI_Element*	mGUI_NextFreeElement;
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
	sSprite*		dword_3B20B;
	int16			word_3B20F;
	int16			word_3B211;
	sSprite*		dword_3B213[15];
	sSprite*		dword_3B24F[3];
	int16			word_3B25B;
	int16			word_3B25D;
	int16			word_3B2CB;
	int16			word_3B2CD;
	int16			word_3B2CF;
	int16			word_3B2D1[6];
	int16			word_3B2DD[5];
	int16			word_3B2ED;
	int16			mSpawnSpriteType;
	int16			word_3B2F1;
	int16			word_3B2F3;
	int16			word_3B2F5;
	int16			word_3B2F7;
	int16			mGame_Load;
	int16			mGame_Save;
	int16			word_3B301;
	int16			word_3B303;

	int16			word_3B305;
	int16			word_3B307;

	void			(cFodder::*dword_3B30D)(void);

	int16			word_3B32F;
	int16			word_3B335;
	int16			word_3B33D;
	int16			word_3B33F;

	int16			mMapGoals[8];
	int16			word_3B447;
	int16			word_3B449;
	int16			word_3B44F;

	int16			word_3B461[3];
	sSprite*		dword_3B477;
	int16			word_3B47B;
	int16			word_3B47D;
	int16			word_3B47F;
	int16			word_3B481;
	int16			word_3B483;
	int16			mHelicopterCallPadCount;
	int16			mHelicopterCallPadPressedCount;
	sSprite*		dword_3B48B[16];
	int16			word_3B4CB;
	int16			word_3B4CD;
	sSprite*		dword_3B4CF;
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

	sSprite*		dword_3B5F5;
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
	
	sSprite*		mSquad_0_Sprites[9];
	sSprite*		mSquad_1_Sprites[9];
	sSprite*		mSquad_2_Sprites[9];
	sSprite*		mSquad_3_Sprites[9];
	sSprite*		mSquad_4_Sprites[9];
	
	sSprite**		mSquads[5];
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
	sSprite*		word_3BF1E[6];

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

	sGUI_Element		stru_3DEDC[2];
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

	size_t			word_4286D;
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
	int16			byte_44AC0;
	int16			word_44B47;

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
	void			sub_10D24();
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
	void			Mission_Text_Completed();
	void			Mission_Text_Sprite_Mission( sSprite* pData2C );
	void			Mission_Text_Sprite_Phase( sSprite* pData2C );
	void			Mission_Text_Sprite_Complete( sSprite* pData2C );
	void			Mission_Text_Prepare( sSprite* pData2C );
	void			Mission_Text_TryAgain();
	void			Mission_Text_Sprite_Try( sSprite* pData2C );
	void			Mission_Text_Sprite_Again( sSprite* pData2C );

	std::string		sub_12AA1( const std::string& pBase, const char* pFinish );
	void			Squad_Member_PhaseCount();
	void			sub_12AEE();
	void			sub_12B6E();
	void			sub_12C69();
	void			Mission_Map_Overview_Show();

	void			Map_Overview_Prepare();
	void			map_SetTileType();

	void			Sprite_Count_HelicopterCallPads();
	void			sub_13148();
	void			Sprite_HelicopterCallPad_Check();
	void			sub_131A2();
	int16			sub_131DE();
	void			sub_13255();
	void			Mission_Text_GameOver( sSprite* pData2C );
	void			Mission_Paused();
	void			sub_136D0();
	void			sub_13800();
	void			Mouse_DrawCursor( );
	void			sub_13C1C(  int32 pParam00, int32 pParam0C, int32 pParam04, int32 pParam08 );
	void			sub_13C8A(  int16 pData0, int16 pData4, int16 pPosX, int16 pPosY );
	void			sub_13CF0(  sSprite* pDi, int16 pData0, int16 pData4 );

	bool			Sprite_OnScreen_Check();
	void			Sprite_Draw( );
	void			sub_14CCB( int16& pData0 );

	void			Sound_Voc_Load();
	void			Sound_Voc_Play( sSprite* pSprite, int16 pData4, int16 pData8 );
	void			Music_Play( int16 pTrack );

	// 14EAC
	void			Briefing_Intro();

	void			Briefing_Intro_Jungle();
	void			sub_15397();
	void			sub_151C6();
	void			sub_15568();
	void			sub_15739();

	void			sub_1590B( );
	void			sub_1594F( );
	void			sub_159A6( );
	void			sub_15A36(  uint8* pDs, int16 pCx );
	void			sub_15B86(  uint8* pDs, int16 pCx );
	void			sub_15B98(  uint8* pDs, int16 pCx );

	void			sub_15CE8(  uint8* pDs, int16 pCx );

	void			sub_15DF0( );
	void			Briefing_Draw_MissionName( );

	/* Recruitment */
	void			Recruit_Show();
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
	void			Recruit_Draw_Actors( );
	void			sub_175C0();
	void			Recruit_Draw_Troops();
	void			sub_1787C();
	void			sub_178DD();
	void			sub_17911();
	void			Recruit_Draw_Truck( );
	void			sub_17B64();
	void			Recruit_Draw();
	void			Recruit_Draw_Graves( );
	void			sub_17C91( int16 Data0, int16 Data8, int16 DataC );
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

	int16			Squad_Join( sSprite* pSprite );

	void			Briefing_Show( );
	void			Briefing_Prepare();
	void			Briefing_Wait();
	void			Briefing_Draw_With( );
	void			Briefing_DrawBox( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, uint8 pData10 );
	void			Briefing_Draw_Horizontal_Line(  int16 pBx, int16 pDx, int16 pCx, uint8 pSi );
	void			Briefing_Draw_Vertical_Line(  int16 pBx, int16 pDx, int16 pCx, uint8 pSi );

	void			sub_18BDF( int16 pBx, int16 pCx, uint8 pSi );

	void			Intro_Print_String( int32 pPosX, const sIntroString* pString );
	bool			Recruit_Check_Buttons_SaveLoad();
	
	void			sub_18D5E();

	void			Sprite_Handle_Loop();

	void			Sprite_Handle_Player( sSprite* pSprite );				// 0
	void			Sprite_Handle_Grenade( sSprite* pSprite );				// 2
	void			sub_1998C( sSprite* pSprite );
	void			sub_199B2( sSprite* pSprite );							// 3
	void			Sprite_Handle_Enemy( sSprite* pSprite );				// 5
	void			Sprite_Handle_Bullet( sSprite* pSprite );				// 6
	void			sub_19E65( sSprite* pSprite );
	void			Sprite_Handle_Explosion( sSprite* pSprite );			// 12
	void			Sprite_Handle_Shrub( sSprite* pSprite );				// 13
	void			Sprite_Handle_Tree( sSprite* pSprite );					// 14
	void			Sprite_Handle_BuildingRoof( sSprite* pSprite );			// 15
	void			sub_1AC03( sSprite* pSprite );							// 16
	void			Sprite_Handle_Shrub2( sSprite* pSprite );				// 17
	void			Sprite_Handle_Waterfall( sSprite* pSprite );			// 18
	void			Sprite_Handle_Bird2_Left( sSprite* pSprite );			// 19

	void			Sprite_Handle_BuildingDoor( sSprite* pSprite );							// 20
	void			Sprite_Handle_Player_Rank( sSprite* pSprite );							// 21
	void			sub_1AF78( sSprite* pSprite );											// 22
	void			sub_1B07C( sSprite* pSprite );											// 23
	void			sub_1B0C0( sSprite* pSprite );											// 24
	void			Sprite_Handle_Building_Door2( sSprite* pSprite );						// 25
	void			Sprite_Handle_Floating_Dead_Soldier( sSprite* pSprite );				// 27
	void			Sprite_Handle_Text_Complete( sSprite* pSprite );						// 28
	void			Sprite_Handle_Text_Mission( sSprite* pSprite );							// 29

	void			Sprite_Handle_Text_Phase( sSprite* pSprite );							// 30
	void			sub_1B4BB( sSprite* pSprite );
	void			Sprite_Handle_Rocket( sSprite* pSprite );								// 33
	void			Sprite_Handle_Text_GameOver( sSprite* pSprite );						// 34
	void			Sprite_Handle_Shadow( sSprite* pSprite );								// 35
	void			Sprite_Handle_Enemy_Rocket( sSprite* pSprite );							// 36
	void			Sprite_Handle_GrenadeBox( sSprite* pSprite );							// 37
	void			Sprite_Handle_RocketBox( sSprite* pSprite );							// 38
	void			sub_1BB11( sSprite* pSprite );											// 39

	void			Sprite_Handle_Helicopter_Grenade_Enemy( sSprite* pSprite );				// 40
	void			Sprite_Handle_Flashing_Light( sSprite* pSprite );						// 41
	void			Sprite_Handle_Helicopter_Grenade2_Enemy( sSprite* pSprite );			// 42
	void			Sprite_Handle_Helicopter_Missile_Enemy( sSprite* pSprite );				// 43
	void			Sprite_Handle_Helicopter_HomingMissile_Enemy( sSprite* pSprite );		// 44
	void			Sprite_Handle_Missile( sSprite* pSprite );								// 45
	void			Sprite_Handle_MissileHoming( sSprite* pSprite );						// 46
	void			sub_1C1C0( sSprite* pSprite );											// 47
	void			sub_1C268( sSprite* pSprite );											// 48
	void			Sprite_Handle_Helicopter_Grenade2_Human( sSprite* pSprite );			// 49

	void			Sprite_Handle_Helicopter_Grenade_Human( sSprite* pSprite );				// 50
	void			Sprite_Handle_Helicopter_Missile_Human( sSprite* pSprite );				// 51
	void			Sprite_Handle_Helicopter_HomingMissile_Human( sSprite* pSprite );		// 52
	void			sub_1C2D2( sSprite* pSprite );											// 53
	void			Sprite_Handle_Mine( sSprite* pSprite );									// 54
	void			Sprite_Handle_Mine2( sSprite* pSprite );								// 55
	void			Sprite_Handle_Spike( sSprite* pSprite );								// 56
	void			sub_1C4C2( sSprite* pSprite );											// 57
	void			Sprite_Handle_Text_Try( sSprite* pSprite );								// 58
	void			Sprite_Handle_Text_Again( sSprite* pSprite );							// 59

	void			Sprite_Handle_BoilingPot( sSprite* pSprite );			// 60
	void			Sprite_Handle_Indigenous( sSprite* pSprite );			// 61
	void			Sprite_Handle_Indigenous2( sSprite* pSprite );			// 62
	void			Sprite_Handle_VehicleNoGun_Human( sSprite* pSprite );	// 63
	void			Sprite_Handle_VehicleGun_Human( sSprite* pSprite );		// 64
	void			Sprite_Handle_Tank_Human( sSprite* pSprite );			// 65
	void			Sprite_Handle_Bird_Left( sSprite* pSprite );			// 66
	void			Sprite_Handle_Bird_Right( sSprite* pSprite );			// 67
	void			Sprite_Handle_Seal( sSprite* pSprite );					// 68
	void			Sprite_Handle_Tank_Enemy( sSprite* pSprite );			// 69

	void			Sprite_Handle_Indigenous_Spear( sSprite* pSprite );		// 70
	void			sub_1CE80( sSprite* pSprite );							// 71
	void			Sprite_Handle_Hostage( sSprite* pSprite );				// 72
	void			Sprite_Handle_Hostage_Rescue_Tent( sSprite* pSprite );	// 73
	void			Sprite_Handle_Door_Indigenous( sSprite* pSprite );		// 74
	void			Sprite_Handle_Door2_Indigenous( sSprite* pSprite );		// 75
	void			Sprite_Handle_Door_Indigenous_Spear( sSprite* pSprite );// 76
	void			sub_1D4D2( sSprite* pSprite );							// 77
	void			Sprite_Handle_Turret_Missile_Human( sSprite* pSprite );	// 78
	void			Sprite_Handle_Turret_Missile2_Human( sSprite* pSprite );// 79

	void			Sprite_Handle_VehicleNoGun_Enemey( sSprite* pSprite );	// 80
	void			Sprite_Handle_VehicleGun_Enemy( sSprite* pSprite );		// 81
	void			Sprite_Handle_Vehicle_Unk_Enemy( sSprite* pSprite );	// 82
	void			sub_1D7B6( sSprite* pSprite );							// 83
	void			Sprite_Handle_Turret_Missile_Enemy( sSprite* pSprite );	// 84
	void			Sprite_Handle_Turret_Missile2_Enemy( sSprite* pSprite );// 85
	void			sub_1D7DD( sSprite* pSprite );							// 86
	void			sub_1D802( sSprite* pSprite );							// 87
	void			Sprite_Handle_Building_Door3( sSprite* pSprite );		// 88
	void			Sprite_Handle_Explosion2( sSprite* pSprite );			// 89

	void			sub_1DA48( sSprite* pSprite );							// 90
	void			sub_1DACF( sSprite* pSprite );							// 91
	void			Sprite_Handle_Spider_Mine( sSprite* pSprite );			// 92
	void			Sprite_Handle_RankToGeneral( sSprite* pSprite );		// 93
	void			Sprite_Handle_Set50Rockets( sSprite* pSprite );			// 94
	void			sub_1DD4C( sSprite* pSprite );							// 95
	void			Sprite_Handle_Set50RocketsAndRank( sSprite* pSprite );	// 96
	void			Sprite_Handle_MissileHoming2( sSprite* pSprite );		// 97
	void			Sprite_Handle_SquadToGeneral_Give_50HomingMissiles( sSprite* pSprite );							// 98
	void			Sprite_Handle_Helicopter_CallPad( sSprite* pSprite );	// 99

	void			sub_1DFD2( sSprite* pSprite );							// 100
	void			sub_1E004( sSprite* pSprite );							// 101
	void			sub_1E00E( sSprite* pSprite );							// 102
	void			sub_1E018( sSprite* pSprite );							// 103
	void			sub_1E022( sSprite* pSprite );							// 104
	void			Sprite_Handle_Turret_HomingMissile( sSprite* pSprite );	// 105
	void			Sprite_Handle_Hostage_2( sSprite* pSprite );			// 106
	void			sub_1E031( sSprite* pSprite );							// 107
	void			sub_1E036( sSprite* pSprite );							// 108
	void			sub_1E042( sSprite* pSprite );							// 109
	void			sub_1E04E( sSprite* pSprite );							// 110

	int16			sub_1D92E( sSprite* pSprite );
	void			sub_14D6D( sSprite* pSprite, int16 pData4 );
	int16			sub_1E05A( sSprite* pSprite );
	int16			Troop_Dies( sSprite* pSprite );
	int16			loc_1F043( sSprite* pSprite );
	void			loc_1F092( sSprite* pSprite, sSprite* pData24 );

	int16			sub_1F21E( sSprite* pSprite );
	void			sub_1F429( sSprite* pSprite );
	void			sub_1F5A0( sSprite* pSprite );
	void			sub_1F5CA( sSprite* pSprite );
	void			sub_1F623( sSprite* pSprite );
	void			sub_1F649( sSprite* pSprite );

	void			sub_1F66F( sSprite* pSprite );
	void			sub_1F6F4( sSprite* pSprite );
	void			sub_1F762( sSprite* pSprite );
	void			sub_1FCF2( sSprite* pSprite );
	void			sub_1FDE7( sSprite* pSprite );
	void			sub_1FE35( sSprite* pSprite );
	int16			sub_1FF1A( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30 );
	void			sub_1FFC6( sSprite* pSprite, int16& pData4 );

	void			sub_20456( sSprite* pSprite, int16& pData8 );
	void			sub_20478( sSprite* pSprite );
	void			Hero_Add( sSquad_Member* pSquadMember );

	int16			Sprite_Destroy_Wrapper( sSprite* pSprite );
	int16			Sprite_Destroy( sSprite* pSprite );
	int16			Troop_Fire_Bullet( sSprite* pSprite );
	int16			Troop_Throw_Grenade( sSprite* pSprite );
	void			sub_20E5C( sSprite* pSprite );
	int16			sub_20E91( sSprite* pSprite );
	void			sub_20F19( sSprite* pSprite );

	void			sub_21041( sSprite* pSprite );
	int16			Sprite_Get_Free( int16& pData0, sSprite*& pData2C, sSprite*& pData30 );
	void			Sprite_Clear( sSprite* pSprite );
	void			Sprite_Handle_Explodable( sSprite* pSprite );
	void			sub_21483( sSprite* pSprite );
	void			sub_21525( sSprite* pSprite );
	int16			sub_21618( sSprite* pSprite );
	void			sub_21702( sSprite* pSprite, int16 pData18 );
	void			sub_2183B( sSprite* pSprite );
	void			sub_218E2( sSprite* pSprite );
	int16			sub_21914( int16& pData8, int16& pDataC );
	int16			sub_2194E( sSprite* pData2C, int16& pData8, int16& pDataC );
	int16			Troop_Deploy( sSprite* pSprite, sSprite*& pData2C );
	void			Sprite_Get_Free2( int16& pData0, sSprite*& pData2C, sSprite*& pData30 );
	void			sub_21C00( sSprite* pData2C );
	void			Sprite_Create_Rank( );
	void			sub_21CD1( sSprite* pSprite );

	void			sub_2212A( sSprite* pSprite );
	int16			Sprite_Handle_BuildingDoor_Explode( sSprite* pSprite );
	int16			sub_222A3( sSprite* pSprite );
	void			sub_223B2( sSprite* pSprite );
	int16			sub_2244B( sSprite* pSprite, int16& pData0 );
	int16			sub_224ED( sSprite* pSprite );
	int16			sub_228B5( sSprite* pSprite, sSprite*& pData34 );
	void			sub_229C9( sSprite* pSprite );
	void			sub_22A3D( sSprite* pSprite );
	int16			sub_22A55( sSprite* pSprite );
	void			sub_22AA9( sSprite* pSprite );
	void			sub_22B71( sSprite* pSprite );
	void			sub_22C87( sSprite* pSprite );
	void			sub_22CD7( sSprite* pSprite, int16& Data0, int16& Data4 );
	void			sub_22DFC( sSprite* pSprite );
	void			sub_2315D( sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14, int16 pData18, int16 pData1C );
	int16			sub_2329D( sSprite* pSprite );
	int16			sub_23367( sSprite* pSprite );
	void			Sprite_Handle_Vehicle_Human( sSprite* pSprite );
	int16			sub_23444( sSprite* pSprite, sSprite*& pData2C  );
	void			sub_23525( sSprite* pSprite );
	void			sub_236F7( sSprite* pSprite );
	void			sub_23879( sSprite* pSprite );
	int16			Sprite_Create_Missile( sSprite* pSprite, sSprite*& pData2C );
	void			sub_23C70( sSprite* pData2C );
	void			Sprite_Handle_Helicopter( sSprite* pSprite );
	int16			sub_23D57( sSprite* pSprite );
	void			sub_23E01( sSprite* pSprite, int16& pData0, int16& pData4  );
	void			sub_23EA6( sSprite* pSprite );
	void			Sprite_Handle_Turret( sSprite* pSprite );
	void			Sprite_Handle_Turret_Fire( sSprite* pSprite, sSprite* pData34 );
	int16			Sprite_Find_By_Types( sSprite* pSprite, int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData10, sSprite*& pData28 );
	void			sub_243E9( sSprite* pSprite );
	int16			sub_2449E( sSprite* pSprite );
	void			sub_245BF( sSprite* pSprite );
	int16			sub_246CC( sSprite* pSprite );
	void			sub_248B7( sSprite* pSprite );
	int16			Sprite_Create_Grenade( sSprite* pSprite );
	int16			Sprite_Create_MissileHoming( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData34 );
	int16			sub_2531F( sSprite* pSprite, sSprite*& pData30 );
	void			Sprite_Handle_Vehicle_Enemy( sSprite* pSprite );
	void			sub_24ED7( sSprite* pSprite );
	void			sub_25863( sSprite* pSprite );
	void			sub_258C6( sSprite* pSprite );
	int16			sub_25680( sSprite* pSprite );
	void			sub_257D1( sSprite* pSprite );
	void			sub_2593D( sSprite* pSprite );
	void			sub_25A31( sSprite* pSprite );
	void			sub_25A66( sSprite* pSprite );
	int16			sub_25AAE( sSprite* pSprite );
	int16			sub_25B6B( sSprite* pSprite );
	int16			sub_25DCF( sSprite* pSprite );
	void			sub_25F2B( sSprite* pSprite );
	void			sub_25FDA( sSprite* pSprite );
	void			sub_263F6( sSprite* pSprite );
	void			sub_26450( sSprite* pSprite );
	void			sub_26490( sSprite* pSprite );
	void			sub_264B0( sSprite* pSprite );
	int16			sub_265D6( sSprite* pSprite, sSprite*& pData2C, sSprite*& pData30 );
	int16			sub_266CE( sSprite* pSprite, sSprite*& pData2C );
	void			sub_26781( sSprite* pSprite );
	void			sub_2682B( sSprite* pSprite, int16 pData1C );

	void			sub_29E30( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );
	int16			sub_29EC2( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );

	void			tool_RandomSeed();
	int16			tool_RandomGet();
	void			Sprite_Movement_Calculate( sSprite* pSprite );
	int16			sub_2A1F0( sSprite* pSprite, int16& pData0, int16& pData4 );
	void			sub_2A3D4( sSprite* pSprite );
	void			sub_2A470();
	int16			sub_2A4A2( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );
	void			sub_2A4FD( int16& pData0, int16& pData4, int16& pData8, int16& pDataC, int16& pData18, int16& pData1C );
	int16			sub_2A622( int16& pData0 );
	void			sub_2A74F( int16& pData0, int16& pData4, int16& pData8, int16& pData10, int16& pDataC );
	int16			sub_2A7E2( int16& pData0, int16& pData4 );
	int16			Map_Sprite_Check_Position( sSprite* pSprite, int16& pData0, int16& pData4 );
	int16			Map_Terrain_Check( int16& pData0, int16& pData4, int16& pData10, int16& pData14 );
	void			sub_2A932( int16 pData4, int16 pData8, int16 pDataC, int16 pData10 );
	int16			SquadMember_Sprite_Find_In_Region( sSprite* pSprite, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 );

	uint8*			sub_2AE81( int16& pData0, int16& pData4 );
	void			sub_2AEB6( int16 pData0, int16 pData4, int16 *pData8, int16* pDataC );
	uint8			sub_2AFF5( uint8* pSi, int16 pBx, int16 pCx );

	void			sub_2B016( uint8* pDi, uint8 pAl );
	void			sub_2B12E( sSprite* pSprite );
	void			Sprite_Skidu_Direction_Update( sSprite* pSprite, int16& pData1C );
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
	int16			Sprite_Find_In_Region( sSprite* pSprite, sSprite*& pData24, int16 pData8, int16 pDataC, int16 pData10, int16 pData14 );
	void			sub_2D725();
	void			sub_2D767();
	void			Squad_Prepare_GrenadesAndRockets();
	void			sub_2D7FF();
	void			sub_2D8AF( sSprite* pSprite );
	int16			Sprite_Next_WalkTarget_Set( sSprite* pSprite );
	int16			sub_2DBA3( sSprite* pSprite );
	void			sub_2DCB0( int16 pData0 );
	void			sub_2DD50( sSprite* pSprite );
	void			sub_2DE2C( );
	void			sub_2E01C();
	void			Game_Save_Wrapper2();
	void			sub_2E122();
	void			sub_2E172( const char* pText, int16 pDataC, int16 pData0, int16 pData4 );
	void			sub_2E1B1( int16 pData0, int16 pData4 );
	void			sub_2E302( bool pShowCursor );
	void			Game_Save_Wrapper();
	void			sub_2E244( void(cFodder::*pFunction )(void) ); 
	void			Game_Save();
	void			sub_2E3E3( sGUI_Element* pData20 );
	void			sub_2E5B3();
	void			sub_2E5C3();
	void			sub_2E6A9();
	void			sub_2E704();
	void			Game_Load();
	void			sub_2E953();
	void			sub_2E967();
	void			sub_2EA89();

	int16			sub_2EAC2();
	void			sub_2EAC3();
	void			sub_2EACA();
	
	void			sub_A03E0();
	void			sub_A03EE();
	void			sub_A0400();
	void			GUI_Element_Mouse_Over( const sGUI_Element *pA0 );
	void			sub_9BA08();
	void			sub_9BA1A();
	void			sub_A0436();
	void			sub_A056E();
	void			sub_A0578();
	void			sub_A058C();
	void			sub_A05A0();
	void			sub_A05B4();
	void			sub_A05C8();
	void			sub_A05DC();
	void			sub_A05F0();
	void			sub_A0604();
	void			sub_A0618();
	void			sub_A062C();
	void			sub_A0640( const char* pFilename );

	void			sub_2EBE0( int16& pData0, int16& pData4 );
	void			Mission_Sidebar_Prepare( int16 pData0, int16 pData4 );
	void			sub_2EBC4();
	void			GUI_SetElementsFrom( sGUI_Element* pData20, sGUI_Element* pData24 );
	void			GUI_ClearElement( sGUI_Element *pData20 );
	void			Mission_Sidebar_SplitButton_Draw();
	void			GUI_Prepare_Button_Squad();
	void			GUI_Handle_Button_SelectSquad_0();
	void			GUI_Handle_Button_SelectSquad_1();
	void			GUI_Handle_Button_SelectSquad_2();
	void			Squad_Select( int16 pData4 );
	void			GUI_Handle_Button_SplitSquad();
	void			sub_2EF8A();

	void			map_Tiles_Draw();

	void			Game_CheckLoadSave();
	void			Mission_Sidebar_Grenades_Draw( );

	void			GUI_Prepare_Button_Grenade();
	void			GUI_Handle_Button_Grenades();
	void			sub_2F0D7();
	void			Mission_Sidebar_TroopList_Draw();
	void			Mission_Sidebar_TroopList_Name_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, const char* pData28 );
	void			sub_2F452();
	void			Mission_Sidebar_SquadIcon_Draw();
	int16			sub_2F4CB();
	void			Mission_Sidebar_Refresh();
	void			Mission_Sidebar_SquadIcon_Refresh();
	void			Mission_Sidebar_Grenades_Refresh_CurrentSquad( );
	void			Mission_Sidebar_Grenades_Refresh( int16 pData0 );
	void			Mission_Sidebar_Rockets_Refresh_CurrentSquad( );
	void			Mission_Sidebar_Rockets_Refresh( int16 pData0 );
	int16			sub_2F90B();
	void			Mission_Sidebar_Setup( );
	void			sub_2FA05();
	void			Mission_Sidebar_MapButton_Prepare();
	void			Mission_Sidebar_Squad0_Prepare( );
	void			Mission_Sidebar_Squad1_Prepare( );
	void			Mission_Sidebar_Squad2_Prepare( );
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
	void			Mission_Sidebar_Rockets_Refresh_CurrentSquad_Wrapper( );
	void			sub_30AB0();
	int16			Mouse_Button_Left_Toggled();
	void			sub_30E49();
	void			Squad_Clear_Selected();
	void			sub_303AE();
	void			sub_303B7();
	void			sub_303DA();
	void			sub_30465();
	void			sub_30480();
	void			sub_304D0();
	void			Mission_IsFinalMap();
	int16			sub_305D5( sSprite*& pData20 );
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
	
	void			video_Print_Text( const char* pText, int16 pPosY );
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
	void			VersionSelect_3();
};
