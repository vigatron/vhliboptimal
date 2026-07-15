/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.1 Draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figures/vhliboptimalfig.hpp
 * Content size  : 1484
 * Date / Time   : 16-07-2026 02:43:20
 * MD5           : 9d58127fbb4e9b149e9993bf5c1aaaef
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhliboptimalstructs.hpp"
#include "cmatrix/cmatrix.hpp"
#include "cmatrix/bitfield.hpp"



class VHOptimalFigure {

    public:

        VHOptimalFigure     (BitField & bfld, const CellsMatrix & cmtx, int skipcellsmax);

        const size_t        SpansCount  () const;
        const stspan    &   Span        (int spanidx) const;

        strect              SpanRect    (int spanidx, const CellsMatrix & cmtx) const;
        const strect    &   Position    () const;

        const int           FindSpanCellLR  (const CellsMatrix & cmtx, uint16_t spancy, int sideFlag) const;
        void                Border          (const CellsMatrix & cmtx, SetPosCallback callbackSetPos);

        // Optimization: Sorting routines

        void                    SortFigureParts ();
        void                    SortWaterfall   ();

    private:

        // Содержит в себе массив участков
        std::vector<stspan>     arrspans;

        // Параметры фигуры
        strect                  objrect;

        // Sort related

        int                     QDistance       (const stspan & p1, const stspan & p2, const CellsMatrix & cmtx) const;
        int                     min_n_by_dist   (const stspan & src, int cnt);

        // Waterfall sort related

        uint8_t                 FindWaterfall   (int atpos, int lim, int *poffs, int *pcnt);
        void                    SwapWaterfall   (int len);


};
/* ========================[  END FILE CONTENT  ]========================
 * File             : src/figures/vhliboptimalfig.hpp
 * Content size     : 1484
 * Date / Time      : 16-07-2026 02:43:20
 * MD5              : 9d58127fbb4e9b149e9993bf5c1aaaef
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */