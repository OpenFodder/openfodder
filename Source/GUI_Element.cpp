/*
 *  Cannon Fodder
 *  ------------------------
 *
 *  Copyright (C) 2008-2015 Robert Crossfield
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

/* AFX Menu Buttons */
const sGUI_Element mCoverDisk_Buttons[] = {
	{ &cFodder::sub_2EAC2, 0x6B, 0x6B, 0x4A, 0x6B, &cFodder::sub_2EAC3 },
	{ &cFodder::sub_2EAC2, 0x01, 0x9E, 0x1B, 0x63, &cFodder::sub_A03EE },
	{ &cFodder::sub_2EAC2, 0xA1, 0x9C, 0x1B, 0x63, &cFodder::sub_A0400 },
	{ &cFodder::sub_2EAC2, 0x02, 0x9D, 0x81, 0x63, &cFodder::sub_A03EE },
	{ &cFodder::sub_2EAC2, 0xA1, 0x9D, 0x81, 0x63, &cFodder::sub_A0400 },
	{ 0 }
};

/* Cannon Fodder Plus Menu Buttons */
const sGUI_Element mPlus_Buttons[] = {
	{ &cFodder::sub_2EAC2, 0x6B, 0x6B, 0x4A, 0x6B, &cFodder::sub_A03E0 },
	{ &cFodder::sub_2EAC2, 0x01, 0x9E, 0x1B, 0x63, &cFodder::sub_A03EE },
	{ &cFodder::sub_2EAC2, 0xA1, 0x9C, 0x1B, 0x63, &cFodder::sub_A0400 },
	{ &cFodder::sub_2EAC2, 0x02, 0x9D, 0x81, 0x63, &cFodder::sub_9BA08 },
	{ &cFodder::sub_2EAC2, 0xA1, 0x9D, 0x81, 0x63, &cFodder::sub_9BA1A },
};

/* Cannon Fodder Plus Quiz Buttons */
const sGUI_Element mPlusQuiz_Buttons[] = {
	{ &cFodder::sub_2EAC2, 0x0BD, 0x19, 0x5B, 0x48,	&cFodder::sub_A056E },
	{ &cFodder::sub_2EAC2, 0x0B2, 0x1C, 0xA2, 0x33,	&cFodder::sub_A056E },
	{ &cFodder::sub_2EAC2, 0x000, 0x31, 0x03, 0x1E,	&cFodder::sub_A0578 },
	{ &cFodder::sub_2EAC2, 0x10C, 0x31, 0x03, 0x1E,	&cFodder::sub_A058C },
	{ &cFodder::sub_2EAC2, 0x000, 0x31, 0xD6, 0x1E,	&cFodder::sub_A05A0 },
	{ &cFodder::sub_2EAC2, 0x10C, 0x31, 0xD6, 0x1E,	&cFodder::sub_A05B4 },
	{ &cFodder::sub_2EAC2, 0x02A, 0x32, 0x10, 0xC9,	&cFodder::sub_A05C8 },
	{ &cFodder::sub_2EAC2, 0x05B, 0x27, 0x1F, 0xB4,	&cFodder::sub_A05DC },
	{ &cFodder::sub_2EAC2, 0x081, 0x1E, 0x25, 0xA7,	&cFodder::sub_A05F0 },
	{ &cFodder::sub_2EAC2, 0x09E, 0x1F, 0x24, 0xB1,	&cFodder::sub_A0604 },
	{ &cFodder::sub_2EAC2, 0x0BC, 0x2B, 0x21, 0xAA,	&cFodder::sub_A0618 },
	{ &cFodder::sub_2EAC2, 0x0E6, 0x3B, 0x13, 0xCE,	&cFodder::sub_A062C },
};
