cmake_minimum_required(VERSION 3.16)
project(msgpack-mcu)

# Create the library
add_library(msgpack-mcu STATIC
    src/umsgpack.c
)

# Make sure it's compiled as C
set_source_files_properties(
    src/umsgpack.c
    PROPERTIES
    LANGUAGE C
)

# Include directories
target_include_directories(msgpack-mcu
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler flags - must match your M7 settings
target_compile_options(msgpack-mcu
    PRIVATE
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-d16
        -mfloat-abi=hard
        -Os
        -ffunction-sections
        -fdata-sections
        -g0
        -ffast-math
        $<$<COMPILE_LANGUAGE:C>:-std=c11>
)