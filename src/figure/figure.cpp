/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figures/vhliboptimalfig.cpp
 * Content size  : 11975
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 46ff575121a2f8e496578f8eb6d245a7
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "figure.hpp"
#include "log/log.hpp"

using namespace vhliboptimal;

/**
 * 
 */
VHOptimalFigure::VHOptimalFigure() { }

/**
 * @brief Сканирование битового поля и сохранение участков фигуры
 * 
 * @param bfld BitField
 * @param cmtx 2D Area config
 * @param skipcellsmax макс количество пустых ячеек подряд
 */
verr VHOptimalFigure::Scan(
    BitField & bfld,
    const CellsMatrix & cmtx,
    int skipcellsmax,
    VHLocalSpansArray & arrspans
) {

    bool loop   = true;
    _startIDX   = arrspans.globalstartidx();
    _spansCount = 0;

    // Сканируем участоки фигуры
    while(loop) {

        // начальная ячейка
        int curn = bfld.FindEntryCell(cmtx);
        if(curn == -1) { loop = false; break; }

        #ifdef VHLIB_OPTIMAL_DEBUG
        auto [dbgx, dbgy] = cmtx.CellXY(curn);
        #endif

        // Вычисляем длинну отрезка ( + пустые ячейки, до 3х )
        int spanid = (uint32_t)curn;
        int spanww = bfld.ScanSpanLen(cmtx, curn, skipcellsmax);
        spanword pckword = pack_span(spanid, spanww);

        // добавляем участок в список текущей фигуры
        if(arrspans.add(pckword)) {
            _spansCount++;
        } else {
            return verrmsg(1, "VHOptimalFigure::Scan() spans limit reached");
        }

        // Удаляем участок из поля
        bfld.ClearSpan(pckword);
    }

    return vok;
}

/**
 * @brief Определение координат и размеров фигуры
 */
void VHOptimalFigure::CalcPosAndSize(
    const CellsMatrix & cmtx,
    VHLocalSpansArray & arrspans) {

    // Empty
    if(!_spansCount) return;

    // Initial values from first span
    const spanword spanw = arrspans.get(0);
    const int spanid = get_span_id(spanw);
    const int spanln = get_span_len(spanw);
    auto [cxi1,cyi1] = cmtx.CellXY(spanid);
    auto [cxi2,cyi2] = cmtx.CellXY(spanid + spanln - 1);

    int cxl = cxi1, cxr = cxi2, cyt = cyi1, cyd = cyi2;

    for(uint32_t i = 1; i < _spansCount; i++) {

        const spanword spanw = arrspans.get(i);
        const int spanid = get_span_id(spanw);
        const int spanln = get_span_len(spanw);

        auto [cx1, cy1] = cmtx.CellXY(spanid);
        auto [cx2, cy2] = cmtx.CellXY(spanid + spanln - 1);

        if(cxl > cx1) { cxl = cx1; }
        if(cyt > cy1) { cyt = cy1; }
        if(cxr < cx2) { cxr = cx2; }
        if(cyd < cy2) { cyd = cy2; }
    }

    // Define pos & size
    _area.cellid = cmtx.CellN(cxl, cyt);
    _area.cntx   = cxr - cxl + 1;
    _area.cnty   = cyd - cyt + 1;

}


/**
 *
 */
const VHArea & VHOptimalFigure::Area() const {
    return _area;
}

/**
 * 
 */
const uint16_t VHOptimalFigure::Width () const {
    return _area.cntx;
}

/**
 * 
 */
const uint16_t VHOptimalFigure::Height() const {
    return _area.cnty;
}

// auto [cx, cy] = cmtx.CellXY(_area.cellid);

/**
 * @brief Размер фигуры в ячейках
*/
// const strect & VHOptimalFigure::PosCells() const {
//     return objrect;
// }

/**
 * @brief Размер фигуры в пикселях
*/
// const strect VHOptimalFigure::PosAbs(const CellsMatrix & cmtx) const {
//     strect r;
//     int cs = cmtx.CellSize();
//     r.x1 = objrect.x1 * cs;
//     r.y1 = objrect.y1 * cs;
//     r.x2 = (objrect.x2 + 1) * cs - 1;
//     r.y2 = (objrect.y2 + 1) * cs - 1;
//     return r;
// }


/**
 * @brief Все участки фигуры
 */
// const std::vector<stspan> & VHOptimalFigure::Spans() const {
//     return arrspans;
// }


/**
 * @brief Участок # spann фигуры в пикселях
 */
// strect VHOptimalFigure::SpanRect(int spann, const CellsMatrix & cmtx) const {

//     // Отрезки заданной фигуры
//     const stspan & span = Span(spann);
//     auto [cx, cy] = cmtx.CellXY(span.n);

//     strect spanrect;

//     // Верхний левый угол
//     spanrect.x1  = cx * cmtx.CellSize();
//     spanrect.y1  = cy * cmtx.CellSize();

//     // Нижний правый угол
//     spanrect.x2  = spanrect.x1 + span.l * cmtx.CellSize() - 1;
//     spanrect.y2  = spanrect.y1 + cmtx.CellSize() - 1;

//     return spanrect;
// }

/**
 * @brief Оптимизация участков фигуры
 */
// void VHOptimalFigure::Sort(const CellsMatrix & cmtx) {

    // // Cортировка соседей последовательно
    // if(cfg.loglevel >= LOG_LEVEL_EXT) {
    //     std::string msg = "Figure #" + std::to_string(objid) + ", Sorting Sequental";
    //     VHLibOptimalLogger::lineout(msg);
    // }

//     SortSequental(cmtx);

// }


/**
 * @brief Квадрат расстояния между прямоугольниками ( по начальной точке )
*/
// int VHOptimalFigure::QDistance(
//     const stspan & p1,
//     const stspan & p2,
//     const CellsMatrix & cmtx) const
// {
//     auto [ xa, ya ] = cmtx.CellXY(p1.n);
//     auto [ xb, yb ] = cmtx.CellXY(p2.n);
//     int dx = xa - xb;
//     int dy = ya - yb;
//     int d = (dx*dx) + (dy*dy);
//     return d;
// }

/**
 * @brief   Поиск ближайшего участка по стартовой координате и минимальной дистанции
 *          Учитываем только участка с ненуловей длинной
 */
// int VHOptimalFigure::FindClosestSpan(
//     const stspan & span,
//     const std::vector<stspan> & arr,
//     const CellsMatrix & cmtx ) const
// {

//     int d = -1;
//     int n = -1;

//     for(int i=0; i < arr.size();i++) {

//         // Valid ?
//         if(arr[i].l) {

//             // distance ^ 2
//             int dcur = QDistance(span, arr[i], cmtx);

//             // Not same ?
//             if(dcur) {

//                 if(d==-1) {
//                     d = dcur;
//                     n = i;
//                 }
//                 else if(dcur<d) {
//                     d = dcur;
//                     n = i;
//                 }
//             }
//         }
//     }

//     return n;
// }


/**
 * @brief Для каждой фигуры - сортировка линий
 */
// void VHOptimalFigure::SortSequental(const CellsMatrix & cmtx) {

//     std::vector<stspan> arrAvail = arrspans;

//     std::vector<stspan> arrSorted;
//     arrSorted.reserve(arrspans.size());

//     int idx = 0;

//     while(idx != -1) {
//         arrSorted.push_back(arrAvail[idx]);
//         arrAvail[idx].l = 0;
//         const stspan & span = arrAvail[idx];
//         idx = FindClosestSpan(span, arrAvail, cmtx);
//     }

//     // Replace with sorted
//     arrspans = arrSorted;

// }

/**
 * @brief Найти ячейку слева либо справа ( sideFlag 0-Left/1-Right )
 */
// const int VHOptimalFigure::FindPosLRByY(const CellsMatrix & cmtx, uint16_t spancy, int sideFlag) const {

//     int r = -1;
//     uint16_t cellx;

//     for(int i=0; i < arrspans.size();i++) {

//         auto [ cx, cy ] = cmtx.CellXY(arrspans[i].n);
//         if(cy != spancy) continue;

//         if(r == -1) {
//             r = i;
//             cellx = cx;
//         } else {
//             bool cond = !sideFlag ? (cx < cellx) : (cx > cellx);
//             r = cond ? i : r;
//             cellx = cond ? cx : cellx;
//         }
//     }

//     // Not found
//     if(r == -1) return r;

//     // Return Left or Right celln
//     int cellLeft  = arrspans[r].n;
//     int cellRight = arrspans[r].n + arrspans[r].l - 1;

//     return !sideFlag ? cellLeft : cellRight;
// }

/**
 * @brief Найти ячейку сверху либо снизу ( sideFlag 0-Up / 1-Down )
 * 
 * @param spancx        Позиция ячейки по x
 * @param sideFlag      0: Поиск верхней координаты 1: Поиск нижней координаты
 */
// const int VHOptimalFigure::FindPosUDByX(const CellsMatrix & cmtx, uint16_t spancx, int sideFlag ) const {

//     int r = -1;
//     uint16_t posy;

//     for(int i=0; i < arrspans.size();i++) {

//         // Проверяем spancx на принадлежность
//         auto [cfx, cfy]     = cmtx.CellXY(arrspans[i].n);
//         auto [ctx, cty]     = cmtx.CellXY(arrspans[i].n + arrspans[i].l - 1);

//         // spancx вне диапазона участка ?
//         bool inside = spancx >= cfx && spancx <= ctx;
//         if(!inside) continue;

//         // Первый обнаруженый участок ?
//         if(r == -1) {
//             r = i;
//             posy = !sideFlag ? cfy : cty;
//             continue;
//         }

//         if(!sideFlag) {
//             // Поиск верхней координаты ячейки
//             if(cfy < posy) { posy = cfy; r = i; }
//         } else {
//             // Поиск нижней координаты ячейки
//             if(cty > posy) { posy = cty; r = i; }
//         }

//     }

//     // Not found
//     if(r == -1) return r;

//     // Return Top ot Bottom celln
//     return cmtx.CellN( spancx, posy);
// }


/**
 * @brief Обход по контуру
 */
void VHOptimalFigure::Border(const CellsMatrix & cmtx, CallbackBorder callbackBorder) const {

//     // Empty, exit ...
//     if(!arrspans.size()) return;

//     if(callbackBorder == nullptr) return;

//     uint16_t rows = objrect.y2 - objrect.y1 + 1;
//     int cs = cmtx.CellSize();

//     // Direction DOWN
//     for(int i=0; i < rows; i++) {
//         int n = FindPosLRByY(cmtx, objrect.y1 + i, 0);
//         if(n != -1 ) {
//             auto [cx, cy] = cmtx.CellXY(n);
//             uint16_t pxlx = cx * cs;
//             uint16_t pxly = cy * cs; // + (cs >> 1);
//             uint8_t cmd = !i ? cmdStart : cmdMove;
//             callbackBorder((void *)this, cmd, dirLeft, dirDown, cx, cy, pxlx, pxly);
//         }
//     }

//     // Direction UP
//     for(int i=0; i < rows; i++) {
//         int n = FindPosLRByY(cmtx, objrect.y2 - i, 1);
//         if(n != -1) {
//             auto [cx, cy] = cmtx.CellXY(n);
//             uint16_t pxlx = cx * cs + cs;
//             uint16_t pxly = (cy + 1) * cs - 1; //  + (cs >> 1);
//             uint8_t cmd = (i == rows - 1) ? cmdStop : cmdMove;
//             callbackBorder((void *)this, cmd, dirRight, dirUp, cx, cy, pxlx, pxly);
//         }
//     }

}

/**
 * @brief габариты фигуры по горизонтали
 */
// void VHOptimalFigure::ContentH(const CellsMatrix & cmtx, CallbackContent callbackContent) const {

//     // Empty, exit ...
//     if(!arrspans.size()) return;

//     if(callbackContent == nullptr) return;

//     uint16_t rows = objrect.y2 - objrect.y1 + 1;
//     int cs = cmtx.CellSize();

//     // Direction from Up to DOWN
//     for(int i=0; i < rows; i++) {
//         int nl = FindPosLRByY(cmtx, objrect.y1 + i, 0);
//         if(nl != -1) {
//             int nr = FindPosLRByY(cmtx, objrect.y1 + i, 1);
//             callbackContent((void *)this, nl, nr == -1 ? nl:nr, 0);
//         }
//     }

// }

/**
 * @brief габариты фигуры по вертикали
 */
// void VHOptimalFigure::ContentV(const CellsMatrix & cmtx, CallbackContent callbackContent) const {

//     // Empty, exit ...
//     if(!arrspans.size()) return;

//     if(callbackContent == nullptr) return;

//     uint16_t cols = objrect.x2 - objrect.x1 + 1;
//     int cs = cmtx.CellSize();

//     // Direction from Left to Right
//     for(int i=0; i < cols; i++) {
//         int nu = FindPosUDByX(cmtx, objrect.x1 + i, 0);
//         if(nu != -1) {
//             int nd = FindPosUDByX(cmtx, objrect.x1 + i, 1);
//             callbackContent((void *)this, nu, nd == -1 ? nu:nd, 1);
//         }
//     }

// }

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/figures/vhliboptimalfig.cpp
 * Revision         : 0.8.0-beta
 * Content size     : 11975
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 46ff575121a2f8e496578f8eb6d245a7
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */