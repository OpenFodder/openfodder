#include "stdafx.h"
#include "resources.hpp"
#include "graphics/surface.hpp"

#include <algorithm>

const byte cResources::byte_29921[0x100] = {
	
3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,
6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,
7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8

};

const byte cResources::byte_29A21[0xFF] = {
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

	mData = local_FileRead( "CF_ENG.DAT", mDataSize, false );

	headerLoad();
}

cResources::~cResources() {
	delete mData;
}

bool cResources::headerLoad() {
	if(!mData)
		return false;

	word ptrEnd = readLEWord( mData );

	mFiles.clear();

	for( word ptr = 4; ptr != ptrEnd; ) {
		string	filename	= string( (char*) &mData[ptr], mData[ptr++] );

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

byte *cResources::fileGet( string pFilename, size_t &pFileSize ) {
	vector< cResource_File >::iterator		fileIT;

	std::transform( pFilename.begin(), pFilename.end(), pFilename.begin(), ::tolower );

	for( fileIT = mFiles.begin(); fileIT != mFiles.end(); ++fileIT ) {

		if( fileIT->mName == pFilename ) {

			return file_Get(&(*fileIT), pFileSize,  true );
		}
	}

	return 0;
}

cSurface *cResources::imageLoad( string pFilename, size_t pColors ) {
	size_t fileSize = 0;
	byte *fileBuffer = fileGet(pFilename, fileSize);

	cSurface *surface = new cSurface(320, 200);

	surface->decode( fileBuffer, fileSize, 0, 0x100 );
	
	return surface;
}

byte *cResources::file_Get( cResource_File *pFile, size_t &pFileSize, bool pDecode ) {
	mDataCurrent = &mData[ pFile->mAddressStart ];
	vector<byte>	result;

	for( dword i = 0; i < 0x1000; ++i )
		byte_27EE6[i] = 0x0;

	if(!pDecode) {


	} else {

		// seg006:0075
		mBytesRead = 0;
		byte_26DB0 = 0;

		byte dl = data_Read( );
		byte dh = data_Read( );

		word dx = dl | (dh << 8);
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

		for( dword i = 0; i < 0xFC4; ++i )
			byte_27EE6[i] = 0x20;

		word_26DB4 = 0xFC4;
	}

	for(;;) {

		if( word_26DBC < 0 )
			break;

		if( !(word_26DBC | word_26DBA) )
			break;

		word ax = sub_26C06();

		if( (ax & 0xFF00) == 0 ) {

			result.push_back(ax);

			word bx = word_26DB4;
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
		word saveAX = ax;
	
		ax = sub_26CDF();

		word si = word_26DB4;

		si -= ax;
		--si;

		si &= 0xFFF;
		ax = saveAX;

		ax += 0xFF03;
		short int cx = ax;

		if( word_26DBA > 0 && (word_26DBA - cx) < 0 )
			--word_26DBC;

		word_26DBA -= ax;
		word bx = word_26DB4;

		for(;cx > 0; --cx) {
			byte al = byte_27EE6[si];

			result.push_back( al );
			byte_27EE6[bx] = al;
			++bx;

			bx &= 0xFFF;
			++si;
			si &= 0xFFF;
		}

		word_26DB4 = bx;

	}

	byte *data = new byte[result.size()];

	for(int i = 0; i < result.size(); ++i )
		data[i] = result[i];

	pFileSize = result.size();
	return data;
}

word cResources::sub_26CDF() {

	byte dl = byte_26DB0;
	word ax = 0;
	word bp = saveBP;

	for( word cx = 8; cx > 0; --cx ) {
		if( dl == 0 ) {
			// seg006:3FC
			byte al, ah;

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
	word bx = ax;

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
	word saveAX = ax;

	// seg006:41E
	word cx = byte_29921[bx];
	cx &= 0xFF;

	cx -= 2;
	dl = byte_26DB0;

	for( ; cx > 0; --cx ) {
		if( dl == 0 ) {
			// seg006:3FC
			byte al, ah;

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

byte cResources::data_Read() {
	if( !mBytesRead ) {
		mCurPtr = mDataCurrent;
		mDataCurrent += 0xA00;
		mBytesRead = 0xA00;
	}
	
	byte al = mCurPtr[0];
	++mCurPtr;
	--mBytesRead;

	return al;
}

void cResources::sub_26AA4() {
	word cx = 0;

	while( cx < 0x13A ) {
		word bx = cx;

		word_26DBE[0x620 + bx] = 1;
		
		word ax = cx;
		ax += 0x273;

		word_26DBE[0x3AD + bx] = ax;
		word_26DBE[0x273 + bx] = cx;
		++cx;
	}

	// seg006:01CA
	cx = 0;
	word dx = 0x13A;

	while( dx <= 0x272 ) {
		word bx = cx;

		word_26DBE[bx + 1] = dx;
		word_26DBE[bx] = dx;

		word ax = word_26DBE[0x620 + bx];
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

word cResources::sub_26C06() {
	byte dl = byte_26DB0;

	word si = word_26DBE[0x61F];

	word ax = 0;
	word bp = saveBP;
	word cx = 0;

	for(;;) {

		if( si < 0x273 ) {

			if( dl ) {
				--dl;

			} else {

				// seg006:03CE
				word ax = data_Read( );
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

				word di = si;
				word dx = cx;

				++di; 
				//++di; 
				++dx;

				if( bp > (word) word_26DBE[0x620 + di] ) {

					do {
						++di;
					} while( bp > (word) word_26DBE[0x620 + di] );

					--di;
					dx = di;

					word tmp = word_26DBE[0x620 + di];
					word_26DBE[0x620 + di] = bp;
					bp = tmp;

					word_26DBE[0x620 + si] = bp;

					word bx = word_26DBE[0x3AD + si];
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
	word cx = 0;
	word dx = 0;

	while( dx < 0x273 ) {
		
		word bx = dx;
		if( word_26DBE[0x3AD + bx] >= 0x273 ) {	
			word ax = word_26DBE[0x620 + bx];
			++ax;
			ax >>= 1;

			word bx = cx;
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

	word bp;
	word si;
	word di;

	do {
		bp = dx;
		++bp;
		word bx = dx;

		si = word_26DBE[0x620 + bx];
		si += word_26DBE[0x620 + bx + 1];

		bx = cx;

		word_26DBE[0x620 + bx] = si;
		bp = cx;
		--bp;

		for(;;--bp) {

			bx = bp;
			word ax = word_26DBE[0x620 + bx];

			if( ax <= si )
				break;
		}

		++bp;
		di = cx;
		di -= bp;

		// seg006:28B
		word savedSI = si;
		word savedCX = cx;

		si = bp;
		si += di;
		//si <<= 1;

		bx = si;
		--bx;
		--bx;

		cx = 0;

		while( cx < di ) {
			word ax = word_26DBE[0x620 + bx];
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

		word bx = dx;
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
