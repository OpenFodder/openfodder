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

#pragma once

#include "Setup/HttpDownload.hpp"
#include "Setup/ZipExtract.hpp"

#include <string>

namespace Setup {

// ---------------------------------------------------------------------------
// DataRelease — orchestrator for downloading and installing OpenFodder
// data / scripts releases published to GitHub.
//
// Lifecycle:
//   1. QueryLatest(owner, repo, &manifest)  — hits the GitHub releases API,
//      identifies the .zip and required compat.json assets, and populates
//      a ReleaseManifest with tag, notes, archive URL, and version numbers.
//   2. FetchAndInstall(manifest, targetDir, ...) — version-gates the
//      manifest against EngineVersion's [Min,Max] window, downloads the
//      archive into a sibling staging directory, validates the archive,
//      and publishes the replacement tree with installed.json and rollback.
//   3. ReadInstalledManifest(targetDir, &dv, &sv) — reads installed.json
//      back so the setup wizard can show "currently installed: X" and
//      decide whether an upgrade is needed.
//
// Errors never throw — methods return false and stash a human-readable
// message in LastError().
// ---------------------------------------------------------------------------

struct ReleaseManifest {
    std::string mOwner;
    std::string mRepo;
    std::string mTagName;
    std::string mReleaseNotes;
    std::string mArchiveUrl;        // .zip asset URL
    std::string mCompatUrl;         // compat.json asset URL
    int         mDataVersion = 0;
    int         mScriptVersion = 0;
};

class DataRelease {
public:
    DataRelease() = default;

    DataRelease(const DataRelease&) = delete;
    DataRelease& operator=(const DataRelease&) = delete;

    // Hit GitHub's releases API for the given repo and populate pOut. Returns
    // false on network error, non-2xx status, malformed JSON, or a release
    // that has no .zip or compat.json asset. Metadata must contain a positive
    // version for the requested repository; the other subtree may be zero.
    bool QueryLatest(const std::string& pOwner, const std::string& pRepo, ReleaseManifest& pOut);

    // Version-gate, stage and validate the download, then replace the target.
    // Existing local files are preserved. Failed publication restores the old
    // tree; if restoration is blocked, LastError identifies the retained backup.
    bool FetchAndInstall(const ReleaseManifest& pManifest,
                         const std::string& pTargetDir,
                         ProgressCallback pHttpProgress = {},
                         ZipProgressCallback pZipProgress = {});

    // Persist a small JSON record of what was installed into
    // <pTargetDir>/installed.json so the wizard can compare against future
    // QueryLatest results.
    bool WriteInstalledManifest(const std::string& pTargetDir, const ReleaseManifest& pManifest);

    // Read the previously-written installed.json. Returns false if absent or
    // malformed; sets pDataVersion / pScriptVersion to 0 when their fields
    // are missing from an otherwise-valid file.
    bool ReadInstalledManifest(const std::string& pTargetDir, int& pDataVersion, int& pScriptVersion);

    const std::string& LastError() const { return mLastError; }

    // Static accessors so other modules (setup wizard, CLI flags) don't have
    // to hard-code the repo names — there's exactly one source of truth.
    static const char* DataRepoOwner();
    static const char* DataRepoName();
    static const char* ScriptsRepoOwner();
    static const char* ScriptsRepoName();

private:
    HttpClient  mHttp;
    std::string mLastError;
};

} // namespace Setup
