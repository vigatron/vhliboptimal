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
