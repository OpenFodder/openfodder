/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2024 Open Fodder
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

struct duk_hthread;
class cRandomMap;

class cScriptFileIO {
	std::fstream mStream;

public:
	cScriptFileIO(std::string pFilename, bool pRead = false);
	~cScriptFileIO();

	bool isOpen();
	void close();
	std::string readLine();
	void writeLine(std::string pStr);
};

class cScriptingEngine {
	duk_hthread *mContext;

protected:

	cCampaign* getCampaign();
	std::shared_ptr<cRandomMap> getMap();
	std::shared_ptr<cPhase> getPhase();
	std::shared_ptr<cMission> getMission();

	std::shared_ptr<cPhase> phaseCreate();
	std::shared_ptr<cMission> missionCreate();

	void guiPrintString(const std::string& pText, const size_t pX, const size_t pY, const bool pLarge, const bool pUnderline);

	void mapSave();


	bool scriptCall(const std::string& pFilename);
	bool scriptRun(const std::string& pJS, const std::string& pFilename);
	bool scriptsLoadFolder(const std::string& pFolder);
	void spritesCreateObject();

public:

	cScriptingEngine();
	~cScriptingEngine();

	void init();
	void debuggerEnable();
	duk_hthread *getContext() { return mContext; }

	bool Run(const std::string& pScript);
};