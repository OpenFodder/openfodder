/*
 *  Open Fodder
 *  ---------------
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
	}
}

sImage cGraphics_PC::Decode_Image(const std::string& pFilename, const size_t pCount, const size_t pPaletteOffset, const size_t pStartIndex) {

	sImage TmpImage;

	TmpImage.mData = g_Resource.fileGet(pFilename);
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
		TilePtr += 0x140;
		Target += pTarget->GetWidth();
	}
}

void cGraphics_PC::MapTiles_Draw() {

	uint8* Target = mSurface->GetSurfaceBuffer();

	uint8* CurrentMapPtr = mFodder->mMap->data() + mFodder->mMapTile_Ptr;

	// Y
	for (uint16 cx = 0; cx < 0x0F; ++cx) {
		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = mFodder->mMapTile_RowOffset;
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
				StartX = mFodder->mMapTile_ColumnOffset;
			else
				StartX = 0;

			// Each Tile Row
			for (uint16 i = StartX; i < 16; ++i) {

				memcpy( TargetTmp, TilePtr + StartX, 16 - StartX );
				TilePtr += 0x140;
				TargetTmp += mSurface->GetWidth();
			}

			MapPtr += 2;
			TargetRow += (16-StartX);
		}

		Target += mSurface->GetWidth() * (16-StartY);
		CurrentMapPtr += mFodder->mMapWidth << 1;
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

		memcpy( Target, TilePtr, 16 );
		TilePtr += 0x140;
		Target += mFodder->mSurfaceMapOverview->GetWidth();
	}
}

void cGraphics_PC::Map_Load_Resources() {
	PaletteLoad( mFodder->mTile_BaseBlk->data() + 0xFA00, 0x80, 0x00 );

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
	mSpriteSheet_InGame2.CopyPalette(&mPalette[0x90], 0x10, 0x90);
	mSpriteSheet_InGame2.CopyPalette(&mPalette[0xB0], 0x40, 0xB0);

	// Sprites on Sheet1 occupy palette range from 0xA0-0xAF
	mSpriteSheet_InGame1.CopyPalette(&mPalette[0xA0], 0x10, 0xA0);

	SetActiveSpriteSheet( eGFX_IN_GAME );
}

void cGraphics_PC::Video_Draw_8(cSurface *pTarget) {
	if (!pTarget)
		pTarget = mSurface;

	uint8*	di = pTarget->GetSurfaceBuffer();
	uint8* 	si = mFodder->mVideo_Draw_FrameDataPtr;

	di += pTarget->GetWidth() * mFodder->mVideo_Draw_PosY;
	di += mFodder->mVideo_Draw_PosX;

	mFodder->word_42066 = di;

	mFodder->mVideo_Draw_Columns >>= 1;
	mFodder->mDraw_Source_SkipPixelsPerRow = 160 - mFodder->mVideo_Draw_Columns;
	mFodder->mDraw_Dest_SkipPixelsPerRow = (uint16)(pTarget->GetWidth() - (mFodder->mVideo_Draw_Columns * 2));

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
	uint8*	di = mSurface->GetSurfaceBuffer();
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
	
	uint8*	Buffer = mSurface->GetSurfaceBuffer();
	uint8* 	si = (uint8*) mFodder->mSidebar_Screen_Buffer;

	Buffer += (16 * mSurface->GetWidth()) + 16;

	for (unsigned int Y = 0; Y < 200; ++Y) {

		for (unsigned int X = 0; X < 0x30; ++X) {

			Buffer[X] = *si++;
		}
			
		Buffer += 352;
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

void cGraphics_PC::Sidebar_Copy_ScreenBuffer( uint16 pData0, int16 pData4, int16 pCopyToScreen, uint32*& pBuffer) {
	pData0 += 0x18;
	uint8* SptPtr = (uint8*)mFodder->mSidebar_Screen_Buffer;
	uint32* BuffPtr = (uint32*)(SptPtr + (48 * pData0));

	// Copying to pData20? or from it?
	if (pCopyToScreen == 0) {

		for (int16 cx = pData4; cx > 0; --cx) {
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

		for (int16 cx = pData4; cx > 0; --cx) {
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
	mFodder->mVideo_Draw_ColumnsMax = 0x140;

	Video_Draw_16();
}

void cGraphics_PC::Recruit_Draw_HomeAway( ) {
	const char* strHomeAndAway = "HOME                AWAY";
	
	SetActiveSpriteSheet(eGFX_RECRUIT);

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

	SetActiveSpriteSheet(eGFX_HILL);
}

void cGraphics_PC::Mission_Intro_Load_Resources() {

	// Briefing images
	std::string JunData1 = mTileTypes[mFodder->mMap_TileSet].mName + "p1.dat";
	std::string JunData2 = mTileTypes[mFodder->mMap_TileSet].mName + "p2.dat";
	std::string JunData3 = mTileTypes[mFodder->mMap_TileSet].mName + "p3.dat";
	std::string JunData4 = mTileTypes[mFodder->mMap_TileSet].mName + "p4.dat";
	std::string JunData5 = mTileTypes[mFodder->mMap_TileSet].mName + "p5.dat";

	mImageMissionIntro.mData = g_Resource.fileGet(JunData1);
	mMission_Intro_Gfx_Clouds1 = g_Resource.fileGet(JunData2);
	mMission_Intro_Gfx_Clouds2 = g_Resource.fileGet(JunData3);
	mMission_Intro_Gfx_Clouds3 = g_Resource.fileGet(JunData4);
	mMission_Intro_Gfx_TreesMain = g_Resource.fileGet(JunData5);

	mBriefing_ParaHeli = g_Resource.fileGet( "paraheli.dat" );

	// Copy the palette for the current map tileset, in from paraheli to the briefing intro images
	uint8* si = mBriefing_ParaHeli->data() + 0xF00;
	si += 0x30 * mFodder->mMap_TileSet;
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

void cGraphics_PC::Load_And_Draw_Image( const std::string &pFilename, unsigned int pColors ) {
	std::string Filename = pFilename;

	if (Filename.find('.') == std::string::npos )
		Filename.append( ".dat" );

	auto fileBuffer = g_Resource.fileGet(Filename);
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

void cGraphics_PC::Mission_Intro_DrawHelicopter( uint16 pID ) {

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

void cGraphics_PC::Mission_Intro_Play() {

	mFodder->Mission_Intro_Draw_Mission_Name();

	switch (mFodder->mMap_TileSet) {
	case eTileTypes_Jungle:
		Mission_Intro_Jungle();
		break;

	case eTileTypes_Desert:
		Mission_Intro_Desert();
		break;

	case eTileTypes_Ice:
		Mission_Intro_Ice();
		break;

	case eTileTypes_Moors:
		Mission_Intro_Mor();
		break;

	case eTileTypes_Int:
		Mission_Intro_Int();
		break;
	}
}


void cGraphics_PC::Briefing_Render_1(tSharedBuffer pDs, int16 pCx) {

	if (mFodder->mBriefing_Render_1_Mode != 0)
		sub_15B98(pDs->data(), pCx);
	else
		sub_15CE8(pDs->data(), pCx);
}


void cGraphics_PC::Briefing_Render_2(tSharedBuffer pSource, int16 pCx) {
	uint8* pDs = pSource->data();

	int16 ax = pCx >> 2;
	int16 dx = ax;

	ax -= 0x50;
	word_4285F = -ax;

	uint8* word_4285D = mSurface->GetSurfaceBuffer() + word_4285B + (dx * 4);
	pCx &= 3;

	++dx;

	uint8* di = word_4285D;

	// Loop the 4 planes
	for (uint8 Plane = 0; Plane < 4; ++Plane) {
		di = word_4285D + Plane;

		for (int16 bx = word_42859; bx > 0; --bx) {
			int16 cx;
			for (cx = word_4285F; cx > 0; --cx) {
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
	word_4285F = -ax;

	uint8* word_4285D = mSurface->GetSurfaceBuffer() + word_4285B + (dx * 4);
	pCx &= 3;

	++dx;

	uint8* di = word_4285D;

	for (uint8 Plane = 0; Plane < 4; ++Plane) {
		di = word_4285D + Plane;

		for (int16 bx = word_42859; bx > 0; --bx) {
			int16 cx = word_4285F;

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

void cGraphics_PC::Mission_Intro_Jungle( ) {
	
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageMissionIntro.CopyPalette(mPalette, 0x100, 0);

	mSurface->paletteSet(mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if( mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mSurface->palette_FadeTowardNew();

		// Clouds
		word_42859 = 0x30;
		word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mMission_Intro_Gfx_Clouds3, word_42875 );

		// Mountain
		word_42859 = 0x38;
		word_4285B = 0x102C * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds2, word_42873);

		// Top of trees
		word_42859 = 0x12;
		word_4285B = 0x1D3C * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds1, word_42871);

		// Trees (Main)
		word_42859 = 0x5C;
		word_4285B = 0x236C * 4;
		Briefing_Render_1( mMission_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		word_42859 = 0x2D;
		word_4285B = 0x33EC * 4;
		Briefing_Render_2( mImageMissionIntro.mData, word_4286F );

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
		g_Window.RenderAt( mSurface, cPosition() );
		g_Window.FrameEnd();

		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mSurface->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}

void cGraphics_PC::Mission_Intro_Desert() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageMissionIntro.CopyPalette(mPalette, 0x100, 0);

	mSurface->paletteSet(mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if(mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mSurface->palette_FadeTowardNew();

		// Clouds
		word_42859 = 0x3A;
		word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mMission_Intro_Gfx_Clouds3, word_42875 );

		word_42859 = 0x4C;
		word_4285B = 0x139C * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds2, word_42873 );

		word_42859 = 0x30;
		word_4285B = 0x1CE4 * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds1, word_42871 );

		// Trees (Main)
		word_42859 = 0x40;
		word_4285B = 0x2D64 * 4;
		Briefing_Render_1( mMission_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		word_42859 = 0x30;
		word_4285B = 0x32E4 * 4;
		Briefing_Render_2( mImageMissionIntro.mData, word_4286F );

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
		g_Window.RenderAt( mSurface, cPosition() );
		g_Window.FrameEnd();

		mFodder->Mouse_GetData();
		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mSurface->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}

void cGraphics_PC::Mission_Intro_Ice() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageMissionIntro.CopyPalette(mPalette, 0x100, 0);

	mSurface->paletteSet(mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if(mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mSurface->palette_FadeTowardNew();

		// Clouds
		word_42859 = 0x24;
		word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mMission_Intro_Gfx_Clouds3, word_42875 );

		word_42859 = 0x42;
		word_4285B = 0x102C * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds2, word_42873 );

		// Ice Caps
		word_42859 = 0x18;
		word_4285B = 0x1CE4 * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds1, word_42871 );

		// Ice Mountains
		word_42859 = 0x58;
		word_4285B = 0x2524 * 4;
		Briefing_Render_1( mMission_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		word_42859 = 0x2E;
		word_4285B = 0x3394 * 4;
		// Trees
		Briefing_Render_2( mImageMissionIntro.mData, word_4286F );

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
		g_Window.RenderAt( mSurface, cPosition() );
		g_Window.FrameEnd();

		mFodder->Mouse_GetData();
		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mFodder->mSurface->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}

void cGraphics_PC::Mission_Intro_Mor() {
	int16 word_4286F = 0;
	int16 word_42871 = 0;
	int16 word_42873 = 0;
	int16 word_42875 = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;
	mImageMissionIntro.CopyPalette(mPalette, 0x100, 0);

	mSurface->paletteSet(mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if(mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mSurface->palette_FadeTowardNew();

		// Clouds
		word_42859 = 0x1D;
		word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mMission_Intro_Gfx_Clouds3, word_42875 );

		word_42859 = 0x40;
		word_4285B = 0x1134 * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds2, word_42873 );

		word_42859 = 0x6;
		word_4285B = 0x2524 * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds1, word_42871 );

		// Trees (Main)
		word_42859 = 0x52;
		word_4285B = 0x2734 * 4;
		Briefing_Render_1( mMission_Intro_Gfx_TreesMain, word_42871 );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		word_42859 = 0x30;
		word_4285B = 0x32E4 * 4;
		Briefing_Render_2( mImageMissionIntro.mData, word_4286F );

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
		g_Window.RenderAt( mSurface, cPosition() );
		g_Window.FrameEnd();

		mFodder->Mouse_GetData();
		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mSurface->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}

void cGraphics_PC::Mission_Intro_Int() {
	int16 mMission_Intro_X = 0;
	int16 mMission_Intro_Clouds1_X = 0;
	int16 mMission_Intro_Clouds2_X = 0;
	int16 mMission_Intro_Clouds3_X = 0;

	mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data();

	mFodder->mVideo_Draw_PaletteIndex = 0xE0;

	mImageMissionIntro.CopyPalette(mPalette, 0x100, 0);

	mSurface->paletteSet(mPalette );

	mFodder->mImageFaded = -1;

	do {
		if (mFodder->mBriefing_Helicopter_Moving == -1)
			mFodder->Briefing_Update_Helicopter();

		if(mFodder->mImageFaded == -1 )
			mFodder->mImageFaded = mSurface->palette_FadeTowardNew();

		// Clouds
		word_42859 = 0x40;
		word_4285B = 0x0C64 * 4;
		Briefing_Render_1( mMission_Intro_Gfx_Clouds3, mMission_Intro_Clouds3_X );

		word_42859 = 0x2F;
		word_4285B = 0x16B4 * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds2, mMission_Intro_Clouds2_X );

		word_42859 = 0x22;
		word_4285B = 0x1B2C * 4;
		Briefing_Render_2( mMission_Intro_Gfx_Clouds1, mMission_Intro_Clouds1_X );

		// Trees (Main)
		word_42859 = 0x53;
		word_4285B = 0x26DC * 4;
		Briefing_Render_1( mMission_Intro_Gfx_TreesMain, mMission_Intro_Clouds1_X );

		mFodder->mVideo_Draw_FrameDataPtr = mBriefing_ParaHeli->data() + mBriefing_ParaHeli_Frames[mFodder->mBriefing_ParaHeli_Frame];

		mFodder->mVideo_Draw_PosX = mFodder->mHelicopterPosX >> 16;		// X
		mFodder->mVideo_Draw_PosY = mFodder->mHelicopterPosY >> 16;		// Y 
		mFodder->mVideo_Draw_Columns = 0x40;
		mFodder->mVideo_Draw_Rows = 0x18;
		if (Sprite_OnScreen_Check())
			Video_Draw_8();

		word_42859 = 0x23;
		word_4285B = 0x375C * 4;
		Briefing_Render_2( mImageMissionIntro.mData, mMission_Intro_X );

		mMission_Intro_X += 8;
		if (mMission_Intro_X > 0x140)
			mMission_Intro_X = 0;

		mMission_Intro_Clouds1_X += 4;
		if (mMission_Intro_Clouds1_X > 0x140)
			mMission_Intro_Clouds1_X = 0;

		mMission_Intro_Clouds2_X += 2;
		if (mMission_Intro_Clouds2_X > 0x140)
			mMission_Intro_Clouds2_X = 0;

		++mMission_Intro_Clouds3_X;
		if (mMission_Intro_Clouds3_X > 0x140)
			mMission_Intro_Clouds3_X = 0;

		mFodder->Video_Sleep();
		g_Window.RenderAt( mSurface, cPosition() );
		g_Window.FrameEnd();

		mFodder->Mouse_GetData();
		if (mFodder->mouse_Button_Status || (mFodder->mMission_Aborted && mFodder->word_428D8)) {
			mFodder->word_428D8 = 0;
			mFodder->mSurface->paletteNew_SetToBlack();
			mFodder->mImageFaded = -1;
		}
	} while (mFodder->word_428D8 || mFodder->mImageFaded != 0);
}