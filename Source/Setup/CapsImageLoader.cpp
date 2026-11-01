/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#include "Setup/CapsImageLoader.hpp"
#include <mutex>
#include <filesystem>
#include <fstream>
#include <limits>
#ifdef OPENFODDER_HAS_CAPSIMAGE
#ifndef NOMINMAX
#define NOMINMAX
#endif
// Native SDK headers supplied by SOS_Unpacker, isolated to this translation unit.
#include "stdafx.h"
#endif

namespace Setup {

bool DecodeSensibleTrack(const uint8_t* data, size_t bits, std::vector<uint8_t>& decoded) {
    decoded.clear();
    if (!data || bits < (0x180c * 2) * 8 || bits > 1024 * 1024) return false;
    const auto read = [&](size_t bit, unsigned count) {
        uint32_t value = 0;
        for (unsigned i = 0; i < count; ++i) {
            const size_t at = (bit + i) % bits;
            value = (value << 1) | ((data[at / 8] >> (7 - at % 8)) & 1);
        }
        return value;
    };
    uint16_t sync = uint16_t(read(0, 16));
    for (size_t at = 0; at < bits; ++at) {
        if (sync == 0x4489) {
            size_t start = at + 16;
            unsigned repeats = 0;
            while (repeats < 64 && read(start, 16) == 0x4489) { start += 16; ++repeats; }
            const auto word = [&](size_t index) {
                const uint32_t even = read(start + index * 32, 32) & 0x55555555;
                const uint32_t odd = read(start + (0x180c + index * 4) * 8, 32) & 0x55555555;
                return even | (odd << 1);
            };
            if (word(0) == 0x534f5336) { // "SOS6"
                uint32_t checksum = -word(0) + word(1) - word(2);
                std::vector<uint8_t> track;
                track.reserve(0x1800);
                for (size_t i = 3; i < 3 + 0x1800 / 4; ++i) {
                    const uint32_t value = word(i);
                    checksum -= value;
                    for (int shift = 24; shift >= 0; shift -= 8) track.push_back(uint8_t(value >> shift));
                }
                if (checksum == 0) { decoded = std::move(track); return true; }
            }
        }
        sync = uint16_t((sync << 1) | read(at + 16, 1));
    }
    return false;
}

bool LoadSensibleTracks(const std::string& path, std::vector<std::vector<uint8_t>>& tracks,
                        std::string& error) {
    tracks.clear();
#ifndef OPENFODDER_HAS_CAPSIMAGE
    error = "This build has no IPF/CT RAW decoder. Rebuild with OPENFODDER_ENABLE_CAPSIMAGE enabled.";
    return false;
#else
    // The native SDK keeps global image/codec state. Serialize complete image
    // reads, and release every SDK image before returning its decoded bytes.
    static std::mutex mutex;
    std::lock_guard<std::mutex> guard(mutex);
    if (CAPSInit() != 0) { error = "The native disk decoder could not initialize."; return false; }
    struct SdkGuard { ~SdkGuard() { CAPSExit(); } } sdk;
    const int32_t id = CAPSAddImage();
    if (id < 0) { error = "The disk decoder could not allocate an image."; return false; }
    struct ImageGuard {
        int32_t id;
        ~ImageGuard() { CAPSUnlockAllTracks(id); CAPSUnlockImage(id); CAPSRemImage(id); }
    } cleanup{id};
    // Read through the native filesystem API so UTF-8 host paths work on
    // Windows too; the SDK's narrow fopen cannot represent all filenames.
    std::ifstream input(std::filesystem::u8path(path), std::ios::binary | std::ios::ate);
    const auto size = input ? input.tellg() : std::streampos(-1);
    if (size <= 0 || uint64_t(size) > std::numeric_limits<uint32_t>::max()) {
        error = "The disk image could not be read.";
        return false;
    }
    std::vector<uint8_t> bytes(static_cast<size_t>(size));
    input.seekg(0);
    if (!input.read(reinterpret_cast<char*>(bytes.data()), bytes.size())) {
        error = "The disk image could not be read completely.";
        return false;
    }
    CapsImageInfo info{};
    if (CAPSLockImageMemory(id, bytes.data(), static_cast<uint32_t>(bytes.size()), 0) != 0 || CAPSGetImageInfo(&info, id) != 0 ||
        info.maxcylinder > 83 || info.maxhead > 1) {
        error = "The disk decoder could not open this IPF/CT RAW image.";
        return false;
    }
    tracks.resize((info.maxcylinder + 1) * 2);
    constexpr uint32_t flags = (1 << 2) | (1 << 4) | (1 << 5) | (1 << 8) |
                               (1 << 9) | (1 << 11) | (1 << 12);
    for (size_t number = 2; number < tracks.size(); ++number) {
        CapsTrackInfoT1 track{};
        track.type = 1;
        // SOS logical sides are inverted on original preservation images.
        if (CAPSLockTrack(&track, id, uint32_t(number / 2), (number & 1) ? 0 : 1, flags) == 0)
            DecodeSensibleTrack(track.trackbuf, track.tracklen, tracks[number]);
    }
    if (tracks.size() <= 2 || tracks[2].empty()) {
        tracks.clear();
        error = "No valid Sensible disk directory was decoded. The image may be damaged or use another disk format.";
        return false;
    }
    return true;
#endif
}
}
