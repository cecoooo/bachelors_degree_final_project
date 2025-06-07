@echo off
setlocal

REM ---- CONFIGURATION ----
set QT_PATH=C:\Qt\6.9.1\msvc2022_64
set BUILD_DIR=build
set GENERATOR=NMake Makefiles

REM ---- PROJECT ROOT ----
cd /d %~dp0
cd ../..

REM ---- Kill running executable (optional safety)
taskkill /f /im ServerGUI.exe >nul 2>&1

REM ---- Delete previous build ----
if exist %BUILD_DIR% (
    echo Deleting existing build directory...
    rmdir /s /q %BUILD_DIR%
)

REM ---- Create and enter build directory ----
mkdir %BUILD_DIR%
cd %BUILD_DIR%

REM ---- Run CMake configuration ----
cmake ../src -G "%GENERATOR%" -DCMAKE_PREFIX_PATH="%QT_PATH%/lib/cmake" -DCMAKE_BUILD_TYPE=Release

REM ---- Build the project ----
cmake --build .

REM ---- Deploy Qt DLLs if build succeeded ----
if exist ServerGUI.exe (
    echo Running windeployqt to copy required Qt DLLs...
    "%QT_PATH%\bin\windeployqt.exe" ServerGUI.exe
) else (
    echo ServerGUI.exe not found. Skipping windeployqt.
)

:end
endlocal
pause
