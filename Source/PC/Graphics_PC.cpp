/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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

#include "stdafx.hpp"
#include "PC/SpriteData_PC.hpp"

std::vector<cPosition> BackgroundPositions[] = {
    {   // eTileTypes_Jungle
        { 0x30, 0x0C64 * 4 },
        { 0x38, 0x102C * 4 },
        { 0x12, 0x1D3C * 4 },
        { 0x5C, 0x236C * 4 },
        { 0x2D, 0x33EC * 4 }
    },
    {   // eTileTypes_Desert
        { 0x3A, 0x0C64 * 4 },
        { 0x4C, 0x139C * 4 },
        { 0x30, 0x1CE4 * 4 },
        { 0x40, 0x2D64 * 4 },
        { 0x30, 0x32E4 * 4 }
    },
    {   // eTileTypes_Ice
        { 0x24, 0x0C64 * 4 },
        { 0x42, 0x102C * 4 },
        { 0x18, 0x1CE4 * 4 },
        { 0x58, 0x2524 * 4 },
        { 0x2E, 0x3394 * 4 }
    },
    {   // eTileTypes_Moors
        { 0x1D, 0x0C64 * 4 },
        { 0x40, 0x1134 * 4 },
        { 0x06, 0x2524 * 4 },
        { 0x52, 0x2734 * 4 },
        { 0x30, 0x32E4 * 4 }
    },
    {   // eTileTypes_Int
        { 0x40, 0x0C64 * 4 },
        { 0x2F, 0x16B4 * 4 },
        { 0x22, 0x1B2C * 4 },
        { 0x53, 0x26DC * 4 },
        { 0x23, 0x375C * 4 }
    }
};

cGraphics_PC::~cGraphics_PC() {

}

uint8* cGraphics_PC::GetSpriteData( uint16 pSegment ) {
	
	switch ( pSegment ) {
		case eGFX_IN_GAME:
			return mSpriteSheet_InGame1.mData->data();
			break;

		case eGFX_IN_GAME2:
			return mSpriteSheet_InGame2.mData->data();
			break;
		
		case eGFX_RANKFONT:
			return mSpriteSheet_RankFont.mData->data();
			break;
		
		case eGFX_FONT:
			return mImageFonts.mData->data();
			break;

		case eGFX_PSTUFF:
			return mImagePStuff.mData->data();
			break;
		
		case eGFX_RECRUIT:
			return mImageRecruit.mData->data();
			break;

		case eGFX_HILL:
			return mImageHillSprites.mData->data();
			break;

		case eGFX_SERVICE:
			return mImageService.mData->data();
			break;

		default:
			std::cout << "cGraphics_PC::GetSpriteData: Invalid ID\n";
			break;
	}
	return 0;
}

void cGraphics_PC::Mouse_DrawCursor() {
	
	const sSpriteSheet_pstuff* di = &mSpriteSheet_PStuff[mFodder->mMouseSpriteCurrent];

	mFodder->mVideo_Draw_Columns = di->mColumns;
	mFodder->mVideo_Draw_Rows = di->mRows;

	int16 ax = di->mY * 160;
	int16 bx = di->mX >> 1;

	mFodder->mVideo_Draw_FrameDataPtr = di->GetGraphicsPtr(ax +bx);
	mFodder->mVideo_Draw_PaletteIndex = 0xF0;
	
	Video_Draw_8();
}

void cGraphics_PC::SetActiveSpriteSheet( eGFX_Types pSpriteType ) {
	
	switch (pSpriteType) {
		case eGFX_IN_GAME:
            if (mFodder->mVersionCurrent->isCannonFodder2())
                mFodder->SetActiveSpriteSheetPtr(mSpriteSheetTypes_InGame_PC2);
            else
    			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_InGame_PC );
			return;

		case eGFX_FONT:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Font_PC );
			return;

		case eGFX_RECRUIT:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Recruit_PC );
			return;

		case eGFX_HILL:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Hill_PC );
			return;
		
		case eGFX_BRIEFING:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Briefing_PC );
			return;

		case eGFX_SERVICE:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Service_PC );
			return;

        default:
            std::cout << "Unknown sprite sheet\n";
            exit(1);
	}
}

sImage cGraphics_PC::Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset, const size_t pStartIndex) {

	sImage TmpImage;

	TmpImage.mData = g_Resource->fileGet(pFilename);
	TmpImage.LoadPalette(pPaletteOffset, pCount, pStartIndex);
	TmpImage.CopyPalette(&mPalette[pStartIndex], pCount, pStartIndex);

	return TmpImage;
}

