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
			return mFodder->mDataBaseBlk;
			break;

		case 0x4307:
			return mFodder->mDataPStuff;
			break;

		case 0x4309:
			return mFodder->mDataHillBits;
			break;

		case 0x430B:
			return mFodder->mDataArmy;
			break;

		case 0x6717:
			return mFodder->word_3E1B7;
			break;

		default:
			std::cout << "Invalid Sprite\n";
			break;
	}
	return 0;
}

void cGraphics_PC::Mouse_DrawCursor() {
	
	const struct_2* di = &stru_44B50[mFodder->mMouseSpriteCurrent];

	mFodder->mDrawSpriteColumns = di->field_4;
	mFodder->mDrawSpriteRows = di->field_6;

	int16 ax = di->field_2 * 160;
	int16 bx = di->field_0 >> 1;

	mFodder->mDrawSpriteFrameDataPtr = mFodder->mDataPStuff + (ax +bx);
	mFodder->byte_42070 = 0xF0;
	
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

	g_Resource.fileLoadTo( "pstuff.dat", mFodder->mDataPStuff );
	PaletteLoad( mFodder->mDataPStuff + 0xA000, 0x10, 0xF0 );
}

void cGraphics_PC::Load_Sprite_Font() {
	
	g_Resource.fileLoadTo( "font.dat", mFodder->mDataPStuff );
	PaletteLoad( mFodder->mDataPStuff + 0xA000, 0x10, 0xD0 );

	SetSpritePtr( eSPRITE_FONT );
}

void cGraphics_PC::Load_Hill_Data() {
	
	g_Resource.fileLoadTo( "hill.dat", mFodder->mDataBaseBlk );
	PaletteLoad( mFodder->word_3E1B7 + 0xFA00, 0x50, 0x00 );

	Load_Hill_Bits();
}

void cGraphics_PC::Load_Hill_Bits() {
	g_Resource.fileLoadTo( "hillbits.dat", mFodder->mDataHillBits );
	PaletteLoad( mFodder->mDataHillBits + 0x6900, 0x10, 0xB0 );
	
	SetSpritePtr( eSPRITE_HILL );
}

void cGraphics_PC::Load_Service_Data() {
	
	g_Resource.fileLoadTo( "rankfont.dat", mFodder->mDataHillBits );
	PaletteLoad( mFodder->mDataHillBits + 0xA000, 0x80, 0x40 );

	g_Resource.fileLoadTo( "morphbig.dat", mFodder->mDataBaseBlk );
	PaletteLoad( mFodder->mDataBaseBlk + 0xFA00, 0x40, 0x00 );
}

