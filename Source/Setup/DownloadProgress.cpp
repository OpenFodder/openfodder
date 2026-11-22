/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#include "Setup/DownloadProgress.hpp"

#include <algorithm>
#include <atomic>
#include <iomanip>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>
#include <SDL3/SDL.h>

namespace Setup {

int DownloadProgress::Snapshot::Percent() const {
    if (!total) return -1;
    return static_cast<int>(std::min<long double>(100, (long double)done * 100 / total));
}

std::string DownloadProgress::Snapshot::Status() const {
    if (canceled) return "Canceling...";
    switch (stage) {
    case InstallStage::Checking: return "Checking for the latest data...";
    case InstallStage::Preparing: return "Preparing installation...";
    case InstallStage::Downloading: return "Downloading data and scripts...";
    case InstallStage::Extracting: return "Installing game files...";
    case InstallStage::Publishing: return "Finishing installation...";
    }
    return {};
}

std::string DownloadProgress::Snapshot::Detail() const {
    if (canceled) return "Waiting for the current step";
    if (stage == InstallStage::Downloading) {
        std::ostringstream text;
        text << std::fixed << std::setprecision(1) << (double)done / (1024 * 1024) << " MB";
        if (total)
            text << " / " << (double)total / (1024 * 1024) << " MB  (" << Percent() << "%)";
        else
            text << " downloaded";
        return text.str();
    }
    if (stage == InstallStage::Extracting && total)
        return "File " + std::to_string(std::min(done + 1, total)) + " of " + std::to_string(total);
    return "Please keep this window open";
}

bool DownloadProgress::SetStage(InstallStage stage) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mSnapshot.canceled) return false;
    if (mSnapshot.stage != stage) {
        mSnapshot.stage = stage;
        mSnapshot.done = mSnapshot.total = 0;
    }
    return true;
}

bool DownloadProgress::Download(uint64_t done, uint64_t total) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mSnapshot.canceled) return false;
    mSnapshot.stage = InstallStage::Downloading;
    mSnapshot.done = done;
    mSnapshot.total = total;
    return true;
}

bool DownloadProgress::Extract(size_t entry, size_t count) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mSnapshot.canceled) return false;
    mSnapshot.stage = InstallStage::Extracting;
    mSnapshot.done = entry;
    mSnapshot.total = count;
    return true;
}

void DownloadProgress::Cancel() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mSnapshot.CanCancel()) mSnapshot.canceled = true;
}

DownloadProgress::Snapshot DownloadProgress::Read() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mSnapshot;
}

#ifndef EMSCRIPTEN
namespace {
const SDL_FRect cancelButton{220, 84, 68, 18};

void Draw(SDL_Renderer* renderer, const DownloadProgress::Snapshot& progress) {
    SDL_SetRenderDrawColor(renderer, 19, 27, 24, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 231, 239, 232, 255);
    SDL_RenderDebugText(renderer, 12, 12, "OPENFODDER");
    SDL_RenderDebugText(renderer, 12, 30, progress.Status().c_str());

    SDL_FRect track{12, 50, 276, 10};
    SDL_SetRenderDrawColor(renderer, 53, 67, 59, 255);
    SDL_RenderFillRect(renderer, &track);
    SDL_FRect fill = track;
    const int percent = progress.Percent();
    if (percent >= 0) {
        fill.w *= percent / 100.0f;
    } else {
        const int travel = (int)track.w - 64;
        const int step = (int)((SDL_GetTicks() / 12) % (travel * 2));
        fill.x += (float)(step <= travel ? step : travel * 2 - step);
        fill.w = 64;
    }
    SDL_SetRenderDrawColor(renderer, 137, 193, 96, 255);
    SDL_RenderFillRect(renderer, &fill);
    SDL_SetRenderDrawColor(renderer, 188, 204, 192, 255);
    SDL_RenderDebugText(renderer, 12, 68, progress.Detail().c_str());
    const bool enabled = progress.CanCancel();
    SDL_SetRenderDrawColor(renderer, enabled ? 76 : 39, enabled ? 94 : 49, enabled ? 73 : 42, 255);
    SDL_RenderFillRect(renderer, &cancelButton);
    SDL_SetRenderDrawColor(renderer, 231, 239, 232, 255);
    SDL_RenderDebugText(renderer, 230, 89, enabled ? "CANCEL" : " WAIT ");
    SDL_RenderPresent(renderer);
}
}
#endif

bool RunDownloadWithProgress(const std::string& title,
    const std::function<bool(DownloadProgress&, std::string&)>& task,
    std::string& error, SDL_Window* parent, const std::function<void()>& complete) {
    error.clear();
#ifdef EMSCRIPTEN
    error = "Native downloads are unavailable in the browser build";
    return false;
#else
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window(
        SDL_CreateWindow(title.c_str(), 600, 220, SDL_WINDOW_HIGH_PIXEL_DENSITY), SDL_DestroyWindow);
    if (!window) {
        error = std::string("Could not open download progress: ") + SDL_GetError();
        return false;
    }
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer(
        SDL_CreateRenderer(window.get(), nullptr), SDL_DestroyRenderer);
    if (!renderer) {
        error = std::string("Could not draw download progress: ") + SDL_GetError();
        return false;
    }
    SDL_SetRenderLogicalPresentation(renderer.get(), 600, 220, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderScale(renderer.get(), 2, 2);
    SDL_SetWindowPosition(window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    if (parent) {
        SDL_SetWindowParent(window.get(), parent);
        SDL_SetWindowModal(window.get(), true);
    }
    const bool wasVisible = SDL_CursorVisible();
    const bool wasRelative = parent && SDL_GetWindowRelativeMouseMode(parent);
    if (wasRelative) SDL_SetWindowRelativeMouseMode(parent, false);
    SDL_ShowCursor();
    DownloadProgress progress;
    Draw(renderer.get(), progress.Read()); // Visible before even the release query starts.
    std::atomic<bool> finished{false};
    bool result = false;
    std::vector<SDL_Event> deferred;
    std::thread worker;
    try {
        worker = std::thread([&] {
            try {
                result = task(progress, error);
            } catch (const std::exception& ex) {
                error = ex.what();
            } catch (...) {
                error = "The download failed unexpectedly.";
            }
            finished.store(true, std::memory_order_release);
        });
        const auto id = SDL_GetWindowID(window.get());
        while (!finished.load(std::memory_order_acquire)) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    deferred.push_back(event);
                    progress.Cancel();
                } else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                    progress.Cancel();
                    if (event.window.windowID != id) deferred.push_back(event);
                } else if (event.type == SDL_EVENT_KEY_DOWN &&
                           event.key.windowID == id && event.key.key == SDLK_ESCAPE) {
                    progress.Cancel();
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
                           event.button.windowID == id && event.button.button == SDL_BUTTON_LEFT) {
                    SDL_ConvertEventToRenderCoordinates(renderer.get(), &event);
                    const SDL_FPoint point{event.button.x, event.button.y};
                    if (SDL_PointInRectFloat(&point, &cancelButton)) progress.Cancel();
                }
            }
            Draw(renderer.get(), progress.Read());
            SDL_Delay(16);
        }
    } catch (const std::exception& ex) {
        progress.Cancel();
        if (worker.joinable()) worker.join();
        error = ex.what();
        result = false;
    }
    if (worker.joinable()) worker.join();
    if (result && complete) {
        progress.SetStage(InstallStage::Publishing);
        Draw(renderer.get(), progress.Read());
        try {
            complete();
        } catch (const std::exception& ex) {
            error = ex.what();
            result = false;
        } catch (...) {
            error = "Could not activate the installed data.";
            result = false;
        }
    }
    if (!result && progress.Read().canceled) error.clear();
    if (!result && !progress.Read().canceled && error.empty()) error = "The download failed.";
    renderer.reset();
    window.reset();
    if (wasRelative) SDL_SetWindowRelativeMouseMode(parent, true);
    if (!wasVisible) SDL_HideCursor();
    for (auto& event : deferred) SDL_PushEvent(&event);
    return result;
#endif
}

} // namespace Setup
