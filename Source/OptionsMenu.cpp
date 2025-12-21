#include "stdafx.hpp"

#include <algorithm>
#include <string>

static int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static std::string trunc_label(const std::string& s, size_t maxChars) {
    if (s.size() <= maxChars) return s;
    if (maxChars <= 3) return s.substr(0, maxChars);
    return s.substr(0, maxChars - 3) + "...";
}

static std::string pad_centre(const std::string& s, size_t widthChars) {
    if (s.size() >= widthChars) return s.substr(0, widthChars);
    const size_t totalPad = widthChars - s.size();
    const size_t leftPad = totalPad / 2;
    const size_t rightPad = totalPad - leftPad;
    return std::string(leftPad, ' ') + s + std::string(rightPad, ' ');
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

    // Action rows (Defaults / Apply / Back)
    GUIACT_OPT_ACTION,        // arg = OPT_DEFAULTS / OPT_APPLY / OPT_BACK
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

    // Reset input limiter state
    mLastInputMs = 0;
    mLastInputAction = 0;
    mLastInputArg = 0;

    BuildItems();
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
    mItems.push_back({ eOptType::Spacer, "VIDEO", 0 });
    mItems.push_back({ eOptType::Toggle,   "Window mode",     OPT_WINDOW_MODE });
    mItems.push_back({ eOptType::IntRange, "Window scale",    OPT_WINDOW_SCALE, 0, 6, 1 });
    mItems.push_back({ eOptType::Toggle,   "Integer scaling", OPT_INTEGER_SCALING });

    // These were in INI but missing from menu
    mItems.push_back({ eOptType::IntRange, "Window columns",  OPT_WINDOW_COLUMNS, 0, 80, 1 }); // 0=AUTO
    mItems.push_back({ eOptType::IntRange, "Window rows",     OPT_WINDOW_ROWS,    0, 60, 1 }); // 0=AUTO
    mItems.push_back({ eOptType::Toggle,   "Copy protection", OPT_COPY_PROTECTION });

    // -------------------------------------------------------------------------
    // Input
    // -------------------------------------------------------------------------
    mItems.push_back({ eOptType::Spacer, "INPUT", 0 });
    mItems.push_back({ eOptType::Toggle, "Alternate mouse", OPT_ALTERNATE_MOUSE });
    mItems.push_back({ eOptType::Toggle, "Mouse locked",    OPT_MOUSE_LOCKED });

    // -------------------------------------------------------------------------
    // Audio / gameplay
    // -------------------------------------------------------------------------
    mItems.push_back({ eOptType::Spacer, "AUDIO / GAMEPLAY", 0 });
    mItems.push_back({ eOptType::Toggle, "Disable sound", OPT_DISABLE_SOUND });
    mItems.push_back({ eOptType::Toggle, "Enable cheats", OPT_CHEATS });

    // Skip flags from INI (these were missing from the menu)
    mItems.push_back({ eOptType::Spacer, "SKIP", 0 });
    mItems.push_back({ eOptType::Toggle, "Skip intro",    OPT_SKIP_INTRO });
    mItems.push_back({ eOptType::Toggle, "Skip briefing", OPT_SKIP_BRIEFING });
    mItems.push_back({ eOptType::Toggle, "Skip service",  OPT_SKIP_SERVICE });
    mItems.push_back({ eOptType::Toggle, "Skip hill",     OPT_SKIP_RECRUIT });

    // -------------------------------------------------------------------------
    // Engine
    // -------------------------------------------------------------------------
    mItems.push_back({ eOptType::Spacer, "ENGINE", 0 });
    mItems.push_back({ eOptType::IntRange, "Engine speed", OPT_SLEEP_DELTA, 5, 40, 1 });
    mItems.push_back({ eOptType::IntRange, "Max sprites",  OPT_MAX_SPRITES, 16, 100000, 1 });
    mItems.push_back({ eOptType::IntRange, "Max spawn",    OPT_MAX_SPAWN,   0, 1000, 1 });

    // Engine defaults from INI (platform/engine)
    {
        sOptItem plat{ eOptType::EnumCycle, "Default platform", OPT_DEFAULT_PLATFORM };
        plat.enumCount = 2; // Amiga / PC
        mItems.push_back(plat);

        sOptItem game{ eOptType::EnumCycle, "Default game", OPT_DEFAULT_GAME };
        game.enumCount = 2; // CF1 / CF2
        mItems.push_back(game);
    }

    // -------------------------------------------------------------------------
    // Actions
    // -------------------------------------------------------------------------
    mItems.push_back({ eOptType::Spacer, " ", 0 });
    mItems.push_back({ eOptType::Action, "Defaults", OPT_DEFAULTS });
    mItems.push_back({ eOptType::Action, "Apply",    OPT_APPLY });
    mItems.push_back({ eOptType::Action, "Back",     OPT_BACK });

    mCount = (int16_t)mItems.size();

    if (mCurrentIndex < 0) mCurrentIndex = 0;
    if (mCurrentIndex > mCount) mCurrentIndex = mCount;
}

int cOptionsMenu::GetInt(int optId) const {
    switch (optId) {
    case OPT_WINDOW_MODE:      return g_Fodder->mStartParams->mWindowMode ? 1 : 0;
    case OPT_WINDOW_SCALE:     return (int)g_Fodder->mStartParams->mWindowScale;
    case OPT_INTEGER_SCALING:  return g_Fodder->mStartParams->mIntegerScaling ? 1 : 0;

    case OPT_WINDOW_COLUMNS:   return (int)g_Fodder->mStartParams->mWindowColumns;
    case OPT_WINDOW_ROWS:      return (int)g_Fodder->mStartParams->mWindowRows;
    case OPT_COPY_PROTECTION:  return g_Fodder->mStartParams->mCopyProtection ? 1 : 0;

    case OPT_ALTERNATE_MOUSE:  return g_Fodder->mStartParams->mMouseAlternative ? 1 : 0;
    case OPT_MOUSE_LOCKED:     return g_Fodder->mStartParams->mMouseLocked ? 1 : 0;

    case OPT_DISABLE_SOUND:    return g_Fodder->mStartParams->mDisableSound ? 1 : 0;
    case OPT_CHEATS:           return g_Fodder->mStartParams->mCheatsEnabled ? 1 : 0;

    case OPT_SKIP_INTRO:       return g_Fodder->mStartParams->mSkipIntro ? 1 : 0;
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
    const size_t cols = g_Fodder->mStartParams->mWindowColumns;
    const size_t rows = g_Fodder->mStartParams->mWindowRows;

    // If any "AUTO" dimension is requested, fall back to CalculateWindowSize()
    if (scale == 0 || cols == 0 || rows == 0) {
        g_Fodder->mWindow->CalculateWindowSize();
        return;
    }

    // Scale wins when non-auto; columns/rows are still stored but not used here.
    g_Fodder->mWindow->SetWindowSize((int)scale);
}

void cOptionsMenu::SetInt(int optId, int v) {
    switch (optId) {
    case OPT_WINDOW_MODE: {
        const bool wantWindowed = (v != 0);
        g_Fodder->mStartParams->mWindowMode = wantWindowed;

        if (g_Fodder->mWindow) {
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
        break;

    case OPT_ALTERNATE_MOUSE:
        g_Fodder->mStartParams->mMouseAlternative = (v != 0);
        break;

    case OPT_MOUSE_LOCKED:
        g_Fodder->mStartParams->mMouseLocked = (v != 0);
        break;

    case OPT_DISABLE_SOUND:
        g_Fodder->mStartParams->mDisableSound = (v != 0);
        break;

    case OPT_CHEATS:
        g_Fodder->mStartParams->mCheatsEnabled = (v != 0);
        break;

    case OPT_SKIP_INTRO:
        g_Fodder->mStartParams->mSkipIntro = (v != 0);
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
        break;
    }

    case OPT_DEFAULT_GAME: {
        const int sel = clamp_int(v, 0, 1);
        g_Fodder->mStartParams->mDefaultGame = (sel == 1) ? eGame::CF2 : eGame::CF1;
        break;
    }

    default: break;
    }
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

    return std::to_string(v);
}

void cOptionsMenu::Draw() {
    const size_t YOffset = PLATFORM_BASED(0, 25);

    g_Fodder->mSurface->clearBuffer();
    g_Fodder->mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
    g_Fodder->GUI_Element_Reset();

    g_Fodder->mString_GapCharID = 0x25;
    g_Fodder->String_Print_Large("OPTIONS", false, 0x01);
    g_Fodder->mString_GapCharID = 0;

    const int rowBaseY = 0x20 + (int)YOffset;
    const int rowH = RowH();

    // Layout (no right-side chrome)
    const size_t xL = 0x18;
    const size_t xR = 0xD8;

    const size_t xLabelL = 0x22;

    // Value chip moved right + widened (prevents label collisions)
    const size_t xValueL = 0xA4;
    const size_t xValueR = 0xD2;

    // Arrows drawn inside the chip when hovered
    const size_t xArrowL = xValueL + 6;
    const size_t xArrowR = xValueR - 18;

    const int hoverIdx = MouseToItemIndex();

    // Keep labels from reaching into the chip.
    const size_t maxLabelChars = 20;

    for (int16_t vis = 0; vis < mShownItems; ++vis) {
        const int idx = (int)mCurrentIndex + vis;
        if (idx < 0 || idx >= mCount) break;

        const sOptItem& it = mItems[(size_t)idx];
        const size_t y = (size_t)(rowBaseY + (vis * rowH));
        const bool hovered = (idx == hoverIdx);

        // Headings
        if (it.type == eOptType::Spacer) {
            g_Fodder->GUI_Button_Draw_SmallBoxAt(it.label, xL, xR, y, 0xB2, 0xB3, eTextAlign::Centre);
            continue;
        }

        // Action rows
        if (it.type == eOptType::Action) {
            // Consistent boxed row for actions
            g_Fodder->GUI_Button_Draw_SmallBoxAt(" ", xL, xR, y, 0xB2, 0xB3, eTextAlign::Left);
            g_Fodder->String_Print_Small_InBox(it.label, xL, xR, y);

            g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_ACTION, (int16)it.id);
            continue;
        }

        // Normal option row: always framed, hover shows arrows only
        g_Fodder->GUI_Button_Draw_SmallBoxAt(" ", xL, xR, y, 0xB2, 0xB3, eTextAlign::Left);

        const std::string label = trunc_label(it.label, maxLabelChars);
        g_Fodder->String_Print_Small_Left(label, xLabelL, y);

        std::string chip = pad_centre(FormatValue(it), 8);

        // Draw chip with text (reliable render path)
        g_Fodder->GUI_Button_Draw_SmallBoxAt(chip, xValueL, xValueR, y, 0xB2, 0xB3, eTextAlign::Centre);

        // Whole row => primary action
        g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_ROW_PRIMARY, (int16)idx);

        // Hover arrows (inside chip, no right-side clutter)
        if (hovered) {
            g_Fodder->GUI_Button_Draw_SmallAt("M", xArrowL, y, 0xB2, 0xB3);
            g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_LEFT, (int16)idx);

            g_Fodder->GUI_Button_Draw_SmallAt("P", xArrowR, y, 0xB2, 0xB3);
            g_Fodder->GUI_Button_Setup_New(Gui_OptionsMenu_Click, this, GUIACT_OPT_RIGHT, (int16)idx);
        }
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

    return idx;
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
    g_Fodder->mStartParams->Process(0, {});
}

void cOptionsMenu::Apply() {
    g_Fodder->mStartParams->SaveIni();
    mApplied = true;
    mDone = true;
}

void cOptionsMenu::OnRowClick(int16 action, int16 arg) {
    // IMPORTANT: cooldown / debounce (prevents 60Hz flicker changes)
    if (!AllowInput(action, arg))
        return;

    if (action == GUIACT_OPT_UP) { OnUp(); return; }
    if (action == GUIACT_OPT_DOWN) { OnDown(); return; }
    if (action == GUIACT_OPT_BACK) { OnBack(); return; }

    if (action == GUIACT_OPT_ACTION) {
        const int actionId = (int)arg;
        if (actionId == OPT_DEFAULTS) { ResetDefaults(); return; }
        if (actionId == OPT_APPLY) { Apply(); return; }
        if (actionId == OPT_BACK) { mDone = true; return; }
        return;
    }

    const int idx = (int)arg;
    if (idx < 0 || idx >= mCount) return;

    sOptItem& it = mItems[(size_t)idx];
    if (it.type == eOptType::Spacer) return;

    if (action == GUIACT_OPT_LEFT) { Adjust(it, -1); return; }
    if (action == GUIACT_OPT_RIGHT) { Adjust(it, +1); return; }

    Adjust(it, +1);
}

std::unique_ptr<cOptionsMenu> mOptionsMenu;

bool cFodder::Options_Menu_Run() {
    mOptionsMenu = std::make_unique<cOptionsMenu>();
    mOptionsMenu->Open();

    mMap_Destroy_Tiles.clear();

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

        mOptionsMenu->Tick();

        Video_Sleep();
    }

    const bool applied = mOptionsMenu->WasApplied();

    mInterruptCallback = nullptr;
    mOptionsMenu->Close();
    mOptionsMenu.reset();

    Image_FadeOut();
    return applied;
}
