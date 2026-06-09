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
#include "Setup/SetupWizard.hpp"
#include "Setup/MountedImage.hpp"
#include "Setup/CueSheet.hpp"
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
    GUIACT_SW_USE_CANDIDATE,    // arg = index into mCandidates

    // Result screen
    GUIACT_SW_SAVE_AND_CONTINUE,    // mounted image: persist imageN= and re-mount each launch
    GUIACT_SW_COPY_TO_DATA,         // mounted image: copy files into <cwd>/Data/<release>, unmount
    GUIACT_SW_TRY_AGAIN,
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
    RunAutoScan();
}

void cSetupWizard::Close() {
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
    mCandidates = AutoScan();
}

void cSetupWizard::OnBack() {
    switch (mScreen) {
    case eScreen::Welcome:
        // From the welcome screen, Back == "continue with demo only".
        mDone = true;
        break;
    case eScreen::Locate:
        mScreen = eScreen::Welcome;
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
    case eScreen::Result:
        mScreen = eScreen::Locate;
        break;
    }
}

void cSetupWizard::OnScrollUp() {
    if (mScreen == eScreen::Browse && mBrowser) {
        mBrowser->OnScrollUp();
    }
}

void cSetupWizard::OnScrollDown() {
    if (mScreen == eScreen::Browse && mBrowser) {
        mBrowser->OnScrollDown();
    }
}

void cSetupWizard::EnterBrowse() {
    mBrowser = std::make_unique<cFileBrowser>();
    // Pick the most useful starting point: an existing candidate's parent if
    // we have one, else the user profile, else the cwd.
    std::string start;
    if (!mCandidates.empty()) {
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
            DispatchPick(pPicked);
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

    // Walk the version table; everything mountImage already registered for
    // this image carries a matching firy://<id>/ prefix on its release path.
    const std::string virtPrefix = "firy://" + std::to_string(pId) + "/";
    auto available = g_ResourceMan->GetAvailable();
    for (auto* v : available) {
        if (!v) continue;
        const std::string pathForVersion = g_ResourceMan->FindVersionPath(v);
        if (pathForVersion.size() < virtPrefix.size())
            continue;
        if (pathForVersion.compare(0, virtPrefix.size(), virtPrefix) != 0)
            continue;

        sVersionMatch m;
        m.mVersion = v;
        m.mResolvedPath = pathForVersion;
        m.mFilesExpected = v->mFiles.size();
        m.mFilesFound = v->mFiles.size();    // mountImage only registers complete sets
        m.mFilesMd5Match = m.mFilesFound;    // we don't MD5 mounted images (cheap-fast)
        r.mMatches.push_back(m);
    }
    return r;
}

bool cSetupWizard::DispatchPick(const std::string& pPath) {
    if (pPath.empty() || !g_ResourceMan)
        return false;

    // .cue/.bin sheets: parse + extract track-1 to temp + mount.
    if (LooksLikeCueSheet(pPath))
        return DispatchCueSheet(pPath);

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
        const int id = g_ResourceMan->mountImage(pPath);
        if (id > 0) {
            // If the wizard had a previous pending mount that the user
            // never committed, drop it before we adopt the new one.
            if (mLastMountedId > 0 && mLastMountedId != id) {
                g_ResourceMan->unmountImage(mLastMountedId);
                DiscardPendingCueTemp();
            }
            mLastMountedId = id;
            mLastResult = ResultFromMountedImage(id, pPath);
            mScreen = eScreen::Result;
            return true;
        }

        // firy opened nothing usable. Build an empty "no match" result so
        // the user gets explicit feedback instead of staying in the browser.
        sValidationResult empty;
        empty.mInputFolder = pPath;
        empty.mResolvedRoot = pPath;
        empty.mLooksLikeRoot = false;
        mLastMountedId = 0;
        mLastResult = empty;
        mScreen = eScreen::Result;
        return true;
    }

    // Folder path → existing validation flow.
    mLastMountedId = 0;
    sValidationResult r = ValidateFolder(pPath);
    EnterResult(r);
    return true;
}

bool cSetupWizard::DispatchCueSheet(const std::string& pCuePath) {
    sCueSheet sheet;
    if (!ParseCueSheet(pCuePath, sheet)) {
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
    std::string tempIso = pCuePath + ".track1.iso.tmp";
    const int wrote = ExtractTrackTo(sheet, 1, tempIso);
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

    const int id = g_ResourceMan->mountImage(tempIso);
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

    // If the cue has a track 2, remember it so COPY TO DATA can extract it
    // to Run/Data/Intro.mpg as the CD32 intro video.
    if (sheet.FindTrack(2))
        mPendingCueTrack2 = 2;
    else
        mPendingCueTrack2 = 0;

    mLastResult = ResultFromMountedImage(id, pCuePath);
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

    // Normalise: forward slashes, no trailing slash.
    std::string p = pFolder;
    std::replace(p.begin(), p.end(), '\\', '/');
    while (p.size() > 1 && p.back() == '/')
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

        // Make sure destDir exists. mkdir-p the chain.
        {
            std::string p;
            for (size_t i = 0; i < destDir.size(); ++i) {
                p.push_back(destDir[i]);
                if (destDir[i] == '/') {
                    if (p.size() > 1 && !(p.size() == 3 && p[1] == ':'))
#ifdef _WIN32
                        _mkdir(p.c_str());
#else
                        mkdir(p.c_str(), 0755);
#endif
                }
            }
        }

        // Copy each manifest file from src → dest. We trust ValidateFolder
        // already proved every file is present, so any read failure here is
        // an I/O issue (permissions, mid-copy disk full, etc.) and should
        // surface as -1.
        for (auto& kf : match.mVersion->mFiles) {
            // We need the on-disk casing for the source — re-list once and
            // build a lower→original map. Cheap: at most ~230 entries.
            static thread_local std::map<std::string, std::string> caseMap;
            static thread_local std::string lastDir;
            if (lastDir != srcDir) {
                lastDir = srcDir;
                caseMap.clear();
                auto entries = g_ResourceMan->DirectoryList(srcDir, "");
                for (auto& e : entries) {
                    std::string lo = e;
                    std::transform(lo.begin(), lo.end(), lo.begin(), ::tolower);
                    caseMap[lo] = e;
                }
            }
            std::string wantLower = kf.mName;
            std::transform(wantLower.begin(), wantLower.end(), wantLower.begin(), ::tolower);
            auto it = caseMap.find(wantLower);
            if (it == caseMap.end())
                continue;       // shouldn't happen given IsComplete()

            const std::string srcPath = srcDir + it->second;
            const std::string destPath = destDir + it->second;

            std::ifstream in(srcPath.c_str(), std::ios::binary);
            std::ofstream out(destPath.c_str(), std::ios::binary | std::ios::trunc);
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
    DispatchPick(pPath);
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
        mScreen = eScreen::Welcome;
        return;
    case GUIACT_SW_USE_CANDIDATE: {
        if (arg < 0 || arg >= (int16)mCandidates.size())
            return;
        const auto& c = mCandidates[(size_t)arg];
        if (c.mIsImage) {
            DispatchPick(c.mPath);
        } else {
            EnterResult(c.mResult);
        }
        return;
    }
    case GUIACT_SW_SAVE_AND_CONTINUE: {
        if (mLastResult.mResolvedRoot.empty())
            return;
        // Only save if at least one match is "real" (>=1 file found).
        bool any = false;
        for (auto& m : mLastResult.mMatches) {
            if (m.mFilesFound > 0) { any = true; break; }
        }
        if (!any)
            return;

        if (mLastMountedId > 0) {
            // Image is already mounted (DispatchPick did the mountImage()
            // call); just confirm & exit. The Setup_Wizard_Run trampoline
            // will SaveIni after we close.
            mResourcesChanged = true;
            mDone = true;
        } else {
            CommitPick(mLastResult.mResolvedRoot);
            mDone = true;
        }
        return;
    }
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

void cSetupWizard::DrawWelcome() {
    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("OPEN FODDER", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    int y = 0x20;
    y = NetworkMenu_DrawWrappedBody(
        "Cannon Fodder retail data was not found in any of the searched locations "
        "You can play the bundled demos right now or point at a folder of retail files",
        y, 290);
    y += 6;

    y = NetworkMenu_DrawWrappedBody(
        "Drag and drop a folder onto this window or use Locate Data to browse for one",
        y, 290);

    // Three buttons across the bottom.
    const size_t yBottom = 0xB8;
    const size_t buttonW = 0x52;
    const size_t gap = 0x06;
    const size_t totalW = (buttonW * 3) + (gap * 2);
    const size_t xStart = 160 - (totalW / 2);

    const size_t xLocateL = xStart;                    const size_t xLocateR = xLocateL + buttonW;
    const size_t xDemoL   = xLocateR + gap;            const size_t xDemoR   = xDemoL + buttonW;
    const size_t xQuitL   = xDemoR + gap;              const size_t xQuitR   = xQuitL + buttonW;

    g_Fodder->GUI_Button_Draw_SmallBoxAt("LOCATE", xLocateL, xLocateR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_LOCATE, 0);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("PLAY DEMO", xDemoL, xDemoR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_CONTINUE_DEMO, 0);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("QUIT", xQuitL, xQuitR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_QUIT, 0);
}

void cSetupWizard::DrawLocate() {
    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("LOCATE DATA", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    const size_t xRowL = 0x10;
    const size_t xRowR = 0x130;
    // ONE briefing-font line per candidate, with breathing room above and
    // below. Going to one line per row eliminated the label-vs-detect
    // overlap the previous two-line layout produced.
    const int rowH = 0x0E;
    int rowBaseY = 0x28;

    if (mCandidates.empty()) {
        NetworkMenu_DrawWrappedBody(
            "No retail data was found automatically Click BROWSE to pick a folder "
            "or drag and drop a folder onto this window",
            rowBaseY, 290);
    } else {
        g_Fodder->String_Print_Small("Auto-detected:", 0x10, rowBaseY);
        rowBaseY += 0x0C;

        // Locate's box sits between rowBaseY (~0x34=52) and the bottom
        // button row (yBottom 0xB8=184), so vertical budget is ~130 px.
        // With rowH=14 we can fit 9 visible candidates before crashing
        // into the BROWSE/SYSTEM/BACK row; cap at 9 with a "(+N more)"
        // tail row when there are extras.
        const int16 maxRows = 9;
        const int16 shown = (int16)std::min<size_t>(mCandidates.size(), (size_t)maxRows);

        const int16 boxX = (int16)(xRowL - 2);
        const int16 boxY = (int16)(rowBaseY - 4);
        const int16 boxW = (int16)(xRowR - xRowL + 4);
        const int16 boxH = (int16)((shown * rowH) + 8);
        g_Fodder->Briefing_DrawBox(boxX, boxY, boxW, boxH, 0xF3);
        g_Fodder->Briefing_DrawBox(boxX - 1, boxY - 1, boxW, boxH, 0xF2);

        // Each candidate row gets ONE truncated line. Format priority:
        //   1. Folder candidate that matched a complete release: show the
        //      detected version + path.  ("CANNON FODDER (AMIGA): /path/...")
        //   2. Folder candidate, partial match: same shape, with file count.
        //   3. Folder candidate, no match: just the path (de-duplicated).
        //   4. Image candidate: "<label>: <path>".
        //
        // Inner padding accounts for the box's stroke + a couple of pixels
        // of breathing space so text doesn't kiss the border.
        const int innerPadL = 6;
        const int innerPadR = 6;
        const int contentPx = (int)(xRowR - xRowL) - innerPadL - innerPadR;

        // String_Print_Small upper-cases its input before drawing, but
        // String_MeasureWidth does NOT. mFont_Briefing_Width has 0 widths
        // for lowercase a-z, which would make any lowercase string measure
        // as ~0 px wide and never trigger truncation. Pre-upper-case here
        // so the measurement matches the rendered glyph stream.
        auto measure = [](const std::string& s) {
            std::string up = s;
            std::transform(up.begin(), up.end(), up.begin(), ::toupper);
            return g_Fodder->String_MeasureWidth(mFont_Briefing_Width, up);
        };
        auto platformTag = [](const sGameVersion* v) -> const char* {
            if (!v) return "";
            if (v->mPlatform == ePlatform::Amiga) return "AMIGA";
            if (v->mPlatform == ePlatform::PC)    return "PC";
            return "";
        };

        // Two-part truncation: keep the identifying prefix (version tag)
        // intact and strip from the FRONT of the path tail with ".." prefix
        // so the row looks like:
        //     CANNON FODDER (AMIGA) [RAW] - ..PROJECTS/OPENFODDER/RUN
        // rather than ".. (PC) - D:/PROJECTS/OPENFODDER/RUN" which loses
        // the version name entirely (front-strip on the whole string).
        auto fitPrefixPath = [&](const std::string& pPrefix, const std::string& pPath) -> std::string {
            const std::string sep = " - ";
            std::string full = pPrefix + sep + pPath;
            if (measure(full) <= contentPx)
                return full;

            // Reserve room for prefix + sep + ".." + at-least-some-tail.
            const int prefixPx = measure(pPrefix + sep + "..");
            int tailBudget = contentPx - prefixPx;
            if (tailBudget < 8) {
                // Prefix alone overflows; fall back to whole-string front-trim.
                std::string t = full;
                while (!t.empty() && measure(".." + t) > contentPx)
                    t.erase(t.begin());
                return ".." + t;
            }

            std::string tail = pPath;
            while (!tail.empty() && measure(tail) > tailBudget)
                tail.erase(tail.begin());
            return pPrefix + sep + ".." + tail;
        };

        auto formatCandidate = [&](const sScanCandidate& c) -> std::string {
            if (c.mIsImage) {
                std::string prefix = c.mLabel.empty() ? std::string("IMAGE") : c.mLabel;
                return fitPrefixPath(prefix, c.mPath);
            }

            // Pick ONE representative complete match (retail beats demo;
            // first hit wins otherwise) and use its short name as the
            // prefix. Append "+N" if there are additional matches in the
            // same folder.
            const sVersionMatch* best = nullptr;
            int totalComplete = 0;
            bool anyRaw = false;
            for (auto& m : c.mResult.mMatches) {
                if (!m.mVersion || !m.IsComplete())
                    continue;
                ++totalComplete;
                if (m.mIsRawFolder) anyRaw = true;
                if (!best) { best = &m; continue; }
                if (m.IsRetail() && !best->IsRetail()) { best = &m; continue; }
                if (!m.IsRetail() && best->IsRetail()) continue;
            }

            if (best && best->mVersion) {
                std::ostringstream prefix;
                // Short name (≤12 chars) keeps the prefix compact so the
                // path tail has room. Falls back to the long name if the
                // short name is empty (shouldn't happen — every entry in
                // KnownGameVersions[] has one).
                if (!best->mVersion->mShortName.empty())
                    prefix << best->mVersion->mShortName;
                else
                    prefix << best->mVersion->mName;
                if (totalComplete > 1)
                    prefix << " +" << (totalComplete - 1);
                if (anyRaw) prefix << " RAW";
                return fitPrefixPath(prefix.str(), c.mPath);
            }

            // No complete match — fall back to a partial-match summary.
            for (auto& m : c.mResult.mMatches) {
                if (!m.mVersion) continue;
                std::ostringstream prefix;
                if (!m.mVersion->mShortName.empty())
                    prefix << m.mVersion->mShortName;
                else
                    prefix << m.mVersion->mName;
                prefix << " " << m.mFilesFound << "/" << m.mFilesExpected;
                return fitPrefixPath(prefix.str(), c.mPath);
            }
            return fitPrefixPath("FOUND", c.mPath);
        };

        for (int16 i = 0; i < shown; ++i) {
            const auto& c = mCandidates[(size_t)i];
            const size_t y = (size_t)(rowBaseY + (i * rowH));

            // formatCandidate already calls fitPrefixPath internally to fit
            // contentPx — no outer trim needed.
            g_Fodder->String_Print_Small_LeftInBox(
                formatCandidate(c),
                xRowL + (size_t)innerPadL, xRowR - (size_t)innerPadR, y);

            g_Fodder->mGUI_Temp_X = (int16)xRowL;
            g_Fodder->mGUI_Temp_Y = (int16)y;
            g_Fodder->mGUI_Temp_Width = (int16)(xRowR - xRowL);
            g_Fodder->mGUI_Draw_LastHeight = (int16)(rowH - 2);
            g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_USE_CANDIDATE, i);
        }
    }

    // Bottom row: in-engine browse, native OS picker, back.
    const size_t yBottom = 0xB8;
    const size_t buttonW = 0x52;
    const size_t gap = 0x06;
    const size_t totalW = (buttonW * 3) + (gap * 2);
    const size_t xStart = 160 - (totalW / 2);
    const size_t xBrowseL  = xStart;                       const size_t xBrowseR  = xBrowseL + buttonW;
    const size_t xSystemL  = xBrowseR + gap;               const size_t xSystemR  = xSystemL + buttonW;
    const size_t xBackL    = xSystemR + gap;               const size_t xBackR    = xBackL + buttonW;

    g_Fodder->GUI_Button_Draw_SmallBoxAt("BROWSE...", xBrowseL, xBrowseR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_BROWSE, 0);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("SYSTEM...", xSystemL, xSystemR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_BROWSE_NATIVE, 0);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("BACK", xBackL, xBackR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_BACK_TO_WELCOME, 0);
}

void cSetupWizard::DrawBrowse() {
    if (mBrowser)
        mBrowser->Draw();
}

void cSetupWizard::DrawResult() {
    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("RESULT", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    const bool isImage = (mLastMountedId > 0);

    // Per-line content budget: leave 8 px on each side. String_Print_Small
    // upper-cases internally; pre-upper-case for measurement so we don't
    // under-count lowercase chars (their width-table entries are 0).
    const int lineLeftPad  = 8;
    const int lineRightPad = 8;
    const int contentPx = 320 - lineLeftPad - lineRightPad;
    auto measure = [](const std::string& s) {
        std::string up = s;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        return g_Fodder->String_MeasureWidth(mFont_Briefing_Width, up);
    };
    auto trimRight = [&](const std::string& s) -> std::string {
        if (measure(s) <= contentPx)
            return s;
        // Tail-strip with a trailing ".." marker so the user sees we cut.
        std::string t = s;
        while (!t.empty() && measure(t + "..") > contentPx)
            t.pop_back();
        return t + "..";
    };
    auto trimPathFront = [&](const std::string& pPrefix, const std::string& pPath) -> std::string {
        std::string full = pPrefix + pPath;
        if (measure(full) <= contentPx)
            return full;
        std::string t = pPath;
        while (!t.empty() && measure(pPrefix + ".." + t) > contentPx)
            t.erase(t.begin());
        return pPrefix + ".." + t;
    };

    int y = 0x20;
    {
        const char* hdr = isImage ? "Image: " : "Folder: ";
        g_Fodder->String_Print_Small(trimPathFront(hdr, mLastResult.mResolvedRoot), lineLeftPad, y);
    }
    y += 0x0E;

    if (mLastResult.mMatches.empty()) {
        NetworkMenu_DrawWrappedBody(
            isImage
                ? "Image opened but no Cannon Fodder release was matched. The disk image's filenames "
                  "do not match any version we know about."
                : "No Cannon Fodder data was recognised in this folder.",
            y, 300);
    } else {
        bool anyComplete = false;
        for (auto& m : mLastResult.mMatches)
            if (m.IsComplete()) { anyComplete = true; break; }

        const char* heading = anyComplete ? "Detected releases:" : "Partial detection:";
        g_Fodder->String_Print_Small(heading, lineLeftPad, y);
        y += 0x0C;

        const int rowH = 0x0E;
        const int maxRows = 9;
        int shown = 0;
        for (auto& m : mLastResult.mMatches) {
            if (shown >= maxRows) break;
            std::string line = "  " + DescribeMatch(m);
            g_Fodder->String_Print_Small(trimRight(line), lineLeftPad, y);
            y += rowH;
            ++shown;
        }
    }

    // Buttons.
    bool anyHit = false;
    for (auto& m : mLastResult.mMatches)
        if (m.mFilesFound > 0) { anyHit = true; break; }

    const size_t yBottom = 0xB8;

    // The public-matchmaking opt-in checkbox used to live here. It's gone:
    // pairing is now prompted on demand by MultiplayerMenu the first time
    // the user tries to host or find an internet game (RequireHubToken in
    // Source/Network/MultiplayerMenu.cpp), which is a more honest place for
    // it than a checkbox below the data-locator's Apply buttons.

    if (isImage) {
        // Image picks: COPY (default, lands files on disk) + KEEP MOUNTED
        // (persist as imageN= and re-mount each launch) + TRY AGAIN.
        // KEEP MOUNTED is suppressed for cue/bin pickups because the
        // mounted source is a temp .iso we extracted from the cue — it
        // wouldn't survive past Close().
        const bool isCueMount = !mPendingCuePath.empty();
        const int btnCount = isCueMount ? 2 : 3;
        const size_t buttonW = 0x52;
        const size_t gap = 0x06;
        const size_t totalW = (buttonW * btnCount) + (gap * (btnCount - 1));
        const size_t xStart = 160 - (totalW / 2);
        const size_t xCopyL = xStart;                      const size_t xCopyR = xCopyL + buttonW;
        size_t xMountL = 0, xMountR = 0;
        size_t xBackL = 0, xBackR = 0;
        if (isCueMount) {
            xBackL  = xCopyR + gap;                        xBackR  = xBackL + buttonW;
        } else {
            xMountL = xCopyR + gap;                        xMountR = xMountL + buttonW;
            xBackL  = xMountR + gap;                       xBackR  = xBackL + buttonW;
        }

        if (anyHit) {
            g_Fodder->GUI_Button_Draw_SmallBoxAt("COPY TO DATA", xCopyL, xCopyR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
            g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_COPY_TO_DATA, 0);

            if (!isCueMount) {
                g_Fodder->GUI_Button_Draw_SmallBoxAt("KEEP MOUNTED", xMountL, xMountR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
                g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_SAVE_AND_CONTINUE, 0);
            }
        } else {
            g_Fodder->GUI_Button_Draw_SmallBoxAt("COPY TO DATA", xCopyL, xCopyR, yBottom, 0xC8, 0xC8, eTextAlign::Centre);
            if (!isCueMount)
                g_Fodder->GUI_Button_Draw_SmallBoxAt("KEEP MOUNTED", xMountL, xMountR, yBottom, 0xC8, 0xC8, eTextAlign::Centre);
        }

        g_Fodder->GUI_Button_Draw_SmallBoxAt("TRY AGAIN", xBackL, xBackR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
        g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_TRY_AGAIN, 0);
    } else {
        // Folder picks: action depends on whether the matched files were
        // found in the canonical engine layout (<root>/Data/<release>/) or
        // raw-extracted directly in the picked folder.
        //
        //   Engine layout (mIsRawFolder=false on every complete match):
        //       SAVE & CONTINUE — addUserDir(folder) + persist [paths].
        //       The engine's findVersions will discover Data/<release>/
        //       on its own.
        //
        //   Raw extraction (any complete match has mIsRawFolder=true):
        //       COPY TO DATA — copy the matched manifest into
        //       <cwd>/Data/<release>/ so findVersions can see them.
        bool anyRaw = false;
        for (auto& m : mLastResult.mMatches)
            if (m.IsComplete() && m.mIsRawFolder) { anyRaw = true; break; }

        const size_t buttonW = 0x60;
        const size_t gap = 0x08;
        const size_t totalW = (buttonW * 2) + gap;
        const size_t xStart = 160 - (totalW / 2);
        const size_t xActL  = xStart;                      const size_t xActR  = xActL + buttonW;
        const size_t xBackL = xActR + gap;                 const size_t xBackR = xBackL + buttonW;

        const char* primaryLabel = anyRaw ? "COPY TO DATA" : "USE AS IS";
        const int16 primaryAction = anyRaw ? GUIACT_SW_COPY_TO_DATA : GUIACT_SW_SAVE_AND_CONTINUE;

        if (anyHit) {
            g_Fodder->GUI_Button_Draw_SmallBoxAt(primaryLabel, xActL, xActR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
            g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, primaryAction, 0);
        } else {
            g_Fodder->GUI_Button_Draw_SmallBoxAt(primaryLabel, xActL, xActR, yBottom, 0xC8, 0xC8, eTextAlign::Centre);
        }

        g_Fodder->GUI_Button_Draw_SmallBoxAt("TRY AGAIN", xBackL, xBackR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
        g_Fodder->GUI_Button_Setup_New(Gui_SetupWizard_Click, this, GUIACT_SW_TRY_AGAIN, 0);
    }
}

void cSetupWizard::Draw() {
    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    switch (mScreen) {
    case eScreen::Welcome: DrawWelcome(); break;
    case eScreen::Locate:  DrawLocate();  break;
    case eScreen::Browse:  DrawBrowse();  break;
    case eScreen::Result:  DrawResult();  break;
    }
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
    mGraphics->PaletteSet();
    mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
    mMouseX_Offset = -8;
    mMouseY_Offset = -8;
    Mouse_Setup();
    mMouse_Button_Left_Toggle = 0;
    mMap_Destroy_Tiles.clear();

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

        if (mMouse_Button_Left_Toggle)
            GUI_Handle_Element_Mouse_Check(mGUI_Elements);

        if (mPhase_EscapeKeyAbort || mPhase_Aborted) {
            mPhase_EscapeKeyAbort = false;
            mPhase_Aborted = false;
            wizard.OnBack();
        }

        wizard.Tick();

        // Draw + present.  Reset the cursor sprite each frame so it doesn't
        // get clobbered by stray engine state.
        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        wizard.Draw();
        mMouseSpriteNew = eSprite_pStuff_Mouse_Target;
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
