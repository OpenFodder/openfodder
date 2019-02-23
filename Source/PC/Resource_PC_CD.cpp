/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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

const uint8 cResource_PC_CD::byte_29921[0x100] = {
    3,   3,   3,   3,   3,   3,   3,   3,   3,   3, 
    3,   3,   3,   3,   3,   3,   3,   3,   3,   3, 
    3,   3,   3,   3,   3,   3,   3,   3,   3,   3, 
    3,   3,   4,   4,   4,   4,   4,   4,   4,   4, 
    4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 
    4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 
    4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 
    4,   4,   4,   4,   4,   4,   4,   4,   4,   4, 
    5,   5,   5,   5,   5,   5,   5,   5,   5,   5, 
    5,   5,   5,   5,   5,   5,   5,   5,   5,   5, 
    5,   5,   5,   5,   5,   5,   5,   5,   5,   5, 
    5,   5,   5,   5,   5,   5,   5,   5,   5,   5, 
    5,   5,   5,   5,   5,   5,   5,   5,   5,   5, 
    5,   5,   5,   5,   5,   5,   5,   5,   5,   5, 
    5,   5,   5,   5,   6,   6,   6,   6,   6,   6, 
    6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 
    6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 
    6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 
    6,   6,   6,   6,   6,   6,   6,   6,   6,   6, 
    6,   6,   7,   7,   7,   7,   7,   7,   7,   7, 
    7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 
    7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 
    7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 
    7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8, 
    8,   8,   8,   8,   8,   8
};

const uint8 cResource_PC_CD::byte_29A21[0x101] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
    0,   0,   1,   1,   1,   1,   1,   1,   1,   1, 
    1,   1,   1,   1,   1,   1,   1,   1,   2,   2, 
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 
    2,   2,   2,   2,   3,   3,   3,   3,   3,   3, 
    3,   3,   3,   3,   3,   3,   3,   3,   3,   3, 
    4,   4,   4,   4,   4,   4,   4,   4,   5,   5, 
    5,   5,   5,   5,   5,   5,   6,   6,   6,   6, 
    6,   6,   6,   6,   7,   7,   7,   7,   7,   7, 
    7,   7,   8,   8,   8,   8,   8,   8,   8,   8, 
    9,   9,   9,   9,   9,   9,   9,   9,  10,  10, 
   10,  10,  10,  10,  10,  10,  11,  11,  11,  11, 
   11,  11,  11,  11,  12,  12,  12,  12,  13,  13, 
   13,  13,  14,  14,  14,  14,  15,  15,  15,  15, 
   16,  16,  16,  16,  17,  17,  17,  17,  18,  18, 
   18,  18,  19,  19,  19,  19,  20,  20,  20,  20, 
   21,  21,  21,  21,  22,  22,  22,  22,  23,  23, 
   23,  23,  24,  24,  25,  25,  26,  26,  27,  27, 
   28,  28,  29,  29,  30,  30,  31,  31,  32,  32, 
   33,  33,  34,  34,  35,  35,  36,  36,  37,  37, 
   38,  38,  39,  39,  40,  40,  41,  41,  42,  42, 
   43,  43,  44,  44,  45,  45,  46,  46,  47,  47, 
   48,  49,  50,  51,  52,  53,  54,  55,  56,  57, 
   58,  59,  60,  61,  62,  63, 0
};

cResource_PC_CD::cResource_PC_CD( const std::string& pDataFile ) : cResources() {

	memset( word_26DBE, 0, 0x273 * 2 );
	memset( word_272A4, 0, 0x13A * 2 );
	memset( word_27518, 0, 0x273 * 2 );
	memset( word_279FE, 0, 0x273 * 2 );

	memset( byte_27EE6, 0, 0x1A3C );
	byte_26DB0 = 0;

	mBytesRead = word_26DA8 = word_26DBA = 0;
	word_26DAA = word_26DB4 = saveSI = saveBP = 0;

    
	mData = g_ResourceMan->FileRead(g_Fodder->mVersionCurrent->getDataFilePath(pDataFile));
	if (!mData) {
		std::cout << pDataFile << " not found\n";
		exit( 1 );
	}

	headerLoad();
}

cResource_PC_CD::~cResource_PC_CD() {

}

bool cResource_PC_CD::headerLoad() {
	const uint8* Data = mData->data();

	if(!Data)
		return false;

	uint16 ptrEnd = readLE<uint16>( Data );

	mFiles.clear();

	for( uint16 ptr = 4; ptr != ptrEnd; ) {
		std::string	filename = std::string( (char*) &Data[ptr+1], Data[ptr] );
		++ptr;

		std::transform( filename.begin(), filename.end(), filename.begin(), ::tolower );

		cResource_File	File( filename, 0, 0 );

		ptr += (uint16) File.mName.size();

		// Start address and size
		File.mAddressStart	= readLE<uint32>( &Data[ptr] );

		File.mSize			= readLE<uint32>( &Data[ptr + 4] );

		ptr += 8;

		mFiles.push_back( File );
	}

	return true;
}

void cResource_PC_CD::ExtractFiles() {

	for (auto File : mFiles) {

		auto FileData = fileGet( File.mName );
		/*
		std::string Filename = local_PathGenerate( File.mName, "ExtractedData", eDataType::eData);

		std::ofstream outfile( Filename, std::ofstream::binary );
		outfile.write( (const char*)FileData->data(), FileData->size() );
		outfile.close();*/
	}
}

tSharedBuffer cResource_PC_CD::file_Get( cResource_File *pFile, bool pDecode ) {
	mDataCurrent = mData->data() + pFile->mAddressStart;

	auto result = std::make_shared<std::vector<uint8>>();

	for( uint32 i = 0; i < 0x1A3B; ++i )
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
		word_26DBA = (word_26DBA & 0xFFFF) | dx << 16;

		if ((word_26DA8 | dx) == 0) {
			result->clear();
			return result;
		}

		// seg006:00BA
		sub_26AA4();

		for( uint32 i = 0; i < 0xFC4; ++i )
			byte_27EE6[i] = 0x20;

		word_26DB4 = 0xFC4;
	}

	for(;;) {

		if( (word_26DBA >> 16) < 0 )
			break;

		if( ! word_26DBA )
			break;

		uint16 ax = sub_26C06();

		if( (ax & 0xFF00) == 0 ) {

			result->push_back(ax & 0xFF);

			uint16 bx = word_26DB4;
			byte_27EE6[bx] = ax & 0xFF;
			++bx;
			bx &= 0xFFF;

			word_26DB4 = bx;
			--word_26DBA;
			continue;
		}
		
		
		// seg006:012C
		uint16 saveAX = ax;
	
		ax = sub_26CDF();

		uint16 si = word_26DB4;

		si -= ax;
		--si;

		si &= 0x0FFF;
		ax = saveAX;

		ax += 0xFF03;
		uint16 cx = ax;

		word_26DBA -= cx;
		uint16 bx = word_26DB4;

		for(;cx > 0; --cx) {
			uint8 al = byte_27EE6[si];

			result->push_back( al );
			byte_27EE6[bx] = al;
			++bx;

			bx &= 0xFFF;
			++si;
			si &= 0xFFF;
		}

		word_26DB4 = bx;

	}

	return result;
}

tSharedBuffer cResource_PC_CD::fileGet( std::string pFilename ) {
	std::vector< cResource_File >::iterator		fileIT;

	// Look for local overrides
	auto File = cResources::fileGet( pFilename );
	if (File->size())
		return File;

	std::transform(pFilename.begin(), pFilename.end(), pFilename.begin(), ::tolower);

	for (fileIT = mFiles.begin(); fileIT != mFiles.end(); ++fileIT) {

		if (fileIT->mName == pFilename) {

			return file_Get( &(*fileIT), true );
		}
	}

	g_Debugger->Error("File " + pFilename + " Not Found in DAT");
	std::cout << "Total files inside DAT: " << mFiles.size() << "\n";

	for (fileIT = mFiles.begin(); fileIT != mFiles.end(); ++fileIT)
		g_Debugger->Error(fileIT->mName + "\n");

	exit( 1 );
	return File;
}

uint16 cResource_PC_CD::sub_26CDF() {

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
	
	ax = ((ax & 0x00FF) << 8) | ((ax & 0xFF00) >> 8);
	uint16 saveAX = ax;

	// seg006:41E
	uint16 cx = byte_29921[bx];
	cx &= 0xFF;

	cx -= 2;
	dl = byte_26DB0;

	for( ; cx > 0; --cx ) {
		if( dl == 0 ) {
			// seg006:0445
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

uint8 cResource_PC_CD::data_Read() {
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

void cResource_PC_CD::sub_26AA4() {
	uint16 cx = 0;

	while( cx < 0x13A ) {
		uint16 bx = cx;

		word_279FE[bx] = 1;
		
		uint16 ax = cx;
		ax += 0x273;

		word_27518[bx] = ax;
		word_272A4[bx] = cx;
		++cx;
	}

	// seg006:01CA
	cx = 0;
	uint16 dx = 0x13A;

	while( dx <= 0x272 ) {
		uint16 bx = cx;

		word_26DBE[bx + 1] = dx;
		word_26DBE[bx] = dx;

		uint16 ax = word_279FE[bx];
		ax += word_279FE[bx+1];

		bx = dx;
		//bx <<= 1;
		word_279FE[bx]=  ax;
		word_27518[bx] = cx;
		cx += 2;
		++dx;
	}
	
	word_27EE4 = -1;
	word_26DBE[0x272] = 0;
}

uint16 cResource_PC_CD::sub_26C06() {
	uint8 dl = byte_26DB0;

	uint16 si = word_27518[0x272];

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

			si = word_27518[si];
			continue;

		} else {
			// loc_26C2D
			byte_26DB0 = dl;
			si -= 0x273;

			saveSI = si;
			saveBP = bp;

			if( word_279FE[0x272] == -32768 ) 
				sub_26B11();

			//si <<= 1;
			si = word_272A4[ si ];
			cx = si;

			do {

				++word_279FE[si];
				bp = word_279FE[si];

				uint16 di = si;
				uint16 dx = cx;

				++di; 
				//++di; 
				++dx;

				if( bp > (uint16) word_279FE[di] ) {

					do {
						++di;
					} while( bp > (uint16) word_279FE[di] );

					--di;
					dx = di;

					uint16 tmp = word_279FE[di];
					word_279FE[di] = bp;
					bp = tmp;
					word_279FE[si] = bp;

					uint16 bx = word_27518[si];
					bp = bx;

					word_26DBE[bx] = dx;
					if( bp < 0x273 )
						word_26DBE[bx+1] = dx;

					// loc_26C9C
					tmp = word_27518[di];
					word_27518[di] = bp;
					bp = tmp;

					bx = bp;

					word_26DBE[bx] = cx;
					if( bp < 0x273 )
						word_26DBE[bx+1] = cx;

					word_27518[si] = bp;

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

void cResource_PC_CD::sub_26B11() {
	uint16 cx = 0;
	uint16 dx = 0;

	while( dx < 0x273 ) {
		
		uint16 bx = dx;
		if( word_27518[bx] >= 0x273 ) {	
			uint16 ax = word_279FE[bx];
			++ax;
			ax >>= 1;

			uint16 bx = cx;
			word_279FE[bx] = ax;
			bx = dx;
			ax = word_27518[bx];
			bx = cx;

			word_27518[bx] = ax;
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

		si = word_279FE[bx];
		si += word_279FE[bx + 1];

		bx = cx;

		word_279FE[bx] = si;
		bp = cx;
		--bp;

		for(;;--bp) {

			bx = bp;
			uint16 ax = word_279FE[bx];

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
			uint16 ax = word_279FE[bx];
			word_279FE[si] = ax;
			ax = word_27518[bx];
			word_27518[si] = ax;

			--si;
			--bx;
			++cx;
		};

		cx = savedCX;
		si = savedSI;

		bx = bp;
		word_279FE[si] = si;
		word_27518[bx] = dx;
		dx ++;
		++cx;

	} while( cx < 0x273 );
	// seg006:02D2

	dx = 0;

	while( dx < 0x273 ) {

		uint16 bx = dx;
		bp = word_27518[bx];
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
