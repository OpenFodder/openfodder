/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
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

#pragma once

#include <cstdint>
#include <cmath>

// 32-bit RNG for the RANDOM-MAP GENERATOR ONLY (sMapParams::mRandom, behind the
// JS Map.getRandomInt/Float/seed API). This is deliberately NOT cPseudorand: the
// original game's int16 cPseudorand is kept for gameplay (cFodder::mRandom) so
// original-game behaviour is byte-for-byte preserved. New generated maps get a
// full 32-bit seed space here.
//
// The LCG core and the Int/Float derivation MATCH the JS generator
// (Run/Scripts/MapGen/Random.js MapGen.Random.CreateSeeded): mult 1664525,
// add 1013904223, mod 2^32, with nextFloat() = state / 2^32 in [0,1). Seeding
// the two identically therefore yields identical Int/Float sequences (JS<->C++
// parity), which the grammar generator can rely on.
//
// API mirrors the cPseudorand members that sMapParams::mRandom actually used
// (setSeed / getu / getu(min,max) / getf / getStartingSeed) so it is a drop-in.
class cMapRandom32 {
private:
	uint32_t mState;
	uint32_t mStartingSeed;

	uint32_t next() {
		mState = (uint32_t)((mState * 1664525u) + 1013904223u);
		return mState;
	}

	// [0,1), exactly as MapGen.Random.nextFloat in JS.
	double nextFloat() {
		return (double)next() / 4294967296.0;
	}

public:
	cMapRandom32(uint32_t pSeed = 0) {
		setSeed(pSeed);
	}

	void setSeed(uint32_t pSeed) {
		mStartingSeed = pSeed;
		mState = pSeed;
	}

	uint32_t getStartingSeed() const {
		return mStartingSeed;
	}

	// Raw 32-bit draw (used as a sub-seed, e.g. DiamondSquare, or `% range` by
	// older callers).
	uint32_t getu() {
		return next();
	}

	// Inclusive integer range, matching JS MapGen.Random.Int (floor of a float
	// scaled across the span, with the min/max swap guard).
	uint32_t getu(size_t pMin, size_t pMax) {
		int64_t min = (int64_t)pMin;
		int64_t max = (int64_t)pMax;
		if (max < min) {
			int64_t swap = min;
			min = max;
			max = swap;
		}
		return (uint32_t)(min + (int64_t)std::floor(nextFloat() * (double)((max - min) + 1)));
	}

	// Float range, matching JS MapGen.Random.Float.
	float getf(float pMin, float pMax) {
		return pMin + (float)(nextFloat() * (double)(pMax - pMin));
	}
};
