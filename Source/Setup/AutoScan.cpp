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

static void TryCandidate(std::vector<sScanCandidate>& pOut,
                         const std::string& pLabel,
                         const std::string& pPath) {
    if (pPath.empty() || !g_ResourceMan)
        return;

    if (!g_ResourceMan->DirExists(pPath))
        return;

    // Quick reject: if there's no Data/ child, ValidateFolder will still try
    // to be helpful (e.g. user picked the release dir directly). Either way,
    // ValidateFolder is cheap — only MD5s files it has reason to hash.
    sValidationResult r = ValidateFolder(pPath);
    if (r.mMatches.empty())
        return;

    // De-dup against earlier candidates — same input folder, same matches.
    for (auto& existing : pOut) {
        if (existing.mPath == r.mResolvedRoot)
            return;
    }

    sScanCandidate c;
    c.mPath = r.mResolvedRoot;
    c.mLabel = pLabel;
    c.mResult = std::move(r);
    pOut.push_back(std::move(c));
}

// Probe a set of subfolders that the user (or a scanner) might have created
// inside a base directory like "Documents".
static void ProbeNamedSubfolders(std::vector<sScanCandidate>& pOut,
                                 const std::string& pLabelPrefix,
                                 const std::string& pBase) {
    if (pBase.empty() || !g_ResourceMan)
        return;
    if (!g_ResourceMan->DirExists(pBase))
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
        TryCandidate(pOut, pLabelPrefix + ": " + n, sub);
    }
}

// Add disk-image files in pBase as candidates. The wizard mounts on click,
// not on scan, so we keep the auto-scan cheap (one DirectoryList call per
// extension) and let the user opt in.
//
// Filename heuristic: include any matching file. The wizard's auto-scan
// shouldn't try to be clever about "is this CF data?" — that's the mount's
// job. False positives in this list cost nothing because the user has to
// click.
static void ProbeImageFiles(std::vector<sScanCandidate>& pOut,
                            const std::string& pLabelPrefix,
                            const std::string& pBase) {
    if (pBase.empty() || !g_ResourceMan)
        return;
    if (!g_ResourceMan->DirExists(pBase))
        return;

    static const char* kExts[] = { ".adf", ".iso", ".ipf", ".img", ".ima",
                                    ".fdi", ".hdf", ".d64", ".d71", ".d81" };

    const std::string baseNorm = Norm(pBase);

    for (auto* ext : kExts) {
        auto files = g_ResourceMan->DirectoryList(pBase, ext);
        for (auto& f : files) {
            // DirectoryList returns names; build full path.
            std::string full = baseNorm + f;

            // Lower-case heuristic to surface CF-named files first via label.
            std::string lower = f;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

            sScanCandidate c;
            c.mPath = full;
            c.mIsImage = true;
            c.mLabel = pLabelPrefix + ": " + f;
            pOut.push_back(std::move(c));
            // Cap per-extension to avoid drowning the wizard list with
            // thousands of unrelated ISOs in a Downloads folder.
            if (pOut.size() >= 64)
                return;
        }
    }
}

std::vector<sScanCandidate> AutoScan() {
    std::vector<sScanCandidate> hits;

    if (!g_ResourceMan)
        return hits;

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
        for (auto& p : g_ResourceMan->getAllPaths()) {
            TryCandidate(hits, "Search path", Norm(p));
            ProbeImageFiles(hits, "Image", Norm(p));
        }
    }

    // -----------------------------------------------------------------------
    // 1) Sibling-of-exe and parent-of-exe — the most common "extracted next to
    //    the engine" layouts. cwd is always in mAllPaths but we still probe
    //    explicitly to label the hit nicely.
    // -----------------------------------------------------------------------
    {
        std::string cwd = Norm(g_ResourceMan->getcwd());
        TryCandidate(hits, "Working directory", cwd);
        ProbeImageFiles(hits, "Image", cwd);
        // ../  — useful when the binary lives in Run/Release/ but data is in Run/.
        if (cwd.size() > 1) {
            std::string parentPath = cwd.substr(0, cwd.size() - 1);
            size_t lastSlash = parentPath.find_last_of('/');
            if (lastSlash != std::string::npos) {
                std::string parent = parentPath.substr(0, lastSlash);
                TryCandidate(hits, "Parent of working dir", Norm(parent));
                ProbeImageFiles(hits, "Image", Norm(parent));
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
            ProbeNamedSubfolders(hits, "Documents", docs);
            // The current installer specifically uses Documents\OpenFodder.
            TryCandidate(hits, "Documents: OpenFodder", Norm(docs) + "OpenFodder");
            // Desktop / Downloads — convenient for newcomers.
            ProbeNamedSubfolders(hits, "Desktop", Norm(user) + "Desktop");
            ProbeNamedSubfolders(hits, "Downloads", Norm(user) + "Downloads");
            // And any disk images sitting in those landing zones.
            ProbeImageFiles(hits, "Desktop", Norm(user) + "Desktop");
            ProbeImageFiles(hits, "Downloads", Norm(user) + "Downloads");
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
            ProbeNamedSubfolders(hits, "GOG", r);
        }

        // GOG Galaxy
        std::string lad = GetEnvPath("LOCALAPPDATA");
        if (!lad.empty()) {
            ProbeNamedSubfolders(hits, "GOG Galaxy", Norm(lad) + "GOG.com/Galaxy/Games");
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
            ProbeNamedSubfolders(hits, "Steam", r);
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
            ProbeNamedSubfolders(hits, "Home", home);
            ProbeNamedSubfolders(hits, "Documents", Norm(home) + "Documents");
            ProbeNamedSubfolders(hits, "Desktop", Norm(home) + "Desktop");
            ProbeNamedSubfolders(hits, "Downloads", Norm(home) + "Downloads");
        }
    }
#endif

    return hits;
}

} // namespace Setup