void cGraphics_PC::graphicsBlkPtrsPrepare() {
	uint16 bx = 0, dx = 0;

	for (uint16 cx = 0; cx < 240; ++cx) {

		mGraphicBlkPtrs[cx + 0x00] = mFodder->mDataBaseBlk + bx;
		mGraphicBlkPtrs[cx + 0xF0] = mFodder->mDataSubBlk + bx;

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

void cGraphics_PC::map_Tiles_Draw() {

	uint8* Target = mImage->GetSurfaceBuffer();

	uint8* CurrentMapPtr = &mFodder->mMap[mFodder->mMapTilePtr];

	// Y
	for (uint16 cx = 0; cx < 0x0F; ++cx) {
		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = mFodder->word_3B610;
		else
			StartY = 0;

		// X
		for (uint16 cx2 = 0; cx2 < 0x16; ++cx2) {
			uint8* TargetTmp = TargetRow;

			uint16 Tile = readLEWord( MapPtr ) & 0x1FF;
			if (Tile > 0x1DF)
				Tile = 0;

			uint8* TilePtr = mGraphicBlkPtrs[Tile];
			uint16 StartX = 0;

			TilePtr += StartY * 0x140;
			
			if (cx2 == 0)
				StartX = mFodder->word_3B60E;
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
	
	uint8* TilePtr = mGraphicBlkPtrs[pTile];

	for (uint16 i = 0; i < 16; ++i) {

		memcpy( Target, TilePtr, 16 );
		TilePtr += 0x140;
		Target += mFodder->mSurfaceMapOverview->GetWidth();
	}
}

void cGraphics_PC::map_Load_Resources() {
	PaletteLoad( mFodder->mDataBaseBlk + 0xFA00, 0x80, 0x00 );

	mFodder->mFilenameCopt = mFodder->Filename_CreateFromBase( mFodder->mFilenameCopt, "dat" );
	mFodder->mFilenameArmy = mFodder->Filename_CreateFromBase( mFodder->mFilenameArmy, "dat" );

	g_Resource.fileLoadTo( mFodder->mFilenameCopt, mFodder->mDataHillBits );
	PaletteLoad( mFodder->mDataHillBits + 0xD2A0, 0x40, 0xB0 );
	PaletteLoad( mFodder->mDataHillBits + 0xD360, 0x10, 0x90 );

	g_Resource.fileLoadTo( mFodder->mFilenameArmy, mFodder->mDataArmy );
	PaletteLoad( mFodder->mDataArmy + 0xD200, 0x10, 0xA0 );

	SetSpritePtr( eSPRITE_IN_GAME );
}

void cGraphics_PC::video_Draw_Linear( ) {
	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->mDrawSpriteFrameDataPtr;
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
	uint8* 	si = Fodder->mDrawSpriteFrameDataPtr;
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
	int8 bl = Fodder->byte_42070;
	
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

	si = Fodder->mDrawSpriteFrameDataPtr;
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
	
	++Fodder->mDrawSpriteFrameDataPtr;
	si = Fodder->mDrawSpriteFrameDataPtr;
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

	si = Fodder->mDrawSpriteFrameDataPtr;
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

void cGraphics_PC::sub_145AF( int16 pData0, int16 pData8, int16 pDataC ) {
	const struct_2* str2 = &stru_44B50[pData0];
	
	int16 cx = str2->field_4;
	int16 dx = str2->field_6;
	mFodder->mDrawSpriteColumns = cx;
	mFodder->mDrawSpriteRows = dx;
	
	uint16 ax = 0xA0 * str2->field_2;
	uint16 bx = str2->field_0 >> 1;
	ax += bx;
	
	mFodder->mDrawSpriteFrameDataPtr = mFodder->mDataPStuff + ax;
	
	mFodder->byte_42070 = 0xF0;
	
	uint16 w42066 = 0x0C * pDataC;
	w42066 += pData8 >> 2;
	
	ax = pData8 & 3;
	ax *= 0x960;
	w42066 += ax;
	
	uint8* di = ((uint8*)mFodder->word_3D5B7) + w42066;
	uint8* si = mFodder->mDrawSpriteFrameDataPtr;
	
	int8 bl = mFodder->byte_42070;
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
	
	si = mFodder->mDrawSpriteFrameDataPtr;
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
	
	++mFodder->mDrawSpriteFrameDataPtr;
	si = mFodder->mDrawSpriteFrameDataPtr;
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

	si = mFodder->mDrawSpriteFrameDataPtr;
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

	mFodder->mDrawSpriteFrameDataPtr = mFodder->word_3E1B7 + 0xA00;

	mFodder->mDrawSpritePositionX = 0x40;
	mFodder->mDrawSpritePositionY = 0x28;
	mFodder->mDrawSpriteColumns = 0x110;
	mFodder->mDrawSpriteRows = 0xB0;
	mFodder->word_42078 = 0x140;

	video_Draw_Linear();
	
	for( uint32 x = 0; x < 0xA000; ++x) {
		mFodder->word_3E1B7[x] = 0;
	}
}

void cGraphics_PC::Recruit_Draw_HomeAway( ) {
	const char* strHomeAndAway = "HOME                AWAY";
	
	SetSpritePtr( eSPRITE_HILL );

	// Load Icon
	mFodder->Sprite_Draw_Frame( 0x18, 0, 0, 0 );
	
	int16 Data4 = mFodder->mMission_Save_Availability[ (mFodder->mMissionNumber - 1) ];

	// Save Icon
	mFodder->Sprite_Draw_Frame( 0x19, 0, Data4, 0x130 );
	
	mFodder->String_CalculateWidth( 320, mFont_Recruit_Width, strHomeAndAway );
	mFodder->String_Print( mFont_Recruit_Width, 0x0D, mFodder->mGUI_Temp_X, 0x0A, strHomeAndAway );
	
	mFodder->Sprite_Draw_Frame( 0x0E, 0x0A, 0, 0x9B );
	
	std::string Home = tool_StripLeadingZero(tool_NumToString( mFodder->mTroops_Home ));
	mFodder->Recruit_Draw_String( 0x0D, (int16) (0x9A - (Home.length() * 0x0C)), 0x0A, Home );

	std::string Away = tool_StripLeadingZero(tool_NumToString( mFodder->mTroops_Away ));
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

	delete mFodder->mMap;
	mFodder->mMap = g_Resource.fileGet( MapName, mFodder->mMapSize );

	mFodder->map_SetTileType();

	JunData1.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );
	JunData2.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );
	JunData3.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );
	JunData4.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );
	JunData5.insert( 0, mTileType_Names[mFodder->mMap_TileSet] );

	mFodder->mDataBaseBlkSize = g_Resource.fileLoadTo( JunData1, mFodder->mDataBaseBlk );
	mFodder->word_42861 = mFodder->mDataBaseBlk;
	mFodder->word_4286D = mFodder->mDataBaseBlkSize;

	g_Resource.fileLoadTo( JunData2, mFodder->mDataSubBlk );
	mFodder->word_42863 = mFodder->mDataSubBlk;
	
	g_Resource.fileLoadTo( JunData3, mFodder->mDataHillBits );
	mFodder->word_42865 = mFodder->mDataHillBits;

	g_Resource.fileLoadTo( JunData4, mFodder->mDataArmy );
	mFodder->word_42867 = mFodder->mDataArmy;

	g_Resource.fileLoadTo( JunData5, mFodder->mDataPStuff );
	mFodder->word_42869 = mFodder->mDataPStuff;

	g_Resource.fileLoadTo( "paraheli.dat", (uint8*) mFodder->mMapSptPtr );
	mFodder->word_4286B = mFodder->mMapSptPtr;

	uint8* si = ((uint8*)mFodder->mMapSptPtr) + 0xF00;
	si += 0x30 * mFodder->mMap_TileSet;

	memcpy( (mFodder->word_42861 + mFodder->mDataBaseBlkSize) - 0x60, si, 0x30 );
	memcpy( (mFodder->word_42861 + mFodder->mDataBaseBlkSize) - 0x30, mFodder->mDataPStuff + 0xA000, 0x30 );
}

void cGraphics_PC::sub_2AF19( int16 pColumns, int16 pRows, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pGraphics ) {
	pColumns &= 0xFFFF;
	pRows &= 0xFFFF;

	uint8* es = mFodder->word_3E1B7;

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

	size_t fileSize = 0;
	uint8* fileBuffer = g_Resource.fileGet(Filename, fileSize);
	uint8* srcBuffer = fileBuffer;

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
		PaletteLoad( fileBuffer + (fileSize - (0x100 * 3)), pColors );

	delete[] fileBuffer;
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

		g_Fodder.mDrawSpriteFrameDataPtr += ax;
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
		g_Fodder.mDrawSpriteFrameDataPtr += ax;
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
