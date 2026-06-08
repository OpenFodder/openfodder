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


// Campaign Selection screen tile data
static std::vector<unsigned char> mCampaignSelectMap_Jungle = {
    0x18, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0xd1, 0x00, 0x6a, 0x01, 0x30, 0x01, 0x6b, 0x01, 0x8b,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x08, 0x01, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x4d, 0x01, 0x4e, 0x01, 0x7c, 0x00, 0x19, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01,
    0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1b, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x10, 0x10, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x60, 0x01, 0x61, 0x01, 0x62, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0xf3, 0x00, 0x5a, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1b,
    0x01, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x74, 0x01, 0x75, 0x01, 0x76, 0x01, 0x77, 0x01, 0x7b, 0x00, 0x0a, 0x01, 0x5a, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1a, 0x01, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00,
    0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x88, 0x01, 0x89, 0x01, 0x8a, 0x01, 0x10, 0x10, 0x00, 0x00, 0x32, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf4, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b,
    0x00, 0x7b, 0x00, 0x7b, 0x00, 0xf0, 0x00, 0x05, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x06, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0xf1, 0x00, 0x8b, 0x01,
    0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf2, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x05, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7c, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0xf6, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x10, 0x10, 0xf1, 0x00, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x06, 0x01,
    0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x8c, 0x00, 0x0a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b,
    0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x6f, 0x01, 0x0a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1d, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x10, 0x10, 0x7b, 0x00, 0xf0, 0x00,
    0x05, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf2, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x36, 0x01, 0x37, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x68, 0x01, 0x69, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x69, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x08, 0x01,
    0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00
};

static std::vector<unsigned char> mCampaignSelectMap_AF = {
    0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5b, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x3d, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72,
    0x00, 0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x29, 0x00, 0x14, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x83, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x72, 0x00, 0x71, 0x00, 0x14, 0x00, 0x14, 0x00, 0x9a, 0x00, 0x9b, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5d, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x0c, 
    0x00, 0xae, 0x00, 0xaf, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5c, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x14, 0x00, 0x0b, 0x00, 0xc2, 0x00, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5c, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x28, 0x00, 0x14, 0x00, 0x14, 0x00, 0x07, 0x00, 0x07, 0x00, 0x04, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 
    0x00, 0x14, 0x00, 0x70, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x94, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x97, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x56, 0x00, 0x3d, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x52, 0x00, 0x53, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 
    0x00, 0x59, 0x00, 0x14, 0x10, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x14, 0x10, 0x14, 0x00, 
    0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 
    0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x73, 0x00, 0x73, 0x00, 0x73, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x80, 0x00, 0x14, 0x00, 0x0c, 0x00, 0x11, 0x00, 0x11, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x14, 0x00, 0x82, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x71, 0x00, 0x14, 0x00, 0x18, 0x00, 0x23, 0x00, 0x24, 0x00, 0x16, 0x00, 0x14, 0x00, 0x14, 0x00, 0x96, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 
    0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x94, 0x00, 0x14, 0x00, 0x0b, 0x00, 0x07, 0x00, 0x07, 0x00, 0x04, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x64, 0x00, 0x64, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
    0x10, 0x10, 0x0b, 0x00, 0x10, 0x10, 0x07, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
};

bool cFodder::Campaign_Load(std::string pName) {

    // If no campaign name was provided, use the default for the current version
    if (!pName.size()) {

        if (!mVersionCurrent->isCustom()) {
            pName = mVersionCurrent->mName;
        }
    }

    VersionSwitch(mVersions->GetForCampaign(pName, mParams->mDefaultPlatform));
    if (!mGame_Data.mCampaign.LoadCampaign(pName, pName != mVersionCurrent->mName)) {
        // TODO: But what?

        return false;
    }

    return true;
}

// Campaign-select layout helpers.
//
// Pre-cleanup this screen used the briefing (large) font for everything —
// campaign-name buttons centred at full screen width, side-buttons (KEYS /
// MULTIPLAYER / OPTIONS / ABOUT) at fixed X=0xA. Long campaign names like
// "Cannon Fodder 2" centred at 320 px overlapped the side-button column on
// the rows where their Y bands met, producing the "background text being
// overwritten" effect.
//
// New layout: campaign list and side-buttons render in the small pstuff
// sidebar font (Source/SidebarFontGlyphs.cpp + MainScreen_Print_Sidebar).
// The list lives in a centred column from X=0x40..0xF0 — geometrically
// disjoint from the side-button column at X=8..0x38 — and rows are 14 px
// tall instead of 21, so 8 campaigns fit per page instead of 4.
namespace {
constexpr int16 kCampaignSideX        = 0x08;     // left-aligned column
constexpr int16 kCampaignSideWidth    = 0x30;     // 48 px — fits "MULTIPLAYER"
constexpr int16 kCampaignListX1       = 0x40;
constexpr int16 kCampaignListX2       = 0xF0;
constexpr int16 kCampaignListRowY0    = 0x40;
constexpr int16 kCampaignListRowH     = 0x0E;     // 14 px (was 21)
constexpr int16 kCampaignSideRowH     = 0x10;
}

void cFodder::Campaign_Select_DrawSideButton(const char* pLabel, int16 pY,
                                              void (cFodder::*pHandler)())
{
    // Tight box around a sidebar-font label. Width auto-fits the longest
    // label so KEYS (4 chars) and MULTIPLAYER (11 chars) share the column
    // edge but don't waste space.
    const int16 textWidth = (int16)MainScreen_MeasureSidebarWidth(pLabel);
    const int16 boxW      = (int16)kCampaignSideWidth;
    const int16 textX     = (int16)(kCampaignSideX + ((boxW - textWidth) / 2));

    MainScreen_Print_Sidebar(pLabel, (size_t)textX, (size_t)pY,
                             eSidebarFontColor::Normal);

    mGUI_Temp_X = kCampaignSideX;
    mGUI_Temp_Y = pY;
    mGUI_Temp_Width = boxW;
    mGUI_Draw_LastHeight = 8;
    GUI_Box_Draw(0xB2, 0xB3);
    GUI_Button_Setup(pHandler);
}

void cFodder::Campaign_Select_DrawMenu(const char* pTitle, const char* pSubTitle) {
    size_t YOffset = PLATFORM_BASED(0, 25);

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    GUI_Element_Reset();

    mString_GapCharID = 0x25;
    String_Print_Large(pTitle, true, 0x01);
    mString_GapCharID = 0x00;

#ifdef OPENFODDER_ENABLE_NETWORK
    if (mStartParams->mNetworkEnabled)
        String_Print_Large("MULTIPLAYER", false, 0x18);
    else
#endif
    String_Print_Large(pSubTitle, false, 0x18);

    // ===== Top / bottom navigation (briefing font, full-width centred). =====
    if (mGUI_Select_File_Count != mGUI_Select_File_ShownItems) {
        GUI_Button_Draw_Small("UP", 0x30);
        GUI_Button_Setup(&cFodder::GUI_Button_Load_Up);

        GUI_Button_Draw_Small("DOWN", 0x99 + YOffset);
        GUI_Button_Setup(&cFodder::GUI_Button_Load_Down);
    }

    GUI_Button_Draw_Small("EXIT", 0xB3 + YOffset);
    GUI_Button_Setup(&cFodder::GUI_Button_Load_Exit);

    // ===== Side-button column (sidebar font, narrow). =====
    // KEYS / [MULTIPLAYER] / OPTIONS / ABOUT live in a column that's
    // explicitly disjoint from the campaign list (X 0x08..0x38 vs
    // 0x40..0xF0) so long campaign names can't overlap them.
    int16 sideY = 0x6E + (int16)YOffset;
    Campaign_Select_DrawSideButton("KEYS", sideY, &cFodder::GUI_Button_Show_Shortcuts);
    sideY += kCampaignSideRowH;
#ifdef OPENFODDER_ENABLE_NETWORK
    Campaign_Select_DrawSideButton("MULTIPLAYER", sideY, &cFodder::GUI_Button_Show_Multiplayer);
    sideY += kCampaignSideRowH;
#endif
    Campaign_Select_DrawSideButton("OPTIONS", sideY, &cFodder::GUI_Button_Show_Options);
    sideY += kCampaignSideRowH;
    Campaign_Select_DrawSideButton("ABOUT", sideY, &cFodder::GUI_Button_Show_About);

    // ===== Campaign list (sidebar font, centred in its column). =====
    int16 ItemCount = 0;
    auto FileIT = mCampaignList.begin() + mGUI_Select_File_CurrentIndex;
    for (; ItemCount < mGUI_Select_File_ShownItems && FileIT != mCampaignList.end(); ++ItemCount) {
        const int16 rowY = (int16)(kCampaignListRowY0 + (ItemCount * kCampaignListRowH));
        const std::string& Name = *FileIT;

        // Centre the name inside the campaign-list column.
        MainScreen_Print_Sidebar_CentreInBox(Name,
                                              (size_t)kCampaignListX1,
                                              (size_t)kCampaignListX2,
                                              (size_t)rowY,
                                              eSidebarFontColor::Normal);

        // Frame the row so it's clickable as a unit. Match the row height to
        // the sidebar-font glyph height plus a small padding.
        mGUI_Temp_X = kCampaignListX1;
        mGUI_Temp_Y = rowY;
        mGUI_Temp_Width = (int16)(kCampaignListX2 - kCampaignListX1);
        mGUI_Draw_LastHeight = 8;
        GUI_Box_Draw(0xB2, 0xB3);
        GUI_Button_Setup(&cFodder::GUI_Button_Filename);

        ++FileIT;
    }
}

void cFodder::Campaign_Select_Setup() {
	mCampaignList.clear();

	mPhase_In_Progress = true;
	mPhase_Aborted = false;
	mGUI_SaveLoadAction = 0;

	{
		for (auto& Name : mVersions->GetCampaignNames()) {

			if (g_ResourceMan->isCampaignAvailable(Name) || Name == "Single Map" || Name == "Random Map")
				mCampaignList.push_back(Name);
		}
	}

	{
		auto Files = g_ResourceMan->GetCampaigns();

		// Append all custom campaigns to the list
		for (auto& File : Files) {
			size_t Pos = File.find_first_of(".");
			std::string FileName = File.substr(0, Pos);

			// Don't add known campaigns
			if (mVersions->isCampaignKnown(FileName))
				continue;

			mCampaignList.push_back(FileName);
		}
	}

	mGUI_Select_File_CurrentIndex = 0;
	mGUI_Select_File_Count = (int16)mCampaignList.size();



	// Create the title screen depending on which data is loaded
	if (mVersionCurrent->isRetail()) {
		sMapParams Params(0x15, 0x0F, eTileTypes_Jungle, eTileSub_0);
		Map_Create(Params);
		std::memcpy(mMap->data() + 0x60, mCampaignSelectMap_Jungle.data(), mMap->size() - 0x60);
	}
	else {
		sMapParams Params(0x15, 0x0F, eTileTypes_AFX, eTileSub_0);
		Map_Create(Params);
		std::memcpy(mMap->data() + 0x60, mCampaignSelectMap_AF.data(), mMap->size() - 0x60);
	}

	Campaign_Select_Sprite_Prepare();
    Music_Play(mMapLoaded->getTileType() + 0x32, 4);
    mSound->Music_SetVolume(-1, 0x10);

	if (mGUI_SaveLoadAction != 3) {
		mSurface->palette_FadeTowardNew();
		Mouse_Setup();
	}
	mGUI_SaveLoadAction = 0;

	mGraphics->PaletteSet();
	mSurface->Save();

	mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
	mDemo_ExitMenu = 0;

	Camera_Reset();

}

std::string cFodder::Campaign_Select_File(const char* pTitle, const char* pSubTitle, const char* pPath, const char* pType, eDataType pData) {

	Campaign_Select_Setup();
    MapTiles_ResetScrollState();

    mInterruptCallback = [this, pTitle, pSubTitle]() {
        if (!mStartParams->mDisableVideo) {
            mGraphics->MapTiles_Draw();
        }
        Sprites_Draw();
        Campaign_Select_DrawMenu(pTitle, pSubTitle);
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);

        Mouse_DrawCursor();
    };

    bool SelectionCompleted = false;
#ifdef OPENFODDER_ENABLE_NETWORK
    if (auto* mp = dynamic_cast<cFodderMultiplayer*>(this)) {
        if (mp->ConsumeReturnToMultiplayerLobby()) {
            if (mp->Multiplayer_ReopenLobby()) {
                mGUI_SaveLoadAction = 2;
                SelectionCompleted = true;
            }
            else {
                mGUI_SaveLoadAction = 6;
            }
        }

        if (!SelectionCompleted && mStartParams->mNetworkMenuStart != eNetworkMenuStart_None) {
            if (mp->Multiplayer_Menu_Run()) {
                if (mp->mLobby) {
                    mp->Lobby_CampaignSelection();

                    if (mStartParams->mNetworkEnabled) {
                        mGUI_SaveLoadAction = 2;
                        SelectionCompleted = true;
                    }
                }
                else {
                    mGUI_SaveLoadAction = 0;
                    mMouse_Button_Left_Toggle = 0;
                    mGraphics->PaletteSet();
                    mSurface->palette_FadeTowardNew();
                    mSurface->Save();
                }
            }

            if (!SelectionCompleted) {
                mStartParams->mNetworkEnabled = false;
                mGUI_SaveLoadAction = 0;
                mMouse_Button_Left_Toggle = 0;
                mGraphics->PaletteSet();
                mSurface->palette_FadeTowardNew();
                mSurface->Save();
            }
        }
    }
#endif

    while (!SelectionCompleted) {

        Campaign_Select_File_Loop(pTitle, pSubTitle);

        if (mGUI_SaveLoadAction != 3)
            break;
    }


    mInterruptCallback = nullptr;
    mPhase_In_Progress = false;

    if (mGUI_SaveLoadAction == 1)
        return "";

	return mCampaignList[mGUI_Select_File_CurrentIndex + mGUI_Select_File_SelectedFileIndex];
}

void cFodder::Campaign_Selection() {
    mPhase_Complete = false;

    Phase_EngineReset();
    GameData_Reset();
    mMap_Destroy_Tiles.clear();

    Image_FadeOut();

    mGraphics->PaletteSet();

    mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    mMouseX_Offset = -8;
    mMouseY_Offset = -8;

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    mCustom_Mode = eCustomMode_None;

    std::string CampaignFile;

SelectCampaign:;

    CampaignFile = Campaign_Select_File("OPEN FODDER", "SELECT CAMPAIGN", "", "*.ofc", eDataType::eCampaign);

    // Exit Pressed?
    if (mGUI_SaveLoadAction == 1 || mGUI_SaveLoadAction == 4 || !CampaignFile.size()) {

        // Return to custom menu
        mDemo_ExitMenu = 1;
        mCustom_Mode = eCustomMode_None;

        return;
    }

    // Find a data version to use with this campaign
    // If no version is found, we use the currently loaded one
    {
        const sGameVersion* Version = mVersions->GetForCampaign(CampaignFile, mVersionCurrent->mPlatform);

        if (!Version) {
            Version = mVersions->GetForCampaign(CampaignFile);
        }

        // Load a new version?
        if (Version && Version != mVersionCurrent) {
            VersionSwitch(Version);
        }

        // Set the default/starting version
        mVersionDefault = mVersionCurrent;

        // Single Map Mode?
        if (CampaignFile == "Single Map" || CampaignFile == "Random Map") {

            if (CampaignFile == "Random Map") {
                if (!RandomMapOptions_RunCampaign()) {
                    mGUI_SaveLoadAction = 0;
                    goto SelectCampaign;
                }
            }

            mGame_Data.mCampaign.SetSingleMapCampaign();
            mCustom_Mode = eCustomMode_Map;
            return;

            // If no version, it must be a custom campaign
        }
        else if (!Version) {

			// If version is currently XMAS, it means no retail is available
			if (mVersionCurrent->isAmigaXmas()) {

				VersionSwitch(mVersions->GetForCampaign("Amiga Action"));

				// Set the default/starting version
				mVersionDefault = mVersions->GetForCampaign("Amiga Action");
			}

            mCustom_Mode = eCustomMode_Set;
        }
    }

    // Load the campaign
    if (Campaign_Load(CampaignFile) == true) {

        // 
        if (mVersionCurrent->isCustom()) {
            mDemo_ExitMenu = 1;
            mCustom_ExitMenu = 1;
        }

        WindowTitleBaseSetup();
        return;
    }

    mDemo_ExitMenu = 1;
    mCustom_Mode = eCustomMode_None;
}

void cFodder::Campaign_Select_Sprite_Prepare() {

    int16 x = 0;

    Sprite_Clear_All();

    Phase_SquadPrepare();

    mSquad_CurrentVehicle = &mSprites[x];
    mSprites[x].mPosX = 0xe6;
    mSprites[x].mPosY = 0xcd;
    mSprites[x].mSheetIndex = 0xD2;
    mSprites[x].mFrameIndex = 5;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x].mSpriteType = eSprite_Turret_Missile_Human;
    mSprites[x++].mVehicleType = eVehicle_Turret_Missile;

    mSprites[x].mPosX = tool_RandomGet() & 0xFF;
    mSprites[x].mPosY = tool_RandomGet() & 0xff;
    mSprites[x].mSheetIndex = 6;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Civilian_Spear;

    mSprites[x].mPosX = 0xff;
    mSprites[x].mPosY = 16 + (tool_RandomGet() % 0x60);
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Bird_Left;

    mSprites[x].mPosX = 0;
    mSprites[x].mPosY = 16 + (tool_RandomGet() % 0xc0);
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Bird_Left;

    mSprites[x].mPosX = 185;
    mSprites[x].mPosY = 19;
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Hostage;

    mSprites[x].mPosX = 100;
    mSprites[x].mPosY = 0xd0;
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Floating_Dead_Soldier;

    mSprites[x].mPosX = 220;
    mSprites[x].mPosY = 60;
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_BoilingPot;

    mSprites[x].mPosX = 220;
    mSprites[x].mPosY = 60;
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Null;

    mSquad_Leader = mSquad_CurrentVehicle;

    word_3AA1D = word_3BED5[0];

    mSoundDisabled = true;
    if(mMapLoaded->getTileType() == eTileTypes_Jungle)
        Map_Add_Structure(mStructuresBarracksWithSoldier[mMapLoaded->getTileType()], 4, 2);

    if(mMapLoaded->getTileType() == eTileTypes_AFX)
        Map_Add_Structure(mStructuresBarracksWithSoldier[mMapLoaded->getTileType()], 2, 5);
    mSoundDisabled = false;
}

