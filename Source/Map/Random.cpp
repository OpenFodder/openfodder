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


#include "stdafx.hpp"
#include "Random.hpp"

#include <cstdint>
#include <limits>

#include "Utils/SimplexNoise.hpp"
#include "Utils/SimplexIslands.hpp"
#include "Utils/diamondsquare.hpp"
#include "IceEdgeMatcher.hpp"
#include "TerrainPathfinder.hpp"

// Process-static so the authored atlas (pushed once from JS, guarded by a module
// flag) survives across cRandomMap instances within a generation process.
static IceEdge::Matcher gIceEdgeMatcher;

static std::vector<int> parseTileCsv(const std::string& pText)
{
	std::vector<int> result;
	int value = 0;
	bool haveValue = false;
	bool negative = false;

	for (size_t i = 0; i <= pText.size(); ++i) {
		char ch = (i < pText.size()) ? pText[i] : ',';
		if (ch == '-' && !haveValue) {
			negative = true;
			continue;
		}
		if (ch >= '0' && ch <= '9') {
			value = (value * 10) + (ch - '0');
			haveValue = true;
			continue;
		}
		if (ch == ',') {
			result.push_back(negative ? -value : value);
			value = 0;
			haveValue = false;
			negative = false;
		}
	}

	return result;
}

static inline int mapGenIndex(int32 pWidth, int32 pX, int32 pY)
{
	return (pY * pWidth) + pX;
}

static std::vector<int> mapGenSearchResult(bool pSuccess, int pExpanded, int pPushed, int pMaxOpen, int pPathLength = 0)
{
	return {
		pSuccess ? 1 : 0,
		pExpanded,
		pPushed,
		pMaxOpen,
		pPathLength
	};
}

cRandomMap::cRandomMap(const sMapParams& pParams) : cOriginalMap() {
	mParams = pParams;
	mPathSearchUnitType = 0;
	mPathTilesNotTouchable = 0;
	mData = std::make_shared<std::vector<uint8_t>>();
	ClearTiles(0);

	saveHeader();
}

int32 cRandomMap::getSpriteTypeCount(size_t pSpriteType) {
	int32 count = 0;

	for (auto& Sprite : mSprites) {

		if (Sprite.mSpriteType == pSpriteType)
			++count;
	}

	return count;
}

std::vector<sSprite*> cRandomMap::getSpritesByType(size_t pSpriteType) {
	std::vector<sSprite*> results;

	for (auto& Sprite : mSprites) {
		
		if(Sprite.mSpriteType == pSpriteType)
			results.push_back(&Sprite);
	}

	return results;
}

bool cRandomMap::CheckRadiusTileID(std::vector<size_t> pTileIDs, cPosition* pPosition, int32 pRadius) {
	for (int32 x = pPosition->mX - pRadius; x < pPosition->mX + pRadius; x++) {
		if (x < 0)
			continue;

		int32 yspan = (int32)(pRadius * sin(acos((pPosition->mX - x) / pRadius)));
		for (int32 y = pPosition->mY - yspan; y < pPosition->mY + yspan; y++) {
			if (y < 0)
				continue;

			int32 TileID = Tile_Get(x, y);
			if (std::find(pTileIDs.begin(), pTileIDs.end(), TileID) == pTileIDs.end())
				return false;
		}
	}

	return true;
}

bool cRandomMap::CheckRadiusFeatures(const std::vector<eTerrainFeature> &pTypes, cPosition* pPosition, int32 pRadius) {
	for (int32 x = pPosition->mX - pRadius; x < pPosition->mX + pRadius; x++) {
		if (x < 0)
			continue;

		int32 yspan = (int32) (pRadius * sin(acos((pPosition->mX - x) / pRadius)));
		for (int32 y = pPosition->mY - yspan; y < pPosition->mY + yspan; y++) {
			if (y < 0)
				continue;
			
			int32 Terrain = g_Fodder->Map_Terrain_Get(x, y);
			if (std::find(pTypes.begin(), pTypes.end(), Terrain) == pTypes.end())
				return false;
		}
	}

	return true;
}

bool cRandomMap::CheckRadiusSprites(cPosition* pPosition, int32 pRadius) {

	int32 xLeft = pPosition->mX - pRadius;
	int32 xRight = pPosition->mX + pRadius;

	int32 yspan = (int32) (pRadius * sin(acos((pPosition->mX - pPosition->mX) / pRadius)));

	int32 yTop = pPosition->mY - yspan;
	int32 yBottom = pPosition->mY + yspan;


	for (auto& Sprite : mSprites) {

		if (Sprite.mPosX < 0 || Sprite.mPosY < 0)
			continue;

		if ((Sprite.mPosX >= xLeft && Sprite.mPosX <= xRight) && 
			(Sprite.mPosY >= yTop && Sprite.mPosY <= yBottom))
			return true;
	}

	return false;
}

float cRandomMap::getRandomFloat(float pMin, float pMax) {

	return mParams.mRandom.getf(pMin, pMax);
}

int32 cRandomMap::getRandomInt(int32 pMin, int32 pMax) {
	if(pMin || pMax)
		return mParams.mRandom.getu(pMin, pMax);

	if (!pMin && !pMax)
		return 0;

	return mParams.mRandom.getu();
}

uint32 cRandomMap::getSeed() const {
	return mParams.mRandom.getStartingSeed();
}

void cRandomMap::setSeed(const uint32 pSeed) {

	return mParams.mRandom.setSeed(pSeed);
}

cPosition* cRandomMap::getRandomXYByTileID(std::vector<size_t> pTiles, size_t pRadius) {
	int32 Radius = (int32) pRadius;
	cPosition* Position = new cPosition();

	cPosition PosStart;
	PosStart.mX = pRadius + (mParams.mRandom.getu() % (mParams.mWidth - pRadius));
	PosStart.mY = pRadius + (mParams.mRandom.getu() % (mParams.mHeight - pRadius));
	
	// Move 1 tile each direction
	for (Position->mY = PosStart.mY; Position->mY < getHeight() - Radius; Position->mY++) {
		if (Position->mY != PosStart.mY)
			PosStart.mX = 0;

		for (Position->mX = PosStart.mX; Position->mX < getWidth() - Radius; Position->mX++) {

			if (CheckRadiusTileID(pTiles, Position, pRadius)) {
				Position->mX *= TILE_WIDTH_PIXELS;
				Position->mY *= TILE_HEIGHT_PIXELS;
				return Position;
			}
		}
	}

	// Return to start of map
	for (Position->mY = 0; Position->mY < PosStart.mY; Position->mY++) {
		for (Position->mX = 0; Position->mX <= getWidth() - Radius; Position->mX++) {

			if (CheckRadiusTileID(pTiles, Position, pRadius)) {
				Position->mX *= TILE_WIDTH_PIXELS;
				Position->mY *= TILE_HEIGHT_PIXELS;
				return Position;
			}
		}
	}

	Position->mX = -1;
	Position->mY = -1;
	return Position;
}


cPosition* cRandomMap::getRandomXYByFeatures(std::vector<eTerrainFeature> pFeatures, size_t pRadius, bool pIgnoreSprites) {
	size_t Radius = pRadius * TILE_WIDTH_PIXELS;
	cPosition* Position = new cPosition();

	cPosition PosStart;
	PosStart.mX = 16 + (pRadius + (mParams.mRandom.getu() % (mParams.mWidth - pRadius))) * TILE_WIDTH_PIXELS;
	PosStart.mY = 16 + (pRadius + (mParams.mRandom.getu() % (mParams.mHeight - pRadius))) * TILE_HEIGHT_PIXELS;

	// Move 1 tile each direction
	for (Position->mY = PosStart.mY; Position->mY <= getHeightPixels(); Position->mY += 16) {
		if (Position->mY != PosStart.mY)
			PosStart.mX = Radius;

		for (Position->mX = PosStart.mX; Position->mX <= getWidthPixels(); Position->mX += 16) {


			if (!pIgnoreSprites) {
				if (CheckRadiusSprites(Position, Radius))
					continue;
			}
			// Check land for one of the features
			if (CheckRadiusFeatures(pFeatures, Position, Radius))
				return Position;
		}
	}

	// Return to start of map
	for (Position->mY = Radius; Position->mY < PosStart.mY; Position->mY += 8) {
		for (Position->mX = Radius; Position->mX <= getWidthPixels(); Position->mX += 8) {

			if (!pIgnoreSprites) {
				if (CheckRadiusSprites(Position, Radius))
					continue;
			}
			// Check land for one of the features
			if (CheckRadiusFeatures(pFeatures, Position, Radius))
				return Position;
		}
	}

	// Fail
	Position->mX = -1;
	Position->mY = -1;

	return Position;
}

cPosition* cRandomMap::getRandomXYByTerrainType(eTerrainFeature pType, size_t pRadius) {
	
	return getRandomXYByFeatures({ pType }, pRadius, true);
}

int32 cRandomMap::getDistanceBetweenPositions(cPosition* pPos1, cPosition* pPos2) {

	return g_Fodder->Map_Get_Distance_BetweenPositions(*pPos1, *pPos2, 1000);
}

std::vector<std::vector<float>> cRandomMap::createSimplexIslands(size_t pOctaves, float pRoughness, float pScale, short pSeed, bool pRadialEnabled, float pEdgeFade) {

	SimplexIslands Islands;

	auto noise = Islands.CreateArray(mParams.mWidth, mParams.mHeight, pOctaves, pRoughness, pScale, pSeed, pRadialEnabled, pEdgeFade);

	return noise;
}

std::vector<std::vector<double>> cRandomMap::createDiamondSquare() {

	int32 PowerOf = 0;
	size_t Size;

	if (mParams.mWidth < mParams.mHeight)
		Size = mParams.mHeight;
	else
		Size = mParams.mWidth;
	while (Size > 0) {
		PowerOf++;
		Size = Size >> 1;
	}

	cDiamondSquare DS(PowerOf, mParams.mRandom.getu());
	return DS.generate();
}

std::vector<std::vector<float>> cRandomMap::createSimplexNoise(size_t pOctaves, float pFrequency, float pLacunarity, float pPersistence) {
	SimplexNoise Noise(pFrequency, pLacunarity, pPersistence);

	return Noise.create(mParams.mWidth, mParams.mHeight, pOctaves);
}

void cRandomMap::create(size_t pWidth, size_t pHeight, eTileTypes pTileType, eTileSub pTileSub) {

	mParams.mWidth = pWidth;
	mParams.mHeight = pHeight;
	mParams.mTileType = pTileType;
	mParams.mTileSub = pTileSub;

	mSprites.clear();
	g_Fodder->mSprites = mSprites;
	ClearTiles(0);
	saveHeader();
	g_Fodder->Map_Load_Resources();
}

std::vector<cPosition*> cRandomMap::calculatePath(size_t pSpriteType, cPosition* Pos1, cPosition* Pos2) {
	mPathSearchUnitType = pSpriteType;

	switch (pSpriteType) {
	default:
		mPathTilesNotTouchable = mTiles_NotWalkable;
		break;

	case eSprite_Tank_Enemy:
	case eSprite_Tank_Human:
		mPathTilesNotTouchable = mTiles_NotDriveable;
		break;

	case eSprite_Civilian:
	case eSprite_Civilian2:
	case eSprite_Hostage:
	case eSprite_Enemy_Leader:
		mPathTilesNotTouchable = mTiles_NotFlyable;
		break;

	}

	cTerrainPathfinder pathfinder(
		getWidthPixels(),
		getHeightPixels(),
		[this](int32 pX, int32 pY) { return Passable(pX, pY); });
	std::vector<cPosition> path = pathfinder.findPath(Pos1, Pos2);

	std::vector<cPosition*> paths;

	for (auto p : path) {
		paths.push_back(new cPosition(p));
	}

	return paths;
}

int16 cRandomMap::getTileTerrainFeature(int32 pTileX, int32 pTileY)
{
	if (pTileX < 0 || pTileY < 0 ||
		(size_t)pTileX >= mParams.mWidth || (size_t)pTileY >= mParams.mHeight)
		return -1;

	// Sample at the tile centre (pixel coords); Map_Terrain_Get resolves the
	// HIT/BHT sub-tile feature for the loaded map tileset.
	int16 PixelX = (int16)(pTileX * TILE_WIDTH_PIXELS + (TILE_WIDTH_PIXELS / 2));
	int16 PixelY = (int16)(pTileY * TILE_HEIGHT_PIXELS + (TILE_HEIGHT_PIXELS / 2));

	return g_Fodder->Map_Terrain_Get(PixelX, PixelY);
}

bool cRandomMap::isTileWalkable(int32 pTileX, int32 pTileY)
{
	int16 TerrainType = getTileTerrainFeature(pTileX, pTileY);
	if (TerrainType < 0)
		return false;

	// Same verdict the engine uses for foot units (see Passable / pathing).
	return mTiles_NotWalkable[TerrainType] == 0;
}

void cRandomMap::setIceEdgeAtlas(std::vector<std::string> pTileRecords,
	std::vector<std::string> pByCenter,
	std::vector<std::string> pCharToClass)
{
	gIceEdgeMatcher.setAtlas(pTileRecords, pByCenter, pCharToClass);
}

std::vector<int> cRandomMap::applyIceEdgeRule(int32 pWidth, int32 pHeight,
	std::string pChars, std::string pHints,
	std::string pReqCenter, std::string pReqContents,
	double pSeed)
{
	uint32_t seed = (uint32_t)pSeed;
	return gIceEdgeMatcher.apply(pWidth, pHeight, pChars, pHints, pReqCenter, pReqContents, seed);
}

std::vector<int> cRandomMap::applyIceEdgeRuleMasked(int32 pWidth, int32 pHeight,
	std::string pChars, std::string pHints,
	std::string pReqCenter, std::string pReqContents,
	std::string pDirtyMask, std::string pPreviousTiles,
	double pSeed)
{
	uint32_t seed = (uint32_t)pSeed;
	std::vector<int> previousTiles = parseTileCsv(pPreviousTiles);
	return gIceEdgeMatcher.apply(
		pWidth, pHeight, pChars, pHints, pReqCenter, pReqContents,
		seed, &pDirtyMask, &previousTiles);
}

std::vector<int> cRandomMap::applyIceEdgeRuleMaskedRegion(int32 pWidth, int32 pHeight,
	std::string pChars, std::string pHints,
	std::string pReqCenter, std::string pReqContents,
	std::string pDirtyMask, std::string pPreviousTiles,
	double pSeed,
	int32 pMinX, int32 pMinY, int32 pMaxX, int32 pMaxY)
{
	uint32_t seed = (uint32_t)pSeed;
	std::vector<int> previousTiles = parseTileCsv(pPreviousTiles);
	return gIceEdgeMatcher.apply(
		pWidth, pHeight, pChars, pHints, pReqCenter, pReqContents,
		seed, &pDirtyMask, &previousTiles,
		pMinX, pMinY, pMaxX, pMaxY);
}

void cRandomMap::setMapGenPathCostGrid(int32 pWidth, int32 pHeight, std::vector<double> pCosts)
{
	if (pWidth <= 0 || pHeight <= 0 || pCosts.size() != (size_t)(pWidth * pHeight)) {
		mMapGenPathWidth = 0;
		mMapGenPathHeight = 0;
		mMapGenPathCosts.clear();
		return;
	}

	mMapGenPathWidth = pWidth;
	mMapGenPathHeight = pHeight;
	mMapGenPathCosts = std::move(pCosts);
}

void cRandomMap::setMapGenPathCost(int32 pX, int32 pY, double pCost)
{
	if (pX < 0 || pY < 0 || pX >= mMapGenPathWidth || pY >= mMapGenPathHeight)
		return;

	const int index = mapGenIndex(mMapGenPathWidth, pX, pY);
	if (index >= 0 && (size_t)index < mMapGenPathCosts.size())
		mMapGenPathCosts[index] = pCost;
}

std::vector<int> cRandomMap::mapGenAstar(int32 pStartX, int32 pStartY, int32 pEndX, int32 pEndY)
{
	const int32 width = mMapGenPathWidth;
	const int32 height = mMapGenPathHeight;
	const int total = width * height;
	int expanded = 0;
	int pushed = 0;
	int maxOpen = 0;

	if (width <= 0 || height <= 0 || mMapGenPathCosts.size() != (size_t)total)
		return mapGenSearchResult(false, expanded, pushed, maxOpen);
	if (pStartX < 0 || pStartY < 0 || pStartX >= width || pStartY >= height)
		return mapGenSearchResult(false, expanded, pushed, maxOpen);
	if (pEndX < 0 || pEndY < 0 || pEndX >= width || pEndY >= height)
		return mapGenSearchResult(false, expanded, pushed, maxOpen);

	const int startIndex = mapGenIndex(width, pStartX, pStartY);
	const int endIndex = mapGenIndex(width, pEndX, pEndY);
	if (!std::isfinite(mMapGenPathCosts[startIndex]) || !std::isfinite(mMapGenPathCosts[endIndex]))
		return mapGenSearchResult(false, expanded, pushed, maxOpen);

	const double infinity = std::numeric_limits<double>::infinity();
	std::vector<double> gScore(total, infinity);
	std::vector<int> cameFrom(total, -1);
	std::vector<uint8_t> closed(total, 0);

	struct OpenNode {
		int32 x;
		int32 y;
		double f;
	};

	const auto heuristic = [](int32 ax, int32 ay, int32 bx, int32 by) -> double {
		const int32 dx = ax > bx ? ax - bx : bx - ax;
		const int32 dy = ay > by ? ay - by : by - ay;
		const int32 diagonal = dx < dy ? dx : dy;
		const int32 straight = (dx + dy) - (2 * diagonal);
		return straight + (1.41421356 * diagonal);
	};

	std::vector<OpenNode> open;
	open.reserve(256);
	gScore[startIndex] = 0.0;
	open.push_back({ pStartX, pStartY, heuristic(pStartX, pStartY, pEndX, pEndY) });
	pushed = 1;
	maxOpen = 1;

	const int32 dx[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };
	const int32 dy[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };
	const double stepCost[8] = { 1.0, 1.0, 1.0, 1.0, 1.41421356, 1.41421356, 1.41421356, 1.41421356 };

	while (!open.empty()) {
		size_t bestIndex = 0;
		for (size_t i = 1; i < open.size(); ++i) {
			if (open[i].f < open[bestIndex].f)
				bestIndex = i;
		}

		const OpenNode current = open[bestIndex];
		open.erase(open.begin() + bestIndex);
		++expanded;

		const int currentIndex = mapGenIndex(width, current.x, current.y);
		if (current.x == pEndX && current.y == pEndY) {
			std::vector<int> reversed;
			for (int index = currentIndex; index >= 0; index = cameFrom[index]) {
				reversed.push_back(index);
			}

			std::vector<int> result = mapGenSearchResult(true, expanded, pushed, maxOpen, (int)reversed.size());
			result.reserve(5 + (reversed.size() * 2));
			for (auto it = reversed.rbegin(); it != reversed.rend(); ++it) {
				const int index = *it;
				result.push_back(index % width);
				result.push_back(index / width);
			}
			return result;
		}

		closed[currentIndex] = 1;

		for (int direction = 0; direction < 8; ++direction) {
			const int32 nx = current.x + dx[direction];
			const int32 ny = current.y + dy[direction];
			if (nx < 0 || ny < 0 || nx >= width || ny >= height)
				continue;

			const int nextIndex = mapGenIndex(width, nx, ny);
			if (closed[nextIndex])
				continue;

			const double cellCost = mMapGenPathCosts[nextIndex];
			if (!std::isfinite(cellCost))
				continue;

			if (dx[direction] != 0 && dy[direction] != 0) {
				const int orthogonalA = mapGenIndex(width, current.x + dx[direction], current.y);
				const int orthogonalB = mapGenIndex(width, current.x, current.y + dy[direction]);
				if (!std::isfinite(mMapGenPathCosts[orthogonalA]) &&
					!std::isfinite(mMapGenPathCosts[orthogonalB]))
					continue;
			}

			const double tentative = gScore[currentIndex] + (stepCost[direction] * cellCost);
			if (tentative >= gScore[nextIndex])
				continue;

			cameFrom[nextIndex] = currentIndex;
			gScore[nextIndex] = tentative;
			open.push_back({ nx, ny, tentative + heuristic(nx, ny, pEndX, pEndY) });
			++pushed;
			if ((int)open.size() > maxOpen)
				maxOpen = (int)open.size();
		}
	}

	return mapGenSearchResult(false, expanded, pushed, maxOpen);
}

void cRandomMap::setMapGenWalkabilityGrid(int32 pWidth, int32 pHeight, std::string pWalkableMask)
{
	if (pWidth <= 0 || pHeight <= 0 || pWalkableMask.size() != (size_t)(pWidth * pHeight)) {
		mMapGenWalkWidth = 0;
		mMapGenWalkHeight = 0;
		mMapGenWalkable.clear();
		return;
	}

	mMapGenWalkWidth = pWidth;
	mMapGenWalkHeight = pHeight;
	mMapGenWalkable.assign(pWalkableMask.size(), 0);
	for (size_t index = 0; index < pWalkableMask.size(); ++index)
		mMapGenWalkable[index] = pWalkableMask[index] == '1' ? 1 : 0;
}

std::vector<int> cRandomMap::mapGenShortestPath(int32 pStartX, int32 pStartY, int32 pEndX, int32 pEndY,
	std::vector<int> pBlockedIndices)
{
	const int32 width = mMapGenWalkWidth;
	const int32 height = mMapGenWalkHeight;
	const int total = width * height;
	int expanded = 0;
	int pushed = 0;
	int maxQueue = 0;

	if (width <= 0 || height <= 0 || mMapGenWalkable.size() != (size_t)total)
		return mapGenSearchResult(false, expanded, pushed, maxQueue);
	if (pStartX < 0 || pStartY < 0 || pStartX >= width || pStartY >= height)
		return mapGenSearchResult(false, expanded, pushed, maxQueue);
	if (pEndX < 0 || pEndY < 0 || pEndX >= width || pEndY >= height)
		return mapGenSearchResult(false, expanded, pushed, maxQueue);

	const int startIndex = mapGenIndex(width, pStartX, pStartY);
	const int endIndex = mapGenIndex(width, pEndX, pEndY);
	if (!mMapGenWalkable[startIndex] || !mMapGenWalkable[endIndex])
		return mapGenSearchResult(false, expanded, pushed, maxQueue);

	std::vector<uint8_t> blocked;
	if (!pBlockedIndices.empty()) {
		blocked.assign(total, 0);
		for (int index : pBlockedIndices) {
			if (index >= 0 && index < total)
				blocked[index] = 1;
		}
		if (blocked[startIndex] || blocked[endIndex])
			return mapGenSearchResult(false, expanded, pushed, maxQueue);
	}

	std::vector<uint8_t> visited(total, 0);
	std::vector<int> cameFrom(total, -1);
	std::vector<int> queue;
	queue.reserve(total);
	size_t queueHead = 0;

	queue.push_back(startIndex);
	visited[startIndex] = 1;
	pushed = 1;
	maxQueue = 1;

	const int32 dx[4] = { 1, -1, 0, 0 };
	const int32 dy[4] = { 0, 0, 1, -1 };

	while (queueHead < queue.size()) {
		const int currentIndex = queue[queueHead++];
		++expanded;

		if (currentIndex == endIndex) {
			std::vector<int> reversed;
			for (int index = currentIndex; index >= 0; index = cameFrom[index])
				reversed.push_back(index);

			std::vector<int> result = mapGenSearchResult(true, expanded, pushed, maxQueue, (int)reversed.size());
			result.reserve(5 + (reversed.size() * 2));
			for (auto it = reversed.rbegin(); it != reversed.rend(); ++it) {
				const int index = *it;
				result.push_back(index % width);
				result.push_back(index / width);
			}
			return result;
		}

		const int32 currentX = currentIndex % width;
		const int32 currentY = currentIndex / width;
		for (int direction = 0; direction < 4; ++direction) {
			const int32 nx = currentX + dx[direction];
			const int32 ny = currentY + dy[direction];
			if (nx < 0 || ny < 0 || nx >= width || ny >= height)
				continue;

			const int nextIndex = mapGenIndex(width, nx, ny);
			if (visited[nextIndex])
				continue;
			if (!blocked.empty() && blocked[nextIndex])
				continue;
			if (!mMapGenWalkable[nextIndex])
				continue;

			visited[nextIndex] = 1;
			cameFrom[nextIndex] = currentIndex;
			queue.push_back(nextIndex);
			++pushed;
			const int queueDepth = (int)(queue.size() - queueHead);
			if (queueDepth > maxQueue)
				maxQueue = queueDepth;
		}
	}

	return mapGenSearchResult(false, expanded, pushed, maxQueue);
}

std::vector<int> cRandomMap::mapGenCanReach(int32 pStartX, int32 pStartY, int32 pEndX, int32 pEndY)
{
	const int32 width = mMapGenWalkWidth;
	const int32 height = mMapGenWalkHeight;
	const int total = width * height;
	int expanded = 0;
	int pushed = 1;
	int maxQueue = 1;

	if (width <= 0 || height <= 0 || mMapGenWalkable.size() != (size_t)total)
		return mapGenSearchResult(false, 0, 0, 0);
	if (pStartX < 0 || pStartY < 0 || pStartX >= width || pStartY >= height)
		return mapGenSearchResult(false, expanded, pushed, maxQueue);
	if (pEndX < 0 || pEndY < 0 || pEndX >= width || pEndY >= height)
		return mapGenSearchResult(false, expanded, pushed, maxQueue);

	const int startIndex = mapGenIndex(width, pStartX, pStartY);
	const int endIndex = mapGenIndex(width, pEndX, pEndY);
	if (!mMapGenWalkable[startIndex] || !mMapGenWalkable[endIndex])
		return mapGenSearchResult(false, expanded, pushed, maxQueue);

	std::vector<uint8_t> visited(total, 0);
	std::vector<int> queue;
	queue.reserve(total);
	size_t queueHead = 0;

	queue.push_back(startIndex);
	visited[startIndex] = 1;

	const int32 dx[4] = { 1, -1, 0, 0 };
	const int32 dy[4] = { 0, 0, 1, -1 };

	while (queueHead < queue.size()) {
		const int currentIndex = queue[queueHead++];
		++expanded;

		if (currentIndex == endIndex)
			return mapGenSearchResult(true, expanded, pushed, maxQueue);

		const int32 currentX = currentIndex % width;
		const int32 currentY = currentIndex / width;
		for (int direction = 0; direction < 4; ++direction) {
			const int32 nx = currentX + dx[direction];
			const int32 ny = currentY + dy[direction];
			if (nx < 0 || ny < 0 || nx >= width || ny >= height)
				continue;

			const int nextIndex = mapGenIndex(width, nx, ny);
			if (visited[nextIndex] || !mMapGenWalkable[nextIndex])
				continue;

			visited[nextIndex] = 1;
			queue.push_back(nextIndex);
			++pushed;
			const int queueDepth = (int)(queue.size() - queueHead);
			if (queueDepth > maxQueue)
				maxQueue = queueDepth;
		}
	}

	return mapGenSearchResult(false, expanded, pushed, maxQueue);
}

int cRandomMap::Passable(int nx, int ny)
{
	auto TerrainType = g_Fodder->Map_Terrain_Get(nx, ny);
	if (TerrainType == -1)
		return 0;

	// Blocked
	if (mPathTilesNotTouchable[TerrainType])
		return 0;

	switch (mPathSearchUnitType) {

		default:
			break;

		case eSprite_Helicopter_Grenade_Enemy:
		case eSprite_Helicopter_Unarmed_Enemy:
		case eSprite_Helicopter_Missile_Enemy:
		case eSprite_Helicopter_Homing_Enemy:
		case eSprite_Helicopter_Grenade_Human:
		case eSprite_Helicopter_Unarmed_Human:
		case eSprite_Helicopter_Missile_Human:
		case eSprite_Helicopter_Homing_Human:
		case eSprite_Helicopter_Grenade_Human_Called:
		case eSprite_Helicopter_Unarmed_Human_Called:
		case eSprite_Helicopter_Missile_Human_Called:
		case eSprite_Helicopter_Homing_Human_Called:
		case eSprite_Helicopter_Homing_Enemy2:
			return 1;
	}

	switch (TerrainType) {
	default:
		return 1;

	case eTerrainFeature_DeepWater:
	case eTerrainFeature_ShallowWater:
	case eTerrainFeature_SoftHazard:

		// Tanks cant drive through water/quicksand
		if (mPathSearchUnitType == eSprite_Tank_Human || mPathSearchUnitType == eSprite_Tank_Enemy)
			return 0;

		return 2;
	}

	return 1;
}
