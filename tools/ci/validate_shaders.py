#!/usr/bin/env python3
"""Validate non-empty, stage-specific GLSL files without generating SPIR-V."""

from __future__ import annotations

import argparse
from pathlib import Path
import shutil
import subprocess


STAGES = {
    ".vert": "vert",
    ".frag": "frag",
    ".geom": "geom",
    ".tesc": "tesc",
    ".tese": "tese",
    ".comp": "comp",
}

EXCLUDED_DIRECTORIES = {".cache", ".git", "build", "thirdparty"}


def is_project_file(path: Path, root: Path) -> bool:
    relative_parts = path.relative_to(root).parts
    return not any(part in EXCLUDED_DIRECTORIES for part in relative_parts)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("root", nargs="?", type=Path, default=Path("."))
    args = parser.parse_args()

    root = args.root.resolve()
    shaders = sorted(
        path
        for path in root.rglob("*")
        if path.is_file()
        and path.suffix.lower() in STAGES
        and path.stat().st_size > 0
        and is_project_file(path, root)
    )

    if not shaders:
        print("No non-empty stage-specific GLSL shaders found; skipping validation.")
        return 0

    validator = shutil.which("glslangValidator")
    if validator is None:
        print("ERROR: glslangValidator is required when shaders are present.")
        return 1

    failures = 0
    for shader in shaders:
        relative_path = shader.relative_to(root)
        stage = STAGES[shader.suffix.lower()]
        print(f"Validating {relative_path} as {stage}.", flush=True)
        result = subprocess.run(
            [validator, "-S", stage, str(relative_path)],
            cwd=root,
            check=False,
        )
        if result.returncode != 0:
            failures += 1

    if failures:
        print(f"ERROR: {failures} of {len(shaders)} shaders failed validation.")
        return 1

    print(f"Validated {len(shaders)} shaders.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
