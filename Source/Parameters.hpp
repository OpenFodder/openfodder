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

namespace cxxopts {
	class Options;
}

#include <array>
#include <vector>

#include "Network/NetworkTypes.hpp"
#ifdef OPENFODDER_ENABLE_NETWORK
#include "Network/HubAuth.hpp"
#endif

// Per-peer remote endpoint, indexed by peer slot. mNetworkRemotePeers[i]
// is peer i's address as advertised by the OFHUB/2 hub (or for LAN, the
// direct partner address). Slot mNetworkPeerIndex is the local player and
// has no entry here. Sized 0..kMaxRoomCapacity-1.
struct sNetworkPeerEndpoint {
    std::string host;
    uint16      port = 0;
};

/* These values override the original engine values, when in custom mode */
static constexpr size_t CUSTOM_DEFAULT_MAX_SPRITES = 1000;
static constexpr size_t CUSTOM_DEFAULT_MAX_SPAWN = 25;

enum eNetworkMenuStart {
	eNetworkMenuStart_None = 0,
	eNetworkMenuStart_Main,
	eNetworkMenuStart_Host,
	eNetworkMenuStart_HostMapOptions,
	eNetworkMenuStart_Join,
	eNetworkMenuStart_FindLan,
	eNetworkMenuStart_FindInternet,
};

class sFodderParameters {
public:
	static cxxopts::Options* mCliOptions;

	bool mAppVeyor;             // Running on AppVeyor
	bool mShowAbout;            // Show the about screen
	bool mSkipIntro;            // Skip the OpenFodder intro, and the game intro
	bool mSkipRecruit;          // Skip the recruit screen and go straight into the mission
	bool mSkipBriefing;         // Skip mission briefing
	bool mSkipService;          // Skip mission debrief

	bool mMouseAlternative;		// Use the alternate mouse behaviour
	bool mMouseLocked;			// Mouse is locked to window
	float mMouseSpeed;          // Mouse speed scale (SDL relative speed scale)

	bool mWindowMode;           // Start in a window
	bool mIntegerScaling;       // Use integer scaling in fullscreen mode
	size_t mWindowScale;		// Start with window scaled at
	size_t mWindowRows;
	size_t mWindowColumns;

	bool mBilinearFilter;		// Use bilinear filtering

	bool mRandom;               // Start a random map
	bool mRandomSave;			// Create a random map
	bool mRandomMenuOnStart;    // Open the create-random-map options menu on startup
	std::string mRandomFilename;// Name to save random map as
	bool mRandomMapOptionsEnabled; // Use explicit random-map options for campaign random maps
	uint32 mRandomMapSeed;
	eNetworkMapSize mRandomMapSize;
	bool mRandomMapSizeExplicit;
	eNetworkMapTerrain mRandomMapTerrain;
	uint32 mRandomMapTerrainSub;
	eNetworkVehicleSet mRandomMapVehicleSet;
	eNetworkPickupDensity mRandomMapPickupDensity;
	eNetworkCoverDensity mRandomMapCoverDensity;
	eNetworkMapProfile mRandomMapProfile;
	std::string mRandomMapProfileName;

	std::string mScriptRun;		// Name of a script to run

	ePlatform mDefaultPlatform; // Default platform to use
	eGame mDefaultGame;			// Default game to use for single/random

	bool mDemoRecord;               // Recording a demo
	bool mDemoPlayback;             // Playing back a demo
	size_t mDemoRecordResumeCycle;  // Record a demo, after playing back the event queue up until this cycle
	std::string mDemoFile;          // The Demo file to save/load to/from

	size_t mSleepDelta;             // Engine sleep delta

	std::string mSingleMap;			// Name of single map to load
	std::string mMapGeneratePng;     // Render a map to a PNG and exit
	std::string mMapPngOutput;       // Output path for map PNG rendering
	bool mMapPngAddCoords;           // Overlay tile coordinates on generated map PNG
	bool mMapPngAddTileIds;          // Overlay tile IDs on generated map PNG
	size_t mMapPngScale;             // Nearest-neighbor output scale for map PNG rendering
	std::string mMapRouteOracle;       // Run engine route/walkable oracle queries against a .map and exit
	std::string mMapRouteOracleOutput; // Output path for --map-route-oracle results JSON
	std::string mCampaignName;      // Campaign to start
	size_t mMissionNumber;          // Mission to start on
	size_t mPhaseNumber;            // Phase to start on

	bool mCheatsEnabled;
	bool mUnitTesting;              // Execute unit testing
	bool mSinglePhase;              // Play a single phase
	bool mPlayground;               // Launch into the sprite playground

	bool mDisableVideo;             // Disable all video output
	bool mDisableIntroVideo;        // Disable intro video playback
	bool mDisableSound;             // Disable all sound

	bool mDebugger;

	size_t mSpritesMax;
	size_t mSpawnEnemyMax;

	static constexpr size_t MIN_SPRITES_MAX = 45;
	static constexpr size_t MAX_SPRITES_MAX = 100000;

	bool mShowHelp;
	bool mCopyProtection;
	bool mForceSetupWizard;     // --setup: force the in-game first-run wizard even if retail data is detected

	// Cooperative network multiplayer (GGPO)
	bool        mNetworkEnabled;        // Enable GGPO network session
	bool        mNetworkSyncTest;       // Run GGPO sync-test instead of real network
	eNetworkMenuStart mNetworkMenuStart; // Optional multiplayer setup screen to show on startup
	bool        mNetworkInternet;       // Use relay hub instead of direct peer/LAN endpoint learning
	std::string mNetworkHubHost;        // Relay hub control hostname / IP
	uint16      mNetworkHubPort;        // Relay hub control UDP port
	std::string mNetworkRoomCode;       // Relay room/session code
	std::string mNetworkHostBearer;     // OFHUB/2 verified-host bearer JWT (host only; empty for joiners). Loaded from cHubAuth token cache at host Start time.
	std::array<unsigned char, 32> mNetworkSessionKey; // OFHUB/2 32-byte HMAC key, scoped to (room, peer); zero-init when not in internet mode.
	uint8       mNetworkPeerIndex;      // OFHUB/2 sender peer index (host=0, joiner=1 in 2-player; range 0..7 per spec 6.1)
	uint8       mNetworkNumPlayers;     // P1 A1: active peer count for this session, 1..kMaxRollbackPlayers. Default 2 keeps legacy 2P behavior. Wired host->hub-claim->peers; consumed by GGPOSession::Start, briefing ReadySync, and the lobby roster UI.
	bool        mNetworkSessionKeyValid;// True once mNetworkSessionKey has been populated from the hub claim
	int         mNetworkPlayerIndex;    // 0 = player 1 is local, 1 = player 2 is local
	std::string mNetworkRemoteHost;     // DEPRECATED — Phase C/P1 migrates consumers to mNetworkRemotePeers. 1-element legacy view of the partner endpoint.
	uint16      mNetworkRemotePort;     // DEPRECATED — Phase C/P1 migrates consumers to mNetworkRemotePeers. 1-element legacy view of the partner endpoint.
	std::vector<sNetworkPeerEndpoint> mNetworkRemotePeers; // OFHUB/2 N-player roster: peer i's host/port for i != mNetworkPeerIndex (filled by hub claim/menu); empty until claim arrives.
	uint16      mNetworkLocalPort;      // Local UDP port to bind
	eNetworkGameMode mNetworkGameMode;  // Selected multiplayer mode
	uint32      mNetworkMapSeed;        // Synced random-map seed for PvP modes
	uint16      mNetworkKillLimit;      // Synced kill limit for PvP modes
	uint16      mNetworkTimeLimitSeconds; // 0 = no time limit
	uint8       mNetworkTeamCount;      // Number of teams in team modes
	uint8       mNetworkTeamSize;       // Players per team
	bool        mNetworkFriendlyFire;   // Allow same-team damage
	eNetworkMapSize mNetworkMapSize;   // Random-map dimensions
	eNetworkMapTerrain mNetworkMapTerrain; // Random-map terrain tileset
	uint32      mNetworkMapTerrainSub; // Random-map terrain sub-tileset
	eNetworkVehicleSet mNetworkVehicleSet; // Random-map vehicle placement
	eNetworkPickupDensity mNetworkPickupDensity; // Random-map pickup placement
	eNetworkCoverDensity mNetworkCoverDensity; // Random-map tree and cover density
	uint8       mNetworkSelectedTeam;   // Local team selection
	uint8       mNetworkSelectedClass;  // Local character/class selection
	bool        mNetworkLockedIn;       // Local lobby lock-in state

	sFodderParameters() {
		clear();
	}

	virtual ~sFodderParameters() {
	}

	virtual void clear() {

		mDebugger = false;

		mShowHelp = false;
		mShowAbout = false;
		mPlayground = false;
		mDisableSound = false;
		mDisableVideo = false;
		mDisableIntroVideo = false;
		mSleepDelta = 20;

		mAppVeyor = false;
		mSkipService = false;
		mSkipBriefing = false;
		mSkipIntro = false;
		mSkipRecruit = false;

		mMissionNumber = 0;
		mPhaseNumber = 0;
		mMouseAlternative = false;
		mMouseLocked = false;
		mMouseSpeed = 1.5f;

		mWindowMode = false;
		mIntegerScaling = true;
		mWindowScale = 0;
		mBilinearFilter = false;

		mWindowRows = 0;
		mWindowColumns = 0;

		mRandom = false;
		mRandomSave = false;
		mRandomMenuOnStart = false;
		mRandomMapOptionsEnabled = false;
		mRandomMapSeed = NETWORK_MAP_SEED_DEFAULT;
		mRandomMapSize = NETWORK_MAP_SIZE_DEFAULT;
		mRandomMapSizeExplicit = false;
		mRandomMapTerrain = NETWORK_MAP_TERRAIN_DEFAULT;
		mRandomMapTerrainSub = 0;
		mRandomMapVehicleSet = NETWORK_VEHICLE_SET_DEFAULT;
		mRandomMapPickupDensity = NETWORK_PICKUP_DENSITY_DEFAULT;
		mRandomMapCoverDensity = NETWORK_COVER_DENSITY_DEFAULT;
		mRandomMapProfile = NETWORK_MAP_PROFILE_DEFAULT;
		mRandomMapProfileName = "";

		mDefaultPlatform = ePlatform::Any;
		mDefaultGame = eGame::CF1;

		mDemoRecord = false;
		mDemoPlayback = false;
		mDemoRecordResumeCycle = 0;
		mSingleMap = "";
		mMapGeneratePng = "";
		mMapPngOutput = "";
		mMapPngAddCoords = false;
		mMapPngAddTileIds = false;
		mMapPngScale = 1;
		mMapRouteOracle = "";
		mMapRouteOracleOutput = "";

		mCheatsEnabled = false;
		mUnitTesting = false;
		mSinglePhase = false;
		mSpritesMax = 45;
		mSpawnEnemyMax = 10;

		mCopyProtection = false;
		mForceSetupWizard = false;

		mNetworkEnabled     = false;
		mNetworkSyncTest    = false;
		mNetworkMenuStart   = eNetworkMenuStart_None;
		mNetworkInternet    = false;
		mNetworkHubHost     = "hub.openfodder.com";
		mNetworkHubPort     = 27770;
		mNetworkRoomCode    = "";
		mNetworkHostBearer  = "";
		mNetworkSessionKey.fill(0);
		mNetworkPeerIndex   = 0;
		mNetworkNumPlayers  = NETWORK_MAX_PLAYERS; // P1 A1: 2 by default; widens via host menu (CreateInternetRoom) and joiner JOINOK.
		mNetworkSessionKeyValid = false;
		mNetworkPlayerIndex = 0;
		mNetworkRemoteHost  = "";
		mNetworkRemotePort  = 7001;
		mNetworkRemotePeers.clear();
		mNetworkLocalPort   = 7000;
		mNetworkGameMode    = eNetworkGameMode_CoopCampaign;
		mNetworkMapSeed     = NETWORK_MAP_SEED_DEFAULT;
		mNetworkKillLimit   = NETWORK_KILL_LIMIT_DEFAULT;
		mNetworkTimeLimitSeconds = NETWORK_TIME_LIMIT_DEFAULT;
		mNetworkTeamCount   = NETWORK_TEAM_COUNT_DEFAULT;
		mNetworkTeamSize    = NETWORK_TEAM_SIZE_DEFAULT;
		mNetworkFriendlyFire = false;
		mNetworkMapSize     = NETWORK_MAP_SIZE_DEFAULT;
		mNetworkMapTerrain  = NETWORK_MAP_TERRAIN_DEFAULT;
		mNetworkMapTerrainSub = 0;
		mNetworkVehicleSet  = NETWORK_VEHICLE_SET_DEFAULT;
		mNetworkPickupDensity = NETWORK_PICKUP_DENSITY_DEFAULT;
		mNetworkCoverDensity = NETWORK_COVER_DENSITY_DEFAULT;
		mNetworkSelectedTeam = 0;
		mNetworkSelectedClass = 0;
		mNetworkLockedIn    = false;
	}

protected:
	virtual void PrepareOptions();
	virtual bool ProcessCLI(int argc, char *argv[]);
	bool ProcessINI();


public:
	std::string ToJson();
	bool FromJson(const std::string& pJson);

	bool Process(int argc, char *argv[]) {
		ProcessINI();
		return ProcessCLI(argc, argv);
	}

	bool SaveIni();

	bool isOriginalSpriteMax() const { return mSpritesMax == 45; }
	int32 getSpritesMax() const { return (int32) mSpritesMax; }
};
