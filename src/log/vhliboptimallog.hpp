/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/log/vhliboptimallog.hpp
 * Content size  : 2113
 * Date / Time   : 24-07-2026 11:54:48
 * MD5           : 16ab4fae17973e0e1599cf28f077c69b
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhliboptimal.hpp"

namespace vhliboptimal {

class VHLibOptimalLogger {

    public:

        // ------------------------------------------------

        static void partout(const std::string & str);

        static void newlout();

        static void lineout(const std::string & str);

        // ------------------------------------------------

        static std::string fmt(
            const std::string & strparam,
            int value);

        static std::string fmt(
            const std::string & strparam1,
            int value,
            const std::string & strparam2);

        static std::string fmt(
            int value1,
            const std::string & strparam,
            int value2);


        // ------------------------------------------------

        static void PicProps(
            const VHLibOptimal & obj,
            const CellsMatrix & cmatrix);

        // ------------------------------------------------

        static void DumpCellsHEX(
            const VHLibOptimal & obj,
            const CellsMatrix & cmatrix,
            const std::vector<uint8_t> & arr,
            const char *msg = nullptr);

        static void DumpCellsTXT(
            const VHLibOptimal & obj,
            const CellsMatrix & cmatrix,
            const std::vector<uint8_t> & arr,
            const char *msg = nullptr,
            const int cellMarker = -1 );

        static void DumpCell(
            const std::string & msg,
            int celln,
            int cellx, int celly,
            int sx, int sy );

        // ------------------------------------------------

        static void DumpSpan(
            const stspan & spn,
            const CellsMatrix & cmx,
            int spann);

        static void DumpFigureSpans(
            const VHOptimalFigure & objfig,
            const CellsMatrix & cmx );

        // ------------------------------------------------

        static void DumpFigurePos(
            const VHOptimalFigure & objfig,
            int showfigidx);


        static void DumpFigures(const VHLibOptimal & obj);

};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/log/vhliboptimallog.hpp
 * Revision         : 0.7.0-beta
 * Content size     : 2113
 * Date / Time      : 24-07-2026 11:54:48
 * MD5              : 16ab4fae17973e0e1599cf28f077c69b
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */