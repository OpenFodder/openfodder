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

#include <cctype>

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
