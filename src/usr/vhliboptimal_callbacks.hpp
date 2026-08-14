#pragma once

#include <cstdint>

namespace vhliboptimal {

/**
 * 
 */
void CALLBACK_VHLIBOPTIMAL_Border(
    void *userData,
    uint8_t cmd,
    uint8_t dirh, 
    uint8_t dirv,
    uint16_t cellx,
    uint16_t celly, 
    uint16_t imgx,
    uint16_t imgy
);

/**
 * 
 */
void CALLBACK_VHLIBOPTIMAL_Content(
    void *userData,
    uint32_t cell1,
    uint32_t cell2,
    uint8_t dir
);


/**
 * 
 */
void CALLBACK_VHLIBOPTIMAL_Benchmark(
    void *userData,
    int cmd,
    int param
);


};