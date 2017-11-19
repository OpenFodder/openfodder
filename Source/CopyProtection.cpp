/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2017 Robert Crossfield
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
