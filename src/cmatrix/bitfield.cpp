/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.4
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/bitfield.cpp
 * Content size  : 3717
 * Date / Time   : 20-07-2026 06:32:56
 * MD5           : aeed05d48da46a805abfb9c41263b483
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "bitfield.hpp"

using namespace vhliboptimal;

/**
 * 
 */
void BitField::Setup(const CellsMatrix & cmtx) {
    size_t arrsz = (cmtx.CellsT() / CHAR_BIT) + ((cmtx.CellsT() % CHAR_BIT) ? 1:0);
    arrmem = std::vector<uint8_t>(arrsz, 0);
}

/**
 * 
 */
const std::vector<uint8_t> & BitField::arr() const {
    return arrmem;
}

/**
 * 
 */
void BitField::ClrCell(int celln) {
    VHBits::BitClr(arrmem, celln);
}

/**
 *
 */
void BitField::ClrCell(const CellsMatrix & cmtx, int cellx, int celly) {
    int n = cmtx.CellN(cellx, celly);
    VHBits::BitClr(arrmem, n);
}

/**
 * 
 */
void BitField::SetCell(int celln) {
    VHBits::BitSet(arrmem, celln);
}

/**
 *
 */
void BitField::SetCell(const CellsMatrix & cmtx, int cellx, int celly) {
    int n = cmtx.CellN(cellx, celly);
    VHBits::BitSet(arrmem, n);
}

/**
 * 
 */
bool BitField::GetCell(int celln) const {
    return VHBits::BitVal(arrmem, celln);
}

/**
 * @brief Find non-empty cell of the map
 */
const int BitField::FindEntryCell(const CellsMatrix & cmtx) const {
    int r = -1;
    for(int i=0; i < cmtx.CellsT(); i++) {
        if(VHBits::BitVal(arrmem, i)) {
            return i;
        }
    }
    return r;
}

/**
 * @brief Поиск соседних ячеек
 */
const int BitField::FindNearest(const CellsMatrix & cmtx, int n) const {

    auto [lx, ly] = cmtx.CellXY(n);
    int wx = cmtx.CellsX();
    int wy = cmtx.CellsY();

    int crn[8] = {            // "corners"
            (n - wx - 1),     // #0 : 1
            (n - 1),          // #1 : 4
            (n + wx - 1),     // #2 : 7
            (n + wx),         // #3 : 8
            (n + wx + 1),     // #4 : 9
            (n + 1),          // #5 : 6
            (n - wx + 1),     // #6 : 3
            (n - wx)          // #7 : 2
     };

    // ----------------------------------------
    // Cut out-of range ...
    // ----------------------------------------
    if(!lx)               { crn[0] = -1; crn[1] = -1; crn[2] = -1; }
    if(lx == (wx-1))      { crn[4] = -1; crn[5] = -1; crn[6] = -1; }
    if(!ly)               { crn[0] = -1; crn[6] = -1; crn[7] = -1; }
    if(ly == (wy-1))      { crn[2] = -1; crn[3] = -1; crn[4] = -1; }

    // ----------------------------------------
    for(int i=0;i<8;i++) {

        // Check limits
        if((crn[i]>=0) && (crn[i]<cmtx.CellsT())) {
            if(GetCell(crn[i])) {
                return crn[i];
            }
        }
    }

    return -1;
}

/**
 * @brief Проход по фигуре fldfig : Поиск ответвлений
 */
const int BitField::FindPath(const CellsMatrix & cmtx, const BitField & fldfig) const {
    for(int i=0;i<cmtx.CellsT();i++) {
        if(fldfig.GetCell(i)) {
            int r = FindNearest(cmtx, i);
            if(r != -1) {
                return r;
            }
        }
    }
    return -1;
}

/**
 * Вычисление длинны учитывая пропуски ( SPACER )
 */
int BitField::ScanSpanLen(const CellsMatrix & cmtx, int startcell, int skipmax) const {

    // вычисляем координаты ячейки по номеру
    auto [cx, cy] = cmtx.CellXY(startcell);

    // Последняя ячейка в линии
    int spc    = skipmax;
    int finn   = (cy+1) * cmtx.CellsX() - 1;
    int curn   = startcell;
    int validn = startcell;

    while(curn<=finn)
    {
        if(GetCell(curn)) {
            validn = curn;
            spc = skipmax;
        }
        else {
            if(!spc) break;
            spc--;
        }
        curn++;
    }

    return validn - startcell + 1;
}

/**
 * 
 */
void BitField::ClearSpan(const stspan & span)  {
    for(size_t i=0; i < span.l; i++) {
        ClrCell(span.n+i);
    }
}

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/bitfield.cpp
 * Revision         : 0.4
 * Content size     : 3717
 * Date / Time      : 20-07-2026 06:32:56
 * MD5              : aeed05d48da46a805abfb9c41263b483
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */