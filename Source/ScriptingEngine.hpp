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

struct duk_hthread;
class cRandomMap;

class cScriptingEngine {
	duk_hthread *mContext;
	std::shared_ptr<cRandomMap> mMap;

protected:

	bool scriptCall(const std::string& pFilename);
	bool scriptLoad(const std::string& pJS);
	bool scriptsLoadFolder(const std::string& pFolder);

	bool scriptRun(const std::string& pJS);

	void spritesCreateObject();
public:

	cScriptingEngine();
	~cScriptingEngine();

	void init();

	void Randomise(std::shared_ptr<cRandomMap> pMap, const std::string& pScript);
};