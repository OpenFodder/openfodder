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
#include "Utils/dukglue/dukglue.h"
#include "Utils/SimplexNoise.hpp"
#include "Map/Random.hpp"

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

cScriptingEngine::cScriptingEngine() {

	mContext = duk_create_heap_default();
	mMap = 0;

	init();
	spritesCreateObject();

	scriptsLoadFolder(g_ResourceMan->GetScriptPath("General/"));

	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Kill.All.Enemy/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Destroy.Enemy.Buildings/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Rescue.Hostages/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Protect.Civilians/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Kidnap.Leader/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Destroy.Factory/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Destroy.Computer/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Get.Civilian.Home/"));
	// CF2 Engine
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Activate.Switches/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Objectives/Rescue.Hostage/"));

	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Terrain/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("Terrain/Jungle/"));

}

cScriptingEngine::~cScriptingEngine() {

	duk_destroy_heap(mContext);
}

void cScriptingEngine::spritesCreateObject() {

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
	for (auto &TileType : mTileTypes) {

		duk_push_int(mContext, TileType.mType);
		duk_put_prop_string(mContext, obj_idx, TileType.mFullName.c_str());
	}
	duk_put_global_string(mContext, "TileTypes");

	duk_push_global_object(mContext);
	obj_idx = duk_push_object(mContext);
	for (int x = 0; x < mTerrainTypeNames.size(); ++x) {

		duk_push_int(mContext, (eTerrainType)x);
		duk_put_prop_string(mContext, obj_idx, mTerrainTypeNames[x].c_str());
	}
	duk_put_global_string(mContext, "TerrainType");

}

void cScriptingEngine::init() {

	dukglue_register_function(mContext, print, "print");

	dukglue_register_method(mContext, &cScriptingEngine::scriptCall, "scriptCall");

	// SimplexNoise
	dukglue_register_constructor<SimplexNoise, float, float, float>(mContext, "SimplexNoise");
	dukglue_register_method(mContext, &SimplexNoise::fractalX, "fractalX");
	dukglue_register_method(mContext, &SimplexNoise::fractalXY, "fractalXY");
	dukglue_register_method(mContext, &SimplexNoise::fractalXYZ, "fractalXYZ");
	dukglue_register_method(mContext, &SimplexNoise::noiseX, "noiseX");
	dukglue_register_method(mContext, &SimplexNoise::noiseX, "noiseXY");
	dukglue_register_method(mContext, &SimplexNoise::noiseX, "noiseXYZ");

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

	dukglue_register_method(mContext, &cRandomMap::addBarracks, "addBarracks");

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

	dukglue_register_method(mContext, &cRandomMap::getRandomInt, "getRandomInt");
	dukglue_register_method(mContext, &cRandomMap::getRandomFloat, "getRandomFloat");

	dukglue_register_method(mContext, &cRandomMap::getDistanceBetweenPositions, "getDistanceBetweenPositions");
	
	dukglue_register_global(mContext, this, "ScriptingEngine");
}

bool cScriptingEngine::scriptCall(const std::string& pFilename) {

	auto path = g_ResourceMan->GetScriptPath(pFilename);

	if (path.size()) {
		auto script = g_ResourceMan->FileReadStr(path);
		return scriptLoad(script);
	}

	return false;
}

bool cScriptingEngine::scriptLoad(const std::string& pJS) {
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

bool cScriptingEngine::scriptsLoadFolder(const std::string& pFolder) {
	auto scripts = g_ResourceMan->DirectoryList(pFolder, "js");

	for (auto scriptFile : scripts) {
		auto script = g_ResourceMan->FileReadStr(pFolder + scriptFile);
		
		if (scriptLoad(script) == false) {
			g_Debugger->Error(script + " Failed to execute");
		}
	}

	return true;
}

bool cScriptingEngine::scriptRun(const std::string& pJS) {
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

void cScriptingEngine::Randomise(std::shared_ptr<cRandomMap> pMap, const std::string& pScript) {
	mMap = pMap;
	dukglue_register_global(mContext, mMap, "Map");

	auto path = g_ResourceMan->GetScriptPath(pScript);
	auto script = g_ResourceMan->FileReadStr(path);
	if (scriptRun(script) == false) {
		g_Debugger->Error(pScript + " Failed to execute");
	}


}
