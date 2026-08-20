cmake_minimum_required(VERSION 3.16)
project(vhliboptimal VERSION 0.8.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/version_vhliboptimal.h"
)

add_library(vhlib_optimal STATIC)

target_sources(vhlib_optimal PRIVATE
    src/bitfield/bitfield.cpp
    src/figure/figure.cpp
    src/log/lognostl.cpp
    src/log/logstl.cpp
    src/sys/sys.cpp
    src/vhliboptimal.cpp
)

target_include_directories(vhlib_optimal PUBLIC
    src
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)
