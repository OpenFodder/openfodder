"""V1 baseline-capture aggregator (companion to V1BaselineCapture.ps1).

Phase 1 deliverable P1.5b for the MapGen v3 rewrite (D11 §3.5).

The PowerShell entry point drives the engine and produces per-seed artefacts.
This script provides the four pure-Python orchestration primitives that the
PowerShell tool shells into, in the same style `SmokeRandomMaps.ps1:919-929`
shells out to `Tools/Corpus/GeneratedTacticsAudit.py`:

  manifest         build top-level manifest.json over seeds/**/record.json
  dedupe           sha256-verify a captured byte blob and copy into bytes/
  verify           re-hash the bytes referenced by one record.json
  aggregate-probes join probe JSON outputs (TurretSlotCheck, EnemyClearance,
                   ...) into a single audit summary

All subcommands are stdlib-only (hashlib, json, pathlib, argparse, shutil).
JSON is emitted with 2-space indent and sorted keys for stable diffs.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Any


# ---------------------------------------------------------------------------
# helpers
# ---------------------------------------------------------------------------

CHUNK = 1 << 20  # 1 MiB; stream-hash so a 100 MB corpus doesn't blow RAM


def sha256_file(path: Path) -> str:
    """Stream-hash a file with sha256, return lowercase hex digest."""
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(CHUNK), b""):
            h.update(chunk)
    return h.hexdigest()


def write_json(path: Path, data: Any) -> None:
    """Write a JSON document with stable formatting (2-space, sorted keys)."""
    path.parent.mkdir(parents=True, exist_ok=True)
    text = json.dumps(data, indent=2, sort_keys=True)
    # Trailing newline keeps git/POSIX tooling happy.
    path.write_text(text + "\n", encoding="utf-8")


def read_json(path: Path) -> Any:
    return json.loads(path.read_text(encoding="utf-8"))


def bytes_subpath(sha: str, ext: str) -> Path:
    """Layout used by D11 §2: bytes/<2-char prefix>/<full-sha>.<ext>."""
    sha = sha.lower()
    if len(sha) != 64 or not all(c in "0123456789abcdef" for c in sha):
        raise ValueError(f"not a sha256 hex digest: {sha!r}")
    ext = ext.lstrip(".")
    return Path(sha[:2]) / f"{sha}.{ext}"


def err(msg: str) -> None:
    print(f"V1BaselineCapture: {msg}", file=sys.stderr)


# ---------------------------------------------------------------------------
# subcommand: manifest
# ---------------------------------------------------------------------------

def cmd_manifest(args: argparse.Namespace) -> int:
    """Walk seeds/**/record.json and assemble the top-level manifest.

    The manifest is the single read-once index a verify run consults; it
    embeds the capture identity (tag SHA, exe SHA, captured-at-utc) and the
    full per-seed matrix. Per D11 §3.3 it lives at
    Tools/Verify/V1Baseline/manifest.json.
    """
    baseline_dir: Path = args.baseline_dir.resolve()
    seeds_dir = baseline_dir / "seeds"

    if not seeds_dir.is_dir():
        err(f"seeds directory not found: {seeds_dir}")
        return 2

    records: list[dict[str, Any]] = []
    profiles: dict[str, int] = {}
    seed_ids: set[int] = set()
    skipped: list[str] = []

    # Sort for deterministic manifest ordering — matches the on-disk layout
    # seeds/<profile>/<seed>/record.json.
    for record_path in sorted(seeds_dir.rglob("record.json")):
        try:
            rec = read_json(record_path)
        except (OSError, json.JSONDecodeError) as exc:
            skipped.append(f"{record_path}: {exc}")
            continue

        profile = rec.get("profile", "<unknown>")
        seed = rec.get("seed")
        rel = record_path.relative_to(baseline_dir).as_posix()

        entry: dict[str, Any] = {
            "profile":    profile,
            "seed":       seed,
            "kind":       rec.get("kind"),
            "terrain":    rec.get("terrain"),
            "terrainSub": rec.get("terrainSub"),
            "schema":     rec.get("schema"),
            "recordPath": rel,
            "output":     rec.get("output", {}),
        }
        # Optional v3-only / D10 extension fields — None on v1 records.
        for k in ("conceptId", "conceptIntent", "conceptMatch"):
            if k in rec:
                entry[k] = rec[k]

        records.append(entry)
        profiles[profile] = profiles.get(profile, 0) + 1
        if isinstance(seed, int):
            seed_ids.add(seed)

    manifest = {
        "schema":          1,
        "tag":             "v1-mapgen-baseline-pre-strip",
        "tagSha":          args.tag_sha,
        "capturedAtUtc":   args.captured_at_utc,
        "exe": {
            "path":   "Run/openfodder.exe",
            "sha256": args.exe_sha,
        },
        "counts": {
            "records":  len(records),
            "profiles": len(profiles),
            "seeds":    len(seed_ids),
        },
        "profiles": profiles,
        "records":  records,
        "skipped":  skipped,
    }

    output: Path = args.output.resolve()
    write_json(output, manifest)
    print(f"wrote {output} ({len(records)} records, {len(profiles)} profiles)")
    if skipped:
        print(f"skipped {len(skipped)} unreadable records")
    return 0


# ---------------------------------------------------------------------------
# subcommand: dedupe
# ---------------------------------------------------------------------------

def cmd_dedupe(args: argparse.Namespace) -> int:
    """Hash-verify a captured artefact and copy it into the dedup store.

    Returns 0 on success (whether it was already present or newly stored),
    non-zero if the source bytes don't match the asserted sha256 — this is
    the contract that lets the PowerShell driver pass a hash it computed
    once and trust this script to refuse silent corruption.
    """
    bytes_dir: Path = args.bytes_dir.resolve()
    source: Path = args.source.resolve()
    expected = args.input.lower()
    ext = args.ext.lstrip(".")

    if not source.is_file():
        err(f"source artefact not found: {source}")
        return 2

    actual = sha256_file(source)
    if actual != expected:
        err(
            f"sha256 mismatch for {source}: "
            f"expected {expected}, got {actual}"
        )
        return 3

    dest = bytes_dir / bytes_subpath(actual, ext)
    if dest.exists():
        # Cheap re-hash to defend against an out-of-band corrupted store.
        existing = sha256_file(dest)
        if existing != actual:
            err(
                f"existing {dest} hashes {existing}, "
                f"expected {actual} — corrupt store"
            )
            return 4
        print(f"present  {dest.relative_to(bytes_dir).as_posix()}")
        return 0

    dest.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, dest)
    # Re-hash the copy: we just wrote bytes; if the filesystem lied, fail
    # now rather than at verify time months later.
    after = sha256_file(dest)
    if after != actual:
        err(f"copy verification failed for {dest}: {after} != {actual}")
        try:
            dest.unlink()
        except OSError:
            pass
        return 5
    print(f"stored   {dest.relative_to(bytes_dir).as_posix()}")
    return 0


# ---------------------------------------------------------------------------
# subcommand: verify
# ---------------------------------------------------------------------------

def cmd_verify(args: argparse.Namespace) -> int:
    """Re-hash every bytes/ artefact referenced by one record.json.

    This is the per-record half of the §5 verify procedure. The §5 driver
    walks every record and calls this; a single mismatch fails the gate.
    """
    baseline_dir: Path = args.baseline_dir.resolve()
    record_rel = Path(args.record_path)
    record_path = (baseline_dir / record_rel).resolve()

    if not record_path.is_file():
        err(f"record not found: {record_path}")
        return 2

    try:
        rec = read_json(record_path)
    except (OSError, json.JSONDecodeError) as exc:
        err(f"unreadable record {record_path}: {exc}")
        return 2

    output = rec.get("output", {})
    files = rec.get("files", {})

    # (file-key in record.files, hash-key in record.output) pairs the schema
    # actually pins. Sidecars (mapgenJson, contextJson) live next to the
    # record itself and aren't dedup-stored, so they're not hash-checked
    # here — verify presence only.
    hashed_pairs = [
        ("map",        "mapHash"),
        ("spt",        "sptHash"),
        ("render",     "renderHash"),
        ("iceCharmap", "iceCharmapHash"),
    ]
    sidecar_keys = ("mapgenJson", "contextJson")

    failures: list[str] = []
    checked: list[dict[str, Any]] = []

    for file_key, hash_key in hashed_pairs:
        rel = files.get(file_key)
        expected = output.get(hash_key)
        if rel is None and expected is None:
            continue  # field genuinely absent (e.g. iceCharmap on jungle)
        if rel is None:
            failures.append(f"{file_key}: no path but hash {expected!r} present")
            continue
        if expected is None:
            failures.append(f"{file_key}: path {rel!r} but no hash")
            continue

        # files[*] paths in record.json are stored relative to the record's
        # own directory (e.g. "../../bytes/aa/<sha>.map") — resolve against
        # the record, not the baseline root.
        artefact = (record_path.parent / rel).resolve()
        if not artefact.is_file():
            failures.append(f"{file_key}: missing {artefact}")
            continue

        actual = sha256_file(artefact)
        ok = actual == expected.lower()
        checked.append({
            "kind":     file_key,
            "path":     str(artefact),
            "expected": expected,
            "actual":   actual,
            "ok":       ok,
        })
        if not ok:
            failures.append(
                f"{file_key}: hash mismatch at {artefact} "
                f"(expected {expected}, got {actual})"
            )

    for key in sidecar_keys:
        rel = files.get(key)
        if rel is None:
            continue
        sidecar = (record_path.parent / rel).resolve()
        if not sidecar.is_file():
            failures.append(f"{key}: missing sidecar {sidecar}")
        else:
            checked.append({
                "kind": key,
                "path": str(sidecar),
                "ok":   True,
            })

    summary = {
        "record":   str(record_path),
        "profile":  rec.get("profile"),
        "seed":     rec.get("seed"),
        "checked":  checked,
        "failures": failures,
        "ok":       not failures,
    }
    print(json.dumps(summary, indent=2, sort_keys=True))
    return 0 if not failures else 1


# ---------------------------------------------------------------------------
# subcommand: aggregate-probes
# ---------------------------------------------------------------------------

def cmd_aggregate_probes(args: argparse.Namespace) -> int:
    """Join multiple probe JSON outputs into one audit summary.

    Each probe (TurretSlotCheck, EnemyClearanceAudit, ...) emits its own
    schema; this aggregator does NOT impose a unified shape, only indexes
    them by probe filename and surfaces a derived `ok` rollup so the
    PowerShell driver can fail the gate without re-parsing each probe.
    """
    baseline_dir: Path = args.baseline_dir.resolve()
    audit_dir = baseline_dir / "audit"

    probes: dict[str, Any] = {}
    failures: list[str] = []
    overall_ok = True

    for probe_path_arg in args.probe_output:
        probe_path = probe_path_arg.resolve()
        name = probe_path.stem  # e.g. turret_slot, enemy_clearance

        if not probe_path.is_file():
            failures.append(f"{name}: missing {probe_path}")
            overall_ok = False
            probes[name] = {
                "path":   str(probe_path),
                "loaded": False,
                "error":  "missing",
            }
            continue

        try:
            data = read_json(probe_path)
        except (OSError, json.JSONDecodeError) as exc:
            failures.append(f"{name}: unreadable ({exc})")
            overall_ok = False
            probes[name] = {
                "path":   str(probe_path),
                "loaded": False,
                "error":  str(exc),
            }
            continue

        # Probe outputs may report status under several conventional keys;
        # treat anything explicitly false as a failure, but don't invent a
        # failure when the probe doesn't ship an `ok` field at all.
        probe_ok: bool | None = None
        for key in ("ok", "passed", "success"):
            if isinstance(data, dict) and key in data:
                probe_ok = bool(data[key])
                break
        if probe_ok is False:
            overall_ok = False
            failures.append(f"{name}: probe reports failure")

        probes[name] = {
            "path":   str(probe_path),
            "loaded": True,
            "ok":     probe_ok,
            "data":   data,
        }

    summary = {
        "schema":         1,
        "generatedAtUtc": datetime.now(timezone.utc)
                          .replace(microsecond=0)
                          .isoformat()
                          .replace("+00:00", "Z"),
        "baselineDir":    str(baseline_dir),
        "ok":             overall_ok,
        "probes":         probes,
        "failures":       failures,
    }

    output = audit_dir / "probes_summary.json"
    write_json(output, summary)
    print(f"wrote {output} ({len(probes)} probes, ok={overall_ok})")
    return 0 if overall_ok else 1


# ---------------------------------------------------------------------------
# CLI plumbing
# ---------------------------------------------------------------------------

def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="V1BaselineCapture.py",
        description="V1 baseline-capture aggregator (D11 §3.5).",
    )
    sub = parser.add_subparsers(dest="cmd", required=True)

    # manifest -----------------------------------------------------------
    p_manifest = sub.add_parser(
        "manifest",
        help="Build top-level manifest.json over seeds/**/record.json",
    )
    p_manifest.add_argument("--baseline-dir", type=Path, required=True,
                            help="Tools/Verify/V1Baseline (root)")
    p_manifest.add_argument("--tag-sha", required=True,
                            help="git rev-list output for the baseline tag")
    p_manifest.add_argument("--exe-sha", required=True,
                            help="sha256 of Run/openfodder.exe used to capture")
    p_manifest.add_argument("--captured-at-utc", required=True,
                            help="ISO-8601 UTC timestamp of the capture run")
    p_manifest.add_argument("--output", type=Path, required=True,
                            help="Where to write manifest.json")
    p_manifest.set_defaults(func=cmd_manifest)

    # dedupe -------------------------------------------------------------
    p_dedupe = sub.add_parser(
        "dedupe",
        help="sha256-verify and copy a captured artefact into bytes/<prefix>/",
    )
    p_dedupe.add_argument("--bytes-dir", type=Path, required=True,
                          help="Tools/Verify/V1Baseline/bytes")
    p_dedupe.add_argument("--input", required=True,
                          help="Expected sha256 hex of the source artefact")
    p_dedupe.add_argument("--source", type=Path, required=True,
                          help="Path to the original artefact (.map/.spt/...)")
    p_dedupe.add_argument("--ext", required=True,
                          choices=["map", "spt", "png", "txt"],
                          help="Extension to use for the dedup-stored copy")
    p_dedupe.set_defaults(func=cmd_dedupe)

    # verify -------------------------------------------------------------
    p_verify = sub.add_parser(
        "verify",
        help="Hash-check every bytes/ artefact referenced by one record",
    )
    p_verify.add_argument("--baseline-dir", type=Path, required=True,
                          help="Tools/Verify/V1Baseline (root)")
    p_verify.add_argument("--record-path", required=True,
                          help="Record path relative to baseline-dir, "
                               "e.g. seeds/grammar_ice/4242/record.json")
    p_verify.set_defaults(func=cmd_verify)

    # aggregate-probes ---------------------------------------------------
    p_probes = sub.add_parser(
        "aggregate-probes",
        help="Join probe JSON outputs into audit/probes_summary.json",
    )
    p_probes.add_argument("--baseline-dir", type=Path, required=True,
                          help="Tools/Verify/V1Baseline (root)")
    p_probes.add_argument("--probe-output", type=Path, action="append",
                          required=True,
                          help="Probe JSON output (repeatable)")
    p_probes.set_defaults(func=cmd_aggregate_probes)

    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
