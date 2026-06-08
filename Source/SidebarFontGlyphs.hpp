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

#pragma once

// Sidebar (small) font extension glyphs.
//
// The original game ships A-Z + space in pstuff.lbm at sprite slots
// 0x29..0x42 (normal), 0x4D..0x66 (selected/gold) and 0x71..0x8A
// (inactive/grey) — see mSidebar_Font_ColorBases in
// GUI_Element.cpp. The in-between slots are unused by the engine.
//
// To use this font for campaign-select labels we hand-author the missing
// codepoints (digits, common punctuation) as 1-bit bitmaps. They're painted
// at runtime by String_Print_DrawSidebarSpecialGlyph using the SAME stroke
// and shadow colours we sample from a real pstuff letter sprite — so digits
// and letters always share the same palette no matter which colour variant
// is active.
//
// Mirrors the briefing-font precedent (FontData_Special.hpp).

struct sSidebarSpecialGlyph {
    uint8 mAscii;       // ASCII codepoint, e.g. '0' = 0x30
    uint8 mWidth;       // Pixel advance width (must match mFont_Sidebar_Width[mAscii])
    uint8 mHeight;      // Pixel height — pstuff sidebar A-Z are 7 px tall
    uint8 mYOffset;     // Top padding rows before the first drawn row
    uint8 mRows[8];     // 1-bit pattern, MSB = leftmost pixel; rows 0..(mHeight-1) used
};

// Returns the glyph for pAscii, or null if no extra glyph is defined for
// the sidebar font (caller should fall through to the existing String_Print
// "skip but advance width" behaviour).
const sSidebarSpecialGlyph* GetSidebarSpecialGlyph(uint8 pAscii);
