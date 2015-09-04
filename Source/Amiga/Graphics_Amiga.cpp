#include "stdafx.hpp"

cGraphics_Amiga::cGraphics_Amiga() {
	mBlkData = 0;
	mPalette = 0;
}

void cGraphics_Amiga::graphicsBlkPtrsPrepare() {
	uint16 bx = 0, dx = 0;

	delete mBlkData;
	delete mPalette;

	mBlkData = new uint8[g_Fodder.mDataBaseBlkSize + g_Fodder.mDataSubBlkSize];
	mPalette = g_Resource.fileGet( "afxbase.pal", mPaletteSize );

	memcpy( mBlkData, g_Fodder.mDataBaseBlk, g_Fodder.mDataBaseBlkSize );
	memcpy( mBlkData + g_Fodder.mDataBaseBlkSize, g_Fodder.mDataSubBlk, g_Fodder.mDataSubBlkSize );
}

void cGraphics_Amiga::map_Tiles_Draw() {
	uint8* Target = g_Fodder.mImage->GetSurfaceBuffer();

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
			Tile <<= 7;

			//if (/*Tile > 0xE0 && Tile < 0xF0 ||*/ Tile > 0x1DF)
			//	Tile = 0;

			uint8* TilePtr = &mBlkData[Tile];
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
				TargetTmp += g_Fodder.mImage->GetWidth();
			}

			MapPtr += 2;
			TargetRow += (16-StartX);
		}

		Target += g_Fodder.mImage->GetWidth() * (16-StartY);
		CurrentMapPtr += g_Fodder.mMapWidth << 1;
	}

	g_Fodder.mImage->Save();
}
