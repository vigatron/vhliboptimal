/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bitfield/bitfield.hpp
 * Content size  : 1903
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 365b4ad00048a4f22357dbb7e950f1ca
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "cmatrix/cmatrix.hpp"
#include "cfg/cfg.hpp"

namespace vhliboptimal {

class BitField {

    public:

        bool Setup(const CellsMatrix & cmtx, uint8_t * arrptr, uint32_t limbytes);

        void ClearArea      (const CellsMatrix & cmtx) noexcept;
        void ClearBorder    (const CellsMatrix & cmtx) noexcept;

        // Принудительный инлайнинг для микро-методов доступа
        #define VH_INLINE __attribute__((always_inline)) inline

        VH_INLINE void      ClrCell (int celln) { VHBits::BitClr(arrPtr, celln); }
        VH_INLINE void      SetCell (int celln) { VHBits::BitSet(arrPtr, celln); }
        VH_INLINE bool      GetCell (int celln) const { return VHBits::BitVal(arrPtr, celln); };

        VH_INLINE void      ClrCell (const CellsMatrix & cmtx, int cellx, int celly) {
            int n = cmtx.CellN(cellx, celly);
            VHBits::BitClr(arrPtr, n);
        }

        VH_INLINE void      SetCell (const CellsMatrix & cmtx, int cellx, int celly) {
            int n = cmtx.CellN(cellx, celly);
            VHBits::BitSet(arrPtr, n);
        }

        VH_INLINE bool      GetCell (const CellsMatrix & cmtx, int cellx, int celly) const {
            int n = cmtx.CellN(cellx, celly);
            return GetCell(n);
        }

        const int           FindEntryCell       (const CellsMatrix & cmtx);
        const int           FindNearest         (const CellsMatrix & cmtx, int n) const;
        const int           FindPath            (const CellsMatrix & cmtx, BitField & fldfig);
        int                 ScanSpanLen         (const CellsMatrix & cmtx, int startcell, int skipmax) const;

        void                ClearSpan           (const spanword word);

        void                ResetSearchIndex    (const CellsMatrix & cmtx) noexcept;

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
 * Revision         : 0.8.0-beta
 * Content size     : 1903
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 365b4ad00048a4f22357dbb7e950f1ca
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */