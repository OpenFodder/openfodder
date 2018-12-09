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
#include "Versions_Files.hpp"

const std::vector<eTileTypes> Retail_Tile_Types = {
    eTileTypes_Jungle,
    eTileTypes_Desert,
    eTileTypes_Ice,
    eTileTypes_Moors,
    eTileTypes_Int
};

const std::vector<eTileTypes> PCFormat_TileTypes = {
    eTileTypes_Jungle,
    eTileTypes_Ice
};

const std::vector<eTileTypes> AmigaFormat_Tile_Types = {
    eTileTypes_AFX
};

const std::vector<eTileTypes> AmigaTheOne_TileTypes = {
    eTileTypes_Jungle,
    eTileTypes_Ice
};

const std::vector<eTileTypes> AmigaPlus_Tile_Types = {
    eTileTypes_Jungle
};

const std::vector<eTileTypes> AmigaNVF_Tile_Types = {
    eTileTypes_Desert,
    eTileTypes_Ice
};
const std::vector<eGFX_Types> Retail_GFX_Types = {
    eGFX_IN_GAME,	eGFX_IN_GAME2,
    eGFX_FONT,		eGFX_HILL,
    eGFX_RECRUIT,   eGFX_BRIEFING,
    eGFX_SERVICE,   eGFX_RANKFONT,
    eGFX_PSTUFF
};

const std::vector<eGFX_Types> Amiga_Demo_GFX_Types = {
    eGFX_IN_GAME,	eGFX_IN_GAME2,
    eGFX_PSTUFF
};

const std::vector<eGFX_Types> PCFormat_GFX_Types = {
    eGFX_IN_GAME,	eGFX_IN_GAME2,
    eGFX_FONT,      eGFX_HILL,
    eGFX_RECRUIT,   eGFX_BRIEFING,
    eGFX_SERVICE,   eGFX_RANKFONT,  
    eGFX_PSTUFF
};

/**
 * Known versions of Cannon Fodder
 */
const sGameVersion KnownGameVersions[] = {

    /* Retail */
    { "Cannon Fodder",					eGame::CF1, ePlatform::Amiga,	eRelease::Retail,	mIntroText_Amiga,	"Amiga",			mAmigaFiles,	 Retail_Tile_Types, Retail_GFX_Types },
    { "Cannon Fodder",					eGame::CF1, ePlatform::Amiga,	eRelease::Retail,	mIntroText_Amiga,	"Amiga_CD",			mAmigaCD32Files, Retail_Tile_Types, Retail_GFX_Types },
    { "Cannon Fodder",					eGame::CF1, ePlatform::PC,		eRelease::Retail,	mIntroText_PC,		"Dos_CD",			mPCFiles,		 Retail_Tile_Types, Retail_GFX_Types },
    { "Cannon Fodder 2",				eGame::CF2, ePlatform::Amiga,	eRelease::Retail,	mIntroText_Amiga2,	"Amiga2",			mAmiga2Files,	 Retail_Tile_Types, Retail_GFX_Types },
    { "Cannon Fodder 2",				eGame::CF2, ePlatform::PC,		eRelease::Retail,	mIntroText_PC2,		"Dos2_CD",			mPC2Files,		 Retail_Tile_Types, Retail_GFX_Types },

    /* Amiga Magazine Demos */
    { "Cannon Fodder Plus",				eGame::CF1, ePlatform::Amiga,	eRelease::AmigaPower,		    mIntroText_Amiga,	"Plus",				mPlusFiles,			AmigaPlus_Tile_Types, Amiga_Demo_GFX_Types },
    { "Amiga The One",                  eGame::CF1, ePlatform::Amiga,   eRelease::AmigaTheOne,          mIntroText_Amiga,   "AmigaTheOne",      mAmigaTheOneFiles,  AmigaTheOne_TileTypes, Amiga_Demo_GFX_Types },
    { "Amiga Action",                   eGame::CF1, ePlatform::Amiga,   eRelease::AmigaAction,          mIntroText_Amiga,   "AmigaAction",      mAmigaActionFiles,  AmigaPlus_Tile_Types, Amiga_Demo_GFX_Types},
    { "Amiga Format Christmas Special", eGame::CF1, ePlatform::Amiga,	eRelease::AmigaXMAS,		    mIntroText_Amiga,	"AmigaFormat_XMAS", mAmigaFormatFiles,	AmigaFormat_Tile_Types,  Amiga_Demo_GFX_Types },
    { "Amiga Format Not Very Festive",  eGame::CF2, ePlatform::Amiga,   eRelease::AmigaNotVeryFestive,  mIntroText_Amiga,   "AmigaFormat_NVF",      mAmigaFormat_NotVeryFestiveFiles,  AmigaPlus_Tile_Types, Amiga_Demo_GFX_Types},

    /* PC Demos */
    { "PC Format",                      eGame::CF1, ePlatform::PC,      eRelease::PCFormat, mIntroText_PC, "PCFormat", mPCFormatFiles, PCFormat_TileTypes, PCFormat_GFX_Types  },

    /* Custom & Random must be last, as they depend on a previous retail version being detected first */
    { "Single Map", eGame::CF1, ePlatform::Amiga,   eRelease::Custom,	mIntroText_PC,      "Custom", { }, { } },
    { "Single Map", eGame::CF1, ePlatform::PC,      eRelease::Custom,	mIntroText_PC,      "Custom", { }, { } },

    { "Random Map", eGame::CF1, ePlatform::Amiga,   eRelease::Custom,	mIntroText_PC,      "Custom", { }, { } },
    { "Random Map", eGame::CF1, ePlatform::PC,      eRelease::Custom,	mIntroText_PC,      "Custom", { }, { } },

};

cVersions::cVersions() {

    FindKnownVersions();
}

const sGameVersion* cVersions::GetRetail(const ePlatform pPlatform) const {

    auto RetailRelease =
        std::find_if(mAvailable.begin(), mAvailable.end(),
            [pPlatform](const sGameVersion* a)->bool { return a->mRelease == eRelease::Retail && 
                                                                (pPlatform == ePlatform::Any || a->mPlatform == pPlatform); });

    // If we didnt find the platform, revert to any
    if (RetailRelease == mAvailable.end()) {

        RetailRelease = std::find_if(mAvailable.begin(), mAvailable.end(),
                            [pPlatform](const sGameVersion* a)->bool { return a->mRelease == eRelease::Retail; });

        if (RetailRelease == mAvailable.end())
            return 0;
    }

    return *RetailRelease;
}

const sGameVersion* cVersions::GetDemo() const {

    if (!mAvailable.size())
        return 0;

    auto Release =
        std::find_if(mAvailable.begin(), mAvailable.end(),
            [](const sGameVersion* a)->bool { return a->mRelease == eRelease::AmigaXMAS; });

    if (Release == mAvailable.end()) {
        Release = mAvailable.begin();
    }


    return *Release;
}

const sGameVersion* cVersions::GetForTileset(eTileTypes pTileType) const {
    // Look through all available versions for a campaign name match
    for (auto& Version : mAvailable) {

        if (Version->hasTileset(pTileType))
            return Version;
    }

    return 0;
}

const sGameVersion* cVersions::GetForCampaign(const std::string& pCampaign) const {
    // Look through all available versions for a campaign name match
    for (auto& Version : mAvailable) {

        if (Version->mName == pCampaign)
            return Version;
    }

    return 0;
}

const sGameVersion* cVersions::GetForCampaign(const std::string& pCampaign, const ePlatform pPlatform) const {
    // Look through all available versions for a campaign name match
    for (auto& Version : mAvailable) {

        if (Version->mName == pCampaign && (Version->mPlatform == pPlatform || pPlatform == ePlatform::Any))
            return Version;
    }

    // Couldnt find one for requested platform
    return GetForCampaign(pCampaign);
}

/**
 * Is the campaign name associated with a data version
 */
bool cVersions::isCampaignKnown(const std::string& pName) const {
    for (auto& KnownVersion : KnownGameVersions) {
        // Is this campaign known?
        if (KnownVersion.mName == pName) {
            return true;
        }
    }

    return false;
}

/**
 * Is a campaign available
 */
bool cVersions::isCampaignAvailable(const std::string& pName) const {
    if (!isCampaignKnown(pName))
        return false;

    // Loop each available version
    for (auto& Version : mAvailable) {

        // Is the data available for it?
        if (Version->mName == pName)
            return true;
    }

    return false;
}

bool cVersions::isDataAvailable() const {
    return mAvailable.size() != 0; 
}

std::vector<std::string> cVersions::GetCampaignNames() const {
    std::vector<std::string> SortedList;

    for (auto& KnownVersion : KnownGameVersions) {
        for (auto& Version : mAvailable) {

            if (Version->mName == KnownVersion.mName) {
                // Already have this campaign?
                if (std::find(SortedList.begin(), SortedList.end(), Version->mName) != SortedList.end())
                    break;

                SortedList.push_back(KnownVersion.mName);
                break;
            }
        }
    }

    return SortedList;
}

void cVersions::FindKnownVersions() {
    mAvailable.clear();

    // Loop all known versions
    for (auto& KnownVersion : KnownGameVersions) {
        int16 FileMatches = 0;

        // Loop each file in this version
        for (auto& File : KnownVersion.mFiles) {
            std::string MD5 = local_FileMD5(File.mName, KnownVersion.mDataPath);

            if (MD5 != File.mChecksum) {

                if (MD5.length() == 0) {
                    //std::cout << KnownVersion.mName << ": " << KnownVersion.mFiles[FileNo].mName;
                    //std::cout << " File not found\n";
                } else {
                    //std::cout << "{ \"" << File.mName << "\", \"" << MD5 << "\" }, \n";
                    std::cout << KnownVersion.mName << ": " << File.mName;
                    std::cout << " Unknown MD5: " << MD5 << "\n";
                    ++FileMatches;
                }
            }
            else
                ++FileMatches;
        }

        // A very hacky method for ensuring a retail version is available, before allowing Customs
        if (KnownVersion.isCustom()) {
            if (GetRetail(ePlatform::Any)) {
                mAvailable.push_back(&KnownVersion);
            }
        }
        else {
            // Ensure we atleast have found 1 file, and we have atleast the reuqired number of files, or every file with an MD5 match
            if (KnownVersion.mFiles.size() > 0 && KnownVersion.mFiles.size() == FileMatches)
                mAvailable.push_back(&KnownVersion);
        }

    }
}

std::shared_ptr<cResources> sGameVersion::GetResources(const std::string& pDataPathOverride) const {
    if (isPC()) {
        if(isRetail())
            return std::make_shared<cResource_PC_CD>(pDataPathOverride, "CF_ENG.DAT");
        else
            return std::make_shared<cResource_PC_CD>(pDataPathOverride, "CFCOVER.DAT");

    } else if (isAmiga()) {
        return std::make_shared<cResource_Amiga_File>(pDataPathOverride);
    }
    return 0;
}

std::shared_ptr<cGraphics> sGameVersion::GetGraphics() const {
    if (isPC()) {
        return std::make_shared<cGraphics_PC>();
    }
    else if (isAmiga()) {

        if (isCannonFodder2())
            return std::make_shared<cGraphics_Amiga2>();

        return std::make_shared<cGraphics_Amiga>();
    }
    return 0;
}

std::shared_ptr<cSound> sGameVersion::GetSound() const {
    if (isPC()) {
        if (mGame == eGame::CF2)
            return std::make_shared<cSound_PC2>();

        return std::make_shared<cSound_PC>();
    }
    else if (isAmiga()) {
        return std::make_shared<cSound_Amiga>();
    }
    return 0;
}