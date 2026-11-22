/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#pragma once

#include "Setup/DataRelease.hpp"
#include <functional>
#include <mutex>
#include <string>

struct SDL_Window;

namespace Setup {

// Worker updates only this model; the main thread owns every SDL operation.
class DownloadProgress {
public:
    struct Snapshot {
        InstallStage stage = InstallStage::Checking;
        uint64_t done = 0, total = 0;
        bool canceled = false;
        bool CanCancel() const { return !canceled && stage != InstallStage::Publishing; }
        int Percent() const;
        std::string Status() const;
        std::string Detail() const;
    };

    bool SetStage(InstallStage stage);
    bool Download(uint64_t done, uint64_t total);
    bool Extract(size_t entry, size_t count);
    void Cancel();
    Snapshot Read() const;

private:
    mutable std::mutex mMutex;
    Snapshot mSnapshot;
};

// Main-thread modal progress window. The task runs on a worker and receives
// cancellation through its callbacks. Completion runs on the main thread with
// the window still visible. An empty error on false means canceled.
bool RunDownloadWithProgress(const std::string& title,
    const std::function<bool(DownloadProgress&, std::string&)>& task,
    std::string& error, SDL_Window* parent = nullptr,
    const std::function<void()>& complete = {});

} // namespace Setup
