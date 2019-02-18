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

class cResourceMan {
	std::vector<std::string> mAllPaths;
	std::vector<std::string> mValidPaths;

	std::map<const sGameVersion*, std::string> mReleasePath;
	std::map<std::string, std::string> mCampaigns;

	std::map<std::string, std::string> mMaps;
	std::map<std::string, std::string> mSaves;

protected:
	void addBaseDir(std::string pPath);
	void addDefaultDirs();
	void validatePaths();


	std::string FileMD5(const std::string& pFile);

	void findCampaigns();
	void findVersions();
	void findSaves();
	void findMaps();

public:

	cResourceMan();

	void addDir(const std::string& pPath);

	void refresh();

	std::string	FindVersionPath(eRelease pRelease, eGame pGame, ePlatform pPlatform) const;
	bool		VersionHasFile(eRelease pRelease, eGame pGame, ePlatform pPlatform, const std::string& pFile) const;

	tSharedBuffer 		FileRead(const std::string& pFile);
	std::string			PathGenerate(const std::string& pFile, eDataType pDataType) const;

	std::string			GetCampaignData(const std::string& pName);
	std::string			GetSaveNewName() const;
	std::string			GetMapPath(const std::string& pName) const;
	std::string			GetSave(const std::string &pName) const;

	std::vector<const sGameVersion*> GetAvailable() const;
	std::vector<std::string> GetCampaigns() const;
	std::vector<std::string> GetSaves() const;
	std::vector<std::string> GetMaps() const;

	bool isCampaignAvailable(std::string pName) const;
	bool isDataAvailable() const;

	std::vector<std::string> getValidPaths() const;
	std::vector<std::string> getAllPaths() const;
};