"""Validate and ZIP a demo-only Emscripten distribution."""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import zipfile

from package_assets import DEMOS

WEB_FILES = ("OpenFodder.html", "OpenFodder.js", "OpenFodder.wasm", "OpenFodder.data")
README = """Open Fodder browser build

Extract this ZIP, then serve this directory over HTTP:

    python3 -m http.server 8080 --bind 127.0.0.1

Open http://localhost:8080/OpenFodder.html and click inside the game.
On Windows, 'python' can be used instead of 'python3'.
Opening OpenFodder.html directly as a local file will not work.
Keep the HTML, JavaScript, WebAssembly, and data files together.

Includes the seven demo releases, scripts, and PC/Amiga audio support.
Retail data, desktop binaries, and local saves are excluded.
Saves are stored in the browser's IndexedDB for this site's origin.

Source and build instructions: https://github.com/OpenFodder/openfodder
License notices: COPYING and licenses/
"""


def main():
    root = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, required=True)
    parser.add_argument("--runtime", type=Path, default=root / "RunE")
    parser.add_argument("--version", default="latest")
    parser.add_argument("--output-dir", type=Path, default=root / "build" / "web-dist")
    args = parser.parse_args()
    if not re.fullmatch(r"latest|[0-9]+\.[0-9]+\.[0-9]+(?:[-+][A-Za-z0-9.-]+)?", args.version):
        parser.error("Version must be 'latest' or a semantic version")

    files = {name: args.runtime / name for name in WEB_FILES}
    files["COPYING"] = root / "COPYING"
    for name in ("SDL3", "SDL3_mixer", "libxmp"):
        files[f"licenses/{name}.txt"] = args.build_dir / "web-licenses" / f"{name}.txt"
    for name, path in files.items():
        if not path.is_file() or not path.stat().st_size:
            parser.error(f"Missing or empty distribution file: {name} ({path})")
    if files["OpenFodder.wasm"].read_bytes()[:8] != b"\0asm\x01\0\0\0":
        parser.error("The WebAssembly output is invalid")
    script = files["OpenFodder.js"].read_text(encoding="utf-8")
    if "_openfodder_web_state" in script or "_openfodder_web_play_audio" in script:
        parser.error("Build with OPENFODDER_WEB_DIAGNOSTICS=OFF before packaging")

    assets = args.build_dir / "web-assets"
    manifest = json.loads((assets / "manifest.json").read_text(encoding="utf-8"))
    if not manifest:
        parser.error("The browser asset manifest is empty")
    data_size = 0
    for name, expected_hash in manifest.items():
        parts = PurePosixPath(name).parts
        if (not parts or ".." in parts or name.startswith("/") or "\\" in name
                or parts[0] not in ("Data", "Scripts", "Campaigns", "about.png", "openfodder.ini")
                or (parts[0] == "Data" and (len(parts) < 3 or parts[1] not in DEMOS))):
            parser.error(f"Unexpected or non-demo asset: {name}")
        content = (assets / "runtime" / name).read_bytes()
        if hashlib.sha256(content).hexdigest() != expected_hash:
            parser.error(f"Asset changed after the build: {name}")
        data_size += len(content)
    if files["OpenFodder.data"].stat().st_size != data_size:
        parser.error("The data bundle does not match the asset manifest; rebuild first")

    args.output_dir.mkdir(parents=True, exist_ok=True)
    archive = args.output_dir / f"OpenFodder-{args.version}-emscripten.zip"
    with zipfile.ZipFile(archive, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=9) as bundle:
        for name, path in files.items():
            bundle.write(path, name)
        bundle.writestr("README-web.txt", README)
    with zipfile.ZipFile(archive) as bundle:
        if bundle.testzip() is not None:
            raise RuntimeError("Archive integrity check failed")
    print(f"Packaged {archive} ({archive.stat().st_size:,} bytes)")


if __name__ == "__main__":
    main()