void cFodder::Campaign_Select_File_Cycle(const char* pTitle, const char* pSubTitle) {
	static int16 Timedown = 0;

    if (mPhase_InterruptTicks >= 3) {
        mPhase_InterruptTicks = 0;
        Mission_Sprites_Handle();
        Sound_Tick();
        if(mInterruptCallback)
            mInterruptCallback();
    }

	if (mSurface->isPaletteAdjusting())
		mSurface->palette_FadeTowardNew();


	if (Timedown)
		--Timedown;

	if (mMouse_Button_Left_Toggle && !Timedown) {
		Vehicle_Input_Handle();

		mMouse_Button_Left_Toggle = 0;
		mSprites[0].mWeaponCooldown = -1;
		mSprites[0].mWeaponTargetX = mSquad_Leader->mTargetX + 10;
		mSprites[0].mWeaponTargetY = mSquad_Leader->mTargetY - 18;

		Timedown = 10;
	}

	if (mPhase_Aborted)
		GUI_Button_Load_Exit();

	if (mMouse_Button_Left_Toggle)
		GUI_Handle_Element_Mouse_Check(mGUI_Elements);

    if (mGUI_SaveLoadAction == 5) {
        Options_Menu_Run();
        mGUI_SaveLoadAction = 0;
        mMouse_Button_Left_Toggle = 0;
        mGraphics->PaletteSet();
        mSurface->palette_FadeTowardNew();
        mSurface->Save();
    }

    if (mGUI_SaveLoadAction == GUI_MENU_SHORTCUTS) {
        KeyboardShortcuts();
        mGUI_SaveLoadAction = 0;
        mMouse_Button_Left_Toggle = 0;
        mGraphics->PaletteSet();
        mSurface->palette_FadeTowardNew();
        mSurface->Save();
    }

#ifdef OPENFODDER_ENABLE_NETWORK
    if (mGUI_SaveLoadAction == 6) {
        auto* mp = dynamic_cast<cFodderMultiplayer*>(this);
        if (mp && mp->Multiplayer_Menu_Run()) {

            if (mp->mLobby) {
                // Network mode with lobby — enter lobby campaign selection
                mp->Lobby_CampaignSelection();

                if (mStartParams->mNetworkEnabled) {
                    // Campaign selected, both players agreed — exit with selection
                    mGUI_SaveLoadAction = 2;
                    return;
                }
            } else {
                // Sync test mode — return to normal campaign selection
                // with network enabled, user picks campaign normally
                mGUI_SaveLoadAction = 0;
                mMouse_Button_Left_Toggle = 0;
                mGraphics->PaletteSet();
                mSurface->palette_FadeTowardNew();
                mSurface->Save();
                return;
            }
        }

        // Cancelled or failed — return to campaign selection
        mStartParams->mNetworkEnabled = false;
        mGUI_SaveLoadAction = 0;
        mMouse_Button_Left_Toggle = 0;
        mGraphics->PaletteSet();
        mSurface->palette_FadeTowardNew();
        mSurface->Save();
    }
#endif

	GUI_Button_Load_MouseWheel();
}

void cFodder::Campaign_Select_File_Loop(const char* pTitle, const char* pSubTitle) {

    do {
		Campaign_Select_File_Cycle(pTitle, pSubTitle);
        Video_Sleep();

    } while (mGUI_SaveLoadAction <= 0);

    mPhase_Aborted = false;

    if (mGUI_SaveLoadAction == 3)
        return;

    Image_FadeOut();
}
