/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#include "stdafx.hpp"
#include "Setup/SetupWizard.hpp"
#ifdef OPENFODDER_SETUP_DIAGNOSTICS
#include "Setup/DiskImageList.hpp"
#include "Setup/FileBrowser.hpp"
#include <filesystem>
#include <thread>
#include <atomic>

namespace Setup {
// Drive real modal loops through SDL's event queue, without a desktop window.
// Only compiled into the optional native diagnostic build.
static bool VerifyOptionsDataFlow(const std::string& image, const std::string& output) {
    auto& game = *g_Fodder;
    game.mStartParams->mMouseAlternative = true;
    game.mStartParams->mCheatsEnabled = false;
    game.mParams->mMouseAlternative = true;
    game.mParams->mCheatsEnabled = false;
    game.mStartParams->SaveIni();
    game.mGraphics->MapTiles_Draw();
    game.Sprites_Draw();
    cOptionsMenu preview;
    preview.Open(); preview.Draw();
    game.mSurface->palette_SetFromNew();
    game.mSurface->surfaceSetToPalette();
    game.mSurface->draw();
    game.mSurface->SavePng((std::filesystem::path(output) / "options-add-data.png").u8string());
    preview.Close();
    const auto scale = game.mWindow->GetScale();
    const int extra = game.mVersionCurrent->isAmiga() ? 25 : 0;
    SDL_unsetenv_unsafe("OPENFODDER_SETUP_PREVIEW_DIR");
    std::atomic<bool> finished{false};
    std::thread inputs([&]() {
        const auto click = [&](int x, int y) {
            SDL_Event event{};
            event.type = SDL_EVENT_MOUSE_MOTION;
            event.motion.x = float(x * scale.getWidth());
            event.motion.y = float((y - 4) * scale.getHeight());
            SDL_PushEvent(&event);
            SDL_Delay(100);
            event = {};
            event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
            event.button.button = SDL_BUTTON_LEFT;
            event.button.down = true;
            event.button.clicks = 1;
            event.button.x = float(x * scale.getWidth());
            event.button.y = float((y - 4) * scale.getHeight());
            SDL_PushEvent(&event);
            SDL_Delay(100);
            event.type = SDL_EVENT_MOUSE_BUTTON_UP;
            event.button.down = false;
            SDL_PushEvent(&event);
            SDL_Delay(500);
        };
        SDL_Delay(700);
        click(270, 44); // System tab.
        click(250, 71 + extra / 2 + 4 * 17 + 5); // Draft cheat setting.
        click(160, 162 + extra); // Add data.
        SDL_Delay(1700);
        SDL_Event key{};
        key.type = SDL_EVENT_KEY_DOWN; key.key.scancode = SDL_SCANCODE_ESCAPE;
        SDL_PushEvent(&key);
        SDL_Delay(100);
        key.type = SDL_EVENT_KEY_UP; SDL_PushEvent(&key);
        SDL_Delay(1000);
        click(160, 162 + extra); // Reopen from same options page.
        SDL_Delay(1700);
        SDL_Event drop{};
        drop.type = SDL_EVENT_DROP_FILE;
        drop.drop.data = SDL_strdup(image.c_str());
        SDL_PushEvent(&drop);
        SDL_Delay(2200);
        click(160, 110); // Add the detected game.
        SDL_Delay(1700);
        click(260, 180 + extra); // Cancel draft options, keep added data.
        SDL_Delay(700);
        if (!finished) {
            SDL_Event quit{}; quit.type = SDL_EVENT_QUIT; SDL_PushEvent(&quit);
        }
    });
    const bool applied = game.Options_Menu_Run();
    finished = true;
    inputs.join();
    if (game.mExit || applied || game.mStartParams->mCheatsEnabled) {
        std::cerr << "options check: exit=" << game.mExit << " applied=" << applied
                  << " cheats=" << game.mStartParams->mCheatsEnabled
                  << " mounts=" << g_ResourceMan->getMountedImagePaths().size() << "\n";
        return false;
    }
    const auto saved = g_ResourceMan->FileReadStr("openfodder.ini");
    return saved.find("cheats=false") != std::string::npos &&
           saved.find("image1=") != std::string::npos &&
           !g_ResourceMan->getMountedImagePaths().empty();
}

static bool VerifyMenuFamily(const std::string& output) {
    auto& game = *g_Fodder;
    game.mStartParams->mMouseAlternative = game.mParams->mMouseAlternative = true;
    const auto capture = [&](const char* name) {
        game.mSurface->palette_SetFromNew(); game.mSurface->surfaceSetToPalette(); game.mSurface->draw();
        return game.mSurface->SavePng(output + "/" + name + ".png");
    };
#ifdef OPENFODDER_ENABLE_NETWORK
    const auto originalStart = game.mStartParams->mNetworkMenuStart;
    game.mStartParams->mNetworkMenuStart = eNetworkMenuStart_Main;
    cMultiplayerMenu menu;
    menu.Open(); menu.Draw();
    if (!capture("multiplayer-main")) return false;
    menu.OnRowClick(cMultiplayerMenu::ACT_HOST, 0); menu.Draw();
    if (!capture("multiplayer-host")) return false;
    menu.OnRowClick(cMultiplayerMenu::ACT_CYCLE_MODE, 0); menu.Draw();
    if (!capture("multiplayer-host-random")) return false;
    menu.OnRowClick(cMultiplayerMenu::ACT_MAP_OPTIONS, 0); menu.Draw();
    if (!capture("multiplayer-map")) return false;
    menu.OnBack(); menu.Tick(); menu.OnBack();
    menu.OnRowClick(cMultiplayerMenu::ACT_DIRECT_CONNECT, 0); menu.Draw();
    if (!capture("multiplayer-direct")) return false;
    const auto before = menu.GetRemoteHost();
    menu.OnRowClick(cMultiplayerMenu::ACT_EDIT_REMOTE_HOST, 0);
    game.mKeyCode = SDL_SCANCODE_BACKSPACE; game.mInput_LastKey = 0; menu.Tick(); game.mKeyCode = 0;
    if (menu.GetRemoteHost() == before) return false;
    menu.Draw(); if (!capture("multiplayer-edit")) return false;
    menu.OnBack(); menu.OnBack();
    menu.OnRowClick(cMultiplayerMenu::ACT_FIND_LAN, 0); menu.Draw();
    if (!capture("multiplayer-lan")) return false;
    menu.OnBack(); menu.OnRowClick(cMultiplayerMenu::ACT_HOST_INTERNET, 0); menu.Draw();
    if (!capture("multiplayer-internet")) return false;
    menu.Close();
    auto* multiplayer = dynamic_cast<cFodderMultiplayer*>(&game);
    if (!multiplayer) return false;
    game.MainMenu_Setup();
    const auto liveScale = game.mWindow->GetScale();
    const int liveExtra = game.mVersionCurrent->isAmiga() ? 25 : 0;
    const auto livePress = [&](int x, int y) {
        SDL_Event event{}; event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
        event.button.button = SDL_BUTTON_LEFT; event.button.down = true; event.button.clicks = 1;
        event.button.x = float(x * liveScale.getWidth()); event.button.y = float((y - 4) * liveScale.getHeight());
        SDL_PushEvent(&event); SDL_Delay(100);
        event.type = SDL_EVENT_MOUSE_BUTTON_UP; event.button.down = false;
        SDL_PushEvent(&event); SDL_Delay(700);
    };
    std::atomic<bool> liveFinished{false};
    std::thread liveInputs([&]() {
        SDL_Delay(1000);
        livePress(160, 67 + liveExtra / 2); // Host LAN.
        livePress(70, 180 + liveExtra); // Back from host.
        livePress(70, 180 + liveExtra); // Back from multiplayer.
        SDL_Delay(500);
        if (!liveFinished) { SDL_Event event{}; event.type = SDL_EVENT_QUIT; SDL_PushEvent(&event); }
    });
    const bool started = multiplayer->Multiplayer_Menu_Run();
    liveFinished = true; liveInputs.join();
    if (started || game.mExit) return false;
    game.mStartParams->mNetworkEnabled = true;
    game.mStartParams->mNetworkInternet = false;
    game.mStartParams->mNetworkGameMode = eNetworkGameMode_CoopCampaign;
    multiplayer->mLobby = std::make_unique<cNetworkLobby>();
    if (!multiplayer->mLobby->Start(0, "127.0.0.1", 49152, true)) return false;
    liveFinished = false;
    std::thread lobbyInputs([&]() {
        SDL_Delay(1000);
        livePress(70, 180 + liveExtra);
        SDL_Delay(500);
        if (!liveFinished) { SDL_Event event{}; event.type = SDL_EVENT_QUIT; SDL_PushEvent(&event); }
    });
    multiplayer->Lobby_CampaignSelection();
    liveFinished = true; lobbyInputs.join();
    if (game.mExit || game.mStartParams->mNetworkEnabled) return false;
    multiplayer->mLobby.reset();
    game.mStartParams->mNetworkMenuStart = originalStart;
#endif
    // Drive the actual map picker with SDL clicks, including an empty list.
    SDL_setenv_unsafe("OPENFODDER_MENU_FAMILY_PREVIEW", output.c_str(), 1);
    const auto* retail = game.mVersions->GetRetail(game.mVersionCurrent->mPlatform, game.mVersionCurrent->mGame);
    if (!retail) return false;
    game.mPhase_In_Progress = false;
    game.VersionSwitch(retail);
    const int extra = retail->isAmiga() ? 25 : 0;
    const auto scale = game.mWindow->GetScale();
    const auto runPicker = [&](const std::vector<std::string>& maps) {
        std::atomic<bool> finished{false};
        std::thread inputs([&]() {
            const auto press = [&](int x, int y) {
                SDL_Event event{}; event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
                event.button.button = SDL_BUTTON_LEFT; event.button.down = true; event.button.clicks = 1;
                event.button.x = float(x * scale.getWidth()); event.button.y = float((y - 4) * scale.getHeight());
                SDL_PushEvent(&event); SDL_Delay(100);
                event.type = SDL_EVENT_MOUSE_BUTTON_UP; event.button.down = false;
                SDL_PushEvent(&event); SDL_Delay(700);
            };
            SDL_Delay(1000);
            if (!maps.empty()) {
                press(220, 160 + extra / 2); // Next page.
                press(160, 85 + extra / 2); // Select the second row on that page.
                press(230, 180 + extra); // Start.
            } else press(70, 180 + extra); // Back with no maps.
            SDL_Delay(500);
            if (!finished) { SDL_Event event{}; event.type = SDL_EVENT_QUIT; SDL_PushEvent(&event); }
        });
        const auto result = game.GUI_Select_File("SELECT MAP", {}, maps);
        finished = true; inputs.join();
        return result;
    };
    const std::vector<std::string> maps = {"first.map", "second.map", "third.map", "fourth.map",
        "fifth.map", "sixth.map", "mission.with.dots.map", "A very long mission name that needs fitting.map"};
    if (runPicker(maps) != maps[6] || game.mExit) return false;
    if (!runPicker({}).empty() || game.mExit || game.mGUI_SaveLoadAction != 1) return false;
    SDL_unsetenv_unsafe("OPENFODDER_MENU_FAMILY_PREVIEW");
    std::cout << "PASS: multiplayer pages, direct-address editing, single-map paging, selection, Start and empty-list Back\n";
    return true;
}

static bool VerifyRandomMenu(const std::string& output) {
    auto& game = *g_Fodder;
    sRandomMapOptions initial;
    initial.mSeed = 3156408432u;
    cRandomMapOptionsMenu menu;
    menu.Open(initial, cRandomMapOptionsMenu::eContext::Campaign);
    const int extra = game.mVersionCurrent->isAmiga() ? 25 : 0;
    const auto capture = [&](const char* name) {
        game.mGraphics->MapTiles_Draw(); game.Sprites_Draw(); menu.Draw();
        game.mSurface->palette_SetFromNew(); game.mSurface->surfaceSetToPalette(); game.mSurface->draw();
        return game.mSurface->SavePng((std::filesystem::path(output) / (std::string(name) + ".png")).u8string());
    };
    const auto click = [&](int x, int y) {
        game.mMouseX = x - 32; game.mMouseY = y;
        game.GUI_Handle_Element_Mouse_Check(game.mGUI_Elements);
    };
    if (!capture("random-map")) return false;
    click(190, 151 + extra / 2);
    game.mKeyCode = SDL_SCANCODE_BACKSPACE; game.mInput_LastKey = 0;
    menu.Tick(); game.mKeyCode = 0; menu.Tick();
    if (menu.GetOptions().mSeed != 315640843u || !capture("random-seed-edit")) return false;
    for (int i = 0; i < 9; ++i) {
        game.mKeyCode = SDL_SCANCODE_BACKSPACE; menu.Tick();
        game.mKeyCode = 0; menu.Tick();
    }
    if (!capture("random-empty-seed")) return false;
    click(230, 180 + extra);
    if (menu.IsDone()) return false; // Empty seed cannot start.
    click(284, 151 + extra / 2);
    if (!capture("random-new-seed")) return false;
    click(232, 44);
    if (!capture("random-support")) return false;
    menu.OnBack();
    if (menu.WasAccepted()) return false;
    auto specialized = initial;
    specialized.mProfileName = "grammar_jungle_maze";
    menu.Open(specialized, cRandomMapOptionsMenu::eContext::Campaign);
    if (!capture("random-maze-map")) return false;
    click(232, 44);
    if (!capture("random-maze-support")) return false;
    click(215, 97 + extra / 2);
    if (menu.GetOptions().mPickupDensity != specialized.mPickupDensity) return false;
    specialized.mMapTerrain = eNetworkMapTerrain_Ice;
    specialized.mProfileName = "grammar_ice_skidoo_jump";
    menu.Open(specialized, cRandomMapOptionsMenu::eContext::Campaign);
    capture("random-skidoo-map"); click(232, 44);
    if (!capture("random-skidoo-support")) return false;
    if (menu.GetOptions().mVehicleSet != specialized.mVehicleSet) return false;
    specialized.mMapTerrain = eNetworkMapTerrain_Desert;
    specialized.mProfileName.clear();
    menu.Open(specialized, cRandomMapOptionsMenu::eContext::Campaign);
    if (menu.GetOptions().mMapTerrain != eNetworkMapTerrain_Jungle) return false;
    capture("random-supported-terrain"); click(215, 79 + extra / 2);
    if (menu.GetOptions().mMapTerrain != eNetworkMapTerrain_Ice) return false;
    capture("random-supported-ice"); click(215, 79 + extra / 2);
    if (menu.GetOptions().mMapTerrain != eNetworkMapTerrain_Jungle) return false;

    // Exercise the real main-menu button, Back, reopen, and Start transition.
    game.mStartParams->mMouseAlternative = true;
    game.mParams->mMouseAlternative = true;
    const auto campaignScale = game.mWindow->GetScale();
    std::atomic<bool> campaignFinished{false};
    std::thread campaignInputs([&]() {
        const auto press = [&](int x, int y) {
            SDL_Event event{}; event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
            event.button.button = SDL_BUTTON_LEFT; event.button.down = true; event.button.clicks = 1;
            event.button.x = float(x * campaignScale.getWidth());
            event.button.y = float((y - 4) * campaignScale.getHeight());
            SDL_PushEvent(&event); SDL_Delay(100);
            event.type = SDL_EVENT_MOUSE_BUTTON_UP; event.button.down = false;
            SDL_PushEvent(&event); SDL_Delay(1000);
        };
        SDL_Delay(1000);
        press(160, 88 + extra / 2); // Random Map on the main menu.
        press(70, 180 + extra); // Back to the main menu.
        press(160, 88 + extra / 2); // Reopen Random Map.
        press(235, 180 + extra); // Start Mission.
        SDL_Delay(1000);
        if (!campaignFinished) { SDL_Event event{}; event.type = SDL_EVENT_QUIT; SDL_PushEvent(&event); }
    });
    game.Campaign_Selection();
    campaignFinished = true; campaignInputs.join();
    if (game.mExit || game.mCustom_Mode != eCustomMode_Map ||
        !game.mVersionCurrent->isCustom() || !game.mStartParams->mRandomMapOptionsEnabled) return false;
    std::cout << "PASS: main-menu Random Map button, Back, reopen and Start Mission\n";

    // Main-menu selection retains its map across the switch to Custom, but
    // VersionSwitch replaces the graphics object without loading map assets.
    game.mPhase_In_Progress = false;
    game.VersionSwitch(game.mVersions->GetForCampaign("Random Map", ePlatform::Amiga));
    const auto original = game.mVersionCurrent;
    game.mStartParams->mMouseAlternative = true;
    game.mParams->mMouseAlternative = true;
    const auto scale = game.mWindow->GetScale();
    const int liveExtra = original->isAmiga() ? 25 : 0;
    std::atomic<bool> finished{false};
    std::thread inputs([&]() {
        const auto press = [&](int x, int y, uint32_t hold) {
            SDL_Event event{}; event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
            event.button.button = SDL_BUTTON_LEFT; event.button.down = true; event.button.clicks = 1;
            event.button.x = float(x * scale.getWidth()); event.button.y = float((y - 4) * scale.getHeight());
            SDL_PushEvent(&event); SDL_Delay(hold);
            event.type = SDL_EVENT_MOUSE_BUTTON_UP; event.button.down = false;
            SDL_PushEvent(&event); SDL_Delay(500);
        };
        SDL_Delay(1500);
        press(232, 44, 100);
        press(215, 79 + liveExtra / 2, 1100); // Held vehicle click advances once.
        press(160, 180 + liveExtra, 100);
        SDL_Delay(1000);
        if (!finished) { SDL_Event event{}; event.type = SDL_EVENT_QUIT; SDL_PushEvent(&event); }
    });
    const bool accepted = game.RandomMapOptions_Run(initial, cRandomMapOptionsMenu::eContext::Multiplayer);
    finished = true; inputs.join();
    if (!accepted || game.mExit || game.mVersionCurrent != original ||
        initial.mVehicleSet != static_cast<eNetworkVehicleSet>((NETWORK_VEHICLE_SET_DEFAULT + 1) % eNetworkVehicleSet_Count)) return false;
    return true;
}

bool cSetupWizard::VerifyScreens(const std::string& output, const std::string& folder) {
    namespace fs = std::filesystem;
    fs::create_directories(output);
    const auto capture = [&](const char* name) {
        Draw();
        g_Fodder->mSurface->palette_SetFromNew();
        g_Fodder->mSurface->surfaceSetToPalette();
        g_Fodder->mSurface->draw();
        return g_Fodder->mSurface->SavePng((fs::path(output) / (std::string(name) + ".png")).string());
    };
    const auto require = [](bool value, const char* label) {
        if (!value) throw std::runtime_error(label);
    };
    const auto waitForCheck = [&]() {
        const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(45);
        while (mScreen == eScreen::Checking || mReadTask.valid() || mAutoScanTask.valid()) {
            require(std::chrono::steady_clock::now() < deadline, "background check finishes");
            Tick();
            Draw();
            SDL_PumpEvents();
            SDL_Delay(1);
        }
    };
    try {
        {
            cSurface shade(2, 1);
            cPalette colours[g_MaxColors]{};
            colours[1].mRed = 32; colours[1].mGreen = 40; colours[1].mBlue = 60;
            shade.paletteSet(colours, 0, (uint32)g_MaxColors, true);
            shade.clearBuffer(1);
            shade.dimBackdrop({0xFF, 1});
            shade.draw(0, 0);
            const auto rgb = [&](size_t pixel, uint8 red, uint8 green, uint8 blue) {
                uint8 r, g, b;
                const auto* values = static_cast<const uint32*>(shade.GetSurface()->pixels);
                SDL_GetRGB(values[pixel], SDL_GetPixelFormatDetails(shade.GetSurface()->format), nullptr, &r, &g, &b);
                return r == red && g == green && b == blue;
            };
            require(rgb(0, 64, 80, 120) && rgb(1, 128, 160, 240),
                    "backdrop is dimmed without altering foreground or hue");
            shade.draw(0, 0);
            require(rgb(0, 128, 160, 240), "backdrop shading does not leak to the next screen");
        }
        require(capture("welcome"), "welcome render");
        waitForCheck();
#ifdef _WIN32
        if (std::getenv("OPENFODDER_BROWSER_CHECK")) {
            const auto root = fs::absolute(output).root_path().u8string();
            std::string picked;
            bool cancelled = false;
            mBrowser = std::make_unique<cFileBrowser>();
            auto& browser = *mBrowser;
            mScreen = eScreen::Browse;
            const auto open = [&](const std::string& path) {
                browser.Open(path, cFileBrowser::eFilter::DirectoriesOnly,
                    [&](const std::string& value) { picked = value; },
                    [&]() { cancelled = true; });
            };
            const auto render = [&](const char* name) {
                Draw();
                g_Fodder->mSurface->palette_SetFromNew();
                g_Fodder->mSurface->surfaceSetToPalette();
                g_Fodder->mSurface->draw();
                require(g_Fodder->mSurface->SavePng((fs::path(output) / name).u8string()), name);
            };
            open(root);
            render("browser-drive-root.png");
            browser.OnRowClick(1, 0); // Parent row at a drive root.
            require(!browser.IsDone() && browser.EntryCount() > 0, "drive list opens from root");
            render("browser-drives.png");
            browser.OnRowClick(2, 0); // Virtual drive list cannot be picked.
            require(picked.empty() && !browser.IsDone(), "drive list is not a folder");
            SDL_Delay(130);
            browser.OnRowClick(1, 0); // First available drive.
            SDL_Delay(130);
            browser.OnRowClick(2, 0);
            require(browser.IsDone() && picked.size() == 3 && picked[1] == ':' && picked[2] == '/',
                    "drive entry opens an absolute root");
            open(output);
            browser.OnBack(); // Parent directory, not cancellation.
            require(!browser.IsDone() && !cancelled, "back navigates to parent");
            open(root);
            browser.OnBack();
            require(!browser.IsDone() && !cancelled, "back from drive root opens drives");
            browser.OnBack();
            require(browser.IsDone() && cancelled, "back from drives cancels");
            const auto names = fs::path(output) / "browser-names";
            fs::create_directories(names / "$RECYCLE.BIN");
            fs::create_directories(names / "A$B");
            open(names.u8string());
            render("browser-dollar-names.png");
            std::cout << "PASS: drive-root parent, drive selection, back navigation and dollar names\n";
            return true;
        }
#endif
        if (std::getenv("OPENFODDER_SQUAD_JOIN_CHECK")) {
            auto& game = *g_Fodder;
            for (auto& troop : game.mGame_Data.mSoldiers_Allocated) troop.Clear();
            auto& source = game.mSprites[0];
            auto& target = game.mSprites[1];
            auto& enemy = game.mSprites[2];
            source.Clear(); target.Clear(); enemy.Clear();
            source.mSpriteType = target.mSpriteType = eSprite_Player;
            enemy.mSpriteType = eSprite_Enemy;
            source.field_32 = enemy.field_32 = 0;
            target.field_32 = 1;
            source.mPosX = target.mPosX = enemy.mPosX = 100;
            source.mPosY = target.mPosY = enemy.mPosY = 100;
            source.mMissionTroop = &game.mGame_Data.mSoldiers_Allocated[0];
            target.mMissionTroop = &game.mGame_Data.mSoldiers_Allocated[1];
            enemy.mMissionTroop = nullptr;
            source.mMissionTroop->mSprite = &source;
            target.mMissionTroop->mSprite = &target;
            source.mMissionTroop->mSelected = 1;
            game.Squad_Rebuild();
            game.mSquad_Selected = 0;
            game.mSquad_Join_TargetSquad[0] = 1;
            game.mSquad_Join_TargetSprite[0] = &target;
            game.mSquad_WalkTargets[1][0].asInt = -1;
            game.mSquad_Grenades[0] = 3; game.mSquad_Grenades[1] = 2;
            game.mSquad_Rockets[0] = 1; game.mSquad_Rockets[1] = 1;
            require(game.Squad_Join_Check(&enemy) == -1 && game.mSquad_Join_TargetSquad[0] == 1,
                    "nearby enemy using squad zero cannot consume or cancel the player's join order");
            require(game.Squad_Join(&enemy) == -1 && enemy.field_32 == 0,
                    "enemy cannot enter the merge routine or dereference a player troop record");
            enemy.mSpriteType = eSprite_Enemy_Rocket;
            require(game.Squad_Join_Check(&enemy) == -1 && game.mSquad_Join_TargetSquad[0] == 1,
                    "rocket enemies cannot join player squads");
            require(game.Squad_Join_Check(nullptr) == -1 && game.Squad_Join(nullptr) == -1,
                    "null source is rejected");
            source.field_32 = -1;
            require(game.Squad_Join_Check(&source) == -1 && game.Squad_Join(&source) == -1,
                    "negative source squad is rejected before indexing");
            source.field_32 = NETWORK_MAX_SQUADS;
            require(game.Squad_Join_Check(&source) == -1 && game.Squad_Join(&source) == -1,
                    "out-of-range source squad is rejected before indexing");
            source.field_32 = 0;
            game.mSquad_Join_TargetSprite[0] = nullptr;
            require(game.Squad_Join_Check(&source) == -1, "missing join target is rejected");
            game.mSquad_Join_TargetSprite[0] = INVALID_SPRITE_PTR;
            require(game.Squad_Join_Check(&source) == -1, "invalidated join target is rejected");
            game.mSquad_Join_TargetSprite[0] = &target;
            game.mSquad_Join_TargetSquad[0] = NETWORK_MAX_SQUADS;
            require(game.Squad_Join_Check(&source) == -1 && game.Squad_Join(&source) == -1,
                    "out-of-range destination squad is rejected");
            game.mSquad_Join_TargetSquad[0] = 1;
            game.mSquad_Selected = -1;
            require(game.Squad_Join_Check(&source) == -1, "no selected squad cannot index the squad list");
            game.mSquad_Selected = 0;
            source.mPosX = 300;
            require(game.Squad_Join_Check(&source) == 0 && source.field_32 == 0,
                    "player troop keeps approaching its target before merging");
            source.mPosX = 100;
            source.mPersonType = eSprite_PersonType_AI; // Player sprites can be AI controlled.
            require(game.Squad_Join_Check(&source) == -1 && source.field_32 == 1 &&
                    game.mSquads_TroopCount[0] == 0 && game.mSquads_TroopCount[1] == 2,
                    "real player troop still joins the destination squad");
            require(game.mSquad_Grenades[0] == 0 && game.mSquad_Grenades[1] == 5 &&
                    game.mSquad_Rockets[0] == 0 && game.mSquad_Rockets[1] == 2 &&
                    (source.mMissionTroop->mSelected & 1) == 0,
                    "legitimate join transfers ammunition and clears troop selection");
            std::cout << "PASS: enemy squad-zero join regression, invalid targets and normal player squad merge\n";
            return true;
        }
        if (std::getenv("OPENFODDER_CURSOR_TRANSITION_CHECK")) {
            const auto wasAlternative = g_Fodder->mStartParams->mMouseAlternative;
            const auto hadFocus = g_Fodder->mWindow_Focus;
            const auto wasInputEnabled = g_Fodder->mInput_Enabled;
            const auto wasPaused = g_Fodder->mPhase_Paused;
            const auto mission = g_Fodder->mGame_Data.mMission_Number;
            SDL_ShowCursor();
            g_Fodder->mWindow->SetRelativeMouseMode(false);
            require(!SDL_CursorVisible(), "releasing relative capture keeps native cursor hidden");
            g_Fodder->mWindow_Focus = false;
            g_Fodder->mStartParams->mMouseAlternative = false;
            SDL_ShowCursor();
            g_Fodder->Mouse_Cursor_Handle();
            g_Fodder->mWindow->Cycle();
            require(!SDL_CursorVisible(), "focus loss does not enable the Windows arrow");
            g_Fodder->mStartParams->mMouseAlternative = true;
            SDL_ShowCursor();
            g_Fodder->Mouse_Cursor_Handle();
            g_Fodder->mWindow->Cycle();
            require(!SDL_CursorVisible(), "alternative mouse mode keeps the native cursor hidden");
            g_Fodder->mStartParams->mMouseAlternative = wasAlternative;
            g_Fodder->mWindow_Focus = hadFocus;
            // Pause interrupt input updates, so the intro must establish
            // cursor visibility itself rather than relying on Mouse_ReadInputs.
            g_Fodder->mInput_Enabled = true;
            g_Fodder->mPhase_Paused = true;
            SDL_ShowCursor();
            std::thread skipIntro([] {
                SDL_Delay(500);
                SDL_Event event{};
                event.type = SDL_EVENT_KEY_DOWN;
                event.key.scancode = SDL_SCANCODE_ESCAPE;
                SDL_PushEvent(&event);
                SDL_Delay(40);
                event.type = SDL_EVENT_KEY_UP;
                SDL_PushEvent(&event);
            });
            const auto tiles = g_Fodder->mVersionCurrent->getTileTypes();
            g_Fodder->mGame_Data.mMission_Number = 0; // Same setup as Intro_OpenFodder.
            g_Fodder->Briefing_Intro_Helicopter_Play(true, tiles.front().Type);
            skipIntro.join();
            require(!SDL_CursorVisible(), "helicopter transition hides the native cursor without input ticks");
            g_Fodder->mInput_Enabled = wasInputEnabled;
            g_Fodder->mPhase_Paused = wasPaused;
            g_Fodder->mGame_Data.mMission_Number = mission;
            std::cout << "PASS: native cursor remains hidden across capture release, focus loss, alternative input and helicopter intro\n";
            return true;
        }
        for (const auto& candidate : mCandidates)
            for (const auto& match : candidate.mResult.mMatches)
                require(std::find(mAvailableAtOpen.begin(), mAvailableAtOpen.end(), match.mVersion) == mAvailableAtOpen.end(),
                        "automatic suggestions exclude already playable games");
        if (!mAvailableAtOpen.empty()) {
            QueuePick(g_ResourceMan->getcwd() + "/Data");
            require(capture("checking-own-data"), "own data check render");
            waitForCheck();
            require(mLastResult.mAlreadyAvailable && mLastResult.mMatches.empty(), "own data cannot be added again");
            require(capture("already-added"), "already added render");
        }
        {
            // Hold a worker open while normal UI frames continue. A blocking
            // Tick regression fails this check instead of hiding behind fast disks.
            auto released = std::make_shared<std::atomic<bool>>(false);
            mReadCancelled = std::make_shared<std::atomic<bool>>(false);
            mReadTask = std::async(std::launch::async, [released] {
                const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
                while (!released->load() && std::chrono::steady_clock::now() < deadline) SDL_Delay(1);
                return std::shared_ptr<sReadResult>{};
            });
            mScreen = eScreen::Checking;
            mPendingPick.clear();
            for (int frame = 0; frame < 3; ++frame) {
                Tick();
                require(mReadTask.valid(), "UI ticks without waiting for the file worker");
                require(capture(("background-check-frame-" + std::to_string(frame)).c_str()), "live check frame render");
                SDL_Delay(180);
            }
            mReadCancelled->store(true);
            released->store(true);
            OnBack();
            waitForCheck();
            require(mScreen == eScreen::Locate, "cancelled check does not publish a stale result");
        }
        // Simulate a fresh installation for the import and collection checks.
        mAvailableAtOpen.clear();
        mScreen = eScreen::Locate;
        if (FolderCandidates(ValidateFolderCollection(folder), "Preview").size() > 1) {
            g_ResourceMan->addDir(folder);
            RunAutoScan();
        }
        waitForCheck();
        require(capture("suggested-games"), "suggestions render");
        if (FolderCandidates(ValidateFolderCollection(folder), "Preview").size() > 1) {
            const auto group = std::find_if(mCandidates.begin(), mCandidates.end(), [](const sScanCandidate& c) {
                return c.mResult.mMatches.size() > 1;
            });
            require(group != mCandidates.end(), "collection appears as a grouped suggestion");
            if (group != mCandidates.end()) {
                const auto expected = group->mResult.mMatches.size();
                mCandidateStart = int(group - mCandidates.begin());
                g_Fodder->mMouseX = 160 - 32;
                g_Fodder->mMouseY = 70;
                require(capture("game-collection"), "grouped folder render");
                mLastInputMs = 0;
                g_Fodder->GUI_Handle_Element_Mouse_Check(g_Fodder->mGUI_Elements);
                require(mCandidates.size() == expected && !mCandidateFolder.empty(), "collection click expands its games");
                OnBack();
                require(mCandidateFolder.empty(), "back returns to grouped locations");
            }
            QueuePick(folder);
            require(capture("checking-cancelled"), "cancel check render");
            Tick();
            OnBack();
            require(mScreen == eScreen::Locate, "back is responsive during a file check");
            waitForCheck();
            require(mScreen == eScreen::Locate, "cancelled worker cannot replace the current screen");
            QueuePick(folder);
            require(capture("checking-folder"), "checking render");
            waitForCheck();
            require(mScreen == eScreen::Locate && mCandidates.size() > 1, "one choice per detected game");
            g_Fodder->mMouseX = 160 - 32;
            g_Fodder->mMouseY = 70;
            require(capture("folder-games"), "folder game cards render");
            require(CandidatePageSize() == 7, "names-only list fits seven games per page");
            if (mCandidates.size() > size_t(CandidatePageSize())) {
                const auto pageSize = CandidatePageSize();
                OnScrollDown();
                require(mCandidateStart == pageSize, "next page starts after all visible games");
                require(capture("more-folder-games"), "second folder page render");
                OnScrollUp();
                require(mCandidateStart == 0, "previous folder page");
            }
            const auto count = std::count_if(mCandidates.begin(), mCandidates.end(), [](const sScanCandidate& c) {
                return c.mResult.mMatches.front().IsComplete();
            });
            if (count > 1) {
                g_Fodder->mMouseX = 256 - 32;
                g_Fodder->mMouseY = 180 + PLATFORM_BASED(0, 25);
                require(capture("collection-add-all"), "add all button render");
                mLastInputMs = 0;
                g_Fodder->GUI_Handle_Element_Mouse_Check(g_Fodder->mGUI_Elements);
                require(mScreen == eScreen::Result && mLastResult.mMatches.size() == size_t(count) && !mDone,
                        "add all offers confirmation for every complete game");
                require(capture("confirm-all-games"), "bulk confirmation render");
                require(CommitCopyFromRawFolder() > 0, "bulk import copies game files");
                for (const auto& match : mLastResult.mMatches) {
                    for (const auto& file : match.mVersion->mFiles) {
                        const auto target = g_ResourceMan->getcwd() + "/Data/" + match.mVersion->mDataPath + "/" + file.mName;
                        require(g_ResourceMan->FileMD5(target) == g_ResourceMan->FileMD5(match.mResolvedPath + file.mName),
                                "all imported game files match their sources");
                    }
                }
            }
            const auto selected = mCandidates.front().mResult.mMatches.front().mVersion;
            mScreen = eScreen::Locate;
            mCandidateStart = 0;
            g_Fodder->mMouseX = 160 - 32;
            g_Fodder->mMouseY = 65;
            require(capture("select-one-game"), "single game selection render");
            mLastInputMs = 0;
            g_Fodder->GUI_Handle_Element_Mouse_Check(g_Fodder->mGUI_Elements);
            require(mScreen == eScreen::Result && mLastResult.mMatches.size() == 1 &&
                    mLastResult.mMatches.front().mVersion == selected && mPendingPick.empty(),
                    "selected release stays selected without rescanning its shared folder");
            require(capture("folder-game-found"), "single game render");
            mBrowsePath = folder;
            EnterBrowse();
            require(capture("browse-game-folders"), "folder browser render");
            std::cout << "PASS: collection choices, paging, add-all confirmation, imported file checksums and browser rendering\n";
            return true;
        }
        QueuePick(folder);
        require(capture("checking-folder"), "checking render");
        waitForCheck();
        require(mScreen == eScreen::Locate && !mCandidates.empty(), "RAW folder opens a disk-set chooser");
        require(capture("disk-sets"), "disk-set choices render");
        if (mCandidates.size() > size_t(CandidatePageSize())) {
            OnScrollDown();
            require(mCandidateStart > 0, "next choices page");
            require(capture("more-disk-sets"), "second choices page render");
            OnScrollUp();
            require(mCandidateStart == 0, "previous choices page");
        }
        mBrowsePath = folder;
        EnterBrowse();
        require(mBrowser && mBrowser->EntryCount() > 1, "RAW disks visible in browser");
        require(capture("browse-raw-files"), "RAW browser render");
        const auto choices = FindDiskImages(folder);
        auto retail = std::find_if(choices.begin(), choices.end(), [](const sDiskImageChoice& c) { return c.mDisks == 3; });
        require(retail != choices.end(), "three-disk set discovered");
        QueuePick(retail->mPath);
        require(capture("checking-disks"), "checking disks render");
        waitForCheck();
        require(mScreen == eScreen::Result && mLastMountedId > 0, "disk set mounted");
        require(std::any_of(mLastResult.mMatches.begin(), mLastResult.mMatches.end(),
            [](const sVersionMatch& m) { return m.IsComplete(); }), "complete game detected");
        require(capture("game-found"), "complete game render");
        const auto incomplete = fs::path(output) / "one-disk";
        fs::create_directories(incomplete);
        fs::copy_file(retail->mPath, incomplete / fs::path(retail->mPath).filename(), fs::copy_options::overwrite_existing);
        DispatchPick(incomplete.string());
        require(mScreen == eScreen::Result && !mLastResult.mMatches.empty(), "incomplete set reported");
        require(std::none_of(mLastResult.mMatches.begin(), mLastResult.mMatches.end(),
            [](const sVersionMatch& m) { return m.IsComplete(); }), "incomplete set cannot play");
        require(capture("incomplete-set"), "incomplete result render");
        const auto empty = fs::path(output) / "empty";
        fs::create_directories(empty);
        DispatchPick(empty.string());
        require(mLastResult.mMatches.empty(), "empty folder report");
        require(capture("empty-folder"), "empty result render");
        mCandidates.clear(); mCandidateFolder.clear(); mScreen = eScreen::Locate;
        require(capture("find-game-empty"), "empty suggestions render");
        // Exercise the same action as PLAY FROM DISKS, then use the normal
        // resource manager route after the wizard has closed and refreshed.
        const bool hadExtractedData = fs::exists(fs::path(g_ResourceMan->getcwd()) / "Data" / "Amiga");
        QueuePick(retail->mPath);
        require(capture("checking-direct-play"), "direct play checking render");
        waitForCheck();
        require(capture("direct-play-ready"), "direct play result render");
        require(mLastMountedId > 0, "direct play mounted set");
        const auto version = std::find_if(mLastResult.mMatches.begin(), mLastResult.mMatches.end(),
            [](const sVersionMatch& match) { return match.IsComplete() && match.IsRetail(); });
        require(version != mLastResult.mMatches.end(), "direct play retail version");
        const sGameVersion* game = version->mVersion;
        const int directPlayId = mLastMountedId;
        g_Fodder->mMouseX = 160 - 32;
        g_Fodder->mMouseY = 110;
        g_Fodder->GUI_Handle_Element_Mouse_Check(g_Fodder->mGUI_Elements);
        require(mDone && mResourcesChanged, "direct play confirms remembered images");
        Close();
        g_ResourceMan->refresh();
        for (const char* file : {"mapm1.map", "junbase.blk", "cftitle.raw"}) {
            const auto path = game->getDataFilePath(file);
            require(path.find("firy://") == 0, "direct play uses mounted resource path");
            const auto bytes = g_ResourceMan->FileRead(path);
            require(bytes && !bytes->empty(), "direct play reads real game asset");
        }
        require(hadExtractedData || !fs::exists(fs::path(g_ResourceMan->getcwd()) / "Data" / "Amiga"),
                "direct play never extracts a data folder");
        if (std::getenv("OPENFODDER_OPTIONS_DATA_CHECK")) {
            g_ResourceMan->unmountImage(directPlayId);
            require(VerifyOptionsDataFlow(retail->mPath, output), "options add-data round trip and independent settings persistence");
            std::cout << "PASS: options -> add data -> back -> add disk set -> cancel options keeps paths and restores settings\n";
        }
        if (std::getenv("OPENFODDER_MENU_FAMILY_CHECK")) {
            require(VerifyMenuFamily(output), "multiplayer and single-map menus");
        }
        if (std::getenv("OPENFODDER_RANDOM_MENU_CHECK")) {
            require(VerifyRandomMenu(output), "random menu rendering, seed editing, empty seed, direct launch and held-click apply");
            std::cout << "PASS: random map tabs, seed input, disabled empty seed, direct launch, held-click and version restore\n";
        }
        std::cout << "PASS: setup folder discovery, RAW browser, paging, checks, complete/incomplete sets, direct disk resource reads and rendered screens\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "FAIL: setup screens: " << e.what() << "\n";
        return false;
    }
}
}
#endif
