/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/structs/vhliboptimalstructs.hpp
 * Content size  : 2456
 * Date / Time   : 22-08-2026 15:42:01
 * MD5           : fd2c5d9a7bc5c75bed73a90156eb0d49
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

#pragma pack(push, 1)

struct VHArea {
    public:
        uint32_t cellid;        // Координата UpperLeftCorner
        uint16_t cntx;          // Длинна
        uint16_t cnty;          // Высота
};

struct BMPFileHeader {
    uint16_t file_type;          // Сигнатура ("BM")
    uint32_t file_size;          // Размер файла
    uint16_t reserved1;          // Всегда 0
    uint16_t reserved2;          // Всегда 0
    uint32_t offset_data;        // Смещение до пикселей
};

struct BMPInfoHeader {
    uint32_t size;               // Размер структуры (40)
    int32_t width;               // Ширина
    int32_t height;              // Высота
    uint16_t planes;             // Количество плоскостей (1)
    uint16_t bit_count;          // Глубина цвета (например, 24)
    uint32_t compression;        // Сжатие (0)
    uint32_t size_image;         // Размер массива пикселей
    int32_t x_pixels_per_meter;  // Разрешение по X
    int32_t y_pixels_per_meter;  // Разрешение по Y
    uint32_t colors_used;        // Цветов в палитре
    uint32_t colors_important;   // Важных цветов
};

struct PixelRGB {
    uint8_t blue;                // Порядок BGR
    uint8_t green;
    uint8_t red;
};

#pragma pack(pop)

};


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/structs/vhliboptimalstructs.hpp
 * Revision         : 0.8.1
 * Content size     : 2456
 * Date / Time      : 22-08-2026 15:42:01
 * MD5              : fd2c5d9a7bc5c75bed73a90156eb0d49
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */