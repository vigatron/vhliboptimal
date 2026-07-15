/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.1 Draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/bitfield.cpp
 * Content size  : 3391
 * Date / Time   : 16-07-2026 02:43:20
 * MD5           : 4f23832c3b3ce3c609ee933ca6d06f71
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "bitfield.hpp"

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
void BitField::Clr(size_t celln) {
    VHBits::BitClr(arrmem, celln);
}

/**
 * 
 */
void BitField::Set(size_t celln) {
    VHBits::BitSet(arrmem, celln);
}


/**
 * 
 */
bool BitField::Get(size_t celln) const {
    return VHBits::BitVal(arrmem, celln);
}

/**
 * @brief Find non-empty cell of the map
 */
const int BitField::FindEntryCell(const CellsMatrix & cmtx) const {
    int r = -1;
    for(uint16_t i=0; i < cmtx.CellsT(); i++) {
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
            if(Get(crn[i])) {
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
    for(uint16_t i=0;i<cmtx.CellsT();i++) {
        if(fldfig.Get(i)) {
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
size_t BitField::ScanSpanLen(const CellsMatrix & cmtx, size_t startcell, int skipmax) const {

    // вычисляем координаты ячейки по номеру
    auto [cx, cy] = cmtx.CellXY(startcell);

    // Последняя ячейка в линии
    uint16_t spc    = skipmax;
    uint16_t finn   = (cy+1) * cmtx.CellsX() - 1;
    uint16_t curn   = startcell;
    uint16_t validn = startcell;

    while(curn<=finn)
    {
        if(Get(curn)) {
            validn = curn;
            spc = skipmax;
        }
        else {
            spc--;
            if(!spc) break;
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
        Clr(span.n+i);
    }
}
/* ========================[  END FILE CONTENT  ]========================
 * File             : src/cmatrix/bitfield.cpp
 * Content size     : 3391
 * Date / Time      : 16-07-2026 02:43:20
 * MD5              : 4f23832c3b3ce3c609ee933ca6d06f71
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */