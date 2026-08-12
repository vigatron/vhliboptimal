/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figures/vhliboptimalfig.hpp
 * Content size  : 3042
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 5235b21b7f5609e539be680316096ba0
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "structs/vhliboptimalstructs.hpp"
#include "cmatrix/cmatrix.hpp"
#include "bitfield/bitfield.hpp"
#include "structs/vhliboptimalcallbacks.hpp"


namespace vhliboptimal {


class VHLocalSpansArray {

    public:

        void Init(spanword *  arr, uint32_t sidx) {
            arrlocal = arr;
            arrlocal += sidx;
            startIdx = sidx;
            curIdx = 0;
        }

        bool add(spanword word) {
            if(startIdx + curIdx >= VHOPTIMAL_SPANS_MAX) return false;
            arrlocal[curIdx++] = word;
            return true;
        }

        spanword get(uint32_t pos) {
            return arrlocal[pos];
        }

        uint32_t globalstartidx()   { return startIdx; }
        uint32_t localspanscnt()    { return curIdx; }

    private:
        spanword * arrlocal;
        uint32_t startIdx;
        uint32_t curIdx;
};


class VHOptimalFigure {

    public:

        VHOptimalFigure();

        void Init(uint32_t startidx) {
            _startIDX = startidx;
            _spansCount = 0;
        }

        verr Scan(
            BitField & bfld,
            const CellsMatrix & cmtx,
            int skipcellsmax,
            VHLocalSpansArray & localspans
        );

        void CalcPosAndSize  (const CellsMatrix & cmtx, VHLocalSpansArray & arrspans);

        /**
         * @brief Количество участков фигуры
        */
        const uint16_t SpansCount() const { return _spansCount; }
        const uint32_t StartSpanIDX() const { return _startIDX; }

        const VHArea & Area     () const;
        const uint16_t Width    () const;
        const uint16_t Height   () const;

        // const strect                &       PosCells        () const;
        // const strect                        PosAbs          (const CellsMatrix & cmtx) const;

        // const std::vector<stspan>   &       Spans           () const;
        // strect                              SpanRect        (int spanidx, const CellsMatrix & cmtx) const;

        // void                                Sort            (const CellsMatrix & cmtx);
        // const int                           FindPosLRByY    (const CellsMatrix & cmtx, uint16_t spancy, int sideFlag ) const;
        // const int                           FindPosUDByX    (const CellsMatrix & cmtx, uint16_t spancx, int sideFlag ) const;
        void                                Border          (const CellsMatrix & cmtx, CallbackBorder callbackBorder ) const;

        // void                                ContentH        (const CellsMatrix & cmtx, CallbackContent callbackContentH) const;
        // void                                ContentV        (const CellsMatrix & cmtx, CallbackContent callbackContentV) const;

        // static const uint8_t                cmdStart   = 0;
        // static const uint8_t                cmdMove    = 1;
        // static const uint8_t                cmdStop    = 2;

        // static const uint8_t                dirLeft    = 1;
        // static const uint8_t                dirRight   = 2;
        // static const uint8_t                dirUp      = 3;
        // static const uint8_t                dirDown    = 4;

    private:

        // Позиция и размеры фигуры
        VHArea      _area;

        uint32_t    _reserved;

        // Стартовый индекс отрезков фигур
        uint16_t    _startIDX;

        // Количество отрезков
        uint16_t    _spansCount;


        // // Calculate Distance

        // int QDistance(
        //     const stspan & p1,
        //     const stspan & p2,
        //     const CellsMatrix & cmtx) const;

        // // Find Closest Span

        // int FindClosestSpan(
        //     const stspan & span,
        //     const std::vector<stspan> & arr,
        //     const CellsMatrix & cmtx) const;

        // // Optimization: Sorting parts

        // void SortSequental(const CellsMatrix & cmtx);

} __attribute__((packed));

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/figures/vhliboptimalfig.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 3042
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 5235b21b7f5609e539be680316096ba0
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */