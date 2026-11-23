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
#include "FontData_Special.hpp"
#include "FileSystem.hpp"
#include "Setup/SetupWizard.hpp"
#include "Setup/MountedImage.hpp"
#include "Setup/CueSheet.hpp"
#include "Setup/DiskImageList.hpp"
#include <filesystem>
#include "Network/NetworkMenuText.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#endif

namespace Setup {

enum eGuiAction : int16 {
    GUIACT_SW_NONE = 0,

    // Welcome screen
    GUIACT_SW_LOCATE,
    GUIACT_SW_CONTINUE_DEMO,
    GUIACT_SW_QUIT,

    // Locate screen
    GUIACT_SW_BROWSE,
    GUIACT_SW_BROWSE_NATIVE,
    GUIACT_SW_BACK_TO_WELCOME,
    GUIACT_SW_ADD_ALL,
    GUIACT_SW_USE_CANDIDATE,    // arg = index into mCandidates

    // Result screen
    GUIACT_SW_SAVE_AND_CONTINUE,    // mounted image: persist imageN= and re-mount each launch
    GUIACT_SW_COPY_TO_DATA,         // mounted image: copy files into <cwd>/Data/<release>, unmount
    GUIACT_SW_TRY_AGAIN,
    GUIACT_SW_PREVIOUS, GUIACT_SW_NEXT,
    // Public-matchmaking pairing has moved out of the wizard — see
    // MultiplayerMenu's RequireHubToken (it prompts on demand the first
    // time the user tries to host/find an internet game).
};

static uint32_t now_ms_u32() {
    using clock = std::chrono::steady_clock;
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        clock::now().time_since_epoch()
    ).count();
    return (uint32_t)ms;
}

static void Gui_SetupWizard_Click(void* ctx, int16 action, int16 arg) {
    cSetupWizard* w = static_cast<cSetupWizard*>(ctx);
    if (!w) return;
    w->OnRowClick(action, arg);
}

static std::string FitSetupText(std::string text, int width, bool tail = false) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return char(std::toupper(c)); });
    if (g_Fodder->String_MeasureWidth(mFont_Briefing_Width, text) <= width) return text;
    while (!text.empty() && g_Fodder->String_MeasureWidth(mFont_Briefing_Width, ".." + text) > width) {
        if (tail) text.erase(text.begin()); else text.pop_back();
    }
    return tail ? ".." + text : text + "..";
}

static std::string GameName(const sGameVersion* version) {
    if (!version) return "GAME FILES";
    const char* platform = version->mDataPath == "Amiga_CD" ? " - AMIGA CD32" :
                           version->mDataPath == "Amiga" || version->mDataPath == "Amiga2" ? " - AMIGA DISKS" :
                           version->mPlatform == ePlatform::Amiga ? " - AMIGA" :
                           version->mPlatform == ePlatform::PC ? " - PC" : "";
    return version->mName + platform;
}

cSetupWizard::cSetupWizard() = default;
cSetupWizard::~cSetupWizard() = default;

void cSetupWizard::Open(bool pHaveAnyVersion) {
    mDone = false;
    mResourcesChanged = false;
    mHaveAnyVersion = pHaveAnyVersion;
    mScreen = eScreen::Welcome;
    mLastInputMs = 0;
    mLastInputAction = 0;
    mLastInputArg = 0;
    mLastMountedId = 0;
    mCandidates.clear();
    mAvailableAtOpen = g_ResourceMan ? g_ResourceMan->GetAvailable() : std::vector<const sGameVersion*>{};
    RunAutoScan();
}

void cSetupWizard::Close() {
    if (mReadCancelled) mReadCancelled->store(true);
    mPendingPick.clear();
    if (mAutoScanCancelled) mAutoScanCancelled->store(true);
#ifndef EMSCRIPTEN
    if (mReadTask.valid()) {
        try { mReadTask.get(); } catch (...) {}
    }
    if (mAutoScanTask.valid()) {
        try { mAutoScanTask.get(); } catch (...) {}
    }
#endif
    // If the user backed out without confirming, drop any pending mount
    // so we don't leak it into the running engine state.
    if (mLastMountedId > 0 && !mResourcesChanged) {
        g_ResourceMan->unmountImage(mLastMountedId);
        mLastMountedId = 0;
    }
    DiscardPendingCueTemp();
    mBrowser.reset();
    mDone = true;
}

void cSetupWizard::RunAutoScan() {
    mCandidates.clear();
    mCandidateStart = 0;
    mCandidateFolder.clear();
#ifdef EMSCRIPTEN
    // The web build has no native worker threads.
    mCandidates = AutoScan(mAvailableAtOpen);
#else
    if (!mAutoScanTask.valid()) {
        mAutoScanCancelled = std::make_shared<std::atomic<bool>>(false);
        auto cancelled = mAutoScanCancelled;
        sAutoScanInput input;
        input.mAvailable = mAvailableAtOpen;
        if (g_ResourceMan) {
            input.mPaths = g_ResourceMan->getAllPaths();
            input.mWorkingDirectory = g_ResourceMan->getcwd();
        }
        mAutoScanTask = std::async(std::launch::async, [input, cancelled] {
            return AutoScan(input, [cancelled] { return cancelled->load(); });
        });
    }
#endif
}

void cSetupWizard::OnBack() {
    switch (mScreen) {
    case eScreen::Welcome:
        // From the welcome screen, Back == "continue with demo only".
        mDone = true;
        break;
    case eScreen::Locate:
        if (!mCandidateFolder.empty()) RunAutoScan();
        else mScreen = eScreen::Welcome;
        break;
    case eScreen::Browse:
        if (mBrowser) {
            mBrowser->OnBack();
            if (mBrowser->IsDone()) {
                mBrowser.reset();
                mScreen = eScreen::Locate;
            }
        }
        break;
    case eScreen::Checking:
        if (mReadCancelled) mReadCancelled->store(true);
        mPendingPick.clear();
        mCheckingDrawn = false;
        mScreen = eScreen::Locate;
        break;
    case eScreen::Result:
        if (mLastMountedId > 0) {
            g_ResourceMan->unmountImage(mLastMountedId);
            mLastMountedId = 0;
            DiscardPendingCueTemp();
        }
        mScreen = eScreen::Locate;
        break;
    }
}

int cSetupWizard::CandidatePageSize() const {
    const bool namesOnly = std::all_of(mCandidates.begin(), mCandidates.end(), [](const sScanCandidate& candidate) {
        return !candidate.mIsImage && candidate.mResult.mMatches.size() == 1 &&
            candidate.mResult.mMatches.front().IsComplete();
    });
    return namesOnly ? 7 : 3;
}

void cSetupWizard::OnScrollUp() {
    if (mScreen == eScreen::Locate) mCandidateStart = std::max(0, mCandidateStart - CandidatePageSize());
    if (mScreen == eScreen::Browse && mBrowser) {
        mBrowser->OnScrollUp();
    }
}

void cSetupWizard::OnScrollDown() {
    if (mScreen == eScreen::Locate && mCandidateStart + CandidatePageSize() < int(mCandidates.size())) mCandidateStart += CandidatePageSize();
    if (mScreen == eScreen::Browse && mBrowser) {
        mBrowser->OnScrollDown();
    }
}

void cSetupWizard::EnterBrowse() {
    mBrowser = std::make_unique<cFileBrowser>();
    // Pick the most useful starting point: an existing candidate's parent if
    // we have one, else the user profile, else the cwd.
    std::string start = mBrowsePath;
    if (!start.empty()) {
        // Continue where the user was browsing.
    } else if (!mCandidates.empty()) {
        start = mCandidates.front().mPath;
    } else {
#ifdef _WIN32
        char buf[1024]; size_t len = 0;
        if (getenv_s(&len, buf, sizeof(buf), "USERPROFILE") == 0 && len)
            start = std::string(buf, len - 1);
#else
        const char* h = std::getenv("HOME");
        if (h && *h)
            start = h;
#endif
    }
    if (start.empty())
        start = g_ResourceMan->getcwd();

    mBrowser->Open(start, cFileBrowser::eFilter::DirsAndImages,
        [this](const std::string& pPicked) {
            QueuePick(pPicked);
        },
        [this]() {
            mScreen = eScreen::Locate;
        });
    mScreen = eScreen::Browse;
}

bool cSetupWizard::LooksLikeImagePath(const std::string& pPath) {
    // Sniff by file extension. We deliberately accept a couple more types
    // than firy can read today (.ipf / .raw — Phase 4 territory) so the
    // wizard's "image not recognised" feedback is meaningful instead of
    // silently treating an .ipf as a directory and erroring on traversal.
    static const char* kExt[] = { ".adf", ".iso", ".ipf", ".raw", ".img", ".ima",
                                  ".fdi", ".hdf" };
    for (auto* e : kExt) {
        const size_t L = std::strlen(e);
        if (pPath.size() < L) continue;
        bool match = true;
        for (size_t i = 0; i < L; ++i) {
            char a = (char)std::tolower((unsigned char)pPath[pPath.size() - L + i]);
            char b = (char)std::tolower((unsigned char)e[i]);
            if (a != b) { match = false; break; }
        }
        if (match) return true;
    }
    return false;
}

sValidationResult cSetupWizard::ResultFromMountedImage(int pId,
                                                         const std::string& pImagePath) const {
    sValidationResult r;
    r.mInputFolder = pImagePath;
    r.mResolvedRoot = pImagePath;
    r.mLooksLikeRoot = false;     // image path isn't a "root" in the folder sense

    if (pId <= 0)
        return r;

    auto image = MountedImageGet(pId);
    if (!image) return r;
    r = ValidateImage(*image);
    const auto choices = FolderCandidates(r, "Image");
    r.mMatches.clear();
    for (const auto& choice : choices) {
        auto match = choice.mResult.mMatches.front();
        match.mResolvedPath = "firy://" + std::to_string(pId) + "/";
        r.mMatches.push_back(match);
    }
    r.mInputFolder = r.mResolvedRoot = pImagePath;
    ExcludeAvailable(r, mAvailableAtOpen);
    return r;
}

struct cSetupWizard::sReadResult {
    sValidationResult validation;
    std::vector<sDiskImageChoice> images;
    std::shared_ptr<cMountedImage> image;
    sCueSheet cue;
    std::string tempIso;
    std::string error;
    ~sReadResult() {
        if (!tempIso.empty()) {
            std::error_code ec;
            std::filesystem::remove(std::filesystem::u8path(tempIso), ec);
        }
    }
};

std::shared_ptr<cSetupWizard::sReadResult> cSetupWizard::ReadPick(
    const std::string& path, const std::shared_ptr<std::atomic<bool>>& cancelled) {
    auto result = std::make_shared<sReadResult>();
    try {
        const auto isCancelled = [cancelled] { return cancelled->load(); };
        std::string imagePath;
        if (LooksLikeCueSheet(path)) {
            if (!ParseCueSheet(path, result->cue)) {
                result->error = "Could not read the CUE sheet.";
                return result;
            }
            const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
            result->tempIso = (std::filesystem::temp_directory_path() /
                ("openfodder-setup-" + std::to_string(stamp) + ".iso")).u8string();
            if (isCancelled() || ExtractTrackTo(result->cue, 1, result->tempIso) <= 0) {
                result->error = "Could not read the data track.";
                return result;
            }
            imagePath = result->tempIso;
        } else if (LooksLikeImagePath(path)) {
            imagePath = path;
        }
        if (!imagePath.empty()) {
            if (isCancelled()) return result;
            result->image = std::make_shared<cMountedImage>();
            if (!result->image->Open(imagePath)) {
                result->error = result->image->LastError();
                result->image.reset();
            } else {
                result->validation = ValidateImage(*result->image, isCancelled);
                const auto choices = FolderCandidates(result->validation, "Image");
                result->validation.mMatches.clear();
                for (const auto& choice : choices)
                    result->validation.mMatches.push_back(choice.mResult.mMatches.front());
            }
        } else {
            result->validation = ValidateFolderCollection(path, isCancelled);
            if (!isCancelled() && !std::any_of(result->validation.mMatches.begin(), result->validation.mMatches.end(),
                    [](const sVersionMatch& match) { return match.IsComplete(); }))
                result->images = FindDiskImages(path, &result->error);
        }
    } catch (const std::exception& error) {
        result->error = error.what();
    } catch (...) {
        result->error = "Could not check the selected data.";
    }
    return result;
}

void cSetupWizard::QueuePick(const std::string& path) {
    if (path.empty()) return;
    if (mReadCancelled) mReadCancelled->store(true);
    mPendingPick = path;
    mCheckingDrawn = false;
    mScreen = eScreen::Checking;
    g_Fodder->mMouse_Button_Left_Toggle = 0;
}

bool cSetupWizard::DispatchPick(const std::string& pPath, std::shared_ptr<sReadResult> read) {
    if (pPath.empty() || !g_ResourceMan)
        return false;

    // Reject the previous uncommitted choice before registering another set.
    // Otherwise its release entries could mask the newly mounted files.
    if (mLastMountedId > 0) {
        g_ResourceMan->unmountImage(mLastMountedId);
        mLastMountedId = 0;
        DiscardPendingCueTemp();
    }

    std::error_code pathError;
    mBrowsePath = std::filesystem::is_directory(pPath, pathError) ? pPath :
                  std::filesystem::path(pPath).parent_path().string();

    // .cue/.bin sheets: parse + extract track-1 to temp + mount.
    if (LooksLikeCueSheet(pPath))
        return DispatchCueSheet(pPath, read);

    // Standalone .mpg / .mp2 — CD32 second-track intro videos that the user
    // has already extracted from the disc. Copy directly to Run/Data/Intro.mpg.
    auto endsWithCI = [&](const char* ext) {
        const size_t L = std::strlen(ext);
        if (pPath.size() < L) return false;
        for (size_t i = 0; i < L; ++i) {
            char a = (char)std::tolower((unsigned char)pPath[pPath.size() - L + i]);
            char b = (char)std::tolower((unsigned char)ext[i]);
            if (a != b) return false;
        }
        return true;
    };
    if (endsWithCI(".mpg") || endsWithCI(".mp2") || endsWithCI(".mpeg")) {
        return DispatchStandaloneVideo(pPath);
    }

    if (LooksLikeImagePath(pPath)) {
        // Try to mount via firy. Success → registers versions and writes
        // synthesised release paths into mReleasePath/mReleaseFiles.
        std::string imageError;
        const int id = read ? (read->image ? g_ResourceMan->mountImage(read->image, read->validation, &imageError) : 0) :
                              g_ResourceMan->mountImage(pPath, &imageError);
        if (read && !read->error.empty()) imageError = read->error;
        if (id > 0) {
            // If the wizard had a previous pending mount that the user
            // never committed, drop it before we adopt the new one.
            if (mLastMountedId > 0 && mLastMountedId != id) {
                g_ResourceMan->unmountImage(mLastMountedId);
                DiscardPendingCueTemp();
            }
            mLastMountedId = id;
            mLastResult = read ? read->validation : ResultFromMountedImage(id, pPath);
            mLastResult.mInputFolder = mLastResult.mResolvedRoot = pPath;
            ExcludeAvailable(mLastResult, mAvailableAtOpen);
            mScreen = eScreen::Result;
            return true;
        }

        // firy opened nothing usable. Build an empty "no match" result so
        // the user gets explicit feedback instead of staying in the browser.
        sValidationResult empty;
        empty.mError = imageError;
        empty.mInputFolder = pPath;
        empty.mResolvedRoot = pPath;
        empty.mLooksLikeRoot = false;
        mLastMountedId = 0;
        mLastResult = empty;
        mScreen = eScreen::Result;
        return true;
    }

    // Prefer already-extracted files. A directory of disk captures is a
    // collection to choose from, rather than an unsuccessful loose-file scan.
    auto result = read ? read->validation : ValidateFolderCollection(pPath);
    ExcludeAvailable(result, mAvailableAtOpen);
    if (result.mAlreadyAvailable) { EnterResult(result); return true; }
    auto folderChoices = FolderCandidates(result, "Selected folder");
    if (folderChoices.size() > 1) {
        mCandidates = std::move(folderChoices);
        mCandidateFolder = pPath;
        mCandidateStart = 0;
        mScreen = eScreen::Locate;
        return true;
    }
    if (folderChoices.size() == 1)
        result = folderChoices.front().mResult;
    const bool complete = std::any_of(result.mMatches.begin(), result.mMatches.end(),
        [](const sVersionMatch& m) { return m.IsComplete(); });
    if (complete) { EnterResult(result); return true; }
    std::string imageError;
    const auto images = read ? read->images : FindDiskImages(pPath, &imageError);
    if (read) imageError = read->error;
    if (images.size() == 1) { QueuePick(images.front().mPath); return true; }
    if (!images.empty()) {
        mCandidates.clear();
        for (const auto& image : images) {
            sScanCandidate choice;
            choice.mPath = image.mPath;
            choice.mLabel = image.mName;
            choice.mFormat = image.mFormat;
            choice.mDisks = image.mDisks;
            choice.mIsImage = true;
            mCandidates.push_back(std::move(choice));
        }
        mCandidateFolder = pPath;
        mCandidateStart = 0;
        mScreen = eScreen::Locate;
        return true;
    }
    result.mError = imageError;
    EnterResult(result);
    return true;
}

bool cSetupWizard::DispatchCueSheet(const std::string& pCuePath, std::shared_ptr<sReadResult> read) {
    sCueSheet sheet;
    if (read) sheet = read->cue;
    if ((read && (!read->error.empty() || !read->image)) || (!read && !ParseCueSheet(pCuePath, sheet))) {
        // Surface a "no match" result so the user knows the sheet failed.
        sValidationResult empty;
        empty.mInputFolder = pCuePath;
        empty.mResolvedRoot = pCuePath;
        empty.mLooksLikeRoot = false;
        mLastMountedId = 0;
        mLastResult = empty;
        mScreen = eScreen::Result;
        return true;
    }

    // Track 1 must be the data track. CUE doesn't constrain ordering but
    // every CD32 dump in the wild puts data at track 1.
    const sCueTrack* t1 = sheet.FindTrack(1);
    if (!t1) {
        sValidationResult empty;
        empty.mInputFolder = pCuePath;
        empty.mResolvedRoot = pCuePath;
        mLastMountedId = 0;
        mLastResult = empty;
        mScreen = eScreen::Result;
        return true;
    }

    // Extract track-1 to a temp .iso alongside the cue. We use a fixed
    // suffix so re-running the wizard with the same cue overwrites
    // gracefully instead of accumulating stale temp files.
    std::string tempIso = read ? read->tempIso : pCuePath + ".track1.iso.tmp";
    const int wrote = read ? 1 : ExtractTrackTo(sheet, 1, tempIso);
    if (wrote <= 0) {
        sValidationResult empty;
        empty.mInputFolder = pCuePath;
        empty.mResolvedRoot = pCuePath;
        mLastMountedId = 0;
        mLastResult = empty;
        mScreen = eScreen::Result;
        return true;
    }

    // Drop any previous pending mount + temp file before adopting this one.
    if (mLastMountedId > 0)
        g_ResourceMan->unmountImage(mLastMountedId);
    DiscardPendingCueTemp();

    const int id = read ? g_ResourceMan->mountImage(read->image, read->validation) : g_ResourceMan->mountImage(tempIso);
    if (id <= 0) {
        // Track-1 extracted OK but firy can't recognise it as ISO9660.
        // Drop the temp file so we don't litter.
        std::remove(tempIso.c_str());

        sValidationResult empty;
        empty.mInputFolder = pCuePath;
        empty.mResolvedRoot = pCuePath;
        mLastMountedId = 0;
        mLastResult = empty;
        mScreen = eScreen::Result;
        return true;
    }

    mLastMountedId = id;
    mPendingCuePath = pCuePath;
    mPendingTempIsoPath = tempIso;
    if (read) read->tempIso.clear();

    // If the cue has a track 2, remember it so COPY TO DATA can extract it
    // to Run/Data/Intro.mpg as the CD32 intro video.
    if (sheet.FindTrack(2))
        mPendingCueTrack2 = 2;
    else
        mPendingCueTrack2 = 0;

    mLastResult = read ? read->validation : ResultFromMountedImage(id, pCuePath);
    mLastResult.mInputFolder = mLastResult.mResolvedRoot = pCuePath;
    ExcludeAvailable(mLastResult, mAvailableAtOpen);
    mScreen = eScreen::Result;
    return true;
}

bool cSetupWizard::DispatchStandaloneVideo(const std::string& pVideoPath) {
    if (pVideoPath.empty() || !g_ResourceMan)
        return false;

    std::string cwd = g_ResourceMan->getcwd();
    std::replace(cwd.begin(), cwd.end(), '\\', '/');
    if (!cwd.empty() && cwd.back() != '/')
        cwd.push_back('/');
    const std::string destDir = cwd + "Data/";
    const std::string destPath = destDir + "Intro.mpg";

    // Make sure Data/ exists. ensureDirExists is in MountedImage.cpp; we
    // duplicate the small mkdir here to avoid exposing that helper publicly.
#ifdef _WIN32
    _mkdir(destDir.c_str());
#else
    mkdir(destDir.c_str(), 0755);
#endif

    std::ifstream in(pVideoPath.c_str(), std::ios::binary);
    std::ofstream out(destPath.c_str(), std::ios::binary | std::ios::trunc);
    if (!in || !out)
        return false;

    std::vector<char> buf(64 * 1024);
    while (in) {
        in.read(buf.data(), (std::streamsize)buf.size());
        const std::streamsize got = in.gcount();
        if (got <= 0)
            break;
        out.write(buf.data(), got);
        if (!out)
            return false;
    }

    // Surface success on the Result screen as a one-line "did the thing".
    sValidationResult r;
    r.mInputFolder = pVideoPath;
    r.mResolvedRoot = destPath;
    r.mLooksLikeRoot = false;
    mLastMountedId = 0;
    mLastResult = r;
    mScreen = eScreen::Result;
    mResourcesChanged = true;     // refresh + SaveIni after wizard closes
    return true;
}

void cSetupWizard::DiscardPendingCueTemp() {
    if (!mPendingTempIsoPath.empty()) {
        std::remove(mPendingTempIsoPath.c_str());
        mPendingTempIsoPath.clear();
    }
    mPendingCuePath.clear();
    mPendingCueTrack2 = 0;
}

void cSetupWizard::EnterResult(const sValidationResult& pResult) {
    mLastResult = pResult;
    mScreen = eScreen::Result;
}

void cSetupWizard::CommitPick(const std::string& pFolder) {
    if (pFolder.empty() || !g_ResourceMan)
        return;

    // Normalise separators and trim trailing slashes except at drive roots.
    std::string p = pFolder;
    std::replace(p.begin(), p.end(), '\\', '/');
    while (p.size() > 1 && !(p.size() == 3 && p[1] == ':') && p.back() == '/')
        p.pop_back();

    if (g_ResourceMan->addUserDir(p)) {
        mResourcesChanged = true;
    }
}

int cSetupWizard::CommitCopyFromRawFolder() {
    if (!g_ResourceMan)
        return 0;

    // Destination root: <cwd>/Data/<release>/. findVersions() picks this
    // up automatically because cwd is always in mAllPaths.
    std::string cwd = g_ResourceMan->getcwd();
    std::replace(cwd.begin(), cwd.end(), '\\', '/');
    if (!cwd.empty() && cwd.back() != '/')
        cwd.push_back('/');

    int totalWritten = 0;
    bool anyVersionCopied = false;

    for (auto& match : mLastResult.mMatches) {
        if (!match.mVersion || !match.IsComplete() || !match.mIsRawFolder)
            continue;
        if (match.mResolvedPath.empty())
            continue;

        // Source: the raw folder we matched (with trailing /).
        // Destination: cwd/Data/<release>/.
        std::string srcDir = match.mResolvedPath;
        if (!srcDir.empty() && srcDir.back() != '/')
            srcDir.push_back('/');

        std::string destDir = cwd + "Data/" + match.mVersion->mDataPath + "/";

        const auto destination = FileSystem::Resolve(destDir, true);
        if (destination.empty()) return -1;
        std::error_code copyError;
        std::filesystem::create_directories(destination, copyError);
        if (copyError) return -1;

        for (const auto& kf : match.mVersion->mFiles) {
            const auto source = FileSystem::Resolve(srcDir + kf.mName);
            if (source.empty()) return -1;
            const auto target = FileSystem::Resolve((destination / source.filename()).u8string(), true);
            if (target.empty()) return -1;
            // Selecting our own Data/<release> is valid, but opening the
            // same file for output would truncate the source before reading.
            copyError.clear();
            if (std::filesystem::equivalent(source, target, copyError)) {
                ++totalWritten;
                continue;
            }
            std::ifstream in(source, std::ios::binary);
            std::ofstream out(target, std::ios::binary | std::ios::trunc);
            if (!in || !out)
                return -1;

            std::vector<char> buf(64 * 1024);
            while (in) {
                in.read(buf.data(), (std::streamsize)buf.size());
                std::streamsize got = in.gcount();
                if (got <= 0) break;
                out.write(buf.data(), got);
                if (!out)
                    return -1;
            }
            ++totalWritten;
        }
        anyVersionCopied = true;
    }

    if (!anyVersionCopied)
        return 0;

    mResourcesChanged = true;     // refresh + SaveIni after wizard closes
    return totalWritten;
}

int cSetupWizard::CommitCopyFromImage() {
    if (mLastMountedId <= 0 || !g_ResourceMan)
        return 0;

    auto image = MountedImageGet(mLastMountedId);
    if (!image)
        return 0;

    // Destination: alongside the engine binary, in <cwd>/Data/<release>/.
    // findVersions() will pick this up automatically on the next refresh()
    // because cwd is always the first entry in mAllPaths (addDefaultDirs).
    std::string cwd = g_ResourceMan->getcwd();
    std::replace(cwd.begin(), cwd.end(), '\\', '/');
    if (!cwd.empty() && cwd.back() != '/')
        cwd.push_back('/');

    int totalWritten = 0;
    bool anyVersionCopied = false;

    for (auto& match : mLastResult.mMatches) {
        if (!match.mVersion || !match.IsComplete())
            continue;

        // Build the subset of basename-lower the engine expects for this
        // version. This is exactly mFiles, lower-cased.
        std::vector<std::string> wanted;
        wanted.reserve(match.mVersion->mFiles.size());
        for (auto& f : match.mVersion->mFiles) {
            std::string lower = f.mName;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            wanted.push_back(std::move(lower));
        }

        std::string destDir = cwd + "Data/" + match.mVersion->mDataPath + "/";
        const int n = image->CopyToFolder(destDir, wanted);
        if (n < 0) {
            // Mid-copy I/O failure. We leave whatever was already written
            // on disk — partial overlays of correct files are fine because
            // the user can re-run the wizard. Surface the failure via the
            // return path.
            return -1;
        }
        totalWritten += n;
        if (n > 0)
            anyVersionCopied = true;
    }

    if (!anyVersionCopied)
        return 0;

    // CD32 cue/bin: also extract track 2 (MPEG video) into Run/Data/Intro.mpg
    // so the engine's intro video loader picks it up. Track 2 is raw stream
    // data — we copy verbatim (ExtractTrackTo handles the byte range).
    if (!mPendingCuePath.empty() && mPendingCueTrack2 > 0) {
        sCueSheet sheet;
        if (ParseCueSheet(mPendingCuePath, sheet)) {
            std::string introDest = cwd + "Data/Intro.mpg";
            const int v = ExtractTrackTo(sheet, mPendingCueTrack2, introDest);
            if (v > 0)
                totalWritten += v;
            // Failure to extract the video is non-fatal — the game still
            // plays without it.
        }
    }

    // Drop the mount: the image's job is done now that files are on disk.
    // We deliberately DO NOT call addUserDir(cwd) here — cwd is implicit
    // in mAllPaths from addDefaultDirs(), so writing it back into [paths]
    // would just be noise.
    g_ResourceMan->unmountImage(mLastMountedId);
    mLastMountedId = 0;
    DiscardPendingCueTemp();

    // Tell Setup_Wizard_Run to refresh + SaveIni. Because we unmounted, no
    // imageN= entry will be persisted — only the previous-attempts cleanup.
    mResourcesChanged = true;
    return totalWritten;
}

void cSetupWizard::OnPathDropped(const std::string& pPath) {
    // Treat dropped folders/images as if the user picked them in the browser.
    QueuePick(pPath);
}

void cSetupWizard::ConfirmAllGames() {
    sValidationResult selected;
    selected.mInputFolder = selected.mResolvedRoot = mCandidateFolder;
    for (const auto& candidate : mCandidates) {
        if (candidate.mIsImage || candidate.mResult.mMatches.size() != 1)
            continue;
        auto match = candidate.mResult.mMatches.front();
        if (!match.IsComplete()) continue;
        match.mIsRawFolder = true;
        selected.mMatches.push_back(std::move(match));
    }
    if (selected.mMatches.size() > 1) {
        // A folder import must not inherit a disk mount from a previous
        // unconfirmed result reached through Escape.
        if (mLastMountedId > 0) {
            g_ResourceMan->unmountImage(mLastMountedId);
            mLastMountedId = 0;
            DiscardPendingCueTemp();
        }
        EnterResult(selected);
    }
}

void cSetupWizard::UseCurrentGame() {
    if (mLastResult.mResolvedRoot.empty()) return;
    const bool complete = std::any_of(mLastResult.mMatches.begin(), mLastResult.mMatches.end(),
        [](const sVersionMatch& match) { return match.IsComplete(); });
    if (!complete) return;
    if (mLastMountedId > 0) {
        // Keep the existing mount; Setup_Wizard_Run persists imageN= on exit.
        mResourcesChanged = true;
    } else CommitPick(mLastResult.mResolvedRoot);
    mDone = true;
}

void cSetupWizard::OnRowClick(int16 action, int16 arg) {
    // Debounce.
    const uint32_t kDebounceMs = 120;
    const uint32_t kRepeatMs = 90;
    const uint32_t t = now_ms_u32();
    if (action == mLastInputAction && arg == mLastInputArg) {
        if (t - mLastInputMs < kRepeatMs) return;
    } else {
        if (t - mLastInputMs < kDebounceMs) return;
    }
    mLastInputMs = t;
    mLastInputAction = action;
    mLastInputArg = arg;

    switch (action) {
    case GUIACT_SW_PREVIOUS: OnScrollUp(); return;
    case GUIACT_SW_NEXT: OnScrollDown(); return;
    case GUIACT_SW_LOCATE:
        mScreen = eScreen::Locate;
        return;
    case GUIACT_SW_CONTINUE_DEMO:
        mDone = true;
        return;
    case GUIACT_SW_QUIT:
        // Trigger engine exit. cFodder::Exit isn't accessible from here cleanly;
        // the wizard loop in Setup_Wizard_Run watches for this.
        mDone = true;
        return;
    case GUIACT_SW_BROWSE:
        EnterBrowse();
        return;
    case GUIACT_SW_BROWSE_NATIVE: {
        // Open the OS folder picker. Result lands in cFodder::mDroppedPaths
        // via the SDL callback below; the per-frame drain in Setup_Wizard_Run
        // routes it through OnPathDropped() like a real drag-and-drop.
        struct sCallbackUserdata { /* nothing — callback writes via global */ };
        auto cb = [](void* /*userdata*/, const char* const* filelist, int /*filter*/) {
            if (!filelist || !filelist[0])
                return;
            if (g_Fodder)
                g_Fodder->mDroppedPaths.push_back(filelist[0]);
        };
        SDL_Window* win = (g_Fodder && g_Fodder->mWindow) ? g_Fodder->mWindow->GetWindow() : nullptr;
        SDL_ShowOpenFolderDialog(cb, nullptr, win, nullptr, false);
        return;
    }
    case GUIACT_SW_BACK_TO_WELCOME:
        OnBack();
        return;
    case GUIACT_SW_USE_CANDIDATE: {
        if (arg < 0 || arg >= (int16)mCandidates.size())
            return;
        const auto c = mCandidates[(size_t)arg];
        if (!c.mIsImage && c.mResult.mMatches.size() > 1) {
            mCandidates = FolderCandidates(c.mResult, "Collection");
            mCandidateFolder = c.mPath;
            mCandidateStart = 0;
            return;
        }
        if (!c.mIsImage && c.mResult.mMatches.size() == 1) {
            auto selected = c.mResult;
            ExcludeAvailable(selected, mAvailableAtOpen);
            EnterResult(selected);
        } else QueuePick(c.mPath);
        return;
    }
    case GUIACT_SW_ADD_ALL:
        ConfirmAllGames();
        return;
    case GUIACT_SW_SAVE_AND_CONTINUE:
        UseCurrentGame();
        return;
    case GUIACT_SW_COPY_TO_DATA: {
        // Two flavours: image-mount (mLastMountedId > 0) or raw-extracted
        // folder match (mLastResult has any mIsRawFolder match). Both lead
        // to the same outcome: files land in <cwd>/Data/<release>/.
        int n = 0;
        if (mLastMountedId > 0) {
            n = CommitCopyFromImage();
        } else {
            bool hasRaw = false;
            for (auto& m : mLastResult.mMatches)
                if (m.IsComplete() && m.mIsRawFolder) { hasRaw = true; break; }
            if (hasRaw)
                n = CommitCopyFromRawFolder();
        }
        if (n <= 0) {
            // Either nothing matched or an I/O failure mid-copy. Stay on
            // Result so the user can choose KEEP MOUNTED / TRY AGAIN.
            return;
        }
        mDone = true;
        return;
    }
    case GUIACT_SW_TRY_AGAIN:
        // If the user is rejecting a pending image mount, unmount it before
        // sending them back to Locate. Folders need no cleanup.
        if (mLastMountedId > 0) {
            g_ResourceMan->unmountImage(mLastMountedId);
            mLastMountedId = 0;
        }
        DiscardPendingCueTemp();
        mScreen = eScreen::Locate;
        return;
    }
}

void cSetupWizard::Tick() {
    bool checkingInProgress = false;
#ifndef EMSCRIPTEN
    if (mAutoScanTask.valid() && mAutoScanTask.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
        try {
            auto candidates = mAutoScanTask.get();
            if (mCandidateFolder.empty())
                mCandidates = std::move(candidates);
        } catch (...) {
            // Browsing remains available if an automatic suggestion fails.
        }
    }
    if (mReadTask.valid() && mReadTask.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
        try {
            auto read = mReadTask.get();
            if (read && !mReadCancelled->load() && mScreen == eScreen::Checking)
                DispatchPick(read->validation.mInputFolder, read);
        } catch (const std::exception& error) {
            if (!mReadCancelled->load() && mScreen == eScreen::Checking) {
                sValidationResult failed;
                failed.mError = error.what();
                EnterResult(failed);
            }
        }
    }
    checkingInProgress = mReadTask.valid();
#endif
    if (mScreen == eScreen::Checking && mCheckingDrawn && !mPendingPick.empty() && !checkingInProgress) {
        std::string path;
        path.swap(mPendingPick);
        if (mLastMountedId > 0) {
            g_ResourceMan->unmountImage(mLastMountedId);
            mLastMountedId = 0;
            DiscardPendingCueTemp();
        }
        mReadCancelled = std::make_shared<std::atomic<bool>>(false);
        auto cancelled = mReadCancelled;
        // Validation reads host files only. Retain resources until the worker
        // retires; all UI and resource-registration changes stay on this thread.
        auto resources = g_ResourceMan;
#ifdef EMSCRIPTEN
        auto read = ReadPick(path, cancelled);
        read->validation.mInputFolder = path;
        DispatchPick(path, read);
#else
        mReadTask = std::async(std::launch::async, [path, cancelled, resources] {
            auto read = ReadPick(path, cancelled);
            read->validation.mInputFolder = path;
            return read;
        });
#endif
    }
    if (mScreen == eScreen::Browse && mBrowser) {
        mBrowser->Tick();
        if (mBrowser->IsDone()) {
            // Browser already invoked one of our callbacks.
            mBrowser.reset();
            // mScreen was either updated to Result or left at Locate.
            if (mScreen == eScreen::Browse)
                mScreen = eScreen::Locate;
        }
    }

    // Public-matchmaking pair-code capture used to live here as a sub-screen
    // text-input pump. Pairing is now handled on demand by MultiplayerMenu
    // (see RequireHubToken / DrawAuthPairingMenu in
    // Source/Network/MultiplayerMenu.cpp), so the wizard tick stays narrow.
}

// Transparent controls share the main menu's hover outline and text treatment.
static void SetupButton(cSetupWizard* wizard, const std::string& label,
                        int left, int right, int y, int action, int arg = 0) {
    const bool hover = g_Fodder->mMouseX + 0x20 >= left && g_Fodder->mMouseX + 0x20 <= right &&
        g_Fodder->mMouseY >= y && g_Fodder->mMouseY <= y + 15;
    g_Fodder->String_Print_Small_CentreInBox(label, left, right, y + 5);
    if (hover) g_Fodder->Briefing_DrawBox(left, y, right - left, 16, 0xF3);
    g_Fodder->mGUI_Temp_X = left;
    g_Fodder->mGUI_Temp_Y = y;
    g_Fodder->mGUI_Temp_Width = right - left;
    g_Fodder->mGUI_Draw_LastHeight = 10;
    g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, wizard, action, arg);
}

static void SetupCaption(const std::string& text, int y) {
    // Paths, disk counts and format lists need punctuation and digits;
    // the sidebar caption font only contains A-Z and spaces.
    g_Fodder->String_Print_Small_CentreInBox(FitSetupText(text, 292), 14, 306, y);
}

// Compact 4x5 lettering for collection locations. Use the existing briefing
// glyph painter so its palette and shadow match the rest of the menu.
static void SetupCollectionPath(std::string path, int x, int y) {
    static const uint8 letters[][5] = {
        {6,9,15,9,9}, {14,9,14,9,14}, {7,8,8,8,7}, {14,9,9,9,14},
        {15,8,14,8,15}, {15,8,14,8,8}, {7,8,11,9,7}, {9,9,15,9,9},
        {7,2,2,2,7}, {1,1,1,9,6}, {9,10,12,10,9}, {8,8,8,8,15},
        {9,15,15,9,9}, {9,13,11,9,9}, {6,9,9,9,6}, {14,9,14,8,8},
        {6,9,9,11,7}, {14,9,14,10,9}, {7,8,6,1,14}, {7,2,2,2,2},
        {9,9,9,9,6}, {9,9,9,6,6}, {9,9,15,15,9}, {9,9,6,9,9},
        {5,5,2,2,2}, {15,1,6,8,15},
        {6,9,9,9,6}, {2,6,2,2,7}, {6,9,2,4,15}, {14,1,6,1,14},
        {9,9,15,1,1}, {15,8,14,1,14}, {7,8,14,9,6}, {15,1,2,4,4},
        {6,9,6,9,6}, {6,9,7,1,14}
    };
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string text;
    for (unsigned char c : path) {
        if ((c & 0xC0) == 0x80) continue;
        text += c < 128 ? char(std::toupper(c)) : '?';
    }
    constexpr size_t maxLetters = 54; // 270 pixels including character spacing.
    if (text.size() > maxLetters)
        text = ".." + text.substr(text.size() - (maxLetters - 2));
    for (unsigned char c : text) {
        uint8 rows[5]{};
        const auto index = alphabet.find(char(c));
        if (index != std::string::npos) {
            std::copy_n(letters[index], 5, rows);
        } else {
            const char* pattern = nullptr;
            switch (c) {
            case ':': pattern = "02020"; break;
            case '/': pattern = "11248"; break;
            case '\\': pattern = "88421"; break;
            case '.': pattern = "00002"; break;
            case '-': pattern = "00600"; break;
            case '_': pattern = "0000F"; break;
            case '(': pattern = "24442"; break;
            case ')': pattern = "42224"; break;
            case '[': pattern = "64446"; break;
            case ']': pattern = "62226"; break;
            case ' ': break;
            default: pattern = "69202"; break;
            }
            if (pattern)
                for (size_t row = 0; row < 5; ++row)
                    rows[row] = pattern[row] == 'F' ? 15 : uint8(pattern[row] - '0');
        }
        sBriefingSpecialGlyph glyph{};
        glyph.mAscii = c;
        glyph.mWidth = 4;
        glyph.mHeight = 5;
        for (size_t row = 0; row < 5; ++row)
            glyph.mRows[row] = rows[row] << 4;
        g_Fodder->String_Print_DrawTinyGlyph(&glyph, x, y);
        x += 5;
    }
}

void cSetupWizard::DrawWelcome() {
    const bool retail = g_Fodder->mVersions->AnyRetailAvailable();
    g_Fodder->String_Print_Large(retail ? "ADD GAME DATA" : "WELCOME ABOARD", false, 19);
    NetworkMenu_DrawWrappedBody(retail ? "Choose another game folder or disk set." :
        "Play a demo, or add your own copy of Cannon Fodder.", 64, 272);
    SetupButton(this, "FIND GAME DATA", 54, 266, 105, GUIACT_SW_LOCATE);
    SetupButton(this, retail ? "BACK TO MENU" : "PLAY DEMO", 54, 266, 150, GUIACT_SW_CONTINUE_DEMO);
}

void cSetupWizard::DrawLocate() {
    const int footer = 175 + PLATFORM_BASED(0, 25);
    g_Fodder->String_Print_Large("FIND GAME DATA", false, 19);
    if (mCandidates.empty()) {
        bool searching = false;
#ifndef EMSCRIPTEN
        searching = mAutoScanTask.valid();
#endif
        NetworkMenu_DrawWrappedBody(searching ? "Looking for new game data..." :
            "Choose a game folder or disk image.", 76, 276);
        SetupCaption("YOU CAN ALSO DROP IT ONTO THIS WINDOW", 117);
    } else {
        SetupCaption(mCandidateFolder.empty() ? "CHOOSE A GAME OR FOLDER" :
            std::to_string(mCandidates.size()) + " FOUND IN THIS FOLDER", 44);
        const int pageSize = CandidatePageSize();
        const int rowSpacing = pageSize == 7 ? 12 : 30;
        mCandidateStart = std::max(0, std::min(mCandidateStart, int(mCandidates.size()) - 1));
        for (int row = 0; row < pageSize && mCandidateStart + row < int(mCandidates.size()); ++row) {
            const int index = mCandidateStart + row;
            const auto& candidate = mCandidates[index];
            const int y = 62 + row * rowSpacing;
            const sVersionMatch* best = nullptr;
            for (const auto& match : candidate.mResult.mMatches)
                if (!best || (match.IsComplete() && !best->IsComplete())) best = &match;
            const bool collection = !candidate.mIsImage && candidate.mResult.mMatches.size() > 1;
            auto folder = std::filesystem::u8path(candidate.mPath).lexically_normal();
            if (folder.filename().empty()) folder = folder.parent_path();
            const std::string title = collection ? folder.filename().u8string() + " - " +
                std::to_string(candidate.mResult.mMatches.size()) + " GAMES" :
                best ? GameName(best->mVersion) : candidate.mLabel;
            const bool extraLine = collection || candidate.mIsImage || (best && !best->IsComplete());
            const int rowHeight = extraLine ? 26 : 11;
            const bool hover = g_Fodder->mMouseX + 0x20 >= 14 && g_Fodder->mMouseX + 0x20 <= 306 &&
                g_Fodder->mMouseY >= y - 3 && g_Fodder->mMouseY <= y - 3 + rowHeight;
            if (hover) g_Fodder->Briefing_DrawBox(14, y - 3, 292, rowHeight, 0xF3);
            g_Fodder->String_Print_Small(FitSetupText(title, 274), 22, y);
            std::string detail;
            if (candidate.mIsImage) {
                detail = std::to_string(candidate.mDisks) + " " + candidate.mFormat +
                    (candidate.mDisks == 1 ? " DISK" : " DISKS");
                std::transform(detail.begin(), detail.end(), detail.begin(), [](unsigned char c) { return char(std::toupper(c)); });
            } else if (!collection && best && !best->IsComplete()) {
                detail = "FILES MISSING";
            }
            if (collection)
                SetupCollectionPath(candidate.mPath, 22, y + 13);
            else if (!detail.empty())
                g_Fodder->String_Print_Small(FitSetupText(detail, 274), 22, y + 13);
            g_Fodder->mGUI_Temp_X = 14;
            g_Fodder->mGUI_Temp_Y = y - 3;
            g_Fodder->mGUI_Temp_Width = 292;
            g_Fodder->mGUI_Draw_LastHeight = rowHeight - 5;
            g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_USE_CANDIDATE, index);
        }
        if (mCandidateStart > 0) SetupButton(this, "PREVIOUS", 14, 98, 153, GUIACT_SW_PREVIOUS);
        if (mCandidateStart + pageSize < int(mCandidates.size())) SetupButton(this, "NEXT", 222, 306, 153, GUIACT_SW_NEXT);
        g_Fodder->String_Print_Small_Right(std::to_string(mCandidateStart + 1) + " - " +
            std::to_string(std::min(mCandidateStart + pageSize, int(mCandidates.size()))), 157, 158);
        g_Fodder->String_Print_Small_Left("OF " + std::to_string(mCandidates.size()), 169, 158);
    }
    const bool addAll = !mCandidateFolder.empty() && std::count_if(mCandidates.begin(), mCandidates.end(),
        [](const sScanCandidate& candidate) {
            return !candidate.mIsImage && candidate.mResult.mMatches.size() == 1 &&
                candidate.mResult.mMatches.front().IsComplete();
        }) > 1;
    if (addAll) {
        SetupButton(this, "BACK", 14, 94, footer, GUIACT_SW_BACK_TO_WELCOME);
        SetupButton(this, "BROWSE", 105, 195, footer, GUIACT_SW_BROWSE);
        SetupButton(this, "ADD ALL", 206, 306, footer, GUIACT_SW_ADD_ALL);
    } else {
        SetupButton(this, "BROWSE", 165, 306, footer, GUIACT_SW_BROWSE);
        SetupButton(this, "BACK", 14, 145, footer, GUIACT_SW_BACK_TO_WELCOME);
    }
}

void cSetupWizard::DrawBrowse() {
    if (mBrowser) mBrowser->Draw();
}

void cSetupWizard::DrawResult() {
    const bool image = mLastMountedId > 0;
    const sVersionMatch* best = nullptr;
    for (const auto& match : mLastResult.mMatches)
        if (!best || (match.IsComplete() && !best->IsComplete()) ||
            (match.IsComplete() == best->IsComplete() && match.mFilesFound > best->mFilesFound)) best = &match;
    const bool ready = best && best->IsComplete() && mLastResult.mError.empty();
    const int footer = 175 + PLATFORM_BASED(0, 25);
    const bool bulk = !image && ready && mLastResult.mMatches.size() > 1;
    const std::string heading = bulk ? std::to_string(mLastResult.mMatches.size()) + " GAMES FOUND" : "GAME FOUND";
    g_Fodder->String_Print_Large(mLastResult.mAlreadyAvailable ? "ALREADY ADDED" : ready ? heading.c_str() : best ?
        (image ? "DISK SET INCOMPLETE" : "GAME INCOMPLETE") : "NO GAME FOUND", false, 19);
    if (best) SetupCaption(bulk ? "ADD ALL GAMES FROM THIS FOLDER" : FitSetupText(GameName(best->mVersion), 292), 44);
    if (mLastResult.mAlreadyAvailable) {
        const auto count = mLastResult.mAvailableMatches.size();
        SetupCaption(std::to_string(count) + (count == 1 ? " GAME FOUND" : " GAMES FOUND"), 48);
        NetworkMenu_DrawWrappedBody("Already available in your game menu.", 74, 276);
        SetupCaption("FOUND IN THESE FOLDERS", 108);
        std::vector<std::string> paths;
        for (const auto& match : mLastResult.mAvailableMatches)
            if (std::none_of(paths.begin(), paths.end(), [&](const std::string& path) {
                return FileSystem::Equal(path, match.mResolvedPath);
            })) paths.push_back(match.mResolvedPath);
        const size_t shown = std::min<size_t>(3, paths.size());
        for (size_t i = 0; i < shown; ++i)
            SetupCollectionPath(paths[i], 22, 126 + int(i) * 12);
        if (paths.size() > shown)
            SetupCollectionPath("AND " + std::to_string(paths.size() - shown) + " MORE FOLDERS", 22, 162);
        SetupButton(this, "BROWSE", 165, 306, footer, GUIACT_SW_BROWSE);
    } else if (ready) {
        const bool install = image || best->mIsRawFolder;
        const bool useDisks = image && mPendingCuePath.empty();
        SetupCaption(bulk ? "ALL COMPLETE GAMES WILL BE ADDED" :
            useDisks ? "PLAY DIRECTLY FROM THESE DISKS" : "ADD THIS GAME TO YOUR MENU", 78);
        SetupButton(this, bulk ? "ADD ALL " + std::to_string(mLastResult.mMatches.size()) + " GAMES" :
            install && !useDisks ? "COPY GAME FILES" : "ADD GAME", 54, 266, 105,
            install && !useDisks ? GUIACT_SW_COPY_TO_DATA : GUIACT_SW_SAVE_AND_CONTINUE);
        if (useDisks)
            SetupButton(this, "COPY FILES INSTEAD", 54, 266, 143, GUIACT_SW_COPY_TO_DATA);
    } else {
        const char* message = best ? (image ?
            "Keep all disks from the same set in one folder, then choose it again." :
            "Choose a folder with the complete game, or try its original disks.") :
            !mLastResult.mError.empty() ? "Could not read game data here. Try another folder or disk image." :
            "No Cannon Fodder data was found here. Choose a game folder or original disks.";
        NetworkMenu_DrawWrappedBody(message, 82, 276);
        SetupButton(this, "BROWSE", 165, 306, footer, GUIACT_SW_BROWSE);
    }
    SetupButton(this, "BACK", 14, 145, footer, GUIACT_SW_TRY_AGAIN);
}

void cSetupWizard::Draw() {
    // The setup runner prepares the same demo-safe scene as the home menu.
    // Drawing it here also keeps diagnostics and the browser on fresh pixels.
    if (g_Fodder->mMapLoaded && !g_Fodder->mStartParams->mDisableVideo) {
        g_Fodder->mGraphics->MapTiles_Draw();
        g_Fodder->Sprites_Draw();
    } else g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();
    auto* pixels = g_Fodder->mSurface->GetSurfaceBuffer();
    const size_t size = g_Fodder->mSurface->GetSurfaceBufferSize();
    const size_t stride = g_Fodder->mSurface->GetWidth();
    const std::vector<uint8> background(pixels, pixels + size);
    constexpr uint8 transparent = 0xFF;
    std::fill(pixels, pixels + size, transparent);
    g_Fodder->mString_GapCharID = 0;
    switch (mScreen) {
    case eScreen::Welcome: DrawWelcome(); break;
    case eScreen::Locate: DrawLocate(); break;
    case eScreen::Browse: DrawBrowse(); break;
    case eScreen::Result: DrawResult(); break;
    case eScreen::Checking:
        g_Fodder->String_Print_Large("CHECKING YOUR GAME", false, 19);
        SetupCaption("READING GAME FILES" + std::string((now_ms_u32() / 350) % 4, '.'), 91);
        SetupButton(this, "CANCEL", 54, 266, 150, GUIACT_SW_BACK_TO_WELCOME);
        mCheckingDrawn = true;
        break;
    }
    const std::vector<uint8> foreground(pixels, pixels + size);
    std::copy(background.begin(), background.end(), pixels);
    for (size_t y = 1; y + 1 < g_Fodder->mSurface->GetHeight(); ++y)
        for (size_t x = 1; x + 1 < stride; ++x) {
            if (foreground[y * stride + x] == transparent) continue;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    pixels[(y + dy) * stride + x + dx] = 0;
        }
    for (size_t i = 0; i < size; ++i)
        if (foreground[i] != transparent) pixels[i] = foreground[i];
    g_Fodder->mSurface->dimBackdrop(foreground);
}

} // namespace Setup

// ---------------------------------------------------------------------------
// cFodder::Setup_Wizard_Run — modelled on cFodder::About() and ::KeyboardShortcuts():
// own Cycle() loop, mInterruptCallback suppressed, and the wizard owns the full
// surface every frame.  This is the right shape because Amiga magazine demos
// (which cVersions::GetDemo prefers) ship WITHOUT eGFX_BRIEFING / eGFX_FONT
// sheets — calling String_Print_Large/Briefing_DrawBox/etc. on them renders
// garbage.  We therefore force-switch to a chrome-capable version
// (retail → PCFormat → any briefing-capable demo) before drawing, so the
// wizard always has real font glyphs and box graphics to draw with.
// ---------------------------------------------------------------------------

bool cFodder::Setup_Wizard_Run(bool pHaveAnyVersion) {
    // Force a version that actually has briefing/font sheets.  Without this,
    // an AmigaXMAS demo (the default cVersions::GetDemo() pick) leaves the
    // briefing sprite sheet empty — text and boxes render as garbage.
    const sGameVersion* chrome = mVersions->GetVersionForMenuChrome();
    if (chrome && chrome != mVersionCurrent)
        VersionSwitch(chrome);

    if (!mVersionCurrent || !mGraphics) {
        // No graphics at all — wizard cannot run.  Fall through; the caller
        // (Start) will then take the DataNotFound console path on the next
        // refresh if no retail was added.
        return false;
    }

    Setup::cSetupWizard wizard;
    wizard.Open(pHaveAnyVersion);

    // Engine prep — same shape About uses (Source/About.cpp:93-100).
    Phase_EngineReset();
    mService_Draw_List.clear();
    MainMenu_CreateBackground();
    MainMenu_PrepareScenery();
    Camera_Reset();
    MapTiles_ResetScrollState();
    // Full-screen menus begin at map column zero after cropping the border.
    mMapTile_Ptr += 3 * sizeof(uint16);
    mGraphics->PaletteSet();
    mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    mMouseX_Offset = -8;
    mMouseY_Offset = -8;
    Mouse_Setup();
    mMouse_Button_Left_Toggle = 0;
    mMap_Destroy_Tiles.clear();

#ifdef OPENFODDER_SETUP_DIAGNOSTICS
    static bool previewStarted = false;
    const char* output = std::getenv("OPENFODDER_SETUP_PREVIEW_DIR");
    if (output && *output && !previewStarted) {
        previewStarted = true;
        const char* folder = std::getenv("OPENFODDER_SETUP_PREVIEW_DISKS");
        const bool ok = folder && wizard.VerifyScreens(output, folder);
        wizard.Close();
        std::exit(ok ? 0 : 1);
    }
#endif

    // Suppress the parent interrupt callback so it doesn't try to draw
    // background sprites on top of the wizard.  Same pattern as
    // cFodder::KeyboardShortcuts (Fodder.cpp:3807-3815).
    const auto prevInterruptCallback = mInterruptCallback;
    mInterruptCallback = []() {};

    while (!wizard.IsDone() && !mExit) {
        // Drain any drag-and-drop paths the window event loop pushed in.
        if (!mDroppedPaths.empty()) {
            std::vector<std::string> drained;
            drained.swap(mDroppedPaths);
            for (auto& p : drained)
                wizard.OnPathDropped(p);
        }

        // Mouse wheel forwards to whichever sub-screen is active.
        if (mMouse_EventLastWheel.mY > 0) {
            wizard.OnScrollUp();
            mMouse_EventLastWheel.mY = 0;
        } else if (mMouse_EventLastWheel.mY < 0) {
            wizard.OnScrollDown();
            mMouse_EventLastWheel.mY = 0;
        }

        if (Mouse_Button_Left_Toggled() >= 0)
            GUI_Handle_Element_Mouse_Check(mGUI_Elements);

        if (mPhase_EscapeKeyAbort || mPhase_Aborted) {
            mPhase_EscapeKeyAbort = false;
            mPhase_Aborted = false;
            wizard.OnBack();
        }

        wizard.Tick();
        if (mPhase_InterruptTicks >= 3) {
            mPhase_InterruptTicks = 0;
            Mission_Sprites_Handle();
            Sound_Tick();
        }

        // Draw + present.  Reset the cursor sprite each frame so it doesn't
        // get clobbered by stray engine state.
        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        wizard.Draw();
        mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        Mouse_DrawCursor();

        Video_Sleep();
    }

    const bool changed = wizard.ResourcesChanged();
    wizard.Close();

    mInterruptCallback = prevInterruptCallback;

    if (changed) {
        // The wizard added at least one search root. Refresh the resource
        // manager so VersionSwitch can pick up retail data on the next call,
        // and persist the new [paths] section.
        g_ResourceMan->refresh();
        if (mStartParams)
            mStartParams->SaveIni();
    }

    mMouse_Button_Left_Toggle = 0;
    mMouse_EventLastButtonsPressed = 0;
    mKeyCode = 0;
    mKeyCodeAscii = 0;
    mInput_LastKey = 0;
    return changed;
}
