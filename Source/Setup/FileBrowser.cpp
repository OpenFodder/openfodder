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
#include "Setup/DiskImageList.hpp"
#include "FileSystem.hpp"
#include <filesystem>

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
    GUIACT_FB_LIST_UP, GUIACT_FB_LIST_DOWN,
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
    // A bare Windows drive is drive-relative; retain the root slash so moving
    // up to C:/ cannot quietly send the browser back to that drive's cwd.
    if (p.size() == 2 && p[1] == ':') p.push_back('/');
    while (p.size() > 1 && !(p.size() == 3 && p[1] == ':') && p.back() == '/')
        p.pop_back();
    return p;
}

static std::string TruncToWidth(const std::string& s, int maxPx) {
    if (maxPx <= 0)
        return "";
    std::string upper = s;
    std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) { return char(std::toupper(c)); });

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
            std::string leaf = std::filesystem::u8path(normalised).filename().u8string();
            for (size_t i = 0; i < mEntries.size(); ++i) {
                if (FileSystem::Equal(mEntries[i].mName, leaf)) {
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
        for (const auto& image : FindDiskImages(mCurrentPath)) {
            sEntry entry;
            entry.mName = std::filesystem::u8path(image.mPath).filename().u8string();
            entry.mDisplayName = image.mName;
            entry.mDetail = std::to_string(image.mDisks) + " " + image.mFormat +
                (image.mDisks == 1 ? " DISK" : " DISKS");
            // Keep release/crack tags visible when several editions have the
            // same friendly title, so the player can tell the sets apart.
            const auto tag = entry.mName.find_first_of("([");
            if (tag != std::string::npos) entry.mDetail += " - " + entry.mName.substr(tag);
            entry.mIsDir = false;
            mEntries.push_back(std::move(entry));
        }
        for (const char* ext : {".mpg", ".mp2", ".mpeg"}) {
            for (const auto& file : g_ResourceMan->DirectoryList(mCurrentPath, ext)) {
                sEntry entry;
                entry.mName = file;
                entry.mDetail = "INTRO VIDEO";
                entry.mIsDir = false;
                mEntries.push_back(std::move(entry));
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
    case GUIACT_FB_LIST_UP:
        mCurrentIndex = std::max<int16>(0, mCurrentIndex - mShownItems);
        return;
    case GUIACT_FB_LIST_DOWN:
        mCurrentIndex = std::min<int16>(std::max<int16>(0, (int16)mEntries.size() - mShownItems),
                                       mCurrentIndex + mShownItems);
        return;
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
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    auto* pixels = g_Fodder->mSurface->GetSurfaceBuffer();
    const size_t size = g_Fodder->mSurface->GetSurfaceBufferSize();
    const size_t stride = g_Fodder->mSurface->GetWidth();
    const std::vector<uint8> background(pixels, pixels + size);
    constexpr uint8 transparent = 0xFF;
    std::fill(pixels, pixels + size, transparent);
    const int extraHeight = PLATFORM_BASED(0, 25);

    const auto hint = [&](const std::string& text, int x, int y) {
        std::string fitted = text;
        std::transform(fitted.begin(), fitted.end(), fitted.begin(), [](unsigned char c) { return char(std::toupper(c)); });
        if (g_Fodder->String_MeasureWidth(mFont_Briefing_Width, fitted) > 300 - x) {
            while (!fitted.empty() && g_Fodder->String_MeasureWidth(mFont_Briefing_Width, fitted + "...") > 300 - x)
                fitted.pop_back();
            fitted += "...";
        }
        g_Fodder->String_Print_Small(fitted, x, y);
    };
    const auto button = [&](const std::string& label, int left, int right, int y, int action) {
        g_Fodder->String_Print_Small_CentreInBox(label, left, right, y + 5);
        const bool hover = g_Fodder->mMouseX + 0x20 >= left &&
            g_Fodder->mMouseX + 0x20 <= right &&
            g_Fodder->mMouseY >= y && g_Fodder->mMouseY <= y + 15;
        if (hover) g_Fodder->Briefing_DrawBox(left, y, right - left, 16, 0xF3);
        g_Fodder->mGUI_Temp_X = (int16)left;
        g_Fodder->mGUI_Temp_Y = (int16)y;
        g_Fodder->mGUI_Temp_Width = (int16)(right - left);
        g_Fodder->mGUI_Draw_LastHeight = 10;
        g_Fodder->GUI_Button_Setup_New(Gui_FileBrowser_Click, this, action, 0);
    };

    g_Fodder->mString_GapCharID = 0;
    g_Fodder->String_Print_Large("CHOOSE GAME DATA", false, 19);
    std::string shown = mCurrentPath;
    std::transform(shown.begin(), shown.end(), shown.begin(), [](unsigned char c) { return char(std::toupper(c)); });
    while (!shown.empty() && g_Fodder->String_MeasureWidth(mFont_Briefing_Width, ".." + shown) > 284)
        shown.erase(shown.begin());
    if (shown.size() < mCurrentPath.size()) shown = ".." + shown;
    hint(shown, 18, 43);

    const int rowH = 23;
    const int rowBaseY = 58 + extraHeight / 3;
    for (int16 v = 0; v < mShownItems; ++v) {
        const int16 idx = mCurrentIndex + v;
        if (idx >= (int16)mEntries.size()) break;
        const sEntry& entry = mEntries[(size_t)idx];
        const int y = rowBaseY + v * rowH;
        const std::string label = entry.mIsParentLink ? "< PARENT FOLDER" :
            entry.mDisplayName.empty() ? entry.mName : entry.mDisplayName;
        const bool hover = g_Fodder->mMouseX + 0x20 >= 16 &&
            g_Fodder->mMouseX + 0x20 <= 304 &&
            g_Fodder->mMouseY >= y && g_Fodder->mMouseY <= y + rowH - 2;
        g_Fodder->String_Print_Small_Left(TruncToWidth(label, entry.mIsDir ? 254 : 275), 22, y + 4);
        if (entry.mIsDir && !entry.mIsParentLink)
            g_Fodder->String_Print_Small_Right(">", 296, y + 4);
        if (!entry.mIsDir && !entry.mDetail.empty()) {
            // Edition tags distinguish otherwise identical sets when hovered;
            // the usual list only needs the disk count and image format.
            const auto tag = entry.mDetail.find(" - ");
            hint(hover ? entry.mDetail : entry.mDetail.substr(0, tag), 23, y + 15);
        }
        if (hover) g_Fodder->Briefing_DrawBox(16, y - 1, 288, rowH - 1, 0xF3);
        g_Fodder->mGUI_Temp_X = 16;
        g_Fodder->mGUI_Temp_Y = (int16)y;
        g_Fodder->mGUI_Temp_Width = 288;
        g_Fodder->mGUI_Draw_LastHeight = rowH - 5;
        g_Fodder->GUI_Button_Setup_New(Gui_FileBrowser_Click, this, GUIACT_FB_ROW, idx);
    }

    if (mEntries.empty() || (mEntries.size() == 1 && mEntries.front().mIsParentLink)) {
        g_Fodder->String_Print_Small_CentreInBox("NO DISKS OR FOLDERS HERE", 16, 304, 86);
        hint("CHOOSE ANOTHER FOLDER", 66, 106);
    }
    const int pagerY = 153 + extraHeight / 2;
    if (mCurrentIndex > 0) button("PREVIOUS", 16, 100, pagerY, GUIACT_FB_LIST_UP);
    if (mCurrentIndex + mShownItems < int(mEntries.size()))
        button("NEXT", 220, 304, pagerY, GUIACT_FB_LIST_DOWN);
    const int footerY = 175 + extraHeight;
    button("BACK", 14, 145, footerY, GUIACT_FB_CANCEL);
    button("USE THIS FOLDER", 165, 306, footerY, GUIACT_FB_USE_HERE);

    const std::vector<uint8> foreground(pixels, pixels + size);
    std::copy(background.begin(), background.end(), pixels);
    for (size_t y = 1; y + 1 < g_Fodder->mSurface->GetHeight(); ++y) {
        for (size_t x = 1; x + 1 < stride; ++x) {
            const size_t index = y * stride + x;
            if (foreground[index] == transparent) continue;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    pixels[(y + dy) * stride + x + dx] = 0;
        }
    }
    for (size_t index = 0; index < size; ++index)
        if (foreground[index] != transparent) pixels[index] = foreground[index];
}

} // namespace Setup
