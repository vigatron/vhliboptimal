/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/cmatrix.hpp
 * Content size  : 4995
 * Date / Time   : 19-08-2026 21:18:15
 * MD5           : 0873eecdc469e88a4743504a7e397cdc
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "cfg/cfg.hpp"

// * GRID TYPE *
//
// - VHLIB_OPTIMAL_GRID_FLEX
// - VHLIB_OPTIMAL_GRID_FIXED

#ifdef VHLIB_OPTIMAL_PLATFORM_PC

#ifndef VHLIB_OPTIMAL_GRID_FIXED
#define VHLIB_OPTIMAL_GRID_FLEX
#endif

#endif

#ifdef VHLIB_OPTIMAL_PLATFORM_EMBEDDED

#ifdef VHLIB_OPTIMAL_GRID_FLEX
#error "Only fixed size grid supported for bare-metal!"
#endif

// Fixed grid mode for bare-metal
#ifndef VHLIB_OPTIMAL_GRID_FIXED
#define VHLIB_OPTIMAL_GRID_FIXED
#endif

#endif

// if not set default value is 8 (256 pxls)
#ifndef VHLIB_OPTIMAL_GRID_LX
#define VHLIB_OPTIMAL_GRID_LX 8
#endif

// if not set default value is 8 (256 pxls)
#ifndef VHLIB_OPTIMAL_GRID_LY
#define VHLIB_OPTIMAL_GRID_LY 8
#endif

// Max grid size 12 ( 4096 )
static_assert(VHLIB_OPTIMAL_GRID_LX > 2);
static_assert(VHLIB_OPTIMAL_GRID_LX < 13);

// Max grid size 12 ( 4096 )
static_assert(VHLIB_OPTIMAL_GRID_LY > 2);
static_assert(VHLIB_OPTIMAL_GRID_LY < 13);

namespace vhliboptimal
{

    class CellsMatrix
    {

    public:
        struct Point
        {
            uint16_t x;
            uint16_t y;
        };

        /**
         *
         */
        explicit CellsMatrix()
        {
        }

        /**
         *
         */
        bool Setup(uint16_t gridw, uint16_t gridh)
        {

#ifdef VHLIB_OPTIMAL_GRID_FLEX
            _cellsw = gridw;
            _cellsh = gridh;
            _cellst = CellsX() * CellsY();
#endif

#ifdef VHLIB_OPTIMAL_GRID_FIXED
            // check-only corresponded parameters
            // because params for GRID_FIXED are const
            uint8_t levelx = bits_needed(gridw - 1);
            uint8_t levely = bits_needed(gridh - 1);
            bool flageqx = (levelx == VHLIB_OPTIMAL_GRID_LX);
            bool flageqy = (levely == VHLIB_OPTIMAL_GRID_LY);

            if (!(flageqx && flageqy))
                return false;
#endif

            return true;
        }

#ifdef VHLIB_OPTIMAL_GRID_FLEX

        inline constexpr size_t CellsX() const noexcept
        {
            return _cellsw;
        }

        inline constexpr size_t CellsY() const noexcept
        {
            return _cellsh;
        }

        inline constexpr size_t CellsT() const noexcept
        {
            return _cellst;
        }

        inline constexpr size_t CellN(size_t x, size_t y) const noexcept
        {
            return y * CellsX() + x;
        }

        //
        inline constexpr std::pair<size_t, size_t> CellXY(size_t n) const noexcept
        {
            return {
                static_cast<size_t>(n % CellsX()),
                static_cast<size_t>(n / CellsX())};
        }

#endif

#ifdef VHLIB_OPTIMAL_GRID_FIXED

        inline constexpr size_t CellsX() const noexcept
        {
            return fixedw;
        }

        inline constexpr size_t CellsY() const noexcept
        {
            return fixedh;
        }

        inline constexpr size_t CellsT() const noexcept
        {
            return fixedt;
        }

        /**
         *
         */
        inline constexpr size_t CellN(size_t x, size_t y) const noexcept
        {
            return (y << VHLIB_OPTIMAL_GRID_LX) + x;
        }

        //
        inline constexpr Point CellXY(size_t n) const noexcept
        {
            return Point{
                static_cast<uint16_t>(n & fixedm),
                static_cast<uint16_t>(n >> VHLIB_OPTIMAL_GRID_LX)};
        }

#endif

        // Подсчет размера в байтах
        inline constexpr size_t BitMaskSizeBytes() const noexcept
        {
            size_t arrsz = (CellsT() / CHAR_BIT) + ((CellsT() % CHAR_BIT) ? 1 : 0);
            return arrsz;
        }

        // Стартовый индекс поиска
        constexpr size_t CellCornerTopLeft() const noexcept
        {
            return CellsX() + 1;
        }

        //! Конечный индекс поиска
        constexpr size_t CellCornerBottomRight() const noexcept
        {
            return CellsT() - CellsX() - 2;
        }

    private:
        // TODO: Scan Window
        struct stScanWnd
        {
            size_t sx;
            size_t sy;
            size_t ex;
            size_t ey;
        };

#ifdef VHLIB_OPTIMAL_GRID_FLEX
        uint16_t _cellsw; // Cells per H - Horizontal cells count
        uint16_t _cellsh; // Cells per V - Vertical   cells count
        size_t _cellst;   // Cells total
#endif

#ifdef VHLIB_OPTIMAL_GRID_FIXED

        static constexpr size_t fixedw = (1 << VHLIB_OPTIMAL_GRID_LX);
        static constexpr size_t fixedh = (1 << VHLIB_OPTIMAL_GRID_LY);
        static constexpr size_t fixedt = fixedw * fixedh;

        // mask for fast AND
        static constexpr size_t fixedm = fixedw - 1;

#endif

        uint8_t bits_needed(uint16_t value)
        {
            uint8_t bits = 0;
            while (value)
            {
                ++bits;
                value >>= 1;
            }
            return bits ? bits : 1;
        }

    }; // CellsMatrix

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/cmatrix.hpp
 * Revision         : 0.8.0
 * Content size     : 4995
 * Date / Time      : 19-08-2026 21:18:15
 * MD5              : 0873eecdc469e88a4743504a7e397cdc
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */