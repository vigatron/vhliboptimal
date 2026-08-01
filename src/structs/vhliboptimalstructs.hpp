/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimalstructs.hpp
 * Content size  : 2275
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 92092b6dbe291939cbc07d6cfebae75d
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <cstring>
#include <climits>
#include <cstdio>


#include "cfg/cfg.hpp"

namespace vhliboptimal {

using spanword  = uint32_t;

#ifdef VHARCH_32BIT

#else

#endif



// Выносим константы в тип constexpr
constexpr uint32_t  id_bits  = 22;  // 
constexpr uint32_t  len_bits = 10;  // 1023 max

constexpr uint32_t  id_mask  = (1U << id_bits ) - 1;        // 0x3FFFFF
constexpr uint32_t  len_mask = (1U << len_bits) - 1;        // 0x3FF


// Упаковка
[[nodiscard]] constexpr inline uint32_t pack_span(uint32_t id, uint16_t len) noexcept {
    return (static_cast<uint32_t>(len) << id_bits) | (id & id_mask);
}

// Извлечение длины (старшие 10 бит)
[[nodiscard]] constexpr inline uint16_t get_span_len(uint32_t packed) noexcept {
    return static_cast<uint16_t>(packed >> id_bits);
}

// Извлечение ID (младшие 22 бита)
[[nodiscard]] constexpr inline uint32_t get_span_id(uint32_t packed) noexcept {
    return packed & id_mask;
}

//
class VHArea {
    public:
        uint32_t cellid;
        uint16_t cnty;
        uint16_t cntx;
} __attribute__((packed));


};


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimalstructs.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 2275
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 92092b6dbe291939cbc07d6cfebae75d
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */