#pragma once

#ifndef VHPLATFORM_INCLUDED

#include <cstdint>
#include <cstddef>

#ifndef verror

using verr = uint32_t;
constexpr verr vok = 0;
constexpr verr verror(uint32_t code) noexcept { return code; }

constexpr size_t F1K = 1024;
constexpr size_t F1M = 1024 * 1024;

verr verrmsg(int x, const char * msg);

namespace vhliboptimal {
void asrts(bool cond, int module, const char * msg);
};

#endif


#ifndef VHBits
#include "bits/vhbitsmini.hpp"
#endif

#endif
