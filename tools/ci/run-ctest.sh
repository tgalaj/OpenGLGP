#!/usr/bin/env bash

set -euo pipefail

build_dir="${BUILD_DIR:-build/ci}"

if cmake --list-presets=test 2>/dev/null |
  grep -Eq '(^|[[:space:]])"ci"'; then
  echo "Running CTest with test preset 'ci'."
  ctest --preset ci --output-on-failure
else
  echo "CMake test preset 'ci' not found; testing '$build_dir'."
  ctest \
    --test-dir "$build_dir" \
    --build-config Release \
    --output-on-failure
fi
