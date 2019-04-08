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
		DUKGLUE_SIMPLE_VALUE_TYPE(eTerrainFeature, duk_is_number, duk_get_uint, duk_push_uint, value);
		DUKGLUE_SIMPLE_VALUE_TYPE(ePhaseObjective, duk_is_number, duk_get_uint, duk_push_uint, value);
	}
}

void print(const std::string pString) {

	g_Debugger->Notice(pString);
}

void consoleLog(DukValue pValue) {
	std::string json = duk_json_encode(pValue.context(), -1);
	g_Debugger->Notice(json);
}

void consoleClear() {
	g_Debugger->ClearConsole();
}

cScriptFileIO::cScriptFileIO(std::string pFilename, bool pRead) {

	mStream.open(pFilename, (pRead == true) ? std::ios::in : std::ios::out);
}

cScriptFileIO::~cScriptFileIO() {

	mStream.close();
}

std::string cScriptFileIO::readLine() {
	std::string line;

	std::getline(mStream, line);
	return line;
}

void cScriptFileIO::writeLine(std::string pStr) {

	if (isOpen())
		mStream << pStr << "\n";
}

bool cScriptFileIO::isOpen() {
	return mStream.is_open();
}

void cScriptFileIO::close() {
	mStream.close();
}

cScriptingEngine::cScriptingEngine() {

	mContext = duk_create_heap_default();

	init();
	spritesCreateObject();

	scriptsLoadFolder(g_ResourceMan->GetScriptPath("General/"));
	scriptsLoadFolder(g_ResourceMan->GetScriptPath("General/Names/"));
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

	// This is left here incase similar functionality is needed later

	// Setup SpriteTypes.
	/*duk_push_global_object(mContext);
	duk_idx_t obj_idx = duk_push_object(mContext);
	for (size_t x = 0; x < 118; ++x) {
		if (!g_Fodder->mSprite_Names[x].size())
			continue;

		duk_push_int(mContext, x);
		duk_put_prop_string(mContext, obj_idx, g_Fodder->mSprite_Names[x].c_str());
	}
	duk_put_global_string(mContext, "SpriteTypes");*/

}

