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
	PC		= 0,
	Amiga	= 1
};

enum eRelease {
	Retail		= 0,
	Demo		= 1,
	AmigaFormat = 2,
	AmigaPower  = 3,
	Custom		= 4
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

struct sRelease {
	std::string			mName;

	eGame				mGame;
	ePlatform			mPlatform;
	eRelease			mRelease;
};

struct sGameVersion {
	const std::string	mName;

	const eGame				mGame;
	const ePlatform			mPlatform;
	const eRelease			mRelease;

	const std::vector<sIntroText>	mIntroData;
	const std::string	            mDataPath;
	const std::vector<sFile>		mFiles;

	const std::vector<eTileTypes>	mTileTypes;
	const std::vector<eGFX_Types>   mGfxTypes;


	bool hasGfx(eGFX_Types pGfxType) const {

		return std::find(mGfxTypes.begin(), mGfxTypes.end(), pGfxType) != mGfxTypes.end();
	}

	bool hasTileset(eTileTypes pTileType) const {

		return std::find(mTileTypes.begin(), mTileTypes.end(), pTileType) != mTileTypes.end();
	}

	bool isCustom() const {

		return mRelease == eRelease::Custom;
	}

	bool isDemo() const {
		return  mRelease == eRelease::Demo || 
				mRelease == eRelease::AmigaFormat || 
				mRelease == eRelease::AmigaPower ||
				mRelease == eRelease::Custom;
	}

	bool isRetail() const {
		return mRelease == eRelease::Retail;
	}

	bool isAmigaFormat() const {
		return mRelease == eRelease::AmigaFormat;
	}

	bool isAmigaPower() const {
		return mRelease == eRelease::AmigaPower;
	}

	bool isAmiga() const {
		return mPlatform == ePlatform::Amiga;
	}

	bool isPC() const {
		return mPlatform == ePlatform::PC;
	}

};

class cVersions {

    std::vector<const sGameVersion*> mAvailable;

protected:
    void FindKnownVersions();

public:
    cVersions();

    bool isCampaignKnown(const std::string& pName) const;
    bool isCampaignDataAvailable(const std::string& pName) const;
    bool isDataAvailable() const;

    std::vector<std::string> GetAvailableNames() const;

    const sGameVersion* GetForCampaign(const std::string& pCampaign) const;
    const sGameVersion* GetForCampaign(const std::string& pCampaign, const ePlatform pPlatform) const;
    const sGameVersion* GetForTileset(eTileTypes pTileType) const;
    const sGameVersion* GetRetail() const;
};
