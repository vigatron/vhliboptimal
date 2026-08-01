/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/cmatrix.hpp
 * Content size  : 1310
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 783cb302428912067bf21c9b768ef856
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "structs/vhliboptimalstructs.hpp"

namespace vhliboptimal {

class CellsMatrix {

    public:

        CellsMatrix();

        bool Setup(uint8_t levelx, uint8_t levely);

        const size_t                        CellsX              () const;
        const size_t                        CellsY              () const;
        const size_t                        CellsT              () const;
        const size_t                        CellN               (size_t x, size_t y) const;
        const std::pair<size_t, size_t>     CellXY              (size_t n) const;

        // Подсчет размера в байтах
        const size_t                        BitMaskSizeBytes    () const;

        // Стартовый индекс поиска
        const size_t                        CellInnerFrom       () const;

        //! Конечный индекс поиска
        const size_t                        CellInnerTo         () const;

    private:


        size_t cellin;        // Within Border: Start cell
        size_t cellout;       // Within Border: Last  cell

        // TODO: Scan Window
        size_t wndx1;
        size_t wndy1;
        size_t wndx2;
        size_t wndy2;


        #ifdef VHLIB_OPTIMAL_GRID_FIXED

        static constexpr int fixedw = (1 << VHLIB_OPTIMAL_GRID_LX); 
        static constexpr int fixedh = (1 << VHLIB_OPTIMAL_GRID_LY);
        static constexpr int fixedt = fixedw * fixedh;

        static constexpr int fixedm = fixedw-1;

        #else

        // Cells per H - Horizontal cells count
        uint8_t levx;

        // Cells per V - Vertical   cells count
        uint8_t levy;

        // Cells total
        size_t cellst;

        #endif
};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/cmatrix.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 1310
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 783cb302428912067bf21c9b768ef856
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */