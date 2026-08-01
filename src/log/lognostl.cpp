#include "lognostl.hpp"

using namespace vhliboptimal;


static const char * strbit0 = "0";
static const char * strbit1 = "1";
static const char * strbitP = "*";


// ================================================================

static void newlout() {
    printf("\n");
}

static void partout(const char *msg) {
    printf("%s", msg);
}

static void lineout(const char *msg) {
    printf("%s\n", msg);
}

static void append_param(const char * msg, int val) {
    printf("%s%d", msg, val);
}

static void print_param(const int val) {
    printf("%d", val);
}

static void print_param(const char * msg, int val, bool nl=true) {
    append_param(msg, val);
    if(nl) newlout();
}

// ================================================================

/**
 * 
 */
void log::newlout() {
    ::newlout();
}

/**
 * 
 */
void log::print_param(const char *msg, int val) {
    ::print_param(msg, val);
}

/**
 * 
 */
void log::partint(const int val) {
    ::print_param(val);
}

/**
 * 
 */
void log::partout(const char *msg) {
    ::partout(msg);
}

/**
 * 
 */
void log::lineout(const char *msg) {
    ::lineout(msg);
}

/**
 * @brief General Picture Information
 */
void log::PicProps(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix)
{
    int w = cmatrix.CellsX() * obj.CellSize();
    int h = cmatrix.CellsY() * obj.CellSize();

    partout("Picture Props:");
    append_param(" W=", w);
    append_param(" H=", h);

    partout("Cells Matrix: ");
    partout("<");
    append_param("cellsw=", cmatrix.CellsX());
    partout(" ");
    append_param("cellsh=", cmatrix.CellsY());
    partout(">");

    append_param(" total=", cmatrix.CellsT());
    append_param(" cellsize=", obj.CellSize());
    append_param(" buffer size (bytes) ", cmatrix.BitMaskSizeBytes());

    newlout();
}

/**
 * 
 */
void log::DumpCellsHEX(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix,
    const uint8_t * arrptr,
    const char *msg)
{

    int blocklen = cmatrix.BitMaskSizeBytes();

    if(msg) {
        partout("DumpCellsHEX: ");
        lineout(msg); }

    uint16_t bytescnt = cmatrix.BitMaskSizeBytes();
    
    append_param("Bitfields original ", bytescnt);
    lineout(" bytes");

    partout("(");
    append_param(  " ", cmatrix.CellsX());
    append_param(" x ", cmatrix.CellsY());
    partout(") ");
    append_param(  " ", cmatrix.CellsT());
    lineout(" bits");

    for(int i=0; i < blocklen; i++) {
        if(!(i%16)) { newlout(); printf("%4X: ", i); }
        printf("%.2X", arrptr[i]);
    }

    newlout();
}


void log::DumpCellsTXT(
    const VHLibOptimal & obj,
    const CellsMatrix & cmatrix,
    const uint8_t * arrptr,
    const char *msg,
    const int cellMarker) {

    if(msg) {
        partout("Dumping: ");
        lineout(msg); }

    for(uint16_t cy=0;cy<cmatrix.CellsY();cy++) {
        for(uint16_t cx=0; cx<cmatrix.CellsX(); cx++) {
            size_t celln = cmatrix.CellN(cx,cy);
            if(cellMarker != -1 && celln == cellMarker) {
                partout(strbitP);
            } else {
                bool bval = VHBits::BitVal(arrptr, celln);
                partout(bval ? strbit1:strbit0);
            }
        }
        newlout();
    }

}

/**
 * 
 */
void log::DumpSpan(
    const spanword spn,
    const CellsMatrix & cmtx,
    int cellsize,
    int spann)
{

    {
        int spanid = get_span_id  (spann);
        int spanwd = get_span_len (spann);
        printf("# %5d ", spanid);
    }

    int spanid = get_span_id(spn);
    int spanln = get_span_len(spn);
    auto [cellx, celly] = cmtx.CellXY(spanid);
    int x1 = cellx * cellsize;
    int y1 = celly * cellsize;
    int x2 = (cellx + spanln) * cellsize - 1;
    int y2 = (celly + 1) * cellsize - 1;

    {
        printf(" N: %5d ", spanid);
        printf(" (cx:cy %4d:%4d) L: %3d ", (int)cellx, (int)celly, spanln);
        printf(" sx:sy - ex:ey = %5d:%5d - %5d:%5d", x1, y1, x2, y2);
    }

    newlout();
}

/**
 * 
 */
void log::DumpFigureSpans(
    const VHOptimalFigure & objfig,
    const CellsMatrix & cmx,
    int cellsize)
{
    print_param("Spans Count = ", objfig.SpansCount());

    for(int i=0; i < objfig.SpansCount(); i++) {
        const spanword wspn = objfig.Span(i);
        DumpSpan(wspn, cmx, cellsize, i);
    }

}

