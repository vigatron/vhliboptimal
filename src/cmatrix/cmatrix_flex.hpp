/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-draft-2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/cmatrix/cmatrix_flex.hpp
 * Content size  : 2025
 * Date / Time   : 15-08-2026 02:03:05
 * MD5           : 01eb0c6575f90aba92a3a800991be764
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <climits>


namespace vhliboptimal {

class CellsMatrix {

    public:

        explicit CellsMatrix() { }

        bool Setup(uint8_t levelx, uint8_t levely) {
            levx        = levelx;
            levy        = levely;
            cellst      = CellsX() * CellsY();
            cellin      = CellsX() + 1;
            cellout     = cellst - CellsX() - 2;
            return true;
        }

        inline constexpr size_t CellsX () const noexcept { return 1 << levx; }
        inline constexpr size_t CellsY () const noexcept { return 1 << levy; }
        inline constexpr size_t CellsT () const noexcept { return cellst; }

        inline constexpr size_t CellN  (size_t x, size_t y) const noexcept {
            return y * CellsX() + x; }

        //
        inline constexpr std::pair<size_t, size_t> CellXY(size_t n) const noexcept {
            return {
                static_cast<size_t>(n % CellsX()),
                static_cast<size_t>(n / CellsX()) }; }

        // Подсчет размера в байтах
        inline constexpr size_t BitMaskSizeBytes() const noexcept {
            size_t arrsz = (cellst / CHAR_BIT) + ((cellst % CHAR_BIT) ? 1:0);
            return arrsz;
        }

        // Стартовый индекс поиска
        inline constexpr size_t CellInnerFrom  () const { return cellin; }

        //! Конечный индекс поиска
        inline constexpr size_t CellInnerTo    () const { return cellout; }

    private:

        // TODO: Scan Window
        // size_t wndx1;
        // size_t wndy1;
        // size_t wndx2;
        // size_t wndy2;

        uint8_t levx;           // Cells per H - Horizontal cells count
        uint8_t levy;           // Cells per V - Vertical   cells count
        size_t cellst;          // Cells total
        size_t cellin;          // Within Border: Start cell
        size_t cellout;         // Within Border: Last  cell

};

};
/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/cmatrix/cmatrix_flex.hpp
 * Revision         : 0.8.0-draft-2
 * Content size     : 2025
 * Date / Time      : 15-08-2026 02:03:05
 * MD5              : 01eb0c6575f90aba92a3a800991be764
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */