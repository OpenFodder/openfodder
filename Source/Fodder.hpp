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

	cPosition		mMousePosition;
	uint32			mMouseButtons;

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
	int32			dword_39F7C;

	int16			dword_39F98;
	int16			dword_39F9C;
	int16			word_39FCE;

	int16			word_39FD0;
	int16			word_39FD4;

	int16			word_3A01A;
	int16			word_3A06B;
	int16			word_3A28D;

	int16			dword_3A8DB;
	uint8			byte_3A8DF;

	int16			dword_3A000;
	int16			dword_3A004;
	int16			dword_3A008;
	int16			word_3A016;
	uint16			word_3A024;
	uint16			word_3A054;
	uint32			dword_3A05A;
	uint8			byte_3A05E;
	uint16			word_3A05F;
	uint32			dword_3A39D;
	int16			word_3A3BB;
	int16			word_3A3BD;
	int32			dword_3A3FD[30];
	int32			dword_3A475[30];
	int32			dword_3A4ED[30];
	int32			dword_3A565[30];
	int32			dword_3A5DD[30];
	int32			dword_3A655[30];
	int32			dword_3A6CD[30];
	int32			dword_3A745[30];
	int32			dword_3A7BD[30];
	int32			dword_3A835[30];

	int16			word_3A9AA;
	uint16			word_3A9AC;
	uint16			word_3A9B2;
	uint16			word_3A9F7;
	int16			word_3AA17;
	int16			word_3AA19;
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
	int16			word_3ABE9;
	int16			word_3ABEB;

	int16			word_3AC19;
	int16			word_3AC21;
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

	int16			word_3B447;
	int16			word_3B4F3;

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
	
	int32*			off_3BEF3[10];
	int16			word_3BF1B;
	int16			word_3BF1E[6];

	uint16			mIntroDone;

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

	uint16			word_40054;
	uint8*			word_42062;
	uint8*			word_42066;
	int16			word_42068;
	uint16			word_4206A;
	int16			word_4206C;
	int16			word_4206E;
	uint8			byte_42070;
	uint8			byte_42071;
	int16			word_42074;
	int16			word_42076;
	uint16			word_42078;

	int16			word_42851;
	uint8*			word_42861;
	uint8*			word_42863;
	uint8*			word_42865;
	uint8*			word_42867;
	uint8*			word_42869;
	uint16*			word_4286B;
	
	uint8			mErrorCode;

	int16			mouse_Button_Status;
	int16			mouse_Pos_Column;
	int16			mouse_Pos_Row;

	sMouseData*		mouseData0;
	sMouseData*		mouseData1;

	int16			word_42072;
	uint16			word_427D4;
	
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

	void			sub_15DF0( cSurface* pImage );
	void			Mission_Brief_Name_Prepare( cSurface* pImage );
	\
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
	
	void			sub_18C45( cSurface* pImage, int32 pPosX, const sIntroString* pString );
	bool			sub_18C7A();

	uint8*			sub_2AE81( int16 *pData0, int16 *pData4 );
	void			sub_2AEB6( int16 pData0, int16 pData4, int16 *pData8, int16* pDataC );
	void			sub_2AF19( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 );
	uint8			sub_2AFF5( uint8* pSi, int16 pBx, int16 pCx );
	void			sub_2B016( uint8* pDi, uint8 pAl );

	void			sub_2E04C();

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

public:

					cFodder();
					~cFodder();

	bool			EventAdd( cEvent pEvent );

	void			Prepare();
	void			Start();
	void			Exit( unsigned int pExitCode );

};
