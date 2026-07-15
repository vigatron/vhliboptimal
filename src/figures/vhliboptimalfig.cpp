/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.1 Draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figures/vhliboptimalfig.cpp
 * Content size  : 5691
 * Date / Time   : 16-07-2026 02:43:20
 * MD5           : e109ac57c5edd4dcad9c118cd9688cb4
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "vhliboptimalfig.hpp"
#include "vhliboptimallog.hpp"

/**
 * @brief Сканирование битового поля и сохранение участков фигуры
 * 
 * @param bfld BitField
 * @param cmtx 2D Area config
 * @param skipcellsmax макс количество пустых ячеек подряд
 */
VHOptimalFigure::VHOptimalFigure(
    BitField & bfld,
    const CellsMatrix & cmtx,
    int skipcellsmax
) {

    bool loop = true;

    // Сканируем линии фигуры
    while(loop) {

        // начальная ячейка
        int curn = bfld.FindEntryCell(cmtx);
        if(curn == -1) {
            loop = false;
            break;
        }

        // Вычисляем длинну отрезка ( + пустые ячейки, до 3х )
        int spanlen = bfld.ScanSpanLen(cmtx, curn, skipcellsmax);

        // Корректируем габаритный размер фигуры: начальная точка X:Y
        auto [cur_x, cur_y] = cmtx.CellXY(curn);

        if(! arrspans.size()) {

            objrect.x1 = cur_x;
            objrect.y1 = cur_y;
            objrect.x2 = cur_x + spanlen - 1;
            objrect.y2 = cur_y;

        } else {

            if(cur_x < objrect.x1) objrect.x1 = cur_x;
            if(cur_y < objrect.y1) objrect.y1 = cur_y;

            if((cur_x + spanlen - 1) > objrect.x2) objrect.x2 = cur_x + spanlen - 1;
            if(cur_y > objrect.y2) objrect.y2 = cur_y;

        }

        // добавляем линию в список текущей фигуры
        stspan span = { .n = (uint16_t)curn, .l = (uint16_t)spanlen };
        arrspans.push_back(span);

        // Удаляем линию из поля
        bfld.ClearSpan(span);
    }

}

/**
 * @brief Количество прямоугольников фигуры
 * @return количество участков
*/
const size_t VHOptimalFigure::SpansCount() const {
    return arrspans.size();
}

/**
 *
 */
const stspan & VHOptimalFigure::Span(int spann) const {
    return arrspans[spann];
}

/**
 * @brief Прямоугольник n (фигуры fn) в пикселях
 * @return selected region rectangle area
 */
strect VHOptimalFigure::SpanRect(int spann, const CellsMatrix & cmtx) const {

    // Отрезки заданной фигуры
    const stspan & span = Span(spann);
    auto [cx, cy] = cmtx.CellXY(span.n);

    strect spanrect;

    // Верхний левый угол
    spanrect.x1  = cx * cmtx.CellSize();
    spanrect.y1  = cy * cmtx.CellSize();

    // Нижний правый угол
    spanrect.x2  = spanrect.x1 + span.l * cmtx.CellSize() - 1;
    spanrect.y2  = spanrect.y1 + cmtx.CellSize() - 1;

    return spanrect;
}

/**
 * @brief Размер фигуры
*/
const strect & VHOptimalFigure::Position() const {
    return objrect;
}

/**
 * @brief Квадрат расстояния между прямоугольниками ( по начальной точке )
*/
int VHOptimalFigure::QDistance(
    const stspan & p1,
    const stspan & p2,
    const CellsMatrix & cmtx) const
{
    auto [ xa, ya ] = cmtx.CellXY(p1.n);
    auto [ xb, yb ] = cmtx.CellXY(p2.n);
    int dx = xa - xb;
    int dy = ya - yb;
    int d = (dx*dx) + (dy*dy);
    return d;
}

/**
 * @brief Поиск прямоугольника по минимальной дистанции
 */
int VHOptimalFigure::min_n_by_dist(const stspan & src, int cnt) {

    int d = -1;
    int n = -1;

    return n;
}


/**
 * @brief Для каждой фигуры - сортировка линий
 */
void VHOptimalFigure::SortFigureParts() {

}

/**
 *
 */
uint8_t VHOptimalFigure::FindWaterfall(int atpos, int lim, int *poffs, int *pcnt) {

    return 1;
}

/**
 *
 */
void VHOptimalFigure::SwapWaterfall(int len) {

}

/**
 *
 */
void VHOptimalFigure::SortWaterfall() {
}


/**
 * @brief Найти крайний левый либо крайний правый ( sideFlag 0-Left/1-Right )
 */
const int VHOptimalFigure::FindSpanCellLR(const CellsMatrix & cmtx, uint16_t spancy, int sideFlag) const {

    int r = -1;
    uint16_t x;

    for(int i=0; i < arrspans.size();i++) {

        auto [ cx, cy ] = cmtx.CellXY(arrspans[i].n);
        if(cy != spancy) continue;

        if(r == -1) {
            r = i;
            x = cx;
        } else {
            bool cond = !sideFlag ? (cx<x) : (cx>x);
            r = cond ? i : r;
            x = cond ? cx : x;
        }
    }

    // Not found
    if(r == -1) return r;
    int cellLeft  = arrspans[r].n;
    int cellRight = arrspans[r].n + arrspans[r].l - 1;

    return !sideFlag ? cellLeft : cellRight;
}

/**
 * @brief Обход по контуру
 */
void VHOptimalFigure::Border(const CellsMatrix & cmtx, SetPosCallback callbackSetPos) {

    // Empty, exit ...
    if(!arrspans.size()) return;

    uint16_t height = objrect.y2 - objrect.y1 + 1;
    
    // Direction DOWN
    for(int i=0; i < height; i++) {
        int n = FindSpanCellLR(cmtx, objrect.y1 + i, 0);
        auto [cx, cy] = cmtx.CellXY(n);
        uint16_t x = cx * cmtx.CellSize();
        uint16_t y = cy * cmtx.CellSize() + (cmtx.CellSize() >> 1);
        callbackSetPos(x, y);
        VHLibOptimalLogger::DumpCell("Border-Down:", n, cx, cy, x, y);
    }

    // Direction UP
    for(int i=0; i < height; i++) {
        int n = FindSpanCellLR(cmtx, objrect.y2 - i, 1);
        auto [cx, cy] = cmtx.CellXY(n);
        uint16_t x = cx * cmtx.CellSize() + cmtx.CellSize();
        uint16_t y = cy * cmtx.CellSize() + ( cmtx.CellSize() >> 1);
        callbackSetPos(x, y);
        VHLibOptimalLogger::DumpCell("Border-Up:", n, cx, cy, x, y);
    }

}

/* ========================[  END FILE CONTENT  ]========================
 * File             : src/figures/vhliboptimalfig.cpp
 * Content size     : 5691
 * Date / Time      : 16-07-2026 02:43:20
 * MD5              : e109ac57c5edd4dcad9c118cd9688cb4
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */