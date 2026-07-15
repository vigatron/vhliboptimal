/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.1 Draft
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.hpp
 * Content size  : 1736
 * Date / Time   : 16-07-2026 02:43:20
 * MD5           : b541740a04554d814d3aedbdab73ce48
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



class VHLibOptimal {

    public:

        explicit VHLibOptimal();

        verr Start(
            const stConfig &        cfgparams,
            GetPixelsCallback       callbackGetPixelsHandler,
            SetPosCallback          callbackSetPosHandler );

        const uint16_t              GetObjectsCount     () const;

        const uint16_t              GetSpansTotal       () const;

        void                        SetLogLevel(int verbose);

    private:

        const int                       LOG_LEVEL_NONE  = 0;
        const int                       LOG_LEVEL_BASE  = 1;
        const int                       LOG_LEVEL_EXT   = 2;

        // Settings
        stConfig                        cfg;

        // Callbacks
        GetPixelsCallback               callbackGetPixels    = nullptr;
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

        bool CheckWhiteLevel(const std::vector<uint8_t> & arr, uint8_t whitelevel) const;

        bool IsCellFilled(uint16_t cellx, uint16_t celly, uint8_t whitelevel);

        void SortAllFigures();

};
/* ========================[  END FILE CONTENT  ]========================
 * File             : src/vhliboptimal.hpp
 * Content size     : 1736
 * Date / Time      : 16-07-2026 02:43:20
 * MD5              : b541740a04554d814d3aedbdab73ce48
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */