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

#include "IceEdgeMatcher.hpp"

#include <cmath>
#include <cstring>

namespace IceEdge {

// --- small string helpers -------------------------------------------------

static std::vector<std::string> splitChar(const std::string& s, char d) {
    std::vector<std::string> out;
    size_t start = 0;
    for(size_t i = 0; i <= s.size(); ++i) {
        if(i == s.size() || s[i] == d) {
            out.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }
    return out;
}

// glyph -> contents bit. S=1 I=2 e=4 W=8 ; anything else 0.
static int glyphBit(char g) {
    switch(g) {
        case 'S': return 1;
        case 'I': return 2;
        case 'e': return 4;
        case 'W': return 8;
        default:  return 0;
    }
}

static int contentsMaskFromGlyphs(const std::string& g) {
    int m = 0;
    for(size_t i = 0; i < g.size(); ++i)
        m |= glyphBit(g[i]);
    return m;
}

// --- ECMAScript integer semantics (for an exact HashTile port) ------------

static int32_t toInt32(double d) {
    if(!(d == d) || std::isinf(d))
        return 0;
    double t = std::trunc(d);
    double m = std::fmod(t, 4294967296.0);
    if(m < 0.0)
        m += 4294967296.0;
    return (int32_t)(uint32_t)m;
}

// Faithful port of MapGen.Random.HashTile. The third step is a LOSSY double
// multiply in JS (`value * 1274126177 | 0`), so we replicate it as a double
// multiply followed by ToInt32 rather than an int64 multiply — the jitter
// tie-break reads the low 3 bits, so this must match bit-for-bit.
static uint32_t hashTile(uint32_t pSeed, int pX, int pY, int pSalt) {
    double sum = (double)(int32_t)pSeed
        + (double)pX * 374761393.0
        + (double)pY * 668265263.0
        + (double)pSalt * 1013904223.0;
    int32_t value = toInt32(sum);

    uint32_t u = (uint32_t)value;
    value = (int32_t)(u ^ (u >> 13));

    value = toInt32((double)value * 1274126177.0);

    uint32_t u2 = (uint32_t)value;
    return u2 ^ (u2 >> 16);
}

// --- scoring (pure glyph arithmetic, ported verbatim from Core.js) --------

static bool edgeGlyphsDisjoint(char a, char b) {
    if(a == '.' || b == '.') return false;
    if(a == 'S') return b == 'W' || b == 'e';
    if(b == 'S') return a == 'W' || a == 'e';
    if(a == 'I') return b == 'W';
    if(b == 'I') return a == 'W';
    return false;
}

// pNeighbourEdge == nullptr -> "no placed neighbour" (use the hint branch).
// pHintGlyph == 0 -> "no hint".
static double scoreEdgeHint(const std::string& pCandidateEdge,
                            char pHintGlyph) {
    if(!pHintGlyph)
        return 0.0;

    const char* disjoint = "";
    if(pHintGlyph == 'S')      disjoint = "We";
    else if(pHintGlyph == 'I') disjoint = "W";
    else if(pHintGlyph == 'e') disjoint = "S";
    else if(pHintGlyph == 'W') disjoint = "SI";

    const char* soft = "";
    if(pHintGlyph == 'e') soft = "IW";
    else if(pHintGlyph == 'I') soft = "e";

    int n = 0, bad = 0, softCount = 0;
    for(int j = 0; j < 16; ++j) {
        char c = pCandidateEdge[j];
        if(c == pHintGlyph) ++n;
        else if(std::strchr(disjoint, c) && c) ++bad;
        else if(std::strchr(soft, c) && c) ++softCount;
    }
    return (double)(n * 4) - (double)(bad * 8) - (double)(softCount * 10);
}

static int countEdgeGlyphs(const std::string& pEdge, const char* pGlyphs) {
    int n = 0;
    for(size_t i = 0; i < pEdge.size(); ++i) {
        if(std::strchr(pGlyphs, pEdge[i]))
            ++n;
    }
    return n;
}

static double scoreEdgeContext(const std::string& pCandidateEdge,
                               char pHintGlyph) {
    if(!pHintGlyph)
        return 0.0;

    if(pHintGlyph == 'e') {
        int deep = countEdgeGlyphs(pCandidateEdge, "W");
        int ice = countEdgeGlyphs(pCandidateEdge, "I");
        double score = deep > 3 ? (double)(deep - 3) * -80.0 : 0.0;
        score += ice > 4 ? (double)(ice - 4) * -80.0 : 0.0;
        return score;
    }
    if(pHintGlyph == 'I') {
        int water = countEdgeGlyphs(pCandidateEdge, "We");
        return water > 1 ? (double)(water - 1) * -90.0 : 0.0;
    }
    if(pHintGlyph == 'S') {
        int water = countEdgeGlyphs(pCandidateEdge, "We");
        return water > 1 ? (double)(water - 1) * -90.0 : 0.0;
    }
    if(pHintGlyph == 'W') {
        int land = countEdgeGlyphs(pCandidateEdge, "SI");
        return land > 3 ? (double)(land - 3) * -80.0 : 0.0;
    }
    return 0.0;
}

static double scoreEdgeMatch(const std::string& pCandidateEdge,
                             const std::string* pNeighbourEdge,
                             char pHintGlyph) {
    if(pNeighbourEdge != nullptr) {
        double edgeScore = 0.0;
        if(pCandidateEdge == *pNeighbourEdge) {
            edgeScore = 100.0;
        } else {
            int same = 0;
            int bad = 0;
            for(int i = 0; i < 16; ++i) {
                char cg = pCandidateEdge[i];
                char ng = (*pNeighbourEdge)[i];
                if(cg == ng) ++same;
                else if(edgeGlyphsDisjoint(cg, ng)) ++bad;
            }
            edgeScore = -50.0 + (double)(same * 150) / 16.0 - (double)(bad * 4);
        }

        return edgeScore + scoreEdgeHint(pCandidateEdge, pHintGlyph) +
            scoreEdgeContext(pCandidateEdge, pHintGlyph);
    }

    return scoreEdgeHint(pCandidateEdge, pHintGlyph) +
        scoreEdgeContext(pCandidateEdge, pHintGlyph);
}

static double scoreStructuralBias(const TileRec& pRec) {
    char centerGlyph = pRec.center;
    if(!centerGlyph)
        return 0.0;

    int structural = 0;
    for(int e = 0; e < 4; ++e) {
        const std::string& edge = pRec.edges[e];
        for(size_t i = 0; i < edge.size(); ++i)
            if(edge[i] == centerGlyph) ++structural;
    }

    return structural < 4 ? -150.0 : 0.0;
}

// --- atlas ----------------------------------------------------------------

void Matcher::setAtlas(const std::vector<std::string>& pTileRecords,
                       const std::vector<std::string>& pByCenter,
                       const std::vector<std::string>& pCharToClass) {
    mTiles.clear();
    mByCenter.clear();
    for(int i = 0; i < 256; ++i)
        mCharToClass[i] = 0;
    mDefaultClass = 'S';

    for(size_t i = 0; i < pTileRecords.size(); ++i) {
        std::vector<std::string> f = splitChar(pTileRecords[i], '|');
        if(f.size() < 7)
            continue;
        int id = std::atoi(f[0].c_str());
        TileRec rec;
        rec.valid = true;
        rec.center = f[1].empty() ? 0 : f[1][0];
        rec.contentsMask = contentsMaskFromGlyphs(f[2]);
        rec.edges[0] = f[3];  // N
        rec.edges[1] = f[4];  // E
        rec.edges[2] = f[5];  // S
        rec.edges[3] = f[6];  // W
        mTiles[id] = rec;
    }

    for(size_t i = 0; i < pByCenter.size(); ++i) {
        std::vector<std::string> kv = splitChar(pByCenter[i], ':');
        if(kv.size() < 2 || kv[0].empty())
            continue;
        char cls = kv[0][0];
        std::vector<int>& list = mByCenter[(int)cls];
        std::vector<std::string> ids = splitChar(kv[1], ',');
        for(size_t j = 0; j < ids.size(); ++j) {
            if(!ids[j].empty())
                list.push_back(std::atoi(ids[j].c_str()));
        }
    }

    for(size_t i = 0; i < pCharToClass.size(); ++i) {
        const std::string& e = pCharToClass[i];
        size_t eq = e.find('=');
        if(eq == std::string::npos || eq + 1 >= e.size())
            continue;
        std::string key = e.substr(0, eq);
        char glyph = e[eq + 1];
        if(key == "*")
            mDefaultClass = glyph;
        else if(!key.empty())
            mCharToClass[(unsigned char)key[0]] = glyph;
    }

    mReady = true;
}

// --- matcher --------------------------------------------------------------

static void appendCandidates(std::vector<int>& pResult,
                             std::vector<char>& pSeen,
                             const std::vector<int>* pExtra) {
    if(!pExtra || pExtra->empty())
        return;
    for(size_t i = 0; i < pExtra->size(); ++i) {
        int id = (*pExtra)[i];
        if(id >= 0 && id < (int)pSeen.size() && pSeen[id])
            continue;
        if(id >= 0 && id < (int)pSeen.size())
            pSeen[id] = 1;
        pResult.push_back(id);
    }
}

std::vector<int> Matcher::apply(int pWidth, int pHeight,
                                const std::string& pChars,
                                const std::string& pHints,
                                const std::string& pReqCenter,
                                const std::string& pReqContents,
                                uint32_t pSeed,
                                const std::string* pDirtyMask,
                                const std::vector<int>* pPreviousTiles,
                                int pMinX,
                                int pMinY,
                                int pMaxX,
                                int pMaxY) const {
    std::vector<int> tilesOut((size_t)pWidth * pHeight, 0);
    if(!mReady || pWidth <= 0 || pHeight <= 0)
        return tilesOut;

    size_t cells = (size_t)pWidth * pHeight;
    if(pChars.size() < cells || pReqCenter.size() < cells ||
        pReqContents.size() < cells || pHints.size() < cells * 4)
        return tilesOut;

    bool masked = pDirtyMask && pPreviousTiles &&
        pDirtyMask->size() >= cells && pPreviousTiles->size() >= cells;
    std::vector<int> placed(cells, -1);
    if(masked) {
        for(size_t i = 0; i < cells; ++i) {
            int prev = (*pPreviousTiles)[i];
            tilesOut[i] = prev;
            placed[i] = prev;
        }
    }

    int scanMinX = 0;
    int scanMinY = 0;
    int scanMaxX = pWidth - 1;
    int scanMaxY = pHeight - 1;
    if(pMaxX >= 0 && pMaxY >= 0) {
        if(pMinX > pMaxX || pMinY > pMaxY ||
            pMaxX < 0 || pMaxY < 0 ||
            pMinX >= pWidth || pMinY >= pHeight)
            return tilesOut;
        scanMinX = pMinX < 0 ? 0 : pMinX;
        scanMinY = pMinY < 0 ? 0 : pMinY;
        scanMaxX = pMaxX >= pWidth ? pWidth - 1 : pMaxX;
        scanMaxY = pMaxY >= pHeight ? pHeight - 1 : pMaxY;
    }

    int maxTileId = 0;
    for(std::unordered_map<int, TileRec>::const_iterator it = mTiles.begin(); it != mTiles.end(); ++it)
        if(it->first > maxTileId) maxTileId = it->first;
    std::vector<char> seen(maxTileId + 1, 0);

    // class glyph at a char-map cell (default for OOB), mirroring charClassAt.
    // local lambda-free helper via a small closure object would be cleaner, but
    // keep it explicit for clarity.
    #define CLASS_AT(xx, yy) ( ((xx) < 0 || (yy) < 0 || (xx) >= pWidth || (yy) >= pHeight) \
        ? mDefaultClass \
        : ( mCharToClass[(unsigned char)pChars[(size_t)(yy) * pWidth + (xx)]] \
            ? mCharToClass[(unsigned char)pChars[(size_t)(yy) * pWidth + (xx)]] \
            : mDefaultClass ) )

    std::unordered_map<int, std::vector<int>>::const_iterator byIce = mByCenter.find((int)'I');

    auto tileRec = [&](int pTileId) -> const TileRec* {
        if(pTileId < 0)
            return nullptr;
        std::unordered_map<int, TileRec>::const_iterator t = mTiles.find(pTileId);
        return t != mTiles.end() ? &t->second : nullptr;
    };

    auto tileRecAt = [&](const std::vector<int>& pTiles, int pX, int pY) -> const TileRec* {
        if(pX < 0 || pY < 0 || pX >= pWidth || pY >= pHeight)
            return nullptr;
        return tileRec(pTiles[(size_t)pY * pWidth + pX]);
    };

    auto isAquatic = [](char pCls) -> bool {
        return pCls == 'e' || pCls == 'W';
    };

    auto isAquaticShore = [&](int pX, int pY, char pCls) -> bool {
        if(pCls != 'I')
            return false;
        return isAquatic(CLASS_AT(pX, pY - 1)) ||
            isAquatic(CLASS_AT(pX + 1, pY)) ||
            isAquatic(CLASS_AT(pX, pY + 1)) ||
            isAquatic(CLASS_AT(pX - 1, pY));
    };

    auto shoreRepeatPenalty = [&](int pTileId, int pX, int pY, char pCls,
                                  const std::vector<int>& pTiles) -> double {
        if(!isAquaticShore(pX, pY, pCls))
            return 0.0;

        double penalty = 0.0;
        if(pY > 0 && pTiles[(size_t)(pY - 1) * pWidth + pX] == pTileId)
            penalty -= 64.0;
        if(pX + 1 < pWidth && pTiles[(size_t)pY * pWidth + (pX + 1)] == pTileId)
            penalty -= 64.0;
        if(pY + 1 < pHeight && pTiles[(size_t)(pY + 1) * pWidth + pX] == pTileId)
            penalty -= 64.0;
        if(pX > 0 && pTiles[(size_t)pY * pWidth + (pX - 1)] == pTileId)
            penalty -= 64.0;
        return penalty;
    };

    auto buildCandidates = [&](int pX, int pY, size_t pIdx, char pCls,
                               std::vector<int>& pCandidates) -> bool {
        pCandidates.clear();
        for(size_t k = 0; k < seen.size(); ++k) seen[k] = 0;

        std::unordered_map<int, std::vector<int>>::const_iterator base = mByCenter.find((int)pCls);
        if(base == mByCenter.end() || base->second.empty())
            return false;

        appendCandidates(pCandidates, seen, &base->second);

        if(pCls == 'S') {
            bool west  = CLASS_AT(pX - 1, pY) == 'e';
            bool east  = CLASS_AT(pX + 1, pY) == 'e';
            bool north = CLASS_AT(pX, pY - 1) == 'e';
            bool south = CLASS_AT(pX, pY + 1) == 'e';
            if((west || east) && (north || south) && byIce != mByCenter.end())
                appendCandidates(pCandidates, seen, &byIce->second);
        }

        char hintList[4] = {
            pHints[pIdx * 4 + 0],
            pHints[pIdx * 4 + 1],
            pHints[pIdx * 4 + 2],
            pHints[pIdx * 4 + 3]
        };
        for(int h = 0; h < 4; ++h) {
            char g = hintList[h];
            if(g == '-')
                g = 0;
            char hc = (g == 'S' || g == 'I' || g == 'e' || g == 'W') ? g : 0;
            if(hc && hc != pCls) {
                std::unordered_map<int, std::vector<int>>::const_iterator hit = mByCenter.find((int)hc);
                if(hit != mByCenter.end())
                    appendCandidates(pCandidates, seen, &hit->second);
            }
        }

        char reqC = pReqCenter[pIdx];
        if(reqC != '-') {
            std::vector<int> filtered;
            for(size_t i = 0; i < pCandidates.size(); ++i) {
                const TileRec* rec = tileRec(pCandidates[i]);
                if(rec && rec->center == reqC)
                    filtered.push_back(pCandidates[i]);
            }
            if(!filtered.empty())
                pCandidates.swap(filtered);
        }

        char reqCt = pReqContents[pIdx];
        if(reqCt != '-') {
            int reqMask = reqCt - 'A';
            std::vector<int> filtered;
            for(size_t i = 0; i < pCandidates.size(); ++i) {
                const TileRec* rec = tileRec(pCandidates[i]);
                if(rec && rec->contentsMask == reqMask)
                    filtered.push_back(pCandidates[i]);
            }
            if(!filtered.empty())
                pCandidates.swap(filtered);
        }

        if(isAquaticShore(pX, pY, pCls)) {
            std::vector<int> filtered;
            for(size_t i = 0; i < pCandidates.size(); ++i) {
                const TileRec* rec = tileRec(pCandidates[i]);
                if(rec && (rec->contentsMask & (4 | 8)) && !(rec->contentsMask & 1))
                    filtered.push_back(pCandidates[i]);
            }
            if(!filtered.empty())
                pCandidates.swap(filtered);
        }

        return !pCandidates.empty();
    };

    auto scoreCandidateFinal = [&](int pTileId, int pX, int pY, size_t pIdx,
                                   char pCls, const std::vector<int>& pTiles) -> double {
        const TileRec* rec = tileRec(pTileId);
        if(!rec)
            return -1e9;

        const TileRec* northRec = tileRecAt(pTiles, pX, pY - 1);
        const TileRec* eastRec  = tileRecAt(pTiles, pX + 1, pY);
        const TileRec* southRec = tileRecAt(pTiles, pX, pY + 1);
        const TileRec* westRec  = tileRecAt(pTiles, pX - 1, pY);

        char hN = pHints[pIdx * 4 + 0]; if(hN == '-') hN = 0;
        char hW = pHints[pIdx * 4 + 1]; if(hW == '-') hW = 0;
        char hS = pHints[pIdx * 4 + 2]; if(hS == '-') hS = 0;
        char hE = pHints[pIdx * 4 + 3]; if(hE == '-') hE = 0;

        if(!hN) hN = (pY > 0) ? CLASS_AT(pX, pY - 1) : pCls;
        if(!hW) hW = (pX > 0) ? CLASS_AT(pX - 1, pY) : pCls;
        if(!hS) hS = (pY + 1 < pHeight) ? CLASS_AT(pX, pY + 1) : pCls;
        if(!hE) hE = (pX + 1 < pWidth) ? CLASS_AT(pX + 1, pY) : pCls;

        double score = scoreEdgeMatch(rec->edges[0], northRec ? &northRec->edges[2] : nullptr, hN);
        score += scoreEdgeMatch(rec->edges[1], eastRec ? &eastRec->edges[3] : nullptr, hE);
        score += scoreEdgeMatch(rec->edges[2], southRec ? &southRec->edges[0] : nullptr, hS);
        score += scoreEdgeMatch(rec->edges[3], westRec ? &westRec->edges[1] : nullptr, hW);
        score += scoreStructuralBias(*rec);
        score += shoreRepeatPenalty(pTileId, pX, pY, pCls, pTiles);
        score += (double)(hashTile(pSeed, pX, pY, 4096 + pTileId) & 7) * 0.001;
        return score;
    };

    for(int y = scanMinY; y <= scanMaxY; ++y) {
        for(int x = scanMinX; x <= scanMaxX; ++x) {
            size_t idx = (size_t)y * pWidth + x;
            if(masked && (*pDirtyMask)[idx] != '1')
                continue;

            char cls = CLASS_AT(x, y);
            std::vector<int> candidates;
            if(!buildCandidates(x, y, idx, cls, candidates))
                continue;  // unmatched: leave tilesOut[idx]=0, placed stays -1

            // hints: packed N,W,S,E ; '-' = null
            char hN = pHints[idx * 4 + 0]; if(hN == '-') hN = 0;
            char hW = pHints[idx * 4 + 1]; if(hW == '-') hW = 0;
            char hS = pHints[idx * 4 + 2]; if(hS == '-') hS = 0;
            char hE = pHints[idx * 4 + 3]; if(hE == '-') hE = 0;

            // placed neighbour facing edges
            int northTileId = (y > 0) ? placed[(size_t)(y - 1) * pWidth + x] : -1;
            int westTileId  = (x > 0) ? placed[(size_t)y * pWidth + (x - 1)] : -1;
            const TileRec* northRec = tileRec(northTileId);
            const TileRec* westRec  = tileRec(westTileId);
            const std::string* northFacing = northRec ? &northRec->edges[2] : nullptr;
            const std::string* westFacing  = westRec  ? &westRec->edges[1]  : nullptr;

            if(!hN) hN = (y > 0) ? CLASS_AT(x, y - 1) : cls;
            if(!hW) hW = (x > 0) ? CLASS_AT(x - 1, y) : cls;
            if(!hS) hS = (y + 1 < pHeight) ? CLASS_AT(x, y + 1) : cls;
            if(!hE) hE = (x + 1 < pWidth) ? CLASS_AT(x + 1, y) : cls;

            int bestId = candidates.empty() ? 0 : candidates[0];
            double bestScore = -1e9;
            for(size_t i = 0; i < candidates.size(); ++i) {
                int candId = candidates[i];
                std::unordered_map<int, TileRec>::const_iterator t = mTiles.find(candId);
                if(t == mTiles.end())
                    continue;
                const TileRec& rec = t->second;

                double score = scoreEdgeMatch(rec.edges[0], northFacing, hN);
                score += scoreEdgeMatch(rec.edges[3], westFacing, hW);
                score += scoreEdgeMatch(rec.edges[2], nullptr, hS);
                score += scoreEdgeMatch(rec.edges[1], nullptr, hE);
                score += scoreStructuralBias(rec);
                if(isAquaticShore(x, y, cls)) {
                    if(northTileId == candId)
                        score -= 64.0;
                    if(westTileId == candId)
                        score -= 64.0;
                }

                double jitter = (double)(hashTile(pSeed, x, y, 4096 + candId) & 7) * 0.001;
                double total = score + jitter;
                if(total > bestScore) {
                    bestScore = total;
                    bestId = candId;
                }
            }

            tilesOut[idx] = bestId;
            placed[idx] = bestId;
        }
    }

    const double relaxMinImprove = 64.0;
    for(int pass = 0; pass < 2; ++pass) {
        std::vector<int> nextTiles = tilesOut;
        int changed = 0;

        for(int y = scanMinY; y <= scanMaxY; ++y) {
            for(int x = scanMinX; x <= scanMaxX; ++x) {
                size_t idx = (size_t)y * pWidth + x;
                if(masked && (*pDirtyMask)[idx] != '1')
                    continue;

                char cls = CLASS_AT(x, y);
                std::vector<int> candidates;
                if(!buildCandidates(x, y, idx, cls, candidates))
                    continue;

                int currentId = tilesOut[idx];
                double currentScore = scoreCandidateFinal(currentId, x, y, idx, cls, tilesOut);
                int bestId = currentId;
                double bestScore = currentScore;

                for(size_t i = 0; i < candidates.size(); ++i) {
                    int candId = candidates[i];
                    double score = scoreCandidateFinal(candId, x, y, idx, cls, tilesOut);
                    if(score > bestScore) {
                        bestScore = score;
                        bestId = candId;
                    }
                }

                if(bestId != currentId && bestScore > currentScore + relaxMinImprove) {
                    nextTiles[idx] = bestId;
                    ++changed;
                }
            }
        }

        if(changed <= 0)
            break;

        tilesOut.swap(nextTiles);
        placed = tilesOut;
    }

    #undef CLASS_AT
    return tilesOut;
}

} // namespace IceEdge
