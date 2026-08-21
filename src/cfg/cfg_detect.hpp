#pragma once

#if !defined(VHPLATFORM_STM32) && !defined(VHPLATFORM_ESP32) && !defined(VHPLATFORM_PC)

// === STM32 ===
#if defined(__ARM_ARCH_7EM__)
#define VHPLATFORM_STM32

// === ESP32 ===
#elif defined(ESP_PLATFORM) || defined(__XTENSA__)
#define VHPLATFORM_ESP32

// === PC / SBC ===
#elif defined(__gnu_linux__)
#define VHPLATFORM_PC

// Unknown
#else
#error "No PLATFORM specified & autodetect failed - Unknown target platform"

#endif

#endif
