/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2018 Open Fodder
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

#ifdef EMSCRIPTEN
#include "stdafx.hpp"
cAbout* About = 0;

void phase_loop();
void menu_loop() {
	static int16 result = -1;
	if (result == -1) {

		g_Fodder->VersionSwitch(g_Fodder->mVersions->GetForCampaign("Amiga Format Christmas Special"));
		g_Fodder->mGame_Data.mCampaign.Clear();

		result = 0;
		g_Fodder->Campaign_Select_Setup();
		return;
	}

	if (About) {

		if (About->Cycle()) {
			g_Fodder->mWindow->RenderAt(g_Fodder->mSurface);
			g_Fodder->mWindow->FrameEnd();
			g_Fodder->Cycle_End();
			return;
		}

		delete About;
		About = 0;
		g_Fodder->mGUI_SaveLoadAction = 0;
		result = -1;
		return;
	}

	g_Fodder->Campaign_Select_File_Cycle("OPEN FODDER", "SELECT CAMPAIGN");
	if(g_Fodder->mGUI_SaveLoadAction == 3 || g_Fodder->mGUI_SaveLoadAction == 0) {
		return;
	}
	if (g_Fodder->mGUI_SaveLoadAction == 4) {
		g_Fodder->mGUI_SaveLoadAction = 0;
		About = new cAbout();
		return;
	}

	g_Fodder->mPhase_Aborted = false;
	g_Fodder->mPhase_In_Progress = false;

	std::string Campaign = g_Fodder->mCampaignList[g_Fodder->mGUI_Select_File_CurrentIndex + g_Fodder->mGUI_Select_File_SelectedFileIndex];
	if (g_Fodder->mGUI_SaveLoadAction == 1)
		Campaign = "";

	g_Fodder->VersionSwitch(g_Fodder->mVersions->GetForCampaign(Campaign));
	g_Fodder->mGame_Data.mCampaign.LoadCampaign(Campaign, Campaign != g_Fodder->mVersionCurrent->mName);
	g_Fodder->Game_Setup();

	result = -1;
	emscripten_cancel_main_loop();
	emscripten_set_main_loop(phase_loop, 24, 1);
}

void phase_loop() {
	static int16 result = -1;

	// No recruits left?
	if (result != 1) {
		if (!g_Fodder->mGame_Data.mRecruits_Available_Count) {
			emscripten_cancel_main_loop();
			emscripten_set_main_loop(menu_loop, 24, 1);

			result = -1;
		}
	}

	if (result == 0) {
		// Game Won?
		if (!g_Fodder->mGame_Data.Phase_Next()) {
			emscripten_cancel_main_loop();
			emscripten_set_main_loop(menu_loop, 24, 1);
			// Break to version screen
			return;
		}
		result = -1;
	}

	if (result == -1) {
		if (g_Fodder->mPhase_Aborted2) {
			g_Fodder->mPhase_Aborted2 = false;
			emscripten_cancel_main_loop();
			emscripten_set_main_loop(menu_loop, 24, 1);
			return;
		}
		g_Fodder->Phase_EngineReset();
		g_Fodder->Phase_SquadPrepare();
		g_Fodder->Phase_Prepare();
	}

	// -1 = Phase Try Again 
	//  0 = Phase Won
	//  1 = Phase Running

	result = g_Fodder->Phase_Cycle();
	g_Fodder->Cycle_End();
}

int start(int argc, char *argv[]) {

	g_Debugger = std::make_shared<cDebugger>();
	g_Window = std::make_shared<cWindow>();
	g_ResourceMan = std::make_shared<cResourceMan>();
	g_Fodder = std::make_shared<cFodder>(g_Window);

	auto Params = std::make_shared<sFodderParameters>();
	Params->Process(argc, argv);

	if (Params->mShowHelp)
		return 0;

	Params->mCheatsEnabled = true;
	Params->mMouseAlternative = true;
	g_Fodder->Prepare(Params);
	g_Fodder->Phase_SquadPrepare();

	emscripten_set_main_loop(menu_loop, 24, 1);

	//g_Fodder->Service_Show();

	return 0;
}

#endif
