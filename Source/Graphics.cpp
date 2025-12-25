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


void cGraphics::HeliIntro_TickParallaxAndText(double dtSeconds)
{
    // ----- Text slide (PAL) -----
    //
    // Original Amiga PAL behaviour:
    //   Each step moves 4 pixels
    // => 4 * 50 = 200 px/sec toward 0x0C.
    const double textVelPxPerSec = 200.0;

    if (Heli_TextPosBottom != 0x0C) {
        g_Fodder->mHeliText_SubPx += textVelPxPerSec * dtSeconds;

        const int32_t stepPx = (int32_t)std::floor(g_Fodder->mHeliText_SubPx);
        if (stepPx > 0) {
            g_Fodder->mHeliText_SubPx -= (double)stepPx;
            Heli_TextPosBottom -= stepPx;

            if (Heli_TextPosBottom <= 0x0C) {
                Heli_TextPosBottom = 0x0C;
                g_Fodder->mHeliText_SubPx = 0.0;
            }
        }
    }

    Heli_TextPos = 344 - Heli_TextPosBottom;

    // ----- Parallax scroll (PAL) -----
    //
    // Original behaviour per PAL frame (50Hz):
    //   baseSpeed = 0x8000 (16.16 units) per frame
    // => per-second speed = 0x8000 * 50 (16.16 units/sec)
    const double baseSpeed_16_16_per_sec = (double)0x8000 * 50.0;

    g_Fodder->mHeliParallax_SubPx += baseSpeed_16_16_per_sec * dtSeconds;

    const int32_t baseStep = (int32_t)std::floor(g_Fodder->mHeliParallax_SubPx);
    if (baseStep != 0) {
        g_Fodder->mHeliParallax_SubPx -= (double)baseStep;

        const int32_t wrap = 320 << 16;

        int32_t d0 = baseStep;

        Heli_VeryBack -= d0;
        if (Heli_VeryBack < 0) Heli_VeryBack += wrap;

        d0 <<= 1;
        Heli_Back -= d0;
        if (Heli_Back < 0) Heli_Back += wrap;

        d0 <<= 1;
        Heli_middle -= d0;
        if (Heli_middle < 0) Heli_middle += wrap;

        d0 <<= 1;
        Heli_Front -= d0;
        if (Heli_Front < 0) Heli_Front += wrap;
    }
}