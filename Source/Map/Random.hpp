/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
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
class cScriptingEngine;

class cRandomMap : public cOriginalMap {
	friend class cScriptingEngine;
private:
	int32					mMapGenPathWidth = 0;
	int32					mMapGenPathHeight = 0;
	std::vector<double>		mMapGenPathCosts;
	int32					mMapGenWalkWidth = 0;
	int32					mMapGenWalkHeight = 0;
	std::vector<uint8_t>	mMapGenWalkable;

protected:
	const int8* mPathTilesNotTouchable;
	size_t mPathSearchUnitType;

	int Passable(int nx, int ny);

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
	uint32					getSeed() const;
	void					setSeed(const uint32 pSeed);

	cPosition*				getRandomXYByTileID(std::vector<size_t> pTiles, size_t pRadius);
	cPosition*				getRandomXYByFeatures(std::vector<eTerrainFeature> pFeatures, size_t pRadius, bool pIgnoreSprites);
	cPosition*				getRandomXYByTerrainType(eTerrainFeature pType, size_t pRadius);

	// Engine-authoritative terrain queries, for the map generator to test what a
	// tile actually IS in-game (HIT/BHT collision) instead of guessing from tile
	// IDs. Coordinates are TILE coordinates; the feature is sampled at the tile
	// centre. getTileTerrainFeature returns the eTerrainFeature (-1 out of bounds);
	// isTileWalkable applies the engine's mTiles_NotWalkable verdict for foot units.
	int16					getTileTerrainFeature(int32 pTileX, int32 pTileY);
	bool					isTileWalkable(int32 pTileX, int32 pTileY);

	// Native ice tile-art "Wang" edge matcher (the hot ~47% of each ice render).
	// The authored atlas + all per-cell semantics stay in JS; these just cache
	// the static atlas once (SetIceEdgeAtlas) and run the per-cell scoring loop
	// (ApplyIceEdgeRule). See Source/Map/IceEdgeMatcher.{hpp,cpp}. This preserves
	// the JS-facing ApplyEdgeRule contract while keeping the hot loop native.
	void					setIceEdgeAtlas(std::vector<std::string> pTileRecords,
								std::vector<std::string> pByCenter,
								std::vector<std::string> pCharToClass);
	std::vector<int>		applyIceEdgeRule(int32 pWidth, int32 pHeight,
								std::string pChars, std::string pHints,
								std::string pReqCenter, std::string pReqContents,
								double pSeed);
	std::vector<int>		applyIceEdgeRuleMasked(int32 pWidth, int32 pHeight,
								std::string pChars, std::string pHints,
								std::string pReqCenter, std::string pReqContents,
								std::string pDirtyMask, std::string pPreviousTiles,
								double pSeed);
	std::vector<int>		applyIceEdgeRuleMaskedRegion(int32 pWidth, int32 pHeight,
								std::string pChars, std::string pHints,
								std::string pReqCenter, std::string pReqContents,
								std::string pDirtyMask, std::string pPreviousTiles,
								double pSeed,
								int32 pMinX, int32 pMinY, int32 pMaxX, int32 pMaxY);

	// MapGen-native pathfinding over generator-layer grids. These are separate
	// from calculatePathBetweenPositions, which routes over rendered engine
	// terrain. JS owns the layer semantics and pushes the flattened cost/mask
	// grids whenever it rebuilds them; native code owns only the hot search loop.
	void					setMapGenPathCostGrid(int32 pWidth, int32 pHeight, std::vector<double> pCosts);
	void					setMapGenPathCost(int32 pX, int32 pY, double pCost);
	std::vector<int>		mapGenAstar(int32 pStartX, int32 pStartY, int32 pEndX, int32 pEndY);
	void					setMapGenWalkabilityGrid(int32 pWidth, int32 pHeight, std::string pWalkableMask);
	std::vector<int>		mapGenShortestPath(int32 pStartX, int32 pStartY, int32 pEndX, int32 pEndY,
								std::vector<int> pBlockedIndices);
	std::vector<int>		mapGenCanReach(int32 pStartX, int32 pStartY, int32 pEndX, int32 pEndY);

	int32					getDistanceBetweenPositions(cPosition* pPos1, cPosition* pPos2);

	std::vector<std::vector<double>> createDiamondSquare();

	std::vector<std::vector<float>> createSimplexIslands(size_t pOctaves, float pRoughness, float pScale, short pSeed, bool pRadialEnabled, float pEdgeFade);
	std::vector<std::vector<float>> createSimplexNoise(size_t pOctaves, float pFrequency, float pLacunarity, float pPersistence);
};

