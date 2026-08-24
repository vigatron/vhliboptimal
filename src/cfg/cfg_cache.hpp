/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cfg/cfg_cache.hpp
 * Content size  : 1166
 * Date / Time   : 24-08-2026 16:26:25
 * MD5           : a2f7bcea90bb7f4477d84d7f72906f53
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

// I-CACHE / D-CACHE
#ifdef VHLIB_OPTIMAL_PLATFORM_EMBEDDED

#ifdef VHPLATFORM_STM32

#define VHLIB_OPTIMAL_ALIGNED16 alignas(16)

#ifdef VHLIB_OPTIMAL_USE_ICACHE
static const char _vhlib_optimal_icache[] = VHLIB_OPTIMAL_USE_ICACHE;
static_assert(sizeof(_vhlib_optimal_icache) > 1, "ICACHE section is empty");
#define VHLIB_OPTIMAL_FASTFUNC __attribute__((section(VHLIB_OPTIMAL_USE_ICACHE), noinline))
// #define VHLIB_OPTIMAL_FASTFUNC __attribute__((section(VHLIB_OPTIMAL_USE_ICACHE)))

#endif

#ifdef VHLIB_OPTIMAL_USE_DCACHE
static const char _vhlib_optimal_dcache[] = VHLIB_OPTIMAL_USE_DCACHE;
static_assert(sizeof(_vhlib_optimal_dcache) > 1, "DCACHE section is empty");
#define VHLIB_OPTIMAL_FASTSRAM __attribute__((section(VHLIB_OPTIMAL_USE_DCACHE)))
#endif

#endif

#ifdef VHPLATFORM_ESP32
#include "esp_attr.h"

#define VHLIB_OPTIMAL_FASTFUNC IRAM_ATTR
#define VHLIB_OPTIMAL_FASTSRAM DRAM_ATTR

#endif

#endif

#ifndef VHLIB_OPTIMAL_ALIGNED16
#define VHLIB_OPTIMAL_ALIGNED16 alignas(16)
#endif

#ifndef VHLIB_OPTIMAL_FASTFUNC
#define VHLIB_OPTIMAL_FASTFUNC
#endif

#ifndef VHLIB_OPTIMAL_FASTSRAM
#define VHLIB_OPTIMAL_FASTSRAM
#endif

// #endif
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cfg/cfg_cache.hpp
 * Revision         : 0.8.1
 * Content size     : 1166
 * Date / Time      : 24-08-2026 16:26:25
 * MD5              : a2f7bcea90bb7f4477d84d7f72906f53
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */