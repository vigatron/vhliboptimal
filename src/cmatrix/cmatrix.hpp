/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/cmatrix.hpp
 * Content size  : 855
 * Date / Time   : 20-07-2026 03:16:52
 * MD5           : fee073be96383370b77e7ea3a36a3c1b
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhplatform.hpp"

class CellsMatrix {

    public:

        CellsMatrix() { };

        void                            Setup       (uint16_t imgpxlsw, uint16_t imgpxlsh, uint16_t csize);
        const size_t                    CellsX      () const;
        const size_t                    CellsY      () const;
        const size_t                    CellsT      () const;
        const size_t                    CellSize    () const;
        const size_t                    CellN       (size_t x, size_t y) const;
        const std::pair<size_t, size_t> CellXY      (size_t n) const;

    private:

        size_t cellsx;        // Cells per H - Horizontal cells count
        size_t cellsy;        // Cells per V - Vertical   cells count
        size_t cellst;        // Cells total
        size_t cellsz;        // Cell size
};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/cmatrix.hpp
 * Revision         : 0.2
 * Content size     : 855
 * Date / Time      : 20-07-2026 03:16:52
 * MD5              : fee073be96383370b77e7ea3a36a3c1b
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */