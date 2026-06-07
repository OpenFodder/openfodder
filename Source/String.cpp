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
#include "FontData_Special.hpp"

void cFodder::String_Input_Print(int16 pPosY) {
    GUI_Input_CheckKey();

    GUI_Render_Text_Centred(mInput.c_str(), pPosY);

    if (mKeyCodeAscii == 0x0D && mInput.size())
        mGUI_SaveLoadAction = 2;

    if (mKeyCodeAscii == 8)
        goto loc_2E675;

    // Is Number
    if (mKeyCodeAscii >= 0x30 && mKeyCodeAscii <= 0x39)
        goto loc_2E636;

    if (mKeyCodeAscii < 0x41 || mKeyCodeAscii > 0x5A)
        goto loc_2E6A4;

loc_2E636:;
    if (mInput.size() >= 12)
        goto loc_2E6A4;

    mInput.push_back((char)mKeyCodeAscii);

    goto loc_2E6A4;

loc_2E675:;

    if (mInput.size()) {
        mInput.pop_back();
    }

loc_2E6A4:;
    String_Input_Check();
}

void cFodder::String_Input_Check() {

    if (mKeyCodeAscii != 0x0D) {
        byte_44AC0 &= 0x3F;
        if (byte_44AC0 < 0x20)
            goto loc_2E6EA;
    }
    return;

loc_2E6EA:;
}

void cFodder::String_Print_Small(std::string pText, const size_t pY) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    String_CalculateWidth(320, mFont_Briefing_Width, pText);
    String_Print(mFont_Briefing_Width, 0, mGUI_Temp_X, pY, pText);
}

void cFodder::String_Print_Small(std::string pText, const size_t pX, const size_t pY) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    String_CalculateWidth(320, mFont_Briefing_Width, pText);
    String_Print(mFont_Briefing_Width, 0, pX, pY, pText);
}

void cFodder::String_Print_Large(std::string pText, const bool pOverAndUnderLine, const size_t pY) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    String_CalculateWidth(320, mFont_Underlined_Width, pText);
    String_Print(mFont_Underlined_Width, pOverAndUnderLine == true ? 1 : 3, mGUI_Temp_X, pY, pText);
}

void cFodder::String_Print_Large(std::string pText, const bool pOverAndUnderLine, const size_t pX, const size_t pY) {
	std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

	String_CalculateWidth(320, mFont_Underlined_Width, pText);
	String_Print(mFont_Underlined_Width, pOverAndUnderLine == true ? 1 : 3, pX, pY, pText);
}

int32 cFodder::String_MeasureWidth(const uint8* pWidths, const char* pString) {
    int32 w = 0;
    for (const char* Text = pString; *Text; ++Text) {
        uint8 Char = (uint8)(*Text);
        if (Char == 0xFE || Char == 0xFD || Char == 0xFF)
            break;
        w += pWidths[Char];
    }
    return w;
}

int32 cFodder::String_MeasureWidth(const uint8* pWidths, const std::string& pString) {
    return String_MeasureWidth(pWidths, pString.c_str());
}

void cFodder::String_Print_DrawTinyGlyph(const sBriefingSpecialGlyph* pGlyph,
                                           size_t pPosX, size_t pPosY) {
    if (!pGlyph || !mSurface)
        return;

    // Match GUI_Draw_Frame_8's coordinate system: the engine prepends a
    // 16-pixel border to mSurface (Sprite_OnScreen_Check / GUI_Draw_Frame_8
    // both add 0x10 to incoming coordinates). Stay consistent so glyphs sit
    // on the same baseline as A-Z neighbours.
    const int32 baseX = (int32)pPosX + 0x10;
    const int32 baseY = (int32)pPosY + 0x10;

    const size_t surfW = mSurface->GetWidth();
    const size_t surfH = mSurface->GetHeight();
    uint8* buf = mSurface->GetSurfaceBuffer();
    if (!buf)
        return;

    // Foreground colour: sample a pixel from a real briefing letter that
    // has already been painted to the left of our position on the same
    // line. The earlier hard-coded 0xB3 (a GUI-button primary) read as a
    // visibly different green next to the briefing letter strokes — see
    // the "OLDER: D:/PROJ" screenshot from the previous pass. By copying
    // an existing letter pixel we follow whatever palette/stroke colour
    // the surrounding letters use, including any per-row palette swap
    // the engine applies through Video_Draw_8.
    auto sampleNeighbourFg = [&]() -> uint8 {
        // Scan a strip ending just left of baseX, covering the glyph's
        // vertical extent. The briefing-font path renders letter pixels
        // in palette range 0xF1..0xFF (palette index 0xF0 OR'd with a
        // 1..F nibble; see Video_Draw_8). Pick the brightest pixel in
        // that range — that's the strongest stroke shade, the same one
        // the surrounding A-Z use.
        const int32 scanW = 24;             // a few letters' worth left
        const int32 scanX0 = baseX - scanW;
        const int32 scanY0 = baseY;
        const int32 scanY1 = baseY + (int32)pGlyph->mYOffset + (int32)pGlyph->mHeight;
        uint8 best = 0;
        for (int32 yy = scanY0; yy < scanY1; ++yy) {
            if (yy < 0 || (size_t)yy >= surfH) continue;
            for (int32 xx = scanX0; xx < baseX; ++xx) {
                if (xx < 0 || (size_t)xx >= surfW) continue;
                uint8 v = buf[(size_t)yy * surfW + (size_t)xx];
                if (v >= 0xF1 && v > best) best = v;
            }
        }
        return best;
    };
    uint8 fg = sampleNeighbourFg();
    if (fg == 0) {
        // No briefing-range pixel found nearby (e.g. we're the first
        // glyph on the line, or an Amiga-style render with a different
        // palette layout). Fall back to the brightest grey/white slot.
        fg = 0xFF;
    }

    for (uint8 row = 0; row < pGlyph->mHeight; ++row) {
        const uint8 bits = pGlyph->mRows[row];
        if (bits == 0)
            continue;

        const int32 y = baseY + (int32)pGlyph->mYOffset + (int32)row;
        if (y < 0 || (size_t)y >= surfH)
            continue;

        for (uint8 col = 0; col < pGlyph->mWidth; ++col) {
            // MSB-first bit selection: bit (7-col).
            if (col >= 8) break;
            if (!(bits & (uint8)(0x80u >> col)))
                continue;
            const int32 x = baseX + (int32)col;
            if (x < 0 || (size_t)x >= surfW)
                continue;
            buf[(size_t)y * surfW + (size_t)x] = fg;
        }
    }
}

void cFodder::String_Print_Small_Left(std::string pText, const size_t pX, const size_t pY) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);
    String_Print(mFont_Briefing_Width, 0, (int32)pX, (int32)pY, pText);
}

void cFodder::String_Print_Small_Right(std::string pText, const size_t pRightX, const size_t pY) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    const int32 w = String_MeasureWidth(mFont_Briefing_Width, pText);
    int32 x = (int32)pRightX - w;
    if (x < 0) x = 0;

    String_Print(mFont_Briefing_Width, 0, x, (int32)pY, pText);
}

void cFodder::String_Print_Small_CentreInBox(std::string pText, const size_t x1, const size_t x2, const size_t y) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    const int32 boxW = (int32)x2 - (int32)x1;
    if (boxW <= 0) return;

    const int32 textW = String_MeasureWidth(mFont_Briefing_Width, pText);
    int32 x = (int32)x1 + (boxW - textW) / 2;
    if (x < (int32)x1) x = (int32)x1;

    String_Print(mFont_Briefing_Width, 0, x, (int32)y, pText);
}

void cFodder::String_Print_Small_LeftInBox(std::string pText, const size_t x1, const size_t x2, const size_t y, const size_t padPx) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    int32 x = (int32)x1 + (int32)padPx;
    if (x < 0) x = 0;

    String_Print(mFont_Briefing_Width, 0, x, (int32)y, pText);
}

void cFodder::String_Print_Small_RightInBox(std::string pText, const size_t x1, const size_t x2, const size_t y, const size_t padPx) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    const int32 textW = String_MeasureWidth(mFont_Briefing_Width, pText);
    int32 x = (int32)x2 - (int32)padPx - textW;
    if (x < (int32)x1) x = (int32)x1;

    String_Print(mFont_Briefing_Width, 0, x, (int32)y, pText);
}

void cFodder::String_Print(const uint8* pWidths, int32 pFontSpriteID, size_t pParam08, size_t pParamC, const std::string& pText) {

    String_Print(pWidths, pFontSpriteID, pParam08, pParamC, pText.c_str());
}

void cFodder::String_Print_Small_InBox(std::string pText, const size_t x1, const size_t x2, const size_t y) {
    std::transform(pText.begin(), pText.end(), pText.begin(), ::toupper);

    const int32 boxW = (int32)x2 - (int32)x1;
    if (boxW <= 0) return;

    const int32 textW = String_MeasureWidth(mFont_Briefing_Width, pText);
    int32 px = (int32)x1 + (boxW - textW) / 2;
    if (px < (int32)x1) px = (int32)x1;

    String_Print(mFont_Briefing_Width, 0, px, (int32)y, pText);
}

void cFodder::String_Print(const uint8* pWidths, int32 pFontSpriteID, size_t pParam08, size_t pParamC, const char* pText) {
    const uint8* ptr = 0;
    uint8 al = 0;
    int32 unk14 = 0;

    mGUI_Temp_Y = (int16)pParamC;
    mGUI_Draw_LastHeight = 0;

    for (;;) {
    String_Print_Next:;
        uint8 NextChar = *pText++;
        uint8 NextChar10 = 0;

        //seg007:0170
        if (NextChar == 0xFD) {
            //sub_29DE2(0x10);

        }
        else if (NextChar == 0xFE) {
            //sub_29DE2(0x0F);

        }
        else if (NextChar == 0x00)
            break;

        else {
            //seg007:01A1
            NextChar10 = NextChar;

            if (!(NextChar & 0x80)) {

                //01AB
                if (mString_GapCharID) {
                    if (NextChar == 0x20) {
                        NextChar = (uint8)mString_GapCharID;
                        goto loc_29D71;
                    }
                }
                else {
                    //1C4
                    if (NextChar == 0x20)
                        goto loc_29DC7;
                }
                //1CD
                if (NextChar <= 0x39) {
                    if (NextChar >= 0x30) {
                        NextChar -= 0x30;
                        NextChar += 0x1A;
                        goto loc_29D71;
                    }
                }
                else {
                    //loc_29D07
                    // Briefing-font: bracket-range chars (0x5B..0x60 = [\]^_`)
                    // sit between Z and lowercase 'a' in ASCII, but the
                    // original lowercase-mapping math (NextChar -= 0x61)
                    // wraps them into bogus sprite slots (e.g. '[' was
                    // rendering as digit '8'). Route these through the
                    // small-font special-glyph fallback below, alongside
                    // punctuation, instead of the lowercase gate.
                    const bool isBracketRange = (NextChar >= 0x5B && NextChar <= 0x60);
                    if (NextChar > 0x5A && !(pWidths == mFont_Briefing_Width && isBracketRange)) {
                        NextChar -= 0x61;
                        NextChar += PLATFORM_BASED(0x39, 0x28);

                        goto loc_29D71;
                    }
                    if (NextChar >= 0x41 && NextChar <= 0x5A) {
                        NextChar -= 0x41;
                        goto loc_29D71;
                    }
                    // Fall through to the special-character handler.
                }
            }

            // Must be a special character
            // 20D
            //loc_29D2D

            // pstuff's small briefing font only ships one special glyph
            // ('.' at slot 36); the shared mGUI_Font_SpecialCharacters
            // table is authored for the larger fonts and would route
            // small-font specials past the end of stru_42928 (37 entries),
            // reading into the next sprite-sheet struct.
            //
            // For everything else (':', '/', '\\', '(', ')', etc.) we paint
            // a hand-drawn 1-bit briefing-style glyph directly to the
            // surface — see String_Print_DrawTinyGlyph + FontData_Special.
            if (pWidths == mFont_Briefing_Width) {
                if (NextChar == '.') {
                    NextChar = 0x24;        // slot 36 = '.'
                    goto loc_29D71;
                }
                if (NextChar == 0x20) {
                    // Real space (no gap-char override) — width advances
                    // via mFont_Briefing_Width[0x20]; nothing to draw.
                    goto loc_29DC7;
                }
                if (auto* g = GetBriefingSpecialGlyph(NextChar)) {
                    if (!mGUI_Print_String_To_Sidebar)
                        String_Print_DrawTinyGlyph(g, pParam08, pParamC);
                    goto loc_29DC7;
                }
                goto loc_29DC7;             // unknown char: still advance width
            }

            unk14 = -1;
            ptr = mGUI_Font_SpecialCharacters;
            do {

                // Reached end of table? then skip this character
                if (*ptr == 0xFF)
                    goto String_Print_Next;

                ++unk14;
                al = *ptr++;

                // Loop until we find this character in the table
            } while (al != NextChar);

            // Found the special character, lets use its index
            NextChar = 0x24 + unk14;

        loc_29D71:;

            if (mGUI_Print_String_To_Sidebar)
                mGraphics->Sidebar_Copy_Sprite_To_ScreenBufPtr(pFontSpriteID + NextChar, pParam08, pParamC);
            else
                GUI_Draw_Frame_8(pFontSpriteID, NextChar, pParam08, pParamC);

        }
    loc_29DC7:;

        NextChar10 = pWidths[NextChar10];

        pParam08 += NextChar10;
    }
}

void cFodder::String_CalculateWidth(int32 pPosX, const uint8* pWidths, const std::string& pString) {

    String_CalculateWidth(pPosX, pWidths, pString.c_str());
}

void cFodder::String_CalculateWidth(int32 pPosX, const uint8* pWidths, const char* pString) {
    int32 PositionX = 0;

    for (const char* Text = pString; *Text; ++Text) {
        uint8 Char = *Text;

        if (Char == 0xFE || Char == 0xFD || Char == 0xFF)
            break;

        PositionX += pWidths[Char];
    }

    pPosX -= PositionX;
    if (pPosX < 0) {
        pPosX = 0;
        PositionX = 318;
    }

    pPosX >>= 1;
    mGUI_Temp_X = pPosX;
    mGUI_Temp_Width = PositionX;
}
