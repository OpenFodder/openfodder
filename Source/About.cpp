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
#include "gitver.hpp"

#include "Setup/DataRelease.hpp"
#include "Setup/EngineVersion.hpp"
#include "ResourceMan.hpp"

#include <SDL3/SDL.h>
#include <filesystem>
#include <string>

static std::string compiled = std::string(__DATE__) + " AT " + std::string(__TIME__);
static std::string version = "VERSION " + std::string(gitversion);

const size_t TEXTPOS_BUILD    = 250;
const size_t TEXTPOS_TEAM     = 400;
const size_t TEXTPOS_THANKS   = 550;
const size_t TEXTPOS_POWERED  = 660;

cAbout::cAbout() {
    std::transform(compiled.begin(), compiled.end(), compiled.begin(), ::toupper);
    std::transform(version.begin(), version.end(), version.begin(), ::toupper);
    std::replace(compiled.begin(), compiled.end(), ':', ' ');

    std::vector<sIntroString> AboutText = {
        { TEXTPOS_BUILD + 0 , "OPEN FODDER"},
        //{ TEXTPOS_BUILD + 10 , "BUILD VERSION "},
        { TEXTPOS_BUILD + 10, version.c_str()},
        { TEXTPOS_BUILD + 20, compiled.c_str()},
        { TEXTPOS_BUILD + 30, "WWW DOT OPENFODDER DOT COM"},

        { TEXTPOS_TEAM + 0 ,  "THE TEAM" },
        { TEXTPOS_TEAM + 20 , "PROJECT LEAD" },
        { TEXTPOS_TEAM + 30,  "ROBERT CROSSFIELD" },

        { TEXTPOS_TEAM + 80, "ENGINE" },
        { TEXTPOS_TEAM + 90,  "ROBERT CROSSFIELD" },
        { TEXTPOS_TEAM + 100, "ALESSANDRO PETRALIA" },

		{ TEXTPOS_TEAM + 120, "SCRIPTING" },
		{ TEXTPOS_TEAM + 130, "STARWINDZ" },


        { TEXTPOS_THANKS + 0 , "THANKS TO" },
        { TEXTPOS_THANKS + 10, "SENSIBLE SOFTWARE"},
		{ TEXTPOS_THANKS + 20, "STOO CAMBRIDGE"},
        { TEXTPOS_THANKS + 30, "CLAIRE CROSSFIELD"},
        { TEXTPOS_THANKS + 40, "LACHIE CROSSFIELD"},
        { TEXTPOS_THANKS + 50, "HAMISH CROSSFIELD"},
        { TEXTPOS_THANKS + 60, "AUDREY CROSSFIELD"},
        { TEXTPOS_THANKS + 70, "RILEY ELLETT"},
        { TEXTPOS_THANKS + 80, "SCUMMVM"},

        { TEXTPOS_POWERED + 0,  "POWERED BY"},
        { TEXTPOS_POWERED + 10, "CPP17"},
        { TEXTPOS_POWERED + 20, "SDL3"},
		{ TEXTPOS_POWERED + 30, "DUKTAPE"},
		{ TEXTPOS_POWERED + 40, "DUKGLUE"},
        { TEXTPOS_POWERED + 50, "IDA"},

        // Just for fun: loc_2B48E
        { 1100, "PUSH CX"},
        { 1110, "PUSH SI"},
        { 1120, "PUSH DI"},
        { 1130, "MOV CX 16H"},

        { 1150, "MOV BX ES SI"},
        { 1160, "AND BX 1FFH"},
        { 1170, "PUSH CS"},
        { 1180, "CALL SUB 2B4B8"},
        { 1190, "ADD SI 2"},

        { 1500, "W W W  DOT  O P E N F O D D E R  DOT  C O M"}
    };

    mSurface = new cSurface(0, 0);
    mSurface->LoadPng(g_ResourceMan->GetAboutFile());

    g_Fodder->mGraphics->PaletteSet();
    g_Fodder->Phase_EngineReset();
    g_Fodder->mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    g_Fodder->mService_Draw_List.clear();

    g_Fodder->mGraphics->Load_Service_Data();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_SERVICE);
    g_Fodder->mGraphics->PaletteSet();


    for (auto& Text : AboutText) {
        g_Fodder->Service_Draw_String(Text.mText, false, Text.mPosition);
    }

    g_Fodder->mSound->Music_Play(PLATFORM_BASED(16, CANNON_BASED(16, 20)));
}

cAbout::~cAbout() {

    delete mSurface;
}

bool cAbout::Cycle() {

    g_Fodder->GUI_Element_Reset();

    g_Fodder->mSurface->palette_FadeTowardNew();
    g_Fodder->mSurface->clearBuffer();

    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->Service_Draw_List();

    if (g_Fodder->mInterruptTick % 2 == 0)
        g_Fodder->Service_ScrollUp_DrawList();

    {
        g_Fodder->mString_GapCharID = 0x25;
        g_Fodder->String_Print_Large("Open Fodder", true, 0x01);
        g_Fodder->mString_GapCharID = 0x00;

        // UPDATE sits one row above BACK (empty slot called out in the
        // layout note for the About screen). Forwarded to cAbout::OnUpdateClicked
        // via the cFodder trampoline because GUI_Button_Setup requires a
        // void(cFodder::*)() member-function pointer.
        g_Fodder->GUI_Button_Draw_Small("UPDATE", 0x9C + PLATFORM_BASED(0, 25));
        g_Fodder->GUI_Button_Setup(&cFodder::GUI_Button_About_Update);

        g_Fodder->GUI_Button_Draw_Small("BACK", 0xB3 + PLATFORM_BASED(0, 25));
        g_Fodder->GUI_Button_Setup(&cFodder::GUI_Button_Load_Exit);
    }

    if (g_Fodder->mPhase_Aborted)
        g_Fodder->GUI_Button_Load_Exit();

    if (g_Fodder->mMouse_Button_Left_Toggle) {
        g_Fodder->GUI_Handle_Element_Mouse_Check(g_Fodder->mGUI_Elements);

        if (g_Fodder->mGUI_SaveLoadAction == 1) {

            g_Fodder->mSurface->paletteNew_SetToBlack();
            g_Fodder->mSurface->palette_FadeTowardNew();
        }

    }

    g_Fodder->Mouse_DrawCursor();

    g_Fodder->mSurface->draw();
    g_Fodder->Video_Sleep(mSurface, true);
    g_Fodder->mWindow->RenderAt(g_Fodder->mSurface);
    //g_Fodder->Video_SurfaceRender(false, false, 0, false);

    if (g_Fodder->mGUI_SaveLoadAction == 1 && !g_Fodder->mSurface->isPaletteAdjusting())
        return false;

    return true;
}

// ---------------------------------------------------------------------------
// UPDATE button — query GitHub for newer compatible data + scripts releases
// and, on confirmation, download + extract them into ./Data alongside the exe.
//
// TODO: async progress UI. The two QueryLatest calls and (when accepted) two
// FetchAndInstall calls run on the main thread, so the About screen will
// freeze for a few seconds at minimum. The CLI shows progress through stdout;
// the GUI currently has no equivalent surface, so we just block.
// ---------------------------------------------------------------------------
void cAbout::OnUpdateClicked() {
    using Setup::DataRelease;
    using Setup::ReleaseManifest;

    // Each repo lands in its own subtree under the working dir (Run/ in dev,
    // the install dir for shipped builds). Tracking installed.json *per repo*
    // avoids the two installs clobbering each other's version record.
    const std::filesystem::path cwd = std::filesystem::current_path();
    const std::string dataTargetDir    = (cwd / "Data").string();
    const std::string scriptsTargetDir = (cwd / "Scripts").string();

    DataRelease release;
    ReleaseManifest dataManifest;
    ReleaseManifest scriptManifest;

    auto showError = [](const std::string& pMessage) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "OpenFodder Update",
                                 pMessage.c_str(),
                                 nullptr);
    };

    if (!release.QueryLatest(DataRelease::DataRepoOwner(), DataRelease::DataRepoName(), dataManifest)) {
        showError("Could not check for updates: " + release.LastError());
        return;
    }
    if (!release.QueryLatest(DataRelease::ScriptsRepoOwner(), DataRelease::ScriptsRepoName(), scriptManifest)) {
        showError("Could not check for updates: " + release.LastError());
        return;
    }

    int installedDataVer = 0;
    int installedScriptVer = 0;
    // ReadInstalledManifest returns false when installed.json is missing or
    // malformed, but it still zero-initialises the out-params — which is the
    // behaviour we want (treat "no record" as "version 0", everything newer).
    // Each tree carries its own manifest, so we read them independently. The
    // ignored out-param on each call is the field that doesn't apply to that
    // tree (Scripts/installed.json doesn't carry a dataVersion and vice
    // versa); the caller below only consults the relevant one.
    int dataIgnored = 0;
    int scriptsIgnored = 0;
    release.ReadInstalledManifest(dataTargetDir,    installedDataVer,   dataIgnored);
    release.ReadInstalledManifest(scriptsTargetDir, scriptsIgnored,     installedScriptVer);

    const bool dataNewer = (dataManifest.mDataVersion > installedDataVer)
                         && Setup::IsDataVersionCompatible(dataManifest.mDataVersion);
    const bool scriptNewer = (scriptManifest.mScriptVersion > installedScriptVer)
                           && Setup::IsScriptVersionCompatible(scriptManifest.mScriptVersion);

    if (!dataNewer && !scriptNewer) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "OpenFodder Update",
                                 "You have the latest compatible version.",
                                 nullptr);
        return;
    }

    std::string message = "Updates available:\n";
    if (dataNewer)
        message += "  data: v" + std::to_string(installedDataVer)
                 + " \xE2\x86\x92 v" + std::to_string(dataManifest.mDataVersion) + "\n";
    if (scriptNewer)
        message += "  scripts: v" + std::to_string(installedScriptVer)
                 + " \xE2\x86\x92 v" + std::to_string(scriptManifest.mScriptVersion) + "\n";
    message += "\nInstall now?";

    const SDL_MessageBoxButtonData buttons[2] = {
        // returnKey-default | escapeKey-default — Install on Enter, Cancel on Esc.
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Install" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Cancel"  },
    };
    SDL_MessageBoxData prompt{};
    prompt.flags       = SDL_MESSAGEBOX_INFORMATION;
    prompt.window      = nullptr;
    prompt.title       = "OpenFodder Update";
    prompt.message     = message.c_str();
    prompt.numbuttons  = 2;
    prompt.buttons     = buttons;
    prompt.colorScheme = nullptr;

    int chosen = -1;
    if (!SDL_ShowMessageBox(&prompt, &chosen) || chosen != 1)
        return;

    // Install whichever repos have updates; surface the first failure and
    // bail rather than continuing into a half-installed state. Each repo
    // lands in its own subtree so the two installs (and their installed.json
    // records) don't collide.
    if (dataNewer) {
        if (!release.FetchAndInstall(dataManifest, dataTargetDir)) {
            showError("Data install failed: " + release.LastError());
            return;
        }
    }
    if (scriptNewer) {
        if (!release.FetchAndInstall(scriptManifest, scriptsTargetDir)) {
            showError("Scripts install failed: " + release.LastError());
            return;
        }
    }

    // Pick up new files without restarting.
    if (g_ResourceMan)
        g_ResourceMan->refresh();

    std::string done = "Updated to";
    if (dataNewer)
        done += " data v" + std::to_string(dataManifest.mDataVersion);
    if (dataNewer && scriptNewer)
        done += " /";
    if (scriptNewer)
        done += " scripts v" + std::to_string(scriptManifest.mScriptVersion);
    done += ".";

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "OpenFodder Update",
                             done.c_str(),
                             nullptr);
}
