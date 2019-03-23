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

struct sFile {
	const char* mName;
	const char* mChecksum;
};

class cResources;
class cGraphics;
class cSound;

struct sGameVersion : public sVersion {

	const std::string				mDataPath;
	const std::vector<sFile>		mFiles;

	sGameVersion(const std::string& pName, eGame pGame, ePlatform pPlatform, eRelease pRelease, const std::string& pDataPath, const std::vector<sFile>& pFiles) :
		sVersion(pName,pGame,pPlatform,pRelease), mDataPath(pDataPath), mFiles(pFiles) {
	
	}

	bool hasGfx(eGFX_Types pGfxType) const {
		auto gfx = getGfxTypes();
		return std::find(gfx.begin(), gfx.end(), pGfxType) != gfx.end();
	}

	bool hasTileset(eTileTypes pTileType, eTileSub pSub = eTileSub::eTileSub_0 ) const {
		auto tiles = getTileTypes();

		return std::find_if(tiles.begin(), tiles.end(),
			[pTileType, pSub](const sTileset& ms) {
				if (ms.Type != pTileType)
					return false;

				return std::find(ms.Subs.begin(), ms.Subs.end(), pSub) != ms.Subs.end();
		}
		) != tiles.end();
	}

    bool hasBriefingScreen() const {
        return hasGfx(eGFX_BRIEFING);
    }

    bool hasServiceScreen() const {
        return hasGfx(eGFX_SERVICE);
    }

    cDimension GetScreenSize() const {
        if (isAmiga())
            return { 320,225 };

        return { 320,200 };
    }

    cDimension GetSecondScreenSize() const {
        if (isAmiga())
            return { 320,260 };

        return { 320,200 };
    }

	std::vector<eGFX_Types> getGfxTypes() const;
	std::vector<sTileset> getTileTypes() const;

	std::vector<sIntroText>* getIntroData() const;

    std::shared_ptr<cResources> GetResources() const;
    std::shared_ptr<cGraphics> GetGraphics() const;
    std::shared_ptr<cSound> GetSound() const;
	bool CanUseAmigaSound() const;

	std::string getDataPath() const;
	std::string getDataFilePath(std::string pFile) const;
};

class cVersions {

protected:

public:

    bool isCampaignKnown(const std::string& pName) const;
    bool isCampaignAvailable(const std::string& pName) const;

    std::vector<std::string> GetCampaignNames() const;

    const sGameVersion* GetForCampaign(const std::string& pCampaign) const;
    const sGameVersion* GetForCampaign(const std::string& pCampaign, const ePlatform pPlatform) const;
    const sGameVersion* GetForTileset(eTileTypes pTileType, eTileSub pSub) const;
    const sGameVersion* GetRetail(const ePlatform pPlatform, const eGame pGame) const;
    const sGameVersion* GetDemo() const;
};

extern const sGameVersion KnownGameVersions[20];