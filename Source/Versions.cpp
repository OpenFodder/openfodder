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

#include "stdafx.hpp"
#include "Versions_Files.hpp"

const std::vector<sTileset> Retail_Tile_Types = {
	{ eTileTypes_Jungle,	{ eTileSub_0, eTileSub_1 } },
    { eTileTypes_Desert,	{ eTileSub_0 } },
    { eTileTypes_Ice,		{ eTileSub_0 } },
    { eTileTypes_Moors,		{ eTileSub_0 } },
    { eTileTypes_Int,		{ eTileSub_0 } },
};

const std::vector<sTileset> PCFormat_TileTypes = {
	{ eTileTypes_Jungle,	{ eTileSub_0, eTileSub_1 } },
	{ eTileTypes_Ice,		{ eTileSub_0 } },
};

const std::vector<sTileset> AmigaFormat_Tile_Types = {
	{ eTileTypes_AFX,		{ eTileSub_0 } },
};

const std::vector<sTileset> AmigaTheOne_TileTypes = {
	{ eTileTypes_Jungle,	{ eTileSub_0, eTileSub_1 } },
	{ eTileTypes_Ice,		{ eTileSub_0 } },
};

const std::vector<sTileset> AmigaPlus_Tile_Types = {
	{ eTileTypes_Jungle,	{ eTileSub_1 } },
};

const std::vector<sTileset> AmigaAction_Tile_Types = {
	{ eTileTypes_Jungle,	{ eTileSub_0, eTileSub_1 } },
};

const std::vector<sTileset> AmigaNVF_Tile_Types = {
	{ eTileTypes_Desert,	{ eTileSub_0 } },
	{ eTileTypes_Ice,		{ eTileSub_0 } },
};

const std::vector<sTileset> AmigaAL_Tile_Types = {
	{ eTileTypes_Int,	{ eTileSub_0 } },
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
	{ "Cannon Fodder",					eGame::CF1, ePlatform::Amiga,	eRelease::Retail,	"Amiga",	mAmigaFiles },
	{ "Cannon Fodder",					eGame::CF1, ePlatform::Amiga,	eRelease::Retail,	"Amiga_CD",	mAmigaCD32Files },
	{ "Cannon Fodder",					eGame::CF1, ePlatform::PC,		eRelease::Retail,	"Dos_CD",	mPCFiles },
	{ "Cannon Fodder 2",				eGame::CF2, ePlatform::Amiga,	eRelease::Retail,	"Amiga2",	mAmiga2Files },
	{ "Cannon Fodder 2",				eGame::CF2, ePlatform::PC,		eRelease::Retail,	"Dos2_CD",	mPC2Files },

	/* Amiga Magazine Demos */
	{ "Cannon Fodder Plus",				eGame::CF1, ePlatform::Amiga,	eRelease::AmigaPower,		    "Plus",				mPlusFiles },
	{ "Amiga The One",                  eGame::CF1, ePlatform::Amiga,   eRelease::AmigaTheOne,          "AmigaTheOne",      mAmigaTheOneFiles },
	{ "Amiga Action",                   eGame::CF1, ePlatform::Amiga,   eRelease::AmigaAction,          "AmigaAction",      mAmigaActionFiles },
	{ "Amiga Power Alien Levels",		eGame::CF2, ePlatform::Amiga,	eRelease::AmigaAlienLevels,		"AmigaAlienLevels", mAmigaAlienLevels },
	{ "Amiga Format Christmas Special", eGame::CF1, ePlatform::Amiga,	eRelease::AmigaXMAS,		    "AmigaFormat_XMAS", mAmigaFormatFiles },
	{ "Amiga Format Not Very Festive",  eGame::CF2, ePlatform::Amiga,   eRelease::AmigaNotVeryFestive,  "AmigaFormat_NVF",  mAmigaFormat_NotVeryFestiveFiles },

	/* PC Demos */
	{ "PC Format",                      eGame::CF1, ePlatform::PC,      eRelease::PCFormat, "PCFormat", mPCFormatFiles },

	/* Custom & Random must be last, as they depend on a previous retail version being detected first */
	{ "Single Map", eGame::CF1, ePlatform::Amiga,   eRelease::Custom,	"Custom", { } },
	{ "Single Map", eGame::CF1, ePlatform::PC,      eRelease::Custom,	"Custom", { } },
	{ "Single Map", eGame::CF2, ePlatform::Amiga,   eRelease::Custom,	"Custom", { } },
	{ "Single Map", eGame::CF2, ePlatform::PC,      eRelease::Custom,	"Custom", { } },

	{ "Random Map", eGame::CF1, ePlatform::Amiga,   eRelease::eRandom,	"Custom", { } },
	{ "Random Map", eGame::CF1, ePlatform::PC,      eRelease::eRandom,	"Custom", { } },
	{ "Random Map", eGame::CF2, ePlatform::Amiga,   eRelease::eRandom,	"Custom", { } },
	{ "Random Map", eGame::CF2, ePlatform::PC,      eRelease::eRandom,	"Custom", { } },

};

const sGameVersion* cVersions::GetRetail(const ePlatform pPlatform, const eGame pGame) const {

	auto Available = g_ResourceMan->GetAvailable();

    auto RetailRelease =
        std::find_if(Available.begin(), Available.end(),
            [pPlatform, pGame](const sGameVersion* a)->bool { return a->mRelease == eRelease::Retail && a->mGame == pGame &&
                                                                (pPlatform == ePlatform::Any || a->mPlatform == pPlatform); });

    // If we didnt find the platform, revert to any
    if (RetailRelease == Available.end()) {

        RetailRelease = std::find_if(Available.begin(), Available.end(),
                            [](const sGameVersion* a)->bool { return a->mRelease == eRelease::Retail; });

        if (RetailRelease == Available.end())
            return 0;
    }

    return *RetailRelease;
}

const sGameVersion* cVersions::GetDemo() const {
	auto Available = g_ResourceMan->GetAvailable();

    if (!Available.size())
        return 0;

    auto Release =
        std::find_if(Available.begin(), Available.end(),
            [](const sGameVersion* a)->bool { return a->mRelease == eRelease::AmigaXMAS; });

    if (Release == Available.end()) {
        Release = Available.begin();
    }


    return *Release;
}

const sGameVersion* cVersions::GetForTileset(eTileTypes pTileType, eTileSub pSub) const {
    // Look through all available versions for a campaign name match
    for (auto& Version : g_ResourceMan->GetAvailable()) {

        if (Version->hasTileset(pTileType, pSub))
            return Version;
    }

    return 0;
}

const sGameVersion* cVersions::GetForCampaign(const std::string& pCampaign) const {
    // Look through all available versions for a campaign name match
    for (auto& Version : g_ResourceMan->GetAvailable()) {

        if (Version->mName == pCampaign)
            return Version;
    }

    return 0;
}

const sGameVersion* cVersions::GetForCampaign(const std::string& pCampaign, const ePlatform pPlatform) const {
    // Look through all available versions for a campaign name match
    for (auto& Version : g_ResourceMan->GetAvailable()) {

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
    for (auto& Version : g_ResourceMan->GetAvailable()) {

        // Is the data available for it?
        if (Version->mName == pName)
            return true;
    }

    return false;
}

std::vector<std::string> cVersions::GetCampaignNames() const {
    std::vector<std::string> SortedList;

    for (auto& KnownVersion : KnownGameVersions) {
        for (auto& Version : g_ResourceMan->GetAvailable()) {

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

std::shared_ptr<cResources> sGameVersion::GetResources() const {
    if (isPC()) {
        if(isPCFormat())
            return std::make_shared<cResource_PC_CD>("CFCOVER.DAT");
		else
			return std::make_shared<cResource_PC_CD>("CF_ENG.DAT");

    } else if (isAmiga()) {
        return std::make_shared<cResources>();
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

bool sGameVersion::CanUseAmigaSound() const {
	if (g_ResourceMan->FileExists(getDataPath() + "JON.INS"))
		return true;
	return false;
}

std::shared_ptr<cSound> sGameVersion::GetSound() const {

	// Check for JON.INS in the game data folder, this allows replacing PC audio with Amiga for both CF1 and CF2
	if (CanUseAmigaSound())
		return std::make_shared<cSound_Amiga>();

    if (isPC()) {
		if (isCannonFodder2())
			return std::make_shared<cSound_PC2>();

		return std::make_shared<cSound_PC>();
    }
    else if (isAmiga()) {
        return std::make_shared<cSound_Amiga>();
    }
    return 0;
}

std::string sGameVersion::getDataPath() const {
	
	if (isCustom()) {
		return g_Fodder->mVersions->GetRetail(ePlatform::Any, mGame)->getDataPath();
	}

	return g_ResourceMan->FindVersionPath(this);
}

std::string sGameVersion::getDataFilePath(std::string pFile) const {

	// First try get the file from found files in the data folder
	auto path = g_ResourceMan->GetFilePath(this, pFile);
	if (path.size())
		return path;

	// If its custom, check the retail data
	if (isCustom()) {
		path = g_Fodder->mVersions->GetRetail(mPlatform, mGame)->getDataFilePath(pFile);
		if (path.size() && g_ResourceMan->FileExists(path))
			return path;
	}

	// Fall back to the data folder
	return g_ResourceMan->FindVersionPath(this) + pFile;
}

std::vector<eGFX_Types> sGameVersion::getGfxTypes() const {
	switch (mRelease) {
	case eRelease::Retail:
		return Retail_GFX_Types;

	case eRelease::PCFormat:
		return PCFormat_GFX_Types;

	case eRelease::Custom:
		return {};

		// All other releases 
	default:
		return Amiga_Demo_GFX_Types;
	}

}

std::vector<sTileset> sGameVersion::getTileTypes() const {

	switch (mRelease) {
	case eRelease::Retail:
		return Retail_Tile_Types;
		
	case eRelease::PCFormat:
		return PCFormat_TileTypes;
		
	case eRelease::AmigaPower:
		return AmigaPlus_Tile_Types;
		
	case eRelease::AmigaTheOne:
		return AmigaTheOne_TileTypes;
		
	case eRelease::AmigaAction:
		return AmigaAction_Tile_Types;
		
	case eRelease::AmigaAlienLevels:
		return AmigaAL_Tile_Types;
		
	case eRelease::AmigaXMAS:
		return AmigaFormat_Tile_Types;
		
	case eRelease::AmigaNotVeryFestive:
		return AmigaNVF_Tile_Types;

	default:
	case eRelease::Custom:
		return {};
	}
}

std::vector<sIntroText>* sGameVersion::getIntroData() const {
	if (isAmiga()) {
		if (isCannonFodder1())
			return &mIntroText_Amiga;
		else
			return &mIntroText_Amiga2;
	}
	else {
		if (isCannonFodder1())
			return &mIntroText_PC;
		else
			return &mIntroText_PC2;
	}

}