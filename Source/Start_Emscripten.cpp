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

void phase_loop() {
	static int16 result = -1;

	// No recruits left?
	if (result != 1) {
		if (!g_Fodder->mGame_Data.mRecruits_Available_Count) {
			//g_Fodder->mGame_Data.mCampaign.Clear();

			g_Fodder->Game_Setup();

			result = -1;
		}
	}

	if (result == 0) {

		// Game Won?
		if (!g_Fodder->mGame_Data.Phase_Next()) {

			emscripten_cancel_main_loop();

			// Break to version screen
			return;
		}

		result = -1;
	}
	if (result == -1) {
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

	g_Fodder->Prepare(Params);
	g_Fodder->Phase_SquadPrepare();

	g_Fodder->VersionSwitch(g_Fodder->mVersions->GetForCampaign("Amiga Format Christmas Special"));
	g_Fodder->mGame_Data.mCampaign.Clear();

	if (g_Fodder->mParams->mCampaignName == "")
		g_Fodder->mGame_Data.mCampaign.LoadCampaign("Amiga Format Christmas Special", false);
	else
		g_Fodder->mGame_Data.mCampaign.LoadCampaign(g_Fodder->mParams->mCampaignName, false);

	g_Fodder->Game_Setup();

	emscripten_set_main_loop(phase_loop, 24, 1);

	//g_Fodder->Service_Show();

	return 0;
}

#endif
