/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/usr/vhliboptimal_callbacks.hpp
 * Content size  : 454
 * Date / Time   : 24-08-2026 16:26:25
 * MD5           : a1adda3f1d85f0eaf0d50f668e439f55
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include <cstdint>

namespace vhliboptimal {

/**
 * 
 */
void CALLBACK_VHLIBOPTIMAL_Border(
    void *userData,
    uint8_t cmd,
    uint8_t dirh, 
    uint8_t dirv,
    uint16_t cellx,
    uint16_t celly);

/**
 * 
 */
void CALLBACK_VHLIBOPTIMAL_Content(
    void *userData,
    uint32_t cell1,
    uint32_t cell2,
    uint8_t dir);


/**
 * 
 */
void CALLBACK_VHLIBOPTIMAL_Benchmark(
    void *userData,
    int cmd,
    int param);


};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/usr/vhliboptimal_callbacks.hpp
 * Revision         : 0.8.1
 * Content size     : 454
 * Date / Time      : 24-08-2026 16:26:25
 * MD5              : a1adda3f1d85f0eaf0d50f668e439f55
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */