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

#include "vhplatform.hpp"

namespace vhliboptimal {

class CellsMatrix {

    public:

        CellsMatrix() { };

        void Setup(
            uint16_t imgpxlsw,
            uint16_t imgpxlsh,
            uint16_t csize);

        const size_t                        CellsX              () const;
        const size_t                        CellsY              () const;
        const size_t                        CellsT              () const;
        const size_t                        CellN               (size_t x, size_t y) const;
        const std::pair<size_t, size_t>     CellXY              (size_t n) const;
        const size_t                        BitMaskSizeBytes    () const;

        // Стартовый индекс поиска
        const size_t                        CellInnerFrom       () const;
        //! Конечный индекс поиска
        const size_t                        CellInnerTo         () const;

    private:

        // Cells per H - Horizontal cells count
        //  6  :   64
        //  7  :  128
        //  8  :  256
        //  9  :  512
        // 10  : 1024
        size_t cellsx;

        // Cells per V - Vertical   cells count
        //  6  :   64
        //  7  :  128
        //  8  :  256
        //  9  :  512
        // 10  : 1024
        size_t cellsy;

        // Cells total
        size_t cellst;

        size_t cellin;        // Within Border: Start cell
        size_t cellout;       // Within Border: Last  cell

        // TODO: Scan Window
        size_t wndx1;
        size_t wndy1;
        size_t wndx2;
        size_t wndy2;
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