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
#include "Utils/dukglue/dukglue.h"
#include "Utils/SimplexNoise.hpp"
#include "Map/Random.hpp"
#include "JitScripting.hpp"
#include "Setup/DataRelease.hpp"
#include "Setup/EngineVersion.hpp"
#include <filesystem>
#include <stdexcept>
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

static duk_ret_t PositionConstructor(duk_context* ctx) {
	if (!duk_is_constructor_call(ctx)) {
		duk_error(ctx, DUK_RET_TYPE_ERROR, "Constructor must be called with new cPosition().");
		return DUK_RET_TYPE_ERROR;
	}

	const duk_idx_t ArgumentCount = duk_get_top(ctx);
	int X = 0;
	int Y = 0;

	if (ArgumentCount > 0 && !duk_is_undefined(ctx, 0))
		X = duk_require_int(ctx, 0);
	if (ArgumentCount > 1 && !duk_is_undefined(ctx, 1))
		Y = duk_require_int(ctx, 1);

	cPosition* Position = new cPosition();
	Position->setX(X);
	Position->setY(Y);

	duk_push_this(ctx);
	duk_push_pointer(ctx, Position);
	duk_put_prop_string(ctx, -2, "\xFF" "obj_ptr");
	dukglue::detail::RefManager::register_native_object(ctx, Position);
	duk_pop(ctx);

	return 0;
}

static void RegisterPositionConstructor(duk_context* ctx) {
	duk_push_c_function(ctx, PositionConstructor, DUK_VARARGS);
	dukglue::detail::ProtoManager::push_prototype<cPosition>(ctx);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "cPosition");
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
    // Legacy/manual script trees may lack a manifest. When a manifest is
    // supplied, reject damaged or incompatible metadata before executing JS.
    try {
        const auto root = g_ResourceMan->GetScriptPath("");
        if (root.empty())
            throw std::runtime_error("Scripts directory is missing");
        if (std::filesystem::exists(std::filesystem::path(root) / "installed.json")) {
            Setup::DataRelease release;
            int dv = 0, sv = 0;
            if (!release.ReadInstalledManifest(root, dv, sv))
                throw std::runtime_error(release.LastError());
            if (!Setup::IsScriptVersionCompatible(sv))
                throw std::runtime_error("Installed scripts are incompatible with this engine (version " + std::to_string(sv) + ")");
        }
    } catch (const std::exception& ex) {
        mScriptsLoaded = false;
        g_Debugger->Error(ex.what());
        return;
    }
    const bool requireJit = std::ifstream("mapgen_jit.flag").good();
    if(!std::ifstream("mapgen_interpreter.flag").good() && !g_Fodder->mStartParams->mDebugger) {
        auto jit = std::make_unique<cJitScripting>(mContext);
        if(jit->available()) mJit = std::move(jit);
    }
    if(requireJit && !mJit) {
        g_Debugger->Error("JIT scripting was requested but is unavailable");
        mScriptsLoaded = false;
    }
	spritesCreateObject();

	scriptsLoadFolder("Common/");
	scriptsLoadFolder("Common/Names/");
	scriptsLoadFolder("Common/Structures/");
	scriptsLoadFolder("Common/Generated/");

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

	scriptsLoadFolder("MapGen/");
	// P1.2: v3 IntentMap skeleton folder — must load BEFORE Grammar so
	// MapGen.Intent.* is available when Concepts register at module init.
	scriptsLoadFolder("MapGen/Intent/");
	scriptsLoadFolder("MapGen/Intent/Concepts/");
	scriptsLoadFolder("MapGen/Grammar/");
	scriptsLoadFolder("MapGen/Layout/");
	scriptsLoadFolder("MapGen/Layout/Templates/");
	scriptsLoadFolder("MapGen/Terrain/");
	scriptsLoadFolder("MapGen/Terrain/Smoothing/Data/");
	scriptsLoadFolder("MapGen/Terrain/Smoothing/");
	scriptsLoadFolder("MapGen/Features/");
	scriptsLoadFolder("MapGen/Encounters/");
	scriptsLoadFolder("MapGen/Decor/");
	scriptsLoadFolder("MapGen/Integration/");

	scriptsLoadFolder("Scenarios/");
    if (!Run("Settings.js"))
        mScriptsLoaded = false;
}

