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

const char* mBinTable[] = { "rjnull.bin", 
							"rjnull.bin", 
							"player.bin", 
							"player.bin", 
							"player.bin", 
							"player.bin" };

const uint16 mMissionPhaseTable[] = {	01, 02, 01, 04, 03, 02, 03, 04,
										02, 05, 03, 06, 01, 03, 03, 02,
										01, 05, 01, 04, 01, 04, 05, 06 };

unsigned char mFontWidths[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x09, 0x07, 
  0x08, 0x08, 0x09, 0x08, 0x09, 0x08, 0x09, 0x09, 0x0C, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0B, 0x0C, 0x0C, 0x0A, 
  0x0A, 0x0D, 0x0C, 0x05, 0x07, 0x0C, 0x0B, 0x0E, 0x0C, 0x0D, 
  0x0A, 0x0D, 0x0C, 0x09, 0x0B, 0x0C, 0x0E, 0x0E, 0x0D, 0x0D, 
  0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x09, 0x08, 
  0x09, 0x08, 0x07, 0x09, 0x0A, 0x05, 0x05, 0x0A, 0x05, 0x0F, 
  0x0A, 0x09, 0x09, 0x09, 0x08, 0x08, 0x07, 0x09, 0x0A, 0x0E, 
  0x0A, 0x0A, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0xB0, 
  0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 
  0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 
  0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 
  0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 
  0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 
  0xA0, 0xA0, 0x90, 0x90, 0x80, 0x80, 0x70, 0x70, 0x60, 0x60, 
  0x50, 0x50, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
  0x40, 0x40, 0x50, 0x50, 0x60, 0x60, 0x70, 0x70, 0x80, 0x80, 
  0x90, 0x90, 0xA0, 0xA0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 
  0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 
  0x00, 0x00
};

unsigned char mUnkStringModifier[] =
{
  0x27, 0x2E, 0x2C, 0x21, 0x93, 0x82, 0x87, 0x2D, 0x81, 0x8E, 
  0x99, 0x9A, 0x84, 0x94, 0xA4, 0xA0, 0xA3, 0xA1, 0x3F, 0xA8, 
  0xA5, 0xFF
};

const struct_0 stru_373BA[294] =
{
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 60, 0 },
  { 305, 58, 0 },
  { 305, 56, 0 },
  { 305, 55, 0 },
  { 304, 54, 0 },
  { 303, 59, 1 },
  { 305, 53, 0 },
  { 304, 53, 0 },
  { 303, 54, 0 },
  { 302, 55, 0 },
  { 301, 56, 0 },
  { 301, 57, 0 },
  { 300, 58, 0 },
  { 300, 59, 0 },
  { 299, 60, 0 },
  { 299, 61, 0 },
  { 298, 62, 0 },
  { 298, 63, 0 },
  { 297, 64, 0 },
  { 297, 65, 0 },
  { 296, 66, 0 },
  { 296, 67, 0 },
  { 296, 68, 0 },
  { 295, 69, 0 },
  { 295, 70, 0 },
  { 295, 71, 0 },
  { 294, 72, 0 },
  { 294, 73, 0 },
  { 294, 74, 0 },
  { 294, 75, 0 },
  { 293, 76, 0 },
  { 293, 79, 0 },
  { 292, 82, 0 },
  { 292, 84, 0 },
  { 291, 84, 0 },
  { 292, 84, 0 },
  { 292, 84, 0 },
  { 292, 84, 0 },
  { 292, 84, 0 },
  { 292, 84, 0 },
  { 292, 84, 0 },
  { 292, 84, 0 },
  { 292, 84, 0 },
  { 288, 84, 0 },
  { 287, 82, 0 },
  { 286, 81, 0 },
  { 285, 79, 0 },
  { 285, 77, 0 },
  { 276, 84, 2 },
  { 284, 77, 0 },
  { 283, 78, 0 },
  { 283, 79, 0 },
  { 282, 81, 0 },
  { 281, 82, 0 },
  { 281, 84, 0 },
  { 281, 86, 0 },
  { 281, 87, 0 },
  { 281, 89, 0 },
  { 282, 90, 0 },
  { 282, 92, 0 },
  { 282, 94, 0 },
  { 283, 95, 0 },
  { 283, 97, 0 },
  { 284, 99, 0 },
  { 284, 101, 0 },
  { 285, 103, 0 },
  { 286, 106, 0 },
  { 287, 109, 0 },
  { 287, 112, 0 },
  { 289, 116, 0 },
  { 290, 118, 0 },
  { 292, 121, 0 },
  { 293, 122, 0 },
  { 293, 124, 0 },
  { 294, 126, 0 },
  { 295, 128, 0 },
  { 295, 129, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 295, 133, 0 },
  { 308, 134, 0 },
  { 308, 134, 0 },
  { 308, 132, 0 },
  { 307, 130, 0 },
  { 307, 128, 0 },
  { 307, 126, 0 },
  { 307, 125, 0 },
  { 306, 124, 0 },
  { 288, 121, 3 },
  { 306, 124, 0 },
  { 306, 125, 0 },
  { 305, 126, 0 },
  { 305, 127, 0 },
  { 305, 129, 0 },
  { 304, 130, 0 },
  { 304, 133, 0 },
  { 304, 135, 0 },
  { 304, 137, 0 },
  { 303, 138, 0 },
  { 303, 139, 0 },
  { 304, 141, 0 },
  { 304, 143, 0 },
  { 303, 145, 0 },
  { 303, 147, 0 },
  { 303, 149, 0 },
  { 303, 150, 0 },
  { 303, 152, 0 },
  { 302, 154, 0 },
  { 301, 156, 0 },
  { 301, 158, 0 },
  { 300, 161, 0 },
  { 299, 162, 0 },
  { 298, 163, 0 },
  { 297, 165, 0 },
  { 295, 167, 0 },
  { 294, 168, 0 },
  { 293, 169, 0 },
  { 292, 170, 0 },
  { 291, 172, 0 },
  { 290, 173, 0 },
  { 289, 174, 0 },
  { 288, 175, 0 },
  { 287, 176, 0 },
  { 285, 178, 0 },
  { 283, 179, 0 },
  { 281, 180, 0 },
  { 280, 182, 0 },
  { 278, 183, 0 },
  { 277, 185, 0 },
  { 275, 186, 0 },
  { 273, 186, 0 },
  { 272, 187, 0 },
  { 271, 188, 0 },
  { 269, 189, 0 },
  { 267, 189, 0 },
  { 265, 189, 0 },
  { 263, 190, 0 },
  { 262, 190, 0 },
  { 260, 190, 0 },
  { 258, 190, 0 },
  { 256, 190, 0 },
  { 254, 190, 0 },
  { 252, 190, 0 },
  { 250, 190, 0 },
  { 249, 190, 0 },
  { 247, 190, 0 },
  { 246, 191, 0 },
  { 244, 191, 0 },
  { 242, 191, 0 },
  { 240, 191, 0 },
  { 238, 191, 0 },
  { 236, 191, 0 },
  { 234, 191, 0 },
  { 232, 191, 0 },
  { 230, 191, 0 },
  { 227, 191, 0 },
  { 225, 191, 0 },
  { 223, 191, 0 },
  { 221, 191, 0 },
  { 219, 191, 0 },
  { 217, 191, 0 },
  { 215, 191, 0 },
  { 213, 191, 0 },
  { 211, 191, 0 },
  { 209, 191, 0 },
  { 207, 191, 0 },
  { 205, 191, 0 },
  { 203, 191, 0 },
  { 201, 191, 0 },
  { 199, 191, 0 },
  { 197, 191, 0 },
  { 195, 191, 0 },
  { 193, 191, 0 },
  { 191, 191, 0 },
  { 189, 191, 0 },
  { 187, 191, 0 },
  { 185, 191, 0 },
  { 183, 191, 0 },
  { 181, 191, 0 },
  { 179, 191, 0 },
  { 177, 192, 0 },
  { 175, 192, 0 },
  { 173, 192, 0 },
  { 171, 192, 0 },
  { 169, 192, 0 },
  { 167, 192, 0 },
  { 165, 192, 0 },
  { 163, 193, 0 },
  { 161, 192, 0 },
  { 159, 192, 0 },
  { 157, 192, 0 },
  { 155, 192, 0 },
  { 153, 192, 0 },
  { 151, 192, 0 },
  { 149, 192, 0 },
  { 147, 192, 0 },
  { 145, 192, 0 },
  { 143, 192, 0 },
  { 141, 192, 0 },
  { 139, 192, 0 },
  { 137, 192, 0 },
  { 135, 192, 0 },
  { 133, 192, 0 },
  { 131, 192, 0 },
  { 131, 192, 0 },
  { 129, 192, 0 },
  { 127, 192, 0 },
  { 125, 192, 0 },
  { 123, 192, 0 },
  { 121, 192, 0 },
  { 119, 192, 0 },
  { 117, 192, 0 },
  { 115, 192, 0 },
  { 113, 192, 0 },
  { 111, 192, 0 },
  { 109, 192, 0 },
  { 107, 192, 0 },
  { 105, 192, 0 },
  { 103, 193, 0 },
  { 101, 192, 0 },
  { 99, 192, 0 },
  { 96, 192, 0 },
  { 94, 192, 0 },
  { 92, 192, 0 },
  { 90, 192, 0 },
  { 88, 192, 0 },
  { 86, 192, 0 },
  { 84, 192, 0 },
  { 82, 192, 0 },
  { 80, 192, 0 },
  { 78, 192, 0 },
  { 76, 192, 0 },
  { 74, 192, 0 },
  { 72, 192, 0 },
  { 70, 192, 0 },
  { 68, 192, 0 },
  { 66, 192, 0 },
  { 64, 192, 0 },
  { 62, 192, 0 },
  { 60, 192, 0 },
  { 58, 192, 0 },
  { 56, 192, 0 },
  { 54, 192, 0 },
  { 52, 192, 0 },
  { 50, 192, 0 },
  { 48, 192, 0 },
  { 46, 192, 0 },
  { 44, 192, 0 },
  { 42, 192, 0 },
  { 40, 192, 0 },
  { 38, 192, 0 },
  { 36, 191, 0 },
  { 65535, 65535, 0 }
};

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
	
	mMapSptPtr = new uint16[2655];

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
	word_390D6 = 0;
	word_390E8 = 0;
	word_390EA = 0;
	mMissionPhaseRemain = 0;
	mMissionPhases = 0;

	for (unsigned int x = 0; x < 8; ++x) {
		stru_390FA[x].field_0 = 0;
		stru_390FA[x].field_1 = 0;
		stru_390FA[x].field_2 = 0;
		stru_390FA[x].field_3 = 0;
		stru_390FA[x].field_4 = 0;
		stru_390FA[x].field_5 = 0;
		stru_390FA[x].field_6 = 0;
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

	for (unsigned int x = 0; x < 25; ++x)
		byte_3978E[x] = 0;
	
	word_397D2 = 0;
	word_397D4 = 0;
	mTroopsAvailable = 0;
}

void cFodder::sub_10B6D() {
	word_3915E = -1;
	word_3915A = -1;

	for (unsigned int x = 0; x < 8; ++x) {
		stru_390FA[x].field_3 = -1;
		stru_390FA[x].field_0 = -1;
		stru_390FA[x].field_1 = 0;
		stru_390FA[x].field_2 = 0;
	}
}

void cFodder::sub_10BFA() {
	
	for (unsigned int x = 0; x < 25; ++x) {
		byte_3978E[x] = -1;
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

	word_3A9F7 = 0;
	word_3AA17 = 0;
	word_3AA19 = 0;

	word_3AAD1 = 0;
	word_3AB39 = 0;
	word_3AC19 = 0;
	word_3AC21 = 0;

	dword_3B1FB = 0;
	
	word_3B2CD = 0;
	word_3B2CF = 0;
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
	mMapSpt_Loaded[0] = 0x8000;
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

	mMapSptPtr = (uint16*) g_Resource.fileGet(Filename_Spt, mMapSptSize);
	tool_EndianSwap( (uint8*) mMapSptPtr, mMapSptSize );
	
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
	
	for( int16 x = 7; x > 0; --x ) {
		
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

	mouse_Pos_Column = mMousePosition.mX;
	mouse_Pos_Row = mMousePosition.mY;
	mouse_Button_Status = mMouseButtons;
}

void cFodder::mouse_Handle() {
	
	mouse_GetData();
	mouse_ButtonCheck();

	g_Window.SetMousePosition( cPosition( 160, 100 ) );

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

void cFodder::sub_13C1C( cSurface* pImage, int32 pParam00, int32 pParam0C, int32 pParam04, int32 pParam08 ) {

	uint16 bx = mSpriteDataPtr[pParam00][pParam04].field_0;

	switch ( mSpriteDataPtr[pParam00][pParam04].field_2) {
		case 0x4307:
			word_42062 = mDataPStuff + bx;
			pImage->paletteLoad( mDataPStuff + 0xA000, 0x10, 0xD0 );
			break;

		default:
			break;
	}

	word_42068 = pParam08;
	word_4206A = pParam0C + 0x10;
	word_4206C = mSpriteDataPtr[pParam00][pParam04].mColCount;
	word_4206E = mSpriteDataPtr[pParam00][pParam04].mRowCount;
	if (word_3B307 > pParam00) 
		word_3B307 = pParam00;
	
	byte_42070 = mSpriteDataPtr[pParam00][pParam04].field_C;
	
	if (sub_1429B() )
		video_Draw_Sprite_( pImage );

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

void cFodder::sub_13F58(  cSurface* pImage ) {
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
	unsigned int count = 0;
	for( int16 dx = 225; dx > 0; --dx ) {
		
		for( cx = 0; cx <= word_4206C; ++cx ) {
			byte al = *si;
			if(al)
				*di = al;
			
			si ++;
			di ++;
			++count;
		}
		
		//si += word_42074;
		//di += word_42076;
	}

	pImage->draw();
	return;

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
	
	pImage->draw();
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
		ax - word_4206C;
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

void cFodder::Show_Recruits() {
	
	mouse_Setup();
	map_ClearSpt();
	
	cSurface* ImageHill = new cSurface(320,260);
	
	delete word_3E1B7;
	word_3E1B7 = g_Resource.fileGet( "hill.dat", word_3E1B7_size );
	ImageHill->paletteLoad( word_3E1B7 + 0xFA00, 0x50 );

	delete mDataHillBits;
	mDataHillBits = g_Resource.fileGet( "hillbits.dat", mDataHillBitsSize );

	dword_3B1FB = stru_373BA;
	word_3AAD1 = -1;
	word_3AB39 = -1;
	
	mSpriteDataBasePtr = mHillBitsSpriteSheetPtr;
	Sprite_SetDataPtrToBase();
	//video_Draw_unk_0();

	/*sub_16BC3();
	sub_16C6C();
	Recruit_Draw_LeftMenu();*/
	Recruit_Draw_Hill( ImageHill );

	g_Window.RenderAt( ImageHill, cPosition() );
	g_Window.FrameEnd();
	for (;;) {

	}

	/*sub_17B64();
	
	mSpriteDataBasePtr = off_35E42;
	Sprite_SetDataPtrToBase();
	
	sub_17CD3();
	video_Draw_Unk_2( ImageHill );
	
	word_3BEC1 = 0;
	word_3BEC3 = 0x1D;
	word_39020 = 0;
	
	sub_17368();
	
	word_3AAC9 = 0;
	word_3AACB = word_3B60A;
	
	int16 ax = word_397D4;
	ax -= 1;
	if( ax >= 0 ) {
		
		while( ax > 0 ) 
			sub_17429();
	}

	word_3AA51 = -1;
	word_3A016 = 0;
	word_3AA67 = 0;
	word_3AA71 = word_397D2;
	sub_1787C();
	sub_17592();
	sub_13837();
	sub_17592();
	sub_13837();
	
	si = byte_3D7D2;
	di = byte_3DAD2;
	sub_138A8( 1 );
	
	word_39F02 = 0;
	
	for( ;; ) {
		if( word_39F02 ) {
			word_39F02 = 0;
			
			if( sub_18C7A() )
				break;
		}
		
		sub_17C30();
	}
	
	//loc_168CC
	word_3A9F7 = 0x23;
	word_3AA51 = 0;
	
	si = byte_3DAD2;
	di = byte_3D7D2;
	sub_138A8(0);
	
	while( word_40044 == -1 ) {
		sub_17C30();
	}
	*/
	delete ImageHill;
}

void cFodder::Recruit_Draw_Hill( cSurface* pImage ) {
	word_42062 = word_3E1B7 + 0xA00;
	
	word_42068 = 0x30;
	word_4206A = 0x28;
	word_4206C = 0x110;
	word_4206E = 0xB0;
	word_42078 = 0x140;
	
	sub_13F58( pImage );
	
	// TODO: Why?
	//for( uint32 x = 0; x < 0xA000; ++x) {
	//	word_3E1B7[x] = 0;
	//}
}

void cFodder::video_Draw_Unk_2( cSurface* pImage ) {
	
}

void cFodder::map_ClearSpt() {
	
	//for (uint16 cx = 0; cx < 2655; ++cx )
	//	mMapSptPtr[cx] = 0;
}

void cFodder::sub_18C45( cSurface* pImage, int32 pPosY,  const sIntroString* pString ) {

	String_CalculateWidth( 320, mFontWidths, pString );
	String_Print( pImage, mFontWidths, pPosY, 0, word_3B301, pString->mText );
}

void cFodder::String_Print( cSurface* pImage, uint8* pWidths, int32 pPosY, int32 pParam0, int32 pParam08, const char* pText ) {

	word_3B305 = pPosY;
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
						NextChar = word_3AC19;
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
							NextChar += pParam0;
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
				//sub_145AF( pParam0 + NextChar, pPosX );
			}
			else			//0		// C    // 4	   // 8
				sub_13C1C( pImage, pParam0, pPosY, NextChar, pParam08 );

		}
		loc_29DC7:;
		//seg007:02A7
		NextChar10 = pWidths[NextChar10];

		pParam08 += NextChar10;
	}
}

void cFodder::String_CalculateWidth( int32 pPosX, uint8* pWidths, const sIntroString* pString ) {
	int32 PositionX = 0;

	for (const char* Text = pString->mText; *Text; ++Text) {

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

	//video_Draw_unk_0();
	mSpriteDataBasePtr = mFontSpriteSheetPtr;
	Sprite_SetDataPtrToBase();
	

	for ( word_3B2CF = 0; mIntroText[word_3B2CF].mImageNumber != 0; ++word_3B2CF) {

		word_3B447 = 0x288;
		
		delete CurrentImage;
		if (mIntroText[word_3B2CF].mImageNumber != 0xFF) {

			std::stringstream ImageName;
			size_t ImageSize = 0;
			ImageName << mIntroText[word_3B2CF].mImageNumber;
			ImageName << ".dat";

			CurrentImage = g_Resource.image4PlaneLoad( ImageName.str(), 0x100 );
		}
		else {
			CurrentImage = new cSurface( 320, 230 );
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
			g_Window.EventCheck();

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
	return 0;
}

void cFodder::Sprite_SetDataPtrToBase() {

	mSpriteDataPtr = mSpriteDataBasePtr;
}

void cFodder::Load_Sprite_Font() {
	
	delete mDataPStuff;
	mDataPStuff = g_Resource.fileGet( "font.dat", mDataPStuffSize );

	mSpriteDataBasePtr = mFontSpriteSheetPtr;
	Sprite_SetDataPtrToBase();

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
		stru_390FA[x].field_2 = 0;
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
				//map_Load_Players();
				//sub_1142D();
				//sub_115F7();

				word_3ABA7 = 0;
				Show_Recruits();
				//sub_2E04C();
				/*
				if (!word_3B2FD) {
					if (word_3B2FF) {
						word_390B8 = -1;
						word_390EA = -1;
						word_3A9B2 = -1;
						continue;	// goto loc_1042E;
					}
					word_390B8 = 0;
					sub_10CE7();
					word_3ABE9 = 0;
					word_3ABEB = 0;

					phase_Intro_Brief();
				}*/
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
