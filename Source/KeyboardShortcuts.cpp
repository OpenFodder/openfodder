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

#include <cctype>

struct sShortcutLine {
    size_t mY;
    const char* mKey;
    const char* mAction;
};

static std::string KeyboardShortcuts_FitText(const char* pText, int pMaxPx)
{
    std::string Result;
    int Width = 0;

    if (!pText || pMaxPx <= 0)
        return Result;

    for (const char* Text = pText; *Text; ++Text)
    {
        const unsigned char Char = (unsigned char)std::toupper((unsigned char)*Text);
        const int CharWidth = (int)mFont_Briefing_Width[Char];

        if (Width + CharWidth > pMaxPx)
            break;

        Result.push_back((char)Char);
        Width += CharWidth;
    }

    while (!Result.empty() && Result.back() == ' ')
        Result.pop_back();

    return Result;
}

static void KeyboardShortcuts_PrintLeftInBox(const char* pText, size_t pX1, size_t pX2, size_t pY)
{
    const int MaxPx = (int)pX2 - (int)pX1;
    g_Fodder->String_Print_Small_LeftInBox(KeyboardShortcuts_FitText(pText, MaxPx), pX1, pX2, pY, 0);
}

static void KeyboardShortcuts_DrawHeading(const char* pText, size_t pX1, size_t pX2, size_t pY)
{
    g_Fodder->String_Print_Small_CentreInBox(pText, pX1, pX2, pY);
}

static void KeyboardShortcuts_DrawLine(size_t pKeyX, size_t pActionX, size_t pRightX, const sShortcutLine& pLine)
{
    KeyboardShortcuts_PrintLeftInBox(pLine.mKey, pKeyX, pActionX - 0x08, pLine.mY);
    KeyboardShortcuts_PrintLeftInBox(pLine.mAction, pActionX, pRightX, pLine.mY);
}

cKeyboardShortcuts::cKeyboardShortcuts()
{
    mSurface = new cSurface(g_Fodder->mWindow->GetScreenSize());
    if (mSurface->GetTexture())
        SDL_SetTextureBlendMode(mSurface->GetTexture(), SDL_BLENDMODE_NONE);

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
        { 0x46, "F1", "AMIGA" },
        { 0x54, "F2", "PC" },
        { 0x62, "F11", "FULLSCREEN" },
        { 0x70, "F12", "MOUSE LOCK" },
        { 0x7E, "PLUS", "SIZE UP" },
        { 0x8C, "MINUS", "SIZE DOWN" },
        { 0x9A, "ESC", "BACK ABORT" }
    };

    static const sShortcutLine MissionLines[] = {
        { 0x46, "P", "PAUSE" },
        { 0x54, "M", "MAP" },
        { 0x62, "SPACE", "WEAPON" },
        { 0x70, "1 2 3", "SQUAD" },
        { 0x8C, "F5", "AUTOSAVE" },
        { 0x9A, "F9", "LOAD INVINC" },
        { 0xA8, "F10", "WIN MISSION" }
    };

    g_Fodder->GUI_Element_Reset();

    g_Fodder->mSurface->palette_FadeTowardNew();
    g_Fodder->mSurface->clearBuffer();

    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("Keyboard Shortcuts", true, 0x01);
    g_Fodder->mString_GapCharID = 0x00;

    g_Fodder->Briefing_DrawBox(0x04, 0x2A, 0x98, 0x84, 0xF3);
    g_Fodder->Briefing_DrawBox(0x03, 0x29, 0x98, 0x84, 0xF2);
    g_Fodder->Briefing_DrawBox(0xA4, 0x2A, 0x98, 0x84, 0xF3);
    g_Fodder->Briefing_DrawBox(0xA3, 0x29, 0x98, 0x84, 0xF2);

    KeyboardShortcuts_DrawHeading("SYSTEM", 0x04, 0x9C, 0x34);
    for (const auto& Line : SystemLines)
        KeyboardShortcuts_DrawLine(0x0C, 0x3D, 0x94, Line);

    KeyboardShortcuts_DrawHeading("MISSION", 0xA4, 0x13C, 0x34);
    for (const auto& Line : MissionLines)
        KeyboardShortcuts_DrawLine(0xAC, 0xDD, 0x134, Line);

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
