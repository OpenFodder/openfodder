"""Export the game's browser runtime without host files or local saves."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

DEMOS = ("AmigaAction", "AmigaAlienLevels", "AmigaFormat_NVF",
         "AmigaFormat_XMAS", "AmigaTheOne", "PCFormat", "Plus", "Custom")
RETAIL = ("Amiga", "Amiga_CD", "Amiga2", "Dos_CD", "Dos2_CD")
DEMO_CAMPAIGNS = ("Amiga Action", "Amiga Power Alien Levels", "Amiga Format Not Very Festive",
                  "Amiga Format Christmas Special", "Amiga The One", "PC Format", "Cannon Fodder Plus")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--retail", action="store_true")
    args = parser.parse_args()
    source, output = args.source.resolve(), args.output.resolve()
    if source == output or source in output.parents or output in source.parents:
        parser.error("The asset output must be separate from the runtime source")
    files = {}
    for name in DEMOS + (RETAIL if args.retail else ()):
        folder = source / "Data" / name
        if folder.is_dir():
            for path in folder.rglob("*"):
                if path.is_file() and not any(p.startswith(".") for p in path.relative_to(folder).parts):
                    files[path.relative_to(source).as_posix()] = path
    for folder_name in ("Scripts", "Campaigns"):
        folder = source / folder_name
        for path in folder.rglob("*"):
            parts = path.relative_to(folder).parts
            if (path.is_file() and not any(p.startswith(".") for p in parts)
                    and parts[0] not in ("Tests", "Tools", "__pycache__")
                    and path.suffix.lower() in (".js", ".json", ".xml", ".map", ".spt", ".ofc")):
                files[path.relative_to(source).as_posix()] = path
    files["about.png"] = source / "about.png"
    files["openfodder.ini"] = Path(__file__).with_name("openfodder.ini")
    required = [f"Campaigns/{name}.ofc" for name in DEMO_CAMPAIGNS]
    required += ["Scripts/Settings.js", "Scripts/Common/OpenFodder.js", "Scripts/random.js", "about.png"]
    missing = [name for name in required if name not in files or not files[name].is_file()]
    missing += [f"Data/{name}" for name in DEMOS if name != "Custom"
                and not any(path.startswith(f"Data/{name}/") for path in files)]
    if missing:
        parser.error("Missing browser runtime assets: " + ", ".join(missing)
                     + ". Initialize the Run submodule and its Scripts submodule.")
    runtime = output / "runtime"
    runtime.mkdir(parents=True, exist_ok=True)
    # Synchronize only the exporter-owned runtime folder, including removals.
    for path in runtime.rglob("*"):
        if path.is_file() and path.relative_to(runtime).as_posix() not in files:
            path.unlink()
    for name, path in sorted(files.items()):
        dest = runtime / name
        if not dest.exists() or dest.read_bytes() != path.read_bytes():
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(path, dest)
    # mtime is also the link dependency: relink only when the bundle changes.
    manifest = {name: hashlib.sha256((runtime / name).read_bytes()).hexdigest()
                for name in sorted(files)}
    manifest_text = json.dumps(manifest, indent=2) + "\n"
    manifest_path = output / "manifest.json"
    if not manifest_path.exists() or manifest_path.read_text() != manifest_text:
        manifest_path.write_text(manifest_text)
    print(f"Browser bundle: {len(files)} files, "
          f"{sum(p.stat().st_size for p in runtime.rglob('*') if p.is_file()):,} bytes")


if __name__ == "__main__":
    main()
