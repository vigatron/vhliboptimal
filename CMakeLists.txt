cmake_minimum_required(VERSION 3.16)
project(vhliboptimal VERSION 0.8.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/version_vhliboptimal.h"
)

add_library(vhlib_optimal STATIC)

file(GLOB_RECURSE srcfiles
    src/*
    src/cfg/*
)

target_sources(vhlib_optimal PRIVATE
    ${srcfiles}
    src/cmatrix/cmatrix.cpp
    src/bitfield/bitfield.cpp
    src/figures/figures.cpp
    src/log/lognostl.cpp
    src/log/logstl.cpp
    src/sys/sys.cpp
)

target_include_directories(vhlib_optimal PUBLIC
    src
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

target_compile_definitions(vhlib_platform PUBLIC 
    VHLIB_PLATFORM_STL=0
)
