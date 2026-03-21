/*
 *  Open Fodder
 *  ---------------
 *
 *  Copyright (C) 2008-2026 Open Fodder
 *
 *  Cooperative multiplayer implementation using GGPO rollback netcode.
 *
 *  Design:
 *   - Two players cooperate on the same mission simultaneously.
 *   - Each player controls their own cursor and can order any squad.
 *   - GGPO "frame" = one complete simulation step (3 interrupt ticks +
 *     one Phase_Cycle mission-logic pass), running at ~16.67 Hz.
 *   - Both cursors are transmitted each frame; P2's cursor is drawn as
 *     a distinct overlay sprite.
 *   - State save / load uses raw memcpy of the sprite array and map
 *     buffer.  Pointer stability is guaranteed within one process run
 *     because mSprites and mGame_Data.mSoldiers_Allocated never move.
 *
 *  This program is free software; see the GPL v3 header in Fodder.cpp.
 */

#ifdef OPENFODDER_ENABLE_NETWORK

#include "stdafx.hpp"
// Fodder.hpp, Event.hpp, Parameters.hpp are already included via stdafx.hpp
#include "Network/GGPOSession.hpp"
#include "Network/NetworkTypes.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>

#ifdef WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#  include <fcntl.h>
#  define SOCKET int
#  define INVALID_SOCKET (-1)
#  define SOCKET_ERROR (-1)
#  define closesocket close
#endif

// ============================================================
// Desync diagnostic log — writes to sync_p1.log / sync_p2.log
// ============================================================
static FILE* sSyncLog = nullptr;

static FILE* SyncLog_Get(int playerIndex) {
    if (!sSyncLog) {
        const char* name = (playerIndex == 0) ? "sync_p1.log" : "sync_p2.log";
        sSyncLog = fopen(name, "w");
    }
    return sSyncLog;
}

// ============================================================
// Internal binary serialisation helpers
// ============================================================

namespace {

struct sWriter {
    std::vector<uint8_t> buf;

    template<typename T>
    void write(const T& v) {
        using PlainT = typename std::remove_volatile<T>::type;
        const PlainT tmp = static_cast<PlainT>(v);
        const auto* p = reinterpret_cast<const uint8_t*>(&tmp);
        buf.insert(buf.end(), p, p + sizeof(PlainT));
    }

    void writeBytes(const void* data, size_t size) {
        const auto* p = reinterpret_cast<const uint8_t*>(data);
        buf.insert(buf.end(), p, p + size);
    }
};

struct sReader {
    const uint8_t* buf;
    size_t         pos;
    size_t         total;

    sReader(const uint8_t* b, size_t sz) : buf(b), pos(0), total(sz) {}

    template<typename T>
    T read() {
        T v;
        memcpy(&v, buf + pos, sizeof(T));
        pos += sizeof(T);
        return v;
    }

    void readBytes(void* dest, size_t size) {
        memcpy(dest, buf + pos, size);
        pos += size;
    }

    bool ok() const { return pos <= total; }
};

} // anonymous namespace

// ============================================================
// Camera state save / restore helpers
// ============================================================

void cFodder::Network_CameraSave(sNetCameraState& s) {
    s.CameraX                        = mCameraX;
    s.CameraY                        = mCameraY;
    s.MapTile_Ptr                    = mMapTile_Ptr;
    s.MapTile_ColumnOffset           = mMapTile_ColumnOffset;
    s.MapTile_RowOffset              = mMapTile_RowOffset;
    s.MapTile_DrawX                  = mMapTile_DrawX;
    s.MapTile_DrawY                  = mMapTile_DrawY;
    s.MapTile_SpeedX                 = mMapTile_SpeedX;
    s.MapTile_SpeedY                 = mMapTile_SpeedY;
    s.MapTile_TargetX                = mMapTile_TargetX;
    s.MapTile_TargetY                = mMapTile_TargetY;
    s.MapTile_MoveDirectionX         = mMapTile_MoveDirectionX;
    s.MapTile_MoveDirectionY         = mMapTile_MoveDirectionY;
    s.MapTile_MoveDirectionX_Previous= mMapTile_MoveDirectionX_Previous;
    s.MapTile_MoveDirectionY_Previous= mMapTile_MoveDirectionY_Previous;
    s.MapTile_SpeedX_Previous        = mMapTile_SpeedX_Previous;
    s.MapTile_SpeedY_Previous        = mMapTile_SpeedY_Previous;
    s.MapTile_MovedHorizontal        = mMapTile_MovedHorizontal;
    s.MapTile_MovedVertical          = mMapTile_MovedVertical;
    s.MapTile_Column_CurrentScreen   = mMapTile_Column_CurrentScreen;
    s.MapTile_Row_CurrentScreen      = mMapTile_Row_CurrentScreen;
    s.Camera_AccelerationX           = mCamera_AccelerationX;
    s.Camera_AccelerationY           = mCamera_AccelerationY;
    s.Camera_MoveDirectionX          = mCamera_MoveDirectionX;
    s.Camera_MoveDirectionY          = mCamera_MoveDirectionY;
    s.Camera_MovePauseX              = mCamera_MovePauseX;
    s.Camera_MovePauseY              = mCamera_MovePauseY;
    s.Camera_PanTargetX              = mCamera_PanTargetX;
    s.Camera_PanTargetY              = mCamera_PanTargetY;
    s.Camera_Scroll_Speed            = mCamera_Scroll_Speed;
    s.Camera_Speed_Reset_X           = mCamera_Speed_Reset_X;
    s.Camera_Speed_Reset_Y           = mCamera_Speed_Reset_Y;
    s.Camera_Speed_X                 = mCamera_Speed_X;
    s.Camera_Speed_Y                 = mCamera_Speed_Y;
    s.Camera_StartPosition_X         = mCamera_StartPosition_X;
    s.Camera_StartPosition_Y         = mCamera_StartPosition_Y;
    s.Camera_SquadLeaderX            = mCamera_SquadLeaderX;
    s.Camera_SquadLeaderY            = mCamera_SquadLeaderY;
    s.Camera_TileSpeed_Overflow      = mCamera_TileSpeed_Overflow;
    s.Camera_TileX                   = mCamera_TileX;
    s.Camera_TileY                   = mCamera_TileY;
    s.Camera_TileSpeedX              = mCamera_TileSpeedX;
    s.Camera_TileSpeedY              = mCamera_TileSpeedY;
    s.Camera_Speed_Max               = mCamera_Speed_Max;
    s.Camera_Start_Adjust            = mCamera_Start_Adjust;
    s.Camera_Reached_Target          = mCamera_Reached_Target;
    s.Camera_Panning_ToTarget        = mCamera_Panning_ToTarget;
    s.dword_39F36                    = dword_39F36;
    s.Mouse_Locked                   = mMouse_Locked;
}

void cFodder::Network_CameraRestore(const sNetCameraState& s) {
    mCameraX                          = s.CameraX;
    mCameraY                          = s.CameraY;
    mMapTile_Ptr                      = s.MapTile_Ptr;
    mMapTile_ColumnOffset             = s.MapTile_ColumnOffset;
    mMapTile_RowOffset                = s.MapTile_RowOffset;
    mMapTile_DrawX                    = s.MapTile_DrawX;
    mMapTile_DrawY                    = s.MapTile_DrawY;
    mMapTile_SpeedX                   = s.MapTile_SpeedX;
    mMapTile_SpeedY                   = s.MapTile_SpeedY;
    mMapTile_TargetX                  = s.MapTile_TargetX;
    mMapTile_TargetY                  = s.MapTile_TargetY;
    mMapTile_MoveDirectionX           = s.MapTile_MoveDirectionX;
    mMapTile_MoveDirectionY           = s.MapTile_MoveDirectionY;
    mMapTile_MoveDirectionX_Previous  = s.MapTile_MoveDirectionX_Previous;
    mMapTile_MoveDirectionY_Previous  = s.MapTile_MoveDirectionY_Previous;
    mMapTile_SpeedX_Previous          = s.MapTile_SpeedX_Previous;
    mMapTile_SpeedY_Previous          = s.MapTile_SpeedY_Previous;
    mMapTile_MovedHorizontal          = s.MapTile_MovedHorizontal;
    mMapTile_MovedVertical            = s.MapTile_MovedVertical;
    mMapTile_Column_CurrentScreen     = s.MapTile_Column_CurrentScreen;
    mMapTile_Row_CurrentScreen        = s.MapTile_Row_CurrentScreen;
    mCamera_AccelerationX             = s.Camera_AccelerationX;
    mCamera_AccelerationY             = s.Camera_AccelerationY;
    mCamera_MoveDirectionX            = s.Camera_MoveDirectionX;
    mCamera_MoveDirectionY            = s.Camera_MoveDirectionY;
    mCamera_MovePauseX                = s.Camera_MovePauseX;
    mCamera_MovePauseY                = s.Camera_MovePauseY;
    mCamera_PanTargetX                = s.Camera_PanTargetX;
    mCamera_PanTargetY                = s.Camera_PanTargetY;
    mCamera_Scroll_Speed              = s.Camera_Scroll_Speed;
    mCamera_Speed_Reset_X             = s.Camera_Speed_Reset_X;
    mCamera_Speed_Reset_Y             = s.Camera_Speed_Reset_Y;
    mCamera_Speed_X                   = s.Camera_Speed_X;
    mCamera_Speed_Y                   = s.Camera_Speed_Y;
    mCamera_StartPosition_X           = s.Camera_StartPosition_X;
    mCamera_StartPosition_Y           = s.Camera_StartPosition_Y;
    mCamera_SquadLeaderX              = s.Camera_SquadLeaderX;
    mCamera_SquadLeaderY              = s.Camera_SquadLeaderY;
    mCamera_TileSpeed_Overflow        = s.Camera_TileSpeed_Overflow;
    mCamera_TileX                     = s.Camera_TileX;
    mCamera_TileY                     = s.Camera_TileY;
    mCamera_TileSpeedX                = s.Camera_TileSpeedX;
    mCamera_TileSpeedY                = s.Camera_TileSpeedY;
    mCamera_Speed_Max                 = s.Camera_Speed_Max;
    mCamera_Start_Adjust              = s.Camera_Start_Adjust;
    mCamera_Reached_Target            = s.Camera_Reached_Target;
    mCamera_Panning_ToTarget          = s.Camera_Panning_ToTarget;
    dword_39F36                       = s.dword_39F36;
    mMouse_Locked                     = s.Mouse_Locked;
}

// ============================================================
// Network_Start
// ============================================================

bool cFodder::Network_Start() {
    g_Debugger->Notice("[GGPO] Network_Start: player=" + std::to_string(mStartParams->mNetworkPlayerIndex) +
                       " host=" + mStartParams->mNetworkRemoteHost +
                       " remotePort=" + std::to_string(mStartParams->mNetworkRemotePort) +
                       " localPort=" + std::to_string(mStartParams->mNetworkLocalPort));
    mNetSession = std::make_unique<cGGPOSession>();
    mNetFrameCount = 0;
    mInterruptTick = 0;          // Both machines must start with the same tick counter
    mNetKeyFlagsLocal = 0;
    mNet_P2_CursorX = 0;
    mNet_P2_CursorY = 0;
    mNet_RemoteCursorSprite = 0;
    mNet_LocalCursorWorldX = 0;
    mNet_LocalCursorWorldY = 0;
    mNetLocalPlayerIndex = mStartParams->mNetworkPlayerIndex;
    mNet_P2CamInitialised = false;
    memset(&mNet_DetCam, 0, sizeof(mNet_DetCam));
    memset(&mNet_P2Cam,  0, sizeof(mNet_P2Cam));
    memset(mNet_ButtonLeftToggle,  0, sizeof(mNet_ButtonLeftToggle));
    memset(mNet_ButtonRightToggle, 0, sizeof(mNet_ButtonRightToggle));
    memset(mNet_ButtonLRToggle,    0, sizeof(mNet_ButtonLRToggle));
    memset(mNet_ButtonLRToggle2,   0, sizeof(mNet_ButtonLRToggle2));
    memset(mNet_WalkTargetX,       0, sizeof(mNet_WalkTargetX));
    memset(mNet_WalkTargetY,       0, sizeof(mNet_WalkTargetY));
    memset(mNet_CameraPanTargetX,  0, sizeof(mNet_CameraPanTargetX));
    memset(mNet_CameraPanTargetY,  0, sizeof(mNet_CameraPanTargetY));

    // Both machines must use the same RNG state for determinism.
    // The default cPseudorand constructor seeds from time(), which differs
    // between machines.  Reset to a fixed seed so both start identical.
    mRandom.setSeed(0x1337);
    mRandomCallCount = 0;

    // Wire GGPO callbacks to member functions via lambdas.
    mNetSession->cbSaveState = [this](uint8_t** buf, int* len, int* csum, int /*frame*/) {
        return Network_SaveState(buf, len, csum);
    };
    mNetSession->cbLoadState = [this](uint8_t* buf, int len) {
        return Network_LoadState(buf, len);
    };
    mNetSession->cbFreeBuffer = [this](void* buf) {
        Network_FreeBuffer(buf);
    };
    mNetSession->cbAdvanceFrame = [this](sNetworkInput* inputs, int /*flags*/) {
        return Network_AdvanceFrame(inputs);
    };

    bool ok = false;
    if (mStartParams->mNetworkSyncTest) {
        ok = mNetSession->StartSyncTest();
    } else {
        ok = mNetSession->Start(
            mStartParams->mNetworkPlayerIndex,
            mStartParams->mNetworkLocalPort,
            mStartParams->mNetworkRemoteHost,
            mStartParams->mNetworkRemotePort
        );
    }

    if (!ok) {
        mNetSession.reset();
        return false;
    }
    return true;
}

// ============================================================
// Network_Stop
// ============================================================

void cFodder::Network_Stop() {
    if (mNetSession) {
        mNetSession->Stop();
        mNetSession.reset();
    }
}

// ============================================================
// Network_Sidebar_ForceSquadIcons
// Force-draw sidebar squad icons directly into mSidebar_Screen_Buffer
// using the local player's squad as "selected".  Called at render
// time (50 Hz) so the correct highlight is always in the buffer
// that Sidebar_Copy_To_Surface reads from.
// ============================================================

void cFodder::Network_Sidebar_ForceSquadIcons() {
    // Ensure we draw directly into the screen buffer (the one
    // Sidebar_Copy_To_Surface_Common reads from).
    uint16* savedBufPtr = mSidebar_Screen_BufferPtr;
    mSidebar_Screen_BufferPtr = mSidebar_Screen_Buffer;

    // Save state that the GUI functions modify as side effects.
    // These are also read during deterministic simulation (e.g.
    // Squad_Switch_Weapon uses mGUI_Loop_Squad_Current), so we must
    // restore them to avoid desync between machines.
    const int16 savedNextDrawY = mGUI_Squad_NextDraw_Y;
    const int16 savedSidebarDrawY = mSidebar_Draw_Y;
    const int16 savedLoopSquad = mGUI_Loop_Squad_Current;
    const int16 savedLoopDrawY = mGUI_Loop_Draw_Y;
    const int16 savedLoopIsCurrentSquad = mGUI_Loop_Is_CurrentSquad;

    const int16 localSq = static_cast<int16>(mNetLocalPlayerIndex);

    for (int16 sq = 0; sq < 3; ++sq) {
        mGUI_Loop_Squad_Current = sq;
        mGUI_Loop_Draw_Y = word_3AC2D[sq];
        if (!mSquads_TroopCount[sq])
            continue;
        mGUI_Loop_Is_CurrentSquad = (sq == localSq) ? -1 : 0;

        // Redraw the squad icon (active/inactive walking man).
        GUI_Sidebar_SquadIcon_Set();
        GUI_Sidebar_SquadIcon_Current_Draw();

        // Redraw the troop list backgrounds and names.  In the original
        // game these are only drawn during sidebar setup, which runs with
        // mSquad_Selected = 0 (deterministic).  We must repaint them here
        // so each player sees the correct active/inactive styling.
        GUI_Sidebar_TroopList_Draw();
    }

    // Restore all side-effect state so simulation isn't corrupted.
    mGUI_Squad_NextDraw_Y = savedNextDrawY;
    mSidebar_Draw_Y = savedSidebarDrawY;
    mGUI_Loop_Squad_Current = savedLoopSquad;
    mGUI_Loop_Draw_Y = savedLoopDrawY;
    mGUI_Loop_Is_CurrentSquad = savedLoopIsCurrentSquad;
    mSidebar_Screen_BufferPtr = savedBufPtr;
}

// ============================================================
// Network_GUI_Sidebar_Draw
// Network-aware version of GUI_Sidebar_Draw.  The simulation
// logic (weapon switching, timers) runs with the deterministic
// mSquad_Selected = 0, but the visual icon/grenade/rocket draw
// uses the local player's squad so the sidebar shows the correct
// highlight for whoever is playing on this machine.
// ============================================================

void cFodder::Network_GUI_Sidebar_Draw() {
    // --- Simulation logic (deterministic, mSquad_Selected = 0) ---
    Mission_Final_TimeToDie();

    // Weapon auto-switch: iterates all 3 squads by index,
    // independent of mSquad_Selected.
    {
        int16 Data4 = 2;
        int8 *Data20 = &mGUI_RefreshSquadGrenades[2];
        int8 *Data24 = &mGUI_RefreshSquadRockets[2];
        int16 *Data28 = mSquad_Grenades;
        int16 *Data2C = mSquad_Rockets;
        int16 *Data30 = mSquad_CurrentWeapon;

        for (int16 Data0 = 2; Data0 >= 0; --Data0) {
            if (*Data20 && !*(Data28 + Data4) && *(Data30 + Data4) == 1) {
                *(Data30 + Data4) = 3;
                *Data24 = -1;
            }
            if (*Data24 && !*(Data2C + Data4) && *(Data30 + Data4) == 3) {
                *(Data30 + Data4) = 1;
                *Data20 = -1;
            }
            --Data20;
            --Data24;
            --Data4;
        }
    }

    // Squad icon rendering is handled at render time by
    // Network_Sidebar_ForceSquadIcons() — no need to draw here.

    // Grenade/rocket refresh draws (still needed during simulation).
    {
        int8 *Data20 = &mGUI_RefreshSquadGrenades[2];
        int8 *Data24 = &mGUI_RefreshSquadRockets[2];

        for (int16 Data0 = 2; Data0 >= 0; --Data0, --Data20, --Data24) {
            if (*Data20) {
                *Data20 = 0;
                GUI_Sidebar_Grenades_Draw(Data0);
            }
            if (*Data24) {
                *Data24 = 0;
                GUI_Sidebar_Rockets_Draw(Data0);
            }
        }
    }
}

// ============================================================
// Network_Recruit_Show
// Network-aware version of Recruit_Show.  Skips the interactive
// recruitment loop (both machines auto-select defaults) and plays
// the helicopter intro without allowing mouse/ESC skip.
// Always returns 0 (success) — no abort path in network mode.
// ============================================================

int16 cFodder::Network_Recruit_Show() {
    Map_Load();
    Map_Load_Sprites();

    Phase_Soldiers_Count();
    mGame_Data.Soldier_Sort();
    Phase_Soldiers_Prepare(true);
    Phase_Soldiers_AttachToSprites();

    WindowTitleSet(false);

    if (!mStartParams->mDisableSound)
        mSound->Music_Play(0);

    // Skip interactive Recruit_Loop — both machines use the default selection.

    mRecruit_Mission_Restarting = false;
    GameData_Backup();

    // Retail or Custom Mode: play the helicopter intro.
    // The helicopter animation itself is guarded against mouse-skip
    // in network mode (see Graphics_PC.cpp / Graphics_Amiga.cpp).
    if (mVersionCurrent->isRetail() ||
        mCustom_Mode != eCustomMode_None)
    {
        Map_Load();
        Briefing_Intro_Helicopter_Play(false, mMapLoaded->getTileType());
    }

    mGraphics->Load_pStuff();
    return 0;
}

// ============================================================
// Network_Briefing_Show
// Network-aware version of Briefing_Show.
//
// Shows the briefing text, then waits for the local player to
// click.  Once clicked, shows "WAITING FOR PLAYER X" and
// exchanges UDP "READY" packets with the remote peer.  Both
// machines proceed once both have signalled ready.
//
// Always returns 1 (continue to phase).
// ============================================================

int16 cFodder::Network_Briefing_Show() {
    if (mVersionCurrent->isDemo() && mVersionDefault->isDemo())
        return 1;

    if (mVersionCurrent->hasBriefingScreen() || mCustom_Mode != eCustomMode_None || mGame_Data.mCampaign.isRandom()) {
        mBriefing_Screen_Active = true;

        Window_UpdateScreenSize();

        // Show the pre-ready Briefing Screen (mission title, phase, fade-in)
        Briefing_Show_PreReady();

        Map_Load();
        Map_Load_Sprites();

        // Prepare Squads
        Phase_Soldiers_Count();
        mGame_Data.Soldier_Sort();
        Phase_Soldiers_Prepare(false);
        Phase_Soldiers_AttachToSprites();

        mPhase_Aborted = false;

        // --- Show briefing info (same draws as Briefing_Show_Ready) ---
        mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
        Briefing_Draw_Mission_Title(0x2C);
        Briefing_Draw_Phase();
        Briefing_Draw_With();

        mSurface->Save();
        mGraphics->PaletteSet();

        // --- Wait for local player to click ---
        mMouse_Exit_Loop = false;
        do {
            Music_Increase_Channel_Volume();
            Video_Sleep();

            if (mSurface->isPaletteAdjusting())
                mSurface->palette_FadeTowardNew();
        } while (!mMouse_Exit_Loop && !mPhase_Aborted);

        mMouse_Exit_Loop = false;

        // --- UDP ready exchange with remote peer ---
        if (!mStartParams->mNetworkSyncTest)
            Network_Briefing_ReadySync();

        // Fade out
        Image_FadeOut();
        Mouse_Setup();

        mBriefing_Screen_Active = false;
        mBriefing_Aborted = 0;
        mPhase_Aborted = false;
    }

    return 1;
}

// ============================================================
// Network_Briefing_ReadySync
// Simple UDP "READY" exchange between peers.  Uses the same
// local/remote ports that GGPO will use (GGPO isn't started yet).
// Shows "WAITING FOR PLAYER X" overlay while waiting.
// ============================================================

void cFodder::Network_Briefing_ReadySync() {
    const uint32_t READY_MAGIC = 0x52445921; // "RDY!"

#ifdef WIN32
    // GGPO's cGGPOSession constructor calls WSAStartup, but the session
    // doesn't exist yet at briefing time.  Init Winsock here.
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        g_Debugger->Error("[NET] ReadySync: WSAStartup failed, skipping sync.");
        return;
    }
#endif

    // Create a non-blocking UDP socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        g_Debugger->Error("[NET] ReadySync: socket() failed, skipping sync.");
        return;
    }

    // Bind to the local port
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family      = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port        = htons(mStartParams->mNetworkLocalPort);

    if (bind(sock, (struct sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
        g_Debugger->Error("[NET] ReadySync: bind() failed on port " +
                          std::to_string(mStartParams->mNetworkLocalPort) + ", skipping sync.");
        closesocket(sock);
        return;
    }

    // Set non-blocking
#ifdef WIN32
    u_long nonBlocking = 1;
    ioctlsocket(sock, FIONBIO, &nonBlocking);
#else
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

    // Remote address
    struct sockaddr_in remoteAddr;
    memset(&remoteAddr, 0, sizeof(remoteAddr));
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port   = htons(mStartParams->mNetworkRemotePort);
    inet_pton(AF_INET, mStartParams->mNetworkRemoteHost.c_str(), &remoteAddr.sin_addr);

    bool remoteReady = false;
    const int remotePlayerNum = (mStartParams->mNetworkPlayerIndex == 0) ? 2 : 1;

    // Draw "WAITING FOR PLAYER X" on surface2 once
    {
        mSurface2->clearBuffer();
        mGraphics->PaletteSet(mSurface2);
        mSurface2->surfaceSetToPaletteNew();
        mSurface->paletteNew_SetDimmed(64);

        mGraphics->SetImage(mSurface2);
        mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
        mString_GapCharID = 0x25;

        std::string waitMsg = "WAITING FOR PLAYER " + std::to_string(remotePlayerNum);
        String_CalculateWidth(320, mFont_Underlined_Width, waitMsg);
        String_Print(mFont_Underlined_Width, 1, mGUI_Temp_X, 0x54, waitMsg);

        mSurface2->draw();
        mString_GapCharID = 0;
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        mGraphics->SetImageOriginal();
    }

    // Release the mouse grab so the cursor isn't trapped during the wait.
    mWindow->SetRelativeMouseMode(false);
    SDL_ShowCursor();

    while (!remoteReady && !mExit) {
        // Send our READY packet
        sendto(sock, (const char*)&READY_MAGIC, sizeof(READY_MAGIC), 0,
               (struct sockaddr*)&remoteAddr, sizeof(remoteAddr));

        // Check for remote READY
        uint32_t recvBuf = 0;
        struct sockaddr_in fromAddr;
#ifdef WIN32
        int fromLen = sizeof(fromAddr);
#else
        socklen_t fromLen = sizeof(fromAddr);
#endif
        int n = recvfrom(sock, (char*)&recvBuf, sizeof(recvBuf), 0,
                         (struct sockaddr*)&fromAddr, &fromLen);
        if (n == sizeof(recvBuf) && recvBuf == READY_MAGIC)
            remoteReady = true;

        // Render the waiting overlay
        {
            std::lock_guard<std::mutex> lock(mSurfaceMtx);
            if (!mStartParams->mDisableVideo) {
                mSurface->draw();
                mSurface2->draw();
                mSurface->mergeSurfaceBuffer(mSurface2);
                mWindow->RenderAt(mSurface);
                mWindow->FrameEnd();
            }
        }

        if (mSurface->isPaletteAdjusting())
            mSurface->palette_FadeTowardNew();

        mWindow->Cycle();
        eventsProcess();
        SDL_Delay(16);
    }

    // Send a few extra READY packets so the remote is sure to receive ours
    for (int i = 0; i < 10; ++i) {
        sendto(sock, (const char*)&READY_MAGIC, sizeof(READY_MAGIC), 0,
               (struct sockaddr*)&remoteAddr, sizeof(remoteAddr));
        SDL_Delay(5);
    }

    closesocket(sock);

#ifdef WIN32
    WSACleanup();
#endif

    // Recapture the mouse for gameplay.
    SDL_HideCursor();
    mWindow->SetRelativeMouseMode(true);

    // Restore palette
    mGraphics->PaletteSet();
    mSurface->palette_SetFromNew();
    mSurface->surfaceSetToPalette();
}

// ============================================================
// Network_GatherLocalInput
// ============================================================

void cFodder::Network_GatherLocalInput(sNetworkInput& out) {
    memset(&out, 0, sizeof(out));

    // Use the SDL-derived world cursor that the 50 Hz render loop maintains.
    // mNet_LocalCursorWorldX/Y is updated ONLY from Mouse_Cursor_Handle
    // (real SDL input) and is never overwritten by GGPO sync data (which
    // goes into mMouseX/mInputMouseX for rendering).  This prevents the
    // feedback loop where an unfocused window feeds back the GGPO-synced
    // position as its own input, causing the remote cursor to track the
    // local one.
    out.mMouseX = static_cast<int16_t>(mNet_LocalCursorWorldX);
    out.mMouseY = static_cast<int16_t>(mNet_LocalCursorWorldY);

    // Mouse buttons
    if (mMouse_EventLastButtonsPressed & 1)  out.mMouseButtons |= 1; // left
    if (mMouse_EventLastButtonsPressed & 2)  out.mMouseButtons |= 2; // right

    // Keyboard flags accumulated since last frame
    out.mKeyFlags = mNetKeyFlagsLocal;
    mNetKeyFlagsLocal = 0;

    // Current cursor sprite so the remote player can draw the correct cursor
    out.mCursorSprite = static_cast<int8_t>(mMouseSpriteCurrent);
}

// ============================================================
// Network_RedistributeSquads
// Called once after Phase_Prepare when networking is enabled.
// Re-assigns field_32 for all alive troops, alternating between
// squad 0 (P1) and squad 1 (P2), then rebuilds the squad lists.
// ============================================================

void cFodder::Network_RedistributeSquads() {
    int idx = 0;
    for (auto& Troop : mGame_Data.mSoldiers_Allocated) {
        if (Troop.mSprite == INVALID_SPRITE_PTR || Troop.mSprite == 0)
            continue;

        sSprite* Sprite = Troop.mSprite;

        // Skip dying troops (same filter as Squad_Rebuild)
        if (!(Sprite->field_75 & eSprite_Flag_Invincibility)) {
            if (Sprite->mAnimState < eSprite_Anim_Slide1) {
                if (Sprite->mAnimState && !Sprite->mInVehicle)
                    continue;
            }
        }

        if (Sprite->field_32 < 0)
            continue;

        // Alternate between squad 0 (P1) and squad 1 (P2)
        int16 oldSquad = Sprite->field_32;
        Sprite->field_32 = (int16)(idx & 1);
        g_Debugger->Notice("[GGPO] RedistributeSquads: troop idx=" + std::to_string(idx) +
                           " pos=(" + std::to_string(Sprite->mPosX) + "," + std::to_string(Sprite->mPosY) + ")" +
                           " squad " + std::to_string(oldSquad) + " -> " + std::to_string(Sprite->field_32));
        ++idx;
    }

    Squad_Rebuild();
    g_Debugger->Notice("[GGPO] RedistributeSquads: " + std::to_string(idx) + " troops split" +
                       " squad0=" + std::to_string(mSquads_TroopCount[0]) +
                       " squad1=" + std::to_string(mSquads_TroopCount[1]));

    // Each player starts commanding their own squad
    mSquad_Selected = (int16)mNetLocalPlayerIndex;
}

// ============================================================
// Network_ApplyInputs
// Apply global (non-cursor) inputs from both players.
// Cursor/squad ordering is handled per-player in Network_AdvanceFrame.
// ============================================================

void cFodder::Network_ApplyInputs(const sNetworkInput inputs[NETWORK_MAX_PLAYERS]) {
    const sNetworkInput& p1 = inputs[eNetPlayer_1];
    const sNetworkInput& p2 = inputs[eNetPlayer_2];

    // Store player-2 cursor position for overlay rendering
    mNet_P2_CursorX = p2.mMouseX;
    mNet_P2_CursorY = p2.mMouseY;

    // Store the remote player's cursor sprite for the overlay
    const int remote = (mNetLocalPlayerIndex == eNetPlayer_1) ? eNetPlayer_2 : eNetPlayer_1;
    mNet_RemoteCursorSprite = inputs[remote].mCursorSprite;

    // Global key flags – either player can pause or abort
    uint8_t sharedFlags = p1.mKeyFlags | p2.mKeyFlags;
    if (sharedFlags & eNetKey_Pause)  mPhase_Paused  = !mPhase_Paused;
    if (sharedFlags & eNetKey_Map)    mPhase_ShowMapOverview = 1;
    if (sharedFlags & eNetKey_Escape) mPhase_Aborted = true;

    // Weapon keys apply to each player's own fixed squad
    if (p1.mKeyFlags & eNetKey_WeaponG) {
        mSquad_SwitchWeapon = 1;
        mSquad_CurrentWeapon[0] = eWeapon_Grenade;
    }
    if (p1.mKeyFlags & eNetKey_WeaponR) {
        mSquad_SwitchWeapon = 1;
        mSquad_CurrentWeapon[0] = eWeapon_Rocket;
    }
    if (p2.mKeyFlags & eNetKey_WeaponG) {
        mSquad_SwitchWeapon = 1;
        mSquad_CurrentWeapon[1] = eWeapon_Grenade;
    }
    if (p2.mKeyFlags & eNetKey_WeaponR) {
        mSquad_SwitchWeapon = 1;
        mSquad_CurrentWeapon[1] = eWeapon_Rocket;
    }
}

// ============================================================
// Network_AdvanceFrame
// Run one complete simulation step using pre-synchronised inputs.
// Called both from Network_Tick() (normal play) and from the GGPO
// advance_frame callback (during rollback replay).
// ============================================================

bool cFodder::Network_AdvanceFrame(const sNetworkInput inputs[NETWORK_MAX_PLAYERS]) {
    // --- Desync diagnostic: log RNG + sprite checksum at frame entry ---
    {
        FILE* sl = SyncLog_Get(mNetLocalPlayerIndex);
        if (sl) {
            int16 rs0, rs1, rs2, rs3;
            mRandom.getSeeds(rs0, rs1, rs2, rs3);
            // Hash only gameplay-relevant sprite fields (not padding)
            uint32_t sprHash = 0x811c9dc5u;
            auto fnv = [&](const void* data, size_t len) {
                const uint8_t* p = (const uint8_t*)data;
                for (size_t i = 0; i < len; ++i) {
                    sprHash ^= p[i];
                    sprHash *= 0x01000193u;
                }
            };
            for (auto& sp : mSprites) {
                // Skip cosmetic-only sprite types that legitimately differ
                // between machines (rank icon follows local squad leader).
                if (sp.mSpriteType == eSprite_Player_Rank ||
                    sp.mSpriteType == eSprite_Player_Shadow)
                    continue;
                fnv(&sp.mPosX, sizeof(sp.mPosX));
                fnv(&sp.mPosY, sizeof(sp.mPosY));
                fnv(&sp.mPosXFrac, sizeof(sp.mPosXFrac));
                fnv(&sp.mPosYFrac, sizeof(sp.mPosYFrac));
                fnv(&sp.mSpriteType, sizeof(sp.mSpriteType));
                fnv(&sp.mAnimState, sizeof(sp.mAnimState));
                fnv(&sp.mDirection, sizeof(sp.mDirection));
                fnv(&sp.mSpeed, sizeof(sp.mSpeed));
                fnv(&sp.mTargetX, sizeof(sp.mTargetX));
                fnv(&sp.mTargetY, sizeof(sp.mTargetY));
                fnv(&sp.field_44, sizeof(sp.field_44));
                fnv(&sp.mFrameIndex, sizeof(sp.mFrameIndex));
            }
            fprintf(sl, "[SYNC-ENTRY] f=%d rng=%04X%04X%04X%04X spr=%08X itick=%d ptick=%d p1=(%d,%d b%d k%d) p2=(%d,%d b%d k%d)\n",
                    mNetFrameCount,
                    (uint16)rs0, (uint16)rs1, (uint16)rs2, (uint16)rs3,
                    sprHash,
                    (int)mInterruptTick, (int)mPhase_InterruptTicks,
                    (int)inputs[0].mMouseX, (int)inputs[0].mMouseY,
                    (int)inputs[0].mMouseButtons, (int)inputs[0].mKeyFlags,
                    (int)inputs[1].mMouseX, (int)inputs[1].mMouseY,
                    (int)inputs[1].mMouseButtons, (int)inputs[1].mKeyFlags);
            // Detailed per-sprite dump around the divergence frame
            {
                int idx = 0;
                for (auto& sp : mSprites) {
                    if (sp.mPosX != -32768) {
                        fprintf(sl, "  [SPR] f=%d i=%d type=%d x=%d y=%d xf=%d yf=%d dir=%d spd=%d tx=%d ty=%d f44=%d fi=%d anim=%d drawn=%d f32=%d\n",
                                mNetFrameCount, idx,
                                (int)sp.mSpriteType,
                                (int)sp.mPosX, (int)sp.mPosY,
                                (int)sp.mPosXFrac, (int)sp.mPosYFrac,
                                (int)sp.mDirection, (int)sp.mSpeed,
                                (int)sp.mTargetX, (int)sp.mTargetY,
                                (int)sp.field_44, (int)sp.mFrameIndex,
                                (int)sp.mAnimState, (int)sp.mWasDrawn,
                                (int)sp.field_32);
                    }
                    ++idx;
                }
            }
            fflush(sl);
        }
    }

    // Apply global (non-cursor) inputs: pause/abort/weapon keys
    Network_ApplyInputs(inputs);

    // Camera + cursor handling (equivalent to 3× Phase_Loop_Interrupt
    // minus the hardware-input read).
    for (int tick = 0; tick < 3; ++tick) {
        ++mPhase_InterruptTicks;
        ++mInterruptTick;

        if (mInput_Enabled) {
            // Camera must follow the SAME squad on both machines so that
            // mCameraX/Y (part of the saved state) are deterministic.
            // We always follow squad 0 / P1 cursor for the simulation camera.
            // P2's visual camera is handled separately after GGPO state-save.
            mSquad_Selected = 0;
            if (mSquads[0][0] != INVALID_SPRITE_PTR && mSquads[0][0] != nullptr)
                mSquad_Leader = mSquads[0][0];
            {
                const int16 cX = static_cast<int16>(mCameraX >> 16);
                const int16 cY = static_cast<int16>(mCameraY >> 16);
                mMouseX = static_cast<int16>(inputs[eNetPlayer_1].mMouseX - cX);
                mMouseY = static_cast<int16>(inputs[eNetPlayer_1].mMouseY - cY);
            }
            Camera_Handle();

            // Camera_Handle adjusted mCamera_PanTargetX/Y (via
            // Camera_PanTarget_AdjustToward_SquadLeader) to track
            // the squad leader.  Propagate these to P1's per-player
            // saved state so the per-player loop below doesn't
            // overwrite them with stale values, which would cause
            // the camera to bounce instead of smoothly following.
            mNet_CameraPanTargetX[0] = mCamera_PanTargetX;
            mNet_CameraPanTargetY[0] = mCamera_PanTargetY;

            if (!mPhase_Finished) {
                // Run Mouse_Inputs_Check for each player independently.
                // The engine was designed for 1 player — many globals assume
                // a single active squad.  We must save/restore ALL of them
                // around each player's call so state doesn't bleed between
                // players.
                const int16 camX = static_cast<int16>(mCameraX >> 16);
                const int16 camY = static_cast<int16>(mCameraY >> 16);

                for (int pl = 0; pl < NETWORK_MAX_PLAYERS; ++pl) {
                    const int squad = pl; // P1 = squad 0, P2 = squad 1

                    // --- Squad pointers ---
                    mSquad_Selected = static_cast<int16>(squad);
                    if (mSquads[squad][0] != INVALID_SPRITE_PTR && mSquads[squad][0] != nullptr)
                        mSquad_Leader = mSquads[squad][0];
                    mSquad_CurrentVehicle = mSquad_CurrentVehicles[squad];

                    // --- Cursor (world → screen) ---
                    mMouseX      = static_cast<int16>(inputs[pl].mMouseX - camX);
                    mMouseY      = static_cast<int16>(inputs[pl].mMouseY - camY);
                    mInputMouseX = mMouseX;
                    mInputMouseY = mMouseY;

                    // --- Button state (per-player) ---
                    mMouseButtonStatus             = inputs[pl].mMouseButtons;
                    mMouse_Button_Left_Toggle      = mNet_ButtonLeftToggle[pl];
                    mMouse_Button_Right_Toggle     = mNet_ButtonRightToggle[pl];
                    mMouse_Button_LeftRight_Toggle = mNet_ButtonLRToggle[pl];
                    mMouse_Button_LeftRight_Toggle2= mNet_ButtonLRToggle2[pl];

                    // --- Per-squad walk target memory ---
                    // Squad_Walk_Target_Set stores the last click in shared
                    // globals mSquad_WalkTargetX/Y; we must isolate them so
                    // one player's click doesn't become a waypoint for the
                    // other squad.
                    mSquad_WalkTargetX = mNet_WalkTargetX[pl];
                    mSquad_WalkTargetY = mNet_WalkTargetY[pl];

                    // --- Per-player camera pan target ---
                    // Mouse_Inputs_Check sets mCamera_PanTargetX/Y on click;
                    // isolate so P2's click doesn't steer the deterministic camera.
                    mCamera_PanTargetX = mNet_CameraPanTargetX[pl];
                    mCamera_PanTargetY = mNet_CameraPanTargetY[pl];

                    // --- Cursor sprite (per-player) ---
                    // Mouse_Inputs_Check sets mMouseSpriteNew (e.g. crosshair
                    // when firing). Without isolation P2's firing changes P1's
                    // cursor appearance.
                    int16 savedMouseSpriteNew = mMouseSpriteNew;
                    int16 savedMouseSpriteCur = mMouseSpriteCurrent;
                    int16 savedMouseXOff      = mMouseX_Offset;
                    int16 savedMouseYOff      = mMouseY_Offset;

                    Mouse_UpdateButtons();
                    Mouse_Inputs_Check();

                    // --- Save per-player state back ---
                    mNet_ButtonLeftToggle[pl]  = mMouse_Button_Left_Toggle;
                    mNet_ButtonRightToggle[pl] = mMouse_Button_Right_Toggle;
                    mNet_ButtonLRToggle[pl]    = mMouse_Button_LeftRight_Toggle;
                    mNet_ButtonLRToggle2[pl]   = mMouse_Button_LeftRight_Toggle2;
                    mNet_WalkTargetX[pl]       = mSquad_WalkTargetX;
                    mNet_WalkTargetY[pl]       = mSquad_WalkTargetY;
                    mNet_CameraPanTargetX[pl]  = mCamera_PanTargetX;
                    mNet_CameraPanTargetY[pl]  = mCamera_PanTargetY;

                    // Restore cursor sprite state so the other player's
                    // input doesn't change the local cursor appearance.
                    if (pl != mNetLocalPlayerIndex) {
                        mMouseSpriteNew  = savedMouseSpriteNew;
                        mMouseSpriteCurrent = savedMouseSpriteCur;
                        mMouseX_Offset   = savedMouseXOff;
                        mMouseY_Offset   = savedMouseYOff;
                    }
                }

                // Restore deterministic state (squad 0 / P1) so that
                // saved state and sprite handling are identical on both machines.
                mSquad_Selected       = 0;
                if (mSquads[0][0] != INVALID_SPRITE_PTR && mSquads[0][0] != nullptr)
                    mSquad_Leader = mSquads[0][0];
                mSquad_CurrentVehicle = mSquad_CurrentVehicles[0];
                mMouseX      = static_cast<int16>(inputs[eNetPlayer_1].mMouseX - camX);
                mMouseY      = static_cast<int16>(inputs[eNetPlayer_1].mMouseY - camY);
                mInputMouseX = mMouseX;
                mInputMouseY = mMouseY;
                mMouseButtonStatus             = inputs[eNetPlayer_1].mMouseButtons;
                mMouse_Button_Left_Toggle      = mNet_ButtonLeftToggle[0];
                mMouse_Button_Right_Toggle     = mNet_ButtonRightToggle[0];
                mMouse_Button_LeftRight_Toggle = mNet_ButtonLRToggle[0];
                mMouse_Button_LeftRight_Toggle2= mNet_ButtonLRToggle2[0];
                mSquad_WalkTargetX             = mNet_WalkTargetX[0];
                mSquad_WalkTargetY             = mNet_WalkTargetY[0];
                mCamera_PanTargetX             = mNet_CameraPanTargetX[0];
                mCamera_PanTargetY             = mNet_CameraPanTargetY[0];

                // Re-derive button press state from P1's button status.
                // Mouse_UpdateButtons was already called for P1 inside the
                // per-player loop; we just need to ensure the derived globals
                // reflect P1's state (P2's call overwrote them).
                mButtonPressLeft  = (inputs[eNetPlayer_1].mMouseButtons & 1) ? -1 : 0;
                mButtonPressRight = (inputs[eNetPlayer_1].mMouseButtons & 2) ? -1 : 0;
            }
        }

        // Sound/music bookkeeping
        if (mSound)
            Music_Increase_Channel_Volume();

        if (mSquad_SwitchWeapon) {
            --mSquad_SwitchWeapon;
            Squad_Switch_Weapon();
        }

        if (!mInput_Enabled) continue;
        if (!mPhase_In_Progress) continue;
        if (mPhase_Completed_Timer || mMapTile_Music_Play ||
            mPhase_Complete || mPhase_TryAgain || mPhase_Aborted) {
            mPhase_Finished = true;
        }

        if (!(mInterruptTick & 3))
            Music_Fade_SwitchTrack();

        if (mSoundEffectToPlay) {
            Sound_Play(mSquad_Leader, mSoundEffectToPlay, 0);
            mSoundEffectToPlay = 0;
        }
    }

    // Run mission simulation logic (the same work Phase_Cycle does after
    // the 3-tick threshold is met).
    ++mMission_EngineTicks;
    mPhase_InterruptTicks = 0;

    if (mCamera_Start_Adjust) {
        Camera_SetTargetToStartPosition();
        mCamera_Start_Adjust = false;
    } else if (!mPhase_TryAgain) {
        if (mGUI_Sidebar_Setup >= 0)
            GUI_Sidebar_Setup();
        else
            Network_GUI_Sidebar_Draw();

        Phase_Progress_Check();
        mHelicopterCallPadPressedCount = 0;
        if (word_3A9B8 >= 0)
            --word_3A9B8;

        Sprite_Find_HumanVehicles();

        // Mirror Phase_Cycle: check mission goals before sprite simulation.
        Phase_Goals_Check();

        // Decrement walk-target step counters and rebuild squad membership
        // arrays from sprite field_32 values.  Both are called every frame
        // in Phase_Cycle; omitting them here caused stale squad lists and
        // broken walk-target routing.
        Squad_Walk_Steps_Decrease();
        Squad_Rebuild();

        // Store world-space cursors for per-squad sprite AI (Sprite_Handle_Troop_Direct_TowardMouse).
        mNetSquadCursorX[0] = inputs[eNetPlayer_1].mMouseX;
        mNetSquadCursorY[0] = inputs[eNetPlayer_1].mMouseY;
        mNetSquadCursorX[1] = inputs[eNetPlayer_2].mMouseX;
        mNetSquadCursorY[1] = inputs[eNetPlayer_2].mMouseY;

        // Set a consistent mSquad_Selected and mMouseX for
        // Mission_Sprites_Handle so sprite AI produces identical results
        // on both machines.  This also covers the case where mInput_Enabled
        // was false (camera pan intro) and the 3-tick loop didn't set
        // mMouseX from synced inputs — without this, mMouseX would retain
        // whatever the 50 Hz SDL loop set, diverging between machines.
        mSquad_Selected = 0;
        if (mSquads[0][0] != INVALID_SPRITE_PTR && mSquads[0][0] != nullptr)
            mSquad_Leader = mSquads[0][0];
        {
            const int16 camX = static_cast<int16>(mCameraX >> 16);
            const int16 camY = static_cast<int16>(mCameraY >> 16);
            mMouseX      = static_cast<int16>(inputs[eNetPlayer_1].mMouseX - camX);
            mMouseY      = static_cast<int16>(inputs[eNetPlayer_1].mMouseY - camY);
            mInputMouseX = mMouseX;
            mInputMouseY = mMouseY;
        }

        // --- Desync diagnostic: log RNG state before sprite handling ---
        {
            FILE* sl = SyncLog_Get(mNetLocalPlayerIndex);
            mNetSyncLogFile = sl;  // expose to Sprite_Handle_Loop
            if (sl) {
                int16 rs0, rs1, rs2, rs3;
                mRandom.getSeeds(rs0, rs1, rs2, rs3);
                fprintf(sl, "[SYNC-PRE] f=%d rng=%04X%04X%04X%04X rc=%u\n",
                        mNetFrameCount,
                        (uint16)rs0, (uint16)rs1, (uint16)rs2, (uint16)rs3,
                        mRandomCallCount);
            }
        }

        // Force all sprites to be considered "on screen" for the simulation.
        // mWasDrawn is set during RENDERING based on the LOCAL camera, but
        // P1 and P2 have different cameras.  Sprite handlers that check
        // mWasDrawn (smoke, fire trails, native spawning, explosions, etc.)
        // would execute on one machine but not the other, diverging the RNG
        // sequence and causing permanent desync.
        for (auto& sprite : mSprites) {
            if (sprite.mPosX != -32768)
                sprite.mWasDrawn = true;
        }

        Mission_Sprites_Handle();

        // --- Desync diagnostic: log RNG state after sprite handling ---
        {
            FILE* sl = SyncLog_Get(mNetLocalPlayerIndex);
            if (sl) {
                int16 rs0, rs1, rs2, rs3;
                mRandom.getSeeds(rs0, rs1, rs2, rs3);
                fprintf(sl, "[SYNC] f=%d rng=%04X%04X%04X%04X rc=%u\n",
                        mNetFrameCount,
                        (uint16)rs0, (uint16)rs1, (uint16)rs2, (uint16)rs3,
                        mRandomCallCount);
                fflush(sl);
            }
        }

        // Mirror the post-sprite-handle calls from Phase_Cycle.
        Squad_Switch_Timer();
        Sprite_Bullet_SetData();
        Squad_EnteredVehicle_TimerTick();

        // Squad_Set_CurrentVehicle uses mSquad_Selected — run for both squads.
        for (int sq = 0; sq < NETWORK_MAX_PLAYERS; ++sq) {
            mSquad_Selected = static_cast<int16>(sq);
            Squad_Set_CurrentVehicle();
        }
        mSquad_Selected = 0;

        // Keep deterministic state for GGPO save (mSquad_Selected = 0).
        // Local rendering state is set in Network_Tick after AdvanceFrame.
    }

    // Drive palette fade animations (e.g. fade-in after camera pan).
    // Phase_Cycle does this; we must mirror it in the network path.
    if (mSurface->isPaletteAdjusting())
        mSurface->palette_FadeTowardNew();

    ++mNetFrameCount;
    return true;
}

// ============================================================
// Network_Tick
// Called from the main Phase_Loop() in place of Video_Sleep() /
// Phase_Cycle() when networking is enabled.
// Returns: 1 = running, 0 = phase won, -1 = phase aborted.
// ============================================================

int16 cFodder::Network_Tick() {
    if (!mNetSession || !mNetSession->IsRunning())
        return -1;

    // Allow GGPO to pump the network
    mNetSession->Idle(0);

    // Gather local input
    sNetworkInput localInput;
    Network_GatherLocalInput(localInput);

    // Submit local input to GGPO.
    // If GGPO rejects it (prediction threshold reached), stall until remote
    // inputs arrive so the rollback window never grows beyond MAX_PREDICTION_FRAMES.
    if (!mNetSession->AddLocalInput(localInput)) {
        return 1;
    }

    // Synchronise inputs from both players.
    // This call may trigger rollback (save/load + advance_frame callbacks).
    sNetworkInput syncInputs[NETWORK_MAX_PLAYERS];
    memset(syncInputs, 0, sizeof(syncInputs));
    int disconnectFlags = 0;
    if (!mNetSession->SynchronizeInput(syncInputs, disconnectFlags)) {
        // Remote input not yet available – wait next tick
        return 1;
    }

    // --- Restore deterministic camera before simulation ---
    // After the previous frame we may have overridden the camera for the
    // local player's viewport (P2 follows squad 1).  Restore the
    // deterministic camera so the simulation produces identical results
    // on both machines.  On the very first frame mNet_P2CamInitialised is
    // false, so we skip the restore (camera is already deterministic).
    if (mNet_P2CamInitialised && mNetLocalPlayerIndex != eNetPlayer_1) {
        Network_CameraRestore(mNet_DetCam);
    }

    // Run one full simulation frame with the synchronised inputs
    Network_AdvanceFrame(syncInputs);

    // Notify GGPO that the frame has been simulated (saves state).
    mNetSession->AdvanceFrame();

    // --- Save the deterministic camera for next-frame restore ---
    Network_CameraSave(mNet_DetCam);

    // --- Restore local view state for rendering ---
    // The simulation kept mSquad_Selected = 0 for determinism.  Now that
    // GGPO has saved the deterministic state, switch to the local player's
    // squad so the sidebar, cursor, and camera show the right context.
    {
        const int16 localSq = static_cast<int16>(mNetLocalPlayerIndex);
        mSquad_Selected = localSq;
        if (mSquads[localSq][0] != INVALID_SPRITE_PTR && mSquads[localSq][0] != nullptr)
            mSquad_Leader = mSquads[localSq][0];

        // --- P2 local camera: follow squad 1 with smooth scrolling ---
        if (mNetLocalPlayerIndex != eNetPlayer_1) {
            // During the camera-pan intro (mInput_Enabled == false) both
            // players must see the same pan, so P2 just mirrors the
            // deterministic camera.  Once gameplay begins we run P2's
            // own independent camera following squad 1.
            if (!mInput_Enabled) {
                // Pan intro still running — mirror the deterministic camera.
                // mNet_DetCam was just saved above so tile state is correct.
                mNet_P2CamInitialised = false;
            } else if (!mNet_P2CamInitialised) {
                // First gameplay frame: seed P2's camera from the
                // deterministic state (which has now panned to the soldiers).
                mNet_P2Cam = mNet_DetCam;
                mNet_P2CamInitialised = true;
            } else {
                // Subsequent frames: load P2's camera from previous frame
                Network_CameraRestore(mNet_P2Cam);

                const int16 camX = static_cast<int16>(mCameraX >> 16);
                const int16 camY = static_cast<int16>(mCameraY >> 16);
                mMouseX = static_cast<int16>(syncInputs[eNetPlayer_2].mMouseX - camX);
                mMouseY = static_cast<int16>(syncInputs[eNetPlayer_2].mMouseY - camY);

                // Run Camera_Handle 3 times to match the tick rate
                for (int tick = 0; tick < 3; ++tick)
                    Camera_Handle();

                // Save P2's updated camera for next frame
                Network_CameraSave(mNet_P2Cam);
            }
        }

        // Restore the local cursor from SDL tracking (mNet_LocalCursorWorldX).
        // The simulation and P2 camera handling above may have modified
        // mMouseX; we must restore it so the 50 Hz Mouse_Cursor_Handle
        // delta accumulation works from the correct base.
        // We use mNet_LocalCursorWorldX (pure SDL, never overwritten by GGPO)
        // rather than syncInputs — with frame delay > 0 the synced position
        // is stale and would cause ping-pong.
        {
            const int16 camX = static_cast<int16>(mCameraX >> 16);
            const int16 camY = static_cast<int16>(mCameraY >> 16);
            mMouseX      = static_cast<int16>(mNet_LocalCursorWorldX - camX);
            mMouseY      = static_cast<int16>(mNet_LocalCursorWorldY - camY);
            mInputMouseX = mMouseX;
            mInputMouseY = mMouseY;
        }

    }

    // Mirror Phase_Cycle completion logic: wait for the full
    // Phase_Progress_Check timer countdown (shows "PHASE COMPLETE" /
    // "TRY AGAIN" text, fades to black) before returning.
    if (mGame_Data.mGamePhase_Data.mIsComplete) {
        if (mPhase_Aborted)
            Sprite_Handle_Player_Destroy_Unk();
        else {
            if (!mPhase_TryAgain)
                return 0;
            Sprite_Handle_Player_DestroyAll();
        }
        return -1;
    }

    return 1;
}

// ============================================================
// Network_Draw_WaitingForPlayer
// Displayed during GGPO peer synchronization (before the
// first simulation frame).  Modelled on Draw_Phase_Paused.
// ============================================================

void cFodder::Network_Draw_WaitingForPlayer() {
    mSurface2->clearBuffer();

    mGraphics->PaletteSet(mSurface2);
    mSurface2->surfaceSetToPaletteNew();

    // Dim the current surface
    mSurface->paletteNew_SetDimmed(64);

    // Draw to the secondary surface
    {
        mGraphics->SetImage(mSurface2);

        mGraphics->SetActiveSpriteSheet(eGFX_BRIEFING);
        mString_GapCharID = 0x25;

        String_CalculateWidth(320 + SIDEBAR_WIDTH, mFont_Underlined_Width, "WAITING FOR PLAYER");
        String_Print(mFont_Underlined_Width, 1, mGUI_Temp_X, 0x54, "WAITING FOR PLAYER");

        mSurface2->draw();
        mString_GapCharID = 0;
        mGraphics->SetActiveSpriteSheet(eGFX_IN_GAME);
        mGraphics->SetImageOriginal();
    }
}

// ============================================================
// Network_DrawP2Cursor
// Render the remote player's cursor as a simple crosshair
// on top of the game surface.
// ============================================================

void cFodder::Network_DrawP2Cursor() {
    if (!mNetSession || !mNetSession->IsRunning()) return;
    if (!mSurface || !mPhase_In_Progress) return;

    // Determine which player's cursor is "remote" and needs an overlay.
    int16 remoteWorldX, remoteWorldY;
    if (mNetLocalPlayerIndex == eNetPlayer_1) {
        remoteWorldX = mNet_P2_CursorX;
        remoteWorldY = mNet_P2_CursorY;
    } else {
        // P2's screen: draw P1's cursor overlay using last known world pos.
        remoteWorldX = mNetSquadCursorX[eNetPlayer_1];
        remoteWorldY = mNetSquadCursorY[eNetPlayer_1];
    }

    // No data yet (first frame before any simulation has run)
    if (remoteWorldX == 0 && remoteWorldY == 0)
        return;

    // Convert world coords to screen coords
    const int16 camX = static_cast<int16>(mCameraX >> 16);
    const int16 camY = static_cast<int16>(mCameraY >> 16);
    const int16 screenX = static_cast<int16>(remoteWorldX - camX);
    const int16 screenY = static_cast<int16>(remoteWorldY - camY);

    // Skip if off-screen or near edges.  Video_Draw_8 has no bounds checking
    // and Mouse_DrawCursor adds SIDEBAR_WIDTH (48) to X and 12 to Y, so we
    // need generous margins to prevent buffer overruns.
    if (screenX < 0 || screenX > static_cast<int16>(getCameraWidth()) - 16 ||
        screenY < 0 || screenY > static_cast<int16>(getCameraHeight()) - 16)
        return;

    // Draw the remote cursor with a distinct palette vs the local cursor.
    // Save/restore all state that Mouse_DrawCursor and its callees touch.
    const int16 savedX      = mMouseX;
    const int16 savedY      = mMouseY;
    const int16 savedOffX   = mMouseX_Offset;
    const int16 savedOffY   = mMouseY_Offset;
    const int16 savedSprNew = mMouseSpriteNew;
    const int16 savedSprCur = mMouseSpriteCurrent;

    mMouseX = screenX;
    mMouseY = screenY;
    mMouseX_Offset = 0;
    mMouseY_Offset = 0;
    mMouseSpriteCurrent = mNet_RemoteCursorSprite;
    //GetGraphics<cGraphics_Amiga>()->SetCursorPalette(0x40);
    //Mouse_DrawCursor();
    //GetGraphics<cGraphics_Amiga>()->SetCursorPalette(0xE0);
    mMouseX          = savedX;
    mMouseY          = savedY;
    mMouseX_Offset   = savedOffX;
    mMouseY_Offset   = savedOffY;
    mMouseSpriteNew  = savedSprNew;
    mMouseSpriteCurrent = savedSprCur;
}

// ============================================================
// Network_SaveState
// Serialise all simulation-relevant state into a malloc'd buffer.
// GGPO will call Network_FreeBuffer when done with it.
// ============================================================

bool cFodder::Network_SaveState(uint8_t** buffer, int* len, int* checksum) {
    sWriter w;

    // --- Magic header ---
    const uint32_t MAGIC = 0x464F4444u; // "FODD"
    w.write(MAGIC);
    w.write((uint32_t)mNetFrameCount);

    // --- RNG state (4 × int16) ---
    {
        int16 s0, s1, s2, s3;
        mRandom.getSeeds(s0, s1, s2, s3);
        w.write(s0); w.write(s1); w.write(s2); w.write(s3);
        w.write(mRandomCallCount);
    }

    // --- Core tick counters ---
    w.write(mInterruptTick);
    w.write(mPhase_InterruptTicks);
    w.write(mMission_EngineTicks);
    w.write(mPhase_Completed_Timer);

    // --- Phase flags ---
    w.write(mPhase_Complete);
    w.write(mPhase_Aborted);
    w.write(mPhase_TryAgain);
    w.write(mPhase_Finished);
    w.write(mPhase_In_Progress);
    w.write(mPhase_Paused);
    w.write(mPhase_ShowMapOverview);
    w.write(mMapTile_Music_Play);
    w.write(mPhase_Aborted2);
    w.write(mPhase_EscapeKeyAbort);
    w.write(mInput_Enabled);

    // --- Camera / map scroll ---
    // Camera state is needed for determinism: mCameraX/Y is used to convert
    // world-space mouse coords to screen-space during Network_AdvanceFrame.
    // During GGPO rollback, Network_AdvanceFrame is called directly (not via
    // Network_Tick), so mNet_DetCam restore doesn't run — the save state
    // must contain correct camera values.
    w.write(mCameraX);
    w.write(mCameraY);
    w.write(mMapTile_Ptr);
    w.write(mMapTile_ColumnOffset);
    w.write(mMapTile_RowOffset);
    w.write(mMapTile_DrawX);
    w.write(mMapTile_DrawY);
    w.write(mMapTile_SpeedX);
    w.write(mMapTile_SpeedY);
    w.write(mMapTile_TargetX);
    w.write(mMapTile_TargetY);
    w.write(mMapTile_MoveDirectionX);
    w.write(mMapTile_MoveDirectionY);
    w.write(mMapTile_MoveDirectionX_Previous);
    w.write(mMapTile_MoveDirectionY_Previous);
    w.write(mMapTile_SpeedX_Previous);
    w.write(mMapTile_SpeedY_Previous);
    w.write(mMapTile_MovedHorizontal);
    w.write(mMapTile_MovedVertical);
    w.write(mMapTile_Column_CurrentScreen);
    w.write(mMapTile_Row_CurrentScreen);
    w.write(mCamera_AccelerationX);
    w.write(mCamera_AccelerationY);
    w.write(mCamera_MoveDirectionX);
    w.write(mCamera_MoveDirectionY);
    w.write(mCamera_MovePauseX);
    w.write(mCamera_MovePauseY);
    w.write(mCamera_PanTargetX);
    w.write(mCamera_PanTargetY);
    w.write(mCamera_Scroll_Speed);
    w.write(mCamera_Speed_Reset_X);
    w.write(mCamera_Speed_Reset_Y);
    w.write(mCamera_Speed_X);
    w.write(mCamera_Speed_Y);
    w.write(mCamera_StartPosition_X);
    w.write(mCamera_StartPosition_Y);
    w.write(mCamera_SquadLeaderX);
    w.write(mCamera_SquadLeaderY);
    w.write(mCamera_TileSpeed_Overflow);
    w.write(mCamera_TileX);
    w.write(mCamera_TileY);
    w.write(mCamera_TileSpeedX);
    w.write(mCamera_TileSpeedY);
    w.write(mCamera_Speed_Max);
    w.write(mCamera_Start_Adjust);
    w.write(mCamera_Reached_Target);
    w.write(mCamera_Panning_ToTarget);

    // --- Squad state ---
    w.writeBytes(mSquad_Grenades,         sizeof(mSquad_Grenades));
    w.writeBytes(mSquad_Rockets,          sizeof(mSquad_Rockets));
    w.writeBytes(mSquad_CurrentWeapon,    sizeof(mSquad_CurrentWeapon));
    w.writeBytes(mSquads_TroopCount,      sizeof(mSquads_TroopCount));
    w.writeBytes(mSquad_Walk_Target_Indexes, sizeof(mSquad_Walk_Target_Indexes));
    w.writeBytes(mSquad_Walk_Target_Steps,   sizeof(mSquad_Walk_Target_Steps));
    // Walk-target queue and squad-join state — needed for correct rollback.
    w.writeBytes(mSquad_WalkTargets,         sizeof(mSquad_WalkTargets));
    w.writeBytes(mSquad_Join_TargetSquad,    sizeof(mSquad_Join_TargetSquad));
    // mSquad_Join_TargetSprite: raw pointers into mSprites (stable addresses).
    for (int i = 0; i < 6; ++i)
        w.write(reinterpret_cast<uintptr_t>(mSquad_Join_TargetSprite[i]));
    w.write(mSquad_Select_Timer);
    w.writeBytes(mNet_ButtonLeftToggle,  sizeof(mNet_ButtonLeftToggle));
    w.writeBytes(mNet_ButtonRightToggle, sizeof(mNet_ButtonRightToggle));
    w.writeBytes(mNet_ButtonLRToggle,    sizeof(mNet_ButtonLRToggle));
    w.writeBytes(mNet_ButtonLRToggle2,   sizeof(mNet_ButtonLRToggle2));
    w.writeBytes(mNet_WalkTargetX,       sizeof(mNet_WalkTargetX));
    w.writeBytes(mNet_WalkTargetY,       sizeof(mNet_WalkTargetY));
    w.writeBytes(mNet_CameraPanTargetX, sizeof(mNet_CameraPanTargetX));
    w.writeBytes(mNet_CameraPanTargetY, sizeof(mNet_CameraPanTargetY));
    w.writeBytes(mNetSquadCursorX,      sizeof(mNetSquadCursorX));
    w.writeBytes(mNetSquadCursorY,      sizeof(mNetSquadCursorY));
    w.write(mSquad_Selected);
    w.write(mSquad_JoiningTo);
    w.write(mSquad_WalkTargetX);
    w.write(mSquad_WalkTargetY);
    w.write(mSquad_SwitchWeapon);
    w.write(mSquad_Grenade_SplitMode);
    w.write(mSquad_Rocket_SplitMode);
    w.write(mSquad_Member_Fire_CoolDown);
    w.write(mSquad_Member_Fire_CoolDown_Override);
    w.writeBytes(mSquad_EnteredVehicleTimer, sizeof(mSquad_EnteredVehicleTimer));
    w.write(mSquad_Member_Clicked_TroopInSameSquad);
    // mSquad_Member_Clicked_TroopPtr is a raw pointer into mSoldiers_Allocated –
    // stable, so save address directly.
    w.write(reinterpret_cast<uintptr_t>(mSquad_Member_Clicked_TroopPtr));

    // --- Sprite pointer fields in cFodder (raw addresses, stable) ---
    w.write(reinterpret_cast<uintptr_t>(mSquad_Leader));
    w.writeBytes(mMission_Troops_SpritePtrs, sizeof(mMission_Troops_SpritePtrs));
    w.writeBytes(mSquad_CurrentVehicles,     sizeof(mSquad_CurrentVehicles));
    w.write(reinterpret_cast<uintptr_t>(mSquad_CurrentVehicle));
    w.write(reinterpret_cast<uintptr_t>(mHostage_Rescue_Tent));
    w.write(reinterpret_cast<uintptr_t>(mSprite_OpenCloseDoor_Ptr));
    w.write(reinterpret_cast<uintptr_t>(mTroop_InRange_Callpad));
    w.writeBytes(mSprite_TroopsAlive, sizeof(mSprite_TroopsAlive));

    // --- Enemy / mission counts ---
    w.write(mEnemy_BuildingCount);
    w.write(mTroops_Enemy_Count);
    w.write(mHostage_Count);
    w.write(mMission_EngineTicks);
    w.write(mMission_Final_TimeToDie_Ticker);
    w.write(mMission_Final_TimeRemain);
    w.write(mMission_Final_TimeToAbort);

    // --- Sprite animation frame counters ---
    w.write(mSprite_Frame_1);
    w.write(mSprite_Frame_Modifier_Update_Countdown);
    w.write(mSprite_Frame_2);
    w.write(mSprite_Frame3_ChangeCount);
    w.write(mSprite_Frame_3);
    w.write(mSprite_Frame1_Modifier);
    w.write(mSprite_Frame2_Modifier);
    w.write(mSprite_Frame_3_Modifier);
    w.write(mSprite_SpareUsed);
    w.write(mSprite_SpareUsed2);

    // --- Projectile counters ---
    w.writeBytes(mSprite_Projectile_Counters,        sizeof(mSprite_Projectile_Counters));
    w.writeBytes(mSprite_Missile_Projectile_Counters, sizeof(mSprite_Missile_Projectile_Counters));

    // --- Misc simulation vars ---
    w.write(mTroop_Rotate_Next);
    w.write(mSprite_Bullet_Destroy);
    w.write(mSprite_Helicopter_DestroyLight);
    w.write(mSprite_DistanceTo_Squad0);
    w.write(mHelicopterCallPadCount);
    w.write(mHelicopterCallPadPressedCount);
    w.write(mHelicopterCall_X);
    w.write(mHelicopterCall_Y);
    w.write(mSwitchesActivated);
    w.write(mTurretFires_HomingMissile);
    w.write(dword_39F36);
    w.write(dword_3A030);
    w.write(dword_3B1CB);
    w.write(word_3A9B8);
    w.write(mSprite_HumanVehicles_Found);
    w.write(mSoundEffectToPlay);
    w.write(mSoundEffectToPlay_Set);
    w.writeBytes(mGUI_Squad_Icon, sizeof(mGUI_Squad_Icon));
    w.writeBytes(mGUI_RefreshSquadGrenades, sizeof(mGUI_RefreshSquadGrenades));
    w.writeBytes(mGUI_RefreshSquadRockets,  sizeof(mGUI_RefreshSquadRockets));
    w.writeBytes(mMission_Save_Blocked,     sizeof(mMission_Save_Blocked));

    // --- Additional simulation state (sprite handler globals that gate RNG) ---
    w.write(mSprite_FaceWeaponTarget);
    w.write(word_3AA45);
    w.write(dword_3B24B);
    w.write(word_3B25B);
    w.write(word_3B2ED);
    w.write(word_3B2F3);
    w.write(word_3ABB1);
    w.writeBytes(word_3B2D1, sizeof(word_3B2D1));
    w.write(mMouse_Locked);
    w.write(word_3A05F);
    w.write(word_3A3BF);
    w.write(word_3AA1D);
    w.write(word_3AA21);
    w.writeBytes(word_3AC2D, sizeof(word_3AC2D));
    w.write(word_3AC4B);
    w.write(word_3AC4D);
    w.write(word_3AC51);
    w.write(word_3B25D);
    w.write(word_3B2CF);
    w.write(word_3B2F1);
    w.write(word_3B2F7);
    w.write(word_3B481);
    w.write(word_3B483);
    w.writeBytes(word_3B4ED, sizeof(word_3B4ED));
    w.writeBytes(word_3BED5, sizeof(word_3BED5));
    w.write(mSprite_Bumped_Into_SquadMember);
    w.write(mSprite_Player_CheckWeapon);
    w.write(mSprite_Reached_Target);
    w.write(mStoredSpriteFrame);
    w.write(mDirectionMod);
    w.write(mSprite_Civilian_GotHome);
    w.write(mSprite_Civilian_Tmp_X);
    w.write(mSprite_Civilian_Tmp_Y);
    w.write(mSpawnSpriteType);
    w.write(mSprite_Field10_Saved);
    w.write(mSprite_Bullet_Time_Modifier);
    w.write(mSprite_Bullet_Fire_Speed_Modifier);
    w.write(mSprite_Bullet_Deviate_Counter);
    w.write(mStoredSpriteX);
    w.write(mStoredSpriteY);
    w.write(mTmp_FrameNumber);
    w.writeBytes(mSprite_Find_Types, sizeof(mSprite_Find_Types));
    w.write(mSprite_Find_Distance);
    w.writeBytes(&mCheckPattern_Position, sizeof(sMapPosition));
    w.writeBytes(&mSprite_Weapon_Data, sizeof(sWeaponData));
    w.write(mGUI_Sidebar_Setup);
    w.write(mGUI_Sidebar_MapButton_Prepared);
    w.write(mMouseDisabled);
    w.write(mTroop_Weapon_Grenade_Disabled);
    w.write(mTroop_Weapon_Bullet_Disabled);
    w.write(mTroop_Weapon_Rocket_Disabled);

    // --- Map tile destruction queue ---
    {
        auto count = (uint32_t)mMap_Destroy_Tiles.size();
        w.write(count);
        if (count > 0)
            w.writeBytes(mMap_Destroy_Tiles.data(),
                         count * sizeof(sMapPosition));
    }
    w.write(mMap_Destroy_Tile_X);
    w.write(mMap_Destroy_Tile_Y);
    w.write(mMap_Destroy_Tile_LastTile);
    w.write(mMap_Destroy_Tiles_Countdown);

    // --- Tile track animations ---
    {
        auto count = (uint32_t)mMapTileTracks.size();
        w.write(count);
        if (count > 0)
            w.writeBytes(mMapTileTracks.data(),
                         count * sizeof(sTileTrack));
    }

    // --- mSprites_HumanVehicles (raw pointers, stable) ---
    {
        auto count = (uint32_t)mSprites_HumanVehicles.size();
        w.write(count);
        if (count > 0)
            w.writeBytes(mSprites_HumanVehicles.data(),
                         count * sizeof(sSprite*));
    }

    // --- Game data (scores, goals, troop data) ---
    w.write(mGame_Data.mScore_Kills_Away);
    w.write(mGame_Data.mScore_Kills_Home);
    w.writeBytes(mGame_Data.mGamePhase_Data.mGoals_Remaining,
                 sizeof(mGame_Data.mGamePhase_Data.mGoals_Remaining));
    w.write(mGame_Data.mGamePhase_Data.mIsComplete);
    w.write(mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage);
    w.write(mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin);
    w.write(mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax);
    w.write(mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext);
    w.write(mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement);
    w.write(mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionCreated_Count);
    w.write(mGame_Data.mGamePhase_Data.mSoldiers_Required);
    w.write(mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count);
    w.write(mGame_Data.mGamePhase_Data.mSoldiers_Available);
    w.write(mGame_Data.mGamePhase_Data.mSoldiers_Prepare_SetFromSpritePtrs);

    // Troop data (mSprite pointer inside is stable; save raw)
    w.writeBytes(mGame_Data.mSoldiers_Allocated,
                 sizeof(mGame_Data.mSoldiers_Allocated));

    // --- Sprite array ---
    {
        auto count = (uint32_t)mSprites.size();
        w.write(count);
        // Raw memcpy: all intra-vector pointers remain valid after restore
        w.writeBytes(mSprites.data(), count * sizeof(sSprite));
    }

    // --- Map tile data (destructible tiles) ---
    if (mMap && mMap->size() > 0) {
        auto mapSz = (uint32_t)mMap->size();
        w.write(mapSz);
        w.writeBytes(mMap->data(), mapSz);
    } else {
        w.write((uint32_t)0);
    }

    // --- Transfer buffer ownership to GGPO ---
    *len    = (int)w.buf.size();
    *buffer = static_cast<uint8_t*>(malloc(*len));
    if (!*buffer) return false;
    memcpy(*buffer, w.buf.data(), *len);

    // Simple checksum (sum of all bytes)
    uint32_t csum = 0;
    for (int i = 0; i < *len; ++i)
        csum += (*buffer)[i];
    *checksum = (int)csum;

    return true;
}

// ============================================================
// Network_LoadState
// Restore simulation state from a GGPO-managed buffer.
// ============================================================

bool cFodder::Network_LoadState(const uint8_t* buffer, int len) {
    sReader r(buffer, (size_t)len);

    // --- Header ---
    const uint32_t MAGIC = 0x464F4444u;
    auto magic  = r.read<uint32_t>();
    auto frame  = r.read<uint32_t>();
    if (magic != MAGIC) {
        fprintf(stderr, "[GGPO] LoadState: bad magic %08X\n", magic);
        return false;
    }
    mNetFrameCount = (int)frame;

    // --- RNG ---
    {
        int16 s0 = r.read<int16>(), s1 = r.read<int16>();
        int16 s2 = r.read<int16>(), s3 = r.read<int16>();
        mRandom.setSeed(s0, s1, s2, s3);
        mRandomCallCount = r.read<uint32>();
    }

    // --- Tick counters ---
    mInterruptTick         = r.read<int16>();
    mPhase_InterruptTicks  = r.read<uint16>();
    mMission_EngineTicks   = r.read<int16>();
    mPhase_Completed_Timer = r.read<int16>();

    // --- Phase flags ---
    mPhase_Complete       = r.read<bool>();
    mPhase_Aborted        = r.read<bool>();
    mPhase_TryAgain       = r.read<bool>();
    mPhase_Finished       = r.read<bool>();
    mPhase_In_Progress    = (volatile bool)r.read<bool>();
    mPhase_Paused         = r.read<bool>();
    mPhase_ShowMapOverview= (volatile int16)r.read<int16>();
    mMapTile_Music_Play   = r.read<bool>();
    mPhase_Aborted2       = r.read<bool>();
    mPhase_EscapeKeyAbort = r.read<bool>();
    mInput_Enabled        = r.read<bool>();

    // --- Camera ---
    mCameraX                          = r.read<int32>();
    mCameraY                          = r.read<int32>();
    mMapTile_Ptr                      = r.read<int32>();
    mMapTile_ColumnOffset             = r.read<int16>();
    mMapTile_RowOffset                = r.read<int16>();
    mMapTile_DrawX                    = r.read<int16>();
    mMapTile_DrawY                    = r.read<int16>();
    mMapTile_SpeedX                   = r.read<int32>();
    mMapTile_SpeedY                   = r.read<int32>();
    mMapTile_TargetX                  = r.read<int32>();
    mMapTile_TargetY                  = r.read<int32>();
    mMapTile_MoveDirectionX           = r.read<int16>();
    mMapTile_MoveDirectionY           = r.read<int16>();
    mMapTile_MoveDirectionX_Previous  = r.read<int16>();
    mMapTile_MoveDirectionY_Previous  = r.read<int16>();
    mMapTile_SpeedX_Previous          = r.read<int16>();
    mMapTile_SpeedY_Previous          = r.read<int16>();
    mMapTile_MovedHorizontal          = r.read<int16>();
    mMapTile_MovedVertical            = r.read<int16>();
    mMapTile_Column_CurrentScreen     = r.read<int16>();
    mMapTile_Row_CurrentScreen        = r.read<int16>();
    mCamera_AccelerationX             = r.read<int32>();
    mCamera_AccelerationY             = r.read<int32>();
    mCamera_MoveDirectionX            = r.read<int16>();
    mCamera_MoveDirectionY            = r.read<int16>();
    mCamera_MovePauseX                = r.read<int16>();
    mCamera_MovePauseY                = r.read<int16>();
    mCamera_PanTargetX                = r.read<int16>();
    mCamera_PanTargetY                = r.read<int16>();
    mCamera_Scroll_Speed              = r.read<int16>();
    mCamera_Speed_Reset_X             = r.read<bool>();
    mCamera_Speed_Reset_Y             = r.read<bool>();
    mCamera_Speed_X                   = r.read<int32>();
    mCamera_Speed_Y                   = r.read<int32>();
    mCamera_StartPosition_X           = r.read<int16>();
    mCamera_StartPosition_Y           = r.read<int16>();
    mCamera_SquadLeaderX              = r.read<int16>();
    mCamera_SquadLeaderY              = r.read<int16>();
    mCamera_TileSpeed_Overflow        = r.read<int32>();
    mCamera_TileX                     = r.read<int16>();
    mCamera_TileY                     = r.read<int16>();
    mCamera_TileSpeedX                = r.read<int32>();
    mCamera_TileSpeedY                = r.read<int32>();
    mCamera_Speed_Max                 = r.read<int32>();
    mCamera_Start_Adjust              = r.read<bool>();
    mCamera_Reached_Target            = r.read<int16>();
    mCamera_Panning_ToTarget          = r.read<int16>();

    // --- Squad state ---
    r.readBytes(mSquad_Grenades,            sizeof(mSquad_Grenades));
    r.readBytes(mSquad_Rockets,             sizeof(mSquad_Rockets));
    r.readBytes(mSquad_CurrentWeapon,       sizeof(mSquad_CurrentWeapon));
    r.readBytes(mSquads_TroopCount,         sizeof(mSquads_TroopCount));
    r.readBytes(mSquad_Walk_Target_Indexes, sizeof(mSquad_Walk_Target_Indexes));
    r.readBytes(mSquad_Walk_Target_Steps,   sizeof(mSquad_Walk_Target_Steps));
    r.readBytes(mSquad_WalkTargets,          sizeof(mSquad_WalkTargets));
    r.readBytes(mSquad_Join_TargetSquad,     sizeof(mSquad_Join_TargetSquad));
    for (int i = 0; i < 6; ++i)
        mSquad_Join_TargetSprite[i] = reinterpret_cast<sSprite*>(r.read<uintptr_t>());
    mSquad_Select_Timer               = r.read<int16>();
    r.readBytes(mNet_ButtonLeftToggle,  sizeof(mNet_ButtonLeftToggle));
    r.readBytes(mNet_ButtonRightToggle, sizeof(mNet_ButtonRightToggle));
    r.readBytes(mNet_ButtonLRToggle,    sizeof(mNet_ButtonLRToggle));
    r.readBytes(mNet_ButtonLRToggle2,   sizeof(mNet_ButtonLRToggle2));
    r.readBytes(mNet_WalkTargetX,       sizeof(mNet_WalkTargetX));
    r.readBytes(mNet_WalkTargetY,       sizeof(mNet_WalkTargetY));
    r.readBytes(mNet_CameraPanTargetX, sizeof(mNet_CameraPanTargetX));
    r.readBytes(mNet_CameraPanTargetY, sizeof(mNet_CameraPanTargetY));
    r.readBytes(mNetSquadCursorX,      sizeof(mNetSquadCursorX));
    r.readBytes(mNetSquadCursorY,      sizeof(mNetSquadCursorY));
    mSquad_Selected                   = r.read<int16>();
    mSquad_JoiningTo                  = r.read<int16>();
    mSquad_WalkTargetX                = r.read<int16>();
    mSquad_WalkTargetY                = r.read<int16>();
    mSquad_SwitchWeapon               = r.read<int16>();
    mSquad_Grenade_SplitMode          = r.read<uint16>();
    mSquad_Rocket_SplitMode           = r.read<uint16>();
    mSquad_Member_Fire_CoolDown       = r.read<int16>();
    mSquad_Member_Fire_CoolDown_Override = r.read<bool>();
    r.readBytes(mSquad_EnteredVehicleTimer, sizeof(mSquad_EnteredVehicleTimer));
    mSquad_Member_Clicked_TroopInSameSquad = r.read<int16>();
    mSquad_Member_Clicked_TroopPtr =
        reinterpret_cast<sMission_Troop*>(r.read<uintptr_t>());

    // --- cFodder sprite pointers ---
    mSquad_Leader = reinterpret_cast<sSprite*>(r.read<uintptr_t>());
    r.readBytes(mMission_Troops_SpritePtrs, sizeof(mMission_Troops_SpritePtrs));
    r.readBytes(mSquad_CurrentVehicles,     sizeof(mSquad_CurrentVehicles));
    mSquad_CurrentVehicle =
        reinterpret_cast<sSprite*>(r.read<uintptr_t>());
    mHostage_Rescue_Tent =
        reinterpret_cast<sSprite*>(r.read<uintptr_t>());
    mSprite_OpenCloseDoor_Ptr =
        reinterpret_cast<sSprite*>(r.read<uintptr_t>());
    mTroop_InRange_Callpad =
        reinterpret_cast<sSprite*>(r.read<uintptr_t>());
    r.readBytes(mSprite_TroopsAlive, sizeof(mSprite_TroopsAlive));

    // --- Counts ---
    mEnemy_BuildingCount              = r.read<int16>();
    mTroops_Enemy_Count               = r.read<size_t>();
    mHostage_Count                    = r.read<int16>();
    mMission_EngineTicks              = r.read<int16>();
    mMission_Final_TimeToDie_Ticker   = r.read<int16>();
    mMission_Final_TimeRemain         = r.read<int16>();
    mMission_Final_TimeToAbort        = r.read<int16>();

    // --- Animation frame counters ---
    mSprite_Frame_1                        = r.read<int16>();
    mSprite_Frame_Modifier_Update_Countdown= r.read<int16>();
    mSprite_Frame_2                        = r.read<int16>();
    mSprite_Frame3_ChangeCount             = r.read<int16>();
    mSprite_Frame_3                        = r.read<int16>();
    mSprite_Frame1_Modifier                = r.read<int16>();
    mSprite_Frame2_Modifier                = r.read<int16>();
    mSprite_Frame_3_Modifier               = r.read<int16>();
    mSprite_SpareUsed                      = r.read<int16>();
    mSprite_SpareUsed2                     = r.read<int16>();

    // --- Projectile counters ---
    r.readBytes(mSprite_Projectile_Counters,         sizeof(mSprite_Projectile_Counters));
    r.readBytes(mSprite_Missile_Projectile_Counters,  sizeof(mSprite_Missile_Projectile_Counters));

    // --- Misc ---
    mTroop_Rotate_Next             = r.read<size_t>();
    mSprite_Bullet_Destroy         = r.read<int16>();
    mSprite_Helicopter_DestroyLight= r.read<int16>();
    mSprite_DistanceTo_Squad0      = r.read<int16>();
    mHelicopterCallPadCount        = r.read<int16>();
    mHelicopterCallPadPressedCount = r.read<int16>();
    mHelicopterCall_X              = r.read<int16>();
    mHelicopterCall_Y              = r.read<int16>();
    mSwitchesActivated             = r.read<bool>();
    mTurretFires_HomingMissile     = r.read<int16>();
    dword_39F36                    = r.read<int32>();
    dword_3A030                    = r.read<int32>();
    dword_3B1CB                    = r.read<int32>();
    word_3A9B8                     = r.read<int16>();
    mSprite_HumanVehicles_Found    = r.read<bool>();
    mSoundEffectToPlay             = r.read<int16>();
    mSoundEffectToPlay_Set         = r.read<int16>();
    r.readBytes(mGUI_Squad_Icon,          sizeof(mGUI_Squad_Icon));
    r.readBytes(mGUI_RefreshSquadGrenades, sizeof(mGUI_RefreshSquadGrenades));
    r.readBytes(mGUI_RefreshSquadRockets,  sizeof(mGUI_RefreshSquadRockets));
    r.readBytes(mMission_Save_Blocked,     sizeof(mMission_Save_Blocked));

    // --- Additional simulation state (sprite handler globals that gate RNG) ---
    mSprite_FaceWeaponTarget       = r.read<int16>();
    word_3AA45                     = r.read<int16>();
    dword_3B24B                    = r.read<int16>();
    word_3B25B                     = r.read<int16>();
    word_3B2ED                     = r.read<int16>();
    word_3B2F3                     = r.read<int16>();
    word_3ABB1                     = r.read<int16>();
    r.readBytes(word_3B2D1, sizeof(word_3B2D1));
    mMouse_Locked                  = r.read<bool>();
    word_3A05F                     = r.read<uint16>();
    word_3A3BF                     = r.read<int16>();
    word_3AA1D                     = r.read<int16>();
    word_3AA21                     = r.read<int16>();
    r.readBytes(word_3AC2D, sizeof(word_3AC2D));
    word_3AC4B                     = r.read<int16>();
    word_3AC4D                     = r.read<int16>();
    word_3AC51                     = r.read<int16>();
    word_3B25D                     = r.read<int16>();
    word_3B2CF                     = r.read<int16>();
    word_3B2F1                     = r.read<int16>();
    word_3B2F7                     = r.read<int16>();
    word_3B481                     = r.read<int16>();
    word_3B483                     = r.read<int16>();
    r.readBytes(word_3B4ED, sizeof(word_3B4ED));
    r.readBytes(word_3BED5, sizeof(word_3BED5));
    mSprite_Bumped_Into_SquadMember= r.read<int16>();
    mSprite_Player_CheckWeapon     = r.read<int16>();
    mSprite_Reached_Target         = r.read<int16>();
    mStoredSpriteFrame             = r.read<int16>();
    mDirectionMod                  = r.read<int16>();
    mSprite_Civilian_GotHome       = r.read<int16>();
    mSprite_Civilian_Tmp_X         = r.read<int16>();
    mSprite_Civilian_Tmp_Y         = r.read<int16>();
    mSpawnSpriteType               = r.read<int16>();
    mSprite_Field10_Saved          = r.read<int16>();
    mSprite_Bullet_Time_Modifier   = r.read<int16>();
    mSprite_Bullet_Fire_Speed_Modifier = r.read<int16>();
    mSprite_Bullet_Deviate_Counter = r.read<int16>();
    mStoredSpriteX                 = r.read<int32>();
    mStoredSpriteY                 = r.read<int32>();
    mTmp_FrameNumber               = r.read<int16>();
    r.readBytes(mSprite_Find_Types, sizeof(mSprite_Find_Types));
    mSprite_Find_Distance          = r.read<int16>();
    r.readBytes(&mCheckPattern_Position, sizeof(sMapPosition));
    r.readBytes(&mSprite_Weapon_Data, sizeof(sWeaponData));
    mGUI_Sidebar_Setup             = r.read<int16>();
    mGUI_Sidebar_MapButton_Prepared= r.read<int16>();
    mMouseDisabled                 = r.read<int16>();
    mTroop_Weapon_Grenade_Disabled = r.read<bool>();
    mTroop_Weapon_Bullet_Disabled  = r.read<bool>();
    mTroop_Weapon_Rocket_Disabled  = r.read<bool>();

    // --- Tile destruction queue ---
    {
        auto count = r.read<uint32_t>();
        mMap_Destroy_Tiles.resize(count);
        if (count > 0)
            r.readBytes(mMap_Destroy_Tiles.data(),
                        count * sizeof(sMapPosition));
    }
    mMap_Destroy_Tile_X        = r.read<int16>();
    mMap_Destroy_Tile_Y        = r.read<int16>();
    mMap_Destroy_Tile_LastTile = r.read<int16>();
    mMap_Destroy_Tiles_Countdown = r.read<int16>();

    // --- Tile tracks ---
    {
        auto count = r.read<uint32_t>();
        mMapTileTracks.resize(count);
        if (count > 0)
            r.readBytes(mMapTileTracks.data(),
                        count * sizeof(sTileTrack));
    }

    // --- Human vehicles list ---
    {
        auto count = r.read<uint32_t>();
        mSprites_HumanVehicles.resize(count);
        if (count > 0)
            r.readBytes(mSprites_HumanVehicles.data(),
                        count * sizeof(sSprite*));
    }

    // --- Game data ---
    mGame_Data.mScore_Kills_Away = r.read<int16>();
    mGame_Data.mScore_Kills_Home = r.read<int16>();
    r.readBytes(mGame_Data.mGamePhase_Data.mGoals_Remaining,
                sizeof(mGame_Data.mGamePhase_Data.mGoals_Remaining));
    mGame_Data.mGamePhase_Data.mIsComplete              = r.read<bool>();
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionAverage   = r.read<int16>();
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMin        = r.read<int16>();
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionMax        = r.read<int16>();
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionNext       = r.read<int16>();
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionIncrement  = r.read<int16>();
    mGame_Data.mGamePhase_Data.mSprite_Enemy_AggressionCreated_Count = r.read<int16>();
    mGame_Data.mGamePhase_Data.mSoldiers_Required       = r.read<int16>();
    mGame_Data.mGamePhase_Data.mSoldiers_Allocated_Count= r.read<int16>();
    mGame_Data.mGamePhase_Data.mSoldiers_Available      = r.read<int16>();
    mGame_Data.mGamePhase_Data.mSoldiers_Prepare_SetFromSpritePtrs = r.read<bool>();

    r.readBytes(mGame_Data.mSoldiers_Allocated,
                sizeof(mGame_Data.mSoldiers_Allocated));

    // --- Sprites ---
    {
        auto count = r.read<uint32_t>();
        // The vector must already be the right size (set at phase prepare time)
        if (count != mSprites.size()) {
            fprintf(stderr, "[GGPO] LoadState: sprite count mismatch %u vs %zu\n",
                    count, mSprites.size());
            return false;
        }
        r.readBytes(mSprites.data(), count * sizeof(sSprite));
    }

    // --- Map ---
    {
        auto mapSz = r.read<uint32_t>();
        if (mapSz > 0 && mMap && mMap->size() == mapSz) {
            r.readBytes(mMap->data(), mapSz);
        } else if (mapSz > 0) {
            // Size mismatch; skip bytes to keep reader aligned
            r.pos += mapSz;
        }
    }

    if (!r.ok()) {
        fprintf(stderr, "[GGPO] LoadState: reader overflowed (%zu > %d)\n",
                r.pos, len);
        return false;
    }

    // Rebuild squad membership arrays (mSquad_0_Sprites etc.) from the
    // restored sprite field_32 values.  These arrays are not serialised
    // because they are fully derivable from sprite state, but they must be
    // current before the next Mission_Sprites_Handle() call.
    Squad_Rebuild();

    // Invalidate the sprite draw lists so they are rebuilt next render
    mSprite_DrawList_First.clear();
    mSprite_DrawList_Second.clear();
    mSprite_DrawList_Third.clear();
    mSprite_DrawList_Final.clear();

    return true;
}

// ============================================================
// Network_FreeBuffer
// ============================================================

void cFodder::Network_FreeBuffer(void* buffer) {
    free(buffer);
}

#endif // OPENFODDER_ENABLE_NETWORK
