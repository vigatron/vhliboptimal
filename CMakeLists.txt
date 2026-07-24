cmake_minimum_required(VERSION 3.16)
project(vhliboptimal VERSION 0.7.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/version_vhliboptimal.h"
)

# Собираем только исходники самой библиотеки
file(GLOB_RECURSE srcfiles
    src/*
    src/cmatrix/*
    src/bitfield/*
    src/figures/*
    src/log/*
)

add_library(vhlib_optimal STATIC ${srcfiles})

target_include_directories(vhlib_optimal PUBLIC
    src
    src/cmatrix
    src/bitfield
    src/figures
    src/log
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

target_link_libraries(vhlib_optimal PUBLIC vhlib_platform)

target_compile_definitions(vhlib_platform PUBLIC 
    VHLIB_OPTIMAL_FASTIDX=2
)
