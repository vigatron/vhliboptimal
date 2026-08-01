#include "structs/vhliboptimalstructs.hpp"
#include "log/log.hpp"

void vhliboptimal::asrts(bool cond, int module, const char * msg) {
    if(cond) return;
    log::partout("ASSERT: ");
    log::partout("Module: ");
    log::partint(module);
    log::partout(" Msg: ");
    log::partout(msg);
    log::lineout("");

#if VHLIB_OPTIMAL_EMBEDDED==1
    while(1) { asm("nop"); }
#else
    exit(1);
#endif
}
