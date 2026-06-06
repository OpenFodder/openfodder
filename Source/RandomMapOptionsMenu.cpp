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
    InferProfile();
}

void cRandomMapOptionsMenu::Tick()
{
    HandleTextInput();
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
        MarkProfileCustom();
        mOptions.mMapSize = static_cast<eNetworkMapSize>(((uint8_t)mOptions.mMapSize + 1) % eNetworkMapSize_Count);
        break;

    case ACT_CYCLE_MAP_TERRAIN:
        mEditField = eEditField::None;
        MarkProfileCustom();
        mOptions.mMapTerrain = static_cast<eNetworkMapTerrain>(((uint8_t)mOptions.mMapTerrain + 1) % eNetworkMapTerrain_Count);
        if (mOptions.mMapTerrain != eNetworkMapTerrain_Jungle)
            mOptions.mMapTerrainSub = 0;
        break;

    case ACT_CYCLE_COVER:
        mEditField = eEditField::None;
        MarkProfileCustom();
        mOptions.mCoverDensity = static_cast<eNetworkCoverDensity>(((uint8_t)mOptions.mCoverDensity + 1) % eNetworkCoverDensity_Count);
        break;

    case ACT_CYCLE_VEHICLES:
        mEditField = eEditField::None;
        MarkProfileCustom();
        mOptions.mVehicleSet = static_cast<eNetworkVehicleSet>(((uint8_t)mOptions.mVehicleSet + 1) % eNetworkVehicleSet_Count);
        break;

    case ACT_CYCLE_PICKUPS:
        mEditField = eEditField::None;
        MarkProfileCustom();
        mOptions.mPickupDensity = static_cast<eNetworkPickupDensity>(((uint8_t)mOptions.mPickupDensity + 1) % eNetworkPickupDensity_Count);
        break;

    case ACT_CYCLE_PROFILE:
        mEditField = eEditField::None;
        CycleProfile();
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
    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("RANDOM MAP", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    const size_t YOffset = PLATFORM_BASED(0, 25);
    DrawTabs(0x2C);

    int16 rowY = 0x43;
    const int16 rowH = 0x10;

    g_Fodder->Briefing_DrawBox(0x1E, 0x3F, 0x10C, 0x63, 0xF3);
    g_Fodder->Briefing_DrawBox(0x1D, 0x3E, 0x10C, 0x63, 0xF2);

    switch (mTab) {
    case eTab::Main:
        DrawValueButton("PROFILE", GetProfileName(), rowY, ACT_CYCLE_PROFILE);
        rowY += rowH;
        DrawField("SEED", mSeedText, rowY, ACT_EDIT_SEED, mEditField == eEditField::Seed);
        rowY += rowH;
        DrawValueButton("NEW SEED", "RANDOM", rowY, ACT_RANDOMIZE_SEED);
        rowY += rowH;
        DrawValueButton("SIZE", Network_MapSizeName(mOptions.mMapSize), rowY, ACT_CYCLE_MAP_SIZE);
        rowY += rowH;
        DrawValueButton("TERRAIN", GetTerrainName(), rowY, ACT_CYCLE_MAP_TERRAIN);
        rowY += rowH;
        DrawValueButton("COVER", Network_CoverDensityName(mOptions.mCoverDensity), rowY, ACT_CYCLE_COVER);
        break;

    case eTab::Support:
        DrawValueButton("VEHICLES", Network_VehicleSetName(mOptions.mVehicleSet), rowY, ACT_CYCLE_VEHICLES);
        rowY += rowH;
        DrawValueButton("PICKUPS", Network_PickupDensityName(mOptions.mPickupDensity), rowY, ACT_CYCLE_PICKUPS);
        rowY += rowH;
        if (mContext == eContext::Multiplayer)
            DrawValueButton("MATCH", GetModeName(), rowY, ACT_CYCLE_MODE);
        else
            DrawStaticValue("OBJECTIVE", "CAMPAIGN", rowY);
        rowY += rowH;
        DrawStaticValue("QUALITY", "VALIDATED", rowY);
        break;
    }

    DrawFooter();

    if (mContext == eContext::Campaign) {
        g_Fodder->GUI_Button_Draw_Small("START", 0x9E + YOffset, CanStart() ? 0xB2 : 0xF2, CanStart() ? 0xB3 : 0xF3);
        g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_START);
    }

    g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + YOffset);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, ACT_BACK);
}

void cRandomMapOptionsMenu::DrawTabs(int16 pY)
{
    const size_t xRowL = 0x20;
    const size_t xRowR = 0x128;
    const size_t tabGap = 4;
    const size_t tabCount = 2;
    const size_t totalW = xRowR - xRowL + 1;
    const size_t tabW = (totalW - (tabGap * (tabCount - 1))) / tabCount;

    DrawTab("MAIN", ACT_TAB_MAIN, mTab == eTab::Main, xRowL + (0 * (tabW + tabGap)), xRowL + (0 * (tabW + tabGap)) + tabW - 1, pY);
    DrawTab("SUPPORT", ACT_TAB_SUPPORT, mTab == eTab::Support, xRowL + (1 * (tabW + tabGap)), xRowL + (1 * (tabW + tabGap)) + tabW - 1, pY);
}

void cRandomMapOptionsMenu::DrawTab(const char* pLabel, int16 pAction, bool pActive, size_t pX1, size_t pX2, int16 pY)
{
    g_Fodder->GUI_Button_Draw_SmallBoxAt(
        pLabel,
        pX1,
        pX2,
        pY,
        pActive ? 0xF2 : 0xB2,
        pActive ? 0xF3 : 0xB3,
        eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, pAction);
}

void cRandomMapOptionsMenu::DrawValueButton(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction)
{
    const size_t FieldX1 = 0x88;
    const size_t FieldX2 = 0x128;
    const std::string FittedValue = NetworkMenu_FitText(pValue, (int)(FieldX2 - FieldX1 - 4));

    g_Fodder->String_Print_Small(pLabel, 0x20, pY);
    g_Fodder->String_Print_Small_LeftInBox(FittedValue, FieldX1, FieldX2, pY, 2);

    g_Fodder->mGUI_Temp_X = (int16)FieldX1;
    g_Fodder->mGUI_Temp_Y = pY;
    g_Fodder->mGUI_Temp_Width = (int16)(FieldX2 - FieldX1);
    if (!g_Fodder->mGUI_Draw_LastHeight)
        g_Fodder->mGUI_Draw_LastHeight = 6;

    g_Fodder->GUI_Box_Draw(0xB2, 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, pAction);
}

void cRandomMapOptionsMenu::DrawField(const char* pLabel, const std::string& pValue, int16 pY, int16 pAction, bool pActive)
{
    const size_t FieldX1 = 0x88;
    const size_t FieldX2 = 0x128;
    const std::string Value = pValue.size() ? pValue : "ENTER";
    const std::string FittedValue = NetworkMenu_FitText(Value, (int)(FieldX2 - FieldX1 - 4));

    g_Fodder->String_Print_Small(pLabel, 0x20, pY);
    g_Fodder->String_Print_Small_LeftInBox(FittedValue, FieldX1, FieldX2, pY, 2);

    g_Fodder->mGUI_Temp_X = (int16)FieldX1;
    g_Fodder->mGUI_Temp_Y = pY;
    g_Fodder->mGUI_Temp_Width = (int16)(FieldX2 - FieldX1);
    if (!g_Fodder->mGUI_Draw_LastHeight)
        g_Fodder->mGUI_Draw_LastHeight = 6;

    g_Fodder->GUI_Box_Draw(pActive ? 0xF2 : 0xB2, pActive ? 0xF3 : 0xB3);
    g_Fodder->GUI_Button_Setup_New(OnButtonClick, this, pAction);
}

void cRandomMapOptionsMenu::DrawStaticValue(const char* pLabel, const std::string& pValue, int16 pY)
{
    const size_t FieldX1 = 0x88;
    const size_t FieldX2 = 0x128;
    const std::string FittedValue = NetworkMenu_FitText(pValue, (int)(FieldX2 - FieldX1 - 4));

    g_Fodder->String_Print_Small(pLabel, 0x20, pY);
    g_Fodder->String_Print_Small_LeftInBox(FittedValue, FieldX1, FieldX2, pY, 2);
}

void cRandomMapOptionsMenu::DrawFooter()
{
    std::string Summary = "SEED ";
    Summary += mSeedText;
    Summary += " ";
    Summary += Network_MapSizeName(mOptions.mMapSize);
    Summary += " ";
    Summary += GetTerrainName();
    Summary += " ";
    Summary += GetModeShortName();

    Summary = NetworkMenu_FitText(Summary, 246);
    g_Fodder->String_Print_Small_LeftInBox(Summary, 0x20, 0x128, 0xA7, 0);
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

// Display-only: if there is no explicit script profile name, reverse-infer the
// named menu profile from the small option set the menu actually owns.
void cRandomMapOptionsMenu::InferProfile()
{
    mOptions.mProfile = Network_NormalizeMapProfile((uint8_t)mOptions.mProfile);
    if (!mOptions.mProfileName.empty()) {
        mProfile = mOptions.mProfile;
        return;
    }

    if (mOptions.mProfile != eNetworkMapProfile_Custom) {
        mProfile = mOptions.mProfile;
        return;
    }

    mProfile = eNetworkMapProfile_Custom;
    if (mOptions.mMapSize == eNetworkMapSize_Medium
        && mOptions.mMapTerrain == eNetworkMapTerrain_Jungle
        && mOptions.mMapTerrainSub == 0
        && mOptions.mVehicleSet == eNetworkVehicleSet_None
        && mOptions.mPickupDensity == eNetworkPickupDensity_Normal
        && mOptions.mCoverDensity == eNetworkCoverDensity_Normal) {
        mProfile = eNetworkMapProfile_Jungle;
    }
    else if (mOptions.mMapSize == eNetworkMapSize_Medium
        && mOptions.mMapTerrain == eNetworkMapTerrain_Jungle
        && mOptions.mMapTerrainSub == 1
        && mOptions.mVehicleSet == eNetworkVehicleSet_None
        && mOptions.mPickupDensity == eNetworkPickupDensity_Normal
        && mOptions.mCoverDensity == eNetworkCoverDensity_Normal) {
        mProfile = eNetworkMapProfile_Beach;
    }
    else if (mOptions.mMapSize == eNetworkMapSize_Medium
        && mOptions.mMapTerrain == eNetworkMapTerrain_Ice
        && mOptions.mVehicleSet == eNetworkVehicleSet_None
        && mOptions.mPickupDensity == eNetworkPickupDensity_Normal
        && mOptions.mCoverDensity == eNetworkCoverDensity_Normal) {
        mProfile = eNetworkMapProfile_Ice;
    }
    else if (mOptions.mMapSize == eNetworkMapSize_Medium
        && mOptions.mMapTerrain == eNetworkMapTerrain_Random
        && mOptions.mVehicleSet == eNetworkVehicleSet_None
        && mOptions.mPickupDensity == eNetworkPickupDensity_Normal
        && mOptions.mCoverDensity == eNetworkCoverDensity_Normal) {
        mProfile = eNetworkMapProfile_Random;
    }

    mOptions.mProfile = mProfile;
}

std::string cRandomMapOptionsMenu::GetProfileName() const
{
    if (!mOptions.mProfileName.empty() && mProfile == eNetworkMapProfile_Custom)
        return mOptions.mProfileName;

    return Network_MapProfileName(mProfile);
}

const char* cRandomMapOptionsMenu::GetTerrainName() const
{
    if (mOptions.mMapTerrain == eNetworkMapTerrain_Jungle && mOptions.mMapTerrainSub == 1)
        return "BEACH";

    return Network_MapTerrainName(mOptions.mMapTerrain);
}

void cRandomMapOptionsMenu::ApplyProfile()
{
    mOptions.mProfile = mProfile;
    ClearProfileName();

    switch (mProfile) {
    case eNetworkMapProfile_Jungle:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Jungle;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Normal;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        break;

    case eNetworkMapProfile_JungleMaze:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Jungle;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Dense;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        mOptions.mProfileName = "grammar_jungle_maze";
        break;

    case eNetworkMapProfile_JungleNeck:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Jungle;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Dense;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        mOptions.mProfileName = "grammar_jungle_neck";
        break;

    case eNetworkMapProfile_Beach:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Jungle;
        mOptions.mMapTerrainSub = 1;
        mOptions.mCoverDensity = eNetworkCoverDensity_Normal;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        break;

    case eNetworkMapProfile_Ice:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Ice;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Normal;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        break;

    case eNetworkMapProfile_IceMaze:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Ice;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Dense;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        mOptions.mProfileName = "grammar_ice_maze";
        break;

    case eNetworkMapProfile_IceMazeXL:
        mOptions.mMapSize = eNetworkMapSize_ExtraLarge;
        mOptions.mMapTerrain = eNetworkMapTerrain_Ice;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Dense;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        mOptions.mProfileName = "grammar_ice_maze_xl";
        break;

    case eNetworkMapProfile_IceNeck:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Ice;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Normal;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        mOptions.mProfileName = "grammar_ice_neck";
        break;

    case eNetworkMapProfile_IceSkidooJump:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Ice;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Normal;
        mOptions.mVehicleSet = eNetworkVehicleSet_Light;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        mOptions.mProfileName = "grammar_ice_skidoo_jump";
        break;

    case eNetworkMapProfile_Random:
        mOptions.mMapSize = eNetworkMapSize_Medium;
        mOptions.mMapTerrain = eNetworkMapTerrain_Random;
        mOptions.mMapTerrainSub = 0;
        mOptions.mCoverDensity = eNetworkCoverDensity_Normal;
        mOptions.mVehicleSet = eNetworkVehicleSet_None;
        mOptions.mPickupDensity = eNetworkPickupDensity_Normal;
        break;

    case eNetworkMapProfile_Custom:
    default:
        break;
    }
}

void cRandomMapOptionsMenu::CycleProfile()
{
    const eNetworkMapProfile Profiles[] = {
        eNetworkMapProfile_Jungle,
        eNetworkMapProfile_JungleMaze,
        eNetworkMapProfile_JungleNeck,
        eNetworkMapProfile_Beach,
        eNetworkMapProfile_Ice,
        eNetworkMapProfile_IceMaze,
        eNetworkMapProfile_IceMazeXL,
        eNetworkMapProfile_IceNeck,
        eNetworkMapProfile_IceSkidooJump,
        eNetworkMapProfile_Random,
    };
    const size_t ProfileCount = sizeof(Profiles) / sizeof(Profiles[0]);
    size_t Current = ProfileCount - 1;

    for (size_t Index = 0; Index < ProfileCount; ++Index) {
        if (Profiles[Index] == mProfile) {
            Current = Index;
            break;
        }
    }

    mProfile = Profiles[(Current + 1) % ProfileCount];
    ApplyProfile();
}

void cRandomMapOptionsMenu::MarkProfileCustom()
{
    mProfile = eNetworkMapProfile_Custom;
    mOptions.mProfile = eNetworkMapProfile_Custom;
    ClearProfileName();
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
