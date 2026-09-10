@echo off
setlocal

set "PRESET=%~1"
if not defined PRESET set "PRESET=dev"

cmake --preset "%PRESET%" || exit /b %ERRORLEVEL%
cmake --build --preset "%PRESET%" || exit /b %ERRORLEVEL%