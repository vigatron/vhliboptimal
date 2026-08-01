#pragma once

#include "figures/figures.hpp"


namespace vhliboptimal {


extern void                                 VHLIB_OPTIMAL_IFACE_FrameReset        ();

extern VHOptimalFigure &                    VHLIB_OPTIMAL_IFACE_Object          (int objn);
extern bool                                 VHLIB_OPTIMAL_IFACE_AddObject       ();
extern bool                                 VHLIB_OPTIMAL_IFACE_RemoveObject    ();
extern int                                  VHLIB_OPTIMAL_IFACE_ObjectsCount    ();

extern const spanword                       VHLIB_OPTIMAL_IFACE_GetSpan         (int spanid);
extern verr                                 VHLIB_OPTIMAL_IFACE_AddSpan         (spanword word);
extern const int                            VHLIB_OPTIMAL_IFACE_GetSpansCount   ();

extern uint8_t *                            VHLIB_OPTIMAL_IFACE_BitFieldSrcPtr  ();
extern uint8_t *                            VHLIB_OPTIMAL_IFACE_BitFieldDstPtr  ();


};
