/*
 *  Cannon Fodder
 *  ------------------------
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

enum ePlatform {
	PC		= 0,
	Amiga	= 1
};

enum eRelease {
	Retail	= 0,
	Retail2 = 1,
	Demo	= 2
};

enum eVersion {
	Dos_CD		= 0,
	Dos2_CD		= 1,
	Amiga_Disk	= 2,
	Amiga_CD	= 3,
	AmigaFormat = 4,
	AmigaPlus	= 5,
    Custom      = 6,
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

struct sFile {
	const char* mName;
	const char* mChecksum;
};

struct sVersion {
	const std::string		mName;
	const std::string		mCampaignDefault;
	eGame					mGame;
	eVersion				mVersion;
	ePlatform				mPlatform;
	eRelease				mRelease;

	const sIntroText*		mIntroData;
	const char*				mDataPath;
	const sFile*			mFiles;


	bool isCustom() const {

		return mVersion == eVersion::Custom;
	}
};

extern std::vector<sVersion> Versions;
extern std::vector<const sVersion*> g_AvailableDataVersions;

bool isCampaignKnown(const std::string& pName);
bool IsCampaignDataAvailable(const std::string& pName);

std::vector<std::string> GetAvailableVersions();
void FindFodderVersions();
const sVersion* FindAvailableVersionForCampaign(const std::string& pCampaign);
