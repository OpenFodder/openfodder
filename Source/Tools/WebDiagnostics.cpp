// Engine state and playback controls for the optional browser integration checks.
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

extern "C" EMSCRIPTEN_KEEPALIVE int openfodder_web_play_audio(int kind) {
    if (!g_Fodder || !g_Fodder->mVersionCurrent || !g_Fodder->mVersionCurrent->isPCFormat()
        || !dynamic_cast<cSound_PC*>(g_Fodder->mSound.get())) return 0;
    switch (kind) {
    case 0: g_Fodder->mSound->Stop(); break;
    case 1: g_Fodder->mSound->Music_Play(16); break; // CANNON2.DAT (MOD)
    case 2: g_Fodder->mSound->Sound_Play(0, 0x10, 128, 0); break; // ALL16.VOC
    default: return 0;
    }
    return 1;
}
#endif
