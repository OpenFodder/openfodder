/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#include <SDL3/SDL_stdinc.h>
#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

// Game paths use UTF-8 and case-insensitive names on every host. Keep the
// spelling on disk: renaming files or lowercasing a whole path breaks Linux.
namespace FileSystem {
inline bool Equal(const std::string& a, const std::string& b) {
    return SDL_strcasecmp(a.c_str(), b.c_str()) == 0;
}

// Existing components are resolved individually. Prefer an exact spelling;
// ambiguous folded names fail rather than choosing a different file at random.
// No cache: newly installed files must be visible immediately.
inline std::filesystem::path Resolve(const std::string& name, bool allowMissing = false) {
    namespace fs = std::filesystem;
    if (name.empty()) return {};
    const auto input = fs::u8path(name);
    auto result = input.root_path();
    for (const auto& component : input.relative_path()) {
        if (component.empty() || component == ".") continue;
        if (component == "..") { result /= component; continue; }
        std::error_code ec;
        const auto direct = result / component;
        if (fs::exists(direct, ec)) { result = direct; continue; }
        fs::path match;
        const auto parent = result.empty() ? fs::path(".") : result;
        fs::directory_iterator end;
        for (fs::directory_iterator it(parent, ec); !ec && it != end; it.increment(ec)) {
            if (Equal(it->path().filename().u8string(), component.u8string())) {
                if (!match.empty()) return {};
                match = it->path().filename();
            }
        }
        if (match.empty() && allowMissing && (!ec || ec == std::errc::no_such_file_or_directory)) {
            result /= component;
            continue;
        }
        if (ec || match.empty()) return {};
        result /= match;
    }
    if (result.empty()) result = ".";
    std::error_code ec;
    if (!allowMissing && !fs::exists(result, ec)) return {};
    return result;
}

inline std::vector<std::string> List(const std::string& folder,
                                     const std::string& suffix, bool directories) {
    namespace fs = std::filesystem;
    std::vector<std::string> names;
    const auto parent = Resolve(folder.empty() ? "." : folder);
    if (parent.empty()) return names;
    std::error_code ec;
    fs::directory_iterator end;
    for (fs::directory_iterator it(parent, ec); !ec && it != end; it.increment(ec)) {
        std::error_code typeError;
        if (directories ? !it->is_directory(typeError) : !it->is_regular_file(typeError)) continue;
        const auto name = it->path().filename().u8string();
        if (directories || suffix.empty() || (name.size() >= suffix.size() &&
            Equal(name.substr(name.size() - suffix.size()), suffix))) names.push_back(name);
    }
    std::sort(names.begin(), names.end(), [](const std::string& a, const std::string& b) {
        const int order = SDL_strcasecmp(a.c_str(), b.c_str());
        return order ? order < 0 : a < b;
    });
    return names;
}
}
