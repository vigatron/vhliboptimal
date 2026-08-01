#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <climits>

namespace vhliboptimal {

/**
 * @brief Speed-optimized routines
 */
class VHBits {
public:

    /**
     * @brief Index to [byteidx,bitidx] conversion
     */
    constexpr static std::pair<size_t, uint8_t> splidx(size_t v) noexcept {
        return { v / CHAR_BIT, static_cast<uint8_t>((CHAR_BIT-1) - (v & (CHAR_BIT-1))) };
    }

    /**
     * @brief Bitnumber to mask conversion
     */
    constexpr static uint8_t mask(uint8_t bitn) noexcept { 
        return 1 << bitn;
    }

    static void BitClr(uint8_t *ptr, int n) {
        auto [byteIdx, bitIdx] = splidx(n);
        ptr[byteIdx] &= ~ mask(bitIdx);
    }

    static void BitSet(uint8_t *ptr, int n) {
        auto [byteIdx, bitIdx] = splidx(n);
        ptr[byteIdx] |= mask(bitIdx);
    }

    static bool BitVal(const uint8_t *ptr, int n) {
        auto [byteIdx, bitIdx] = splidx(n);
        return (ptr[byteIdx] & mask(bitIdx)) != 0;
    }
};

};
