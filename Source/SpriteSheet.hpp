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

struct sSpriteSheet {
	uint16 mLoadOffset;
	uint16 mLoadSegment;
	int16 field_4;
	int16 field_6;
	int16 mColCount;
	int16 mRowCount;
	int16 mPalleteIndex;
	int8 mModX;
	int8 mModY;

	uint8* GetGraphicsPtr() const;
};

struct sSpriteSheet_pstuff {
	int16	mX;
	int16	mY;
	int16	mColumns;
	int16	mRows;

	uint8* GetGraphicsPtr(uint16 pOffset) const;
};