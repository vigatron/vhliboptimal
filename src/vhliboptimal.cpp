/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.cpp
 * Content size  : 8479
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : c4ae9812788294ed0e28fd49ceaf4718
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "vhliboptimal.hpp"
#include "log/log.hpp"

using namespace vhliboptimal;

VHLibOptimal::VHLibOptimal() { }


verr VHLibOptimal::Setup(
    const stConfig      &       cfgparams,
    CallbackBorder              funcBorder,
    CallbackContent             funcContent,
    CallbackBenchmark           funcBenchmark
) {

    // Setup callbacks
    callbackBorder          = funcBorder;
    callbackContent         = funcContent;
    callbackBenchmark       = funcBenchmark;

    // Save initial parameters
    cfg = cfgparams;

    // Return check status
    return CheckCfgParams();
}

/**
 * @brief Start process
 */
verr VHLibOptimal::Run() {

    if(vok != CheckCfgParams())
        return verrmsg(ERR_InvalidParams, "VHLibOptimal: Invalid parameters");

    VHLIB_OPTIMAL_IFACE_FrameReset();

    if(cfg.loglevel >= LOG_LEVEL_MAX) {
        log::DumpCellsHEX(*this, cmatrix, memlay.BitFieldSrcPtr(), "Original Bitfield HEX"); }

    if(cfg.loglevel >= LOG_LEVEL_EXT) {
        log::DumpCellsTXT(*this, cmatrix, memlay.BitFieldSrcPtr(), "Original Bitfield TXT"); }

    // Important!
    bitfieldSrc.ResetSearchIndex(cmatrix);

    // Scan objects task started
    if(callbackBenchmark != nullptr) callbackBenchmark(nullptr, eCmdBenchmarkScan, 0);

    while(FindFigure()) {
        if(vok != ConvertFigure()) break;
    }

    // Scan objects task completed
    if(callbackBenchmark != nullptr) callbackBenchmark(nullptr, eCmdBenchmarkScan, 1);

    if(cfg.loglevel >= LOG_LEVEL_BASE) {
        int objcount = VHLIB_OPTIMAL_IFACE_ObjectsCount();
        log::partout("Found "); log::partint(objcount); log::partout(" objects");
        log::newlout();
    }

    return vok;
}


/**
 * @brief Initialization: Check parameters
 */
verr VHLibOptimal::CheckCfgParams() {

    if(cfg.levelcs > 10)
        return verrmsg(3, "VHLibOptimal: Invalid settings: cell size > 10");

    // Initial parameters valid
    return vok;
}

/**
 * 
 */
bool VHLibOptimal::FindFigure() {

    // Clearing figure before processing
    bitfieldDst.Clear(cmatrix);

    // find entry point of figure
    int celln = bitfieldSrc.FindEntryCell(cmatrix);
    if(celln < 0) return false;


    bool flagLoopCells = true;

    while(flagLoopCells) {

        bitfieldSrc.ClrCell(celln);
        bitfieldDst.SetCell(celln);

        // Find from prev
        celln = bitfieldSrc.FindNearest(cmatrix, celln);
        if(celln != -1) continue;

        // Find in full path
        celln = bitfieldSrc.FindPath(cmatrix, bitfieldDst);
        if(celln != -1) continue;

        // step-out, processing next figure ...
        flagLoopCells = false;
    }

    return true;
}

/**
 * 
 */
verr VHLibOptimal::ConvertFigure() {

    // Out of mem ?
    if( !VHLIB_OPTIMAL_IFACE_AddObject() )
        return verror(1);

    int objid = VHLIB_OPTIMAL_IFACE_ObjectsCount() - 1;

    if(cfg.loglevel >= LOG_LEVEL_EXT) {
        log::partout("Figure #");
        log::partint(objid);
        log::partout(" found");
        log::newlout();
    }

    if(cfg.loglevel >= LOG_LEVEL_MAX) {
        log::DumpCellsTXT(*this, cmatrix, memlay.BitFieldSrcPtr(), "Original");
    }
    
    if(cfg.loglevel >= LOG_LEVEL_EXT) {
        log::DumpCellsTXT(*this, cmatrix, memlay.BitFieldDstPtr(), "Figure");
    }

    // 
    VHOptimalFigure & newfigure = VHLIB_OPTIMAL_IFACE_Object(objid);

    newfigure.Scan(bitfieldDst, cmatrix, cfg.spccnt);
    newfigure.CalcPosAndSize(cmatrix);

    // if(IsSortEnabled())
    //     newfigure.Sort(cmatrix);

    if(cfg.loglevel >= LOG_LEVEL_EXT) {
        log::DumpFigureSpans(newfigure, cmatrix, CellSZ());
    }

    uint16_t figw   = newfigure.Width (cmatrix, CellSZ());
    uint16_t figh   = newfigure.Height(cmatrix, CellSZ());
    bool sizew = figw >= cfg.min_obj_width && figw <= cfg.max_obj_width;
    bool sizeh = figh >= cfg.min_obj_height && figh <= cfg.max_obj_height;

    if(!(sizew && sizeh)) {
        VHLIB_OPTIMAL_IFACE_RemoveObject();
        if(cfg.loglevel >= LOG_LEVEL_EXT) {
            log::partout("Figure #");
            log::partint(objid);
            log::partout(" skipped");
            log::newlout();
        }
    }

    return vok;
}

/**
 * 
 */
bool VHLibOptimal::IsSortEnabled() {
    return cfg.sortMode > 0;
}

/** 
 * @brief Количество фигур
 * 
 * @return общее количество
 */
const size_t VHLibOptimal::GetObjectsCount() const {
    return VHLIB_OPTIMAL_IFACE_ObjectsCount();
}

/** 
 * @brief Объект фигуры по индексу
 */
const VHOptimalFigure & VHLibOptimal::GetObject(int idx) const {
    asrts(idx < GetObjectsCount(), 0, "VHLibOptimal::GetObject out of range");
    return VHLIB_OPTIMAL_IFACE_Object(idx);
}

/**
 * @brief Количество участков
 * 
 * @return общее количество всех фигур
*/
const size_t VHLibOptimal::GetSpansTotal() const {

    int r = 0;

    for(int i=0; i < GetObjectsCount();i++) {
        const VHOptimalFigure & obj = GetObject(i);
        r += obj.SpansCount();
    }

    return r;
}

/**
 * @brief 2D Конфигурационная сетка
*/
const CellsMatrix & VHLibOptimal::GetCMatrix() const {
    return cmatrix;
}

/**
 * 
 */
// const size_t VHLibOptimal::CellSize    () const { return cfg.cellsize; }

/**
 * 
 */
bool VHLibOptimal::Border(int objn) const {

    for(int i = 0; i < GetObjectsCount(); i++) {
        const vhliboptimal::VHOptimalFigure & obj = GetObject(i);
        const vhliboptimal::CellsMatrix & cmtx = GetCMatrix();
        // obj.Border(cmtx, callbackBorder);
    }

    return true;
}

/**
 * 
 */
bool VHLibOptimal::ContentH(int objn) const {
    const vhliboptimal::VHOptimalFigure & objfig = GetObject(objn);
    // objfig.ContentH(GetCMatrix(), callbackContent);
    return true;
}

/**
 * 
 */
bool VHLibOptimal::ContentV(int objn) const {
    const vhliboptimal::VHOptimalFigure & objfig = GetObject(objn);
    // objfig.ContentV(GetCMatrix(), callbackContent);
    return true;
}

VHMemoryLayout & VHLibOptimal::MemoryLayout() {
    return memlay;
}

BitField & VHLibOptimal::BitFieldSrc() {
    return bitfieldSrc;
}


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimal.cpp
 * Revision         : 0.8.0-beta
 * Content size     : 8479
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : c4ae9812788294ed0e28fd49ceaf4718
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */