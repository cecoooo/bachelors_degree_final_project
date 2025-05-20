cmake_minimum_required(VERSION 3.21)
project(WEATHER_STATION)

set(CMAKE_CXX_STANDARD 17)

# Add include directory for header file visibility
include_directories(${CMAKE_SOURCE_DIR}/include)

# Update source list with correct paths
set(SOURCES
    source/ClientConnectingException.cpp
    source/ClientConnection.cpp
    source/ClientContainer.cpp
    source/CustomMutex.cpp
    source/ErrorInClientException.cpp
    source/FileOpeningException.cpp
    main.cpp
    source/Printer.cpp
    source/RequestToClient.cpp
    source/Server.cpp
    source/ServerListeningException.cpp
    source/SocketBindingException.cpp
    source/SocketCreatingException.cpp
    source/WinsockInitializingException.cpp
)

# Add executable with updated source list
add_executable(WEATHER_STATION ${SOURCES})

# Allow using std::max, etc. safely with Windows headers
add_definitions(-DNOMINMAX)
