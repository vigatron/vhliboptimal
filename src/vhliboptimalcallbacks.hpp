/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimalcallbacks.hpp
 * Content size  : 2235
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 68b5be0ec2fd8ff0cc67743522afb8fe
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhplatform.hpp"

namespace vhliboptimal {


enum enCmdBenchmark {

    eCmdBenchmarkMemalloc = 0,      // Memory allocation process
    eCmdBenchmarkSampling,          // Initial grid fill
    eCmdBenchmarkScan,              // Start recognition task
    eCmdBenchmarkSubfuncFigure      // Subtask for single object measurement

};


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
typedef void (*CallbackGetSrcPxls)(
    void *userData,
    uint8_t *dstptr,
    uint16_t bytescnt,
    uint16_t srcid,
    uint16_t srcx,
    uint16_t srcy);


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
typedef void (*CallbackBorder)(
    void *userData,
    uint8_t cmd,
    uint8_t dirh,
    uint8_t dirv,
    uint16_t cellx,
    uint16_t celly,
    uint16_t imgx,
    uint16_t imgy);

/**
 * CallbackContent          - Object Area
 * 
 * void *       userData    - User context pointer
 * uint32_t     cell1       - Left or Top  Cell 
 * uint32_t     cell2       - Right or Bottom Cell
 * uint8_t      dir         - Direction 0: LR 1: UD
 */
typedef void (*CallbackContent)(
    void *userData,
    uint32_t cell1,
    uint32_t cell2,
    uint8_t dir);


/**
 * CallbackBenchmark        - Benchmarks
 *  cmd
 *              1) Downsampling started
 *              2) Downsampling finished
 */

typedef void (*CallbackBenchmark)(
    void *userData,
    int cmd,
    int param);

};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimalcallbacks.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 2235
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 68b5be0ec2fd8ff0cc67743522afb8fe
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */