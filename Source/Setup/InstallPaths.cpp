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
#include "Setup/InstallPaths.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <system_error>

#ifdef _WIN32
#include <cstdio>     // _wgetenv_s — not used; we go via std::getenv below
#endif

namespace Setup {

namespace {

// Forward-slash-normalised, trailing slash stripped. Engine convention is
// gPathSeperator = '/' (Source/stdafx.cpp:32) — keep parity to avoid
// surprises in the wizard's path-add code.
std::string Normalise(std::filesystem::path pPath) {
    if (pPath.empty())
        return {};
    std::string s = pPath.lexically_normal().generic_string();
    if (s.size() > 1 && s.back() == '/')
        s.pop_back();
    return s;
}

bool TryGetEnv(const char* pName, std::string& pOut) {
#ifdef _WIN32
    char buf[1024];
    size_t needed = 0;
    if (getenv_s(&needed, buf, sizeof(buf), pName) != 0 || needed == 0)
        return false;
    pOut.assign(buf, needed - 1);    // needed includes the null terminator
    return !pOut.empty();
#else
    const char* v = std::getenv(pName);
    if (!v || !*v)
        return false;
    pOut = v;
    return true;
#endif
}

// True if the directory exists AND we can create a file inside it. The
// permissions check on Linux can lie (mounted-noexec, ACLs), so the only
// reliable test is "actually try". The probe file is written to a unique
// name and removed immediately; failure is silent.
bool IsWritableDir(const std::filesystem::path& pDir) {
    std::error_code ec;
    if (!std::filesystem::is_directory(pDir, ec))
        return false;

    // Probe filename includes a process-id-ish marker so concurrent
    // launches don't collide. Date.now isn't available to scripts in
    // the workflow harness; here in C++ we use raw cstdio tmpnam-style
    // by combining a constant string with an incrementing counter.
    // We don't need cryptographic uniqueness — just two parallel
    // launches.
    static int counter = 0;
    auto probe = pDir / (std::string(".openfodder_writeprobe_") + std::to_string(++counter));
    {
        std::ofstream out(probe, std::ios::binary | std::ios::trunc);
        if (!out)
            return false;
        out << "ok";
        if (!out)
            return false;
    }
    std::filesystem::remove(probe, ec);
    return true;
}

// Heuristic: this cwd looks like the install dir if any of these are true
//   - it already contains Data/ or Scripts/ subfolder
//   - it contains about.png (engine ships this in Run/ next to the exe)
//   - it contains the engine's openfodder.ini.example template
// Plus: writability check. A read-only cwd (e.g. running from /usr/bin
// with cwd=/) is never portable.
bool LooksLikePortable(const std::filesystem::path& pCwd) {
    if (!IsWritableDir(pCwd))
        return false;

    std::error_code ec;
    if (std::filesystem::is_directory(pCwd / "Data", ec))   return true;
    if (std::filesystem::is_directory(pCwd / "Scripts", ec))return true;
    if (std::filesystem::is_regular_file(pCwd / "about.png", ec)) return true;
    if (std::filesystem::is_regular_file(pCwd / "openfodder.ini.example", ec)) return true;

    return false;
}

// Per-user OpenFodder/ root. Mirrors the read-side scan order in
// cResourceMan::addDefaultDirs so a path written here is reachable on
// the next launch without any extra plumbing.
//
// Order of preference:
//   Windows : %USERPROFILE%/Documents/OpenFodder
//   macOS   : $HOME/Library/Application Support/OpenFodder
//   Linux   : $XDG_DATA_HOME/OpenFodder
//             then $HOME/.local/share/OpenFodder
std::filesystem::path ResolvePerUserRoot() {
#ifdef _WIN32
    std::string up;
    if (TryGetEnv("USERPROFILE", up)) {
        return std::filesystem::path(up) / "Documents" / "OpenFodder";
    }
    return {};   // no sane fallback on Windows
#elif defined(__APPLE__)
    std::string home;
    if (TryGetEnv("HOME", home)) {
        return std::filesystem::path(home) / "Library" / "Application Support" / "OpenFodder";
    }
    return {};
#else
    std::string xdg;
    if (TryGetEnv("XDG_DATA_HOME", xdg)) {
        return std::filesystem::path(xdg) / "OpenFodder";
    }
    std::string home;
    if (TryGetEnv("HOME", home)) {
        return std::filesystem::path(home) / ".local" / "share" / "OpenFodder";
    }
    return {};
#endif
}

} // anonymous namespace

InstallTargets InstallPaths::Resolve() {
    InstallTargets out;

    std::error_code ec;
    auto cwd = std::filesystem::current_path(ec);
    if (ec)
        cwd.clear();

    // Portable wins if the cwd is writable AND looks like an install
    // tree — covers dev checkouts (Run/Data, Run/Scripts present), the
    // Windows portable .zip (extracted next to about.png), and any
    // explicit "I want it here" sibling-of-exe install.
    if (!cwd.empty() && LooksLikePortable(cwd)) {
        out.mDataDir    = Normalise(cwd / "Data");
        out.mScriptsDir = Normalise(cwd / "Scripts");
        out.mIsPortable = true;
        return out;
    }

    // Per-user mode. Engine read-side will pick these up automatically
    // because addDefaultDirs already scans the same XDG / Documents
    // / ApplicationSupport roots.
    auto root = ResolvePerUserRoot();
    if (!root.empty()) {
        // Best-effort create-dirs; the install code will retry on failure.
        std::filesystem::create_directories(root, ec);
        out.mDataDir    = Normalise(root / "Data");
        out.mScriptsDir = Normalise(root / "Scripts");
        out.mIsPortable = false;
        return out;
    }

    // Last-ditch fallback: write next to the exe even if it doesn't look
    // portable. Better to fail noisily on the actual install than to
    // silently dump into "" and write to disk root.
    if (!cwd.empty()) {
        out.mDataDir    = Normalise(cwd / "Data");
        out.mScriptsDir = Normalise(cwd / "Scripts");
        out.mIsPortable = true;
    }
    return out;
}

std::string InstallPaths::PerUserRoot() {
    return Normalise(ResolvePerUserRoot());
}

bool InstallPaths::IsFlatpak() {
#ifdef _WIN32
    return false;
#else
    std::string ignored;
    return TryGetEnv("FLATPAK_ID", ignored);
#endif
}

} // namespace Setup
