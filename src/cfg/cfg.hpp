/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft-2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cfg/cfg.hpp
 * Content size  : 1455
 * Date / Time   : 15-08-2026 02:03:05
 * MD5           : a940c6b6431dcb4c24a30b7a6d399006
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include <cstdint>
#include <cstddef>
#include <climits>

// === Platform selection ===

// VHLIB_OPTIMAL_PLATFORM_PC
// * Default for PC
// * Default for SBC

// VHLIB_OPTIMAL_PLATFORM_EMBEDDED
// Optimized for speed:
// * STM32F4/F7/H7
// * ESP32

// Default is VHLIB_OPTIMAL_PLATFORM_PC if not specified
#ifndef VHLIB_OPTIMAL_PLATFORM_PC

#ifndef VHLIB_OPTIMAL_PLATFORM_EMBEDDED
#define VHLIB_OPTIMAL_PLATFORM_PC
#else
#define VHLIB_OPTIMAL_PLATFORM_EMBEDDED
#endif

#endif


#if defined(VHLIB_OPTIMAL_PLATFORM_EMBEDDED)

#define VHLIB_OPTIMAL_MODE_32
// #define VHLIB_OPTIMAL_MODE_64

#else

#if !defined(__x86_64__)
#define VHLIB_OPTIMAL_MODE_32
#else
#define VHLIB_OPTIMAL_MODE_64
#endif

#endif


// Default config
#ifndef VHLIB_OPTIMAL_OBJS_MAX
#define VHLIB_OPTIMAL_OBJS_MAX 256
#endif

#ifndef VHLIB_OPTIMAL_SPNS_MAX
#define VHLIB_OPTIMAL_SPNS_MAX 4096
#endif


// Partial implementation of VHPlatform 
// (for zero dependencies case)

#ifndef VHPLATFORM_INCLUDED
#include "platform/platform.hpp"
#endif



namespace vhliboptimal {


using spanword  = uint32_t;

//
typedef struct _stConfig {

    // Максимально допустимое количество
    // пустых ячеек подряд в линии
    uint16_t    spccnt;

    // Минимальный размер объекта в пикселях
    uint16_t    min_obj_width;
    uint16_t    min_obj_height;

    // Максимальный размер объекта в пикселях
    uint16_t    max_obj_width;
    uint16_t    max_obj_height;

    // Sort flags
    uint8_t     sortMode;

    // Режим отладки
    uint8_t     loglevel;

} stConfig;


};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cfg/cfg.hpp
 * Revision         : 0.8.0-draft-2
 * Content size     : 1455
 * Date / Time      : 15-08-2026 02:03:05
 * MD5              : a940c6b6431dcb4c24a30b7a6d399006
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */