$ErrorActionPreference = "Stop"

$buildDir = if ($env:BUILD_DIR) { $env:BUILD_DIR } else { "build/ci" }

function Test-CiPreset {
    param(
        [Parameter(Mandatory)]
        [ValidateSet("configure", "build")]
        [string] $Type
    )

    $presetList = & cmake "--list-presets=$Type" 2>$null | Out-String
    return $LASTEXITCODE -eq 0 -and $presetList -match '(?m)^\s*"ci"'
}

if (Test-CiPreset -Type configure) {
    Write-Host "Configuring with CMake preset 'ci'."
    & cmake --preset ci -DBUILD_TESTING=ON
} else {
    Write-Host "CMake configure preset 'ci' not found; using '$buildDir'."
    & cmake `
        -S . `
        -B $buildDir `
        -G Ninja `
        -DCMAKE_BUILD_TYPE=Release `
        -DBUILD_TESTING=ON
}

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

if (Test-CiPreset -Type build) {
    Write-Host "Building with CMake build preset 'ci'."
    & cmake --build --preset ci --parallel
} else {
    Write-Host "CMake build preset 'ci' not found; building '$buildDir'."
    & cmake --build $buildDir --config Release --parallel
}

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
