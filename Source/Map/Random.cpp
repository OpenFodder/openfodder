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

void cRandomMap::Randomise_Sprites(const size_t pHumanCount) {
	int16 DistanceY = 8;
	int16 DistanceX = 8;

	int16 MiddleX = 0;
	int16 MiddleY = 0;

	int16 TileLand = 123;

	size_t Count = 0;
	while (Count < 100000) {
		int16 StartTileX = g_Fodder->tool_RandomGet(2, mParams.mWidth - 2);
		int16 StartTileY = g_Fodder->tool_RandomGet(2, mParams.mHeight - 2);

		// TODO: Check if we will overlap an existing structure,
		//       or place on water
		auto Tile = Tile_Get(StartTileX, StartTileY);
		if (Tile != TileLand)
			continue;

		MiddleX = StartTileX * 16;
		MiddleY = StartTileY * 16;
		break;
	}

	// Add atleast two sprites
	for (size_t x = 0; x < pHumanCount; ++x) {
		if (g_Fodder->tool_RandomGet() % 2)
			Sprite_Add(eSprite_Player, MiddleX - DistanceX, MiddleY + DistanceY);
		else
			Sprite_Add(eSprite_Player, MiddleX + DistanceX, MiddleY - DistanceY);

		DistanceX += 6;
		DistanceY += 6;
	}


	// Add some weapons
	Sprite_Add(eSprite_RocketBox, MiddleX, MiddleY + DistanceY);
	Sprite_Add(eSprite_GrenadeBox, MiddleX + DistanceX, MiddleY + DistanceY);
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

		int32 yspan = (int32) (pRadius * sin(acos((pPosition->mX - x) / pRadius)));

		for (int32 y = pPosition->mY - yspan; y < pPosition->mY + yspan; y++) {
			
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
		Position->mX = (mParams.mRandom.getu() % (mParams.mWidth - pRadius)) * 16;
		Position->mY = (mParams.mRandom.getu() % (mParams.mHeight - pRadius)) * 16;

		if (count++ > 1000) {
			Position->mX = -1;
			Position->mY = -1;
			break;
		}

	} while (!CheckRadiusTerrain(pType, Position, Radius));

	// Scan from top left to bottom right
	if (Position->mX == -1 || Position->mY == -1) {

		for (Position->mY = Radius; Position->mY < getHeightPixels() - 16; Position->mY += Radius) {

			for (Position->mX = Radius; Position->mX < getWidthPixels() - 16; Position->mX += Radius) {

				if (CheckRadiusTerrain(pType, Position, Radius))
					return Position;
			}
		}
		// Fail
	}

	return Position;
}
