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

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

// Native port of the JS ice tile-art "Wang" edge matcher
// (MapGen.Terrain.Smoothing.Core.ApplyEdgeRule). The authored tile-edge atlas
// (IceTileEdges) and all per-cell semantic decisions stay in JS; this only
// runs the hot per-cell x per-candidate scoring loop. It is a PURE function of
// its inputs (no engine state) and must reproduce the JS output BYTE-FOR-BYTE,
// so every formula mirrors the JS exactly (including ECMAScript ToInt32 / lossy
// double-multiply semantics in the HashTile tie-break).
//
// A "class" is represented throughout by its single edge-glyph char:
//   snow='S' ice='I' shallow='e' deep='W'  ('.'=other, no class).
// Tile edges are 16-char glyph strings (N/E/S/W). Contents are a 4-bit mask:
//   bit0 S, bit1 I, bit2 e, bit3 W.
namespace IceEdge {

struct TileRec {
    bool   valid = false;
    char   center = 0;        // class glyph
    int    contentsMask = 0;  // 4-bit class set
    std::string edges[4];     // index 0=N 1=E 2=S 3=W ; each 16 chars
};

class Matcher {
public:
    // Cache the authored atlas (called once from JS). All inputs are flat
    // strings so dukglue marshals them cheaply:
    //   pTileRecords[i] = "id|center|contents|edgeN|edgeE|edgeS|edgeW"
    //                     (center = one class glyph, contents = glyph run e.g. "SI")
    //   pByCenter[i]    = "G:id,id,id"        (G = class glyph)
    //   pCharToClass[i] = "c=G"               (c = char-map char; "*" = default)
    void setAtlas(const std::vector<std::string>& pTileRecords,
                  const std::vector<std::string>& pByCenter,
                  const std::vector<std::string>& pCharToClass);

    bool ready() const { return mReady; }

    // Run the matcher over a width*height grid. Returns width*height tile ids in
    // row-major order. Per-cell inputs are precomputed in JS and packed:
    //   pChars       : width*height char-map chars (one per cell)
    //   pHints       : width*height*4 chars, N,W,S,E per cell; '-' = null/no hint
    //   pReqCenter   : width*height chars, a class glyph or '-' (null)
    //   pReqContents : width*height chars, '-'=null else ('A'+contentsMask)
    //   pSeed        : already coerced to uint32 in JS (pContext.Seed >>> 0)
    std::vector<int> apply(int pWidth, int pHeight,
                           const std::string& pChars,
                           const std::string& pHints,
                           const std::string& pReqCenter,
                           const std::string& pReqContents,
                           uint32_t pSeed,
                           const std::string* pDirtyMask = nullptr,
                           const std::vector<int>* pPreviousTiles = nullptr,
                           int pMinX = 0,
                           int pMinY = 0,
                           int pMaxX = -1,
                           int pMaxY = -1) const;

private:
    bool mReady = false;
    int  mCharToClass[256];                 // char-map char -> class glyph (0 if none)
    char mDefaultClass = 'S';
    std::unordered_map<int, TileRec> mTiles;
    std::unordered_map<int, std::vector<int>> mByCenter;  // class glyph -> ordered tile ids
};

} // namespace IceEdge
