/* ======================================================================================
 * Library       : vhliboptimal
 * Description   : C++ library for shape contour detection and image outline recognition
 * Revision      : 0.7.4-beta
 * Source        : https://github.com/vigatron/vhliboptimal
 * Disclaimer    : Provided "AS IS", without warranty.
 * License       : MIT
 * File          : src/log/vhliboptimallog.cpp
 * Content size  : 6443
 * Date / Time   : 27-07-2026 18:19:01
 * MD5           : f7a6a76fe3f805f359b9e25e4edde99f
 * Notes         : MD5 = file content without header/footer
 * Encoding      : UTF-8
 * Author        : Viktor Glebov / V01G04A81
 * Copyright     : © 2006–2026 Viktor Glebov
 * ========================[ BEGIN FILE CONTENT ]====================================== */
#include <format>
#include <sstream>
#include <iomanip>

#include "vhliboptimal.hpp"
#include "vhliboptimallog.hpp"

using namespace vhliboptimal;

static const std::string strbit0 = "0";
static const std::string strbit1 = "1";
static const std::string strbitP = "*";


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
    msg += fmt(" buffer size (bytes) ", cmatrix.BitMaskSizeBytes());

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

            std::ostringstream oss;

            oss << std::setw(4)
                << std::setfill('0')
                << std::hex
                << i
                << ": ";

            std::string msg = oss.str();

            partout(msg);
        }

        std::ostringstream oss;
        oss << std::uppercase 
            << std::setw(2)
            << std::setfill('0')
            << std::hex
            << static_cast<int>(arr[i])
            << " ";

        std::string msg = oss.str();

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

    std::ostringstream oss;
    oss << ": cn=" << std::setw(4) << celln
        << ", cx=" << cellx
        << " cy=" << celly
        << ", x=" << sx
        << "  y=" << sy;
    std::string txt = oss.str();

    lineout(txt);
}

/**
 * 
 */
void VHLibOptimalLogger::DumpFigurePos(const VHOptimalFigure & objfig, int showfigidx) {

    const strect & rect = objfig.PosCells();

    partout(fmt("Figure #", showfigidx));
    partout(" Position: ");

    std::ostringstream oss;
    oss << "[" << rect.x1 << ":" << rect.y1 << "] - ["
        << rect.x2 << ":" << rect.y2 << "]\n";
    std::string msg = oss.str();

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
    {
        std::ostringstream oss;
        oss << "# " << std::setw(5) << std::left << spann << " ";
        partout(oss.str());
    }

    auto [cellx, celly] = cmx.CellXY(spn.n);
    {
        std::ostringstream oss;
        oss << " N:" << std::setw(5) << spn.n
            << " (cx:cy " << std::setw(4) << cellx
            << ":" << std::setw(4) << std::left << celly << std::right << ")"
            << "  L:" << std::setw(3) << spn.l << " , ";
        partout(oss.str());
    }

    int cs = cmx.CellSize();
    int x1 = cellx * cs;
    int y1 = celly * cs;
    int x2 = (cellx + spn.l) * cs - 1;
    int y2 = (celly + 1) * cs - 1;

    {
    std::ostringstream oss;
    oss << " sx:sy - ex:ey = "
        << std::setw(5) << x1 << ":"
        << std::setw(5) << y1 << ":"
        << std::setw(5) << x2 << ":"
        << std::setw(5) << y2;
        partout(oss.str());
    }

    newlout();
}

/**
 * 
 */
void VHLibOptimalLogger::DumpFigureSpans(
    const VHOptimalFigure & objfig,
    const CellsMatrix & cmx)
{

    std::ostringstream oss;
    oss << "Spans Count = " << objfig.SpansCount();
    std::string msg = oss.str();
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

        std::ostringstream oss;
        oss << "Figure #" << i;
        std::string msg = oss.str();

        lineout(msg);

        DumpFigurePos(objfig, i);
        DumpFigureSpans(objfig, objlib.GetCMatrix());
    }

}


/* ========================[  END FILE CONTENT  ]========================
 * Library          : vhliboptimal
 * File             : src/log/vhliboptimallog.cpp
 * Revision         : 0.7.4-beta
 * Content size     : 6443
 * Date / Time      : 27-07-2026 18:19:01
 * MD5              : f7a6a76fe3f805f359b9e25e4edde99f
 * Copyright        : © 2006–2026 Viktor Glebov
 * ====================================================================== */