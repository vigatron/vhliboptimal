/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bitfield/bitfield.cpp
 * Content size  : 6260
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : cb968610d7ffd7af8428f12446bbd600
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "bitfield.hpp"

#include <cstdio>
#include <cstring>

#include "structs/vhliboptimalstructs.hpp"


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
void BitField::Clear(const CellsMatrix & cmtx) noexcept {
    std::memset(arrPtr, 0, arrSizeInBytes);
    ResetSearchIndex(cmtx);
}


/**
 * @brief Find non-empty cell of the map
 */
const int BitField::FindEntryCell(const CellsMatrix & cmtx) {

    int r = -1;

    // Fast Entry point
    int idxstart = FastIdxNonZero();
    if(idxstart == -1) return r;

    #ifdef VHLIB_OPTIMAL_LOG_LEVEL
    auto [dbgx, dbgy] = cmtx.CellXY(idxstart);
    #endif

    return idxstart;

    // for(int i=idxstart; i < cmtx.CellsT(); i++) {
    //     if(VHBits::BitVal(arrPtr, i)) {
    //         return i;
    //     }
    // }
    // return r;
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
void BitField::ClearSpan(const spanword word)  {
    int spanid = get_span_id(word);
    int spanln = get_span_len(word);
    int end = spanid + spanln;
    for(size_t i=spanid; i < end; i++) {
        ClrCell(i);
    }
}


#if defined(VHLIB_OPTIMAL_MODE_32)

/**
 * 
 */
void BitField::ResetSearchIndex(const CellsMatrix & cmtx) noexcept {
    curSearchWord    = cmtx.CellCornerTopLeft()     / 32;
    lastSearchsByte  = cmtx.CellCornerBottomRight() / 8;
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
            #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                word = __builtin_bswap32(word);
            #endif

            int bitPos = __builtin_clz(word);
            size_t byteIndex = i * sizeof(uint32_t) * CHAR_BIT + bitPos;
            return static_cast<int>(byteIndex);
        }
    }

    // // Хвост (если размер массива не кратен 8)
    // const size_t processedBytes = numWords * sizeof(uint32_t);
    // for (size_t i = processedBytes; i < lastSearchsByte; ++i) {
    //     if (arrPtr[i] != 0) {
    //         return static_cast<int>(i * CHAR_BIT);
    //     }
    // }

    return -1;
}

#endif


#if defined(VHLIB_OPTIMAL_MODE_64)

/**
 * 
 */
void BitField::ResetSearchIndex(const CellsMatrix & cmtx) noexcept {
    curSearchWord    = cmtx.CellCornerTopLeft()     / 64;
    lastSearchsByte  = cmtx.CellCornerBottomRight() /  8;
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
            #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            word = __builtin_bswap64(word);
            #endif

            int bitPos = __builtin_clzll(word);
            size_t byteIndex = i * sizeof(uint64_t) * CHAR_BIT + bitPos;
            return static_cast<int>(byteIndex);
        }
    }

    // // Хвост (если размер массива не кратен 8)
    // const size_t processedBytes = numWords * sizeof(uint64_t);
    // for (size_t i = processedBytes; i < lastSearchsByte; ++i) {
    //     if (arrPtr[i] != 0) {
    //         return static_cast<int>(i * CHAR_BIT);
    //     }
    // }

    return -1;
}

#endif


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/bitfield/bitfield.cpp
 * Revision         : 0.8.0-beta
 * Content size     : 6260
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : cb968610d7ffd7af8428f12446bbd600
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */