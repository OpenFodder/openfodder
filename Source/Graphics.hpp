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
	eSPRITE_IN_GAME			= 0,
	eSPRITE_IN_GAME2		= 1,
	eSPRITE_FONT			= 2,
	eSPRITE_HILL			= 3,
	eSPRITE_RECRUIT	= 4,
	eSPRITE_BRIEFING		= 5,
	eSPRITE_SERVICE			= 6,

	eSPRITE_RANKFONT		= 7,
	eSPRITE_PSTUFF			= 8,

	eSPRITE_BRIEFING_AMIGA_1 = 9,
	eSPRITE_BRIEFING_AMIGA_2 = 10,
	eSPRITE_BRIEFING_AMIGA_3 = 11
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

	sILBM_BMHD() {
		mWidth = 0;
		mHeight = 0;
		mPlanes = 0;
	}

	uint16 ScreenSize() const {
		return (mWidth * mHeight);
	}
};

struct sImage {
	tSharedBuffer		mData;
	cDimension			mDimension;
	cPalette			mPalette[256];
	uint8				mPlanes;

protected:
	sILBM_BMHD			mBMHD;

public:

	sImage() {
		mData = std::make_shared<std::vector<uint8>>();
		mDimension = { 0,0 };
		mPlanes = 0;
	}

	sILBM_BMHD* GetHeader() {

		if (!mBMHD.mWidth || !mBMHD.mHeight) {
			mBMHD.mWidth = mDimension.mWidth;
			mBMHD.mHeight = mDimension.mHeight;
			mBMHD.mPlanes = mPlanes;
		}
		return &mBMHD;
	}

	/*
	 * Load a Palette from a location in the loaded data
	 */
	void LoadPalette(size_t pFrom, const size_t pCount, const size_t pStartColorID = 0) {

		auto Buffer = mData->data();

		for (size_t ColorID = pStartColorID; ColorID < pStartColorID + pCount; ColorID++) {

			// Get the next color values
			mPalette[ColorID].mRed = *(Buffer + pFrom++);
			mPalette[ColorID].mGreen = *(Buffer + pFrom++);
			mPalette[ColorID].mBlue = *(Buffer + pFrom++);
		}
	}

	/**
	 * Load a Palette from a buffer (BIG ENDIAN)
	 */
	void LoadPalette_Amiga( const uint8 *pFrom, const size_t pCount, const size_t pStartColorID = 0 ) {

		uint16 color;

		for (size_t ColorID = pStartColorID; ColorID < pStartColorID + pCount; ColorID++) {

			// Get the next color codes
			color = readBEWord(pFrom);
			pFrom += 2;

			// Extract each color from the word
			//  X X X X   R3 R2 R1 R0     G3 G2 G1 G0   B3 B2 B1 B0

			mPalette[ColorID].mRed = ((color >> 8) & 0xF) << 2;
			mPalette[ColorID].mGreen = ((color >> 4) & 0xF) << 2;
			mPalette[ColorID].mBlue = ((color >> 0) & 0xF) << 2;
		}		
	}

	void CopyPalette( cPalette* pDestination, const size_t pCount, const size_t pStartColorID = 0) {

		memcpy(pDestination, &mPalette[pStartColorID], sizeof(cPalette) * pCount);
	}
};

class cGraphics : public cSingleton<cGraphics> {
	public:
	sImage				mSpriteSheet_InGame1;	// Army
	sImage				mSpriteSheet_InGame2;	// Copt

	sImage				mSpriteSheet_RankFont;

	sImage				mImageHill;
	sImage				mImageService;
	sImage				mImagePStuff;
	sImage				mImageFonts;
	sImage				mImageBriefingIntro;

protected:
	cSurface*			mImage;
	cSurface*			mImageOriginal;
	cFodder*			mFodder;


public:
						cGraphics();
	virtual				~cGraphics() { };

	virtual uint8*		GetSpriteData( uint16 pSegment ) = 0;
	virtual void		Mouse_DrawCursor() = 0;
	virtual void		Load_pStuff() = 0;

	virtual void		Load_Sprite_Font() = 0;
	virtual void		Load_Hill_Data() = 0;
	virtual void		Load_Hill_Bits() = 0;
	virtual void		Load_Service_Data() = 0;

	virtual void		Tile_Prepare_Gfx() = 0;

	virtual sImage		Decode_Image(const std::string& pFilename, const size_t pCount = 0, const size_t pPaletteOffset = 0, const size_t pStartIndex = 0) = 0;

	virtual void		Load_And_Draw_Image( const std::string &pFilename, unsigned int pColors ) = 0;

	virtual void		Map_Tiles_Draw() = 0;
	virtual void		Map_Load_Resources() = 0;

	virtual void		MapOverview_Render_Tiles( uint16 pTile, uint16 pDestX, uint16 pDestY ) = 0;

	virtual void		PaletteSetOverview() = 0;
	virtual void		PaletteSet() = 0;
	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 ) = 0;

	virtual void		video_Draw_Linear() = 0;
	virtual void		video_Draw_Sprite() = 0;

	virtual void		SetActiveSpriteSheet( eSpriteType pSpriteType ) = 0;
	virtual void		SetImage( cSurface* pImage );
	virtual void		SetImageOriginal();
	virtual bool		Sprite_OnScreen_Check() = 0;

	virtual void		Sidebar_Copy_To_Surface( int16 pStartY = 0 ) = 0;
	virtual void		Sidebar_Render_Sprite( int16 pSpriteType, int16 pX, int16 pY ) = 0;
	virtual void		Sidebar_Render_SquadNames( uint16 Data0, int16 Data4, int16 Data8, uint32*& Data20 ) = 0;
	virtual void		Recruit_Sprite_Draw( int16 pColumns, int16 pRows, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pGraphics ) = 0;

	virtual void		Briefing_Load_Resources() = 0;
	virtual void		Briefing_DrawHelicopter( uint16 pID ) = 0;

	virtual void		Recruit_Draw_Hill() = 0;
	virtual void		Recruit_Draw_HomeAway() = 0;
};
