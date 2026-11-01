/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include <cstddef>
#include <string>
#include <vector>

namespace Setup {
struct sDiskImageChoice {
    std::string mPath;
    std::string mName;
    std::string mFormat;
    size_t mDisks = 1;
};
// A lightweight directory scan: group numbered disks without decoding them.
// CT RAW capture headers distinguish disk images from loose .RAW artwork.
std::vector<sDiskImageChoice> FindDiskImages(const std::string& folder, std::string* error = nullptr);
}
