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

#include "stdafx.hpp"
#include "PC/SpriteData_PC.hpp"

cGraphics_PC::~cGraphics_PC() {

}

uint8* cGraphics_PC::GetSpriteData( uint16 pSegment ) {
	
	switch ( pSegment ) {
		case eSPRITE_IN_GAME:
			return mSpriteSheet_InGame1.mData->data();
			break;

		case eSPRITE_IN_GAME2:
			return mSpriteSheet_InGame2.mData->data();
			break;
		
		case eSPRITE_RANKFONT:
			return mSpriteSheet_RankFont.mData->data();
			break;
		
		case eSPRITE_FONT:
			return mImageFonts.mData->data();
			break;

		case eSPRITE_PSTUFF:
			return mImagePStuff.mData->data();
			break;
		
		case eSPRITE_RECRUIT:
			return mImageRecruit.mData->data();
			break;

		case eSPRITE_HILL:
			return mImageHill.mData->data();
			break;

		case eSPRITE_SERVICE:
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

void cGraphics_PC::SetActiveSpriteSheet( eSpriteType pSpriteType ) {
	
	switch (pSpriteType) {
		case eSPRITE_IN_GAME:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_InGame_PC );
			return;

		case eSPRITE_FONT:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Font_PC );
			return;

		case eSPRITE_RECRUIT:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Recruit_PC );
			return;

		case eSPRITE_HILL:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Hill_PC );
			return;
		
		case eSPRITE_BRIEFING:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Briefing_PC );
			return;

		case eSPRITE_SERVICE:
			mFodder->SetActiveSpriteSheetPtr( mSpriteSheetTypes_Service_PC );
			return;
	}
}

sImage cGraphics_PC::Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset, const size_t pStartIndex) {

	sImage Hill;

	Hill.mData = g_Resource.fileGet(pFilename);
	Hill.LoadPalette(pPaletteOffset, pCount, pStartIndex);
	Hill.CopyPalette(&mFodder->mPalette[pStartIndex], pCount, pStartIndex);

	return Hill;
}

void cGraphics_PC::Load_pStuff() {

	mImagePStuff = Decode_Image("pstuff.dat", 0x10, 0xA000, 0xF0);
}

void cGraphics_PC::Load_Sprite_Font() {
	
	mImageFonts = Decode_Image("font.dat", 0x10, 0xA000, 0xD0);

	SetActiveSpriteSheet( eSPRITE_FONT );
}

void cGraphics_PC::Load_Hill_Data() {

	mImageHill = Decode_Image("hill.dat", 0x50, 0xFA00, 0x00);
	mImageRecruit = Decode_Image("hillbits.dat", 0x10, 0x6900, 0xB0);
}

void cGraphics_PC::Load_Service_Data() {
	
	mSpriteSheet_RankFont = Decode_Image("rankfont.dat", 0x80, 0xA000, 0x40);
	mImageService = Decode_Image("morphbig.dat", 0x40, 0xFA00, 0x00);
}

void cGraphics_PC::Tile_Prepare_Gfx() {
	uint16 bx = 0, dx = 0;

	for (uint16 cx = 0; cx < 240; ++cx) {

		mTile_Gfx_Ptrs[cx + 0x00] = mFodder->mDataBaseBlk->data() + bx;
		mTile_Gfx_Ptrs[cx + 0xF0] = mFodder->mDataSubBlk->data() + bx;

		++dx;
		bx += 0x10;
		if (dx % 0x14 == 0) {
			dx = 0;
			bx += 0x12C0;
		}
	}			
}

void cGraphics_PC::PaletteLoad( const uint8  *pBuffer, uint32 pColors, uint32 pColorID ) {
	size_t colorStartID = pColorID;

	if (pColors >= g_MaxColors)
		pColors = g_MaxColors - 1;

	for (; pColorID < pColors + colorStartID; pColorID++) {

		// Get the next color values
		mFodder->mPalette[pColorID].mRed = *pBuffer++;
		mFodder->mPalette[pColorID].mGreen = *pBuffer++;
		mFodder->mPalette[pColorID].mBlue = *pBuffer++;
	}
}

void cGraphics_PC::PaletteSetOverview() {

	mFodder->mSurfaceMapOverview->paletteSet( mFodder->mPalette, 0, g_MaxColors, true );
}

void cGraphics_PC::PaletteSet() {

	mImage->paletteSet( mFodder->mPalette );
}

void cGraphics_PC::Map_Tiles_Draw() {

	uint8* Target = mImage->GetSurfaceBuffer();

	uint8* CurrentMapPtr = mFodder->mMap->data() + mFodder->mMapTilePtr;

	// Y
	for (uint16 cx = 0; cx < 0x0F; ++cx) {
		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = mFodder->mCamera_Pan_RowOffset;
		else
			StartY = 0;

		// X
		for (uint16 cx2 = 0; cx2 < 0x16; ++cx2) {
			uint8* TargetTmp = TargetRow;

			uint16 Tile = readLEWord( MapPtr ) & 0x1FF;
			if (Tile > 0x1DF)
				Tile = 0;

			uint8* TilePtr = mTile_Gfx_Ptrs[Tile];
			uint16 StartX = 0;

			TilePtr += StartY * 0x140;
			
			if (cx2 == 0)
				StartX = mFodder->mCamera_Pan_ColumnOffset;
			else
				StartX = 0;

			// Each Tile Row
			for (uint16 i = StartX; i < 16; ++i) {

				memcpy( TargetTmp, TilePtr + StartX, 16 - StartX );
				TilePtr += 0x140;
				TargetTmp += mImage->GetWidth();
			}

			MapPtr += 2;
			TargetRow += (16-StartX);
		}

		Target += mImage->GetWidth() * (16-StartY);
		CurrentMapPtr += mFodder->mMapWidth << 1;
	}

	mImage->Save();
}

void cGraphics_PC::MapOverview_Render_Tiles( uint16 pTile, uint16 pDestX, uint16 pDestY ) {
	uint8* Target = mFodder->mSurfaceMapOverview->GetSurfaceBuffer();

	pDestX *= 16;

	Target += (pDestY * 16) * mFodder->mSurfaceMapOverview->GetWidth();
	Target += pDestX;
	
	uint8* TilePtr = mTile_Gfx_Ptrs[pTile];

	for (uint16 i = 0; i < 16; ++i) {

		memcpy( Target, TilePtr, 16 );
		TilePtr += 0x140;
		Target += mFodder->mSurfaceMapOverview->GetWidth();
	}
}

void cGraphics_PC::Map_Load_Resources() {
	PaletteLoad( mFodder->mDataBaseBlk->data() + 0xFA00, 0x80, 0x00 );

	mFodder->mFilenameCopt = mFodder->Filename_CreateFromBase( mFodder->mFilenameCopt, "dat" );
	mFodder->mFilenameArmy = mFodder->Filename_CreateFromBase( mFodder->mFilenameArmy, "dat" );

	//
	{
		mSpriteSheet_InGame2.mData = g_Resource.fileGet( mFodder->mFilenameCopt );
		mSpriteSheet_InGame2.LoadPalette( 0xD360, 0x10, 0x90 );
		mSpriteSheet_InGame2.LoadPalette( 0xD2A0, 0x40, 0xB0 );
	}

	// 
	{
		mSpriteSheet_InGame1.mData = g_Resource.fileGet( mFodder->mFilenameArmy );
		mSpriteSheet_InGame1.LoadPalette( 0xD200, 0x10, 0xA0 );
	}

	// Sprites on Sheet2 occupy palette range from 0x90-0x9F and 0xB0-0xCF
	mSpriteSheet_InGame2.CopyPalette(&mFodder->mPalette[0x90], 0x10, 0x90);
	mSpriteSheet_InGame2.CopyPalette(&mFodder->mPalette[0xB0], 0x40, 0xB0);

	// Sprites on Sheet1 occupy palette range from 0xA0-0xAF
	mSpriteSheet_InGame1.CopyPalette(&mFodder->mPalette[0xA0], 0x10, 0xA0);

	SetActiveSpriteSheet( eSPRITE_IN_GAME );
}

void cGraphics_PC::Video_Draw_8() {
	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->mVideo_Draw_FrameDataPtr;

	di += mImage->GetWidth() * mFodder->mVideo_Draw_PosY;
	di += mFodder->mVideo_Draw_PosX;

	mFodder->word_42066 = di;

	mFodder->mVideo_Draw_Columns >>= 1;
	mFodder->mDraw_Source_SkipPixelsPerRow = 160 - mFodder->mVideo_Draw_Columns;
	mFodder->mDraw_Dest_SkipPixelsPerRow = (uint16)(mImage->GetWidth() - (mFodder->mVideo_Draw_Columns * 2));

	for (int16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx) {

		for (int16 cx = mFodder->mVideo_Draw_Columns; cx > 0; --cx) {
			uint8 ah = *si;

			uint8 al = ah >> 4;
			if (al)
				*di = al | mFodder->mVideo_Draw_PaletteIndex;

			al = ah & 0x0F;
			if (al)
				*(di + 1) = al | mFodder->mVideo_Draw_PaletteIndex;

			si++;
			di+=2;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_PC::Video_Draw_16() {
	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->mVideo_Draw_FrameDataPtr;

	di += 352 * mFodder->mVideo_Draw_PosY;
	di += mFodder->mVideo_Draw_PosX;

	mFodder->word_42066 = di;

	mFodder->mDraw_Source_SkipPixelsPerRow = mFodder->mVideo_Draw_ColumnsMax - mFodder->mVideo_Draw_Columns;
	mFodder->mDraw_Dest_SkipPixelsPerRow = 352 - mFodder->mVideo_Draw_Columns;

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

void cGraphics_PC::Sidebar_Copy_To_Surface( int16 pStartY ) {
	
	uint8*	Buffer = mImage->GetSurfaceBuffer();
	uint8* 	si = (uint8*) mFodder->mSidebar_Screen_Buffer;

	Buffer += (16 * mImage->GetWidth()) + 16;

	mFodder->word_42066 = Buffer;
	for (unsigned int Plane = 0; Plane < 4; Plane++) {

		Buffer = mFodder->word_42066;

		for (unsigned int Y = 0; Y < 200; ++Y) {

			for (unsigned int X = Plane; X < 0x30; X += 4) {

				Buffer[X] = *si++;
			}
			
			Buffer += 352;
		}
	}
}

void cGraphics_PC::Sidebar_Copy_Sprite_To_ScreenBufPtr( int16 pSpriteType, int16 pX, int16 pY ) {
	const sSpriteSheet_pstuff* str2 = &mSpriteSheet_PStuff[pSpriteType];
	
	mFodder->mVideo_Draw_Columns = str2->mColumns;
	mFodder->mVideo_Draw_Rows = str2->mRows;
	
	uint16 ax = 0xA0 * str2->mY;
	uint16 bx = str2->mX >> 1;
	ax += bx;
	
	mFodder->mVideo_Draw_FrameDataPtr = str2->GetGraphicsPtr( ax );
	
	mFodder->mVideo_Draw_PaletteIndex = 0xF0;
	
	uint16 w42066 = 0x0C * pY;
	w42066 += pX >> 2;
	
	ax = pX & 3;
	ax *= 0x960;
	w42066 += ax;
	
	uint8* si = mFodder->mVideo_Draw_FrameDataPtr;
	uint8* di = ((uint8*)mFodder->mSidebar_Screen_BufferPtr) + w42066;

	mFodder->mVideo_Draw_Columns >>= 1;
	mFodder->mDraw_Source_SkipPixelsPerRow = 0xA0 - mFodder->mVideo_Draw_Columns;

	mFodder->mVideo_Draw_Columns >>= 1;
	mFodder->mDraw_Dest_SkipPixelsPerRow = 0x0C - mFodder->mVideo_Draw_Columns;
	
	for( uint16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mVideo_Draw_Columns; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | mFodder->mVideo_Draw_PaletteIndex;
			
			si += 2;
			++di;
		}
			
		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	si = mFodder->mVideo_Draw_FrameDataPtr;
	di =  ((uint8*)mFodder->mSidebar_Screen_BufferPtr) + w42066;
	
	for( uint16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mVideo_Draw_Columns; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | mFodder->mVideo_Draw_PaletteIndex;
			
			si += 2;
			++di;
		}
		
		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
		
	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	++mFodder->mVideo_Draw_FrameDataPtr;
	si = mFodder->mVideo_Draw_FrameDataPtr;
	di =  ((uint8*)mFodder->mSidebar_Screen_BufferPtr) + w42066;
	
	for( uint16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mVideo_Draw_Columns; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | mFodder->mVideo_Draw_PaletteIndex;
			
			si += 2;
			++di;
		}
			
		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;

	si = mFodder->mVideo_Draw_FrameDataPtr;
	di =  ((uint8*)mFodder->mSidebar_Screen_BufferPtr) + w42066;
	
	for( uint16 dx = mFodder->mVideo_Draw_Rows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mVideo_Draw_Columns; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | mFodder->mVideo_Draw_PaletteIndex;
			
			si += 2;
			++di;
		}
		
		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}

}

void cGraphics_PC::Sidebar_Copy_ScreenBuffer( uint16 pData0, int16 pData4, int16 pCopyToScreen, uint32*& pBuffer) {
	pData0 += 0x18;
	uint8* SptPtr = (uint8*)mFodder->mSidebar_Screen_Buffer;
	uint32* BuffPtr = (uint32*)(SptPtr + (0x0C * pData0));

	// Copying to pData20? or from it?
	if (pCopyToScreen == 0) {

		for (int16 cx = pData4; cx > 0; --cx) {
			*pBuffer++ = *BuffPtr;
			*pBuffer++ = *(BuffPtr + 0x258);
			*pBuffer++ = *(BuffPtr + 0x4B0);
			*pBuffer++ = *(BuffPtr + 0x708);
			++BuffPtr;

			*pBuffer++ = *BuffPtr;
			*pBuffer++ = *(BuffPtr + 0x258);
			*pBuffer++ = *(BuffPtr + 0x4B0);
			*pBuffer++ = *(BuffPtr + 0x708);
			++BuffPtr;

			*pBuffer++ = *BuffPtr;
			*pBuffer++ = *(BuffPtr + 0x258);
			*pBuffer++ = *(BuffPtr + 0x4B0);
			*pBuffer++ = *(BuffPtr + 0x708);
			++BuffPtr;
		}
	}
	else {

		for (int16 cx = pData4; cx > 0; --cx) {
			*BuffPtr = *pBuffer++;
			*(BuffPtr+0x258) = *pBuffer++;
			*(BuffPtr+0x4B0) = *pBuffer++;
			*(BuffPtr+0x708) = *pBuffer++;
			++BuffPtr;

			*BuffPtr = *pBuffer++;
			*(BuffPtr+0x258) = *pBuffer++;
			*(BuffPtr+0x4B0) = *pBuffer++;
			*(BuffPtr+0x708) = *pBuffer++;
			++BuffPtr;

			*BuffPtr = *pBuffer++;
			*(BuffPtr+0x258) = *pBuffer++;
			*(BuffPtr+0x4B0) = *pBuffer++;
			*(BuffPtr+0x708) = *pBuffer++;
			++BuffPtr;
		}
	}
}

void cGraphics_PC::Recruit_Draw_Hill( ) {

	mFodder->mVideo_Draw_FrameDataPtr = mImageHill.mData->data() + 0xA00;

	mFodder->mVideo_Draw_PosX = 0x40;
	mFodder->mVideo_Draw_PosY = 0x28;
	mFodder->mVideo_Draw_Columns = 0x110;
	mFodder->mVideo_Draw_Rows = 0xB0;
	mFodder->mVideo_Draw_ColumnsMax = 0x140;

	Video_Draw_16();
	
	for( uint32 x = 0; x < 0xA000; ++x) {
		mImageHill.mData->data()[x] = 0;
	}
}

void cGraphics_PC::Recruit_Draw_HomeAway( ) {
	const char* strHomeAndAway = "HOME                AWAY";
	
	SetActiveSpriteSheet(eSPRITE_RECRUIT);

	// Load Icon
	mFodder->GUI_Draw_Frame_8( 0x18, 0, 0, 0 );
	
	// Save Icon (Coloured or Gray)
	int16 Data4 = mFodder->mMission_Save_Availability[ (mFodder->mMissionNumber - 1) ];
	mFodder->GUI_Draw_Frame_8( 0x19, Data4, 0x130, 0 );
	
	mFodder->String_CalculateWidth( 320, mFont_Recruit_Width, strHomeAndAway );
	mFodder->String_Print( mFont_Recruit_Width, 0x0D, mFodder->mGUI_Temp_X, 0x0A, strHomeAndAway );
	
	mFodder->GUI_Draw_Frame_8( 0x0E, 0, 0x9B, 0x0A );
	
	auto Home = tool_StripLeadingZero(tool_NumToString( mFodder->mTroops_Home ));
	mFodder->Recruit_Draw_String( 0x0D, (int16) (0x9A - (Home.length() * 0x0C)), 0x0A, Home );

	auto Away = tool_StripLeadingZero(tool_NumToString( mFodder->mTroops_Away ));
	mFodder->Recruit_Draw_String( 0x0D, 0xAA, 0x0A, Away );

	SetActiveSpriteSheet(eSPRITE_HILL);
}

void cGraphics_PC::Briefing_Load_Resources() {

	// Load the current map & Set TileType
	mFodder->mMap = g_Resource.fileGet(mFodder->map_Filename_MapGet());
	mFodder->Map_SetTileType();

	// Briefing images
	std::string JunData1 = mTileTypes[mFodder->mMap_TileSet].mName + "p1.dat";
	std::string JunData2 = mTileTypes[mFodder->mMap_TileSet].mName + "p2.dat";
	std::string JunData3 = mTileTypes[mFodder->mMap_TileSet].mName + "p3.dat";
	std::string JunData4 = mTileTypes[mFodder->mMap_TileSet].mName + "p4.dat";
	std::string JunData5 = mTileTypes[mFodder->mMap_TileSet].mName + "p5.dat";

	mImageBriefingIntro.mData = g_Resource.fileGet(JunData1);
	mBriefing_Intro_Gfx_Clouds1 = g_Resource.fileGet(JunData2);
	mBriefing_Intro_Gfx_Clouds2 = g_Resource.fileGet(JunData3);
	mBriefing_Intro_Gfx_Clouds3 = g_Resource.fileGet(JunData4);
	mBriefing_Intro_Gfx_TreesMain = g_Resource.fileGet(JunData5);

	mBriefing_ParaHeli = g_Resource.fileGet( "paraheli.dat" );

	// Copy the palette for the current map tileset, in from paraheli to the briefing intro images
	uint8* si = mBriefing_ParaHeli->data() + 0xF00;
	si += 0x30 * mFodder->mMap_TileSet;
	std::memcpy( (mImageBriefingIntro.mData->data() + mImageBriefingIntro.mData->size()) - 0x60, si, 0x30 );

	// Copy the palette from mImagePStuff 
	std::memcpy( (mImageBriefingIntro.mData->data() + mImageBriefingIntro.mData->size()) - 0x30, mImagePStuff.mData->data() + 0xA000, 0x30 );

	// Load the palette
	mImageBriefingIntro.LoadPalette(mImageBriefingIntro.mData->size() - 0x300, 0x100, 0);
}

uint8 cGraphics_PC::Video_Get_Pixel(uint8* pSi, int16 pBx, int16 pCx) {

	pSi += 0xA0 * pBx;
	pSi += (pCx >> 1);
	if (pCx & 1)
		return (*pSi) & 0x0F;

	return (*pSi) >> 4;
}

void cGraphics_PC::Video_Put_Pixel(uint8* pDi, uint8 pAl) {

	pDi += 0xA0 * (dword_44A3A >> 16);
	pDi += (dword_44A36 >> 16) >> 1;

	if ((dword_44A36 >> 16) & 1) {
		*pDi &= 0xF0;
		*pDi |= pAl & 0x0F;
		return;
	}

	*pDi &= 0x0F;
	*pDi |= pAl << 4;
}

void cGraphics_PC::Recruit_Sprite_Draw( int16 pColumns, int16 pRows, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pGraphics ) {
	pColumns &= 0xFFFF;
	pRows &= 0xFFFF;

	uint8* es = mImageHill.mData->data();

	dword_44A36 = (pData10 - (pData8 >> 1)) << 16;
	dword_44A3E = dword_44A36;

	dword_44A3A = (pData14 - (pDataC >> 1)) << 16;
	int32 eax = (pData8 << 0x10);
	if (eax <= 0)
		return;

	int32 dword_44A42 = eax / pColumns;
	eax = pDataC << 0x10;
	if (eax <= 0)
		return;

	int32 dword_44A46 = eax / pRows;
	for (int16 bx = 0; bx != pRows; ++bx) {
		dword_44A36 = dword_44A3E;

		for (int16 cx = 0; cx != pColumns; ++cx) {
			uint8 al = Video_Get_Pixel( pGraphics, bx, cx );
			Video_Put_Pixel( es, al );
			dword_44A36 += dword_44A42;
		}

		dword_44A3A += dword_44A46;
   	}

}

void cGraphics_PC::Load_And_Draw_Image( const std::string &pFilename, unsigned int pColors ) {
	std::string Filename = pFilename;

	if (Filename.find('.') == std::string::npos )
		Filename.append( ".dat" );

	auto fileBuffer = g_Resource.fileGet(Filename);
	uint8* srcBuffer = fileBuffer->data();

	uint8 *Buffer = 0;

	mImage->clearBuffer();

	for (unsigned int Plane = 0; Plane < 4; Plane++) {
		Buffer = mImage->GetSurfaceBuffer() + (16 * mImage->GetWidth()) + 16;
		
		for (unsigned int Y = 0; Y < 200; ++Y) {

			for (unsigned int X = Plane; X < 320; X += 4 ) {

				Buffer[X] = *srcBuffer++;
			}

			Buffer += mImage->GetWidth();
		}
	}

	if(pColors)
		PaletteLoad( fileBuffer->data() + (fileBuffer->size() - (0x100 * 3)), pColors );
}

void cGraphics_PC::Briefing_DrawHelicopter( uint16 pID ) {

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

	if (ax > 231) {
		if (mFodder->mVideo_Draw_PosY > 231)
			return false;

		ax -= 231;
		mFodder->mVideo_Draw_Rows -= ax;
	}

	if (mFodder->mVideo_Draw_PosX < 0) {
		ax = mFodder->mVideo_Draw_PosX + mFodder->mVideo_Draw_Columns;
		--ax;
		if (ax < 0)
			return false;

		ax -= 0;
		ax -= mFodder->mVideo_Draw_Columns;
		++ax;
		ax = -ax;
		--ax;

		do {
			++ax;
		} while (ax & 3);

		mFodder->mVideo_Draw_PosX += ax;
		mFodder->mVideo_Draw_Columns -= ax;
		ax >>= 1;
		mFodder->mVideo_Draw_FrameDataPtr += ax;
	}

	ax = mFodder->mVideo_Draw_PosX + mFodder->mVideo_Draw_Columns;
	--ax;

	if (ax > 351) {
		if (mFodder->mVideo_Draw_PosX > 351)
			return false;

		ax -= 351;
		--ax;

		do {
			++ax;
		} while (ax & 3);

		mFodder->mVideo_Draw_Columns -= ax;
	}

	if (mFodder->mVideo_Draw_Columns <= 0)
		return false;

	if (mFodder->mVideo_Draw_Rows <= 0)
		return false;

	return true;
}

void cGraphics_PC::Briefing_Intro() {
	mImage->clearBuffer();

	Briefing_Load_Resources();
	SetActiveSpriteSheet(eSPRITE_BRIEFING);

	mFodder->mSound->Music_Play(0x07);
	mFodder->Briefing_Helicopter_Start();


	mFodder->Briefing_Draw_Mission_Name();

	switch (mFodder->mMap_TileSet) {
	case eTileTypes_Jungle:
		Briefing_Intro_Jungle();
		break;

	case eTileTypes_Desert:
		Briefing_Intro_Desert();
		break;

	case eTileTypes_Ice:
		Briefing_Intro_Ice();
		break;

	case eTileTypes_Moors:
		Briefing_Intro_Mor();
		break;

	case eTileTypes_Int:
		Briefing_Intro_Int();
		break;
	}

	Load_pStuff();
}


void cGraphics_PC::Briefing_Render_1(tSharedBuffer pDs, int16 pCx) {

	if (mFodder->word_3E75B != 0)
		sub_15B98(pDs->data(), pCx);
	else
		sub_15CE8(pDs->data(), pCx);
}


void cGraphics_PC::Briefing_Render_2(tSharedBuffer pSource, int16 pCx) {
	uint8* pDs = pSource->data();

	int16 ax = pCx >> 2;
	int16 dx = ax;

	ax -= 0x50;
	mFodder->word_4285F = -ax;

	uint8* word_4285D = mImage->GetSurfaceBuffer() + mFodder->word_4285B + (dx * 4);
	pCx &= 3;

	++dx;

	uint8* di = word_4285D;

	// Loop the 4 planes
	for (uint8 Plane = 0; Plane < 4; ++Plane) {
		di = word_4285D + Plane;

		for (int16 bx = mFodder->word_42859; bx > 0; --bx) {
			int16 cx;
			for (cx = mFodder->word_4285F; cx > 0; --cx) {
				uint8 al = *pDs++;
				if (al)
					*di = al;

				di += 4;
			}

			di -= 0x51 * 4;
			--pDs;
			for (cx = dx; cx > 0; --cx) {
				uint8 al = *pDs++;
				if (al)
					*di = al;

				di += 4;
			}

			di += 0x58 * 4;
		}
	}
}

void cGraphics_PC::sub_15B98(uint8* pDsSi, int16 pCx) {
	int16 ax = pCx >> 2;
	int16 dx = ax;

	ax -= 0x50;
	mFodder->word_4285F = -ax;

	uint8* word_4285D = mImage->GetSurfaceBuffer() + mFodder->word_4285B + (dx * 4);
	pCx &= 3;

	++dx;

	uint8* di = word_4285D;

	for (uint8 Plane = 0; Plane < 4; ++Plane) {
		di = word_4285D + Plane;

		for (int16 bx = mFodder->word_42859; bx > 0; --bx) {
			int16 cx = mFodder->word_4285F;

			if (cx & 1) {
				*di = *pDsSi++;
				di += 4;
			}

			cx >>= 1;
			while (cx > 0) {
				*di = *pDsSi++;
				di += 4;

				*di = *pDsSi++;
				di += 4;

				--cx;
			}

			cx = dx;
			di -= 0x51 * 4;
			--pDsSi;
			if (cx & 1) {
				*di = *pDsSi++;
				di += 4;
			}

			cx >>= 1;
			while (cx > 0) {
				*di = *pDsSi++;
				di += 4;

				*di = *pDsSi++;
				di += 4;

				--cx;
			}
			di += 0x58 * 4;
		}
	}
}

void cGraphics_PC::sub_15CE8(uint8* pDs, int16 pCx) {
	//todo
	assert(1 == 0);
}

void cGraphics_PC::Briefing_Intro_Jungle( ) {
	
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageBriefingIntro.CopyPalette(mFodder->mPalette, 0x100, 0);

	mImage->paletteSet(mFodder->mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if( mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mImage->palette_FadeTowardNew();

		// Clouds
		mFodder->word_42859 = 0x30;
		mFodder->word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_Clouds3, word_42875 );

		mFodder->word_42859 = 0x38;
		mFodder->word_4285B = 0x102C * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds2, word_42873);

		mFodder->word_42859 = 0x12;
		mFodder->word_4285B = 0x1D3C * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds1, word_42871);

		// Trees (Main)
		mFodder->word_42859 = 0x5C;
		mFodder->word_4285B = 0x236C * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		mFodder->word_42859 = 0x2D;
		mFodder->word_4285B = 0x33EC * 4;
		Briefing_Render_2( mImageBriefingIntro.mData, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		mFodder->Mouse_GetData();
		mFodder->Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mImage->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}

void cGraphics_PC::Briefing_Intro_Desert() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageBriefingIntro.CopyPalette(mFodder->mPalette, 0x100, 0);

	mImage->paletteSet(mFodder->mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if(mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mImage->palette_FadeTowardNew();

		// Clouds
		mFodder->word_42859 = 0x3A;
		mFodder->word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_Clouds3, word_42875 );

		mFodder->word_42859 = 0x4C;
		mFodder->word_4285B = 0x139C * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds2, word_42873 );

		mFodder->word_42859 = 0x30;
		mFodder->word_4285B = 0x1CE4 * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds1, word_42871 );

		// Trees (Main)
		mFodder->word_42859 = 0x40;
		mFodder->word_4285B = 0x2D64 * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		mFodder->word_42859 = 0x30;
		mFodder->word_4285B = 0x32E4 * 4;
		Briefing_Render_2( mImageBriefingIntro.mData, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		mFodder->Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		mFodder->Mouse_GetData();
		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mImage->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}

void cGraphics_PC::Briefing_Intro_Ice() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageBriefingIntro.CopyPalette(mFodder->mPalette, 0x100, 0);

	mImage->paletteSet(mFodder->mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if(mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mImage->palette_FadeTowardNew();

		// Clouds
		mFodder->word_42859 = 0x24;
		mFodder->word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_Clouds3, word_42875 );

		mFodder->word_42859 = 0x42;
		mFodder->word_4285B = 0x102C * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds2, word_42873 );

		// Ice Caps
		mFodder->word_42859 = 0x18;
		mFodder->word_4285B = 0x1CE4 * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds1, word_42871 );

		// Ice Mountains
		mFodder->word_42859 = 0x58;
		mFodder->word_4285B = 0x2524 * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		mFodder->word_42859 = 0x2E;
		mFodder->word_4285B = 0x3394 * 4;
		// Trees
		Briefing_Render_2( mImageBriefingIntro.mData, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		mFodder->Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		mFodder->Mouse_GetData();
		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mFodder->mImage->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}

void cGraphics_PC::Briefing_Intro_Mor() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;
	mImageBriefingIntro.CopyPalette(mFodder->mPalette, 0x100, 0);

	mImage->paletteSet(mFodder->mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if(mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mImage->palette_FadeTowardNew();

		// Clouds
		mFodder->word_42859 = 0x1D;
		mFodder->word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_Clouds3, word_42875 );

		mFodder->word_42859 = 0x40;
		mFodder->word_4285B = 0x1134 * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds2, word_42873 );

		mFodder->word_42859 = 0x6;
		mFodder->word_4285B = 0x2524 * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds1, word_42871 );

		// Trees (Main)
		mFodder->word_42859 = 0x52;
		mFodder->word_4285B = 0x2734 * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		mFodder->word_42859 = 0x30;
		mFodder->word_4285B = 0x32E4 * 4;
		Briefing_Render_2( mImageBriefingIntro.mData, word_4286F );

		word_4286F += 8;
		if (word_4286F > 0x140)
			word_4286F = 0;

		word_42871 += 4;
		if (word_42871 > 0x140)
			word_42871 = 0;

		word_42873 += 2;
		if (word_42873 > 0x140)
			word_42873 = 0;

		++word_42875;
		if (word_42875 > 0x140)
			word_42875 = 0;

		mFodder->Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		mFodder->Mouse_GetData();
		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mImage->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}

