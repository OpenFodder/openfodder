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

#include "stdafx.hpp"

void cFodder::Image_FadeIn() {
    mSurface->Save();
    mGraphics->PaletteSet();
    mGame_Data.mDemoRecorded.DisableTicks();
    while (mSurface->isPaletteAdjusting()) {
        mSurface->palette_FadeTowardNew();

        Video_Sleep();
        Video_Sleep();
        Video_Sleep();
    }

    mGame_Data.mDemoRecorded.EnableTicks();
}

void cFodder::Image_FadeOut() {

    mSurface->Save();
    mSurface->paletteNew_SetToBlack();
#ifdef EMSCRIPTEN
	return;
#endif
    mGame_Data.mDemoRecorded.DisableTicks();
    while (mSurface->isPaletteAdjusting()) {

        mSurface->palette_FadeTowardNew();

        Video_Sleep();

    }
    mGame_Data.mDemoRecorded.EnableTicks();

}

int16 cFodder::ShowImage_ForDuration(const std::string& pFilename, uint16 pDuration, size_t pBackColor, bool pCanAbort) {
    bool DoBreak = false;

    mGraphics->Load_And_Draw_Image(pFilename, 0x100, pBackColor);
    mGraphics->PaletteSet();
    mSurface->Save();

    while (mSurface->isPaletteAdjusting() || DoBreak == false) {
        --pDuration;

        if (pDuration) {
            if (mSurface->isPaletteAdjusting()) {
                mSurface->palette_FadeTowardNew();
                if (!DoBreak) {
                    Video_Sleep();
                    Video_Sleep();
                    Video_Sleep();
                    continue;
                }
            }

            if (pCanAbort && (mMouseButtonStatus || mPhase_Aborted)) {
                mPhase_Aborted = false;
                mImage_Aborted = -1;
                mSurface->paletteNew_SetToBlack();
                DoBreak = true;
            }
        }
        else {
            if (DoBreak == false) {
                mSurface->paletteNew_SetToBlack();
                DoBreak = true;
            }
        }

        Video_Sleep();
    }

    return mImage_Aborted;
}
