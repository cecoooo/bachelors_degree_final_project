cmake_minimum_required(VERSION 3.16)
project(WeatherStationServer)

set(CMAKE_CXX_STANDARD 17)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

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
    odbc32
    ws2_32
    crypt32
    advapi32
    userenv
    bcrypt
)

target_compile_definitions(ServerGUI PRIVATE NOMINMAX)











# cmake_minimum_required(VERSION 3.16)
# project(WeatherStationServer)

# set(CMAKE_CXX_STANDARD 17)

# # Qt6 setup
# find_package(Qt6 REQUIRED COMPONENTS Widgets)

# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

# # Collect files
# file(GLOB_RECURSE SOURCES "source/*.cpp" "gui/*.cpp" "main.cpp")
# file(GLOB_RECURSE HEADERS "include/*.hpp" "gui/*.hpp")
# file(GLOB UI_FILES "gui/*.ui")

# # Create the executable
# add_executable(ServerGUI ${SOURCES} ${HEADERS} ${UI_FILES})

# # Includes
# target_include_directories(ServerGUI PRIVATE include gui)

# # Link Qt6
# target_link_libraries(ServerGUI PRIVATE Qt6::Widgets)
# target_link_libraries(ServerGUI PRIVATE ws2_32)
