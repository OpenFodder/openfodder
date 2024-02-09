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

#include "stdafx.hpp"
#include "Utils/md5.hpp"

#include <chrono>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

const char* EXTENSION_SAVEGAME = ".ofg";
const char* EXTENSION_CAMPAIGN = ".ofc";
const char* EXTENSION_MAP = ".map";
const char* EXTENSION_DEMO = ".ofd";

cResourceMan::cResourceMan() {

	addDefaultDirs();
}

void cResourceMan::addDir(const std::string& pPath) {
	mAllPaths.push_back(pPath);
}

void cResourceMan::addBaseDir(std::string pPath) {
	// Ensure trailing / on path
	if(pPath.size())
		if (pPath[pPath.size() - 1] != '/')
			pPath.append("/");

	mAllPaths.push_back(pPath + "OpenFodder/");
}

void cResourceMan::addDefaultDirs() {
	std::string path;
	
	// Working Dir
	mAllPaths.push_back(getcwd());

#ifdef WIN32
	size_t a = 512;
	char path1[512];
	getenv_s(&a, path1, "USERPROFILE");
	path.append(path1, a - 1);

	if (path.size())
		addBaseDir(path + "/Documents/");
#else
	char* path1 = std::getenv("XDG_DATA_DIRS");
	if (path1) {
		std::stringstream ss;
		ss << path1;
		while (ss.good()) {
			std::string substr;
			std::getline(ss, substr, ':');
			addBaseDir(substr);
		}
	}
	path1 = std::getenv("HOME");
	if (path1) {
		path = path1;
		addBaseDir(path + "/.local/share/");
	}
	addBaseDir("/usr/local/share/");
#endif
}

void cResourceMan::validatePaths() {

	mValidPaths.clear();
	for (auto path : mAllPaths) {

		if (path[path.size() - 1] != '/')
			path.append("/");

		if (FileExists(path + "Data")) {

			// Ensure we have a trailing /
			if (path[path.size() - 1] != '/')
				path.append("/");

			mValidPaths.push_back(path);
		}
	}
}

void cResourceMan::findCampaigns() {
	mCampaigns.clear();

	// Loop each path
	for (auto& ValidPath : mValidPaths) {
		auto basepath = ValidPath + PathGenerate("", eCampaign);

		auto files = DirectoryList(basepath, EXTENSION_CAMPAIGN);

		for (auto& file : files) {
			size_t Pos = file.find_first_of(".");
			std::string FileName = file.substr(0, Pos);
			mCampaigns.emplace(std::make_pair(FileName, basepath + FileName));
		}
	}
}

void cResourceMan::findVersions() {
	bool haveRetail = false;

	mReleasePath.clear();
	mReleaseFiles.clear();

	// Loop each path
	for (auto& ValidPath : mValidPaths) {
		// Loop all known versions
		for (auto& KnownVersion : KnownGameVersions) {
			// If this release has files, continue on
			if (mReleaseFiles.find(&KnownVersion) != mReleaseFiles.end())
				continue;

			std::string base = ValidPath + PathGenerate(KnownVersion.mDataPath, eData) + "/";
			tStringMap ReleaseFiles;

			// Loop all files in the data directory
			auto baseFiles = DirectoryList(base, "");
			for (auto& baseFile : baseFiles) {
				std::string baseFileLower = baseFile;
				transform(baseFileLower.begin(), baseFileLower.end(), baseFileLower.begin(), ::tolower);

				// Loop each file of known version
				for (auto& File : KnownVersion.mFiles) {
					std::string FileLower = File.mName;
					transform(FileLower.begin(), FileLower.end(), FileLower.begin(), ::tolower);

					// See if we match
					if (baseFileLower == FileLower) {
						std::string MD5 = FileMD5(base + baseFile);

						ReleaseFiles.insert(std::make_pair(FileLower, base + baseFile));

						if (MD5 != File.mChecksum) {
							if (MD5.length() == 0) {
								//std::cout << KnownVersion.mName << ": " << KnownVersion.mFiles[FileNo].mName;
								//std::cout << " File not found\n";
							} else {
								//std::cout << "{ \"" << File.mName << "\", \"" << MD5 << "\" }, \n";
								std::cout << KnownVersion.mName << ": " << File.mName;
								std::cout << " Unknown MD5: " << MD5 << "\n";
							}
						}

						break;
					}
				}
			}

			// A very hacky method for ensuring a retail version is available, before allowing Customs
			if (KnownVersion.isCustom()) {
				if (g_Fodder->mParams->mDefaultGame == KnownVersion.mGame) {
					if (haveRetail)
						mReleasePath.insert(std::make_pair(&KnownVersion, base));
				}
			} else {

				// Ensure we atleast have found 1 file, and we have atleast the reuqired number of files, or every file with an MD5 match
				if (KnownVersion.mFiles.size() > 0 && KnownVersion.mFiles.size() == ReleaseFiles.size()) {
					if (!haveRetail)
						haveRetail = KnownVersion.isRetail();

					// if we found files, add them to our tracker
					mReleaseFiles.insert(std::make_pair(&KnownVersion, ReleaseFiles));
					mReleasePath.insert(mReleasePath.end(), std::make_pair(&KnownVersion, base));
				}
				else {
					// 
				}
			}
		}
	}
}

void cResourceMan::findSaves() {
	mSaves.clear();

	// Loop each path
	for (auto& ValidPath : mValidPaths) {
		auto basepath = ValidPath + PathGenerate("", eSave);

		auto files = DirectoryList(basepath, EXTENSION_SAVEGAME);

		for (auto& file : files) {
			mSaves.emplace(std::make_pair(file, basepath + file));
		}
	}
}

void cResourceMan::findCustomMaps() {
	mMaps.clear();

	// Loop each path
	for (auto& ValidPath : mValidPaths) {
		auto basepath = ValidPath + PathGenerate("Custom/Maps/", eData);

		if (FileExists(basepath)) {
			mCustomMapPath = basepath;

			auto files = DirectoryList(basepath, EXTENSION_MAP);

			for (auto& file : files) {
				mMaps.emplace(std::make_pair(file, basepath + file));
			}
		}
	}
}

void cResourceMan::refresh() {
	validatePaths();

	findCampaigns();
	findVersions();
	findSaves();
	findCustomMaps();
}

std::string cResourceMan::FindVersionPath(const sGameVersion* pVersion) const {

	for (auto Release = mReleasePath.begin(); Release != mReleasePath.end(); ++Release) {

		if (Release->first->mRelease == pVersion->mRelease) {
			if (Release->first->mGame == pVersion->mGame) {
				if (Release->first->mPlatform == pVersion->mPlatform || pVersion->mPlatform == ePlatform::Any) {

					return Release->second;
				}
			}
		}
	}

	return "";
}

std::string cResourceMan::GetFilePath(const sGameVersion* pVersion, std::string pFile) const {
	transform(pFile.begin(), pFile.end(), pFile.begin(), ::tolower);

	auto VerFiles = mReleaseFiles.find(pVersion);
	if (VerFiles == mReleaseFiles.end())
		return "";

	for (const auto& File : VerFiles->second) {
		if (File.first == pFile)
			return File.second;
	}

	return "";
}

std::string cResourceMan::PathGenerate(const std::string& pFile, eDataType pDataType) const {
	std::stringstream	 filePathFinal;

	switch (pDataType) {
	case eData:
		filePathFinal << "Data" << gPathSeperator;
		break;

	case eSave:
		filePathFinal << "Saves" << gPathSeperator;
		break;

	case eCampaign:
		filePathFinal << "Campaigns" << gPathSeperator;
		break;

	case eTest:
		filePathFinal << "Tests" << gPathSeperator;
		break;

	case eScript:
		filePathFinal << "Scripts" << gPathSeperator;
		break;

	case eRoot:
	case eNone:
	default:
		break;
	}

	filePathFinal << pFile;

	return filePathFinal.str();
}

