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

#include "Network/NetworkTypes.hpp"

struct sRandomMapOptions {
    eNetworkGameMode      mGameMode = eNetworkGameMode_CoopCampaign;
    uint32                mSeed = NETWORK_MAP_SEED_DEFAULT;
    eNetworkMapSize       mMapSize = NETWORK_MAP_SIZE_DEFAULT;
    eNetworkMapTerrain    mMapTerrain = NETWORK_MAP_TERRAIN_DEFAULT;
    uint32                mMapTerrainSub = 0;
    eNetworkVehicleSet    mVehicleSet = NETWORK_VEHICLE_SET_DEFAULT;
    eNetworkPickupDensity mPickupDensity = NETWORK_PICKUP_DENSITY_DEFAULT;
    eNetworkCoverDensity  mCoverDensity = NETWORK_COVER_DENSITY_DEFAULT;
    eNetworkMapProfile    mProfile = NETWORK_MAP_PROFILE_DEFAULT;
};

class cRandomMapOptionsMenu {
public:
    enum class eContext {
        Campaign,
        Multiplayer,
    };

    void Open(const sRandomMapOptions& pOptions, eContext pContext);
    void Draw();
    void Tick();
    void OnBack();
    void OnRowClick(int16 pAction, int16 pArg);

    static void OnButtonClick(void* pCtx, int16 pAction, int16 pArg) {
        static_cast<cRandomMapOptionsMenu*>(pCtx)->OnRowClick(pAction, pArg);
    }

    bool IsDone() const { return mDone; }
    bool WasAccepted() const { return mAccepted; }
    const sRandomMapOptions& GetOptions() const { return mOptions; }

private:
    enum eAction : int16 {
        ACT_NONE = 0,
        ACT_BACK,
        ACT_START,
        ACT_EDIT_SEED,
        ACT_CYCLE_MODE,
        ACT_CYCLE_MAP_SIZE,
        ACT_CYCLE_MAP_TERRAIN,
        ACT_CYCLE_COVER,
        ACT_CYCLE_VEHICLES,
        ACT_CYCLE_PICKUPS,
        ACT_CYCLE_PROFILE,
        ACT_RANDOMIZE_SEED,
        ACT_TAB_MAIN,
        ACT_TAB_LAYOUT,
        ACT_TAB_TERRAIN,
        ACT_TAB_SUPPORT,
        ACT_TAB_FAIRNESS,
    };

    enum class eEditField {
        None,
        Seed,
    };

    enum class eTab {
        Main,
        Layout,
        Terrain,
        Support,
        Fairness,
    };

    void DrawTabs(int16 pY);
    void DrawTab(const char* pLabel, int16 pAction, bool pActive, size_t pX1, size_t pX2, int16 pY);
    void DrawValueButton(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction);
    void DrawField(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction, bool pActive);
    void DrawStaticValue(const char* pLabel, const std::string& pValue, int16 pY);
    void DrawFooter();
    void HandleTextInput();
    void SelectField(int16 pAction);
    bool CanStart() const;
    void SyncSeedValue();
    void RandomizeSeed();
    void CycleProfile();
    void ApplyProfile();
    void MarkProfileCustom();
    void InferProfile();
    const char* GetProfileName() const;
    const char* GetTerrainName() const;
    const char* GetModeName() const;
    const char* GetModeShortName() const;
    bool IsPvPMode() const;

    sRandomMapOptions mOptions;
    eContext mContext = eContext::Campaign;
    eEditField mEditField = eEditField::None;
    eTab mTab = eTab::Main;
    eNetworkMapProfile mProfile = NETWORK_MAP_PROFILE_DEFAULT;
    bool mDone = false;
    bool mAccepted = false;
    std::string mSeedText = std::to_string(NETWORK_MAP_SEED_DEFAULT);
};
