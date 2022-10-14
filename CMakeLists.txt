#Set the mimum required version of CMake
cmake_minimum_required(VERSION 3.12)

#Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

#Set the project name, the programming language
project(speedometer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#Initialize the SDK
pico_sdk_init()

#Add an executable target to be build from the source files
add_executable(speedometer speedometer.c)

#Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(speedometer pico_stdlib)

#Enable USB output, disable UART output
pico_enable_stdio_usb(speedometer 1)
pico_enable_stdio_uart(speedometer 0)

#Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(speedometer)