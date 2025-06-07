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




@REM @echo off
@REM setlocal

@REM REM Navigate to build folder
@REM cd ../../build

@REM REM Check if ServerGUI.exe exists and run it
@REM if exist ServerGUI.exe (
@REM     echo Running ServerGUI.exe ...
@REM     ServerGUI.exe
@REM ) else (
@REM     echo ServerGUI.exe not found. Make sure the project is built.
@REM )

@REM :end
@REM endlocal