std::string cResourceMan::FileMD5(const std::string& pFile) {
	md5_context ctx;
	unsigned char MD5[16];

	auto File = FileRead(pFile);
	if (!File->size())
		return "";

	md5_starts(&ctx);
	md5_update(&ctx, File->data(), (uint32)File->size());
	md5_finish(&ctx, MD5);

	std::string FinalMD5;
	FinalMD5.reserve(32);

	for (size_t i = 0; i != 16; ++i) {
		FinalMD5 += "0123456789ABCDEF"[MD5[i] / 16];
		FinalMD5 += "0123456789ABCDEF"[MD5[i] % 16];
	}

	return FinalMD5;
}

std::string	cResourceMan::FileReadStr(const std::string& pFile) {

	auto content = FileRead(pFile);
	
	return std::string((char*)content->data(), content->size());
}

tSharedBuffer cResourceMan::FileRead(const std::string& pFile) {
	std::ifstream*	fileStream;
	auto			fileBuffer = std::make_shared<std::vector<uint8_t>>();

	// Attempt to open the file
	fileStream = new std::ifstream(pFile.c_str(), std::ios::binary);
	if (fileStream->is_open() != false) {

		// Get file size
		fileStream->seekg(0, std::ios::end);
		fileBuffer->resize(static_cast<const unsigned int>(fileStream->tellg()));
		fileStream->seekg(std::ios::beg);

		// Allocate buffer, and read the file into it
		fileStream->read((char*)fileBuffer->data(), fileBuffer->size());
		if (!(*fileStream))
			fileBuffer->clear();
	}

	// Close the stream
	fileStream->close();
	delete fileStream;

	// All done ;)
	return fileBuffer;
}

std::string	cResourceMan::GetCampaignData(const std::string& pName) {
	for (auto& Campaign : mCampaigns) {
		if (Campaign.first == pName)
			return Campaign.second;
	}

	return "";
}

std::vector<const sGameVersion*> cResourceMan::GetAvailable() const {
	std::vector<const sGameVersion*> results;
	for (auto release : mReleasePath) {
		results.push_back(release.first);
	}

	return results;
}

std::vector<std::string> cResourceMan::GetCampaigns() const {
	std::vector<std::string> results;
	for (auto& Campaign : mCampaigns)
		results.push_back(Campaign.first);

	return results;
}

std::vector<std::string> cResourceMan::GetSaves() const {
	std::vector<std::string> results;
	for (auto& Campaign : mSaves)
		results.push_back(Campaign.first);

	return results;
}

std::vector<std::string> cResourceMan::GetMaps() const {
	std::vector<std::string> results;

	for (auto& Campaign : mMaps)
		results.push_back(Campaign.first);

	return results;
}

std::string cResourceMan::GetSaveNewName() const {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	return mValidPaths[0] + PathGenerate( std::to_string(in_time_t) + EXTENSION_SAVEGAME, eSave);
}

std::string cResourceMan::GetScriptPath(const std::string& pType) const {
	
	for (auto& ValidPath : mValidPaths) {
		auto basepath = ValidPath + PathGenerate(pType, eScript);
		if (FileExists(basepath))
			return basepath;
	}
	return "";
}

std::string cResourceMan::GetTestPath(const sGameVersion* pVersion, const std::string pFile) const {
	for (auto& ValidPath : mValidPaths) {
		auto basepath = ValidPath + PathGenerate(pFile, eTest);
		if (FileExists(basepath))
			return basepath;
	}
	return "";
}

std::string cResourceMan::GetAboutFile() const {
	for (auto& ValidPath : mValidPaths) {
		auto basepath = ValidPath + "about.png";
		if (FileExists(basepath))
			return basepath;
	}

	return "";
}

std::string cResourceMan::GetWavPath(const std::string& pFile) const {
	for (auto& ValidPath : mValidPaths) {
		auto basepath = ValidPath + PathGenerate("WAV/" + pFile, eData);
		if (FileExists(basepath))
			return basepath;
	}

	return "";
}

std::string cResourceMan::GetMapPath(const std::string& pName) const {
	for (auto& Map : mMaps) {
		if (Map.first == pName)
			return Map.second;
	}
	return "";
}

