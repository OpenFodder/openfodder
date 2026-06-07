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

#include <string>

namespace Setup {

// Where the cold-start prompt and the in-game Update button install the
// downloaded data + scripts archives.
//
// Two modes:
//
//  * Portable (Windows .zip, dev checkouts) — install next to the exe:
//      <cwd>/Data/   <cwd>/Scripts/
//    matches what the engine's read-side scans first via
//    cResourceMan::addDefaultDirs (cwd → mAllPaths[0]).
//
//  * Per-user (Linux distro install, macOS bundle) — install into the
//    user's data directory, NOT next to the exe (which is typically
//    /usr/bin/openfodder, owned by root, write-blocked):
//      $XDG_DATA_HOME/OpenFodder/Data/   ... /Scripts/
//    falling back through:
//      $HOME/.local/share/OpenFodder/Data/  ... /Scripts/
//      $HOME/Library/Application Support/OpenFodder/Data/  ... /Scripts/  (macOS)
//    matches what addDefaultDirs scans on the read side.
//
// Mode selection: if the working directory looks "portable" (writable +
// already contains a Data/ or Scripts/ subdir, or a sibling about.png the
// engine ships in Run/), portable mode wins. Otherwise per-user mode.
//
// All paths are returned forward-slash normalised for parity with the rest
// of the engine (gPathSeperator at Source/stdafx.cpp:32).
struct InstallTargets {
    std::string mDataDir;          // absolute, trailing slash stripped
    std::string mScriptsDir;       // absolute, trailing slash stripped
    bool        mIsPortable = true; // dev checkouts and Windows .zip → true
};

class InstallPaths {
public:
    // Resolve the install targets for THIS launch. Cheap; safe to call
    // multiple times (the result is recomputed each call rather than
    // cached, since cwd can technically change at runtime).
    static InstallTargets Resolve();

    // Just the per-user "OpenFodder/" root, with no Data/Scripts suffix.
    // Used by the wizard's path-add code so [paths] gets the *parent* of
    // the install dir as a search root rather than the leaf.
    static std::string PerUserRoot();

    // True if the engine is running inside a Flatpak sandbox. The
    // cold-start downloader and the in-app Update button are disabled
    // in that case because:
    //
    //   - Updates flow through Flatpak (`flatpak update org.openfodder.OpenFodder`)
    //     and bypassing it would put the data tree out of sync with the
    //     manifest's recorded `data` extension version.
    //
    //   - Network access is permission-gated (--share=network in the
    //     manifest). A naive download attempt would fail confusingly
    //     unless that permission was granted.
    //
    //   - The Flatpak ships its own data; if the user has somehow
    //     deleted it, the right fix is `flatpak repair`, not a side-load
    //     from GitHub.
    //
    // Detection: $FLATPAK_ID is set inside the sandbox (it carries the
    // app-id, e.g. "org.openfodder.OpenFodder"). $container=flatpak is
    // also set but FLATPAK_ID is the canonical signal.
    static bool IsFlatpak();
};

} // namespace Setup
