#pragma once

// Optimized for speed:
// * STM32F4/F7/H7
// * ESP32

#include <cstddef>
#include <cstdint>
#include <climits>


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
        constexpr size_t CellCornerTopLeft() const noexcept {
            return CellsX() + 1; }

        //! Конечный индекс поиска
        constexpr size_t CellCornerBottomRight() const noexcept {
            return CellsT() - CellsX() - 2; }

    private:

        static constexpr size_t fixedw = (1 << VHLIB_OPTIMAL_GRID_LX); 
        static constexpr size_t fixedh = (1 << VHLIB_OPTIMAL_GRID_LY);
        static constexpr size_t fixedt = fixedw * fixedh;
        static constexpr size_t fixedm = fixedw-1;

};

};
