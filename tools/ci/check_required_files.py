#!/usr/bin/env python3
"""Check that explicitly listed project files and directories are usable."""

from __future__ import annotations

import argparse
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "paths",
        nargs="+",
        type=Path,
        help="Required files or non-empty directories, relative to the repository root.",
    )
    args = parser.parse_args()

    failures: list[str] = []
    for path in args.paths:
        if not path.exists():
            failures.append(f"missing: {path}")
        elif path.is_file() and path.stat().st_size == 0:
            failures.append(f"empty file: {path}")
        elif path.is_dir() and not any(item.is_file() for item in path.rglob("*")):
            failures.append(f"directory contains no files: {path}")
        else:
            print(f"OK: {path}")

    if failures:
        for failure in failures:
            print(f"ERROR: {failure}")
        return 1

    print(f"Checked {len(args.paths)} required paths.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
