/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cfg/cfg_cache.hpp
 * Content size  : 776
 * Date / Time   : 20-08-2026 05:00:12
 * MD5           : 4913c4c3aeb93ac0639942b145797a3d
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

// I-CACHE / D-CACHE
#ifdef VHLIB_OPTIMAL_PLATFORM_EMBEDDED

#ifdef VHLIB_OPTIMAL_USE_ICACHE
static const char _vhlib_optimal_icache[] = VHLIB_OPTIMAL_USE_ICACHE;
static_assert(sizeof(_vhlib_optimal_icache) > 1, "ICACHE section is empty");
#define VHLIB_OPTIMAL_FASTFUNC __attribute__((section(VHLIB_OPTIMAL_USE_ICACHE))) 
#endif

#ifdef VHLIB_OPTIMAL_USE_DCACHE 
static const char _vhlib_optimal_dcache[] = VHLIB_OPTIMAL_USE_DCACHE;
static_assert(sizeof(_vhlib_optimal_dcache) > 1, "DCACHE section is empty");
#define VHLIB_OPTIMAL_FASTSRAM __attribute__((section(VHLIB_OPTIMAL_USE_DCACHE))) 
#endif

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
 * Revision         : 0.8.0
 * Content size     : 776
 * Date / Time      : 20-08-2026 05:00:12
 * MD5              : 4913c4c3aeb93ac0639942b145797a3d
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */