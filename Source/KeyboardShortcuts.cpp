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

struct sShortcutLine {
    size_t mY;
    const char* mKey;
    const char* mAction;
};

static void KeyboardShortcuts_DrawHeading(const char* pText, size_t pX, size_t pY)
{
    g_Fodder->String_Print_Small(pText, pX, pY);
}

static void KeyboardShortcuts_DrawLine(size_t pX, const sShortcutLine& pLine)
{
    g_Fodder->String_Print_Small_LeftInBox(pLine.mKey, pX, pX + 0x28, pLine.mY, 0);
    g_Fodder->String_Print_Small_LeftInBox(pLine.mAction, pX + 0x2E, pX + 0x86, pLine.mY, 0);
}

cKeyboardShortcuts::cKeyboardShortcuts()
{
    mSurface = new cSurface(0, 0);
    mSurface->LoadPng(g_ResourceMan->GetAboutFile());

    g_Fodder->mGUI_SaveLoadAction = 0;
    g_Fodder->mMouse_Button_Left_Toggle = 0;
    g_Fodder->mPhase_Aborted = false;
    g_Fodder->mGraphics->PaletteSet();
    g_Fodder->mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->mGraphics->PaletteSet();
}

cKeyboardShortcuts::~cKeyboardShortcuts()
{
    delete mSurface;
}

bool cKeyboardShortcuts::Cycle()
{
    static const sShortcutLine SystemLines[] = {
        { 0x46, "F1", "AMIGA VERSION" },
        { 0x54, "F2", "PC VERSION" },
        { 0x62, "F11", "FULLSCREEN" },
        { 0x70, "F12", "MOUSE LOCK" },
        { 0x7E, "PLUS", "WINDOW LARGER" },
        { 0x8C, "MINUS", "WINDOW SMALLER" },
        { 0x9A, "ESC", "BACK OR ABORT" }
    };

    static const sShortcutLine MissionLines[] = {
        { 0x46, "P", "PAUSE" },
        { 0x54, "M", "MAP" },
        { 0x62, "SPACE", "SWITCH WEAPON" },
        { 0x70, "1 2 3", "SELECT SQUAD" },
        { 0x8C, "F5", "AUTOSAVE CHEATS" },
        { 0x9A, "F9", "LOAD OR INVINCIBLE" },
        { 0xA8, "F10", "COMPLETE MISSION" }
    };

    g_Fodder->GUI_Element_Reset();

    g_Fodder->mSurface->palette_FadeTowardNew();
    g_Fodder->mSurface->clearBuffer();

    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("Keyboard Shortcuts", true, 0x01);
    g_Fodder->mString_GapCharID = 0x00;

    g_Fodder->Briefing_DrawBox(0x08, 0x2A, 0x88, 0x84, 0xF3);
    g_Fodder->Briefing_DrawBox(0x07, 0x29, 0x88, 0x84, 0xF2);
    g_Fodder->Briefing_DrawBox(0x9A, 0x2A, 0x98, 0x84, 0xF3);
    g_Fodder->Briefing_DrawBox(0x99, 0x29, 0x98, 0x84, 0xF2);

    KeyboardShortcuts_DrawHeading("SYSTEM", 0x30, 0x34);
    for (const auto& Line : SystemLines)
        KeyboardShortcuts_DrawLine(0x18, Line);

    KeyboardShortcuts_DrawHeading("MISSION", 0xC0, 0x34);
    for (const auto& Line : MissionLines)
        KeyboardShortcuts_DrawLine(0xA8, Line);

    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + PLATFORM_BASED(0, 25));
    g_Fodder->GUI_Button_Setup(&cFodder::GUI_Button_Load_Exit);

    if (g_Fodder->mPhase_Aborted)
        g_Fodder->GUI_Button_Load_Exit();

    if (g_Fodder->mMouse_Button_Left_Toggle) {
        g_Fodder->GUI_Handle_Element_Mouse_Check(g_Fodder->mGUI_Elements);

        if (g_Fodder->mGUI_SaveLoadAction == 1) {
            g_Fodder->mSurface->paletteNew_SetToBlack();
            g_Fodder->mSurface->palette_FadeTowardNew();
        }
    }

    g_Fodder->Mouse_DrawCursor();

    g_Fodder->mSurface->draw();
    g_Fodder->Video_Sleep(mSurface, true);
    g_Fodder->mWindow->RenderAt(g_Fodder->mSurface);

    if (g_Fodder->mGUI_SaveLoadAction == 1 && !g_Fodder->mSurface->isPaletteAdjusting())
        return false;

    return true;
}
