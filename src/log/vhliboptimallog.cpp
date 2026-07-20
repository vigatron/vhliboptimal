/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.3
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/log/vhliboptimallog.cpp
 * Content size  : 5399
 * Date / Time   : 20-07-2026 05:40:29
 * MD5           : 427d6094800ce9c8d4c2c4345ded7abb
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include <format>

#include "vhliboptimal.hpp"
#include "vhliboptimallog.hpp"

using namespace vhliboptimal;

constexpr std::string strbit0 = "0";
constexpr std::string strbit1 = "1";
constexpr std::string strbitP = "*";


void VHLibOptimalLogger::partout(const std::string & str) {
    std::cout << str;
}

void VHLibOptimalLogger::newlout() {
    std::cout << std::endl;
}

void VHLibOptimalLogger::lineout(const std::string & str) {
    std::cout << str;
    newlout();
}

std::string VHLibOptimalLogger::fmt(
    const std::string & strparam,
    int value) {
    std::string r = strparam + std::to_string(value);
    return r;
}

std::string VHLibOptimalLogger::fmt(
    const std::string & strparam1,
    int value,
    const std::string & strparam2) {
    std::string r = strparam1 + std::to_string(value) + strparam2;
    return r;
}

std::string VHLibOptimalLogger::fmt(
    int value1,
    const std::string & strparam,
    int value2) {
    std::string r = std::to_string(value1) + strparam + std::to_string(value2);
    return r;
}

/**
 * @brief General Picture Information
 */
void VHLibOptimalLogger::PicProps(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix)
{
    std::string msg;

    int w = cmatrix.CellsX() * cmatrix.CellSize();
    int h = cmatrix.CellsY() * cmatrix.CellSize();

    msg = "Picture Props:";
    msg += fmt("W=",w) + " " +fmt("H=",h);
    lineout(msg);

    msg = "Cells Matrix: ";

    msg += "<";
    msg += fmt("cellsw=", cmatrix.CellsX()) + " " + fmt("cellsh=", cmatrix.CellsY());
    msg += ">";

    msg += fmt(" total=", cmatrix.CellsT());
    msg += fmt(" cellsize=", cmatrix.CellSize());

    lineout(msg);
}

/**
 * 
 */
void VHLibOptimalLogger::DumpCellsHEX(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix,
    const std::vector<uint8_t> & arr,
    const char *msg )
{

    if(msg) lineout( "Dumping: " + std::string(msg));

    uint16_t bytescnt = (cmatrix.CellsT() / 8) + ((cmatrix.CellsT() % 8) ? 1:0);

    std::string str1 = fmt("Bitfields original ", bytescnt, " bytes");
    lineout(str1);

    std::string strx = fmt(cmatrix.CellsX(), " x ", cmatrix.CellsY());
    std::string str2 = "(" + strx + ") " + std::to_string(cmatrix.CellsT()) + " bits";
    lineout(str2);

    for(int i=0; i<arr.size();i++) {

        if(!(i%16)) {
            newlout();
            std::string msg = std::format("{:04X}: ", i);
            partout(msg);
        }

        std::string msg = std::format("{:02X} ", arr[i]);
        partout(msg);
    }

     newlout();
}

/**
 * 
 */
void VHLibOptimalLogger::DumpCellsTXT(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix,
    const std::vector<uint8_t> & arr,
    const char *msg,
    const int cellMarker
)
{
    if(msg) lineout("Dumping: " + std::string(msg));

    for(uint16_t cy=0;cy<cmatrix.CellsY();cy++) {
        for(uint16_t cx=0; cx<cmatrix.CellsX(); cx++) {
            size_t celln = cmatrix.CellN(cx,cy);
            if(cellMarker != -1 && celln == cellMarker) {
                partout(strbitP);
            } else {
                partout(VHBits::BitVal(arr, celln) ? strbit1:strbit0);
            }
        }
        newlout();
    }
}

/**
 * @brief Параметры ячейки
 */
void VHLibOptimalLogger::DumpCell(
    const std::string & msg,
    int celln, int cellx, int celly,
    int sx, int sy ) {

    partout(msg);
    std::string txt = std::format(
        ": cn={:4d}, cx={:d} cy={:d}, x={:d}  y={:d}",
        celln, cellx, celly, sx, sy);
    lineout(txt);
}

/**
 * 
 */
void VHLibOptimalLogger::DumpFigurePos(const VHOptimalFigure & objfig, int showfigidx) {

    const strect & rect = objfig.PosCells();

    partout(fmt("Figure #", showfigidx));
    partout(" Position: ");

    std::string msg = std::format("[{:d}:{:d}] - [{:d}:{:d}]\n", rect.x1, rect.y1, rect.x2, rect.y2);
    lineout(msg);
}

/**
 * 
 */
void VHLibOptimalLogger::DumpSpan(
    const stspan & spn,
    const CellsMatrix & cmx,
    int spann)
{
    std::string msgn = std::format("# {:<5d} ", spann );

    auto [cellx, celly] = cmx.CellXY(spn.n);
    std::string msgc = std::format(" N:{:>5d} (cx:cy {:>4d}:{:<4d})  L:{:>3d} , ",
        spn.n, cellx, celly, spn.l );

    int cs = cmx.CellSize();
    int x1 = cellx * cs;
    int y1 = celly * cs;
    int x2 = (cellx + spn.l + 1) * cs - 1;
    int y2 = (celly + 1) * cs - 1;
    std::string msgg = std::format(" sx:sy - ex:ey = {:>5d}:{:>5d}:{:>5d}:{:>5d}",
        x1, y1, x2, y2);

    std::string msg = msgn + msgc + msgg;
    lineout(msg);
}

/**
 * 
 */
void VHLibOptimalLogger::DumpFigureSpans(
    const VHOptimalFigure & objfig,
    const CellsMatrix & cmx)
{

    std::string msg = std::format("Spans Count = {:d}", objfig.SpansCount());
    lineout(msg);

    for(int i=0; i < objfig.SpansCount(); i++) {
        const stspan & objspn = objfig.Span(i);
        DumpSpan(objspn, cmx, i);
    }

}

/**
 * 
 */
void VHLibOptimalLogger::DumpFigures(const VHLibOptimal & objlib) {

    lineout(fmt("Figs  count: ", objlib.GetObjectsCount()));
    lineout(fmt("Total spans: ", objlib.GetSpansTotal()));

    for(int i=0; i < objlib.GetObjectsCount(); i++) {

        const VHOptimalFigure & objfig = objlib.GetObject(i);

        std::string msg = std::format("Figure #{:d}", i);
        lineout(msg);

        DumpFigurePos(objfig, i);
        DumpFigureSpans(objfig, objlib.GetCMatrix());
    }

}


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/log/vhliboptimallog.cpp
 * Revision         : 0.3
 * Content size     : 5399
 * Date / Time      : 20-07-2026 05:40:29
 * MD5              : 427d6094800ce9c8d4c2c4345ded7abb
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */