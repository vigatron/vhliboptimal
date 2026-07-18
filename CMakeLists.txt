cmake_minimum_required(VERSION 3.16)
project(vhliboptimal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Собираем только исходники самой библиотеки
file(GLOB_RECURSE srcfiles
    src/*
    src/cmatrix/*
    src/figures/*
    src/log/*
)

add_library(vhlib_optimal STATIC ${srcfiles})

target_include_directories(vhlib_optimal PUBLIC
    src
    src/cmatrix
    src/figures
    src/log
)

target_link_libraries(vhlib_optimal PUBLIC vhlib_platform)