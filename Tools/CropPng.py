"""Crop sub-regions from a PNG and save them."""
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    sys.stderr.write("Pillow is required: pip install pillow\n")
    sys.exit(1)


def main(args):
    if len(args) < 4:
        print("usage: CropPng.py <src> <dst> <x> <y> [w] [h]")
        return 2

    src = Path(args[0])
    dst = Path(args[1])
    x = int(args[2])
    y = int(args[3])
    w = int(args[4]) if len(args) > 4 else 200
    h = int(args[5]) if len(args) > 5 else 200

    img = Image.open(src)
    crop = img.crop((x, y, x + w, y + h))
    crop.save(dst)
    print(f"wrote={dst} crop=({x},{y},{w}x{h}) src={img.size}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
