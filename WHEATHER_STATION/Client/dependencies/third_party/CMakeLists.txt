cmake_minimum_required(VERSION 3.13)

set(lib_name gpio_lib)

add_library(${lib_name} INTERFACE)

target_include_directories(${lib_name} INTERFACE 
				"${CMAKE_CURRENT_SOURCE_DIR}/include"
				"${CMAKE_CURRENT_SOURCE_DIR}")
  
target_sources(${lib_name} INTERFACE 
		"${CMAKE_CURRENT_SOURCE_DIR}/common.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/library/gpio.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/library/bbio.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/library/pwm.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/c_adc.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/c_pinmux.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/c_pwm.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/c_uart.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/event_gpio.c")
		
target_compile_definitions(${lib_name} INTERFACE NO_PYTHON)
target_compile_definitions(${lib_name} INTERFACE BBBVERSION41)
target_link_options(${lib_name} INTERFACE -pthread)