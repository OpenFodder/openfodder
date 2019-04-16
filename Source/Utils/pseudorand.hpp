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

class cPseudorand {
private:
	int16 mRandom_0;
	int16 mRandom_1;
	int16 mRandom_2;
	int16 mRandom_3;

	int16 mStartingSeed;
public:

	cPseudorand(int16 pSeed = 0) {
		if (!pSeed) {
			const time_t now = time(0);
			tm* ltm;
#ifndef _WIN32
			ltm = localtime(&now);
#else
			ltm = new tm();
			localtime_s(ltm, &now);
#endif
			pSeed = tool_DecimalToBinaryCodedDecimal(ltm->tm_sec);
			pSeed |= tool_DecimalToBinaryCodedDecimal(ltm->tm_min) << 8;
			pSeed += 0x40B;
#ifdef _WIN32
			delete ltm;
#endif
		}
		setSeed(pSeed);
	}

	void setSeed(int16 pSeed) {
		mStartingSeed = pSeed;

		mRandom_0 = mStartingSeed;
		mRandom_1 = -mStartingSeed;
		mRandom_2 = 1;
		mRandom_3 = 0;
	}

	void setSeed(int16 pSeed0, int16 pSeed1, int16 pSeed2, int16 pSeed3 ) {
		mStartingSeed = 0;

		mRandom_0 = pSeed0;
		mRandom_1 = pSeed1;
		mRandom_2 = pSeed2;
		mRandom_3 = pSeed3;
	}

	void getSeeds(int16& pSeed0, int16& pSeed1, int16& pSeed2, int16& pSeed3) {
		pSeed0 = mRandom_0;
		pSeed1 = mRandom_1;
		pSeed2 = mRandom_2;
		pSeed3 = mRandom_3;
	}

	int16 getStartingSeed() const {
		return mStartingSeed;
	}


	float getf(float pMin, float pMax) {
		
		return pMin + static_cast <float> (getu() % RAND_MAX) / (static_cast <float> (RAND_MAX) / (pMax - pMin));
	}

	uint16 getu() {
		return (uint16)get();
	}

	uint16 getu(size_t pMin, size_t pMax) {
		return (uint16)(pMin + (getu() % static_cast<int>(pMax - pMin + 1)));
	}

	int16 get() {
		int16 Data0 = mRandom_0;
		int16 Data2 = mRandom_1;
		int16 Data4 = mRandom_2;
		int16 Data6 = mRandom_3;

		uint32 Dat4 = Data4 | (Data6 << 16);
		uint8 CF = Data4 & 1;
		uint32 Data8 = Data0 | (Data2 << 16);

		uint8 CF2 = Data8 & 1;
		Data8 >>= 1;

		if (CF)
			Data8 |= 0x80000000;

		Dat4 += CF2;
		Data4 = Dat4 & 0xFFFF;
		Data6 = Dat4 >> 16;

		for (uint16 cx = 0x0C; cx > 0; --cx) {
			CF = 0;

			if (Data0 & 0x8000)
				CF = 1;
			Data0 <<= 1;
			Data2 <<= 1;
			if (CF)
				Data2 |= 1;
		}

		int16 DataA = Data8 >> 16;

		Data0 ^= Data8 & 0xFFFF;
		Data2 ^= DataA;
		Data8 = Data0;
		DataA = Data2;

		int16 ax = Data8;
		int16 bx = DataA;
		Data8 = bx;
		DataA = ax;

		Data8 >>= 4;
		Data0 ^= Data8;

		mRandom_0 = Data0;
		mRandom_1 = Data2;
		mRandom_2 = Data4;
		mRandom_3 = Data6;
		return Data0;
	}
};