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
#include "Random.hpp"

#include "Utils/SimplexNoise.hpp"
#include "Utils/SimplexIslands.hpp"


cRandomMap::cRandomMap(const sMapParams& pParams) : cOriginalMap() {
	mParams = pParams;

	mData = std::make_shared<std::vector<uint8_t>>();
	ClearTiles(0);

	saveHeader();
}

void cRandomMap::Randomise() {
	ClearTiles(0);

	//Randomise_Structures(2);
	//Randomise_Sprites(2);
}

void cRandomMap::Randomise_Structures(const size_t pCount) {

	size_t StructsCount = 0;

	int16 TileLand = 123;

	// This is very lame :)
	while (StructsCount < pCount) {
		auto Struct = mStructuresBarracksWithSoldier[mParams.mTileType];

		size_t StartTileX = (size_t)g_Fodder->tool_RandomGet(Struct.MaxWidth() + 2, mParams.mWidth - Struct.MaxWidth());
		size_t StartTileY = (size_t)g_Fodder->tool_RandomGet(Struct.MaxHeight() + 2, mParams.mHeight - Struct.MaxWidth());

		// TODO: Check if we will overlap an existing structure,
		//       or place on water
		auto Tile = Tile_Get(StartTileX, StartTileY);
		if (Tile != TileLand)
			continue;

		Structure_Add(Struct, StartTileX, StartTileY);

		// Add an enemy below each building
		//Sprite_Add(eSprite_Enemy, StartTileX * 16, (StartTileY + 3) * 16);

		++StructsCount;
	}
}

void cRandomMap::addBarracks(size_t pX, size_t pY) {

	Structure_Add(mStructuresBarracksWithSoldier[mParams.mTileType], pX / 16, pY / 16);
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

bool cRandomMap::CheckRadiusTerrain(eTerrainType pType, cPosition* pPosition, int32 pRadius) {

	for (int32 x = pPosition->mX - pRadius; x < pPosition->mX + pRadius; x++) {

		if (x < 0)
			continue;

		int32 yspan = (int32) (pRadius * sin(acos((pPosition->mX - x) / pRadius)));

		for (int32 y = pPosition->mY - yspan; y < pPosition->mY + yspan; y++) {
			
			if (y < 0)
				continue;

			if (g_Fodder->Map_Terrain_Get(x, y) != pType)
				return false;
		}
	}

	return true;
}

float cRandomMap::getRandomFloat(float pMin, float pMax) {

	return mParams.mRandom.getf(pMin, pMax);
}

int32 cRandomMap::getRandomInt(int32 pMin, int32 pMax) {
	if(pMin || pMax)
		return mParams.mRandom.getu(pMin, pMax);

	return mParams.mRandom.getu();
}

cPosition* cRandomMap::getRandomXYByTerrainType(eTerrainType pType, size_t pRadius) {
	size_t Radius = pRadius * 16;

	cPosition* Position = new cPosition();

	int count = 0;

	do {
		Position->mX = ((mParams.mRandom.getu() + pRadius) % (mParams.mWidth - pRadius)) * 16;
		Position->mY = ((mParams.mRandom.getu() + pRadius) % (mParams.mHeight - pRadius)) * 16;

		if (count++ > 1000) {
			Position->mX = -1;
			Position->mY = -1;
			break;
		}

	} while (!CheckRadiusTerrain(pType, Position, Radius));

	// Scan from top left to bottom right
	if (Position->mX == -1 || Position->mY == -1) {

		for (Position->mY = Radius; Position->mY < getHeightPixels() - 32; Position->mY += Radius) {

			for (Position->mX = Radius; Position->mX < getWidthPixels() - 32; Position->mX += Radius) {

				if (CheckRadiusTerrain(pType, Position, Radius))
					return Position;
			}
		}
		// Fail
	}

	return Position;
}

int32 cRandomMap::getDistanceBetweenPositions(cPosition* pPos1, cPosition* pPos2) {

	return g_Fodder->Map_Get_Distance_BetweenPositions(*pPos1, *pPos2, 1000);
}


std::vector<std::vector<float>> cRandomMap::createSimplexIslands(size_t pOctaves, float pRoughness, float pScale, short pSeed, bool pRadialEnabled) {

	SimplexIslands Islands;

	auto noise = Islands.CreateArray(mParams.mWidth, mParams.mHeight, pOctaves, pRoughness, pScale, pSeed, pRadialEnabled);

	return noise;
}

std::vector<std::vector<float>> cRandomMap::createSimplexNoise(size_t pOctaves, float pFrequency, float pAmplitude, float pLacunarity, float pPersistence) {
	SimplexNoise Noise(pFrequency, pAmplitude, pLacunarity, pPersistence);
	std::vector<std::vector<float>> result;

	for (size_t x = 0; x < mParams.mWidth; ++x) {

		std::vector<float> row;
		for (size_t y = 0; y < mParams.mHeight; ++y) {
		
			row.push_back(Noise.fractalXY(pOctaves, x, y));
		}

		result.push_back(row);
	}

	return result;

}
void cRandomMap::create(size_t pWidth, size_t pHeight, eTileTypes pTileType, eTileSub pTileSub) {

	mParams.mWidth = pWidth;
	mParams.mHeight = pHeight;
	mParams.mTileType = pTileType;
	mParams.mTileSub = pTileSub;

	ClearTiles(0);
	saveHeader();
}

void cRandomMap::createRandom(size_t pSeed) {

	if (!pSeed)
		pSeed = mParams.mRandom.getu();

	mParams.Randomise(pSeed);

	ClearTiles(0);
	saveHeader();
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
		case eSprite_Helicopter_Grenade2_Enemy:
		case eSprite_Helicopter_Missile_Enemy:
		case eSprite_Helicopter_Homing_Enemy:
		case eSprite_Helicopter_Grenade2_Human:
		case eSprite_Helicopter_Grenade_Human:
		case eSprite_Helicopter_Missile_Human:
		case eSprite_Helicopter_Homing_Human:
		case eSprite_Helicopter_Grenade2_Human_Called:
		case eSprite_Helicopter_Grenade_Human_Called:
		case eSprite_Helicopter_Missile_Human_Called:
		case eSprite_Helicopter_Homing_Human_Called:
		case eSprite_Helicopter_Homing_Enemy2:
			return 1;
	}

	switch (TerrainType) {
	default:
		return 1;

	case eTerrainType_Water:
	case eTerrainType_WaterEdge:
	case eTerrainType_QuickSand:

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