void cGraphics_PC::Load_pStuff() {

	mImagePStuff = Decode_Image("pstuff.dat", 0x10, 0xA000, 0xF0);
}

void cGraphics_PC::Load_Sprite_Font() {
	
	mImageFonts = Decode_Image("font.dat", 0x10, 0xA000, 0xD0);

	SetActiveSpriteSheet( eGFX_FONT );
}

void cGraphics_PC::Load_Hill_Data() {

	mImageHillBackground = Decode_Image("hill.dat", 0x50, 0xFA00, 0x00);
	if (!mImageHillBackground.mData->size())
		return;

	mImageRecruit = Decode_Image("hillbits.dat", 0x10, 0x6900, 0xB0);

	// Parts of this surface have the recruits from mImageRecruit copied onto it
	mImageHillSprites = Decode_Image("hill.dat", 0x50, 0xFA00, 0x00);
	for (uint32 x = 0; x < 0xA000; ++x) {
		mImageHillSprites.mData->data()[x] = 0;
	}
}

void cGraphics_PC::Load_Service_Data() {
	
	mSpriteSheet_RankFont = Decode_Image("rankfont.dat", 0x80, 0xA000, 0x40);
	mImageService = Decode_Image("morphbig.dat", 0x40, 0xFA00, 0x00);
}

void cGraphics_PC::Tile_Prepare_Gfx() {
	uint16 TileOffset = 0, Tile = 0;

    for (auto& TilePtr : mTile_Gfx_Ptrs) {
        TilePtr = 0;
    }

	for (uint16 cx = 0; cx < 240; ++cx) {

		mTile_Gfx_Ptrs[cx + 0x00] = mFodder->mTile_BaseBlk->data() + TileOffset;
		mTile_Gfx_Ptrs[cx + 0xF0] = mFodder->mTile_SubBlk->data() + TileOffset;

		++Tile;
		TileOffset += 0x10;

		// 20 Tiles per row, have we reached end of row?
		if (Tile % 20 == 0) {
			Tile = 0;

			// Skip the rest of the rows for the current tile (240 * 20)
			TileOffset += 0x12C0;
		}
	}			
}

void cGraphics_PC::PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID ) {
	size_t colorStartID = pColorID;

	if (pColors >= g_MaxColors)
		pColors = g_MaxColors - 1;

	for (; pColorID < pColors + colorStartID; pColorID++) {

		// Get the next color values
		mPalette[pColorID].mRed = *pBuffer++;
		mPalette[pColorID].mGreen = *pBuffer++;
		mPalette[pColorID].mBlue = *pBuffer++;
	}
}

void cGraphics_PC::PaletteSetOverview() {

	mFodder->mSurfaceMapOverview->paletteSet( mPalette, 0, g_MaxColors, true );
}

void cGraphics_PC::PaletteSet(cSurface *pTarget) {
	if (!pTarget)
		pTarget = mSurface;

	pTarget->paletteSet( mPalette );
}

void cGraphics_PC::Map_Tile_Draw( cSurface *pTarget, uint16 pTile, uint16 pX, uint16 pY, uint16 pOffset) {
	uint8* Target = pTarget->GetSurfaceBuffer();
	
	pX *= (16 + pOffset);

	Target += (pY * (16 + pOffset)) * pTarget->GetWidth();
	Target += pX;

	uint8* TilePtr = mTile_Gfx_Ptrs[pTile];

	for (uint16 i = 0; i < 16; ++i) {

        memcpy(Target, TilePtr, 16);

		TilePtr += 320;
		Target += pTarget->GetWidth();
	}
}

void cGraphics_PC::MapTiles_Draw() {

    uint8* Target = mSurface->GetSurfaceBuffer();

    uint8* CurrentMapPtr = mFodder->mMap->data() + mFodder->mMapTile_Ptr;

    // Y
    for (uint16 cx = 0; cx <= g_Fodder->getWindowRows() + 1; ++cx) {
        uint8* MapPtr = CurrentMapPtr;
        uint8* TargetRow = Target;

        uint16 StartY = 0;

        if (cx == 0)
            StartY = mFodder->mMapTile_RowOffset;

        // X
        for (uint16 cx2 = 0; cx2 <= g_Fodder->getWindowColumns() + 1; ++cx2) {
            uint8* TargetTmp = TargetRow;

            // Verify we are inside the actual map data
            if (MapPtr >= mFodder->mMap->data()) {

                if (MapPtr >= mFodder->mMap->data() + mFodder->mMap->size())
                    continue;

                uint16 Tile = readLE<uint16>(MapPtr) & 0x1FF;
                if (Tile > 0x1DF)
                    Tile = 0;

                uint8* TilePtr = mTile_Gfx_Ptrs[Tile];
                uint16 StartX = 0;

                TilePtr += StartY * 0x140;

                if (cx2 == 0)
                    StartX = mFodder->mMapTile_ColumnOffset;

                // Each Tile Row
                for (uint16 i = StartY; i < 16; ++i) {

                    memcpy(TargetTmp, TilePtr + StartX, 16 - StartX);

                    TilePtr += 0x140;
                    TargetTmp += mSurface->GetWidth();
                }

                MapPtr += 2;
                TargetRow += (16 - StartX);
            }
        }

        Target += mSurface->GetWidth() * (16 - StartY);
        CurrentMapPtr += mFodder->mMapLoaded->getWidth() << 1;
    }
    mSurface->Save();
}

void cGraphics_PC::MapOverview_Render_Tiles( uint16 pTile, uint16 pDestX, uint16 pDestY ) {
	uint8* Target = mFodder->mSurfaceMapOverview->GetSurfaceBuffer();

	pDestX *= 16;

	Target += (pDestY * 16) * mFodder->mSurfaceMapOverview->GetWidth();
	Target += pDestX;
	
	uint8* TilePtr = mTile_Gfx_Ptrs[pTile];

	for (uint16 i = 0; i < 16; ++i) {

        memcpy(Target, TilePtr, 16);

		TilePtr += 320;
		Target += mFodder->mSurfaceMapOverview->GetWidth();
	}
}

void cGraphics_PC::Map_Load_Resources() {
	PaletteLoad( mFodder->mTile_BaseBlk->data() + 0xFA00, 0x80, 0x00 );

	mFodder->mFilenameCopt += ".dat";
	mFodder->mFilenameArmy += ".dat";

	//
	{
		mSpriteSheet_InGame2.mData = g_Resource->fileGet( mFodder->mFilenameCopt );
		mSpriteSheet_InGame2.LoadPalette( 0xD360, 0x10, 0x90 );
		mSpriteSheet_InGame2.LoadPalette( 0xD2A0, 0x40, 0xB0 );
	}

	// 
	{
		mSpriteSheet_InGame1.mData = g_Resource->fileGet( mFodder->mFilenameArmy );
		mSpriteSheet_InGame1.LoadPalette( 0xD200, 0x10, 0xA0 );
	}

	// Sprites on Sheet2 occupy palette range from 0x90-0x9F and 0xB0-0xCF
	mSpriteSheet_InGame2.CopyPalette(&mPalette[0x90], 0x10, 0x90);
	mSpriteSheet_InGame2.CopyPalette(&mPalette[0xB0], 0x40, 0xB0);

	// Sprites on Sheet1 occupy palette range from 0xA0-0xAF
	mSpriteSheet_InGame1.CopyPalette(&mPalette[0xA0], 0x10, 0xA0);

	SetActiveSpriteSheet( eGFX_IN_GAME );
}

