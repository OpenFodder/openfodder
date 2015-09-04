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