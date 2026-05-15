"""Emit JS rule blocks for IceData.js using the user's canonical tile ranges.

Reads `IceTilePixelTable.txt` (every tile's bm for every chain pair) and, for
each chain pair, picks the bm signature of every tile that falls inside the
user-specified canonical range — regardless of which pair the auto-classifier
assigned. This is required because tiles 80-99 (Ice/WaterEdge DUAL) often have
dominant snow+ice pixel content but are still part of the ice/shallow
transition strip in the retail tilesheet.

Output: Documentation/IceBmRulesEmitted.txt
"""
import os
import re

HERE = os.path.dirname(os.path.abspath(__file__))
TABLE = os.path.normpath(os.path.join(HERE, '..', 'Documentation', 'IceTilePixelTable.txt'))
OUT = os.path.normpath(os.path.join(HERE, '..', 'Documentation', 'IceBmRulesEmitted.txt'))

TILE_LINE = re.compile(r'^tile\s+(\d+)\s+.*$')
BM_LINE = re.compile(
    r'^\s+bm\s+snow_ice=(\d{8})\s+ice_shallow=(\d{8})\s+shallow_deep=(\d{8})\s*$'
)

# Canonical Cannon Fodder ICE atlas regions (user-supplied):
#   Snow & Ice           : 0-25, 33-34, 42-44, 62-63
#   Ice & Water (shallow): 80-99, 112-113, 128-131, 148-151
#   Water shallow & deep : 100-111, 114-119, 132-139, 156-159, 176-179
TILE_RANGES = {
    'snow_ice':     [(0, 25), (33, 34), (42, 44), (62, 63)],
    'ice_shallow':  [(80, 99), (112, 113), (128, 131), (148, 151)],
    'shallow_deep': [(100, 111), (114, 119), (132, 139), (156, 159), (176, 179)],
}

# Required minimum pixel count of each member class for a tile to be emitted.
# Pure-fill tiles (all snow, all deep, etc.) get bm 11111111 / 00000000 and
# aren't useful as transition tiles, so we filter them out per pair.
PAIR_CLASSES = {
    'snow_ice':     ('snow', 'ice'),
    'ice_shallow':  ('ice', 'shallow'),
    'shallow_deep': ('shallow', 'deep'),
}
MIN_CLASS_PIXELS = 8


def in_any_range(tid, ranges):
    return any(lo <= tid <= hi for lo, hi in ranges)


def parse_table():
    """Return dict tid -> {bm_pair: bmstring, classes: {snow:N,ice:N,shallow:N,deep:N,other:N}}."""
    tiles = {}
    cur_tid = None
    cur_classes = None
    class_re = re.compile(
        r'snow:(\d+)\s+ice:(\d+)\s+shallow:(\d+)\s+deep:(\d+)\s+other:(\d+)'
    )
    with open(TABLE) as f:
        for line in f:
            m = TILE_LINE.match(line)
            if m:
                cur_tid = int(m.group(1))
                cm = class_re.search(line)
                if cm:
                    cur_classes = {
                        'snow':    int(cm.group(1)),
                        'ice':     int(cm.group(2)),
                        'shallow': int(cm.group(3)),
                        'deep':    int(cm.group(4)),
                        'other':   int(cm.group(5)),
                    }
                else:
                    cur_classes = {}
                continue
            m = BM_LINE.match(line)
            if m and cur_tid is not None:
                tiles[cur_tid] = {
                    'bm': {
                        'snow_ice':     m.group(1),
                        'ice_shallow':  m.group(2),
                        'shallow_deep': m.group(3),
                    },
                    'classes': cur_classes or {},
                }
                cur_tid = None
                cur_classes = None
    return tiles


def emit_block(pair, tiles_data):
    rule = f'bm_cf1_{pair}'
    ranges = TILE_RANGES[pair]
    cls_a, cls_b = PAIR_CLASSES[pair]

    bm_to_tiles = {}
    skipped_pure = []
    for tid in sorted(tiles_data.keys()):
        if not in_any_range(tid, ranges):
            continue
        rec = tiles_data[tid]
        cls = rec['classes']
        a = cls.get(cls_a, 0)
        b = cls.get(cls_b, 0)
        # Drop pure-fill tiles - we want transition tiles only.
        if a < MIN_CLASS_PIXELS or b < MIN_CLASS_PIXELS:
            skipped_pure.append(tid)
            continue
        bm = rec['bm'][pair]
        bm_to_tiles.setdefault(bm, []).append(tid)

    out = [f'{rule}: {{', '    bitmask: [']
    body = []
    for bm in sorted(bm_to_tiles):
        tile_objs = ', '.join(f'{{ tile: "{t}" }}' for t in bm_to_tiles[bm])
        body.append(f'        {{ bm: "{bm}", tiles: [{tile_objs}] }},')
    if body:
        body[-1] = body[-1].rstrip(',')
    out.extend(body)
    out.append('    ]')
    out.append('},')
    return out, skipped_pure


def main():
    tiles = parse_table()
    out_lines = []
    summary = {}
    for pair in ['shallow_deep', 'ice_shallow', 'snow_ice']:
        block, skipped = emit_block(pair, tiles)
        out_lines.extend(block)
        out_lines.append('')
        summary[pair] = {
            'kept_bms': sum(1 for ln in block if ln.lstrip().startswith('{ bm:')),
            'skipped_pure': skipped,
        }

    with open(OUT, 'w') as f:
        f.write('\n'.join(out_lines))
    print('wrote', OUT)
    for pair, s in summary.items():
        print(f'  {pair}: bms={s["kept_bms"]}  skipped_pure={s["skipped_pure"]}')


if __name__ == '__main__':
    main()
