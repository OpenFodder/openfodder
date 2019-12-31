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
#ifdef OF_JS_DEBUG
#include "Utils/duk_trans_socket.h"
#endif

namespace dukglue {
	namespace types {
		DUKGLUE_SIMPLE_VALUE_TYPE(eTileTypes, duk_is_number, duk_get_uint, duk_push_uint, value);
		DUKGLUE_SIMPLE_VALUE_TYPE(eTileSub, duk_is_number, duk_get_uint, duk_push_uint, value);
		DUKGLUE_SIMPLE_VALUE_TYPE(eTerrainFeature, duk_is_number, duk_get_uint, duk_push_uint, value);
		DUKGLUE_SIMPLE_VALUE_TYPE(ePhaseObjective, duk_is_number, duk_get_uint, duk_push_uint, value);
	}
}

void print(const std::string pString) {
#ifdef DEBUG
	g_Debugger->Notice(pString);
#endif
}

void consoleLog(DukValue pValue) {
#ifdef DEBUG
	std::string json = duk_json_encode(pValue.context(), -1);
	g_Debugger->Notice(json);
#endif
}

void consoleClear() {
#ifdef DEBUG
	g_Debugger->ClearConsole();
#endif
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

static duk_idx_t debugger_request(duk_context *ctx, void *udata, duk_idx_t nvalues) {
	const char *cmd;
	//int i;

	(void)udata;

	if (nvalues < 1) {
		duk_push_string(ctx, "missing AppRequest argument(s)");
		return -1;
	}

	cmd = duk_get_string(ctx, -nvalues + 0);
	/*
	if (cmd && strcmp(cmd, "CommandLine") == 0) {
		if (!duk_check_stack(ctx, main_argc)) {
			/* Callback should avoid errors for now, so use
			 * duk_check_stack() rather than duk_require_stack().
			 *
			duk_push_string(ctx, "failed to extend stack");
			return -1;
		}
		for (i = 0; i < main_argc; i++) {
			duk_push_string(ctx, main_argv[i]);
		}
		return main_argc;
	}*/
	duk_push_sprintf(ctx, "command not supported");
	return -1;
}

static void debugger_detached(duk_context *ctx, void *udata) {
#ifdef OF_JS_DEBUG
	fprintf(stderr, "Debugger detached, udata: %p\n", (void *)udata);
	fflush(stderr);

	/* Ensure socket is closed even when detach is initiated by Duktape
	 * rather than debug client.
	 */
	duk_trans_socket_finish();
	/*
	if (debugger_reattach) {
		// For automatic reattach testing.
		duk_trans_socket_init();
		duk_trans_socket_waitconn();
		fprintf(stderr, "Debugger reconnected, call duk_debugger_attach()\n");
		fflush(stderr);
		duk_debugger_attach(ctx,
			duk_trans_socket_read_cb,
			duk_trans_socket_write_cb,
			duk_trans_socket_peek_cb,
			duk_trans_socket_read_flush_cb,
			duk_trans_socket_write_flush_cb,
			debugger_request,
			debugger_detached,
			NULL);
	}*/
#endif
}

void cScriptingEngine::debuggerEnable() {

#ifdef OF_JS_DEBUG
	g_Debugger->Error("Debugger enabled, create socket and wait for connection\n");
	duk_trans_socket_init();
	duk_trans_socket_waitconn();
	g_Debugger->Error("Debugger connected\n");

	duk_debugger_attach(mContext,
		duk_trans_socket_read_cb,
		duk_trans_socket_write_cb,
		duk_trans_socket_peek_cb,
		duk_trans_socket_read_flush_cb,
		duk_trans_socket_write_flush_cb,
		debugger_request,
		debugger_detached,
		NULL);
#endif
}

cScriptingEngine::cScriptingEngine() {

	mContext = duk_create_heap_default();

	init();
	spritesCreateObject();

	scriptsLoadFolder("Terrain/");
	scriptsLoadFolder("Terrain/Jungle/");

	scriptsLoadFolder("General/");
	scriptsLoadFolder("General/Names/");
	scriptsLoadFolder("General/Structures/");

	scriptsLoadFolder("Objectives/");
	scriptsLoadFolder("Objectives/Kill.All.Enemy/");
	scriptsLoadFolder("Objectives/Destroy.Enemy.Buildings/");
	scriptsLoadFolder("Objectives/Rescue.Hostages/");
	scriptsLoadFolder("Objectives/Protect.Civilians/");
	scriptsLoadFolder("Objectives/Kidnap.Leader/");
	scriptsLoadFolder("Objectives/Destroy.Factory/");
	scriptsLoadFolder("Objectives/Destroy.Computer/");
	scriptsLoadFolder("Objectives/Get.Civilian.Home/");
	// CF2 Engine
	scriptsLoadFolder("Objectives/Activate.Switches/");
	scriptsLoadFolder("Objectives/Rescue.Hostage/");


	scriptsLoadFolder("Scenarios/");
	Run("Settings.js");
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
	dukglue_register_method(mContext, &cRandomMap::create, "Create");

	dukglue_register_method(mContext, &cRandomMap::createDiamondSquare, "DiamondSquare");
	dukglue_register_method(mContext, &cRandomMap::createSimplexIslands, "SimplexIslands");
	dukglue_register_method(mContext, &cRandomMap::createSimplexNoise, "SimplexNoise");

	
	dukglue_register_method(mContext, &cRandomMap::getTileType, "getTileType");
	dukglue_register_method(mContext, &cRandomMap::getTileSub, "getTileSub");
	
	dukglue_register_method(mContext, &cRandomMap::getArea, "getArea");
	dukglue_register_method(mContext, &cRandomMap::getAreaPixels, "getAreaPixels");
	
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

	dukglue_register_property(mContext, &cRandomMap::getSeed, &cRandomMap::setSeed, "seed");

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

	dukglue_register_method(mContext, &cScriptingEngine::guiPrintString, "guiPrintString");
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

void cScriptingEngine::guiPrintString(const std::string& pText, const size_t pX, const size_t pY, const bool pLarge, const bool pUnderline) {
	g_Fodder->mWindow->SetScreenSize(g_Fodder->mVersionCurrent->GetScreenSize());
	g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
	if (!pLarge) {
		if (!pX)
			g_Fodder->String_Print_Small(pText, pY);
		else
			g_Fodder->String_Print_Small(pText, pX, pY);
	}
	else {
		if (!pX)
			g_Fodder->String_Print_Large(pText, pUnderline, pY);
		else
			g_Fodder->String_Print_Large(pText, pUnderline, pX, pY);
	}

	g_Fodder->Video_SurfaceRender(false);
	g_Fodder->Cycle_End();
	g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
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
		return scriptRun(script, pFilename);
	}

	return false;
}

bool cScriptingEngine::scriptsLoadFolder(const std::string& pFolder) {

	auto finalpath = g_ResourceMan->GetScriptPath(pFolder);
	auto scripts = g_ResourceMan->DirectoryList(finalpath, "js");

	for (auto scriptFile : scripts) {

		auto finalName = pFolder + scriptFile;
		auto script = g_ResourceMan->FileReadStr(finalpath + scriptFile);
		
		if (!script.size() || (scriptRun(script, finalName) == false)) {
			g_Debugger->Error(finalpath + scriptFile + " Failed to execute");
			return false;
		}
	}

	return true;
}

bool cScriptingEngine::scriptRun(const std::string& pJS, const std::string& pFilename) {
	int success = DUK_EXEC_ERROR;

	// Compile the JS into bytecode
	duk_push_string(mContext, pFilename.c_str());
	if (duk_pcompile_string_filename(mContext, 0, pJS.c_str()) != 0) {
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

bool cScriptingEngine::Run(const std::string& pScript) {

	dukglue_register_global(mContext, this, "Engine");

	auto path = g_ResourceMan->GetScriptPath(pScript);
	auto script = g_ResourceMan->FileReadStr(path);

	if (!script.size() || (scriptRun(script, pScript) == false)) {
		g_Debugger->Error(path + " Failed to execute");
		return false;
	}

	return true;
}
