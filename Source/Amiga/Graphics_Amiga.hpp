/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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

const extern sSpriteSheet* mSpriteSheetTypes_InGame_Amiga[];
const extern sSpriteSheet* mSpriteSheetTypes_InGame_Amiga_TheOne[];
const extern sSpriteSheet* mSpriteSheetTypes_Font_Amiga[];
const extern sSpriteSheet* mSpriteSheetTypes_Recruit_Amiga[];
const extern sSpriteSheet* mSpriteSheetTypes_Hill_Amiga[];
const extern sSpriteSheet* mSpriteSheetTypes_Briefing_Amiga[];
const extern sSpriteSheet* mSpriteSheetTypes_Service_Amiga[];



struct sHillOverlay_Amiga {
	int16	mSpriteType;
	int16	mFrame;
	int16	mX;
	int16	mY;
};

struct sStruct0_Amiga {
	int16 mSpriteSheet;	// 0 = mDataHillData  : 1 = mSpriteSheet_RankFont
	int16 mOffset;
	int16 mWidth;
	int16 mHeight;
};

class cGraphics_Amiga : public cGraphics {
protected:

	tSharedBuffer 		mBlkData;

	sILBM_BMHD*			mBMHD_Current;
	uint16				mCursorPalette;


	virtual void		DecodePalette(const uint8* pData, size_t pColorID, const size_t pColors);
	virtual sImage		DecodeIFF(const std::string& pFilename);

	public:
						cGraphics_Amiga();
	virtual				~cGraphics_Amiga();

	virtual sImage		Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset = 0, const size_t pStartIndex = 0);

	virtual void		SetCursorPalette( uint16 pIndex );
	
	virtual void		DrawPixel(uint8* pSource, uint8* pDestination, uint16 pSourceX, uint16 pSourceY, uint16 pX, uint16 pY);
	virtual void		DrawPixels_8( uint8* pSource, uint8* pDestination );
	virtual void		DrawPixels_16( uint8* pSource, uint8* pDestination, const uint8 pPalleteIndex );

	virtual void		Load_And_Draw_Image( const std::string &pFilename, unsigned int pColors = 0, size_t pBackColor = 0 );

	virtual uint8*		GetSpriteData( uint16 pSegment );
	virtual void		Mouse_DrawCursor();
	virtual void		Tile_Prepare_Gfx();
	virtual void		Load_pStuff();
	virtual void		Load_Sprite_Font();
	virtual void		Load_Hill_Data();
	virtual void		Load_Service_Data();

	virtual void		Map_Load_Resources();
	virtual void		Map_Tile_Draw(cSurface *pTarget, uint16 pTile, uint16 pX, uint16 pY, uint16 pOffset);
	virtual void		MapTiles_Draw();
	virtual void		Hill_Prepare_Overlays();
	virtual void		MapOverview_Render_Tiles( uint16 pTile, uint16 pDestX, uint16 pDestY );
	
	virtual void		PaletteSetOverview();
	virtual void		PaletteSet(cSurface *pTarget);
	virtual void		PaletteBriefingSet();

	virtual void		PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID = 0 );

	virtual void		Video_Draw_16_Offset( int16 pCx );
	virtual void		Video_Draw_16( const uint8* RowPallete = 0 );
	virtual void		Video_Draw_8( cSurface *pTarget = 0, const uint8* RowPallete = 0 );
    virtual void		Video_Draw_8_Alt( const uint8* RowPallete = 0 );

	virtual void		SetActiveSpriteSheet(eGFX_Types pSpriteType );

	virtual void		Sidebar_Copy_To_Surface( int16 pStartY );
	virtual void		Sidebar_Copy_Sprite_To_ScreenBufPtr( int16 pSpriteType, size_t pX, size_t pY );
	virtual void		Sidebar_Copy_ScreenBuffer( uint16 pRow, int16 pRows, int16 pCopyToScreen, uint32*& pBuffer);

	virtual void		Recruit_Sprite_Draw(int16 pRows, int16 pColumns, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 );

	virtual bool		Sprite_OnScreen_Check(  );
	virtual bool		Sprite_OnScreen_Check( bool p16bit = false );

	virtual void		Mission_Intro_Play( const bool pShowHelicopter, const eTileTypes pTileset);
	virtual void		Mission_Intro_Load_Resources(const eTileTypes pTileset);
	virtual void		Mission_Intro_DrawHelicopter( uint16 pID );

	virtual void		Recruit_Draw_Hill();
	virtual void		Recruit_Draw_HomeAway();
	virtual void		Service_Draw( int16 pSpriteID, int16 pX, int16 pY);
};
