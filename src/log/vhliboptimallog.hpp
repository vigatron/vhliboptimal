/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.1 Draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/log/vhliboptimallog.hpp
 * Content size  : 1467
 * Date / Time   : 16-07-2026 02:43:20
 * MD5           : 712b29636d0d6242937806af0e1efb47
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhliboptimal.hpp"

class VHLibOptimalLogger {

    public:

        static void partout(const std::string & str);

        static void newlout();

        static void lineout(const std::string & str);

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

        static void PicProps(
            const VHLibOptimal & obj,
            const CellsMatrix & cmatrix);

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

        static void DumpFigurePos(
            const VHOptimalFigure & objfig,
            int showfigidx);

        static void DumpOPATH(const VHLibOptimal & obj);

        static void DumpCell(
            const std::string & msg,
            int celln,
            int cellx, int celly,
            int sx, int sy );

};
/* ========================[  END FILE CONTENT  ]========================
 * File             : src/log/vhliboptimallog.hpp
 * Content size     : 1467
 * Date / Time      : 16-07-2026 02:43:20
 * MD5              : 712b29636d0d6242937806af0e1efb47
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */