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
#include "Utils/diamondsquare.hpp"
#include "Utils/dukglue/dukglue.h"

cRandomMap::cRandomMap(const sMapParams& pParams) : cOriginalMap() {
	mParams = pParams;

	mData = std::make_shared<std::vector<uint8_t>>();
	ClearTiles(0);

	saveHeader();
}

void cRandomMap::Randomise() {
	ClearTiles(0);
	mRandomSprites = std::make_shared<cRandomSprites>(this);
	
	Randomise_Tiles();
	Randomise_TileSmooth();
	Randomise_Structures(2);
	//Randomise_Sprites(2);

	mRandomSprites->Randomise();
}

 // persistance closer to 0 produces more rivers,
 // but a higher number produces more mountains

void cRandomMap::Randomise_Tiles() {
	float scale = mParams.mRandom.getf(0.01f, 0.1f);
	float lacunarity = mParams.mRandom.getf(0.1f, 0.5f);
	float persistance = mParams.mRandom.getf(0.1f, 1.f);	// higher produces more trees
	float offset_z = mParams.mRandom.getf(0.1f, 10.f);

	const SimplexNoise simplex(scale, scale, lacunarity, persistance);
	const int octaves = static_cast<int>(5 + std::log(scale));

	// jungle only
	int16 TileWater = 326;
	//int16 TileQuickSand = 167;
	int16 TileLand = 123;
	int16 TileBounce = 82;

	int16* MapPtr = (int16*)(mData->data() + 0x60);

	for (size_t Y = 0; Y < mParams.mHeight; ++Y) {

		for (size_t X = 0; X < mParams.mWidth; ++X) {

			const float noise = simplex.fractal(octaves, (float)X, (float)Y, offset_z);

			if (noise < -0.500f) {
				*MapPtr = TileWater;
			}
			else if (noise < -0.020f) {
				*MapPtr = TileWater;
			}
			else if (noise < -0.000f) {
				*MapPtr = TileLand;
			}
			else if (noise < 0.005f) {
				*MapPtr = TileLand;
			}
			else if (noise > 0.300f && noise < 0.400f) {
				*MapPtr = TileLand;// TileQuickSand;
			}
			else if (noise < 0.500f) {
				*MapPtr = TileLand;
			}
			else if (noise < 0.700f) {
				*MapPtr = TileLand;
			}
			else if (noise < 0.900f) {
				*MapPtr = TileBounce;
			}
			else {
				*MapPtr = TileBounce;
			}
			++MapPtr;
		}
	}

}

void cRandomMap::Randomise_TileSmooth() {
	/*
	for (int32 y = 1; y < mHeight; ++y) {
		for (int32 x = 1; x < mWidth; ++x) {

			int32 TileX = x * 16;
			int32 TileY = y * 16;

			int16 Tile          = Tile_Get(TileX, TileY);

			int16 TileUp        = Tile_Get(TileX,      TileY - 1);
			int16 TileLeftUp    = Tile_Get(TileX - 1,  TileY - 1);
			int16 TileLeft      = Tile_Get(TileX - 1,  TileY);
			int16 TileLeftDown  = Tile_Get(TileX - 1,  TileY + 1);
			int16 TileDown      = Tile_Get(TileX,      TileY + 1);
			int16 TileRightDown = Tile_Get(TileX + 1,  TileY + 1);
			int16 TileRight     = Tile_Get(TileX + 1,  TileY);
			int16 TileRightUp   = Tile_Get(TileX + 1,  TileY - 1);

		} // Width
	} // Height
	*/
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


void cRandomMap::Randomise_Tiles_DS() {
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

	cDiamondSquare DS(PowerOf, mParams.mRandom.getStartingSeed());
	auto HeightMap = DS.generate();

	int16* MapPtr = (int16*)(mData->data() + 0x60);

	// Find the highest and lowest points in the height map
	double HeightMin = 0;
	double HeightMax = 0;
	for (auto& Row : HeightMap) {
		for (auto& Column : Row) {

			if (Column > HeightMax) HeightMax = Column;
			if (Column < HeightMin) HeightMin = Column;
		}
	}

	// Calcukate the difference between top/bottom
	double diff = HeightMax - HeightMin;
	double flood = 0.3;
	double mount = 0.7;

	// Calculate the flood/moutain levels
	flood *= diff;
	mount *= diff;

	// Jungle
	int16 TileWater = 326;
	int16 TileLand = 123;
	int16 TileBounce = 82;
	size_t X = 0;
	size_t Y = 0;

	switch (mParams.mTileType) {
	case eTileTypes_Ice:
		TileLand = 1;
		break;
	default:
		break;
	}

	static bool Found = false;

	// Loop each tile row
	for (auto Row : HeightMap) {
		X = 0;
		// Loop each tile column
		for (auto Column : Row) {
			Column -= HeightMin;

			if (Column < flood) {
				//if(!Found)
				*MapPtr = TileWater;
				Found = true;
			}
			else if (Column > mount) {
				*MapPtr = TileBounce;
			}
			else {
				*MapPtr = TileLand;
			}
			++MapPtr;

			if (++X >= mParams.mWidth)
				break;
		}

		if (++Y >= mParams.mHeight)
			break;
	}
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

		int32 yspan = (int32) (pRadius * std::sin(std::acos((pPosition->mX - x) / pRadius)));

		for (int32 y = pPosition->mY - yspan; y < pPosition->mY + yspan; y++) {
			
			if (g_Fodder->Map_Terrain_Get(x, y) != pType)
				return false;
		}
	}

	return true;
}

int32 cRandomMap::getRandomNumber(int32 pMin, int32 pMax) {
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

namespace dukglue {
	namespace types {
		DUKGLUE_SIMPLE_VALUE_TYPE(eTileTypes, duk_is_number, duk_get_uint, duk_push_uint, value);
		DUKGLUE_SIMPLE_VALUE_TYPE(eTileSub, duk_is_number, duk_get_uint, duk_push_uint, value);
		DUKGLUE_SIMPLE_VALUE_TYPE(eTerrainType, duk_is_number, duk_get_uint, duk_push_uint, value);

	}
}

void print(const std::string pString) {
	std::cout << pString << "\n";
}

cRandomSprites::cRandomSprites(cRandomMap *pMap) {

	mContext = duk_create_heap_default();
	mMap = pMap;

	init();
	spritesCreateObject();

	scriptsLoadFolder( g_ResourceMan->GetScriptPath("General/") );

	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Kill.All.Enemy/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Destroy.Enemy.Building/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Rescue.Hostages/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Protect.Civilians/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Kidnap.Leader/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Destroy.Factory/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Destroy.Computer/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Get.Civilian.Home/"));

	// CF2 Engine
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Activate.Switches/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Rescue.Hostage/"));
}

cRandomSprites::~cRandomSprites() {

	duk_destroy_heap(mContext);
}

void cRandomSprites::spritesCreateObject() {

	// Setup the "SpriteTypes" object
	duk_push_global_object(mContext);
	duk_idx_t obj_idx = duk_push_object(mContext);
	for (int x = 0; x < 118; ++x) {
		if (!g_Fodder->mSprite_Names[x].size())
			continue;

		duk_push_int(mContext, x);
		duk_put_prop_string(mContext, obj_idx, g_Fodder->mSprite_Names[x].c_str());
	}
	duk_put_global_string(mContext, "SpriteTypes");

	// Setup Tilesets
	duk_push_global_object(mContext);
	obj_idx = duk_push_object(mContext);
	for(auto &TileType : mTileTypes) {

		duk_push_int(mContext, TileType.mType);
		duk_put_prop_string(mContext, obj_idx, TileType.mFullName.c_str());
	}
	duk_put_global_string(mContext, "TileTypes");

	duk_push_global_object(mContext);
	obj_idx = duk_push_object(mContext);
	for (int x = 0; x < mTerrainTypeNames.size(); ++x) {

		duk_push_int(mContext, (eTerrainType) x);
		duk_put_prop_string(mContext, obj_idx, mTerrainTypeNames[x].c_str());
	}
	duk_put_global_string(mContext, "TerrainType");
	
}

void cRandomSprites::init() {

	dukglue_register_function(mContext, print, "print");

	dukglue_register_method(mContext, &cRandomSprites::scriptCall, "scriptCall");

	// cPosition
	dukglue_register_constructor<cPosition>(mContext, "cPosition");
	dukglue_register_property(mContext, &cPosition::getX, &cPosition::setX, "x");
	dukglue_register_property(mContext, &cPosition::getY, &cPosition::setY, "y");

	// sSprite
	dukglue_register_constructor<sSprite>(mContext, "sSprite");
	dukglue_register_method(mContext, &sSprite::Clear, "Clear");
	dukglue_register_property(mContext, &sSprite::getX, &sSprite::setX, "x");
	dukglue_register_property(mContext, &sSprite::getY, &sSprite::setY, "y");

	// cMap
	dukglue_set_base_class<cMap, cRandomMap>(mContext);
	dukglue_register_constructor<cRandomMap, const sMapParams& >(mContext, "cRandomMap");
	dukglue_register_method(mContext, &cRandomMap::getTileType, "getTileType");
	dukglue_register_method(mContext, &cRandomMap::getTileSub, "getTileSub");

	dukglue_register_method(mContext, &cRandomMap::getWidth, "getWidth");
	dukglue_register_method(mContext, &cRandomMap::getHeight, "getHeight");
	dukglue_register_method(mContext, &cRandomMap::getWidthPixels, "getWidthPixels");
	dukglue_register_method(mContext, &cRandomMap::getHeightPixels, "getHeightPixels");

	dukglue_register_method(mContext, &cRandomMap::getSpriteTypeCount, "getSpriteTypeCount");
	dukglue_register_method(mContext, &cRandomMap::getSpritesByType, "getSpritesByType");
	dukglue_register_method(mContext, &cRandomMap::getRandomXYByTerrainType, "getRandomXYByTerrainType");
	
	dukglue_register_method(mContext, &cRandomMap::Sprite_Add, "SpriteAdd");
	dukglue_register_method(mContext, &cRandomMap::Tile_Get, "TileGet");
	dukglue_register_method(mContext, &cRandomMap::Tile_Set, "TileSet");

	dukglue_register_method(mContext, &cRandomMap::getRandomNumber, "getRandomNumber");

	dukglue_register_global(mContext, mMap, "Map");
	dukglue_register_global(mContext, this, "RandomSprite");
}

bool cRandomSprites::scriptCall(const std::string& pFilename) {

	auto path = g_ResourceMan->GetScriptPath(pFilename);

	if (path.size()) {
		auto script = g_ResourceMan->FileReadStr(path);
		return scriptLoad(script);
	}

	return false;
}

bool cRandomSprites::scriptLoad(const std::string& pJS) {
	// Compile the JS into bytecode
	if (duk_pcompile_string(mContext, 0, pJS.c_str()) != 0) {
		g_Debugger->Error("Compile failed: ");
		g_Debugger->Error(duk_safe_to_string(mContext, -1));
		return false;
	}
	else {
		duk_pcall(mContext, 0);
		//printf("program result: %s\n", duk_safe_to_string(mContext, -1));
	}
	duk_pop(mContext);
	return true;
}

bool cRandomSprites::scriptsLoadFolder(const std::string& pFolder) {
	auto scripts = g_ResourceMan->DirectoryList(pFolder, "js");

	for (auto scriptFile : scripts) {
		auto script = g_ResourceMan->FileReadStr(pFolder + scriptFile);
		scriptLoad(script);
	}

	return true;
}

bool cRandomSprites::scriptRun(const std::string& pJS) {
	bool success = false;

	if (duk_pcompile_string(mContext, 0, pJS.c_str()) != 0) {
		g_Debugger->Error("Compile failed: ");
		g_Debugger->Error(duk_safe_to_string(mContext, -1));
	}
	else {
		duk_pcall(mContext, 0);
		success = true;
		printf("program result: %s\n", duk_safe_to_string(mContext, -1));
	}
	duk_pop(mContext);
	return success;
}

void cRandomSprites::Randomise() {
	auto path = g_ResourceMan->GetScriptPath("test.js");

	auto script = g_ResourceMan->FileReadStr(path);
	scriptRun(script);

}
