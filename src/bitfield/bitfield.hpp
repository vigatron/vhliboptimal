/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bitfield/bitfield.hpp
 * Content size  : 1753
 * Date / Time   : 24-07-2026 11:54:48
 * MD5           : 7b79cc239c9264a885aa563ab667f9b4
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

        const int                       FindEntryCell       (const CellsMatrix & cmtx) const;
        const int                       FindNearest         (const CellsMatrix & cmtx, int n) const;
        const int                       FindPath            (const CellsMatrix & cmtx, const BitField & fldfig) const;
        int                             ScanSpanLen         (const CellsMatrix & cmtx, int startcell, int skipmax) const;

        void                            ClearSpan           (const stspan & span);

    private:

        // Ссылка на блок памяти для анализа битового массива

        uint8_t *                       arrPtr;
        uint32_t                        arrSizeInBytes;

        // Оптимизация под uint8_t / uint16_t / uint32_t / uint64_t / AVX2

        int                             FastIdxNonZero() const;
};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/bitfield/bitfield.hpp
 * Revision         : 0.7.0-beta
 * Content size     : 1753
 * Date / Time      : 24-07-2026 11:54:48
 * MD5              : 7b79cc239c9264a885aa563ab667f9b4
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */