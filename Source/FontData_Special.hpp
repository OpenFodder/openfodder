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

// Briefing-font special glyphs: 6-row × up-to-7-column 1-bit bitmaps for
// punctuation and structural characters that don't ship as sprite frames in
// the original game's pstuff sheet (which only carries A-Z, 0-9, '.').
//
// Each glyph is exposed as a 6-byte array — one byte per row, MSB-first up
// to mWidth columns. Hand-drawn to match the chunky 1-bit stroke weight of
// the briefing font's letterforms (compare A-Z slot 0..25 in pstuff:
// uniform 1-pixel strokes, 5-6 pixel glyph height, 1-pixel descenders).
//
// Dimensions (mWidth/mHeight) include any internal whitespace; the engine's
// String_Print advances pPosX by mFont_Briefing_Width[ASCII], not by these
// values, so the two MUST stay in sync.

struct sBriefingSpecialGlyph {
    uint8 mAscii;       // ASCII codepoint, e.g. ':' = 0x3A
    uint8 mWidth;       // Pixel width 1..7 (must match mFont_Briefing_Width[mAscii])
    uint8 mHeight;      // Pixel height (always 6 for now; matches A-Z baseline)
    uint8 mYOffset;     // Top-padding rows before the first drawn row
    uint8 mRows[8];     // 1-bit pattern, MSB = leftmost pixel; rows 0..(mHeight-1) used
};

// Returns the glyph for pAscii, or null if no extra glyph is defined (i.e.
// String_Print's existing sprite path / skip path applies).
const sBriefingSpecialGlyph* GetBriefingSpecialGlyph(uint8 pAscii);
