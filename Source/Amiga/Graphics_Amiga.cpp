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
#include "Amiga/SpriteData_Amiga.hpp"

const sStruct3_Amiga stru_A5BC0[] = {
	{ 0x12, 0, 0x90, 0xA8 },
	{ 0x12, 1, 0xA0, 0xA8 },
	{ 0x12, 2, 0xC0, 0xA8 },
	{ 0x13, 0, 0x30, 0xC8 },
	{ 0x13, 1, 0x40, 0xC8 },
	{ 0x13, 2, 0x50, 0xC8 },
	{ 0x13, 3, 0x60, 0xC8 },
	{ 0x13, 4, 0x70, 0xC8 },
	{ 0x13, 5, 0x80, 0xC8 },
	{ 0x13, 6, 0x90, 0xC8 },
	{ 0x13, 7, 0xA0, 0xC8 },
	{ 0x13, 8, 0xB0, 0xC8 },
	{ 0x13, 9, 0xC0, 0xC8 },
	{ 0x13, 0xA, 0xD0, 0xC8 },
	{ 0x13, 0xB, 0xE0, 0xC8 },
	{ -1 }
};

const sStruct0_Amiga stru_A918A[] = {
	{ 0, 0x00, 5, 0x90 },
	{ 0, 0x0A, 5, 0x90 },
	{ 0, 0x14, 5, 0x90 },
	{ 0, 0x1E, 5, 0x90 },
	{ 0, 0x1680, 5, 0x90 },
	{ 0, 0x168A, 5, 0x90 },
	{ 0, 0x1694, 5, 0x90 },
	{ 0, 0x1E, 5, 0x90 },
	{ 1, 0x2170, 0xE, 0x1C },
	{ 1, 0x1D10, 0xE, 0x1C },
	{ 1, 0x2B6, 8, 0x16 },
};

cGraphics_Amiga::cGraphics_Amiga() : cGraphics() {
	mBlkData = 0;

	memset( &mPalette, 0, 0xC0 );
	memset( &mPaletteArmy, 0, 0x20 );
	memset( &mPaletteCopt, 0, 0x20 );
	memset( &mPalletePStuff, 0, 0x20 );
	memset( &mPalleteHill, 0, 0x20 );
}

cGraphics_Amiga::~cGraphics_Amiga() {
	
	delete[] mBlkData;
}

uint8* cGraphics_Amiga::GetSpriteData( uint16 pSegment ) {

	switch (pSegment) {
	case 0:
		mFodder->byte_42070 = 0;
		mBMHD_Current = &mBMHDArmy;
		return mFodder->mDataArmy;

	case 1:
		mFodder->byte_42070 = 0;
		mBMHD_Current = &mBMHDCopt;
		return mFodder->mDataHillBits;

	case 2:
		if (mFodder->mVersion->mKey == "AFX")
			mFodder->byte_42070 = mCursorPalette;
		else
			mFodder->byte_42070 = 0xE0;

		mBMHD_Current = &mBMHDPStuff;
		return mFodder->mDataPStuff;

	case 3:
		mFodder->byte_42070 = 0xF0;
		mBMHD_Current = &mBMHDFont;
		return mFodder->mDataPStuff;

	case 4:
		mFodder->byte_42070 = 0xD0;
		mBMHD_Current = &mBMHDHill;
		return mFodder->mDataBaseBlk;

	default:
		std::cout << "cGraphics_Amiga::GetSpriteData: Invalid ID " << pSegment << "\n";
		return 0;
	}

}

void cGraphics_Amiga::Mouse_DrawCursor() {
	const struct_2* di = &stru_44B50[mFodder->mMouseSpriteCurrent];

	mFodder->word_4206C = di->field_4 >> 3;
	mFodder->word_4206E = di->field_6;

	int16 ax = di->field_2 & 0xFF;
	int16 bx = (di->field_0 >> 3) & -2;

	ax <<= 3;
	int16 d1 = ax;

	ax <<= 2;
	ax += d1;

	mFodder->word_42062 = GetSpriteData( 2 ) + (ax + bx);
	video_Draw_Sprite();
}

void cGraphics_Amiga::LoadpStuff() {

	size_t Size = 0;
	uint8* pstuff = g_Resource.fileGet( "pstuff.lbm", Size );

	DecodeIFF( pstuff, mFodder->mDataPStuff, &mBMHDPStuff, mPalletePStuff );

	delete[] pstuff;
}

void cGraphics_Amiga::Load_Hill_Data() {
	
	size_t Size = 0;
	uint8* pstuff = g_Resource.fileGet( "hills.lbm", Size );

	DecodeIFF( pstuff, mFodder->mDataBaseBlk, &mBMHDHill, mPalleteHill );

	delete[] pstuff;

	Load_Hill_Bits();
}

void cGraphics_Amiga::Load_Hill_Bits() {

	SetSpritePtr( eSPRITE_HILL );
}

void cGraphics_Amiga::Load_Sprite_Font() {
	size_t Size = 0;

	uint8* Font = g_Resource.fileGet( "FONT.RAW", Size );
	memcpy( mFodder->mDataPStuff, Font + 0x20, Size - 0x20 );
	memcpy( mPalleteFont, Font, 0x20 );

	mBMHDFont.mWidth = 0x140;
	mBMHDFont.mHeight = 0x100;
	mBMHDFont.mPlanes = 4;

	delete[] Font;
	//Font = g_Resource.fileGet( "font.pl8", Size );
	//PaletteLoad( Font, 0x0, 0x08 );
	//delete[] Font;
	SetSpritePtr( eSPRITE_FONT );
}

void cGraphics_Amiga::Load_Service_Data() {
	size_t Size = 0;
	uint8* pstuff = g_Resource.fileGet( "rankfont.lbm", Size );

	DecodeIFF( pstuff, mFodder->mDataHillBits, &mBMHDCopt, mPaletteCopt );
	delete[] pstuff;

	pstuff = g_Resource.fileGet( "morphbig.lbm", Size );
	DecodeIFF( pstuff, mFodder->mDataBaseBlk, &mBMHDHill, mPalette );
	delete[] pstuff;
}

void cGraphics_Amiga::imageLoad( const std::string &pFilename, unsigned int pColors ) {
	sILBM_BMHD	Header;
	std::string	Filename = pFilename;

	if (Filename.find('.') == std::string::npos )
		Filename.append( ".raw" );

	size_t Size = 0;
	uint8* File = g_Resource.fileGet( Filename, Size );
	int Colors = 16;

	if (DecodeIFF( File, mFodder->mDataBaseBlk, &Header, mPalette ) == false) {

		if (Size == 51464) {
			Header.mPlanes = 5;
			Colors = 32;
		} else
			Header.mPlanes = 4;

		// Not an iff, so its probably a RAW file
		memcpy( mFodder->mDataBaseBlk, File + (Colors*2), Size - (Colors*2));
		memcpy( mPalette, File, Colors * 2 );
		
		Header.mWidth = 0x140;
		Header.mHeight = 0x101;
	}
	
	mBMHD_Current = &Header;
	g_Fodder.word_42062 = g_Fodder.mDataBaseBlk;
	
	g_Fodder.mDrawSpritePositionX = 16;
	g_Fodder.mDrawSpritePositionY = 22;
	g_Fodder.word_4206C = Header.mWidth >> 3;
	g_Fodder.word_4206E = Header.mHeight;
	g_Fodder.byte_42070 = 0;

	if (Header.mPlanes == 5)
		Colors = 32;

	mImage->clearBuffer();

 	video_Draw_Linear();
	mBMHD_Current = 0;

	delete[] File;
}

 void cGraphics_Amiga::PaletteLoad( const uint8  *pBuffer, uint32 pColorID, uint32 pColors ) {
	int16  color;
	int16  ColorID = pColorID;

	for( ; pColorID < ColorID + pColors; pColorID++) {
		
		// Get the next color codes
		color = readBEWord( pBuffer );
		pBuffer += 2;

		// Extract each color from the word
		//  X X X X   R3 R2 R1 R0     G3 G2 G1 G0   B3 B2 B1 B0

		mFodder->mPalette[pColorID].mRed	= ((color >> 8) & 0xF) << 2;
		mFodder->mPalette[pColorID].mGreen	= ((color >> 4) & 0xF) << 2;
		mFodder->mPalette[pColorID].mBlue	= ((color >> 0) & 0xF) << 2;
	}
}

void cGraphics_Amiga::SetCursorPalette( uint16 pIndex ) {
	mCursorPalette = pIndex;
}

void cGraphics_Amiga::SetSpritePtr( eSpriteType pSpriteType ) {

	switch (pSpriteType) {
	case eSPRITE_IN_GAME:
		mFodder->Sprite_SetDataPtrToBase( off_8BFB8 );
		return;

	case eSPRITE_FONT:
		mFodder->Sprite_SetDataPtrToBase( mSpriteSheetPtr_Font_Amiga );
		return;
		
	case eSPRITE_HILL:
		mFodder->Sprite_SetDataPtrToBase( mHillSpriteSheetPtr_Amiga );
		return;

	case eSPRITE_HILL_UNK:
		mFodder->Sprite_SetDataPtrToBase( off_90F10 );
		return;

	case eSPRITE_BRIEFING:
		mFodder->Sprite_SetDataPtrToBase( off_A6DC8 );
		return;

	case eSPRITE_SERVICE:
		mFodder->Sprite_SetDataPtrToBase( off_A91E2 );
		return;
	}
}

void cGraphics_Amiga::graphicsBlkPtrsPrepare() {

	delete mBlkData;

	mBlkData = new uint8[mFodder->mDataBaseBlkSize + mFodder->mDataSubBlkSize];
	g_Resource.fileLoadTo( mFodder->mFilenameBasePal, (uint8*) mPalette );

	memcpy( mBlkData, mFodder->mDataBaseBlk, mFodder->mDataBaseBlkSize );
	memcpy( mBlkData + mFodder->mDataBaseBlkSize, mFodder->mDataSubBlk, mFodder->mDataSubBlkSize );
}

void cGraphics_Amiga::PaletteSet() {

	mImage->paletteLoad_Amiga( mPalette, 0, 0x40 );
	//mImage->paletteLoad_Amiga( mPaletteArmy, 0x80 );
	mImage->paletteLoad_Amiga( mPalletePStuff, 0xE0 );
	mImage->paletteLoad_Amiga( mPalleteHill, 0xD0 );
	mImage->paletteLoad_Amiga( mPalleteFont, 0xF0 );
}


bool cGraphics_Amiga::DecodeIFF( uint8* pData, uint8* pDataDest, sILBM_BMHD* pBMHD, uint8* pPalette ) {

	if (readBEDWord( pData ) != 'FORM')
		return false;
	pData += 4;

	size_t FileSize = readBEDWord( pData );
	pData += 4;

	if (readBEDWord( pData ) != 'ILBM')
		return false;
	pData += 4;
	FileSize -= 4;

	uint32 Header = 0;
	uint32 Size = 0;

	while (FileSize > 8) {
		Header = readBEDWord( pData );
		pData += 4; FileSize -= 4;
		Size = readBEDWord( pData );
		pData += 4; FileSize -= 4;

		switch (Header) {
		case 'BMHD':
			pBMHD->mWidth = readBEWord( pData ); pData += 2;
			pBMHD->mHeight = readBEWord( pData ); pData += 2;
			pData += 2; pData += 2;	// X, Y
			pBMHD->mPlanes = *pData++;
			pBMHD->mMask = *pData++;
			pBMHD->mCompression = *pData++;
			pData += 2; ++pData; ++pData; ++pData;
			pData += 2; pData += 2;
			FileSize -= Size;
			break;

		case 'BODY': {
			int16 Width = pBMHD->mWidth + 0x0F;
			Width >>= 4;
			Width <<= 1;

			int16 Height = pBMHD->mHeight - 1;

			for (int16 Y = Height; Y >= 0; --Y) {
				uint8* DataDest = pDataDest;

				for (int8 Plane = 0; Plane < pBMHD->mPlanes; ++Plane) {

					for (int16 X = Width; X > 0;) {
						--FileSize;
						int8 d0 = *pData++;

						if (d0 < 0) {
							if (d0 == -128)
								continue;

							int8 d1 = -d0;

							--FileSize;
							d0 = *pData++;

							do {
								*DataDest++ = d0;
								--X;
							} while (d1-- > 0);

							continue;

						}
						else {

							do {
								*DataDest++ = *pData++;
								--X;
								--FileSize;
							} while (d0-- > 0);

						}
					}

					// Move the destination back to start of row
					DataDest -= Width;

					// Move it to the next plane
					DataDest += (Width * pBMHD->mHeight);
				}

				// Next Row
				pDataDest += Width;
			}

			break;
		}

		case 'CMAP':
			for (int16 i = 0; i < Size / 3; ++i) {
				int16 d0 = (int16)*pData++;
				int16 Final = 0;

				d0 >>= 4;
				d0 <<= 8;
				Final += d0;

				d0 = *pData++;
				d0 >>= 4;
				d0 = (int16)d0;
				d0 <<= 4;
				Final += d0;

				d0 = *pData++;
				d0 >>= 4;
				d0 = (int16)d0;
				Final += d0;

				writeBEWord( pPalette, Final );
				pPalette += 2;
				FileSize -= 3;

			}
			break;

		default:
			if (Size & 1)
				++Size;

			pData += Size;
			FileSize -= Size;
			break;
		}
	}

	return true;
}

void cGraphics_Amiga::map_Tiles_Draw() {
	mImage->clearBuffer();
	uint8* Target = mImage->GetSurfaceBuffer();

	uint8* CurrentMapPtr = &mFodder->mMap[mFodder->mMapTilePtr];

	// Y
	for (uint16 cx = 0; cx < 0x10; ++cx) {

		uint8* MapPtr = CurrentMapPtr;
		uint8* TargetRow = Target;

		uint16 StartY = 0;

		if (cx == 0)
			StartY = mFodder->word_3B610;
		else
			StartY = 0;

		if (CurrentMapPtr >= mFodder->mMap) {

			// X
			for (uint16 cx2 = 0; cx2 < 0x16; ++cx2) {
				uint8* TargetTmp = TargetRow;

				uint16 Tile = readLEWord( MapPtr ) & 0x1FF;

				if (/*Tile > 0xE0 && Tile < 0xF0 ||*/ Tile > 0x1C0)
					Tile = 0;

				Tile <<= 7;
				uint8* TilePtr = &mBlkData[Tile];
				uint16 StartX = 0;

				TilePtr += StartY * 2;

				if (cx2 == 0)
					StartX = mFodder->word_3B60E;
				else
					StartX = 0;


				// Each bitfield
				for (uint16 BitField = 0; BitField < 4; ++BitField) {

					// Each Tile Row
					for (uint16 i = StartY; i < 16; ++i) {

						uint16 RowData = readBEWord( TilePtr );
						TilePtr += 2;
						RowData <<= StartX;

						// Each pixel of a Tile Row
						for (uint16 x = StartX; x < 16; ++x) {
							uint8 Bit = (RowData & 0x8000) ? 1 : 0;
							RowData <<= 1;

							if (Bit)
								*(TargetTmp + x) |= (Bit << BitField);

						}

						TargetTmp += mImage->GetWidth();
					}

					// Next Bitfield
					TargetTmp = TargetRow;
				}

				MapPtr += 2;
				TargetRow += (16 - StartX);
			}
		}

		Target += (mImage->GetWidth() * (16 - StartY));
		CurrentMapPtr += (mFodder->mMapWidth << 1);
	}

	mImage->Save();
}

void cGraphics_Amiga::sub_A5B46() {
	const sStruct3_Amiga* a3 = stru_A5BC0;

	for (; a3->field_0 != -1; ++a3) {

		int16 d0 = a3->field_0;
		int16 d1 = a3->field_2;

		uint8* a0 = mFodder->sub_2AE81( d0, d1 );
		
		d1 -= 1;
		d0 >>= 4;
		d0 -= 1;

		uint8* a1 = mFodder->mDataBaseBlk;

		int16 d2 = a3->field_4;
		d2 >>= 3;
		d2 &= 0xFFE;

		a1 += d2;
		d2 = a3->field_6;
		int16 d1_s = d1;
		d2 <<= 3;
		d1 = d2;
		d2 <<= 2;
		d2 += d1;
		d1 = d1_s;

		a1 += d2;
		uint8* a4 = a0;
		uint8* a5 = a1;

		do {

			for (d2 = d1; d2 >= 0; --d2) {

				writeBEWord( a5 , readBEWord( a4 ) );
				writeBEWord( a5 + 0x2828, readBEWord( a4 + 0x2800 ) );
				writeBEWord( a5 + 0x5050, readBEWord( a4 + 0x5000 ) );
				writeBEWord( a5 + 0x7878, readBEWord( a4 + 0x7800 ) );

				a4 += 0x28;
				a5 += 0x28;
			}

			a4 = a0 + 2;
			a5 = a1 + 2;

		} while (--d0 >= 0);
	}
}

void cGraphics_Amiga::sub_2B04B( uint16 pTile, uint16 pDestX, uint16 pDestY ) {
	//TODO
}

void cGraphics_Amiga::map_Load_Resources() {
	mFodder->mFilenameCopt = mFodder->sub_12AA1( mFodder->mFilenameCopt, "lbm" );
	mFodder->mFilenameArmy = mFodder->sub_12AA1( mFodder->mFilenameArmy, "lbm" );

	size_t Size = 0;
	uint8* Copt = g_Resource.fileGet( mFodder->mFilenameCopt, Size );
	uint8* Army = g_Resource.fileGet( mFodder->mFilenameArmy, Size );

	DecodeIFF( Copt, mFodder->mDataHillBits, &mBMHDCopt, mPaletteCopt );
	DecodeIFF( Army, mFodder->mDataArmy, &mBMHDArmy, mPaletteArmy );

	delete[] Copt;
	delete[] Army;

	SetSpritePtr( eSPRITE_IN_GAME );
}

void cGraphics_Amiga::video_Draw_Linear() {

	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->word_42062;
	int16	ax, cx;

	di += mImage->GetWidth() * mFodder->mDrawSpritePositionY;

	ax = mFodder->mDrawSpritePositionX;
	ax += mFodder->word_40054;
	//ax >>= 2;

	di += ax;
	mFodder->word_42066 = di;

	mFodder->word_42074 = (mBMHD_Current->mWidth >> 3) - mFodder->word_4206C;
	mFodder->word_4206C >>= 1;
	mFodder->word_42076 = mImage->GetWidth() - (mFodder->word_4206C * 16);

	// Height
	for (int16 dx = mFodder->word_4206E; dx > 0; --dx) {

		// Width
		for (cx = 0; cx < mFodder->word_4206C; ++cx) {

			DrawPixels_16( si, di );

			di += 16;
			si += 2;
		}

		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}
}

void cGraphics_Amiga::video_Draw_Sprite() {

	uint8*	di = mImage->GetSurfaceBuffer();
	uint8* 	si = mFodder->word_42062;
	int16	ax, cx;

	di += mImage->GetWidth() * mFodder->mDrawSpritePositionY;

	ax = mFodder->mDrawSpritePositionX;
	ax += mFodder->word_40054;
	di += ax;

	mFodder->word_4206C -= 1;
	mFodder->word_4206C <<= 1;

	mFodder->word_42074 = (mBMHD_Current->mWidth >> 3) - (mFodder->word_4206C);
	mFodder->word_42076 = mImage->GetWidth() - (mFodder->word_4206C * 8);

	// Height
	for (int16 dx = mFodder->word_4206E; dx > 0; --dx) {

		// Width
		for (cx = 0; cx < mFodder->word_4206C; ++cx) {

			DrawPixels_8( si, di );

			di += 8;
			si += 1;
		}

		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}
}

void cGraphics_Amiga::sub_144A2( int16 pStartY ) {

	uint8*	Buffer = mImage->GetSurfaceBuffer();
	uint8* 	si = (uint8*)mFodder->mMapSptPtr;

	Buffer += (16 * mImage->GetWidth()) + 16;
	mFodder->byte_42071 = 1 << mFodder->word_40054;
	mFodder->word_42066 = Buffer;

	Buffer = mFodder->word_42066;
	if (pStartY) {
		Buffer += (mImage->GetWidth() * pStartY);
		si += (0x30 * pStartY);
	}

	for (unsigned int Y = 0; Y < 250; ++Y) {

		for (unsigned int X = 0; X < 0x30; X++) {

			if (si >= ((uint8*)mFodder->mMapSptPtr) + (0x300 * 16))
				Buffer[X] = 0;
			else
				Buffer[X] = *si++;
		}

		Buffer += mImage->GetWidth();
	}

}

void cGraphics_Amiga::sub_145AF( int16 pData0, int16 pData8, int16 pDataC ) {

	const struct_2* str2 = &stru_44B50[pData0];

	int16 cx = str2->field_4;
	int16 dx = str2->field_6;
	mFodder->word_4206C = cx >> 3;
	mFodder->word_4206E = dx;

	uint16 ax = str2->field_2 & 0xFF;
	uint16 bx = str2->field_0 >> 3 & -2;

	ax <<= 3;
	int16 d1 = ax;

	ax <<= 2;
	ax += d1;

	mFodder->word_42062 = GetSpriteData( 2 ) + (ax + bx);

	mFodder->word_42074 = 40 - mFodder->word_4206C;
	mFodder->word_4206C >>= 1;
	mFodder->word_42076 = 0x30 - (mFodder->word_4206C * 16);

	uint16 w42066 = (0x30 * pDataC) + pData8;

	uint8* di = ((uint8*)mFodder->word_3D5B7) + w42066;
	uint8* si = mFodder->word_42062;

	// Height
	for (int16 dx = mFodder->word_4206E; dx > 0; --dx) {

		// Width
		for (cx = 0; cx < mFodder->word_4206C; ++cx) {

			DrawPixels_16( si, di );

			di += 16;
			si += 2;
		}

		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}
}

void cGraphics_Amiga::sub_17480( uint16 pData0, int16 pData4, int16 pData8, uint32*& pData20 ) {
	uint8* SptPtr = (uint8*)mFodder->mMapSptPtr;

	uint32* esi = (uint32*)(SptPtr + (0x30 * pData0));

	if (pData8 == 0) {
		for (int16 cx = pData4; cx > 0; --cx) {
			*pData20++ = *(esi + 0);
			*pData20++ = *(esi + 0x0c);
			/**pData20++ = *(esi + 0x450);
			*pData20++ = *(esi + 0x45C);
			*pData20++ = *(esi + 0x468);
			*pData20++ = *(esi + 0x474);
			*pData20++ = *(esi + 0x480);
			*pData20++ = *(esi + 0x48C);
			*pData20++ = *(esi + 0x498);
			*pData20++ = *(esi + 0x4A4);
			*pData20++ = *(esi + 0x4B0);
			*pData20++ = *(esi + 0x4BC);
			*pData20++ = *(esi + 0x4C8);
			*pData20++ = *(esi + 0x4D4);
			*pData20++ = *(esi + 0x4E0);
			*pData20++ = *(esi + 0x4EC);*/
			//*pData20++ = *(esi + 0x4F8);
			++esi;
		}
	}
	else {
		for (int16 cx = pData4; cx > 0; --cx) {
			*(esi+0) = *pData20++;
			*(esi+0x0c) = *pData20++;
/*
			*(esi+0x450) = *pData20++;
			*(esi+0x45C) = *pData20++;
			*(esi+0x468) = *pData20++;
			*(esi+0x474) = *pData20++;
			*(esi+0x480) = *pData20++;
			*(esi+0x48C) = *pData20++;
			*(esi+0x498) = *pData20++;
			*(esi+0x4A4) = *pData20++;
			*(esi+0x4B0) = *pData20++;
			*(esi+0x4BC) = *pData20++;
			*(esi+0x4C8) = *pData20++;
			*(esi+0x4D4) = *pData20++;
			*(esi+0x4E0) = *pData20++;
			*(esi+0x4EC) = *pData20++;*/
			//*(esi+0x4F8) = *pData20++;
			++esi;
		}
	}
}

void cGraphics_Amiga::Recruit_Draw_Hill( ) {
	mFodder->word_42062 = mFodder->word_3E1B7 + (29 * 40) + 6;

	mBMHD_Current = &mBMHDHill;
	mFodder->byte_42070 = 0xD0;

	mFodder->mDrawSpritePositionX = 0x40;
	mFodder->mDrawSpritePositionY = 0x28;
	mFodder->word_4206C = 0x110 >> 3;	//W
	mFodder->word_4206E = 0xB8;		// H
	mFodder->word_42078 = 0x140;
	
	video_Draw_Linear();
}

void cGraphics_Amiga::Recruit_Draw_HomeAway( ) {
	const char* strHomeAndAway = "HOME                AWAY";

	mFodder->sub_13C1C( 0x11, 0, 0, 0xfe );
	int16 Data4 = mFodder->word_3E0E5[ (mFodder->mMissionNumber - 1) ];

	mFodder->sub_13C1C( 0x16, 0, 0x0, 0x0 );
	
	mFodder->sub_13C1C( 0x17, 0, Data4, 0x130 );

	mFodder->sub_13C1C( 0xE, 0x0A, 0, 0x9B );

	mFodder->String_CalculateWidth( 320, mUnkStringModifier_Recruit, strHomeAndAway );
	mFodder->String_Print( mUnkStringModifier_Recruit, 0x0D, mFodder->word_3B301, 0x0A, strHomeAndAway );
	
	std::string Home = tool_StripLeadingZero(tool_NumToString( mFodder->word_397AE ));
	mFodder->sub_16B55( 0x0D, 0x9A - (Home.length() * 0x0C), 0x0A, Home );

	std::string Away = tool_StripLeadingZero(tool_NumToString( mFodder->word_397AC ));
	mFodder->sub_16B55( 0x0D, 0xAA, 0x0A, Away );
}

void cGraphics_Amiga::Service_Draw( int16 pD0, int16 pD1, int16 pD2, int16 pD3 ) {
	
	uint8*	di = mImage->GetSurfaceBuffer() + 16;

	pD2 += 16;
	di += mImage->GetWidth() * pD2;
	di += pD1;

	uint8* si = 0;
	
	if (stru_A918A[pD0].mField_0 == 0) {
		si = mFodder->mDataBaseBlk;
		mBMHD_Current = &mBMHDHill;
	} 
	else {
		si = mFodder->mDataHillBits;
		mBMHD_Current = &mBMHDCopt;
	}

	int16 d0 = stru_A918A[pD0].mField_4 << 1;
	mFodder->word_42074 = 40 - d0;
	mFodder->word_42076 = mImage->GetWidth() - (d0 * 8);
	mFodder->byte_42070 = 0;

	si += stru_A918A[pD0].mField_1;

	// Height
	for (int16 dx = stru_A918A[pD0].mField_6; dx > 0; --dx) {

		// Width
		for (int16 cx = 0; cx < stru_A918A[pD0].mField_4; ++cx) {

			DrawPixels_16( si, di );

			di += 16;
			si += 2;
		}

		si += mFodder->word_42074;
		di += mFodder->word_42076;
	}
}

void cGraphics_Amiga::Briefing_Load_Resources() {
	std::string MapName = mFodder->map_Filename_MapGet();
	std::string JunData1 = "play.lbm";
	std::string JunData2 = "sky.pl8";
	std::string JunData3 = "mid.pl8";
	std::string JunData4 = "fgnd.pl8";
	std::string JunData5 = "fgn2.pl8";
	std::string JunData6 = "heli.pal";

	g_Resource.fileLoadTo( MapName, mFodder->mMap );

	mFodder->map_SetTileType();

	JunData1.insert( 0, mMapTypes[mFodder->mMap_TileSet] );
	JunData2.insert( 0, mMapTypes[mFodder->mMap_TileSet] );
	JunData3.insert( 0, mMapTypes[mFodder->mMap_TileSet] );
	JunData4.insert( 0, mMapTypes[mFodder->mMap_TileSet] );
	JunData5.insert( 0, mMapTypes[mFodder->mMap_TileSet] );
	JunData6.insert( 0, mMapTypes[mFodder->mMap_TileSet] );

	size_t Size = 0;
	uint8* Data = g_Resource.fileGet( JunData2, Size );
	PaletteLoad( Data, 0x00, 16 );
	delete[] Data;

	Data = g_Resource.fileGet( JunData3, Size );
	PaletteLoad( Data, 0x10, 16 );
	delete[] Data;

	Data = g_Resource.fileGet( JunData4, Size );
	PaletteLoad( Data, 0x20, 16 );
	delete[] Data;

	Data = g_Resource.fileGet( JunData5, Size );
	PaletteLoad( Data, 0x30, 16 );
	delete[] Data;

	Data = g_Resource.fileGet( JunData6, Size );
	PaletteLoad( Data, 0x40, 16 );
	delete[] Data;

}

void cGraphics_Amiga::sub_2AF19( int16 pD0, int16 pD1, int16 pD2, int16 pD4, int16 pD5, int16 pD3, uint8* a0 ) {
	int32 d0, d1, d2, d3, d4, d5, d6;

	d0 = pD0; 
	d1 = pD1;
	d2 = pD2; 
	d3 = pD3;
	d4 = pD4;
	d5 = pD5;
	
	uint8* a1 = mFodder->word_3E1B7;

	int32 word_8271A = d0;
	int32 word_8271C = d1;
	int32 word_8271E = d2;
	if (d2 <= 0)
		return;

	int32 word_82720 = d3;
	if (d3 <= 0)
		return;

	d6 = word_8271E;
	d6 >>= 1;
	d4 -= d6;

	d6 = word_82720;
	d6 >>= 1;
	d5 -= d6;


	int32 D1_Saved = d1;
	d5 <<= 3;
	d1 = d5;
	d5 <<= 2;
	d5 += d1;

	d1 = D1_Saved;
	a1 += d5;
	d6 = d4;
	d6 >>= 3;

	a1 += d6;
	d4 &= 7;
	d6 = 7;
	d6 -= d4;

	int32 word_82726 = d6;
	d4 = d2;
	d4 >>= 3;
	d0 &= 0xFFF;
	d1 &= 0xFFF;

	d0 /= d2;
	d1 /= d3;

	d0 = 0;

	d0 = d1 = d2 = d3 = 0;

	d0 = word_8271A;
	d1 = word_8271C;
	int32 d0_u = (d0 % word_8271E) << 16;
	d0 /= word_8271E;
	d0 |= d0_u;

	int32 word_8159E = d0 & 0xFFFF;
	d0 &= 0xFFFF0000;
	d0 /= word_8271E;
	d0 = (d0_u >> 16) | (d0 << 16);

	d0 = (d0 & 0xFFFF0000) | word_8159E;
	d0 = (d0 >> 16) | (d0 << 16);

	int32 d1_u = (d1 % word_82720) << 16;
	d1 /= word_82720;
	d1 |= d1_u;

	word_8159E = d1 & 0xFFFF;
	d1 &= 0xFFFF0000;
	d1 /= word_82720;
	d1 = (d1_u >> 16) | (d1 << 16);

	d1 = (d1 & 0xFFFF0000) | word_8159E;
	d1 = (d1 >> 16) | (d1 << 16);

	//loc_A0850
	do {
		uint8* a3 = a0;
		uint8* a6 = a1;

		d2 = 0;
		d4 = 7;
		d5 = word_8271E;
		d6 = word_82726;

	loc_A0864:;
		do {
			--d5;
			if (d5 < 0)
				goto loc_A08A8;

			if (*a3 & (1 << d4))
				*a6 |= (1 << d6);

			if (*(a3 + 0x2800) & (1 << d4))
				*(a6 + 0x2828) |= (1 << d6);

			if (*(a3 + 0x5000) & (1 << d4))
				*(a6 + 0x5050) |= (1 << d6);

			if (*(a3 + 0x7800) & (1 << d4))
				*(a6 + 0x7878) |= (1 << d6);

			d2 += d0;
			int32 d7 = d2;
			d7 = (d7 >> 16) | (d7 << 16);

			d4 = 7;
			d4 -= (d7 & 0xFFFF);
			d7 = (d7 & 0xFFFF0000) | ((d7 & 0xFFFF) >> 3);
			
			a3 = a0 + (d7 & 0xffff);
		} while (--d6 >= 0);

		++a6;
		d6 = 7;
		goto loc_A0864;

	loc_A08A8:;
		
		a1 += 0x28;
		d3 += d1;
		d3 = (d3 >> 16) | (d3 << 16);

		D1_Saved = d1;

		d3 = (d3 & 0xFFFF0000) | ((d3 & 0xFFFF) << 3);

		d1 = (d1 & 0xFFFF0000) | (d3 & 0xFFFF);
		d3 = (d3 & 0xFFFF0000) | ((d3 & 0xFFFF) << 2);
		d3 += (d1 & 0xFFFF);
		d1 = D1_Saved;
		a0 += (d3 & 0xFFFF);
		d3 = d3 >> 16;

	} while (--word_82720 > 0);

}

void cGraphics_Amiga::DrawPixels_8( uint8* pSource, uint8* pDestination ) {
	uint8	Planes[5];
	uint8	bl = mFodder->byte_42070;

	// Load bits for all planes
	for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane)
		Planes[Plane] = *(pSource + ((mBMHD_Current->mHeight * 40) * Plane) );

	// Loop each pixel 
	for (uint8 X = 0; X < 8; ++X) {
		uint16 Bit = (0x80 >> X);
		uint8 Result = 0;

		// Value for each plane
		for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane) {
			Result |= Planes[Plane] & Bit ? (1 << Plane) : 0;
		}

		if (Result)
			pDestination[X] = bl | Result;
	}
}

void cGraphics_Amiga::DrawPixels_16( uint8* pSource, uint8* pDestination ) {
	uint16	Planes[5];
	uint8	bl = mFodder->byte_42070;

	// Load bits for all planes
	for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane)
		Planes[Plane] = readBEWord( pSource + ((mBMHD_Current->mHeight * 40) * Plane) );

	// Loop each pixel 
	for (uint8 X = 0; X < 16; ++X) {
		uint16 Bit = (0x8000 >> X);
		uint8 Result = 0;

		// Value for each plane
		for (uint8 Plane = 0; Plane < mBMHD_Current->mPlanes; ++Plane) {
			Result |= Planes[Plane] & Bit ? (1 << Plane) : 0;
		}

		if (Result)
			pDestination[X] = bl | Result;
	}
}
