#pragma once

#include "structs/vhliboptimalstructs.hpp"
#include "figure/figure.hpp"

namespace vhliboptimal {

//
class VHMemoryLayout {

    public:

        static_assert(sizeof(VHOptimalFigure)   == 16, "16 bytes VHOptimalFigure"   );
        static_assert(sizeof(spanword)          ==  4, " 4 bytes for spanword"      );

        static_assert(VHOPTIMAL_GRID_X_LEVEL > 2);
        static_assert(VHOPTIMAL_GRID_X_LEVEL < 13);

        static_assert(VHOPTIMAL_GRID_Y_LEVEL > 2);
        static_assert(VHOPTIMAL_GRID_Y_LEVEL < 13);

        /**
         * Memory Layout Calculation
         */
        size_t CalcMemory() {

            bytesTotal = 0;

            int gridWidth       = 1 << VHOPTIMAL_GRID_X_LEVEL;
            int gridHeight      = 1 << VHOPTIMAL_GRID_Y_LEVEL;

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

            bool flagalign = (reinterpret_cast<std::uintptr_t>(ptr) & 3) == 0;
            if(!flagalign)
                return verrmsg(100, "MemoryLayout::SetupMemory() alignment issue");

            size_t offs = 0;

            _pMemBitFieldSrc        = ptr + offs; offs += bytesPerGrid;
            _pMemBitFieldDst        = ptr + offs; offs += bytesPerGrid;
            _pMemObjects            = ptr + offs; offs += bytesObjects;
            _pMemSpans              = ptr + offs; offs += bytesSpans;

            if(memsize != bytesTotal || memsize != offs)
                return verrmsg(1, "vhliboptimal::SetupMemory() : Invalid block size");

            return vok;
        }

        uint8_t *   BitFieldSrcPtr     () { return _pMemBitFieldSrc; }
        size_t      BitFieldSrcSize    () { return bytesPerGrid; }

        uint8_t *   BitFieldDstPtr     () { return _pMemBitFieldDst; }
        size_t      BitFieldDstSize    () { return bytesPerGrid; }

        // Безопасный доступ через reinterpret_cast
        VHOptimalFigure& Obj(size_t pos) { 
            return *(reinterpret_cast<VHOptimalFigure*>(_pMemObjects) + pos); 
        }

        const VHOptimalFigure & Obj(size_t pos) const noexcept { 
            return *(reinterpret_cast<VHOptimalFigure*>(_pMemObjects) + pos); 
        }

        // Возвращаем по значению без лишнего const
        spanword Spn(size_t pos) const { 
            return *(reinterpret_cast<spanword*>(_pMemSpans) + pos); 
        }

        spanword * GlobalSpans() const {
            return reinterpret_cast<spanword*>(_pMemSpans);
        }

        // Запись элемента массива
        void SetSpn(spanword spn, size_t pos) { 
            *(reinterpret_cast<spanword*>(_pMemSpans) + pos) = spn; 
        }

    private:

        size_t bytesPerGrid;
        size_t bytesObjects;
        size_t bytesSpans;
        size_t bytesTotal;

        // Static Memory Segments
        uint8_t * _pMemBitFieldSrc;
        uint8_t * _pMemBitFieldDst;
        uint8_t * _pMemObjects;
        uint8_t * _pMemSpans;

};

}; // namespace vhliboptimal