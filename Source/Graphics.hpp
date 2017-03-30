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

enum eSpriteType {
	eSPRITE_IN_GAME		= 0,
	eSPRITE_FONT		= 1,
	eSPRITE_HILL		= 2,
	eSPRITE_HILL_UNK	= 3,
	eSPRITE_BRIEFING	= 4,
	eSPRITE_SERVICE		= 5
};

class cFodder;

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

	uint16 ScreenSize() const {
		return (mWidth * mHeight);
	}
};

struct sImage {
	tSharedBuffer		mData;
	cDimension			mDimension;
	std::vector<int16>	mPallete;
	uint8				mPlanes;

	sImage() {
		mData = std::make_shared<std::vector<uint8>>();
		mDimension = { 0,0 };
		mPallete.resize( 512 );
		mPlanes = 0;
	}

	sILBM_BMHD GetHeader() {
		sILBM_BMHD Result;

		Result.mWidth = mDimension.mWidth;
		Result.mHeight = mDimension.mHeight;
		Result.mPlanes = mPlanes;
		return Result;
	}
};

class cGraphics : public cSingleton<cGraphics> {
	
protected:
	cSurface*			mImage;
	cSurface*			mImageOriginal;
	cFodder*			mFodder;
	sImage				mSpriteSheet_InGame1;
	sImage				mSpriteSheet_InGame2;

public:
						cGraphics();
	virtual				~cGraphics() { };

	virtual uint8*		GetSpriteData( uint16 pSegment ) = 0;
	virtual void		Mouse_DrawCursor() = 0;
	virtual void		LoadpStuff() = 0;

	virtual void		Load_Sprite_Font() = 0;
	virtual void		Load_Hill_Data() = 0;
	virtual void		Load_Hill_Bits() = 0;
	virtual void		Load_Service_Data() = 0;

	virtual void		Tile_Prepare_Gfx() = 0;
	
	virtual void		imageLoad( const std::string &pFilename, unsigned int pColors ) = 0;

	virtual void		Map_Tiles_Draw() = 0;
	virtual void		Map_Load_Resources() = 0;

	virtual void		sub_2B04B( uint16 pTile, uint16 pDestX, uint16 pDestY ) = 0;

	virtual void		PaletteSetOverview() = 0;
	virtual void		PaletteSet() = 0;
	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 ) = 0;

	virtual void		video_Draw_Linear() = 0;
	virtual void		video_Draw_Sprite() = 0;

	virtual void		SetSpritePtr( eSpriteType pSpriteType ) = 0;
	virtual void		SetImage( cSurface* pImage );
	virtual void		SetImageOriginal();
	virtual bool		Sprite_OnScreen_Check() = 0;

	virtual void		sub_144A2( int16 pStartY = 0 ) = 0;
	virtual void		sub_145AF( int16 pSpriteType, int16 pX, int16 pY ) = 0;
	virtual void		sub_17480( uint16 Data0, int16 Data4, int16 Data8, uint32*& Data20 ) = 0;
	virtual void		Recruit_Sprite_Draw( int16 pColumns, int16 pRows, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pGraphics ) = 0;

	virtual void		Briefing_Load_Resources() = 0;
	virtual void		Briefing_DrawHelicopter( uint16 pID ) = 0;

	virtual void		Recruit_Draw_Hill() = 0;
	virtual void		Recruit_Draw_HomeAway() = 0;
};
