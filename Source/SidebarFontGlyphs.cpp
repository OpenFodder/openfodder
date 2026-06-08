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
#include "SidebarFontGlyphs.hpp"

// Sidebar font, hand-authored 1-bit bitmaps.
//
// The original game's sidebar A-Z lives in pstuff.lbm at sprite slots
// 0x29..0x42 (Normal), 0x4D..0x66 (Selected/Gold), 0x71..0x8A (Inactive/Grey),
// rendered to the in-game sidebar via cGraphics::Sidebar_Copy_Sprite_To_ScreenBufPtr.
// That path writes only into mSidebar_Screen_BufferPtr, so we cannot reuse it
// for main-screen text on the campaign-select / multiplayer-menu surfaces.
//
// Re-routing the existing sprite-sheet system to point at the same pstuff
// regions would mean two divergent layouts (PC packed nibbles + 160-byte
// stride vs Amiga planar 40-byte stride) plus a third sheet array per
// platform. Authoring the glyphs as plain 1-bit bitmaps and painting them
// directly to mSurface is platform-agnostic, matches what
// String_Print_DrawTinyGlyph already does for the briefing-font specials,
// and gives us digits + punctuation for free in the same scheme.
//
// All glyphs render in a 5×5 stroke box centred in a 5px-wide × 7px-tall
// cell — matches the pstuff sidebar A-Z body height. Row 0 and row 6 stay
// blank so the bottom-shadow pass has somewhere to paint without colliding
// with the next line.
//
// Bitmap encoding (mirrors FontData_Special.cpp):
//   - mRows[r] is a single byte; bit 0x10 = leftmost-of-5, bit 0x01 = rightmost.
//   - mWidth is the advance width in pixels (must match mFont_Sidebar_Width).
//   - mHeight is the painted height (typically 7).
//   - mYOffset gives the top padding to the first painted row (typically 1).

static const sSidebarSpecialGlyph kSidebarGlyphs[] = {

    // ===== Letters A-Z =====
    // Letterforms hand-authored to match the visual rhythm of pstuff's
    // sidebar A-Z (uniform 1-px stroke weight, square counters, 5×5 body).
    // Verify by eye on the campaign-select screen against an in-game
    // troop-name 'A' once both render at full size.

    // A:  .#.       01000  = 0x08
    //     #.#       10100  = 0x14
    //     ###       11100  = 0x1C
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    { 'A', 4, 7, 1, { 0x00, 0x08, 0x14, 0x1C, 0x14, 0x14, 0x00, 0x00 } },
    // B:  ##.       11000  = 0x18
    //     #.#       10100  = 0x14
    //     ##.       11000  = 0x18
    //     #.#       10100  = 0x14
    //     ##.       11000  = 0x18
    { 'B', 4, 7, 1, { 0x00, 0x18, 0x14, 0x18, 0x14, 0x18, 0x00, 0x00 } },
    // C:  .##       01100  = 0x0C
    //     #..       10000  = 0x10
    //     #..       10000  = 0x10
    //     #..       10000  = 0x10
    //     .##       01100  = 0x0C
    { 'C', 4, 7, 1, { 0x00, 0x0C, 0x10, 0x10, 0x10, 0x0C, 0x00, 0x00 } },
    // D:  ##.       11000  = 0x18
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     ##.       11000  = 0x18
    { 'D', 4, 7, 1, { 0x00, 0x18, 0x14, 0x14, 0x14, 0x18, 0x00, 0x00 } },
    // E:  ###       11100  = 0x1C
    //     #..       10000  = 0x10
    //     ##.       11000  = 0x18
    //     #..       10000  = 0x10
    //     ###       11100  = 0x1C
    { 'E', 4, 7, 1, { 0x00, 0x1C, 0x10, 0x18, 0x10, 0x1C, 0x00, 0x00 } },
    // F:  ###       11100  = 0x1C
    //     #..       10000  = 0x10
    //     ##.       11000  = 0x18
    //     #..       10000  = 0x10
    //     #..       10000  = 0x10
    { 'F', 4, 7, 1, { 0x00, 0x1C, 0x10, 0x18, 0x10, 0x10, 0x00, 0x00 } },
    // G:  .##       01100  = 0x0C
    //     #..       10000  = 0x10
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     .##       01100  = 0x0C
    { 'G', 4, 7, 1, { 0x00, 0x0C, 0x10, 0x14, 0x14, 0x0C, 0x00, 0x00 } },
    // H:  #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     ###       11100  = 0x1C
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    { 'H', 4, 7, 1, { 0x00, 0x14, 0x14, 0x1C, 0x14, 0x14, 0x00, 0x00 } },
    // I:  #         10000  = 0x10
    //     #         10000  = 0x10
    //     #         10000  = 0x10
    //     #         10000  = 0x10
    //     #         10000  = 0x10
    { 'I', 2, 7, 1, { 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00 } },
    // J:  ..#       00100  = 0x04
    //     ..#       00100  = 0x04
    //     ..#       00100  = 0x04
    //     #.#       10100  = 0x14
    //     .#.       01000  = 0x08
    { 'J', 4, 7, 1, { 0x00, 0x04, 0x04, 0x04, 0x14, 0x08, 0x00, 0x00 } },
    // K:  #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     ##.       11000  = 0x18
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    { 'K', 4, 7, 1, { 0x00, 0x14, 0x14, 0x18, 0x14, 0x14, 0x00, 0x00 } },
    // L:  #..       10000  = 0x10
    //     #..       10000  = 0x10
    //     #..       10000  = 0x10
    //     #..       10000  = 0x10
    //     ###       11100  = 0x1C
    { 'L', 4, 7, 1, { 0x00, 0x10, 0x10, 0x10, 0x10, 0x1C, 0x00, 0x00 } },
    // M:  #...#     10001  = 0x11
    //     ##.##     11011  = 0x1B
    //     #.#.#     10101  = 0x15
    //     #...#     10001  = 0x11
    //     #...#     10001  = 0x11
    { 'M', 6, 7, 1, { 0x00, 0x11, 0x1B, 0x15, 0x11, 0x11, 0x00, 0x00 } },
    // N:  #..#      1001   = 0x9
    //     ##.#      1101   = 0xD
    //     #.##      1011   = 0xB
    //     #..#      1001   = 0x9
    //     #..#      1001   = 0x9
    { 'N', 5, 7, 1, { 0x00, 0x09, 0x0D, 0x0B, 0x09, 0x09, 0x00, 0x00 } },
    // O:  .#.       01000  = 0x08
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     .#.       01000  = 0x08
    { 'O', 4, 7, 1, { 0x00, 0x08, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00 } },
    // P:  ##.       11000  = 0x18
    //     #.#       10100  = 0x14
    //     ##.       11000  = 0x18
    //     #..       10000  = 0x10
    //     #..       10000  = 0x10
    { 'P', 4, 7, 1, { 0x00, 0x18, 0x14, 0x18, 0x10, 0x10, 0x00, 0x00 } },
    // Q:  .#.       01000  = 0x08
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     ##.       11000  = 0x18
    //     ..#       00100  = 0x04
    { 'Q', 4, 7, 1, { 0x00, 0x08, 0x14, 0x14, 0x18, 0x04, 0x00, 0x00 } },
    // R:  ##.       11000  = 0x18
    //     #.#       10100  = 0x14
    //     ##.       11000  = 0x18
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    { 'R', 4, 7, 1, { 0x00, 0x18, 0x14, 0x18, 0x14, 0x14, 0x00, 0x00 } },
    // S:  .##       01100  = 0x0C
    //     #..       10000  = 0x10
    //     .#.       01000  = 0x08
    //     ..#       00100  = 0x04
    //     ##.       11000  = 0x18
    { 'S', 4, 7, 1, { 0x00, 0x0C, 0x10, 0x08, 0x04, 0x18, 0x00, 0x00 } },
    // T:  ###       11100  = 0x1C
    //     .#.       01000  = 0x08
    //     .#.       01000  = 0x08
    //     .#.       01000  = 0x08
    //     .#.       01000  = 0x08
    { 'T', 4, 7, 1, { 0x00, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00 } },
    // U:  #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     .#.       01000  = 0x08
    { 'U', 4, 7, 1, { 0x00, 0x14, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00 } },
    // V:  #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     .#.       01000  = 0x08
    { 'V', 4, 7, 1, { 0x00, 0x14, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00 } },
    // W:  #...#     10001  = 0x11
    //     #...#     10001  = 0x11
    //     #.#.#     10101  = 0x15
    //     #.#.#     10101  = 0x15
    //     .#.#.     01010  = 0x0A
    { 'W', 6, 7, 1, { 0x00, 0x11, 0x11, 0x15, 0x15, 0x0A, 0x00, 0x00 } },
    // X:  #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     .#.       01000  = 0x08
    //     #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    { 'X', 4, 7, 1, { 0x00, 0x14, 0x14, 0x08, 0x14, 0x14, 0x00, 0x00 } },
    // Y:  #.#       10100  = 0x14
    //     #.#       10100  = 0x14
    //     .#.       01000  = 0x08
    //     .#.       01000  = 0x08
    //     .#.       01000  = 0x08
    { 'Y', 4, 7, 1, { 0x00, 0x14, 0x14, 0x08, 0x08, 0x08, 0x00, 0x00 } },
    // Z:  ###       11100  = 0x1C
    //     ..#       00100  = 0x04
    //     .#.       01000  = 0x08
    //     #..       10000  = 0x10
    //     ###       11100  = 0x1C
    { 'Z', 4, 7, 1, { 0x00, 0x1C, 0x04, 0x08, 0x10, 0x1C, 0x00, 0x00 } },

    // ===== Digits 0-9 =====
    // 5px wide for body shape; share advance width with letters.

    // 0:  .###.    01110  = 0x0E
    //     #..#.    10010  = 0x12
    //     #..#.    10010  = 0x12
    //     #..#.    10010  = 0x12
    //     .##..    01100  = 0x0C
    { '0', 5, 7, 1, { 0x00, 0x0E, 0x12, 0x12, 0x12, 0x0C, 0x00, 0x00 } },
    // 1:  .#...    01000  = 0x08
    //     ##...    11000  = 0x18
    //     .#...    01000  = 0x08
    //     .#...    01000  = 0x08
    //     ###..    11100  = 0x1C
    { '1', 4, 7, 1, { 0x00, 0x08, 0x18, 0x08, 0x08, 0x1C, 0x00, 0x00 } },
    // 2:  .##..    01100  = 0x0C
    //     #..#.    10010  = 0x12
    //     ..#..    00100  = 0x04
    //     .#...    01000  = 0x08
    //     ####.    11110  = 0x1E
    { '2', 5, 7, 1, { 0x00, 0x0C, 0x12, 0x04, 0x08, 0x1E, 0x00, 0x00 } },
    // 3:  ###..    11100  = 0x1C
    //     ...#.    00010  = 0x02
    //     .##..    01100  = 0x0C
    //     ...#.    00010  = 0x02
    //     ###..    11100  = 0x1C
    { '3', 5, 7, 1, { 0x00, 0x1C, 0x02, 0x0C, 0x02, 0x1C, 0x00, 0x00 } },
    // 4:  #..#.    10010  = 0x12
    //     #..#.    10010  = 0x12
    //     ####.    11110  = 0x1E
    //     ...#.    00010  = 0x02
    //     ...#.    00010  = 0x02
    { '4', 5, 7, 1, { 0x00, 0x12, 0x12, 0x1E, 0x02, 0x02, 0x00, 0x00 } },
    // 5:  ####.    11110  = 0x1E
    //     #....    10000  = 0x10
    //     ###..    11100  = 0x1C
    //     ...#.    00010  = 0x02
    //     ###..    11100  = 0x1C
    { '5', 5, 7, 1, { 0x00, 0x1E, 0x10, 0x1C, 0x02, 0x1C, 0x00, 0x00 } },
    // 6:  .##..    01100  = 0x0C
    //     #....    10000  = 0x10
    //     ###..    11100  = 0x1C
    //     #..#.    10010  = 0x12
    //     .##..    01100  = 0x0C
    { '6', 5, 7, 1, { 0x00, 0x0C, 0x10, 0x1C, 0x12, 0x0C, 0x00, 0x00 } },
    // 7:  ####.    11110  = 0x1E
    //     ...#.    00010  = 0x02
    //     ..#..    00100  = 0x04
    //     .#...    01000  = 0x08
    //     .#...    01000  = 0x08
    { '7', 5, 7, 1, { 0x00, 0x1E, 0x02, 0x04, 0x08, 0x08, 0x00, 0x00 } },
    // 8:  .##..    01100  = 0x0C
    //     #..#.    10010  = 0x12
    //     .##..    01100  = 0x0C
    //     #..#.    10010  = 0x12
    //     .##..    01100  = 0x0C
    { '8', 5, 7, 1, { 0x00, 0x0C, 0x12, 0x0C, 0x12, 0x0C, 0x00, 0x00 } },
    // 9:  .##..    01100  = 0x0C
    //     #..#.    10010  = 0x12
    //     .###.    01110  = 0x0E
    //     ...#.    00010  = 0x02
    //     .##..    01100  = 0x0C
    { '9', 5, 7, 1, { 0x00, 0x0C, 0x12, 0x0E, 0x02, 0x0C, 0x00, 0x00 } },

    // ===== Punctuation =====

    // ':'  ..       blank
    //      .#.      01000  = 0x08
    //      ...      blank
    //      .#.      01000  = 0x08
    //      ...      blank
    { ':', 3, 7, 1, { 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00 } },
    // '-'  ...      blank
    //      ...      blank
    //      ###.     11100  = 0x1C
    //      ...      blank
    //      ...      blank
    { '-', 4, 7, 1, { 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00 } },
    // '\''  #       10000  = 0x10
    //       #       10000  = 0x10
    //       .       blank
    //       .       blank
    //       .       blank
    { '\'', 2, 7, 1, { 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    // '('  .#.      01000  = 0x08
    //      #..      10000  = 0x10
    //      #..      10000  = 0x10
    //      #..      10000  = 0x10
    //      .#.      01000  = 0x08
    { '(', 3, 7, 1, { 0x00, 0x08, 0x10, 0x10, 0x10, 0x08, 0x00, 0x00 } },
    // ')'  #..      10000  = 0x10
    //      .#.      01000  = 0x08
    //      .#.      01000  = 0x08
    //      .#.      01000  = 0x08
    //      #..      10000  = 0x10
    { ')', 3, 7, 1, { 0x00, 0x10, 0x08, 0x08, 0x08, 0x10, 0x00, 0x00 } },
    // ','  ...      blank
    //      ...      blank
    //      ...      blank
    //      .#.      01000  = 0x08
    //      .#.      01000  = 0x08
    //      #..      10000  = 0x10  (descender)
    { ',', 3, 7, 1, { 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10, 0x00 } },
    // '!'  .#       01000  = 0x08
    //      .#       01000  = 0x08
    //      .#       01000  = 0x08
    //      ..       blank
    //      .#       01000  = 0x08
    { '!', 2, 7, 1, { 0x00, 0x08, 0x08, 0x08, 0x00, 0x08, 0x00, 0x00 } },
    // '?'  .##.     01100  = 0x0C
    //      #..#     10010  = 0x12
    //      ..#.     00100  = 0x04
    //      ....     blank
    //      ..#.     00100  = 0x04
    { '?', 5, 7, 1, { 0x00, 0x0C, 0x12, 0x04, 0x00, 0x04, 0x00, 0x00 } },
    // '/'  ...#     00010  = 0x02
    //      ..#.     00100  = 0x04
    //      ..#.     00100  = 0x04
    //      .#..     01000  = 0x08
    //      .#..     01000  = 0x08
    { '/', 4, 7, 1, { 0x00, 0x02, 0x04, 0x04, 0x08, 0x08, 0x00, 0x00 } },
    // '.'  ...      blank
    //      ...      blank
    //      ...      blank
    //      ...      blank
    //      .#.      01000  = 0x08
    { '.', 2, 7, 1, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00 } },

    // Sentinel: 'X' is reused above; this list is terminated by table size.
};

static constexpr size_t kSidebarGlyphCount = sizeof(kSidebarGlyphs) / sizeof(kSidebarGlyphs[0]);

const sSidebarSpecialGlyph* GetSidebarSpecialGlyph(uint8 pAscii) {
    for (size_t i = 0; i < kSidebarGlyphCount; ++i) {
        if (kSidebarGlyphs[i].mAscii == pAscii)
            return &kSidebarGlyphs[i];
    }
    return nullptr;
}
