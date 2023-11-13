cmake_minimum_required(VERSION 3.21)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_hal C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
    -Wno-format          
    -Wno-unused-function
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../../../RadioLib" "${CMAKE_CURRENT_BINARY_DIR}/RadioLib")

add_executable(pico_hal
    main.cpp
)

# Pull in common dependencies
target_link_libraries(pico_hal RadioLib pico_stdlib hardware_spi hardware_timer hardware_gpio)

pico_enable_stdio_usb(pico_hal 1)
pico_enable_stdio_uart(pico_hal 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(pico_hal)