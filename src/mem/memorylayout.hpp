/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/mem/memorylayout.hpp
 * Content size  : 6432
 * Date / Time   : 16-08-2026 07:08:10
 * MD5           : 20c940ba1d9c3a3472c23a012569f2f1
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "cfg/cfg.hpp"
#include "structs/vhliboptimalstructs.hpp"
#include "figure/figure.hpp"

#include "memregion.hpp"

namespace vhliboptimal
{
    static constexpr size_t CFG_GRID_WIDTH_MAX = (1 << VHLIB_OPTIMAL_GRID_LX);
    static constexpr size_t CFG_GRID_HEIGHT_MAX = (1 << VHLIB_OPTIMAL_GRID_LY);
    static constexpr size_t CFG_MEMSIZE_BYTES_PerGrid =  (CFG_GRID_WIDTH_MAX >> 3 ) * CFG_GRID_HEIGHT_MAX;
    static constexpr size_t CFG_MEMSIZE_BYTES_Objects = sizeof(VHOptimalFigure) * VHLIB_OPTIMAL_OBJS_MAX;
    static constexpr size_t CFG_MEMSIZE_BYTES_Spans = sizeof(spanword) * VHLIB_OPTIMAL_SPNS_MAX;

    static constexpr size_t CFG_MEMSIZE_BYTES_Total =
        CFG_MEMSIZE_BYTES_PerGrid * 2 +
        CFG_MEMSIZE_BYTES_Objects +
        CFG_MEMSIZE_BYTES_Spans;

    static_assert(sizeof(spanword) == 4, " 4 bytes for spanword");

    static_assert(sizeof(VHOptimalFigure) == 16, "16 bytes VHOptimalFigure");

    //
    class VHMemoryLayout
    {

    public:
        /**
         * Memory Layout Stat
         */
        void ShowMemoryStat()
        {

            printf("\n=== VHLibOptimal::CalcMemory() === \n");
            printf("sizeof(VHOptimalFigure) = %d bytes\n", (int)sizeof(VHOptimalFigure));
            printf("sizeof(spanword) = %d bytes\n", (int)sizeof(spanword));

            int gridWidth = 1 << VHLIB_OPTIMAL_GRID_LX;
            int gridHeight = 1 << VHLIB_OPTIMAL_GRID_LY;
            printf("Grid size Max: %d x %d\n", gridWidth, gridHeight);

            printf("x1 grid  = %d bytes\n", (int)CFG_MEMSIZE_BYTES_PerGrid);
            printf("x2 grids = %d bytes\n", (int)CFG_MEMSIZE_BYTES_PerGrid * 2);

            printf("%d objects x %d bytes = %d bytes\n",
                   (int)VHLIB_OPTIMAL_OBJS_MAX,
                   (int)sizeof(VHOptimalFigure),
                   (int)CFG_MEMSIZE_BYTES_Objects);

            printf("%d spans x %d bytes = %d bytes\n",
                   (int)VHLIB_OPTIMAL_SPNS_MAX,
                   (int)sizeof(spanword),
                   (int)CFG_MEMSIZE_BYTES_Spans);

            // Dump memory segments

            printf("\n");

            printf("%-20s : @ %p %d bytes\n", "Address SRC_GRID",
                   memSrcGrid.data(), (int)memSrcGrid.size());

            printf("%-20s : @ %p %d bytes\n", "Address DST_GRID",
                   memDstGrid.data(), (int)memDstGrid.size());

            printf("%-20s : @ %p %d bytes\n", "Address Objects",
                   memObjects.data(), (int)memObjects.size());

            printf("%-20s : @ %p %d bytes\n", "Address Spans",
                   memSpans.data(), (int)memSpans.size());

            printf("\n");
            printf(">>> VHLIBOptimal Memory Layout Total: %d bytes\n", (int)CFG_MEMSIZE_BYTES_Total);
            printf("\n");
        }

        /**
         * Memory Layout Setup
         */
        verr SetupMemory(
            const VHMemRegion &regGridSrc,
            const VHMemRegion &regGridDst,
            const VHMemRegion &regObjects,
            const VHMemRegion &regSpans)
        {
            // Is all regions allocated ?
            bool valid = true;
            if (!regGridSrc.checkIsRegionValid())
                valid = false;
            if (!regGridDst.checkIsRegionValid())
                valid = false;
            if (!regObjects.checkIsRegionValid())
                valid = false;
            if (!regSpans.checkIsRegionValid())
                valid = false;
            if (!valid)
                return verrmsg(102, "MemoryLayout::SetupMemory() unallocated memory issue");

            // Check alignment
            size_t align = sizeof(uint32_t);
            bool align1 = regGridSrc.checkMemRegion(CFG_MEMSIZE_BYTES_PerGrid, align);
            bool align2 = regGridDst.checkMemRegion(CFG_MEMSIZE_BYTES_PerGrid, align);
            bool align3 = regObjects.checkMemRegion(CFG_MEMSIZE_BYTES_Objects, align);
            bool align4 = regSpans.checkMemRegion(CFG_MEMSIZE_BYTES_Spans, align);
            if(!(align1 && align2 && align3 && align4))
                return verrmsg(102, "MemoryLayout::SetupMemory() issue");

            // Check is crossed ?
            bool cross = false;

            if(regGridSrc.checkIsCrossed(regGridDst)) cross = true;
            if(regGridSrc.checkIsCrossed(regObjects)) cross = true;
            if(regGridSrc.checkIsCrossed(regSpans)) cross = true;

            if(regGridDst.checkIsCrossed(regObjects)) cross = true;
            if(regGridDst.checkIsCrossed(regSpans)) cross = true;

            if(regObjects.checkIsCrossed(regSpans)) cross = true;

            if(cross)
                return verrmsg(103, "MemoryLayout::SetupMemory() crossed regions issue");

            // accept and store configuration
            memSrcGrid = regGridSrc;
            memDstGrid = regGridDst;
            memObjects = regObjects;
            memSpans   = regSpans;

            return vok;
        }

        uint8_t *BitFieldSrcPtr()
        {
            return memSrcGrid.data();
        }

        size_t BitFieldSrcSize()
        {
            return memSrcGrid.size();
        }

        uint8_t *BitFieldDstPtr()
        {
            return memDstGrid.data();
        }

        size_t BitFieldDstSize()
        {
            return memDstGrid.size();
        }

        // Безопасный доступ через reinterpret_cast
        VHOptimalFigure &Obj(size_t pos)
        {
            return *(reinterpret_cast<VHOptimalFigure *>(memObjects.data()) + pos);
        }

        const VHOptimalFigure &Obj(size_t pos) const noexcept
        {
            return *(reinterpret_cast<const VHOptimalFigure *>(memObjects.data()) + pos);
        }

        // Возвращаем по значению без лишнего const
        const spanword Spn(size_t pos) const
        {
            return *(reinterpret_cast<const spanword *>(memSpans.data()) + pos);
        }

        spanword *GlobalSpans() const
        {
            return reinterpret_cast<spanword *>(memSpans.data());
        }

        // Запись элемента массива
        void SetSpn(spanword spn, size_t pos)
        {
            *(reinterpret_cast<spanword *>(memSpans.data()) + pos) = spn;
        }

    private:
        VHMemRegion memSrcGrid;
        VHMemRegion memDstGrid;
        VHMemRegion memObjects;
        VHMemRegion memSpans;
    };

}; // namespace vhliboptimal
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/mem/memorylayout.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 6432
 * Date / Time      : 16-08-2026 07:08:10
 * MD5              : 20c940ba1d9c3a3472c23a012569f2f1
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */