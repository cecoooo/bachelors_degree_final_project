cmake_minimum_required(VERSION 3.13)

add_executable(read_ain main.cpp)
target_link_libraries(read_ain PUBLIC gpio_wrapper gpio_lib)