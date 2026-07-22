/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.6beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.cpp
 * Content size  : 6402
 * Date / Time   : 22-07-2026 14:54:04
 * MD5           : ab8fa10c7237696cdb5e76c7be822f79
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "vhliboptimal.hpp"
#include "vhliboptimallog.hpp"

using namespace vhliboptimal;

VHLibOptimal::VHLibOptimal() { 

}

verr VHLibOptimal::Setup(
    const stConfig      &       cfgparams,
    CallbackGetSrcPxls          funcGetPixels,
    CallbackBorder              funcBorder,
    CallbackContent             funcContent
) {

    // Setup callbacks
    callbackGetPixels       = funcGetPixels;
    callbackBorder          = funcBorder;
    callbackContent         = funcContent;

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

    if(vok != InitialScanImage(srcimgid))
        return verrmsg(ERR_PictureInitialization, "VHLibOptimal: InitPicture failed");

    arrFigures.clear();

    // Scan objects
    while(FindFigure()) {
        ConvertFigure();
    }

    return vok;
}

/**
 * 
 */
void VHLibOptimal::SetLogLevel(int verbose) {
    loglevel = verbose;
}

/**
 * @brief Initialization: Check parameters
 */
verr VHLibOptimal::CheckCfgParams() {

    // Check callbacks
    if(!callbackGetPixels || !callbackBorder || !callbackContent)
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

    if(loglevel)
        VHLibOptimalLogger::PicProps(*this, cmatrix);

    uint32_t bitbuffsize = cmatrix.BitMaskSizeBytes();

    // Setup Original Bitfield and allocate memory buffer
    buffArrSrc.assign(bitbuffsize, 0);
    bitfieldSrc.Setup(cmatrix, buffArrSrc.data(), bitbuffsize);

    // Setup Destination Bitfield and allocate memory buffer
    buffArrDst.assign(bitbuffsize, 0);
    bitfieldDst.Setup(cmatrix, buffArrDst.data(), bitbuffsize);

    // Initial Scan
    for(uint16_t celly=0; celly < cmatrix.CellsY(); celly++) {
        for(uint16_t cellx=0; cellx < cmatrix.CellsX(); cellx++) {
            if(IsCellFilled(srcimgid, cellx, celly, cfg.minColorVal)) {
                int idx = cmatrix.CellN(cellx,celly);
                bitfieldSrc.SetCell(idx);
            }
        }
    }

    // Dump CellsMatrix
    if(loglevel >= LOG_LEVEL_MAX) {
        VHLibOptimalLogger::DumpCellsHEX( *this, cmatrix, buffArrSrc, "Original Bitfield HEX");
    }
    
    if(loglevel >= LOG_LEVEL_EXT) {
        VHLibOptimalLogger::DumpCellsTXT( *this, cmatrix, buffArrSrc, "Original Bitfield TXT");
    }

    return vok;
}

/**
 * 
 */
bool VHLibOptimal::FindFigure() {

    // Clearing figure before processing
    std::memset(buffArrDst.data(), 0, buffArrDst.size());

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
bool VHLibOptimal::ConvertFigure() {

    int fign = arrFigures.size();

    if(loglevel) {
        std::string msg = "Figure #" + std::to_string(fign) + " found";
        VHLibOptimalLogger::lineout(msg);
    }

    if(loglevel >= LOG_LEVEL_MAX)
        VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, buffArrSrc, "Original");
    
    if(loglevel >= LOG_LEVEL_EXT)
        VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, buffArrDst, "Figure");

    // Структура параметров текущей фигуры
    VHOptimalFigure newfigure(bitfieldDst, cmatrix, cfg.spccnt);

    // Cортировка соседей последовательно
    if(loglevel >= LOG_LEVEL_EXT) {
        std::string msg = "Fig #" + std::to_string(fign) + ", Sorting Sequental";
        VHLibOptimalLogger::lineout(msg);
    }

    newfigure.Sort(cmatrix);

    if(loglevel >= LOG_LEVEL_EXT)
        VHLibOptimalLogger::DumpFigureSpans(newfigure, cmatrix);

    arrFigures.push_back(newfigure);

    return true;
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

    for(int l=0;l<cmatrix.CellSize();l++) {

        uint16_t imgposx = cellx * cmatrix.CellSize();
        uint16_t imgposy = celly * cmatrix.CellSize() + l;

        callbackGetPixels((void *)this, buffLine.data(), buffLine.size(), srcimgid, imgposx, imgposy);
        if(CheckWhiteLevel(buffLine, whitelevel))
            return true;
    }

    return false;
}

/** 
 * @brief Количество фигур
 * 
 * @return общее количество
 */
const size_t VHLibOptimal::GetObjectsCount() const {
    return arrFigures.size();
}

/** 
 * @brief Объект фигуры по индексу
 */
const VHOptimalFigure & VHLibOptimal::GetObject(int idx) const {
    asrts(idx < GetObjectsCount(), 0, "VHLibOptimal::GetObject out of range");
    return arrFigures[idx];
}

/**
 * @brief Количество участков
 * 
 * @return общее количество всех фигур
*/
const size_t VHLibOptimal::GetSpansTotal() const {

    int r = 0;

    for(int i=0;i<arrFigures.size();i++) {
        r += arrFigures[i].SpansCount();
    }

    return r;
}

/**
 * @brief 2D Конфигурационная сетка
*/
const CellsMatrix & VHLibOptimal::GetCMatrix() const {
    return cmatrix;
}


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimal.cpp
 * Revision         : 0.6beta
 * Content size     : 6402
 * Date / Time      : 22-07-2026 14:54:04
 * MD5              : ab8fa10c7237696cdb5e76c7be822f79
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */