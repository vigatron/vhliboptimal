/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft-2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/structs/vhliboptimalcallbacks.hpp
 * Content size  : 1484
 * Date / Time   : 15-08-2026 02:03:05
 * MD5           : 163c999a33cada330882535ee8ab9b37
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include <cstdint>

namespace vhliboptimal {


enum enCmdBenchmark {

    eCmdBenchmarkMemalloc = 0,      // Memory allocation process
    eCmdBenchmarkSampling,          // Initial grid fill
    eCmdBenchmarkScan,              // Start recognition task
    eCmdBenchmarkSubfuncFigure      // Subtask for single object measurement

};

/**
 * CallbackBorder           - Move across figure border
 * 
 * void *       userData    - User context pointer
 * uint8_t      cmd         - Command: `CMD_START` / `CMD_MOVE` / `CMD_STOP` (defined in `vhliboptimalstructs.hpp`)
 * uint8_t      dirh        - Direction H
 * uint8_t      dirv        - Direction V
 * uint32_t     cellx       - Cell X
 * uint32_t     celly       - Cell Y
 */
typedef void (*CallbackBorder)(
    void *userData,
    uint8_t cmd,
    uint8_t dirh,
    uint8_t dirv,
    uint16_t cellx,
    uint16_t celly);

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
 * File             : src/structs/vhliboptimalcallbacks.hpp
 * Revision         : 0.8.0-draft-2
 * Content size     : 1484
 * Date / Time      : 15-08-2026 02:03:05
 * MD5              : 163c999a33cada330882535ee8ab9b37
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */