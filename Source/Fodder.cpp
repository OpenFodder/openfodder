/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield
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
#include "SpriteData.hpp"
#include "IntroData.hpp"
#include "Troops.hpp"
#include "UnknownData.hpp"

const char* mBinTable[] = { "rjnull.bin", 
							"rjnull.bin", 
							"player.bin", 
							"player.bin", 
							"player.bin", 
							"player.bin" };

const uint16 mMissionPhaseTable[] = {	01, 02, 01, 04, 03, 02, 03, 04,
										02, 05, 03, 06, 01, 03, 03, 02,
										01, 05, 01, 04, 01, 04, 05, 06 };

cFodder::cFodder() {

	mResources = new cResources();
	mWindow = new cWindow();

	mTicksDiff = 0;
	mTicks = 0;

	mIntroDone = 0;
	mErrorCode = 0;
	word_427D4 = -1;

	mMouseButtons = 0;

	mouse_Button_Status = 0;
	mouse_Pos_Column = 0;
	mouse_Pos_Row = 0;

	mButtonPressLeft = mButtonPressRight = 0;
	word_39EF8 = word_39F02 = 0;
	word_39EF6 = word_39EFA = 0;
	word_39EFC = word_39F04 = 0;
	word_3AA43 = 0;
	word_3ABA7 = 0;
	word_3A9B2 = 0;
	word_3E1B7 = 0;
	
	mMapSptPtr = new uint16[4800];

	word_40054 = 0;
	word_42062 = 0;
	word_42066 = 0;
	word_42068 = 0;
	word_4206A = 0;
	word_4206C = 0;
	word_4206E = 0;
	byte_42070 = 0;
	byte_42071 = 0;
	word_42074 = 0;
	word_42076 = 0;

	mouseData0 = new sMouseData();
	mouseData1 = new sMouseData();

	mDataPStuff = 0;
	mDataHillBits = 0;
	mDataArmy = new uint8[54528];
	mDataSubBlk = new uint8[64768];

	word_3BDAD = new int16[4800];
	word_3A3BB = 0;
	word_3A3BD = 0;
	word_42072 = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A3FD[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A475[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A4ED[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A565[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A5DD[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A655[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A6CD[x] = 0;
	 
	for (unsigned int x = 0; x < 30; ++x)
		dword_3A745[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A7BD[x] = 0;

	for (unsigned int x = 0; x < 30; ++x)
		dword_3A835[x] = 0;

	for (unsigned int x = 0; x < 0x18; ++x) {
		word_3E0E5[x] = 0;
	}
}

cFodder::~cFodder() {
	
}

void cFodder::sub_10BBC() {
	word_390A4 = 0;
	word_390A6 = 0;
	word_390AE = 0;
	word_390B8 = 0;
	mMapNumber = 0;
	mMissionNumber = 0;
	mMissionPhase = 0;
	word_39096 = 0;
	word_390CE = 0;
	word_390D0 = 0;
	word_390D2 = 0;
	word_390D4 = 0;

	for (unsigned int x = 0; x < 8; ++x) {
		word_390D6[x] = 0;
	}

	word_390E8 = 0;
	word_390EA = 0;
	mMissionPhaseRemain = 0;
	mMissionPhases = 0;
	word_390F4 = 0;

	for (unsigned int x = 0; x < 8; ++x) {
		stru_390FA[x].field_0 = 0;
		stru_390FA[x].mRank = 0;
		stru_390FA[x].field_3 = 0;
		stru_390FA[x].field_4 = 0;
		stru_390FA[x].field_6 = 0;
		stru_390FA[x].field_8 = 0;
		stru_390FA[x].mNumberOfKills = 0;
	}

	word_3915A = 0;
	word_3915E = 0;

	for (unsigned int x = 0; x < 361; ++x)
		word_391D2[x] = 0;

	dword_394A4 = 0;
	dword_394A8 = 0;

	for (unsigned int x = 0; x < 361; ++x)
		word_394AC[x] = 0;

	dword_3977E = 0;

	for (unsigned int x = 0; x < 5; ++x) {
		stru_3978E[x].field_0 = 0;
		stru_3978E[x].field_1 = 0;
		stru_3978E[x].field_3 = 0;
	}
	word_397AC = 0;
	word_397AE = 0;
	word_397D2 = 0;
	word_397D4 = 0;
	mTroopsAvailable = 0;
}

void cFodder::sub_10B6D() {
	word_3915E = -1;
	word_3915A = -1;

	for (unsigned int x = 0; x < 8; ++x) {
		stru_390FA[x].field_4 = -1;
		stru_390FA[x].field_0 = -1;
		stru_390FA[x].mRank = 0;
		stru_390FA[x].field_3 = 0;
	}
}

void cFodder::sub_10BFA() {
	
	for (unsigned int x = 0; x < 5; ++x) {
		stru_3978E[x].field_0 = -1;
		stru_3978E[x].field_1 = -1;
		stru_3978E[x].field_3 = -1;
	}
}

void cFodder::sub_10D61() {
	// Clear memory 2454 to 3B58
	mButtonPressLeft = 0;
	mButtonPressRight = 0;
	word_39EF8 = 0;
	word_39EFA = 0;
	word_39EFC = 0;
	word_39F02 = 0;
	word_39F04 = 0;
	//word_39F06 = 0;

	dword_39F98 = 0;
	dword_39F9C = 0;
	word_39FCE = 0 ;
	
	dword_3A000 = 0;
	dword_3A004 = 0;
	dword_3A008 = 0;
	word_3A016 = 0;
	word_3A9F7 = 0;
	word_3AA17 = 0;
	word_3AA19 = 0;
	word_3AA51 = 0;
	word_3AA55 = 0;
	word_3AA67 = 0;
	word_3AA71 = 0;
	word_3AAC7 = 0;
	dword_3AAC9 = 0;
	word_3AACD = 0;
	word_3AACF = 0;
	word_3AAD1 = 0;
	word_3AB39 = 0;
	word_3AC19 = 0;
	word_3AC21 = 0;
	dword_3B1CB = 0;

	for (uint16 x = 0; x < 0x0F; ++x ) 
		word_3B1CF[x] = 0;

	word_3B1EF = 0;
	word_3B1F1 = 0;
	dword_3B1FB = 0;
	
	word_3B2CD = 0;
	word_3B2CF = 0;

	word_3B2FD = 0;
	word_3B2FF = 0;

	word_3B301 = 0;
	word_3B303 = 0;
	word_3B305 = 0;
	word_3B307 = 0;
	word_3B447 = 0;
	word_3B4F3 = 0;
}

void cFodder::sub_10D9F() {
	word_3BDE7 = 1;
	word_3BDE9 = 1;
	word_3BDEB = 5;
	// -1
	off_3BDEF[0] = dword_3BE03;
	off_3BDEF[1] = dword_3BE27;
	off_3BDEF[2] = dword_3BE4B;
	off_3BDEF[3] = dword_3BE6F;
	off_3BDEF[4] = dword_3BE93;
	
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE03[x] = -1;
	}
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE27[x] = -1;
	}
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE4B[x] = -1;
	}
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE6F[x] = -1;
	}
	for (unsigned int x = 0; x < 9; ++x) {
		dword_3BE93[x] = -1;
	}
	word_3BEB9 = 1;
	word_3BEBB = 1;
	word_3BEBD = 1;
	word_3BEC1 = 0x1A;
	word_3BEC3 = 0x12;
	word_3BEC9 = 0xC8;
	word_3BED5[0] = 2;
	word_3BED5[1] = 2;
	word_3BED5[2] = 2;
	word_3BED5[3] = 2;
	word_3BED5[4] = 2;
	word_3BEDF[0] = 1;
	word_3BEDF[1] = 1;
	word_3BEDF[2] = 1;
	word_3BEDF[3] = 1;
	word_3BEDF[4] = 1;
	word_3BEDF[5] = 0;
	word_3BEDF[6] = 0;
	word_3BEDF[7] = 0;
	word_3BEDF[8] = 0;
	word_3BEDF[9] = 0;

	off_3BEF3[0] = dword_3A3FD;
	off_3BEF3[1] = dword_3A475;
	off_3BEF3[2] = dword_3A4ED;
	off_3BEF3[3] = dword_3A565;
	off_3BEF3[4] = dword_3A5DD;
	off_3BEF3[5] = dword_3A655;
	off_3BEF3[6] = dword_3A6CD;
	off_3BEF3[7] = dword_3A745;
	off_3BEF3[8] = dword_3A7BD;
	off_3BEF3[9] = dword_3A835;

	word_3BF1B = -1;
	word_3BF1E[0] = 0;
	word_3BF1E[1] = 0;
	word_3BF1E[2] = 0;
	word_3BF1E[3] = 0;
	word_3BF1E[4] = 0;
	word_3BF1E[5] = 0;
}

void cFodder::sub_10DEC() {
	word_39FD4 = 0;
	word_3A054 = 0;
	dword_3A39D = 0;
	word_39FD0 = 0;
	word_3A9AA = 0;
	word_3A9AC = 0;
	dword_39F7C = 0;
	word_3A9F7 = 0x23;
	dword_3A05A = 0;
	byte_3A05E = 0;
	dword_3A8DB = 0;
	byte_3A8DF = 0;
	word_3A06B = -1;
	word_3A28D = 0x0C;
	dword_3A3FD[0] = -1;
	dword_3A475[0] = -1;
	dword_3A4ED[0] = -1;
	dword_3A565[0] = -1;
	dword_3A5DD[0] = -1;
	dword_3A655[0] = -1;
	dword_3A6CD[0] = -1;
	dword_3A745[0] = -1;
	dword_3A7BD[0] = -1;
	dword_3A835[0] = -1;
}

void cFodder::sub_10EA4() {
	dword_39F98 = 0;
	dword_39F9C = 0;
	mMapSpt_Loaded[0] = -32768;
	word_39FCE = 0;
}

void cFodder::sub_10EC3() {
	sub_10EE3();
	dword_3A000 = 0;
	dword_3A004 = 0;
	dword_3A008 = 0;

}

void cFodder::sub_10EE3() {

	int16* Data = mMapSpt_Loaded;
	for( int16 count = 0x2C; count >= 0; --count ) {
		
		for( int16 count2 = 0x3A; count2 >= 0; --count2 ) {
			*Data++ = 0;
		}
		*(Data - 0x3B) = -32768;
	}
	*(Data - 0x3B) = -1;
}

void cFodder::map_Load_Spt() {
	
	std::string Filename_Map = map_Filename_MapGet();
	std::string Filename_Spt = map_Filename_SptGet();

	uint8* Map = g_Resource.fileGet(Filename_Spt, mMapSptSize);
	tool_EndianSwap( (uint8*) Map, mMapSptSize );
	//memset( mMapSpt_Loaded, 0, sizeof(mMapSpt_Loaded));

	memcpy( mMapSptPtr, Map, mMapSptSize );
	delete[] Map;

	word_3AA17 = 0;
	word_3AA19 = 0;
	
	uint16* dword_37AC0 = mMapSptPtr;
	int16* dword_37AC4 = mMapSpt_Loaded;
	
	uint16* dword_37AC8 = dword_37AC0 + (mMapSptSize / 2);
	int16 dword_37ABC = 0x0A;

	for(uint16 mTmpCount = 0; dword_37AC0 != dword_37AC8; dword_37AC4 += 0x3B ) {
		++dword_37AC0;

		int16* si = dword_37AC4;
		si[0x04] = 0x7C;
		
		uint16 ax = mTmpCount / 8;

		si[0x19] = ax;
		++dword_37AC0;
		ax = dword_37AC0[0];
		++dword_37AC0;
		
		ax += 0x10;
		si[0x00] = ax;
		si[0x13] = ax;
		
		ax = dword_37AC0[0];
		++dword_37AC0;
		si[0x02] = ax;
		si[0x14] = ax;
		
		ax = dword_37AC0[0];
		++dword_37AC0;
		si[0x0C] = ax;
		
		if( si[0x0C] == 0x6A || si[0x0C] == 0x48 ) {
				
			++word_3AA19;
		}
		
		if( si[0x0C] != 0 ) {
			//10EE
			if( si[0x0C] == 0x24 ) {
				si[0x11] = 1;
				++word_3AA17;
				
			} else {
				if( si[0x0C] == 5 )
					++word_3AA17;
			}
			
			dword_37ABC += 0x0A;
			if( si[0x31] > 4 )
				dword_37ABC = 0;
			
			si[0x25] = dword_37ABC;
			
		} else {
			// 113C
			++mTmpCount;
			si[0x25] = 0;
		}
		
		// 114B
	}
}

std::string cFodder::map_Filename_Get() {
	std::stringstream	filename;
	
	filename << "mapm";
	filename << (mMapNumber + 1);

	return filename.str();
}

std::string cFodder::map_Filename_MapGet() {
	std::string	filename = map_Filename_Get();

	filename.append(".map");

	return filename;
}

std::string cFodder::map_Filename_SptGet() {
	std::string	filename = map_Filename_Get();

	filename.append(".spt");

	return filename;
}

void cFodder::map_Troops_Prepare() {
	word_397D2 = 0;
	int16* dword_37AC0 = mMapSpt_Loaded;
	
	for( int16 mTmpCount = 0x1D; mTmpCount > 0; --mTmpCount ) {
		int16* si = dword_37AC0;
		if( *si != -32768 ) {
			
			if( si[0x0C] == 0 )
				++word_397D2;
		}
		dword_37AC0 += 0x3B;
	}
	
	word_397D4 = 0;
	
	for( int16 x = 7; x >= 0; --x ) {
		
		if( stru_390FA[x].field_0 != -1 ) {
			--word_397D2;
			++word_397D4;
		}
	}
	
	int16 ax = word_397D2;
	
	if( word_397D2 >= 0 ) {

		if( ax > word_390CE )
			ax = word_390CE;
		
		word_397D2 = ax;
		word_390CE -= ax;
	}
	
	ax += word_397D4;
	mTroopsAvailable = ax;
}

void cFodder::map_Load_Players() {

	for (int16 Data1c = 7; Data1c >= 0; --Data1c) {
		sMission_unk0* Data20 = stru_390FA;

		for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {

			if (Data20->field_0 == -1) {

				sMission_unk0* Data24 = Data20 + 1;
				sMission_unk0* Data28 = Data20;

				*Data28 = *Data24;
				Data28->field_0 = -1;
				Data28->mRank = 0;
			}
		}
	}
	
	sMission_unk0* Data20 = stru_390FA;

	//seg000:1347
	for (int16 Data1c = 7; Data1c >= 0; --Data1c, ++Data20) {
		sMission_unk0* Data24 = stru_390FA;

		for (int16 Data18 = 7; Data18 >= 0; --Data18, ++Data24) {

			if (Data20 == Data24)
				continue;

			if (Data20->field_0 == -1 || Data24->field_0 == -1)
				continue;

			if (Data20->mRank != Data24->mRank)
				continue;

			if (Data20->mNumberOfKills <= Data24->mNumberOfKills)
				continue;

			sMission_unk0 Spare = *Data20;

			*Data20 = *Data24;
			*Data24 = Spare;
		}
	}
}

void cFodder::sub_1142D() {
	sub_301F7();

	if (!word_3ABA7) {

		if (word_390D4) {
			sMission_unk0* Data20 = stru_390FA;
			uint16* Data24 = word_390D6;

			for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
				Data20->field_4 = *Data24++;
			}
		}

		//seg000:1481                loc_11481:
		word_390D4 = -1;
		sMission_unk0* Data20 = stru_390FA;
		uint16* Data24 = word_390D6;
		for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {
			*Data24++ = Data20->field_4;
		}

	}
	//seg000:14C6                loc_114C6:
	int16 Data1C = word_397D2 - 1;
	while (Data1C >= 0) {
		sub_1152F();
		--Data1C;
	}

	if (word_3ABA7)
		return;

	Data1C = mTroopsAvailable;
	sMission_unk0* Data20 = stru_390FA;
	for (int16 Data0 = 7; Data0 >= 0; --Data0, ++Data20) {

		if (Data20->field_4 == -1)
			continue;

		if (Data1C)
			--Data1C;
		else
			Data20->field_4 = -1;
	}

}

void cFodder::sub_1152F() {
	sMission_unk0* Data20 = stru_390FA;

	for (int16 Data0 = 7; Data0 >= 0; --Data0) {

		if (Data20->field_0 == -1) {

			if (word_390F4 >= 360)
				return;

			Data20->field_0 = word_390F4;

			struct_Troops* Data24 = stru_36712;
			Data24 += word_390F4;

			Data20->mRank =  (mMissionNumber - 1) / 3;
			Data20->field_8 = 0;
			Data20->field_6 = 3;
			++word_390F4;

			return;
		}

		++Data20;
	}
}

void cFodder::sub_115F7() {

	word_3A016 = mTroopsAvailable;
	int16* Data20 = mMapSpt_Loaded;
	sMission_unk0* Data34 = stru_390FA;

	for (int16 Data18 = 0x1D; Data18 >= 0; --Data18, Data20 += 0x3B ) {

		if (*Data20 == -32768)
			continue;

		if (Data20[0x0C])
			continue;

		--word_3A016;
		if (word_3A016 < 0) {
			Data34->field_4 = -1;
			Data20[0] = -32768;
			Data20[0x0C] = 4;
			Data20[0x08] = 0x7C;
			++Data34;
		}
		else {
			// loc_1166B
			*((uint32*)&Data20[0x23]) = (uint32)Data34;

			int16 Data24 = (int16)(Data20 - &mMapSpt_Loaded[0]);

			Data34->field_4 = Data24;
			Data20[0x08] = 0x40;
			Data20[0x11] = 0;

			++Data34;
		}
	}
}

void cFodder::sub_12AB1() {
	
	word_3E75B = -1;
}

void cFodder::Load_PlayerBin() {
	
}

bool cFodder::EventAdd( cEvent pEvent ) {
	
	mEvents.push_back( pEvent );

	return true;
}

void cFodder::Load_File( const std::string& pFilename ) {
	size_t FileSize = 0;

	uint8* Buffer = local_FileRead( pFilename, "", FileSize );

	word_3E7ED = readLEWord( Buffer );
	mEffectDriver = readLEWord( Buffer + 2 );
	mMusicDriver = readLEWord( Buffer + 4 );

	delete Buffer;
}

void cFodder::memory_XMS_Detect() {

	word_427D4 = 1;
	mErrorCode = 0;
}

void cFodder::eventProcess() {
	g_Window.EventCheck();

	for (std::vector<cEvent>::iterator EventIT = mEvents.begin(); EventIT != mEvents.end(); ++EventIT) {

		switch (EventIT->mType) {
			case eEvent_MouseLeftDown:
				mMousePosition = EventIT->mPosition;
				mMouseButtons |= 1;
				break;

			case eEvent_MouseRightDown:
				mMousePosition = EventIT->mPosition;
				mMouseButtons |= 2;
				break;

			case eEvent_MouseLeftUp:
				mMousePosition = EventIT->mPosition;
				mMouseButtons &= ~1;
				break;

			case eEvent_MouseRightUp:
				mMousePosition = EventIT->mPosition;
				mMouseButtons &= ~2;
				break;

			case eEvent_MouseMove:
				mMousePosition = EventIT->mPosition;
				break;

			case eEvent_Quit:

				break;
		}

	}

	mEvents.clear();
}

void cFodder::mouse_Setup() {
	
	word_3BDAF = 0x7F;
	word_3BDB1 = 0x67;
	word_3BDB3 = 0;
	word_3BDB5 = 0;

	mouseData1->anonymous_0 = 0;
	mouseData1->anonymous_1 = 0;
	mouseData1->anonymous_2 = 2;
	mouseData1->mColumn = 160;
	mouseData1->mRow = 100;
	mouseData1->anonymous_5 = 0x10;
	mouseData1->anonymous_6 = 0x10;
	mouseData1->anonymous_7 = 0x10;
	mouseData1->anonymous_8 = 0x10;
	mouseData1->anonymous_9 = 0x30;
	mouseData1->anonymous_10 = 0xC8;

	mouseData0->anonymous_0 = 0;
	mouseData0->anonymous_1 = 0;
	mouseData0->anonymous_2 = 2;
	mouseData0->mColumn = 160;
	mouseData0->mRow = 100;
	mouseData0->anonymous_5 = 0x10;
	mouseData0->anonymous_6 = 0x10;
	mouseData0->anonymous_7 = 0x10;
	mouseData0->anonymous_8 = 0x10;
	mouseData0->anonymous_9 = 0x30;
	mouseData0->anonymous_10 = 0xC8;

	g_Window.SetMousePosition( cPosition( 160, 100 ) );
}

void cFodder::mouse_GetData() {

	mouse_Pos_Column = (int16) mMousePosition.mX;
	mouse_Pos_Row = (int16) mMousePosition.mY;
	mouse_Button_Status = mMouseButtons;
}

void cFodder::mouse_Handle() {
	
	mouse_GetData();
	mouse_ButtonCheck();

	//g_Window.SetMousePosition( cPosition( 160, 100 ) );

	int16 dword_37AA4 = mouse_Pos_Column - 0x70;
	word_3BDB3 = dword_37AA4;

	dword_37AA4 += word_3BDAF;

	if ((word_3A024 == 0 && dword_37AA4 < -32 ) || dword_37AA4 < -16) {
		//loc_13B41
		if (word_3A024)
			dword_37AA4 = -16;
		else
			dword_37AA4 = -32;
	} else {
		//loc_13B58
		if (dword_37AA4 > 287)
			dword_37AA4 = 287;
	}

	//loc_13B66
	word_3BDAF = dword_37AA4;
	dword_37AA0 = mouse_Pos_Row - 88;

	word_3BDB5 = dword_37AA0;
	dword_37AA0 += word_3BDB1;

	if (dword_37AA0 < 4)
		dword_37AA0 = 4;
	else {

		if (dword_37AA0 > 203)
			dword_37AA0 = 203;
	}

	word_3BDB1 = dword_37AA0;
}

void cFodder::mouse_ButtonCheck() {

	mButtonPressLeft = 0;
	if (mouse_Button_Status & 1) {
		mButtonPressLeft -= 1;
		if (word_39EF8 == 0) {
			word_39EF8 = -1;
			word_39F02 = -1;

			if (word_39EF6) {
				word_39EFC = -1;
				word_3AA43 = -1;
			}
		}

	} else {
		word_3AA43 = 0;
		word_39EF8 = 0;
	}

	mButtonPressRight = 0;
	if (mouse_Button_Status & 2) {
		mButtonPressRight -= 1;
		if (word_39EFA == 0) {
			word_39F04 = -1;
			word_39EFA = -1;
		}
	} else {
		word_39EFA = 0;
	}

}

void cFodder::Prepare() {

	mWindow->InitWindow( "Open Fodder" );

	Load_File("setup.dat");
	sub_12AB1();
	//Resource_Data_Open();
	//Load_EffectDriver();
	//Load_MusicDriver();
	memory_XMS_Detect();
}

void cFodder::sub_13800() {

	//videoSleep();
	//videoSleep();
}

void cFodder::paletteLoad( uint8* pBuffer, uint16 pColors, uint16 pColorID ) {
		size_t colorStartID = pColorID;

	if( pColors >= g_MaxColors )
		pColors = g_MaxColors-1;

	for(; pColorID < pColors + colorStartID; pColorID++) {
		
		// Get the next color values
		mPalette[ pColorID ].mRed =		*pBuffer++;
		mPalette[ pColorID ].mGreen =	*pBuffer++;
		mPalette[ pColorID ].mBlue =	*pBuffer++;
	}
}

void cFodder::sub_13C1C( cSurface* pImage, int32 pParam00, int32 pParam0C, int32 pParam04, int32 pParam08 ) {

	uint16 bx = mSpriteDataPtr[pParam00][pParam04].field_0;

	word_42062 = GetSpriteData( mSpriteDataPtr[pParam00][pParam04].field_2 ) + bx;

	word_42068 = (pParam08 + 0x10) - 16;
	word_4206A = pParam0C + 0x10;
	word_4206C = mSpriteDataPtr[pParam00][pParam04].mColCount;
	word_4206E = mSpriteDataPtr[pParam00][pParam04].mRowCount;
	if (word_3B307 > pParam00) 
		word_3B307 = pParam00;
	
	byte_42070 = (uint8) mSpriteDataPtr[pParam00][pParam04].field_C;
	
	if (sub_1429B() )
		video_Draw_Sprite_( pImage );

}

void cFodder::sub_13C8A( cSurface* pImage, int16 pData0, int16 pData4, int16 pPosX, int16 pPosY ) {
	uint16 bx = mSpriteDataPtr[pData0][pData4].field_0;

	word_42062 = GetSpriteData( mSpriteDataPtr[pData0][pData4].field_2 ) + bx;
	
	word_42068 = (pPosX + 0x10) - 16;
	word_4206A = pPosY + 0x10;
	word_4206C = mSpriteDataPtr[pData0][pData4].mColCount;
	word_4206E = mSpriteDataPtr[pData0][pData4].mRowCount;
	byte_42070 = mSpriteDataPtr[pData0][pData4].field_C;
	word_42078 = 0x140;

	if (sub_1429B())
		video_Draw_Linear_To_Planar( pImage );
}

void cFodder::video_Draw_Sprite_( cSurface* pImage ) {
	uint8*	di = pImage->GetSurfaceBuffer();
	uint8* 	si = word_42062;
	int16	ax, cx;
	
	di += 320 * word_4206A;

	ax = word_42068;
	ax += word_40054;
	//ax >>= 2;
	
	di += ax;
	word_42066 = di;
	cx = word_42068;
	cx += word_40054;
	cx &= 3;

	uint8 Plane = 0;

	byte_42071 = 1 << cx;
	byte bl = byte_42070;
	
	word_4206C >>= 1;
	word_42074 = 0xA0 - word_4206C;
	word_4206C >>= 1;
	
	word_42076 = 320 - (word_4206C*4);
	
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = 0; cx < word_4206C; ++cx ) {
			byte al = (*si) >> 4;
			if(al)
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}

	si = word_42062;
	di = word_42066;
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = word_4206C; cx > 0; --cx ) {
			byte al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}
	
	++word_42062;
	si = word_42062;
	di = word_42066;
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = word_4206C; cx > 0; --cx ) {
			
			byte al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			di+=4;
			
		}
		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}

	si = word_42062;
	di = word_42066;
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = word_4206C; cx > 0; --cx ) {
			
			byte al = (*si) & 0x0F;
			if( al ) 
				*di = al | bl;
			
			si += 2;
			di+=4;
		}
		
		si += word_42074;
		di += word_42076;
	}

	pImage->draw();
}

void cFodder::video_Draw_Linear_To_Planar( cSurface* pImage ) {
	uint8*	di = pImage->GetSurfaceBuffer();
	uint8* 	si = word_42062;
	int16	ax, cx;
	
	di += 320 * word_4206A;

	ax = word_42068;
	ax += word_40054;
	//ax >>= 2;
	
	di += ax;
	word_42066 = di;
	cx = word_42068;
	cx += word_40054;
	cx &= 3;

	uint8 Plane = 0;

	byte_42071 = 1 << cx;
	word_42074 = word_42078 - word_4206C;
		                 
	//word_4206C >>= 1;
	word_42076 = 320 - word_4206C;

	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = word_4206C; cx > 0; --cx ) {
			byte al = *si;
			if(al)
				*di = al;
			
			si ++;
			di ++;
		}

		si += word_42074;
		di += word_42076;
	}

	pImage->draw();
	return;
	/*
	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}
	
	++word_42062;
	si = word_42062;
	di = word_42066;
	
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = 0; cx < word_4206C; ++cx ) {
			byte al = *si;
			if(al)
				*di = al;
			
			si += 3;
			di ++;
		}
		
		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}
	
	++word_42062;
	si = word_42062;
	di = word_42066;
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = 0; cx < word_4206C; ++cx ) {
			byte al = *si;
			if(al)
				*di = al;
			
			si += 3;
			di ++;
		}
		
		si += word_42074;
		di += word_42076;
	}

	++Plane;
	if (Plane == 4) {
		Plane = 0;
		++word_42066;
	}
	
	++word_42062;
	si = word_42062;
	di = word_42066;
	di += Plane;
	for( int16 dx = word_4206E; dx > 0; --dx ) {
		
		for( cx = 0; cx < word_4206C; ++cx ) {
			byte al = *si;
			if(al)
				*di = al;
			
			si += 3;
			di ++;
		}
		
		si += word_42074;
		di += word_42076;
	}
	
	pImage->draw();*/
}

bool cFodder::sub_1429B() {
	int16 ax;
	
	if( word_4206A < 0 ) {
		ax = word_4206A + word_4206E;
		--ax;
		if( ax < 0 )
			return false;
		
		ax -= 0;
		ax -= word_4206E;
		++ax;
		ax = -ax;
		word_4206A += ax;
		word_4206E -= ax;
		ax *= 0xA0;
		word_42062 += ax;
	}
	
	ax = word_4206A + word_4206E;
	--ax;
	if( ax > 0xE7 ) {
		if( word_4206A > 0xE7 )
			return false;
		
		ax -= 0xE7;
		word_4206E -= ax;
		
	}

	if( word_42068 < 0 ) {
		ax = word_42068 + word_4206C;
		--ax;
		if( ax < 0 )
			return false;
		
		ax -= 0;
		ax -= word_4206C;
		++ax;
		ax = -ax;
		--ax;
		
		while( ax & 3 )
			++ax;
		
		word_42068 += ax;
		word_4206C -= ax;
		ax >>= 1;
		word_42062 += ax;
	}

	ax = word_42068 + word_4206C;
	--ax;
	
	if( ax > 0x15F ) {
		if( word_42068 >= 0x15F )
			return false;
		
		ax -= 0x15F;
		--ax;
		
		while( ax & 3 )
			++ax;
		
		word_4206C -= ax;
	}

	if( word_4206C <= 0 )
		return false;
	
	if( word_4206E <= 0 )
		return false;
	
	return true;
}

void cFodder::sub_144A2( cSurface* pImage ) {
	
	uint8*	Buffer = pImage->GetSurfaceBuffer();
	uint8* 	si = (uint8*) mMapSptPtr;

	Buffer += 16 * 320; // 0x584
	byte_42071 = 1 << word_40054;

	word_42066 = Buffer;
	for (unsigned int Plane = 0; Plane < 4; Plane++) {

		Buffer = word_42066;

		for (unsigned int Y = 0; Y < 200; ++Y) {

			for (unsigned int X = Plane; X < 0x30; X += 4) {

				Buffer[X] = *si++;
			}
			
			Buffer += 320;
		}
	}

	pImage->draw();
}

void cFodder::sub_145AF( int16 pData0, int16 pData8, int16 pDataC ) {
	const struct_2* str2 = &stru_44B50[pData0];
	
	int16 cx = str2->field_4;
	int16 dx = str2->field_6;
	word_4206C = cx;
	word_4206E = dx;
	
	uint16 ax = 0xA0 * str2->field_2;
	uint16 bx = str2->field_0 >> 1;
	ax += bx;
	
	word_42062 = mDataPStuff + ax;
	
	byte_42070 = 0xF0;
	
	uint16 w42066 = 0x0C * pDataC;
	w42066 += pData8 >> 2;
	
	ax = pData8 & 3;
	ax *= 0x960;
	w42066 += ax;
	
	uint8* di = ((uint8*)mMapSptPtr) + w42066;
	uint8* si = word_42062;
	
	byte bl = byte_42070;
	word_4206C >>= 1;
	
	word_42074 = 0xA0 - word_4206C;
	word_4206C >>= 1;
	word_42076 = 0x0C - word_4206C;
	
	for( uint16 dx = word_4206E; dx > 0; --dx ) {
		
		for( uint16 cx = word_4206C; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
			
		si += word_42074;
		di += word_42076;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	si = word_42062;
	di =  ((uint8*)mMapSptPtr) + w42066;
	
	for( uint16 dx = word_4206E; dx > 0; --dx ) {
		
		for( uint16 cx = word_4206C; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
		
		si += word_42074;
		di += word_42076;
	}
		
	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;
	
	++word_42062;
	si = word_42062;
	di =  ((uint8*)mMapSptPtr) + w42066;
	
	for( uint16 dx = word_4206E; dx > 0; --dx ) {
		
		for( uint16 cx = word_4206C; cx > 0; --cx ) {
			
			uint8 al = (*si) >> 4;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
			
		si += word_42074;
		di += word_42076;
	}

	w42066 += 0x960;
	if( w42066 >= 0x2580 )
		w42066 -= 0x257F;

	si = word_42062;
	di =  ((uint8*)mMapSptPtr) + w42066;
	
	for( uint16 dx = word_4206E; dx > 0; --dx ) {
		
		for( uint16 cx = word_4206C; cx > 0; --cx ) {
			uint8 al = (*si) & 0x0F;
			if( al )
				*di = al | bl;
			
			si += 2;
			++di;
		}
		
		si += word_42074;
		di += word_42076;
	}

}

void cFodder::Recruit_Show() {
	
	mouse_Setup();
	map_ClearSpt();
	
	cSurface* ImageHill = new cSurface(320,230);
	
	delete word_3E1B7;
	word_3E1B7 = g_Resource.fileGet( "hill.dat", word_3E1B7_size );
	paletteLoad( word_3E1B7 + 0xFA00, 0x50, 0x00 );

	delete mDataHillBits;
	mDataHillBits = g_Resource.fileGet( "hillbits.dat", mDataHillBitsSize );
	paletteLoad( mDataHillBits + 0x6900, 0x10, 0xB0 );

	dword_3B1FB = stru_373BA;
	word_3AAD1 = -1;
	word_3AB39 = -1;
	
	Sprite_SetDataPtrToBase( mHillBitsSpriteSheetPtr );

	sub_16BC3();
	sub_16C6C();
	Recruit_Render_LeftMenu( ImageHill );
	Recruit_Draw_Hill( ImageHill );
	
	sub_17B64();
	
	Sprite_SetDataPtrToBase( off_35E42 );
	
	sub_17CD3( ImageHill );

	ImageHill->Save();
	ImageHill->paletteSet( mPalette );

	word_3BEC1 = 0;
	word_3BEC3 = 0x1D;
	//word_39020 = 0;
	
	sub_17368();
	
	dword_3AAC9 = mDataSubBlk;
	
	for (int16 ax = word_397D4 - 1; ax >= 0;--ax )
		sub_17429();
	
	word_3AA51 = -1;
	word_3A016 = 0;
	word_3AA67 = 0;
	word_3AA71 = word_397D2;
	
	sub_1787C();

	Recruit_Draw_Actors( ImageHill );
	Recruit_Draw_Actors( ImageHill );
	
	word_39F02 = 0;
	
	for( ;; ) {
		if( word_39F02 ) {
			word_39F02 = 0;
			
			if( sub_18C7A() )
				break;
		}

		Recruit_Draw( ImageHill );
	}
	
	word_3A9F7 = 0x23;
	word_3AA51 = 0;
	
	ImageHill->paletteFadeOut();
	
	while( ImageHill->GetFaded() == false ) {
		Recruit_Draw( ImageHill );
	}
	
	delete ImageHill;
}

void cFodder::Recruit_Draw_Hill( cSurface* pImage ) {
	word_42062 = word_3E1B7 + 0xA00;
	
	word_42068 = 0x30;
	word_4206A = 0x28;
	word_4206C = 0x110;
	word_4206E = 0xB0;
	word_42078 = 0x140;
	
	video_Draw_Linear_To_Planar( pImage );
	
	for( uint32 x = 0; x < 0xA000; ++x) {
		word_3E1B7[x] = 0;
	}
}

void cFodder::Recruit_Draw_HomeAway( cSurface* pImage ) {
	const char* strHomeAndAway = "HOME                AWAY";
	
	Sprite_SetDataPtrToBase( mHillBitsSpriteSheetPtr );
	
	sub_13C1C( pImage, 0x18, 0, 0, 0 );
	
	int16 Data4 = word_3E0E5[ (mMissionNumber - 1) ];

	sub_13C1C( pImage, 0x19, 0, Data4, 0x130 );
	
	String_CalculateWidth( 320, mUnkStringModifier_Recruit, strHomeAndAway );
	String_Print( pImage, mUnkStringModifier_Recruit, 0x0D, word_3B301, 0x0A, strHomeAndAway );
	
	sub_13C1C( pImage, 0x0E, 0x0A, 0, 0x9B );
	
	std::string Home = tool_StripLeadingZero(tool_NumToString( word_397AE ));
	sub_16B55( pImage, 0x0D, 0x9A - (Home.length() * 0x0C), 0x0A, Home );

	std::string Away = tool_StripLeadingZero(tool_NumToString( word_397AC ));
	sub_16B55( pImage, 0x0D, 0xAA, 0x0A, Away );

	Sprite_SetDataPtrToBase( off_35E42 );
}

void cFodder::sub_16B55( cSurface* pImage, int16 pParam0, int16 pParam8, int16 pParamC, const std::string& pString ) {

	for ( std::string::const_iterator Text = pString.begin(); Text != pString.end(); ++Text, pParam8 += 0x0C) {
		char NextChar = *Text;

		if (NextChar == 0x20)
			continue;

		if (NextChar <= 0x39) {
			NextChar -= 0x30;
			NextChar += 0x1A;
		}
		else
			NextChar -= 0x41;

		sub_13C1C( pImage, pParam0, pParamC, NextChar, pParam8 );
	}
}

void cFodder::sub_16BC3() {
	int16* di = word_3BDAD;	// 20h
	int16  ax = word_397D2;
	int16  count;
	
	if( ax > 0 ) {
		
		sub_16C45( &di, word_3E115 );
		count = word_397D2;
		count -= 2;

		while( count >= 0 ) {
			sub_16C45( &di, word_3E135 );
			--count;
		}
		
		sub_16C45( &di, word_3E12B );
		sub_16C45( &di, word_3E15D );
	}

	*di++ = 0;
	*di = -1;
}

void cFodder::sub_16C45( int16** pDi, int16* pSource ) {
	int16 ax;
	
	for(;;) {
		ax = *pSource++;
		
		if( ax < 0 )
			break;
		
		**pDi = ax;
		(*pDi)++;
	}	
}

void cFodder::sub_16C6C() {
	int16 *si = word_391D2;
	
	while( *si != -1 ) {
		uint8* si1 = (uint8*) si;
		
		*(si1+1) += 1;
		++si;
	}
}

void cFodder::sub_17C91( cSurface *pImage, int16 pData0, int16 pData8, int16 pDataC ) {
	pData0 >>= 1;
	if (pData0 > 8)
		pData0 = 0;

	int16 Data4 = pDataC;
	Data4 -= 0x14;
	Data4 >>= 5;
	if (Data4 >= 5)
		Data4 = 4;

	pDataC -= 8;
	sub_13C1C( pImage, pData0, pDataC, Data4, pData8 );
}

void cFodder::sub_17CD3( cSurface* pImage ) {
	int16 Data0 = 1440;
	int32 Data1C = -1;
	int16* Data24 = word_391D2;

	for(; *Data24 >= 0 ; ++Data24) {
		Data0 -= 4;
		Data1C++;
	}

	if (Data24 == word_391D2)
		return;

	--Data24;
	int16* Data20 = word_3E1B9 + (Data0 / 2);

	do {
		Data0 = *Data24;
		if (Data0 < 0)
			return;

		--Data24;
		int16 Data8 = *Data20++;
		int16 DataC = *Data20++;

		sub_17C91( pImage, Data0, Data8, DataC );

		--Data1C;
	} while (Data1C >= 0);

}

void cFodder::Recruit_Render_LeftMenu( cSurface *pImage ) {
	int16 Data0 = 0xAD;
	int16 Data8 = 0;
	int16 DataC	= 0x18;
	
	// Draw Heroes Heading
	sub_145AF( Data0, Data8, DataC );
	
	struct_1* Data20 = stru_3978E;
	int16 Data14 = 0x0E;
	Data0 = 4;
	
	for( struct_1* Data20 = stru_3978E; Data0 >= 0; --Data0, ++Data20  ) {
		if( Data20->field_0 == -1 )
			break;
		
		Data14 += 0x0C;
	}

	// Draw Empty Hero Slots
	DataC = 0x0E;
	do {
		
		Data0 = 0xA9;
		if( DataC >= Data14 )
			Data0 = 0xAB;
		
		Data8 = 0;
		sub_145AF( Data0, Data8, DataC + 0x18 );
		DataC += 0x0C;
		
	} while( DataC < 0x4A );

	//seg003:1E89

	// Draw Squad Heading
	Data0 = 0xAE;
	Data8 = 0;
	DataC = 0x4A + 0x18;
	sub_145AF( Data0, Data8, DataC );
	
	Data14 = word_397D2 + word_397D4;
	Data14 /= 0x0C;
	DataC = 0x58;
	Data14 += DataC;

	// Draw Used Slot into Temporary Area
	do {
		Data0 = 0xA9;
		if( DataC >= Data14 )
			Data0 = 0xAB;

		Data8 = 0;
		sub_145AF( Data0, Data8, DataC + 0x18 );
		DataC += 0x0C;
		
	} while( DataC < 0xA0 );

	word_3BEC9 = 0xB8;
	word_3AA55 = 0x0F;
	word_3AAC7 = -1;

	Recruit_Render_Squad_RankKills();
	Recruit_Render_Squad_Names();
	
	word_3AAC7 = 0;
	word_3AA55 = -59;
	
	Recruit_Render_HeroList();
	word_3AA55 = 0;
}

void cFodder::Recruit_Render_Squad_Names() {

	word_3A3BB = 7;
	word_3A3BD = 0;
	
	for( uint16 x = 0; x < 8; ++x, --word_3A3BB ) {
		sMission_unk0* Data2C = &stru_390FA[x];
		
		int16 Data0 = Data2C->field_4;
		
		if( Data0 == -1 )
			continue;
		
		int16* Data20 = mMapSpt_Loaded;

		int16* si = Data20 + Data0;
		if( word_39FD0 != si[0x19])
			continue;

		struct_Troops* Data28 = &stru_36712[Data2C->field_0];
		int16 Data14;

		for( Data14 = 0; Data14 <= 5; ++Data14 ) {
			
			if( Data28->field_0[Data14] == 0x20 )
				break;
			
		} 
		
		Data0 = Data14;
		Data14 <<= 2;
		int16 word_3A05F = (0x30 - Data14) >> 1;

		if( word_3AAC7 )
			word_3A05F -= 1;
		
		// Draw Troop name to list
		for( Data14 = 0; Data14 <= 5; ++Data14 ) {
			
			if( Data28->field_0[Data14] != 0x20 ) {
				Data0 = Data28->field_0[Data14];
				Data0 -= 0x41;
				Data0 += 0x29;
				
				int16 Data8 = Data14;
				Data8 <<= 2;
				Data8 += word_3A05F;
				int16 DataC = 0x4B;
				DataC += word_3A3BD;
				DataC += word_3AA55;

				sub_145AF( Data0, Data8, DataC + 0x19 );
			}
		}

		word_3A3BD += 0x0C;
	}

}

void cFodder::Recruit_Render_Squad_RankKills() {
	int16 Data0;
	int16* Data34 = mMapSpt_Loaded;

	int16 word_3A061 = 7;
	word_3A3BD = 0;
	
	int16 Data4;
	
	for( uint16 x = 0; x < 8; ++x, --word_3A061 ) {
		sMission_unk0* Data38 = &stru_390FA[x];

		if( Data38->field_4 == -1 )
			continue;
		
		Data4 = word_39FD0;
		int16* si = Data34 + Data38->field_4;
		
		if( Data4 != si[0x19]  )
			continue;
		
		int16 Data8 = 0;
		int16 DataC = word_3A3BD;
		DataC += 0x4A;
		DataC += word_3AA55;
		
		if( !word_3AAC7 ) {
			Data0 = 0;
			Data0 = Data38->mRank;
			Data0 += 9;
			Data8 = 0x23;
			DataC = word_3A3BD;
			DataC -= 1;
			DataC += 0x4A;
			DataC += word_3AA55;
			DataC += 0x19;
			
			sub_145AF( Data0, Data8, DataC );
			
			DataC -= 0x19;
			
		} else {
			// Draw Kills
			sub_170A4( Data38->mNumberOfKills, 0x43 );
			
		}
		
		//Draw Rank
		Data0 = Data38->mRank + 9;
		DataC = word_3A3BD - 1;

		DataC += 0x4A;
		DataC += word_3AA55;
		DataC += 0x19;
		sub_145AF( Data0, 0, DataC );
		DataC -= 0x19;
		word_3A3BD += 0x0C;
	}
}

void cFodder::sub_170A4( int16 pData4, int16 pData10 ) {
	
	pData10 -= 0x30;
	std::string Data20 = tool_StripLeadingZero(tool_NumToString( pData4 ));
	uint16 Data0 = (uint16) Data20.length() * 4;

	int16 Data8 = 0x30 - Data0;
	int16 DataC = 0x4B + word_3A3BD + word_3AA55;

	for (std::string::iterator CharIT = Data20.begin(); CharIT != Data20.end(); ++CharIT) {

		Data0 = (*CharIT & 0xFF) + pData10;
		sub_145AF( Data0, Data8, DataC + 0x19 );
		Data8 += 4;
	}
}

void cFodder::Recruit_Render_HeroList() {
	const struct_1* Data2C = stru_3978E;

	word_3A3BB = 4;
	word_3A3BD = 0;

	for (word_3A3BB = 4; word_3A3BB >= 0; --word_3A3BB, ++Data2C) {

		if (Data2C->field_1 < 0)
			continue;

		int16 Data0 = Data2C->field_0 + 9;
		int16 Data8 = 0;
		int16 DataC = word_3A3BD - 1;
		DataC += 0x4A + word_3AA55+ 0x19;

		sub_145AF( Data0, Data8, DataC );

		struct_Troops* Data28 = &stru_36712[ Data2C->field_1 ];

		int16 Data14;
		for( Data14 = 0; Data14 <= 5; ++Data14 ) {
			
			if( Data28->field_0[Data14] == 0x20 )
				break;
		} 

		Data0 = Data14;
		Data14 <<= 2;
		word_3A05F = (0x30 - Data14) >> 1;
		--word_3A05F;

		for (Data14 = 0; Data14 <= 5; ++Data14) {
			Data0 = 0;

			uint8 Character = Data28->field_0[Data14];
			if (Character == 0x20)
				continue;

			Character -= 0x41;
			Character += 0x29;
			Data8 = Data14;
			Data8 <<= 2;
			Data8 += word_3A05F;
			DataC += 0x4B;
			DataC += word_3A3BD + word_3AA55;

			sub_145AF( Character, Data8, DataC  + 0x19 );
		}

		sub_170A4( Data2C->field_3, 0x67 );
		word_3A3BD += 0x0C;
	}
}

void cFodder::sub_17368() {
	uint32* Data20 = (uint32*) mDataArmy;

	uint16* Data24 = (uint16*) mDataSubBlk;

	for ( int16 Data0 = 0x58; Data0 < 0xA0; Data0 += 0x0C) {
		*((uint32*)Data24) = (uint32) Data20;
		Data24 += 2;

		*Data24++ = Data0;

		sub_17480( Data0, 0x0C, 0, Data20 );
	}

	//seg003:2532
	*Data24 = -1;
	int16 DataC = 0x58;

	for (int16 Data4 = 5; Data4 >= 0; --Data4) {
		sub_145AF( 0xAC, 0, DataC + 0x18 );

		DataC += 0x0C;
	}
}

void cFodder::sub_17429() {
	uint8* Data24 = dword_3AAC9;

	if (*((uint32*)Data24) < 0)
		return;

	uint8* Data20 = (uint8*) *((uint32*)Data24);
	Data24 += 4;

	uint16 Data0 = *((uint16*)Data24);
	Data24 += 2;

	dword_3AAC9 = Data24;
	uint32* Dataa20 =  (uint32*) Data20;
	sub_17480( Data0, 0x0C, -1, Dataa20 );
}

void cFodder::sub_17480( uint16 pData0, int16 pData4, int16 pData8, uint32*& pData20 ) {
	pData0 += 0x18;

	uint8* SptPtr = (uint8*)mMapSptPtr;

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

void cFodder::Recruit_Draw_Actors( cSurface* pImage ) {
	sMouseData* bp = mouseData1 + 1;

	word_42072 = 2;

	Recruit_Draw_Truck( pImage );
	Recruit_Draw_Troops( pImage );

	bp = mouseData1;
	bp->anonymous_0 = 0;
	bp->anonymous_2 = word_42072;
}

void cFodder::sub_175C0() {
	struct_4* Data20 = 0;

	if (word_3B1F1 > 0) {

		struct_4* Data20 = &stru_373BA[293];
		do {
			if (Data20 == &stru_373BA[0])
				return;

			--Data20;

		} while (Data20->field_4 == 0 || Data20->field_4 < 4);

		int16 Data0 = Data20->field_4;
		Data20->field_4 = 0;
		Data0 ^= 1;

		if ((Data20 + 1)->field_0 >= 0) {
			(Data20 + 1)->field_4 = Data0;
		} else {
			--word_3B1F1;
			sub_17429();
		}
	}
	//loc_17652

	uint16 aa = ~(dword_3B1CB >> 16);

	dword_3B1CB = (aa << 16) | (dword_3B1CB & 0xFFFF);

	Data20 = &stru_373BA[293];
	struct_4* Data28 = &stru_373BA[271];

loc_17686:;
	for (; Data20 != &stru_373BA[0];) {
		--Data20;

		if (Data20->field_4 == 0)
			continue;

		if (Data20->field_4 < 4)
			continue;

		if (Data20 >= Data28)
			continue;

		int16 Data8 = 3;
		struct_4* Data30 = Data20;
		//loc_176CE

		do {
			++Data30;
			if (Data30->field_4) {
				if (Data30->field_4 > 3)
					goto loc_17686;

				++Data30;
			}
			--Data8;
		} while (Data8 >= 0);

		int16 Data0 = Data20->field_4;
		Data20->field_4 = 0;
		if ((dword_3B1CB >> 16) < 0)
			Data0 ^= 1;

		if ((Data20 + 1)->field_4) {
			(Data20 + 2)->field_4 = Data0;
		}
		else {
			(Data20 + 1)->field_4 = Data0;
		}
	}

}

void cFodder::Recruit_Draw_Troops( cSurface *pImage ) {

	if (word_3B1EF) {
		sub_175C0();
	}
	else {
		sub_178DD();
		sub_17911();
	}

	struct_4* Data20 = stru_373BA;
	struct_4* dword_3B1C7;

	for ( ; ;Data20 = dword_3B1C7) {

		dword_3B1C7 = Data20;

		if (Data20->field_0 < 0)
			break;

		int16 Data0;
		int16 Data8 = Data20->field_0;
		int16 DataC = Data20->field_2;
		int16 Data4 = Data20->field_4;
		++Data20;
		dword_3B1C7 = Data20;
		if (Data4 == 0)
			continue;

		--Data4;
		if (Data4 >= 3) {
			//loc_177E3
			Data4 -= 3;
			Data0 = 0x0A;
			Data0 += Data4;

			if (DataC >= 0xB7)
				Data0 += 4;
			else if (DataC > 0x7C )
				Data0 += 2;

			Data4 = 0;

			if (Data20 > &stru_373BA[0x46]) {
				Data4 = 1;

				if (Data20 > &stru_373BA[0x7A])
					Data4 = 2;
			}
		}
		else
			Data0 = 9;

		//loc_1784C
		DataC -= 8;
		if (Data0 != 9)
			sub_13C1C( pImage, Data0, DataC, Data4, Data8 );
		else {
			++Data8;
			--DataC;
			sub_13C8A( pImage, Data0, Data4, Data8, DataC );
		}
	}
}

void cFodder::sub_1787C() {
	
	if (word_390B8)
		return;
	
	word_3B1F1 = word_397D2;
	word_3B1EF = 0;

	int16 Data0 = 0xB4;
	int16* Data20 = word_3B1CF;
	const int16* Data24 = word_3E177;

	for ( int16 Data4 = 0x0E; Data4 >= 0; --Data4 ) {

		*Data20++ = Data0;
		Data0 -= *Data24++;

	}
}

void cFodder::sub_178DD() {
	struct_4* Data20 = stru_373BA;

	for (; Data20->field_0 >= 0; ++Data20) {
		
		if (Data20->field_4 >= 4)
			Data20->field_4 = 0;
	}
}

void cFodder::sub_17911() {
	struct_4* Data24 = stru_373BA;
	int16*    Data2C = word_3E197;

	uint32 Data8 = (uint32) dword_394A4;
	int16 Data0;

	Data8 -= (uint32) word_391D2;
	Data8 >>= 1;
	Data8 &= 0x1E;

	if (word_390B8)
		Data0 = word_390CE;
	else {
		Data0 = word_390E8;
		if (!Data0)
			goto loc_179B2;
	}

	// loc_1795E
	--Data0;
	int16 Data4 = 0x660;

	//loc_17968
	for (; Data0 >= 0; --Data0) {

		if (Data4 >= 0) {
			if ((Data24 + (Data4/6))->field_4)
				Data4 -= 6;

			(Data24 + (Data4/6))->field_4 = *(Data2C + (Data8/2));
		}

		Data8 += 2;
		Data8 &= 0x1F;
		Data4 -= 0x18;
	}

loc_179B2:;
	if (word_390B8)
		return;

	word_3B1ED = 0;
	Data0 = word_3B1CF[0];
	Data8 = 5;

	for ( ; ; Data0+=6 ) {

		Data4 = (Data24 + (Data0/6))->field_4;
		if (Data4) {
			if (Data4 > 3) {
				word_3B1ED = -1;
				break;
			}
			else
				continue;
		}
		else {
			if (Data0 == 0x660) {
				word_3B1ED = -1;
				break;
			}
			--Data8;
			if (!Data8)
				break;
		}
	}
	//seg003:2B6D
	if (!word_3B1ED) {
		dword_3B1CB += 0x8000;
		dword_3B1CB &= 0x1FFFF;
	}
	else
		dword_3B1CB = 0x10000;

	int16* Data20 = word_3B1CF;

	for (int16 DData8 = 0x0E; DData8 >= 0; --DData8) {

		Data0 = *Data20;
		if (Data0 >= 0) {
			Data4 = (dword_3B1CB >> 16);

			Data4 += *Data2C++;
			if ((Data24 + (Data0/6))->field_0 < 0)
				Data0 = 0;

			if ((Data24 + (Data0/6))->field_4)
				Data0 += 6;

			(Data24 + (Data0/6))->field_4 = Data4;
		}
		// 17A9D

		if (!word_3B1ED)
			Data0 += 6;

		//loc_17AA9
		*Data20 = Data0;
		++Data20;
	}

	if (word_3B1ED)
		word_3B1EF = -1;
}

void cFodder::Recruit_Draw_Truck( cSurface* pImage ) {
	int16 Data0 = 0x22;
	int16 Data4 = word_3AACD;
	int16 Data8 = 0x31;
	int16 DataC = 0xB6;

	sub_13C1C( pImage, Data0, DataC, Data4, Data8 );

	if (!word_3B1EF)
		return;

	int16* Data20 = word_3BDAD;

	Data0 = *(Data20 + word_3AACF);
	if (Data0 < 0)
		return;

	++word_3AACF;
	word_3AACD = Data0;
}

void cFodder::sub_17B64() {

	const struct_3* stru = stru_35C70;

	for (; stru->field_0 != -1;) {
		int16 word_3B19F = stru->field_0;
		int16 word_3B1A1 = stru->field_2;
		int16 word_3B1A3 = stru->field_4;
		int16 word_3B1A5 = stru->field_6;
		int16* Data34 = stru->field_8;

		for (;;) {
			int16 Data0 = word_3B19F;
			int16 Data4 = word_3B1A1;
			int16 Data8 = word_3B1A3;
			int16 DataC = word_3B1A5;

			uint8* Data20 = sub_2AE81( &Data0, &Data4 );

			Data8 = *Data34++;
			if (Data8 < 0) {
				++stru;
				break;
			}

			sub_2AEB6( Data0, Data4, &Data8, &DataC );
			int16 Data10 = word_3B1A3 + 0x08;
			int16 Data14 = word_3B1A5;
			sub_2AF19( Data0, Data4, Data8, Data10, Data14, DataC, Data20 );
			word_3B1A3 += 0x10;
		}
	}

}

void cFodder::Recruit_Draw( cSurface *pImage ) {
	eventProcess();
	mouse_Handle();

	Recruit_Draw_Actors( pImage );
	sub_144A2( pImage );
	Recruit_Draw_HomeAway( pImage );
	//Mouse_DrawCursor();
	sub_13800();

	if (pImage->GetFaded() == false )
		pImage->paletteFade();

	word_42068 = 0x40;
	word_4206A = 0x28;
	word_4206C = 0x110;
	word_4206E = 0xB0;

	//sub_14367();

	g_Window.RenderAt( pImage, cPosition() );
	g_Window.FrameEnd();
	pImage->Restore();
}

uint8* cFodder::GetSpriteData( uint16 pSegment ) {
	
	switch ( pSegment ) {
		case 0x4307:
			return mDataPStuff;
			break;

		case 0x4309:
			return mDataHillBits;
			break;

		case 0x6717:
			return word_3E1B7;
			break;

		default:
			std::cout << "Invalid Sprite\n";
			break;
	}
	return 0;
}

bool cFodder::sub_18C7A() {
	word_3B2FD = 0;
	word_3B2FF = 0;

	if (word_3BDAF <= -16 && word_3BDB1 <= 26) {
		word_3B2FD = -1;
		return true;
	}

	if (word_3BDAF >= 0x10F) {
		if (word_3BDB1 > 0x1A)
			return true;

		if (word_3E0E5[mMissionNumber-1])
			return true;

		return false;
	}

	if (word_3BDB1 < 0x1A)
		return false;

	return true;
}

uint8* cFodder::sub_2AE81( int16 *pData0, int16 *pData4 ) {
	const sSpriteSheet* Sheet = &mSpriteDataPtr[*pData0][*pData4];

	*pData0 = Sheet->mColCount;
	*pData4 = Sheet->mRowCount;

	return GetSpriteData( Sheet->field_2 ) + Sheet->field_0;
}
 
void cFodder::sub_2AF19( int16 pData0, int16 pData4, int16 pData8, int16 pData10, int16 pData14, int16 pDataC, uint8* pData20 ) {
	pData0 &= 0xFFFF;
	pData4 &= 0xFFFF;

	uint8 *si = pData20;
	uint8* es = word_3E1B7;

	dword_44A36 = (pData10 - (pData8 >> 1)) << 16;
	dword_44A3E = dword_44A36;

	dword_44A3A = (pData14 - (pDataC >> 1)) << 16;
	int32 eax = (pData8 << 0x10);
	if (eax <= 0)
		return;

	dword_44A42 = eax / pData0;
	eax = pDataC << 0x10;
	if (eax <= 0)
		return;

	dword_44A46 = eax / pData4;
	for (int16 bx = 0; bx != pData4; ++bx) {
		dword_44A36 = dword_44A3E;

		for (int16 cx = 0; cx != pData0; ++cx) {
			uint8 al = sub_2AFF5( si, bx, cx );
			sub_2B016( es, al );
			dword_44A36 += dword_44A42;
		}

		dword_44A3A += dword_44A46;
   	}

}

uint8 cFodder::sub_2AFF5( uint8* pSi, int16 pBx, int16 pCx ) {

	pSi += 0xA0 * pBx;
	pSi += (pCx >> 1);
	if (pCx & 1)
		return (*pSi) & 0x0F;

	return (*pSi) >> 4;
}

void cFodder::sub_2AEB6( int16 pData0, int16 pData4, int16* pData8, int16* pDataC ) {
	int32	Data = pData0 * *pData8;
	pData0 = (int16) Data / 0x64;
	
	int16 Final8 = pData0;
	
	Data = pData4 * *pData8;
	*pDataC = (int16) Data / 0x64;

	*pData8 = Final8;
}

void cFodder::sub_2B016( uint8* pDi, uint8 pAl ) {
	
	pDi += 0xA0 * (dword_44A3A >> 16); 
	pDi += (dword_44A36 >> 16) >> 1;

	if ((dword_44A36 >> 16) & 1) {
		*pDi &= 0xF0;
		*pDi |= pAl & 0x0F;
		return;
	}

	*pDi &= 0x0F;
	*pDi |= pAl << 4;
}

void cFodder::video_Draw_Unk_2( cSurface* pImage ) {
	
}

void cFodder::map_ClearSpt() {
	
	for (uint16 cx = 0; cx < 4800; ++cx )
		mMapSptPtr[cx] = 0;
}

void cFodder::sub_18C45( cSurface* pImage, int32 pPosY,  const sIntroString* pString ) {

	String_CalculateWidth( 320, mFontWidths, pString->mText );
	String_Print( pImage, mFontWidths, 0, word_3B301, pPosY, pString->mText );
}

void cFodder::String_Print( cSurface* pImage, uint8* pWidths, int32 pParam0, int32 pParam08, int32 pParamC, const char* pText ) {

	word_3B305 = pParamC;
	word_3B307 = 0;

	for (;;) {
	loc_29C7A:;
		uint8 NextChar = *pText++;
		uint8 NextChar10;

		//seg007:0170
		if (NextChar == 0xFD) {
			//sub_29DE2(0x10);

		} else if (NextChar == 0xFE) {
			//sub_29DE2(0x0F);

		} else if (NextChar == 0x00)
			break;

		else {
			//seg007:01A1
			NextChar10 = NextChar;

			if (!(NextChar & 0x80)) {

				//01AB
				if (word_3AC19) {
					if (NextChar == 0x20) {
						NextChar = (uint8) word_3AC19;
						goto loc_29D71;
					}
				} else {
				//1C4
					if (NextChar == 0x20)
						goto loc_29DC7;
				}
				//1CD
				if (NextChar <= 0x39) {
					if (NextChar >= 0x30) {
						NextChar -= 0x30;
						NextChar += 0x1A;
						goto loc_29D71;
					}
				}
				else {
					//loc_29D07
					if (NextChar > 0x5A) {
						NextChar -= 0x61;
						NextChar += 0x39;
						goto loc_29D71;
					}
					else {
						if (NextChar >= 0x41) {
							NextChar -= 0x41;
							goto loc_29D71;
						}
					}
				}
			}

			// 20D
			//loc_29D2D
			int32 unk14 = -1;
			uint8 al;
			uint8* ptr = mUnkStringModifier;
			do {
				if (*ptr == 0xFF)
					goto loc_29C7A;
				++unk14;

				al = *ptr++;

			} while(al != NextChar);

			NextChar = unk14;
			NextChar += 0x24;

			loc_29D71:;

			if (word_3AC21) {
				sub_145AF( pParam0 + NextChar, pParam08, pParamC );
			}
			else			         //0	// C     // 4	   // 8
				sub_13C1C( pImage, pParam0, pParamC, NextChar, pParam08 );

		}
		loc_29DC7:;
		//seg007:02A7
		NextChar10 = pWidths[NextChar10];

		pParam08 += NextChar10;
	}
}

void cFodder::String_CalculateWidth( int32 pPosX, uint8* pWidths, const char* pString ) {
	int32 PositionX = 0;

	for (const char* Text = pString; *Text; ++Text) {

		PositionX += pWidths[*Text];
	}

	pPosX -= PositionX;
	if (pPosX < 0) {
		pPosX = 0;
		PositionX = 318;
	}

	pPosX >>= 1;
	word_3B301 = pPosX;
	word_3B303 = PositionX;
}

int16 cFodder::introPlayText() {
	cSurface* CurrentImage = 0;

	Sprite_SetDataPtrToBase( mFontSpriteSheetPtr );

	for ( word_3B2CF = 0; mIntroText[word_3B2CF].mImageNumber != 0; ++word_3B2CF) {

		word_3B447 = 0x288;
		
		delete CurrentImage;
		if (mIntroText[word_3B2CF].mImageNumber != 0xFF) {

			std::stringstream ImageName;
			size_t ImageSize = 0;
			ImageName << mIntroText[word_3B2CF].mImageNumber;
			ImageName << ".dat";

			CurrentImage = g_Resource.image4PlaneLoad( ImageName.str(), 0xD0 );
			CurrentImage->paletteSet( mPalette, 0xD0 );
		}
		else {
			CurrentImage = new cSurface( 320, 230 );
			CurrentImage->paletteSet( mPalette, 0xD0 );
			word_3B447 = 0xAF;
			
		}

		const sIntroString* IntroString = mIntroText[word_3B2CF].mText;
		while (IntroString->mPosition) {

			sub_18C45( CurrentImage, IntroString->mPosition - 0x19, IntroString );
			++IntroString;
		}
		//loc_16710
		
		int16 Duration = word_3B447;
		int16 Fade = -1;
		bool DoBreak = false;

		while( Fade == -1 || DoBreak == false  ) {
			eventProcess();

			--Duration;

			if (Duration) {
				if (Fade)
					Fade = CurrentImage->paletteFade();

				mouse_GetData();
				if (mouse_Button_Status) {
					word_3B2CF = 15;
					word_3B4F3 = -1;
					CurrentImage->paletteFadeOut();
					Fade = -1;
					DoBreak = true;
				}
			}
			else {
				CurrentImage->paletteFadeOut();
				Fade = -1;
				DoBreak = true;
			}

			g_Window.RenderAt( CurrentImage, cPosition() );
			g_Window.FrameEnd();
			//videoSleep();
		}
	}

	delete CurrentImage;
	return word_3B4F3;
}

void cFodder::Sprite_SetDataPtrToBase( const sSpriteSheet** pSpriteSheet ) {

	mSpriteDataPtr = pSpriteSheet;
}

void cFodder::Load_Sprite_Font() {
	
	delete mDataPStuff;
	mDataPStuff = g_Resource.fileGet( "font.dat", mDataPStuffSize );

	paletteLoad( mDataPStuff + 0xA000, 0x10, 0xD0 );
	Sprite_SetDataPtrToBase( mFontSpriteSheetPtr );
}

void cFodder::intro() {

	//copyprotection();
	word_42851 = 0;
	//video_Draw_unk_0();

	word_3B4F3 = 0;
	Load_Sprite_Font();
	//intro_Music_Play();
	if (ShowImage_ForDuration( "cftitle.dat", 0x1F8 ))
		goto introDone;

	if (introPlayText())
		goto introDone;

	if (ShowImage_ForDuration( "virgpres.dat", 0x2D0 ))
		goto introDone;
	
	if (ShowImage_ForDuration( "sensprod.dat", 0x2D0 ))
		goto introDone;
	
	word_42851 = -1;
	if (ShowImage_ForDuration( "cftitle.dat", 0x318 ))
		goto introDone;

introDone:;
	mIntroDone = -1;
	//sub_1645F();

	delete mDataPStuff;
	mDataPStuff = g_Resource.fileGet( "pstuff.dat", mDataPStuffSize );
	paletteLoad( mDataPStuff + 0xA000, 0x10, 0xF0 );
	//Sound_Unk();
	//Music_Unk();
}

int16 cFodder::ShowImage_ForDuration( const std::string& pFilename, uint16 pDuration ) {
	bool DoBreak = false;
	int16 Fade = -1;
	cSurface* img = g_Resource.image4PlaneLoad( pFilename, 0x100 );
	
	while( Fade == -1 || DoBreak == false  ) {
		eventProcess();

		--pDuration;

		if (pDuration) {
			if (Fade)
				Fade = img->paletteFade();

			mouse_GetData();
			if (mouse_Button_Status) {
				word_3B4F3 = -1;
				img->paletteFadeOut();
				Fade = -1;
				DoBreak = true;
			}
		}
		else {
			img->paletteFadeOut();
			Fade = -1;
			DoBreak = true;
		}

		g_Window.RenderAt( img, cPosition() );
		g_Window.FrameEnd();
		//videoSleep();
	}

	delete img;
	return word_3B4F3;
}

void cFodder::mission_PhaseNext() {
	mMissionPhase += 1;
	mMissionPhaseRemain -= 1;
	if (mMissionPhaseRemain)
		return;

	for (unsigned int x = 0; x < 8; ++x) {
		stru_390FA[x].field_3 = 0;
	}

	word_390D0 = mMissionNumber;
	word_390D2 = mMapNumber;
	--word_390D2;

	mMissionPhaseRemain = mMissionPhases = mMissionPhaseTable[mMissionNumber];
	mMissionPhase = 0;
	++mMissionNumber;
	word_390CE += 0x0F;

	word_390E8 = word_390CE;
	word_390E8 -= 0x0F;
	word_390EA = -1;
	word_390D4 = 0;

	dword_3977E = word_394AC;
	word_394AC[0] = -1;
	dword_394A8 = dword_394A4;
}

void cFodder::videoSleep() {
	static uint64_t delta = 1;

	mTicksDiff = GetTickCount() - mTicksDiff;

	mTicks = mTicksDiff * 60 / 1000;

	sleepLoop(delta * 1000 / 60 - mTicksDiff);

	mTicksDiff = GetTickCount();
}

void cFodder::sleepLoop( int64 pMilliseconds ) {

	uint64 TimeStarted = GetTickCount();
	uint64 TimeFinish = TimeStarted + pMilliseconds;

	do {
		//_system->updateScreen();
		mWindow->EventCheck();

		if (GetTickCount() >= TimeFinish)
			break;

		_sleep( 10 );

	} while (1);

}

void cFodder::WonGame() {
	//seg003:3E6D
	cSurface* img = g_Resource.image4PlaneLoad( "won.dat", 0x100 );

	while(img->paletteFade()) {

		g_Window.RenderAt( img, cPosition() );
		g_Window.FrameEnd();
	}

	while (1) {
		g_Window.EventCheck();
	}
}
void cFodder::sub_2E04C() {

	/*
	if( word_3B2FD ) {
		sub_2E72B();
		return;
	} 

	if( word_3B2FF ) {
		sub_2E064();
	}*/
}

void cFodder::Start() {

	mouse_Setup();
	mouse_Handle();

	for (;;) {
		sub_10BBC();

		word_3A9AC = 0;
		mMapNumber = 0;
		word_3901E = 0x3333;

		sub_10B6D();
		sub_10BFA();

		dword_394A4 = word_391D2;
		word_391D2[0] = -1;

		mMissionPhaseRemain = 1;
		mMissionNumber = 0;

		mission_PhaseNext();

		word_3A9AA = -1;
		word_39096 = -1;

		size_t StuffSize = 0;
		delete mDataPStuff;
		mDataPStuff = g_Resource.fileGet( "pstuff.dat", StuffSize );

		for (unsigned int x = 0; x < 0x30; ++x)
			byte_3DDA2[x] = mDataPStuff[0xA000 + x];

		//loc_1042E:;
		for (;;) {
			//video_?_1();

			if (!word_3A9B2 && !word_3A9AA) {

				if (mMapNumber == 71)
					WonGame();

				++mMapNumber;
				mission_PhaseNext();
				word_3901E = 0x3333;
			}

			// loc_1045F
			sub_10D61();
			sub_10D9F();
			sub_10DEC();
			//video_?_1();
			word_390A6 = 0;

			if (!mIntroDone)
				intro();

			if (dword_3901A != 0x12345678)
				dword_3901A = 0x12345678;

			//loc_10496
			sub_10EA4();
			sub_10EC3();
			if (word_390EA) {
				word_390EA = 0;

				map_Load_Spt();
				word_3ABA7 = -1;

				map_Troops_Prepare();
				map_Load_Players();
				sub_1142D();
				sub_115F7();

				word_3ABA7 = 0;
				Recruit_Show();
				sub_2E04C();
				
				if (!word_3B2FD) {
					if (word_3B2FF) {
						word_390B8 = -1;
						word_390EA = -1;
						word_3A9B2 = -1;
						continue;	// goto loc_1042E;
					}
					word_390B8 = 0;
					//sub_10CE7();
					//word_3ABE9 = 0;
					//word_3ABEB = 0;

					//phase_Intro_Brief();
				}
			}

			//loc_10513
			//sub_18908();
			//map_Load_TileSet();
			map_Load_Spt();

		}
	}
}

void cFodder::Exit( unsigned int pExitCode ) {

}

void cFodder::sub_301F7() {
	sMission_unk0* Data38 = stru_390FA;
	
	for (int16 Data1c = 7; Data1c >= 0; --Data1c) {

		Data38->field_9 &= 0;

	}
}