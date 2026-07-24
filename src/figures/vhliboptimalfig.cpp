/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figures/vhliboptimalfig.cpp
 * Content size  : 10127
 * Date / Time   : 24-07-2026 12:39:50
 * MD5           : d71b93598fa82d086b43551eb9abb832
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "vhliboptimalfig.hpp"
#include "vhliboptimallog.hpp"

using namespace vhliboptimal;

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

    // Сканируем участоки фигуры
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

        stspan span = { .n = (uint32_t)curn, .l = (uint32_t)spanlen };

        // добавляем участок в список текущей фигуры
        arrspans.push_back(span);

        // Удаляем участок из поля
        bfld.ClearSpan(span);
    }

}

/**
 * @brief Размер фигуры в ячейках
*/
const strect & VHOptimalFigure::PosCells() const {
    return objrect;
}

/**
 * @brief Размер фигуры в пикселях
*/
const strect VHOptimalFigure::PosAbs(const CellsMatrix & cmtx) const {
    strect r;
    int cs = cmtx.CellSize();
    r.x1 = objrect.x1 * cs;
    r.y1 = objrect.y1 * cs;
    r.x2 = objrect.x2 * cs;
    r.y2 = objrect.y2 * cs;
    return r;
}

/**
 * @brief Все участки фигуры
 */
const std::vector<stspan> & VHOptimalFigure::Spans() const {
    return arrspans;
}

/**
 * @brief Количество участков фигуры
*/
const size_t VHOptimalFigure::SpansCount() const {
    return arrspans.size();
}

/**
 * @brief выбраный участок как объект
 */
const stspan & VHOptimalFigure::Span(int spann) const {
    return arrspans[spann];
}

/**
 * @brief Участок # spann фигуры в пикселях
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
 * @brief Оптимизация участков фигуры
 */
void VHOptimalFigure::Sort(const CellsMatrix & cmtx) {
    
    SortSequental(cmtx);

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
 * @brief   Поиск ближайшего участка по стартовой координате и минимальной дистанции
 *          Учитываем только участка с ненуловей длинной
 */
int VHOptimalFigure::FindClosestSpan(
    const stspan & span,
    const std::vector<stspan> & arr,
    const CellsMatrix & cmtx ) const
{

    int d = -1;
    int n = -1;

    for(int i=0; i < arr.size();i++) {

        // Valid ?
        if(arr[i].l) {

            // distance ^ 2
            int dcur = QDistance(span, arr[i], cmtx);

            // Not same ?
            if(dcur) {

                if(d==-1) {
                    d = dcur;
                    n = i;
                }
                else if(dcur<d) {
                    d = dcur;
                    n = i;
                }
            }
        }
    }

    return n;
}


/**
 * @brief Для каждой фигуры - сортировка линий
 */
void VHOptimalFigure::SortSequental(const CellsMatrix & cmtx) {

    std::vector<stspan> arrAvail = arrspans;

    std::vector<stspan> arrSorted;
    arrSorted.reserve(arrspans.size());

    int idx = 0;

    while(idx != -1) {
        arrSorted.push_back(arrAvail[idx]);
        arrAvail[idx].l = 0;
        const stspan & span = arrAvail[idx];
        idx = FindClosestSpan(span, arrAvail, cmtx);
    }

    // Replace with sorted
    arrspans = arrSorted;

}

/**
 * @brief Найти ячейку слева либо справа ( sideFlag 0-Left/1-Right )
 */
const int VHOptimalFigure::FindPosLRByY(const CellsMatrix & cmtx, uint16_t spancy, int sideFlag) const {

    int r = -1;
    uint16_t cellx;

    for(int i=0; i < arrspans.size();i++) {

        auto [ cx, cy ] = cmtx.CellXY(arrspans[i].n);
        if(cy != spancy) continue;

        if(r == -1) {
            r = i;
            cellx = cx;
        } else {
            bool cond = !sideFlag ? (cx < cellx) : (cx > cellx);
            r = cond ? i : r;
            cellx = cond ? cx : cellx;
        }
    }

    // Not found
    if(r == -1) return r;

    // Return Left or Right celln
    int cellLeft  = arrspans[r].n;
    int cellRight = arrspans[r].n + arrspans[r].l - 1;

    return !sideFlag ? cellLeft : cellRight;
}

/**
 * @brief Найти ячейку сверху либо снизу ( sideFlag 0-Up / 1-Down )
 * 
 * @param spancx        Позиция ячейки по x
 * @param sideFlag      0: Поиск верхней координаты 1: Поиск нижней координаты
 */
const int VHOptimalFigure::FindPosUDByX(const CellsMatrix & cmtx, uint16_t spancx, int sideFlag ) const {

    int r = -1;
    uint16_t posy;

    for(int i=0; i < arrspans.size();i++) {

        // Проверяем spancx на принадлежность
        auto [cfx, cfy]     = cmtx.CellXY(arrspans[i].n);
        auto [ctx, cty]     = cmtx.CellXY(arrspans[i].n + arrspans[i].l - 1);

        // spancx вне диапазона участка ?
        bool inside = spancx >= cfx && spancx <= ctx;
        if(!inside) continue;

        // Первый обнаруженый участок ?
        if(r == -1) {
            r = i;
            posy = !sideFlag ? cfy : cty;
            continue;
        }

        if(!sideFlag) {
            // Поиск верхней координаты ячейки
            if(cfy < posy) { posy = cfy; r = i; }
        } else {
            // Поиск нижней координаты ячейки
            if(cty > posy) { posy = cty; r = i; }
        }

    }

    // Not found
    if(r == -1) return r;

    // Return Top ot Bottom celln
    return cmtx.CellN( spancx, posy);
}


/**
 * @brief Обход по контуру
 */
void VHOptimalFigure::Border(const CellsMatrix & cmtx, CallbackBorder callbackBorder) const {

    // Empty, exit ...
    if(!arrspans.size()) return;

    if(callbackBorder == nullptr) return;

    uint16_t rows = objrect.y2 - objrect.y1 + 1;
    int cs = cmtx.CellSize();

    // Direction DOWN
    for(int i=0; i < rows; i++) {
        int n = FindPosLRByY(cmtx, objrect.y1 + i, 0);
        if(n != -1 ) {
            auto [cx, cy] = cmtx.CellXY(n);
            uint16_t pxlx = cx * cs;
            uint16_t pxly = cy * cs; // + (cs >> 1);
            uint8_t cmd = !i ? cmdStart : cmdMove;
            callbackBorder((void *)this, cmd, dirLeft, dirDown, cx, cy, pxlx, pxly);
        }
    }

    // Direction UP
    for(int i=0; i < rows; i++) {
        int n = FindPosLRByY(cmtx, objrect.y2 - i, 1);
        if(n != -1) {
            auto [cx, cy] = cmtx.CellXY(n);
            uint16_t pxlx = cx * cs + cs;
            uint16_t pxly = (cy + 1) * cs - 1; //  + (cs >> 1);
            uint8_t cmd = (i == rows - 1) ? cmdStop : cmdMove;
            callbackBorder((void *)this, cmd, dirRight, dirUp, cx, cy, pxlx, pxly);
        }
    }

}

/**
 * @brief габариты фигуры по горизонтали
 */
void VHOptimalFigure::ContentH(const CellsMatrix & cmtx, CallbackContent callbackContent) const {

    // Empty, exit ...
    if(!arrspans.size()) return;

    if(callbackContent == nullptr) return;

    uint16_t rows = objrect.y2 - objrect.y1 + 1;
    int cs = cmtx.CellSize();

    // Direction from Up to DOWN
    for(int i=0; i < rows; i++) {
        int nl = FindPosLRByY(cmtx, objrect.y1 + i, 0);
        if(nl != -1) {
            int nr = FindPosLRByY(cmtx, objrect.y1 + i, 1);
            callbackContent((void *)this, nl, nr == -1 ? nl:nr, 0);
        }
    }

}

/**
 * @brief габариты фигуры по вертикали
 */
void VHOptimalFigure::ContentV(const CellsMatrix & cmtx, CallbackContent callbackContent) const {

    // Empty, exit ...
    if(!arrspans.size()) return;

    if(callbackContent == nullptr) return;

    uint16_t cols = objrect.x2 - objrect.x1 + 1;
    int cs = cmtx.CellSize();

    // Direction from Left to Right
    for(int i=0; i < cols; i++) {
        int nu = FindPosUDByX(cmtx, objrect.x1 + i, 0);
        if(nu != -1) {
            int nd = FindPosUDByX(cmtx, objrect.x1 + i, 1);
            callbackContent((void *)this, nu, nd == -1 ? nu:nd, 1);
        }
    }

}


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/figures/vhliboptimalfig.cpp
 * Revision         : 0.7.0-beta
 * Content size     : 10127
 * Date / Time      : 24-07-2026 12:39:50
 * MD5              : d71b93598fa82d086b43551eb9abb832
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */