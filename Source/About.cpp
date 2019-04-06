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
static std::string version = "VERSION " + std::string(gitversion);

const size_t TEXTPOS_BUILD    = 250;
const size_t TEXTPOS_TEAM     = 400;
const size_t TEXTPOS_THANKS   = 550;
const size_t TEXTPOS_POWERED  = 660;

cAbout::cAbout() {
    std::transform(compiled.begin(), compiled.end(), compiled.begin(), ::toupper);
    std::transform(version.begin(), version.end(), version.begin(), ::toupper);
    std::replace(compiled.begin(), compiled.end(), ':', ' ');

    std::vector<sIntroString> AboutText = {
        { TEXTPOS_BUILD + 0 , "OPEN FODDER"},
        //{ TEXTPOS_BUILD + 10 , "BUILD VERSION "},
        { TEXTPOS_BUILD + 10, version.c_str()},
        { TEXTPOS_BUILD + 20, compiled.c_str()},
        { TEXTPOS_BUILD + 30, "WWW DOT OPENFODDER DOT COM"},

        { TEXTPOS_TEAM + 0 ,  "THE TEAM" },
        { TEXTPOS_TEAM + 20 , "PROJECT LEAD" },
        { TEXTPOS_TEAM + 30,  "ROBERT CROSSFIELD" },

        { TEXTPOS_TEAM + 80, "ENGINE" },
        { TEXTPOS_TEAM + 90, "ALESSANDRO PETRALIA" },

		{ TEXTPOS_TEAM + 110, "SCRIPTING" },
		{ TEXTPOS_TEAM + 120, "STARWINDZ" },


        { TEXTPOS_THANKS + 0 , "THANKS TO" },
        { TEXTPOS_THANKS + 10, "SENSIBLE SOFTWARE"},
		{ TEXTPOS_THANKS + 20, "STOO CAMBRIDGE"},
        { TEXTPOS_THANKS + 30, "CLAIRE CROSSFIELD"},
        { TEXTPOS_THANKS + 40, "LACHIE CROSSFIELD"},
        { TEXTPOS_THANKS + 50, "RILEY ELLETT"},
        { TEXTPOS_THANKS + 60, "SCUMMVM"},

        { TEXTPOS_POWERED + 0,  "POWERED BY"},
        { TEXTPOS_POWERED + 10, "CPP14"},
        { TEXTPOS_POWERED + 20, "SDL2"},
		{ TEXTPOS_POWERED + 30, "DUKTAPE"},
		{ TEXTPOS_POWERED + 40, "DUKGLUE"},

        // Just for fun: loc_2B48E
        { 1100, "PUSH CX"},
        { 1110, "PUSH SI"},
        { 1120, "PUSH DI"},
        { 1130, "MOV CX 16H"},

        { 1150, "MOV BX ES SI"},
        { 1160, "AND BX 1FFH"},
        { 1170, "PUSH CS"},
        { 1180, "CALL SUB 2B4B8"},
        { 1190, "ADD SI 2"},

        { 1500, "W W W  DOT  O P E N F O D D E R  DOT  C O M"}
    };

    mSurface = new cSurface(0, 0);
    mSurface->LoadBitmap(g_ResourceMan->GetAboutFile());
    g_Fodder->mGraphics->PaletteSet();
    g_Fodder->Phase_EngineReset();
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

        g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + PLATFORM_BASED(0, 25));
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
