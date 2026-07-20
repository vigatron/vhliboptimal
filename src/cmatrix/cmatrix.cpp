/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.3
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/cmatrix.cpp
 * Content size  : 970
 * Date / Time   : 20-07-2026 05:40:29
 * MD5           : 931f4c89861b3b3d8d8940cacbaff424
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

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/cmatrix.cpp
 * Revision         : 0.3
 * Content size     : 970
 * Date / Time      : 20-07-2026 05:40:29
 * MD5              : 931f4c89861b3b3d8d8940cacbaff424
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */