#pragma once

// I-CACHE / D-CACHE
#ifdef VHLIB_OPTIMAL_PLATFORM_EMBEDDED

#if defined(STM32F407xx) && defined(VHLIB_OPTIMAL_USE_CACHE)

#define VHLIB_OPTIMAL_ICACHE
#define VHLIB_OPTIMAL_DCACHE __attribute__((section(".ccmram")))

#elif defined(STM32F746xx)

#if defined(VHLIB_OPTIMAL_USE_ICACHE)
#define VHLIB_OPTIMAL_ICACHE __attribute__((section(".itcm_text"), noinline))
#else
#define VHLIB_OPTIMAL_ICACHE
#endif

#if defined(VHLIB_OPTIMAL_USE_DCACHE)
#define VHLIB_OPTIMAL_DCACHE __attribute__((section(".dtcm_bss")))
#else 
#define VHLIB_OPTIMAL_DCACHE
#endif

#define VHLIB_OPTIMAL_FASTFUNC VHLIB_OPTIMAL_ICACHE

#elif defined(STM32H750xx) && defined(VHLIB_OPTIMAL_USE_CACHE)

#define VHLIB_OPTIMAL_ICACHE
#define VHLIB_OPTIMAL_DCACHE __attribute__((section(".dtcm_data")))

#else
#define VHLIB_OPTIMAL_ICACHE
#define VHLIB_OPTIMAL_DCACHE
#define VHLIB_OPTIMAL_FASTFUNC
#endif

#endif