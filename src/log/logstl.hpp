/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/log/logstl.hpp
 * Content size  : 594
 * Date / Time   : 16-08-2026 07:08:10
 * MD5           : 95bde3495eee8d4da13ead5b79cdc30a
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhliboptimal.hpp"

namespace vhliboptimal {


class VHLibOptimalLogger {

    // ------------------------------------------------
    static void newlout();
    static void partout(const std::string & str);
    static void lineout(const std::string & str);

    // ------------------------------------------------

    static std::string fmt(const std::string & strparam,int value);
    static std::string fmt(const std::string & strparam1,int value,const std::string & strparam2);
    static std::string fmt(int value1,const std::string & strparam,int value2);


};

};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/log/logstl.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 594
 * Date / Time      : 16-08-2026 07:08:10
 * MD5              : 95bde3495eee8d4da13ead5b79cdc30a
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */