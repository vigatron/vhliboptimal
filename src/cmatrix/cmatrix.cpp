/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.5-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/cmatrix.cpp
 * Content size  : 1364
 * Date / Time   : 27-07-2026 18:49:23
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
void CellsMatrix::Setup(uint16_t imgpxlsw, uint16_t imgpxlsh, uint16_t csize) {

    cellsx      = (imgpxlsw / csize) + ((imgpxlsw % csize) ? 1:0);   // cells H
    cellsy      = (imgpxlsh / csize) + ((imgpxlsh % csize) ? 1:0);   // cells V
    cellst      = cellsx * cellsy;
    cellsz      = csize;

    cellin      = cellsx + 1;
    cellout     = cellst - cellsx - 2;
}

/**
 * 
 */
const size_t CellsMatrix::CellsX      () const {
    return cellsx;
}

/**
 * 
 */
const size_t CellsMatrix::CellsY      () const {
    return cellsy;
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
const size_t CellsMatrix::CellSize    () const {
    return cellsz;
}

/**
 * 
 */
const size_t CellsMatrix::CellN(size_t x, size_t y) const {
    return y * cellsx + x;
}

/**
 * 
 */
const std::pair<size_t, size_t> CellsMatrix::CellXY(size_t n) const {
    return { static_cast<size_t>(n % cellsx), static_cast<size_t>(n / cellsx) };
}

/**
 * 
 */
const size_t CellsMatrix::BitMaskSizeBytes() const {
    size_t arrsz = (cellst / CHAR_BIT) + ((cellst % CHAR_BIT) ? 1:0);
    return arrsz;
}

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
 * Revision         : 0.7.5-beta
 * Content size     : 1364
 * Date / Time      : 27-07-2026 18:49:23
 * MD5              : 3b75304cdd3d35808349baf397b13755
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */