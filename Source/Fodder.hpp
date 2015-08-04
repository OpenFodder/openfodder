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
	uint8		field_3;
	sSprite_0*	field_4;
	uint16		field_6;
	uint8		field_8;
	uint8		field_9;
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

struct sIntroString {
	uint8		mPosition;
	const char* mText;
};

struct sIntroText {
	uint8				mImageNumber;
	const sIntroString*	mText;
};

struct sRecruit {
	char  mName[7];	// increased to 7, to allow the null terminator
	uint8 field_6;
	uint8 field_7;
	uint8 field_8;
};

struct sHero {
	int8	mRecruitID;	//field_0
	int16	field_1;	//field_2
	int16	mKills;		//field_3
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
	void	(cFodder::*field_C)();
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

extern struct sRecruit mRecruits[360];

class cFodder : public cSingleton < cFodder > {
	bool					mSkipIntro;
	std::vector<cEvent>		mEvents;

	cResources*				mResources;
	cWindow*				mWindow;

	sSprite_0				word_3B4F7;
	sSprite_0				mSprites[45];

	const sSpriteSheet**	mSpriteDataPtr;

	cSurface*		mSurfaceMapOverview;
	cSurface*		mImage;

	cPosition		mMousePosition;
	uint32			mMouseButtons;

	int32			mGraphicBlkPtrs[480];
	uint16			mMapWidth;
	uint16			mMapHeight;
	uint8			mKeyCode;

	uint8*			mDataPStuff;
	uint8* 			mDataHillBits;
	uint8*			mDataArmy;
	uint8*			mMap;
	size_t			mMapSize;
	uint8*			mDataBaseBlk;
	uint8*			mDataSubBlk;

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
	uint16			word_390E8;
	int16			word_390EA;
	int16			word_390EC;
	uint16			mMissionPhaseRemain;
	uint16			mMissionPhases;
	uint16			word_390F4;
	int16			word_390F8;
	sSquad_Member	mSquad[8];
	int16			word_3915A;
	int16			word_3915E;

	int16			word_391D2[361];
	int16*			dword_394A4;
	int16*			dword_394A8;
	int16			word_394AC[361];
	int16*			dword_3977E;

	sHero			mHeroes[6];

	int16			word_397AC;
	int16			word_397AE;
	int16			word_397D2;
	int16			word_397D4;
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
	int16			word_39FA8;
	int16			word_39FAA;
	int16			word_39FAC;

	int16			word_39FAE;
	int16			word_39FBA;

	int16			word_39FBC;
	int16			word_39EF6;
	int16			word_39EFA;
	int16			word_39EFC;
	int16			word_39F04;
	int32			dword_39F18;
	int32			dword_39F1C;
	int32			dword_39F20;
	int32			dword_39F24;
	int32			dword_39F28;
	int32			dword_39F2C;
	int32			dword_39F30;
	int32			dword_39F36;
	int16			word_39F3A;
	int16			word_39F3C;
	int16			word_39F3E;

	int32			dword_39F84;
	int32			dword_39F88;
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
	sSprite_0*		word_39FCE;	// This stores the number of bytes, inside the sSprite array

	int16			word_39FD0;
	int16			word_39FD2;
	int16			word_39FD4;

	int16			word_39FF2;
	int16			word_39FF4;
	int16			word_39FF6;
	int16			word_39FF8;
	int16			word_39FFA;

	int16			word_3A01A;
	struct_6*		dword_3A02A;
	int32			dword_3A030;
	int16			word_3A063;
	int16			word_3A065;
	int16			word_3A067;
	int16			word_3A069;
	int16			word_3A06B;
	sSprite_0*		dword_3A071[20];
	sSprite_0*		dword_3A125[45];
	sSprite_0*		dword_3A1D9[45];
	int16			word_3A28D;

	int16			word_3A8B1;
	int16			word_3A8D5;
	int16			word_3A8D7;
	int16			word_3A8D9;
	int32			dword_3A8DB;
	uint8			byte_3A8DE[200];

	int16			dword_3A000;
	int16			dword_3A004;
	int16			dword_3A008;
	int16			word_3A00C;
	int16			word_3A00E;
	int16			word_3A010;
	int16			word_3A016;
	uint16			word_3A024;
	uint16			word_3A054;
	int8			byte_3A05A[4];
	uint8			byte_3A05E;
	uint16			word_3A05F;
	sSprite_0*		dword_3A291[14];
	int32			dword_3A391;
	uint32			dword_3A395;
	int16			word_3A399;
	uint32			dword_3A39D;
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
	uint16			word_3A9AC;
	int16			word_3A9AE;
	int16			word_3A9B0;
	uint16			word_3A9B2;
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
	int16			word_3AA05[3];
	int16			word_3AA0B[3];
	int16			word_3AA11[3];
	int16			word_3AA17;
	int16			word_3AA19;
	int16			word_3AA1B;
	int16			word_3AA1D;
	int16			word_3AA1F;
	int16			word_3AA21;
	int16			word_3AA41;
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
	int8			byte_3ABA9;
	int16			word_3ABB1;
	int16			word_3ABB3;
	int16			word_3ABB5;
	int16			word_3ABB7;
	struct_5		stru_3ABB9;
	int16			word_3ABA7;
	int16			word_3ABAD;
	int16			word_3ABAF;
	int16			word_3ABBB;
	int16			word_3ABC3;
	int16			word_3ABC5;
	int16			word_3ABC7;
	sSquad_Member*	dword_3ABC9;
	int16			word_3ABE7;
	int16			word_3ABE9;
	int16			word_3ABEB;
	int16			word_3ABFB;
	int16			word_3ABFD;

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
	int16			word_3AC3F[6];
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
	int32			dword_3B1CB;
	int16			word_3B1AB;
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
	int16			word_3B2CB;
	int16			word_3B2CD;
	int16			word_3B2CF;
	int16			word_3B2F1;
	int16			word_3B2F3;
	int16			word_3B2FD;
	int16			word_3B2FF;
	int16			word_3B301;
	int16			word_3B303;

	int16			word_3B305;
	int16			word_3B307;
	
	int16			mMapGoals[8];
	int16			word_3B447;
	int16			word_3B44F;

	int16			word_3B461[3];
	int16			word_3B47B;
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
	int16			word_3B4DB;
	int16			word_3B4DD;
	int16			word_3B4EB;
	int16			word_3B4F1;
	int16			word_3B4F3;
	int16			word_3B4F5;

	int16			mMapTilePtr;
	int16			word_3B60E;
	int16			word_3B610;
	int16			word_3B612;
	int16			word_3B614;
	int16			word_3B616;

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
	uint16			word_3BEB9;
	uint16			word_3BEBB;
	uint16			word_3BEBD;
	uint16			word_3BEC1;
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
	int16			word_3D473;
	int16			word_3D475;
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
	int16			word_3FA37;
	int16			mKeyControlPressed;

	int16			mImageFaded;
	int16			mMouseSpriteCurrent;
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
	int16			word_42074;
	int16			word_42076;
	uint16			word_42078;

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

	int64			mTicks;
	int64			mTicksDiff;

	cPalette		mPalette[ g_MaxColors ];

	void(cFodder::*off_3DEF6[3])(void);

protected:

	uint8*			GetSpriteData( uint16 pSegment );
	int16			Mission_Loop( cSurface* pImage );

	void			Mouse_Handle( cSurface* pImage );
	void			sub_10937();
	void			sub_1096B();
	void			sub_10BBC();
	void			Troops_Clear();
	void			Heroes_Clear();
	void			sub_10CE7();
	void			sub_10D61();
	void			sub_10D9F();
	void			sub_10DEC();
	void			sub_10EA4();
	void			sub_10EC3();
	void			sub_10EE3();

	void			map_Load_Spt();
	void			map_Troops_Prepare();
	void			map_Load_Players();
	void			sub_1142D();
	void			sub_1152F();
	void			sub_115F7();
	void			sub_11885();
	int16			sub_119E1( int16& pData0, int16& pData4, int16& pData8, int16& pDataC );

	void			sub_11B06();
	void			sub_11CAD();
	void			sub_11CD6( cSurface* pImage );
	void			map_Load_TileSet();
	void			sub_11E60();
	void			sub_11E6C( cSurface* pImage );
	void			sub_11EC2();
	void			sub_11FCD();

	void			sub_12018();
	void			sub_12083();
	void			sub_120F6();
	void			sub_121B6( int32* pData20 );
	void			sub_12245();
	void			sub_1229C();
	void			sub_122BD();
	void			Mission_Sprites_Handle( cSurface* pImage );
	void			sub_12419();
	void			sub_124DB();
	void			Mission_Goals_Check();
	void			sub_126BB();
	void			sub_126DD();
	void			sub_12790( cSurface* pImage );
	void			sub_12838();
	void			sub_12877( sSprite_0* pData2C );
	void			sub_128A9( sSprite_0* pData2C );
	void			sub_128DB( sSprite_0* pData2C );
	void			sub_128F4( sSprite_0* pData2C );
	void			sub_12952();
	void			sub_1298C( sSprite_0* pData2C );
	void			sub_129B6( sSprite_0* pData2C );

	std::string		sub_12AA1( std::string pBase, const char* pFinish );
	void			sub_12A5F();
	void			sub_12AEE();
	void			sub_12B6E();
	void			sub_12C69();

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
	void			Mouse_DrawCursor( cSurface* pImage );
	void			sub_13C1C( cSurface* pImage, int32 pParam00, int32 pParam0C, int32 pParam04, int32 pParam08 );
	void			sub_13C8A( cSurface* pImage, int16 pData0, int16 pData4, int16 pPosX, int16 pPosY );
	void			sub_13CF0( cSurface* pImage, sSprite_0* pDi, int16 pData0, int16 pData4 );

	void			video_Draw_Sprite_( cSurface* pImage );
	void			video_Draw_Linear_To_Planar(  cSurface* pImage );
	bool			Sprite_OnScreen_Check();
	void			Mission_Sprite_Draw( cSurface* pImage );
	void			sub_144A2( cSurface* pImage );
	void 			sub_145AF( int16 pData0, int16 pData8, int16 pDataC );
	void			sub_14B84( sSprite_0* pSprite, int16 pData4, int16 pData8 );
	
	// 14EAC
	void			Mission_Brief();

	void			Briefing_Intro_Jungle( cSurface *pImage );


	void			sub_1590B( cSurface* pImage );
	void			sub_1594F( cSurface* pImage );
	void			sub_159A6( cSurface* pImage );
	void			sub_15A36( cSurface* pImage, uint8* pDs, int16 pCx );
	void			sub_15B86( cSurface* pImage, uint8* pDs, int16 pCx );
	void			sub_15B98( cSurface* pImage, uint8* pDs, int16 pCx );

	void			sub_15CE8( cSurface* pImage, uint8* pDs, int16 pCx );

	void			sub_15DF0( cSurface* pImage );
	void			Briefing_Draw_MissionName( cSurface* pImage );
	
	void			map_Load_Resources();

	/* Recruitment */
	void			Recruit_Show();
	void			Recruit_Draw_Hill( cSurface *pImage );
	void			Recruit_Draw_HomeAway( cSurface *pImage );
	void			sub_16B55( cSurface* pImage, int16 pParam0, int16 pParam8, int16 pParamC, const std::string& pString );
	void			sub_16BC3();
	void			sub_16C45( uint16** pDi, int16* pSource );
	void			sub_16C6C();
	void			Recruit_Render_LeftMenu( cSurface *pImage );
	void			Recruit_Render_Squad_Names();
	void			Recruit_Render_Squad_RankKills();
	void			sub_170A4( int16 pData4, int16 pData10 );
	void			Recruit_Render_HeroList();
	void			sub_17368();
	void			sub_17429();
	void			sub_17480( uint16 Data0, int16 Data4, int16 Data8, uint32*& Data20 );
	void			Recruit_Draw_Actors( cSurface* pImage );
	void			sub_175C0();
	void			Recruit_Draw_Troops( cSurface *pImage );
	void			sub_1787C();
	void			sub_178DD();
	void			sub_17911();
	void			Recruit_Draw_Truck( cSurface* pImage );
	void			sub_17B64();
	void			Recruit_Draw( cSurface *pImage );
	void			sub_17CD3( cSurface* pImage );
	void			sub_17C91( cSurface *pImage, int16 Data0, int16 Data8, int16 DataC );
	void			sub_17DB3();

	void			video_Draw_Unk_2( cSurface* pImage );
	/* End Recruitment */
	
	int16			loc_2D9D5( sSprite_0* pSprite );

	void			Briefing_Show( cSurface* pImage );
	void			sub_18908();
	void			Briefing_Wait();
	void			Briefing_Draw_With( cSurface* pImage );
	void			Briefing_DrawBox( cSurface* pImage, int16 pData0, int16 pData4, int16 pData8, int16 pDataC, uint8 pData10 );

