/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft-2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/platform/platform.hpp
 * Content size  : 466
 * Date / Time   : 15-08-2026 02:03:05
 * MD5           : 033e6527da055a146d29c1406184e30a
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */

#pragma once

// Check is VHPlatform already included
#ifndef VHPLATFORM_INCLUDED

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
 * Revision         : 0.8.0-draft-2
 * Content size     : 466
 * Date / Time      : 15-08-2026 02:03:05
 * MD5              : 033e6527da055a146d29c1406184e30a
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */