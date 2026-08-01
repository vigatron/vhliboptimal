#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <climits>

// TODO: Scan Window
// size_t wndx1;
// size_t wndy1;
// size_t wndx2;
// size_t wndy2;

namespace vhliboptimal {

class CellsMatrix {

    public:

        // Легковесная структура для координат вместо тяжелого std::pair
        struct Point {
            uint16_t x;
            uint16_t y;
        };

        /**
         * 
         */
        CellsMatrix() { }

        /**
         * 
         */
        bool Setup(uint8_t levelx, uint8_t levely) {
            bool flageqx = (levelx == VHLIB_OPTIMAL_GRID_LX);
            bool flageqy = (levely == VHLIB_OPTIMAL_GRID_LY);

            if(! (flageqx && flageqy) )
                return false;

            // cellin  = CellsX() + 1;
            // cellout = CellsT() - CellsX() - 2;
            return true;
        }

        constexpr size_t CellsX() const noexcept { return fixedw; }
        constexpr size_t CellsY() const noexcept { return fixedh; }
        constexpr size_t CellsT() const noexcept { return fixedt; }

        //
        constexpr size_t CellN(size_t x, size_t y) const noexcept {
            return (y << VHLIB_OPTIMAL_GRID_LX) + x; }

        //
        constexpr Point CellXY(size_t n) const noexcept {
            return Point{
                static_cast<uint16_t>(n  & fixedm),
                static_cast<uint16_t>(n >> VHLIB_OPTIMAL_GRID_LX) };
            }

        // Подсчет размера в байтах
        constexpr size_t BitMaskSizeBytes() const noexcept {
            size_t arrsz = (fixedt / CHAR_BIT) + ((fixedt % CHAR_BIT) ? 1:0);
            return arrsz;
        }

        // Стартовый индекс поиска
        constexpr size_t CellInnerFrom() const noexcept {
            return CellsX() + 1; }

        //! Конечный индекс поиска
        constexpr size_t CellInnerTo() const noexcept {
            return CellsT() - CellsX() - 2; }

    private:

        // static constexpr size_t cellin;    // Within Border: Start cell
        // static constexpr size_t cellout;   // Within Border: Last  cell

        static constexpr size_t fixedw = (1 << VHLIB_OPTIMAL_GRID_LX); 
        static constexpr size_t fixedh = (1 << VHLIB_OPTIMAL_GRID_LY);
        static constexpr size_t fixedt = fixedw * fixedh;
        static constexpr size_t fixedm = fixedw-1;

};

};
