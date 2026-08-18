/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bitfield/bitfield.hpp
 * Content size  : 2318
 * Date / Time   : 16-08-2026 07:08:10
 * MD5           : 7779fd7b57a4844ae5e55f85a5e3b7a2
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "cfg/cfg.hpp"
#include "cmatrix/cmatrix.hpp"

namespace vhliboptimal
{

    class BitField
    {

    public:
        bool Setup(const CellsMatrix &cmtx, uint8_t *arrptr, uint32_t limbytes);

        VHLIB_OPTIMAL_FASTFUNC
        void ClearArea(const CellsMatrix &cmtx) noexcept;

        VHLIB_OPTIMAL_FASTFUNC
        void ClearBorder(const CellsMatrix &cmtx) noexcept;

        VH_ALWAYS_INLINE
        void ClrCell(int celln) noexcept
        {
            VHBits::BitClr(arrPtr, celln);
        }

        VH_ALWAYS_INLINE
        void SetCell(int celln) noexcept
        {
            VHBits::BitSet(arrPtr, celln);
        }

        VH_ALWAYS_INLINE
        bool GetCell(int celln) const noexcept
        {
            return VHBits::BitVal(arrPtr, celln);
        };

        VH_ALWAYS_INLINE
        void ClrCell(const CellsMatrix &cmtx, int cellx, int celly) noexcept
        {
            int n = cmtx.CellN(cellx, celly);
            VHBits::BitClr(arrPtr, n);
        }

        VH_ALWAYS_INLINE void SetCell(const CellsMatrix &cmtx, int cellx, int celly) noexcept
        {
            int n = cmtx.CellN(cellx, celly);
            VHBits::BitSet(arrPtr, n);
        }

        VH_ALWAYS_INLINE bool GetCell(const CellsMatrix &cmtx, int cellx, int celly) const noexcept
        {
            int n = cmtx.CellN(cellx, celly);
            return GetCell(n);
        }

        VHLIB_OPTIMAL_FASTFUNC
        const int FindEntryCell(const CellsMatrix &cmtx);

        VHLIB_OPTIMAL_FASTFUNC
        const int FindNearest(const CellsMatrix &cmtx, int n) const;

        VHLIB_OPTIMAL_FASTFUNC
        const int FindPath(const CellsMatrix &cmtx, BitField &fldfig);

        VHLIB_OPTIMAL_FASTFUNC
        int ScanSpanLen(const CellsMatrix &cmtx, int startcell, int skipmax) const;

        VHLIB_OPTIMAL_FASTFUNC
        void ClearSpan(const spanword word);

        VHLIB_OPTIMAL_FASTFUNC
        void ResetSearchIndex(const CellsMatrix &cmtx) noexcept;

    private:
        // Ссылка на блок памяти для анализа битового массива

        uint8_t *arrPtr;
        uint32_t arrSizeInBytes;

        size_t curSearchWord;
        size_t lastSearchsByte;

        // Оптимизация под uint32_t / uint64_t
        VHLIB_OPTIMAL_FASTFUNC
        int FastIdxNonZero();
    };

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/bitfield/bitfield.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 2318
 * Date / Time      : 16-08-2026 07:08:10
 * MD5              : 7779fd7b57a4844ae5e55f85a5e3b7a2
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */