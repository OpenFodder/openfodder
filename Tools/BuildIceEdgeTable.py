"""Build a Wang-Tile edge table for every ice tile.

For each tile in the ICE atlas:
  1. Decode 16x16 pixels from ICEBASE.BLK + ICESUB0.BLK.
  2. Classify each pixel into {snow, ice, shallow, deep, other}.
  3. Extract 4 edge strips of 16 chars each:
        N = row 0, E = col 15 (top-down), S = row 15, W = col 0 (top-down)
  4. Compute the tile's centre class = inner 8x8 (rows 4-11, cols 4-11)
     majority of {snow, ice, shallow, deep}, ignoring 'other'.
  5. Compute an edge profile triple per edge:
        (leftClass, rightClass, transitionIndex)
     where left = majority of pixels 0..3, right = majority of pixels 12..15,
     and transitionIndex = first index (0..16) where the strip stops being
     leftClass; 16 if uniform.
  6. Bucket tiles by `byCenter[centerClass]` and by `bySeam[a|b][edge]` keys.

Output:
  - Run/Scripts/MapGen/Terrain/Smoothing/Data/IceTileEdges.js  (JS data file)
  - Documentation/IceTileEdgeTable.txt                          (human-readable)

Edge alphabet:
    S = snow, I = ice, e = shallow, W = deep, . = other

The runtime matcher (Core.ApplyEdgeRule) consumes IceTileEdges.js to pick a
tile per cell whose N/E/S/W edge strips are compatible with neighbour tiles
already placed (or with the cell's char-map class for unplaced neighbours).
"""
from __future__ import annotations

import json
import os
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import dernc  # noqa: E402

DATA_DIR = os.path.normpath(os.path.join(HERE, '..', 'Run', 'Data', 'Amiga'))
DOC_DIR = os.path.normpath(os.path.join(HERE, '..', 'Documentation'))
JS_OUT = os.path.normpath(os.path.join(
    HERE, '..', 'Run', 'Scripts', 'MapGen', 'Terrain', 'Smoothing', 'Data',
    'IceTileEdges.js'))
TXT_OUT = os.path.join(DOC_DIR, 'IceTileEdgeTable.txt')

CLASS_FOR_INDEX = {1: 'snow', 2: 'ice', 7: 'shallow', 8: 'deep'}
GLYPH_FOR_CLASS = {'snow': 'S', 'ice': 'I', 'shallow': 'e', 'deep': 'W', 'other': '.'}
CLASS_ORDER = ['snow', 'ice', 'shallow', 'deep', 'other']

# Canonical Cannon Fodder ICE atlas regions used by the runtime placer.
# Tiles outside these ranges are decorations (trees, paths, cliffs, outcrops)
# placed by separate passes; the edge matcher must not pick them.
CANONICAL_RANGES = [
    (0, 25), (33, 34), (42, 45), (62, 63),
    (80, 119), (128, 139), (148, 151), (156, 159), (176, 179),
]


def is_canonical(tid):
    return any(lo <= tid <= hi for lo, hi in CANONICAL_RANGES)


# Tiles whose centre-class auto-detection picks the wrong class. Manually
# pinned here so that the runtime matcher routes them correctly. Empty until
# we observe ambiguous cases during validation.
CENTER_OVERRIDE = {
    # tile_id : 'snow' | 'ice' | 'shallow' | 'deep'
}


def load_blk(path):
    with open(path, 'rb') as f:
        return dernc.unpack(f.read())


def tile_pixels(blk, tid):
    base = tid * 128
    rows = [[0] * 16 for _ in range(16)]
    for plane in range(4):
        for row in range(16):
            off = base + plane * 32 + row * 2
            word = (blk[off] << 8) | blk[off + 1]
            for x in range(16):
                bit = (word >> (15 - x)) & 1
                if bit:
                    rows[row][x] |= (1 << plane)
    return rows


def classify_pixel(idx):
    return CLASS_FOR_INDEX.get(idx, 'other')


def classify_grid(pixels):
    return [[classify_pixel(pixels[r][c]) for c in range(16)] for r in range(16)]


def edge_strip(grid, edge):
    if edge == 'N':
        return [grid[0][c] for c in range(16)]
    if edge == 'S':
        return [grid[15][c] for c in range(16)]
    if edge == 'W':
        return [grid[r][0] for r in range(16)]
    if edge == 'E':
        return [grid[r][15] for r in range(16)]
    raise ValueError(edge)


def strip_to_string(strip):
    return ''.join(GLYPH_FOR_CLASS[c] for c in strip)


def majority_class(items, ignore_other=True):
    c = Counter(items)
    if ignore_other:
        c.pop('other', None)
    if not c:
        return 'other'
    # Tie-break by CLASS_ORDER for determinism.
    best = None
    best_n = -1
    for cls in CLASS_ORDER:
        n = c.get(cls, 0)
        if n > best_n:
            best = cls
            best_n = n
    return best


def edge_profile(strip):
    """Return (leftClass, rightClass, transitionIndex)."""
    left = majority_class(strip[0:4], ignore_other=True)
    right = majority_class(strip[12:16], ignore_other=True)
    # transition: first index where the strip ceases to be left
    # Treat 'other' as matching left to suppress noise.
    ti = 16
    for i, c in enumerate(strip):
        if c == left or c == 'other':
            continue
        ti = i
        break
    return [left, right, ti]


def center_class(grid, tid):
    if tid in CENTER_OVERRIDE:
        return CENTER_OVERRIDE[tid]
    inner = []
    for r in range(4, 12):
        for c in range(4, 12):
            inner.append(grid[r][c])
    return majority_class(inner, ignore_other=True)


def seam_key(a, b):
    return '|'.join(sorted((a, b)))


def build():
    base = load_blk(os.path.join(DATA_DIR, 'ICEBASE.BLK'))
    sub = load_blk(os.path.join(DATA_DIR, 'ICESUB0.BLK'))
    base_n = len(base) // 128
    sub_n = len(sub) // 128
    total_n = base_n + sub_n

    def pixels_for(tid):
        if tid < base_n:
            return tile_pixels(base, tid)
        return tile_pixels(sub, tid - base_n)

    tiles = {}
    by_center = {cls: [] for cls in CLASS_ORDER if cls != 'other'}
    by_center['other'] = []
    by_seam = {}

    # Only canonical CF1 transition tiles are processed. The remaining tile
    # IDs in the atlas are trees, fences, buildings, cliffs, and other
    # decorations placed by separate passes - they must never be picked by
    # the edge matcher and there is no point storing their edge strips.
    for tid in range(total_n):
        if not is_canonical(tid):
            continue
        grid = classify_grid(pixels_for(tid))
        edges = {}
        profiles = {}
        for edge in ('N', 'E', 'S', 'W'):
            strip = edge_strip(grid, edge)
            edges[edge] = strip_to_string(strip)
            profiles[edge] = edge_profile(strip)

        cc = center_class(grid, tid)
        rec = {
            'center': cc,
            'edges': edges,
            'edgeProfile': profiles,
        }
        tiles[tid] = rec
        by_center.setdefault(cc, []).append(tid)
        for edge, prof in profiles.items():
            a, b = prof[0], prof[1]
            if a == b:
                continue
            key = seam_key(a, b)
            by_seam.setdefault(key, {'N': [], 'E': [], 'S': [], 'W': []})
            by_seam[key][edge].append(tid)

    return tiles, by_center, by_seam


