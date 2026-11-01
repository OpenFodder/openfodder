/*
 * Open Fodder - main menu and background scene.
 * Copyright (C) 2026 Open Fodder
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "stdafx.hpp"
#include "Network/NetworkMenuText.hpp"

namespace {
enum MenuAction : int16 {
    Campaigns, RandomMap, SingleMap, Multiplayer, Options, Help, Quit,
    Back, Keys, AboutScreen, PickCampaign, PreviousPage, NextPage
};

// The gameplay viewport starts three cells before the map's first column to
// make room for the sidebar. Menus have no sidebar; after the surface's
// one-cell border is cropped, their first visible cell must be map column 0.
constexpr int kMenuMapStartColumn = 3;
constexpr int kMenuMapStartOffset = kMenuMapStartColumn * sizeof(uint16);
constexpr int kMenuJungleShiftColumns = 4;

}

void cFodder::MainMenu_Button(const std::string& pText, int16 pX, int16 pY,
                            int16 pWidth, int16 pAction, int16 pArg) {
    const bool Available = (pAction != RandomMap && pAction != SingleMap) ||
        mVersions->isCampaignAvailable(pAction == RandomMap ? "Random Map" : "Single Map");
    const bool Hover = Available && mMouseX + 0x20 >= pX && mMouseX + 0x20 <= pX + pWidth &&
                       mMouseY >= pY && mMouseY <= pY + 15;
    if (!Available) {
        const int Width = Sidebar_Menu_MeasureWidth(pText);
        Sidebar_Menu_Print(pText, pX + (pWidth - Width) / 2, pY + 5, eSidebarMenuColor::Inactive);
        return;
    }
    const std::string Text = NetworkMenu_FitText(pText, pWidth - 12);
    String_Print_Small_CentreInBox(Text, pX, pX + pWidth, pY + 5);
    mGUI_Temp_X = pX + 3;
    mGUI_Temp_Y = pY;
    mGUI_Temp_Width = pWidth - 4;
    mGUI_Draw_LastHeight = 10;
    if (Hover)
        Briefing_DrawBox(pX, pY, pWidth, 16, 0xF3);
    GUI_Button_Setup_New([](void* ctx, int16 action, int16 arg) {
        static_cast<cFodder*>(ctx)->MainMenu_OnAction(action, arg);
    }, this, pAction, pArg);
}

void cFodder::MainMenu_OnAction(int16 pAction, int16 pArg) {
    switch (pAction) {
    case Campaigns:
        mMainMenuPage = eMainMenuPage::Campaigns;
        mGUI_Select_File_CurrentIndex = 0;
        break;
    case Help: mMainMenuPage = eMainMenuPage::Help; break;
    case Back:
        mMainMenuPage = mMainMenuPage == eMainMenuPage::Keys ? eMainMenuPage::Help : eMainMenuPage::Home;
        break;
    case RandomMap:
    case SingleMap:
        // These modes were previously exposed only when their backing data
        // version was installed. Keep that availability rule after splitting
        // the main menu from the campaign list.
        if (!mVersions->isCampaignAvailable(pAction == RandomMap ? "Random Map" : "Single Map"))
            break;
        mMainMenuSelection = pAction == RandomMap ? "Random Map" : "Single Map";
        mGUI_SaveLoadAction = 2;
        break;
    case Multiplayer: GUI_Button_Show_Multiplayer(); break;
    case Options: GUI_Button_Show_Options(); break;
    case Keys: mMainMenuPage = eMainMenuPage::Keys; break;
    case AboutScreen: GUI_Button_Show_About(); break;
    case Quit: GUI_Button_Load_Exit(); break;
    case PickCampaign:
        if (pArg >= 0 && pArg < mGUI_Select_File_Count) {
            mMainMenuSelection = mCampaignList[pArg];
            mGUI_SaveLoadAction = 2;
        }
        break;
    case PreviousPage: GUI_Button_Load_Up(); break;
    case NextPage: GUI_Button_Load_Down(); break;
    }
    // Mouse_Button_Left_Toggled consumes the press until the button is released.
}

void cFodder::MainMenu_Draw() {
    const int16 ExtraHeight = PLATFORM_BASED(0, 25);
    mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    GUI_Element_Reset();

    // Draw the menu into a transparent mask, then outline the lettering over
    // the live scene. No backing panel: the map fills the entire screen.
    auto* Pixels = mSurface->GetSurfaceBuffer();
    const size_t Size = mSurface->GetSurfaceBufferSize();
    const size_t Stride = mSurface->GetWidth();
    const std::vector<uint8> Background(Pixels, Pixels + Size);
    constexpr uint8 Transparent = 0xFF; // not a display palette entry
    std::fill(Pixels, Pixels + Size, Transparent);
    const bool Picker = mMainMenuPage == eMainMenuPage::Campaigns;

    mString_GapCharID = 0;
    String_Print_Large(Picker ? "CAMPAIGNS" :
                       mMainMenuPage == eMainMenuPage::Help ? "HELP" :
                       mMainMenuPage == eMainMenuPage::Keys ? "CONTROLS" : "OPEN FODDER", false, 19);

    if (mMainMenuPage == eMainMenuPage::Home) {
        Sidebar_Menu_Print("CHOOSE YOUR MISSION", 122, 42, eSidebarMenuColor::Inactive);
        int16 Y = 57 + ExtraHeight / 2;
        MainMenu_Button("CAMPAIGNS", 82, Y, 156, Campaigns); Y += 25;
        MainMenu_Button("RANDOM MAP", 82, Y, 156, RandomMap); Y += 25;
        MainMenu_Button("SINGLE MAP", 82, Y, 156, SingleMap); Y += 25;
#ifdef OPENFODDER_ENABLE_NETWORK
        MainMenu_Button("MULTIPLAYER", 82, Y, 156, Multiplayer);
#endif
        if (!mVersions->isCampaignAvailable("Random Map"))
            Sidebar_Menu_Print("MAP MODES REQUIRE RETAIL DATA", 104, 161 + ExtraHeight / 2,
                               eSidebarMenuColor::Inactive);
        MainMenu_Button("OPTIONS", 18, 175 + ExtraHeight, 82, Options);
        MainMenu_Button("HELP", 128, 175 + ExtraHeight, 64, Help);
        MainMenu_Button("QUIT", 238, 175 + ExtraHeight, 64, Quit);
    }
    else if (Picker) {
        Sidebar_Menu_Print("SELECT A CAMPAIGN", 126, 41, eSidebarMenuColor::Inactive);
        mGUI_Select_File_Count = (int16)mCampaignList.size();
        mGUI_Select_File_CurrentIndex = std::max<int16>(0, std::min<int16>(
            mGUI_Select_File_CurrentIndex,
            std::max<int16>(0, mGUI_Select_File_Count - mGUI_Select_File_ShownItems)));
        for (int16 Row = 0; Row < mGUI_Select_File_ShownItems; ++Row) {
            const int16 Index = mGUI_Select_File_CurrentIndex + Row;
            if (Index >= mGUI_Select_File_Count) break;
            MainMenu_Button(mCampaignList[Index], 16, 53 + Row * 20, 288, PickCampaign, Index);
        }
        if (mCampaignList.empty())
            String_Print_Small("NO CAMPAIGNS INSTALLED", 90);
        if (mGUI_Select_File_Count > mGUI_Select_File_ShownItems) {
            const int16 Y = 154 + ExtraHeight;
            if (mGUI_Select_File_CurrentIndex > 0)
                MainMenu_Button("UP", 16, Y, 54, PreviousPage);
            if (mGUI_Select_File_CurrentIndex + mGUI_Select_File_ShownItems < mGUI_Select_File_Count)
                MainMenu_Button("DOWN", 250, Y, 54, NextPage);
        }
        MainMenu_Button("BACK", 126, 174 + ExtraHeight, 68, Back);
    }
    else if (mMainMenuPage == eMainMenuPage::Keys) {
        Sidebar_Menu_Print("SYSTEM", 70, 42);
        Sidebar_Menu_Print("MISSION", 228, 42);
        const char* System[][2] = {
            {"F1", "AMIGA"}, {"F2", "PC"}, {"F11", "FULLSCREEN"},
            {"F12", "MOUSE LOCK"}, {"PLUS", "SIZE UP"}, {"MINUS", "SIZE DOWN"},
            {"ESC", "BACK ABORT"}
        };
        const char* Mission[][2] = {
            {"P", "PAUSE"}, {"M", "MAP"}, {"SPACE", "WEAPON"},
            {"1 2 3", "SQUAD"}, {"F5", "AUTOSAVE"}, {"F9", "LOAD INVINC"},
            {"F10", "WIN MISSION"}
        };
        for (int Row = 0; Row < 7; ++Row) {
            const int Y = 59 + Row * 16;
            String_Print_Small_Left(System[Row][0], 8, Y);
            String_Print_Small_Left(NetworkMenu_FitText(System[Row][1], 108), 48, Y);
            String_Print_Small_Left(Mission[Row][0], 166, Y);
            String_Print_Small_Left(NetworkMenu_FitText(Mission[Row][1], 108), 208, Y);
        }
        MainMenu_Button("BACK", 126, 175 + ExtraHeight, 68, Back);
    }
    else {
        MainMenu_Button("KEYS", 82, 67 + ExtraHeight / 2, 156, Keys);
        MainMenu_Button("ABOUT", 82, 97 + ExtraHeight / 2, 156, AboutScreen);
        MainMenu_Button("BACK", 126, 168 + ExtraHeight, 68, Back);
    }

    const std::vector<uint8> Foreground(Pixels, Pixels + Size);
    std::copy(Background.begin(), Background.end(), Pixels);
    for (size_t Y = 1; Y + 1 < mSurface->GetHeight(); ++Y) {
        for (size_t X = 1; X + 1 < Stride; ++X) {
            const size_t Index = Y * Stride + X;
            if (Foreground[Index] == Transparent) continue;
            for (int DY = -1; DY <= 1; ++DY)
                for (int DX = -1; DX <= 1; ++DX)
                    Pixels[(Y + DY) * Stride + X + DX] = 0;
        }
    }
    for (size_t Index = 0; Index < Size; ++Index)
        if (Foreground[Index] != Transparent)
            Pixels[Index] = Foreground[Index];
}

// Campaign Selection screen tile data
static const std::vector<unsigned char> kMainMenuMap_Jungle = {
    0x18, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0xd1, 0x00, 0x6a, 0x01, 0x30, 0x01, 0x6b, 0x01, 0x8b,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x08, 0x01, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x4d, 0x01, 0x4e, 0x01, 0x7c, 0x00, 0x19, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01,
    0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1b, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x10, 0x10, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x60, 0x01, 0x61, 0x01, 0x62, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0xf3, 0x00, 0x5a, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1b,
    0x01, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x74, 0x01, 0x75, 0x01, 0x76, 0x01, 0x77, 0x01, 0x7b, 0x00, 0x0a, 0x01, 0x5a, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1a, 0x01, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00,
    0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x88, 0x01, 0x89, 0x01, 0x8a, 0x01, 0x10, 0x10, 0x00, 0x00, 0x32, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf4, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b,
    0x00, 0x7b, 0x00, 0x7b, 0x00, 0xf0, 0x00, 0x05, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x06, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0xf1, 0x00, 0x8b, 0x01,
    0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf2, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x05, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7c, 0x00, 0x7c, 0x00, 0x7b, 0x00, 0xf6, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x10, 0x10, 0xf1, 0x00, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x06, 0x01,
    0x7c, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x8c, 0x00, 0x0a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b,
    0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x6f, 0x01, 0x0a, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x1d, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x10, 0x10, 0x7b, 0x00, 0xf0, 0x00,
    0x05, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0xf2, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x36, 0x01, 0x37, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x68, 0x01, 0x69, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x46,
    0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x6c, 0x01, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x69, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x5a, 0x01, 0x8b, 0x01, 0x46, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x8b, 0x01, 0x08, 0x01,
    0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00, 0x7b, 0x00
};

static const std::vector<unsigned char> kMainMenuMap_AF = {
    0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5b, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x3d, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72,
    0x00, 0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x29, 0x00, 0x14, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x83, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x72, 0x00, 0x71, 0x00, 0x14, 0x00, 0x14, 0x00, 0x9a, 0x00, 0x9b, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5d, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x0c, 
    0x00, 0xae, 0x00, 0xaf, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5c, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x14, 0x00, 0x0b, 0x00, 0xc2, 0x00, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5c, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x28, 0x00, 0x14, 0x00, 0x14, 0x00, 0x07, 0x00, 0x07, 0x00, 0x04, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 
    0x00, 0x14, 0x00, 0x70, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x94, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x97, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x56, 0x00, 0x3d, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x52, 0x00, 0x53, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 
    0x00, 0x59, 0x00, 0x14, 0x10, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x81, 0x00, 0x14, 0x10, 0x14, 0x00, 
    0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x5a, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x50, 
    0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x73, 0x00, 0x73, 0x00, 0x73, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x80, 0x00, 0x14, 0x00, 0x0c, 0x00, 0x11, 0x00, 0x11, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x14, 0x00, 0x82, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 
    0x72, 0x00, 0x73, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x71, 0x00, 0x14, 0x00, 0x18, 0x00, 0x23, 0x00, 0x24, 0x00, 0x16, 0x00, 0x14, 0x00, 0x14, 0x00, 0x96, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 
    0x00, 0x72, 0x00, 0x72, 0x00, 0x72, 0x00, 0x94, 0x00, 0x14, 0x00, 0x0b, 0x00, 0x07, 0x00, 0x07, 0x00, 0x04, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x64, 0x00, 0x64, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
    0x10, 0x10, 0x0b, 0x00, 0x10, 0x10, 0x07, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
};

void cFodder::MainMenu_CreateBackground() {
    // Select only from the active release. Magazine demos may contain just
    // interior, snow, or jungle sub1; asking Map_Create for another set can
    // otherwise switch data versions or open the missing-data screen.
    const auto Sets = mVersionCurrent->getTileTypes();
    if (Sets.empty()) return;
    auto Set = Sets.begin();
    for (auto It = Sets.begin(); It != Sets.end(); ++It) {
        if (It->Type == eTileTypes_Jungle) { Set = It; break; }
    }
    const auto Sub = Set->Subs.front();
    Map_Create(sMapParams(21, 15, Set->Type, Sub));
    mMapTile_Ptr += kMenuMapStartOffset;

    if (Set->Type == eTileTypes_AFX) {
        std::copy(kMainMenuMap_AF.begin(), kMainMenuMap_AF.end(), mMap->begin() + 0x60);
    }
    else if (Set->Type == eTileTypes_Jungle && Sub == eTileSub_0) {
        // Coastal outpost: move the original continuous shoreline four cells
        // east, giving the left edge a broad clearing for the barracks. Keep
        // shoreline pairs intact; sub0 art is never used with the Plus demo.
        for (size_t Y = 0; Y < 15; ++Y) {
            for (size_t X = 0; X < 21; ++X) {
                uint16 Tile = ((X + Y * 3) % 7 == 0) ? 124 : 123;
                if (X >= kMenuJungleShiftColumns) {
                    const size_t Offset = (Y * 21 + X - kMenuJungleShiftColumns) * 2;
                    Tile = readLE<uint16>(kMainMenuMap_Jungle.data() + Offset);
                    // The source scene includes a baked-in barracks at
                    // (0,2). Scenery preparation stamps our new barracks and
                    // its actors separately, so clear the old footprint.
                    if (X < 8 && Y >= 2 && Y <= 5 &&
                        (Tile == 333 || Tile == 334 ||
                         (Tile >= 352 && Tile <= 354) ||
                         (Tile >= 372 && Tile <= 375) ||
                         (Tile >= 392 && Tile <= 394)))
                        Tile = 123;
                }
                writeLEWord(mMap->data() + 0x60 + (Y * 21 + X) * 2, Tile);
            }
        }
        // Small ground details use shared base tiles, away from the central menu.
        const int Details[][3] = {{1, 1, 27}, {2, 2, 84}, {0, 10, 27}, {2, 12, 84}};
        for (const auto& Detail : Details)
            writeLEWord(mMap->data() + 0x60 + (Detail[1] * 21 + Detail[0]) * 2,
                        (uint16)Detail[2]);
    }
    else {
        // Safe base-only scene for restricted demos. Tile zero belongs to
        // every set; jungle's two plain grass tiles are shared by both subs.
        for (size_t Y = 0; Y < 15; ++Y)
            for (size_t X = 0; X < 21; ++X)
                writeLEWord(mMap->data() + 0x60 + (Y * 21 + X) * 2,
                    Set->Type == eTileTypes_Jungle ? uint16(123 + ((X + Y) % 7 == 0)) : 0);
    }
}

void cFodder::MainMenu_Setup() {
    mCampaignList.clear();

    mPhase_In_Progress = true;
    mPhase_Aborted = false;
    mGUI_SaveLoadAction = 0;

    for (auto& Name : mVersions->GetCampaignNames()) {
        if (Name != "Single Map" && Name != "Random Map" &&
            g_ResourceMan->isCampaignAvailable(Name))
            mCampaignList.push_back(Name);
    }

    {
        auto Files = g_ResourceMan->GetCampaigns();

        // Append all custom campaigns to the list
        for (auto& File : Files) {
            size_t Pos = File.find_first_of(".");
            std::string FileName = File.substr(0, Pos);

            // Don't add known campaigns
            if (mVersions->isCampaignKnown(FileName))
                continue;

            mCampaignList.push_back(FileName);
        }
    }

    mGUI_Select_File_CurrentIndex = 0;
    mGUI_Select_File_Count = (int16)mCampaignList.size();
    mGUI_Select_File_ShownItems = PLATFORM_BASED(5, 6);
    mGUI_Select_File_SelectedFileIndex = 0;

    MainMenu_CreateBackground();
    MainMenu_PrepareScenery();
    Music_Play(mMapLoaded->getTileType() + 0x32, 4);
    mSound->Music_SetVolume(-1, 0x10);

    if (mGUI_SaveLoadAction != 3) {
        mSurface->palette_FadeTowardNew();
        Mouse_Setup();
    }
    mGUI_SaveLoadAction = 0;

    mGraphics->PaletteSet();
    mSurface->Save();

    mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    mDemo_ExitMenu = 0;

    Camera_Reset();

}

void cFodder::MainMenu_PrepareScenery() {

    int16 x = 0;

    Sprite_Clear_All();

    Phase_SquadPrepare();

    // Restricted magazine releases may not have the jungle/AFX scenery or
    // matching sprite sheets. Their base-only background needs no actors.
    if (mMapLoaded->getTileType() != eTileTypes_Jungle &&
        mMapLoaded->getTileType() != eTileTypes_AFX)
        return;

    mSquad_CurrentVehicle = &mSprites[x];
    // Keep the rotating turret on its baked-in foundation: the full-screen
    // viewport removes the sidebar offset, and the jungle map is shifted east.
    const bool Jungle = mMapLoaded->getTileType() == eTileTypes_Jungle;
    const bool HasTurretBase = !Jungle || mMapLoaded->getTileSub() == eTileSub_0;
    const int SceneryShift = (Jungle ? kMenuJungleShiftColumns : 0) - kMenuMapStartColumn;
    mSprites[x].mPosX = 0xe6 + SceneryShift * 16;
    mSprites[x].mPosY = 0xcd;
    mSprites[x].mSheetIndex = 0xD2;
    mSprites[x].mFrameIndex = 5;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x].mSpriteType = HasTurretBase ? eSprite_Turret_Missile_Human : eSprite_Null;
    mSprites[x++].mVehicleType = eVehicle_Turret_Missile;

    mSprites[x].mPosX = tool_RandomGet() & 0xFF;
    mSprites[x].mPosY = tool_RandomGet() & 0xff;
    mSprites[x].mSheetIndex = 6;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Civilian_Spear;

    mSprites[x].mPosX = 0xff;
    mSprites[x].mPosY = 16 + (tool_RandomGet() % 0x60);
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Bird_Left;

    mSprites[x].mPosX = 0;
    mSprites[x].mPosY = 16 + (tool_RandomGet() % 0xc0);
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Bird_Left;

    mSprites[x].mPosX = 185;
    mSprites[x].mPosY = 19;
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Hostage;

    mSprites[x].mPosX = 100;
    mSprites[x].mPosY = 0xd0;
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Floating_Dead_Soldier;

    mSprites[x].mPosX = 220;
    mSprites[x].mPosY = 60;
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_BoilingPot;

    mSprites[x].mPosX = 220;
    mSprites[x].mPosY = 60;
    mSprites[x].mSheetIndex = 2;
    mSprites[x].mFrameIndex = 0;
    mSprites[x].mRowsToSkip = 0;
    mSprites[x].mHeight = 0;
    mSprites[x++].mSpriteType = eSprite_Null;

    mSquad_Leader = mSquad_CurrentVehicle;

    word_3AA1D = word_3BED5[0];

    mSoundDisabled = true;
    if(mMapLoaded->getTileType() == eTileTypes_Jungle && mMapLoaded->getTileSub() == eTileSub_0)
        Map_Add_Structure(mStructuresBarracksWithSoldier[mMapLoaded->getTileType()], 4, 4);

    if(mMapLoaded->getTileType() == eTileTypes_AFX)
        Map_Add_Structure(mStructuresBarracksWithSoldier[mMapLoaded->getTileType()], 2, 5);
    mSoundDisabled = false;
}

std::string cFodder::MainMenu_Run() {

    mMainMenuPage = eMainMenuPage::Home;
    mMainMenuSelection.clear();
    MainMenu_Setup();
    MapTiles_ResetScrollState();
    mMapTile_Ptr += kMenuMapStartOffset;

    mInterruptCallback = [this]() {
        if (!mStartParams->mDisableVideo) {
            mGraphics->MapTiles_Draw();
        }
        Sprites_Draw();
        MainMenu_Draw();
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);

        Mouse_DrawCursor();
    };

    bool SelectionCompleted = false;
#ifdef OPENFODDER_ENABLE_NETWORK
    if (auto* mp = dynamic_cast<cFodderMultiplayer*>(this)) {
        if (mp->ConsumeReturnToMultiplayerLobby()) {
            if (mp->Multiplayer_ReopenLobby()) {
                mGUI_SaveLoadAction = 2;
                SelectionCompleted = true;
            }
            else {
                mGUI_SaveLoadAction = 6;
            }
        }

        if (!SelectionCompleted && mStartParams->mNetworkMenuStart != eNetworkMenuStart_None) {
            if (mp->Multiplayer_Menu_Run()) {
                if (mp->mLobby) {
                    mp->Lobby_CampaignSelection();

                    if (mStartParams->mNetworkEnabled) {
                        mGUI_SaveLoadAction = 2;
                        SelectionCompleted = true;
                    }
                }
                else {
                    mGUI_SaveLoadAction = 0;
                    mMouse_Button_Left_Toggle = 0;
                    mGraphics->PaletteSet();
                    mSurface->palette_FadeTowardNew();
                    mSurface->Save();
                }
            }

            if (!SelectionCompleted) {
                mStartParams->mNetworkEnabled = false;
                mGUI_SaveLoadAction = 0;
                mMouse_Button_Left_Toggle = 0;
                mGraphics->PaletteSet();
                mSurface->palette_FadeTowardNew();
                mSurface->Save();
            }
        }
    }
#endif

    while (!SelectionCompleted) {

        MainMenu_Loop();

        if (mGUI_SaveLoadAction != 3)
            break;
        mGUI_SaveLoadAction = 0;
    }

    mInterruptCallback = nullptr;
    mPhase_In_Progress = false;

    if (mGUI_SaveLoadAction == 1)
        return "";

    if (!mMainMenuSelection.empty())
        return mMainMenuSelection;

    // Generated-map MP modes (Coop Random / PvP) don't pick a campaign — the
    // list may even be empty when no SP placeholders are visible. Return ""
    // and let cFodder::Campaign_Selection's exit branch take over (the
    // returned name is unused on the MP gameplay path; Mission_Loop reads
    // mNetworkGameMode directly to route to CreateRandom).
    const size_t pickIdx = (size_t)mGUI_Select_File_CurrentIndex + (size_t)mGUI_Select_File_SelectedFileIndex;
    if (pickIdx >= mCampaignList.size())
        return "";

    return mCampaignList[pickIdx];
}

void cFodder::MainMenu_Cycle() {

    if (mPhase_InterruptTicks >= 3) {
        mPhase_InterruptTicks = 0;
        Mission_Sprites_Handle();
        Sound_Tick();
        if(mInterruptCallback)
            mInterruptCallback();
    }

    if (mSurface->isPaletteAdjusting())
        mSurface->palette_FadeTowardNew();

    if (mPhase_Aborted || mPhase_EscapeKeyAbort) {
        mPhase_Aborted = false;
        mPhase_EscapeKeyAbort = false;
        if (mMainMenuPage == eMainMenuPage::Keys)
            mMainMenuPage = eMainMenuPage::Help;
        else if (mMainMenuPage != eMainMenuPage::Home)
            mMainMenuPage = eMainMenuPage::Home;
        else
            GUI_Button_Load_Exit();
        mMouse_Button_Left_Toggle = 0;
    }

    if (Mouse_Button_Left_Toggled() >= 0)
        GUI_Handle_Element_Mouse_Check(mGUI_Elements);

    if (mGUI_SaveLoadAction == 5 || mGUI_SaveLoadAction == 6) {
        // A modal must start with its own hit targets and a fresh press.
        // In particular the home footer can overlap an options action.
        GUI_Element_Reset();
        mMouse_EventLastButtonsPressed = 0;
        mMouseButtonStatus = 0;
        mMouse_Button_Left_Toggle = 0;
    }

    if (mGUI_SaveLoadAction == 5) {
        Options_Menu_Run();
        // Options may change the active platform and its available tiles.
        MainMenu_Setup();
        if (mInterruptCallback) mInterruptCallback();
    }

#ifdef OPENFODDER_ENABLE_NETWORK
    if (mGUI_SaveLoadAction == 6) {
        auto* mp = dynamic_cast<cFodderMultiplayer*>(this);
        if (mp && mp->Multiplayer_Menu_Run()) {

            if (mp->mLobby) {
                // Network mode with lobby — enter lobby campaign selection
                mp->Lobby_CampaignSelection();

                if (mStartParams->mNetworkEnabled) {
                    // Campaign selected, both players agreed — exit with selection
                    mGUI_SaveLoadAction = 2;
                    return;
                }
            } else {
                // Sync test mode — return to normal campaign selection
                // with network enabled, user picks campaign normally
                mGUI_SaveLoadAction = 0;
                mMouse_Button_Left_Toggle = 0;
                mGraphics->PaletteSet();
                mSurface->palette_FadeTowardNew();
                mSurface->Save();
                return;
            }
        }

        // Cancelled or failed — return to campaign selection
        mStartParams->mNetworkEnabled = false;
        mGUI_SaveLoadAction = 0;
        mMouse_Button_Left_Toggle = 0;
        mGraphics->PaletteSet();
        mSurface->palette_FadeTowardNew();
        mSurface->Save();
    }
#endif

    if (mMainMenuPage == eMainMenuPage::Campaigns)
        GUI_Button_Load_MouseWheel();
}

void cFodder::MainMenu_Loop() {

    do {
        MainMenu_Cycle();
        Video_Sleep();

    } while (mGUI_SaveLoadAction <= 0);

    mPhase_Aborted = false;

    if (mGUI_SaveLoadAction == 3)
        return;

    Image_FadeOut();
}
