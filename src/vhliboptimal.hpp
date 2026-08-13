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

#include "bitfield/bitfield.hpp"
#include "figure/figure.hpp"

#include "mem/memorylayout.hpp"


namespace vhliboptimal {


constexpr int LOG_LEVEL_NONE  = 0;
constexpr int LOG_LEVEL_BASE  = 1;
constexpr int LOG_LEVEL_EXT   = 2;
constexpr int LOG_LEVEL_MAX   = 3;


class VHLibOptimal {

    public:

        explicit VHLibOptimal();

        // 
        verr Setup(
            const stConfig & cfgparams,
            const VHMemoryLayout::stMemLayout & sMemlayout,
            CallbackBorder funcBorder,
            CallbackContent funcContent,
            CallbackBenchmark funcBenchmark );

        /**
         * bitfield_src should be already filled !
         */
        verr Run();

        /**
         *
         */
        void FrameReset() {
            _objCount = 0;
            _spnCount = 0;
        }

        /* ************** GLOBAL OBJECTS RELATED **************** */

        /** 
         * @brief Количество фигур
         * 
         * @return общее количество
         */
        uint16_t ObjectsCount() const noexcept { 
            return _objCount;
        }

        /** 
         * @brief Объект фигуры по индексу
         */        
        VHOptimalFigure & Object(uint16_t pos) {
            asrts(pos < ObjectsCount(), 0, "VHLibOptimal::GetObject out of range");
            return memlay.Obj(pos);
        }

        /**
         *
         */
        const VHOptimalFigure & Object(uint16_t pos) const {
            asrts(pos < ObjectsCount(), 0, "VHLibOptimal::GetObject out of range");
            return memlay.Obj(pos);
        }

        /**
         *
         */
        bool AddObject() {
            if(_objCount>=VHOPTIMAL_OBJECTS_MAX) return false;
            _objCount++;
            return true;
        }

        /**
         *
         */
        bool RemoveObject() {
            if(!_objCount) return false;
            _objCount--;
            return true;
        }

        /* *************** GLOBAL SPANS RELATED ***************** */

        /**
         *
         */
        const uint32_t  GlobalSpansCount        () const noexcept { return _spnCount; }

        // Calculating thru objects
        const size_t    CalcSpansTotal          () const;

        /**
         *
         */
        const spanword GetGlobalSpan(uint32_t pos) const {
            if(pos >= VHOPTIMAL_SPANS_MAX) return 0;
            return memlay.Spn(pos);
        }

        // TODO: direct spans count / check

        const CellsMatrix &             GetCMatrix          () const;
        bool                            Border              (int objn) const;
        bool                            ContentH            (int objn) const;
        bool                            ContentV            (int objn) const;

        BitField                    &   BitFieldSrc() noexcept { return bitfieldSrc; }

        // Forwarding Memory layout interface
        VHMemoryLayout              &   MemoryLayout() noexcept { return memlay; }

        /**
         * 
         */
        void BMPParserReset() {
            bmpParseStage = 0;
            bmpParsePos = 0;
            bmpLineY = 0;
        }

        /**
         * 
         */
        verr BMPParserByte(uint8_t v, uint8_t lvscale) {

            verr r;

            switch(bmpParseStage) {

                case eBMPParserFileHeader:
                    r = BMPParserFileHeader(v);
                    break;

                case eBMPParserInfoHeader:
                    r = BMPParserInfoHeader(v);
                    break;

                case eBMPParserPalette:
                    r = BMPParserPalette(v);
                    break;

                case eBMPParserData:
                    r = BMPParserData(v, lvscale);
                    break;

                default: {
                    r = verror(101);
                } break;
            }

            if(r) {
                BMPParserReset();
            }

            return r;
        }

        /**
         * 
         */
        void DumpBitfield(bool hexmode=false);

    private:

        static constexpr int            ERR_InvalidParams = 1;
        static constexpr int            ERR_PictureInitialization = 2;

        static constexpr uint8_t        DEF_CELL_SIZE = 1;

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

        // Runtime array
        VHLocalSpansArray               arrRntSpans;

        verr CheckCfgParams();

        verr InitialScanImage(uint16_t srcimgid);

        bool FindFigure();

        verr ConvertFigure();

        bool IsSortEnabled();

        // ==== IFACE ====
        // Массив фигур
        // Содержит в себе массив участков:
        // spanlen [31 .. 21] + spanid  [20 ..  0]
        // std::vector<uint32_t> arrSpans;

        uint16_t _objCount;
        uint32_t _spnCount;

        verr SetupMemory(const VHMemoryLayout::stMemLayout & sMemlayout) {

            if(memlay.SetupMemory(sMemlayout)) return verror(1);

            // Init Src & Dst BitFields
            bitfieldSrc.Setup(cmatrix, memlay.BitFieldSrcPtr(), memlay.BitFieldSrcSize());
            bitfieldDst.Setup(cmatrix, memlay.BitFieldDstPtr(), memlay.BitFieldDstSize());

            return vok;
        }

        // BMP Parser
        enum enBMPParserPhase {
            eBMPParserFileHeader = 0,
            eBMPParserInfoHeader,
            eBMPParserPalette,
            eBMPParserData
        };

        uint8_t  bmpParseStage      = 0;
        uint16_t bmpParsePos        = 0;
        uint16_t bmpLineY           = 0;
        uint16_t bmpBytesPerLine    = 0;

        BMPFileHeader   sBMPFileHDR;
        BMPInfoHeader   sBMPInfoHDR;

        /**
         * 
         */
        verr BMPParserFileHeader(uint8_t v) {

            if(!bmpParsePos) {
                if(v!='B')
                    return verror(1);
            }
            else if(bmpParsePos == 1) {
                if(v!='M')
                    return verror(2);
            }
            
            ((uint8_t *) &sBMPFileHDR)[bmpParsePos++] = v;
            
            if(bmpParsePos < sizeof(BMPFileHeader)) {
                return vok;
            }

            bmpParseStage++;
            bmpParsePos = 0;

            return vok;
        }

        /**
         * 
         */
        verr BMPParserInfoHeader(uint8_t v) {

            ((uint8_t *) &sBMPInfoHDR)[bmpParsePos++] = v;

            if(bmpParsePos < sizeof(BMPInfoHeader)) {
                return vok;
            } else {
                if(!validate_bmp())
                    return verror(1);
            }

            bmpParseStage++;
            bmpParsePos = 0;

            return vok;
        }

        /**
         * palette ignored in B&W mode
         */
        verr BMPParserPalette   (uint8_t v) {
            size_t sz = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
            bmpParsePos++;
            while( (sz + bmpParsePos) < (sBMPFileHDR.offset_data)) {
                return vok;
            }

            bmpParseStage++;
            bmpParsePos = 0;

            return vok;
        }

        /**
         * 
         */
        verr BMPParserData(uint8_t v, uint8_t lvscale) {

            const BMPInfoHeader & hdr = sBMPInfoHDR;

            // Vertical UP/DU mode
            bool flagup = hdr.height < 0;
            uint16_t h = flagup ? (hdr.height  * -1) : hdr.height;

            uint16_t posx = bmpParsePos << 3;
            uint16_t posy = flagup ? bmpLineY : (h - bmpLineY -1);

            // Check X-Y range
            if(bmpLineY >= h)
                return verror(2);

            if(posx < hdr.width) {

                if(v & 0x80) setBitSrcBitfield(posx+0, posy, lvscale);
                if(v & 0x40) setBitSrcBitfield(posx+1, posy, lvscale);
                if(v & 0x20) setBitSrcBitfield(posx+2, posy, lvscale);
                if(v & 0x10) setBitSrcBitfield(posx+3, posy, lvscale);

                if(v & 0x08) setBitSrcBitfield(posx+4, posy, lvscale);
                if(v & 0x04) setBitSrcBitfield(posx+5, posy, lvscale);
                if(v & 0x02) setBitSrcBitfield(posx+6, posy, lvscale);
                if(v & 0x01) setBitSrcBitfield(posx+7, posy, lvscale);

            }

            bmpParsePos++;
            if(bmpParsePos >= bmpBytesPerLine) {
                bmpParsePos = 0;
                bmpLineY++;
                if(bmpLineY >= h) BMPParserReset();
            }

            return vok;
        }

        /**
         * 
         */
        bool validate_bmp() {

            BMPInfoHeader & hdr = sBMPInfoHDR;
            bool b1 = hdr.planes == 1;
            bool b2 = hdr.bit_count == 1;
            bool b3 = hdr.compression == 0;
            bool b4 = hdr.colors_used == 2;
            bool b = b1 && b2 && b3 && b4;
            if(!b) return false;

            uint8_t align = sizeof(uint32_t);
            bmpBytesPerLine  = hdr.width / CHAR_BIT;
            uint8_t delta = bmpBytesPerLine % align;
            if(delta) {
                bmpBytesPerLine &= ~(align - 1);
                bmpBytesPerLine += align;
            }

            // TODO: Check limits X-Y
            return true;
        }

        /**
         * 
         */
        void setBitSrcBitfield(uint16_t bmpx, uint16_t bmpy, uint8_t lvscale) {

            const CellsMatrix & cmtx = GetCMatrix();

            // Scaller
            uint16_t cx = bmpx >> lvscale;
            uint16_t cy = bmpy >> lvscale;

            if(cx >= cmtx.CellsX()) return;
            if(cy >= cmtx.CellsY()) return;

            // Set bit
            bitfieldSrc.SetCell(GetCMatrix(), cx, cy);

        }
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