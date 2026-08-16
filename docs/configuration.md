## VHLIBOPTIMAL Configuration

### Target Configuration

`VHLIB_OPTIMAL_PLATFORM_PC`

`VHLIB_OPTIMAL_PLATFORM_EMBEDDED`

sds3#dfdfdfdfdfdfdf

#### Before the startup procedure, image source parameters and settings are specified

- image width   pixels      For example 800 / 1024 / 1600 / custom
- image height  pixels      For example 600 /  768 / 1200 / custom
- cells size    pixels      For example 1 / 2 / 4 / 8 / 16 / custom

##### Additionally:
* Maximum number of figures                 (for example 128)
* Maximum number of grid particles          (for example 1024)

Or:
* Available memory size in kilobytes

#### Structure sizes

* `stspan`                     8 bytes
* `VHOptimalFigure`           16 bytes

<br>

#### Configuration Examples

| Parameter                         | Example #1   | Example #2    | Example #3    | Example #4      |
|-----------------------------------|--------------|---------------|---------------|-----------------|
| **Maximum number of figures**     | 128          | 128           | 128           | 128             |
| **Cell size**                     | 8 px         | 4 px          | 2 px          | 1 px            |
| **Resolution**                    | 800×600      | 800×600       | 800×600       | 800×600         |
| **Cells per frame**               | 100×75       | 200×150       | 400×300       | 800×600         |
| **Bitmask (bits)**                | 7500         | 30000         | 120000        | 480000          |
| **Bitmask (bytes)**               | 938          | 3750          | 15000         | 60000           |
| **×2 bitmasks (Global + Local)**  | 1876         | 7500          | 30000         | 120000          |
| **Memory (Worst case)**           |  60000 bytes |  240000 bytes | 960000 bytes  | 3840000 bytes   |
| **Memory (Typical)**              |  16000 bytes |   64000 bytes | 240000 bytes  | 960000 bytes    |
| **+128 figures × 16**             |   2048 bytes |    2048 bytes | 2048 bytes    | 2048 bytes      |

<br>

---

### Key Data Structures (`src/vhliboptimalstructs.hpp`)
*   `stConfig` — Scanning parameters (image boundaries, grid cell size, color thresholding tolerance `minColorVal`, and `spccnt` which defines the maximum consecutive empty cells allowed before breaking a span).
*   `strect` — Bounding box structure representation (`x1, y1` to `x2, y2`).
*   `stspan` — Horizontal/vertical segments representing the continuous boundaries of a shape.

<br>


---
#### Compiler Tags

* VHLIBOPTIMAL_MCU32    - Build for PC / 32-bit version
* VHLIBOPTIMAL_MCU64    - Build for PC / 64-bit version
* VHLIBOPTIMAL_OPI      - Orange Pi
* VHLIBOPTIMAL_RPI      - RaspberryPi
* VHLIBOPTIMAL_STM32    - STM32 F4/F7/H7
* VHLIBOPTIMAL_ESP32    - ESP32 Xtensa

### FIXED GRID CONFIGURATION

```CMake
target_compile_definitions(vhlib_optimal PUBLIC
    VHLIB_OPTIMAL_GRID_FIXED
    VHLIB_OPTIMAL_GRID_LX=8
    VHLIB_OPTIMAL_GRID_LY=8
    VHLIB_OPTIMAL_OBJS_MAX=256
    VHLIB_OPTIMAL_SPNS_MAX=4096
)
```
