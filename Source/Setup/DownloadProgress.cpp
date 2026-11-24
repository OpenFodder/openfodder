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

// Both startup prompts and progress use this asset-free window. Keep it alive
// across the confirmation, download and failure screens.
struct ProgressWindow {
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{nullptr, SDL_DestroyRenderer};
    std::vector<SDL_Event> deferred;
    SDL_Window* parent;
    bool savedMouse = false, wasVisible = false, wasRelative = false;

    ProgressWindow(const std::string& title, SDL_Window* owner, std::string& error) : parent(owner) {
        window.reset(SDL_CreateWindow(title.c_str(), 600, 220, SDL_WINDOW_HIGH_PIXEL_DENSITY));
        if (!window) { error = std::string("Could not open download window: ") + SDL_GetError(); return; }
        renderer.reset(SDL_CreateRenderer(window.get(), nullptr));
        if (!renderer) { error = std::string("Could not draw download window: ") + SDL_GetError(); return; }
        Resize(220);
        SDL_SetWindowPosition(window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        if (parent) {
            SDL_SetWindowParent(window.get(), parent);
            SDL_SetWindowModal(window.get(), true);
        }
        wasVisible = SDL_CursorVisible();
        wasRelative = parent && SDL_GetWindowRelativeMouseMode(parent);
        savedMouse = true;
        if (wasRelative) SDL_SetWindowRelativeMouseMode(parent, false);
        SDL_ShowCursor();
    }

    ~ProgressWindow() {
        renderer.reset();
        window.reset();
        if (savedMouse) {
            if (wasRelative) SDL_SetWindowRelativeMouseMode(parent, true);
            if (!wasVisible) SDL_HideCursor();
        }
        for (auto& event : deferred) SDL_PushEvent(&event);
    }

    void Resize(int height) {
        SDL_SetWindowSize(window.get(), 600, height);
        SDL_SetRenderLogicalPresentation(renderer.get(), 600, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
        SDL_SetRenderScale(renderer.get(), 2, 2);
    }

    bool Prompt(const DownloadPrompt& prompt) {
        Resize(360);
        std::vector<std::string> lines;
        std::istringstream paragraphs(prompt.message);
        std::string paragraph;
        while (std::getline(paragraphs, paragraph)) {
            if (paragraph.empty()) { lines.emplace_back(); continue; }
            while (paragraph.size() > 34) {
                auto split = paragraph.rfind(' ', 34);
                if (split == std::string::npos || split == 0) split = 34;
                lines.push_back(paragraph.substr(0, split));
                paragraph.erase(0, split);
                if (!paragraph.empty() && paragraph.front() == ' ') paragraph.erase(0, 1);
            }
            lines.push_back(paragraph);
        }
        constexpr size_t pageLines = 10;
        const size_t maxTop = lines.size() > pageLines ? lines.size() - pageLines : 0;
        size_t top = 0;
        bool acceptSelected = true;
        const SDL_FRect accept{170, 154, 118, 18}, cancel{12, 154, 110, 18};
        const auto id = SDL_GetWindowID(window.get());
#ifdef OPENFODDER_DOWNLOAD_DIAGNOSTICS
        bool captured = false;
#endif
        for (;;) {
            SDL_SetRenderDrawColor(renderer.get(), 19, 27, 24, 255);
            SDL_RenderClear(renderer.get());
            SDL_SetRenderDrawColor(renderer.get(), 231, 239, 232, 255);
            SDL_RenderDebugText(renderer.get(), 12, 12, "OPENFODDER");
            SDL_RenderDebugText(renderer.get(), 12, 26, prompt.heading.c_str());
            SDL_SetRenderDrawColor(renderer.get(), 188, 204, 192, 255);
            for (size_t i = top; i < std::min(lines.size(), top + pageLines); ++i)
                SDL_RenderDebugText(renderer.get(), 12, 42 + float(i - top) * 10, lines[i].c_str());
            if (maxTop) SDL_RenderDebugText(renderer.get(), 12, 142, "UP/DOWN TO SCROLL");
            const auto button = [&](const SDL_FRect& rect, const std::string& label, bool selected) {
                SDL_SetRenderDrawColor(renderer.get(), selected ? 100 : 76, selected ? 127 : 94, selected ? 87 : 73, 255);
                SDL_RenderFillRect(renderer.get(), &rect);
                SDL_SetRenderDrawColor(renderer.get(), 231, 239, 232, 255);
                SDL_RenderDebugText(renderer.get(), rect.x + (rect.w - float(label.size()) * 8) / 2,
                    rect.y + 5, label.c_str());
            };
            button(accept, prompt.accept, acceptSelected);
            if (!prompt.cancel.empty()) button(cancel, prompt.cancel, !acceptSelected);
#ifdef OPENFODDER_DOWNLOAD_DIAGNOSTICS
            if (!captured) {
                if (const char* path = SDL_getenv("OPENFODDER_DOWNLOAD_PREVIEW")) {
                    if (auto* pixels = SDL_RenderReadPixels(renderer.get(), nullptr)) {
                        SDL_SaveBMP(pixels, path);
                        SDL_DestroySurface(pixels);
                    }
                }
                captured = true;
            }
#endif
            SDL_RenderPresent(renderer.get());
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                    if (event.type == SDL_EVENT_QUIT || event.window.windowID != id) deferred.push_back(event);
                    return false;
                }
                if (event.type == SDL_EVENT_KEY_DOWN && event.key.windowID == id) {
                    if (event.key.key == SDLK_ESCAPE) return false;
                    if (event.key.key == SDLK_RETURN || event.key.key == SDLK_KP_ENTER) return acceptSelected;
                    if ((event.key.key == SDLK_TAB || event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT) &&
                        !prompt.cancel.empty()) acceptSelected = !acceptSelected;
                    if (event.key.key == SDLK_UP && top) --top;
                    if (event.key.key == SDLK_DOWN && top < maxTop) ++top;
                } else if (event.type == SDL_EVENT_MOUSE_WHEEL && event.wheel.windowID == id) {
                    if (event.wheel.y > 0 && top) --top;
                    if (event.wheel.y < 0 && top < maxTop) ++top;
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.windowID == id &&
                           event.button.button == SDL_BUTTON_LEFT) {
                    SDL_ConvertEventToRenderCoordinates(renderer.get(), &event);
                    const SDL_FPoint point{event.button.x, event.button.y};
                    if (SDL_PointInRectFloat(&point, &accept)) return true;
                    if (!prompt.cancel.empty() && SDL_PointInRectFloat(&point, &cancel)) return false;
                }
            }
            SDL_Delay(16);
        }
    }
};

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
    std::string& error, SDL_Window* parent, const std::function<void()>& complete, const DownloadPrompt* prompt) {
    error.clear();
#ifdef EMSCRIPTEN
    error = "Native downloads are unavailable in the browser build";
    return false;
#else
    ProgressWindow view(title, parent, error);
    if (!view.renderer) return false;
    if (prompt && !view.Prompt(*prompt)) return false;
    view.Resize(220);
    auto& window = view.window;
    auto& renderer = view.renderer;
    DownloadProgress progress;
    Draw(renderer.get(), progress.Read()); // Visible before even the release query starts.
    std::atomic<bool> finished{false};
    bool result = false;
    auto& deferred = view.deferred;
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
    if (prompt && !result && !error.empty())
        view.Prompt({"DOWNLOAD FAILED", error, "QUIT", ""});
    return result;
#endif
}

void ShowDownloadNotice(const std::string& title, const std::string& message) {
#ifndef EMSCRIPTEN
    std::string error;
    ProgressWindow view(title, nullptr, error);
    if (view.renderer) view.Prompt({"MISSING GAME FILES", message, "QUIT", ""});
    else SDL_Log("%s: %s", title.c_str(), error.c_str());
#endif
}

} // namespace Setup
