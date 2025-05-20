# Add source directories
add_subdirectory(dependencies)
add_subdirectory(src)

# Add executable that uses the client library
add_executable(run src/main.cpp)
target_link_libraries(run PRIVATE client)