std::string cResourceMan::GetSave(const std::string &pName) const {
	for (auto& Save : mSaves) {
		if (Save.first == pName)
			return Save.second;
	}
	return "";
}

bool cResourceMan::isDataAvailable() const {
	return mReleasePath.size() > 0;
}

bool cResourceMan::isCampaignAvailable(std::string pName) const {
	return (mCampaigns.find(pName) != mCampaigns.end());
}

std::vector<std::string> cResourceMan::getValidPaths() const {
	return mValidPaths;
}
std::vector<std::string> cResourceMan::getAllPaths() const {
	return mAllPaths;
}

bool cResourceMan::FileExists(const std::string& pPath) const {
	struct stat info;

	if (stat(pPath.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	else if (info.st_mode & S_IFMT)
		return true;

	return false;
}

#ifdef WIN32
#include "Windows.h"
#include <direct.h>

std::string cResourceMan::getcwd() {
	char buff[1024];
	_getcwd(buff, 1024);
	std::string cwd(buff);
	return cwd;
}

std::vector<std::string> cResourceMan::DirectoryList(const std::string& pPath, const std::string& pExtension) {
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;
	std::vector<std::string> results;

	// Build the file path
	std::stringstream finalPath;

	if (pPath.size())
		finalPath << pPath;

	finalPath << "/*" << pExtension;

	size_t size = MultiByteToWideChar(0, 0, finalPath.str().c_str(), (int)finalPath.str().length(), 0, 0);
	WCHAR    *pathFin = new WCHAR[size + 1];
	memset(pathFin, 0, size + 1);

	size = MultiByteToWideChar(0, 0, finalPath.str().c_str(), (int)size, pathFin, (int)size);
	pathFin[size] = 0;

	if ((dhandle = FindFirstFile(pathFin, &fdata)) == INVALID_HANDLE_VALUE) {
		delete pathFin;
		return results;
	}

	delete pathFin;
	size_t tmp = 0;

	{
		char *file = new char[wcslen(fdata.cFileName) + 1];
		memset(file, 0, wcslen(fdata.cFileName) + 1);

		wcstombs_s(&tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName));
		results.push_back(std::string(file));
		delete file;
	}

	while (1) {
		if (FindNextFile(dhandle, &fdata)) {
			char *file = new char[wcslen(fdata.cFileName) + 1];
			memset(file, 0, wcslen(fdata.cFileName) + 1);

			wcstombs_s(&tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName));
			results.push_back(std::string(file));
			delete file;

		}
		else {
			if (GetLastError() == ERROR_NO_MORE_FILES) {
				break;
			}
			else {
				FindClose(dhandle);
				return results;
			}
		}
	}

	FindClose(dhandle);

	return results;
}

#else
#include <dirent.h>
std::string findType;

std::string cResourceMan::getcwd() {
	char buff[1024];
	::getcwd(buff, 1024);
	std::string cwd(buff);
	return cwd;
}

int file_select(const struct dirent *entry) {
	std::string name = entry->d_name;

	transform(name.begin(), name.end(), name.begin(), ::toupper);

	if (name.find(findType) == std::string::npos)
		return false;

	return true;
}

std::vector<std::string> cResourceMan::DirectoryList(const std::string& pPath, const std::string& pExtension) {
	struct dirent		**directFiles;
	std::vector<std::string>		  results;

	// Build the file path
	std::stringstream finalPath;

	finalPath << pPath << "/";

	findType = pExtension;

	transform(findType.begin(), findType.end(), findType.begin(), ::toupper);

	int count = scandir(finalPath.str().c_str(), (dirent***)&directFiles, file_select, 0);

	for (int i = 0; i < count; ++i) {

		results.push_back(std::string(directFiles[i]->d_name));
	}

	transform(findType.begin(), findType.end(), findType.begin(), ::tolower);

	count = scandir(finalPath.str().c_str(), (dirent***)&directFiles, file_select, 0);

	for (int i = 0; i < count; ++i) {

		results.push_back(std::string(directFiles[i]->d_name));
	}

	return results;
}

#endif