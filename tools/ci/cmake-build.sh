#!/usr/bin/env bash

set -euo pipefail

build_dir="${BUILD_DIR:-build/ci}"

has_ci_preset() {
  local preset_type="$1"
  cmake --list-presets="$preset_type" 2>/dev/null |
    grep -Eq '(^|[[:space:]])"ci"'
}

configure_options=(-DBUILD_TESTING=ON)
if command -v ccache >/dev/null 2>&1; then
  configure_options+=(
    -DCMAKE_C_COMPILER_LAUNCHER=ccache
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
  )
fi

if has_ci_preset configure; then
  echo "Configuring with CMake preset 'ci'."
  cmake --preset ci "${configure_options[@]}"
else
  echo "CMake configure preset 'ci' not found; using '$build_dir'."
  cmake \
    -S . \
    -B "$build_dir" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    "${configure_options[@]}"
fi

if has_ci_preset build; then
  echo "Building with CMake build preset 'ci'."
  cmake --build --preset ci --parallel
else
  echo "CMake build preset 'ci' not found; building '$build_dir'."
  cmake --build "$build_dir" --config Release --parallel
fi
