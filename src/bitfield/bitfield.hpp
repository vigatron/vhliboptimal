/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bitfield/bitfield.hpp
 * Content size  : 2484
 * Date / Time   : 24-08-2026 16:26:25
 * MD5           : 70196ed18cab14506ae3b14d18311d1f
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

        // VHLIB_OPTIMAL_FASTFUNC
        void ClearArea(const CellsMatrix &cmtx) noexcept;

        // VHLIB_OPTIMAL_FASTFUNC
        void ClearBorder(const CellsMatrix &cmtx) noexcept;

        // VH_ALWAYS_INLINE
        void ClrCell(int celln) noexcept
        {
            VHBits::BitClr(arrPtr, celln);
        }

        // VH_ALWAYS_INLINE
        void SetCell(int celln) noexcept
        {
            VHBits::BitSet(arrPtr, celln);
        }

        // VH_ALWAYS_INLINE
        bool GetCell(int celln) const noexcept
        {
            return VHBits::BitVal(arrPtr, celln);
        };

        // VH_ALWAYS_INLINE
        void ClrCell(const CellsMatrix &cmtx, int cellx, int celly) noexcept
        {
            int n = cmtx.CellN(cellx, celly);
            VHBits::BitClr(arrPtr, n);
        }

        // VH_ALWAYS_INLINE
        void SetCell(const CellsMatrix &cmtx, int cellx, int celly) noexcept
        {
            int n = cmtx.CellN(cellx, celly);
            VHBits::BitSet(arrPtr, n);
        }

        // VH_ALWAYS_INLINE
        bool GetCell(const CellsMatrix &cmtx, int cellx, int celly) const noexcept
        {
            int n = cmtx.CellN(cellx, celly);
            return GetCell(n);
        }

        // VHLIB_OPTIMAL_FASTFUNC
        int FindEntryCell(const CellsMatrix &cmtx) noexcept;

        // VHLIB_OPTIMAL_FASTFUNC
        int FindNearest(const CellsMatrix &cmtx, int n) const noexcept;

        // VHLIB_OPTIMAL_FASTFUNC
        int FindPath(const CellsMatrix &cmtx, BitField &fldfig) noexcept;

        // VHLIB_OPTIMAL_FASTFUNC
        int ScanSpanLen(const CellsMatrix &cmtx, int startcell, int skipmax) const noexcept;

        // VHLIB_OPTIMAL_FASTFUNC
        void ClearSpan(const spanword word) noexcept;

        // VH_ALWAYS_INLINE
        void ResetSearchIndex(const CellsMatrix &cmtx) noexcept;

    private:
        // Ссылка на блок памяти для анализа битового массива

        uint8_t *arrPtr;
        uint32_t arrSizeInBytes;

        size_t curSearchWord;
        size_t lastSearchsByte;

        // Оптимизация под uint32_t / uint64_t
        // VH_ALWAYS_INLINE
        // VHLIB_OPTIMAL_FASTFUNC
        int FastIdxNonZero() noexcept;
    };

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/bitfield/bitfield.hpp
 * Revision         : 0.8.1
 * Content size     : 2484
 * Date / Time      : 24-08-2026 16:26:25
 * MD5              : 70196ed18cab14506ae3b14d18311d1f
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */