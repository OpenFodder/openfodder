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

#ifdef __EMSCRIPTEN__
#include "stdafx.hpp"

int startGame(int argc, char* argv[]);

namespace {
std::vector<std::string> browserArguments;
std::vector<char*> browserArgumentPointers;

void RunBrowserGame() {
    // Asyncify resumes the foreground loop before rewinding a sleep. Only
    // the first callback starts the game; later callbacks must not re-enter it.
    static bool started = false;
    if (started) return;
    started = true;
    startGame(static_cast<int>(browserArguments.size()), browserArgumentPointers.data());
    emscripten_cancel_main_loop();
}
}

int start(int argc, char* argv[]) {
    // The original main stack is unwound by set_main_loop. Own the command
    // line before it becomes invalid and launch the normal game on a callback.
    for (int i = 0; i < argc; ++i) browserArguments.emplace_back(argv[i]);
    for (auto& argument : browserArguments) browserArgumentPointers.push_back(argument.data());
    browserArgumentPointers.push_back(nullptr);
    emscripten_set_main_loop(RunBrowserGame, 0, true);
    return 0;
}
#endif