void cGraphics_PC::Briefing_Intro_Int() {
	int16 mBriefing_Intro_X = 0;
	int16 mBriefing_Intro_Clouds1_X = 0;
	int16 mBriefing_Intro_Clouds2_X = 0;
	int16 mBriefing_Intro_Clouds3_X = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageBriefingIntro.CopyPalette(mFodder->mPalette, 0x100, 0);

	mImage->paletteSet(mFodder->mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if(mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mImage->palette_FadeTowardNew();

		// Clouds
		mFodder->word_42859 = 0x40;
		mFodder->word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_Clouds3, mBriefing_Intro_Clouds3_X );

		mFodder->word_42859 = 0x2F;
		mFodder->word_4285B = 0x16B4 * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds2, mBriefing_Intro_Clouds2_X );

		mFodder->word_42859 = 0x22;
		mFodder->word_4285B = 0x1B2C * 4;
		Briefing_Render_2( mBriefing_Intro_Gfx_Clouds1, mBriefing_Intro_Clouds1_X );

		// Trees (Main)
		mFodder->word_42859 = 0x53;
		mFodder->word_4285B = 0x26DC * 4;
		Briefing_Render_1( mBriefing_Intro_Gfx_TreesMain, mBriefing_Intro_Clouds1_X );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		mFodder->word_42859 = 0x23;
		mFodder->word_4285B = 0x375C * 4;
		Briefing_Render_2( mImageBriefingIntro.mData, mBriefing_Intro_X );

		mBriefing_Intro_X += 8;
		if (mBriefing_Intro_X > 0x140)
			mBriefing_Intro_X = 0;

		mBriefing_Intro_Clouds1_X += 4;
		if (mBriefing_Intro_Clouds1_X > 0x140)
			mBriefing_Intro_Clouds1_X = 0;

		mBriefing_Intro_Clouds2_X += 2;
		if (mBriefing_Intro_Clouds2_X > 0x140)
			mBriefing_Intro_Clouds2_X = 0;

		++mBriefing_Intro_Clouds3_X;
		if (mBriefing_Intro_Clouds3_X > 0x140)
			mBriefing_Intro_Clouds3_X = 0;

		mFodder->Video_Sleep();
		g_Window.RenderAt( mImage, cPosition() );
		g_Window.FrameEnd();

		mFodder->Mouse_GetData();
		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mFodder->mImage->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}