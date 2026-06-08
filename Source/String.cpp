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
#include "SidebarFontGlyphs.hpp"

#include <algorithm>
#include <cctype>

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

    // Sample the actual briefing-font sprite for letter 'A' (slot 0 in
    // the briefing sheet — String_Print maps 'A' = 0x41 to NextChar 0).
    // Every shipped briefing letter in pstuff is encoded with exactly
    // two distinct nibble values: a STROKE nibble for the letter body
    // and a BEVEL nibble for the bottom/right shadow that sits beneath
    // it. We read those two nibbles straight off the sprite, so the
    // colours we paint are always pixel-identical to what the engine
    // paints for A-Z. Earlier attempts (sample-from-surface and
    // histogram-based picks) kept getting fooled by the candidate-list
    // box border (also in 0xF1..0xFF), the per-row palette swap, or by
    // a glyph having more shadow than stroke pixels.
    //
    // Sprite frames decode as packed nibbles: each byte holds two
    // pixels (high nibble first, low nibble second), zero is the
    // transparent colour. Stride is `mPalleteIndex` bytes per row
    // (240 / 0xF0 for pstuff). Once we have the two nibbles, the final
    // surface colours are nibble | mPalleteIndex (0xF0) — exactly what
    // Video_Draw_8 produces.
    struct ColourPair { uint8 stroke; uint8 shadow; };
    static ColourPair sCachedCol{ 0, 0 };
    auto resolveBriefingColours = [&]() -> ColourPair {
        if (sCachedCol.stroke) return sCachedCol;
        if (!mSprite_SheetPtr) return ColourPair{ 0, 0 };
        const sSpriteSheet* a = Sprite_Get_Sheet(0, 0);
        if (!a || a->mColCount <= 0 || a->mRowCount <= 0)
            return ColourPair{ 0, 0 };
        const uint8* gp = a->GetGraphicsPtr();
        if (!gp) return ColourPair{ 0, 0 };
        // Histogram nibbles 1..15 across the full sprite cell.
        uint16 hist[16] = { 0 };
        const int32 cols = (int32)a->mColCount;       // pixels wide
        const int32 rows = (int32)a->mRowCount;       // rows
        const int32 stride = a->mPalleteIndex;         // bytes per row
        for (int32 r = 0; r < rows; ++r) {
            const uint8* row = gp + (size_t)r * (size_t)stride;
            for (int32 c = 0; c < (cols >> 1); ++c) {
                uint8 b = row[c];
                uint8 hi = (uint8)(b >> 4);
                uint8 lo = (uint8)(b & 0x0F);
                if (hi) hist[hi]++;
                if (lo) hist[lo]++;
            }
        }
        // Top two populated bins by frequency.
        int n1 = 0, n2 = 0; uint16 c1 = 0, c2 = 0;
        for (int n = 1; n <= 15; ++n) {
            if (hist[n] > c1) { c2 = c1; n2 = n1; c1 = hist[n]; n1 = n; }
            else if (hist[n] > c2) { c2 = hist[n]; n2 = n; }
        }
        ColourPair r{ 0, 0 };
        if (n1) r.stroke = (uint8)(0xF0 | n1);
        if (n2) r.shadow = (uint8)(0xF0 | n2);
        // Cache only when we got both — otherwise retry next frame.
        if (r.stroke && r.shadow) sCachedCol = r;
        return r;
    };

    ColourPair col = resolveBriefingColours();
    if (!col.stroke) {
        // Briefing sprite sheet not bound yet (shouldn't happen on the
        // wizard, but defensively) — fall back to a hard-coded pair
        // that lands in the briefing palette range.
        col.stroke = 0xFD;
        col.shadow = 0xF8;
    }
    if (!col.shadow) {
        uint8 nibble = (uint8)(col.stroke & 0x0F);
        uint8 dn = (nibble > 4) ? (uint8)(nibble - 4) : (uint8)1;
        col.shadow = (uint8)(0xF0 | dn);
    }

    auto setPixel = [&](int32 x, int32 y, uint8 c) {
        if (x < 0 || (size_t)x >= surfW) return;
        if (y < 0 || (size_t)y >= surfH) return;
        buf[(size_t)y * surfW + (size_t)x] = c;
    };

    // Two-pass paint with a thin bottom shadow.
    //
    // Pass 1 — shadow along the BOTTOM EDGE only (offset 0,+1), so the
    // shadow doesn't extend past the glyph's right side. A previous
    // attempt used a +1,+1 diagonal dilation which roughly doubled the
    // visible area of small punctuation (':' '+' '-') and drew them
    // chunkier than the surrounding A-Z. Bottom-only matches the way
    // the briefing-font sprites bevel their letterforms vertically
    // without changing the glyph's horizontal footprint.
    //
    // Pass 2 — stroke at 0,0, overwriting any shadow pixel where the
    // glyph itself paints (the row directly below a stroke pixel that
    // is also stroke).
    for (int pass = 0; pass < 2; ++pass) {
        const uint8 c = (pass == 0) ? col.shadow : col.stroke;
        const int32 oy = (pass == 0) ? 1 : 0;
        for (uint8 row = 0; row < pGlyph->mHeight; ++row) {
            const uint8 bits = pGlyph->mRows[row];
            if (bits == 0) continue;
            const int32 y = baseY + (int32)pGlyph->mYOffset + (int32)row + oy;
            for (uint8 col2 = 0; col2 < pGlyph->mWidth; ++col2) {
                if (col2 >= 8) break;
                if (!(bits & (uint8)(0x80u >> col2))) continue;
                setPixel(baseX + (int32)col2, y, c);
            }
        }
    }
}

// ============================================================================
// Sidebar (small) font on the main screen.
//
// The original game's sidebar A-Z lives in pstuff.lbm at sprite slots
// 0x29..0x42 / 0x4D..0x66 / 0x71..0x8A (three colour variants), but the
// existing render path (Sidebar_Copy_Sprite_To_ScreenBufPtr) only writes
// into mSidebar_Screen_BufferPtr. Routing those slots to the main surface
// would require new sprite-sheet entries on both PC and Amiga and a third
// String_Print dispatch path.
//
// Simpler: paint hand-authored 1-bit glyphs from Source/SidebarFontGlyphs.cpp
// directly to mSurface — same pattern as String_Print_DrawTinyGlyph for the
// briefing-font specials. Stroke colour for each colour variant is sampled
// from a real pstuff letter so we share palette with the rendered glyphs
// across PC and Amiga without duplicating the variant tables.
// ============================================================================

namespace {

// Pick the stroke colour for a sidebar-font variant by reading a real pstuff
// letter from the pstuff sprite sheet. mSidebar_Font_ColorBases (in
// GUI_Element.cpp) holds the slot bases — Normal=0x29, Selected=0x4D,
// Inactive=0x71. The letter at those slots is 'A'; sampling its most-
// frequent non-zero nibble gives the same colour the
// in-game sidebar uses to paint the letter. Cached per (variant, sheet) for
// the lifetime of one paint pass; pstuff is loaded once, so a static cache is
// safe.

struct SidebarStrokeCache {
    uint8 normal = 0;
    uint8 selected = 0;
    uint8 inactive = 0;
    bool primed = false;
} static sStrokeCache;

uint8 SampleSlotStroke(uint16 pSlot) {
    if (!g_Fodder || !g_Fodder->mGraphics)
        return 0;
    // mSpriteSheet_PStuff (declared in Fodder.hpp) describes each pstuff
    // sprite as (X, Y, Cols, Rows) inside the pstuff bitmap. Sample a
    // histogram of non-zero nibbles inside the letter cell.
    const sSpriteSheet_pstuff& sheet = mSpriteSheet_PStuff[pSlot];

    // PC pstuff: 0xA0 bytes per row, 2 nibbles per byte (high-first); a
    // histogram of non-zero nibbles inside the cell gives the stroke index.
    // Amiga pstuff is planar (40 bytes per row, 4 bitplanes) — the same
    // histogram trick doesn't directly apply, but both platforms decode
    // pstuff into mImagePStuff via Decode_Image at boot, so reading from
    // the unpacked sprite data via GetGraphicsPtr is uniform across
    // platforms. If the byte stream is empty/uninitialised we return 0 and
    // the caller falls back to a hard-coded colour pair.
    const uint8* gp = sheet.GetGraphicsPtr((0xA0 * sheet.mY) + (sheet.mX >> 1));
    if (!gp)
        return 0;

    uint16 hist[16] = { 0 };
    const int rows = sheet.mRows;
    const int colsHalf = sheet.mColumns >> 1;
    const int srcSkip = 0xA0 - colsHalf;
    const uint8* row = gp;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < colsHalf; ++c) {
            const uint8 b = row[c];
            const uint8 hi = (uint8)(b >> 4);
            const uint8 lo = (uint8)(b & 0x0F);
            if (hi) hist[hi]++;
            if (lo) hist[lo]++;
        }
        row += colsHalf + srcSkip;
    }
    int best = 0; uint16 bestC = 0;
    for (int n = 1; n <= 15; ++n) {
        if (hist[n] > bestC) { bestC = hist[n]; best = n; }
    }
    if (best)
        return (uint8)(0xF0 | best);
    return 0;
}

void PrimeStrokeCache() {
    if (sStrokeCache.primed)
        return;
    // Only prime when we have something to read; otherwise let the next call
    // try again. mSpriteSheet_PStuff is loaded by Load_pStuff at engine boot.
    const uint8 normal   = SampleSlotStroke(0x29);
    const uint8 selected = SampleSlotStroke(0x4D);
    const uint8 inactive = SampleSlotStroke(0x71);
    if (normal && selected && inactive) {
        sStrokeCache.normal = normal;
        sStrokeCache.selected = selected;
        sStrokeCache.inactive = inactive;
        sStrokeCache.primed = true;
    }
}

uint8 ResolveStroke(eSidebarFontColor pColor) {
    PrimeStrokeCache();
    if (!sStrokeCache.primed) {
        // Fallback: hard-coded colours that land in the pstuff palette range.
        // Better than rendering invisibly if the sheet isn't bound yet.
        switch (pColor) {
        case eSidebarFontColor::Normal:   return 0xFD;
        case eSidebarFontColor::Selected: return 0xFB;
        case eSidebarFontColor::Inactive: return 0xF9;
        }
        return 0xFD;
    }
    switch (pColor) {
    case eSidebarFontColor::Normal:   return sStrokeCache.normal;
    case eSidebarFontColor::Selected: return sStrokeCache.selected;
    case eSidebarFontColor::Inactive: return sStrokeCache.inactive;
    }
    return sStrokeCache.normal;
}

uint8 DeriveShadow(uint8 pStroke) {
    // Step the palette index a couple of slots toward black; matches the
    // briefing-font tiny-glyph fallback.
    const uint8 nibble = (uint8)(pStroke & 0x0F);
    const uint8 dn = (nibble > 4) ? (uint8)(nibble - 4) : (uint8)1;
    return (uint8)(0xF0 | dn);
}

} // namespace

void cFodder::String_Print_DrawSidebarGlyph(const sSidebarSpecialGlyph* pGlyph,
                                             size_t pPosX, size_t pPosY,
                                             eSidebarFontColor pColor) {
    if (!pGlyph || !mSurface)
        return;

    // Surface coordinates: like GUI_Draw_Frame_8 / String_Print_DrawTinyGlyph,
    // mSurface has a 16-px engine border on each axis. Add it so the small
    // font lands on the same baseline as briefing-font neighbours when both
    // are drawn at the same nominal Y.
    const int32 baseX = (int32)pPosX + 0x10;
    const int32 baseY = (int32)pPosY + 0x10;

    const size_t surfW = mSurface->GetWidth();
    const size_t surfH = mSurface->GetHeight();
    uint8* buf = mSurface->GetSurfaceBuffer();
    if (!buf)
        return;

    const uint8 stroke = ResolveStroke(pColor);
    const uint8 shadow = DeriveShadow(stroke);

    auto setPixel = [&](int32 x, int32 y, uint8 c) {
        if (x < 0 || (size_t)x >= surfW) return;
        if (y < 0 || (size_t)y >= surfH) return;
        buf[(size_t)y * surfW + (size_t)x] = c;
    };

    // Two-pass paint: bottom shadow first, then stroke on top. Keeps small-
    // glyph footprints visually identical to briefing-font specials.
    for (int pass = 0; pass < 2; ++pass) {
        const uint8 c = (pass == 0) ? shadow : stroke;
        const int32 oy = (pass == 0) ? 1 : 0;
        for (uint8 row = 0; row < pGlyph->mHeight; ++row) {
            const uint8 bits = pGlyph->mRows[row];
            if (bits == 0) continue;
            const int32 y = baseY + (int32)pGlyph->mYOffset + (int32)row + oy;
            // mWidth is the advance width; the bitmap-bit field is encoded
            // MSB-first inside a 5-bit body. We scan up to 5 columns for
            // body strokes (bits 0x10..0x01).
            for (uint8 col = 0; col < 5; ++col) {
                if (!(bits & (uint8)(0x10u >> col))) continue;
                setPixel(baseX + (int32)col, y, c);
            }
        }
    }
}

size_t cFodder::MainScreen_Print_Sidebar(const std::string& pText,
                                          size_t pX, size_t pY,
                                          eSidebarFontColor pColor) {
    int32 x = (int32)pX;
    for (char raw : pText) {
        const uint8 ch = (uint8)std::toupper((unsigned char)raw);
        const uint8 width = mFont_SidebarMain_Width[ch];
        if (!width)
            continue;
        if (ch == 0x20) {
            // Space: no glyph, just advance.
            x += width;
            continue;
        }
        if (auto* g = GetSidebarSpecialGlyph(ch))
            String_Print_DrawSidebarGlyph(g, (size_t)x, pY, pColor);
        // 1 px gap between glyphs (already baked into mFont_SidebarMain_Width
        // for body-width-N glyphs as N+1 advance) — width is the advance.
        x += width;
    }
    return (size_t)x;
}

void cFodder::MainScreen_Print_Sidebar_CentreInBox(const std::string& pText,
                                                    size_t pX1, size_t pX2, size_t pY,
                                                    eSidebarFontColor pColor) {
    if (pX2 <= pX1)
        return;
    int32 width = 0;
    for (char raw : pText) {
        const uint8 ch = (uint8)std::toupper((unsigned char)raw);
        width += (int32)mFont_SidebarMain_Width[ch];
    }
    const int32 boxW = (int32)pX2 - (int32)pX1;
    int32 x = (int32)pX1 + (boxW - width) / 2;
    if (x < (int32)pX1) x = (int32)pX1;
    (void)MainScreen_Print_Sidebar(pText, (size_t)x, pY, pColor);
}

int32 cFodder::MainScreen_MeasureSidebarWidth(const std::string& pText) {
    int32 width = 0;
    for (char raw : pText) {
        const uint8 ch = (uint8)std::toupper((unsigned char)raw);
        width += (int32)mFont_SidebarMain_Width[ch];
    }
    return width;
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
