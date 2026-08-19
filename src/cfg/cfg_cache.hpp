/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cfg/cfg_cache.hpp
 * Content size  : 1459
 * Date / Time   : 19-08-2026 21:18:15
 * MD5           : 56abe50495a9383695311d19ed4e9176
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

// I-CACHE / D-CACHE
#ifdef VHLIB_OPTIMAL_PLATFORM_EMBEDDED

#ifdef VHLIB_OPTIMAL_USE_ICACHE
#define VHLIB_OPTIMAL_FASTFUNC __attribute__((section(VHLIB_OPTIMAL_USE_ICACHE))) 
#endif

#ifdef VHLIB_OPTIMAL_USE_DCACHE 
#define VHLIB_OPTIMAL_FASTSRAM __attribute__((section(VHLIB_OPTIMAL_USE_DCACHE))) 
#endif

#endif 


#ifndef VHLIB_OPTIMAL_FASTFUNC
#define VHLIB_OPTIMAL_FASTFUNC
#endif

#ifndef VHLIB_OPTIMAL_FASTSRAM
#define VHLIB_OPTIMAL_FASTSRAM
#endif


// "noinline" need ?

// // STM32F407xx
// #if defined(STM32F407xx)

// #if defined(VHLIB_OPTIMAL_USE_ICACHE)
// #define VHLIB_OPTIMAL_ICACHE
// #endif

// #if defined(VHLIB_OPTIMAL_USE_DCACHE)
// #define VHLIB_OPTIMAL_DCACHE __attribute__((section(".ccmram")))
// #endif

// // STM32F746xx
// #elif defined(STM32F746xx)

// #if defined(VHLIB_OPTIMAL_USE_ICACHE)
// #define VHLIB_OPTIMAL_ICACHE __attribute__((section(".itcm_text"), noinline))
// #endif

// #if defined(VHLIB_OPTIMAL_USE_DCACHE)
// #define VHLIB_OPTIMAL_DCACHE __attribute__((section(".dtcm_bss")))
// #endif

// #define VHLIB_OPTIMAL_FASTFUNC VHLIB_OPTIMAL_ICACHE

// // STM32H750xx
// #elif defined(STM32H750xx)

// #if defined(VHLIB_OPTIMAL_USE_ICACHE)
// #define VHLIB_OPTIMAL_ICACHE
// #endif

// #if defined(VHLIB_OPTIMAL_USE_DCACHE)
// #define VHLIB_OPTIMAL_DCACHE __attribute__((section(".dtcm_bss")))
// #endif

// #define VHLIB_OPTIMAL_FASTFUNC VHLIB_OPTIMAL_ICACHE

// // Unknown platform
// #else

// #endif
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cfg/cfg_cache.hpp
 * Revision         : 0.8.0
 * Content size     : 1459
 * Date / Time      : 19-08-2026 21:18:15
 * MD5              : 56abe50495a9383695311d19ed4e9176
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */