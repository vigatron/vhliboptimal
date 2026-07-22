# vhliboptimal

![Language](https://img.shields.io/badge/Language-C%2B%2B23-blue.svg)
![CMake](https://img.shields.io/badge/Build-CMake-1f4f9c.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Cross--platform-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Version](https://img.shields.io/badge/Version-0.5-orange.svg)

---

| Header       | Description                       |
|--------------|-----------------------------------|
| Project      | VHLibOptimal                      |
| Description  | C++ library for shape contour detection and image outline recognition |
| Revision     | 0.5                               |
| Date         | 2012 - 2026                       |
| Author       | V01G04A81 / Viktor Glebov         |
| License      | MIT                               |
| Source code  | [https://github.com/vigatron/vhliboptimal](https://github.com/vigatron/vhliboptimal) |

![img](docs/stereocam.jpg)


*A high-performance C++23 library for fast shape contour detection and image outline recognition using optimized grid-based scanning.*


---

## Project Overview


`vhliboptimal` is a high-performance C++23 library for fast shape contour detection and image outline recognition.  

Originally developed in plain C (2012–2017) for commercial embedded projects on ARM and AVR platforms.  

It has been completely modernized in 2026 with a clean object-oriented C++23 interface while preserving its efficiency-focused philosophy.  

It uses an optimized grid-based approach: the image is divided into a configurable **Cells Matrix**, and connectivity is tracked using compact **BitFields**.  

This design delivers excellent performance with very low memory and CPU usage, making it ideal for embedded systems and real-time applications.  

Unlike heavyweight computer vision libraries such as OpenCV, `vhliboptimal` is lightweight, highly portable, and works through simple callback interfaces.  

It excels at processing binary or high-contrast images and gracefully handles small gaps and noise thanks to tunable parameters.  




## Key Features

* Zero heavy external dependencies
* Three flexible C-style callbacks for maximum integration flexibility
* Support for multiple image sources via `srcimgid`
* Highly optimized grid-based scanning with bit-packing
* Configurable cell size and noise tolerance
* Real-time contour and content processing via callbacks

---

### History & Evolution

The `vhliboptimal` library has deep roots in real-world embedded computer vision and a long history of practical use under severe resource constraints.


#### 2012–2017 — Legacy Era (Plain C + FPGA)
The original algorithm was developed in plain C as a lightweight, header-only library. It was successfully deployed in several commercial products running on highly constrained platforms, including **ARM 32-bit** and **AVR 8-bit** microcontrollers (ATMega, ESP32, STM32).

In 2016 the algorithm was integrated into a **stereo camera** system based on a Xilinx Spartan-6 FPGA + SDRAM (166 MHz) paired with an STM32F7 microcontroller (200 MHz).  

This was an extremely optimized hybrid solution:
- Performance-critical parts were implemented in **Verilog** directly on the FPGA.
- Control logic and remaining processing ran on the STM32 in plain C.
- The FPGA and MCU operated in tight parallel cooperation.

Thanks to heavy bit-packing, grid-based scanning, and minimal memory usage, the library demonstrated reliable real-time performance even on 8-bit MCUs with only a few kilobytes of RAM. It proved particularly effective for industrial applications such as robotics, quality control, and automated inspection lines, where full-featured solutions like OpenCV were too heavy or slow.

#### 2026 — Modern C++ Rewrite
The library has been completely redesigned and rewritten from the ground up in modern **C++23**.  

This major update brings:
- A clean object-oriented interface
- Three flexible C-style callbacks
- Better code organization and improved safety
- Full CMake build system support

Remarkably, on modern processors — even with standard compiler optimizations and AVX2 support — the new software implementation achieves **performance comparable to the 2016 FPGA-based version**.

This evolution reflects the author’s 25+ years of experience in embedded systems, FPGA development, and low-level optimization. The core philosophy — **efficient contour detection without heavy dependencies** — remains unchanged, but the implementation is now much more modern, portable, and developer-friendly.

---


## 🛠 Technical Specifications

- **Language**: C++23 (with backward compatibility considerations for embedded toolchains)
- **Platform**: Primarily developed and tested on Linux. The codebase is OS-abstracted through `vhlibplatform`, making it easily portable to Windows, macOS, and bare-metal embedded targets (ARM, AVR, RISC-V, etc.).
- **License**: MIT
- **Author**: Viktor Glebov (V01G04A81)
- **Build System**: CMake 3.16+


### Dependencies
- [`vhlibplatform`](https://github.com/vigatron/vhlibplatform) — a lightweight cross-platform layer providing base primitive types, assertions, error handling, and system abstractions.


### Limitations
- Best suited for binary or high-contrast images
- Currently single-threaded (multi-threading support planned)
- Requires C++23 compliant compiler (or C++20 with some adjustments)

---

##  Architecture & Key Components

The library operates completely abstracted from raw graphic decoders or UI frameworks (like OpenCV or `stb_image`). It processes data streams through an abstract coordinate grid:

*   **`CellsMatrix`**: Manages the spatial geometry of the grid. Images are analyzed in configurable blocks (`cellsize`), decreasing overall data dimensionality.
*   **`BitField`**: A packed bit array tracking filled/empty cells. It allows instantaneous pathfinding operations and cell clearing during figure extraction loops.
*   **`VHOptimalFigure`**: Encapsulates a single extracted shape, containing its bounding box, sorted sequential contours, and analytical span strings.

### Key Data Structures (`src/vhliboptimalstructs.hpp`)
*   `stConfig` — Scanning parameters (image boundaries, grid cell size, color thresholding tolerance `minColorVal`, and max consecutive empty spaces `spccnt`).
*   `strect` — Bounding box structure representation (`x1, y1` to `x2, y2`).
*   `stspan` — Horizontal/vertical segments representing the continuous boundaries of a shape.

---

## Quick Start & Integration Example

### Callbacks Architecture

The library is completely decoupled from image sources and result processing.

**1. CallbackGetSrcPxls — Fetch source image pixels**

```cpp
/**
 * CallbackGetSrcPxls - Read a horizontal line of pixels from source image
 * 
 * @param userData   User context pointer (passed through from Setup)
 * @param dstptr     Destination buffer to fill
 * @param bytescnt   Number of bytes to read
 * @param srcid      Source image ID
 * @param srcx       Starting X coordinate
 * @param srcy       Starting Y coordinate
 */
typedef void (*CallbackGetSrcPxls)(void *userData, uint8_t *dstptr, 
                                   uint16_t bytescnt, uint16_t srcid, 
                                   uint16_t srcx, uint16_t srcy);
```

**2. CallbackBorder — Process figure border (contour)**

```cpp
/**
 * CallbackBorder - Called during border tracing of a detected shape
 * 
 * @param userData   User context pointer
 * @param cmd        Command: cmdStart / cmdMove / cmdStop
 * @param dirh       Horizontal direction
 * @param dirv       Vertical direction
 * @param cellx      Cell X coordinate
 * @param celly      Cell Y coordinate
 * @param imgx       Image X coordinate (pixels)
 * @param imgy       Image Y coordinate (pixels)
 */
typedef void (*CallbackBorder)(void *userData, uint8_t cmd, uint8_t dirh, 
                               uint8_t dirv, uint32_t cellx, uint32_t celly, 
                               uint16_t imgx, uint16_t imgy);
```

**3. CallbackContent — Process horizontal spans inside the figure**

```cpp
/**
 * CallbackContent - Called for each horizontal span inside the object
 * 
 * @param userData   User context pointer
 * @param cxl        Left cell index
 * @param cxr        Right cell index
 */
typedef void (*CallbackContent)(void *userData, uint32_t cxl, uint32_t cxr);
```

### Basic Usage Example

```cpp
#include <iostream>
#include "vhliboptimal.hpp"

namespace vhliboptimal {

// Callback to fetch pixels from your framebuffer/camera
void MyGetPixels(void* userData, uint8_t* dstptr, uint16_t bytescnt,
                 uint16_t srcid, uint16_t srcx, uint16_t srcy) {
    // TODO: Fill dstptr with real image data
    // Example (dummy):
    // std::memset(dstptr, 0, bytescnt); // all black
}

// Callback for shape border tracing
void MyBorderCallback(void* userData, uint8_t cmd, uint8_t dirh, uint8_t dirv,
                      uint32_t cellx, uint32_t celly, uint16_t imgx, uint16_t imgy) {
    std::cout << "Border [cmd=" << (int)cmd 
              << ", dir=" << (int)dirh << "/" << (int)dirv 
              << "] cell(" << cellx << "," << celly 
              << ") px(" << imgx << "," << imgy << ")\n";
}

// Callback for internal content spans
void MyContentCallback(void* userData, uint32_t cxl, uint32_t cxr) {
    std::cout << "Content span: cells " << cxl << " to " << cxr << "\n";
}

} // namespace vhliboptimal

int main() {
    using namespace vhliboptimal;

    VHLibOptimal detector;

    // 1. Configuration
    stConfig cfg{};
    cfg.imageWidth   = 1920;
    cfg.imageHeight  = 1080;
    cfg.cellsize     = 8;      // Grid cell size in pixels
    cfg.spccnt       = 2;      // Max consecutive empty cells (noise tolerance)
    cfg.minColorVal  = 128;    // Brightness threshold

    // 2. Setup with callbacks
    verr result = detector.Setup(cfg, 
                                 MyGetPixels, 
                                 MyBorderCallback, 
                                 MyContentCallback);

    if (result != vok) {
        std::cerr << "Setup failed!" << std::endl;
        return -1;
    }

    // 3. Run processing
    detector.SetLogLevel(LOG_LEVEL_BASE);
    result = detector.Run(0);   // srcimgid = 0 (you can use multiple IDs)

    if (result == vok) {
        std::cout << "Scan completed successfully!\n";
        std::cout << "Objects found: " << detector.GetObjectsCount() << "\n";
        
        for (size_t i = 0; i < detector.GetObjectsCount(); ++i) {
            const VHOptimalFigure& fig = detector.GetObject(i);
            std::cout << "  Figure " << i 
                      << ": " << fig.SpansCount() << " spans, "
                      << "rect (" << fig.PosCells().x1 << "," 
                      << fig.PosCells().y1 << ") - ("
                      << fig.PosCells().x2 << "," 
                      << fig.PosCells().y2 << ")\n";
        }
    }

    return 0;
}
```

 * Copyright     : © 20012 – 2026 V01G04A81 / Viktor Glebov
