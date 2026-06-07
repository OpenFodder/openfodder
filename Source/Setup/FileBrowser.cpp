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
#include "Setup/FileBrowser.hpp"

#include <algorithm>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Setup {

// Wizard browser uses its own GUI action ids so it can share the eGUI_Element
// dispatcher with the rest of the engine without a clash.
enum eGuiAction : int16 {
    GUIACT_FB_NONE = 0,
    GUIACT_FB_ROW,        // arg = entry index in mEntries
    GUIACT_FB_USE_HERE,   // accept current directory
    GUIACT_FB_PARENT,
    GUIACT_FB_CANCEL,
    GUIACT_FB_HOME,       // jump to user home
};

static uint32_t now_ms_u32() {
    using clock = std::chrono::steady_clock;
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        clock::now().time_since_epoch()
    ).count();
    return (uint32_t)ms;
}

static void Gui_FileBrowser_Click(void* ctx, int16 action, int16 arg) {
    cFileBrowser* fb = static_cast<cFileBrowser*>(ctx);
    if (!fb)
        return;
    fb->OnRowClick(action, arg);
}

static std::string NormalisePath(std::string p) {
    std::replace(p.begin(), p.end(), '\\', '/');
    // Strip a single trailing slash unless the path is just "/".
    while (p.size() > 1 && (p.back() == '/' || p.back() == '\\'))
        p.pop_back();
    return p;
}

static std::string TruncToWidth(const std::string& s, int maxPx) {
    if (maxPx <= 0)
        return "";
    std::string upper = s;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    const int ellipsisW = g_Fodder->String_MeasureWidth(mFont_Briefing_Width, "...");
    if (ellipsisW >= maxPx)
        return "...";

    const int maxContent = maxPx - ellipsisW;
    int w = 0;
    size_t cut = 0;
    for (; cut < upper.size(); ++cut) {
        const unsigned char c = (unsigned char)upper[cut];
        const int cw = (int)mFont_Briefing_Width[c];
        if (w + cw > maxContent)
            break;
        w += cw;
    }
    if (cut >= upper.size())
        return upper;
    return upper.substr(0, cut) + "...";
}

cFileBrowser::cFileBrowser() = default;
cFileBrowser::~cFileBrowser() = default;

void cFileBrowser::Open(const std::string& pInitialPath, eFilter pFilter,
                        std::function<void(const std::string&)> pOnPicked,
                        std::function<void()> pOnCancel) {
    mDone = false;
    mFilter = pFilter;
    mOnPicked = std::move(pOnPicked);
    mOnCancel = std::move(pOnCancel);

    mLastInputMs = 0;
    mLastInputAction = 0;
    mLastInputArg = 0;

    NavigateAbsolute(pInitialPath.empty() ? g_ResourceMan->getcwd() : pInitialPath);
}

void cFileBrowser::NavigateAbsolute(const std::string& pPath) {
    std::string normalised = NormalisePath(pPath);
    if (normalised.empty())
        normalised = NormalisePath(g_ResourceMan->getcwd());

    // If the user hands us a file rather than a folder, treat it as "navigate
    // to the parent directory and pre-select this file."
    if (g_ResourceMan->FileExists(normalised) && !g_ResourceMan->DirExists(normalised)) {
        std::string parent = ParentOf(normalised);
        if (!parent.empty()) {
            mCurrentPath = parent;
            RescanCurrent();
            // Try to highlight the file in the newly-loaded list.
            std::string leaf = normalised.substr(parent.size() + 1);
            std::string leafLower = leaf;
            std::transform(leafLower.begin(), leafLower.end(), leafLower.begin(), ::tolower);
            for (size_t i = 0; i < mEntries.size(); ++i) {
                std::string n = mEntries[i].mName;
                std::transform(n.begin(), n.end(), n.begin(), ::tolower);
                if (n == leafLower) {
                    mSelectedItem = (int16)i;
                    if (mCurrentIndex + mShownItems <= (int16)i)
                        mCurrentIndex = (int16)i - mShownItems + 1;
                    if (mCurrentIndex < 0)
                        mCurrentIndex = 0;
                    break;
                }
            }
            return;
        }
    }

    mCurrentPath = normalised;
    RescanCurrent();
}

void cFileBrowser::RescanCurrent() {
    mEntries.clear();
    mCurrentIndex = 0;
    mSelectedItem = -1;

    if (!g_ResourceMan)
        return;

    // Synthetic ".." parent link unless we're already at a root.
    std::string parent = ParentOf(mCurrentPath);
    if (!parent.empty() && parent != mCurrentPath) {
        sEntry e;
        e.mName = "..";
        e.mIsDir = true;
        e.mIsParentLink = true;
        mEntries.push_back(std::move(e));
    }

    // Directories first.
    auto dirs = g_ResourceMan->DirectoryListDirs(mCurrentPath);
    for (auto& d : dirs) {
        sEntry e;
        e.mName = d;
        e.mIsDir = true;
        mEntries.push_back(std::move(e));
    }

    if (mFilter == eFilter::DirsAndImages) {
        // Image / cue / video extensions — leaf-name match, listed after dirs.
        static const char* kExts[] = {
            ".adf", ".iso", ".ipf", ".raw", ".img", ".ima",
            ".cue",                       // CD32 multi-track sheet
            ".mpg", ".mp2", ".mpeg"        // Standalone CD32 intro video
        };
        for (auto* ext : kExts) {
            auto files = g_ResourceMan->DirectoryList(mCurrentPath, ext);
            for (auto& f : files) {
                if (!IsRecognisedImage(f))
                    continue;
                sEntry e;
                e.mName = f;
                e.mIsDir = false;
                mEntries.push_back(std::move(e));
            }
        }
    }

#ifdef _WIN32
    // On Windows, when we're "at root" (e.g. C:/), additionally inject all
    // available drive letters at the parent-link slot so the user can flick
    // between drives without typing.
    if (parent.empty() || parent == mCurrentPath) {
        DWORD mask = GetLogicalDrives();
        for (char l = 'A'; l <= 'Z'; ++l) {
            if (mask & (1u << (l - 'A'))) {
                std::string driveName;
                driveName.push_back(l);
                driveName.push_back(':');
                // Skip the drive we're already on.
                if (mCurrentPath.size() >= 2 &&
                    (char)std::toupper((unsigned char)mCurrentPath[0]) == l && mCurrentPath[1] == ':')
                    continue;
                sEntry e;
                e.mName = driveName + " (drive)";
                e.mIsDir = true;
                // Stash the drive root in the leaf name; Tick() handles "(drive)" suffix.
                mEntries.push_back(std::move(e));
            }
        }
    }
#endif
}

void cFileBrowser::NavigateInto(const std::string& pLeafName) {
    // Special handling for "<X>: (drive)" leaves we synthesised at root.
    auto driveSuffix = std::string(" (drive)");
    if (pLeafName.size() > driveSuffix.size() &&
        pLeafName.compare(pLeafName.size() - driveSuffix.size(), driveSuffix.size(), driveSuffix) == 0) {
        std::string drive = pLeafName.substr(0, pLeafName.size() - driveSuffix.size());
        NavigateAbsolute(drive + "/");
        return;
    }

    std::string next = mCurrentPath;
    if (!next.empty() && next.back() != '/')
        next.push_back('/');
    next += pLeafName;
    NavigateAbsolute(next);
}

void cFileBrowser::NavigateToParent() {
    std::string parent = ParentOf(mCurrentPath);
    if (parent.empty())
        return;
    NavigateAbsolute(parent);
}

std::string cFileBrowser::ParentOf(const std::string& pPath) const {
    if (pPath.empty())
        return {};

    // Drive root on Windows: "C:" or "C:/" → no parent.
    if (pPath.size() <= 3 && pPath.size() >= 2 && pPath[1] == ':')
        return {};

    // POSIX root.
    if (pPath == "/")
        return {};

    size_t lastSlash = pPath.find_last_of('/');
    if (lastSlash == std::string::npos)
        return {};

    std::string parent = pPath.substr(0, lastSlash);
    if (parent.empty())
        return "/";          // POSIX: the parent of /foo is "/"
    if (parent.size() == 2 && parent[1] == ':')
        return parent + "/"; // Windows: parent of "C:/Foo" is "C:/"
    return parent;
}

bool cFileBrowser::IsRecognisedImage(const std::string& pName) const {
    auto endsWith = [&](const char* ext) {
        size_t L = std::strlen(ext);
        if (pName.size() < L) return false;
        for (size_t i = 0; i < L; ++i) {
            char a = (char)std::tolower((unsigned char)pName[pName.size() - L + i]);
            char b = (char)std::tolower((unsigned char)ext[i]);
            if (a != b) return false;
        }
        return true;
    };
    return endsWith(".adf") || endsWith(".iso") || endsWith(".ipf") ||
           endsWith(".raw") || endsWith(".img") || endsWith(".ima") ||
           endsWith(".cue") ||      // CD32 multi-track cue/bin pair
           endsWith(".mpg") || endsWith(".mp2") || endsWith(".mpeg");
}

void cFileBrowser::OnScrollUp() {
    if (mCurrentIndex > 0)
        --mCurrentIndex;
}

void cFileBrowser::OnScrollDown() {
    int16 maxTop = (int16)mEntries.size() - mShownItems;
    if (maxTop < 0) maxTop = 0;
    if (mCurrentIndex < maxTop)
        ++mCurrentIndex;
}

void cFileBrowser::OnBack() {
    if (!mCurrentPath.empty()) {
        std::string parent = ParentOf(mCurrentPath);
        if (!parent.empty()) {
            NavigateToParent();
            return;
        }
    }
    if (mOnCancel)
        mOnCancel();
    mDone = true;
}

void cFileBrowser::ForcePick(const std::string& pFullPath) {
    std::string p = NormalisePath(pFullPath);
    if (mOnPicked)
        mOnPicked(p);
    mDone = true;
}

void cFileBrowser::OnRowClick(int16 action, int16 arg) {
    // Debounce — copy-paste of cOptionsMenu::AllowInput's shape.
    const uint32_t kDebounceMs = 120;
    const uint32_t kRepeatMs = 90;
    const uint32_t t = now_ms_u32();
    if (action == mLastInputAction && arg == mLastInputArg) {
        if (t - mLastInputMs < kRepeatMs)
            return;
    } else {
        if (t - mLastInputMs < kDebounceMs)
            return;
    }
    mLastInputMs = t;
    mLastInputAction = action;
    mLastInputArg = arg;

    switch (action) {
    case GUIACT_FB_USE_HERE:
        if (mOnPicked)
            mOnPicked(mCurrentPath);
        mDone = true;
        return;

    case GUIACT_FB_PARENT:
        NavigateToParent();
        return;

    case GUIACT_FB_CANCEL:
        if (mOnCancel)
            mOnCancel();
        mDone = true;
        return;

    case GUIACT_FB_HOME: {
#ifdef _WIN32
        char buf[1024]; size_t len = 0;
        if (getenv_s(&len, buf, sizeof(buf), "USERPROFILE") == 0 && len) {
            NavigateAbsolute(std::string(buf, len - 1));
        }
#else
        const char* h = std::getenv("HOME");
        if (h && *h)
            NavigateAbsolute(h);
#endif
        return;
    }

    case GUIACT_FB_ROW: {
        if (arg < 0 || arg >= (int16)mEntries.size())
            return;
        const sEntry& e = mEntries[(size_t)arg];
        if (e.mIsParentLink) {
            NavigateToParent();
            return;
        }
        if (e.mIsDir) {
            NavigateInto(e.mName);
            return;
        }
        // Image file (DirsAndImages mode).
        std::string full = mCurrentPath;
        if (!full.empty() && full.back() != '/')
            full.push_back('/');
        full += e.mName;
        if (mOnPicked)
            mOnPicked(full);
        mDone = true;
        return;
    }
    }
}

void cFileBrowser::Tick() {
    // No per-frame work yet — kept as a hook for future polling needs.
}

void cFileBrowser::Draw() {
    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("LOCATE DATA", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    // Path strip across the top.
    {
        const size_t pathY = 0x18;
        std::string shown = TruncToWidth(mCurrentPath, 320 - 16);
        g_Fodder->String_Print_Small(shown, 8, pathY);
    }

    // List rows.
    const int rowH = 0x10;
    const int rowBaseY = 0x28;
    const size_t xRowL = 0x10;
    const size_t xRowR = 0x130;

    int16 visibleCount = 0;
    for (int16 v = 0; v < mShownItems; ++v) {
        int16 idx = (int16)mCurrentIndex + v;
        if (idx >= (int16)mEntries.size())
            break;
        ++visibleCount;
    }

    if (visibleCount > 0) {
        const int16 boxX = (int16)(xRowL - 2);
        const int16 boxY = (int16)(rowBaseY - 4);
        const int16 boxW = (int16)(xRowR - xRowL + 4);
        const int16 boxH = (int16)((visibleCount * rowH) + 8);
        g_Fodder->Briefing_DrawBox(boxX, boxY, boxW, boxH, 0xF3);
        g_Fodder->Briefing_DrawBox(boxX - 1, boxY - 1, boxW, boxH, 0xF2);
    }

    for (int16 v = 0; v < visibleCount; ++v) {
        const int16 idx = (int16)mCurrentIndex + v;
        const sEntry& e = mEntries[(size_t)idx];

        const size_t y = (size_t)(rowBaseY + (v * rowH));

        std::string label;
        if (e.mIsParentLink)
            label = "[..]";
        else if (e.mIsDir)
            label = "[" + e.mName + "]";
        else
            label = e.mName;

        const std::string fitted = TruncToWidth(label, (int)(xRowR - xRowL - 8));
        g_Fodder->String_Print_Small_LeftInBox(fitted, xRowL + 4, xRowR - 4, y);

        // Hitbox on the row.
        g_Fodder->mGUI_Temp_X = (int16)xRowL;
        g_Fodder->mGUI_Temp_Y = (int16)y;
        g_Fodder->mGUI_Temp_Width = (int16)(xRowR - xRowL);
        g_Fodder->mGUI_Draw_LastHeight = (int16)(rowH - 2);
        g_Fodder->GUI_Button_Setup_New(Gui_FileBrowser_Click, this, GUIACT_FB_ROW, idx);
    }

    // Bottom action row.
    const size_t yBottom = 0xB0;
    const size_t buttonW = 0x40;
    const size_t gap = 0x06;
    const size_t totalW = (buttonW * 4) + (gap * 3);
    const size_t xStart = 160 - (totalW / 2);

    const size_t xUseL = xStart;                      const size_t xUseR = xUseL + buttonW;
    const size_t xParentL = xUseR + gap;              const size_t xParentR = xParentL + buttonW;
    const size_t xHomeL = xParentR + gap;             const size_t xHomeR = xHomeL + buttonW;
    const size_t xCancelL = xHomeR + gap;             const size_t xCancelR = xCancelL + buttonW;

    g_Fodder->GUI_Button_Draw_SmallBoxAt("USE HERE", xUseL, xUseR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_FileBrowser_Click, this, GUIACT_FB_USE_HERE, 0);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("UP", xParentL, xParentR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_FileBrowser_Click, this, GUIACT_FB_PARENT, 0);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("HOME", xHomeL, xHomeR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_FileBrowser_Click, this, GUIACT_FB_HOME, 0);

    g_Fodder->GUI_Button_Draw_SmallBoxAt("CANCEL", xCancelL, xCancelR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
    g_Fodder->GUI_Button_Setup_New(Gui_FileBrowser_Click, this, GUIACT_FB_CANCEL, 0);
}

} // namespace Setup
