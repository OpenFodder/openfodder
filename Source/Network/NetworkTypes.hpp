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

#pragma once

#include <cstdint>

// Maximum number of players in a network session.
static const int NETWORK_MAX_PLAYERS = 2;
static const int NETWORK_MAX_LOCAL_SQUADS = 3;
static const int NETWORK_MAX_SQUADS = NETWORK_MAX_PLAYERS * NETWORK_MAX_LOCAL_SQUADS;
static const int8_t NETWORK_INVALID_SQUAD_OWNER = -1;

// Default UDP port for the local GGPO endpoint.
static const int NETWORK_LOCAL_PORT_DEFAULT = 7001;

static const uint32_t NETWORK_MAP_SEED_DEFAULT = 0x1337;
static const uint16_t NETWORK_KILL_LIMIT_DEFAULT = 10;
static const uint16_t NETWORK_TIME_LIMIT_DEFAULT = 0;
static const uint8_t NETWORK_TEAM_COUNT_DEFAULT = 2;
static const uint8_t NETWORK_TEAM_SIZE_DEFAULT = 1;
static const uint8_t NETWORK_COMPATIBILITY_VERSION = 3;
static const int8_t NETWORK_MATCH_NO_WINNER = -1;
static const int8_t NETWORK_MATCH_DRAW = -2;
static const uint16_t NETWORK_SIM_FRAMES_PER_SECOND = 17;

enum eNetworkMapSize : uint8_t {
    eNetworkMapSize_Small = 0,
    eNetworkMapSize_Medium,
    eNetworkMapSize_Large,
    eNetworkMapSize_Count,
};

inline const char* Network_MapSizeName(eNetworkMapSize pSize) {
    switch (pSize) {
    case eNetworkMapSize_Small:  return "SMALL";
    case eNetworkMapSize_Medium: return "MEDIUM";
    case eNetworkMapSize_Large:  return "LARGE";
    default:                     return "MEDIUM";
    }
}

inline eNetworkMapSize Network_NormalizeMapSize(uint8_t pSize) {
    if (pSize >= eNetworkMapSize_Count)
        return eNetworkMapSize_Medium;

    return static_cast<eNetworkMapSize>(pSize);
}

enum eNetworkMapTerrain : uint8_t {
    eNetworkMapTerrain_Random = 0,
    eNetworkMapTerrain_Jungle,
    eNetworkMapTerrain_Desert,
    eNetworkMapTerrain_Ice,
    eNetworkMapTerrain_Moors,
    eNetworkMapTerrain_Count,
};

inline const char* Network_MapTerrainName(eNetworkMapTerrain pTerrain) {
    switch (pTerrain) {
    case eNetworkMapTerrain_Random: return "RANDOM";
    case eNetworkMapTerrain_Jungle: return "JUNGLE";
    case eNetworkMapTerrain_Desert: return "DESERT";
    case eNetworkMapTerrain_Ice:    return "ICE";
    case eNetworkMapTerrain_Moors:  return "MOORS";
    default:                        return "JUNGLE";
    }
}

inline eNetworkMapTerrain Network_NormalizeMapTerrain(uint8_t pTerrain) {
    if (pTerrain >= eNetworkMapTerrain_Count)
        return eNetworkMapTerrain_Jungle;

    return static_cast<eNetworkMapTerrain>(pTerrain);
}

enum eNetworkVehicleSet : uint8_t {
    eNetworkVehicleSet_None = 0,
    eNetworkVehicleSet_Light,
    eNetworkVehicleSet_Armed,
    eNetworkVehicleSet_Tanks,
    eNetworkVehicleSet_Mixed,
    eNetworkVehicleSet_Count,
};

inline const char* Network_VehicleSetName(eNetworkVehicleSet pSet) {
    switch (pSet) {
    case eNetworkVehicleSet_None:  return "NONE";
    case eNetworkVehicleSet_Light: return "LIGHT";
    case eNetworkVehicleSet_Armed: return "ARMED";
    case eNetworkVehicleSet_Tanks: return "TANKS";
    case eNetworkVehicleSet_Mixed: return "MIXED";
    default:                       return "NONE";
    }
}

inline eNetworkVehicleSet Network_NormalizeVehicleSet(uint8_t pSet) {
    if (pSet >= eNetworkVehicleSet_Count)
        return eNetworkVehicleSet_None;

    return static_cast<eNetworkVehicleSet>(pSet);
}

enum eNetworkPickupDensity : uint8_t {
    eNetworkPickupDensity_Low = 0,
    eNetworkPickupDensity_Normal,
    eNetworkPickupDensity_High,
    eNetworkPickupDensity_Count,
};

inline const char* Network_PickupDensityName(eNetworkPickupDensity pDensity) {
    switch (pDensity) {
    case eNetworkPickupDensity_Low:    return "LOW";
    case eNetworkPickupDensity_Normal: return "NORMAL";
    case eNetworkPickupDensity_High:   return "HIGH";
    default:                           return "NORMAL";
    }
}

inline eNetworkPickupDensity Network_NormalizePickupDensity(uint8_t pDensity) {
    if (pDensity >= eNetworkPickupDensity_Count)
        return eNetworkPickupDensity_Normal;

    return static_cast<eNetworkPickupDensity>(pDensity);
}

enum eNetworkCoverDensity : uint8_t {
    eNetworkCoverDensity_Sparse = 0,
    eNetworkCoverDensity_Normal,
    eNetworkCoverDensity_Dense,
    eNetworkCoverDensity_Heavy,
    eNetworkCoverDensity_Count,
};

inline const char* Network_CoverDensityName(eNetworkCoverDensity pDensity) {
    switch (pDensity) {
    case eNetworkCoverDensity_Sparse: return "SPARSE";
    case eNetworkCoverDensity_Normal: return "NORMAL";
    case eNetworkCoverDensity_Dense:  return "DENSE";
    case eNetworkCoverDensity_Heavy:  return "HEAVY";
    default:                          return "NORMAL";
    }
}

inline eNetworkCoverDensity Network_NormalizeCoverDensity(uint8_t pDensity) {
    if (pDensity >= eNetworkCoverDensity_Count)
        return eNetworkCoverDensity_Normal;

    return static_cast<eNetworkCoverDensity>(pDensity);
}

enum eNetworkMapProfile : uint8_t {
    eNetworkMapProfile_Jungle = 0,
    eNetworkMapProfile_Beach,
    eNetworkMapProfile_Ice,
    eNetworkMapProfile_Random,
    eNetworkMapProfile_Custom,
    eNetworkMapProfile_IceMaze,
    eNetworkMapProfile_IceNeck,
    eNetworkMapProfile_IceSkidooJump,
    eNetworkMapProfile_JungleMaze,
    eNetworkMapProfile_JungleNeck,
    eNetworkMapProfile_Count,
};

inline const char* Network_MapProfileName(eNetworkMapProfile pProfile) {
    switch (pProfile) {
    case eNetworkMapProfile_Jungle: return "JUNGLE";
    case eNetworkMapProfile_Beach:  return "BEACH";
    case eNetworkMapProfile_Ice:    return "ICE";
    case eNetworkMapProfile_Random: return "RANDOM";
    case eNetworkMapProfile_Custom: return "CUSTOM";
    case eNetworkMapProfile_IceMaze: return "ICE MAZE";
    case eNetworkMapProfile_IceNeck: return "ICE NECK";
    case eNetworkMapProfile_IceSkidooJump: return "SKIDOO";
    case eNetworkMapProfile_JungleMaze: return "JUNGLE MAZE";
    case eNetworkMapProfile_JungleNeck: return "JUNGLE NECK";
    default:                        return "CUSTOM";
    }
}

inline eNetworkMapProfile Network_NormalizeMapProfile(uint8_t pProfile) {
    if (pProfile >= eNetworkMapProfile_Count)
        return eNetworkMapProfile_Jungle;

    return static_cast<eNetworkMapProfile>(pProfile);
}

static const eNetworkMapSize NETWORK_MAP_SIZE_DEFAULT = eNetworkMapSize_Medium;
static const eNetworkMapTerrain NETWORK_MAP_TERRAIN_DEFAULT = eNetworkMapTerrain_Jungle;
static const eNetworkVehicleSet NETWORK_VEHICLE_SET_DEFAULT = eNetworkVehicleSet_None;
static const eNetworkPickupDensity NETWORK_PICKUP_DENSITY_DEFAULT = eNetworkPickupDensity_Normal;
static const eNetworkCoverDensity NETWORK_COVER_DENSITY_DEFAULT = eNetworkCoverDensity_Normal;
static const eNetworkMapProfile NETWORK_MAP_PROFILE_DEFAULT = eNetworkMapProfile_Jungle;

enum eNetworkObjectiveState : uint8_t {
    eNetworkObjectiveState_None = 0,
    eNetworkObjectiveState_Dropped,
    eNetworkObjectiveState_Carried,
    eNetworkObjectiveState_Extracted,
};

enum eNetworkGameMode : uint8_t {
    eNetworkGameMode_CoopCampaign = 0,
    eNetworkGameMode_Deathmatch,
    eNetworkGameMode_SquadDeathmatch,
    eNetworkGameMode_RescuePrisoner,
    eNetworkGameMode_AvatarDeathmatch,
    eNetworkGameMode_TeamAvatar,
    eNetworkGameMode_Count,
};

inline const char* Network_GameModeName(eNetworkGameMode pMode) {
    switch (pMode) {
    case eNetworkGameMode_CoopCampaign:     return "CO-OP CAMPAIGN";
    case eNetworkGameMode_Deathmatch:       return "DEATHMATCH";
    case eNetworkGameMode_SquadDeathmatch:  return "SQUAD DEATHMATCH";
    case eNetworkGameMode_RescuePrisoner:   return "RESCUE PRISONER";
    case eNetworkGameMode_AvatarDeathmatch: return "AVATAR DEATHMATCH";
    case eNetworkGameMode_TeamAvatar:       return "TEAM AVATAR";
    default:                                return "UNKNOWN";
    }
}

inline eNetworkGameMode Network_NormalizeGameMode(uint8_t pMode) {
    if (pMode >= eNetworkGameMode_Count)
        return eNetworkGameMode_CoopCampaign;

    return static_cast<eNetworkGameMode>(pMode);
}

inline bool Network_IsPvPMode(eNetworkGameMode pMode) {
    return pMode != eNetworkGameMode_CoopCampaign;
}

inline bool Network_IsAvatarMode(eNetworkGameMode pMode) {
    return pMode == eNetworkGameMode_AvatarDeathmatch ||
           pMode == eNetworkGameMode_TeamAvatar;
}

inline bool Network_IsTeamMode(eNetworkGameMode pMode) {
    return pMode == eNetworkGameMode_CoopCampaign ||
           pMode == eNetworkGameMode_SquadDeathmatch ||
           pMode == eNetworkGameMode_RescuePrisoner ||
           pMode == eNetworkGameMode_TeamAvatar;
}

struct sNetworkMatchSettings {
    eNetworkGameMode mGameMode = eNetworkGameMode_CoopCampaign;
    uint8_t          mTeamCount = NETWORK_TEAM_COUNT_DEFAULT;
    uint8_t          mTeamSize = NETWORK_TEAM_SIZE_DEFAULT;
    uint8_t          mFriendlyFire = 0;
    uint32_t         mMapSeed = NETWORK_MAP_SEED_DEFAULT;
    uint16_t         mKillLimit = NETWORK_KILL_LIMIT_DEFAULT;
    uint16_t         mTimeLimitSeconds = NETWORK_TIME_LIMIT_DEFAULT;
    eNetworkMapSize  mMapSize = NETWORK_MAP_SIZE_DEFAULT;
    eNetworkMapTerrain mMapTerrain = NETWORK_MAP_TERRAIN_DEFAULT;
    eNetworkVehicleSet mVehicleSet = NETWORK_VEHICLE_SET_DEFAULT;
    eNetworkPickupDensity mPickupDensity = NETWORK_PICKUP_DENSITY_DEFAULT;
    eNetworkCoverDensity mCoverDensity = NETWORK_COVER_DENSITY_DEFAULT;
    uint8_t          mMapTerrainSub = 0; // Random-map terrain sub-tileset (e.g. jungle beach = 1); synced host->joiner
};

static_assert(sizeof(sNetworkMatchSettings) == 20, "sNetworkMatchSettings must stay compact for lobby packets");

struct sNetworkMatchState {
    uint16_t mKills[NETWORK_MAX_PLAYERS] = {};
    uint16_t mDeaths[NETWORK_MAX_PLAYERS] = {};
    uint16_t mTimerSeconds = 0;
    int8_t   mWinnerTeam = NETWORK_MATCH_NO_WINNER;
    int8_t   mObjectiveCarrierPlayer = -1;
    uint8_t  mObjectiveState = 0;
    int8_t   mLastDamageOwner[NETWORK_MAX_PLAYERS] = {NETWORK_MATCH_NO_WINNER, NETWORK_MATCH_NO_WINNER};
    uint8_t  mPadding[1] = {};
};

static_assert(sizeof(sNetworkMatchState) == 16, "sNetworkMatchState must stay compact for GGPO snapshots");

