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

void cFodder::Service_Show() {
    if (mParams->mSkipService)
        return;

    word_82176 = 0;

	mWindow->SetScreenSize(mVersionCurrent->GetScreenSize());
    mVersionPlatformSwitchDisabled = true;

    WindowTitleSet(false);

    mGraphics->Load_Service_Data();
    mGraphics->SetActiveSpriteSheet(eGFX_SERVICE);
    mGraphics->PaletteSet();

    Music_Play(0);
    Service_KIA_Loop();

    if(!mVersionCurrent->isAmigaTheOne())
        Service_Promotion_Loop();

    Mouse_Setup();

    mGraphics->Load_pStuff();
    mVersionPlatformSwitchDisabled = false;
    mPhase_Aborted = false;
}

void cFodder::Service_KIA_Loop() {
    Mouse_Setup();
    mService_Promotion_Exit_Loop = 0;

    if (Service_KIA_Troop_Prepare() < 0)
        return;

    mSurface->clearBuffer();

    if (mVersionCurrent->isPC()) {
        GUI_Draw_Frame_8(5, 0, 0x34, 0);
        GUI_Draw_Frame_16(7, 0, 0, 0x31);
        GUI_Draw_Frame_16(7, 0, 0xF0, 0x31);
    }
    else {
        GetGraphics<cGraphics_Amiga>()->Service_Draw(8, 0x30, 0);
        GetGraphics<cGraphics_Amiga>()->Service_Draw(0, 0, 0x40);
        GetGraphics<cGraphics_Amiga>()->Service_Draw(4, 0xF0, 0x40);
    }

    mMouse_Exit_Loop = false;
    mService_ExitLoop = 0;
    mGraphics->PaletteSet();
    mSurface->Save();

    mInterruptCallback = [this]() {
        Service_Draw_List();
        if (mInterruptTick % 2 == 0)
            Service_ScrollUp_DrawList();
    };

    do {
        Video_Sleep();
        Video_Sleep();

        if (mService_Promotion_Exit_Loop == -1 || mMouse_Exit_Loop) {
            mMouse_Exit_Loop = false;
            mSurface->paletteNew_SetToBlack();
            mService_ExitLoop = 1;
        }

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

    } while (mSurface->isPaletteAdjusting() || mService_ExitLoop == 0);

    mInterruptCallback = nullptr;
}

void cFodder::Service_Promotion_Loop() {
    Mouse_Setup();
    mService_Promotion_Exit_Loop = 0;
    
    if (Service_Promotion_Prepare_Draw() < 0)
        goto loc_18001;

    mSurface->clearBuffer();
    if (mVersionCurrent->isPC()) {
        GUI_Draw_Frame_8(6, 0, 0x34, 0);
        GUI_Draw_Frame_16(8, 0, 0, 0x31);
        GUI_Draw_Frame_16(8, 0, 0xF0, 0x31);
    }
    else {
        GetGraphics<cGraphics_Amiga>()->Service_Draw(9, 0x30, 0);        // Heroes in Victory
		GetGraphics<cGraphics_Amiga>()->Service_Draw(3, 0, 0x40);        //  Left Symbol
		GetGraphics<cGraphics_Amiga>()->Service_Draw(7, 0xF0, 0x40);     //  Right Symbol
    }

    mService_ExitLoop = 0;
    mMouse_Exit_Loop = false;
    mGraphics->PaletteSet();
    mSurface->Save();

    mInterruptCallback = [this]() {
        Service_Draw_List();
        if (mInterruptTick % 2 == 0)
            Service_ScrollUp_DrawList();
        Service_Promotion_Check();
    };
    do {
        Video_Sleep();
        Video_Sleep();
        if (mService_Promotion_Exit_Loop == -1 || mMouse_Exit_Loop) {
            mMouse_Exit_Loop = false;
            mSurface->paletteNew_SetToBlack();
            mService_ExitLoop = 1;
        }

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

    } while (mSurface->isPaletteAdjusting() || mService_ExitLoop == 0);

    mInterruptCallback = nullptr;

loc_18001:;

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {
        Troop.Promote();
    }
}

int16 cFodder::Service_KIA_Troop_Prepare() {
    mService_Draw_List.clear();

    mVideo_Draw_PosY = 0xE8;
    Service_Mission_Text_Prepare();
    mVideo_Draw_PosY += 0x40;

    if (mGame_Data.mHeroes.empty() || mGame_Data.mGamePhase_Data.mHeroesCount == mGame_Data.mHeroes.size())
        return -1;

   
    for (size_t i = mGame_Data.mGamePhase_Data.mHeroesCount; i < mGame_Data.mHeroes.size(); ++i) {

        auto& Hero = mGame_Data.mHeroes[i];

        Service_Draw_Troop_And_Rank(Hero.mRecruitID, Hero.mRank);
        mVideo_Draw_PosY += 0x40;
    }

    return 0;
}

int16 cFodder::Service_Promotion_Prepare_Draw() {
    mService_Draw_List.clear();

    mVideo_Draw_PosY = 0xE8;
    Service_Mission_Text_Prepare();
    mVideo_Draw_PosY += 0x40;
    size_t Drawn = mService_Draw_List.size();

    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {

        if (Troop.mSprite != INVALID_SPRITE_PTR) {
            int16 ax = Troop.mRecruitID;
            int8 bl = Troop.mRank;

            bl &= 0xFF;

            Service_Draw_Troop_And_Rank(ax, bl);

            mVideo_Draw_PosY += 0x40;
        }
    }

    // No names drawn?
    if (Drawn == mService_Draw_List.size())
        return -1;

    return 0;
}

void cFodder::Service_Draw_String(const std::string& pText, const bool pLarge, const size_t pY) {

    const uint8* FontWidth = mFont_Briefing_Width;
    if (pLarge)
        FontWidth = mFont_ServiceName_Width;

    String_CalculateWidth(320, FontWidth, pText.c_str());
    Service_Draw_String(pText, FontWidth, pLarge ? 3 : 0, mGUI_Temp_X, (int16) pY);

}

void cFodder::Service_Draw_Troop_And_Rank(int16 pRecruitID, int16 pRank) {

    mService_Draw_List.emplace_back(PLATFORM_BASED(9, 0), PLATFORM_BASED(0, 1), 0x60, mVideo_Draw_PosY);
    mService_Draw_List.emplace_back(2, pRank, 0x64, mVideo_Draw_PosY + 4);
    mService_Draw_List.emplace_back(2, pRank, 0xCC, mVideo_Draw_PosY + 4);

    //seg003:3237
    const char* si = mRecruits[pRecruitID].mName;
    std::stringstream tmpString;

    for (int16 cx = 5; cx >= 0; --cx) {
        // Break on space
        if (*si == 0x20)
            break;

        tmpString << *si++;
    }

    Service_Draw_String(tmpString.str(), true, (size_t) mVideo_Draw_PosY + 6);
}

void cFodder::Service_Promotion_Check() {
    auto* Troop = &mGame_Data.mSoldiers_Allocated[0];

    if (!mService_Draw_List.size())
        return;

    for( size_t x = 0; x < mService_Draw_List.size() - 1; ++x) {
        auto Draw = &mService_Draw_List[x];

        if (Draw->mSpriteType != 2)
            continue;

        int16 newRank = Troop->GetPromotedRank();
        if (Troop->mRecruitID == -1) {
            ++Troop;
            --x;
            continue;
        }
        ++Troop;
        ++x;

        if (Draw->mY >= (getCameraHeight() - 8) / 2)
            continue;

        (Draw+0)->mFrame = newRank;
        (Draw+1)->mFrame = newRank;
    }
}

void cFodder::Service_Draw_List() {

    for(auto& Draw : mService_Draw_List) {

        if (Draw.mY < 8)
            continue;

        Service_Sprite_Draw(Draw.mSpriteType, Draw.mFrame, Draw.mX, Draw.mY);
    }
}

void cFodder::Service_ScrollUp_DrawList() {
    mService_Promotion_Exit_Loop = -1;

    for (auto& Draw : mService_Draw_List) {

        Draw.mY -= 1;

        if(Draw.mY >= -4)
            mService_Promotion_Exit_Loop = 0;

    }
}

void cFodder::Service_Draw_String(const std::string& pText, const uint8* pData28, int16 pData0, int16 pData8, int16 pDataC) {

    const char* Data20 = pText.c_str();

    for (;;) {
        int16 Data4 = *Data20++;
        if (Data4 == 0)
            break;

        int16 Data10 = Data4;

        if (mString_GapCharID) {

            if (Data4 == 0x20)
                goto loc_18229;

            Data4 = mString_GapCharID;
            goto loc_18259;
        }
        //loc_18222
        if (Data4 == 0x20)
            goto loc_1826D;
    loc_18229:;
        if (Data4 > 0x39)
            goto loc_1823C;

        Data4 -= 0x30;
        Data4 += 0x1A;
        goto loc_18259;
    loc_1823C:;
        if (Data4 > 0x5A) {
            Data4 -= 0x61;
            Data4 += 0x39;
        }
        else {
            Data4 -= 0x41;
            Data4 += 0;
        }

    loc_18259:;
        mService_Draw_List.emplace_back(pData0, Data4, pData8, pDataC);

    loc_1826D:;

        Data10 = pData28[Data10];
        pData8 += Data10;
    }
}

int16 cFodder::Service_Sprite_Draw(int16& pSpriteType, int16& pFrame, int16& pX, int16& pY) {
    auto SheetData = Sprite_Get_Sheet(pSpriteType, pFrame);

    mVideo_Draw_PaletteIndex = SheetData->mPalleteIndex & 0xFF;
    mVideo_Draw_FrameDataPtr = SheetData->GetGraphicsPtr();

    mVideo_Draw_PosX = pX + 0x10;
    mVideo_Draw_PosY = pY + 0x10;


    mVideo_Draw_Columns = SheetData->mColCount;
    mVideo_Draw_Rows = SheetData->mRowCount;

    if (!Service_Sprite_OnScreen_Check()) {
        if (mVersionCurrent->mPlatform == ePlatform::Amiga)
            mGraphics->Video_Draw_16(mFont_Service_PalleteIndex_Amiga);
        else
            mGraphics->Video_Draw_8(0, mFont_Service_PalleteIndex);

        return 1;
    }
    return 0;
}

int16 cFodder::Service_Sprite_OnScreen_Check() {
    int16 ax;

    if (mVideo_Draw_PosY < 0x2C) {
        ax = mVideo_Draw_PosY;
        ax += mVideo_Draw_Rows;
        --ax;
        if (ax < 0x2C)
            return -1;

        ax -= 0x2C;
        ax -= mVideo_Draw_Rows;
        ++ax;
        ax = -ax;
        mVideo_Draw_PosY += ax;
        mVideo_Draw_Rows -= ax;

        ax *= PLATFORM_BASED(160, 40);

        mVideo_Draw_FrameDataPtr += ax;
    }
    //loc_184FC

    ax = mVideo_Draw_PosY;
    ax += mVideo_Draw_Rows;
    --ax;

    if (ax <= 0xE7)
        return 0;

    if (mVideo_Draw_PosY > 0xE7)
        return -1;

    ax -= 0xE7;
    mVideo_Draw_Rows -= ax;
    return 0;
}

void cFodder::Service_Mission_Text_Prepare() {
    std::stringstream Mission;
    Mission << "MISSION ";

    Mission << tool_StripLeadingZero(std::to_string(mGame_Data.mMission_Number));

    String_CalculateWidth(320, mFont_Service_Width, Mission.str().c_str());

    Service_Draw_String(Mission.str(), mFont_Service_Width, 4, mGUI_Temp_X, mVideo_Draw_PosY);
}
