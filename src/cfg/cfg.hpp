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

#if defined(VHLIB_OPTIMAL_EMBEDDED)

    #define VHLIB_OPTIMAL_MODE_32
    // #define VHLIB_OPTIMAL_MODE_64

#else

    #if !defined(__x86_64__)
    #define VHLIB_OPTIMAL_MODE_32
    #else
    #define VHLIB_OPTIMAL_MODE_64
    #endif

#endif



namespace vhliboptimal {

// Константы устанавливается при сборке

static constexpr int VHOPTIMAL_GRID_X_LEVEL = VHLIB_OPTIMAL_GRID_LX;
static constexpr int VHOPTIMAL_GRID_Y_LEVEL = VHLIB_OPTIMAL_GRID_LY;
static constexpr int VHOPTIMAL_GRID_S_ORDER = 1;

static constexpr int VHOPTIMAL_OBJECTS_MAX  = VHLIB_OPTIMAL_OBJS_MAX;
static constexpr int VHOPTIMAL_SPANS_MAX    = VHLIB_OPTIMAL_SPNS_MAX;

using spanword  = uint32_t;

//
typedef struct _stConfig {

    // Максимально допустимое количество пустых ячеек подряд в линии
    uint16_t    spccnt;

    // 2 в степени: вычисление размера ячейки в пикселях
    uint8_t     levelcs;

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