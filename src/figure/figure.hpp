/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft-2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figure/figure.hpp
 * Content size  : 3185
 * Date / Time   : 15-08-2026 02:03:05
 * MD5           : 1b5b2d1f8a5105995311bd87d87b0bda
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
        void CalcPosAndSize(
            const CellsMatrix &cmtx,
            const VHLocalSpansArray &arrspans);

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


        /**
         * 
         */
        const int FindPosLRByY(
            const CellsMatrix & cmtx,
            const VHLocalSpansArray &arrspans,
            uint16_t spancy,
            int sideFlag ) const;

        /**
         * 
         */
        const int FindPosUDByX(
            const CellsMatrix & cmtx,
            const VHLocalSpansArray &arrspans,
            uint16_t spancx,
            int sideFlag ) const;

        /**
         * 
         */
        void Border(
            const CellsMatrix &cmtx,
            const VHLocalSpansArray &arrspans,
            void *caller,
            CallbackBorder callbackBorder) const;

        /**
         * 
         */
        void ContentH(
            const CellsMatrix & cmtx,
            const VHLocalSpansArray &arrspans,
            void *caller,
            CallbackContent callbackContentH) const;

        /**
         * 
         */
        void ContentV(
            const CellsMatrix & cmtx,
            const VHLocalSpansArray &arrspans,
            void *caller,
            CallbackContent callbackContentV) const;


    private:
        // Позиция и размеры фигуры
        VHArea _area;

        //
        uint32_t _reserved;

        // Стартовый индекс отрезков фигур
        uint16_t _startIDX;

        // Количество отрезков
        uint16_t _spansCount;

    } __attribute__((packed));

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/figure/figure.hpp
 * Revision         : 0.8.0-draft-2
 * Content size     : 3185
 * Date / Time      : 15-08-2026 02:03:05
 * MD5              : 1b5b2d1f8a5105995311bd87d87b0bda
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */