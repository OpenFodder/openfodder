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

class cGraphics_Amiga : public cGraphics {

	uint8*				mBlkData;
	uint16				mPaletteArmy[0x10];
	uint16				mPaletteCopt[0x10];
	uint16				mPalletePStuff[0x10];
	uint16				mPalleteAfx[0x20];
	uint8*				mPalette;
	size_t				mPaletteSize;
	sILBM_BMHD			mBMHDArmy;
	sILBM_BMHD			mBMHDCopt;
	sILBM_BMHD			mBMHDPStuff;
	sILBM_BMHD			mBMHDAfx;

	sILBM_BMHD*			mBMHD_Current;
	uint16				mCursorPalette;

	protected:


	public:
						cGraphics_Amiga();

	virtual bool		DecodeIFF( uint8* pData, uint8* pDataDest, sILBM_BMHD* pBMHD, uint16* pPalette );

	virtual void		LoadAFXMenu();
	virtual void		SetCursorPalette( uint16 pIndex );
	
	virtual void		DrawPixels_8( uint8* pSource, uint8* pDestination );
	virtual void		DrawPixels_16( uint8* pSource, uint8* pDestination );

	virtual uint8*		GetSpriteData( uint16 pSegment );
	virtual void		Mouse_DrawCursor();

	virtual void		graphicsBlkPtrsPrepare();
	virtual void		LoadpStuff();
	virtual void		map_Tiles_Draw();
	virtual void		map_Load_Resources();
	virtual void		PaletteSet();

	virtual void		video_Draw_Linear();
	virtual void		video_Draw_Sprite();

	virtual void		SetSpritePtr( eSpriteType pSpriteType );

	virtual void		sub_144A2();
	virtual void		sub_145AF( int16 pData0, int16 pData8, int16 pDataC );
};