	void			sub_18BDF( cSurface* pImage, int16 pBx, int16 pCx, uint8 pSi );
	void			Brief_Draw_Horizontal_Line( cSurface* pImage, int16 pBx, int16 pDx, int16 pCx, uint8 pSi );

	void			sub_18C45( cSurface* pImage, int32 pPosX, const sIntroString* pString );
	bool			sub_18C7A();
	void			Brief_Draw_Vertical_Line( cSurface* pImage, int16 pBx, int16 pDx, int16 pCx, uint8 pSi );
	void			sub_18D5E();
	void			Sprite_Handle_Loop();

	void			Sprite_Handle_Player( sSprite_0* pSprite );				// 0
	void			sub_199F3( sSprite_0* pSprite );						// 5
	void			sub_1ABD3( sSprite_0* pSprite );						// 13
	void			Sprite_Handle_Tree( sSprite_0* pSprite );				// 14

	void			Sprite_Handle_Player_Rank( sSprite_0* pSprite );		// 21
	void			Sprite_Handle_Bird( sSprite_0* pSprite );				// 66
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
	void			sub_1FF1A( sSprite_0* pSprite );
	void			sub_1FFC6( sSprite_0* pSprite );

	void			sub_20456( sSprite_0* pSprite, int16& pData8 );
	void			sub_20478( sSprite_0* pSprite );
	void			sub_20534( sSquad_Member* pSquadMember );

	int16			sub_2060F( sSprite_0* pSprite );
	int16			sub_2061C( sSprite_0* pSprite );
	int16			sub_20653( sSprite_0* pSprite );
	int16			sub_20A39( sSprite_0* pSprite );
	void			sub_20E5C( sSprite_0* pSprite );
	int16			sub_20E91( sSprite_0* pSprite );
	void			sub_20F19( sSprite_0* pSprite );

	void			sub_21041( sSprite_0* pSprite );
	void			sub_211BA( int16& pData0, sSprite_0*& pData2C, sSprite_0*& pData30 );
	void			sub_212C4( sSprite_0* pSprite );
	void			sub_212F9( sSprite_0* pSprite );
	void			sub_21483( sSprite_0* pSprite );
	
	int16			sub_21914( int16& pData8, int16& pDataC );
	int16			sub_2194E( sSprite_0* pData2C, int16& pData8, int16& pDataC );
	void			sub_21C6F( );

	void			sub_223B2( sSprite_0* pSprite );
	int16			sub_224ED( sSprite_0* pSprite );
	int16			sub_228B5( sSprite_0* pSprite, sSprite_0*& pData34 );
	void			sub_22AA9( sSprite_0* pSprite );
	void			sub_22B71( sSprite_0* pSprite );

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
	int16			sub_2A7F7( sSprite_0* pSprite, int16& pData0, int16& pData4 );
	int16			Map_Terrain_Check( int16& pData0, int16& pData4, int16& pData10, int16& pData14 );
	void			sub_2A932( int16 pData4, int16 pData8, int16 pDataC, int16 pData10 );

	uint8*			sub_2AE81( int16 *pData0, int16 *pData4 );
	void			sub_2AEB6( int16 pData0, int16 pData4, int16 *pData8, int16* pDataC );
	void			sub_2AF19( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 );
	uint8			sub_2AFF5( uint8* pSi, int16 pBx, int16 pCx );

	void			sub_2B016( uint8* pDi, uint8 pAl );
	void			sub_2B04B( uint8* pTileGraphicPtr, uint16 pDestX, uint16 pDestY );

	void			Camera_Pan( cSurface* pImage );

	void			Camera_Pan_Right( cSurface* pImage );
	void			Camera_Pan_Left( cSurface* pImage );
	void			Camera_Pan_Down( cSurface* pImage );
	void			Camera_Pan_Up( cSurface* pImage );
	void			sub_2CF6D();
	void			sub_2CFEA();

	void			sub_2D06C();
	void			sub_2D26A( sSquad_Member* pData24, int16& pData8 );
	void			sub_2D2D4();
	void			sub_2D725();
	void			sub_2D767();
	void			sub_2D7C0();
	void			sub_2D7FF();
	void			sub_2D8AF( sSprite_0* pSprite );
	int16			sub_2D91E( sSprite_0* pSprite );
	int16			sub_2DBA3( sSprite_0* pSprite );
	void			sub_2DCB0( int16 pData0 );
	void			sub_2DE2C( cSurface* pImage );
	void			sub_2E01C();
	int16			sub_2EAC2();
	void			sub_2EAC3();
	void			sub_2EACA( cSurface *pImage );
	void			sub_2EBE0( int16& pData0, int16& pData4 );
	void			Mission_Sidebar_Prepare( cSurface* pImage, int16 pData0, int16 pData4 );
	void			sub_2EBC4();
	void			sub_2EC0E( struct_6* pData20, struct_6* pData24 );
	void			sub_2ECC7( struct_6 *pData20 );
	void			Mission_Sidebar_SplitButton_Draw();
	void			sub_2EE02();
	void			sub_2EEBD();
	void			sub_2EEC8();
	void			sub_2EED3();
	void			sub_2EEDE( int16 pData4 );
	void			sub_2EF3E();
	void			sub_2EF8A();

	void			graphicsBlkPtrsPrepare();
	void			map_Tiles_Draw();
	void			map_Tiles_Draw_();
	void			video_Draw_MapTile( cSurface* pImage, uint16 pTile, uint16 pPosX, uint16 pPosY );

	void			sub_2E04C();
	void			Mission_Sidebar_Grenades_Draw( cSurface* pImage );

	void			sub_2F01D();
	void			sub_2F0A6();
	void			sub_2F0D7();
	void			Mission_Sidebar_TroopList_Draw();
	void			Mission_Sidebar_TroopList_Name_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pDataC, sRecruit* pData28 );
	void			sub_2F452();
	void			Mission_Sidebar_SquadIcon_Draw();
	int16			sub_2F4CB();
	void			sub_2F5ED( cSurface *pImage );
	void			sub_2F757();
	void			sub_2F7D7( cSurface* pImage );
	void			sub_2F7E4( cSurface* pImage, int16 pData0 );
	void			sub_2F871( cSurface* pImage );
	void			sub_2F87E( cSurface* pImage, int16 pData0 );
	int16			sub_2F90B();
	void			sub_2F9B3( cSurface* pImage );
	void			sub_2FA05();
	void			Mission_Sidebar_MapButton_Prepare();
	void			sub_2FA44( cSurface* pImage );
	void			sub_2FA5B( cSurface* pImage );
	void			sub_2FA75( cSurface* pImage );
	void			sub_2FA8F();
	void			Mission_Sidebar_Rockets_Draw( cSurface* pImage );
	void			sub_2FB95();
	void			sub_2FC1E();
	void			sub_2FC4F();
	void			sub_2FCB7();
	void			sub_2FDB8();
	int16			sub_2FF41();

	void			sub_30082();

	void			Mission_Sidebar_MapButton_RenderWrapper();
	void			sub_302DE( cSurface *pImage, int16 pData4, int16 pData8, int16 pDataC, int16 pData10 );
	void			Mission_Sidebar_MapButton_Render();
	void			sub_302C9();
	void			sub_3037A( cSurface* pImage );
	void			sub_3049B( cSurface* pImage );
	void			sub_304CB( cSurface* pImage );
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
	void			sub_30CDC();
	int16			sub_30E2A();
	void			sub_31033();
	void			sub_31075( int16 pData0 );
	void			sub_310CB();
	void			sub_311A7();
	int16			sub_313CD();
	void			sub_3169B();

	void			String_CalculateWidth( int32 pPosX, const uint8* pWidths, const char* pString );
	void			String_Print( cSurface* pImage, const uint8* pWidths, int32 pParam0, int32 pParam08, int32 pParamC, const char* pText );

	std::string		map_Filename_Get();
	std::string		map_Filename_MapGet();
	std::string		map_Filename_SptGet();

	int16			introPlayText();
	void			intro();

	void			map_ClearSpt();
	void			mission_PhaseNext();

	void			sub_12AB1();
	void			WonGame();

	void			Load_File( const std::string& pFilename );
	void			Load_PlayerBin();
	
	void			Load_Sprite_Font();
	void			Sprite_SetDataPtrToBase( const sSpriteSheet** pSpriteSheet );

	void			videoSleep();

	void			sleepLoop( int64 pMilliseconds );
	int16			ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration );

	void			paletteLoad( uint8* pBuffer, uint16 pColors, uint16 pColorID );

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

	bool			EventAdd( cEvent pEvent );

	void			Prepare();
	void			Start();
	void			Exit( unsigned int pExitCode );

};
