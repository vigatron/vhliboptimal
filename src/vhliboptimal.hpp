/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.0-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/vhliboptimal.hpp
 * Content size  : 2807
 * Date / Time   : 24-07-2026 11:54:48
 * MD5           : 3955c4aced59c3512851bd2ce73e0553
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
            CallbackGetSrcPxls          funcGetPixels,
            CallbackBorder              funcBorder,
            CallbackContent             funcContent
        );

        verr Run(uint16_t srcimgid);

        const size_t                    GetObjectsCount     () const;

        const VHOptimalFigure &         GetObject           (int idx) const;

        const size_t                    GetSpansTotal       () const;

        const CellsMatrix &             GetCMatrix          () const;

        bool                            Border              (int objn) const;
        bool                            ContentH            (int objn) const;
        bool                            ContentV            (int objn) const;

    private:

        const int                       ERR_InvalidParams = 1;
        const int                       ERR_PictureInitialization = 2;

        // Settings
        stConfig                        cfg;

        // Callback: Source Image Content / Get Pixels
        CallbackGetSrcPxls              callbackGetPixels   = nullptr;

        // Callback: Moving across object border
        CallbackBorder                  callbackBorder      = nullptr;

        // Callback: Moving across object content ( Left > Right / Up > Down )
        CallbackContent                 callbackContent   = nullptr;

        // 2D Configuration
        CellsMatrix                     cmatrix;

        // Буффер для хранения строки изображения внешнего источника
        std::vector<uint8_t>            buffLine;

        // Битовое поле фрагментов
        BitField                        bitfieldSrc;
        std::vector<uint8_t>            buffArrSrc;

        // Битовое поле выбранного фрагмента
        BitField                        bitfieldDst;
        std::vector<uint8_t>            buffArrDst;

        // Массив фигур
        std::vector<VHOptimalFigure>    arrFigures;

        verr CheckCfgParams();

        verr InitialScanImage(uint16_t srcimgid);

        bool FindFigure();

        bool ConvertFigure();

        bool CheckWhiteLevel(const std::vector<uint8_t> & arr, uint8_t whitelevel) const;

        bool IsCellFilled(uint16_t srcimgid, uint16_t cellx, uint16_t celly, uint8_t whitelevel);

};

};

/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/vhliboptimal.hpp
 * Revision         : 0.7.0-beta
 * Content size     : 2807
 * Date / Time      : 24-07-2026 11:54:48
 * MD5              : 3955c4aced59c3512851bd2ce73e0553
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */