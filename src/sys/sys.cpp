/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/sys/sys.cpp
 * Content size  : 439
 * Date / Time   : 24-08-2026 16:26:25
 * MD5           : b407387a19ddc6578050355144a398e7
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "structs/vhliboptimalstructs.hpp"
#include "log/log.hpp"

#include <cstdlib>

void vhliboptimal::asrts(bool cond, int module, const char * msg) {
    if(cond) return;
    log::partout("ASSERT: ");
    log::partout("Module: ");
    log::partint(module);
    log::partout(" Msg: ");
    log::partout(msg);
    log::lineout("");

    // Exit app
#if VHLIB_OPTIMAL_EMBEDDED==1
    while(1) { asm("nop"); }
#else
    exit(1);
#endif

}
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/sys/sys.cpp
 * Revision         : 0.8.1
 * Content size     : 439
 * Date / Time      : 24-08-2026 16:26:25
 * MD5              : b407387a19ddc6578050355144a398e7
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */