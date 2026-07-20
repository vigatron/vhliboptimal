/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/bitfield.hpp
 * Content size  : 1427
 * Date / Time   : 20-07-2026 03:16:52
 * MD5           : 3b8b4b87088d624e4ca22059c3348fcd
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhplatform.hpp"

#include "cmatrix.hpp"
#include "vhliboptimalstructs.hpp"

namespace vhliboptimal {

class BitField {

    public:

        void                            Setup               (const CellsMatrix & cmtx);
        const std::vector<uint8_t> &    arr                 () const;

        void                            ClrCell             (int celln);
        void                            ClrCell             (const CellsMatrix & cmtx, int cellx, int celly);
        void                            SetCell             (int celln);
        void                            SetCell             (const CellsMatrix & cmtx, int cellx, int celly);
        bool                            GetCell             (int celln) const;

        const int                       FindEntryCell       (const CellsMatrix & cmtx) const;
        const int                       FindNearest         (const CellsMatrix & cmtx, int n) const;
        const int                       FindPath            (const CellsMatrix & cmtx, const BitField & fldfig) const;
        int                             ScanSpanLen         (const CellsMatrix & cmtx, int startcell, int skipmax) const;
        void                            ClearSpan           (const stspan & span);

    private:

        // Память для анализа полного изображения
        std::vector<uint8_t>    arrmem;
};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/bitfield.hpp
 * Revision         : 0.2
 * Content size     : 1427
 * Date / Time      : 20-07-2026 03:16:52
 * MD5              : 3b8b4b87088d624e4ca22059c3348fcd
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */