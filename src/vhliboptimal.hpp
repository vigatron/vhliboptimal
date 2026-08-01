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

#include "cfg/cfg.hpp"

#include "structs/vhliboptimalstructs.hpp"
#include "structs/vhliboptimalcallbacks.hpp"
#include "structs/vhliboptimalext.hpp"

#include "bitfield/bitfield.hpp"
#include "figures/figures.hpp"

#include "mem/memorylayout.hpp"


namespace vhliboptimal {


constexpr int LOG_LEVEL_NONE  = 0;
constexpr int LOG_LEVEL_BASE  = 1;
constexpr int LOG_LEVEL_EXT   = 2;
constexpr int LOG_LEVEL_MAX   = 3;


class VHLibOptimal {

    public:

        explicit VHLibOptimal();

        // Frame Setup
        verr Setup(
            const stConfig &            cfgparams,
            CallbackBorder              funcBorder,
            CallbackContent             funcContent,
            CallbackBenchmark           funcBenchmark );

        // bitfield_src should be already filled !
        verr                            Run();

        const size_t                    GetObjectsCount     () const;

        const VHOptimalFigure &         GetObject           (int idx) const;

        const size_t                    GetSpansTotal       () const;

        const CellsMatrix &             GetCMatrix          () const;

        const uint8_t                   CellSZLevel         () const noexcept { return cfg.levelcs; }

        const uint8_t                   CellSZ              () const noexcept { return 1 << cfg.levelcs; }

        bool                            Border              (int objn) const;
        bool                            ContentH            (int objn) const;
        bool                            ContentV            (int objn) const;

        BitField                    &   BitFieldSrc();

        inline uint8_t                  FilterLevel() const noexcept { return cfg.minColorVal; }

        // Forwarding Memory layout interface
        VHMemoryLayout              &   MemoryLayout();

        size_t  CalcMemory() { return memlay.CalcMemory(); }

        verr    SetupMemory(uint8_t * ptr, size_t memsize) {

            if(memlay.SetupMemory(ptr, memsize)) return verror(1);

            // Init Src & Dst BitFields
            bitfieldSrc.Setup(cmatrix, memlay.BitFieldSrcPtr(), memlay.BitFieldSrcSize());
            bitfieldDst.Setup(cmatrix, memlay.BitFieldDstPtr(), memlay.BitFieldDstSize());

            return vok;
        }

    private:

        const int                       ERR_InvalidParams = 1;
        const int                       ERR_PictureInitialization = 2;

        VHMemoryLayout                  memlay;

        // Settings
        stConfig                        cfg;

        // Callback: Moving across object border
        CallbackBorder                  callbackBorder      = nullptr;

        // Callback: Moving across object content ( Left > Right / Up > Down )
        CallbackContent                 callbackContent     = nullptr;

        // Callback: Benchmarks
        CallbackBenchmark               callbackBenchmark   = nullptr;

        // 2D Configuration
        CellsMatrix                     cmatrix;

        // Битовое поле фрагментов
        BitField                        bitfieldSrc;

        // Битовое поле выбранного фрагмента
        BitField                        bitfieldDst;

        verr CheckCfgParams();

        verr InitialScanImage(uint16_t srcimgid);

        bool FindFigure();

        verr ConvertFigure();

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