/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#include "FileSystem.hpp"
#include "Setup/DiskImageList.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <map>
#include <regex>

namespace Setup {
std::vector<sDiskImageChoice> FindDiskImages(const std::string& folder, std::string* error) {
    namespace fs = std::filesystem;
    const auto lower = [](std::string value) {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return char(std::tolower(c)); });
        return value;
    };
    const std::regex number("(disk|disc)[ _.-]*([0-9]+|[a-z])(?=[^a-z0-9]|$)", std::regex::icase);
    const std::regex ofNumber("([0-9]+)([ _.-]*of[ _.-]*[0-9]+)", std::regex::icase);
    std::error_code ec;
    std::vector<fs::path> files;
    fs::directory_iterator end;
    for (fs::directory_iterator it(FileSystem::Resolve(folder), ec); !ec && it != end; it.increment(ec)) {
        if (it->is_regular_file(ec)) files.push_back(it->path());
    }
    if (ec) {
        if (error) *error = "This folder could not be opened. Check that the drive is connected and choose it again.";
        return {};
    }
    std::sort(files.begin(), files.end());
    std::map<std::string, sDiskImageChoice> grouped;
    for (const auto& path : files) {
        const auto ext = lower(path.extension().string());
        if (ext != ".adf" && ext != ".ipf" && ext != ".raw" && ext != ".iso" &&
            ext != ".img" && ext != ".ima" && ext != ".fdi" && ext != ".hdf" && ext != ".cue") continue;
        const auto stem = lower(path.stem().string());
        if (stem == "save" || stem == "save disk" || stem == "savedisk") continue;
        if (ext == ".raw") {
            std::ifstream input(path, std::ios::binary);
            char signature[4]{};
            if (!input.read(signature, 4) || std::string(signature, 4) != "CAPS") continue;
        }
        const std::string key = ext + std::regex_replace(std::regex_replace(stem, number, "$1#"), ofNumber, "#$2");
        auto found = grouped.find(key);
        if (found != grouped.end()) { ++found->second.mDisks; continue; }
        sDiskImageChoice choice;
        choice.mPath = path.u8string();
        choice.mName = path.stem().u8string();
        const auto tag = choice.mName.find_first_of("([");
        if (tag != std::string::npos) choice.mName.erase(tag);
        choice.mName = std::regex_replace(choice.mName, ofNumber, "");
        choice.mName = std::regex_replace(choice.mName, std::regex("([a-z])([A-Z])"), "$1 $2");
        std::replace(choice.mName.begin(), choice.mName.end(), '.', ' ');
        std::replace(choice.mName.begin(), choice.mName.end(), '_', ' ');
        while (!choice.mName.empty() && choice.mName.back() == ' ') choice.mName.pop_back();
        choice.mFormat = ext == ".raw" ? "CT RAW" : ext.substr(1);
        grouped.emplace(key, std::move(choice));
    }
    std::vector<sDiskImageChoice> choices;
    for (auto& entry : grouped) choices.push_back(std::move(entry.second));
    return choices;
}
}
