/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bits/vhbitsmini.hpp
 * Content size  : 1294
 * Date / Time   : 22-08-2026 15:42:01
 * MD5           : 42584952efd7fc590e94a52712491f7c
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "cfg/cfg.hpp"

namespace vhliboptimal {

/**
 * @brief Speed-optimized routines
 */
class VHBits {
public:

    /**
     * @brief Прямой расчет индекса байта и бита без лишних вычитаний
     */
    // [[nodiscard]] constexpr static std::pair<size_t, uint8_t> splidx(size_t v) noexcept {
    //     // Компилятор превратит это в v >> 3 и v & 7
    //     return { v / 8, static_cast<uint8_t>(v % 8) };
    // }

    VH_ALWAYS_INLINE static void BitClr(uint8_t * __restrict ptr, size_t n) noexcept {
        // Вместо вычитания в splidx и сдвига (1 << bitIdx), 
        // мы инвертируем маску сразу на этапе сдвига: 0x80 >> (n % 8)
        ptr[n / 8] &= static_cast<uint8_t>(~(0x80 >> (n % 8)));
    }

    VH_ALWAYS_INLINE static void BitSet(uint8_t * __restrict ptr, size_t n) noexcept {
        // Старший бит (0x80) сдвигаем вправо на остаток от деления
        ptr[n / 8] |= static_cast<uint8_t>(0x80 >> (n % 8));
    }

    [[nodiscard]] VH_ALWAYS_INLINE static bool BitVal(const uint8_t * __restrict ptr, size_t n) noexcept {
        return (ptr[n / 8] & (0x80 >> (n % 8))) != 0;
    }

};

};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/bits/vhbitsmini.hpp
 * Revision         : 0.8.1
 * Content size     : 1294
 * Date / Time      : 22-08-2026 15:42:01
 * MD5              : 42584952efd7fc590e94a52712491f7c
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */