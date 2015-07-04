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

#include <algorithm>

const uint8 cResources::byte_29921[0x100] = {
	
3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8

};

const uint8 cResources::byte_29A21[0xFF] = {
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   
	1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   
	2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   
	3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   
	4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   
	6,   6,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   7,   
	8,   8,   8,   8,   8,   8,   8,   8,   9,   9,   9,   9,   9,   9,   9,   9, 
	0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 
	0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 0x10, 
	0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x14, 0x14, 
	0x14, 0x14, 0x15, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17, 0x18, 0x18, 0x19, 
	0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F, 0x20, 0x20, 0x21, 0x21, 
	0x22, 0x22, 0x23, 0x23, 0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27, 0x28, 0x28, 0x29, 0x29, 0x2A, 
	0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 
	0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E
};

cResources::cResources() {
	mData = 0;
	mDataSize = 0;
	mExeDataSize = 0;

	mData = local_FileRead( "CF_ENG.DAT", "", mDataSize );
	mExeData = local_FileRead( "CF_ENG.EXE", "", mExeDataSize );

	headerLoad();
}

cResources::~cResources() {
	delete mData;
}

bool cResources::headerLoad() {
	if(!mData)
		return false;

	uint16 ptrEnd = readLEWord( mData );

	mFiles.clear();

	for( uint16 ptr = 4; ptr != ptrEnd; ) {
		std::string	filename = std::string( (char*) &mData[ptr], mData[ptr++] );

		std::transform( filename.begin(), filename.end(), filename.begin(), ::tolower );

		cResource_File	File( filename, 0, 0 );

		ptr += File.mName.size();

		// Start address and size
		File.mAddressStart	= readLEDWord( &mData[ptr] );

		File.mSize			= readLEDWord( &mData[ptr + 4] );

		ptr += 8;

		mFiles.push_back( File );
	}

	return true;
}

uint8* cResources::fileGet( std::string pFilename, size_t &pFileSize ) {
	std::vector< cResource_File >::iterator		fileIT;

	std::transform( pFilename.begin(), pFilename.end(), pFilename.begin(), ::tolower );

	for( fileIT = mFiles.begin(); fileIT != mFiles.end(); ++fileIT ) {

		if( fileIT->mName == pFilename ) {

			return file_Get(&(*fileIT), pFileSize,  true );
		}
	}

	return 0;
}

size_t cResources::fileLoadTo( std::string pFilename, uint8* pTarget ) {
	size_t Size = 0;

	uint8* File = fileGet( pFilename, Size );

	memcpy( pTarget, File, Size );

	delete[] File;

	return Size;
}

cSurface *cResources::imageLoad( std::string pFilename, size_t pColors ) {
	size_t fileSize = 0;
	uint8* fileBuffer = fileGet(pFilename, fileSize);

	cSurface *surface = new cSurface(320, 200);

	surface->decode( fileBuffer, fileSize, 0, pColors );
	
	return surface;
}

cSurface* cResources::image4PlaneLoad( const std::string &pFilename, size_t pColors ) {
	size_t fileSize = 0;
	uint8* fileBuffer = fileGet(pFilename, fileSize);

	cSurface *surface = new cSurface(320, 230);

	surface->decode4Plane( fileBuffer, (fileSize - (pColors * 3)), 320, 200 );

	if(pColors) {
		surface->paletteLoad( fileBuffer + (fileSize - (pColors * 3)), 0xf0 );
	}

	return surface;
}

uint8* cResources::file_Get( cResource_File *pFile, size_t &pFileSize, bool pDecode ) {
	mDataCurrent = &mData[ pFile->mAddressStart ];
	std::vector<uint8>	result;

	for( uint32 i = 0; i < 0x1000; ++i )
		byte_27EE6[i] = 0x0;

	if(!pDecode) {


	} else {

		// seg006:0075
		mBytesRead = 0;
		byte_26DB0 = 0;

		uint8 dl = data_Read( );
		uint8 dh = data_Read( );

		uint16 dx = dl | (dh << 8);
		word_26DA8 = dx;
		word_26DBA = dx;

		dl = data_Read( );
		dh = data_Read( );
		dx = dl | (dh << 8);

		word_26DAA = dx;
		word_26DBC = dx;

		if( (word_26DA8 | dx) == 0 )
			return 0;

		// seg006:00BA
		sub_26AA4();

		for( uint32 i = 0; i < 0xFC4; ++i )
			byte_27EE6[i] = 0x20;

		word_26DB4 = 0xFC4;
	}

	for(;;) {

		if( word_26DBC < 0 )
			break;

		if( !(word_26DBC | word_26DBA) )
			break;

		uint16 ax = sub_26C06();

		if( (ax & 0xFF00) == 0 ) {

			result.push_back(ax);

			uint16 bx = word_26DB4;
			byte_27EE6[bx] = ax & 0xFF;
			++bx;
			bx &= 0xFFF;

			word_26DB4 = bx;
			--word_26DBA;
			if( word_26DBA != 0 )
				continue;

			--word_26DBC;
			continue;
		}
		
		
		// seg006:012C
		uint16 saveAX = ax;
	
		ax = sub_26CDF();

		uint16 si = word_26DB4;

		si -= ax;
		--si;

		si &= 0xFFF;
		ax = saveAX;

		ax += 0xFF03;
		short int cx = ax;

		if( word_26DBA > 0 && (word_26DBA - cx) < 0 )
			--word_26DBC;

		word_26DBA -= ax;
		uint16 bx = word_26DB4;

		for(;cx > 0; --cx) {
			uint8 al = byte_27EE6[si];

			result.push_back( al );
			byte_27EE6[bx] = al;
			++bx;

			bx &= 0xFFF;
			++si;
			si &= 0xFFF;
		}

		word_26DB4 = bx;

	}

	uint8 *data = new uint8[result.size()];

	for(int i = 0; i < result.size(); ++i )
		data[i] = result[i];

	pFileSize = result.size();
	return data;
}

uint16 cResources::sub_26CDF() {

	uint8 dl = byte_26DB0;
	uint16 ax = 0;
	uint16 bp = saveBP;

	for( uint16 cx = 8; cx > 0; --cx ) {
		if( dl == 0 ) {
			// seg006:3FC
			uint8 al, ah;

			ah = data_Read();
			al = data_Read();

			bp = (ah << 8) | al;

			dl = 0x0F;
		} else
			--dl;

		//seg006:3EF
		bool cf = false;

		if(bp & 0x8000)
			cf = true;

		bp <<= 1;
		ax <<= 1;
		if( cf )
			ax |= 0x01;
	}

	byte_26DB0 = dl;
	uint16 bx = ax;

	ax = 0;
	ax = byte_29A21[bx];
	bool cf = false;

	if( ax & 1 )
		cf = true;
	ax >>= 1;
	if(cf)
		ax |= 0x8000;

	cf = false;
	if( ax & 1 )
		cf = true;
	ax >>= 1;
	if(cf)
		ax |= 0x8000;
	
	ax = (ax & 0xFF) << 8 | (ax & 0xFF00) >> 8;
	uint16 saveAX = ax;

	// seg006:41E
	uint16 cx = byte_29921[bx];
	cx &= 0xFF;

	cx -= 2;
	dl = byte_26DB0;

	for( ; cx > 0; --cx ) {
		if( dl == 0 ) {
			// seg006:3FC
			uint8 al, ah;

			ah = data_Read();
			al = data_Read();

			bp = (ah << 8) | al;

			dl = 0x10;

		} 
		//seg006:431
		bool cf = false;

		if(bp & 0x8000)
			cf = true;

		bp <<= 1;
		bx <<= 1;
		if( cf )
			bx |= 0x01;

		--dl;
	}

	byte_26DB0 = dl;
	bx &= 0x3F;

	saveBP = bp;
	return saveAX | bx;

}

uint8 cResources::data_Read() {
	if( !mBytesRead ) {
		mCurPtr = mDataCurrent;
		mDataCurrent += 0xA00;
		mBytesRead = 0xA00;
	}
	
	uint8 al = mCurPtr[0];
	++mCurPtr;
	--mBytesRead;

	return al;
}

void cResources::sub_26AA4() {
	uint16 cx = 0;

	while( cx < 0x13A ) {
		uint16 bx = cx;

		word_26DBE[0x620 + bx] = 1;
		
		uint16 ax = cx;
		ax += 0x273;

		word_26DBE[0x3AD + bx] = ax;
		word_26DBE[0x273 + bx] = cx;
		++cx;
	}

	// seg006:01CA
	cx = 0;
	uint16 dx = 0x13A;

	while( dx <= 0x272 ) {
		uint16 bx = cx;

		word_26DBE[bx + 1] = dx;
		word_26DBE[bx] = dx;

		uint16 ax = word_26DBE[0x620 + bx];
		ax += word_26DBE[0x621 + bx];

		bx = dx;
		//bx <<= 1;
		word_26DBE[0x620 + bx]=  ax;
		word_26DBE[0x3AD + bx] = cx;
		cx += 2;
		++dx;
	}
	
	word_26DBE[0x893] = -1;
	word_26DBE[0x272] = 0;
}

uint16 cResources::sub_26C06() {
	uint8 dl = byte_26DB0;

	uint16 si = word_26DBE[0x61F];

	uint16 ax = 0;
	uint16 bp = saveBP;
	uint16 cx = 0;

	for(;;) {

		if( si < 0x273 ) {

			if( dl ) {
				--dl;

			} else {

				// seg006:03CE
				uint16 ax = data_Read( );
				ax <<= 8;
				ax |= data_Read( );

				bp = ax;
				dl = 0x0F;
			}
			// loc_26C1F
			if( bp & 0x8000 )
				++si;

			bp <<= 1;
			//si <<= 1;

			si = word_26DBE[0x3AD + si];
			continue;

		} else {
			// loc_26C2D
			byte_26DB0 = dl;
			si -= 0x273;

			saveSI = si;
			saveBP = bp;

			if( word_26DBE[ 0x892 ] == 0x8000 ) 
				sub_26B11();

			//si <<= 1;
			si = word_26DBE[ 0x273 + si ];
			cx = si;

			do {

				++word_26DBE[0x620 + si];
				bp = word_26DBE[0x620 + si];

				uint16 di = si;
				uint16 dx = cx;

				++di; 
				//++di; 
				++dx;

				if( bp > (uint16) word_26DBE[0x620 + di] ) {

					do {
						++di;
					} while( bp > (uint16) word_26DBE[0x620 + di] );

					--di;
					dx = di;

					uint16 tmp = word_26DBE[0x620 + di];
					word_26DBE[0x620 + di] = bp;
					bp = tmp;

					word_26DBE[0x620 + si] = bp;

					uint16 bx = word_26DBE[0x3AD + si];
					bp = bx;

					word_26DBE[bx] = dx;
					if( bp < 0x273 )
						word_26DBE[bx+1] = dx;

					// loc_26C9C
					tmp = word_26DBE[0x3AD + di];
					word_26DBE[0x3AD + di] = bp;
					bp = tmp;

					bx = bp;

					word_26DBE[bx] = cx;
					if( bp < 0x273 )
						word_26DBE[bx+1] = cx;

					word_26DBE[0x3AD + si] = bp;

					si = di;
					cx = dx;
				}

			si = word_26DBE[si];
			cx = si;

			} while( cx );

			bp = saveBP;
			return saveSI;
		}
	}
}

void cResources::sub_26B11() {
	uint16 cx = 0;
	uint16 dx = 0;

	while( dx < 0x273 ) {
		
		uint16 bx = dx;
		if( word_26DBE[0x3AD + bx] >= 0x273 ) {	
			uint16 ax = word_26DBE[0x620 + bx];
			++ax;
			ax >>= 1;

			uint16 bx = cx;
			word_26DBE[0x620 + bx] = ax;
			bx = dx;
			ax = word_26DBE[0x3AD + bx];
			bx = cx;

			word_26DBE[0x3AD + bx] = ax;
			++cx;
		}
		++dx;
	};

	dx = 0;
	cx = 0x13A;

	uint16 bp;
	uint16 si;
	uint16 di;

	do {
		bp = dx;
		++bp;
		uint16 bx = dx;

		si = word_26DBE[0x620 + bx];
		si += word_26DBE[0x620 + bx + 1];

		bx = cx;

		word_26DBE[0x620 + bx] = si;
		bp = cx;
		--bp;

		for(;;--bp) {

			bx = bp;
			uint16 ax = word_26DBE[0x620 + bx];

			if( ax <= si )
				break;
		}

		++bp;
		di = cx;
		di -= bp;

		// seg006:28B
		uint16 savedSI = si;
		uint16 savedCX = cx;

		si = bp;
		si += di;
		//si <<= 1;

		bx = si;
		--bx;
		--bx;

		cx = 0;

		while( cx < di ) {
			uint16 ax = word_26DBE[0x620 + bx];
			word_26DBE[0x620 + si] = ax;
			ax = word_26DBE[0x3AD + bx];
			word_26DBE[0x3AD + si] = ax;

			--si;
			--bx;
			++cx;
		};

		cx = savedCX;
		si = savedSI;

		bx = bp;
		word_26DBE[0x620 + si] = si;
		word_26DBE[0x3AD + bx] = dx;
		dx ++;
		++cx;

	} while( cx < 0x273 );
	// seg006:02D2

	dx = 0;

	while( dx < 0x273 ) {

		uint16 bx = dx;
		bp = word_26DBE[0x3AD + bx];
		if( bp >= 0x273 ) {
			bx = bp;
			word_26DBE[bx] = dx;
		} else {
			bx = bp;
			word_26DBE[bx+1] = dx;
			word_26DBE[bx] = dx;
		}

		++dx;
	}
}

uint8 *cResources::spriteDataGet( size_t pIndex ) {

	uint16 ptrTable = readLEWord( mExeData + 0x241F0 + 0xE1C + (pIndex * 4) );
	
	return mExeData + 0x241F0 + ptrTable;
}
