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
