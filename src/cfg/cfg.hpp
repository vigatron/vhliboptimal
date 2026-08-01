/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cfg/cfg.hpp
 * Content size  : 44
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 656b95f4338b023bafc1abe9b84f86e2
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include <cstdint>

// VHPlatform partially implemented for zero dependencies case
#include "platform/platform.hpp"
#include "cfg/cfg.hpp"


namespace vhliboptimal {

// Константы устанавливается при сборке

static constexpr int VHOPTIMAL_GRID_X_LEVEL = 8;        // 256 pxls
static constexpr int VHOPTIMAL_GRID_Y_LEVEL = 8;        // 256 pxls
static constexpr int VHOPTIMAL_GRID_S_ORDER = 1;        //   2 pxls

static constexpr int VHOPTIMAL_OBJECTS_MAX  = 128;      // F1K*4;
static constexpr int VHOPTIMAL_SPANS_MAX    = F1K*4;    //


//
typedef struct _stConfig {

    // // Размер картинки в пикселях, длинна
    // uint16_t    imageWidth;

    // // Размер картинки в пикселях, высота
    // uint16_t    imageHeight;

    // Максимально допустимое количество пустых ячеек подряд в линии
    uint16_t    spccnt;

    // Размер ячейки в пикселях
    uint8_t     cellsize;

    // Подсветка цветности ячейки
    // меньшие значения принимаем за черный
    uint8_t     minColorVal;

    // Минимальный размер объекта в пикселях
    uint16_t    min_obj_width;
    uint16_t    min_obj_height;

    // Максимальный размер объекта в пикселях
    uint16_t    max_obj_width;
    uint16_t    max_obj_height;

    // Sort flags
    uint8_t     sortMode;

    // Режим отладки
    uint8_t     loglevel;

} stConfig;


};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cfg/cfg.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 44
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 656b95f4338b023bafc1abe9b84f86e2
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */