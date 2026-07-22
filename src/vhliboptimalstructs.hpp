/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.6beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimalstructs.hpp
 * Content size  : 2525
 * Date / Time   : 22-07-2026 14:54:04
 * MD5           : 2efb160c5cb45d654b92c9290ca58449
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
 * uint32_t     cxl         - Left  Cell
 * uint32_t     cxr         - Right Cell
 */
typedef void (*CallbackContent)(void *userData, uint32_t cxl, uint32_t cxr);



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

} stConfig;

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimalstructs.hpp
 * Revision         : 0.6beta
 * Content size     : 2525
 * Date / Time      : 22-07-2026 14:54:04
 * MD5              : 2efb160c5cb45d654b92c9290ca58449
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */