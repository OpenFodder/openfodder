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

#include <functional>
#include <string>
#include <vector>

namespace Setup {

// Full-screen browser drawn over the setup menu's background scene.
// Disk sets appear once, with the same grouping used by setup discovery.
class cFileBrowser {
public:
    enum class eFilter : uint8_t {
        DirectoriesOnly,    // Only directories are listed (Phase 2 wizard usage).
        DirsAndImages,      // Directories + .adf / .iso / .ipf / .raw files (Phase 3).
    };

    struct sEntry {
        std::string mName;          // Just the leaf name; UI display
        std::string mDisplayName;   // Friendly grouped-disk title, if available.
        std::string mDetail;
        bool        mIsDir = true;
        bool        mIsParentLink = false; // ".." pseudo-entry
    };

    // pOnPicked is invoked when the user activates a directory (or, with
    // DirsAndImages, an image file). Argument is the FULL path with
    // forward-slash separators and no trailing slash.
    cFileBrowser();
    ~cFileBrowser();

    void Open(const std::string& pInitialPath, eFilter pFilter,
              std::function<void(const std::string&)> pOnPicked,
              std::function<void()> pOnCancel);

    // The wizard's interrupt callback calls Draw() each frame.
    void Draw();

    // The wizard's main loop calls Tick() each frame.
    void Tick();

    bool IsDone() const { return mDone; }
    size_t EntryCount() const { return mEntries.size(); }

    // Public for the static GUI trampoline.
    void OnRowClick(int16 pAction, int16 pArg);

    // Mouse-wheel scroll (one tick per call).
    void OnScrollUp();
    void OnScrollDown();

    // Esc / right-click: pop one level, or cancel if at root.
    void OnBack();

    // The Drop handler can synthesise a "user picked this path" without a
    // mouse interaction.
    void ForcePick(const std::string& pFullPath);

private:
    void RescanCurrent();
    void NavigateInto(const std::string& pLeafName);
    void NavigateToParent();
    void NavigateAbsolute(const std::string& pPath);

    std::string ParentOf(const std::string& pPath) const;

private:
    bool mDone = false;
    eFilter mFilter = eFilter::DirectoriesOnly;

    std::function<void(const std::string&)> mOnPicked;
    std::function<void()> mOnCancel;

    std::string             mCurrentPath;       // Always normalised, forward slashes, NO trailing slash.
    std::vector<sEntry>     mEntries;

    int16   mCurrentIndex = 0;          // Top of scroll window.
    int16   mShownItems = 4;
    int16   mSelectedItem = -1;         // Highlighted row, relative to mCurrentIndex.

    // Debounce, like cOptionsMenu::AllowInput.
    uint32_t mLastInputMs = 0;
    int16    mLastInputAction = 0;
    int16    mLastInputArg = 0;
};

} // namespace Setup
