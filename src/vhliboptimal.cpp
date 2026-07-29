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

VHLibOptimal::VHLibOptimal() { 
    SetSortMode(1);
}

// Memory Layout Setup
/**
 *
 */
size_t VHLibOptimal::CalcMemory() {

    // Hardware Setup
    printf("\n=== VHLibOptimal::CalcMemory() === \n");
    printf("sizeof(VHOptimalFigure) = %d bytes\n",  (int)sizeof(VHOptimalFigure));
    printf("sizeof(spanword) = %d bytes\n",         (int)sizeof(spanword));

    size_t total = 0;

    int gridWidth           = 1 << VHOPTIMAL_GRID_X_LEVEL;
    int gridHeight          = 1 << VHOPTIMAL_GRID_Y_LEVEL;
    printf("Grid size: %d x %d\n", gridWidth, gridHeight);

    int bytesPerGrid        = (gridWidth >> 3) * gridHeight;
    printf("x1 grid  = %d bytes\n", bytesPerGrid);

    total += bytesPerGrid * 2;
    printf("x2 grids = %d bytes\n", bytesPerGrid * 2);

    int bytesObjects        = sizeof(VHOptimalFigure) * VHOPTIMAL_OBJECTS_MAX;
    printf("%d objects x %d bytes = %d bytes\n",
        (int)sizeof(VHOptimalFigure),
        VHOPTIMAL_OBJECTS_MAX,
        bytesObjects );

    int bytesSpans          = sizeof(spanword) * VHOPTIMAL_SPANS_MAX;
    printf("%d spans x %d bytes = %d bytes\n",
        (int)sizeof(spanword),
        VHOPTIMAL_SPANS_MAX,
        bytesSpans );

    printf(">>> VHLIBOptimal Memory Layout Total: %d bytes\n", (int)total);
    printf("\n");

    return total;
}


verr VHLibOptimal::SetupMemory(uint8_t * ptr, size_t memsize) {

    // uint8_t     x_level,
    // uint8_t     y_level,
    // uint16_t    maxobjs,
    // uint16_t    maxspns

    return vok;
}



verr VHLibOptimal::Setup(
    const stConfig      &       cfgparams,
    CallbackGetSrcPxls          funcGetPixels,
    CallbackBorder              funcBorder,
    CallbackContent             funcContent,
    CallbackBenchmark           funcBenchmark
) {

    // Setup callbacks
    callbackGetPixels       = funcGetPixels;
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
verr VHLibOptimal::Run(uint16_t srcimgid) {

    if(vok != CheckCfgParams())
        return verrmsg(ERR_InvalidParams, "VHLibOptimal: Invalid parameters");

    buffLine.resize(cfg.cellsize);

    VHLIB_OPTIMAL_IFACE_FrameReset();

    if(vok != InitialScanImage(srcimgid))
        return verrmsg(ERR_PictureInitialization, "VHLibOptimal: InitPicture failed");


    // Scan objects task started
    if(callbackBenchmark != nullptr) callbackBenchmark(nullptr, eCmdBenchmarkScan, 0);

    while(FindFigure()) {
        if(vok != ConvertFigure()) break;
    }

    // Scan objects task completed
    if(callbackBenchmark != nullptr) callbackBenchmark(nullptr, eCmdBenchmarkScan, 1);


    if(cfg.loglevel >= LOG_LEVEL_BASE) {
        int objcount = VHLIB_OPTIMAL_IFACE_ObjectsCount();
        std::string msg = "Found " + std::to_string(objcount) + " objects";
        // VHLibOptimalLogger::lineout(msg);
        VHLibOptimalLogger::lineout(msg.c_str());
    }

    return vok;
}


/**
 * @brief Initialization: Check parameters
 */
verr VHLibOptimal::CheckCfgParams() {

    // Check callbacks
    if(!callbackGetPixels || !callbackBorder || !callbackContent )
        return verrmsg(1, "VHLibOptimal: Invalid callbacks / nullptr");

    // Check source
    if(!cfg.imageWidth || !cfg.imageHeight)
        return verrmsg(2, "VHLibOptimal: Invalid settings: source image props");

    if(!cfg.cellsize)
        return verrmsg(3, "VHLibOptimal: Invalid settings: cell size");

    // Initial parameters valid
    return vok;
}

/**
 * @brief Initialization: Initial Picture scan
 */
verr VHLibOptimal::InitialScanImage(uint16_t srcimgid) {

    // Calculate Cells Matrix Geometry
    cmatrix.Setup(cfg.imageWidth, cfg.imageHeight, cfg.cellsize);

    if(cfg.loglevel)
        VHLibOptimalLogger::PicProps(*this, cmatrix);

    uint32_t buffsize = cmatrix.BitMaskSizeBytes();

    // Setup Original Bitfield: allocate memory buffer
    bitfieldSrc.Setup(cmatrix, VHLIB_OPTIMAL_IFACE_BitFieldSrcPtr(), buffsize);

    // TODO: Only border (?)
    bitfieldSrc.Clear();

    // Setup Destination Bitfield: allocate memory buffer
    bitfieldDst.Setup(cmatrix, VHLIB_OPTIMAL_IFACE_BitFieldDstPtr(), buffsize);

    // Initial Scan
    if(callbackBenchmark != nullptr) callbackBenchmark(nullptr, eCmdBenchmarkSampling, 0);


    for(uint16_t celly=1; celly < cmatrix.CellsY() - 1; celly++) {
        for(uint16_t cellx=1; cellx < cmatrix.CellsX() - 1; cellx++) {
            if(IsCellFilled(srcimgid, cellx, celly, cfg.minColorVal)) {
                bitfieldSrc.SetCell(cmatrix.CellN(cellx,celly)); } } }

    if(callbackBenchmark != nullptr) callbackBenchmark(nullptr, eCmdBenchmarkSampling, 1);

    // Dump CellsMatrix
    if(cfg.loglevel >= LOG_LEVEL_MAX) {
        const uint8_t * ptr = VHLIB_OPTIMAL_IFACE_BitFieldSrcPtr();
        VHLibOptimalLogger::DumpCellsHEX(*this, cmatrix, ptr, "Original Bitfield HEX");
    }

    if(cfg.loglevel >= LOG_LEVEL_EXT) {
        const uint8_t * ptr = VHLIB_OPTIMAL_IFACE_BitFieldDstPtr();
        VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, ptr, "Original Bitfield TXT");
    }

    return vok;
}

/**
 * 
 */
bool VHLibOptimal::FindFigure() {

    // Clearing figure before processing
    bitfieldDst.Clear();
    bitfieldDst.ResetSearchIndex(cmatrix);

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
        std::string msg = "Figure #" + std::to_string(objid) + " found";
        // VHLibOptimalLogger::lineout(msg);
        VHLibOptimalLogger::lineout(msg.c_str());
    }

    if(cfg.loglevel >= LOG_LEVEL_MAX) {
        uint8_t * ptr = VHLIB_OPTIMAL_IFACE_BitFieldSrcPtr();
        VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, ptr, "Original");
    }
    
    if(cfg.loglevel >= LOG_LEVEL_EXT) {
        uint8_t * ptr = VHLIB_OPTIMAL_IFACE_BitFieldDstPtr();
        VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, ptr, "Figure");
    }

    // 
    VHOptimalFigure & newfigure = VHLIB_OPTIMAL_IFACE_Object(objid);

    newfigure.Scan(bitfieldDst, cmatrix, cfg.spccnt);
    newfigure.CalcPosAndSize(cmatrix);

    // if(IsSortEnabled())
    //     newfigure.Sort(cmatrix);

    if(cfg.loglevel >= LOG_LEVEL_EXT)
        VHLibOptimalLogger::DumpFigureSpans(newfigure, cmatrix, CellSize() );

    int figw   = newfigure.Width (cmatrix, CellSize());
    int figh   = newfigure.Height(cmatrix, CellSize());

    bool sizew = figw >= cfg.min_obj_width && figw <= cfg.max_obj_width;
    bool sizeh = figh >= cfg.min_obj_height && figh <= cfg.max_obj_height;

    if(!(sizew && sizeh)) {
        VHLIB_OPTIMAL_IFACE_RemoveObject();
        if(cfg.loglevel >= LOG_LEVEL_EXT) {
            std::string msg = "Figure #" + std::to_string(objid) + " skipped";
            // VHLibOptimalLogger::lineout(msg);
            VHLibOptimalLogger::lineout(msg.c_str());
        }
    }

    return vok;
}

/**
 * 
 */
bool VHLibOptimal::CheckWhiteLevel(const std::vector<uint8_t> & arr, uint8_t whitelevel) const {
    for(size_t i=0;i<arr.size();i++) {
        if(arr[i] >= whitelevel) return true;
    }
    return false;
}

/**
 * 
 */
bool VHLibOptimal::IsCellFilled(uint16_t srcimgid, uint16_t cellx, uint16_t celly, uint8_t whitelevel) {

    for(int l=0;l<CellSize();l++) {

        uint16_t imgposx = cellx * CellSize();
        uint16_t imgposy = celly * CellSize() + l;

        callbackGetPixels((void *)this, buffLine.data(), buffLine.size(), srcimgid, imgposx, imgposy);
        if(CheckWhiteLevel(buffLine, whitelevel))
            return true;
    }

    return false;
}

/**
 * 
 */
bool VHLibOptimal::IsSortEnabled() {
    return sortMode > 0;
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
const size_t VHLibOptimal::CellSize    () const {
    return cfg.cellsize;
}

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

/**
 * 
 */
void VHLibOptimal::SetSortMode(uint8_t mode) {
    sortMode = mode;
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