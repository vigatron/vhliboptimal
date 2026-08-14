/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/mem/memorylayout.hpp
 * Content size  : 5751
 * Date / Time   : 13-08-2026 20:05:21
 * MD5           : fe57e9925fe5b16d342a07c5a95a2cbf
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "structs/vhliboptimalstructs.hpp"
#include "figure/figure.hpp"

namespace vhliboptimal
{

    static constexpr size_t DEF_GRID_WIDTH = 1 << VHOPTIMAL_GRID_X_LEVEL;
    static constexpr size_t DEF_GRID_HEIGHT = 1 << VHOPTIMAL_GRID_Y_LEVEL;

    static constexpr size_t CFG_MEMSIZE_BYTES_PerGrid = (DEF_GRID_WIDTH >> 3) * DEF_GRID_HEIGHT;
    static constexpr size_t CFG_MEMSIZE_BYTES_Objects = sizeof(VHOptimalFigure) * VHOPTIMAL_OBJECTS_MAX;
    static constexpr size_t CFG_MEMSIZE_BYTES_Spans = sizeof(spanword) * VHOPTIMAL_SPANS_MAX;

    static constexpr size_t CFG_MEMSIZE_BYTES_Total =
        CFG_MEMSIZE_BYTES_PerGrid * 2 +
        CFG_MEMSIZE_BYTES_Objects +
        CFG_MEMSIZE_BYTES_Spans;

    //
    class VHMemoryLayout
    {

    public:
        static_assert(sizeof(VHOptimalFigure) == 16, "16 bytes VHOptimalFigure");
        static_assert(sizeof(spanword) == 4, " 4 bytes for spanword");

        static_assert(VHOPTIMAL_GRID_X_LEVEL > 2);
        static_assert(VHOPTIMAL_GRID_X_LEVEL < 13);

        static_assert(VHOPTIMAL_GRID_Y_LEVEL > 2);
        static_assert(VHOPTIMAL_GRID_Y_LEVEL < 13);

        //
        struct stMemRegion
        {
            uint8_t *ptr;
            size_t size;
        };

        //
        struct stMemLayout
        {
            stMemRegion memSrcGrid;
            stMemRegion memDstGrid;
            stMemRegion memObject;
            stMemRegion memSpans;
        };

        /**
         * Memory Layout Stat
         */
        void ShowMemoryStat()
        {

            int gridWidth = 1 << VHOPTIMAL_GRID_X_LEVEL;
            int gridHeight = 1 << VHOPTIMAL_GRID_Y_LEVEL;

            printf("\n=== VHLibOptimal::CalcMemory() === \n");
            printf("sizeof(VHOptimalFigure) = %d bytes\n", (int)sizeof(VHOptimalFigure));
            printf("sizeof(spanword) = %d bytes\n", (int)sizeof(spanword));
            printf("Grid size: %d x %d\n", gridWidth, gridHeight);
            printf("x1 grid  = %d bytes\n", (int)CFG_MEMSIZE_BYTES_PerGrid);
            printf("x2 grids = %d bytes\n", (int)CFG_MEMSIZE_BYTES_PerGrid * 2);

            printf("%d objects x %d bytes = %d bytes\n",
                   (int)VHOPTIMAL_OBJECTS_MAX,
                   (int)sizeof(VHOptimalFigure),
                   (int)CFG_MEMSIZE_BYTES_Objects);

            printf("%d spans x %d bytes = %d bytes\n",
                   (int)VHOPTIMAL_SPANS_MAX,
                   (int)sizeof(spanword),
                   (int)CFG_MEMSIZE_BYTES_Spans);

            // Dump memory segments

            printf("\n");

            printf("%-20s : @ %p %d bytes\n", "Address SRC_GRID",
                   _mlay.memSrcGrid.ptr, (int)_mlay.memSrcGrid.size);

            printf("%-20s : @ %p %d bytes\n", "Address DST_GRID",
                   _mlay.memDstGrid.ptr, (int)_mlay.memDstGrid.size);

            printf("%-20s : @ %p %d bytes\n", "Address Objects",
                   _mlay.memObject.ptr, (int)_mlay.memObject.size);

            printf("%-20s : @ %p %d bytes\n", "Address Spans",
                   _mlay.memSpans.ptr, (int)_mlay.memSpans.size);

            printf("\n");
            printf(">>> VHLIBOptimal Memory Layout Total: %d bytes\n", (int)CFG_MEMSIZE_BYTES_Total);
            printf("\n");
        }

        /**
         * Memory Layout Setup
         */
        verr SetupMemory(const stMemLayout &memlay)
        {
            size_t align = sizeof(uint32_t);
            bool align1 = checkMemRegion(memlay.memSrcGrid, CFG_MEMSIZE_BYTES_PerGrid, align);
            bool align2 = checkMemRegion(memlay.memDstGrid, CFG_MEMSIZE_BYTES_PerGrid, align);
            bool align3 = checkMemRegion(memlay.memObject, CFG_MEMSIZE_BYTES_Objects, align);
            bool align4 = checkMemRegion(memlay.memSpans, CFG_MEMSIZE_BYTES_Spans, align);

            if (!(align1 && align2 && align3 && align4))
                return verrmsg(100, "MemoryLayout::SetupMemory() issue");

            memcpy(&_mlay, &memlay, sizeof(stMemLayout));

            return vok;
        }

        uint8_t *BitFieldSrcPtr()
        {
            return _mlay.memSrcGrid.ptr;
        }

        size_t BitFieldSrcSize()
        {
            return _mlay.memSrcGrid.size;
        }

        uint8_t *BitFieldDstPtr()
        {
            return _mlay.memDstGrid.ptr;
        }

        size_t BitFieldDstSize()
        {
            return _mlay.memDstGrid.size;
        }

        // Безопасный доступ через reinterpret_cast
        VHOptimalFigure &Obj(size_t pos)
        {
            return *(reinterpret_cast<VHOptimalFigure *>(_mlay.memObject.ptr) + pos);
        }

        const VHOptimalFigure &Obj(size_t pos) const noexcept
        {
            return *(reinterpret_cast<VHOptimalFigure *>(_mlay.memObject.ptr) + pos);
        }

        // Возвращаем по значению без лишнего const
        spanword Spn(size_t pos) const
        {
            return *(reinterpret_cast<spanword *>(_mlay.memSpans.ptr) + pos);
        }

        spanword *GlobalSpans() const
        {
            return reinterpret_cast<spanword *>(_mlay.memSpans.ptr);
        }

        // Запись элемента массива
        void SetSpn(spanword spn, size_t pos)
        {
            *(reinterpret_cast<spanword *>(_mlay.memSpans.ptr) + pos) = spn;
        }

    private:
        /**
         *
         */
        bool checkMemRegion(const stMemRegion &mreg, size_t fixedsize, size_t align)
        {
            if (mreg.ptr == nullptr)
                return false;
            if (!checkAlignment(mreg.ptr, align))
                return false;
            if (mreg.size != fixedsize)
                return false;
            return true;
        }

        /**
         *
         */
        bool checkAlignment(uint8_t *ptr, uint32_t alignbytes)
        {
            return (reinterpret_cast<std::uintptr_t>(ptr) & (alignbytes - 1)) == 0;
        }

        stMemLayout _mlay;
    };

}; // namespace vhliboptimal
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/mem/memorylayout.hpp
 * Revision         : 0.8.0-draft
 * Content size     : 5751
 * Date / Time      : 13-08-2026 20:05:21
 * MD5              : fe57e9925fe5b16d342a07c5a95a2cbf
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */