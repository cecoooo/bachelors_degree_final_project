cmake_minimum_required(VERSION 3.13)

# Collect source and header files
file(GLOB_RECURSE CLIENT_SRC "source/*.cpp")
file(GLOB_RECURSE CLIENT_HEADERS "include/*.hpp")

# Create the client library
add_library(client ${CLIENT_SRC} ${CLIENT_HEADERS})

# Include own headers
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link to interface libraries provided in dependencies
target_link_libraries(client PUBLIC gpio_wrapper gpio_lib)
