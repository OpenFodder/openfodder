"""Validate every (bm, tile) mapping in IceData.js against the dump file.

For each tile referenced in bm_cf1_snow_ice / bm_cf1_ice_shallow / bm_cf1_shallow_deep:
  1. Parse the BHT 8x8 visualisation from IceTileHitBhtDump.txt.
  2. Compute per-octant majority of (secondary glyph - primary glyph) and check
     that this matches what the brush block expects.
     - snow_ice block:   primary=Snow / secondary=Ice or WaterEdge -> bit 1 = octant has 'I' or 'e' (ice/wet)
       Actually for the brush: center=ice, bit 1 = ground/snow neighbour. So we want
       the octant's MAJORITY class to be the more land-ward of (snow, ice) = snow.
     - ice_shallow:      bit 1 = ice (vs shallow) dominates octant
     - shallow_deep:     bit 1 = shallow (vs deep) dominates octant
  3. Compute the same per-octant majority from the pixels: pixels are downsampled
     to 8x8 with S=pure-snow, .=pure-deep, *=mixed (includes ice and shallow), ?=other.
     For each brush, we approximate visual bm using the most relevant glyph(s):
         snow_ice:    bit 1 = octant is dominated by 'S'
         ice_shallow: bit 1 = octant has any non-water content (S or *) -- weak
         shallow_deep: bit 1 = octant has 'S' or '*' (vs '.')
     This is a proxy because the dump can't distinguish ice from shallow at the pixel layer.
  4. Compare both computations to the bm string declared in IceData.js, flag mismatches.

Octants (8x8 partitioning):
    NW: r0-2,c0-2   N: r0-2,c3-4    NE: r0-2,c5-7
    W:  r3-4,c0-2                    E:  r3-4,c5-7
    SW: r5-7,c0-2   S: r5-7,c3-4    SE: r5-7,c5-7
"""
from __future__ import annotations

import json
import os
import re
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
DUMP = os.path.normpath(os.path.join(HERE, '..', 'Documentation', 'IceTileHitBhtDump.txt'))
ICEDATA = os.path.normpath(os.path.join(HERE, '..', 'Run', 'Scripts', 'MapGen', 'Terrain', 'Smoothing', 'Data', 'IceData.js'))
PIXELTABLE = os.path.normpath(os.path.join(HERE, '..', 'Documentation', 'IceTilePixelTable.txt'))

OCT = [
    ('NW', [(r, c) for r in range(0, 3) for c in range(0, 3)]),
    ('N',  [(r, c) for r in range(0, 3) for c in range(3, 5)]),
    ('NE', [(r, c) for r in range(0, 3) for c in range(5, 8)]),
    ('W',  [(r, c) for r in range(3, 5) for c in range(0, 3)]),
    ('E',  [(r, c) for r in range(3, 5) for c in range(5, 8)]),
    ('SW', [(r, c) for r in range(5, 8) for c in range(0, 3)]),
    ('S',  [(r, c) for r in range(5, 8) for c in range(3, 5)]),
    ('SE', [(r, c) for r in range(5, 8) for c in range(5, 8)]),
]
BM_ORDER = ['NW', 'N', 'NE', 'W', 'E', 'SW', 'S', 'SE']

CENTER_CELLS = [(r, c) for r in range(3, 5) for c in range(3, 5)]


def parse_dump(path):
    """Return tiles[tid] = {'primary': c, 'secondary': c, 'bht': [8 lines], 'pix': [8 lines]}"""
    tiles = {}
    cur_tid = None
    cur = None
    state = None
    with open(path, 'r', errors='replace') as f:
        lines = f.read().splitlines()

    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r'^tile\s+(\d+)\s+hit=(\S+)\s+primary=\w+\(([^)]+)\)\s+secondary=\w+\(([^)]+)\)', line)
        if m:
            cur_tid = int(m.group(1))
            cur = {'primary': m.group(3), 'secondary': m.group(4), 'bht': [], 'pix': []}
            tiles[cur_tid] = cur
            i += 1
            # next line is bht=...
            i += 1
            # next 8 lines are bht visualization
            for _ in range(8):
                cur['bht'].append(lines[i].strip())
                i += 1
            # 'pixels:' line
            if i < len(lines) and lines[i].strip() == 'pixels:':
                i += 1
                for _ in range(8):
                    if i < len(lines):
                        cur['pix'].append(lines[i].strip())
                        i += 1
            continue
        i += 1
    return tiles


def parse_pixeltable(path):
    """Return tiles[tid] = {'classes': {snow, ice, shallow, deep, other}, 'bm': {'snow_ice', 'ice_shallow', 'shallow_deep'}}"""
    tiles = {}
    cur_tid = None
    cur = None
    with open(path) as f:
        for line in f:
            m = re.match(r'^tile\s+(\d+).*snow:(\d+)\s+ice:(\d+)\s+shallow:(\d+)\s+deep:(\d+)\s+other:(\d+)', line)
            if m:
                cur_tid = int(m.group(1))
                cur = {
                    'classes': {
                        'snow': int(m.group(2)),
                        'ice': int(m.group(3)),
                        'shallow': int(m.group(4)),
                        'deep': int(m.group(5)),
                        'other': int(m.group(6)),
                    },
                    'bm': {},
                }
                tiles[cur_tid] = cur
                continue
            m = re.match(r'\s+bm\s+snow_ice=(\d+)\s+ice_shallow=(\d+)\s+shallow_deep=(\d+)', line)
            if m and cur is not None:
                cur['bm'] = {'snow_ice': m.group(1), 'ice_shallow': m.group(2), 'shallow_deep': m.group(3)}
    return tiles


def parse_icedata_rules(path):
    """Return {brush_name: [(bm, [tile_ids])]}"""
    txt = open(path).read()
    out = {}
    for brush in ('bm_cf1_snow_ice', 'bm_cf1_ice_shallow', 'bm_cf1_shallow_deep'):
        # Find the brush block - locate the bm pattern then capture until matching ]
        idx = txt.find(brush + ':')
        if idx < 0:
            print(f'  WARN no block for {brush}')
            continue
        # Find the bitmask: [ array
        bm_open = txt.find('bitmask:', idx)
        if bm_open < 0:
            print(f'  WARN no bitmask in {brush}')
            continue
        bracket_open = txt.find('[', bm_open)
        # Match brackets to find close
        depth = 1
        j = bracket_open + 1
        while j < len(txt) and depth > 0:
            if txt[j] == '[':
                depth += 1
            elif txt[j] == ']':
                depth -= 1
            j += 1
        body = txt[bracket_open + 1:j - 1]
        rules = []
        for rm in re.finditer(r'\{\s*bm:\s*"(\d{8})",\s*tiles:\s*\[(.*?)\]\s*\}', body):
            bm = rm.group(1)
            tile_ids = [int(t) for t in re.findall(r'tile:\s*"(\d+)"', rm.group(2))]
            rules.append((bm, tile_ids))
        out[brush] = rules
    return out


def octant_bm_from_grid(rows, on_pred):
    bits = []
    for name in BM_ORDER:
        cells = dict(OCT)[name]
        ones = sum(1 for (r, c) in cells if on_pred(rows[r][c]))
        # majority: > half
        bits.append('1' if ones * 2 > len(cells) else '0')
    return ''.join(bits)


def center_class_bht(rows, primary, secondary):
    """Return 'primary' or 'secondary' depending on majority in the 4x4 centre."""
    # 8x8 centre 2x2 (rows 3-4, cols 3-4)
    sec = sum(1 for (r, c) in CENTER_CELLS if rows[r][c] == secondary)
    return 'secondary' if sec > 2 else 'primary'


def fmt_octant_count(rows, on_pred):
    parts = []
    for name in BM_ORDER:
        cells = dict(OCT)[name]
        ones = sum(1 for (r, c) in cells if on_pred(rows[r][c]))
        parts.append(f'{name}={ones}/{len(cells)}')
    return ' '.join(parts)


def validate():
    tiles = parse_dump(DUMP)
    pixtab = parse_pixeltable(PIXELTABLE)
    rules = parse_icedata_rules(ICEDATA)

    BRUSH_PAIR = {
        'bm_cf1_snow_ice':      ('snow', 'ice'),       # bit 1 = snow > ice
        'bm_cf1_ice_shallow':   ('ice', 'shallow'),    # bit 1 = ice > shallow
        'bm_cf1_shallow_deep':  ('shallow', 'deep'),   # bit 1 = shallow > deep
    }

    issues = []
    for brush, rule_list in rules.items():
        cls_a, cls_b = BRUSH_PAIR[brush]
        for bm, tile_ids in rule_list:
            for tid in tile_ids:
                tile = tiles.get(tid)
                if tile is None:
                    issues.append((brush, bm, tid, 'missing-from-dump', '', ''))
                    continue

                # BHT-derived bm (works only for DUAL tiles where primary != secondary)
                # Determine which glyph corresponds to class A.
                pri = tile['primary']
                sec = tile['secondary']
                glyph_to_class = {
                    'S': 'snow', 'I': 'ice', 'e': 'shallow', 'W': 'deep', '.': 'land',
                }
                bht_bm = None
                if pri in glyph_to_class and sec in glyph_to_class:
                    # bit 1 = octant has class A majority (per brush definition)
                    pri_cls = glyph_to_class[pri]
                    sec_cls = glyph_to_class[sec]
                    a_glyph = None
                    if pri_cls == cls_a:
                        a_glyph = pri
                    elif sec_cls == cls_a:
                        a_glyph = sec
                    if a_glyph is not None:
                        bht_bm = octant_bm_from_grid(tile['bht'], lambda c, ag=a_glyph: c == ag)

                # Pixel-table-derived bm (full 16x16, gold standard)
                pix_bm = None
                pixrec = pixtab.get(tid)
                if pixrec is not None:
                    pair_key = f'{cls_a}_{cls_b}'
                    pix_bm = pixrec['bm'].get(pair_key)

                # Centre check (using BHT). Determines if the tile's centre class
                # matches what the brush expects (centre = the more water-ward class).
                centre_bht = None
                if pri in glyph_to_class and sec in glyph_to_class:
                    pri_cls = glyph_to_class[pri]
                    sec_cls = glyph_to_class[sec]
                    sec_count = sum(1 for (r, c) in CENTER_CELLS if tile['bht'][r][c] == sec)
                    centre_glyph = sec if sec_count > 2 else pri
                    centre_bht = glyph_to_class[centre_glyph]

                # The brush expects centre = cls_b (more water-ward)
                centre_ok = centre_bht is None or centre_bht == cls_b

                # Pixel class breakdown if available
                cls_counts = pixrec['classes'] if pixrec else {}
                # majority class overall
                if cls_counts:
                    majority_overall = max(cls_counts.items(), key=lambda kv: kv[1])[0]
                else:
                    majority_overall = '?'

                problems = []
                if pix_bm is not None and pix_bm != bm:
                    problems.append(f'pix_bm={pix_bm}!=declared')
                if bht_bm is not None and bht_bm != bm:
                    problems.append(f'bht_bm={bht_bm}!=declared')
                if not centre_ok:
                    problems.append(f'centre={centre_bht} (expected {cls_b})')

                # also flag tiles where the dominant pixel pair differs from the brush pair
                if pixrec:
                    a_n = cls_counts.get(cls_a, 0)
                    b_n = cls_counts.get(cls_b, 0)
                    others = {k: v for k, v in cls_counts.items() if k not in (cls_a, cls_b, 'other')}
                    biggest_other = max(others.values()) if others else 0
                    biggest_other_cls = max(others.items(), key=lambda kv: kv[1])[0] if others else None
                    if biggest_other > min(a_n, b_n):
                        problems.append(f'dominated by {biggest_other_cls}={biggest_other} (pair {cls_a}={a_n} {cls_b}={b_n})')

                if problems:
                    issues.append((brush, bm, tid, '; '.join(problems), bht_bm or '-', pix_bm or '-'))

    # Print report
    if not issues:
        print('All clear.')
        return

    print(f'{len(issues)} issues found:\n')
    by_brush = {}
    for it in issues:
        by_brush.setdefault(it[0], []).append(it)
    for brush, items in by_brush.items():
        print(f'== {brush} ==')
        for brush, bm, tid, problem, bht_bm, pix_bm in items:
            print(f'  bm {bm}  tile {tid:3d}  bht={bht_bm}  pix={pix_bm}   {problem}')
        print()


if __name__ == '__main__':
    validate()
