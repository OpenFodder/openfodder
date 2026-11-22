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
#include "FileSystem.hpp"
#include "Setup/AutoScan.hpp"
#include "Setup/Validation.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace Setup {

static std::string GetEnvPath(const char* pName) {
#ifdef _WIN32
    char buf[1024];
    size_t len = 0;
    if (getenv_s(&len, buf, sizeof(buf), pName) != 0 || len == 0)
        return {};
    return std::string(buf, (len ? len - 1 : 0));
#else
    const char* v = std::getenv(pName);
    return v ? std::string(v) : std::string{};
#endif
}

// Ensure forward slashes + a trailing slash, matching the rest of the engine.
static std::string Norm(std::string p) {
    if (p.empty())
        return p;
    std::replace(p.begin(), p.end(), '\\', '/');
    if (p.back() != '/')
        p.push_back('/');
    return p;
}

std::vector<sScanCandidate> FolderCandidates(const sValidationResult& pResult,
                                             const std::string& pLabel) {
    std::vector<sScanCandidate> candidates;
    auto result = pResult;
    RemoveOverlappingMatches(result);
    for (const auto& match : result.mMatches) {
        if (!match.mVersion || match.mResolvedPath.empty())
            continue;
        sScanCandidate candidate;
        candidate.mPath = match.mResolvedPath;
        candidate.mLabel = pLabel;
        candidate.mResult.mInputFolder = match.mResolvedPath;
        candidate.mResult.mResolvedRoot = match.mResolvedPath;
        candidate.mResult.mMatches.push_back(match);
        candidate.mResult.mMatches.back().mIsRawFolder = true;
        candidates.push_back(std::move(candidate));
    }
    return candidates;
}

static bool IsDirectory(const std::string& path) {
    std::error_code ec;
    return std::filesystem::is_directory(FileSystem::Resolve(path), ec);
}

static void TryCandidate(std::vector<sScanCandidate>& pOut,
                         const std::string& pLabel,
                         const std::string& pPath, const std::vector<const sGameVersion*>& available, const CancelCheck& cancelled) {
    if ((cancelled && cancelled()) || pPath.empty() || !IsDirectory(pPath))
        return;

    auto result = ValidateFolder(pPath, cancelled);
    result.mMatches.erase(std::remove_if(result.mMatches.begin(), result.mMatches.end(),
        [](const sVersionMatch& match) { return !match.IsComplete(); }), result.mMatches.end());
    ExcludeAvailable(result, available);
    auto choices = FolderCandidates(result, pLabel);
    if (choices.empty()) return;
    sScanCandidate candidate;
    if (choices.size() == 1) {
        candidate = std::move(choices.front());
    } else {
        candidate.mPath = result.mResolvedRoot;
        candidate.mLabel = pLabel;
        candidate.mResult = result;
        candidate.mResult.mMatches.clear();
        for (const auto& choice : choices)
            candidate.mResult.mMatches.push_back(choice.mResult.mMatches.front());
    }
    const bool duplicate = std::any_of(pOut.begin(), pOut.end(), [&](const sScanCandidate& existing) {
        return FileSystem::Equal(existing.mPath, candidate.mPath);
    });
    if (!duplicate)
        pOut.push_back(std::move(candidate));
}

// Probe a set of subfolders that the user (or a scanner) might have created
// inside a base directory like "Documents".
static void ProbeNamedSubfolders(std::vector<sScanCandidate>& pOut,
                                 const std::string& pLabelPrefix,
                                 const std::string& pBase, const std::vector<const sGameVersion*>& available, const CancelCheck& cancelled) {
    if (pBase.empty() || !IsDirectory(pBase))
        return;

    // Each entry: directory NAME we expect inside pBase.
    static const char* kNames[] = {
        "Cannon Fodder",
        "Cannon Fodder 2",
        "CannonFodder",
        "CannonFodder2",
        "OpenFodder",
        "Open Fodder",
    };

    for (auto* n : kNames) {
        std::string sub = Norm(pBase) + n;
        TryCandidate(pOut, pLabelPrefix + ": " + n, sub, available, cancelled);
    }
}

// Automatic suggestions contain verified game folders only. Disk images
// remain available in the file browser for the folder the user opens.
std::vector<sScanCandidate> AutoScan() {
    return AutoScan(g_ResourceMan ? g_ResourceMan->GetAvailable() : std::vector<const sGameVersion*>{});
}

std::vector<sScanCandidate> AutoScan(const std::vector<const sGameVersion*>& available, const CancelCheck& cancelled) {
    if (!g_ResourceMan) return {};
    return AutoScan(sAutoScanInput{g_ResourceMan->getAllPaths(), g_ResourceMan->getcwd(), available}, cancelled);
}

