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

class cAbout {

    cSurface*    mSurface;

public:
        cAbout();
        ~cAbout();

        bool Cycle();

        // UPDATE button click handler. Invoked from cFodder::GUI_Button_About_Update,
        // which is the trampoline registered with GUI_Button_Setup (the GUI dispatch
        // requires a void(cFodder::*)() member-function pointer, so this is the
        // closest we can get to "method on the About menu class").
        //
        // Non-blocking: kicks Setup::UpdateChecker (a thread-pumped state machine
        // drained from cAbout::Cycle each frame). The window stays responsive
        // while the GitHub query and download+extract run on a worker thread.
        static void OnUpdateClicked();
};
