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

struct sMission_unk0 {
	int16 field_0;
	uint8 field_2;
	uint8 field_3;
	uint16 field_4;
	uint16 field_6;
	uint16 field_8;
	uint16 field_A; 
};

struct sSpriteSheet {
	int16 field_0;
	int16 field_2;
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

struct struct_0 {
	int16	field_0;
	int16	field_2;
	int16	field_4;
};

struct struct_1 {
	int8	field_0;
	int16	field_1;
	int16	field_3;
};

struct struct_2 {
	int16	field_0;
	int16	field_2;
	int16	field_4;
	int16	field_6;
};

struct struct_Troops {
	char field_0[7];	// increased to 7, to allow the null terminator
	uint8 field_6;
	uint8 field_7;
	uint8 field_8;
};

struct struct_3 {
	int16	field_0;
	int16	field_2;
	int16	field_4;
	int16	field_6;
	int16*	field_8;
};

class cFodder : public cSingleton < cFodder > {

	std::vector<cEvent>		mEvents;

	cResources*				mResources;
	cWindow*				mWindow;

	int16					mMapSpt_Loaded[2655];
	const sSpriteSheet**	mSpriteDataBasePtr;
	const sSpriteSheet**	mSpriteDataPtr;

	cPosition		mMousePosition;
	uint32			mMouseButtons;

	uint8*			mDataPStuff;
	uint8* 			mDataHillBits;
	
	size_t			mDataPStuffSize;
	size_t			mDataHillBitsSize;

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
	uint16			word_390D6;
	uint16			word_390E8;
	int16			word_390EA;
	uint16			mMissionPhaseRemain;
	uint16			mMissionPhases;
	sMission_unk0	stru_390FA[8];
	int16			word_3915A;
	int16			word_3915E;

	int16			word_391D2[361];
	int16*			dword_394A4;
	int16*			dword_394A8;
	int16			word_394AC[361];
	int16*			dword_3977E;

	struct_1		stru_3978E[5];

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
	
	int16			word_3A06B;
	int16			word_3A28D;

	int16			dword_3A8DB;
	uint8			byte_3A8DF;

	int16			dword_3A000;
	int16			dword_3A004;
	int16			dword_3A008;
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
	int16			word_3AA55;
	int16			word_3AAC7;
	int16			word_3AAD1;

	int16			word_3AA43;
	int16			word_3AB39;
	int16			word_3ABA7;

	int16			word_3AC19;
	int16			word_3AC21;
	
	const struct_0*		dword_3B1FB;

	int16			word_3B2CD;
	int16			word_3B2CF;
	int16			word_3B301;
	int16			word_3B303;

	int16			word_3B305;
	int16			word_3B307;

	int16			word_3B447;
	int16			word_3B4F3;

	uint16*			mMapSptPtr;
	size_t			mMapSptSize;

	int16*			word_3BDAD;
	uint16			word_3BDAF;
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
	
	uint8*			word_3E1B7;
	size_t			word_3E1B7_size;
	
	int16			word_3E75B;

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

	/* Configuration Settings (SETUP.DAT) */
	uint16			word_3E7ED;
	uint16			mEffectDriver;
	uint16			mMusicDriver;
	/* End Configuration Settings */

	uint8			mErrorCode;

	int16			mouse_Button_Status;
	int16			mouse_Pos_Column;
	int16			mouse_Pos_Row;

	sMouseData*		mouseData0;
	sMouseData*		mouseData1;

	uint16			word_427D4;
	int64			mTicks;
	int64			mTicksDiff;

protected:

	void			sub_10BBC();
	void			sub_10B6D();
	void			sub_10BFA();
	void			sub_10D61();
	void			sub_10D9F();
	void			sub_10DEC();
	void			sub_10EA4();
	void			sub_10EC3();
	void			sub_10EE3();
	void			map_Load_Spt();
	
	void			sub_13C1C( cSurface* pImage, int32 pParam0, int32 pPosX, int32 NextChar, int32 pParam08 );
	void			video_Draw_Sprite_( cSurface* pImage );
	void			video_Draw_Linear_To_Planar(  cSurface* pImage );
	bool			sub_1429B();
	void 			sub_145AF( int16 pData0, int16 pData8, int16 pDataC );
	
	/* Recruitment */
	void			Show_Recruits();
	void			Recruit_Draw_Hill( cSurface *pImage );
	void			sub_16BC3();
	void			sub_16C45( int16** pDi, int16* pSource );
	void			sub_16C6C();
	void			Recruit_Draw_LeftMenu( cSurface *pImage );
	void			sub_16DF2();
	void			sub_16F78();
	void			sub_17B64();

	void			video_Draw_Unk_2( cSurface* pImage );
	/* End Recruitment */
	
	void			sub_18C45( cSurface* pImage, int32 pPosX, const sIntroString* pString );

	uint32			sub_2AE81( int16 *pData0, int16 *pData4 );
	void			sub_2AEB6( int16 pData0, int16 pData4, int16 *pData8, int16* pDataC );
	void			sub_2AF19( int16 pData0, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, int16* pData20 );

	void			String_CalculateWidth( int32 pPosX, uint8* pWidths, const sIntroString* pString );
	void			String_Print( cSurface* pImage, uint8* pWidths, int32 pPosX, int32 pParam0, int32 pParam08, const char* pText );

	std::string		map_Filename_Get();
	std::string		map_Filename_MapGet();
	std::string		map_Filename_SptGet();
	void			map_Troops_Prepare();

	int16			introPlayText();
	void			intro();

	void			map_ClearSpt();
	void			mission_PhaseNext();

	void			sub_12AB1();
	void			WonGame();

	void			Load_File( const std::string& pFilename );
	void			Load_PlayerBin();
	
	void			Load_Sprite_Font();
	void			Sprite_SetDataPtrToBase();

	void			videoSleep();

	void			sleepLoop( int64 pMilliseconds );
	int16			ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration );

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
