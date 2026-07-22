/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.6beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figures/vhliboptimalfig.hpp
 * Content size  : 2206
 * Date / Time   : 22-07-2026 14:54:04
 * MD5           : 99b00822c19c7040e741b37747faeffb
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhliboptimalstructs.hpp"
#include "cmatrix/cmatrix.hpp"
#include "bitfield/bitfield.hpp"


namespace vhliboptimal {

class VHOptimalFigure {

    public:

        VHOptimalFigure(BitField & bfld, const CellsMatrix & cmtx, int skipcellsmax);

        const strect                &       PosCells        () const;
        const strect                        PosAbs          (const CellsMatrix & cmtx) const;

        const std::vector<stspan>   &       Spans           () const;
        const size_t                        SpansCount      () const;
        const stspan                &       Span            (int spanidx) const;
        strect                              SpanRect        (int spanidx, const CellsMatrix & cmtx) const;

        void                                Sort            (const CellsMatrix & cmtx);
        const int                           FindSpanByYLR   (const CellsMatrix & cmtx, uint16_t spancy, int sideFlag) const;
        void                                Border          (const CellsMatrix & cmtx, CallbackBorder callbackBorder ) const;
        void                                Content         (const CellsMatrix & cmtx, CallbackContent callbackContent) const;


        static const uint8_t cmdStart   = 0;
        static const uint8_t cmdMove    = 1;
        static const uint8_t cmdStop    = 2;

        static const uint8_t dirLeft    = 1;
        static const uint8_t dirRight   = 2;
        static const uint8_t dirUp      = 3;
        static const uint8_t dirDown    = 4;

    private:

        // Содержит в себе массив участков

        std::vector<stspan>     arrspans;

        // Параметры фигуры

        strect                  objrect;

        // Calculate Distance

        int QDistance(
            const stspan & p1,
            const stspan & p2,
            const CellsMatrix & cmtx) const;

        // Find Closest Span

        int FindClosestSpan(
            const stspan & span,
            const std::vector<stspan> & arr,
            const CellsMatrix & cmtx) const;

        // Optimization: Sorting parts

        void SortSequental(const CellsMatrix & cmtx);

};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/figures/vhliboptimalfig.hpp
 * Revision         : 0.6beta
 * Content size     : 2206
 * Date / Time      : 22-07-2026 14:54:04
 * MD5              : 99b00822c19c7040e741b37747faeffb
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */