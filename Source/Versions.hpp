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

enum Platform {
	PC	= 0,
	Amiga	= 1
};

struct sVersion {
	const char* mName;
	std::string mKey;
	Platform	mPlatform;
	const char* mDataPath;
};

const sVersion Versions[3] = {
	{ "",								"Dos",	 Platform::PC,		"" },
	{ "",								"Amiga", Platform::Amiga,	"Amiga" },
	{ "Amiga Format Christmas Special",	"AFX",	 Platform::Amiga,	"AmigaFormat_XMAS" }
};
