@echo off
setlocal

REM Navigate to project root and then to build folder
cd /d "%~dp0\..\.."
if not exist build (
    echo Build folder not found. Please build the project first.
    goto end
)

cd build

REM Check and run ServerGUI.exe
if exist ServerGUI.exe (
    echo Running ServerGUI.exe ...
    start "" ServerGUI.exe
) else (
    echo ServerGUI.exe not found. Make sure the project is built.
)

:end
endlocal
