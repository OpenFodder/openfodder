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
#include "Setup/DataRelease.hpp"
#include "Setup/EngineVersion.hpp"
#include "Setup/InstallTransaction.hpp"
#include "Utils/json.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <system_error>

using Json = nlohmann::json;

namespace Setup {

// ---------------------------------------------------------------------------
// Repo identity. Centralised so callers can refer to the "data" / "scripts"
// repos by name without sprinkling string literals across the codebase.
// ---------------------------------------------------------------------------
static constexpr const char* kDataRepoOwner       = "OpenFodder";
static constexpr const char* kDataRepoName        = "data";
static constexpr const char* kScriptsRepoOwner    = "OpenFodder";
static constexpr const char* kScriptsRepoName     = "scripts";
static constexpr const char* kInstalledManifestFile = "installed.json";

const char* DataRelease::DataRepoOwner()     { return kDataRepoOwner; }
const char* DataRelease::DataRepoName()      { return kDataRepoName; }
const char* DataRelease::ScriptsRepoOwner()  { return kScriptsRepoOwner; }
const char* DataRelease::ScriptsRepoName()   { return kScriptsRepoName; }

namespace {

// Case-insensitive ".zip" suffix test — GitHub asset names usually arrive in
// the case the uploader chose, so we don't assume lower-case.
bool EndsWithZip(const std::string& pName) {
    if (pName.size() < 4)
        return false;
    const auto tail = pName.substr(pName.size() - 4);
    for (size_t i = 0; i < 4; ++i) {
        const char want = ".zip"[i];
        const char have = tail[i];
        const char lower = (have >= 'A' && have <= 'Z') ? char(have - 'A' + 'a') : have;
        if (lower != want)
            return false;
    }
    return true;
}

// JSON numeric conversion permits bools, fractions, and narrowing. Validate
// before conversion so malformed metadata cannot become a compatible version.
int VersionField(const Json& record, const char* key) {
    const auto value = record.find(key);
    if (value == record.end())
        return 0; // Legacy installed records may omit the other subtree's field.
    if (!value->is_number_integer())
        throw std::runtime_error(std::string(key) + " must be a non-negative integer");
    if (value->is_number_unsigned()) {
        const auto number = value->get<uint64_t>();
        if (number <= static_cast<uint64_t>(std::numeric_limits<int>::max()))
            return static_cast<int>(number);
    } else {
        const auto number = value->get<int64_t>();
        if (number >= 0 && number <= std::numeric_limits<int>::max())
            return static_cast<int>(number);
    }
    throw std::runtime_error(std::string(key) + " is outside the supported integer range");
}

void ReadVersions(const Json& record, int& dataVersion, int& scriptVersion) {
    if (!record.is_object())
        throw std::runtime_error("metadata must be a JSON object");
    const int dv = VersionField(record, "dataVersion");
    const int sv = VersionField(record, "scriptVersion");
    dataVersion = dv;
    scriptVersion = sv;
}

std::string TextField(const Json& record, const char* key, bool required = false, bool allowNull = false) {
    const auto value = record.find(key);
    if (value == record.end() || (allowNull && value->is_null())) {
        if (!required) return {};
        throw std::runtime_error(std::string("missing ") + key);
    }
    if (!value->is_string())
        throw std::runtime_error(std::string(key) + " must be a string");
    auto text = value->get<std::string>();
    if (required && text.empty())
        throw std::runtime_error(std::string(key) + " must not be empty");
    return text;
}

void RequireVersion(const ReleaseManifest& manifest) {
    if (manifest.mDataVersion < 0 || manifest.mScriptVersion < 0)
        throw std::runtime_error("release versions must be non-negative");
    if ((manifest.mRepo == kDataRepoName && manifest.mDataVersion == 0) ||
        (manifest.mRepo == kScriptsRepoName && manifest.mScriptVersion == 0) ||
        (manifest.mDataVersion == 0 && manifest.mScriptVersion == 0))
        throw std::runtime_error("release is missing its compatibility version");
}

void ValidateLayout(const std::set<std::string>& files, const ReleaseManifest& manifest) {
    if (manifest.mRepo == kScriptsRepoName) {
        for (const auto* name : {"Settings.js", "random.js", "multiplayer.js", "mapeditor.js",
                                "Common/Classes.js", "Common/OpenFodder.js"}) {
            if (!files.count(name))
                throw std::runtime_error(std::string("scripts archive is missing root file ") + name);
        }
    } else {
        // Data is installed into Data/, not the repository root. A wrapper
        // directory or source-only archive must not count as a data install.
        bool hasTiles = false;
        for (const auto& name : files) {
            const auto slash = name.find('/');
            auto suffix = name.size() > 4 ? name.substr(name.size() - 4) : std::string{};
            std::transform(suffix.begin(), suffix.end(), suffix.begin(), [](unsigned char c) {
                return c >= 'A' && c <= 'Z' ? char(c - 'A' + 'a') : char(c);
            });
            if (slash != std::string::npos && name.find('/', slash + 1) == std::string::npos &&
                suffix == ".blk")
                hasTiles = true;
        }
        if (!hasTiles)
            throw std::runtime_error("data archive has no tilesets directly inside a release folder");
    }
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// QueryLatest
// ---------------------------------------------------------------------------
bool DataRelease::QueryLatest(const std::string& pOwner, const std::string& pRepo, ReleaseManifest& pOut) {
    mLastError.clear();
    pOut = ReleaseManifest{};
    try {
        ReleaseManifest result;
        result.mOwner = pOwner;
        result.mRepo = pRepo;
        const std::string url = "https://api.github.com/repos/" + pOwner + "/" + pRepo + "/releases/latest";
        HttpResponse response;
        if (!mHttp.Get(url, response) || response.mStatus != 200)
            throw std::runtime_error("GET " + url + " failed: " + mHttp.LastError());
        const auto root = Json::parse(response.mBody.begin(), response.mBody.end());
        if (!root.is_object())
            throw std::runtime_error("releases JSON is not an object");
        result.mTagName = TextField(root, "tag_name", true);
        result.mReleaseNotes = TextField(root, "body", false, true);
        if (!root.contains("assets") || !root["assets"].is_array())
            throw std::runtime_error("releases JSON missing assets array");
        for (const auto& asset : root["assets"]) {
            if (!asset.is_object())
                throw std::runtime_error("release asset is not an object");
            const auto name = TextField(asset, "name", true);
            const auto downloadUrl = TextField(asset, "browser_download_url", true);
            if (name == "compat.json")
                result.mCompatUrl = downloadUrl;
            else if (EndsWithZip(name) && result.mArchiveUrl.empty())
                result.mArchiveUrl = downloadUrl;
        }
        if (result.mArchiveUrl.empty())
            throw std::runtime_error("release '" + result.mTagName + "' has no .zip asset");
        if (result.mCompatUrl.empty())
            throw std::runtime_error("release '" + result.mTagName + "' has no compat.json asset");
        HttpResponse compatResponse;
        if (!mHttp.Get(result.mCompatUrl, compatResponse) || compatResponse.mStatus != 200)
            throw std::runtime_error("GET compat.json failed: " + mHttp.LastError());
        const auto compat = Json::parse(compatResponse.mBody.begin(), compatResponse.mBody.end());
        ReadVersions(compat, result.mDataVersion, result.mScriptVersion);
        RequireVersion(result);
        pOut = std::move(result); // Publish only a fully validated result.
        return true;
    } catch (const std::exception& ex) {
        mLastError = std::string("release query failed: ") + ex.what();
        return false;
    }
}

// ---------------------------------------------------------------------------
// FetchAndInstall
// ---------------------------------------------------------------------------
bool DataRelease::FetchAndInstall(const ReleaseManifest& pManifest,
                                  const std::string& pTargetDir,
                                  ProgressCallback pHttpProgress,
                                  ZipProgressCallback pZipProgress) {
    mLastError.clear();
    try {
        if (pManifest.mArchiveUrl.empty())
            throw std::runtime_error("manifest has no archive URL");
        RequireVersion(pManifest);
        if (pManifest.mDataVersion && !IsDataVersionCompatible(pManifest.mDataVersion))
            throw std::runtime_error("incompatible data version " + std::to_string(pManifest.mDataVersion));
        if (pManifest.mScriptVersion && !IsScriptVersionCompatible(pManifest.mScriptVersion))
            throw std::runtime_error("incompatible script version " + std::to_string(pManifest.mScriptVersion));

        InstallTransaction transaction(pTargetDir);
        transaction.Prepare();
        if (!mHttp.Download(pManifest.mArchiveUrl, transaction.Archive().string(), pHttpProgress))
            throw std::runtime_error("download failed: " + mHttp.LastError());

        std::set<std::string> files;
        {
            // Close the ZIP before transaction cleanup, including on failure.
            ZipArchive zip;
            if (!zip.OpenFromFile(transaction.Archive().string()))
                throw std::runtime_error("open zip failed: " + zip.LastError());
            std::vector<std::string> entries;
            if (!zip.ListEntries(entries))
                throw std::runtime_error("list zip failed: " + zip.LastError());
            for (auto name : entries) {
                std::replace(name.begin(), name.end(), '\\', '/');
                if (!name.empty() && name.back() != '/') {
                    name = std::filesystem::path(name).lexically_normal().generic_string();
                    auto lower = name;
                    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
                        return c >= 'A' && c <= 'Z' ? char(c - 'A' + 'a') : char(c);
                    });
                    if (lower == kInstalledManifestFile && name != kInstalledManifestFile)
                        throw std::runtime_error("archive manifest must be named installed.json (lower case)");
                    files.insert(name);
                }
            }
            if (!zip.ExtractAllTo(transaction.Staging().string(), pZipProgress))
                throw std::runtime_error("extract failed: " + zip.LastError());
        }
        ValidateLayout(files, pManifest);
        // An old installed record was copied into staging too. Only compare
        // metadata that the new archive actually supplied.
        if (files.count(kInstalledManifestFile)) {
            int dv = 0, sv = 0;
            if (!ReadInstalledManifest(transaction.Staging().string(), dv, sv))
                throw std::runtime_error(mLastError);
            if (dv != pManifest.mDataVersion || sv != pManifest.mScriptVersion)
                throw std::runtime_error("archive installed.json disagrees with compat.json");
        }
        if (!WriteInstalledManifest(transaction.Staging().string(), pManifest))
            throw std::runtime_error(mLastError);
        transaction.Commit();
        return true;
    } catch (const std::exception& ex) {
        mLastError = std::string("install failed: ") + ex.what();
        return false;
    }
}

// ---------------------------------------------------------------------------
// WriteInstalledManifest
// ---------------------------------------------------------------------------
bool DataRelease::WriteInstalledManifest(const std::string& pTargetDir, const ReleaseManifest& pManifest) {
    mLastError.clear();
    try {
        if (pTargetDir.empty())
            throw std::runtime_error("manifest target is empty");
        Json record = {
            {"owner", pManifest.mOwner}, {"repo", pManifest.mRepo}, {"tagName", pManifest.mTagName},
            {"dataVersion", pManifest.mDataVersion}, {"scriptVersion", pManifest.mScriptVersion},
        };
        int dv = 0, sv = 0;
        ReadVersions(record, dv, sv);
        const auto serialized = record.dump(2); // Validate before opening a file.
        std::filesystem::create_directories(pTargetDir);
        const auto path = std::filesystem::path(pTargetDir) / kInstalledManifestFile;
        if (std::filesystem::is_symlink(std::filesystem::symlink_status(path)))
            throw std::runtime_error("installed.json must not be a symbolic link");
        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        if (!out)
            throw std::runtime_error("open " + path.string() + " for write failed");
        out << serialized;
        out.flush();
        if (!out)
            throw std::runtime_error("write " + path.string() + " failed");
        out.close();
        if (!out)
            throw std::runtime_error("close " + path.string() + " failed");
        return true;
    } catch (const std::exception& ex) {
        mLastError = std::string("write installed.json failed: ") + ex.what();
        return false;
    }
}

bool DataRelease::ReadInstalledManifest(const std::string& pTargetDir, int& pDataVersion, int& pScriptVersion) {
    mLastError.clear();
    pDataVersion = 0;
    pScriptVersion = 0;
    try {
        const auto path = std::filesystem::path(pTargetDir) / kInstalledManifestFile;
        std::ifstream in(path, std::ios::binary);
        if (!in)
            throw std::runtime_error("cannot read " + path.string());
        const auto record = Json::parse(in);
        ReadVersions(record, pDataVersion, pScriptVersion);
        return true;
    } catch (const std::exception& ex) {
        mLastError = std::string("read installed.json failed: ") + ex.what();
        return false;
    }
}

} // namespace Setup
