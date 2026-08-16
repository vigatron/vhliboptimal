/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/platform/platform.hpp
 * Content size  : 424
 * Date / Time   : 16-08-2026 07:08:10
 * MD5           : 2fd498fb283c2487172854e72b64d20f
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */

#pragma once

// Check is VHPlatform already included
#ifndef VHPLATFORM_INCLUDED

#include <cstdint>
#include <cstddef>

using verr = uint32_t;
constexpr verr vok = 0;
constexpr verr verror(uint32_t code) noexcept { return code; }

constexpr size_t F1K = 1024;
constexpr size_t F1M = 1024 * 1024;

verr verrmsg(int x, const char *msg);

namespace vhliboptimal
{
    void asrts(bool cond, int module, const char *msg);
};

#include "bits/vhbitsmini.hpp"

#endif

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/platform/platform.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 424
 * Date / Time      : 16-08-2026 07:08:10
 * MD5              : 2fd498fb283c2487172854e72b64d20f
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */