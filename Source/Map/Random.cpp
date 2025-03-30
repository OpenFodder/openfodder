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
#include "Random.hpp"

#include "Utils/SimplexNoise.hpp"
#include "Utils/SimplexIslands.hpp"
#include "Utils/diamondsquare.hpp"

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

		if (Sprite.field_18 == pSpriteType)
			++count;
	}

	return count;
}

std::vector<sSprite*> cRandomMap::getSpritesByType(size_t pSpriteType) {
	std::vector<sSprite*> results;

	for (auto& Sprite : mSprites) {
		
		if(Sprite.field_18 == pSpriteType)
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

		if (Sprite.field_0 < 0 || Sprite.field_4 < 0)
			continue;

		if ((Sprite.field_0 >= xLeft && Sprite.field_0 <= xRight) && 
			(Sprite.field_4 >= yTop && Sprite.field_4 <= yBottom))
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

int16 cRandomMap::getSeed() const {
	return mParams.mRandom.getStartingSeed();
}

void cRandomMap::setSeed(const int16 pSeed) {

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
	std::vector<cPosition> path;

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

	auto pather = new micropather::MicroPather(this, 1000);
	float totalCost;
	auto result = pather->Solve(Pos1, Pos2, &path, &totalCost);

	delete pather;

	std::vector<cPosition*> paths;

	for (auto p : path) {
		paths.push_back(new cPosition(p));
	}

	return paths;
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

	case eTerrainFeature_Water:
	case eTerrainFeature_WaterEdge:
	case eTerrainFeature_QuickSand:

		// Tanks cant drive through water/quicksand
		if (mPathSearchUnitType == eSprite_Tank_Human || mPathSearchUnitType == eSprite_Tank_Enemy)
			return 0;

		return 2;
	}

	return 1;
}

float cRandomMap::LeastCostEstimate(cPosition* nodeStart, cPosition* nodeEnd) {

	/* Compute the minimum path cost using distance measurement. It is possible
	   to compute the exact minimum path using the fact that you can move only
	   on a straight line or on a diagonal, and this will yield a better result.
	*/
	int dx = nodeStart->mX - nodeEnd->mX;
	int dy = nodeStart->mY - nodeEnd->mY;
	return (float)sqrt((double)(dx*dx) + (double)(dy*dy));
}

void cRandomMap::AdjacentCost(cPosition* node, std::vector< micropather::StateCost > *neighbors) {
	const int dx[8] = { 8, 8, 0, -8, -8, -8, 0, 8 };
	const int dy[8] = { 0, 8, 8, 8, 0, -8, -8, -8 };
	const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

	for (int i = 0; i < 8; ++i) {
		int nx = node->mX + dx[i];
		int ny = node->mY + dy[i];

		if (nx < 0 || ny < 0)
			continue;

		int pass = Passable(nx, ny);
		if (pass > 0) {
			if (pass == 1)
			{
				// Normal floor
				micropather::StateCost nodeCost = { cPosition(nx, ny), cost[i] };
				neighbors->push_back(nodeCost);
			}
			else
			{
				// Normal floor
				micropather::StateCost nodeCost = { cPosition(nx, ny), cost[i] + pass };
				neighbors->push_back(nodeCost);
			}
		}
	}
}

void cRandomMap::PrintStateInfo(cPosition* node) {
	printf("(%d,%d)", node->mX, node->mY);
}
