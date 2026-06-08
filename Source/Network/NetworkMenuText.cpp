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
#include "NetworkMenuText.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

bool NetworkMenu_UInt32FromText(const std::string& pText, uint32& pValue)
{
    if (pText.empty())
        return false;

    uint64_t Value = 0;
    for (char Char : pText) {
        if (Char < '0' || Char > '9')
            return false;

        Value = (Value * 10) + (uint64_t)(Char - '0');
        if (Value > 0xFFFFFFFFu)
            return false;
    }

    pValue = (uint32)Value;
    return true;
}

std::string NetworkMenu_FitText(const std::string& pText, int pMaxPx)
{
    std::string Result;
    int Width = 0;

    for (char RawChar : pText) {
        unsigned char Char = (unsigned char)std::toupper((unsigned char)RawChar);
        if (!((Char >= 'A' && Char <= 'Z') || (Char >= '0' && Char <= '9') || Char == ' '))
            Char = ' ';

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

int NetworkMenu_DrawWrappedBody(const std::string& pText, int pStartY, int pMaxWidth)
{
    // Trivial word-wrap: split on whitespace, build lines under pMaxWidth pixels
    // using the briefing font width table. Lifted from cSetupWizard so the
    // multiplayer host-setup error banner can wrap long curl messages instead
    // of substr-truncating them.
    auto wordWidth = [](const std::string& word) {
        std::string upper = word;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        int w = 0;
        for (unsigned char c : upper)
            w += (int)mFont_Briefing_Width[c];
        return w;
    };

    std::vector<std::string> lines;
    std::string current;
    int currentW = 0;
    const int spaceW = wordWidth(" ");

    auto flush = [&]() {
        if (!current.empty()) {
            lines.push_back(current);
            current.clear();
            currentW = 0;
        }
    };

    std::stringstream ss(pText);
    std::string word;
    while (ss >> word) {
        const int wW = wordWidth(word);
        if (current.empty()) {
            current = word;
            currentW = wW;
        } else if (currentW + spaceW + wW <= pMaxWidth) {
            current += ' ';
            current += word;
            currentW += spaceW + wW;
        } else {
            flush();
            current = word;
            currentW = wW;
        }
    }
    flush();

    int y = pStartY;
    const int lineH = 0x0A;
    for (auto& line : lines) {
        g_Fodder->String_Print_Small_CentreInBox(line, 8, 312, y);
        y += lineH;
    }
    return y;
}
