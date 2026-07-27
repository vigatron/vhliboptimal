/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.4-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bitfield/bitfield.cpp
 * Content size  : 6190
 * Date / Time   : 27-07-2026 18:19:01
 * MD5           : 4e45eda4765f4e5dbd88fc0b54ce3578
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
bool BitField::Setup(const CellsMatrix & cmtx, uint8_t * arrptr, uint32_t limbytes) {

    // Fast Check
    if(arrptr == nullptr || limbytes < cmtx.BitMaskSizeBytes()) return false;

    arrPtr = arrptr;
    arrSizeInBytes = limbytes;

    ResetSearchIndex(cmtx);

    return true;
}

/**
 * 
 */
void BitField::ClrCell(int celln) {
    VHBits::BitClr(arrPtr, celln);
}

/**
 * 
 */
void BitField::SetCell(int celln) {
    VHBits::BitSet(arrPtr, celln);
}

/**
 * 
 */
bool BitField::GetCell(int celln) const {
    return VHBits::BitVal(arrPtr, celln);
}

/**
 *
 */
void BitField::ClrCell(const CellsMatrix & cmtx, int cellx, int celly) {
    int n = cmtx.CellN(cellx, celly);
    VHBits::BitClr(arrPtr, n);
}

/**
 *
 */
void BitField::SetCell(const CellsMatrix & cmtx, int cellx, int celly) {
    int n = cmtx.CellN(cellx, celly);
    VHBits::BitSet(arrPtr, n);
}

/**
 * 
 */
bool BitField::GetCell(const CellsMatrix & cmtx, int cellx, int celly) const {
    int n = cmtx.CellN(cellx, celly);
    return GetCell(n);
}

/**
 * @brief Find non-empty cell of the map
 */
const int BitField::FindEntryCell(const CellsMatrix & cmtx) {

    int r = -1;

    // Fast Entry point
    int idxstart = FastIdxNonZero();
    if(idxstart == -1) return r;

    for(int i=idxstart; i < cmtx.CellsT(); i++) {
        if(VHBits::BitVal(arrPtr, i)) {
            return i;
        }
    }

    return r;
}

/**
 * @brief Поиск соседних ячеек
 */
const int BitField::FindNearest(const CellsMatrix & cmtx, int n) const {

    int wx = cmtx.CellsX();
    int wy = cmtx.CellsY();
    int tryn;

    { tryn = n + 1;         if(GetCell(tryn)) return tryn; } // 6
    { tryn = n + wx - 1;    if(GetCell(tryn)) return tryn; } // 7
    { tryn = n + wx;        if(GetCell(tryn)) return tryn; } // 8
    { tryn = n + wx + 1;    if(GetCell(tryn)) return tryn; } // 9
    { tryn = n - wx - 1;    if(GetCell(tryn)) return tryn; } // 1
    { tryn = n - wx;        if(GetCell(tryn)) return tryn; } // 2
    { tryn = n - wx + 1;    if(GetCell(tryn)) return tryn; } // 3
    { tryn = n - 1;         if(GetCell(tryn)) return tryn; } // 4

    return -1;
}

/**
 * @brief Проход по фигуре fldfig : Поиск ответвлений
 */
const int BitField::FindPath(const CellsMatrix & cmtx, BitField & fldfig) {

    // Fast Entry point
    int idxstart = fldfig.FastIdxNonZero();
    if(idxstart == -1) return -1;

    for(int i=idxstart;i<cmtx.CellsT();i++) {
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

#if !defined(__x86_64__)
#define VHLIB_OPTIMAL_MODE_32
#else
#define VHLIB_OPTIMAL_MODE_64
#endif


#if defined(VHLIB_OPTIMAL_MODE_32)

/**
 * 
 */
void BitField::ResetSearchIndex(const CellsMatrix & cmtx) {
    curSearchWord    = cmtx.CellInnerFrom() / 32;
    lastSearchsByte  = cmtx.CellInnerTo()   / 8;
}


/**
 * @brief Optimization: fast search entry index
 */
int BitField::FastIdxNonZero() {

    const uint32_t* p32 = reinterpret_cast<const uint32_t*>(arrPtr);
    const size_t numWords = lastSearchsByte / sizeof(uint32_t);

    // Основной цикл — по 32-битным словам
    for (size_t i = curSearchWord; i < numWords; ++i) {
        uint32_t word = p32[i];
        if (word != 0) {
            curSearchWord = i;
            // позиция первого установленного бита
            int bitPos = __builtin_ctz(word);
            size_t byteIndex = i * sizeof(uint32_t) + (bitPos / CHAR_BIT);
            return static_cast<int>(byteIndex * CHAR_BIT);
        }
    }

    // Хвост (если размер массива не кратен 8)
    const size_t processedBytes = numWords * sizeof(uint32_t);
    for (size_t i = processedBytes; i < lastSearchsByte; ++i) {
        if (arrPtr[i] != 0) {
            return static_cast<int>(i * CHAR_BIT);
        }
    }

    return -1;
}

#endif


#if defined(VHLIB_OPTIMAL_MODE_64)

/**
 * 
 */
void BitField::ResetSearchIndex(const CellsMatrix & cmtx) {
    curSearchWord    = cmtx.CellInnerFrom() / 64;
    lastSearchsByte  = cmtx.CellInnerTo()   /  8;
}

/**
 * @brief Optimization: fast search entry index
 */
int BitField::FastIdxNonZero() {
    const uint64_t* p64 = reinterpret_cast<const uint64_t*>(arrPtr);
    const size_t numWords = lastSearchsByte / sizeof(uint64_t);

    // Основной цикл — по 64-битным словам
    for (size_t i = curSearchWord; i < numWords; ++i) {
        uint64_t word = p64[i];
        if (word != 0ULL) {
            curSearchWord = i;
            // первый установленный бит в слове
            int bitPos = __builtin_ctzll(word);
            size_t byteIndex = i * sizeof(uint64_t) + (bitPos / CHAR_BIT);
            return static_cast<int>(byteIndex * CHAR_BIT);
        }
    }

    // Хвост (если размер массива не кратен 8)
    const size_t processedBytes = numWords * sizeof(uint64_t);
    for (size_t i = processedBytes; i < lastSearchsByte; ++i) {
        if (arrPtr[i] != 0) {
            return static_cast<int>(i * CHAR_BIT);
        }
    }

    return -1;
}

#endif


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/bitfield/bitfield.cpp
 * Revision         : 0.7.4-beta
 * Content size     : 6190
 * Date / Time      : 27-07-2026 18:19:01
 * MD5              : 4e45eda4765f4e5dbd88fc0b54ce3578
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */