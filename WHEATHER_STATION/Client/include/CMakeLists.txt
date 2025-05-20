cmake_minimum_required(VERSION 3.13)

set (lib_name AQI)

add_library(${lib_name} INTERFACE)
target_include_directories(${lib_name} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(${lib_name} INTERFACE libaqidetect_amd64.o)
