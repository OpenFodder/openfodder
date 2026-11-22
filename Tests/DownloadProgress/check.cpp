/* Open Fodder - SPDX-License-Identifier: GPL-3.0-or-later */
#include "Setup/DownloadProgress.hpp"
#include <SDL3/SDL.h>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <thread>

using namespace Setup;
static void require(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

static void modelChecks() {
    DownloadProgress progress;
    require(progress.Read().Percent() == -1, "Query must show indeterminate progress");
    require(progress.Download(1024 * 1024, 0), "Unknown-length download failed");
    require(progress.Read().Percent() == -1 && progress.Read().Detail().find("1.0 MB") != std::string::npos,
        "Unknown-length download must report bytes without a false percentage");
    progress.Download(5, 10);
    require(progress.Read().Percent() == 50, "Download percentage is incorrect");
    progress.Download(std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max());
    require(progress.Read().Percent() == 100, "Byte counters overflowed");
    progress.SetStage(InstallStage::Extracting);
    require(progress.Read().Percent() == -1, "New stage retained the download percentage");
    progress.Extract(2, 8);
    require(progress.Read().Percent() == 25 && progress.Read().Detail() == "File 3 of 8",
        "Extraction must describe the entry about to be installed");
    progress.Cancel();
    require(!progress.Download(3, 8) && !progress.Extract(3, 8) &&
        !progress.SetStage(InstallStage::Publishing), "Canceled work continued to publication");
    DownloadProgress publishing;
    publishing.SetStage(InstallStage::Publishing);
    publishing.Cancel();
    require(!publishing.Read().canceled && !publishing.Read().CanCancel(),
        "Publication cannot be canceled halfway through");
}

static void closeWindow() {
    SDL_Event event{};
    event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&event);
}

static void windowChecks() {
    require(SDL_Init(SDL_INIT_VIDEO), SDL_GetError());
    std::string error;
    const auto mainThread = std::this_thread::get_id();
    bool activated = false;
    require(RunDownloadWithProgress("Progress test", [](DownloadProgress& progress, std::string&) {
        SDL_Delay(40);
        progress.Download(4, 10);
        SDL_Delay(40);
        progress.Extract(0, 4);
        SDL_Delay(40);
        progress.SetStage(InstallStage::Publishing);
        SDL_Delay(40);
        return true;
    }, error, nullptr, [&] {
        require(std::this_thread::get_id() == mainThread, "Activation ran on a worker");
        activated = true;
    }), "Successful operation failed");
    require(activated, "Activation was skipped");
    require(!RunDownloadWithProgress("Activation failure", [](DownloadProgress&, std::string&) {
        return true;
    }, error, nullptr, [] { throw std::runtime_error("Fixture activation failed"); }) &&
        error == "Fixture activation failed", "Activation error was lost");
    require(!RunDownloadWithProgress("Failure test", [](DownloadProgress&, std::string& error) {
        error = "Fixture download failed";
        return false;
    }, error) && error == "Fixture download failed", "Failure detail was lost");
    require(!RunDownloadWithProgress("Exception test", [](DownloadProgress&, std::string&) -> bool {
        throw std::runtime_error("Fixture exception");
    }, error) && error == "Fixture exception", "Worker exception escaped");
    require(!RunDownloadWithProgress("Cancellation test", [](DownloadProgress& progress, std::string&) {
        closeWindow();
        for (int i = 0; i < 100; ++i) {
            if (!progress.Download(i, 100)) return false;
            SDL_Delay(5);
        }
        return true;
    }, error) && error.empty(), "Window close did not cancel the download");
    SDL_Event event;
    bool quitPreserved = false;
    while (SDL_PollEvent(&event)) quitPreserved |= event.type == SDL_EVENT_QUIT;
    require(quitPreserved, "Parent quit event was swallowed");
    require(RunDownloadWithProgress("Publication test", [](DownloadProgress& progress, std::string&) {
        progress.SetStage(InstallStage::Publishing);
        closeWindow();
        SDL_Delay(80);
        return !progress.Read().canceled;
    }, error), "Close interrupted publication");
    SDL_Quit();
}

int main(int argc, char** argv) try {
    const std::string mode = argc > 1 ? argv[1] : "--model";
    if (mode == "--model") modelChecks();
    else if (mode == "--window") windowChecks();
    else if (mode == "--preview") {
        require(SDL_Init(SDL_INIT_VIDEO), SDL_GetError());
        const std::string stage = argc > 2 ? argv[2] : "download";
        const int seconds = argc > 3 ? std::stoi(argv[3]) : 120;
        std::string error;
        RunDownloadWithProgress("OpenFodder - Download progress", [&](DownloadProgress& progress, std::string&) {
            if (stage == "install") progress.Extract(42, 120);
            else if (stage == "unknown") progress.Download(4 * 1024 * 1024, 0);
            else progress.Download(21 * 1024 * 1024, 50 * 1024 * 1024);
            for (int i = 0; i < seconds * 20 && !progress.Read().canceled; ++i) SDL_Delay(50);
            return !progress.Read().canceled;
        }, error);
        SDL_Quit();
        require(error.empty(), error.c_str());
    } else throw std::runtime_error("Unknown check mode");
    std::cout << "PASS " << mode << '\n';
    return 0;
} catch (const std::exception& error) {
    std::cerr << "FAIL " << error.what() << '\n';
    return 1;
}
