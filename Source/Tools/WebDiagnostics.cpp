// Read-only engine state for the optional browser integration checks.
#if defined(__EMSCRIPTEN__) && defined(OPENFODDER_WEB_DIAGNOSTICS)
#include "stdafx.hpp"
#include <cstdio>

extern "C" EMSCRIPTEN_KEEPALIVE const char* openfodder_web_state() {
    static char state[768];
    if (!g_Fodder || !g_Fodder->mParams) return "{}";
    const auto& game = *g_Fodder;
    std::snprintf(state, sizeof(state),
        "{\"interrupt\":%u,\"engine\":%u,\"videoDone\":%d,\"videoTicked\":%d,"
        "\"inProgress\":%d,\"paused\":%d,\"exit\":%d,\"campaigns\":%u,"
        "\"mouseX\":%d,\"mouseY\":%d,\"inputX\":%d,\"inputY\":%d,"
        "\"eventX\":%d,\"eventY\":%d,\"buttons\":%d,\"sleepDelta\":%u,"
        "\"mission\":%u,\"phase\":%u,\"paletteAdjusting\":%d}",
        unsigned(game.mInterruptTick), unsigned(game.mMission_EngineTicks),
        int(game.mVideo_Done), int(game.mVideo_Ticked), int(game.mPhase_In_Progress),
        int(game.mPhase_Paused), int(game.mExit), unsigned(game.mCampaignList.size()),
        int(game.mMouseX), int(game.mMouseY), int(game.mInputMouseX), int(game.mInputMouseY),
        int(game.mMouse_EventLastPosition.mX), int(game.mMouse_EventLastPosition.mY),
        int(game.mMouseButtonStatus), unsigned(game.mParams->mSleepDelta),
        unsigned(game.mGame_Data.mMission_Number), unsigned(game.mGame_Data.mMission_Phase),
        int(game.mSurface && game.mSurface->isPaletteAdjusting()));
    return state;
}
#endif
