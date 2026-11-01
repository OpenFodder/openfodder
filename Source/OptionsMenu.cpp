#include "stdafx.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

static int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void ApplyWindowSizingLive();

static std::string trunc_label_px(const std::string& s, int maxPx) {
    if (maxPx <= 0) return "";
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

static void SetupRowHitbox(const size_t x1, const size_t x2, const size_t y, const int rowH) {
    g_Fodder->mGUI_Temp_X = (int16)x1;
    g_Fodder->mGUI_Temp_Y = (int16)y;
    const int32 w = (int32)x2 - (int32)x1;
    g_Fodder->mGUI_Temp_Width = (int16)((w > 0) ? w : 0);
    g_Fodder->mGUI_Draw_LastHeight = (int16)((rowH > 5) ? (rowH - 5) : 0);
}
static inline uint32_t now_ms_u32() {
    using clock = std::chrono::steady_clock;
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        clock::now().time_since_epoch()
    ).count();
    return (uint32_t)ms;
}

enum eGUI_Action : int16 {
    GUIACT_NONE = 0,

    // Chrome (buttons on the right) - removed from rendering, but actions kept
    GUIACT_OPT_UP,
    GUIACT_OPT_DOWN,
    GUIACT_OPT_BACK,

    // Row clicks
    GUIACT_OPT_ROW_PRIMARY,   // click row => toggle/inc
    GUIACT_OPT_LEFT,          // decrement (hover arrow)
    GUIACT_OPT_RIGHT,         // increment (hover arrow)
    GUIACT_OPT_TAB,           // arg = page index
    GUIACT_OPT_SAVE,
    GUIACT_OPT_CANCEL,

    // Action rows (Defaults / Back)
    GUIACT_OPT_ACTION,        // arg = OPT_DEFAULTS / OPT_BACK
    GUIACT_OPT_ADD_DATA,
};

static inline bool IsAdjustAction(int16 action) {
    return action == GUIACT_OPT_LEFT
        || action == GUIACT_OPT_RIGHT
        || action == GUIACT_OPT_ROW_PRIMARY
        || action == GUIACT_OPT_ACTION;
}

// Generic callback thunk for all options-menu GUI elements.
static void Gui_OptionsMenu_Click(void* ctx, int16 action, int16 arg) {
    cOptionsMenu* menu = static_cast<cOptionsMenu*>(ctx);
    if (!menu) return;
    menu->OnRowClick(action, arg);
}

cOptionsMenu::cOptionsMenu() = default;
cOptionsMenu::~cOptionsMenu() = default;

void cOptionsMenu::Open() {
    mDone = false;
    mApplied = false;
    mAddGameData = false;
    mCurrentIndex = 0;
    mCurrentPage = 0;

    // Reset input limiter state
    mLastInputMs = 0;
    mLastInputAction = 0;
    mLastInputArg = 0;

    if (g_Fodder && g_Fodder->mStartParams) {
        mSnapshot = *g_Fodder->mStartParams;
        mSnapshotValid = true;
    }
    else {
        mSnapshotValid = false;
    }

    BuildItems();
    BuildPageItems();
}

void cOptionsMenu::Close() {
    mItems.clear();
    mDrawStrings.clear();
    mDone = true;
}

bool cOptionsMenu::AllowInput(int16 action, int16 arg) {
    // Tune to taste:
    //  - Debounce prevents "double fire" in the same click/frame sequence
    //  - Repeat limits "machine-gun" behaviour if something keeps triggering
    const uint32_t kDebounceMs = 120;
    const uint32_t kRepeatMs = 90;

    if (!IsAdjustAction(action))
        return true;

    const uint32_t t = now_ms_u32();

    // Same exact event spamming? Rate limit.
    if (action == mLastInputAction && arg == mLastInputArg) {
        const uint32_t dt = t - mLastInputMs;
        if (dt < kRepeatMs)
            return false;
    }
    else {
        // Different action/target very soon after last input: debounce.
        const uint32_t dt = t - mLastInputMs;
        if (dt < kDebounceMs)
            return false;
    }

    mLastInputMs = t;
    mLastInputAction = action;
    mLastInputArg = arg;
    return true;
}

void cOptionsMenu::BuildItems() {
    mItems.clear();

    // -------------------------------------------------------------------------
    // Video / Window
    // -------------------------------------------------------------------------
    mItems.push_back({ eOptType::Toggle, eOptSection::Video, "Display mode",     OPT_WINDOW_MODE });
    mItems.push_back({ eOptType::IntRange, eOptSection::Video, "Window scale",    OPT_WINDOW_SCALE, 0, 6, 1 });
    mItems.push_back({ eOptType::Toggle, eOptSection::Video, "Integer scaling", OPT_INTEGER_SCALING });
    mItems.push_back({ eOptType::Toggle, eOptSection::Video, "Bilinear filtering", OPT_BILINEAR_FILTERING });

    // These were in INI but missing from menu
    //mItems.push_back({ eOptType::IntRange, "Window columns",  OPT_WINDOW_COLUMNS, 0, 80, 1 }); // 0=AUTO
    //mItems.push_back({ eOptType::IntRange, "Window rows",     OPT_WINDOW_ROWS,    0, 60, 1 }); // 0=AUTO

    // -------------------------------------------------------------------------
    // Input
    // -------------------------------------------------------------------------
    mItems.push_back({ eOptType::Toggle, eOptSection::Input, "Alternate mouse", OPT_ALTERNATE_MOUSE });
    mItems.push_back({ eOptType::Toggle, eOptSection::Input, "Mouse locked",    OPT_MOUSE_LOCKED });
    mItems.push_back({ eOptType::IntRange, eOptSection::Input, "Mouse speed", OPT_MOUSE_SPEED, 5, 100, 5 });

    // -------------------------------------------------------------------------
    // Audio (no separate page)
    // -------------------------------------------------------------------------

    // Flow options are displayed as positive feature states even when backed by
    // legacy skip flags in the INI.
    mItems.push_back({ eOptType::Toggle, eOptSection::Skip, "Intro sequence",    OPT_SKIP_INTRO });
    mItems.push_back({ eOptType::Toggle, eOptSection::Skip, "Mission briefing", OPT_SKIP_BRIEFING });
    mItems.push_back({ eOptType::Toggle, eOptSection::Skip, "Service screens",  OPT_SKIP_SERVICE });
    mItems.push_back({ eOptType::Toggle, eOptSection::Skip, "Recruitment hill", OPT_SKIP_RECRUIT });

    // -------------------------------------------------------------------------
    // Engine
    // -------------------------------------------------------------------------
    // (platform/engine)
    {
        sOptItem plat{ eOptType::EnumCycle, eOptSection::Engine, "Default platform", OPT_DEFAULT_PLATFORM };
        plat.enumCount = 2; // Amiga / PC
        mItems.push_back(plat);

        sOptItem game{ eOptType::EnumCycle, eOptSection::Engine, "Default game", OPT_DEFAULT_GAME };
        game.enumCount = 1; // CF1 / CF2
        mItems.push_back(game);
    }

    mItems.push_back({ eOptType::Toggle, eOptSection::Engine, "Copy protection", OPT_COPY_PROTECTION });
    mItems.push_back({ eOptType::Toggle, eOptSection::Video, "CF1 CD32 video", OPT_DISABLE_INTRO_VIDEO });

    //mItems.push_back({ eOptType::IntRange, eOptSection::Engine, "Engine speed", OPT_SLEEP_DELTA, 5, 40, 1 });
    //mItems.push_back({ eOptType::IntRange, eOptSection::Engine, "Max sprites",  OPT_MAX_SPRITES, 16, 100000, 1 });
    //mItems.push_back({ eOptType::IntRange, eOptSection::Engine, "Max spawn",    OPT_MAX_SPAWN,   0, 1000, 1 });
    mItems.push_back({ eOptType::Toggle, eOptSection::Engine, "Sound", OPT_DISABLE_SOUND });
    mItems.push_back({ eOptType::Toggle, eOptSection::Engine, "Cheat keys", OPT_CHEATS });


    mCount = (int16_t)mItems.size();
}

void cOptionsMenu::BuildPageItems() {
    mPageItems.clear();

    const eOptSection pageSections[4] = {
        eOptSection::Video,
        eOptSection::Input,
        eOptSection::Skip,
        eOptSection::Engine
    };
    const eOptSection pageSection = pageSections[mCurrentPage];
    for (size_t i = 0; i < mItems.size(); ++i) {
        if (mItems[i].section == pageSection)
            mPageItems.push_back((int)i);
    }

    mCount = (int16_t)mPageItems.size();
    if (mCurrentIndex < 0) mCurrentIndex = 0;
    if (mCurrentIndex > mCount) mCurrentIndex = mCount;
}

void cOptionsMenu::ChangePage(int pageIndex) {
    const int pageCount = 4;
    if (pageIndex < 0) pageIndex = 0;
    if (pageIndex >= pageCount) pageIndex = pageCount - 1;

    if (mCurrentPage == pageIndex)
        return;

    mCurrentPage = (int16_t)pageIndex;
    mCurrentIndex = 0;
    BuildPageItems();
}

void cOptionsMenu::RestoreSnapshot() {
    if (!mSnapshotValid || !g_Fodder || !g_Fodder->mStartParams)
        return;

    *g_Fodder->mStartParams = mSnapshot;
    if (g_Fodder->mParams)
        *g_Fodder->mParams = mSnapshot;

    ApplyWindowSizingLive();
    if (g_Fodder->mWindow) {
        const bool enable = !g_Fodder->mStartParams->mMouseAlternative ||
            (g_Fodder->mStartParams->mMouseAlternative && g_Fodder->mStartParams->mMouseLocked);
        g_Fodder->mWindow->SetRelativeMouseMode(enable);
        g_Fodder->mWindow->SetMouseSpeed(g_Fodder->mStartParams->mMouseSpeed);
    }
    if (g_Fodder->mVersions) {
        g_Fodder->VersionSwitch(g_Fodder->mVersions->GetRetail(g_Fodder->mStartParams->mDefaultPlatform, g_Fodder->mStartParams->mDefaultGame));
        g_Fodder->mVersionDefault = g_Fodder->mVersionCurrent;
    }
}

int cOptionsMenu::GetInt(int optId) const {
    switch (optId) {
    case OPT_WINDOW_MODE:
        if (g_Fodder->mWindow)
            return g_Fodder->mWindow->isFullscreen() ? 0 : 1;
        return g_Fodder->mStartParams->mWindowMode ? 1 : 0;
    case OPT_WINDOW_SCALE:
        if (g_Fodder->mWindow && !g_Fodder->mWindow->isFullscreen())
            return g_Fodder->mWindow->GetScaler();
        return (int)g_Fodder->mStartParams->mWindowScale;
    case OPT_INTEGER_SCALING:  return g_Fodder->mStartParams->mIntegerScaling ? 1 : 0;
    case OPT_BILINEAR_FILTERING: return g_Fodder->mStartParams->mBilinearFilter ? 1 : 0;

    case OPT_WINDOW_COLUMNS:   return (int)g_Fodder->mStartParams->mWindowColumns;
    case OPT_WINDOW_ROWS:      return (int)g_Fodder->mStartParams->mWindowRows;
    case OPT_COPY_PROTECTION:  return g_Fodder->mStartParams->mCopyProtection ? 1 : 0;

    case OPT_ALTERNATE_MOUSE:  return g_Fodder->mStartParams->mMouseAlternative ? 1 : 0;
    case OPT_MOUSE_LOCKED:     return g_Fodder->mStartParams->mMouseLocked ? 1 : 0;
    case OPT_MOUSE_SPEED:      return (int)(g_Fodder->mStartParams->mMouseSpeed * 10.0f + 0.5f);

    case OPT_DISABLE_SOUND:    return g_Fodder->mStartParams->mDisableSound ? 0 : 1;
    case OPT_CHEATS:           return g_Fodder->mStartParams->mCheatsEnabled ? 1 : 0;

    case OPT_SKIP_INTRO:       return g_Fodder->mStartParams->mSkipIntro ? 0 : 1;
    case OPT_DISABLE_INTRO_VIDEO: return g_Fodder->mStartParams->mDisableIntroVideo ? 0 : 1;
    case OPT_SKIP_BRIEFING:    return g_Fodder->mStartParams->mSkipBriefing ? 0 : 1;
    case OPT_SKIP_SERVICE:     return g_Fodder->mStartParams->mSkipService ? 0 : 1;
    case OPT_SKIP_RECRUIT:     return g_Fodder->mStartParams->mSkipRecruit ? 0 : 1;

    case OPT_SLEEP_DELTA:      return (int)g_Fodder->mStartParams->mSleepDelta;
    case OPT_MAX_SPRITES:      return (int)g_Fodder->mStartParams->mSpritesMax;
    case OPT_MAX_SPAWN:        return (int)g_Fodder->mStartParams->mSpawnEnemyMax;

    case OPT_DEFAULT_PLATFORM: return (g_Fodder->mStartParams->mDefaultPlatform == ePlatform::PC) ? 1 : 0;
    case OPT_DEFAULT_GAME:     return (g_Fodder->mStartParams->mDefaultGame == eGame::CF2) ? 1 : 0;

    default: return 0;
    }
}

static inline void ApplyWindowSizingLive() {
    if (!g_Fodder || !g_Fodder->mWindow || !g_Fodder->mStartParams)
        return;

    const size_t scale = g_Fodder->mStartParams->mWindowScale;

    g_Fodder->Window_UpdateScreenSize();

    if (scale == 0) {
        g_Fodder->mWindow->CalculateWindowSize();
        return;
    }

    g_Fodder->mWindow->SetWindowSize((int)scale);
}

void cOptionsMenu::SetInt(int optId, int v) {
    switch (optId) {
    case OPT_WINDOW_MODE: {
        const bool wantWindowed = (v != 0);
        g_Fodder->mStartParams->mWindowMode = wantWindowed;
        if (g_Fodder->mParams)
            g_Fodder->mParams->mWindowMode = wantWindowed;

        if (g_Fodder->mWindow) {
            const bool isFullscreen = g_Fodder->mWindow->isFullscreen();
            if ((wantWindowed && isFullscreen) || (!wantWindowed && !isFullscreen))
                g_Fodder->mWindow->ToggleFullscreen();
        }
        break;
    }
    
    case OPT_WINDOW_SCALE: {
        const int clamped = clamp_int(v, 0, 6);
        g_Fodder->mStartParams->mWindowScale = (uint32)clamped;
        ApplyWindowSizingLive();
        break;
    }

    case OPT_BILINEAR_FILTERING: {
        g_Fodder->mStartParams->mBilinearFilter = (v != 0);
       if (g_Fodder->mParams)
           g_Fodder->mParams->mBilinearFilter = g_Fodder->mStartParams->mBilinearFilter;
        break;
    }

    case OPT_WINDOW_COLUMNS: {
        const int clamped = clamp_int(v, 0, 80);
        g_Fodder->mStartParams->mWindowColumns = (uint32)clamped;
        ApplyWindowSizingLive();
        break;
    }

    case OPT_WINDOW_ROWS: {
        const int clamped = clamp_int(v, 0, 60);
        g_Fodder->mStartParams->mWindowRows = (uint32)clamped;
        ApplyWindowSizingLive();
        break;
    }

    case OPT_INTEGER_SCALING:
        g_Fodder->mStartParams->mIntegerScaling = (v != 0);
        break;

    case OPT_COPY_PROTECTION:
        g_Fodder->mStartParams->mCopyProtection = (v != 0);
        if (g_Fodder->mParams)
            g_Fodder->mParams->mCopyProtection = g_Fodder->mStartParams->mCopyProtection;
        break;

    case OPT_ALTERNATE_MOUSE:
        g_Fodder->mStartParams->mMouseAlternative = (v != 0);
        if (g_Fodder->mParams)
            g_Fodder->mParams->mMouseAlternative = g_Fodder->mStartParams->mMouseAlternative;
        if (g_Fodder->mWindow) {
            const bool enable = !g_Fodder->mStartParams->mMouseAlternative ||
                (g_Fodder->mStartParams->mMouseAlternative && g_Fodder->mStartParams->mMouseLocked);
            g_Fodder->mWindow->SetRelativeMouseMode(enable);
        }
        break;

    case OPT_MOUSE_LOCKED:
        g_Fodder->mStartParams->mMouseLocked = (v != 0);
        if (g_Fodder->mParams)
            g_Fodder->mParams->mMouseLocked = g_Fodder->mStartParams->mMouseLocked;
        if (g_Fodder->mWindow) {
            const bool enable = !g_Fodder->mStartParams->mMouseAlternative ||
                (g_Fodder->mStartParams->mMouseAlternative && g_Fodder->mStartParams->mMouseLocked);
            g_Fodder->mWindow->SetRelativeMouseMode(enable);
        }
        break;

    case OPT_MOUSE_SPEED: {
        const int clamped = clamp_int(v, 5, 100);
        const float speed = (float)clamped / 10.0f;
        g_Fodder->mStartParams->mMouseSpeed = speed;
        if (g_Fodder->mWindow)
            g_Fodder->mWindow->SetMouseSpeed(speed);
        break;
    }

    case OPT_DISABLE_SOUND:
        g_Fodder->mStartParams->mDisableSound = (v == 0);
        break;

    case OPT_CHEATS:
        g_Fodder->mStartParams->mCheatsEnabled = (v != 0);
        break;

    case OPT_SKIP_INTRO:
        g_Fodder->mStartParams->mSkipIntro = (v == 0);
        break;

    case OPT_DISABLE_INTRO_VIDEO:
        g_Fodder->mStartParams->mDisableIntroVideo = (v == 0);
        break;

    case OPT_SKIP_BRIEFING:
        g_Fodder->mStartParams->mSkipBriefing = (v == 0);
        break;

    case OPT_SKIP_SERVICE:
        g_Fodder->mStartParams->mSkipService = (v == 0);
        break;

    case OPT_SKIP_RECRUIT:
        g_Fodder->mStartParams->mSkipRecruit = (v == 0);
        break;

    case OPT_SLEEP_DELTA:
        g_Fodder->mStartParams->mSleepDelta = (uint32)clamp_int(v, 5, 40);
        break;

    case OPT_MAX_SPRITES:
        g_Fodder->mStartParams->mSpritesMax = (uint32)clamp_int(v, 16, 100000);
        break;

    case OPT_MAX_SPAWN:
        g_Fodder->mStartParams->mSpawnEnemyMax = (uint32)clamp_int(v, 0, 1000);
        break;

    case OPT_DEFAULT_PLATFORM: {
        const int sel = clamp_int(v, 0, 1);
        g_Fodder->mStartParams->mDefaultPlatform = (sel == 1) ? ePlatform::PC : ePlatform::Amiga;
        if (g_Fodder->mVersions) {
            g_Fodder->VersionSwitch(g_Fodder->mVersions->GetRetail(g_Fodder->mStartParams->mDefaultPlatform, g_Fodder->mStartParams->mDefaultGame));
            g_Fodder->mVersionDefault = g_Fodder->mVersionCurrent;
        }
        break;
    }

    case OPT_DEFAULT_GAME: {
        const int sel = clamp_int(v, 0, 1);
        g_Fodder->mStartParams->mDefaultGame = (sel == 1) ? eGame::CF2 : eGame::CF1;
        if (g_Fodder->mVersions) {
            g_Fodder->VersionSwitch(g_Fodder->mVersions->GetRetail(g_Fodder->mStartParams->mDefaultPlatform, g_Fodder->mStartParams->mDefaultGame));
            g_Fodder->mVersionDefault = g_Fodder->mVersionCurrent;
        }
        break;
    }

    default: break;
    }

    mApplied = true;
}

std::string cOptionsMenu::FormatValue(const sOptItem& it) const {
    const int v = GetInt(it.id);

    if (it.id == OPT_WINDOW_MODE)
        return v ? "WINDOWED" : "FULLSCREEN";

    if (it.type == eOptType::Toggle)
        return v ? "ON" : "OFF";

    if (it.id == OPT_WINDOW_SCALE) {
        if (v == 0) return "AUTO";
        return std::to_string(v) + "X";
    }

    if (it.id == OPT_WINDOW_COLUMNS) {
        if (v == 0) return "AUTO";
        return std::to_string(v);
    }

    if (it.id == OPT_WINDOW_ROWS) {
        if (v == 0) return "AUTO";
        return std::to_string(v);
    }

    if (it.id == OPT_DEFAULT_PLATFORM) {
        return (v == 1) ? "PC" : "AMIGA";
    }

    if (it.id == OPT_DEFAULT_GAME) {
        return (v == 1) ? "CF2" : "CF1";
    }

    if (it.id == OPT_MOUSE_SPEED) {
        return std::to_string(v / 10) + "." + std::to_string(v % 10) + "X";
    }

    return std::to_string(v);
}

void cOptionsMenu::Draw() {
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    // Match the main menu: outlined lettering over the running map, with
    // outlines appearing around controls only when they are under the pointer.
    auto* pixels = g_Fodder->mSurface->GetSurfaceBuffer();
    const size_t size = g_Fodder->mSurface->GetSurfaceBufferSize();
    const size_t stride = g_Fodder->mSurface->GetWidth();
    const std::vector<uint8> background(pixels, pixels + size);
    constexpr uint8 transparent = 0xFF;
    std::fill(pixels, pixels + size, transparent);

    const int extraHeight = PLATFORM_BASED(0, 25);
    const int rowBaseY = 71 + extraHeight / 2;
    const int rowH = RowH();
    const char* pageLabels[] = { "VIDEO", "INPUT", "FLOW", "SYSTEM" };
    const char* pageDescriptions[] = {
        "DISPLAY AND WINDOW SETTINGS", "MOUSE AND POINTER SETTINGS",
        "CHOOSE WHICH SCREENS TO SHOW", "GAME PREFERENCES AND SOUND"
    };

    auto button = [&](const std::string& text, int x1, int x2, int y,
                      int16 action, int16 arg, bool selected = false) {
        const bool hover = g_Fodder->mMouseX + 0x20 >= x1 &&
            g_Fodder->mMouseX + 0x20 <= x2 &&
            g_Fodder->mMouseY >= y && g_Fodder->mMouseY <= y + 15;
        g_Fodder->String_Print_Small_CentreInBox(text, x1, x2, y + 5);
        if (hover)
            g_Fodder->Briefing_DrawBox(x1, y, x2 - x1, 16, 0xF3);
        if (selected)
            g_Fodder->Briefing_Draw_Horizontal_Line(x1 + 0x14, x2 + 0x0C, y + 0x20, 0xF3);
        SetupRowHitbox(x1, x2, y, 16);
        g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, action, arg);
    };

    g_Fodder->mString_GapCharID = 0;
    g_Fodder->String_Print_Large("OPTIONS", false, 19);
    for (int16 i = 0; i < 4; ++i)
        button(pageLabels[i], 16 + i * 74, 82 + i * 74, 39,
               GUIACT_OPT_TAB, i, i == mCurrentPage);

    const std::string description = pageDescriptions[mCurrentPage];
    const int descriptionWidth = g_Fodder->Sidebar_Menu_MeasureWidth(description);
    g_Fodder->Sidebar_Menu_Print(description, (320 - descriptionWidth) / 2, 61,
                                cFodder::eSidebarMenuColor::Inactive);

    for (int16 vis = 0; vis < mShownItems; ++vis) {
        const int pageIndex = mCurrentIndex + vis;
        if (pageIndex >= mCount) break;
        const int idx = mPageItems[pageIndex];
        const sOptItem& item = mItems[idx];
        const int y = rowBaseY + vis * rowH;
        const bool hasArrows = item.type == eOptType::IntRange || item.type == eOptType::EnumCycle;
        const std::string label = trunc_label_px(item.label, 180);
        g_Fodder->String_Print_Small_Left(label, 22, y + 5);

        if (hasArrows) {
            // Use explicit direction glyphs rather than letter-like controls.
            button("<", 208, 226, y, GUIACT_OPT_LEFT, (int16)idx);
            g_Fodder->String_Print_Small_CentreInBox(FormatValue(item), 228, 280, y + 5);
            button(">", 282, 300, y, GUIACT_OPT_RIGHT, (int16)idx);
        }
        else {
            button(FormatValue(item), 208, 300, y, GUIACT_OPT_ROW_PRIMARY, (int16)idx);
        }

        // Register after the arrow controls so their narrower targets win.
        SetupRowHitbox(18, 302, y, rowH);
        g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_ROW_PRIMARY, (int16)idx);
    }

    const int footerY = 175 + extraHeight;
#ifndef __EMSCRIPTEN__
    button("ADD GAME DATA", 92, 228, footerY - 18, GUIACT_OPT_ADD_DATA, 0);
#endif
    button("DEFAULTS", 14, 100, footerY, GUIACT_OPT_ACTION, OPT_DEFAULTS);
    button("SAVE", 114, 206, footerY, GUIACT_OPT_SAVE, 0);
    button("CANCEL", 220, 306, footerY, GUIACT_OPT_CANCEL, 0);

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
        if (foreground[index] != transparent)
            pixels[index] = foreground[index];
}

void cOptionsMenu::Tick() {}

void cOptionsMenu::OnUp() {
    if (mCurrentIndex > 0) --mCurrentIndex;
}

void cOptionsMenu::OnDown() {
    if (mCurrentIndex + mShownItems < mCount) ++mCurrentIndex;
}

void cOptionsMenu::OnBack() { mDone = true; }

int cOptionsMenu::MouseToItemIndex() const {
    const int my = (int)g_Fodder->mMouseY;
    const int rowBaseY = 0x20 + (int)PLATFORM_BASED(0, 25);

    const int rel = my - rowBaseY;
    if (rel < 0) return -1;

    const int row = rel / RowH();
    if (row < 0 || row >= mShownItems) return -1;

    const int idx = (int)mCurrentIndex + row;
    if (idx < 0 || idx >= mCount) return -1;

    if ((size_t)idx >= mPageItems.size())
        return -1;

    return mPageItems[(size_t)idx];
}

void cOptionsMenu::Adjust(sOptItem& it, int dir) {
    if (it.type == eOptType::Toggle) {
        SetInt(it.id, GetInt(it.id) ? 0 : 1);
        return;
    }

    if (it.type == eOptType::IntRange) {
        int v = GetInt(it.id);
        v += dir * it.step;
        v = clamp_int(v, it.minv, it.maxv);
        SetInt(it.id, v);
        return;
    }

    if (it.type == eOptType::EnumCycle) {
        int v = GetInt(it.id);
        v += dir;
        if (v < 0) v = it.enumCount - 1;
        if (v >= it.enumCount) v = 0;
        SetInt(it.id, v);
        return;
    }
}

void cOptionsMenu::ResetDefaults() {
    g_Fodder->mStartParams->clear();
    mApplied = true;

    // Reset bilinear filtering to false (default value) for current parameters same as toggling the menu option but forcing false since bilinear filtering is OFF by default
    if (g_Fodder->mParams)
        g_Fodder->mParams->mBilinearFilter = false;
}

void cOptionsMenu::OnRowClick(int16 action, int16 arg) {
    // IMPORTANT: cooldown / debounce (prevents 60Hz flicker changes)
    if (!AllowInput(action, arg))
        return;

    if (action == GUIACT_OPT_UP) { OnUp(); return; }
    if (action == GUIACT_OPT_DOWN) { OnDown(); return; }
    if (action == GUIACT_OPT_BACK) { OnBack(); return; }
    if (action == GUIACT_OPT_TAB) { ChangePage(arg); return; }
    if (action == GUIACT_OPT_SAVE) { g_Fodder->mStartParams->SaveIni(); mApplied = true; mDone = true; return; }
    if (action == GUIACT_OPT_CANCEL) { RestoreSnapshot(); mApplied = false; mDone = true; return; }
    if (action == GUIACT_OPT_ADD_DATA) {
        // The outer loop owns the modal transition and retains these options.
        mAddGameData = true;
        return;
    }

    if (action == GUIACT_OPT_ACTION) {
        const int actionId = (int)arg;
        if (actionId == OPT_DEFAULTS) { ResetDefaults(); return; }
        if (actionId == OPT_BACK) { mDone = true; return; }
        return;
    }

    const int idx = (int)arg;
    if (idx < 0 || idx >= (int)mItems.size()) return;

    sOptItem& it = mItems[(size_t)idx];
    if (it.type == eOptType::Spacer) return;

    if (action == GUIACT_OPT_LEFT) { Adjust(it, -1); return; }
    if (action == GUIACT_OPT_RIGHT) { Adjust(it, +1); return; }

    if (it.type == eOptType::Toggle) {
        Adjust(it, +1);
    }
}

std::unique_ptr<cOptionsMenu> mOptionsMenu;

bool cFodder::Options_Menu_Run() {
    mOptionsMenu = std::make_unique<cOptionsMenu>();
    mOptionsMenu->Open();

    mMap_Destroy_Tiles.clear();
    mMouse_Button_Left_Toggle = 0;

    const auto prevInterruptCallback = mInterruptCallback;
    mGraphics->PaletteSet();
    mSurface->palette_FadeTowardNew();

    const auto optionsInterruptCallback = [this]() {
        if (!mStartParams->mDisableVideo)
            mGraphics->MapTiles_Draw();
        Sprites_Draw();
        mOptionsMenu->Draw();
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        Mouse_DrawCursor();
        };
    mInterruptCallback = optionsInterruptCallback;

    // Replace the home menu's hit targets before the first input is handled.
    mInterruptCallback();
    while (!mOptionsMenu->IsDone() && !mExit) {

        if (mPhase_InterruptTicks >= 3) {
            mPhase_InterruptTicks = 0;
            Mission_Sprites_Handle();
            Sound_Tick();
        }

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        // Mouse wheel scroll (one "tick" per frame)
        if (mMouse_EventLastWheel.mY > 0) {
            mOptionsMenu->OnUp();
            mMouse_EventLastWheel.mY = 0;
        }
        else if (mMouse_EventLastWheel.mY < 0) {
            mOptionsMenu->OnDown();
            mMouse_EventLastWheel.mY = 0;
        }

        // Consume a press once, just as on the main menu.
        if (Mouse_Button_Left_Toggled() >= 0)
            GUI_Handle_Element_Mouse_Check(mGUI_Elements);

        if (mPhase_EscapeKeyAbort || mPhase_Aborted) {
            mPhase_EscapeKeyAbort = false;
            mPhase_Aborted = false;
            mOptionsMenu->OnBack();
        }

        mOptionsMenu->Tick();

#ifndef __EMSCRIPTEN__
        if (mOptionsMenu->ConsumeGameDataRequest() && !mExit) {
            const sFodderParameters draft = *mStartParams;
            const sFodderParameters runtimeDraft = *mParams;
            const sGameVersion* optionsVersion = mVersionCurrent;

            // Adding a resource saves [paths]. Keep unsaved option changes
            // out of that write, then return to the same options page/draft.
            *mStartParams = mOptionsMenu->SettingsSnapshot();
            mInterruptCallback = []() {};
            GUI_Element_Reset();
            mMouseButtonStatus = 0;
            mMouse_Button_Left_Toggle = 0;
            mMouse_EventLastButtonsPressed = 0;
            mPhase_In_Progress = false;
            Setup_Wizard_Run(true);

            *mStartParams = draft;
            mPhase_In_Progress = false;
            if (!mExit) {
                VersionSwitch(optionsVersion);
                *mParams = runtimeDraft;
                MainMenu_Setup();
                mInterruptCallback = optionsInterruptCallback;
                mInterruptCallback();
            }
            mMouse_Button_Left_Toggle = 0;
            mMouse_EventLastButtonsPressed = 0;
            mMouseButtonStatus = 0;
            mKeyCode = 0;
            mKeyCodeAscii = 0;
            mInput_LastKey = 0;
        }
#endif

        Video_Sleep();
    }

    const bool applied = mOptionsMenu->WasApplied();

    mInterruptCallback = prevInterruptCallback;
    mOptionsMenu->Close();
    mOptionsMenu.reset();

    //Image_FadeOut();
    if (applied)
        mStartParams->SaveIni();
    mMouse_Button_Left_Toggle = 0;
    mMouse_EventLastButtonsPressed = 0;
    mKeyCode = 0;
    mKeyCodeAscii = 0;
    mInput_LastKey = 0;
    return applied;
}
