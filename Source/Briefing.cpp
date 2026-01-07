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

const int16 mBriefing_Helicopter_Offsets_Amiga[30] =
{
  0x0180, 0x0020, 0x0005, 
  0x01A0, 0x0020, 0x001E, 
  0x01C0, 0x0020, 0x0014, 
  0x01E0, 0x0010, 0x0014, 
  0x0000, 0x0008, 0x0014,
  0x0080, 0x0008, 0x003C, 
  0x0100, 0x0008, 0x0014, 
  0x01A0, 0x0004, 0x002E, 
  0x0170, 0x0032, 0x0046,
  -1, -1, -1
};

void cFodder::Briefing_Update_Helicopter_Frame(double dtSeconds)
{
    if (!mBriefingHelicopter_Moving)
        return;

    mBriefingHelicopter_AccumSeconds += dtSeconds;

    while (mBriefingHelicopter_AccumSeconds >= mBriefingHelicopter_StepSeconds) {
        Briefing_Update_Helicopter();
        mBriefingHelicopter_AccumSeconds -= mBriefingHelicopter_StepSeconds;
    }
}

void cFodder::Briefing_Update_Helicopter()
{
    mBriefingHelicopter_ScreenX = (((int)mHelicopterPosX - 0x20) >> 16);
    mBriefingHelicopter_ScreenY = (((int)mHelicopterPosY) >> 16);

    // countdown is in 50Hz ticks now
    mBriefingHelicopter_NextUpdateCountdown -= 1.0f;

    if (mBriefingHelicopter_NextUpdateCountdown <= 0.0f) {

        mBriefingHelicopter_TargetDirection = mBriefing_Helicopter_Offsets_Amiga[mHelicopterOffsetIndex];
        mBriefingHelicopter_TargetSpeed = mBriefing_Helicopter_Offsets_Amiga[mHelicopterOffsetIndex + 1];
        mBriefingHelicopter_NextUpdateCountdown =
            (float)mBriefing_Helicopter_Offsets_Amiga[mHelicopterOffsetIndex + 2];

        if (mBriefingHelicopter_NextUpdateCountdown <= 0.0f)
            return;

        mHelicopterOffsetIndex += 3;
    }

    mBriefingHelicopter_DirectionIndex &= 0x1FE;
    uint16 bx = mBriefingHelicopter_DirectionIndex;

    float ax = (float)mDirectionVectorTable[(bx / 2) & 0xFF];
    ax /= 4.0f;

    mHelicopterPosX += (ax * mBriefingHelicopter_Speed);

    bx += 0x80;
    bx &= 0x1FE;

    ax = (float)mDirectionVectorTable[(bx / 2) & 0xFF];
    ax /= 4.0f;

    mHelicopterPosY += (ax * mBriefingHelicopter_Speed);

    bx = mBriefingHelicopter_TargetDirection - mBriefingHelicopter_DirectionIndex;
    bx >>= 5;
    --bx;
    bx ^= 0x0F;
    bx &= 0x0F;

    int16 al = mDirectionStepTable[bx];
    al <<= 2;

    mBriefingHelicopter_DirectionIndex += al;
    mBriefingHelicopter_DirectionIndex &= 0x1FE;

    if (mBriefingHelicopter_TargetSpeed != mBriefingHelicopter_Speed) {
        if (mBriefingHelicopter_Speed > mBriefingHelicopter_TargetSpeed)
            mBriefingHelicopter_Speed -= 1.0f;
        else
            mBriefingHelicopter_Speed += 1.0f;
    }

    if (mVersionCurrent->isCannonFodder1() && mVersionCurrent->isPC())
        mBriefingHelicopter_FrameCounter += 1;

    if (mBriefingHelicopter_FrameCounter >= 4)
        mBriefingHelicopter_FrameCounter = 0;
}

void cFodder::Briefing_Helicopter_Check(double dtSeconds)
{
    Music_Increase_Channel_Volume();

    // Transition gate: original “every 2nd frame @ 50 Hz” => 25 steps/sec.
    // That is a step period of 1/25 = 0.04s. Every time we pass it, we do one palette/moving step.
    const double kTransitionStepSeconds = 1.0 / 25.0;

    mBriefingHelicopter_TransitionCounterSeconds += dtSeconds;

    auto doTransitionStepIfDue = [&]() -> bool {
        if (mBriefingHelicopter_TransitionCounterSeconds < kTransitionStepSeconds)
            return false;
        // keep remainder so it’s stable under varying dt
        mBriefingHelicopter_TransitionCounterSeconds =
            std::fmod(mBriefingHelicopter_TransitionCounterSeconds, kTransitionStepSeconds);
        return true;
        };

    // ---- “Fade in until ready, then start heli movement” ----
    if (!mMouse_Exit_Loop || !mPhase_Aborted) {

        if (mBriefingHelicopter_ScreenX > 0x30) {

            if (doTransitionStepIfDue()) {
                // Original: 16 steps total for fade-in.
                if (mBriefingHelicopter_Moving < 16) {
                    mBriefingHelicopter_Moving++;
                    mPaletteLevel++;
                    return;
                }
            }

            // Once fade-in complete, advance helicopter sim (60Hz tick wrapper)
            if (mBriefingHelicopter_Moving >= 16) {
                Briefing_Update_Helicopter_Frame(dtSeconds);
            }
            return;
        }
    }

    // ---- “Fade out / exit path” ----
    if (doTransitionStepIfDue()) {
        if (mBriefingHelicopter_Moving == 0 && mBriefingHelicopter_NotDone) {
            mBriefingHelicopter_NotDone = 0;
            mGraphics->mImageMissionIntro.CopyPalette(mGraphics->mPalette, 0x100, 0);
        }
        else {
            if (mBriefingHelicopter_Moving > 0 || mPaletteLevel > 0) {
                mBriefingHelicopter_Moving--;
                mPaletteLevel--;
                mSurface->paletteNew_SetToBlack();
            }
        }
    }

    // Always keep heli sim running on the exit path too (matches your original behaviour)
    Briefing_Update_Helicopter_Frame(dtSeconds);
}

std::string cFodder::Briefing_Get_Mission_Title() {
    std::stringstream Mission;

    if (mGame_Data.mMission_Number == 0) {
        Mission << "OPEN FODDER";
    }
    else {
        Mission << "MISSION ";
        Mission << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Number));
    }

    return Mission.str();
}

std::string cFodder::Briefing_Get_Phase_Name() {
    if (mGame_Data.mMission_Number == 0)
        return "";

    return mGame_Data.mMission_Current->GetName();
}

/**
 * Draw the Mission Name or the Map Name
 *  Drawing at 0xB5 will cause the Mission Name to be drawn.
 *
 * @param pDrawAtY
 */
void cFodder::Briefing_Draw_Mission_Title(int16 pDrawAtY) {

    // Draw MISSION xx
    {
        std::stringstream Mission;
        Mission << "MISSION ";
        mString_GapCharID = 0x25;
        Mission << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Number));

        String_Print_Large(Mission.str(), true, 0);
    }

    // Draw Mission Name, or Map 
    {
        std::string Title;

        if (pDrawAtY == 0xB5) {
            Title = mGame_Data.mMission_Current->GetName();

            pDrawAtY += PLATFORM_BASED(0, 0x16);

        }
        else {
            Title = mGame_Data.mPhase_Current->GetName();
        }

        String_Print_Large(Title, true, pDrawAtY);
    }
}

void cFodder::Briefing_Draw_Phase() {
    const char* Str_Brief = "BRIEFING";
    const char* Str_Phase = "PHASE ";
    const char* Str_Of = " OF ";

    mString_GapCharID = 0x25;

    String_Print_Large(Str_Brief, true, 0x4E);

    Briefing_DrawBox(1, 0x49, 0x13E, 0x6B, 0xF3);
    Briefing_DrawBox(0, 0x48, 0x13E, 0x6B, 0xF2);

    mString_GapCharID = 0;

    std::stringstream Phase;

    Phase << Str_Phase << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Phase));
    Phase << Str_Of << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Current->NumberOfPhases()));

    String_Print_Small(Phase.str(), 0x1D);

    Phase_Goals_Set();

    int16 DataC = 0x84;
    bool* Goals = mGame_Data.mGamePhase_Data.mGoals_Remaining;

    for (const auto GoalName : mMissionGoal_Titles) {
        if (*Goals++) {

            String_Print_Small(GoalName, DataC - 0x12);
            DataC += 0x0C;
        }
    }
}

void cFodder::Briefing_Show_PreReady() {

    if (mVersionCurrent->isDemo() && !mGame_Data.mCampaign.isCustom() && !mGame_Data.mCampaign.isRandom())
        return;

    if (!mVersionCurrent->hasGfx(eGFX_BRIEFING) && !mGame_Data.mCampaign.isRandom())
        VersionSwitch(mVersionDefault);

	mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());
    mSurface->clearBuffer();
    mGraphics->PaletteSet();

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    Briefing_Draw_Mission_Title(0x2C);
    Briefing_Draw_Phase();

    Image_FadeIn();
}

void cFodder::Briefing_Show_Ready() {

    if (mVersionCurrent->isDemo() && !mGame_Data.mCampaign.isCustom() && !mGame_Data.mCampaign.isRandom())
        return;

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    Briefing_Draw_Mission_Title(0x2C);
    Briefing_Draw_Phase();
    Briefing_Draw_With();

    mSurface->Save();
    mMouse_Exit_Loop = false;

    do {
        Music_Increase_Channel_Volume();

        if (mPhase_Aborted) {
            mBriefing_Aborted = -1;
            mMouseButtonStatus = -1;
            break;
        }

        Video_Sleep();

    } while (!mMouse_Exit_Loop);

    mMouse_Exit_Loop = false;

    Image_FadeOut();
    Mouse_Setup();
}

void cFodder::Briefing_Draw_With() {
    std::stringstream With;

    With << "WITH ";
    With << tool_StripLeadingZero(std::to_string(mGame_Data.mGamePhase_Data.mSoldiers_Available));

    if (mGame_Data.mGamePhase_Data.mSoldiers_Available == 1) {
        With << " SOLDIER YOU MUST";
    }
    else {
        With << " SOLDIERS YOU MUST";
    }

    String_Print_Small(With.str(), 0x64);
    With.str("");

    if (!mGame_Data.mRecruits_Available_Count) {
        With << "THIS IS YOUR LAST CHANCE";
    }
    else {
        With << mGame_Data.mRecruits_Available_Count;
        if (mGame_Data.mRecruits_Available_Count == 1)
            With << " RECRUIT REMAINING";
        else
            With << " RECRUITS REMAINING";
    }

    String_Print_Small(With.str(), 0xA8);
    String_Print_Large("GO FOR IT", false, 0xB8);
}

void cFodder::Briefing_DrawBox(int16 pX, int16 pY, int16 pWidth, int16 pHeight, uint8 pColor) {
    pX += 0x10;
    pY += 0x10;

    // Top and Bottom
    Briefing_Draw_Horizontal_Line(pX, pX + pWidth, pY, pColor);

    Briefing_Draw_Horizontal_Line(pX, pX + pWidth, pY + pHeight, pColor);

    // Sides of box
    Briefing_Draw_Vertical_Line(pX, pY + pHeight, pY, pColor);

    Briefing_Draw_Vertical_Line(pX + pWidth, pY + pHeight, pY, pColor);
}

void cFodder::Briefing_Draw_Pixel(int16 pX, int16 pY, uint8 pColor) {
    uint8* di = mSurface->GetSurfaceBuffer();

    di += mSurface->GetWidth() * pY;
    di += pX;
    *di = pColor;
}

void cFodder::Briefing_Draw_Horizontal_Line(int16 pX, int16 pWidth, int16 pY, uint8 pColor) {

    do {
        Briefing_Draw_Pixel(pX, pY, pColor);
        ++pX;
    } while (pX <= pWidth);
}

void cFodder::Briefing_Draw_Vertical_Line(int16 pX, int16 pHeight, int16 pY, uint8 pColor) {

    do {
        Briefing_Draw_Pixel(pX, pY, pColor);
        ++pY;
    } while (pY <= pHeight);
}

int16 cFodder::Briefing_Show() {

	if (mParams->mSkipBriefing)
		return 1;

	if (mVersionCurrent->isDemo() && mVersionDefault->isDemo())
		return 1;

	// Show the Briefing screen for Retail and Custom 
	if (mVersionCurrent->hasBriefingScreen() || mCustom_Mode != eCustomMode_None || mGame_Data.mCampaign.isRandom()) {

		Window_UpdateScreenSize();

		// Show the pre ready Briefing Screen
		Briefing_Show_PreReady();

		Map_Load();
		Map_Load_Sprites();

		// Prepare Squads
		Phase_Soldiers_Count();
		mGame_Data.Soldier_Sort();
		Phase_Soldiers_Prepare(false);
		Phase_Soldiers_AttachToSprites();

		mPhase_Aborted = false;

		// Needs to split into cycle function
		Briefing_Show_Ready();

		// Aborted?
		if (mBriefing_Aborted == -1) {

			if (mGame_Data.mCampaign.isRandom())
				return -1;// Return to version select

			GameData_Restore();

			mRecruit_Mission_Restarting = true;
			mGame_Data.mMission_Recruitment = -1;
			mPhase_Aborted = true;

			if (!mStartParams->mDisableSound)
				Music_Play(0);

			return 0;
		}
	}

	return 1;
}

void cFodder::Briefing_Intro_Helicopter_Start() {

    mBriefingHelicopter_TransitionCounterSeconds = 0;
    mBriefingHelicopter_AccumSeconds = 0.0;
    mBriefingHelicopter_StepSeconds = 1.0 / 60.0;
    mHeliParallax_SubPx = 0;
    mHeliText_SubPx = 0;

    mHelicopterPosX = 0x01500000;
    mHelicopterPosY = 0x001E0000;

    mBriefingHelicopter_Speed = 0;
    mPaletteLevel = 0;
    mBriefingHelicopter_ScreenX = 0xD0;

    mBriefingHelicopter_Moving = 0;
    mHelicopterOffsetIndex = 0;

    mBriefingHelicopter_NextUpdateCountdown = 0;

    Briefing_Update_Helicopter();

    mBriefingHelicopter_FrameCounter = 0;
    mBriefingHelicopter_Moving = 1;
    mBriefingHelicopter_NotDone = -1;

    mBriefingHelicopter_DirectionIndex = 0x180;
    mBriefingHelicopter_TargetDirection = 0x180;
}

void cFodder::Briefing_Intro_Helicopter_Draw_OpenFodder() {
    // Draw OPEN FODDER
    {
        mString_GapCharID = 0x25;
        String_Print_Large("OPEN FODDER", true, 0x01);
    }

    // Draw Mission Name, or Map 
    {
        int16 DrawY = 0xB5;

        DrawY += PLATFORM_BASED(0, 0x16);
        //String_Print_Large("CAMPAIGNS", false, DrawY);
    }
}

void cFodder::Briefing_Intro_Helicopter_Draw_Mission_Name() {

    if (mGame_Data.mMission_Number == 0)
        Briefing_Intro_Helicopter_Draw_OpenFodder();
    else
        Briefing_Draw_Mission_Title(0xB5);
}

void cFodder::Briefing_Intro_Helicopter_Play(const bool pShowHelicopter, eTileTypes pTileset) {

    // Single maps
    if (mCustom_Mode == eCustomMode_Map)
        return;

    // If we don't have the briefing graphics, there is no point in switching here,
    //  As the correct background for the map intro won't exist
    if (!mVersionCurrent->hasGfx(eGFX_BRIEFING))
        return;

    mSurface->clearBuffer();
	mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());

    if (pTileset >= eTileTypes_Hid)
		pTileset = eTileTypes_Jungle;

    mGraphics->Mission_Intro_Load_Resources(pTileset);
    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    mMouse_Exit_Loop = false;
    Music_Play(0x07);
    Briefing_Intro_Helicopter_Start();

    //Briefing_Intro_Helicopter_Draw_Mission_Name();
    mSurface->Save();

	// Prior to mission 4, the UFO is not shown on the mission intro
    bool ShowHelicopter = true;
    if (mVersionCurrent->isCannonFodder2() && mGame_Data.mMission_Number < 4 && !pShowHelicopter)
        ShowHelicopter = false;

    mWindow->ToggleVSync(true);
    mVersionPlatformSwitchDisabled = true;
    mGraphics->Briefing_Intro_Helicopter_Play(ShowHelicopter, pTileset, Briefing_Get_Mission_Title(), Briefing_Get_Phase_Name());
    mVersionPlatformSwitchDisabled = false;
    mWindow->ToggleVSync(false);
}
