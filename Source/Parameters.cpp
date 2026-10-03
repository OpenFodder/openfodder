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

#include <stdafx.hpp>
#include <cctype>
#include <sstream>
#include "Utils/ini.hpp"
#include "Utils/cxxopts.hpp"
#include "Utils/json.hpp"
#include "Setup/Validation.hpp"

using Json = nlohmann::json;

cxxopts::Options* sFodderParameters::mCliOptions = 0;

static std::string Parameters_ToLower(std::string pValue) {
	std::transform(pValue.begin(), pValue.end(), pValue.begin(), [](unsigned char c) {
		return (char)std::tolower(c);
	});
	return pValue;
}

static eNetworkGameMode Parameters_ParseNetworkMode(const std::string& pValue) {
	const std::string Mode = Parameters_ToLower(pValue);

	if (Mode == "coop" || Mode == "co-op" || Mode == "campaign" || Mode == "co-op-campaign")
		return eNetworkGameMode_CoopCampaign;
	if (Mode == "coop-random" || Mode == "co-op-random" || Mode == "coop_random" || Mode == "random")
		return eNetworkGameMode_CoopRandom;
	if (Mode == "deathmatch" || Mode == "dm")
		return eNetworkGameMode_Deathmatch;
	if (Mode == "squad-deathmatch" || Mode == "squad_dm" || Mode == "squaddeathmatch")
		return eNetworkGameMode_SquadDeathmatch;
	if (Mode == "rescue-prisoner" || Mode == "rescue" || Mode == "ctf")
		return eNetworkGameMode_RescuePrisoner;
	if (Mode == "avatar-deathmatch" || Mode == "avatar_dm" || Mode == "avatardeathmatch")
		return eNetworkGameMode_AvatarDeathmatch;
	if (Mode == "team-avatar" || Mode == "teamavatar")
		return eNetworkGameMode_TeamAvatar;

	return eNetworkGameMode_CoopCampaign;
}

static eNetworkMapSize Parameters_ParseNetworkMapSize(const std::string& pValue) {
	const std::string Size = Parameters_ToLower(pValue);

	if (Size == "small" || Size == "s")
		return eNetworkMapSize_Small;
	if (Size == "large" || Size == "l")
		return eNetworkMapSize_Large;
	if (Size == "extra-large" || Size == "extra_large" || Size == "xlarge" || Size == "xl")
		return eNetworkMapSize_ExtraLarge;
	return eNetworkMapSize_Medium;
}

static eNetworkMapTerrain Parameters_ParseNetworkMapTerrain(const std::string& pValue) {
	const std::string Terrain = Parameters_ToLower(pValue);

	if (Terrain == "random" || Terrain == "rand")
		return eNetworkMapTerrain_Random;
	if (Terrain == "beach")
		return eNetworkMapTerrain_Jungle;
	if (Terrain == "desert")
		return eNetworkMapTerrain_Desert;
	if (Terrain == "ice" || Terrain == "snow")
		return eNetworkMapTerrain_Ice;
	if (Terrain == "moors" || Terrain == "moor")
		return eNetworkMapTerrain_Moors;

	return eNetworkMapTerrain_Jungle;
}

static uint32 Parameters_ParseNetworkMapTerrainSub(const std::string& pValue) {
	const std::string Terrain = Parameters_ToLower(pValue);

	if (Terrain == "beach")
		return 1;

	return 0;
}

static eNetworkVehicleSet Parameters_ParseNetworkVehicleSet(const std::string& pValue) {
	const std::string Set = Parameters_ToLower(pValue);

	if (Set == "light" || Set == "jeep" || Set == "jeeps")
		return eNetworkVehicleSet_Light;
	if (Set == "armed" || Set == "gun")
		return eNetworkVehicleSet_Armed;
	if (Set == "tanks" || Set == "tank" || Set == "heavy")
		return eNetworkVehicleSet_Tanks;
	if (Set == "mixed" || Set == "all")
		return eNetworkVehicleSet_Mixed;

	return eNetworkVehicleSet_None;
}

static eNetworkPickupDensity Parameters_ParseNetworkPickupDensity(const std::string& pValue) {
	const std::string Density = Parameters_ToLower(pValue);

	if (Density == "low")
		return eNetworkPickupDensity_Low;
	if (Density == "high")
		return eNetworkPickupDensity_High;

	return eNetworkPickupDensity_Normal;
}

static eNetworkCoverDensity Parameters_ParseNetworkCoverDensity(const std::string& pValue) {
	const std::string Density = Parameters_ToLower(pValue);

	if (Density == "sparse" || Density == "low")
		return eNetworkCoverDensity_Sparse;
	if (Density == "dense" || Density == "high")
		return eNetworkCoverDensity_Dense;
	if (Density == "heavy" || Density == "max")
		return eNetworkCoverDensity_Heavy;

	return eNetworkCoverDensity_Normal;
}

static eNetworkMenuStart Parameters_ParseNetworkMenuStart(const std::string& pValue) {
	const std::string Menu = Parameters_ToLower(pValue);

	if (Menu == "main" || Menu == "menu")
		return eNetworkMenuStart_Main;
	if (Menu == "host")
		return eNetworkMenuStart_Host;
	if (Menu == "host-map-options" || Menu == "map-options" || Menu == "map")
		return eNetworkMenuStart_HostMapOptions;
	if (Menu == "join" || Menu == "direct-connect" || Menu == "direct")
		return eNetworkMenuStart_Join;
	if (Menu == "find-lan" || Menu == "lan" || Menu == "browser")
		return eNetworkMenuStart_FindLan;
	if (Menu == "find-internet" || Menu == "internet" || Menu == "hub")
		return eNetworkMenuStart_FindInternet;

	return eNetworkMenuStart_Main;
}

std::string sFodderParameters::ToJson() {
	Json Save;

	Save["mSkipIntro"] = mSkipIntro;
	Save["mSkipToMission"] = mSkipRecruit;
	Save["mSkipBriefing"] = mSkipBriefing;
	Save["mSkipService"] = mSkipService;

	Save["mWindowMode"] = mWindowMode;
	Save["mIntegerScaling"] = mIntegerScaling;
	Save["mBilinearFilter"] = mBilinearFilter;

	Save["mRandom"] = mRandom;
	Save["mRandomMapOptionsEnabled"] = mRandomMapOptionsEnabled;
	Save["mRandomMapSeed"] = mRandomMapSeed;
	Save["mRandomMapSize"] = mRandomMapSize;
	Save["mRandomMapSizeExplicit"] = mRandomMapSizeExplicit;
	Save["mRandomMapTerrain"] = mRandomMapTerrain;
	Save["mRandomMapTerrainSub"] = mRandomMapTerrainSub;
	Save["mRandomMapVehicleSet"] = mRandomMapVehicleSet;
	Save["mRandomMapPickupDensity"] = mRandomMapPickupDensity;
	Save["mRandomMapCoverDensity"] = mRandomMapCoverDensity;
	Save["mRandomMapProfile"] = mRandomMapProfile;
	Save["mRandomMapProfileName"] = mRandomMapProfileName;
	Save["mDefaultPlatform"] = mDefaultPlatform;
	Save["mCampaignName"] = mCampaignName;
	Save["mMissionNumber"] = mMissionNumber;
	Save["mPhaseNumber"] = mPhaseNumber;

	Save["mUnitTesting"] = mUnitTesting;
	Save["mSinglePhase"] = mSinglePhase;
	Save["mSpritesMax"] = mSpritesMax;
	Save["mSpawnEnemyMax"] = mSpawnEnemyMax;

	Save["mCheatsEnabled"] = mCheatsEnabled;
	return Save.dump(1);
}

bool sFodderParameters::FromJson(const std::string& pJson) {
	Json LoadedData;

	try {
		LoadedData = Json::parse(pJson);
	}
	catch (std::exception Exception) {
		std::cout << "SaveGame JSON Parsing Error: " << Exception.what() << "\n";
		return false;
	}

	mSkipService = LoadedData["mSkipService"];
	mSkipBriefing = LoadedData["mSkipBriefing"];
	mSkipIntro = LoadedData["mSkipIntro"];
	mSkipRecruit = LoadedData["mSkipToMission"];
	mMissionNumber = LoadedData["mMissionNumber"];
	mPhaseNumber = LoadedData["mPhaseNumber"];
	mWindowMode = LoadedData["mWindowMode"];
	mBilinearFilter = LoadedData["mBilinearFilter"];
	mRandom = LoadedData["mRandom"];
	if (LoadedData.count("mRandomMapOptionsEnabled") > 0)
		mRandomMapOptionsEnabled = LoadedData["mRandomMapOptionsEnabled"];
	if (LoadedData.count("mRandomMapSeed") > 0)
		mRandomMapSeed = LoadedData["mRandomMapSeed"];
	if (LoadedData.count("mRandomMapSize") > 0)
		mRandomMapSize = Network_NormalizeMapSize((uint8_t)LoadedData["mRandomMapSize"]);
	if (LoadedData.count("mRandomMapSizeExplicit") > 0)
		mRandomMapSizeExplicit = LoadedData["mRandomMapSizeExplicit"];
	if (LoadedData.count("mRandomMapTerrain") > 0)
		mRandomMapTerrain = Network_NormalizeMapTerrain((uint8_t)LoadedData["mRandomMapTerrain"]);
	if (LoadedData.count("mRandomMapTerrainSub") > 0)
		mRandomMapTerrainSub = LoadedData["mRandomMapTerrainSub"];
	if (LoadedData.count("mRandomMapVehicleSet") > 0)
		mRandomMapVehicleSet = Network_NormalizeVehicleSet((uint8_t)LoadedData["mRandomMapVehicleSet"]);
	if (LoadedData.count("mRandomMapPickupDensity") > 0)
		mRandomMapPickupDensity = Network_NormalizePickupDensity((uint8_t)LoadedData["mRandomMapPickupDensity"]);
	if (LoadedData.count("mRandomMapCoverDensity") > 0)
		mRandomMapCoverDensity = Network_NormalizeCoverDensity((uint8_t)LoadedData["mRandomMapCoverDensity"]);
	if (LoadedData.count("mRandomMapProfile") > 0)
		mRandomMapProfile = Network_NormalizeMapProfile((uint8_t)LoadedData["mRandomMapProfile"]);
	if (LoadedData.count("mRandomMapProfileName") > 0)
		mRandomMapProfileName = LoadedData["mRandomMapProfileName"];
	mDefaultPlatform = LoadedData["mDefaultPlatform"];
	mCampaignName = LoadedData["mCampaignName"];
	mUnitTesting = LoadedData["mUnitTesting"];
	mSinglePhase = LoadedData["mSinglePhase"];

	// Max Sprites
	{
		if (LoadedData.count("mSpritesMax") > 0)
			mSpritesMax = LoadedData["mSpritesMax"];
		else
			mSpritesMax = 45; // The original engine limit

		if (mSpritesMax < MIN_SPRITES_MAX)
			mSpritesMax = MIN_SPRITES_MAX;
		if (mSpritesMax > MAX_SPRITES_MAX)
			mSpritesMax = MAX_SPRITES_MAX;
	}

	// Max Spawned
	{
		if (LoadedData.count("mSpawnEnemyMax") > 0)
			mSpawnEnemyMax = LoadedData["mSpawnEnemyMax"];
		else
			mSpawnEnemyMax = 0x0A; // The original engine limit
	}

	if (LoadedData.count("mCheatsEnabled") > 0)
		mCheatsEnabled = LoadedData["mCheatsEnabled"];
	else
		mCheatsEnabled = false;

	if (LoadedData.count("mIntegerScaling") > 0) {
		mIntegerScaling = LoadedData["mIntegerScaling"];
	}

	return true;
}

void sFodderParameters::PrepareOptions() {

	if (!mCliOptions)
		mCliOptions = new cxxopts::Options("OpenFodder", "War has never been so much fun");
	
	mCliOptions->allow_unrecognised_options();
	mCliOptions->add_options()
		("about", "About", cxxopts::value<bool>()->default_value("false"))
		("h,help", "Help", cxxopts::value<bool>()->default_value("false"))

		("pc", "Default to PC platform data", cxxopts::value<bool>()->default_value("false"))
		("amiga", "Default to Amiga platform data", cxxopts::value<bool>()->default_value("false"))
		("engine", "Default to engine (single map/random)", cxxopts::value<std::string>()->default_value("cf1"), "cf1")

		("columns", "Number of camera columns", cxxopts::value<std::uint32_t>()->default_value("22"), "22")
		("rows", "Number of camera rows", cxxopts::value<std::uint32_t>()->default_value("16"), "16")

		("alternate-mouse", "Play with non-original mouse behaviour", cxxopts::value<bool>()->default_value("false"))
		("w,window", "Start in window mode", cxxopts::value<bool>()->default_value("false"))
		("window-scale", "Set the window scale", cxxopts::value<std::uint32_t>()->default_value("0"))
		("integer-scaling", "Use integer scaling", cxxopts::value<bool>()->default_value("true"))
		("bilinear", "Apply bilinear filtering", cxxopts::value<bool>()->default_value("false"))

		("cheats", "Enable cheat keys", cxxopts::value<bool>()->default_value("false"))
		("max-sprite", "Set the maximum sprites", cxxopts::value<std::uint32_t>()->default_value("45"), "45")
		("max-spawn", "Set the maximum spawn", cxxopts::value<std::uint32_t>()->default_value("10"), "10")
		("sleep-delta", "Set the engine speed (20 = 50Hz)", cxxopts::value<std::uint32_t>()->default_value("20"), "20")

		("demo-record", "Record Demo", cxxopts::value<std::string>()->default_value(""), "\"Demo File\"")
		("demo-record-all", "Record Demo")
		("demo-play", "Play Demo", cxxopts::value<std::string>()->default_value(""), "\"Demo File\"")

		("net-player", "Cooperative multiplayer: local player index (1 or 2)", cxxopts::value<uint32_t>()->default_value("0"), "1")
		("net-host",   "Cooperative multiplayer: remote peer hostname / IP",   cxxopts::value<std::string>()->default_value(""), "\"192.168.1.x\"")
		("net-port",   "Cooperative multiplayer: remote peer UDP port",         cxxopts::value<uint32_t>()->default_value("7001"), "7001")
		("net-local-port", "Cooperative multiplayer: local UDP port",           cxxopts::value<uint32_t>()->default_value("7000"), "7000")
		("net-synctest","Run GGPO sync-test (local determinism check)",         cxxopts::value<bool>()->default_value("false"))
		("net-menu", "Open multiplayer setup on startup: main, host, host-map-options, join, find-lan, find-internet", cxxopts::value<std::string>()->default_value(""), "\"host-map-options\"")
		("net-internet", "Use relay hub for Internet multiplayer", cxxopts::value<bool>()->default_value("false"))
		("net-hub-host", "Relay hub control hostname / IP", cxxopts::value<std::string>()->default_value("hub.openfodder.com"), "\"hub.openfodder.com\"")
		("net-hub-port", "Relay hub control UDP port", cxxopts::value<uint32_t>()->default_value("27770"), "27770")
		("net-room-code", "Relay room/session code to join", cxxopts::value<std::string>()->default_value(""), "\"ABC123\"")
		("net-mode", "Multiplayer mode: coop, coop-random, deathmatch, squad-deathmatch, rescue-prisoner, avatar-deathmatch, team-avatar", cxxopts::value<std::string>()->default_value(""), "\"coop\"")
		("net-seed", "Multiplayer map seed", cxxopts::value<uint32_t>()->default_value(std::to_string(NETWORK_MAP_SEED_DEFAULT)), "4919")
		("net-kill-limit", "Multiplayer kill limit", cxxopts::value<uint32_t>()->default_value(std::to_string(NETWORK_KILL_LIMIT_DEFAULT)), "10")
		("net-time-limit", "Multiplayer time limit in seconds (0 = none)", cxxopts::value<uint32_t>()->default_value(std::to_string(NETWORK_TIME_LIMIT_DEFAULT)), "0")
		("net-team-size", "Players per team for team modes", cxxopts::value<uint32_t>()->default_value(std::to_string(NETWORK_TEAM_SIZE_DEFAULT)), "1")
		("net-friendly-fire", "Allow friendly fire in multiplayer team modes", cxxopts::value<bool>()->default_value("false"))
		("net-map-size", "Multiplayer random map size: small, medium, large, extra-large", cxxopts::value<std::string>()->default_value(""), "\"medium\"")
		("net-terrain", "Multiplayer random map terrain: random, jungle, desert, ice, moors", cxxopts::value<std::string>()->default_value(""), "\"jungle\"")
		("net-vehicles", "Multiplayer random map vehicles: none, light, armed, tanks, mixed", cxxopts::value<std::string>()->default_value(""), "\"none\"")
		("net-pickups", "Multiplayer random map pickups: low, normal, high", cxxopts::value<std::string>()->default_value(""), "\"normal\"")
		("net-cover", "Multiplayer random map cover density: sparse, normal, dense, heavy", cxxopts::value<std::string>()->default_value(""), "\"normal\"")

		("unit-test", "Run Tests", cxxopts::value<bool>()->default_value("false"))
		("unit-test-headless", "Run Tests, with no output", cxxopts::value<bool>()->default_value("false"))

		("appveyor", "Output for appveyor", cxxopts::value<bool>()->default_value("false"))
		("headless", "Run without video or audio output", cxxopts::value<bool>()->default_value("false"))
		("nosound", "Disable sound output", cxxopts::value<bool>()->default_value("false"))
		("playground", "Sprite playground", cxxopts::value<bool>()->default_value("false"))

		("setup", "Force the in-game first-run setup wizard, even if retail data is detected", cxxopts::value<bool>()->default_value("false"))
		("probe-image", "Mount a disk image (.adf, .iso, ...) via firy, list its contents, and exit. Diagnostic use only.", cxxopts::value<std::string>()->default_value(""), "\"path/to/image.iso\"")

		("skipintro", "Skip all game intros", cxxopts::value<bool>()->default_value("false"))
		("disable-intro-video", "Disable intro video playback", cxxopts::value<bool>()->default_value("false"))
		("skipbriefing", "Skip mission briefing", cxxopts::value<bool>()->default_value("false"))
		("skipservice", "Skip mission debriefing", cxxopts::value<bool>()->default_value("false"))
		("skiphill", "Skip the hill", cxxopts::value<bool>()->default_value("false"))

		("list-campaigns", "List available campaigns", cxxopts::value<bool>()->default_value("false"))
		("c,campaign", "Starting campaign", cxxopts::value<std::string>()->default_value(""), "\"name\"")
		("m,mission", "Starting mission", cxxopts::value<std::uint32_t>()->default_value("0"), "1")
		("p,phase", "Starting phase", cxxopts::value<std::uint32_t>()->default_value("0"), "2")

		("single-map", "Play a single map", cxxopts::value<std::string>()->default_value(""), "\"MyMap\"")
		("map-generate-png", "Render a .map file to PNG and exit", cxxopts::value<std::string>()->default_value(""), "\"Run/random.map\"")
		("map-png-output", "Output path for --map-generate-png", cxxopts::value<std::string>()->default_value(""), "\"MapDumps/random.png\"")
		("map-png-add-coords", "Overlay tile coordinates on --map-generate-png output", cxxopts::value<bool>()->default_value("false"))
		("map-png-add-tileids", "Overlay tile IDs on --map-generate-png output", cxxopts::value<bool>()->default_value("false"))
		("map-png-scale", "Nearest-neighbor output scale for --map-generate-png, 1-4", cxxopts::value<std::uint32_t>()->default_value("1"), "2")
		("map-route-oracle", "Run engine walkable/terrain/route oracle queries against a .map and exit", cxxopts::value<std::string>()->default_value(""), "\"queries.json\"")
		("map-route-oracle-output", "Output path for --map-route-oracle results JSON (defaults to <input>.result.json)", cxxopts::value<std::string>()->default_value(""), "\"queries.result.json\"")
		("r,random", "Generate and play a random map", cxxopts::value<bool>()->default_value("false"))
		("random-menu", "Open the create-random-map options screen on startup", cxxopts::value<bool>()->default_value("false"))
		("random-save", "Generate and save a random map", cxxopts::value<std::string>()->default_value(""), "\"MyMap\"")
		("random-seed", "Random map seed", cxxopts::value<uint32_t>()->default_value("0"), "123")
		("random-tileset", "Random map tileset: random, jungle, beach, desert, ice, moors", cxxopts::value<std::string>()->default_value(""), "\"ice\"")
		("random-subtileset", "Random map terrain sub-tileset override (jungle: 0 inland, 1 beach)", cxxopts::value<uint32_t>()->default_value("0"), "1")
		("random-map-size", "Random map size: small, medium, large, extra-large", cxxopts::value<std::string>()->default_value(""), "\"extra-large\"")
		("random-profile", "Random MapGen profile name", cxxopts::value<std::string>()->default_value(""), "\"grammar_ice\"")
		("script", "Name of script to execute", cxxopts::value<std::string>()->default_value(""), "\"script.js\"")
#ifdef OF_JS_DEBUG
		("debugger", "Wait for debugger in scripts", cxxopts::value<bool>()->default_value("false"))
#endif
		;
}

bool sFodderParameters::ProcessCLI(int argc, char *argv[]) {

	if (!mCliOptions)
		PrepareOptions();

	try {
		auto result = mCliOptions->parse(argc, argv);

		if (result["appveyor"].as<bool>()) {
			mAppVeyor = true;
		}

		if (result["help"].as<bool>() == true) {
			g_Debugger->Notice(mCliOptions->help());
			mShowHelp = true;
			return false;
		}

		if (result.count("pc"))
			mDefaultPlatform = ePlatform::PC;
		if (result.count("amiga"))
			mDefaultPlatform = ePlatform::Amiga;

		if (result.count("engine")) {
			if (result["engine"].as<std::string>() == "cf1")
				mDefaultGame = eGame::CF1;

			if (result["engine"].as<std::string>() == "cf2")
				mDefaultGame = eGame::CF2;

		}

		if (result["list-campaigns"].as<bool>() == true) {
			g_Debugger->Notice("\nAvailable Campaigns\n\n");

			g_ResourceMan->refresh();
			for (auto& Name : g_Fodder->mVersions->GetCampaignNames())
				g_Debugger->Notice(Name);

			return false;
		}

		// --probe-image: mount a disk image via firy, list its contents, exit.
		// Diagnostic / smoke-test only — not part of the normal launch path.
		if (result.count("probe-image")) {
			std::string isoPath = result["probe-image"].as<std::string>();
			if (!isoPath.empty()) {
				Setup::ProbeImage(isoPath);
				return false;
			}
		}

		if (result.count("demo-record-all")) {
			mDemoRecord = true;
			mDemoFile = "-";
		}
		else {
			mDemoFile = result["demo-record"].as<std::string>();
			if (mDemoFile.size())
				mDemoRecord = true;
			else {
				mDemoFile = result["demo-play"].as<std::string>();
				if (mDemoFile.size())
					mDemoPlayback = true;
			}
		}

		mShowAbout = result["about"].as<bool>();

		if (result.count("skipintro"))
			mSkipIntro = result["skipintro"].as<bool>();
		if (result.count("disable-intro-video"))
			mDisableIntroVideo = result["disable-intro-video"].as<bool>();
		if (result.count("skipservice"))
			mSkipService = result["skipservice"].as<bool>();
		if (result.count("skipbriefing"))
			mSkipBriefing = result["skipbriefing"].as<bool>();
		if (result.count("skiphill"))
			mSkipRecruit = result["skiphill"].as<bool>();

		if (result.count("setup"))
			mForceSetupWizard = result["setup"].as<bool>();

		mUnitTesting = result["unit-test"].as<bool>();

		mSingleMap = result["single-map"].as<std::string>();

		if (mSingleMap.size()) {
			
			mSingleMap.append(".map");
			mSinglePhase = true;
		}

		mMapGeneratePng = result["map-generate-png"].as<std::string>();
		mMapPngOutput = result["map-png-output"].as<std::string>();
		mMapPngAddCoords = result["map-png-add-coords"].as<bool>();
		mMapPngAddTileIds = result["map-png-add-tileids"].as<bool>();
		mMapPngScale = std::max<uint32>(1, std::min<uint32>(4, result["map-png-scale"].as<std::uint32_t>()));
		mMapRouteOracle = result["map-route-oracle"].as<std::string>();
		mMapRouteOracleOutput = result["map-route-oracle-output"].as<std::string>();

		mCampaignName = result["campaign"].as<std::string>();
		mMissionNumber = result["mission"].as<std::uint32_t>();
		mPhaseNumber = result["phase"].as<std::uint32_t>();

		if(result.count("alternate-mouse"))
			mMouseAlternative = result["alternate-mouse"].as<bool>();

		if (result.count("window"))
			mWindowMode = result["window"].as<bool>();

		if(result.count("window-scale"))
			mWindowScale = result["window-scale"].as<uint32>();

		if (result.count("integer-scaling"))
			mIntegerScaling = result["integer-scaling"].as<bool>();


		if (result.count("bilinear"))
			mBilinearFilter = result["bilinear"].as<bool>();

		mRandom = result["random"].as<bool>();
		if (result["random-save"].count()) {

			mRandomFilename = result["random-save"].as<std::string>();
			mRandomSave = true;
			mRandom = true;
		}

		if (result["random-menu"].as<bool>()) {
			mRandomMenuOnStart = true;
			mRandom = true;
			mSkipIntro = true;
		}

		if (result.count("random-seed")) {
			mRandomMapSeed = result["random-seed"].as<uint32_t>();
			mRandomMapOptionsEnabled = true;
			mRandomMapProfile = eNetworkMapProfile_Custom;
		}
		if (result.count("random-tileset")) {
			std::string Tileset = result["random-tileset"].as<std::string>();
			if (!Tileset.empty()) {
				mRandomMapTerrain = Parameters_ParseNetworkMapTerrain(Tileset);
				mRandomMapTerrainSub = Parameters_ParseNetworkMapTerrainSub(Tileset);
				mRandomMapOptionsEnabled = true;
				mRandomMapProfile = eNetworkMapProfile_Custom;
			}
		}
		if (result.count("random-subtileset") && result["random-subtileset"].as<uint32_t>() != 0) {
			mRandomMapTerrainSub = result["random-subtileset"].as<uint32_t>();
			mRandomMapOptionsEnabled = true;
			mRandomMapProfile = eNetworkMapProfile_Custom;
		}
		if (result.count("random-map-size")) {
			std::string MapSize = result["random-map-size"].as<std::string>();
			if (!MapSize.empty()) {
				mRandomMapSize = Parameters_ParseNetworkMapSize(MapSize);
				mRandomMapSizeExplicit = true;
				mRandomMapOptionsEnabled = true;
				mRandomMapProfile = eNetworkMapProfile_Custom;
			}
		}
		if (result.count("random-profile")) {
			mRandomMapProfileName = result["random-profile"].as<std::string>();
			if (!mRandomMapProfileName.empty()) {
				mRandomMapOptionsEnabled = true;
				mRandomMapProfile = eNetworkMapProfile_Custom;
			}
		}
		if (mRandomMapTerrain != eNetworkMapTerrain_Jungle)
			mRandomMapTerrainSub = 0;

		mDisableSound = result["nosound"].as<bool>();
		mPlayground = result["playground"].as<bool>();
		mSleepDelta = result["sleep-delta"].as<uint32_t>();

		// Cooperative network multiplayer (GGPO)
		if (result.count("net-player") && result["net-player"].as<uint32_t>() > 0) {
			mNetworkPlayerIndex = (int)(result["net-player"].as<uint32_t>()) - 1; // 1-based CLI -> 0-based internal
			mNetworkEnabled = true;
		}
		if (result.count("net-host")) {
			mNetworkRemoteHost = result["net-host"].as<std::string>();
			if (!mNetworkRemoteHost.empty())
				mNetworkEnabled = true;
		}
		if (result.count("net-port"))
			mNetworkRemotePort = (uint16_t)result["net-port"].as<uint32_t>();
		if (result.count("net-local-port"))
			mNetworkLocalPort = (uint16_t)result["net-local-port"].as<uint32_t>();
		if (result.count("net-synctest") && result["net-synctest"].as<bool>()) {
			mNetworkSyncTest = true;
			mNetworkEnabled  = true;
		}
				if (result.count("net-menu")) {
			std::string NetMenu = result["net-menu"].as<std::string>();
			if (!NetMenu.empty()) {
				mNetworkMenuStart = Parameters_ParseNetworkMenuStart(NetMenu);
				mNetworkEnabled = true;
			}
		}
		if (result.count("net-internet") && result["net-internet"].as<bool>()) {
			mNetworkInternet = true;
			mNetworkEnabled = true;
		}
		if (result.count("net-hub-host")) {
			mNetworkHubHost = result["net-hub-host"].as<std::string>();
			if (!mNetworkHubHost.empty() && mNetworkHubHost != "hub.openfodder.com" && mNetworkHubHost != "127.0.0.1")
				mNetworkInternet = true;
		}
		if (result.count("net-hub-port")) {
			mNetworkHubPort = (uint16)std::min<uint32_t>(result["net-hub-port"].as<uint32_t>(), 65535);
			if (mNetworkHubPort != 27770)
				mNetworkInternet = true;
		}
		if (result.count("net-room-code")) {
			mNetworkRoomCode = result["net-room-code"].as<std::string>();
			if (!mNetworkRoomCode.empty()) {
				mNetworkInternet = true;
				mNetworkEnabled = true;
			}
		}
		if (result.count("net-mode")) {
			std::string NetMode = result["net-mode"].as<std::string>();
			if (!NetMode.empty()) {
				mNetworkGameMode = Parameters_ParseNetworkMode(NetMode);
				mNetworkEnabled = true;
			}
		}
		if (result.count("net-seed"))
			mNetworkMapSeed = result["net-seed"].as<uint32_t>();
		if (result.count("net-kill-limit"))
			mNetworkKillLimit = (uint16)std::min<uint32_t>(result["net-kill-limit"].as<uint32_t>(), 65535);
		if (result.count("net-time-limit"))
			mNetworkTimeLimitSeconds = (uint16)std::min<uint32_t>(result["net-time-limit"].as<uint32_t>(), 65535);
		if (result.count("net-team-size"))
			mNetworkTeamSize = (uint8)std::max<uint32_t>(1, std::min<uint32_t>(result["net-team-size"].as<uint32_t>(), NETWORK_MAX_PLAYERS));
		if (result.count("net-friendly-fire"))
			mNetworkFriendlyFire = result["net-friendly-fire"].as<bool>();
		if (result.count("net-map-size")) {
			std::string MapSize = result["net-map-size"].as<std::string>();
			if (!MapSize.empty())
				mNetworkMapSize = Parameters_ParseNetworkMapSize(MapSize);
		}
		if (result.count("net-terrain")) {
			std::string Terrain = result["net-terrain"].as<std::string>();
			if (!Terrain.empty()) {
				mNetworkMapTerrain = Parameters_ParseNetworkMapTerrain(Terrain);
				mNetworkMapTerrainSub = Parameters_ParseNetworkMapTerrainSub(Terrain);
			}
		}
		if (mNetworkMapTerrain != eNetworkMapTerrain_Jungle)
			mNetworkMapTerrainSub = 0;
		if (result.count("net-vehicles")) {
			std::string Vehicles = result["net-vehicles"].as<std::string>();
			if (!Vehicles.empty())
				mNetworkVehicleSet = Parameters_ParseNetworkVehicleSet(Vehicles);
		}
		if (result.count("net-pickups")) {
			std::string Pickups = result["net-pickups"].as<std::string>();
			if (!Pickups.empty())
				mNetworkPickupDensity = Parameters_ParseNetworkPickupDensity(Pickups);
		}
		if (result.count("net-cover")) {
			std::string Cover = result["net-cover"].as<std::string>();
			if (!Cover.empty())
				mNetworkCoverDensity = Parameters_ParseNetworkCoverDensity(Cover);
		}

		// In network mode skip the intro (not relevant for co-op).
		// Between-phase screens (recruit, briefing, service) are handled
		// by Network_Recruit_Show / Network_Briefing_Show and network-aware
		// service loops � see Fodder_Network.cpp.
		if (mNetworkEnabled) {
			mSkipIntro    = true;
			if (mMissionNumber < 1)
				mMissionNumber = 1;
			// Direct network launches keep the old immediate-match behaviour.
			// Setup-menu launches leave the campaign empty so the lobby flow can run.
			if (mCampaignName.empty() && mNetworkMenuStart == eNetworkMenuStart_None)
				mCampaignName = "Cannon Fodder";
		}

		if (result.count("cheats"))
			mCheatsEnabled = result["cheats"].as<bool>();

		if(result.count("rows"))
			mWindowRows = result["rows"].as<std::uint32_t>();
		
		if (result.count("columns"))
			mWindowColumns = result["columns"].as<std::uint32_t>();

		if (result.count("script"))
			mScriptRun = result["script"].as<std::string>();

		// Random / Script mode increase default max sprites/spawn
		if (mScriptRun.size() || mRandom) {

			mSpritesMax = CUSTOM_DEFAULT_MAX_SPRITES;
			mSpawnEnemyMax = CUSTOM_DEFAULT_MAX_SPAWN;
		}

#ifdef OF_JS_DEBUG
		mDebugger = result["debugger"].as<bool>();
#endif

		if (result.count("max-sprite"))
			mSpritesMax = result["max-sprite"].as<uint32_t>();
		if (result.count("max-spawn"))
			mSpawnEnemyMax = result["max-spawn"].as<uint32_t>();

		if (mSpritesMax < MIN_SPRITES_MAX)
			mSpritesMax = MIN_SPRITES_MAX;
		if (mSpritesMax > MAX_SPRITES_MAX)
			mSpritesMax = MAX_SPRITES_MAX;

		// Cheats perm enabled in debug build
#ifdef _DEBUG
		mCheatsEnabled = true;
#endif

		// Skip intro and recruit screen if a campaign and mission/phase is provided
		if (mCampaignName.length() && (mMissionNumber || mPhaseNumber)) {
			mSkipRecruit = true;
			mSkipIntro = true;
		}

		if (result["unit-test-headless"].as<bool>()) {
			mUnitTesting = true;
			mDisableVideo = true;
			mDisableSound = true;

			g_Window = std::make_shared<cWindowNull>();
			g_Fodder->mWindow = g_Window;
		}

		if (result["headless"].as<bool>()) {
			mDisableVideo = true;
			mDisableSound = true;

			g_Window = std::make_shared<cWindowNull>();
			g_Fodder->mWindow = g_Window;
		}

		if (mMapGeneratePng.size()) {
			mSkipIntro = true;
			mDisableVideo = true;
			mDisableSound = true;

			g_Window = std::make_shared<cWindowNull>();
			g_Fodder->mWindow = g_Window;
		}

		if (mUnitTesting) {
			mWindowMode = true;
		}



#ifdef _DEBUG
		mWindowMode = true;
#endif
	}
	catch (...) {
		g_Debugger->Notice(mCliOptions->help());
		return false;
	}

	return true;
}

std::string str_to_lower(std::string pStr) {
	std::transform(pStr.begin(), pStr.end(), pStr.begin(), ::tolower);
	return pStr;
}

bool sFodderParameters::SaveIni() {
	INI<> ini("openfodder.ini", false);

	ini.parse();

	if (ini.select("openfodder")) {
		ini.set("window", mWindowMode ? "true" : "false");
		ini.set("cheats", mCheatsEnabled ? "true" : "false");

		ini.set("scale", (mWindowScale == 0) ? "auto" : std::to_string(mWindowScale));
		ini.set("integer", mIntegerScaling ? "true" : "false");
		ini.set("bilinear", mBilinearFilter ? "true" : "false");

		ini.set("columns", (mWindowColumns == 0) ? "0" : std::to_string(mWindowColumns));
		ini.set("rows", (mWindowRows == 0) ? "0" : std::to_string(mWindowRows));

		ini.set("alternate-mouse", mMouseAlternative ? "true" : "false");
		ini.set("mouse-locked", mMouseLocked ? "true" : "false");
		{
			std::ostringstream speed;
			speed.setf(std::ios::fixed);
			speed.precision(1);
			speed << mMouseSpeed;
			ini.set("mouse-speed", speed.str());
		}
		ini.set("copyprotection", mCopyProtection ? "true" : "false");
	}
	else {
		return false;
	}

	if (ini.select("engine")) {

		ini.set("platform",
			(mDefaultPlatform == ePlatform::Amiga) ? "amiga" :
			(mDefaultPlatform == ePlatform::PC) ? "pc" :
			"");

		ini.set("engine",
			(mDefaultGame == eGame::CF2) ? "cf2" : "cf1");

		ini.set("maxsprite", std::to_string(mSpritesMax));
		ini.set("maxspawn", std::to_string(mSpawnEnemyMax));

		ini.set("sleep-delta", std::to_string(mSleepDelta));
	}

	if (ini.select("skip")) {
		ini.set("intro", mSkipIntro ? "true" : "false");
		ini.set("intro-video", mDisableIntroVideo ? "true" : "false");
		ini.set("briefing", mSkipBriefing ? "true" : "false");
		ini.set("service", mSkipService ? "true" : "false");
		ini.set("hill", mSkipRecruit ? "true" : "false");
	}

	// [paths] — persist user-added search roots and currently-mounted disk
	// images (both set via the in-game setup wizard or hand-edited). Implicit
	// defaults (cwd, Documents/OpenFodder, XDG dirs) are NOT written back
	// because they're recreated by addDefaultDirs() on every launch.
	{
		const auto userPaths = g_ResourceMan ? g_ResourceMan->getUserPaths()
		                                     : std::vector<std::string>{};
		const auto imagePaths = g_ResourceMan ? g_ResourceMan->getMountedImagePaths()
		                                       : std::vector<std::string>{};

		// Ensure the section exists, then clear and rewrite it.
		if (!ini.select("paths"))
			ini.create("paths");
		ini["paths"].clear();

		for (size_t i = 0; i < userPaths.size(); ++i) {
			std::ostringstream key;
			key << "path" << (i + 1);
			ini.set(key.str(), userPaths[i]);
		}

		// imageN= keys distinct from pathN= so the read path can dispatch
		// each kind to the right ResourceMan API.
		for (size_t i = 0; i < imagePaths.size(); ++i) {
			std::ostringstream key;
			key << "image" << (i + 1);
			ini.set(key.str(), imagePaths[i]);
		}
	}

	return ini.save();
}


