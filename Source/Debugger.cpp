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

#ifdef WIN32
#include <windows.h>
#endif

cDebugger::cDebugger() {

}

void cDebugger::ConsoleOpen() {
#ifdef _CONSOLE
    return;
#endif

#ifdef WIN32
    static bool attached = AttachConsole(ATTACH_PARENT_PROCESS);
    if (attached) {
        AllocConsole();

        FILE *stream, *stream2;
        freopen_s(&stream, "CONIN$", "r", stdin);
        freopen_s(&stream2, "CONOUT$", "w", stdout);
    }
#endif
}

void cDebugger::Notice(const std::string& pMessage) {
    ConsoleOpen();

    if (g_Fodder->mParams.mAppVeyor) {
        std::string Command = "appveyor AddMessage ";
        Command += "\"" + pMessage + "\"";
        Command += " -Category Information";
        system(Command.c_str());
    }
    else {

        std::cout << pMessage << "\n";
    }

}

void cDebugger::Error(const std::string& pMessage) {
    ConsoleOpen();
    if (g_Fodder->mParams.mAppVeyor) {
        std::string Command = "appveyor AddMessage ";
        Command += "\"" + pMessage + "\"";
        Command += " -Category Error";
        system(Command.c_str());
    }
    else {
        std::cout << pMessage << "\n";
    }
}

void cDebugger::TestStart(const std::string& pName) {
    ConsoleOpen();
    if (g_Fodder->mParams.mAppVeyor) {
        std::string Command = "appveyor AddTest";

        Command += " -Name \"" + pName + "\"";
        Command += " -Framework NUnit -FileName OpenFodder";
        Command += " -Outcome Running";
        system(Command.c_str());
    }
    else {
        std::cout << "[Test] " << pName << ": starting\n";
    }
}

void cDebugger::TestComplete(const std::string& pName, const std::string& pMessage, eTestState pTestState) {
    ConsoleOpen();
    if (g_Fodder->mParams.mAppVeyor) {
        std::string Command = "appveyor UpdateTest";
        Command += " -Name \"" + pName + "\"";
        Command += " -Framework NUnit -FileName OpenFodder";
        if (pTestState == eTest_Failed)
            Command += " -ErrorMessage \"" + pMessage + "\"";

        Command += " -Duration 10000";
        Command += " -Outcome ";

        switch (pTestState) {
        case eTest_Passed:
            Command += "Passed";
            break;
        case eTest_Failed:
            Command += "Failed";
            break;
        case eTest_Skipped:
            Command += "Skipped";
            break;
        }

        system(Command.c_str());
    }
    else {
        if(pTestState == eTest_Passed)
            std::cout << "[Test] " << pName << ": Passed - " << pMessage << "\n";
        else
            std::cout << "[Test] " << pName << ": Failed - " << pMessage << "\n";
    }
}
