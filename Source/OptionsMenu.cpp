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
    mItems.push_back({ eOptType::Toggle, eOptSection::Video, "Window mode",     OPT_WINDOW_MODE });
    mItems.push_back({ eOptType::IntRange, eOptSection::Video, "Window scale",    OPT_WINDOW_SCALE, 0, 6, 1 });
    mItems.push_back({ eOptType::Toggle, eOptSection::Video, "Integer scaling", OPT_INTEGER_SCALING });

    // These were in INI but missing from menu
    //mItems.push_back({ eOptType::IntRange, "Window columns",  OPT_WINDOW_COLUMNS, 0, 80, 1 }); // 0=AUTO
    //mItems.push_back({ eOptType::IntRange, "Window rows",     OPT_WINDOW_ROWS,    0, 60, 1 }); // 0=AUTO

    // -------------------------------------------------------------------------
    // Input
    // -------------------------------------------------------------------------
    mItems.push_back({ eOptType::Toggle, eOptSection::Input, "Alternate mouse", OPT_ALTERNATE_MOUSE });
    mItems.push_back({ eOptType::Toggle, eOptSection::Input, "Mouse locked",    OPT_MOUSE_LOCKED });
    mItems.push_back({ eOptType::IntRange, eOptSection::Input, "Mouse speed x10", OPT_MOUSE_SPEED, 5, 100, 5 });

    // -------------------------------------------------------------------------
    // Audio (no separate page)
    // -------------------------------------------------------------------------

    // Skip flags from INI
    mItems.push_back({ eOptType::Toggle, eOptSection::Skip, "Skip intro",    OPT_SKIP_INTRO });
    mItems.push_back({ eOptType::Toggle, eOptSection::Skip, "Skip briefing", OPT_SKIP_BRIEFING });
    mItems.push_back({ eOptType::Toggle, eOptSection::Skip, "Skip service",  OPT_SKIP_SERVICE });
    mItems.push_back({ eOptType::Toggle, eOptSection::Skip, "Skip hill",     OPT_SKIP_RECRUIT });

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
    mItems.push_back({ eOptType::Toggle, eOptSection::Video, "Disable CF1 CD32 Video", OPT_DISABLE_INTRO_VIDEO });

    //mItems.push_back({ eOptType::IntRange, eOptSection::Engine, "Engine speed", OPT_SLEEP_DELTA, 5, 40, 1 });
    //mItems.push_back({ eOptType::IntRange, eOptSection::Engine, "Max sprites",  OPT_MAX_SPRITES, 16, 100000, 1 });
    //mItems.push_back({ eOptType::IntRange, eOptSection::Engine, "Max spawn",    OPT_MAX_SPAWN,   0, 1000, 1 });
    mItems.push_back({ eOptType::Toggle, eOptSection::Engine, "Disable sound", OPT_DISABLE_SOUND });
    mItems.push_back({ eOptType::Toggle, eOptSection::Engine, "Enable cheats", OPT_CHEATS });


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

    case OPT_WINDOW_COLUMNS:   return (int)g_Fodder->mStartParams->mWindowColumns;
    case OPT_WINDOW_ROWS:      return (int)g_Fodder->mStartParams->mWindowRows;
    case OPT_COPY_PROTECTION:  return g_Fodder->mStartParams->mCopyProtection ? 1 : 0;

    case OPT_ALTERNATE_MOUSE:  return g_Fodder->mStartParams->mMouseAlternative ? 1 : 0;
    case OPT_MOUSE_LOCKED:     return g_Fodder->mStartParams->mMouseLocked ? 1 : 0;
    case OPT_MOUSE_SPEED:      return (int)(g_Fodder->mStartParams->mMouseSpeed * 10.0f + 0.5f);

    case OPT_DISABLE_SOUND:    return g_Fodder->mStartParams->mDisableSound ? 1 : 0;
    case OPT_CHEATS:           return g_Fodder->mStartParams->mCheatsEnabled ? 1 : 0;

    case OPT_SKIP_INTRO:       return g_Fodder->mStartParams->mSkipIntro ? 1 : 0;
    case OPT_DISABLE_INTRO_VIDEO: return g_Fodder->mStartParams->mDisableIntroVideo ? 1 : 0;
    case OPT_SKIP_BRIEFING:    return g_Fodder->mStartParams->mSkipBriefing ? 1 : 0;
    case OPT_SKIP_SERVICE:     return g_Fodder->mStartParams->mSkipService ? 1 : 0;
    case OPT_SKIP_RECRUIT:     return g_Fodder->mStartParams->mSkipRecruit ? 1 : 0;

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
        g_Fodder->mStartParams->mDisableSound = (v != 0);
        break;

    case OPT_CHEATS:
        g_Fodder->mStartParams->mCheatsEnabled = (v != 0);
        break;

    case OPT_SKIP_INTRO:
        g_Fodder->mStartParams->mSkipIntro = (v != 0);
        break;

    case OPT_DISABLE_INTRO_VIDEO:
        g_Fodder->mStartParams->mDisableIntroVideo = (v != 0);
        break;

    case OPT_SKIP_BRIEFING:
        g_Fodder->mStartParams->mSkipBriefing = (v != 0);
        break;

    case OPT_SKIP_SERVICE:
        g_Fodder->mStartParams->mSkipService = (v != 0);
        break;

    case OPT_SKIP_RECRUIT:
        g_Fodder->mStartParams->mSkipRecruit = (v != 0);
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

    if (it.type == eOptType::Toggle)
        return v ? "ON" : "OFF";

    if (it.id == OPT_WINDOW_SCALE) {
        if (v == 0) return "AUTO";
        return std::to_string(v);
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
        return std::to_string(v);
    }

    return std::to_string(v);
}

void cOptionsMenu::Draw() {
    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("OPTIONS", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    const int tabY = PLATFORM_BASED(0x1A, 0x18);
    const int rowBaseY = tabY + 0x18;
    const int rowH = RowH();
    const int16 pageCount = 4;
    const char* pageLabels[pageCount] = { "VID", "IN", "SKIP", "ENG" };

    // Layout
    const size_t xRowL = 0x20;
    const size_t xRowR = 0x120;
    const size_t gap = 2;

    const size_t arrowW = 0x10;
    const size_t valueW = 0x30;

    const size_t xArrowRightR = xRowR - 2;
    const size_t xArrowRightL = xArrowRightR - arrowW;
    const size_t xValueR = xArrowRightL - gap;
    const size_t xValueL = xValueR - valueW;
    const size_t xArrowLeftR = xValueL - gap;
    const size_t xArrowLeftL = xArrowLeftR - arrowW;
    const size_t xLabelL = xRowL + 2;
    const size_t xLabelR = (xArrowLeftL > gap) ? (xArrowLeftL - gap) : xLabelL;

    int maxLabelPx = (int)xLabelR - (int)xLabelL - 6;
    if (maxLabelPx < 0)
        maxLabelPx = 0;

    // Top page tabs
    {
        const size_t tabGap = 6;
        const size_t totalW = xRowR - xRowL + 1;
        const size_t tabW = (totalW - (tabGap * (pageCount - 1))) / pageCount;
        for (int16 i = 0; i < pageCount; ++i) {
            const size_t x1 = xRowL + (i * (tabW + tabGap));
            const size_t x2 = x1 + tabW - 1;
            const bool selected = (i == mCurrentPage);
            const size_t shade = selected ? 0xF3 : 0xB2;
            const size_t prim = selected ? 0xF2 : 0xB3;
            g_Fodder->GUI_Button_Draw_SmallBoxAt(pageLabels[i], x1, x2, tabY, prim, shade, eTextAlign::Centre);
            g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_TAB, i);
        }
    }

    // Visible rows for current page
    struct sVisibleRow {
        int idx;
        size_t y;
    };

    std::vector<sVisibleRow> visible;
    visible.reserve(mShownItems);

    for (int16_t vis = 0; vis < mShownItems; ++vis) {
        const int pageIdx = (int)mCurrentIndex + vis;
        if (pageIdx < 0 || pageIdx >= mCount) break;
        const size_t y = (size_t)(rowBaseY + (vis * rowH));
        visible.push_back({ mPageItems[(size_t)pageIdx], y });
    }

    // Single group box for the page
    if (!visible.empty()) {
        const int groupPadTop = 4;
        const int groupPadBottom = 5;
        const int16 boxX = (int16)(xRowL - 2);
        const int16 boxY = (int16)(visible.front().y - groupPadTop);
        const int16 boxW = (int16)(xRowR - xRowL + 4);
        const int16 boxH = (int16)((visible.back().y + rowH + groupPadBottom) - visible.front().y);
        g_Fodder->Briefing_DrawBox(boxX, boxY, boxW, boxH, 0xF3);
        g_Fodder->Briefing_DrawBox(boxX - 1, boxY - 1, boxW, boxH, 0xF2);
    }

    // Row separators inside the page box
    for (size_t i = 0; i + 1 < visible.size(); ++i) {
        const int16 yLine = (int16)(visible[i].y + rowH - 5);
        g_Fodder->Briefing_Draw_Horizontal_Line((int16)xRowL + 0x10, (int16)xRowR + 0x10, (int16)(yLine + 0x10), 0xF2);
    }

    for (size_t i = 0; i < visible.size(); ++i) {
        const int idx = visible[i].idx;
        const size_t y = visible[i].y;
        const sOptItem& it = mItems[(size_t)idx];

        const std::string label = trunc_label_px(it.label, maxLabelPx);
        g_Fodder->String_Print_Small_LeftInBox(label, xLabelL, xLabelR, y);

        g_Fodder->GUI_Button_Draw_SmallBoxAt(FormatValue(it), xValueL, xValueR, y, 0xB2, 0xB3, eTextAlign::Centre);

        const bool hasArrows = (it.type == eOptType::IntRange || it.type == eOptType::EnumCycle);
        if (hasArrows) {
            g_Fodder->GUI_Button_Draw_SmallBoxAt("N", xArrowLeftL, xArrowLeftR, y, 0xB2, 0xB3, eTextAlign::Centre);
            g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_LEFT, (int16)idx);

            g_Fodder->GUI_Button_Draw_SmallBoxAt("P", xArrowRightL, xArrowRightR, y, 0xB2, 0xB3, eTextAlign::Centre);
            g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_RIGHT, (int16)idx);
        }

        SetupRowHitbox(xRowL, xRowR, y, rowH);
        g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_ROW_PRIMARY, (int16)idx);
    }

    // Bottom actions (fixed to bottom of the screen)
    {
        const size_t screenH = (size_t)g_Fodder->mWindow->GetScreenSize().getHeight();
        const size_t yBottom = (screenH > 0x20) ? (screenH - 0x1C) : (rowBaseY + (mShownItems * rowH) + 6);
        const size_t buttonW = 0x40;
        const size_t gapW = 0x08;
        const size_t totalW = (buttonW * 3) + (gapW * 2);
        const size_t xStart = ((xRowL + xRowR) / 2) - (totalW / 2);
        const size_t xDefaultsL = xStart;
        const size_t xDefaultsR = xDefaultsL + buttonW;
        const size_t xSaveL = xDefaultsR + gapW;
        const size_t xSaveR = xSaveL + buttonW;
        const size_t xCancelL = xSaveR + gapW;
        const size_t xCancelR = xCancelL + buttonW;

        g_Fodder->GUI_Button_Draw_SmallBoxAt("DEFAULTS", xDefaultsL, xDefaultsR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
        g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_ACTION, OPT_DEFAULTS);

        g_Fodder->GUI_Button_Draw_SmallBoxAt("SAVE", xSaveL, xSaveR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
        g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_SAVE, 0);

        g_Fodder->GUI_Button_Draw_SmallBoxAt("CANCEL", xCancelL, xCancelR, yBottom, 0xB2, 0xB3, eTextAlign::Centre);
        g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_CANCEL, 0);
    }
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
    g_Fodder->mStartParams->SaveIni();
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

    mInterruptCallback = [this]() {
        Sprites_Draw();
        mOptionsMenu->Draw();
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        Mouse_DrawCursor();
        };

    while (!mOptionsMenu->IsDone()) {

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

        // Mouse click processing
        if (mMouse_Button_Left_Toggle)
            GUI_Handle_Element_Mouse_Check(mGUI_Elements);

        if (mPhase_EscapeKeyAbort || mPhase_Aborted) {
            mPhase_EscapeKeyAbort = false;
            mPhase_Aborted = false;
            mOptionsMenu->OnBack();
        }

        mOptionsMenu->Tick();

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
