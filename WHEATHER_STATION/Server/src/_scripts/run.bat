@echo off
setlocal

REM Navigate to build folder
cd ../../build/Debug

REM Find the first .exe file in the build directory
for %%f in (*.exe) do (
    echo Running %%f ...
    %%f
    goto :end
)

REM If no build is made
echo No .exe file found in the build folder.

:end
endlocal