#pragma once

class cOptionsMenu {
public:
    cOptionsMenu();
    ~cOptionsMenu();

    void Open();
    void Close();

    // Draw (called from interrupt callback)
    void Draw();

    // Per-frame (optional) logic; safe to call from the menu loop
    void Tick();

    // GUI callback entrypoints (called by cFodder trampolines)
    void OnUp();
    void OnDown();
    void OnBack();
    void OnRowClick(int16 pAction, int16 pArg);

    bool IsDone() const { return mDone; }
    bool WasApplied() const { return mApplied; }

private:
    enum class eOptType : uint8_t {
        Toggle,
        IntRange,
        EnumCycle,
        Action,
        Spacer,
    };

    enum class eOptSection : uint8_t {
        Video,
        Input,
        Audio,
        Skip,
        Engine,
    };

    struct sOptItem {
        eOptType type = eOptType::Spacer;
        eOptSection section = eOptSection::Video;
        const char* label = "";
        int id = 0;

        // IntRange
        int minv = 0;
        int maxv = 0;
        int step = 1;

        // EnumCycle
        const char** enumNames = nullptr;
        int enumCount = 0;
    };

    enum eGUI_Action : int16 {
        GUIACT_NONE = 0,

        // Chrome
        GUIACT_OPT_UP,
        GUIACT_OPT_DOWN,
        GUIACT_OPT_BACK,

        // Rows
        GUIACT_OPT_ROW_PRIMARY,   // click row => toggle/inc
        GUIACT_OPT_LEFT,          // decrement (hover arrow)
        GUIACT_OPT_RIGHT,         // increment (hover arrow)
        GUIACT_OPT_TAB,           // arg = page index
        GUIACT_OPT_SAVE,
        GUIACT_OPT_CANCEL,

        // Action rows (Defaults / Back)
        GUIACT_OPT_ACTION,        // arg = OPT_DEFAULTS / OPT_BACK
    };

    enum eOptId : int {
        // existing...
        OPT_WINDOW_MODE = 1,
        OPT_WINDOW_SCALE,
        OPT_INTEGER_SCALING,

        OPT_ALTERNATE_MOUSE,
        OPT_MOUSE_LOCKED,
        OPT_MOUSE_SPEED,

        OPT_DISABLE_SOUND,
        OPT_CHEATS,

        OPT_SLEEP_DELTA,
        OPT_MAX_SPRITES,
        OPT_MAX_SPAWN,

        OPT_DEFAULTS,
        OPT_BACK,

        // ---- add these ----
        OPT_WINDOW_COLUMNS,
        OPT_WINDOW_ROWS,
        OPT_COPY_PROTECTION,
        OPT_DEFAULT_PLATFORM,
        OPT_DEFAULT_GAME,

        OPT_SKIP_INTRO,
        OPT_DISABLE_INTRO_VIDEO,
        OPT_SKIP_BRIEFING,
        OPT_SKIP_SERVICE,
        OPT_SKIP_RECRUIT,
    };


private:
    // Build menu items from current parameters.
    void BuildItems();
    void BuildPageItems();
    void ChangePage(int pageIndex);
    void RestoreSnapshot();

    // Value accessors (from sFodderParameters on cFodder)
    int  GetInt(int optId) const;
    void SetInt(int optId, int v);

    // Actions
    void ResetDefaults();

    // Formatting + interaction
    std::string FormatValue(const sOptItem& it) const;

    int  MouseToItemIndex() const;
    void Adjust(sOptItem& it, int dir);

    // Layout helpers (match your existing UI)
    int RowY0() const { return 0x44; }
    int RowH()  const { return 0x15; }
    
    bool AllowInput(int16 action, int16 arg);

    void RecalcShownItems();
    void ClampScroll();
    bool IsAdjustable(const sOptItem& it) const;
    void PrimaryActivate(sOptItem& it);

private:
    bool mDone = false;
    bool mApplied = false;

    int16_t mCurrentIndex = 0;
    int16_t mShownItems = 8;
    int16_t mCount = 0;

    // Debounce / repeat limiting (milliseconds)
    uint32_t mLastInputMs = 0;
    int16_t  mLastInputAction = 0;
    int16_t  mLastInputArg = 0;

    std::vector<sOptItem> mItems;
    std::vector<int> mPageItems;
    int16_t mCurrentPage = 0;
    sFodderParameters mSnapshot;
    bool mSnapshotValid = false;

    // Keep drawn strings alive for GUI draw calls (c_str pointers)
    std::vector<std::string> mDrawStrings;
};
