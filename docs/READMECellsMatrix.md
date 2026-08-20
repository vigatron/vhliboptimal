

## Hardware Support

| Platform | Series / Architecture | Status |
| :--- | :--- | :--- |
| **STM32** | F4, F7, H7 | Fully Supported |
| **ESP32** | Xtensa / RISC-V | Fully Supported |


## Configuration & Grid Tuning

The library supports both static (compile-time) and dynamic (runtime) grid configurations depending on your target platform.

### 1. Embedded Platforms (STM32 / ESP32)
For resource-constrained MCU targets, the grid parameters are hardcoded via compiler flags to achieve maximum optimization and zero-overhead memory allocation:

```cmake
# Example CMake target definitions for MCUs
target_compile_definitions(${PROJECT_NAME} PRIVATE
    VHLIB_OPTIMAL_GRID_FIXED=1
    VHLIB_OPTIMAL_GRID_LX=8
    VHLIB_OPTIMAL_GRID_LY=8
)
```
