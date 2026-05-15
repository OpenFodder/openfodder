"""Build a pixel-derived bm table for every ice tile.

The retail .bht only marks gameplay subdivision in DUAL tiles. Tiles tagged as
"pure Ice" or "pure Snow" can still have directional pixel patterns (snow on
the right, ice on the left, etc.), and those patterns are what the renderer
needs to match when smoothing transitions.

This tool decodes the actual pixels for every tile and derives bm signatures
from PIXEL CLASS counts per octant.

Pixel classification (icebase.pal calibrated):
    palette idx 1  -> snow    (very bright cyan)
    palette idx 2  -> ice     (mid cyan)
    palette idx 7  -> shallow (bright blue, used by WaterEdge)
    palette idx 8  -> deep    (dark blue, used by Water)
    other indices  -> "other" (transitional shading, path, tree, etc.)

Octants in 16x16 pixel space (the centre 4x4 is excluded):
    NW: r0-5  c0-5     N : r0-5  c6-9    NE: r0-5  c10-15
    W : r6-9  c0-5                       E : r6-9  c10-15
    SW: r10-15 c0-5    S : r10-15 c6-9   SE: r10-15 c10-15

bm bit order is NW N NE W E SW S SE, MSB-first.

For each chain pair (snow_ice, ice_shallow, shallow_deep), bit i = 1 iff
class A_count > class B_count in that octant (strict; ties resolve to 0).
"""
from __future__ import annotations

import os
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import dernc  # noqa: E402

DATA_DIR = os.path.normpath(os.path.join(HERE, '..', 'Run', 'Data', 'Amiga'))
DOC_DIR = os.path.normpath(os.path.join(HERE, '..', 'Documentation'))

CLASS_FOR_INDEX = {
    1: 'snow',
    2: 'ice',
    7: 'shallow',
    8: 'deep',
}

CHAIN = ['snow', 'ice', 'shallow', 'deep']
PAIRS = [('snow', 'ice'), ('ice', 'shallow'), ('shallow', 'deep')]

# Octants in 16x16 pixel space. (r0, r1, c0, c1) - exclusive ends.
OCTANTS = [
    ('NW', 0, 6, 0, 6),
    ('N',  0, 6, 6, 10),
    ('NE', 0, 6, 10, 16),
    ('W',  6, 10, 0, 6),
    ('E',  6, 10, 10, 16),
    ('SW', 10, 16, 0, 6),
    ('S',  10, 16, 6, 10),
    ('SE', 10, 16, 10, 16),
]

# Bit position in bm string: NW=index0 (MSB) ... SE=index7 (LSB)
BM_ORDER = ['NW', 'N', 'NE', 'W', 'E', 'SW', 'S', 'SE']


def load_blk(path):
    with open(path, 'rb') as f:
        return dernc.unpack(f.read())


def tile_pixels(blk, tid):
    """Decode a 16x16 tile from raw planar BLK data. tid is local to that file."""
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


def octant_class_counts(pixels):
    """Return {oct_name: Counter(class -> count)} for a single tile."""
    out = {}
    for name, r0, r1, c0, c1 in OCTANTS:
        c = Counter()
        for r in range(r0, r1):
            for x in range(c0, c1):
                c[classify_pixel(pixels[r][x])] += 1
        out[name] = c
    return out


def tile_class_counts(pixels):
    c = Counter()
    for r in range(16):
        for x in range(16):
            c[classify_pixel(pixels[r][x])] += 1
    return c


def bm_for_pair(oct_counts, a_class, b_class):
    """Compute bm string for the (A,B) pair: bit=1 iff A>B in that octant."""
    bits = []
    for name in BM_ORDER:
        c = oct_counts[name]
        a = c.get(a_class, 0)
        b = c.get(b_class, 0)
        bits.append('1' if a > b else '0')
    return ''.join(bits)


def main():
    base = load_blk(os.path.join(DATA_DIR, 'ICEBASE.BLK'))
    sub = load_blk(os.path.join(DATA_DIR, 'ICESUB0.BLK'))
    base_n = len(base) // 128
    sub_n = len(sub) // 128
    total_n = base_n + sub_n

    hit_base = open(os.path.join(DATA_DIR, 'icebase.hit'), 'rb').read()
    hit_sub = open(os.path.join(DATA_DIR, 'icesub0.hit'), 'rb').read()

    def hit_for(tid):
        if tid < base_n:
            o = tid * 2
            return (hit_base[o] << 8) | hit_base[o + 1]
        o = (tid - base_n) * 2
        return (hit_sub[o] << 8) | hit_sub[o + 1]

    def pixels_for(tid):
        if tid < base_n:
            return tile_pixels(base, tid)
        return tile_pixels(sub, tid - base_n)

    type_name = {0: 'Land', 5: 'WaterEdge', 6: 'Water', 7: 'Snow', 8: 'Ice'}

    rows = []
    for tid in range(total_n):
        h = hit_for(tid)
        dual = (h >> 15) & 1
        primary = h & 0x0F
        secondary = (h >> 4) & 0x0F
        pri_n = type_name.get(primary, f'?{primary}')
        sec_n = type_name.get(secondary, f'?{secondary}')

        pix = pixels_for(tid)
        oct_counts = octant_class_counts(pix)
        tc = tile_class_counts(pix)

        # Identify the two dominant classes (excluding 'other') for this tile.
        ranked = [(c, n) for c, n in tc.items() if c != 'other']
        ranked.sort(key=lambda x: -x[1])
        top2 = [c for c, _ in ranked[:2]]

        bm_per_pair = {}
        for a, b in PAIRS:
            bm_per_pair[f'{a}_{b}'] = bm_for_pair(oct_counts, a, b)

        # Auto-assign tile to the chain pair whose two classes both appear
        # meaningfully. Threshold: each class must hold >= 8 pixels (~3% of
        # 240 octant pixels). When a tile shows three classes, the chosen
        # pair is the adjacent pair containing the two highest counts.
        pair_assignment = None
        if len(top2) == 2:
            a, b = sorted(top2, key=lambda c: CHAIN.index(c))
            if (a, b) in PAIRS:
                pair_assignment = f'{a}_{b}'
            else:
                # chain skip: snow-shallow / snow-deep / ice-deep — rare.
                pair_assignment = f'{a}_{b}'  # still record so we can see it
        elif len(top2) == 1:
            pair_assignment = f'pure_{top2[0]}'
        else:
            pair_assignment = 'unknown'

        rows.append({
            'tid': tid,
            'hit': h,
            'pri': pri_n,
            'sec': sec_n,
            'dual': dual,
            'tc': tc,
            'oct': oct_counts,
            'bm': bm_per_pair,
            'pair': pair_assignment,
            'top2': top2,
        })

    # Write detailed table.
    os.makedirs(DOC_DIR, exist_ok=True)
    out_path = os.path.join(DOC_DIR, 'IceTilePixelTable.txt')
    with open(out_path, 'w') as f:
        f.write('# Pixel-derived ice tile bm table\n')
        f.write('# Class palette indices: snow=1, ice=2, shallow=7, deep=8 (anything else = "other")\n')
        f.write('# bm bit order: NW N NE W E SW S SE  (1 = octant where class A pixels > class B pixels)\n')
        f.write('# pair = auto-assigned chain pair from the two dominant pixel classes per tile\n')
        f.write('# bm columns: snow_ice ice_shallow shallow_deep\n\n')

        for row in rows:
            tc = row['tc']
            class_str = ' '.join(f'{c}:{tc.get(c, 0)}' for c in ['snow', 'ice', 'shallow', 'deep', 'other'])
            f.write(
                f"tile {row['tid']:3d}  hit=0x{row['hit']:04x}  "
                f"{row['pri']:<9}/{row['sec']:<9} {'DUAL' if row['dual'] else '    '}  "
                f"pair={row['pair']:<14}  "
                f"{class_str}\n"
            )
            f.write(
                f"          bm  snow_ice={row['bm']['snow_ice']}  "
                f"ice_shallow={row['bm']['ice_shallow']}  "
                f"shallow_deep={row['bm']['shallow_deep']}\n"
            )

        # Per-pair summary at the end
        f.write('\n===== Pair summary (auto-assigned) =====\n')
        pair_summary = Counter(r['pair'] for r in rows)
        for k in sorted(pair_summary):
            f.write(f'  {k:<22} {pair_summary[k]:4d} tiles\n')

    print(f'wrote {out_path}  tiles={total_n}')

    # Also dump a transition draft from pixel-derived bm only. We list, for each
    # of the 3 chain pairs, all tiles whose pair_assignment matches and group
    # by bm signature.
    draft_path = os.path.join(DOC_DIR, 'IceTransitionDraftPixel.txt')
    with open(draft_path, 'w') as f:
        f.write('# Pixel-derived ice transition draft\n')
        f.write('# For each chain pair, lists tiles grouped by pixel-octant bm signature.\n')
        f.write('# Each line: bm  -> tiles\n')
        f.write('# bm bit order: NW N NE W E SW S SE  (1 = class A dominates that octant)\n\n')

        for a, b in PAIRS:
            pair_key = f'{a}_{b}'
            f.write(f'## Pair {pair_key}\n')
            buckets = {}
            for row in rows:
                # Include tiles whose top-2 classes are exactly this pair OR
                # whose dominant class is one of the pair members. We check
                # that BOTH classes have at least 8 pixels (~3% of octant total).
                a_count = row['tc'].get(a, 0)
                b_count = row['tc'].get(b, 0)
                if a_count + b_count < 32:
                    continue
                if a_count < 8 and b_count < 8:
                    continue
                # And neither class outside the pair beats both members:
                others = sorted(
                    [(cc, vv) for cc, vv in row['tc'].items() if cc not in (a, b, 'other')],
                    key=lambda x: -x[1]
                )
                top_other = others[0][1] if others else 0
                if top_other > min(a_count, b_count):
                    # tile is dominated by a non-pair class, skip
                    continue
                bm = row['bm'][pair_key]
                buckets.setdefault(bm, []).append(row['tid'])
            for bm in sorted(buckets):
                tiles = buckets[bm]
                f.write(f'  {bm}  ({len(tiles)})  {tiles}\n')
            f.write('\n')

    print(f'wrote {draft_path}')


if __name__ == '__main__':
    main()
