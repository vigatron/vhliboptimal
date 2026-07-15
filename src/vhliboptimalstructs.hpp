/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.1 Draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimalstructs.hpp
 * Content size  : 1145
 * Date / Time   : 16-07-2026 02:43:20
 * MD5           : c73c8b79274927a0439b6583664da004
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhplatform.hpp"

#include <functional>

// Определяем типы для колбэков

using   GetPixelsCallback   = std::function<std::vector<uint8_t>(uint16_t, uint16_t, uint16_t)>;
using   SetPosCallback      = std::function<void(uint16_t, uint16_t)>;


typedef struct _stspan  {

    // cell index
    uint16_t n;

    // span length
    uint16_t l;

} stspan;


typedef struct _strect  {

    // Left-top     corner  sx, sy
    uint16_t x1,y1;

    // Right-bottom corner  ex, ey
    uint16_t x2,y2;

} strect;


typedef struct _stConfig {

    // Размер картинки в пикселях, длинна
    uint16_t imageWidth;

    // Размер картинки в пикселях, высота
    uint16_t imageHeight;

    // Размер ячейки
    uint16_t cellsize;

    // Максимально допустимое количество пустых ячеек подряд в линии
    uint16_t spccnt;

    // Подсветка цветности ячейки, меньшие значения принимаем за черный
    uint8_t  minColorVal;

} stConfig;

/* ========================[  END FILE CONTENT  ]========================
 * File             : src/vhliboptimalstructs.hpp
 * Content size     : 1145
 * Date / Time      : 16-07-2026 02:43:20
 * MD5              : c73c8b79274927a0439b6583664da004
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */