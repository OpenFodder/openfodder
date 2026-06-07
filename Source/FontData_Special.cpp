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
// Visual reference: original briefing font A-Z (visible by dumping
// pstuff.lbm) uses 2-pixel-thick strokes inside a 5-6 px wide × 7 px tall
// glyph cell. The earlier 1-pixel strokes here looked spindly next to the
// real letterforms (especially noticeable on ':' '/' '-' in paths like
// "FOLDER: D:/PROJECTS"). All glyphs below are now drawn 2-px-thick to
// match the chunky letter weight.
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

    // '!' (0x21) — 2-px-thick stem with 2x2 baseline dot.
    { 0x21, 4, 6, 0, {
        G(01100000),
        G(01100000),
        G(01100000),
        G(01100000),
        G(00000000),
        G(01100000),
    } },

    // '"' (0x22) — twin 2-px ticks at the cap line.
    { 0x22, 6, 6, 0, {
        G(11011000),
        G(11011000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '#' (0x23) — 2-px verticals + 2-px crossbars; 7 wide for legibility.
    { 0x23, 7, 6, 0, {
        G(00110110),
        G(11111110),
        G(00110110),
        G(11111110),
        G(00110110),
        G(00000000),
    } },

    // '$' (0x24) — 2-px-thick S with vertical stem stroke.
    { 0x24, 6, 6, 0, {
        G(01111100),
        G(11011000),
        G(01111000),
        G(00011000),
        G(11011000),
        G(01111100),
    } },

    // '%' (0x25) — twin 2-px squares joined by a 2-px diagonal.
    { 0x25, 7, 6, 0, {
        G(11000110),
        G(11001100),
        G(00011000),
        G(00110000),
        G(01100110),
        G(11000110),
    } },

    // '&' (0x26) — 2-px ampersand. Hand-tuned to read at 7px tall.
    { 0x26, 7, 6, 0, {
        G(01110000),
        G(11011000),
        G(01110000),
        G(11011100),
        G(11001100),
        G(01110110),
    } },

    // '\'' (0x27) — 2-px tick at cap line. Engine routes '\'' through the
    // pstuff sprite slot first; this is a fallback for callers that bypass
    // it.
    { 0x27, 4, 6, 0, {
        G(01100000),
        G(01100000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '(' (0x28) — 2-px curve, opens right.
    { 0x28, 4, 6, 0, {
        G(00110000),
        G(01100000),
        G(01100000),
        G(01100000),
        G(01100000),
        G(00110000),
    } },

    // ')' (0x29) — mirror of '('.
    { 0x29, 4, 6, 0, {
        G(11000000),
        G(01100000),
        G(01100000),
        G(01100000),
        G(01100000),
        G(11000000),
    } },

    // '*' (0x2A) — 6-arm asterisk; 2-px arms.
    { 0x2A, 6, 6, 0, {
        G(01001000),
        G(11011000),
        G(01111000),
        G(11011000),
        G(01001000),
        G(00000000),
    } },

    // '+' (0x2B) — 2-px crosshair, centred in the 6-row letter cell so
    // the cross arms sit at the briefing-letter midline. Vertical bar
    // doesn't reach cap or baseline (those are reserved for letters).
    { 0x2B, 6, 6, 0, {
        G(00000000),
        G(00110000),
        G(11111100),
        G(11111100),
        G(00110000),
        G(00000000),
    } },

    // ',' (0x2C) — 2-px square baseline dot with a 2-px tail.
    { 0x2C, 4, 6, 4, {
        G(01100000),
        G(01100000),
        G(11000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '-' (0x2D) — 2-row dash on briefing letter midline. 6 px wide bar
    // with 1 px of breathing room either side (advance = 7) so the dash
    // doesn't kiss neighbouring letters in "AUTO-DETECTED".
    { 0x2D, 7, 6, 0, {
        G(00000000),
        G(00000000),
        G(01111110),
        G(01111110),
        G(00000000),
        G(00000000),
    } },

    // '.' (0x2E) — 2x2 baseline dot. Engine resolves '.' through pstuff
    // slot 36 first, so this entry is a fallback only.
    { 0x2E, 4, 6, 4, {
        G(01100000),
        G(01100000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '/' (0x2F) — 2-px-thick diagonal at briefing letter weight,
    // spanning all 6 rows so the slash reaches cap-line and baseline.
    // 5-wide kern: drops the leading whitespace column the previous
    // 6-wide variant added.
    { 0x2F, 5, 6, 0, {
        G(00011000),
        G(00011000),
        G(00110000),
        G(00110000),
        G(11000000),
        G(11000000),
    } },

    // ':' (0x3A) — 3-px-wide × 2-px-tall dots at letter cap and baseline.
    // Earlier 2x2 dots read too small next to A-Z's chunky strokes;
    // 3-wide bumps the visual weight of each dot to ~half a letter
    // stroke. Vertical positioning: upper dot rows 1-2 (just below cap)
    // and lower dot rows 4-5 (at baseline). Glyph advance is 4 — one
    // px of side bearing keeps neighbouring letters from kissing.
    { 0x3A, 4, 6, 0, {
        G(00000000),
        G(11100000),
        G(11100000),
        G(00000000),
        G(11100000),
        G(11100000),
    } },

    // ';' (0x3B) — upper 3x2 dot, lower 3x2 dot with a 2-px descender
    // tail. Same width/positioning as ':' so the dot stack reads
    // consistent across the two glyphs.
    { 0x3B, 4, 6, 0, {
        G(00000000),
        G(11100000),
        G(11100000),
        G(00000000),
        G(11100000),
        G(01100000),
    } },

    // '<' (0x3C) — 2-px chevron pointing left.
    { 0x3C, 6, 6, 0, {
        G(00001100),
        G(00110000),
        G(11000000),
        G(00110000),
        G(00001100),
        G(00000000),
    } },

    // '=' (0x3D) — twin 2-px-thick bars centred on the briefing midline.
    { 0x3D, 6, 6, 1, {
        G(11111100),
        G(11111100),
        G(00000000),
        G(11111100),
        G(11111100),
        G(00000000),
    } },

    // '>' (0x3E) — 2-px chevron pointing right.
    { 0x3E, 6, 6, 0, {
        G(11000000),
        G(00110000),
        G(00001100),
        G(00110000),
        G(11000000),
        G(00000000),
    } },

    // '?' (0x3F) — 2-px stroke arc + tail with baseline 2x2 dot.
    { 0x3F, 6, 6, 0, {
        G(01111000),
        G(11001100),
        G(00011000),
        G(00110000),
        G(00000000),
        G(00110000),
    } },

    // '@' (0x40) — boxed at-sign with 2-px outer ring.
    { 0x40, 8, 6, 0, {
        G(01111100),
        G(11000110),
        G(11011110),
        G(11010110),
        G(11011110),
        G(11000000),
    } },

    // ---------- structural / bracket / underscore (0x5B..0x60) ----------
    // '[' (0x5B) — 2-px-thick square bracket.
    { 0x5B, 4, 6, 0, {
        G(11110000),
        G(11000000),
        G(11000000),
        G(11000000),
        G(11000000),
        G(11110000),
    } },

    // '\\' (0x5C) — 2-px-thick diagonal, mirror of '/'.
    { 0x5C, 5, 6, 0, {
        G(11000000),
        G(11000000),
        G(00110000),
        G(00110000),
        G(00011000),
        G(00011000),
    } },

    // ']' (0x5D) — mirror of '['.
    { 0x5D, 4, 6, 0, {
        G(11110000),
        G(00110000),
        G(00110000),
        G(00110000),
        G(00110000),
        G(11110000),
    } },

    // '^' (0x5E) — 2-px caret at cap line.
    { 0x5E, 6, 6, 0, {
        G(00110000),
        G(01111000),
        G(11001100),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '_' (0x5F) — 2-px-thick underscore at the descender line.
    { 0x5F, 6, 6, 5, {
        G(11111100),
        G(11111100),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // '`' (0x60) — 2-px backtick at cap line.
    { 0x60, 4, 6, 0, {
        G(11000000),
        G(01100000),
        G(00000000),
        G(00000000),
        G(00000000),
        G(00000000),
    } },

    // ---------- braces / pipe / tilde (0x7B..0x7E) ----------
    // '{' (0x7B) — 2-px curly brace, opens right.
    { 0x7B, 5, 6, 0, {
        G(00111000),
        G(01100000),
        G(11000000),
        G(01100000),
        G(01100000),
        G(00111000),
    } },

    // '|' (0x7C) — 2-px-thick vertical bar.
    { 0x7C, 3, 6, 0, {
        G(11000000),
        G(11000000),
        G(11000000),
        G(11000000),
        G(11000000),
        G(11000000),
    } },

    // '}' (0x7D) — mirror of '{'.
    { 0x7D, 5, 6, 0, {
        G(11100000),
        G(00110000),
        G(00011000),
        G(00110000),
        G(00110000),
        G(11100000),
    } },

    // '~' (0x7E) — 2-px wave at cap line.
    { 0x7E, 7, 6, 1, {
        G(01100110),
        G(11110110),
        G(11001110),
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
