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
	int16 mRecruitID;
	uint8 mRank;
	uint8 field_3;
	int16 field_4;
	uint16 field_6;
	uint8 field_8;
	uint8 field_9;
	uint16 mNumberOfKills; 
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
	int8	mRecruitID;
	int16	field_1;
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

class cFodder : public cSingleton < cFodder > {

	std::vector<cEvent>		mEvents;

	cResources*				mResources;
	cWindow*				mWindow;

	int16					mMapSpt_Loaded[2655];
	const sSpriteSheet**	mSpriteDataPtr;

	cSurface*		mSurfaceMapOverview;
	cSurface*		mSurfacePlayfield;

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
	uint8*			mDataBaseBlk;
	uint8*			mDataSubBlk;

	uint32			dword_37AA0;
	uint32			dword_3901A;
	uint16			word_3901E;
	int16			word_39096;
	uint16			word_390A4;
	uint16			word_390A6;
	uint16			word_390AE;
	uint16			word_390B8;
	uint16			mMapNumber;
	uint16			mMissionNumber = 0;
	uint16			mMissionPhase = 0;
	uint16			word_390CE;
	uint16			word_390D0;
	uint16			word_390D2;
	int16			word_390D4;
	uint16			word_390D6[8];
	uint16			word_390E8;
	int16			word_390EA;
	uint16			mMissionPhaseRemain;
	uint16			mMissionPhases;
	uint16			word_390F4;
	sSquad_Member	mSquad[8];
	int16			word_3915A;
	int16			word_3915E;

	int16			word_391D2[361];
	int16*			dword_394A4;
	int16*			dword_394A8;
	int16			word_394AC[361];
	int16*			dword_3977E;

	sHero			mHeroes[5];
	int16			word_397AC;
	int16			word_397AE;
	int16			word_397D2;
	int16			word_397D4;
	int16			mTroopsAvailable;
	
	int16			mButtonPressLeft, mButtonPressRight;
	int16			word_39EF8;
	int16			word_39F02;
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
	int16			word_39F34;
	int16			word_39F38;
	int16			word_39F4C;
	int16			word_39F50;
	int32			dword_39F42;
	int32			dword_39F46;
	int32			dword_39F56;
	int32			dword_39F7C;

	int16			dword_39F98;
	int16			dword_39F9C;
	int8*			word_39FCE;

	int16			word_39FD0;
	int16			word_39FD4;

	int16			word_39FF2;
	int16			word_39FF4;
	int16			word_39FF6;
	int16			word_39FF8;
	int16			word_39FFA;

	int16			word_3A01A;
	int16			word_3A06B;
	int16			word_3A28D;

	int16			dword_3A8DB;
	uint8			byte_3A8DF;

	int16			dword_3A000;
	int16			dword_3A004;
	int16			dword_3A008;
	int16			word_3A00C;
	int16			word_3A00E;
	int16			word_3A010;
	int16			word_3A016;
	uint16			word_3A024;
	uint16			word_3A054;
	uint32			dword_3A05A;
	uint8			byte_3A05E;
	uint16			word_3A05F;
	int16			word_3A399;
	uint32			dword_3A39D;
	int16			word_3A3BB;
	int16			word_3A3BD;
	int16			word_3A3FD[60];
	int16			word_3A475[60];
	int16			word_3A4ED[60];
	int16			word_3A565[60];
	int16			word_3A5DD[60];
	int16			word_3A655[60];
	int16			word_3A6CD[60];
	int16			word_3A745[60];
	int16			word_3A7BD[60];
	int16			word_3A835[60];
	int16			word_3A8CF;

	int16			word_3A9AA;
	uint16			word_3A9AC;
	uint16			word_3A9B2;
	int16			word_3A9B4;
	int16			word_3A9C6;
	int16			word_3A9CE;
	int16			word_3A9D0;
	uint16			word_3A9F7;
	int16			word_3AA17;
	int16			word_3AA19;
	int16			word_3AA1D;
	int16			word_3AA41;
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
	int16			word_3ABAD;
	int16			word_3ABE9;
	int16			word_3ABEB;

	int16			word_3AC19;
	int16			word_3AC21;
	int16			word_3AC3F[6];
	int32			dword_3B1CB;
	int16			word_3B1CF[0x0F];
	int16			word_3B1ED;
	int16			word_3B1EF;
	int16			word_3B1F1;

	const struct_4*		dword_3B1FB;

	int16			mMap_TileSet;
	int16			word_3B2CD;
	int16			word_3B2CF;
	int16			word_3B2FD;
	int16			word_3B2FF;
	int16			word_3B301;
	int16			word_3B303;

	int16			word_3B305;
	int16			word_3B307;
	
	int16			mMapGoals[8];
	int16			word_3B447;
	int16			word_3B4DB;
	int16			word_3B4F1;
	int16			word_3B4F3;
	int16			word_3B4F5;

	int16			word_3B60C;
	int16			word_3B60E;
	int16			word_3B610;
	int16			word_3B612;
	int16			word_3B614;

	uint16*			mMapSptPtr;
	size_t			mMapSptSize;

	int16*			word_3BDAD;
	int16			word_3BDAF;
	uint16			word_3BDB1;
	uint16			word_3BDB3;
	uint16			word_3BDB5;
	

	uint16			word_3BDE7;
	uint16			word_3BDE9;
	uint16			word_3BDEB;
	
	int32			dword_3BE03[9];
	int32			dword_3BE27[9];
	int32			dword_3BE4B[9];
	int32			dword_3BE6F[9];
	int32			dword_3BE93[9];
	
	int32*			off_3BDEF[5];
	uint16			word_3BEB9;
	uint16			word_3BEBB;
	uint16			word_3BEBD;
	uint16			word_3BEC1;
	uint16			word_3BEC3;
	uint16			word_3BEC9;
	uint16			word_3BED5[5];
	uint16			word_3BEDF[10];
	
	int16*			off_3BEF3[10];
	int8			byte_3BF1B[3];
	int16			word_3BF1E[6];

	uint16			mIntroDone;

	int16			word_3C09D[240];
	int16			graphicsSub0[160];
	int16			graphicsBaseBht[960];
	int16			graphicsSub0Bht[640];
	int16			word_3D03D[240];
	int16			word_3D21D[161];

	int16			word_3D473;
	int16			word_3D475;

	uint16			byte_3DDA2[0x30];
	int16			word_3E0E5[0x18];

	uint8*			word_3E1B7;
	size_t			word_3E1B7_size;
	
	int16			word_3E75B;

		/* Configuration Settings (SETUP.DAT) */
	uint16			word_3E7ED;
	uint16			mEffectDriver;
	uint16			mMusicDriver;
	/* End Configuration Settings */

	int32			dword_3F946;
	int32			dword_3F94A;
	int16			word_3F94E;
	int16			word_3F950;

	int32			dword_3E9A3[70];
	int16			word_3FA1F;
	int16			mKeyControlPressed;

	uint16			word_40054;
	uint16			word_40056;
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

protected:

	uint8*			GetSpriteData( uint16 pSegment );
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
	void			map_Load_TileSet();

	void			sub_12083();
	void			sub_126DD();
	std::string		sub_12AA1( std::string pBase, const char* pFinish );

	void			Map_Overview_Prepare();
	void			map_SetTileType();

	void			sub_13800();
	void			sub_13C1C( cSurface* pImage, int32 pParam00, int32 pParam0C, int32 pParam04, int32 pParam08 );
	void			sub_13C8A( cSurface* pImage, int16 pData0, int16 pData4, int16 pPosX, int16 pPosY );

	void			video_Draw_Sprite_( cSurface* pImage );
	void			video_Draw_Linear_To_Planar(  cSurface* pImage );
	bool			sub_1429B();
	void			sub_144A2( cSurface* pImage );
	void 			sub_145AF( int16 pData0, int16 pData8, int16 pDataC );
	void			sub_14FF5( cSurface *pImage );

	void			Mission_Brief();

	void			sub_1590B();
	void			sub_1594F();
	void			sub_159A6();
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
	void			sub_16C45( int16** pDi, int16* pSource );
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

	void			video_Draw_Unk_2( cSurface* pImage );
	/* End Recruitment */
	
	void			Briefing_Show( cSurface* pImage );
	void			sub_18908();
	void			Briefing_Wait();
	void			Briefing_Draw_With( cSurface* pImage );
	void			Briefing_DrawBox( cSurface* pImage, int16 pData0, int16 pData4, int16 pData8, int16 pDataC, int16 pData10 );

	void			sub_18BDF( cSurface* pImage, int16 pBx, int16 pCx, uint8 pSi );
	void			Brief_Draw_Horizontal_Line( cSurface* pImage, int16 pBx, int16 pDx, int16 pCx, uint8 pSi );

	void			sub_18C45( cSurface* pImage, int32 pPosX, const sIntroString* pString );
	bool			sub_18C7A();
	void			Brief_Draw_Vertical_Line( cSurface* pImage, int16 pBx, int16 pDx, int16 pCx, uint8 pSi );
	void			sub_18D5E();
	void			sub_18DD3();

	void			sub_18E2E( int16 *pData20 );
	int16			sub_1E05A( sSprite_0* pSprite );
	int16			sub_1F21E( sSprite_0* pSprite );
	void			sub_1F5A0( sSprite_0* pSprite );

	void			sub_22AA9( sSprite_0* pSprite );
	
	int16			sub_2A030();
	int16			sub_2A7F7( sSprite_0* pSprite, int16& pData0, int16& pData4 );
	int16			sub_2A839( int16& pData0, int16& pData4, int16& pData10, int16& pData14 );

	uint8*			sub_2AE81( int16 *pData0, int16 *pData4 );
	void			sub_2AEB6( int16 pData0, int16 pData4, int16 *pData8, int16* pDataC );
	void			sub_2AF19( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 );
	uint8			sub_2AFF5( uint8* pSi, int16 pBx, int16 pCx );
	void			sub_2B016( uint8* pDi, uint8 pAl );
	void			sub_2B04B( uint8* pTileGraphicPtr, uint16 pDestX, uint16 pDestY );

	void			graphicsBlkPtrsPrepare();
	void			map_Tiles_Draw();
	void			video_Draw_MapTile( uint16 pTile, uint16 pPosX, uint16 pPosY );

	void			sub_2E0v4C();

	void			sub_301F7();

	void			String_CalculateWidth( int32 pPosX, uint8* pWidths, const char* pString );
	void			String_Print( cSurface* pImage, uint8* pWidths, int32 pParam0, int32 pParam08, int32 pParamC, const char* pText );

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
	void			mouse_Handle();
	void			mouse_ButtonCheck();
	void			mouse_GetData();

	void			eventProcess();
	void			keyProcess( uint8 pKeyCode, bool pPressed );

	void			sub_2E04C();
public:

					cFodder();
					~cFodder();

	bool			EventAdd( cEvent pEvent );

	void			Prepare();
	void			Start();
	void			Exit( unsigned int pExitCode );

};
