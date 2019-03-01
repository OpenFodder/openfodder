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

#ifndef _OFED
#ifndef _OFBOT

int start(int argc, char *argv[]) {
	g_Debugger = std::make_shared<cDebugger>();
	g_Window = std::make_shared<cWindow>();
	g_ResourceMan = std::make_shared<cResourceMan>();
	g_Fodder = std::make_shared<cFodder>(g_Window);

	sFodderParameters Params;

	Params.ProcessINI();
	Params.ProcessCLI(argc, argv);

	g_Fodder->Prepare(Params);

	if (Params.mUnitTesting) {
		cUnitTesting Testing;
		return Testing.Start() ? 0 : -1;
	}
	else {
		g_Fodder->Start();
		g_Fodder->mGame_Data.mDemoRecorded.save();
	}

	return 0;
}

int main(int argc, char *argv[]) {

	auto result = start(argc, argv);
	if (result == -1) {
		std::cout << "Press enter to continue\n";
		std::cin.get();
	}

	return result;
}
#endif
#endif