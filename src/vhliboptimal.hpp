/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : Lightweight C++17 library for fast object detection,
 *                 counting, and bounding box extraction.
 * Revision      : 0.8.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.hpp
 * Content size  : 3183
 * Date / Time   : 30-07-2026 21:53:54
 * MD5           : 12d0c43f48d2764a9cee48ddd402fd2b
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhplatform.hpp"
#include "vhliboptimalstructs.hpp"
#include "vhliboptimalcallbacks.hpp"
#include "cfg/cfg.hpp"
#include "bitfield.hpp"
#include "vhliboptimalfig.hpp"
#include "vhliboptimalext.hpp"

namespace vhliboptimal {


constexpr int LOG_LEVEL_NONE  = 0;
constexpr int LOG_LEVEL_BASE  = 1;
constexpr int LOG_LEVEL_EXT   = 2;
constexpr int LOG_LEVEL_MAX   = 3;

// Константы устанавливается при сборке

static constexpr int VHOPTIMAL_GRID_X_LEVEL = 8;        // 256 pxls
static constexpr int VHOPTIMAL_GRID_Y_LEVEL = 8;        // 256 pxls
static constexpr int VHOPTIMAL_GRID_S_ORDER = 1;        //   2 pxls

static constexpr int VHOPTIMAL_OBJECTS_MAX  = 128;      // F1K*4;
static constexpr int VHOPTIMAL_SPANS_MAX    = F1K*4;    //


class VHLibOptimal {

    public:

        explicit VHLibOptimal();

        // Memory Layout Setup
        static size_t   CalcMemory();

        // Memory Layout Setup
        static verr     SetupMemory(uint8_t * ptr, size_t memsize);

        // Frame Setup
        verr Setup(
            const stConfig &            cfgparams,
            CallbackGetSrcPxls          funcGetPixels,
            CallbackBorder              funcBorder,
            CallbackContent             funcContent,
            CallbackBenchmark           funcBenchmark );

        verr                            Run(uint16_t srcimgid);

        const size_t                    GetObjectsCount     () const;

        const VHOptimalFigure &         GetObject           (int idx) const;

        const size_t                    GetSpansTotal       () const;

        const CellsMatrix &             GetCMatrix          () const;

        const size_t                    CellSize            () const;


        bool                            Border              (int objn) const;
        bool                            ContentH            (int objn) const;
        bool                            ContentV            (int objn) const;

        void                            SetSortMode         (uint8_t mode);

    private:

        const int                       ERR_InvalidParams = 1;
        const int                       ERR_PictureInitialization = 2;

        // Static Memory Segments
        uint8_t     *   _pMemBitFieldSrc;
        uint8_t     *   _pMemBitFieldDst;
        uint8_t     *   _pMemObjects;
        uint8_t     *   _pMemSpans;

        // Settings
        stConfig                        cfg;

        // Callback: Source Image Content / Get Pixels
        CallbackGetSrcPxls              callbackGetPixels   = nullptr;

        // Callback: Moving across object border
        CallbackBorder                  callbackBorder      = nullptr;

        // Callback: Moving across object content ( Left > Right / Up > Down )
        CallbackContent                 callbackContent     = nullptr;

        CallbackBenchmark               callbackBenchmark   = nullptr;

        // 2D Configuration
        CellsMatrix                     cmatrix;

        // Буффер для хранения строки изображения внешнего источника
        std::vector<uint8_t>            buffLine;

        // Битовое поле фрагментов
        BitField                        bitfieldSrc;

        // Битовое поле выбранного фрагмента
        BitField                        bitfieldDst;

        uint8_t                         sortMode;

        verr CheckCfgParams();

        verr InitialScanImage(uint16_t srcimgid);

        bool FindFigure();

        verr ConvertFigure();

        bool CheckWhiteLevel(const std::vector<uint8_t> & arr, uint8_t whitelevel) const;

        bool IsCellFilled(uint16_t srcimgid, uint16_t cellx, uint16_t celly, uint8_t whitelevel);

        bool IsSortEnabled();

};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimal.hpp
 * Revision         : 0.8.0-beta
 * Content size     : 3183
 * Date / Time      : 30-07-2026 21:53:54
 * MD5              : 12d0c43f48d2764a9cee48ddd402fd2b
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */