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
#include "Utils/micropather.h"

class cScriptingEngine;

class cRandomMap : public cOriginalMap, public micropather::Graph {
	friend class cScriptingEngine;
private:

protected:
	const int8* mPathTilesNotTouchable;
	size_t mPathSearchUnitType;

	int Passable(int nx, int ny);

	virtual float LeastCostEstimate(cPosition* nodeStart, cPosition* nodeEnd);
	virtual void AdjacentCost(cPosition* node, std::vector< micropather::StateCost > *neighbors);
	virtual void PrintStateInfo(cPosition* node);

public:
	cRandomMap(const sMapParams& pParams);

	bool					CheckRadiusTileID(std::vector<size_t> pTileIDs, cPosition* pPosition, int32 pRadius);
	bool					CheckRadiusFeatures(const std::vector<eTerrainFeature>& pType, cPosition* pPosition, int32 pRadius);
	bool					CheckRadiusSprites(cPosition* pPosition, int32 pRadius);

	void					create(size_t pWidth, size_t pHeight, eTileTypes pTileType, eTileSub pTileSub = eTileSub::eTileSub_0);

	std::vector<cPosition*>	calculatePath(size_t pSpriteType, cPosition* Pos1, cPosition* Pos2);
	int32					getSpriteTypeCount(size_t pSpriteType);
	std::vector<sSprite*>	getSpritesByType(size_t pSpriteType);
	float					getRandomFloat(float pMin, float pMax);
	int32					getRandomInt(int32 pMin = 0, int32 pMax = 0);
	int16					getSeed() const;
	void					setSeed(const int16 pSeed);

	cPosition*				getRandomXYByTileID(std::vector<size_t> pTiles, size_t pRadius);
	cPosition*				getRandomXYByFeatures(std::vector<eTerrainFeature> pFeatures, size_t pRadius, bool pIgnoreSprites);
	cPosition*				getRandomXYByTerrainType(eTerrainFeature pType, size_t pRadius);

	int32					getDistanceBetweenPositions(cPosition* pPos1, cPosition* pPos2);

	std::vector<std::vector<double>> createDiamondSquare();

	std::vector<std::vector<float>> createSimplexIslands(size_t pOctaves, float pRoughness, float pScale, short pSeed, bool pRadialEnabled, float pEdgeFade);
	std::vector<std::vector<float>> createSimplexNoise(size_t pOctaves, float pFrequency, float pLacunarity, float pPersistence);
};

