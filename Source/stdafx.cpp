/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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

std::shared_ptr<cResources> g_Resource;
std::shared_ptr<cWindow>    g_Window;
std::shared_ptr<cFodder>    g_Fodder;
std::shared_ptr<cDebugger>  g_Debugger;
std::shared_ptr<cResourceMan> g_ResourceMan;
std::shared_ptr<cScriptingEngine> g_ScriptingEngine;

const char gPathSeperator = '/';

int main(int argc, char *argv[]) {
	auto result = start(argc, argv);
	if (result == -1) {
		std::cout << "Press enter to continue\n";
		std::cin.get();
	}

	return result;
}

/**
 *
 * @param pBuffer
 * @param pSize Number of words
 */
void tool_EndianSwap(uint8 *pBuffer, size_t pSize) {
	uint8 *pDest = pBuffer;

	pSize /= 2;

	while (pSize--) {
		uint8 al = *pBuffer++;
		uint8 ah = *pBuffer++;

		*pDest++ = ah;
		*pDest++ = al;
	}
}

std::string tool_StripLeadingZero(const std::string& pValue) {
	std::string Final = pValue;

	while (*Final.begin() == 0x30 && Final.length() > 1) {

		Final.erase(Final.begin());
	}

	return Final;
}

uint16 tool_DecimalToBinaryCodedDecimal(uint16 pDecimal) {

	return ((pDecimal / 10) << 4) + (pDecimal % 10);
}
