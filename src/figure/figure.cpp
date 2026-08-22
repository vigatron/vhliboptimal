/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.1
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/figure/figure.cpp
 * Content size  : 8863
 * Date / Time   : 22-08-2026 15:42:01
 * MD5           : c1df3c122f6748e8a98587caf59c3e1d
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
VHOptimalFigure::VHOptimalFigure() {}

/**
 * @brief Сканирование битового поля и сохранение участков фигуры
 *
 * @param bfld BitField
 * @param cmtx 2D Area config
 * @param skipcellsmax макс количество пустых ячеек подряд
 */
verr VHOptimalFigure::Scan(
    BitField &bfld,
    const CellsMatrix &cmtx,
    int skipcellsmax,
    VHLocalSpansArray &arrspans)
{

    bool loop = true;
    _startIDX = arrspans.globalstartidx();
    _spansCount = 0;

    // Сканируем участоки фигуры
    while (loop)
    {

        // начальная ячейка
        int curn = bfld.FindEntryCell(cmtx);
        if (curn == -1)
        {
            loop = false;
            break;
        }

#ifdef VHLIB_OPTIMAL_DEBUG
        auto [dbgx, dbgy] = cmtx.CellXY(curn);
#endif

        // Вычисляем длинну отрезка ( + пустые ячейки, до 3х )
        int spanid = (uint32_t)curn;
        int spanww = bfld.ScanSpanLen(cmtx, curn, skipcellsmax);
        spanword pckword = pack_span(spanid, spanww);

        // добавляем участок в список текущей фигуры
        if (arrspans.add(pckword))
        {
            _spansCount++;
        }
        else
        {
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
    const CellsMatrix &cmtx,
    const VHLocalSpansArray &arrspans)
{

    // Empty
    if (!_spansCount)
        return;

    // Initial values from first span
    const spanword spanw = arrspans.get(0);
    const int spanid = get_span_id(spanw);
    const int spanln = get_span_len(spanw);
    auto [cxi1, cyi1] = cmtx.CellXY(spanid);
    auto [cxi2, cyi2] = cmtx.CellXY(spanid + spanln - 1);

    int cxl = cxi1, cxr = cxi2, cyt = cyi1, cyd = cyi2;

    for (uint32_t i = 1; i < _spansCount; i++)
    {

        const spanword spanw = arrspans.get(i);
        const int spanid = get_span_id(spanw);
        const int spanln = get_span_len(spanw);

        auto [cx1, cy1] = cmtx.CellXY(spanid);
        auto [cx2, cy2] = cmtx.CellXY(spanid + spanln - 1);

        if (cxl > cx1)
        {
            cxl = cx1;
        }
        if (cyt > cy1)
        {
            cyt = cy1;
        }
        if (cxr < cx2)
        {
            cxr = cx2;
        }
        if (cyd < cy2)
        {
            cyd = cy2;
        }
    }

    // Define pos & size
    _area.cellid = cmtx.CellN(cxl, cyt);
    _area.cntx = cxr - cxl + 1;
    _area.cnty = cyd - cyt + 1;
}

/**
 *
 */
const VHArea &VHOptimalFigure::Area() const
{
    return _area;
}

/**
 *
 */
uint16_t VHOptimalFigure::Width() const
{
    return _area.cntx;
}

/**
 *
 */
uint16_t VHOptimalFigure::Height() const
{
    return _area.cnty;
}

/**
 * @brief Найти ячейку слева либо справа ( sideFlag 0-Left/1-Right )
 */
int VHOptimalFigure::FindPosLRByY(
    const CellsMatrix &cmtx,
    const VHLocalSpansArray &arrspans,
    uint16_t spancy,
    int sideFlag) const
{

    int ret = -1;
    uint16_t cellx;

    for (int i = 0; i < _spansCount; i++)
    {
        const spanword spanw = arrspans.get(i);
        const int spanid = get_span_id(spanw);
        // const int spanln = get_span_len(spanw);

        // Проверяем spancy на принадлежность
        auto [cx, cy] = cmtx.CellXY(spanid);
        if (cy != spancy)
            continue;

        if (ret == -1)
        {
            ret = i;
            cellx = cx;
        }
        else
        {
            bool cond = !sideFlag ? (cx < cellx) : (cx > cellx);
            ret = cond ? i : ret;
            cellx = cond ? cx : cellx;
        }
    }

    // Not found
    if (ret == -1)
        return ret;

    // Return Left or Right celln
    const spanword spanw = arrspans.get(ret);
    const int spanid = get_span_id(spanw);
    const int spanln = get_span_len(spanw);

    int cellLeft = spanid;
    int cellRight = spanid + spanln - 1;

    return !sideFlag ? cellLeft : cellRight;
}

/**
 * @brief Найти ячейку сверху либо снизу ( sideFlag 0-Up / 1-Down )
 *
 * @param spancx        Позиция ячейки по x
 * @param sideFlag      0: Поиск верхней координаты 1: Поиск нижней координаты
 */
int VHOptimalFigure::FindPosUDByX(
    const CellsMatrix &cmtx,
    const VHLocalSpansArray &arrspans,
    uint16_t spancx,
    int sideFlag) const
{

    int r = -1;
    uint16_t posy;

    for (int i = 0; i < _spansCount; i++)
    {
        const spanword spanw = arrspans.get(i);
        const int spanid = get_span_id(spanw);
        const int spanln = get_span_len(spanw);

        // Проверяем spancx на принадлежность
        auto [cfx, cfy] = cmtx.CellXY(spanid);
        auto [ctx, cty] = cmtx.CellXY(spanid + spanln - 1);

        // spancx вне диапазона участка ?
        bool inside = spancx >= cfx && spancx <= ctx;
        if (!inside)
            continue;

        // Первый обнаруженый участок ?
        if (r == -1)
        {
            r = i;
            posy = !sideFlag ? cfy : cty;
            continue;
        }

        if (!sideFlag)
        {
            // Поиск верхней координаты ячейки
            if (cfy < posy)
            {
                posy = cfy;
                r = i;
            }
        }
        else
        {
            // Поиск нижней координаты ячейки
            if (cty > posy)
            {
                posy = cty;
                r = i;
            }
        }
    }

    // Not found
    if (r == -1)
        return r;

    // Return Top ot Bottom celln
    return cmtx.CellN(spancx, posy);
}

/**
 * @brief Обход по контуру
 */
void VHOptimalFigure::Border(
    const CellsMatrix &cmtx,
    const VHLocalSpansArray &arrspans,
    void *caller,
    CallbackBorder callbackBorder) const
{

    // Empty, exit ...
    if(!_spansCount)
        return;

    if(callbackBorder == nullptr)
        return;

    auto [scx, scy] = cmtx.CellXY(_area.cellid);

    // Direction DOWN
    for(int i=0; i < _area.cnty; i++) {
        int celln = FindPosLRByY(cmtx, arrspans, scy + i, 0);
        if(celln != -1 ) {
            auto [cx, cy] = cmtx.CellXY(celln);
            uint8_t cmd = !i ? cmdStart : cmdMove;
            callbackBorder(caller, cmd, dirLeft, dirDown, cx, cy);
        }
    }

    // Direction UP
    for(int i=0; i < _area.cnty; i++) {
        int celln = FindPosLRByY(cmtx, arrspans, scy + _area.cnty - 1 - i, 1);
        if(celln != -1) {
            auto [cx, cy] = cmtx.CellXY(celln);
            uint8_t cmd = (i == _area.cnty - 1) ? cmdStop : cmdMove;
            callbackBorder(caller, cmd, dirRight, dirUp, cx, cy);
        }
    }

}

/**
 * @brief габариты фигуры по горизонтали
 */
void VHOptimalFigure::ContentH(
    const CellsMatrix &cmtx,
    const VHLocalSpansArray &arrspans,
    void *caller,
    CallbackContent callbackContent) const
{

    // Empty, exit ...
    if (!_spansCount)
        return;

    // Feature not linked ?
    if (callbackContent == nullptr)
        return;

    auto [scx, scy] = cmtx.CellXY(_area.cellid);

    // Direction from Up to DOWN
    for (int i = 0; i < _area.cnty; i++)
    {
        int nl = FindPosLRByY(cmtx, arrspans, scy + i, 0);
        if (nl != -1)
        {
            int nr = FindPosLRByY(cmtx, arrspans, scy + i, 1);
            callbackContent(caller, nl, nr == -1 ? nl : nr, 0);
        }
    }
}

/**
 * @brief габариты фигуры по вертикали
 */
void VHOptimalFigure::ContentV(
    const CellsMatrix &cmtx,
    const VHLocalSpansArray &arrspans,
    void *caller,
    CallbackContent callbackContent) const
{

    // Empty, exit ...
    if (!_spansCount)
        return;

    // Feature not linked ?
    if (callbackContent == nullptr)
        return;

    auto [scx, scy] = cmtx.CellXY(_area.cellid);

    // Direction from Left to Right
    for (int i = 0; i < _area.cntx; i++)
    {
        int nu = FindPosUDByX(cmtx, arrspans, scx + i, 0);
        if (nu != -1)
        {
            int nd = FindPosUDByX(cmtx, arrspans, scx + i, 1);
            if(nd >= (1<<VHLIB_OPTIMAL_GRID_LX) * (1<<VHLIB_OPTIMAL_GRID_LX)) { 
                asm("nop");
            }
            callbackContent(caller, nu, nd == -1 ? nu : nd, 1);
        }
    }
}

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/figure/figure.cpp
 * Revision         : 0.8.1
 * Content size     : 8863
 * Date / Time      : 22-08-2026 15:42:01
 * MD5              : c1df3c122f6748e8a98587caf59c3e1d
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */