/*
 *  Cannon Fodder
 *  ------------------------
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

const extern sSpriteSheet* off_8BFB8[];
const extern sSpriteSheet* mSpriteSheetPtr_Font_Amiga[];
const extern sSpriteSheet* mHillSpriteSheetPtr_Amiga[];
const extern sSpriteSheet* off_90F10[];
const extern sSpriteSheet* mSpriteSheetPtr_Briefing_Amiga[];
const extern sSpriteSheet* mSpriteSheetPtr_Font_Rank_Amiga[];



struct sStruct3_Amiga {
	int16	field_0;
	int16	field_2;
	int16	field_4;
	int16	field_6;
};

struct sStruct0_Amiga {
	int16 mSpriteSheet;	// 0 = mDataHillData  : 1 = mSpriteSheet_RankFont
	int16 mOffset;
	int16 mWidth;
	int16 mHeight;
};

class cGraphics_Amiga : public cGraphics {
public:
	tSharedBuffer 		mBlkData;
	uint8				mPalette[0xFF];
	uint8				mPaletteArmy[0x20];
	uint8				mPaletteCopt[0x20];
	uint8				mPalletePStuff[0x20];
	uint8				mPalleteHill[0x20];
	uint8				mPalleteFont[0x20];
	uint8				mPaletteBrief[0x1FE];

	uint8*				mPlayData;

	size_t				mPaletteSize;

	sILBM_BMHD			mBMHDPStuff;
	sILBM_BMHD			mBMHDFont;
	sILBM_BMHD			mBMHDHill;
	sILBM_BMHD			mBMHDPlay;

	sILBM_BMHD*			mBMHD_Current;
	uint16				mCursorPalette;

	public:
						cGraphics_Amiga();
	virtual				~cGraphics_Amiga();
	std::tuple<tSharedBuffer, sILBM_BMHD> DecodeIFF( const std::string& pFilename, uint8* pPalette );
	virtual bool		DecodeIFF( uint8* pData, uint8* pDataDest, sILBM_BMHD* pBMHD, uint8* pPalette );
	virtual sImage		DecodeIFF( const std::string& pFilename );

	virtual void		SetCursorPalette( uint16 pIndex );
	
	virtual void		DrawPixels_8( uint8* pSource, uint8* pDestination );
	virtual void		DrawPixels_16( uint8* pSource, uint8* pDestination );

	virtual void		imageLoad( const std::string &pFilename, unsigned int pColors = 0 );

	virtual uint8*		GetSpriteData( uint16 pSegment );
	virtual void		Mouse_DrawCursor();
	virtual void		Tile_Prepare_Gfx();
	virtual void		LoadpStuff();
	virtual void		Load_Sprite_Font();
	virtual void		Load_Hill_Data();
	virtual void		Load_Hill_Bits();
	virtual void		Load_Service_Data();

	virtual void		Map_Load_Resources();
	virtual void		Map_Tiles_Draw();
	virtual void		sub_A5B46();
	virtual void		sub_2B04B( uint16 pTile, uint16 pDestX, uint16 pDestY );
	
	virtual void		PaletteSetOverview();
	virtual void		PaletteSet();
	virtual void		PaletteBriefingSet();

	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 );

	virtual void		video_Draw_Linear();
	virtual void		video_Draw_Sprite();

	virtual void		SetSpritePtr( eSpriteType pSpriteType );

	virtual void		sub_144A2( int16 pStartY );
	virtual void		sub_145AF( int16 pSpriteType, int16 pX, int16 pY );
	virtual void		sub_17480( uint16 Data0, int16 Data4, int16 Data8, uint32*& Data20 );

	virtual void		Recruit_Sprite_Draw( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 );

	virtual bool		Sprite_OnScreen_Check(  );
	virtual bool		Sprite_OnScreen_Check( bool p16bit = false );

	virtual void		Briefing_Load_Resources();
	virtual void		Briefing_DrawHelicopter( uint16 pID );

	virtual void		Recruit_Draw_Hill();
	virtual void		Recruit_Draw_HomeAway();
	virtual void		Service_Draw( int16 pSpriteID, int16 pX, int16 pY);
};
