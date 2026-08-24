/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cfg/cfg_detect.hpp
 * Content size  : 451
 * Date / Time   : 24-08-2026 16:26:25
 * MD5           : ba47d1d7d64c54c5bc11f31af9df8b1d
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
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
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cfg/cfg_detect.hpp
 * Revision         : 0.8.1
 * Content size     : 451
 * Date / Time      : 24-08-2026 16:26:25
 * MD5              : ba47d1d7d64c54c5bc11f31af9df8b1d
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */