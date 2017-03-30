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
		case 0x3B68:
			return mFodder->mDataBaseBlk->data();
			break;

		case 0x4307:
			return mFodder->mDataPStuff->data();
			break;

		case 0x4309:
			return mFodder->mDataHillBits->data();
			break;

		case 0x430B:
			return mFodder->mDataArmy->data();
			break;

		case 0x6717:
			return mFodder->mDataHillData->data();
			break;

		default:
			std::cout << "Invalid Sprite\n";
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

	mFodder->mDraw_Sprite_FrameDataPtr = mFodder->mDataPStuff->data() + (ax +bx);
	mFodder->mDraw_Sprite_PalletIndex = 0xF0;
	
	video_Draw_Sprite();
}

void cGraphics_PC::SetSpritePtr( eSpriteType pSpriteType ) {
	
	switch (pSpriteType) {
		case eSPRITE_IN_GAME:
			mFodder->Sprite_SetDataPtrToBase( off_32C0C );
			return;

		case eSPRITE_FONT:
			mFodder->Sprite_SetDataPtrToBase( mSpriteSheetPtr_Font_PC );
			return;

		case eSPRITE_HILL:
			mFodder->Sprite_SetDataPtrToBase( mHillSpriteSheetPtr_PC );
			return;

		case eSPRITE_HILL_UNK:
			mFodder->Sprite_SetDataPtrToBase( off_35E42 );
			return;
		
		case eSPRITE_BRIEFING:
			mFodder->Sprite_SetDataPtrToBase( off_42918 );
			return;

		case eSPRITE_SERVICE:
			mFodder->Sprite_SetDataPtrToBase( off_43963 );
			return;
	}
}

void cGraphics_PC::LoadpStuff() {

	mFodder->mDataPStuff = g_Resource.fileGet( "pstuff.dat" );
	PaletteLoad( mFodder->mDataPStuff->data() + 0xA000, 0x10, 0xF0 );
}

void cGraphics_PC::Load_Sprite_Font() {
	
	mFodder->mDataPStuff = g_Resource.fileGet( "font.dat" );
	PaletteLoad( mFodder->mDataPStuff->data() + 0xA000, 0x10, 0xD0 );

	SetSpritePtr( eSPRITE_FONT );
}

void cGraphics_PC::Load_Hill_Data() {
	
	mFodder->mDataHillData = g_Resource.fileGet( "hill.dat" );
	PaletteLoad( mFodder->mDataHillData->data() + 0xFA00, 0x50, 0x00 );

	Load_Hill_Bits();
}

void cGraphics_PC::Load_Hill_Bits() {
	mFodder->mDataHillBits = g_Resource.fileGet( "hillbits.dat" );
	PaletteLoad( mFodder->mDataHillBits->data() + 0x6900, 0x10, 0xB0 );
	
	SetSpritePtr( eSPRITE_HILL );
}

void cGraphics_PC::Load_Service_Data() {
	
	mFodder->mDataHillBits = g_Resource.fileGet( "rankfont.dat"  );
	PaletteLoad( mFodder->mDataHillBits->data() + 0xA000, 0x80, 0x40 );

	mFodder->mDataBaseBlk = g_Resource.fileGet( "morphbig.dat" );
	PaletteLoad( mFodder->mDataBaseBlk->data() + 0xFA00, 0x40, 0x00 );
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

void cGraphics_PC::sub_2B04B( uint16 pTile, uint16 pDestX, uint16 pDestY ) {
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

	mFodder->mDataHillBits = g_Resource.fileGet( mFodder->mFilenameCopt );
	PaletteLoad( mFodder->mDataHillBits->data() + 0xD2A0, 0x40, 0xB0 );
	PaletteLoad( mFodder->mDataHillBits->data() + 0xD360, 0x10, 0x90 );

	mFodder->mDataArmy = g_Resource.fileGet( mFodder->mFilenameArmy );
	PaletteLoad( mFodder->mDataArmy->data() + 0xD200, 0x10, 0xA0 );

	SetSpritePtr( eSPRITE_IN_GAME );
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
	mFodder->word_42074 = mFodder->word_42078 - mFodder->mDrawSpriteColumns;
		                 
	//mDrawSpriteColumns >>= 1;
	mFodder->word_42076 = 352 - mFodder->mDrawSpriteColumns;

	di += Plane;
	for( int16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			int8 al = *si;
			if(al)
				*di = al;
			
			si ++;
			di ++;
		}

		si += mFodder->word_42074;
		di += mFodder->word_42076;
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
	int8 bl = Fodder->mDraw_Sprite_PalletIndex;
	
	Fodder->mDrawSpriteColumns >>= 1;
	Fodder->word_42074 = 160 - Fodder->mDrawSpriteColumns;
	Fodder->mDrawSpriteColumns >>= 1;
	
	Fodder->word_42076 = (uint16) (mImage->GetWidth() - (Fodder->mDrawSpriteColumns*4));

	di += Plane;
	for( int16 dx = Fodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( cx = 0; cx < Fodder->mDrawSpriteColumns; ++cx ) {
			int8 al = (*si) >> 4;
			if(al)
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += Fodder->word_42074;
		di += Fodder->word_42076;
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
		
		si += Fodder->word_42074;
		di += Fodder->word_42076;
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
		si += Fodder->word_42074;
		di += Fodder->word_42076;
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
		
		si += Fodder->word_42074;
		di += Fodder->word_42076;
	}
}

void cGraphics_PC::sub_144A2( int16 pStartY ) {
	
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

void cGraphics_PC::sub_145AF( int16 pSpriteType, int16 pX, int16 pY ) {
	const sSpriteSheet_pstuff* str2 = &mSpriteSheet_PStuff[pSpriteType];
	
	mFodder->mDrawSpriteColumns = str2->mColumns;
	mFodder->mDrawSpriteRows = str2->mRows;
	
	uint16 ax = 0xA0 * str2->mY;
	uint16 bx = str2->mX >> 1;
	ax += bx;
	
	mFodder->mDraw_Sprite_FrameDataPtr = mFodder->mDataPStuff->data() + ax;
	
	mFodder->mDraw_Sprite_PalletIndex = 0xF0;
	
	uint16 w42066 = 0x0C * pY;
	w42066 += pX >> 2;
	
	ax = pX & 3;
	ax *= 0x960;
	w42066 += ax;
	
	uint8* di = ((uint8*)mFodder->word_3D5B7) + w42066;
	uint8* si = mFodder->mDraw_Sprite_FrameDataPtr;
	
	int8 bl = mFodder->mDraw_Sprite_PalletIndex;
	mFodder->mDrawSpriteColumns >>= 1;
	
	mFodder->word_42074 = 0xA0 - mFodder->mDrawSpriteColumns;
	mFodder->mDrawSpriteColumns >>= 1;
	mFodder->word_42076 = 0x0C - mFodder->mDrawSpriteColumns;
	
	for( uint16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
			
		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	si = mFodder->mDraw_Sprite_FrameDataPtr;
	di =  ((uint8*)mFodder->word_3D5B7) + w42066;
	
	for( uint16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
		
		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}
		
	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	++mFodder->mDraw_Sprite_FrameDataPtr;
	si = mFodder->mDraw_Sprite_FrameDataPtr;
	di =  ((uint8*)mFodder->word_3D5B7) + w42066;
	
	for( uint16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
			
		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;

	si = mFodder->mDraw_Sprite_FrameDataPtr;
	di =  ((uint8*)mFodder->word_3D5B7) + w42066;
	
	for( uint16 dx = mFodder->mDrawSpriteRows; dx > 0; --dx ) {
		
		for( uint16 cx = mFodder->mDrawSpriteColumns; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
		
		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}

}

void cGraphics_PC::sub_17480( uint16 pData0, int16 pData4, int16 pData8, uint32*& pData20 ) {
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

	mFodder->mDraw_Sprite_FrameDataPtr = mFodder->mDataHillData->data() + 0xA00;

	mFodder->mDrawSpritePositionX = 0x40;
	mFodder->mDrawSpritePositionY = 0x28;
	mFodder->mDrawSpriteColumns = 0x110;
	mFodder->mDrawSpriteRows = 0xB0;
	mFodder->word_42078 = 0x140;

	video_Draw_Linear();
	
	for( uint32 x = 0; x < 0xA000; ++x) {
		mFodder->mDataHillData->data()[x] = 0;
	}
}

void cGraphics_PC::Recruit_Draw_HomeAway( ) {
	const char* strHomeAndAway = "HOME                AWAY";
	
	SetSpritePtr( eSPRITE_HILL );

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

	SetSpritePtr( eSPRITE_HILL_UNK );
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

	mFodder->mDataBaseBlk = g_Resource.fileGet( JunData1 );
	mFodder->word_42861 = mFodder->mDataBaseBlk->data();
	mFodder->word_4286D = mFodder->mDataBaseBlk->size();

	mFodder->mDataSubBlk = g_Resource.fileGet( JunData2 );
	mFodder->word_42863 = mFodder->mDataSubBlk->data();

	mFodder->mDataHillBits = g_Resource.fileGet( JunData3 );
	mFodder->word_42865 = mFodder->mDataHillBits->data();

	mFodder->mDataArmy = g_Resource.fileGet( JunData4 );
	mFodder->word_42867 = mFodder->mDataArmy->data();

	// TODO: This is nasty and needs cleaning
	// The original game loads paraheli over pstuff.dat, however the file is small enough
	// that the fonts in pstuff.dat are left intact, these are then used by the briefing intro screen
	// rendering of 'MISSION xx' and the mission name.
	{
		auto tmp = mFodder->mDataPStuff;

		mFodder->mDataPStuff = g_Resource.fileGet( JunData5 );

		size_t Size = mFodder->mDataPStuff->size();
		mFodder->mDataPStuff->resize( tmp->size() );
		mFodder->word_42869 = mFodder->mDataPStuff;

		memcpy( mFodder->mDataPStuff->data() + Size, tmp->data() + Size, tmp->size() - Size );
	}
	// End nasty

	mFodder->word_4286B = g_Resource.fileGet( "paraheli.dat" );

	uint8* si = mFodder->word_4286B->data() + 0xF00;
	si += 0x30 * mFodder->mMap_TileSet;

	// Copy the pallet in
	memcpy( (mFodder->word_42861 + mFodder->mDataBaseBlk->size()) - 0x60, si, 0x30 );
	memcpy( (mFodder->word_42861 + mFodder->mDataBaseBlk->size()) - 0x30, mFodder->mDataPStuff->data() + 0xA000, 0x30 );
}

void cGraphics_PC::Recruit_Sprite_Draw( int16 pColumns, int16 pRows, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pGraphics ) {
	pColumns &= 0xFFFF;
	pRows &= 0xFFFF;

	uint8* es = mFodder->mDataHillData->data();

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

void cGraphics_PC::imageLoad( const std::string &pFilename, unsigned int pColors ) {
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