void cGraphics_PC::Video_Draw_8(cSurface *pTarget, const uint8* RowPallete) {
	if (!pTarget)
		pTarget = mSurface;

	uint8*	di = pTarget->GetSurfaceBuffer();
	uint8* 	si = mFodder->mVideo_Draw_FrameDataPtr;

	di += pTarget->GetWidth() * mFodder->mVideo_Draw_PosY;
	di += mFodder->mVideo_Draw_PosX;

	mFodder->mVideo_Draw_Columns >>= 1;
	mFodder->mDraw_Source_SkipPixelsPerRow = 160 - mFodder->mVideo_Draw_Columns;
	mFodder->mDraw_Dest_SkipPixelsPerRow = (uint16)(pTarget->GetWidth() - (mFodder->mVideo_Draw_Columns * 2));

	for (int16 dx = 0; dx < mFodder->mVideo_Draw_Rows; ++dx) {
		uint8 Palette = mFodder->mVideo_Draw_PaletteIndex;
		if (RowPallete) {
			int16 bx = mFodder->mVideo_Draw_PosY + dx;

			Palette = RowPallete[bx];
		}

		for (int16 cx = mFodder->mVideo_Draw_Columns; cx > 0; --cx) {
			uint8 ah = *si;


			uint8 al = ah >> 4;
			if (al)
				*di = al | Palette;

			al = ah & 0x0F;
			if (al)
				*(di + 1) = al | Palette;

			si++;
			di+=2;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_PC::Video_Draw_16(const uint8* RowPallete) {
	uint8*	di = mSurface->GetSurfaceBuffer();
	uint8* 	si = mFodder->mVideo_Draw_FrameDataPtr;

	di += mSurface->GetWidth() * mFodder->mVideo_Draw_PosY;
	di += mFodder->mVideo_Draw_PosX;

	mFodder->mDraw_Source_SkipPixelsPerRow = mFodder->mVideo_Draw_ColumnsMax - mFodder->mVideo_Draw_Columns;
	mFodder->mDraw_Dest_SkipPixelsPerRow = mSurface->GetWidth() - mFodder->mVideo_Draw_Columns;

	for (int16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx) {

		for (int16 cx = mFodder->mVideo_Draw_Columns; cx > 0; --cx) {
			int8 al = *si;
			if (al)
				*di = al;

			si++;
			di++;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_PC::Sidebar_Copy_To_Surface( int16 pStartY, cSurface* pSurface) {
	uint8* Buffer = mSurface->GetSurfaceBuffer();

	if (pSurface)
		Buffer = pSurface->GetSurfaceBuffer();

	uint8* 	si = (uint8*) mFodder->mSidebar_Screen_Buffer;

	Buffer += (16 * mSurface->GetWidth()) +     16;

	for (unsigned int Y = 17 + pStartY; Y < mSurface->GetHeight(); ++Y) {

		for (unsigned int X = 0; X < 0x30; ++X) {

			Buffer[X] = *si++;
		}
			
		Buffer += mSurface->GetWidth();
	}
}

void cGraphics_PC::Sidebar_Copy_Sprite_To_ScreenBufPtr(int16 pSpriteType, size_t pX, size_t pY) {
	const sSpriteSheet_pstuff* SpriteSheet = &mSpriteSheet_PStuff[pSpriteType];

	mFodder->mVideo_Draw_Columns = SpriteSheet->mColumns;
	mFodder->mVideo_Draw_Rows = SpriteSheet->mRows;

	uint16 Offset = (0xA0 * SpriteSheet->mY) + (SpriteSheet->mX >> 1);

	mFodder->mVideo_Draw_FrameDataPtr = SpriteSheet->GetGraphicsPtr(Offset);
	mFodder->mVideo_Draw_PaletteIndex = 0xF0;

	uint8* si = mFodder->mVideo_Draw_FrameDataPtr;
	uint8* di = ((uint8*)mFodder->mSidebar_Screen_BufferPtr) + (48 * pY) + pX;

	mFodder->mDraw_Source_SkipPixelsPerRow = 0xA0 - (mFodder->mVideo_Draw_Columns >> 1);
	mFodder->mDraw_Dest_SkipPixelsPerRow = 48 - mFodder->mVideo_Draw_Columns;

	for (uint16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx) {

		for (uint16 cx = (mFodder->mVideo_Draw_Columns / 2); cx > 0; --cx) {
			
			uint8 ah = *si;

			uint8 al = ah >> 4;
			if (al)
				*di = al | mFodder->mVideo_Draw_PaletteIndex;

			al = ah & 0x0F;
			if (al)
				*(di + 1) = al | mFodder->mVideo_Draw_PaletteIndex;

			si++;
			di += 2;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_PC::Sidebar_Copy_ScreenBuffer( uint16 pRow, int16 pRows, int16 pCopyToScreen, uint32*& pBuffer) {
    pRow += 0x18;
	uint8* SptPtr = (uint8*)mFodder->mSidebar_Screen_Buffer;
	uint32* BuffPtr = (uint32*)(SptPtr + (48 * pRow));

	// Copying to pData20? or from it?
	if (pCopyToScreen == 0) {

		for (int16 cx = pRows; cx > 0; --cx) {
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;

			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;

			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
			*pBuffer++ = *BuffPtr++;
		}
	}
	else {

		for (int16 cx = pRows; cx > 0; --cx) {
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;

			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;

			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
			*BuffPtr++ = *pBuffer++;
		}
	}
}

void cGraphics_PC::Recruit_Draw_Hill( ) {

	mFodder->mVideo_Draw_FrameDataPtr = mImageHillBackground.mData->data() + 0xA00;

	mFodder->mVideo_Draw_PosX = 0x40;
	mFodder->mVideo_Draw_PosY = 0x28;
	mFodder->mVideo_Draw_Columns = 0x110;
	mFodder->mVideo_Draw_Rows = 0xB0;
	mFodder->mVideo_Draw_ColumnsMax = 320;

	Video_Draw_16();
}

void cGraphics_PC::Recruit_Draw_HomeAway( ) {
	const char* strHomeAndAway = "HOME                AWAY";
	
	SetActiveSpriteSheet(eGFX_RECRUIT);

	// Load Icon
	mFodder->GUI_Draw_Frame_8( 0x18, 0, 0, 0 );
	
	// Save Icon (Coloured or Gray)
	int16 Data4 = mFodder->mMission_Save_Blocked[ (mFodder->mGame_Data.mMission_Number - 1) ];
	mFodder->GUI_Draw_Frame_8( 0x19, Data4, 0x130, 0 );
	
	mFodder->String_CalculateWidth( 320, mFont_Recruit_Width, strHomeAndAway );
	mFodder->String_Print( mFont_Recruit_Width, 0x0D, mFodder->mGUI_Temp_X, 0x0A, strHomeAndAway );
	
	mFodder->GUI_Draw_Frame_8( 0x0E, 0, 0x9B, 0x0A );
	
	auto Home = tool_StripLeadingZero(std::to_string( mFodder->mGame_Data.mScore_Kills_Home ));
	mFodder->Recruit_Draw_String( 0x0D, (int16) (0x9A - (Home.length() * 0x0C)), 0x0A, Home );

	auto Away = tool_StripLeadingZero(std::to_string( mFodder->mGame_Data.mScore_Kills_Away ));
	mFodder->Recruit_Draw_String( 0x0D, 0xAA, 0x0A, Away );

	SetActiveSpriteSheet(eGFX_HILL);
}

void cGraphics_PC::Mission_Intro_Load_Resources(const eTileTypes pTileset) {

	// Briefing images
	std::string JunData1 = mTileTypes[pTileset].mName + "p1.dat";
	std::string JunData2 = mTileTypes[pTileset].mName + "p2.dat";
	std::string JunData3 = mTileTypes[pTileset].mName + "p3.dat";
	std::string JunData4 = mTileTypes[pTileset].mName + "p4.dat";
	std::string JunData5 = mTileTypes[pTileset].mName + "p5.dat";

	mImageMissionIntro.mData = g_Resource->fileGet(JunData1);
	mMission_Intro_Gfx_Clouds1 = g_Resource->fileGet(JunData2);
	mMission_Intro_Gfx_Clouds2 = g_Resource->fileGet(JunData3);
	mMission_Intro_Gfx_Clouds3 = g_Resource->fileGet(JunData4);
	mMission_Intro_Gfx_TreesMain = g_Resource->fileGet(JunData5);

	mBriefing_ParaHeli = g_Resource->fileGet( "paraheli.dat" );

	// Copy the palette for the current map tileset, in from paraheli to the briefing intro images
	uint8* si = mBriefing_ParaHeli->data() + 0xF00;
	si += 0x30 * pTileset;

	std::memcpy( (mImageMissionIntro.mData->data() + mImageMissionIntro.mData->size()) - 0x60, si, 0x30 );

	// Copy the palette from mImagePStuff 
	std::memcpy( (mImageMissionIntro.mData->data() + mImageMissionIntro.mData->size()) - 0x30, mImagePStuff.mData->data() + 0xA000, 0x30 );

	// Load the palette
	mImageMissionIntro.LoadPalette(mImageMissionIntro.mData->size() - 0x300, 0x100, 0);
}

uint8 cGraphics_PC::Video_Get_Pixel(uint8* pSi, int16 pX, int16 pY) {

	// 0xA0 Bytes per row
	pSi += 0xA0 * pY;

	pSi += (pX >> 1);

	if (pX & 1)
		return (*pSi) & 0x0F;

	return (*pSi) >> 4;
}

void cGraphics_PC::Video_Put_Pixel(uint8* pDi, uint8 pAl) {

	pDi += 0xA0 * (mRecruitDestY >> 16);
	pDi += (mRecruitDestX >> 16) >> 1;

	if ((mRecruitDestX >> 16) & 1) {
		*pDi &= 0xF0;
		*pDi |= pAl & 0x0F;
		return;
	}

	*pDi &= 0x0F;
	*pDi |= pAl << 4;
}

void cGraphics_PC::Recruit_Sprite_Draw( int16 pColumns, int16 pRows, 
										int16 pData8, int16 pData10, 
										int16 pData14, int16 pDataC, uint8* pGraphics ) {
	pColumns &= 0xFFFF;
	pRows &= 0xFFFF;

	uint8* es = mImageHillSprites.mData->data();

	mRecruitDestX = (pData10 - (pData8 >> 1)) << 16;
	dword_44A3E = mRecruitDestX;

	mRecruitDestY = (pData14 - (pDataC >> 1)) << 16;
	int32 eax = (pData8 << 0x10);
	if (eax <= 0)
		return;

	int32 dword_44A42 = eax / pColumns;
	eax = pDataC << 0x10;
	if (eax <= 0)
		return;

	int32 dword_44A46 = eax / pRows;
	for (int16 PosY = 0; PosY != pRows; ++PosY) {
		mRecruitDestX = dword_44A3E;

		for (int16 PosX = 0; PosX != pColumns; ++PosX) {
			uint8 al = Video_Get_Pixel( pGraphics, PosX, PosY);
			Video_Put_Pixel( es, al );

			mRecruitDestX += dword_44A42;
		}

		mRecruitDestY += dword_44A46;
   	}

}

void cGraphics_PC::Load_And_Draw_Image( const std::string &pFilename, unsigned int pColors, size_t pBackColor) {
	std::string Filename = pFilename;

	if (Filename.find('.') == std::string::npos )
		Filename.append( ".dat" );

	auto fileBuffer = g_Resource->fileGet(Filename);
	uint8* srcBuffer = fileBuffer->data();

	uint8 *Buffer = 0;

	mSurface->clearBuffer();

	for (unsigned int Plane = 0; Plane < 4; Plane++) {
		Buffer = mSurface->GetSurfaceBuffer() + (16 * mSurface->GetWidth()) + 16;
		
		for (unsigned int Y = 0; Y < 200; ++Y) {

			for (unsigned int X = Plane; X < 320; X += 4 ) {

				Buffer[X] = *srcBuffer++;
			}

			Buffer += mSurface->GetWidth();
		}
	}

	if(pColors)
		PaletteLoad( fileBuffer->data() + (fileBuffer->size() - (0x100 * 3)), pColors );
}

void cGraphics_PC::Mission_Intro_DrawHelicopter( uint16 ) {

}

bool cGraphics_PC::Sprite_OnScreen_Check() {
	int16 ax;

	if (mFodder->mVideo_Draw_PosY < 0) {
		ax = mFodder->mVideo_Draw_PosY + mFodder->mVideo_Draw_Rows;
		--ax;
		if (ax < 0)
			return false;

		ax -= 0;
		ax -= mFodder->mVideo_Draw_Rows;
		++ax;
		ax = -ax;
		mFodder->mVideo_Draw_PosY += ax;
		mFodder->mVideo_Draw_Rows -= ax;

		ax *= 0xA0;

		mFodder->mVideo_Draw_FrameDataPtr += ax;
	}

	ax = mFodder->mVideo_Draw_PosY + mFodder->mVideo_Draw_Rows;
	--ax;

    auto maxHeight = g_Window->GetScreenSize().getHeight() + 31;
	if (ax > maxHeight) {
		if (mFodder->mVideo_Draw_PosY > maxHeight)
			return false;

		ax -= maxHeight;
		mFodder->mVideo_Draw_Rows -= ax;
	}

	if (mFodder->mVideo_Draw_PosX < 0) {
		ax = mFodder->mVideo_Draw_PosX + mFodder->mVideo_Draw_Columns;
		--ax;
		if (ax < 0)
			return false;

		ax -= mFodder->mVideo_Draw_Columns;
		++ax;
		ax = -ax;
		--ax;

		mFodder->mVideo_Draw_PosX += ax;
		mFodder->mVideo_Draw_Columns -= ax;
		ax >>= 1;
		mFodder->mVideo_Draw_FrameDataPtr += ax;
	}

	ax = mFodder->mVideo_Draw_PosX + mFodder->mVideo_Draw_Columns;
	--ax;
	int16 maxWindowX = mFodder->getWindowWidth() + 31; // 351

	if (ax > maxWindowX) {
		if (mFodder->mVideo_Draw_PosX > maxWindowX)
			return false;

		ax -= maxWindowX;
		--ax;

		do {
			++ax;
		} while (ax & 8);

		mFodder->mVideo_Draw_Columns -= ax;
	}

	if (mFodder->mVideo_Draw_Columns <= 0)
		return false;

	if (mFodder->mVideo_Draw_Rows <= 0)
		return false;

	return true;
}

void cGraphics_PC::Mission_Intro_Render_1(tSharedBuffer pDs, int16 pCx) {

	if (mFodder->mBriefing_Render_1_Mode != 0)
		sub_15B98(pDs, pCx);
	//else
	//	sub_15CE8(pDs, pCx);
}


void cGraphics_PC::Mission_Intro_Render_2(tSharedBuffer pSource, int16 pCx) {

	uint8* pDs = pSource->data();

	int16 ax = pCx >> 2;
	int16 dx = ax;

	ax -= 80;
	int16 word_4285F = -ax;

	uint8* destPtr = mSurface->GetSurfaceBuffer() + mMission_Intro_DrawY + (dx * 4);

	++dx;

	uint8* di = destPtr;

	// Loop the 4 planes
	for (uint8 Plane = 0; Plane < 4; ++Plane) {
		di = destPtr + Plane;

		for (int16 bx = mMission_Intro_DrawX; bx > 0; --bx) {
			int16 cx;
			for (cx = word_4285F; cx > 0; --cx) {
                if (pDs >= pSource->data() + pSource->size())
                    return;

				uint8 al = *pDs;
				if (al)
					*di = al;

                ++pDs;
				di += 4;
			}

			// 28 difference

			di -= mSurface->GetWidth() - 12 - 16; // (81 * 4) // 324;
			--pDs;
			for (cx = dx; cx > 0; --cx) {
                if (pDs >= pSource->data() + pSource->size())
                    return;

				uint8 al = *pDs;
				if (al)
					*di = al;

                ++pDs;
				di += 4;
			}

			di += mSurface->GetWidth(); // (88 * 4) // 352;
		}
	}
}

void cGraphics_PC::sub_15B98(tSharedBuffer pDsSi, int16 pCx) {

    uint8* Ds = pDsSi->data();

	int16 ax = pCx >> 2;
	int16 dx = ax;

	ax -= 80;
	int16 word_4285F = -ax;

	uint8* destPtr = mSurface->GetSurfaceBuffer() + mMission_Intro_DrawY + (dx * 4);

	++dx;

	uint8* di = destPtr;

	for (uint8 Plane = 0; Plane < 4; ++Plane) {
		di = destPtr + Plane;

		for (int16 bx = mMission_Intro_DrawX; bx > 0; --bx) {
			int16 cx = word_4285F;

			if (cx & 1) {
				*di = *Ds++;
				di += 4;
			}

			cx >>= 1;
			while (cx > 0) {
				*di = *Ds++;
				di += 4;

				*di = *Ds++;
				di += 4;

				--cx;
			}

			cx = dx;
			di -= mSurface->GetWidth() - 12 - 16;
			--Ds;
			if (cx & 1) {
				*di = *Ds++;
				di += 4;
			}

			cx >>= 1;
			while (cx > 0) {
				*di = *Ds++;
				di += 4;

				*di = *Ds++;
				di += 4;

				--cx;
			}
			di += mSurface->GetWidth();
		}
	}
}

void cGraphics_PC::Briefing_Helicopter_Background_Unk_1() {
	const float scale = mFodder->mBriefingHelicopter_TimeScale;

	if (Heli_TextPosBottom != 0x0C) {
		Heli_TextPosBottom -= static_cast<int32_t>(4 * scale);

		if (Heli_TextPosBottom <= 0x0C)
			Heli_TextPosBottom = 0x0C;
	}
	Heli_TextPos = 344 - Heli_TextPosBottom;

	int32_t baseSpeed = static_cast<int32_t>(0x12000 * scale);
	int32_t d0 = baseSpeed;

	Heli_VeryBack -= d0;
	if (Heli_VeryBack < 0)
		Heli_VeryBack += 320 << 16;

	d0 <<= 1;
	Heli_Back -= d0;
	if (Heli_Back < 0)
		Heli_Back += 320 << 16;

	d0 <<= 1;
	Heli_middle -= d0;
	if (Heli_middle < 0)
		Heli_middle += 320 << 16;

	d0 <<= 1;
	Heli_Front -= d0;
	if (Heli_Front < 0)
		Heli_Front += 320 << 16;
}


void cGraphics_PC::Mission_Intro_Play( const bool pShowHelicopter, const eTileTypes pTileset, const std::string pTop, const std::string pBottom) {
	
	const std::vector<cPosition>& pPositions = BackgroundPositions[pTileset];

	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;
	static int16 mouseCheck = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageMissionIntro.CopyPalette(mPalette, 0x100, 0);

	mSurface->paletteSet(mPalette );

	Heli_TextPos = 0;
	Heli_TextPosBottom = 320;
	mFodder->mString_GapCharID = 0x25;

	mFodder->String_CalculateWidth(320, mFont_Underlined_Width, pTop);
	auto topTextPos = mFodder->mGUI_Temp_X;

	mFodder->String_CalculateWidth(320, mFont_Underlined_Width, pBottom);
	auto bottomTextPos = mFodder->mGUI_Temp_X - 4;

	do {
        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

		mFodder->Briefing_Helicopter_Check();
		Briefing_Helicopter_Background_Unk_1();

		mFodder->String_Print(mFont_Underlined_Width, 1, -332 + (topTextPos + (Heli_TextPos)), 0x01, pTop);
		mFodder->String_Print(mFont_Underlined_Width, 1, (Heli_TextPosBottom)+bottomTextPos, 0xB5 + 0x16, pBottom);

		// Clouds
		mMission_Intro_DrawX = pPositions[0].mX;
		mMission_Intro_DrawY = pPositions[0].mY;
		Mission_Intro_Render_1( mMission_Intro_Gfx_Clouds3, word_42875);

		mMission_Intro_DrawX = pPositions[1].mX;
		mMission_Intro_DrawY = pPositions[1].mY;
		Mission_Intro_Render_2( mMission_Intro_Gfx_Clouds2, word_42873 );

		mMission_Intro_DrawX = pPositions[2].mX;
        mMission_Intro_DrawY = pPositions[2].mY;
		Mission_Intro_Render_2( mMission_Intro_Gfx_Clouds1, word_42871 );

		// Trees (Main)
		mMission_Intro_DrawX = pPositions[3].mX;
        mMission_Intro_DrawY = pPositions[3].mY;
		Mission_Intro_Render_1( mMission_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefingHelicopter_FrameCounter];

		mFodder->mVideo_Draw_PosX = mFodder->mBriefingHelicopter_ScreenX;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mBriefingHelicopter_ScreenY;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;

        if (pShowHelicopter) {
			mFodder->mVideo_Draw_PaletteIndex = 0xE0;

            if (Sprite_OnScreen_Check())
                Video_Draw_8();
        }

		mMission_Intro_DrawX = pPositions[4].mX;
        mMission_Intro_DrawY = pPositions[4].mY;
		Mission_Intro_Render_2( mImageMissionIntro.mData, word_4286F );

		// Front
		word_4286F += static_cast<int32_t>(8 * mFodder->mBriefingHelicopter_TimeScale);
		if (word_4286F >= 320)
			word_4286F -= 320;

		// Middle
		word_42871 += static_cast<int32_t>(4 * mFodder->mBriefingHelicopter_TimeScale);
		if (word_42871 >= 320)
			word_42871 -= 320;

		// Back
		word_42873 += static_cast<int32_t>(2 * mFodder->mBriefingHelicopter_TimeScale);
		if (word_42873 >= 320)
			word_42873 -= 320;

		// Very Back
		word_42875 += static_cast<int32_t>(1 * mFodder->mBriefingHelicopter_TimeScale);
		if (word_42875 >= 320)
			word_42875 -= 320;


		mFodder->Video_Sleep(0, false, false);

		if (mFodder->mMouseButtonStatus || mFodder->mPhase_Aborted) {
			mFodder->mBriefingHelicopter_NotDone = 0;
			mSurface->paletteNew_SetToBlack();
			mFodder->mMouse_Exit_Loop = false;
			mFodder->mPhase_Aborted = 0;
		}

	} while (mFodder->mBriefingHelicopter_NotDone || mFodder->mSurface->isPaletteAdjusting());
}
