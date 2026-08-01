#pragma once

#include "structs/vhliboptimalstructs.hpp"
#include "figures/figures.hpp"

namespace vhliboptimal {

//
class VHMemoryLayout {

    public:

        /**
         * Memory Layout Calculation
         */
        size_t CalcMemory() {

            bytesTotal = 0;

            int gridWidth           = 1 << VHOPTIMAL_GRID_X_LEVEL;
            int gridHeight          = 1 << VHOPTIMAL_GRID_Y_LEVEL;

            bytesPerGrid        = (gridWidth >> 3) * gridHeight;
            bytesTotal         += bytesPerGrid * 2;

            bytesObjects        = sizeof(VHOptimalFigure) * VHOPTIMAL_OBJECTS_MAX;
            bytesTotal         += bytesObjects;

            bytesSpans          = sizeof(spanword) * VHOPTIMAL_SPANS_MAX;
            bytesTotal         += bytesSpans;

            // Memory Layout Stat
            printf("\n=== VHLibOptimal::CalcMemory() === \n");
            printf("sizeof(VHOptimalFigure) = %d bytes\n",  (int)sizeof(VHOptimalFigure));
            printf("sizeof(spanword) = %d bytes\n",         (int)sizeof(spanword));
            printf("Grid size: %d x %d\n",  gridWidth, gridHeight);
            printf("x1 grid  = %d bytes\n", (int)bytesPerGrid);
            printf("x2 grids = %d bytes\n", (int)bytesPerGrid * 2);

            printf("%d objects x %d bytes = %d bytes\n",
                (int)sizeof(VHOptimalFigure),
                (int)VHOPTIMAL_OBJECTS_MAX,
                (int)bytesObjects );

            printf("%d spans x %d bytes = %d bytes\n",
                (int)sizeof(spanword),
                (int)VHOPTIMAL_SPANS_MAX,
                (int)bytesSpans );

            printf(">>> VHLIBOptimal Memory Layout Total: %d bytes\n", (int)bytesTotal);
            printf("\n");

            return bytesTotal;
        }

        /**
         * Memory Layout Setup
         */
        verr SetupMemory(uint8_t * ptr, size_t memsize) {

            size_t offs = 0;

            _pMemBitFieldSrc        = ptr + offs; offs += bytesPerGrid;
            _pMemBitFieldDst        = ptr + offs; offs += bytesPerGrid;
            _pMemObjects            = ptr + offs; offs += bytesObjects;
            _pMemSpans              = ptr + offs; offs += bytesSpans;

            if(memsize != bytesTotal || memsize != offs)
                return verrmsg(1, "vhliboptimal::SetupMemory() : Invalid block size");

            return vok;
        }

        uint8_t *   BitFieldPtr     () { return _pMemBitFieldSrc; }
        size_t      BitFieldSize    () { return bytesPerGrid; }

    private:

        size_t          bytesPerGrid;
        size_t          bytesObjects;
        size_t          bytesSpans;
        size_t          bytesTotal;

        // Static Memory Segments
        uint8_t     *   _pMemBitFieldSrc;
        uint8_t     *   _pMemBitFieldDst;
        uint8_t     *   _pMemObjects;
        uint8_t     *   _pMemSpans;

};

}; // namespace vhliboptimal