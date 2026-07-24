/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimalstructs.hpp
 * Content size  : 2958
 * Date / Time   : 24-07-2026 12:39:50
 * MD5           : da7de04b47c692b1db1b863da7205f0b
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhplatform.hpp"

#include <functional>


namespace vhliboptimal {

// Определяем типы для колбэков


/**
 * CallbackGetSrcPxls       - Read source image data
 * 
 * void *       userData    - User context pointer
 * uint8_t *    dstptr      - Destination buffer to fill (Expected format: 8-bit grayscale, 1 byte per pixel)
 * uint16_t     bytescnt    - Number of bytes to read (typically equal to image width in pixels)
 * uint16_t     srcid       - Image ID
 * uint16_t     srcx        - Image offset X
 * uint16_t     srcy        - Image offset Y
 */
typedef void (*CallbackGetSrcPxls)(void *userData, uint8_t *dstptr, uint16_t bytescnt, uint16_t srcid, uint16_t srcx, uint16_t srcy);


/**
 * CallbackBorder           - Move across figure border
 * 
 * void *       userData    - User context pointer
 * uint8_t      cmd         - Command: `CMD_START` / `CMD_MOVE` / `CMD_STOP` (defined in `vhliboptimalstructs.hpp`)
 * uint8_t      dirh        - Direction H
 * uint8_t      dirv        - Direction V
 * uint32_t     cellx       - Cell Offset X
 * uint32_t     celly       - Cell Offset Y
 * uint16_t     imgx        - Image X
 * uint16_t     imgy        - Image Y
 */
typedef void (*CallbackBorder)(void *userData, uint8_t cmd, uint8_t dirh, uint8_t dirv, uint16_t cellx, uint16_t celly, uint16_t imgx, uint16_t imgy);


/**
 * CallbackContent          - Object Area
 * 
 * void *       userData    - User context pointer
 * uint32_t     cell1       - Left or Top  Cell 
 * uint32_t     cell2       - Right or Bottom Cell
 * uint8_t      dir         - Direction 0: LR 1: UD
 */
typedef void (*CallbackContent)(void *userData, uint32_t cell1, uint32_t cell2, uint8_t dir);



typedef struct _stspan  {

    // cell index
    uint32_t n;

    // span length
    uint32_t l;

} stspan;


typedef struct _strect  {

    // Left-top     corner  sx, sy
    uint16_t x1, y1;

    // Right-bottom corner  ex, ey
    uint16_t x2, y2;

} strect;


typedef struct _stConfig {

    // Размер картинки в пикселях, длинна
    uint16_t imageWidth;

    // Размер картинки в пикселях, высота
    uint16_t imageHeight;

    // Максимально допустимое количество пустых ячеек подряд в линии
    uint16_t spccnt;

    // Размер ячейки в пикселях
    uint8_t cellsize;

    // Подсветка цветности ячейки
    // меньшие значения принимаем за черный
    uint8_t  minColorVal;

    // Минимальный размер объекта в пикселях
    uint16_t    min_obj_width;
    uint16_t    min_obj_height;

    // Максимальный размер объекта в пикселях
    uint16_t    max_obj_width;
    uint16_t    max_obj_height;

    // Режим отладки
    uint8_t     loglevel;

} stConfig;

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimalstructs.hpp
 * Revision         : 0.7.0-beta
 * Content size     : 2958
 * Date / Time      : 24-07-2026 12:39:50
 * MD5              : da7de04b47c692b1db1b863da7205f0b
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */