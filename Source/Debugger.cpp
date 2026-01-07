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

#ifdef WIN32
#include <windows.h>
#endif

cDebugger::cDebugger() {

}

bool cDebugger::ConsoleOpen() {
	static bool Alloced = false;

	if (Alloced)
		return true;
/*
#ifdef _CONSOLE
    return true;
#endif*/
    if (g_Fodder->mParams->mAppVeyor)
        return true;

#ifdef WIN32
    static bool attached = AttachConsole(ATTACH_PARENT_PROCESS);
	if (!attached)
		attached = AllocConsole();

	FILE *stream, *stream2, *stream3;

	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream2, "CONOUT$", "w", stdout);
	freopen_s(&stream3, "CONOUT$", "w", stderr);

	Alloced = true;
	std::ios::sync_with_stdio(true);
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();
#endif
	return true;
}

void cDebugger::ClearConsole() {

#ifdef WIN32
	system("cls");
#else
	system("clear");
#endif
}

void cDebugger::Output(const std::string pMessage) {
#ifdef WIN32
	OutputDebugStringA(pMessage.c_str());
#endif
	std::cout << pMessage;
}

void cDebugger::Notice(const std::string& pMessage) {
    ConsoleOpen();

    if (g_Fodder->mParams->mAppVeyor) {
        Output("::notice::" + pMessage + "\n");
    }
    else {
		Output(pMessage + "\n" );
    }

}

void cDebugger::Error(const std::string& pMessage) {
    ConsoleOpen();
    if (g_Fodder->mParams->mAppVeyor) {
        Output("::error::" + pMessage + "\n");
    }
    else {
		Output(pMessage + "\n");
    }
}

void cDebugger::TestStart(const std::string& pName, const std::string& pGroup) {
    ConsoleOpen();
    if (g_Fodder->mParams->mAppVeyor) {
        Output("::notice::Test start: " + pGroup + " - " + pName + "\n");
    }
    else {
        std::cout << "[Test] " << pName << ": starting\n";
    }
}

void cDebugger::TestComplete(const std::string& pName, const std::string& pGroup, const std::string& pMessage, const size_t pTime, eTestState pTestState) {
    ConsoleOpen();
    if (g_Fodder->mParams->mAppVeyor) {
        std::string message = "Test " + pGroup + " - " + pName + ": " + pMessage +
            " (" + std::to_string(pTime) + "ms)";
        if (pTestState == eTest_Failed) {
            Output("::error::" + message + "\n");
        } else if (pTestState == eTest_Skipped) {
            Output("::notice::" + message + " [Skipped]\n");
        } else if (pTestState == eTest_Running) {
            Output("::notice::" + message + " [Running]\n");
        } else {
            Output("::notice::" + message + " [Passed]\n");
        }
    }
    else {
        if(pTestState == eTest_Passed)
            std::cout << "[Test] " << pName << ": Passed - " << pMessage << "\n";
        else
            std::cout << "[Test] " << pName << ": Failed - " << pMessage << "\n";
    }
}
