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
#include "Utils/md5.hpp"
#include "Setup/MountedImage.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
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

bool cResourceMan::addDirOnce(const std::string& pPath) {
	// Skip duplicates so the wizard can call refresh() repeatedly without
	// piling up stale [paths] entries. Comparison is exact-string; the caller
	// is responsible for canonicalising before passing in.
	for (const auto& existing : mAllPaths) {
		if (existing == pPath)
			return false;
	}
	mAllPaths.push_back(pPath);
	return true;
}

bool cResourceMan::addUserDir(const std::string& pPath) {
	// Track the path under mUserPaths (so SaveIni writes it back) and also
	// add it to the search-path list for refresh().
	bool added = false;
	bool inUser = false;
	for (const auto& existing : mUserPaths) {
		if (existing == pPath) { inUser = true; break; }
	}
	if (!inUser) {
		mUserPaths.push_back(pPath);
		added = true;
	}
	if (addDirOnce(pPath))
		added = true;
	return added;
}

bool cResourceMan::removeDir(const std::string& pPath) {
	for (auto it = mAllPaths.begin(); it != mAllPaths.end(); ++it) {
		if (*it == pPath) {
			mAllPaths.erase(it);
			return true;
		}
	}
	return false;
}

bool cResourceMan::removeUserDir(const std::string& pPath) {
	bool removed = false;
	for (auto it = mUserPaths.begin(); it != mUserPaths.end(); ++it) {
		if (*it == pPath) {
			mUserPaths.erase(it);
			removed = true;
			break;
		}
	}
	if (removeDir(pPath))
		removed = true;
	return removed;
}

int cResourceMan::mountImage(const std::string& pImagePath) {
	auto img = std::make_shared<Setup::cMountedImage>();
	if (!img->Open(pImagePath))
		return 0;

	const int id = Setup::MountedImageRegister(img);
	if (id == 0)
		return 0;

	// Match the indexed image contents against the KnownGameVersions table.
	// findVersions does the same job on a real folder; we duplicate the
	// matching logic here because we want to point the version's file map at
	// virtual paths ("firy://<id>/<file>") instead of host paths.
	bool addedAnyVersion = false;
	bool haveRetailRegistered = false;

	for (size_t i = 0; i < 20; ++i) {
		const sGameVersion& kv = ::KnownGameVersions[i];
		if (kv.mFiles.empty())
			continue;     // pseudo-versions (Custom/Random) — handled below

		// Skip versions already registered (e.g. retail data is on disk AND
		// we just mounted a CD with the same release files).
		if (mReleaseFiles.find(&kv) != mReleaseFiles.end()) {
			if (kv.mRelease == eRelease::Retail)
				haveRetailRegistered = true;
			continue;
		}

		tStringMap matched;
		for (auto& kf : kv.mFiles) {
			std::string wantLower = kf.mName;
			std::transform(wantLower.begin(), wantLower.end(), wantLower.begin(), ::tolower);
			std::string inImagePath = img->ResolveBasename(wantLower);
			if (inImagePath.empty())
				continue;
			matched.emplace(wantLower, Setup::VirtualPathBuild(id, inImagePath));
		}

		if (matched.size() == kv.mFiles.size()) {
			// Synthetic "release path" — never used as a host path because
			// FileRead routes through the version's mReleaseFiles entries
			// (which are already firy:// urls). Keeping it virtual avoids
			// confusing diagnostic code that looks at mReleasePath.
			std::string virtBase = "firy://" + std::to_string(id) + "/";
			mReleasePath.emplace(&kv, virtBase);
			mReleaseFiles.emplace(&kv, std::move(matched));

			if (kv.mRelease == eRelease::Retail)
				haveRetailRegistered = true;
			addedAnyVersion = true;
		}
	}

	// Custom/Random pseudo-versions need a retail of the same eGame to be
	// registered first — mirrors the rule findVersions enforces.
	if (haveRetailRegistered && g_Fodder && g_Fodder->mParams) {
		for (size_t i = 0; i < 20; ++i) {
			const sGameVersion& kv = ::KnownGameVersions[i];
			if (!kv.isCustom())
				continue;
			if (kv.mGame != g_Fodder->mParams->mDefaultGame)
				continue;
			if (mReleasePath.find(&kv) != mReleasePath.end())
				continue;
			// Keep the same shape findVersions uses: a synthetic base path
			// for Custom/Random.
			mReleasePath.emplace(&kv, "firy://" + std::to_string(id) + "/");
		}
	}

	if (!addedAnyVersion) {
		// Image opened but didn't match any known version. Drop it — a
		// stranded entry in the registry would survive forever.
		Setup::MountedImageUnregister(id);
		return 0;
	}

	return id;
}

bool cResourceMan::unmountImage(int pId) {
	if (pId <= 0)
		return false;

	const std::string virtPrefix = "firy://" + std::to_string(pId) + "/";

	// Drop any release entries that point at this image.
	for (auto it = mReleasePath.begin(); it != mReleasePath.end(); ) {
		if (it->second.size() >= virtPrefix.size() &&
			it->second.compare(0, virtPrefix.size(), virtPrefix) == 0) {
			it = mReleasePath.erase(it);
		} else {
			++it;
		}
	}

	for (auto it = mReleaseFiles.begin(); it != mReleaseFiles.end(); ) {
		bool any = false;
		for (auto& kv : it->second) {
			if (kv.second.size() >= virtPrefix.size() &&
				kv.second.compare(0, virtPrefix.size(), virtPrefix) == 0) {
				any = true;
				break;
			}
		}
		if (any)
			it = mReleaseFiles.erase(it);
		else
			++it;
	}

	Setup::MountedImageUnregister(pId);
	return true;
}

std::vector<std::string> cResourceMan::getMountedImagePaths() const {
	std::vector<std::string> out;
	for (auto& kv : Setup::MountedImagesAll())
		out.push_back(kv.second->ImagePath());
	return out;
}

std::vector<sSearchRoot> cResourceMan::getSearchRoots() const {
	// Walk mAllPaths and classify each entry. Order matters:
	//   1. Implicit defaults appear first (addDefaultDirs is run by ctor
	//      before any addUserDir / mountImage call), so we can identify
	//      them positionally.
	//   2. User-configured paths follow (addUserDir was called for each
	//      [paths] entry in ProcessINI).
	//   3. Mounted-image search roots come from mountImage() — they don't
	//      live in mAllPaths at all (they're indexed by id), so we append
	//      them from the MountedImage registry.
	std::vector<sSearchRoot> out;
	out.reserve(mAllPaths.size() + 4);

	// Build a quick lookup of which mAllPaths entries are also in mUserPaths.
	for (auto& p : mAllPaths) {
		sSearchRoot r;
		r.mPath = p;

		bool isUser = false;
		for (auto& u : mUserPaths) {
			if (u == p) { isUser = true; break; }
		}
		r.mKind = isUser ? ePathKind::UserConfigured : ePathKind::ImplicitDefault;

		// Label heuristic: cwd → "Working dir"; %USERPROFILE%/Documents/...
		// → "Documents"; user paths → just show the leaf so the wizard
		// row stays compact.
		if (r.mKind == ePathKind::ImplicitDefault) {
			r.mLabel = "Default";
		} else {
			r.mLabel = "User";
		}
		out.push_back(std::move(r));
	}

	// Append mounted-image roots.
	for (auto& kv : Setup::MountedImagesAll()) {
		sSearchRoot r;
		r.mKind = ePathKind::MountedImage;
		r.mPath = "firy://" + std::to_string(kv.first) + "/";
		// Label is the host filename (last path component) for human reading.
		std::string p = kv.second->ImagePath();
		std::replace(p.begin(), p.end(), '\\', '/');
		size_t slash = p.find_last_of('/');
		r.mLabel = "Image: " + ((slash == std::string::npos) ? p : p.substr(slash + 1));
		out.push_back(std::move(r));
	}

	return out;
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
	//Per-user data dir (Flatpak: ~/.var/app/<app-id>/data)
	char* path1 = std::getenv("XDG_DATA_HOME");
    if (path1 && *path1) {
        addBaseDir(std::string(path1) + "/");
    }

	//XDG_DATA_DIRS
	path1 = std::getenv("XDG_DATA_DIRS");
	if (path1) {
		std::stringstream ss;
		ss << path1;
		while (ss.good()) {
			std::string substr;
			std::getline(ss, substr, ':');
			addBaseDir(substr);
		}
	}
	//HOME -> ~/.local/share
	path1 = std::getenv("HOME");
	if (path1) {
		path = path1;
		addBaseDir(path + "/.local/share/");
	}
	//Fallback to /usr/local/share
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

	// Clear release entries that came from disk-folder scans on a previous
	// refresh, but PRESERVE entries that point at currently-mounted disk
	// images — those are owned by mountImage() and would otherwise vanish
	// every time the wizard or a save-options click triggers a refresh.
	for (auto it = mReleasePath.begin(); it != mReleasePath.end(); ) {
		if (Setup::VirtualPathIs(it->second)) {
			++it;
			continue;
		}
		// Keep mReleaseFiles in sync.
		mReleaseFiles.erase(it->first);
		it = mReleasePath.erase(it);
	}

	// If a mounted image already registered a retail version, the
	// haveRetail gate further down should skip Custom/Random re-creation.
	for (auto& kv : mReleasePath) {
		if (kv.first->mRelease == eRelease::Retail) {
			haveRetail = true;
			break;
		}
	}

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
	// "firy://<id>/<inImagePath>" virtual paths are routed through the
	// MountedImage registry instead of going through std::ifstream — they
	// don't exist on the host filesystem at all.
	if (Setup::VirtualPathIs(pFile)) {
		int id = 0;
		std::string inImagePath;
		if (Setup::VirtualPathParse(pFile, id, inImagePath)) {
			if (auto image = Setup::MountedImageGet(id))
				return image->Read(inImagePath);
		}
		return std::make_shared<std::vector<uint8_t>>();
	}

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

bool cResourceMan::DeleteSave(const std::string& pName) {
	const std::string SavePath = GetSave(pName);
	if (!SavePath.size())
		return false;

	if (std::remove(SavePath.c_str()) != 0)
		return false;

	refresh();
	return true;
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
std::vector<std::string> cResourceMan::getUserPaths() const {
	return mUserPaths;
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

bool cResourceMan::DirExists(const std::string& pPath) const {
	// Stricter than FileExists — only returns true for a real directory.
	struct stat info;

	if (stat(pPath.c_str(), &info) != 0)
		return false;
	return (info.st_mode & S_IFDIR) != 0;
}

static void ResourceMan_SortDirectoryList(std::vector<std::string>& pResults) {
	std::sort(pResults.begin(), pResults.end());
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
		delete[] pathFin;
		return results;
	}

	delete[] pathFin;
	size_t tmp = 0;

	{
		char *file = new char[wcslen(fdata.cFileName) + 1];
		memset(file, 0, wcslen(fdata.cFileName) + 1);

		wcstombs_s(&tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName));
		results.push_back(std::string(file));
		delete[] file;
	}

	while (1) {
		if (FindNextFile(dhandle, &fdata)) {
			char *file = new char[wcslen(fdata.cFileName) + 1];
			memset(file, 0, wcslen(fdata.cFileName) + 1);

			wcstombs_s(&tmp, file, wcslen(fdata.cFileName) + 1, fdata.cFileName, wcslen(fdata.cFileName));
			results.push_back(std::string(file));
			delete[] file;

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

	ResourceMan_SortDirectoryList(results);
	return results;
}

std::vector<std::string> cResourceMan::DirectoryListDirs(const std::string& pPath) {
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;
	std::vector<std::string> results;

	std::stringstream finalPath;
	if (pPath.size())
		finalPath << pPath;
	finalPath << "/*";

	int wlen = MultiByteToWideChar(0, 0, finalPath.str().c_str(), (int)finalPath.str().length(), 0, 0);
	WCHAR* pathFin = new WCHAR[wlen + 1];
	memset(pathFin, 0, sizeof(WCHAR) * (wlen + 1));
	MultiByteToWideChar(0, 0, finalPath.str().c_str(), (int)finalPath.str().length(), pathFin, wlen);
	pathFin[wlen] = 0;

	if ((dhandle = FindFirstFile(pathFin, &fdata)) == INVALID_HANDLE_VALUE) {
		delete[] pathFin;
		return results;
	}
	delete[] pathFin;

	auto pushIfDir = [&results](const WIN32_FIND_DATA& f) {
		if ((f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			return;
		// Skip "." and ".."
		if (f.cFileName[0] == L'.' && (f.cFileName[1] == 0 ||
			(f.cFileName[1] == L'.' && f.cFileName[2] == 0)))
			return;

		size_t tmp = 0;
		size_t len = wcslen(f.cFileName);
		char* file = new char[len + 1];
		memset(file, 0, len + 1);
		wcstombs_s(&tmp, file, len + 1, f.cFileName, len);
		results.push_back(std::string(file));
		delete[] file;
	};

	pushIfDir(fdata);
	while (FindNextFile(dhandle, &fdata)) {
		pushIfDir(fdata);
	}

	FindClose(dhandle);
	ResourceMan_SortDirectoryList(results);
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
		free(directFiles[i]);
	}
	if (count >= 0)
		free(directFiles);

	transform(findType.begin(), findType.end(), findType.begin(), ::tolower);

	count = scandir(finalPath.str().c_str(), (dirent***)&directFiles, file_select, 0);

	for (int i = 0; i < count; ++i) {

		results.push_back(std::string(directFiles[i]->d_name));
		free(directFiles[i]);
	}
	if (count >= 0)
		free(directFiles);

	ResourceMan_SortDirectoryList(results);
	return results;
}

std::vector<std::string> cResourceMan::DirectoryListDirs(const std::string& pPath) {
	std::vector<std::string> results;
	std::string base = pPath;
	if (base.empty() || base.back() != '/')
		base.append("/");

	DIR* dir = opendir(base.c_str());
	if (!dir)
		return results;

	while (struct dirent* entry = readdir(dir)) {
		// Skip "." and ".."
		if (entry->d_name[0] == '.' &&
			(entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
			continue;

		bool isDir = false;
#ifdef DT_DIR
		if (entry->d_type == DT_DIR) {
			isDir = true;
		} else if (entry->d_type == DT_UNKNOWN) {
#endif
			struct stat st;
			if (::stat((base + entry->d_name).c_str(), &st) == 0 && (st.st_mode & S_IFDIR))
				isDir = true;
#ifdef DT_DIR
		}
#endif
		if (isDir)
			results.push_back(std::string(entry->d_name));
	}
	closedir(dir);

	ResourceMan_SortDirectoryList(results);
	return results;
}

#endif
