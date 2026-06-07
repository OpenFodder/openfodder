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
#include "Utils/json.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>
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

} // anonymous namespace

// ---------------------------------------------------------------------------
// QueryLatest
// ---------------------------------------------------------------------------
bool DataRelease::QueryLatest(const std::string& pOwner, const std::string& pRepo, ReleaseManifest& pOut) {
    mLastError.clear();
    pOut = ReleaseManifest{};
    pOut.mOwner = pOwner;
    pOut.mRepo  = pRepo;

    const std::string url = "https://api.github.com/repos/" + pOwner + "/" + pRepo + "/releases/latest";

    HttpResponse response;
    if (!mHttp.Get(url, response)) {
        mLastError = "GET " + url + " failed: " + mHttp.LastError();
        return false;
    }
    if (response.mStatus != 200) {
        mLastError = "GET " + url + " returned HTTP " + std::to_string(response.mStatus);
        return false;
    }

    Json root;
    try {
        root = Json::parse(response.mBody.begin(), response.mBody.end());
    } catch (const std::exception& ex) {
        mLastError = std::string("malformed releases JSON: ") + ex.what();
        return false;
    }

    if (!root.is_object()) {
        mLastError = "releases JSON is not an object";
        return false;
    }

    pOut.mTagName       = root.value("tag_name", std::string{});
    pOut.mReleaseNotes  = root.value("body",     std::string{});

    if (!root.contains("assets") || !root["assets"].is_array()) {
        mLastError = "releases JSON missing assets array";
        return false;
    }

    for (const auto& asset : root["assets"]) {
        if (!asset.is_object())
            continue;
        const std::string name = asset.value("name", std::string{});
        const std::string downloadUrl = asset.value("browser_download_url", std::string{});
        if (name.empty() || downloadUrl.empty())
            continue;

        if (name == "compat.json") {
            pOut.mCompatUrl = downloadUrl;
        } else if (EndsWithZip(name) && pOut.mArchiveUrl.empty()) {
            pOut.mArchiveUrl = downloadUrl;
        }
    }

    if (pOut.mArchiveUrl.empty()) {
        mLastError = "release '" + pOut.mTagName + "' has no .zip asset";
        return false;
    }

    // Optional compat.json fetch. If the asset isn't present the caller gets
    // dataVersion = scriptVersion = 0 and FetchAndInstall will skip the gate.
    if (!pOut.mCompatUrl.empty()) {
        HttpResponse compatResponse;
        if (!mHttp.Get(pOut.mCompatUrl, compatResponse)) {
            mLastError = "GET " + pOut.mCompatUrl + " failed: " + mHttp.LastError();
            return false;
        }
        if (compatResponse.mStatus != 200) {
            mLastError = "GET compat.json returned HTTP " + std::to_string(compatResponse.mStatus);
            return false;
        }

        Json compat;
        try {
            compat = Json::parse(compatResponse.mBody.begin(), compatResponse.mBody.end());
        } catch (const std::exception& ex) {
            mLastError = std::string("malformed compat.json: ") + ex.what();
            return false;
        }

        if (!compat.is_object()) {
            mLastError = "compat.json is not an object";
            return false;
        }

        pOut.mDataVersion   = compat.value("dataVersion",   0);
        pOut.mScriptVersion = compat.value("scriptVersion", 0);
    }

    return true;
}

// ---------------------------------------------------------------------------
// FetchAndInstall
// ---------------------------------------------------------------------------
bool DataRelease::FetchAndInstall(const ReleaseManifest& pManifest,
                                  const std::string& pTargetDir,
                                  ProgressCallback pHttpProgress,
                                  ZipProgressCallback pZipProgress) {
    mLastError.clear();

    if (pManifest.mArchiveUrl.empty()) {
        mLastError = "manifest has no archive URL";
        return false;
    }

    // 1. Compatibility gate. Version 0 means "no compat.json was present" —
    //    let it through; the caller (setup wizard) is expected to surface a
    //    softer warning before reaching this point.
    if (pManifest.mDataVersion != 0 && !IsDataVersionCompatible(pManifest.mDataVersion)) {
        mLastError = "data version " + std::to_string(pManifest.mDataVersion) +
                     " incompatible (need [" + std::to_string(kDataVersionMin) + "," +
                     std::to_string(kDataVersionMax) + "])";
        return false;
    }
    if (pManifest.mScriptVersion != 0 && !IsScriptVersionCompatible(pManifest.mScriptVersion)) {
        mLastError = "script version " + std::to_string(pManifest.mScriptVersion) +
                     " incompatible (need [" + std::to_string(kScriptVersionMin) + "," +
                     std::to_string(kScriptVersionMax) + "])";
        return false;
    }

    // Ensure the target directory exists before we try to drop a temp file
    // into it — std::filesystem won't auto-create parents on the open below.
    std::error_code ec;
    std::filesystem::create_directories(pTargetDir, ec);
    if (ec) {
        mLastError = "create_directories(" + pTargetDir + ") failed: " + ec.message();
        return false;
    }

    const std::filesystem::path tempZip =
        std::filesystem::path(pTargetDir) / ".openfodder_release.tmp.zip";
    const std::string tempZipStr = tempZip.string();

    // 2. Stream the archive to disk.
    if (!mHttp.Download(pManifest.mArchiveUrl, tempZipStr, pHttpProgress)) {
        mLastError = "download " + pManifest.mArchiveUrl + " failed: " + mHttp.LastError();
        std::filesystem::remove(tempZip, ec); // best-effort cleanup
        return false;
    }

    // 3-4. Open and extract.
    {
        ZipArchive zip;
        if (!zip.OpenFromFile(tempZipStr)) {
            mLastError = "open zip failed: " + zip.LastError();
            std::filesystem::remove(tempZip, ec);
            return false;
        }
        if (!zip.ExtractAllTo(pTargetDir, pZipProgress)) {
            mLastError = "extract failed: " + zip.LastError();
            std::filesystem::remove(tempZip, ec);
            return false;
        }
    }

    // 5. Temp file gone — its job is done. Failure here is non-fatal but
    //    surfaced so the caller can warn the user about a stray file.
    std::filesystem::remove(tempZip, ec);
    if (ec) {
        mLastError = "warning: failed to remove " + tempZipStr + ": " + ec.message();
        // Carry on — the extraction itself succeeded.
    }

    // 6. Write the installed.json record so subsequent runs know what's here.
    if (!WriteInstalledManifest(pTargetDir, pManifest)) {
        return false; // mLastError already set
    }

    return true;
}

// ---------------------------------------------------------------------------
// WriteInstalledManifest
// ---------------------------------------------------------------------------
bool DataRelease::WriteInstalledManifest(const std::string& pTargetDir, const ReleaseManifest& pManifest) {
    mLastError.clear();

    Json record = {
        {"owner",         pManifest.mOwner},
        {"repo",          pManifest.mRepo},
        {"tagName",       pManifest.mTagName},
        {"dataVersion",   pManifest.mDataVersion},
        {"scriptVersion", pManifest.mScriptVersion},
    };

    std::error_code ec;
    std::filesystem::create_directories(pTargetDir, ec);
    if (ec) {
        mLastError = "create_directories(" + pTargetDir + ") failed: " + ec.message();
        return false;
    }

    const std::filesystem::path path = std::filesystem::path(pTargetDir) / kInstalledManifestFile;
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) {
        mLastError = "open " + path.string() + " for write failed";
        return false;
    }

    try {
        out << record.dump(2);
    } catch (const std::exception& ex) {
        mLastError = std::string("serialise installed.json failed: ") + ex.what();
        return false;
    }

    if (!out) {
        mLastError = "write " + path.string() + " failed";
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// ReadInstalledManifest
// ---------------------------------------------------------------------------
bool DataRelease::ReadInstalledManifest(const std::string& pTargetDir, int& pDataVersion, int& pScriptVersion) {
    mLastError.clear();
    pDataVersion = 0;
    pScriptVersion = 0;

    const std::filesystem::path path = std::filesystem::path(pTargetDir) / kInstalledManifestFile;

    std::error_code ec;
    if (!std::filesystem::exists(path, ec) || ec) {
        mLastError = "no installed.json at " + path.string();
        return false;
    }

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        mLastError = "open " + path.string() + " for read failed";
        return false;
    }

    Json record;
    try {
        in >> record;
    } catch (const std::exception& ex) {
        mLastError = std::string("malformed installed.json: ") + ex.what();
        return false;
    }

    if (!record.is_object()) {
        mLastError = "installed.json is not an object";
        return false;
    }

    pDataVersion   = record.value("dataVersion",   0);
    pScriptVersion = record.value("scriptVersion", 0);
    return true;
}

} // namespace Setup