cScriptingEngine::~cScriptingEngine() {

    mJit.reset();
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
	RegisterPositionConstructor(mContext);

	dukglue_register_property(mContext, &cPosition::getX, &cPosition::setX, "x");
	dukglue_register_property(mContext, &cPosition::getY, &cPosition::setY, "y");

	// sSprite
	dukglue_register_constructor<sSprite>(mContext, "sSprite");
	dukglue_register_method(mContext, &sSprite::Clear, "Clear");
	dukglue_register_method(mContext, &sSprite::getPosition, "getPosition");
	dukglue_register_property(mContext, &sSprite::getX, &sSprite::setX, "x");
	dukglue_register_property(mContext, &sSprite::getY, &sSprite::setY, "y");
	
	// cMapData
	dukglue_set_base_class<cMapData, cRandomMap>(mContext);
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

	// Engine-authoritative terrain queries (HIT/BHT collision) so the generator
	// can ask what a tile actually IS in-game instead of guessing from tile IDs.
	dukglue_register_method(mContext, &cRandomMap::getTileTerrainFeature, "TileTerrainFeature");
	dukglue_register_method(mContext, &cRandomMap::isTileWalkable, "TileIsWalkable");

	// Native ice Wang-tile edge matcher (the hot ~47% of each ice render). The
	// authored atlas + per-cell semantics stay in JS (Core.ApplyEdgeRule); these
	// cache the atlas once and run the scoring loop.
	dukglue_register_method(mContext, &cRandomMap::setIceEdgeAtlas, "SetIceEdgeAtlas");
	dukglue_register_method(mContext, &cRandomMap::applyIceEdgeRule, "ApplyIceEdgeRule");
	dukglue_register_method(mContext, &cRandomMap::applyIceEdgeRuleMasked, "ApplyIceEdgeRuleMasked");
	dukglue_register_method(mContext, &cRandomMap::applyIceEdgeRuleMaskedRegion, "ApplyIceEdgeRuleMaskedRegion");

	// MapGen-native pathfinding over generator cost/walkability grids. The JS
	// generator owns layer semantics and keeps a safe fallback; these methods only
	// accelerate the hot deterministic search loops.
	dukglue_register_method(mContext, &cRandomMap::setMapGenPathCostGrid, "SetMapGenPathCostGrid");
	dukglue_register_method(mContext, &cRandomMap::setMapGenPathCost, "SetMapGenPathCost");
	dukglue_register_method(mContext, &cRandomMap::mapGenAstar, "MapGenAstar");
	dukglue_register_method(mContext, &cRandomMap::setMapGenWalkabilityGrid, "SetMapGenWalkabilityGrid");
	dukglue_register_method(mContext, &cRandomMap::mapGenShortestPath, "MapGenShortestPath");
	dukglue_register_method(mContext, &cRandomMap::mapGenCanReach, "MapGenCanReach");

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
    dukglue_register_method(mContext, &cScriptingEngine::scriptRuntime, "scriptRuntime");
    dukglue_register_method(mContext, &cScriptingEngine::scriptRuntimeLibrary, "scriptRuntimeLibrary");
	dukglue_register_method(mContext, &cScriptingEngine::getPhase, "getPhase");
	dukglue_register_method(mContext, &cScriptingEngine::getMission, "getMission");

	dukglue_register_method(mContext, &cScriptingEngine::guiPrintString, "guiPrintString");
	dukglue_register_method(mContext, &cScriptingEngine::networkEnabled, "networkEnabled");
	dukglue_register_method(mContext, &cScriptingEngine::networkGameMode, "networkGameMode");
	dukglue_register_method(mContext, &cScriptingEngine::networkMapSeed, "networkMapSeed");
	dukglue_register_method(mContext, &cScriptingEngine::networkPlayerCount, "networkPlayerCount");
	dukglue_register_method(mContext, &cScriptingEngine::networkTeamCount, "networkTeamCount");
	dukglue_register_method(mContext, &cScriptingEngine::networkTeamSize, "networkTeamSize");
	dukglue_register_method(mContext, &cScriptingEngine::networkKillLimit, "networkKillLimit");
	dukglue_register_method(mContext, &cScriptingEngine::networkTimeLimitSeconds, "networkTimeLimitSeconds");
	dukglue_register_method(mContext, &cScriptingEngine::networkFriendlyFire, "networkFriendlyFire");
	dukglue_register_method(mContext, &cScriptingEngine::networkMapSize, "networkMapSize");
	dukglue_register_method(mContext, &cScriptingEngine::networkMapTerrain, "networkMapTerrain");
	dukglue_register_method(mContext, &cScriptingEngine::networkMapTerrainSub, "networkMapTerrainSub");
	dukglue_register_method(mContext, &cScriptingEngine::networkVehicleSet, "networkVehicleSet");
	dukglue_register_method(mContext, &cScriptingEngine::networkPickupDensity, "networkPickupDensity");
	dukglue_register_method(mContext, &cScriptingEngine::networkCoverDensity, "networkCoverDensity");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapOptionsEnabled, "randomMapOptionsEnabled");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapSeed, "randomMapSeed");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapSize, "randomMapSize");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapSizeExplicit, "randomMapSizeExplicit");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapTerrain, "randomMapTerrain");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapTerrainSub, "randomMapTerrainSub");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapVehicleSet, "randomMapVehicleSet");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapPickupDensity, "randomMapPickupDensity");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapCoverDensity, "randomMapCoverDensity");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapProfile, "randomMapProfile");
	dukglue_register_method(mContext, &cScriptingEngine::randomMapProfileName, "randomMapProfileName");
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

bool cScriptingEngine::networkEnabled() const {
	return g_Fodder && g_Fodder->mStartParams && g_Fodder->mStartParams->mNetworkEnabled;
}

int cScriptingEngine::networkGameMode() const {
	if (!networkEnabled())
		return 0;

	return (int)g_Fodder->mStartParams->mNetworkGameMode;
}

uint32_t cScriptingEngine::networkMapSeed() const {
	if (!networkEnabled())
		return 0;

	return g_Fodder->mStartParams->mNetworkMapSeed;
}

int cScriptingEngine::networkPlayerCount() const {
	if (!networkEnabled())
		return 1;

	const int TeamCount = (int)g_Fodder->mStartParams->mNetworkTeamCount;
	const int TeamSize = (int)g_Fodder->mStartParams->mNetworkTeamSize;
	const int PlayerCount = TeamCount * TeamSize;
	return std::max(1, std::min(PlayerCount, NETWORK_MAX_PLAYERS));
}

int cScriptingEngine::networkTeamCount() const {
	if (!networkEnabled())
		return 1;

	return std::max(1, std::min((int)g_Fodder->mStartParams->mNetworkTeamCount, NETWORK_MAX_PLAYERS));
}

int cScriptingEngine::networkTeamSize() const {
	if (!networkEnabled())
		return 1;

	return std::max(1, std::min((int)g_Fodder->mStartParams->mNetworkTeamSize, NETWORK_MAX_PLAYERS));
}

int cScriptingEngine::networkKillLimit() const {
	if (!networkEnabled())
		return 0;

	return (int)g_Fodder->mStartParams->mNetworkKillLimit;
}

int cScriptingEngine::networkTimeLimitSeconds() const {
	if (!networkEnabled())
		return 0;

	return (int)g_Fodder->mStartParams->mNetworkTimeLimitSeconds;
}

bool cScriptingEngine::networkFriendlyFire() const {
	return networkEnabled() && g_Fodder->mStartParams->mNetworkFriendlyFire;
}

int cScriptingEngine::networkMapSize() const {
	if (!networkEnabled())
		return (int)NETWORK_MAP_SIZE_DEFAULT;

	return (int)g_Fodder->mStartParams->mNetworkMapSize;
}

int cScriptingEngine::networkMapTerrain() const {
	if (!networkEnabled())
		return (int)NETWORK_MAP_TERRAIN_DEFAULT;

	return (int)g_Fodder->mStartParams->mNetworkMapTerrain;
}

int cScriptingEngine::networkMapTerrainSub() const {
	if (!networkEnabled())
		return 0;

	return (int)g_Fodder->mStartParams->mNetworkMapTerrainSub;
}

int cScriptingEngine::networkVehicleSet() const {
	if (!networkEnabled())
		return (int)NETWORK_VEHICLE_SET_DEFAULT;

	return (int)g_Fodder->mStartParams->mNetworkVehicleSet;
}

int cScriptingEngine::networkPickupDensity() const {
	if (!networkEnabled())
		return (int)NETWORK_PICKUP_DENSITY_DEFAULT;

	return (int)g_Fodder->mStartParams->mNetworkPickupDensity;
}

int cScriptingEngine::networkCoverDensity() const {
	if (!networkEnabled())
		return (int)NETWORK_COVER_DENSITY_DEFAULT;

	return (int)g_Fodder->mStartParams->mNetworkCoverDensity;
}

bool cScriptingEngine::randomMapOptionsEnabled() const {
	return g_Fodder && g_Fodder->mStartParams && g_Fodder->mStartParams->mRandomMapOptionsEnabled;
}

uint32_t cScriptingEngine::randomMapSeed() const {
	if (!randomMapOptionsEnabled())
		return 0;

	return g_Fodder->mStartParams->mRandomMapSeed;
}

int cScriptingEngine::randomMapSize() const {
	if (!randomMapOptionsEnabled())
		return (int)NETWORK_MAP_SIZE_DEFAULT;

	return (int)g_Fodder->mStartParams->mRandomMapSize;
}

bool cScriptingEngine::randomMapSizeExplicit() const {
	return randomMapOptionsEnabled() && g_Fodder->mStartParams->mRandomMapSizeExplicit;
}

int cScriptingEngine::randomMapTerrain() const {
	if (!randomMapOptionsEnabled())
		return (int)NETWORK_MAP_TERRAIN_DEFAULT;

	return (int)g_Fodder->mStartParams->mRandomMapTerrain;
}

int cScriptingEngine::randomMapTerrainSub() const {
	if (!randomMapOptionsEnabled())
		return 0;

	return (int)g_Fodder->mStartParams->mRandomMapTerrainSub;
}

int cScriptingEngine::randomMapVehicleSet() const {
	if (!randomMapOptionsEnabled())
		return (int)NETWORK_VEHICLE_SET_DEFAULT;

	return (int)g_Fodder->mStartParams->mRandomMapVehicleSet;
}

int cScriptingEngine::randomMapPickupDensity() const {
	if (!randomMapOptionsEnabled())
		return (int)NETWORK_PICKUP_DENSITY_DEFAULT;

	return (int)g_Fodder->mStartParams->mRandomMapPickupDensity;
}

int cScriptingEngine::randomMapCoverDensity() const {
	if (!randomMapOptionsEnabled())
		return (int)NETWORK_COVER_DENSITY_DEFAULT;

	return (int)g_Fodder->mStartParams->mRandomMapCoverDensity;
}

int cScriptingEngine::randomMapProfile() const {
	if (!randomMapOptionsEnabled())
		return (int)NETWORK_MAP_PROFILE_DEFAULT;

	return (int)g_Fodder->mStartParams->mRandomMapProfile;
}

std::string cScriptingEngine::randomMapProfileName() const {
	if (!randomMapOptionsEnabled())
		return "";

	return g_Fodder->mStartParams->mRandomMapProfileName;
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

	g_Fodder->mSurface->Save();
	g_Fodder->Video_Sleep();
	g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
}


void cScriptingEngine::mapSave() {

	if (getPhase()->GetMapFilename().size() == 0) {
		getPhase()->SetMapFilename("random");
	}

	const std::string MapPath = getCampaign()->GetPathToFile(getPhase()->GetMapFilename());
	if (!g_Fodder->mMapLoaded->save(MapPath, true))
		g_Debugger->Error("Failed to save generated map: " + MapPath);
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
	if (finalpath.empty())
		return true;

	auto scripts = g_ResourceMan->DirectoryList(finalpath, "js");

	for (auto scriptFile : scripts) {

		auto finalName = pFolder + scriptFile;
		auto script = g_ResourceMan->FileReadStr(finalpath + scriptFile);
		
		if (!script.size() || (scriptRun(script, finalName) == false)) {
            mScriptsLoaded = false;
			g_Debugger->Error(finalpath + scriptFile + " Failed to execute script:" + scriptFile);
			return false;
		}
	}

	return true;
}

bool cScriptingEngine::scriptRun(const std::string& pJS, const std::string& pFilename) {
    if(mJit) return mJit->run(pJS, pFilename);
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
    if(!mScriptsLoaded) return false;

	dukglue_register_global(mContext, this, "Engine");
    if(mJit) mJit->syncEngine();

	auto path = g_ResourceMan->GetScriptPath(pScript);
	auto script = g_ResourceMan->FileReadStr(path);

	if (!script.size() || (scriptRun(script, pScript) == false)) {
		g_Debugger->Error(path + " Failed to execute: " + pScript);
		return false;
	}

	return true;
}

std::string cScriptingEngine::scriptRuntime() const {
    return mJit ? "chakra-jit" : "duktape";
}
std::string cScriptingEngine::scriptRuntimeLibrary() const {
    return mJit ? mJit->libraryPath() : std::string();
}