// -----------------------------------------------------------------------
// Per-frame input packet sent through GGPO (fixed size, POD).
// One of these is transmitted per player per GGPO frame.
// -----------------------------------------------------------------------
struct sNetworkInput {
    int16_t mMouseX;        // Cursor X in WORLD space (screen_X + camera_X) — camera-independent
    int16_t mMouseY;        // Cursor Y in WORLD space (screen_Y + camera_Y) — camera-independent
    uint8_t mMouseButtons;  // bit 0 = left button, bit 1 = right button
    uint8_t mKeyFlags;      // packed key-press flags (see eNetworkKeyFlag)
    int8_t  mCursorSprite;  // mMouseSpriteCurrent (for remote cursor overlay)
    uint8_t mCommand;       // packed one-shot sidebar command (see eNetworkCommand)
};

static_assert(sizeof(sNetworkInput) == 8, "sNetworkInput must be exactly 8 bytes for GGPO");

// Bit flags packed into sNetworkInput::mKeyFlags.
// Squad-select flags are local sidebar slots. The deterministic simulation
// maps those slots to the owning player's global squad ids.
enum eNetworkKeyFlag : uint8_t {
    eNetKey_None    = 0,
    eNetKey_Pause   = (1 << 0),  // P key  – toggle pause
    eNetKey_Map     = (1 << 1),  // M key  – show map overview
    eNetKey_Escape  = (1 << 2),  // Escape – abort phase
    eNetKey_WeaponG = (1 << 3),  // G key  – switch squad weapon to grenade
    eNetKey_WeaponR = (1 << 4),  // R key  – switch squad weapon to rocket
    eNetKey_Squad0  = (1 << 5),  // Select local sidebar squad slot 0
    eNetKey_Squad1  = (1 << 6),  // Select local sidebar squad slot 1
    eNetKey_Squad2  = (1 << 7),  // Select local sidebar squad slot 2
};

enum eNetworkCommand : uint8_t {
    eNetCommand_None = 0,
    eNetCommand_ToggleTroop0 = 1, // 1-8 toggle troop row 0-7 in selected squad
    eNetCommand_ToggleTroop7 = eNetCommand_ToggleTroop0 + 7,
    eNetCommand_SplitSelected = 9,
};

// Indices for the two-player slots.
enum eNetworkPlayerSlot {
    eNetPlayer_1 = 0,
    eNetPlayer_2 = 1,
};

// Snapshot of all camera / map-tile state needed to save and restore the
// deterministic simulation camera independently of each player's local
// viewport camera.
struct sNetCameraState {
    int32_t  CameraX, CameraY;
    int32_t  MapTile_Ptr;
    int16_t  MapTile_ColumnOffset, MapTile_RowOffset;
    int16_t  MapTile_DrawX, MapTile_DrawY;
    int32_t  MapTile_SpeedX, MapTile_SpeedY;
    int32_t  MapTile_TargetX, MapTile_TargetY;
    int16_t  MapTile_MoveDirectionX, MapTile_MoveDirectionY;
    int16_t  MapTile_MoveDirectionX_Previous, MapTile_MoveDirectionY_Previous;
    int16_t  MapTile_SpeedX_Previous, MapTile_SpeedY_Previous;
    int16_t  MapTile_MovedHorizontal, MapTile_MovedVertical;
    int16_t  MapTile_Column_CurrentScreen, MapTile_Row_CurrentScreen;
    int32_t  Camera_AccelerationX, Camera_AccelerationY;
    int16_t  Camera_MoveDirectionX, Camera_MoveDirectionY;
    int16_t  Camera_MovePauseX, Camera_MovePauseY;
    int16_t  Camera_PanTargetX, Camera_PanTargetY;
    int16_t  Camera_Scroll_Speed;
    bool     Camera_Speed_Reset_X, Camera_Speed_Reset_Y;
    int32_t  Camera_Speed_X, Camera_Speed_Y;
    int16_t  Camera_StartPosition_X, Camera_StartPosition_Y;
    int16_t  Camera_SquadLeaderX, Camera_SquadLeaderY;
    int32_t  Camera_TileSpeed_Overflow;
    int16_t  Camera_TileX, Camera_TileY;
    int32_t  Camera_TileSpeedX, Camera_TileSpeedY;
    int32_t  Camera_Speed_Max;
    bool     Camera_Start_Adjust;
    int16_t  Camera_Reached_Target;
    int16_t  Camera_Panning_ToTarget;
    int32_t  dword_39F36;           // Camera scroll speed factor (Camera_UpdateSpeedFromPanTarget)
    bool     Mouse_Locked;          // Set by Camera_PanTarget_AdjustToward_SquadLeader
};
