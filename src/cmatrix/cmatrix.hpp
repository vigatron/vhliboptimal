/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft-2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/cmatrix.hpp
 * Content size  : 124
 * Date / Time   : 15-08-2026 02:03:05
 * MD5           : fbdef1d47676633032827193ad13e38f
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
        bool Setup(uint8_t levelx, uint8_t levely)
        {
#ifdef VHLIB_OPTIMAL_GRID_FLEX
            levx = levelx;
            levy = levely;
            cellst = CellsX() * CellsY();
            cellin = CellsX() + 1;
            cellout = cellst - CellsX() - 2;
#endif

#ifdef VHLIB_OPTIMAL_GRID_FIXED
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
            return 1 << levx;
        }

        inline constexpr size_t CellsY() const noexcept
        {
            return 1 << levy;
        }

        inline constexpr size_t CellsT() const noexcept
        {
            return cellst;
        }

        inline constexpr size_t CellN(size_t x, size_t y) const noexcept
        {
            return y * CellsX() + x;
        }

        // Стартовый индекс поиска
        inline constexpr size_t CellInnerFrom() const noexcept
        {
            return cellin;
        }

        //! Конечный индекс поиска
        inline constexpr size_t CellInnerTo() const noexcept
        {
            return cellout;
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
        struct stScanWnd {
            size_t sx;
            size_t sy1;
            size_t ex;
            size_t ey;
        };


#ifdef VHLIB_OPTIMAL_GRID_FLEX
        uint8_t levx;   // Cells per H - Horizontal cells count
        uint8_t levy;   // Cells per V - Vertical   cells count
        size_t cellst;  // Cells total
        size_t cellin;  // Within Border: Start cell
        size_t cellout; // Within Border: Last  cell
#endif

#ifdef VHLIB_OPTIMAL_GRID_FIXED

        // static constexpr int VHOPTIMAL_GRID_X_LEVEL = VHLIB_OPTIMAL_GRID_LX;
        // static constexpr int VHOPTIMAL_GRID_Y_LEVEL = VHLIB_OPTIMAL_GRID_LY;
        // static constexpr int VHOPTIMAL_GRID_S_ORDER = 1;

        static constexpr size_t fixedw = (1 << VHLIB_OPTIMAL_GRID_LX);
        static constexpr size_t fixedh = (1 << VHLIB_OPTIMAL_GRID_LY);
        static constexpr size_t fixedt = fixedw * fixedh;
        
        // mask for fast AND
        static constexpr size_t fixedm = fixedw - 1;

#endif

    }; // CellsMatrix

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/cmatrix.hpp
 * Revision         : 0.8.0-draft-2
 * Content size     : 124
 * Date / Time      : 15-08-2026 02:03:05
 * MD5              : fbdef1d47676633032827193ad13e38f
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */