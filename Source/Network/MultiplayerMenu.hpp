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

#ifdef OPENFODDER_ENABLE_NETWORK

class cMultiplayerMenu {
public:
    cMultiplayerMenu();
    ~cMultiplayerMenu();

    void Open();
    void Close();
    void Draw();
    void Tick();

    void OnRowClick(int16 pAction, int16 pArg);
    void OnBack();

    // Static trampoline for GUI_Button_Setup_New
    static void OnButtonClick(void* ctx, int16 action, int16 arg) {
        static_cast<cMultiplayerMenu*>(ctx)->OnRowClick(action, arg);
    }

    bool IsDone() const { return mDone; }
    bool WasStarted() const { return mStarted; }

    // Results — used by cFodderMultiplayer to configure mStartParams
    int         GetPlayerIndex() const { return mPlayerIndex; }
    std::string GetRemoteHost() const { return mRemoteHost; }
    uint16      GetRemotePort() const { return mRemotePort; }
    uint16      GetLocalPort() const { return mLocalPort; }
    bool        IsSyncTest() const { return mSyncTest; }

    enum eAction : int16 {
        ACT_NONE = 0,
        ACT_HOST,
        ACT_JOIN,
        ACT_SYNC_TEST,
        ACT_BACK,
        ACT_ROW,
        ACT_IP_INPUT,
        ACT_PORT_INPUT,
        ACT_START,
    };

private:
    void DrawMainMenu();
    void DrawHostMenu();
    void DrawJoinMenu();

    enum class eState {
        Main,
        Host,
        Join,
    };

    eState      mState = eState::Main;
    bool        mDone = false;
    bool        mStarted = false;

    int         mPlayerIndex = 0;   // 0 = host/P1, 1 = join/P2
    std::string mRemoteHost = "127.0.0.1";
    uint16      mRemotePort = 7001;
    uint16      mLocalPort  = 7000;
    bool        mSyncTest = false;

    // Keep drawn strings alive for GUI draw calls
    std::vector<std::string> mDrawStrings;
};

#endif // OPENFODDER_ENABLE_NETWORK