std::vector<sScanCandidate> AutoScan(const sAutoScanInput& input, const CancelCheck& cancelled) {
    std::vector<sScanCandidate> hits;
    const auto& available = input.mAvailable;

    // -----------------------------------------------------------------------
    // 0) Engine search-path list. cResourceMan::getAllPaths() includes:
    //      - cwd (added by addDefaultDirs)
    //      - $USERPROFILE/Documents/OpenFodder (Windows) / XDG dirs (POSIX)
    //      - every entry from the ini's [paths] section (the wizard's own
    //        previous saves, plus anything the user hand-edited)
    //    Probing this list first means a user who already has data
    //    registered via openfodder.ini sees it surface in the wizard.
    // -----------------------------------------------------------------------
    {
        for (auto& p : input.mPaths) {
            TryCandidate(hits, "Search path", Norm(p), available, cancelled);
        }
    }

    // -----------------------------------------------------------------------
    // 1) Sibling-of-exe and parent-of-exe — the most common "extracted next to
    //    the engine" layouts. cwd is always in mAllPaths but we still probe
    //    explicitly to label the hit nicely.
    // -----------------------------------------------------------------------
    {
        std::string cwd = Norm(input.mWorkingDirectory);
        TryCandidate(hits, "Working directory", cwd, available, cancelled);
        // ../  — useful when the binary lives in Run/Release/ but data is in Run/.
        if (cwd.size() > 1) {
            std::string parentPath = cwd.substr(0, cwd.size() - 1);
            size_t lastSlash = parentPath.find_last_of('/');
            if (lastSlash != std::string::npos) {
                std::string parent = parentPath.substr(0, lastSlash);
                TryCandidate(hits, "Parent of working dir", Norm(parent), available, cancelled);
            }
        }
    }

#ifdef _WIN32
    // -----------------------------------------------------------------------
    // 2) Documents and user profile.
    // -----------------------------------------------------------------------
    {
        std::string user = GetEnvPath("USERPROFILE");
        if (!user.empty()) {
            std::string docs = Norm(user) + "Documents";
            ProbeNamedSubfolders(hits, "Documents", docs, available, cancelled);
            // The current installer specifically uses Documents\OpenFodder.
            TryCandidate(hits, "Documents: OpenFodder", Norm(docs) + "OpenFodder", available, cancelled);
            // Desktop / Downloads — convenient for newcomers.
            ProbeNamedSubfolders(hits, "Desktop", Norm(user) + "Desktop", available, cancelled);
            ProbeNamedSubfolders(hits, "Downloads", Norm(user) + "Downloads", available, cancelled);
        }
    }

    // -----------------------------------------------------------------------
    // 3) GOG default install locations.
    // -----------------------------------------------------------------------
    {
        const char* kGogRoots[] = {
            "C:/GOG Games",
            "D:/GOG Games",
            "E:/GOG Games",
            "C:/Games/GOG Games",
        };
        for (auto* r : kGogRoots) {
            ProbeNamedSubfolders(hits, "GOG", r, available, cancelled);
        }

        // GOG Galaxy
        std::string lad = GetEnvPath("LOCALAPPDATA");
        if (!lad.empty()) {
            ProbeNamedSubfolders(hits, "GOG Galaxy", Norm(lad) + "GOG.com/Galaxy/Games", available, cancelled);
        }
    }

    // -----------------------------------------------------------------------
    // 4) Steam — best-effort. We don't parse libraryfolders.vdf in Phase 2 to
    //    keep dependencies minimal; just probe the default install path.
    // -----------------------------------------------------------------------
    {
        const char* kSteamRoots[] = {
            "C:/Program Files (x86)/Steam/steamapps/common",
            "C:/Program Files/Steam/steamapps/common",
            "D:/Steam/steamapps/common",
            "D:/SteamLibrary/steamapps/common",
        };
        for (auto* r : kSteamRoots) {
            ProbeNamedSubfolders(hits, "Steam", r, available, cancelled);
        }
    }
#else
    // -----------------------------------------------------------------------
    // POSIX: the engine's addDefaultDirs already picks up XDG_DATA_HOME etc.;
    // the auto-scan adds a few extra hand-rolled candidates.
    // -----------------------------------------------------------------------
    {
        std::string home = GetEnvPath("HOME");
        if (!home.empty()) {
            ProbeNamedSubfolders(hits, "Home", home, available, cancelled);
            ProbeNamedSubfolders(hits, "Documents", Norm(home) + "Documents", available, cancelled);
            ProbeNamedSubfolders(hits, "Desktop", Norm(home) + "Desktop", available, cancelled);
            ProbeNamedSubfolders(hits, "Downloads", Norm(home) + "Downloads", available, cancelled);
        }
    }
#endif

    return hits;
}

} // namespace Setup
