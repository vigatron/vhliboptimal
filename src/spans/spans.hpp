/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft-2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/spans/spans.hpp
 * Content size  : 1027
 * Date / Time   : 15-08-2026 02:03:05
 * MD5           : fd1df50f91263e9fa7c8075873e33cef
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "cfg/cfg.hpp"
#include "structs/vhliboptimalstructs.hpp"

namespace vhliboptimal
{

    /**
     *  массив локальных участков в виде
     *  spanlen [31 .. 21] + spanid  [20 ..  0]
     */

    class VHLocalSpansArray
    {

    public:

        void Init(spanword *arr, uint32_t sidx)
        {
            arrlocal = arr;
            arrlocal += sidx;
            startIdx = sidx;
            curIdx = 0;
        }

        bool add(spanword word)
        {
            if (startIdx + curIdx >= VHLIB_OPTIMAL_SPNS_MAX)
                return false;
            arrlocal[curIdx++] = word;
            return true;
        }

        spanword get(uint32_t pos) const
        {
            return arrlocal[pos];
        }

        uint32_t globalstartidx() const
        {
            return startIdx;
        }

        uint32_t localspanscnt() const
        {
            return curIdx;
        }

    private:
        spanword *arrlocal;
        uint32_t startIdx;
        uint32_t curIdx;
    };

};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/spans/spans.hpp
 * Revision         : 0.8.0-draft-2
 * Content size     : 1027
 * Date / Time      : 15-08-2026 02:03:05
 * MD5              : fd1df50f91263e9fa7c8075873e33cef
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */