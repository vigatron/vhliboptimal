/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figure/figure.hpp
 * Content size  : 4035
 * Date / Time   : 13-08-2026 20:05:21
 * MD5           : 430594ccd78e626960fe5f4d1294e192
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "structs/vhliboptimalstructs.hpp"
#include "cmatrix/cmatrix.hpp"
#include "bitfield/bitfield.hpp"
#include "spans/spans.hpp"
#include "structs/vhliboptimalcallbacks.hpp"

namespace vhliboptimal
{

    class VHOptimalFigure
    {

    public:

        static const uint8_t                cmdStart   = 0;
        static const uint8_t                cmdMove    = 1;
        static const uint8_t                cmdStop    = 2;

        static const uint8_t                dirLeft    = 1;
        static const uint8_t                dirRight   = 2;
        static const uint8_t                dirUp      = 3;
        static const uint8_t                dirDown    = 4;

        /**
         *
         */
        VHOptimalFigure();

        /**
         * 
         */
        void Init(uint32_t startidx)
        {
            _startIDX = startidx;
            _spansCount = 0;
        }

        /**
         *
         */
        verr Scan(
            BitField &bfld,
            const CellsMatrix &cmtx,
            int skipcellsmax,
            VHLocalSpansArray &localspans);

        /**
         *
         */
        void CalcPosAndSize(const CellsMatrix &cmtx, VHLocalSpansArray &arrspans);

        /**
         * @brief Количество участков фигуры
         */
        const uint16_t SpansCount() const { return _spansCount; }

        /**
         *
         */
        const uint32_t StartSpanIDX() const { return _startIDX; }

        /**
         *
         */
        const VHArea &Area() const;

        /**
         *
         */
        const uint16_t Width() const;

        /**
         *
         */
        const uint16_t Height() const;

        // const strect                &       PosCells        () const;
        // const strect                        PosAbs          (const CellsMatrix & cmtx) const;

        // const std::vector<stspan>   &       Spans           () const;
        // strect                              SpanRect        (int spanidx, const CellsMatrix & cmtx) const;

        // void                                Sort            (const CellsMatrix & cmtx);
        // const int                           FindPosLRByY    (const CellsMatrix & cmtx, uint16_t spancy, int sideFlag ) const;
        // const int                           FindPosUDByX    (const CellsMatrix & cmtx, uint16_t spancx, int sideFlag ) const;

        /**
         * 
         */
        void Border(const CellsMatrix &cmtx, CallbackBorder callbackBorder) const;

        // void                                ContentH        (const CellsMatrix & cmtx, CallbackContent callbackContentH) const;
        // void                                ContentV        (const CellsMatrix & cmtx, CallbackContent callbackContentV) const;



    private:
        // Позиция и размеры фигуры
        VHArea _area;

        uint32_t _reserved;

        // Стартовый индекс отрезков фигур
        uint16_t _startIDX;

        // Количество отрезков
        uint16_t _spansCount;

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
 * File             : src/figure/figure.hpp
 * Revision         : 0.8.0-draft
 * Content size     : 4035
 * Date / Time      : 13-08-2026 20:05:21
 * MD5              : 430594ccd78e626960fe5f4d1294e192
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */