#pragma once

// I-CACHE / D-CACHE
#ifdef VHLIB_OPTIMAL_PLATFORM_EMBEDDED


// STM32F407xx
#if defined(STM32F407xx)

#if defined(VHLIB_OPTIMAL_USE_ICACHE)
#define VHLIB_OPTIMAL_ICACHE
#endif

#if defined(VHLIB_OPTIMAL_USE_DCACHE)
#define VHLIB_OPTIMAL_DCACHE __attribute__((section(".ccmram")))
#endif

// STM32F746xx
#elif defined(STM32F746xx)

#if defined(VHLIB_OPTIMAL_USE_ICACHE)
#define VHLIB_OPTIMAL_ICACHE __attribute__((section(".itcm_text"), noinline))
#endif

#if defined(VHLIB_OPTIMAL_USE_DCACHE)
#define VHLIB_OPTIMAL_DCACHE __attribute__((section(".dtcm_bss")))
#endif

#define VHLIB_OPTIMAL_FASTFUNC VHLIB_OPTIMAL_ICACHE

// STM32H750xx
#elif defined(STM32H750xx)

#if defined(VHLIB_OPTIMAL_USE_ICACHE)
#define VHLIB_OPTIMAL_ICACHE
#endif

#if defined(VHLIB_OPTIMAL_USE_DCACHE)
#define VHLIB_OPTIMAL_DCACHE __attribute__((section(".dtcm_bss")))
#endif

#define VHLIB_OPTIMAL_FASTFUNC VHLIB_OPTIMAL_ICACHE

// Unknown platform
#else

#endif


#endif // VHLIB_OPTIMAL_PLATFORM_EMBEDDED


#ifndef VHLIB_OPTIMAL_ICACHE
#define VHLIB_OPTIMAL_ICACHE
#endif

#ifndef VHLIB_OPTIMAL_DCACHE
#define VHLIB_OPTIMAL_DCACHE
#endif

#ifndef VHLIB_OPTIMAL_FASTFUNC
#define VHLIB_OPTIMAL_FASTFUNC
#endif
