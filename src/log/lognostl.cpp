/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/log/lognostl.cpp
 * Content size  : 4875
 * Date / Time   : 22-08-2026 15:42:01
 * MD5           : a7ae8287d881cdc25f096bff17d69641
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#ifndef VHLIB_OPTIMAL_WITHSTL

#include "lognostl.hpp"

using namespace vhliboptimal;


static const char * strbit0 = "0";
static const char * strbit1 = "1";
static const char * strbitP = "*";


// ================================================================

static void newlout() {
    printf("\n");
}

static void partout(const char *msg) {
    printf("%s", msg);
}

static void lineout(const char *msg) {
    printf("%s\n", msg);
}

static void append_param(const char * msg, int val) {
    printf("%s%d", msg, val);
}

static void print_param(const int val) {
    printf("%d", val);
}

static void print_param(const char * msg, int val, bool nl=true) {
    append_param(msg, val);
    if(nl) newlout();
}

// ================================================================

/**
 * 
 */
void log::newlout() {
    ::newlout();
}

/**
 * 
 */
void log::print_param(const char *msg, int val) {
    ::print_param(msg, val);
}

/**
 * 
 */
void log::partint(const int val) {
    ::print_param(val);
}

/**
 * 
 */
void log::partout(const char *msg) {
    ::partout(msg);
}

/**
 * 
 */
void log::lineout(const char *msg) {
    ::lineout(msg);
}

/**
 * @brief General Picture Information
 */
void log::PicProps(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix)
{
    int cs  = 1;
    int w   = cmatrix.CellsX() * cs;
    int h   = cmatrix.CellsY() * cs;

    partout("Picture Props:");
    append_param(" W=", w);
    append_param(" H=", h);

    partout("Cells Matrix: ");
    partout("<");
    append_param("cellsw=", cmatrix.CellsX());
    partout(" ");
    append_param("cellsh=", cmatrix.CellsY());
    partout(">");

    append_param(" total=", cmatrix.CellsT());
    append_param(" cellsize=", cs);
    append_param(" buffer size (bytes) ", cmatrix.BitMaskSizeBytes());

    newlout();
}

/**
 * 
 */
void log::DumpCellsHEX(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix,
    const uint8_t * arrptr,
    const char *msg)
{

    int blocklen = cmatrix.BitMaskSizeBytes();

    if(msg) {
        partout("DumpCellsHEX: ");
        lineout(msg); }

    uint16_t bytescnt = cmatrix.BitMaskSizeBytes();
    
    append_param("Bitfields original ", bytescnt);
    lineout(" bytes");

    partout("(");
    append_param(  " ", cmatrix.CellsX());
    append_param(" x ", cmatrix.CellsY());
    partout(") ");
    append_param(  " ", cmatrix.CellsT());
    lineout(" bits");

    for(int i=0; i < blocklen; i++) {
        if(!(i%16)) { newlout(); printf("%4X: ", i); }
        printf("%.2X", arrptr[i]);
    }

    newlout();
}


void log::DumpCellsTXT(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix,
    const uint8_t * arrptr,
    const char *msg,
    const int cellMarker) {

    if(msg) {
        partout("Dumping: ");
        lineout(msg); }

    const uint8_t sep_dwords = true;
    
    #ifdef VHLIB_OPTIMAL_MODE_32
    const uint8_t sep_size = 32;
    #else
    const uint8_t sep_size = 64;
    #endif

    for(uint16_t cy=0;cy<cmatrix.CellsY();cy++) {

        // Show row
        uint32_t addr = cy * cmatrix.CellsX() >> 3;
        printf(":%-4X | %4d | ", (unsigned int)addr, (unsigned int)cy);

        for(uint16_t cx=0; cx < cmatrix.CellsX(); cx++) {

            // ARCH separator
            if(sep_dwords && !(cx & (sep_size-1))) printf(" ");

            size_t celln = cmatrix.CellN(cx,cy);
            if(cellMarker != -1 && celln == (uint16_t)cellMarker) {
                partout(strbitP);
            } else {
                bool bval = VHBits::BitVal(arrptr, celln);
                partout(bval ? strbit1:strbit0);
            }
        }

        newlout();
    }

}

/**
 * 
 */
void log::DumpSpan(
    const spanword spn,
    const CellsMatrix & cmtx,
    int cellsize,
    int spann)
{

    {
        int spanid = get_span_id  (spann);
        // int spanwd = get_span_len (spann);
        printf("# %5d ", spanid);
    }

    int spanid = get_span_id(spn);
    int spanln = get_span_len(spn);
    auto [cellx, celly] = cmtx.CellXY(spanid);
    int x1 = cellx * cellsize;
    int y1 = celly * cellsize;
    int x2 = (cellx + spanln) * cellsize - 1;
    int y2 = (celly + 1) * cellsize - 1;

    {
        printf(" N: %5d ", spanid);
        printf(" (cx:cy %4d:%4d) L: %3d ", (int)cellx, (int)celly, spanln);
        printf(" sx:sy - ex:ey = %5d:%5d - %5d:%5d", x1, y1, x2, y2);
    }

    newlout();
}

/**
 * 
 */
void log::DumpFigureSpans(
    const VHLibOptimal & detector,
    const VHOptimalFigure & obj,
    const CellsMatrix & cmx,
    int cellsize)
{
    print_param("Spans Count = ", obj.SpansCount());
    print_param("StartSpanIDX # ", obj.StartSpanIDX());

    for(uint32_t i=0; i < obj.SpansCount(); i++) {
        uint32_t globalidx = obj.StartSpanIDX() + i;
        const spanword wspn = detector.GetGlobalSpan(globalidx);
        DumpSpan(wspn, cmx, cellsize, i);
    }

}

#endif
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/log/lognostl.cpp
 * Revision         : 0.8.1
 * Content size     : 4875
 * Date / Time      : 22-08-2026 15:42:01
 * MD5              : a7ae8287d881cdc25f096bff17d69641
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */