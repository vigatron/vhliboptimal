/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.1 Draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/bitfield.hpp
 * Content size  : 1556
 * Date / Time   : 16-07-2026 02:43:20
 * MD5           : 25398ef1289da04d8edae3b7f5dac8f5
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhplatform.hpp"

#include "cmatrix.hpp"
#include "vhliboptimalstructs.hpp"

class BitField {

    public:

        void                            Setup               (const CellsMatrix & cmtx);
        const std::vector<uint8_t> &    arr                 () const;
        void                            Clr                 (size_t celln);
        void                            Set                 (size_t celln);
        bool                            Get                 (size_t celln) const;
        const int                       FindEntryCell       (const CellsMatrix & cmtx) const;
        const int                       FindNearest         (const CellsMatrix & cmtx, int n) const;
        const int                       FindPath            (const CellsMatrix & cmtx, const BitField & fldfig) const;
        size_t                          ScanSpanLen         (const CellsMatrix & cmtx, size_t startcell, int skipmax) const;
        void                            ClearSpan           (const stspan & span);

    private:

        // Макс площадь фигуры в байтах
        // Количество ячеек = sqrt(SCT_LIM*8)
        // 2048 * 8 = 16.384 бит = 128x128 = 2048x2048 max
        // 4096 * 8 = 32.768 бит = 182х182 = 2896х2896 max
        static const size_t SCT_BYTES_MAX = F1K * 4;
        static const size_t SCT_COUNT_MAX = SCT_BYTES_MAX * 8;

        // Память для анализа полного изображения
        std::vector<uint8_t>    arrmem;


};
/* ========================[  END FILE CONTENT  ]========================
 * File             : src/cmatrix/bitfield.hpp
 * Content size     : 1556
 * Date / Time      : 16-07-2026 02:43:20
 * MD5              : 25398ef1289da04d8edae3b7f5dac8f5
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */