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
#include "VideoMPEG.hpp"

void cFodder::Intro_OpenFodder() {

	if (!mOpenFodder_Intro_Done && !mParams->mSkipIntro) {
		bool CF2 = false;

		mPhase_Aborted = false;
		mGame_Data.mMission_Number = 0;

		// Use a CF2 intro?
		if (mVersions->isCampaignAvailable("Cannon Fodder 2"))
			CF2 = (tool_RandomGet() & 2);
		if (CF2)
			VersionSwitch(mVersions->GetForCampaign("Cannon Fodder 2", mParams->mDefaultPlatform));

        Mouse_Setup();

		// Random intro
		auto Tileset = static_cast<eTileTypes>(((uint8)tool_RandomGet()) % eTileTypes_Hid);
		Briefing_Intro_Helicopter_Play(true, Tileset);
		mOpenFodder_Intro_Done = true;
		if (CF2)
			VersionSwitch(mVersions->GetForCampaign("Cannon Fodder", mParams->mDefaultPlatform));

        while (Music_Decrease_Channel_Volume()) {
            sleepLoop(1);
        }
	}
}


void cFodder::Intro_LegionMessage() {
    int16 Duration = 150;
    bool DoBreak = false;

    mSurface->clearBuffer();
    mGraphics->PaletteSet();

    Intro_Print_String(&mVersionCurrent->getIntroData()->at(0).mText[0]);
	Intro_Print_String(&mVersionCurrent->getIntroData()->at(0).mText[1]);
	Intro_Print_String(&mVersionCurrent->getIntroData()->at(0).mText[2]);
    mSurface->Save();

    while (mSurface->isPaletteAdjusting() || DoBreak == false) {

        if (mSurface->isPaletteAdjusting()) {
            mSurface->palette_FadeTowardNew();

            if (!DoBreak) {
                Video_Sleep();
                Video_Sleep();
                Video_Sleep();
                continue;
            }
        }

        if (Duration >= 0)
            --Duration;
        else {
            if (DoBreak == false) {
                mSurface->paletteNew_SetToBlack();

                Duration = 0;
                DoBreak = true;
            }
        }

        Video_Sleep();
    }
}

void cFodder::Intro_Main() {
	if (!mIntroDone) {
		mImage_Aborted = 0;
		mVersionPlatformSwitchDisabled = true;

		if (!mParams->mSkipIntro) {
			mWindow->SetScreenSize(mVersionCurrent->GetSecondScreenSize());

#ifdef OPENFODDER_ENABLE_FFMPEG
			Intro_PlayVideo();
#endif

			// Show the intro for retail releases (and the PC Format demo)
			if (mVersionCurrent->isRetail() || mVersionCurrent->isPCFormat()) {
				Intro_Retail();
			}
			else {
				// Amiga The One has an intro too
				if (mVersionCurrent->isAmigaTheOne()) {
					Intro_AmigaTheOne();
				}
			}
		}

		mGraphics->Load_pStuff();
		mVersionPlatformSwitchDisabled = false;
		mIntroDone = true;
	}
}

void cFodder::Intro_Retail() {

    // Disabled: GOG CD Version doesn't require the game manual check
    CopyProtection();
    mGraphics->Load_Sprite_Font();

	Music_Play(CANNON_BASED(16, 20));
    Music_SetFullVolume();

    if (mVersionCurrent->isCannonFodder1())
        Intro_LegionMessage();

    if (mVersionCurrent->isCannonFodder2())
        sleepLoop(1000);

    mPhase_Aborted = false;

    if (ShowImage_ForDuration("cftitle", CANNON_BASED(0x177, 0x280)))
        return;

    if (Intro_Play()) {

        while (Music_Decrease_Channel_Volume()) {
            sleepLoop(1);
        }

        return;
    }

    if (ShowImage_ForDuration("virgpres", 0x15E))
        return;

    if (ShowImage_ForDuration("sensprod", 0x15E))
        return;

    if (ShowImage_ForDuration("cftitle", 0x258))
        return;
}

void cFodder::Intro_AmigaTheOne() {
    static bool ShownWarning = false;

    if (!ShownWarning) {
        ShownWarning = true;
        ShowImage_ForDuration("PRETENTIOUS1", 0x60, 3, false);
    }

    if (ShowImage_ForDuration("sensprod.lbm", 0x60))
        return;

    if (ShowImage_ForDuration("virgpres.lbm", 0x60))
        return;

    if (ShowImage_ForDuration("cftitle.lbm", 0x100))
        return;
}

int16 cFodder::Intro_Play() {

    mGraphics->Load_Sprite_Font();
    mGraphics->SetActiveSpriteSheet(eGFX_Types::eGFX_FONT);

    for (word_3B2CF = 1; mVersionCurrent->getIntroData()->at(word_3B2CF).mImageNumber != 0; ++word_3B2CF) {

        mIntro_PlayTextDuration = 0x12C;

        mSurface->palette_SetToBlack();

        if (mVersionCurrent->getIntroData()->at(word_3B2CF).mImageNumber == 0 && mVersionCurrent->getIntroData()->at(word_3B2CF).mText == 0)
            break;

        if (mVersionCurrent->getIntroData()->at(word_3B2CF).mImageNumber != 0xFF) {

            std::stringstream ImageName;
            ImageName << (char)mVersionCurrent->getIntroData()->at(word_3B2CF).mImageNumber;

            mGraphics->Load_And_Draw_Image(ImageName.str(), 0xD0);
        }
        else {
            mIntro_PlayTextDuration = 0x96;
            mSurface->clearBuffer();
        }

        mGraphics->PaletteSet();
        const sIntroString* IntroString = mVersionCurrent->getIntroData()->at(word_3B2CF).mText;
        if (IntroString) {
            while (IntroString->mPosition) {

                Intro_Print_String(IntroString);
                ++IntroString;
            }
        }
        //loc_16710

        int16 Duration = mIntro_PlayTextDuration;
        bool DoBreak = false;

        mSurface->Save();

        while (mSurface->isPaletteAdjusting() || DoBreak == false) {

            if (mSurface->isPaletteAdjusting()) {
                mSurface->palette_FadeTowardNew();
                if (!DoBreak) {
                    Video_Sleep();
                    Video_Sleep();
                    Video_Sleep();
                    continue;
                }
            }

            if (Duration) {
                --Duration;
                if (mMouseButtonStatus) {

                    if (mVersionCurrent->getIntroData()->size() >= 2)
                        word_3B2CF = ((int16)mVersionCurrent->getIntroData()->size()) - 2;

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


    }

    return mImage_Aborted;
}

void cFodder::Intro_PlayVideo() {
    if (!mVersionCurrent->isRetail() || !mVersionCurrent->isCannonFodder1()) {
		return;
	}

	if (mStartParams && (mStartParams->mDisableVideo || mStartParams->mDisableIntroVideo))
		return;
#ifdef OPENFODDER_ENABLE_FFMPEG
	cVideoMPEG introVideo;
	if (!introVideo.LoadIntroFromData())
		return;

	if (!introVideo.Play())
		return;

	mMouseButtonStatus = false;
	mPhase_Aborted = false;

    Mouse_Setup();

	uint64 lastTick = SDL_GetTicks();
	while (introVideo.isPlaying()) {
		uint64 nowTick = SDL_GetTicks();
		double deltaSeconds = (nowTick - lastTick) / 1000.0;
		lastTick = nowTick;

		introVideo.Update(deltaSeconds);
		introVideo.Render();

		mWindow->Cycle();
		eventsProcess();

		if (mMouseButtonStatus || mPhase_Aborted) {
			break;
		}

		SDL_Delay(1);
        mVideo_Done = true;
	}

	introVideo.Stop();
	mMouseButtonStatus = false;
	mPhase_Aborted = false;
    #endif
}


void cFodder::Intro_Print_String(const sIntroString* pString) {

    auto PosY = pString->mPosition + PLATFORM_BASED(-0x19, 9);

    String_CalculateWidth(320, mFont_Intro_Width, pString->mText);
    String_Print(mFont_Intro_Width, 0, mGUI_Temp_X, PosY, pString->mText);
}
