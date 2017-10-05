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

	mFodder->mDrawSpriteColumns = di->mColumns;
	mFodder->mDrawSpriteRows = di->mRows;

	int16 ax = di->mY * 160;
	int16 bx = di->mX >> 1;

	mFodder->mDraw_Sprite_FrameDataPtr = di->GetGraphicsPtr(ax +bx);
	mFodder->mDraw_Sprite_PaletteIndex = 0xF0;
	
	video_Draw_Sprite();
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
	
	mImagePStuff = Decode_Image("font.dat", 0x10, 0xA000, 0xD0);


	SetActiveSpriteSheet( eSPRITE_FONT );
}

void cGraphics_PC::Load_Hill_Data() {

	mImageHill = Decode_Image("hill.dat", 0x50, 0xFA00, 0x00);

	Load_Hill_Recruits();
}

void cGraphics_PC::Load_Hill_Recruits() {

	mImageRecruit = Decode_Image("hillbits.dat", 0x10, 0x6900, 0xB0);
	
	SetActiveSpriteSheet(eSPRITE_RECRUIT);
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

	mFodder->mSurfaceMapOverview->paletteSet( mFodder->mPalette, 0, true );
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

void cGraphics_PC::video_Draw_Linear( ) {
	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->mDraw_Sprite_FrameDataPtr;
	int16	ax, cx;
	
	di += 352 * mFodder->mDrawSpritePositionY;

	ax = mFodder->mDrawSpritePositionX;
	ax += mFodder->word_40054;
	//ax >>= 2;
	
	di += ax;
	mFodder->word_42066 = di;
	cx = mFodder->mDrawSpritePositionX;
	cx += mFodder->word_40054;
	cx &= 3;

	uint8 Plane = 0;

	mFodder->byte_42071 = 1 << cx;
	mFodder->mDraw_Source_SkipPixelsPerRow = mFodder->word_42078 - mFodder->mDrawSpriteColumns;
		                 
	//mDrawSpriteColumns >>= 1;
	mFodder->mDraw_Dest_SkipPixelsPerRow = 352 - mFodder->mDrawSpriteColumns;

	di += Plane;
	for( int16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			int8 al = *si;
			if(al)
				*di = al;
			
			si ++;
			di ++;
		}

		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_PC::video_Draw_Sprite() {
	cFodder* Fodder = cFodder::GetSingletonPtr();

	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = Fodder->mDraw_Sprite_FrameDataPtr;
	int16	ax, cx;
	
	di += mImage->GetWidth() * Fodder->mDrawSpritePositionY;

	ax = Fodder->mDrawSpritePositionX;
	ax += Fodder->word_40054;
	//ax >>= 2;
	
	di += ax;
	Fodder->word_42066 = di;
	cx = Fodder->mDrawSpritePositionX;
	cx += Fodder->word_40054;
	cx &= 3;

	uint8 Plane = 0;

	Fodder->byte_42071 = 1 << cx;
	int8 bl = Fodder->mDraw_Sprite_PaletteIndex;
	
	Fodder->mDrawSpriteColumns >>= 1;
	Fodder->mDraw_Source_SkipPixelsPerRow = 160 - Fodder->mDrawSpriteColumns;
	Fodder->mDrawSpriteColumns >>= 1;
	
	Fodder->mDraw_Dest_SkipPixelsPerRow = (uint16) (mImage->GetWidth() - (Fodder->mDrawSpriteColumns*4));

	di += Plane;
	for( int16 dx = Fodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( cx = 0; cx < Fodder->mDrawSpriteColumns; ++cx ) {
			int8 al = (*si) >> 4;
			if(al)
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += Fodder->mDraw_Source_SkipPixelsPerRow;
		di += Fodder->mDraw_Dest_SkipPixelsPerRow;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++Fodder->word_42066;
	}

	si = Fodder->mDraw_Sprite_FrameDataPtr;
	di = Fodder->word_42066;
	di += Plane;
	for( int16 dx = Fodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( cx = Fodder->mDrawSpriteColumns; cx > 0; --cx ) {
			int8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += Fodder->mDraw_Source_SkipPixelsPerRow;
		di += Fodder->mDraw_Dest_SkipPixelsPerRow;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++Fodder->word_42066;
	}
	
	++Fodder->mDraw_Sprite_FrameDataPtr;
	si = Fodder->mDraw_Sprite_FrameDataPtr;
	di = Fodder->word_42066;
	di += Plane;
	for( int16 dx = Fodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( cx = Fodder->mDrawSpriteColumns; cx > 0; --cx ) {
			
			int8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			di+=4;
			
		}
		si += Fodder->mDraw_Source_SkipPixelsPerRow;
		di += Fodder->mDraw_Dest_SkipPixelsPerRow;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++Fodder->word_42066;
	}

	si = Fodder->mDraw_Sprite_FrameDataPtr;
	di = Fodder->word_42066;
	di += Plane;
	for( int16 dx = Fodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( cx = Fodder->mDrawSpriteColumns; cx > 0; --cx ) {
			
			int8 al = (*si) & 0x0F;
			if( al ) 
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += Fodder->mDraw_Source_SkipPixelsPerRow;
		di += Fodder->mDraw_Dest_SkipPixelsPerRow;
	}
}

void cGraphics_PC::Sidebar_Copy_To_Surface( int16 pStartY ) {
	
	uint8*	Buffer = mImage->GetSurfaceBuffer();
	uint8* 	si = (uint8*) mFodder->mMapSptPtr;

	Buffer += (16 * 352) + 16;
	mFodder->byte_42071 = 1 << mFodder->word_40054;

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

void cGraphics_PC::Sidebar_Render_Sprite( int16 pSpriteType, int16 pX, int16 pY ) {
	const sSpriteSheet_pstuff* str2 = &mSpriteSheet_PStuff[pSpriteType];
	
	mFodder->mDrawSpriteColumns = str2->mColumns;
	mFodder->mDrawSpriteRows = str2->mRows;
	
	uint16 ax = 0xA0 * str2->mY;
	uint16 bx = str2->mX >> 1;
	ax += bx;
	
	mFodder->mDraw_Sprite_FrameDataPtr = str2->GetGraphicsPtr( ax );
	
	mFodder->mDraw_Sprite_PaletteIndex = 0xF0;
	
	uint16 w42066 = 0x0C * pY;
	w42066 += pX >> 2;
	
	ax = pX & 3;
	ax *= 0x960;
	w42066 += ax;
	
	uint8* di = ((uint8*)mFodder->mSidebar_Screen_Buffer) + w42066;
	uint8* si = mFodder->mDraw_Sprite_FrameDataPtr;
	
	int8 bl = mFodder->mDraw_Sprite_PaletteIndex;
	mFodder->mDrawSpriteColumns >>= 1;
	
	mFodder->mDraw_Source_SkipPixelsPerRow = 0xA0 - mFodder->mDrawSpriteColumns;
	mFodder->mDrawSpriteColumns >>= 1;
	mFodder->mDraw_Dest_SkipPixelsPerRow = 0x0C - mFodder->mDrawSpriteColumns;
	
	for( uint16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
			
		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	si = mFodder->mDraw_Sprite_FrameDataPtr;
	di =  ((uint8*)mFodder->mSidebar_Screen_Buffer) + w42066;
	
	for( uint16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
		
		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}
		
	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	++mFodder->mDraw_Sprite_FrameDataPtr;
	si = mFodder->mDraw_Sprite_FrameDataPtr;
	di =  ((uint8*)mFodder->mSidebar_Screen_Buffer) + w42066;
	
	for( uint16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
			
		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;

	si = mFodder->mDraw_Sprite_FrameDataPtr;
	di =  ((uint8*)mFodder->mSidebar_Screen_Buffer) + w42066;
	
	for( uint16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
		
		si += mFodder->mDraw_Source_SkipPixelsPerRow;
		di += mFodder->mDraw_Dest_SkipPixelsPerRow;
	}

}

void cGraphics_PC::Sidebar_Render_SquadNames( uint16 pData0, int16 pData4, int16 pData8, uint32*& pData20 ) {
	pData0 += 0x18;

	uint8* SptPtr = (uint8*)mFodder->mMapSptPtr;

	if (pData8 == 0) {
		uint32* esi = (uint32*)(SptPtr + (0x0C * pData0));

		for (int16 cx = pData4; cx > 0; --cx) {
			*pData20++ = *esi;
			*pData20++ = *(esi + 0x258);
			*pData20++ = *(esi + 0x4B0);
			*pData20++ = *(esi + 0x708);
			++esi;

			*pData20++ = *esi;
			*pData20++ = *(esi + 0x258);
			*pData20++ = *(esi + 0x4B0);
			*pData20++ = *(esi + 0x708);
			++esi;

			*pData20++ = *esi;
			*pData20++ = *(esi + 0x258);
			*pData20++ = *(esi + 0x4B0);
			*pData20++ = *(esi + 0x708);
			++esi;
		}
	}
	else {
		uint32* edi = (uint32*)(SptPtr + (0x0C * pData0));

		for (int16 cx = pData4; cx > 0; --cx) {
			*edi = *pData20++;
			*(edi+0x258) = *pData20++;
			*(edi+0x4B0) = *pData20++;
			*(edi+0x708) = *pData20++;
			++edi;
			*edi = *pData20++;
			*(edi+0x258) = *pData20++;
			*(edi+0x4B0) = *pData20++;
			*(edi+0x708) = *pData20++;
			++edi;
			*edi = *pData20++;
			*(edi+0x258) = *pData20++;
			*(edi+0x4B0) = *pData20++;
			*(edi+0x708) = *pData20++;
			++edi;
		}
	}
}

void cGraphics_PC::Recruit_Draw_Hill( ) {

	mFodder->mDraw_Sprite_FrameDataPtr = mImageHill.mData->data() + 0xA00;

	mFodder->mDrawSpritePositionX = 0x40;
	mFodder->mDrawSpritePositionY = 0x28;
	mFodder->mDrawSpriteColumns = 0x110;
	mFodder->mDrawSpriteRows = 0xB0;
	mFodder->word_42078 = 0x140;

	video_Draw_Linear();
	
	for( uint32 x = 0; x < 0xA000; ++x) {
		mImageHill.mData->data()[x] = 0;
	}
}

void cGraphics_PC::Recruit_Draw_HomeAway( ) {
	const char* strHomeAndAway = "HOME                AWAY";
	
	SetActiveSpriteSheet(eSPRITE_RECRUIT);

	// Load Icon
	mFodder->Sprite_Draw_Frame( 0x18, 0, 0, 0 );
	
	// Save Icon (Coloured or Gray)
	int16 Data4 = mFodder->mMission_Save_Availability[ (mFodder->mMissionNumber - 1) ];
	mFodder->Sprite_Draw_Frame( 0x19, 0, Data4, 0x130 );
	
	mFodder->String_CalculateWidth( 320, mFont_Recruit_Width, strHomeAndAway );
	mFodder->String_Print( mFont_Recruit_Width, 0x0D, mFodder->mGUI_Temp_X, 0x0A, strHomeAndAway );
	
	mFodder->Sprite_Draw_Frame( 0x0E, 0x0A, 0, 0x9B );
	
	auto Home = tool_StripLeadingZero(tool_NumToString( mFodder->mTroops_Home ));
	mFodder->Recruit_Draw_String( 0x0D, (int16) (0x9A - (Home.length() * 0x0C)), 0x0A, Home );

	auto Away = tool_StripLeadingZero(tool_NumToString( mFodder->mTroops_Away ));
	mFodder->Recruit_Draw_String( 0x0D, 0xAA, 0x0A, Away );

	SetActiveSpriteSheet(eSPRITE_HILL);
}

void cGraphics_PC::Briefing_Load_Resources() {
	std::string MapName = mFodder->map_Filename_MapGet();
	std::string JunData1 = "p1.dat";
	std::string JunData2 = "p2.dat";
	std::string JunData3 = "p3.dat";
	std::string JunData4 = "p4.dat";
	std::string JunData5 = "p5.dat";

	mFodder->mMap = g_Resource.fileGet( MapName );
	mFodder->map_SetTileType();

	JunData1.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );
	JunData2.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );
	JunData3.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );
	JunData4.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );
	JunData5.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );

	mImageBriefingIntro.mData = g_Resource.fileGet(JunData1);

	mFodder->mBriefing_Intro_Gfx_Clouds1 = g_Resource.fileGet(JunData2);
	mFodder->mBriefing_Intro_Gfx_Clouds2 = g_Resource.fileGet(JunData3);
	mFodder->mBriefing_Intro_Gfx_Clouds3 = g_Resource.fileGet(JunData4);
	mFodder->mBriefing_Intro_Gfx_TreesMain = g_Resource.fileGet(JunData5);

	mFodder->mBriefing_ParaHeli = g_Resource.fileGet( "paraheli.dat" );

	// Copy the palette for the current map tileset, in from paraheli to the briefing intro images
	uint8* si = mFodder->mBriefing_ParaHeli->data() + 0xF00;
	si += 0x30 * mFodder->mMap_TileSet;
	std::memcpy( (mImageBriefingIntro.mData->data() + mImageBriefingIntro.mData->size()) - 0x60, si, 0x30 );

	// Copy the palette from mImagePStuff 
	std::memcpy( (mImageBriefingIntro.mData->data() + mImageBriefingIntro.mData->size()) - 0x30, mImagePStuff.mData->data() + 0xA000, 0x30 );

	// Load the palette
	mImageBriefingIntro.LoadPalette(mImageBriefingIntro.mData->size() - 0x300, 0x100, 0);
}

void cGraphics_PC::Recruit_Sprite_Draw( int16 pColumns, int16 pRows, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pGraphics ) {
	pColumns &= 0xFFFF;
	pRows &= 0xFFFF;

	uint8* es = mImageHill.mData->data();

	mFodder->dword_44A36 = (pData10 - (pData8 >> 1)) << 16;
	mFodder->dword_44A3E = mFodder->dword_44A36;

	mFodder->dword_44A3A = (pData14 - (pDataC >> 1)) << 16;
	int32 eax = (pData8 << 0x10);
	if (eax <= 0)
		return;

	mFodder->dword_44A42 = eax / pColumns;
	eax = pDataC << 0x10;
	if (eax <= 0)
		return;

	mFodder->dword_44A46 = eax / pRows;
	for (int16 bx = 0; bx != pRows; ++bx) {
		mFodder->dword_44A36 = mFodder->dword_44A3E;

		for (int16 cx = 0; cx != pColumns; ++cx) {
			uint8 al = mFodder->sub_2AFF5( pGraphics, bx, cx );
			mFodder->sub_2B016( es, al );
			mFodder->dword_44A36 += mFodder->dword_44A42;
		}

		mFodder->dword_44A3A += mFodder->dword_44A46;
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

	if (g_Fodder.mDrawSpritePositionY < 0) {
		ax = g_Fodder.mDrawSpritePositionY + g_Fodder.mDrawSpriteRows;
		--ax;
		if (ax < 0)
			return false;

		ax -= 0;
		ax -= g_Fodder.mDrawSpriteRows;
		++ax;
		ax = -ax;
		g_Fodder.mDrawSpritePositionY += ax;
		g_Fodder.mDrawSpriteRows -= ax;

		ax *= 0xA0;

		g_Fodder.mDraw_Sprite_FrameDataPtr += ax;
	}

	ax = g_Fodder.mDrawSpritePositionY + g_Fodder.mDrawSpriteRows;
	--ax;

	if (ax > 231) {
		if (g_Fodder.mDrawSpritePositionY > 231)
			return false;

		ax -= 231;
		g_Fodder.mDrawSpriteRows -= ax;
	}

	if (g_Fodder.mDrawSpritePositionX < 0) {
		ax = g_Fodder.mDrawSpritePositionX + g_Fodder.mDrawSpriteColumns;
		--ax;
		if (ax < 0)
			return false;

		ax -= 0;
		ax -= g_Fodder.mDrawSpriteColumns;
		++ax;
		ax = -ax;
		--ax;

		do {
			++ax;
		} while (ax & 3);

		g_Fodder.mDrawSpritePositionX += ax;
		g_Fodder.mDrawSpriteColumns -= ax;
		ax >>= 1;
		g_Fodder.mDraw_Sprite_FrameDataPtr += ax;
	}

	ax = g_Fodder.mDrawSpritePositionX + g_Fodder.mDrawSpriteColumns;
	--ax;

	if (ax > 351) {
		if (g_Fodder.mDrawSpritePositionX > 351)
			return false;

		ax -= 351;
		--ax;

		do {
			++ax;
		} while (ax & 3);

		g_Fodder.mDrawSpriteColumns -= ax;
	}

	if (g_Fodder.mDrawSpriteColumns <= 0)
		return false;

	if (g_Fodder.mDrawSpriteRows <= 0)
		return false;

	return true;
}
