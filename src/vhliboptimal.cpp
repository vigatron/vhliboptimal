/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.2
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.cpp
 * Content size  : 5867
 * Date / Time   : 20-07-2026 03:16:52
 * MD5           : d02e6f3c82e266ee159001250f65a4c6
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
    const stConfig      &   cfgparams,
    GetPixelsCallback       callbackGetPixelsHandler,
    SetPosCallback          callbackSetPosHandler )
{

    // Setup callbacks
    callbackGetPixels     = callbackGetPixelsHandler;
    callbackSetPos        = callbackSetPosHandler;

    // Save initial parameters
    cfg = cfgparams;

    // Return check status
    return CheckCfgParams();
}


/**
 * @brief Start process
 */
verr VHLibOptimal::Run(
) {

    if(vok != CheckCfgParams())
        return verrmsg(ERR_InvalidParams, "VHLibOptimal: Invalid parameters");

    if(vok != InitPicture())
        return verrmsg(ERR_PictureInitialization, "VHLibOptimal: InitPicture failed");

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
    if(!callbackGetPixels || !callbackSetPos)
        return verrmsg(2, "VHLibOptimal: Invalid callbacks / nullptr");

    // Check source
    if(!cfg.imageWidth || !cfg.imageHeight)
        return verrmsg(1, "VHLibOptimal: Invalid source image");

    // Initial parameters valid
    return vok;
}

/**
 * @brief Initialization: Initial Picture scan
 */
verr VHLibOptimal::InitPicture() {

    // Calculate Cells Matrix Geometry
    cmatrix.Setup(cfg.imageWidth, cfg.imageHeight, cfg.cellsize);

    if(loglevel)
        VHLibOptimalLogger::PicProps(*this, cmatrix);

    // Setup Original Bitfield ( callback )
    bitfieldOrig.Setup(cmatrix);

    // Initial Scan Field
    for(uint16_t celly=0; celly < cmatrix.CellsY(); celly++) {
        for(uint16_t cellx=0; cellx < cmatrix.CellsX(); cellx++) {
            if(IsCellFilled(cellx, celly, cfg.minColorVal)) {
                int idx = cmatrix.CellN(cellx,celly);
                bitfieldOrig.SetCell(idx);
            }
        }
    }

    // Dump CellsMatrix
    if(loglevel >= LOG_LEVEL_MAX) {
        VHLibOptimalLogger::DumpCellsHEX( *this, cmatrix, bitfieldOrig.arr(), "Original Bitfield HEX");
    }
    
    if(loglevel >= LOG_LEVEL_EXT) {
        VHLibOptimalLogger::DumpCellsTXT( *this, cmatrix, bitfieldOrig.arr(), "Original Bitfield TXT");
    }

    return vok;
}

/**
 * 
 */
bool VHLibOptimal::FindFigure() {

    // find entry point of figure
    int entry = bitfieldOrig.FindEntryCell(cmatrix);
    if(entry < 0) return false;

    // Clearing figure before processing
    bitfieldFig.Setup(cmatrix);

    int celln = entry;

    bool flagLoopCells = true;

    while(flagLoopCells) {

        bitfieldOrig.ClrCell  (celln);
        bitfieldFig.SetCell   (celln);

        // Find from prev
        int near = bitfieldOrig.FindNearest(cmatrix, celln);
        if(near != -1) {
            celln = near;
            continue; }

        // Find in full path
        near = bitfieldOrig.FindPath(cmatrix, bitfieldFig);
        if(near != -1) {
            celln = near;
            continue; }

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
        VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, bitfieldOrig.arr(), (char *)"Original");
    
    if(loglevel >= LOG_LEVEL_EXT)
        VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, bitfieldFig.arr(),  (char *)"Figure");

    // Структура параметров текущей фигуры
    VHOptimalFigure newfigure(bitfieldFig, cmatrix, cfg.spccnt);

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
bool VHLibOptimal::IsCellFilled(uint16_t cellx, uint16_t celly, uint8_t whitelevel) const {

    for(int l=0;l<cmatrix.CellSize();l++) {

        std::vector<uint8_t> arr = callbackGetPixels(
            cellx * cmatrix.CellSize(),
            celly * cmatrix.CellSize() + l,
            cmatrix.CellSize());

        if(CheckWhiteLevel(arr, whitelevel))
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
 * Revision         : 0.2
 * Content size     : 5867
 * Date / Time      : 20-07-2026 03:16:52
 * MD5              : d02e6f3c82e266ee159001250f65a4c6
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */