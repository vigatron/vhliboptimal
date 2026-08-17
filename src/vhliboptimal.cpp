/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.cpp
 * Content size  : 7324
 * Date / Time   : 16-08-2026 07:08:10
 * MD5           : dd5e5cc027b9b1c0f9c42a2c66468893
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "vhliboptimal.hpp"
#include "log/log.hpp"

using namespace vhliboptimal;

VHLibOptimal::VHLibOptimal() : _initialized(false) {}


verr VHLibOptimal::Setup(
    const stConfig &cfgparams,
    const uint16_t gridw,
    const uint16_t gridh,
    const VHMemRegion &regGridSrc,
    const VHMemRegion &regGridDst,
    const VHMemRegion &regObjects,
    const VHMemRegion &regSpans,
    void *callbackparent,
    CallbackBorder funcBorder,
    CallbackContent funcContent,
    CallbackBenchmark funcBenchmark)
{

    if (!cmatrix.Setup(gridw, gridh))
        return verrmsg(101, "Invalid Grid settings");

    if (memlay.SetupMemory(regGridSrc, regGridDst, regObjects, regSpans))
        return verrmsg(104, "Memory Layout Initialization failed");

    // Init Src Bitfield
    if (!bitfieldSrc.Setup(cmatrix, memlay.BitFieldSrcPtr(), memlay.BitFieldSrcSize()))
        return verrmsg(100, "BitFieldSrc Setup issue");

    // Init Dst Bitfield
    if (!bitfieldDst.Setup(cmatrix, memlay.BitFieldDstPtr(), memlay.BitFieldDstSize()))
        return verrmsg(100, "BitFieldDst Setup issue");

    // Setup callbacks

    if (callbackparent == nullptr)
        return verrmsg(105, "callback: invalid parent caller ");

    callback_caller = callbackparent;
    callbackBorder = funcBorder;
    callbackContent = funcContent;
    callbackBenchmark = funcBenchmark;

    // Save initial parameters
    cfg = cfgparams;

    // Return check status
    verr vcheck = CheckCfgParams();
    if (vok != vcheck)
        return verrmsg(1000, "VHLibOptimal::Setup() not completed");

    _initialized = true;

    return vok;
}

/**
 * @brief Start process
 */
verr VHLibOptimal::Run()
{

    if (!_initialized)
        return verrmsg(ERR_InvalidParams, "VHLibOptimal: Not initialized correctly");

    FrameReset();

    bitfieldSrc.ClearBorder(GetCMatrix());

    if (cfg.loglevel >= LOG_LEVEL_MAX)
    {
        DumpBitfield(true);
    }

    if (cfg.loglevel >= LOG_LEVEL_EXT)
    {
        DumpBitfield();
    }

    // Important!
    bitfieldSrc.ResetSearchIndex(cmatrix);

    // Scan objects task started
    if (callbackBenchmark != nullptr)
        callbackBenchmark(callback_caller, eCmdBenchmarkScan, 0);

    while (ScanAndFindFigure())
    {
        if (vok != ConvertFigure())
            return verrmsg(111, "object scanning: conversion failed");
    }

    // Scan objects task completed
    if (callbackBenchmark != nullptr)
        callbackBenchmark(callback_caller, eCmdBenchmarkScan, 1);

    return vok;
}

/**
 * @brief Initialization: Check parameters
 */
verr VHLibOptimal::CheckCfgParams()
{

    // Initial parameters valid
    return vok;
}

/**
 *
 */
bool VHLibOptimal::ScanAndFindFigure()
{

    // Clearing figure before processing
    bitfieldDst.ClearArea(cmatrix);

    // find entry point of figure
    int celln = bitfieldSrc.FindEntryCell(cmatrix);
    if (celln < 0)
        return false;

    bool flagLoopCells = true;

    while (flagLoopCells)
    {

        bitfieldSrc.ClrCell(celln);
        bitfieldDst.SetCell(celln);

        // Find from prev
        celln = bitfieldSrc.FindNearest(cmatrix, celln);
        if (celln != -1)
            continue;

        // Find in full path
        celln = bitfieldSrc.FindPath(cmatrix, bitfieldDst);
        if (celln != -1)
            continue;

        // step-out, processing next figure ...
        flagLoopCells = false;
    }

    return true;
}

/**
 *
 */
verr VHLibOptimal::ConvertFigure()
{
    uint16_t objid = ObjectsCount();

    // Out of mem ?
    if (!AddObject())
        return verrmsg(1, "VHLibOptimal::ConvertFigure() max objects count reached");

    if (cfg.loglevel >= LOG_LEVEL_EXT)
    {
        log::partout("Figure #");
        log::partint(objid);
        log::partout(" found");
        log::newlout();
    }

    if (cfg.loglevel >= LOG_LEVEL_MAX)
    {
        log::DumpCellsTXT(*this, cmatrix, memlay.BitFieldSrcPtr(), "Original");
    }

    if(cfg.loglevel >= LOG_LEVEL_EXT) {
        log::DumpCellsTXT(*this, cmatrix, memlay.BitFieldDstPtr(), "Figure");
    }

    //
    VHOptimalFigure &newfigure = Object(objid);
    newfigure.Init(GlobalSpansCount());

    // Runtime local array
    VHLocalSpansArray arrRntSpans;
    arrRntSpans.Init(memlay.GlobalSpans(), GlobalSpansCount());

    if (vok == newfigure.Scan(bitfieldDst, cmatrix, cfg.spccnt, arrRntSpans))
    {
        newfigure.CalcPosAndSize(cmatrix, arrRntSpans);

        // Applying size filter
        uint16_t figw = newfigure.Width();
        uint16_t figh = newfigure.Height();
        bool sizew = figw >= cfg.min_obj_width && figw <= cfg.max_obj_width;
        bool sizeh = figh >= cfg.min_obj_height && figh <= cfg.max_obj_height;

        if (sizew && sizeh)
        {
            _spnCount += arrRntSpans.localspanscnt();
        }
        else
        {
            RemoveObject();
            if (cfg.loglevel >= LOG_LEVEL_EXT)
            {
                log::partout("Figure #");
                log::partint(objid);
                log::partout(" skipped");
                log::newlout();
            }
        }
    }
    else
    {
        RemoveObject();
        return verrmsg(2, "VHLibOptimal::ConvertFigure() failed, no spans avail");
    }

    // if(IsSortEnabled())
    //     newfigure.Sort(cmatrix);

    if (cfg.loglevel >= LOG_LEVEL_EXT)
    {
        log::DumpFigureSpans(*this, newfigure, cmatrix, DEF_CELL_SIZE);
    }

    return vok;
}

/**
 *
 */
bool VHLibOptimal::IsSortEnabled()
{
    return cfg.sortMode > 0;
}

/**
 * @brief Подсчет количество участков
 *
 * @return общее количество всех фигур
 */
const size_t VHLibOptimal::CalcSpansTotal() const
{

    int r = 0;

    for (int i = 0; i < ObjectsCount(); i++)
    {
        const VHOptimalFigure &obj = Object(i);
        r += obj.SpansCount();
    }

    return r;
}

/**
 * @brief 2D Конфигурационная сетка
 */
const CellsMatrix &VHLibOptimal::GetCMatrix() const
{
    return cmatrix;
}

/**
 *
 */
bool VHLibOptimal::Border(int objn) const
{

    VHLocalSpansArray arrspans;

    for (int i = 0; i < ObjectsCount(); i++)
    {
        const vhliboptimal::CellsMatrix &cmtx = GetCMatrix();
        const vhliboptimal::VHOptimalFigure &obj = Object(i);
        arrspans.Init(memlay.GlobalSpans(), obj.StartSpanIDX());
        obj.Border(cmtx, arrspans, callback_caller, callbackBorder);
    }

    return true;
}

/**
 *
 */
bool VHLibOptimal::ContentH(int objn) const
{

    const vhliboptimal::VHOptimalFigure &obj = Object(objn);

    VHLocalSpansArray arrspans;
    arrspans.Init(memlay.GlobalSpans(), obj.StartSpanIDX());

    obj.ContentH(GetCMatrix(), arrspans, callback_caller, callbackContent);

    return true;
}

/**
 *
 */
bool VHLibOptimal::ContentV(int objn) const
{

    const vhliboptimal::VHOptimalFigure &obj = Object(objn);

    VHLocalSpansArray arrspans;
    arrspans.Init(memlay.GlobalSpans(), obj.StartSpanIDX());

    obj.ContentV(GetCMatrix(), arrspans, callback_caller, callbackContent);

    return true;
}

/**
 *
 */
void VHLibOptimal::DumpBitfield(bool hexmode)
{

    if (hexmode)
    {

        log::DumpCellsHEX(
            *this,
            cmatrix,
            memlay.BitFieldSrcPtr(),
            "Original Bitfield HEX");
    }
    else
    {

        log::DumpCellsTXT(
            *this,
            cmatrix,
            memlay.BitFieldSrcPtr(),
            "Original Bitfield TXT");
    }
}

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimal.cpp
 * Revision         : 0.8.0-beta
 * Content size     : 7324
 * Date / Time      : 16-08-2026 07:08:10
 * MD5              : dd5e5cc027b9b1c0f9c42a2c66468893
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */