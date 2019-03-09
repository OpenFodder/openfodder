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

#include "stdafx.hpp"

#ifdef EMSCRIPTEN

void phase_loop() {
	
	// -1 = Phase Try Again 
	//  0 = Phase Won
	//  1 = Phase Running
	int16 result = g_Fodder->Phase_Cycle();
	g_Fodder->Cycle_End();

	if (result == -1) {
		g_Fodder->Mission_Memory_Clear();
		g_Fodder->Mission_Prepare_Squads();
		g_Fodder->sub_10DEC();


		g_Fodder->Map_Load();
		g_Fodder->Map_Load_Sprites();
		g_Fodder->Map_Overview_Prepare();

		g_Fodder->Phase_Soldiers_Count();
		g_Fodder->mGame_Data.Soldier_Sort();
		g_Fodder->Phase_Soldiers_Prepare(false);
		g_Fodder->Phase_Soldiers_AttachToSprites();
		g_Fodder->Phase_Prepare();
	}

	if (result == 0) {
		g_Fodder->GameOverCheck();
	}

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

	g_Fodder->Prepare(Params);
	g_Fodder->Mission_Prepare_Squads();

	g_Fodder->VersionSwitch(g_Fodder->mVersions->GetForCampaign("Amiga Format Christmas Special"));
	g_Fodder->mGame_Data.mCampaign.Clear();

	if (g_Fodder->mParams->mCampaignName == "")
		g_Fodder->mGame_Data.mCampaign.LoadCampaign("Amiga Format Christmas Special", false);
	else
		g_Fodder->mGame_Data.mCampaign.LoadCampaign(g_Fodder->mParams->mCampaignName, false);

	g_Fodder->Game_Setup();

	g_Fodder->Mission_Memory_Clear();
	g_Fodder->Mission_Prepare_Squads();
	g_Fodder->sub_10DEC();


	g_Fodder->Map_Load();
	g_Fodder->Map_Load_Sprites();
	g_Fodder->Map_Overview_Prepare();

	g_Fodder->Phase_Soldiers_Count();
	g_Fodder->mGame_Data.Soldier_Sort();
	g_Fodder->Phase_Soldiers_Prepare(false);
	g_Fodder->Phase_Soldiers_AttachToSprites();

	g_Fodder->Phase_Prepare();
	emscripten_set_main_loop(phase_loop, 0, 1);

	//g_Fodder->Service_Show();

	return 0;
}
#else


#ifndef _OFED
#ifndef _OFBOT


int start(int argc, char *argv[]) {
	g_Debugger = std::make_shared<cDebugger>();
	g_Window = std::make_shared<cWindow>();
	g_ResourceMan = std::make_shared<cResourceMan>();
	g_Fodder = std::make_shared<cFodder>(g_Window);

	auto Params = std::make_shared<sFodderParameters>();
	Params->Process(argc, argv);

	if (Params->mShowHelp)
		return 0;

	g_Fodder->Prepare(Params);

	if (g_Fodder->mStartParams->mUnitTesting) {
		cUnitTesting Testing;
		return Testing.Start() ? 0 : -1;
	}
	else {
		g_Fodder->Start();
		g_Fodder->mGame_Data.mDemoRecorded.save();
	}

	return 0;
}

// Debug stuff
void quickServiceScreen() {
	g_Fodder->VersionSwitch(g_Fodder->mVersions->GetRetail(g_Fodder->mParams->mDefaultPlatform, g_Fodder->mParams->mDefaultGame));
	g_Fodder->mGame_Data.mCampaign.Clear();
	g_Fodder->mGame_Data.mCampaign.LoadCampaign("Cannon Fodder", false);

	g_Fodder->Game_Setup();
	g_Fodder->Map_Load();
	g_Fodder->Map_Load_Sprites();

	//g_Fodder->Phase_Prepare();
	g_Fodder->Phase_Soldiers_Count();
	g_Fodder->mGame_Data.Soldier_Sort();
	g_Fodder->Phase_Soldiers_Prepare(false);
	g_Fodder->Phase_Soldiers_AttachToSprites();
	g_Fodder->Service_Show();
}


#endif
#endif



#endif

int main(int argc, char *argv[]) {
	auto result = start(argc, argv);
	if (result == -1) {
		std::cout << "Press enter to continue\n";
		std::cin.get();
	}

	return result;
}