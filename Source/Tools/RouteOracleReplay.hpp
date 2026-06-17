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

#include <string>

// MapGen v3 Phase 1 P1.5c — headless engine route/walkable oracle.
//
// Loads a .map file and the matching base/sub HIT/BHT tile-collision tables
// (the same tables the engine consults at runtime via Map_Terrain_Get) and
// answers a JSON query batch with `walkable`, `terrain`, and
// `route_endtoend` results. Output JSON is written to pOutputPath, or to
// "<pInputPath>.result.json" if pOutputPath is empty.
//
// route_endtoend uses a BFS over engine-walkable cells; this is the D10 row
// 3 reachability oracle described in the v3 plan and is intentionally not a
// match for the in-engine player AI pathfinder.
//
// Returns true on success (input parsed, all queries answered, output
// written), false on any fatal error (input not found, malformed JSON,
// missing tilesets, output write failure). Errors are logged via
// g_Debugger->Error so they show up the same way the PNG path does.
namespace RouteOracleReplay {

	bool Run(const std::string& pInputPath, const std::string& pOutputPath);

}
