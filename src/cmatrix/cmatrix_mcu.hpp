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
