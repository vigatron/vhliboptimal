/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cfg/cfg.hpp
 * Content size  : 1680
 * Date / Time   : 16-08-2026 07:08:10
 * MD5           : 69a8c93987685b7bfb78315b96b071b2
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include <cstdint>
#include <cstddef>
#include <climits>
#include <utility>

#define VHMACRO_STR2(x) #x
#define VHMACRO_STR(x) VHMACRO_STR2(x)

#define VHLIB_OPTIMAL_VERSION_STRING \
    "v" VHMACRO_STR(VHLIB_OPTIMAL_VERSION_MAJOR) "." VHMACRO_STR(VHLIB_OPTIMAL_VERSION_MINOR) "." VHMACRO_STR(VHLIB_OPTIMAL_VERSION_PATCH)

// === Platform selection ===

// VHLIB_OPTIMAL_PLATFORM_PC
// * Default for PC
// * Default for SBC

// VHLIB_OPTIMAL_PLATFORM_EMBEDDED
// Optimized for speed:
// * STM32F4/F7/H7
// * ESP32

// === Linux Platform autodetect ===
#if defined(__gnu_linux__) && !defined(VHPLATFORM_PC)
#define VHPLATFORM_PC
#endif

// === STM32 Platform autodetect ===
#if defined(__ARM_ARCH_7EM__) && !defined(VHPLATFORM_STM32)
#define VHPLATFORM_STM32
#endif

// ===========================

#ifdef VHPLATFORM_PC
#define VHLIB_OPTIMAL_PLATFORM_PC

// GRID FAST IDX MODE
#if !defined(__x86_64__)
#define VHLIB_OPTIMAL_MODE_32
#else
#define VHLIB_OPTIMAL_MODE_64
#endif


#endif

// #ifdef VHPLATFORM_OPI
// #define VHLIB_OPTIMAL_PLATFORM_OPI
// #define VHLIB_OPTIMAL_MODE_32
// #endif

// #ifdef VHPLATFORM_RPI
// #define VHLIB_OPTIMAL_PLATFORM_RPI
// #define VHLIB_OPTIMAL_MODE_32
// #endif

#ifdef VHPLATFORM_STM32
#define VHLIB_OPTIMAL_PLATFORM_EMBEDDED
#define VHLIB_OPTIMAL_MODE_32
#endif

#ifdef VHPLATFORM_ESP32
#define VHLIB_OPTIMAL_PLATFORM_EMBEDDED
#define VHLIB_OPTIMAL_MODE_32
#endif


#include "cfg_cache.hpp"

// Default config
#ifndef VHLIB_OPTIMAL_OBJS_MAX
#define VHLIB_OPTIMAL_OBJS_MAX 256
#endif

#ifndef VHLIB_OPTIMAL_SPNS_MAX
#define VHLIB_OPTIMAL_SPNS_MAX 4096
#endif

static_assert(CHAR_BIT == 8, "This optimized implementation requires 8-bit bytes.");

// Принудительный инлайнинг для GCC/Clang и MSVC
#if defined(__GNUC__) || defined(__clang__)

#define VH_ALWAYS_INLINE __attribute__((always_inline)) inline
// Принудительный инлайнинг для микро-методов доступа
// #define VH_INLINE __attribute__((always_inline)) inline

#elif defined(_MSC_VER)
#define VH_ALWAYS_INLINE __forceinline
#else
#define VH_ALWAYS_INLINE inline
#endif

// Partial implementation of VHPlatform
// (for zero dependencies case)

#ifndef VHPLATFORM_INCLUDED
#include "platform/platform.hpp"
#endif

namespace vhliboptimal
{

    using spanword = uint32_t;

    //
    typedef struct _stConfig
    {

        // Максимально допустимое количество
        // пустых ячеек подряд в линии
        uint16_t spccnt;

        // Минимальный размер объекта в пикселях
        uint16_t min_obj_width;
        uint16_t min_obj_height;

        // Максимальный размер объекта в пикселях
        uint16_t max_obj_width;
        uint16_t max_obj_height;

        // Sort flags
        uint8_t sortMode;

        // Режим отладки
        uint8_t loglevel;

    } stConfig;

};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cfg/cfg.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 1680
 * Date / Time      : 16-08-2026 07:08:10
 * MD5              : 69a8c93987685b7bfb78315b96b071b2
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */