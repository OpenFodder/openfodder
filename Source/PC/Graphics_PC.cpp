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

#include "stdafx.hpp"

uint8* cGraphics_PC::GetSpriteData( uint16 pSegment ) {
	
	switch ( pSegment ) {
		case 0x3B68:
			return g_Fodder.mDataBaseBlk;
			break;

		case 0x4307:
			return g_Fodder.mDataPStuff;
			break;

		case 0x4309:
			return g_Fodder.mDataHillBits;
			break;

		case 0x430B:
			return g_Fodder.mDataArmy;
			break;

		case 0x6717:
			return g_Fodder.word_3E1B7;
			break;

		default:
			std::cout << "Invalid Sprite\n";
			break;
	}
	return 0;
}

void cGraphics_PC::SetSpritePtr( eSpriteType pSpriteType ) {
	
	switch (pSpriteType) {
		case eSPRITE_IN_GAME:
			g_Fodder.Sprite_SetDataPtrToBase( off_32C0C );
			return;
	}
}

void cGraphics_PC::LoadpStuff() {

	g_Resource.fileLoadTo( "pstuff.dat", g_Fodder.mDataPStuff );
}

void cGraphics_PC::graphicsBlkPtrsPrepare() {
	uint16 bx = 0, dx = 0;

	for (uint16 cx = 0; cx < 240; ++cx) {

		mGraphicBlkPtrs[cx + 0x00] = g_Fodder.mDataBaseBlk + bx;
		mGraphicBlkPtrs[cx + 0xF0] = g_Fodder.mDataSubBlk + bx;

		++dx;
		bx += 0x10;
		if (dx % 0x14 == 0) {
			dx = 0;
			bx += 0x12C0;
		}
	}

				
}

void cGraphics_PC::PaletteSet() {

	mImage->paletteSet( g_Fodder.mPalette );
}

void cGraphics_PC::map_Tiles_Draw() {

	uint8* Target = mImage->GetSurfaceBuffer();

	uint8* CurrentMapPtr = &g_Fodder.mMap[g_Fodder.mMapTilePtr];

	// Y
	for (uint16 cx = 0; cx < 0x0F; ++cx) {
		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = g_Fodder.word_3B610;
		else
			StartY = 0;

		// X
		for (uint16 cx2 = 0; cx2 < 0x16; ++cx2) {
			uint8* TargetTmp = TargetRow;

			uint16 Tile = readLEWord( MapPtr ) & 0x1FF;
			if (/*Tile > 0xE0 && Tile < 0xF0 ||*/ Tile > 0x1DF)
				Tile = 0;

			uint8* TilePtr = mGraphicBlkPtrs[Tile];
			uint16 StartX = 0;

			TilePtr += StartY * 0x140;
			
			if (cx2 == 0)
				StartX = g_Fodder.word_3B60E;
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
		CurrentMapPtr += g_Fodder.mMapWidth << 1;
	}

	mImage->Save();
}


void cGraphics_PC::map_Load_Resources() {
	g_Fodder.mFilenameCopt = g_Fodder.sub_12AA1( g_Fodder.mFilenameCopt, "dat" );
	g_Fodder.mFilenameArmy = g_Fodder.sub_12AA1( g_Fodder.mFilenameArmy, "dat" );

	g_Resource.fileLoadTo( g_Fodder.mFilenameCopt, g_Fodder.mDataHillBits );
	g_Fodder.paletteLoad( g_Fodder.mDataHillBits + 0xD2A0, 0x40, 0xB0 );
	g_Fodder.paletteLoad( g_Fodder.mDataHillBits + 0xD360, 0x10, 0x90 );

	g_Resource.fileLoadTo( g_Fodder.mFilenameArmy, g_Fodder.mDataArmy );
	g_Fodder.paletteLoad( g_Fodder.mDataArmy + 0xD200, 0x10, 0xA0 );

	SetSpritePtr( eSPRITE_IN_GAME );
}


void cGraphics_PC::video_Draw_Sprite() {
	cFodder* Fodder = cFodder::GetSingletonPtr();

	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = Fodder->word_42062;
	int16	ax, cx;
	
	di += 352 * Fodder->mDrawSpritePositionY;

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
	
	Fodder->word_4206C >>= 1;
	Fodder->word_42074 = 160 - Fodder->word_4206C;
	Fodder->word_4206C >>= 1;
	
	Fodder->word_42076 = 352 - (Fodder->word_4206C*4);

	di += Plane;
	for( int16 dx = Fodder->word_4206E; dx > 0; --dx ) {
		
		for( cx = 0; cx < Fodder->word_4206C; ++cx ) {
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

	si = Fodder->word_42062;
	di = Fodder->word_42066;
	di += Plane;
	for( int16 dx = Fodder->word_4206E; dx > 0; --dx ) {
		
		for( cx = Fodder->word_4206C; cx > 0; --cx ) {
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
	
	++Fodder->word_42062;
	si = Fodder->word_42062;
	di = Fodder->word_42066;
	di += Plane;
	for( int16 dx = Fodder->word_4206E; dx > 0; --dx ) {
		
		for( cx = Fodder->word_4206C; cx > 0; --cx ) {
			
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

	si = Fodder->word_42062;
	di = Fodder->word_42066;
	di += Plane;
	for( int16 dx = Fodder->word_4206E; dx > 0; --dx ) {
		
		for( cx = Fodder->word_4206C; cx > 0; --cx ) {
			
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