bool sFodderParameters::ProcessINI() {

	INI<> ini("openfodder.ini", false);

	if (!ini.parse())
		return false;

	// Section: Openfodder
	{
		if (ini.select("openfodder")) {
			if (ini.get("window", "false") == "true")
				mWindowMode = true;
			else
				mWindowMode = false;

			if (ini.get("bilinear", "false") == "true")
				mBilinearFilter = true;
			else
				mBilinearFilter = false;

			if (ini.get("cheats", "false") == "true")
				mCheatsEnabled = true;
			else
				mCheatsEnabled = false;

			if (ini.get("scale", "false") == "auto")
				mWindowScale = 0;
			else {
				mWindowScale = ini.get("scale", 0);
			}

			if (ini.get("integer", "true") == "true")
				mIntegerScaling = true;
			else {
				mIntegerScaling = false;
			}

			if (ini.get("columns", "0") == "0")
				mWindowColumns = 0;
			else {
				mWindowColumns = ini.get("columns", 22);
			}

			if (ini.get("rows", "0") == "0")
				mWindowRows = 0;
			else {
				mWindowRows = ini.get("rows", 16);
			}

			if (ini.get("alternate-mouse", "false") == "true")
				mMouseAlternative = true;

			if (ini.get("mouse-locked", "false") == "true")
				mMouseLocked = true;

			mMouseSpeed = (float)ini.get("mouse-speed", 1.5);
			if (mMouseSpeed < 0.5f)
				mMouseSpeed = 0.5f;
			if (mMouseSpeed > 10.0f)
				mMouseSpeed = 10.0f;

			if (ini.get("copyprotection", "false") == "true")
				mCopyProtection = true;
		}
	}

	// Section: Engine
	{
		if (ini.select("engine")) {
			auto platform = str_to_lower(ini.get("platform", ""));
			if (platform == "amiga")
				mDefaultPlatform = ePlatform::Amiga;
			if (platform == "pc")
				mDefaultPlatform = ePlatform::PC;

			auto game = str_to_lower(ini.get("engine", ""));
			if (game == "cf1")
				mDefaultGame = eGame::CF1;
			if (game == "cf2")
				mDefaultGame = eGame::CF2;


			auto maxsprite = ini.get("maxsprite", 0);
			if (maxsprite)
				mSpritesMax = maxsprite;
			auto maxspawn = ini.get("maxspawn", 0);
			if (maxspawn)
				mSpawnEnemyMax = maxspawn;

			if (mSpritesMax < MIN_SPRITES_MAX)
				mSpritesMax = MIN_SPRITES_MAX;
			if (mSpritesMax > MAX_SPRITES_MAX)
				mSpritesMax = MAX_SPRITES_MAX;
		}
	}

	// Section: Skip
	{
		if (ini.select("skip")) {
			if (ini.get("intro", "false") == "true")
				mSkipIntro = true;

			if (ini.get("intro-video", "false") == "true")
				mDisableIntroVideo = true;

			if (ini.get("briefing", "false") == "true")
				mSkipBriefing = true;

			if (ini.get("service", "false") == "true")
				mSkipService = true;

			if (ini.get("hill", "false") == "true")
				mSkipRecruit = true;
		}
	}

	if (ini.select("paths")) {

		for (auto& entry : ini["paths"]) {
			// Dispatch by key prefix:
			//   imageN=...  →  mountImage (disk image, mounted via firy)
			//   anything else → addUserDir (folder added to search roots)
			//
			// addUserDir round-trips: it tracks the path under mUserPaths so
			// SaveIni writes it back instead of dropping it on the next quit.
			const std::string& key = entry.first;
			const std::string& value = entry.second;

			if (key.size() >= 5 && key.compare(0, 5, "image") == 0) {
				g_ResourceMan->mountImage(value);
			} else {
				g_ResourceMan->addUserDir(value);
			}
		}
	}

	return true;
}
