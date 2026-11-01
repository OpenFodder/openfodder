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


bool cFodder::Campaign_Load(std::string pName) {

    // If no campaign name was provided, use the default for the current version
    if (!pName.size()) {

        if (!mVersionCurrent->isCustom()) {
            pName = mVersionCurrent->mName;
        }
    }

    VersionSwitch(mVersions->GetForCampaign(pName, mParams->mDefaultPlatform));
    if (!mGame_Data.mCampaign.LoadCampaign(pName, pName != mVersionCurrent->mName)) {
        // TODO: But what?

        return false;
    }

    return true;
}

void cFodder::Campaign_Selection() {
    mPhase_Complete = false;

    Phase_EngineReset();
    GameData_Reset();
    mMap_Destroy_Tiles.clear();

    Image_FadeOut();

    mGraphics->PaletteSet();

    mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    mMouseX_Offset = -8;
    mMouseY_Offset = -8;

    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);

    mCustom_Mode = eCustomMode_None;

    std::string CampaignFile;

SelectCampaign:;

    CampaignFile = MainMenu_Run();

#ifdef OPENFODDER_ENABLE_NETWORK
    // MP generated-map modes (Coop Random / PvP) come back from
    // MainMenu_Run with an empty CampaignFile — there's no
    // campaign to pick, the gameplay surface is generated. Skip the
    // exit-on-empty test and the version-lookup-by-campaign-name in those
    // cases; Mission_Loop reads mNetworkGameMode directly to route to
    // CreateRandom + multiplayer.js.
    const bool MPSkipsCampaign = mStartParams && mStartParams->mNetworkEnabled &&
                                 Network_UsesGeneratedMap(mStartParams->mNetworkGameMode);
#else
    const bool MPSkipsCampaign = false;
#endif

    // Exit Pressed?
    if (mGUI_SaveLoadAction == 1 || mGUI_SaveLoadAction == 4 ||
        (!MPSkipsCampaign && !CampaignFile.size())) {

        // Return to custom menu
        mDemo_ExitMenu = 1;
        mCustom_Mode = eCustomMode_None;

        return;
    }

    if (MPSkipsCampaign) {
        // No campaign to load — Mission_Loop will see UsesGeneratedMap and
        // run CreateRandom instead. Set mVersionDefault so the rest of the
        // engine has a reasonable starting version.
        mVersionDefault = mVersionCurrent;
        mCustom_Mode = eCustomMode_Map;
        return;
    }

    // Find a data version to use with this campaign
    // If no version is found, we use the currently loaded one
    {
        const sGameVersion* Version = mVersions->GetForCampaign(CampaignFile, mVersionCurrent->mPlatform);

        if (!Version) {
            Version = mVersions->GetForCampaign(CampaignFile);
        }

        // Custom modes must use the graphics platform of their backing retail
        // data, which can differ from the demo used to display the main menu.
        if (Version && Version->isCustom()) {
            const auto* Retail = mVersions->GetRetail(Version->mPlatform, Version->mGame);
            if (Retail) {
                for (const auto* Available : g_ResourceMan->GetAvailable()) {
                    if (Available->mName == CampaignFile && Available->mPlatform == Retail->mPlatform &&
                        Available->mGame == Retail->mGame) {
                        Version = Available;
                        break;
                    }
                }
            }
        }

        // Keep the main menu's loaded scene while configuring a random map.
        // Switching to Custom first replaces its graphics without map assets.
        if (CampaignFile == "Random Map" && !RandomMapOptions_RunCampaign()) {
            if (mExit) {
                mGUI_SaveLoadAction = 1;
                return;
            }
            mGUI_SaveLoadAction = 0;
            goto SelectCampaign;
        }

        // Load a new version?
        if (Version && Version != mVersionCurrent) {
            VersionSwitch(Version);
        }

        // Set the default/starting version
        mVersionDefault = mVersionCurrent;

        // Single Map Mode?
        if (CampaignFile == "Single Map" || CampaignFile == "Random Map") {

            mGame_Data.mCampaign.SetSingleMapCampaign();
            mCustom_Mode = eCustomMode_Map;
            return;

            // If no version, it must be a custom campaign
        }
        else if (!Version) {

			// If version is currently XMAS, it means no retail is available
			if (mVersionCurrent->isAmigaXmas()) {

				VersionSwitch(mVersions->GetForCampaign("Amiga Action"));

				// Set the default/starting version
				mVersionDefault = mVersions->GetForCampaign("Amiga Action");
			}

            mCustom_Mode = eCustomMode_Set;
        }
    }

    // Load the campaign
    if (Campaign_Load(CampaignFile) == true) {

        // 
        if (mVersionCurrent->isCustom()) {
            mDemo_ExitMenu = 1;
            mCustom_ExitMenu = 1;
        }

        WindowTitleBaseSetup();
        return;
    }

    mDemo_ExitMenu = 1;
    mCustom_Mode = eCustomMode_None;
}
