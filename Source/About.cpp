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

#include "stdafx.hpp"
#include "gitver.hpp"

static std::string compiled = std::string(__DATE__) + " AT " + std::string(__TIME__);
static std::string version = "" + std::string(gitversion);

cAbout::cAbout() {
    std::transform(compiled.begin(), compiled.end(), compiled.begin(), ::toupper);
    std::transform(version.begin(), version.end(), version.begin(), ::toupper);
    std::replace(compiled.begin(), compiled.end(), ':', ' ');

    std::vector<sIntroString> AboutText = {
        { 250, "BUILD VERSION"},
        { 260, version.c_str()},
        { 270, compiled.c_str()},
        { 300, "AUTHORS" },
        { 310, "ROBERT CROSSFIELD" },
        { 320, "ALESSANDRO PETRALIA" },
        { 360, "THANKS TO" },
        { 370, "SENSIBLE SOFTWARE"},
        { 380, ""},

    };

    mSurface = new cSurface(0, 0);
    mSurface->LoadBitmap(local_PathGenerate("about.bmp", "", eDataType::eRoot).c_str());
    g_Fodder->mGraphics->PaletteSet();
    g_Fodder->Mission_Memory_Clear();
    g_Fodder->mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    g_Fodder->mService_Draw_List.clear();

    g_Fodder->mGraphics->Load_Service_Data();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_SERVICE);
    g_Fodder->mGraphics->PaletteSet();


    for (auto& Text : AboutText) {
        g_Fodder->Service_Draw_String(Text.mText, false, Text.mPosition);
    }

    g_Fodder->mSound->Music_Play(PLATFORM_BASED(16, CANNON_BASED(16, 20)));
}

cAbout::~cAbout() {

    delete mSurface;
}

bool cAbout::Cycle() {
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mSurface->palette_FadeTowardNew();
    g_Fodder->mSurface->clearBuffer();

    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->Service_Draw_List();
    g_Fodder->Service_ScrollUp_DrawList();

    {
        g_Fodder->mString_GapCharID = 0x25;
        g_Fodder->String_Print_Large("Open Fodder", true, 0x01);
        g_Fodder->mString_GapCharID = 0x00;

        g_Fodder->GUI_Button_Draw_Small("BACK", 0xD3);
        g_Fodder->GUI_Button_Setup_Small(&cFodder::GUI_Button_Load_Exit);
    }

    g_Fodder->Mouse_Inputs_Get();
    g_Fodder->Mouse_DrawCursor();

    if (g_Fodder->mPhase_Aborted)
        g_Fodder->GUI_Button_Load_Exit();

    if (g_Fodder->mMouse_Button_Left_Toggle) {
        g_Fodder->GUI_Handle_Element_Mouse_Check(g_Fodder->mGUI_Elements);

        if (g_Fodder->mGUI_SaveLoadAction == 1) {

            g_Fodder->mSurface->paletteNew_SetToBlack();
            g_Fodder->mSurface->palette_FadeTowardNew();
        }

    }

    g_Fodder->mSurface->draw();
    g_Window->RenderShrunk(mSurface);
    
    if (g_Fodder->mGUI_SaveLoadAction == 1 && !g_Fodder->mSurface->isPaletteAdjusting())
        return false;

    return true;
}
