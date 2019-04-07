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

enum ePlatform {
	Any = 0,
	PC = 1,
	Amiga = 2
};

enum eRelease {
	Retail = 0,
	PCFormat,
	AmigaXMAS,
	AmigaPower,
	AmigaTheOne,
	AmigaAction,
	AmigaNotVeryFestive,
	AmigaAlienLevels,

	Custom
};

enum eGame {
	CF1 = 0,
	CF2 = 1
};

enum eCustomMode {
	eCustomMode_None = 0,
	eCustomMode_Map = 1,
	eCustomMode_Set = 2
};

enum eGFX_Types {
	eGFX_IN_GAME = 0,
	eGFX_IN_GAME2 = 1,
	eGFX_FONT = 2,
	eGFX_HILL = 3,
	eGFX_RECRUIT = 4,
	eGFX_BRIEFING = 5,
	eGFX_SERVICE = 6,

	eGFX_RANKFONT = 7,
	eGFX_PSTUFF = 8,

	eGFX_BRIEFING_AMIGA_1 = 9,
	eGFX_BRIEFING_AMIGA_2 = 10,
	eGFX_BRIEFING_AMIGA_3 = 11
};

enum eTileTypes {
	eTileTypes_Jungle = 0,
	eTileTypes_Desert = 1,
	eTileTypes_Ice = 2,
	eTileTypes_Moors = 3,
	eTileTypes_Int = 4,
	eTileTypes_Hid = 5,
	eTileTypes_AFX = 6,	// Amiga Format Christmas Special
};

enum eTileSub {
	eTileSub_0 = 0,
	eTileSub_1 = 1
};

struct sTileset {
	eTileTypes Type;
	std::vector<eTileSub> Subs;
};

struct sVersion {
	std::string			mName;

	eGame				mGame;
	ePlatform			mPlatform;
	eRelease			mRelease;

	sVersion() {
		mName = "";
		mGame = eGame::CF1;
		mPlatform = ePlatform::Any;
		mRelease = eRelease::Retail;
	}

	sVersion(const std::string& pName, eGame pGame, ePlatform pPlatform, eRelease pRelease) {
		mName = pName;
		mGame = pGame;
		mPlatform = pPlatform;
		mRelease = pRelease;
	}

	bool isCannonFodder1() const {
		return mGame == eGame::CF1;
	}

	bool isCannonFodder2() const {
		return mGame == eGame::CF2;
	}

	bool isCustom() const {
		return mRelease == eRelease::Custom;
	}

	bool isRetail() const {
		return mRelease == eRelease::Retail;
	}

	bool isPCFormat() const {
		return mRelease == eRelease::PCFormat;
	}

	bool isAmigaXmas() const {
		return mRelease == eRelease::AmigaXMAS;
	}

	bool isAmigaPower() const {
		return mRelease == eRelease::AmigaPower;
	}

	bool isAmigaTheOne() const {
		return mRelease == eRelease::AmigaTheOne;
	}

	bool isAmigaAction() const {
		return mRelease == eRelease::AmigaAction;
	}

	bool isAmigaNotVeryFestive() const {
		return mRelease == eRelease::AmigaNotVeryFestive;
	}

	bool isAmigaAlienLevels() const {
		return mRelease == eRelease::AmigaAlienLevels;
	}

	bool isAmiga() const {
		return mPlatform == ePlatform::Amiga;
	}

	bool isPC() const {
		return mPlatform == ePlatform::PC;
	}

	/**
	 * Is this version a demo?
	 *
	 * NOTE: The PC-Format version is not considered a demo, as it is very close to the dos retail
	 */
	bool isDemo() const {
		return 	mRelease == eRelease::AmigaXMAS ||
				mRelease == eRelease::AmigaPower ||
				mRelease == eRelease::AmigaTheOne ||
				mRelease == eRelease::AmigaAction ||
				mRelease == eRelease::AmigaNotVeryFestive ||
				mRelease == eRelease::AmigaAlienLevels;
	}

	bool isCoverDisk() const {

		return (isAmigaPower() || isAmigaAction() || isAmigaTheOne());
	}
};