void cScriptingEngine::init() {

	dukglue_register_function(mContext, print, "print");
	dukglue_register_function(mContext, consoleLog, "consoleLog");
	dukglue_register_function(mContext, consoleClear, "consoleClear");

	// cScriptFileIO
	dukglue_register_constructor<cScriptFileIO, std::string, bool >(mContext, "FileIO");
	dukglue_register_method(mContext, &cScriptFileIO::readLine, "readLine");
	dukglue_register_method(mContext, &cScriptFileIO::writeLine, "writeLine");
	dukglue_register_method(mContext, &cScriptFileIO::close, "close");
	dukglue_register_method(mContext, &cScriptFileIO::isOpen, "isOpen");

	// cPosition
	dukglue_register_constructor<cPosition>(mContext, "cPosition");

	dukglue_register_property(mContext, &cPosition::getX, &cPosition::setX, "x");
	dukglue_register_property(mContext, &cPosition::getY, &cPosition::setY, "y");

	// sSprite
	dukglue_register_constructor<sSprite>(mContext, "sSprite");
	dukglue_register_method(mContext, &sSprite::Clear, "Clear");
	dukglue_register_method(mContext, &sSprite::getPosition, "getPosition");
	dukglue_register_property(mContext, &sSprite::getX, &sSprite::setX, "x");
	dukglue_register_property(mContext, &sSprite::getY, &sSprite::setY, "y");
	
	// cMap
	dukglue_set_base_class<cMap, cRandomMap>(mContext);
	dukglue_register_constructor<cRandomMap, const sMapParams& >(mContext, "cRandomMap");

	dukglue_register_method(mContext, &cRandomMap::save, "save");
	dukglue_register_method(mContext, &cRandomMap::addBarracks, "addBarracks");
	dukglue_register_method(mContext, &cRandomMap::create, "Create");
	dukglue_register_method(mContext, &cRandomMap::createRandom, "CreateRandom");
	
	dukglue_register_method(mContext, &cRandomMap::createDiamondSquare, "DiamondSquare");
	dukglue_register_method(mContext, &cRandomMap::createSimplexIslands, "SimplexIslands");
	dukglue_register_method(mContext, &cRandomMap::createSimplexNoise, "SimplexNoise");

	
	dukglue_register_method(mContext, &cRandomMap::getTileType, "getTileType");
	dukglue_register_method(mContext, &cRandomMap::getTileSub, "getTileSub");

	dukglue_register_method(mContext, &cRandomMap::getWidth, "getWidth");
	dukglue_register_method(mContext, &cRandomMap::getHeight, "getHeight");
	dukglue_register_method(mContext, &cRandomMap::getWidthPixels, "getWidthPixels");
	dukglue_register_method(mContext, &cRandomMap::getHeightPixels, "getHeightPixels");

	dukglue_register_method(mContext, &cRandomMap::getSpriteTypeCount, "getSpriteTypeCount");
	dukglue_register_method(mContext, &cRandomMap::getSpritesByType, "getSpritesByType");

	dukglue_register_method(mContext, &cRandomMap::getRandomXYByTileID, "getRandomXYByTileID");
	dukglue_register_method(mContext, &cRandomMap::getRandomXYByFeatures, "getRandomXYByFeatures");

	dukglue_register_method(mContext, &cRandomMap::getRandomXYByTerrainType, "getRandomXYByTerrainType");

	dukglue_register_method(mContext, &cRandomMap::Sprite_Add, "SpriteAdd");
	dukglue_register_method(mContext, &cRandomMap::Tile_Get, "TileGet");
	dukglue_register_method(mContext, &cRandomMap::Tile_Set, "TileSet");

	dukglue_register_method(mContext, &cRandomMap::getRandomInt, "getRandomInt");
	dukglue_register_method(mContext, &cRandomMap::getRandomFloat, "getRandomFloat");

	dukglue_register_method(mContext, &cRandomMap::getDistanceBetweenPositions, "getDistanceBetweenPositions");
	dukglue_register_method(mContext, &cRandomMap::calculatePath, "calculatePathBetweenPositions");

	// cPhase
	dukglue_register_constructor<cPhase>(mContext, "cPhase");
	dukglue_register_property(mContext, &cPhase::GetMapFilename, &cPhase::SetMapFilename, "map");
	dukglue_register_property(mContext, &cPhase::GetName, &cPhase::SetName, "name");

	dukglue_register_method(mContext, &cPhase::AddGoal, "ObjectiveAdd");
	dukglue_register_method(mContext, &cPhase::RemoveGoal, "ObjectiveRemove");
	dukglue_register_method(mContext, &cPhase::ClearGoals, "ObjectivesClear");
	dukglue_register_method(mContext, &cPhase::SetAggression, "SetAggression");
	dukglue_register_method(mContext, &cPhase::SetMinAggression, "SetMinAggression");
	dukglue_register_method(mContext, &cPhase::SetMaxAggression, "SetMaxAggression");

	// cMission
	dukglue_register_constructor<cMission>(mContext, "cMission");
	dukglue_register_property(mContext, &cMission::GetName, &cMission::SetName, "name");
	dukglue_register_method(mContext, &cMission::NumberOfPhases, "NumberOfPhases");
	dukglue_register_method(mContext, &cMission::PhaseGet, "PhaseGet");

	// cCampaign
	dukglue_register_constructor<cCampaign>(mContext, "cCampaign");
	dukglue_register_property(mContext, &cCampaign::getName, &cCampaign::setName, "name");
	dukglue_register_property(mContext, &cCampaign::getAuthor, &cCampaign::setAuthor, "author");
	dukglue_register_method(mContext, &cCampaign::getMissions, "getMissions");
	dukglue_register_method(mContext, &cCampaign::SetCustomCampaign, "SetCustomCampaign");
	
	// sGameData
	dukglue_register_constructor<sGameData>(mContext, "sGameData");
	dukglue_register_method(mContext, &sGameData::Phase_Start, "Phase_Start");
	dukglue_register_method(mContext, &sGameData::Phase_Next, "Phase_Next");


	// cScriptingEngine
	dukglue_register_constructor<cScriptingEngine>(mContext, "cEngine");
	dukglue_register_method(mContext, &cScriptingEngine::scriptCall, "scriptCall");
	dukglue_register_method(mContext, &cScriptingEngine::mapSave, "mapSave");
	dukglue_register_method(mContext, &cScriptingEngine::phaseCreate, "phaseCreate");
	dukglue_register_method(mContext, &cScriptingEngine::missionCreate, "missionCreate");

	dukglue_register_method(mContext, &cScriptingEngine::getCampaign, "getCampaign");
	dukglue_register_method(mContext, &cScriptingEngine::getMap, "getMap");
	dukglue_register_method(mContext, &cScriptingEngine::getPhase, "getPhase");
	dukglue_register_method(mContext, &cScriptingEngine::getMission, "getMission");
}

std::shared_ptr<cPhase> cScriptingEngine::phaseCreate() {
	auto phase = std::make_shared<cPhase>();
	getMission()->mPhases.push_back(phase);
	++g_Fodder->mGame_Data.mMission_Phases_Remaining;

	mapSave();
	g_Fodder->mCustom_Mode = eCustomMode_Set;
	g_Fodder->mGame_Data.Phase_Next();
	return phase;
}

std::shared_ptr<cMission> cScriptingEngine::missionCreate() {
	auto mission = std::make_shared<cMission>();
	auto phase = std::make_shared<cPhase>();
	mission->mPhases.push_back(phase);
	getCampaign()->missionAdd(mission);

	mapSave();
	g_Fodder->mCustom_Mode = eCustomMode_Set;
	g_Fodder->mGame_Data.Phase_Next();
	return mission;
}

cCampaign* cScriptingEngine::getCampaign() {
	return &g_Fodder->mGame_Data.mCampaign;
}

std::shared_ptr<cRandomMap> cScriptingEngine::getMap() {
	return std::dynamic_pointer_cast<cRandomMap>(g_Fodder->mMapLoaded);
}

std::shared_ptr<cPhase> cScriptingEngine::getPhase() {
	return g_Fodder->mGame_Data.mPhase_Current;
}
std::shared_ptr<cMission> cScriptingEngine::getMission() {
	return g_Fodder->mGame_Data.mMission_Current;
}

void cScriptingEngine::mapSave() {

	if (getPhase()->GetMapFilename().size() == 0) {
		getPhase()->SetMapFilename("random");
	}

	g_Fodder->mMapLoaded->save(getCampaign()->GetPathToFile(getPhase()->GetMapFilename()), true);
}

bool cScriptingEngine::scriptCall(const std::string& pFilename) {

	auto path = g_ResourceMan->GetScriptPath(pFilename);

	if (path.size()) {
		auto script = g_ResourceMan->FileReadStr(path);
		return scriptRun(script);
	}

	return false;
}

bool cScriptingEngine::scriptsLoadFolder(const std::string& pFolder) {
	auto scripts = g_ResourceMan->DirectoryList(pFolder, "js");

	for (auto scriptFile : scripts) {
		auto script = g_ResourceMan->FileReadStr(pFolder + scriptFile);
		
		if (scriptRun(script) == false) {
			g_Debugger->Error(pFolder + scriptFile + " Failed to execute");
		}
	}

	return true;
}

bool cScriptingEngine::scriptRun(const std::string& pJS) {
	int success = DUK_EXEC_SUCCESS;

	// Compile the JS into bytecode
	if (duk_pcompile_string(mContext, 0, pJS.c_str()) != 0) {
		g_Debugger->Error("Compile failed: ");
	} else {
		success = duk_pcall(mContext, 0);
		if(success != DUK_EXEC_SUCCESS)
			g_Debugger->Error("Execute failed: ");
	}

	if (duk_is_error(mContext, -1)) {
		duk_get_prop_string(mContext, -1, "stack");
		g_Debugger->Error(std::string(duk_safe_to_string(mContext, -1)));
	}
	else {
		if (success != DUK_EXEC_SUCCESS)
			g_Debugger->Error(std::string(duk_safe_to_string(mContext, -1)));
	}

	duk_pop(mContext);
	return (success == DUK_EXEC_SUCCESS);
}

void cScriptingEngine::Run(const std::string& pScript) {

	dukglue_register_global(mContext, this, "Engine");

	auto path = g_ResourceMan->GetScriptPath(pScript);
	auto script = g_ResourceMan->FileReadStr(path);

	if (scriptRun(script) == false) {
		g_Debugger->Error(path + " Failed to execute");
	}

}
