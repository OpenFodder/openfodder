/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace Setup {
// Decode one circular MFM track, checking the SOS6 marker and track checksum.
bool DecodeSensibleTrack(const uint8_t* data, size_t bitCount, std::vector<uint8_t>& decoded);
// Uses SOS_Unpacker's native SDK; no SDK types escape this adapter.
bool LoadSensibleTracks(const std::string& path, std::vector<std::vector<uint8_t>>& tracks,
                        std::string& error);
}
