#pragma once

#include "structs/vhliboptimalstructs.hpp"

namespace vhliboptimal
{

    /**
     *  массив локальных участков в виде
     *  spanlen [31 .. 21] + spanid  [20 ..  0]
     */

    class VHLocalSpansArray
    {

    public:

        void Init(spanword *arr, uint32_t sidx)
        {
            arrlocal = arr;
            arrlocal += sidx;
            startIdx = sidx;
            curIdx = 0;
        }

        bool add(spanword word)
        {
            if (startIdx + curIdx >= VHOPTIMAL_SPANS_MAX)
                return false;
            arrlocal[curIdx++] = word;
            return true;
        }

        spanword get(uint32_t pos) const
        {
            return arrlocal[pos];
        }

        uint32_t globalstartidx() const
        {
            return startIdx;
        }

        uint32_t localspanscnt() const
        {
            return curIdx;
        }

    private:
        spanword *arrlocal;
        uint32_t startIdx;
        uint32_t curIdx;
    };

};
