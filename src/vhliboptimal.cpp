/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.1 Draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.cpp
 * Content size  : 5675
 * Date / Time   : 16-07-2026 02:43:20
 * MD5           : 8139293ac830d87f9226cd451c487bb1
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include "vhliboptimal.hpp"
#include "vhliboptimallog.hpp"

VHLibOptimal::VHLibOptimal() { 

}


/**
 * @brief Start process
 */
verr VHLibOptimal::Start(
    const stConfig      &   cfgparams,
    GetPixelsCallback       callbackGetPixelsHandler,
    SetPosCallback          callbackSetPosHandler
) {

    // Check callbacks
    if(!callbackGetPixelsHandler || !callbackSetPosHandler)
        return verrmsg(2, "VHLibOptimal: Invalid callbacks / nullptr");

    // Check source
    if(!cfgparams.imageWidth || !cfgparams.imageHeight)
        return verrmsg(1, "VHLibOptimal: Invalid source image");

    // Setup callbacks
    callbackGetPixels     = callbackGetPixelsHandler;
    callbackSetPos        = callbackSetPosHandler;

    // Save initial parameters
    std::memcpy(&cfg, &cfgparams, sizeof(stConfig));

    // Calculate Cells Matrix Geometry
    cmatrix.Setup(cfg.imageWidth, cfg.imageHeight, cfg.cellsize);
    VHLibOptimalLogger::PicProps(*this, cmatrix);

    // Setup Original Bitfield ( callback )
    bitfieldOrig.Setup(cmatrix);

    // Initial Scan Field
    for(uint16_t celly=0; celly < cmatrix.CellsY(); celly++) {
        for(uint16_t cellx=0; cellx < cmatrix.CellsX(); cellx++) {
            if(IsCellFilled(cellx, celly, cfg.minColorVal)) {
                int idx = cmatrix.CellN(cellx,celly);
                bitfieldOrig.Set(idx);
            }
        }
    }

    // Dump CellsMatrix
    if(loglevel) {
        VHLibOptimalLogger::DumpCellsHEX( *this, cmatrix, bitfieldOrig.arr(), "Original Bitfield HEX");
        VHLibOptimalLogger::DumpCellsTXT( *this, cmatrix, bitfieldOrig.arr(), "Original Bitfield TXT");
        VHLibOptimalLogger::DumpOPATH(*this);
    }

    bool flagLoopFigures = true;

    while(flagLoopFigures) {

        // find entry point of figure
        int entry = bitfieldOrig.FindEntryCell(cmatrix);
        if(entry < 0)
            break;

        // Clearing figure before processing
        bitfieldFig.Setup(cmatrix);

        int celln = entry;

        bool flagLoopCells = true;

        while(flagLoopCells) {

            bitfieldOrig.Clr  (celln);
            bitfieldFig.Set   (celln);

            // Find from prev
            int near = bitfieldOrig.FindNearest(cmatrix, celln);
            if(near != -1) {
                celln = near;
                continue;
            }

            // Find in full path
            near = bitfieldOrig.FindPath(cmatrix, bitfieldFig);
            if(near != -1) {
                celln = near;
                continue;
            }

            if(loglevel) {
                std::string msg = "Figure #" + std::to_string(arrFigures.size()) + " found";
                VHLibOptimalLogger::lineout(msg);
            }

            if(loglevel >= LOG_LEVEL_EXT)
                VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, bitfieldOrig.arr(), (char *)"Original", celln);
            
            if(loglevel >= LOG_LEVEL_BASE)
                VHLibOptimalLogger::DumpCellsTXT(*this, cmatrix, bitfieldFig.arr(),  (char *)"Figure"  , celln);

            // Структура параметров текущей фигуры
            VHOptimalFigure newfigure(bitfieldFig, cmatrix, cfg.spccnt);
            arrFigures.push_back(newfigure);

            // step-out, processing next figure ...
            flagLoopCells = false;
        }

    }

    // Sort In Descent order
    SortAllFigures();

    return vok;
}

/**
 * 
 */
void VHLibOptimal::SetLogLevel(int verbose) {
    loglevel = verbose;
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
bool VHLibOptimal::IsCellFilled(uint16_t cellx, uint16_t celly, uint8_t whitelevel) {

    for(uint16_t l=0;l<cmatrix.CellSize();l++) {

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
const uint16_t VHLibOptimal::GetObjectsCount() const {
    return arrFigures.size();
}

/**
 * @brief Количество участков
 * 
 * @return общее количество всех фигур
*/
const uint16_t VHLibOptimal::GetSpansTotal() const {

    int r = 0;

    for(int i=0;i<arrFigures.size();i++) {
        r += arrFigures[i].SpansCount();
    }

    return r;
}

/**
 * @brief Упорядочивание фрагментов последовательно
 */
void VHLibOptimal::SortAllFigures() {

    if(loglevel)
        VHLibOptimalLogger::DumpOPATH(*this);

    // Для каждой фигуры - сортировка участков
    for(int i=0;i < arrFigures.size();i++) {
        if(loglevel) {
            std::string msg = "Fig #" + std::to_string(i) + ", Sorting by dist";
            VHLibOptimalLogger::lineout(msg);
        }
        arrFigures[i].SortFigureParts();
    }

    if(loglevel)
        VHLibOptimalLogger::DumpOPATH(*this);

    // Для каждой фигуры - водопадная последовательная сортировка
    for(int i=0;i < arrFigures.size();i++) {
        if(loglevel) {
            std::string msg = "Fig #" + std::to_string(i) + ", Waterfall sort";
            VHLibOptimalLogger::lineout(msg);
        }
        arrFigures[i].SortWaterfall();
    }

    if(loglevel)
        VHLibOptimalLogger::DumpOPATH(*this);

}

/* ========================[  END FILE CONTENT  ]========================
 * File             : src/vhliboptimal.cpp
 * Content size     : 5675
 * Date / Time      : 16-07-2026 02:43:20
 * MD5              : 8139293ac830d87f9226cd451c487bb1
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */