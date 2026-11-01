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
#include "Network/NetworkMenuText.hpp"

void cRandomMapOptionsMenu::Open(const sRandomMapOptions& pOptions, eContext pContext)
{
    mOptions = pOptions;
    mContext = pContext;
    mDone = false;
    mAccepted = false;
    mEditField = eEditField::None;
    mTab = eTab::Main;
    mSeedText = std::to_string(mOptions.mSeed);
    InferMapType();
}

void cRandomMapOptionsMenu::Tick()
{
    HandleTextInput();
    if (g_Fodder->mPhase_InterruptTicks >= 3) {
        g_Fodder->mPhase_InterruptTicks = 0;
        g_Fodder->Mission_Sprites_Handle();
        g_Fodder->Sound_Tick();
    }
}

void cRandomMapOptionsMenu::OnBack()
{
    if (mEditField != eEditField::None) {
        mEditField = eEditField::None;
        return;
    }

    SyncSeedValue();
    mAccepted = (mContext == eContext::Multiplayer);
    mDone = true;
}

void cRandomMapOptionsMenu::OnRowClick(int16 pAction, int16 pArg)
{
    (void)pArg;

    switch (pAction) {
    case ACT_BACK:
        OnBack();
        break;

    case ACT_START:
        SyncSeedValue();
        if (CanStart()) {
            mAccepted = true;
            mDone = true;
        }
        break;

    case ACT_EDIT_SEED:
        SelectField(pAction);
        break;

    case ACT_CYCLE_MODE:
        if (mContext == eContext::Multiplayer) {
            mEditField = eEditField::None;
            mOptions.mGameMode = Network_NormalizeGameMode((uint8_t)mOptions.mGameMode + 1);
        }
        break;

    case ACT_CYCLE_MAP_SIZE:
        mEditField = eEditField::None;
        // Size is independent from the selected terrain topology.
        mOptions.mMapSize = static_cast<eNetworkMapSize>(((uint8_t)mOptions.mMapSize + 1) % eNetworkMapSize_Count);
        break;

    case ACT_CYCLE_MAP_TERRAIN:
        mEditField = eEditField::None;
        CycleTerrain();
        break;

    case ACT_CYCLE_COVER:
        mEditField = eEditField::None;
        mOptions.mCoverDensity = static_cast<eNetworkCoverDensity>(((uint8_t)mOptions.mCoverDensity + 1) % eNetworkCoverDensity_Count);
        break;

    case ACT_CYCLE_VEHICLES:
        if (mContext == eContext::Campaign && mMapType == eMapType::SkidooJump) break;
        mEditField = eEditField::None;
        mOptions.mVehicleSet = static_cast<eNetworkVehicleSet>(((uint8_t)mOptions.mVehicleSet + 1) % eNetworkVehicleSet_Count);
        break;

    case ACT_CYCLE_PICKUPS:
        if (mContext == eContext::Campaign && mMapType == eMapType::Maze) break;
        mEditField = eEditField::None;
        mOptions.mPickupDensity = static_cast<eNetworkPickupDensity>(((uint8_t)mOptions.mPickupDensity + 1) % eNetworkPickupDensity_Count);
        break;

    case ACT_CYCLE_MAP_TYPE:
        mEditField = eEditField::None;
        CycleMapType();
        break;

    case ACT_RANDOMIZE_SEED:
        RandomizeSeed();
        break;

    case ACT_TAB_MAIN:
        mEditField = eEditField::None;
        mTab = eTab::Main;
        break;

    case ACT_TAB_SUPPORT:
        mEditField = eEditField::None;
        mTab = eTab::Support;
        break;

    default:
        break;
    }
}

void cRandomMapOptionsMenu::Draw()
{
    // This view is also embedded in multiplayer setup, whose other pages
    // clear the surface. Repaint the scenery instead of inheriting that page.
    if (g_Fodder->mMapLoaded && g_Fodder->mMap && !g_Fodder->mMap->empty()) {
        if (!g_Fodder->mStartParams->mDisableVideo) g_Fodder->mGraphics->MapTiles_Draw();
        g_Fodder->Sprites_Draw();
    } else g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();
    auto* pixels = g_Fodder->mSurface->GetSurfaceBuffer();
    const size_t size = g_Fodder->mSurface->GetSurfaceBufferSize();
    const size_t stride = g_Fodder->mSurface->GetWidth();
    const std::vector<uint8> background(pixels, pixels + size);
    constexpr uint8 transparent = 0xFF;
    std::fill(pixels, pixels + size, transparent);

    const int extra = PLATFORM_BASED(0, 25);
    g_Fodder->mString_GapCharID = 0;
    g_Fodder->String_Print_Large("RANDOM MAP", false, 19);
    DrawTabs(39);
    const std::string description = mTab == eTab::Main ? "SHAPE YOUR NEXT MISSION" : "EQUIP YOUR NEXT MISSION";
    const int width = g_Fodder->Sidebar_Menu_MeasureWidth(description);
    g_Fodder->Sidebar_Menu_Print(description, (320 - width) / 2, 61,
                                cFodder::eSidebarMenuColor::Inactive);
    int16 y = 74 + extra / 2;
    constexpr int16 rowH = 18;
    if (mTab == eTab::Main) {
        DrawValueButton("TERRAIN", GetTerrainName(), y, ACT_CYCLE_MAP_TERRAIN); y += rowH;
        if (mContext == eContext::Campaign || mOptions.mMapTerrain == eNetworkMapTerrain_Jungle) {
            DrawValueButton("MAP SHAPE", GetMapTypeName(), y, ACT_CYCLE_MAP_TYPE); y += rowH;
        }
        DrawValueButton("MAP SIZE", Network_MapSizeName(mOptions.mMapSize), y, ACT_CYCLE_MAP_SIZE); y += rowH;
        DrawValueButton("COVER", Network_CoverDensityName(mOptions.mCoverDensity), y, ACT_CYCLE_COVER); y += rowH;
        DrawField("SEED", mSeedText, y, ACT_EDIT_SEED, mEditField == eEditField::Seed);
        DrawTab("NEW", ACT_RANDOMIZE_SEED, false, 264, 304, y);
    } else {
        if (mContext != eContext::Campaign || mMapType != eMapType::SkidooJump) {
            DrawValueButton("VEHICLES", Network_VehicleSetName(mOptions.mVehicleSet), y, ACT_CYCLE_VEHICLES); y += rowH;
        }
        if (mContext != eContext::Campaign || mMapType != eMapType::Maze) {
            DrawValueButton("PICKUPS", Network_PickupDensityName(mOptions.mPickupDensity), y, ACT_CYCLE_PICKUPS); y += rowH;
        }
        if (mContext == eContext::Multiplayer)
            DrawValueButton("MATCH TYPE", GetModeName(), y, ACT_CYCLE_MODE);
    }
    DrawFooter();
    const int footer = 175 + extra;
    if (mContext == eContext::Campaign) {
        if (CanStart()) DrawTab("START MISSION", ACT_START, false, 165, 306, footer);
        else g_Fodder->String_Print_Small_CentreInBox("ENTER A SEED", 165, 306, footer + 5);
        DrawTab("BACK", ACT_BACK, false, 14, 145, footer);
    } else DrawTab("DONE", ACT_BACK, false, 90, 230, footer);

    const std::vector<uint8> foreground(pixels, pixels + size);
    std::copy(background.begin(), background.end(), pixels);
    for (size_t row = 1; row + 1 < g_Fodder->mSurface->GetHeight(); ++row)
        for (size_t col = 1; col + 1 < stride; ++col) {
            if (foreground[row * stride + col] == transparent) continue;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    pixels[(row + dy) * stride + col + dx] = 0;
        }
    for (size_t index = 0; index < size; ++index)
        if (foreground[index] != transparent) pixels[index] = foreground[index];
}

void cRandomMapOptionsMenu::DrawTabs(int16 pY)
{
    DrawTab("MAP", ACT_TAB_MAIN, mTab == eTab::Main, 24, 152, pY);
    DrawTab("SUPPORT", ACT_TAB_SUPPORT, mTab == eTab::Support, 168, 296, pY);
}

void cRandomMapOptionsMenu::DrawTab(const char* pLabel, int16 pAction, bool pActive, size_t pX1, size_t pX2, int16 pY)
{
    const bool hover = g_Fodder->mMouseX + 0x20 >= int(pX1) && g_Fodder->mMouseX + 0x20 <= int(pX2) &&
        g_Fodder->mMouseY >= pY && g_Fodder->mMouseY <= pY + 15;
    g_Fodder->String_Print_Small_CentreInBox(pLabel, pX1, pX2, pY + 5);
    if (hover || pActive) g_Fodder->Briefing_DrawBox(pX1, pY, pX2 - pX1, 16, 0xF3);
    g_Fodder->mGUI_Temp_X = int16(pX1);
    g_Fodder->mGUI_Temp_Y = pY;
    g_Fodder->mGUI_Temp_Width = int16(pX2 - pX1);
    g_Fodder->mGUI_Draw_LastHeight = 10;
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, pAction);
}

void cRandomMapOptionsMenu::DrawValueButton(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction)
{
    g_Fodder->String_Print_Small_Left(pLabel, 22, pY + 5);
    const std::string value = NetworkMenu_FitText(pValue, 152);
    g_Fodder->String_Print_Small_CentreInBox(value, 126, 286, pY + 5);
    // Clicking the value or its chevron advances to the next choice.
    g_Fodder->String_Print_Small(">", 292, pY + 5);
    const bool hover = g_Fodder->mMouseX + 0x20 >= 124 && g_Fodder->mMouseX + 0x20 <= 304 &&
        g_Fodder->mMouseY >= pY && g_Fodder->mMouseY <= pY + 15;
    if (hover) g_Fodder->Briefing_DrawBox(124, pY, 180, 16, 0xF3);
    g_Fodder->mGUI_Temp_X = 124;
    g_Fodder->mGUI_Temp_Y = pY;
    g_Fodder->mGUI_Temp_Width = 180;
    g_Fodder->mGUI_Draw_LastHeight = 10;
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, pAction);
}

void cRandomMapOptionsMenu::DrawField(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction, bool pActive)
{
    g_Fodder->String_Print_Small_Left(pLabel, 22, pY + 5);
    const std::string value = pValue.empty() ? "ENTER SEED" : pValue;
    DrawTab(value.c_str(), pAction, pActive, 124, 258, pY);
}

void cRandomMapOptionsMenu::DrawStaticValue(const char* pLabel, const std::string& pValue, int16 pY)
{
    g_Fodder->String_Print_Small_Left(pLabel, 22, pY + 5);
    g_Fodder->String_Print_Small_CentreInBox(NetworkMenu_FitText(pValue, 174), 126, 304, pY + 5);
}

void cRandomMapOptionsMenu::DrawFooter()
{
    const char* hint = mEditField == eEditField::Seed ? "TYPE A NUMBER, THEN PRESS ENTER" :
        mTab == eTab::Main ? "CLICK SEED TO EDIT - NEW TO SHUFFLE" :
        mContext == eContext::Campaign && mMapType == eMapType::Maze ? "MAZE LAYOUT SETS PICKUP LEVELS" :
        mContext == eContext::Campaign && mMapType == eMapType::SkidooJump ? "THIS LAYOUT INCLUDES A REQUIRED SKIDOO" :
        mContext == eContext::Campaign && mMapType == eMapType::Random ? "SOME LAYOUTS SET THEIR OWN SUPPORT" :
        "CHANGES APPLY TO THE NEXT MAP";
    g_Fodder->String_Print_Small_CentreInBox(hint, 14, 306, 165 + PLATFORM_BASED(0, 25) / 2);
}

void cRandomMapOptionsMenu::HandleTextInput()
{
    if (mEditField == eEditField::None)
        return;

    int16 KeyAscii = 0;
    if (g_Fodder->mKeyCode != g_Fodder->mInput_LastKey) {
        g_Fodder->mInput_LastKey = g_Fodder->mKeyCode;

        if (g_Fodder->mKeyCode) {
            if (g_Fodder->mKeyCode >= SDL_SCANCODE_1 && g_Fodder->mKeyCode <= SDL_SCANCODE_9)
                KeyAscii = '1' + (g_Fodder->mKeyCode - SDL_SCANCODE_1);

            if (g_Fodder->mKeyCode == SDL_SCANCODE_0)
                KeyAscii = '0';

            if (g_Fodder->mKeyCode == SDL_SCANCODE_BACKSPACE)
                KeyAscii = 8;

            if (g_Fodder->mKeyCode == SDL_SCANCODE_RETURN)
                KeyAscii = 0x0D;
        }
    }

    if (!KeyAscii)
        return;

    if (KeyAscii == 0x0D) {
        mEditField = eEditField::None;
        return;
    }

    if (KeyAscii == 8) {
        if (mSeedText.size())
            mSeedText.pop_back();
        SyncSeedValue();
        return;
    }

    if (mSeedText.size() >= 10 || KeyAscii < '0' || KeyAscii > '9')
        return;

    std::string Candidate = mSeedText + (char)KeyAscii;
    uint32 ParsedSeed = 0;
    if (!NetworkMenu_UInt32FromText(Candidate, ParsedSeed))
        return;

    mSeedText.push_back((char)KeyAscii);
    SyncSeedValue();
}

void cRandomMapOptionsMenu::SelectField(int16 pAction)
{
    if (pAction == ACT_EDIT_SEED)
        mEditField = eEditField::Seed;

    g_Fodder->mInput_LastKey = g_Fodder->mKeyCode;
}

bool cRandomMapOptionsMenu::CanStart() const
{
    uint32 ParsedSeed = 0;
    return NetworkMenu_UInt32FromText(mSeedText, ParsedSeed);
}

void cRandomMapOptionsMenu::SyncSeedValue()
{
    uint32 ParsedSeed = 0;
    if (NetworkMenu_UInt32FromText(mSeedText, ParsedSeed))
        mOptions.mSeed = ParsedSeed;
}

void cRandomMapOptionsMenu::RandomizeSeed()
{
    const uint32 Tick = (uint32)SDL_GetTicks();
    mOptions.mSeed = Tick ^ (mOptions.mSeed * 1664525u) ^ 0xA5A5A5A5u;
    if (!mOptions.mSeed)
        mOptions.mSeed = NETWORK_MAP_SEED_DEFAULT;

    mSeedText = std::to_string(mOptions.mSeed);
    mEditField = eEditField::None;
}

void cRandomMapOptionsMenu::InferMapType()
{
    mOptions.mProfile = Network_NormalizeMapProfile((uint8_t)mOptions.mProfile);
    mOptions.mMapTerrain = Network_NormalizeMapTerrain((uint8_t)mOptions.mMapTerrain);
    mMapType = eMapType::Random;

    const std::string& ProfileName = mOptions.mProfileName;
    if (ProfileName == "grammar_jungle_maze" || ProfileName == "grammar_ice_maze") {
        mMapType = eMapType::Maze;
    }
    else if (ProfileName == "grammar_jungle_neck" || ProfileName == "grammar_ice_neck") {
        mMapType = eMapType::Neck;
    }
    else if (ProfileName == "grammar_beach") {
        mMapType = eMapType::Beach;
    }
    else if (ProfileName == "grammar_ice_skidoo_jump") {
        mMapType = eMapType::SkidooJump;
    }
    else {
        switch (mOptions.mProfile) {
        case eNetworkMapProfile_JungleMaze:
        case eNetworkMapProfile_IceMaze:
            mMapType = eMapType::Maze;
            break;
        case eNetworkMapProfile_JungleNeck:
        case eNetworkMapProfile_IceNeck:
            mMapType = eMapType::Neck;
            break;
        case eNetworkMapProfile_Beach:
            mMapType = eMapType::Beach;
            break;
        case eNetworkMapProfile_IceSkidooJump:
            mMapType = eMapType::SkidooJump;
            break;
        default:
            if (mOptions.mMapTerrain == eNetworkMapTerrain_Jungle && mOptions.mMapTerrainSub == 1)
                mMapType = eMapType::Beach;
            break;
        }
    }

    // Terrain and type are the complete UI model. A missing/invalid terrain
    // opens at the default rather than exposing the old cross-terrain preset.
    if (mOptions.mMapTerrain != eNetworkMapTerrain_Jungle && mOptions.mMapTerrain != eNetworkMapTerrain_Ice)
        mOptions.mMapTerrain = eNetworkMapTerrain_Jungle;
    if (!MapTypeSupported(mMapType, mOptions.mMapTerrain))
        mMapType = eMapType::Random;

    ApplyTerrainAndType(false);
}

const char* cRandomMapOptionsMenu::GetTerrainName() const
{
    return Network_MapTerrainName(mOptions.mMapTerrain);
}

const char* cRandomMapOptionsMenu::GetMapTypeName() const
{
    switch (mMapType) {
    case eMapType::Maze:       return "MAZE";
    case eMapType::Neck:       return "NECK";
    case eMapType::Beach:      return "BEACH";
    case eMapType::SkidooJump: return "SKIDOO JUMP";
    default:                   return mContext == eContext::Multiplayer ? "BALANCED" : "RANDOM";
    }
}

bool cRandomMapOptionsMenu::MapTypeSupported(eMapType pType, eNetworkMapTerrain pTerrain) const
{
    if (pType == eMapType::Random)
        return true;
    if (mContext == eContext::Multiplayer)
        return pTerrain == eNetworkMapTerrain_Jungle && pType == eMapType::Beach;
    if (pTerrain == eNetworkMapTerrain_Jungle)
        return pType == eMapType::Maze || pType == eMapType::Neck || pType == eMapType::Beach;
    if (pTerrain == eNetworkMapTerrain_Ice)
        return pType == eMapType::Maze || pType == eMapType::Neck || pType == eMapType::SkidooJump;
    return false;
}

void cRandomMapOptionsMenu::CycleTerrain()
{
    const eNetworkMapTerrain Terrains[] = {
        eNetworkMapTerrain_Jungle,
        eNetworkMapTerrain_Ice,
    };
    const size_t TerrainCount = sizeof(Terrains) / sizeof(Terrains[0]);
    size_t Current = TerrainCount - 1;
    for (size_t Index = 0; Index < TerrainCount; ++Index) {
        if (Terrains[Index] == mOptions.mMapTerrain) {
            Current = Index;
            break;
        }
    }

    mOptions.mMapTerrain = Terrains[(Current + 1) % TerrainCount];
    if (!MapTypeSupported(mMapType, mOptions.mMapTerrain))
        mMapType = eMapType::Random;
    ApplyTerrainAndType(true);
}

void cRandomMapOptionsMenu::CycleMapType()
{
    const eMapType Types[] = {
        eMapType::Random,
        eMapType::Maze,
        eMapType::Neck,
        eMapType::Beach,
        eMapType::SkidooJump,
    };
    const size_t TypeCount = sizeof(Types) / sizeof(Types[0]);
    size_t Current = TypeCount - 1;
    for (size_t Index = 0; Index < TypeCount; ++Index) {
        if (Types[Index] == mMapType) {
            Current = Index;
            break;
        }
    }

    do {
        Current = (Current + 1) % TypeCount;
    } while (!MapTypeSupported(Types[Current], mOptions.mMapTerrain));

    mMapType = Types[Current];
    ApplyTerrainAndType(true);
}

void cRandomMapOptionsMenu::ApplyTerrainAndType(bool pApplyDefaults)
{
    ClearProfileName();
    mOptions.mMapTerrainSub = 0;
    mOptions.mProfile = eNetworkMapProfile_Custom;

    if (pApplyDefaults) {
        mOptions.mCoverDensity = eNetworkCoverDensity_Normal;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
    }

    if (mOptions.mMapTerrain == eNetworkMapTerrain_Jungle) {
        switch (mMapType) {
        case eMapType::Maze:
            mOptions.mProfile = eNetworkMapProfile_JungleMaze;
            mOptions.mProfileName = "grammar_jungle_maze";
            if (pApplyDefaults)
                mOptions.mCoverDensity = eNetworkCoverDensity_Dense;
            break;
        case eMapType::Neck:
            mOptions.mProfile = eNetworkMapProfile_JungleNeck;
            mOptions.mProfileName = "grammar_jungle_neck";
            if (pApplyDefaults)
                mOptions.mCoverDensity = eNetworkCoverDensity_Dense;
            break;
        case eMapType::Beach:
            mOptions.mProfile = eNetworkMapProfile_Beach;
            mOptions.mProfileName = "grammar_beach";
            mOptions.mMapTerrainSub = 1;
            break;
        default:
            mOptions.mProfile = eNetworkMapProfile_Jungle;
            break;
        }
    }
    else if (mOptions.mMapTerrain == eNetworkMapTerrain_Ice) {
        switch (mMapType) {
        case eMapType::Maze:
            mOptions.mProfile = eNetworkMapProfile_IceMaze;
            mOptions.mProfileName = "grammar_ice_maze";
            if (pApplyDefaults)
                mOptions.mCoverDensity = eNetworkCoverDensity_Dense;
            break;
        case eMapType::Neck:
            mOptions.mProfile = eNetworkMapProfile_IceNeck;
            mOptions.mProfileName = "grammar_ice_neck";
            break;
        case eMapType::SkidooJump:
            mOptions.mProfile = eNetworkMapProfile_IceSkidooJump;
            mOptions.mProfileName = "grammar_ice_skidoo_jump";
            if (pApplyDefaults)
                mOptions.mVehicleSet = eNetworkVehicleSet_Light;
            break;
        default:
            mOptions.mProfile = eNetworkMapProfile_Ice;
            break;
        }
    }
}

void cRandomMapOptionsMenu::ClearProfileName()
{
    mOptions.mProfileName.clear();
}

const char* cRandomMapOptionsMenu::GetModeName() const
{
    if (mContext == eContext::Campaign)
        return "CAMPAIGN";

    return Network_GameModeName(mOptions.mGameMode);
}

const char* cRandomMapOptionsMenu::GetModeShortName() const
{
    if (mContext == eContext::Campaign)
        return "CAMPAIGN";

    switch (mOptions.mGameMode) {
    case eNetworkGameMode_CoopCampaign:     return "COOP";
    case eNetworkGameMode_CoopRandom:       return "COOP RND";
    case eNetworkGameMode_Deathmatch:       return "DM";
    case eNetworkGameMode_SquadDeathmatch:  return "SQUAD DM";
    case eNetworkGameMode_RescuePrisoner:   return "RESCUE";
    case eNetworkGameMode_AvatarDeathmatch: return "AVATAR";
    case eNetworkGameMode_TeamAvatar:       return "TEAM";
    default:                                return "UNKNOWN";
    }
}

bool cRandomMapOptionsMenu::IsPvPMode() const
{
    return mContext == eContext::Multiplayer && Network_IsPvPMode(mOptions.mGameMode);
}
