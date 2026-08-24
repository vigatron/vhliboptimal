/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/mem/memregion.hpp
 * Content size  : 1971
 * Date / Time   : 24-08-2026 16:26:25
 * MD5           : 3dd992fa0c858e65361ee229ccc34292
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include <cstdint>

//
class VHMemRegion
{

public:
    VHMemRegion() : ptr(nullptr), sz(0) {}

    /**
     *
     */
    VHMemRegion(uint8_t *srcptr, size_t srcsize) noexcept : ptr(srcptr), sz(srcsize) {}

    /**
     *
     */
    uint8_t *data() noexcept { return ptr; }

    /**
     *
     */
    uint8_t *data() const noexcept { return ptr; }

    /**
     *
     */
    size_t size() const noexcept { return sz; }

    /**
     *
     */
    void init(uint8_t *srcptr, size_t srcsize) noexcept
    {
        ptr = srcptr;
        sz = srcsize;
    }

    /**
     *
     */
    VHMemRegion &operator=(const VHMemRegion &src) noexcept
    {
        if (this != &src)
        {
            ptr = src.ptr;
            sz = src.sz;
        }
        return *this;
    }

    /**
     *
     */
    bool checkIsRegionValid() const noexcept
    {
        return ptr != nullptr && sz != 0;
    }

    /**
     *
     */
    bool checkAlignment(uint32_t alignbytes) const noexcept
    {
        if (!checkIsRegionValid())
            return false;
        return (reinterpret_cast<std::uintptr_t>(ptr) & (alignbytes - 1)) == 0;
    }

    /**
     *
     */
    bool checkIsInside(const uint8_t *chk) const noexcept
    {
        if (!checkIsRegionValid())
            return false;
        return (ptr <= chk) && (chk < (ptr + sz));
    }

    /**
     *
     */
    bool checkMemRegion(size_t fixedsize, size_t align) const noexcept
    {
        if (!checkIsRegionValid())
            return false;
        if (!checkAlignment(align))
            return false;
        if (sz != fixedsize)
            return false;
        return true;
    }

    /**
     *
     */
    bool checkIsCrossed(const VHMemRegion &reg2) const noexcept
    {
        if(!reg2.checkIsRegionValid()) return false;
        bool f1 = ptr < (reg2.ptr + reg2.sz);
        bool f2 = reg2.ptr < (ptr + sz);
        return f1 && f2;
    }

private:
    uint8_t *ptr;
    size_t sz;
};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/mem/memregion.hpp
 * Revision         : 0.8.1
 * Content size     : 1971
 * Date / Time      : 24-08-2026 16:26:25
 * MD5              : 3dd992fa0c858e65361ee229ccc34292
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */