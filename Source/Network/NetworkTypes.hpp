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

// Default UDP port for the local GGPO endpoint.
static const int NETWORK_LOCAL_PORT_DEFAULT = 7001;

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
    uint8_t mPadding[1];    // pad to 8 bytes
};

static_assert(sizeof(sNetworkInput) == 8, "sNetworkInput must be exactly 8 bytes for GGPO");

// Bit flags packed into sNetworkInput::mKeyFlags.
// Squads are fixed per-player (P1=squad 0, P2=squad 1) so there are
// no squad-select flags; weapon keys apply to each player's own squad.
enum eNetworkKeyFlag : uint8_t {
    eNetKey_None    = 0,
    eNetKey_Pause   = (1 << 0),  // P key  – toggle pause
    eNetKey_Map     = (1 << 1),  // M key  – show map overview
    eNetKey_Escape  = (1 << 2),  // Escape – abort phase
    eNetKey_WeaponG = (1 << 3),  // G key  – switch squad weapon to grenade
    eNetKey_WeaponR = (1 << 4),  // R key  – switch squad weapon to rocket
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
