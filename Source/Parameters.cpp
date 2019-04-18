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

#include <stdafx.hpp>
#include "Utils/ini.hpp"
#include "Utils/cxxopts.hpp"
#include "Utils/json.hpp"

using Json = nlohmann::json;

cxxopts::Options* sFodderParameters::mCliOptions = 0;

/* These values override the original engine values, when in custom mode */
const size_t CUSTOM_DEFAULT_MAX_SPRITES = 100000;
const size_t CUSTOM_DEFAULT_MAX_SPAWN = 25;

std::string sFodderParameters::ToJson() {
	Json Save;

	Save["mSkipIntro"] = mSkipIntro;
	Save["mSkipToMission"] = mSkipRecruit;
	Save["mSkipBriefing"] = mSkipBriefing;
	Save["mSkipService"] = mSkipService;

	Save["mWindowMode"] = mWindowMode;
	Save["mRandom"] = mRandom;
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
	mRandom = LoadedData["mRandom"];
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

		if (mSpritesMax < 16)
			mSpritesMax = 16;
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

		("cheats", "Enable cheat keys", cxxopts::value<bool>()->default_value("false"))
		("max-sprite", "Set the maximum sprites", cxxopts::value<std::uint32_t>()->default_value("45"), "45")
		("max-spawn", "Set the maximum spawn", cxxopts::value<std::uint32_t>()->default_value("10"), "10")
		("sleep-delta", "Set the engine speed", cxxopts::value<std::uint32_t>()->default_value("2"), "2")

		("demo-record", "Record Demo", cxxopts::value<std::string>()->default_value(""), "\"Demo File\"")
		("demo-record-all", "Record Demo")
		("demo-play", "Play Demo", cxxopts::value<std::string>()->default_value(""), "\"Demo File\"")

		("unit-test", "Run Tests", cxxopts::value<bool>()->default_value("false"))
		("unit-test-headless", "Run Tests, with no output", cxxopts::value<bool>()->default_value("false"))

		("appveyor", "Output for appveyor", cxxopts::value<bool>()->default_value("false"))
		("nosound", "Disable sound output", cxxopts::value<bool>()->default_value("false"))
		("playground", "Sprite playground", cxxopts::value<bool>()->default_value("false"))

		("skipintro", "Skip all game intros", cxxopts::value<bool>()->default_value("false"))
		("skipbriefing", "Skip mission briefing", cxxopts::value<bool>()->default_value("false"))
		("skipservice", "Skip mission debriefing", cxxopts::value<bool>()->default_value("false"))
		("skiphill", "Skip the hill", cxxopts::value<bool>()->default_value("false"))

		("list-campaigns", "List available campaigns", cxxopts::value<bool>()->default_value("false"))
		("c,campaign", "Starting campaign", cxxopts::value<std::string>()->default_value(""), "\"name\"")
		("m,mission", "Starting mission", cxxopts::value<std::uint32_t>()->default_value("0"), "1")
		("p,phase", "Starting phase", cxxopts::value<std::uint32_t>()->default_value("0"), "2")

		("single-map", "Play a single map", cxxopts::value<std::string>()->default_value(""), "\"MyMap\"")
		("r,random", "Generate and play a random map", cxxopts::value<bool>()->default_value("false"))
		("random-save", "Generate and save a random map", cxxopts::value<std::string>()->default_value(""), "\"MyMap\"")
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

			for (auto& Name : g_Fodder->mVersions->GetCampaignNames())
				g_Debugger->Notice(Name);

			return false;
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
		if (result.count("skipservice"))
			mSkipService = result["skipservice"].as<bool>();
		if (result.count("skipbriefing"))
			mSkipBriefing = result["skipbriefing"].as<bool>();
		if (result.count("skiphill"))
			mSkipRecruit = result["skiphill"].as<bool>();

		mUnitTesting = result["unit-test"].as<bool>();

		mSingleMap = result["single-map"].as<std::string>();

		if (mSingleMap.size()) {
			
			mSingleMap.append(".map");
			mSinglePhase = true;
		}

		mCampaignName = result["campaign"].as<std::string>();
		mMissionNumber = result["mission"].as<std::uint32_t>();
		mPhaseNumber = result["phase"].as<std::uint32_t>();

		if(result.count("alternate-mouse"))
			mMouseAlternative = result["alternate-mouse"].as<bool>();

		if (result.count("window"))
			mWindowMode = result["window"].as<bool>();

		if(result.count("window-scale"))
			mWindowScale = result["window-scale"].as<uint32>();

		mRandom = result["random"].as<bool>();
		if (result["random-save"].count()) {

			mRandomFilename = result["random-save"].as<std::string>();
			mRandomSave = true;
			mRandom = true;
		}

		mDisableSound = result["nosound"].as<bool>();
		mPlayground = result["playground"].as<bool>();

		mSleepDelta = result["sleep-delta"].as<uint32_t>();

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

		if (mSpritesMax < 16)
			mSpritesMax = 16;

		// Cheats perm enabled in debug build
#ifdef _DEBUG
		mCheatsEnabled = true;
#endif

		if (mMissionNumber || mPhaseNumber) {
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

			if (ini.get("cheats", "false") == "true")
				mCheatsEnabled = true;
			else
				mCheatsEnabled = false;

			if (ini.get("scale", "false") == "auto")
				mWindowScale = 0;
			else {
				mWindowScale = ini.get("scale", 0);
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
		}
	}

	// Section: Skip
	{
		if (ini.select("skip")) {
			if (ini.get("intro", "false") == "true")
				mSkipIntro = true;

			if (ini.get("briefing", "false") == "true")
				mSkipBriefing = true;

			if (ini.get("service", "false") == "true")
				mSkipService = true;

			if (ini.get("hill", "false") == "true")
				mSkipRecruit = true;
		}
	}

	if (ini.select("paths")) {

		for (auto& path : ini["paths"]) {

			g_ResourceMan->addDir(path.second);
		}
	}

	return true;
}
