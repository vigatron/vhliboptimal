/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft-2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/log/lognostl.hpp
 * Content size  : 1900
 * Date / Time   : 15-08-2026 02:03:05
 * MD5           : 79fe2a0137f4b98f31ff459f77f974ba
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhliboptimal.hpp"

namespace vhliboptimal {


class log {

    public:

        static void newlout();
        static void print_param     (const char *msg, int val);
        static void partout         (const char *msg);
        static void partint         (const int val);
        static void lineout         (const char *msg);

        // ------------------------------------------------

        static void PicProps(
            const VHLibOptimal & obj,
            const CellsMatrix & cmatrix);

        // ------------------------------------------------

        static void DumpCellsHEX(
            const VHLibOptimal & obj,
            const CellsMatrix & cmatrix,
            const uint8_t * arrptr,
            const char *msg = nullptr);

        static void DumpCellsTXT(
            const VHLibOptimal & obj,
            const CellsMatrix & cmatrix,
            const uint8_t * arrptr,
            const char *msg = nullptr,
            const int cellMarker = -1 );

        // static void DumpCell(
        //     const std::string & msg,
        //     int celln,
        //     int cellx, int celly,
        //     int sx, int sy );

        // ------------------------------------------------

        static void DumpSpan(
            const spanword spn,
            const CellsMatrix & cmtx,
            int cellsize,
            int spann);

        static void DumpFigureSpans(
            const VHLibOptimal & detector,
            const VHOptimalFigure & obj,
            const CellsMatrix & cmtx,
            int cellsize);

        // ------------------------------------------------

        static void DumpFigurePos(
            const VHOptimalFigure & obj,
            const CellsMatrix & cmtx,
            int showfigidx);

        static void DumpFigures(
            const VHLibOptimal & obj,
            const CellsMatrix & cmtx );

};

};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/log/lognostl.hpp
 * Revision         : 0.8.0-draft-2
 * Content size     : 1900
 * Date / Time      : 15-08-2026 02:03:05
 * MD5              : 79fe2a0137f4b98f31ff459f77f974ba
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */