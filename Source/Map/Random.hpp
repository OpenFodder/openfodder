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

class cRandomSprites;
struct duk_hthread;

class cRandomMap : public cOriginalMap {
	friend class cRandomSprites;
private:
	std::shared_ptr<cRandomSprites> mRandomSprites;

protected:
	virtual void Randomise_Tiles();
	virtual void Randomise_Tiles_DS();

	virtual void Randomise_TileSmooth();
	virtual void Randomise_Structures(const size_t pCount);
	virtual void Randomise_Sprites(const size_t pHumanCount = 2);

public:
	cRandomMap(const sMapParams& pParams);
	virtual void Randomise();
	
	bool					CheckRadiusTerrain(eTerrainType pType, cPosition* pPosition, int32 pRadius);

	int32					getSpriteTypeCount(size_t pSpriteType);
	std::vector<sSprite*>	getSpritesByType(size_t pSpriteType);
	int32					getRandomNumber(int32 pMin = 0, int32 pMax = 0);
	cPosition*				getRandomXYByTerrainType(eTerrainType pType, size_t pRadius);

};

class cRandomSprites {
	duk_hthread *mContext;
	cRandomMap *mMap;

protected:

	bool scriptCall(const std::string& pFilename);
	bool scriptLoad(const std::string& pJS);
	bool scriptsLoadFolder(const std::string& pFolder);

	bool scriptRun(const std::string& pJS);

	void spritesCreateObject();
public:

	cRandomSprites(cRandomMap *pMap);
	~cRandomSprites();

	void init();

	virtual void Randomise();
};