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

// Hand-drawn briefing-font special glyphs.
//
// Visual reference: original briefing font letters are 5-6 pixels tall with
// uniform 1-pixel strokes. Apostrophe sits at the top; period sits at the
// baseline. We aim for the same visual register here.
//
// Bitmap convention: each byte's bit 7 (MSB) is the leftmost pixel. Rows are
// drawn top-down. mYOffset shifts the whole glyph down from the line's top
// (e.g. a period needs to sit on the baseline, not the top).

#define G(b) (uint8)(0b##b)

static const sBriefingSpecialGlyph kGlyphs[] = {
    // ---------- standard ASCII punctuation (0x20..0x40) ----------
    // ' ' (0x20) — space, drawn as a no-op but listed for completeness so
    // the lookup returns non-null and the width advance happens.
    { 0x20, 5, 6, 0, { 0,0,0,0,0,0 } },

    // '!' (0x21)
    { 0x21, 3, 6, 0, {
        G(01000000),
        G(01000000),
        G(01000000),
        G(01000000),
        G(00000000),
        G(01000000),
    } },

    // '"' (0x22)
    { 0x22, 5, 6, 0, {
        G(10100000),
        G(10100000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '#' (0x23)
    { 0x23, 6, 6, 0, {
        G(01010000),
        G(11111100),
        G(01010000),
        G(11111100),
        G(01010000),
        G(00000000),
    } },

    // '$' (0x24)
    { 0x24, 6, 6, 0, {
        G(01111000),
        G(10100000),
        G(01110000),
        G(00101000),
        G(11110000),
        G(00100000),
    } },

    // '%' (0x25)
    { 0x25, 7, 6, 0, {
        G(11000010),
        G(11000100),
        G(00001000),
        G(00010000),
        G(00100110),
        G(01000110),
    } },

    // '&' (0x26)
    { 0x26, 7, 6, 0, {
        G(01100000),
        G(10010000),
        G(01100000),
        G(10010100),
        G(10001000),
        G(01110100),
    } },

    // '\'' (0x27) — duplicates the original sprite-slot glyph; harmless if
    // the existing slot still resolves first via the special-char table.
    { 0x27, 6, 6, 0, {
        G(01100000),
        G(01100000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '(' (0x28)
    { 0x28, 4, 6, 0, {
        G(00100000),
        G(01000000),
        G(01000000),
        G(01000000),
        G(01000000),
        G(00100000),
    } },

    // ')' (0x29)
    { 0x29, 4, 6, 0, {
        G(10000000),
        G(01000000),
        G(01000000),
        G(01000000),
        G(01000000),
        G(10000000),
    } },

    // '*' (0x2A)
    { 0x2A, 5, 6, 0, {
        G(00100000),
        G(10101000),
        G(01110000),
        G(10101000),
        G(00100000),
        G(00000000),
    } },

    // '+' (0x2B)
    { 0x2B, 5, 6, 1, {
        G(00100000),
        G(00100000),
        G(11111000),
        G(00100000),
        G(00100000),
        G(00000000),
    } },

    // ',' (0x2C)
    { 0x2C, 3, 6, 4, {
        G(01100000),
        G(00100000),
        G(01000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '-' (0x2D) — sit on the visual midline of A-Z, span the full
    // character cell. The single-row dash at row 2 was visually too high
    // and made "AUTO-DETECTED" look split; row 3 lines up with the
    // briefing letterforms' midline.
    { 0x2D, 5, 6, 0, {
        G(00000000),
        G(00000000),
        G(00000000),
        G(11111000),
        G(00000000),
        G(00000000),
    } },

    // '.' (0x2E) — duplicates original slot 36; the engine resolves '.' via
    // its slot first, so this is a fallback for code paths that ignore the
    // sprite slot (none today, but harmless).
    { 0x2E, 3, 6, 5, {
        G(01000000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '/' (0x2F)
    { 0x2F, 5, 6, 0, {
        G(00001000),
        G(00010000),
        G(00100000),
        G(01000000),
        G(10000000),
        G(00000000),
    } },

    // ':' (0x3A)
    { 0x3A, 3, 6, 1, {
        G(01000000),
        G(00000000),
        G(00000000),
        G(01000000),
        G(00000000),
        G(00000000),
    } },

    // ';' (0x3B)
    { 0x3B, 3, 6, 1, {
        G(01000000),
        G(00000000),
        G(00000000),
        G(01000000),
        G(01000000),
        G(10000000),
    } },

    // '<' (0x3C)
    { 0x3C, 5, 6, 1, {
        G(00010000),
        G(00100000),
        G(01000000),
        G(00100000),
        G(00010000),
        G(00000000),
    } },

    // '=' (0x3D)
    { 0x3D, 6, 6, 1, {
        G(11111100),
        G(00000000),
        G(11111100),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '>' (0x3E)
    { 0x3E, 5, 6, 1, {
        G(10000000),
        G(01000000),
        G(00100000),
        G(01000000),
        G(10000000),
        G(00000000),
    } },

    // '?' (0x3F)
    { 0x3F, 6, 6, 0, {
        G(01110000),
        G(10001000),
        G(00010000),
        G(00100000),
        G(00000000),
        G(00100000),
    } },

    // '@' (0x40)
    { 0x40, 8, 6, 0, {
        G(00111100),
        G(01000010),
        G(10011010),
        G(10101010),
        G(10011110),
        G(01000000),
    } },

    // ---------- structural / bracket / underscore (0x5B..0x60) ----------
    // '[' (0x5B)
    { 0x5B, 4, 6, 0, {
        G(11100000),
        G(10000000),
        G(10000000),
        G(10000000),
        G(10000000),
        G(11100000),
    } },

    // '\\' (0x5C)
    { 0x5C, 5, 6, 0, {
        G(10000000),
        G(01000000),
        G(00100000),
        G(00010000),
        G(00001000),
        G(00000000),
    } },

    // ']' (0x5D)
    { 0x5D, 4, 6, 0, {
        G(11100000),
        G(00100000),
        G(00100000),
        G(00100000),
        G(00100000),
        G(11100000),
    } },

    // '^' (0x5E)
    { 0x5E, 6, 6, 0, {
        G(00100000),
        G(01010000),
        G(10001000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '_' (0x5F)
    { 0x5F, 6, 6, 5, {
        G(11111100),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '`' (0x60)
    { 0x60, 3, 6, 0, {
        G(10000000),
        G(01000000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // ---------- braces / pipe / tilde (0x7B..0x7E) ----------
    // '{' (0x7B)
    { 0x7B, 4, 6, 0, {
        G(00110000),
        G(01000000),
        G(11000000),
        G(01000000),
        G(01000000),
        G(00110000),
    } },

    // '|' (0x7C)
    { 0x7C, 3, 6, 0, {
        G(01000000),
        G(01000000),
        G(01000000),
        G(01000000),
        G(01000000),
        G(01000000),
    } },

    // '}' (0x7D)
    { 0x7D, 4, 6, 0, {
        G(11000000),
        G(00100000),
        G(00110000),
        G(00100000),
        G(00100000),
        G(11000000),
    } },

    // '~' (0x7E)
    { 0x7E, 6, 6, 1, {
        G(01001000),
        G(10110000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },
};

#undef G

const sBriefingSpecialGlyph* GetBriefingSpecialGlyph(uint8 pAscii) {
    for (auto& g : kGlyphs) {
        if (g.mAscii == pAscii)
            return &g;
    }
    return nullptr;
}
