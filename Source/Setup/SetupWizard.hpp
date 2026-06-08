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

#pragma once

#include "Setup/AutoScan.hpp"
#include "Setup/FileBrowser.hpp"
#include "Setup/Validation.hpp"

#include <memory>
#include <string>
#include <vector>

#ifdef OPENFODDER_ENABLE_NETWORK
#include "Network/HubAuth.hpp"
#endif

namespace Setup {

// First-run / "data missing" wizard. Drawn over whatever VersionSwitch chose
// for the demo background — by the time we reach Setup_Wizard_Run, the engine
// has loaded a demo version's font + sprite sheet, so all the GUI primitives
// (String_Print_Small, GUI_Button_Draw_SmallBoxAt, Briefing_DrawBox) are
// safe to call.
class cSetupWizard {
public:
    enum class eScreen : uint8_t {
        Welcome,
        Locate,         // Auto-scan results + "Browse..."
        Browse,         // FileBrowser is up
        Result,         // Validation outcome for the picked folder
        Pairing,        // 6-digit pair code entry (post-Apply, only if matchmaking checkbox was on)
    };

    cSetupWizard();
    ~cSetupWizard();

    void Open(bool pHaveAnyVersion);
    void Close();

    void Draw();
    void Tick();

    // GUI dispatcher target.
    void OnRowClick(int16 pAction, int16 pArg);

    // Engine-level escape: called from cFodder::Setup_Wizard_Run on Esc.
    void OnBack();

    // The wizard loop calls these to forward mouse-wheel events to whichever
    // screen is on top.
    void OnScrollUp();
    void OnScrollDown();

    bool IsDone() const { return mDone; }

    // True iff the wizard mutated the resource path list. Setup_Wizard_Run
    // calls g_ResourceMan->refresh() when this is set.
    bool ResourcesChanged() const { return mResourcesChanged; }

    // Drag-and-drop sink: the engine loop drains cFodder::mDroppedPaths
    // every frame and calls this. Treats a folder the same as a manual
    // browse-pick; treats an image file the same (Phase 3 will wire the
    // mount path; Phase 2 just validates as if it were a regular folder).
    void OnPathDropped(const std::string& pPath);

private:
    void RunAutoScan();
    void EnterBrowse();
    void EnterResult(const sValidationResult& pResult);
    void CommitPick(const std::string& pFolder);

    // Copy every matched version's manifest from the currently-mounted
    // image into <cwd>/Data/<release>/. On success, unmounts the image
    // (we don't need it any more — files are on disk) and triggers a
    // resource refresh so findVersions picks up the new on-disk install.
    // Returns the number of files copied; 0 if there was nothing to do.
    int  CommitCopyFromImage();

    // Copy a raw-extracted folder's matched files into <cwd>/Data/<release>/.
    // Used when ValidateFolder reported a match where mIsRawFolder=true —
    // i.e. the user pointed at a folder containing CFTITLE.RAW etc. directly,
    // not a parent containing a Data/Amiga/ subfolder. Returns files copied
    // or -1 on I/O failure.
    int  CommitCopyFromRawFolder();

    // Treat a path as either a folder (ValidateFolder) or a disk image
    // (firy mountImage). Returns true if the path was recognised; false
    // means the wizard should leave its current screen unchanged.
    bool DispatchPick(const std::string& pPath);

    // Handle a .cue/.bin pair: extract track 1 to a temp .iso, mount that,
    // remember any track-2 (CD32 intro video) for the COPY phase. Returns
    // true on success.
    bool DispatchCueSheet(const std::string& pCuePath);

    // Standalone .mpg / .mp2 drop: copy to <cwd>/Data/Intro.mpg.
    bool DispatchStandaloneVideo(const std::string& pVideoPath);

    // Drop the temp .iso we extracted for a cue/bin pickup. Best-effort —
    // failures are logged but don't block the wizard.
    void DiscardPendingCueTemp();

    // Convert the most recently mounted image's matches into the same
    // sValidationResult shape ValidateFolder produces, so the Result screen
    // can render both kinds uniformly.
    sValidationResult ResultFromMountedImage(int pId,
                                              const std::string& pImagePath) const;

    static bool LooksLikeImagePath(const std::string& pPath);

    // Layout helpers.
    void DrawWelcome();
    void DrawLocate();
    void DrawBrowse();
    void DrawResult();
    void DrawPairing();

    // ----- Public-matchmaking pairing flow (post-Apply) -----
    //
    // The Result page exposes an "Enable public matchmaking" checkbox; on Apply
    // (SAVE & CONTINUE / COPY TO DATA / KEEP MOUNTED) we kick off the Discord
    // pairing handshake here instead of in OnRowClick to keep the click handler
    // small. Begin* fires the browser; Submit* validates the typed code and
    // calls cHubAuth::ClaimToken. Either step can soft-fail without blocking
    // the rest of the wizard from completing — the user just won't have a
    // public-matchmaking token.

    // True iff the matchmaking checkbox is on. Toggled by clicking the
    // checkbox row on the Result screen. Persists across screens within a
    // single wizard session.
    bool mEnableMatchmaking = false;

    // Pending Apply action — captured when the user clicks the primary
    // button on Result while the checkbox is on, replayed after the
    // pairing flow completes (success OR cancel). 0 means "no pending
    // apply".
    int16 mPendingApplyAction = 0;
    int16 mPendingApplyArg = 0;

#ifdef OPENFODDER_ENABLE_NETWORK
    void StartPairingFlow(int16 pNextAction, int16 pNextArg);
    void SubmitPairCode();
    void CompletePendingApply();    // dispatch mPendingApplyAction back through OnRowClick

    cHubAuth     mHubAuth;
    std::string  mDeviceCode;       // BeginPairing output; empty until pairing started
    std::string  mPairCode;         // user-typed 6-char code
    std::string  mPairingError;     // most recent error from BeginPairing/ClaimToken
    bool         mPairingDone = false;     // true after a successful ClaimToken
#endif

private:
    bool mDone = false;
    bool mHaveAnyVersion = false;       // True iff demo or retail was loaded before the wizard ran
    bool mResourcesChanged = false;
    eScreen mScreen = eScreen::Welcome;

    std::vector<sScanCandidate> mCandidates;

    sValidationResult mLastResult;       // Most recent ValidateFolder() outcome
    int mLastMountedId = 0;              // 0 = last pick was a folder, >0 = id in MountedImage registry

    // CD32 cue/bin context — populated when DispatchPick handles a .cue.
    // mLastMountedId still references the temp .iso we extracted for track-1;
    // these fields carry track-2 info for the COPY TO DATA path.
    std::string mPendingCuePath;
    std::string mPendingTempIsoPath;     // Deleted on success/cancel
    int         mPendingCueTrack2 = 0;   // 0 if no track-2 present in the sheet

    std::unique_ptr<cFileBrowser> mBrowser;

    // Debounce shared between top-level wizard buttons.
    uint32_t mLastInputMs = 0;
    int16    mLastInputAction = 0;
    int16    mLastInputArg = 0;
};

} // namespace Setup
