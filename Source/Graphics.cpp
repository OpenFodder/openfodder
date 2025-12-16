/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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

cGraphics::cGraphics() {

	Heli_VeryBack = 0;
	Heli_Back = 0;
	Heli_middle = 0;
	Heli_Front = 0;
	Heli_TextPos = 0;
	Heli_TextPosBottom = 0;

    mFodder = g_Fodder;
	mSurface = g_Fodder->mSurface;
	mImageOriginal = mSurface;

	mImageTemporary.mData->resize(100000);
}

void cGraphics::SetImage( cSurface* pImage ) {
	mSurface = pImage;
}

void cGraphics::SetImageOriginal() {

	mSurface = mImageOriginal;
}

void cGraphics::SetSurfaceOriginal(cSurface* pImage) {
	mImageOriginal = pImage;
}


void cGraphics::HeliIntro_TickParallaxAndText() {
	const float scale = mFodder->mBriefingHelicopter_TimeScale;

	if (Heli_TextPosBottom != 0x0C) {
		Heli_TextPosBottom -= static_cast<int32_t>(4 * scale);

		if (Heli_TextPosBottom <= 0x0C)
			Heli_TextPosBottom = 0x0C;
	}
	Heli_TextPos = 344 - Heli_TextPosBottom;

	int32_t baseSpeed = static_cast<int32_t>(0x8000 * scale);
	int32_t d0 = baseSpeed;

	Heli_VeryBack -= d0;
	if (Heli_VeryBack < 0)
		Heli_VeryBack += 320 << 16;

	d0 <<= 1;
	Heli_Back -= d0;
	if (Heli_Back < 0)
		Heli_Back += 320 << 16;

	d0 <<= 1;
	Heli_middle -= d0;
	if (Heli_middle < 0)
		Heli_middle += 320 << 16;

	d0 <<= 1;
	Heli_Front -= d0;
	if (Heli_Front < 0)
		Heli_Front += 320 << 16;
}
