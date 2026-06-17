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

#include "stdafx.hpp"
#include "Amiga/dernc.hpp"
#include "Tools/RouteOracleReplay.hpp"
#include "Utils/json.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <deque>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using Json = nlohmann::json;

namespace {

// ---------------------------------------------------------------------------
// Path / file helpers (mirrors MapPng.cpp; the PNG dispatch is the canonical
// pattern for headless map-tooling that can't pull in mResources because we
// haven't run cFodder::Prepare). We deliberately don't share code with
// MapPng.cpp — those helpers are file-static there, and the route-oracle is
// a self-contained replay tool that should not be coupled to the renderer.
// ---------------------------------------------------------------------------

bool RO_FileExists(const std::string& pPath) {
	std::ifstream File(pPath.c_str(), std::ios::binary);
	return File.is_open();
}

size_t RO_ExtensionPos(const std::string& pPath) {
	const size_t Slash = pPath.find_last_of("/\\");
	const size_t Dot = pPath.find_last_of('.');

	if (Dot == std::string::npos)
		return std::string::npos;
	if (Slash != std::string::npos && Dot < Slash)
		return std::string::npos;
	return Dot;
}

std::string RO_DirName(const std::string& pPath) {
	const size_t Slash = pPath.find_last_of("/\\");
	if (Slash == std::string::npos)
		return ".";
	return pPath.substr(0, Slash);
}

std::string RO_JoinPath(const std::string& pDir, const std::string& pName) {
	if (!pDir.size() || pDir == ".")
		return pName;
	const char Last = pDir[pDir.size() - 1];
	if (Last == '/' || Last == '\\')
		return pDir + pName;
	return pDir + "/" + pName;
}

std::string RO_ToLower(std::string pValue) {
	std::transform(pValue.begin(), pValue.end(), pValue.begin(), [](unsigned char c) {
		return (char)std::tolower(c);
	});
	return pValue;
}

std::string RO_ToUpper(std::string pValue) {
	std::transform(pValue.begin(), pValue.end(), pValue.begin(), [](unsigned char c) {
		return (char)std::toupper(c);
	});
	return pValue;
}

std::string RO_ResolveMapPath(const std::string& pPath) {
	if (RO_FileExists(pPath))
		return pPath;

	if (RO_ExtensionPos(pPath) == std::string::npos) {
		const std::string WithMap = pPath + ".map";
		if (RO_FileExists(WithMap))
			return WithMap;
	}

	return pPath;
}

tSharedBuffer RO_ReadFileRaw(const std::string& pPath) {
	auto Buffer = std::make_shared<std::vector<uint8>>();
	std::ifstream File(pPath.c_str(), std::ios::binary);
	if (!File.is_open())
		return Buffer;

	File.seekg(0, std::ios::end);
	const std::streamoff Size = File.tellg();
	File.seekg(0, std::ios::beg);
	if (Size <= 0)
		return Buffer;

	Buffer->resize((size_t)Size);
	File.read((char*)Buffer->data(), Size);
	if (!File.good() && !File.eof())
		Buffer->clear();

	return Buffer;
}

tSharedBuffer RO_DeRnc(tSharedBuffer pBuffer) {
	if (!pBuffer || pBuffer->size() < 8)
		return pBuffer;

	if (readBEDWord(pBuffer->data()) != 0x524E4301)
		return pBuffer;

	const uint32 Size = readBEDWord(pBuffer->data() + 4);
	auto Unpacked = std::make_shared<std::vector<uint8>>();
	Unpacked->resize(Size);

	if (rnc_unpack(pBuffer->data(), Unpacked->data()) < 0)
		Unpacked->clear();

	return Unpacked;
}

tSharedBuffer RO_ReadFileMaybeRnc(const std::string& pPath) {
	if (!RO_FileExists(pPath))
		return std::make_shared<std::vector<uint8>>();
	return RO_DeRnc(RO_ReadFileRaw(pPath));
}

std::string RO_ReadAsciiAt(tSharedBuffer pBuffer, size_t pOffset, size_t pLength) {
	if (!pBuffer || pOffset >= pBuffer->size())
		return "";

	size_t End = pOffset;
	const size_t Stop = std::min<size_t>(pBuffer->size(), pOffset + pLength);
	while (End < Stop && (*pBuffer)[End])
		++End;

	return std::string((const char*)pBuffer->data() + pOffset, End - pOffset);
}

std::string RO_FindDataFile(const std::string& pMapPath, const std::string& pName) {
	const std::string MapDir = RO_DirName(pMapPath);
	const std::string Lower = RO_ToLower(pName);
	const std::string Upper = RO_ToUpper(pName);
	std::vector<std::string> Dirs;
	Dirs.push_back(MapDir);
	Dirs.push_back(RO_JoinPath(MapDir, "Data/Amiga"));
	Dirs.push_back("Run/Data/Amiga");
	Dirs.push_back("Data/Amiga");

	for (size_t dirIndex = 0; dirIndex < Dirs.size(); ++dirIndex) {
		const std::string& Dir = Dirs[dirIndex];
		const std::string Direct = RO_JoinPath(Dir, pName);
		if (RO_FileExists(Direct))
			return Direct;
		const std::string LowerPath = RO_JoinPath(Dir, Lower);
		if (RO_FileExists(LowerPath))
			return LowerPath;
		const std::string UpperPath = RO_JoinPath(Dir, Upper);
		if (RO_FileExists(UpperPath))
			return UpperPath;
	}

	return "";
}

// ---------------------------------------------------------------------------
// In-memory map state. We mirror the runtime's tile collision tables so the
// terrain lookup matches Source/Map/MapRuntime.cpp byte-for-byte (HIT[512]
// int16, BHT[512][8]; base half goes into [0..240], sub half into [240..]).
// Endianness mirrors what cFodder::Map_Load_Resources does after fileLoadTo.
// ---------------------------------------------------------------------------

struct LoadedMap {
	std::vector<uint8>  MapData;            // .map bytes; tile data at +0x60 has been LE-swapped
	uint16              Width = 0;
	uint16              Height = 0;
	std::vector<int16>  TileHit;            // 512 entries
	std::vector<int8>   TileBHit;           // 512 * 8 bytes (row-major: tile * 8 + row)
};

bool RO_LoadHitTables(const std::string& pMapPath,
	const std::string& pBaseName,
	const std::string& pSubName,
	std::vector<int16>& pTileHit,
	std::vector<int8>& pTileBHit) {

	pTileHit.assign(512, 0);
	pTileBHit.assign(512 * 8, 0);

	auto LoadHitInto = [&](const std::string& pBlkName, size_t pSlot) -> bool {
		// "junbase.blk" -> "junbase.hit"
		std::string HitName = pBlkName;
		const size_t Dot = HitName.find_last_of('.');
		if (Dot == std::string::npos)
			HitName += ".hit";
		else
			HitName = HitName.substr(0, Dot) + ".hit";

		const std::string Path = RO_FindDataFile(pMapPath, HitName);
		if (Path.empty())
			return false;

		auto Buffer = RO_ReadFileMaybeRnc(Path);
		if (!Buffer || Buffer->empty())
			return false;

		// Engine path: fileLoadTo into &mTile_Hit[slot] then EndianSwap.
		// mTile_Hit is int16[512]; base half occupies [0..240), sub half [240..512).
		const size_t Capacity = (512 - pSlot) * sizeof(int16);
		const size_t CopySize = std::min<size_t>(Buffer->size(), Capacity);
		std::memcpy((uint8*)&pTileHit[pSlot], Buffer->data(), CopySize);
		tool_EndianSwap((uint8*)&pTileHit[pSlot], CopySize);
		return true;
	};

	auto LoadBhtInto = [&](const std::string& pBlkName, size_t pSlot) -> bool {
		std::string BhtName = pBlkName;
		const size_t Dot = BhtName.find_last_of('.');
		if (Dot == std::string::npos)
			BhtName += ".bht";
		else
			BhtName = BhtName.substr(0, Dot) + ".bht";

		const std::string Path = RO_FindDataFile(pMapPath, BhtName);
		if (Path.empty())
			return false;

		auto Buffer = RO_ReadFileMaybeRnc(Path);
		if (!Buffer || Buffer->empty())
			return false;

		// BHT is int8[512][8] (no endian swap in MapRuntime.cpp).
		const size_t Capacity = (512 - pSlot) * 8;
		const size_t CopySize = std::min<size_t>(Buffer->size(), Capacity);
		std::memcpy((uint8*)&pTileBHit[pSlot * 8], Buffer->data(), CopySize);
		return true;
	};

	if (!LoadHitInto(pBaseName, 0))   return false;
	if (!LoadHitInto(pSubName,  240)) return false;
	// BHT is best-effort — some tilesets ship without sub BHT data; the
	// engine's Map_Load_Resources also tolerates a zero-fill from the memset
	// it does immediately before fileLoadTo.
	LoadBhtInto(pBaseName, 0);
	LoadBhtInto(pSubName,  240);

	return true;
}

bool RO_LoadMap(const std::string& pInputMapPath, LoadedMap& pOut) {
	const std::string MapPath = RO_ResolveMapPath(pInputMapPath);
	auto MapBuffer = RO_ReadFileMaybeRnc(MapPath);

	if (!MapBuffer || MapBuffer->size() < 0x60) {
		g_Debugger->Error("Map route oracle failed: map not found or too small: " + MapPath);
		return false;
	}

	const uint16 Width = readBEWord(MapBuffer->data() + 0x54);
	const uint16 Height = readBEWord(MapBuffer->data() + 0x56);
	const size_t ExpectedSize = 0x60 + ((size_t)Width * (size_t)Height * 2);

	if (!Width || !Height || Width > 4096 || Height > 4096 || ExpectedSize > MapBuffer->size()) {
		g_Debugger->Error("Map route oracle failed: invalid or truncated map: " + MapPath);
		return false;
	}

	const std::string BaseName = RO_ReadAsciiAt(MapBuffer, 0x00, 11);
	const std::string SubName  = RO_ReadAsciiAt(MapBuffer, 0x10, 11);

	if (BaseName.empty() || SubName.empty()) {
		g_Debugger->Error("Map route oracle failed: missing tileset names in header: " + MapPath);
		return false;
	}

	// Take ownership of the bytes and LE-swap the tile-data region exactly
	// like cOriginalMap::loadCF1Map does, so readLE<uint16> matches engine
	// semantics in Map_Terrain_Get.
	pOut.MapData.assign(MapBuffer->begin(), MapBuffer->end());
	tool_EndianSwap(pOut.MapData.data() + 0x60, pOut.MapData.size() - 0x60);

	pOut.Width = Width;
	pOut.Height = Height;

	if (!RO_LoadHitTables(MapPath, BaseName, SubName, pOut.TileHit, pOut.TileBHit)) {
		g_Debugger->Error("Map route oracle failed: could not load HIT tables for " + BaseName + " / " + SubName);
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
// Engine oracle, replayed locally. Contract matches Map/MapRuntime.cpp
// (Tile_GetTerrainFeature + Map_Terrain_Get + cRandomMap::isTileWalkable).
// pX/pY here are PIXEL coordinates inside the map (engine shifts >> 4 to
// reach the tile and uses the low 4 bits for sub-tile resolution).
// ---------------------------------------------------------------------------

int16 RO_TileTerrainFeature(const LoadedMap& pMap, int16 pTileID, int16 pX, int16 pY) {
	if (pTileID < 0 || pTileID >= 512)
		return 0;

	int16 TerrainType = pMap.TileHit[pTileID];

	if (TerrainType < 0) {
		pX >>= 1;
		pX &= 0x07;

		int16 TilePixel = 7;
		TilePixel -= pX;

		pY >>= 1;
		pY &= 0x07;

		int8 RowTerrainType = pMap.TileBHit[(pTileID * 8) + pY];

		if (RowTerrainType & (1 << TilePixel))
			TerrainType >>= 4;
	}

	return (TerrainType & 0x0F);
}

int16 RO_MapTerrainGet(const LoadedMap& pMap, int16 pX, int16 pY) {
	if ((pY >> 4) > (int16)pMap.Height || (pX >> 4) > (int16)pMap.Width)
		return -1;

	int32 MapPtr = ((pY >> 4) * (int32)pMap.Width);
	MapPtr += (pX >> 4);
	MapPtr <<= 1;

	const size_t Offset = 0x60 + (size_t)MapPtr;
	if (Offset + 1 >= pMap.MapData.size())
		return -1;

	const uint16 TileID = readLE<uint16>(pMap.MapData.data() + Offset) & 0x1FF;
	return RO_TileTerrainFeature(pMap, (int16)TileID, pX, pY);
}

int16 RO_TileTerrainAtTile(const LoadedMap& pMap, int32 pTileX, int32 pTileY) {
	if (pTileX < 0 || pTileY < 0 ||
		(uint32)pTileX >= pMap.Width || (uint32)pTileY >= pMap.Height)
		return -1;

	// Sample at tile centre, exactly like cRandomMap::getTileTerrainFeature.
	const int16 PixelX = (int16)(pTileX * 16 + 8);
	const int16 PixelY = (int16)(pTileY * 16 + 8);
	return RO_MapTerrainGet(pMap, PixelX, PixelY);
}

bool RO_IsTileWalkable(const LoadedMap& pMap, int32 pTileX, int32 pTileY) {
	const int16 Feature = RO_TileTerrainAtTile(pMap, pTileX, pTileY);
	if (Feature < 0)
		return false;

	// mTiles_NotWalkable is a 16-entry global table indexed by eTerrainFeature.
	// Same verdict the engine uses for foot units (Source/Map/MapRuntime.cpp:759).
	return mTiles_NotWalkable[Feature] == 0;
}

// ---------------------------------------------------------------------------
// BFS over walkable tiles for `route_endtoend`. D10 row 3 reachability oracle
// only — not the engine's foot-unit pathfinder. Returns shortest path length
// in tile steps (Manhattan-stepped), or -1 if unreachable.
// ---------------------------------------------------------------------------

int32 RO_BfsRouteLength(const LoadedMap& pMap, int32 pFromX, int32 pFromY, int32 pToX, int32 pToY) {
	if (pFromX < 0 || pFromY < 0 || pToX < 0 || pToY < 0 ||
		(uint32)pFromX >= pMap.Width || (uint32)pFromY >= pMap.Height ||
		(uint32)pToX >= pMap.Width || (uint32)pToY >= pMap.Height)
		return -1;

	if (!RO_IsTileWalkable(pMap, pFromX, pFromY))
		return -1;
	if (!RO_IsTileWalkable(pMap, pToX, pToY))
		return -1;

	if (pFromX == pToX && pFromY == pToY)
		return 0;

	const size_t W = pMap.Width;
	const size_t H = pMap.Height;
	std::vector<int32> Dist(W * H, -1);
	std::deque<std::pair<int32, int32>> Queue;

	Dist[(size_t)pFromY * W + (size_t)pFromX] = 0;
	Queue.emplace_back(pFromX, pFromY);

	const int32 DX[4] = { 1, -1, 0,  0 };
	const int32 DY[4] = { 0,  0, 1, -1 };

	while (!Queue.empty()) {
		auto [CX, CY] = Queue.front();
		Queue.pop_front();
		const int32 CD = Dist[(size_t)CY * W + (size_t)CX];

		for (int i = 0; i < 4; ++i) {
			const int32 NX = CX + DX[i];
			const int32 NY = CY + DY[i];
			if (NX < 0 || NY < 0 || (uint32)NX >= W || (uint32)NY >= H)
				continue;

			const size_t Idx = (size_t)NY * W + (size_t)NX;
			if (Dist[Idx] >= 0)
				continue;
			if (!RO_IsTileWalkable(pMap, NX, NY))
				continue;

			Dist[Idx] = CD + 1;
			if (NX == pToX && NY == pToY)
				return CD + 1;

			Queue.emplace_back(NX, NY);
		}
	}

	return -1;
}

// ---------------------------------------------------------------------------
// Default-output helper.
// ---------------------------------------------------------------------------

std::string RO_DefaultOutputPath(const std::string& pInputPath) {
	const size_t Dot = RO_ExtensionPos(pInputPath);
	if (Dot == std::string::npos)
		return pInputPath + ".result.json";
	return pInputPath.substr(0, Dot) + ".result.json";
}

// Pull an integer "x"/"y" coordinate from a JSON object/value with sane
// fallbacks (number, "x"/"y" subkeys, or a "tile":{x,y} subobject).
bool RO_ReadXY(const Json& pIn, int32& pX, int32& pY) {
	if (!pIn.is_object())
		return false;

	auto Pull = [&](const char* pKey, int32& pOut) -> bool {
		if (!pIn.contains(pKey))
			return false;
		const auto& V = pIn.at(pKey);
		if (V.is_number_integer() || V.is_number_unsigned()) {
			pOut = (int32)V.get<int64_t>();
			return true;
		}
		if (V.is_number_float()) {
			pOut = (int32)V.get<double>();
			return true;
		}
		return false;
	};

	if (Pull("x", pX) && Pull("y", pY))
		return true;

	if (pIn.contains("tile") && pIn.at("tile").is_object())
		return RO_ReadXY(pIn.at("tile"), pX, pY);

	return false;
}

} // anonymous namespace

namespace RouteOracleReplay {

bool Run(const std::string& pInputPath, const std::string& pOutputPath) {

	if (pInputPath.empty()) {
		g_Debugger->Error("Map route oracle failed: --map-route-oracle requires an input JSON path");
		return false;
	}

	std::ifstream InputFile(pInputPath.c_str(), std::ios::binary);
	if (!InputFile.is_open()) {
		g_Debugger->Error("Map route oracle failed: cannot open input JSON: " + pInputPath);
		return false;
	}

	std::stringstream Buffer;
	Buffer << InputFile.rdbuf();
	InputFile.close();

	Json InputDoc;
	try {
		InputDoc = Json::parse(Buffer.str());
	}
	catch (const std::exception& e) {
		g_Debugger->Error(std::string("Map route oracle failed: malformed input JSON: ") + e.what());
		return false;
	}

	if (!InputDoc.is_object() || !InputDoc.contains("map") || !InputDoc.at("map").is_string()) {
		g_Debugger->Error("Map route oracle failed: input JSON must be an object with a string \"map\" field");
		return false;
	}

	const std::string MapPath = InputDoc.at("map").get<std::string>();

	LoadedMap Map;
	if (!RO_LoadMap(MapPath, Map))
		return false;

	if (!InputDoc.contains("queries") || !InputDoc.at("queries").is_array()) {
		g_Debugger->Error("Map route oracle failed: input JSON must contain a \"queries\" array");
		return false;
	}

	Json Output = Json::object();
	Output["map"] = MapPath;
	Output["width"] = Map.Width;
	Output["height"] = Map.Height;

	Json Results = Json::array();
	for (const auto& Q : InputDoc.at("queries")) {
		Json R = Json::object();

		if (!Q.is_object() || !Q.contains("kind") || !Q.at("kind").is_string()) {
			R["error"] = "missing or non-string \"kind\"";
			Results.push_back(R);
			continue;
		}

		const std::string Kind = Q.at("kind").get<std::string>();
		R["kind"] = Kind;

		if (Kind == "walkable") {
			int32 X = 0, Y = 0;
			if (!RO_ReadXY(Q, X, Y)) {
				R["error"] = "walkable query needs integer x,y";
				Results.push_back(R);
				continue;
			}
			R["x"] = X;
			R["y"] = Y;
			R["result"] = RO_IsTileWalkable(Map, X, Y);
		}
		else if (Kind == "terrain") {
			int32 X = 0, Y = 0;
			if (!RO_ReadXY(Q, X, Y)) {
				R["error"] = "terrain query needs integer x,y";
				Results.push_back(R);
				continue;
			}
			R["x"] = X;
			R["y"] = Y;
			R["result"] = RO_TileTerrainAtTile(Map, X, Y);
		}
		else if (Kind == "route_endtoend") {
			if (!Q.contains("from") || !Q.contains("to")) {
				R["error"] = "route_endtoend needs \"from\" and \"to\" objects";
				Results.push_back(R);
				continue;
			}
			int32 FX = 0, FY = 0, TX = 0, TY = 0;
			if (!RO_ReadXY(Q.at("from"), FX, FY) || !RO_ReadXY(Q.at("to"), TX, TY)) {
				R["error"] = "route_endtoend from/to need integer x,y";
				Results.push_back(R);
				continue;
			}
			R["from"] = { {"x", FX}, {"y", FY} };
			R["to"]   = { {"x", TX}, {"y", TY} };

			const int32 Length = RO_BfsRouteLength(Map, FX, FY, TX, TY);
			R["result"] = Length >= 0;
			if (Length >= 0)
				R["length"] = Length;
			else
				R["length"] = nullptr;
		}
		else {
			R["error"] = "unknown query kind: " + Kind;
		}

		Results.push_back(R);
	}

	Output["results"] = Results;

	const std::string OutPath = pOutputPath.size() ? pOutputPath : RO_DefaultOutputPath(pInputPath);

	std::ofstream OutFile(OutPath.c_str(), std::ios::binary);
	if (!OutFile.is_open()) {
		g_Debugger->Error("Map route oracle failed: cannot write output JSON: " + OutPath);
		return false;
	}

	OutFile << Output.dump(2);
	OutFile.close();

	if (!OutFile.good()) {
		g_Debugger->Error("Map route oracle failed: write error on output JSON: " + OutPath);
		return false;
	}

	return true;
}

} // namespace RouteOracleReplay
