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

enum eTestState {
    eTest_Running,
    eTest_Passed,
    eTest_Failed,
    eTest_Skipped
};

class cDebugger {

protected:
    bool ConsoleOpen();

	void Output(const std::string pMessage);

public:
    cDebugger();

	void ClearConsole();

    void Notice(const std::string& pMessage);
    void Error(const std::string& pMessage);

    void TestStart( const std::string& pName, const std::string& pGroup );
    void TestComplete(const std::string& pName, const std::string& pGroup, const std::string& pMessage, const size_t pTime, eTestState pTestState);

};
