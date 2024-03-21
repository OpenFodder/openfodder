/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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

const uint8 byte_449BD[] = {
	0xFD, 0xBA, 0x0D, 0xFF
};

const uint8 mCopyProtection_Word_Your2[] = {
	0xFD, 0xB0, 0x1A, 0x44, 0xFF
};

const uint8 mCopyProtection_Word_Extraneous[] = {
	0xFD, 0xA5, 0x21, 0x60, 0x6F, 0x39, 0xA8,
	0xE5, 0xF1, 0xB0, 0xFF
};

const uint8 mCopyProtection_Word_Cannon[] = {
	0xFD, 0xBE, 0x33, 0x76, 0x77, 0x39, 0xFF
};

const uint8 mCopyProtection_Word_Send[] = {
	0xFD, 0xBA, 0x13, 0x4C, 0xFF
};

const uint8 mCopyProtection_Word_You2[] = {
	0xFD, 0xB0, 0x1A, 0xFF
};

const uint8 mCopyProtection_Word_Your[] = {
	0xFD, 0xB0, 0x1A, 0x44, 0xFF
};

const uint8 mCopyProtection_Word_Follow[] = {
	0xFB, 0xB2, 0x37, 0x7D, 0x60, 0x3E, 0xFF
};

const uint8 mCopyProtection_Word_Types[] = {
	0xF7, 0xA0, 0x07, 0x48, 0x56, 0xFF
};

const uint8 byte_449F3[] = {
	0xF7, 0xA9, 0x16, 0x42, 0x53, 0x14, 0xE1, 0xB6, 0xFF
};

const uint8 mCopyProtection_Word_Times[] = {
	0xF7, 0xB0, 0x14, 0x48, 0x56, 0xFF
};

const uint8 mCopyProtection_Word_You[] = {
	0xFD, 0xB0, 0x1A, 0xFF
};

const uint8 mCopyProtection_Word_Saving[] = {
	0xFD, 0xBE, 0x0B, 0x3F, 0x48, 0x12, 0xFF
};

const uint8 byte_44A0D[] = {
	0xFB, 0xB2, 0x25, 0x7E, 0x6E, 0x3B, 0xFF
};

const uint8 mCopyProtection_Word_Pay[] = {
	0xDF, 0xA0, 0x38, 0xFF
};

/*
 * List of pages / words from the manual
 *
 * Page / Paragraph / Line / Word / Answer
 */
const sCopyProtection mCopyProtection_Values[] = {
	{ 4, 2, 1, 1, byte_449BD },
	{ 4, 3, 2, 1, mCopyProtection_Word_Your2 },
	{ 5, 1, 2, 1, mCopyProtection_Word_Extraneous },
	{ 5, 2, 1, 2, mCopyProtection_Word_Cannon },
	{ 5, 3, 1, 1, mCopyProtection_Word_Send },
	{ 6, 1, 1, 2, mCopyProtection_Word_You2 },
	{ 6, 2, 3, 2, mCopyProtection_Word_Your },
	{ 6, 3, 3, 2, mCopyProtection_Word_Follow },
	{ 9, 1, 1, 2, mCopyProtection_Word_Types },
	{ 9, 1, 2, 1, byte_449F3 },
	{ 10, 1, 1, 3, mCopyProtection_Word_Times },
	{ 11, 1, 1, 4, mCopyProtection_Word_You },
	{ 11, 2, 1, 1, mCopyProtection_Word_Saving },
	{ 11, 4, 1, 4, byte_44A0D },
	{ 15, 1, 1, 1, mCopyProtection_Word_Pay }
};

void cFodder::CopyProtection() {

    // Only DOS CD had copy protection
    if (!mVersionCurrent->isCannonFodder1())
        return;

    if (!mVersionCurrent->isRetail())
        return;

    if (!mVersionCurrent->isPC())
        return;

    if (!mParams->mCopyProtection)
        return;

    mGraphics->SetActiveSpriteSheet(eGFX_Types::eGFX_RECRUIT);

    // 3 Attempts
    for (int16 Attempts = 0; Attempts < 3; ++Attempts) {

        int16 Data0;
        do {
            Data0 = tool_RandomGet() & 0x0F;
        } while (Data0 == 0x0F);

        mSurface->clearBuffer();
        mGraphics->PaletteSet();

        const sCopyProtection* word_44A1C = &mCopyProtection_Values[Data0];

        std::string Page = "PAGE " + std::to_string(word_44A1C->mPage);
        std::string Paragraph = "PARAGRAPH " + std::to_string(word_44A1C->mParagraph);
        std::string Line = "LINE " + std::to_string(word_44A1C->mLine);
        std::string Word = "WORD " + std::to_string(word_44A1C->mWord);

        GUI_Render_Text_Centred("ENTER WORD FROM", 0);
        GUI_Render_Text_Centred("MANUAL AT", 0x14);
        GUI_Render_Text_Centred(Page.c_str(), 0x3C);
        GUI_Render_Text_Centred(Paragraph.c_str(), 0x50);
        GUI_Render_Text_Centred(Line.c_str(), 0x64);
        GUI_Render_Text_Centred(Word.c_str(), 0x78);

        int8 mCursorBlinkTimer = 0;
        bool mShow = false;

        mGUI_Temp_X = 5;
        mGUI_Temp_Width = 10;
        mInput.clear();

        mSurface->Save();
        mKeyCodeAscii = 0;

        while (mKeyCodeAscii != 0x0D) {

            if (mSurface->isPaletteAdjusting())
                mSurface->palette_FadeTowardNew();

            String_Input_Print(0xA0);

            ++mCursorBlinkTimer;
            mCursorBlinkTimer &= 0x0F;
            if (!mCursorBlinkTimer)
                mShow = !mShow;

            if (mShow)
                GUI_Draw_Frame_8(0x0F, 0x00, (mGUI_Temp_X + mGUI_Temp_Width), 0xA0);

            Video_Sleep();
        }

        Image_FadeOut();

        CopyProtection_EncodeInput();

        const uint8* Answer = word_44A1C->mAnswer;
        bool Failed = false;

        for (size_t Pos = 0; Pos < mInput.size() && *Answer != 0xFF; ++Pos) {

            if ((uint8)mInput[Pos] != *Answer++) {
                Failed = true;
                break;
            }
        }

        if (!Failed && (*Answer == 0xFF))
            return;
    }

    std::cout << "Copy protection failed.\n";
    Exit(1);
}

void cFodder::CopyProtection_EncodeInput() {
    int16 bx = mInput[0] & 0xFF;
    int16 cx = 0;

    for (auto& Character : mInput) {
        cx -= bx;

        if (Character == -1)
            return;

    rollLoop:;

        bx = (bx << 8) | (bx >> 8);

        Character ^= (bx & 0xFF);
        Character ^= (cx & 0xFF);
        --Character;

        if (Character == -1)
            goto rollLoop;
    }
}