def emit_js(tiles, by_center, by_seam):
    """Emit IceTileEdges.js. Compact list-of-pairs for tiles, plain JSON for indexes."""
    lines = []
    lines.append('var MapGen = MapGen || {};')
    lines.append('MapGen.Terrain = MapGen.Terrain || {};')
    lines.append('MapGen.Terrain.Smoothing = MapGen.Terrain.Smoothing || {};')
    lines.append('')
    lines.append('// Auto-generated by Tools/BuildIceEdgeTable.py - DO NOT EDIT.')
    lines.append('// See plan: replace bm-rule matcher with Wang-Tile edge matcher.')
    lines.append('// Edge alphabet: S=snow I=ice e=shallow W=deep .=other')
    lines.append('// Bit order independent: tile.edges.N is a 16-char string,')
    lines.append('// indexed left-to-right (W=col0 -> E=col15 along N/S rows;')
    lines.append('// N=row0 -> S=row15 along E/W cols).')
    lines.append('')
    lines.append('MapGen.Terrain.Smoothing.IceTileEdges = {')

    # tiles
    lines.append('    tiles: {')
    tids = sorted(tiles.keys())
    for tid in tids:
        rec = tiles[tid]
        e = rec['edges']
        p = rec['edgeProfile']
        line = (
            f'        "{tid}": {{ '
            f'center: "{rec["center"]}", '
            f'edges: {{ '
            f'N: "{e["N"]}", E: "{e["E"]}", S: "{e["S"]}", W: "{e["W"]}" '
            f'}}, '
            f'edgeProfile: {{ '
            f'N: {json.dumps(p["N"])}, E: {json.dumps(p["E"])}, '
            f'S: {json.dumps(p["S"])}, W: {json.dumps(p["W"])} '
            f'}} }},'
        )
        lines.append(line)
    if lines[-1].endswith(','):
        lines[-1] = lines[-1][:-1]
    lines.append('    },')
    lines.append('')

    # byCenter
    lines.append('    byCenter: {')
    keys = sorted(k for k in by_center if by_center[k])
    for i, k in enumerate(keys):
        ids = ', '.join(str(t) for t in by_center[k])
        sep = ',' if i + 1 < len(keys) else ''
        lines.append(f'        {k}: [{ids}]{sep}')
    lines.append('    },')
    lines.append('')

    # bySeam
    lines.append('    bySeam: {')
    skeys = sorted(by_seam.keys())
    for i, k in enumerate(skeys):
        rec = by_seam[k]
        sep = ',' if i + 1 < len(skeys) else ''
        lines.append(f'        "{k}": {{')
        edges = ['N', 'E', 'S', 'W']
        for j, e in enumerate(edges):
            ids = ', '.join(str(t) for t in rec[e])
            esep = ',' if j + 1 < len(edges) else ''
            lines.append(f'            {e}: [{ids}]{esep}')
        lines.append(f'        }}{sep}')
    lines.append('    }')

    lines.append('};')
    lines.append('')
    return '\n'.join(lines)


def emit_txt(tiles, by_center, by_seam):
    out = []
    out.append('# Ice tile edge table (generated)')
    out.append('# Edge alphabet: S=snow I=ice e=shallow W=deep .=other')
    out.append(f'# tiles={len(tiles)}')
    out.append('')
    for tid in sorted(tiles.keys()):
        rec = tiles[tid]
        e = rec['edges']
        p = rec['edgeProfile']
        out.append(
            f'tile {tid:3d}  center={rec["center"]:<8}  '
            f'N="{e["N"]}"  E="{e["E"]}"  S="{e["S"]}"  W="{e["W"]}"'
        )
        out.append(
            f'           profile  '
            f'N={p["N"]}  E={p["E"]}  S={p["S"]}  W={p["W"]}'
        )
    out.append('')
    out.append('## byCenter summary')
    for cls in sorted(by_center.keys()):
        ids = by_center[cls]
        if not ids:
            continue
        out.append(f'  {cls:<8}  ({len(ids)})  {ids}')
    out.append('')
    out.append('## bySeam summary (unordered class pair -> {edge: [tile ids]})')
    for k in sorted(by_seam.keys()):
        rec = by_seam[k]
        total = sum(len(v) for v in rec.values())
        out.append(f'  {k}  total={total}')
        for edge in ('N', 'E', 'S', 'W'):
            if rec[edge]:
                out.append(f'    {edge}: {rec[edge]}')
    return '\n'.join(out) + '\n'


def main():
    tiles, by_center, by_seam = build()
    os.makedirs(DOC_DIR, exist_ok=True)
    os.makedirs(os.path.dirname(JS_OUT), exist_ok=True)

    with open(JS_OUT, 'w') as f:
        f.write(emit_js(tiles, by_center, by_seam))
    print(f'wrote {JS_OUT}')

    with open(TXT_OUT, 'w') as f:
        f.write(emit_txt(tiles, by_center, by_seam))
    print(f'wrote {TXT_OUT}')

    print(f'  tiles={len(tiles)}')
    for cls in CLASS_ORDER:
        n = len(by_center.get(cls, []))
        if n:
            print(f'  byCenter[{cls}]={n}')
    print(f'  bySeam keys={len(by_seam)}')


if __name__ == '__main__':
    main()
