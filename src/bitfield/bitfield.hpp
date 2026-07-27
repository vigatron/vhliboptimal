/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.4-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bitfield/bitfield.hpp
 * Content size  : 1903
 * Date / Time   : 27-07-2026 18:19:01
 * MD5           : 0a3f1c14e6e287005e147546b92a1ce2
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

        bool                            Setup               (const CellsMatrix & cmtx, uint8_t * arrptr, uint32_t limbytes);

        void                            ClrCell             (int celln);
        void                            SetCell             (int celln);
        bool                            GetCell             (int celln) const;

        void                            ClrCell             (const CellsMatrix & cmtx, int cellx, int celly);
        void                            SetCell             (const CellsMatrix & cmtx, int cellx, int celly);
        bool                            GetCell             (const CellsMatrix & cmtx, int cellx, int celly) const;

        const int                       FindEntryCell       (const CellsMatrix & cmtx);
        const int                       FindNearest         (const CellsMatrix & cmtx, int n) const;
        const int                       FindPath            (const CellsMatrix & cmtx, BitField & fldfig);
        int                             ScanSpanLen         (const CellsMatrix & cmtx, int startcell, int skipmax) const;

        void                            ClearSpan           (const stspan & span);

        void                            ResetSearchIndex    (const CellsMatrix & cmtx);

    private:

        // Ссылка на блок памяти для анализа битового массива

        uint8_t *                       arrPtr;
        uint32_t                        arrSizeInBytes;

        size_t                          curSearchWord;
        size_t                          lastSearchsByte;

        // Оптимизация под uint32_t / uint64_t
        int                             FastIdxNonZero();

};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/bitfield/bitfield.hpp
 * Revision         : 0.7.4-beta
 * Content size     : 1903
 * Date / Time      : 27-07-2026 18:19:01
 * MD5              : 0a3f1c14e6e287005e147546b92a1ce2
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */