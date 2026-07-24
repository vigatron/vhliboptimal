/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/bitfield/bitfield.cpp
 * Content size  : 8017
 * Date / Time   : 24-07-2026 12:39:50
 * MD5           : bfe86f9d66dc121b2384d37c2333d092
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
const int BitField::FindEntryCell(const CellsMatrix & cmtx) const {

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

#if VHLIB_OPTIMAL_FASTIDX == 1
/**
 *  @brief Optimization: fast search entry index
 */
int BitField::FastIdxNonZero() const {

    for(size_t i = 0; i < arrSizeInBytes; i++)
        if(arrPtr[i] != 0) return i*CHAR_BIT;
    
    return -1;
}
#endif


#if VHLIB_OPTIMAL_FASTIDX == 2

/**
 * @brief Optimization: fast search entry index
 */
int BitField::FastIdxNonZero() const {

    const uint32_t* p32 = reinterpret_cast<const uint32_t*>(arrPtr);
    const size_t numWords = arrSizeInBytes / sizeof(uint32_t);

    for (size_t i = 0; i < numWords; ++i) {
        uint32_t word = p32[i];
        if (word != 0) {
            // позиция первого установленного бита
            int bitPos = __builtin_ctz(word);
            size_t byteIndex = i * sizeof(uint32_t) + (bitPos / CHAR_BIT);
            return static_cast<int>(byteIndex * CHAR_BIT);
        }
    }

    // Хвост
    const size_t processed = numWords * sizeof(uint32_t);
    for (size_t i = processed; i < arrSizeInBytes; ++i) {
        if (arrPtr[i] != 0) {
            return static_cast<int>(i * CHAR_BIT);
        }
    }

    return -1;
}

#endif

#if VHLIB_OPTIMAL_FASTIDX == 3

/**
 * @brief Optimization: fast search entry index
 */
int BitField::FastIdxNonZero() const {
    const uint64_t* p64 = reinterpret_cast<const uint64_t*>(arrPtr);
    const size_t numWords = arrSizeInBytes / sizeof(uint64_t);

    // Основной цикл — по 64-битным словам
    for (size_t i = 0; i < numWords; ++i) {
        uint64_t word = p64[i];
        if (word != 0ULL) {
            // первый установленный бит в слове
            int bitPos = __builtin_ctzll(word);
            size_t byteIndex = i * sizeof(uint64_t) + (bitPos / CHAR_BIT);
            return static_cast<int>(byteIndex * CHAR_BIT);
        }
    }

    // Хвост (если размер массива не кратен 8)
    const size_t processedBytes = numWords * sizeof(uint64_t);
    for (size_t i = processedBytes; i < arrSizeInBytes; ++i) {
        if (arrPtr[i] != 0) {
            return static_cast<int>(i * CHAR_BIT);
        }
    }

    return -1;
}

#endif


#if VHLIB_OPTIMAL_FASTIDX == 4

#include <immintrin.h>

/**
 * @brief Optimization: fast search entry index
 */
int BitField::FastIdxNonZero() const {
    const uint8_t* p = arrPtr;
    size_t n = arrSizeInBytes;

    // Для очень маленьких массивов — сразу обычный цикл
    if (n < 64) {
        for (size_t i = 0; i < n; ++i) {
            if (p[i] != 0) return static_cast<int>(i * CHAR_BIT);
        }
        return -1;
    }

    size_t i = 0;

    // AVX2 основной цикл (32 байта за раз)
    for (; i + 32 <= n; i += 32) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(p + i));
        __m256i zero = _mm256_setzero_si256();
        int mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, zero));

        if (mask != 0xFFFFFFFF) {
            int bytePos = _tzcnt_u32(~static_cast<uint32_t>(mask));
            return static_cast<int>((i + bytePos) * CHAR_BIT);
        }
    }

    // Остаток (можно добавить 64-битный цикл для хвоста)
    const size_t remaining = n - i;
    if (remaining >= 8) {
        const uint64_t* p64 = reinterpret_cast<const uint64_t*>(p + i);
        size_t num64 = remaining / 8;
        for (size_t j = 0; j < num64; ++j) {
            uint64_t word = p64[j];
            if (word != 0) {
                int bitPos = __builtin_ctzll(word);
                size_t byteIndex = i + j * 8 + (bitPos / CHAR_BIT);
                return static_cast<int>(byteIndex * CHAR_BIT);
            }
        }
    }

    // Финальный байтовый хвост
    for (; i < n; ++i) {
        if (p[i] != 0) return static_cast<int>(i * CHAR_BIT);
    }

    return -1;
}
#endif


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/bitfield/bitfield.cpp
 * Revision         : 0.7.0-beta
 * Content size     : 8017
 * Date / Time      : 24-07-2026 12:39:50
 * MD5              : bfe86f9d66dc121b2384d37c2333d092
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */