/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/cmatrix.cpp
 * Content size  : 1364
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 3b75304cdd3d35808349baf397b13755
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "cmatrix.hpp"

using namespace vhliboptimal;


/**
 * 
 */
CellsMatrix::CellsMatrix() {

}

#ifdef VHLIB_OPTIMAL_GRID_FIXED

/**
 * 
 */
bool CellsMatrix::Setup(uint8_t levelx, uint8_t levely) {
    bool flageqx = (levelx == VHLIB_OPTIMAL_GRID_LX);
    bool flageqy = (levely == VHLIB_OPTIMAL_GRID_LY);
    return flageqx && flageqy;
}

/**
 * 
 */
const size_t CellsMatrix::CellsX      () const {
    return fixedw;
}

/**
 * 
 */
const size_t CellsMatrix::CellsY      () const {
    return fixedh;
}

/**
 * 
 */
const size_t CellsMatrix::CellsT      () const {
    return fixedt;
}

/**
 * 
 */
const size_t CellsMatrix::CellN(size_t x, size_t y) const {
    return (y << VHLIB_OPTIMAL_GRID_LX) + x;
}

/**
 * 
 */
const std::pair<size_t, size_t> CellsMatrix::CellXY(size_t n) const {
    return {
        static_cast<size_t>(n  & fixedm),
        static_cast<size_t>(n >> VHLIB_OPTIMAL_GRID_LX)
    };
}

/**
 * 
 */
const size_t CellsMatrix::BitMaskSizeBytes() const {
    size_t arrsz = (fixedt / CHAR_BIT) + ((fixedt % CHAR_BIT) ? 1:0);
    return arrsz;
}


#else


/**
 * 
 */
bool CellsMatrix::Setup(uint8_t levelx, uint8_t levely) {

    levx        = levelx;
    levy        = levely;
    cellst      = CellsX() * CellsY();
    cellin      = CellsX() + 1;
    cellout     = cellst - CellsX() - 2;

    return true;
}

/**
 * 
 */
const size_t CellsMatrix::CellsX      () const {
    return 1 << levx;
}

/**
 * 
 */
const size_t CellsMatrix::CellsY      () const {
    return 1 << levy;
}

/**
 * 
 */
const size_t CellsMatrix::CellsT      () const {
    return cellst;
}

/**
 * 
 */
const size_t CellsMatrix::CellN(size_t x, size_t y) const {
    return y * CellsX() + x;
}

/**
 * 
 */
const std::pair<size_t, size_t> CellsMatrix::CellXY(size_t n) const {
    return {
        static_cast<size_t>(n % CellsX()),
        static_cast<size_t>(n / CellsX()) };
}

/**
 * 
 */
const size_t CellsMatrix::BitMaskSizeBytes() const {
    size_t arrsz = (cellst / CHAR_BIT) + ((cellst % CHAR_BIT) ? 1:0);
    return arrsz;
}


#endif



/**
 * 
 */
const size_t CellsMatrix::CellInnerFrom() const {
    return cellin;
}

/**
 * 
 */
const size_t CellsMatrix::CellInnerTo() const {
    return cellout;
}


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/cmatrix.cpp
 * Revision         : 0.8.0-beta
 * Content size     : 1364
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 3b75304cdd3d35808349baf397b13755
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */