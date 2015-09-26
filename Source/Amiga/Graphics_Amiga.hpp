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

const extern sSpriteSheet* off_8BFB8[];
const extern sSpriteSheet* mSpriteSheetPtr_Font_Amiga[];
const extern sSpriteSheet* mHillSpriteSheetPtr_Amiga[];
const extern sSpriteSheet* off_90F10[];

struct sILBM_BMHD {
	uint16	mWidth, mHeight;
	uint16	mX, mY;
	uint8	mPlanes;
	uint8	mMask;
	uint8	mCompression;
	uint8	mReserved;
	uint8	mTransparentColor;
	uint8	mAspectX, mAspectY;
	uint16	mPageWidth, mPageHeight;
};

struct sStruct3_Amiga {
	int16	field_0;
	int16	field_2;
	int16	field_4;
	int16	field_6;
};

class cGraphics_Amiga : public cGraphics {
public:
	uint8*				mBlkData;
	uint16				mPalette[0x40];
	uint16				mPaletteArmy[0x10];
	uint16				mPaletteCopt[0x10];
	uint16				mPalletePStuff[0x10];
	uint16				mPalleteHill[0x10];
	uint16				mPalleteFont[0x10];

	size_t				mPaletteSize;

	sILBM_BMHD			mBMHDArmy;
	sILBM_BMHD			mBMHDCopt;
	sILBM_BMHD			mBMHDPStuff;
	sILBM_BMHD			mBMHDFont;
	sILBM_BMHD			mBMHDHill;

	sILBM_BMHD*			mBMHD_Current;
	uint16				mCursorPalette;

	public:
						cGraphics_Amiga();

	virtual bool		DecodeIFF( uint8* pData, uint8* pDataDest, sILBM_BMHD* pBMHD, uint16* pPalette );

	virtual void		SetCursorPalette( uint16 pIndex );
	
	virtual void		DrawPixels_8( uint8* pSource, uint8* pDestination );
	virtual void		DrawPixels_16( uint8* pSource, uint8* pDestination );

	virtual void		imageLoad( const std::string &pFilename, unsigned int pColors = 0 );

	virtual uint8*		GetSpriteData( uint16 pSegment );
	virtual void		Mouse_DrawCursor();

	virtual void		graphicsBlkPtrsPrepare();
	virtual void		LoadpStuff();
	virtual void		Load_Sprite_Font();
	virtual void		Load_Hill_Data();
	
	virtual void		map_Load_Resources();
	virtual void		map_Tiles_Draw();
	virtual void		sub_A5B46();
	virtual void		sub_2B04B( uint16 pTile, uint16 pDestX, uint16 pDestY );

	virtual void		PaletteSet();
	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 );

	virtual void		video_Draw_Linear();
	virtual void		video_Draw_Sprite();

	virtual void		SetSpritePtr( eSpriteType pSpriteType );

	virtual void		sub_144A2();
	virtual void		sub_145AF( int16 pData0, int16 pData8, int16 pDataC );
	virtual void		sub_2AF19( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 );

	virtual void		Recruit_Draw_Hill();
	virtual void		Recruit_Draw_HomeAway();
};
