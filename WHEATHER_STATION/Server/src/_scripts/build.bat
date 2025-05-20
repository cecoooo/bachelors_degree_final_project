cd ../..

REM Delete build directory if exists
if exist build (
    echo Deleting existing build directory...
    rmdir /s /q build
)

REM Create and change directory to build
mkdir build
cd build

REM Run CMake configuration (adjust if needed)
cmake ../src

REM Build the project
cmake --build .