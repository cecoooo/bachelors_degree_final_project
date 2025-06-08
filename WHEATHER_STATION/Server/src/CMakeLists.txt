cmake_minimum_required(VERSION 3.16)
project(WeatherStationServer)

set(CMAKE_CXX_STANDARD 17)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
file(GLOB_RECURSE SOURCES 
    "source/*.cpp" 
    "gui/*.cpp" 
    "database/*.cpp" 
    "main.cpp"
)
file(GLOB_RECURSE HEADERS 
    "include/*.hpp" 
    "gui/*.hpp" 
    "database/*.hpp"
)
file(GLOB UI_FILES "gui/*.ui")

add_executable(ServerGUI ${SOURCES} ${HEADERS} ${UI_FILES})

# Include directories
target_include_directories(ServerGUI PRIVATE
    include
    gui
    database
)

# Link libraries
target_link_libraries(ServerGUI PRIVATE
    Qt6::Widgets
    Qt6::Charts
    odbc32
    ws2_32
    crypt32
    advapi32
    userenv
    bcrypt
)

target_compile_definitions(ServerGUI PRIVATE NOMINMAX)