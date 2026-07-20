/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.4
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.hpp
 * Content size  : 2231
 * Date / Time   : 20-07-2026 06:32:56
 * MD5           : 8e1c83ac03b203572ae113ee1bac58b5
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#pragma once

#include "vhplatform.hpp"
#include "vhliboptimalstructs.hpp"
#include "bitfield.hpp"
#include "vhliboptimalfig.hpp"


namespace vhliboptimal {


constexpr int LOG_LEVEL_NONE  = 0;
constexpr int LOG_LEVEL_BASE  = 1;
constexpr int LOG_LEVEL_EXT   = 2;
constexpr int LOG_LEVEL_MAX   = 3;


class VHLibOptimal {

    public:

        explicit VHLibOptimal();

        verr Setup(
            const stConfig &            cfgparams,
            GetPixelsCallback           callbackGetPixelsHandler,
            SetPosCallback              callbackSetPosHandler );

        verr Run();

        const size_t                    GetObjectsCount     () const;

        const VHOptimalFigure &         GetObject           (int idx) const;

        const size_t                    GetSpansTotal       () const;

        void                            SetLogLevel         (int verbose);

        const CellsMatrix &             GetCMatrix          () const;

    private:

        const int                       ERR_InvalidParams = 1;
        const int                       ERR_PictureInitialization = 2;

        // Settings
        stConfig                        cfg;

        // Callback: LoadSourceContent Line / Cell Pixels
        GetPixelsCallback               callbackGetPixels    = nullptr;

        // Callback: Object Border
        SetPosCallback                  callbackSetPos       = nullptr;

        // 2D Configuration
        CellsMatrix                     cmatrix;

        // Битовое поле фрагментов
        BitField                        bitfieldOrig;

        // Битовое поле выбранного фрагмента
        BitField                        bitfieldFig;

        // Массив фигур
        std::vector<VHOptimalFigure>    arrFigures;

        // Режим отладки
        int                             loglevel = LOG_LEVEL_NONE;


        verr CheckCfgParams();

        verr InitPicture();

        bool FindFigure();

        bool ConvertFigure();

        bool CheckWhiteLevel(const std::vector<uint8_t> & arr, uint8_t whitelevel) const;

        bool IsCellFilled(uint16_t cellx, uint16_t celly, uint8_t whitelevel) const;

};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimal.hpp
 * Revision         : 0.4
 * Content size     : 2231
 * Date / Time      : 20-07-2026 06:32:56
 * MD5              : 8e1c83ac03b203572ae113ee1bac58b5
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */