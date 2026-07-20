# vhliboptimal

![Language](https://img.shields.io/badge/Language-C%2B%2B23-blue.svg)
![CMake](https://img.shields.io/badge/Build-CMake-1f4f9c.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Cross--platform-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A high-performance C++23 library for fast shape contour detection and image outline recognition using optimized grid-based scanning.


---

## Project Overview

`vhliboptimal` is a high-performance C++23 library for fast shape contour detection and image outline recognition.  

It uses an optimized grid-based approach: the image is divided into a configurable **Cells Matrix**, and connectivity is tracked using compact **BitFields**. This design delivers excellent performance with very low memory and CPU usage, making it ideal for embedded systems and real-time applications.  

Unlike heavyweight computer vision libraries such as OpenCV, `vhliboptimal` is lightweight, highly portable, and works through simple callback interfaces. It excels at processing binary or high-contrast images and gracefully handles small gaps and noise thanks to tunable parameters.  

Originally developed in plain C (2012–2017) for commercial embedded projects on ARM and AVR platforms, it has been completely modernized in 2026 with a clean object-oriented C++23 interface while preserving its efficiency-focused philosophy.


### Evolution & Background

The `vhliboptimal` library has a long practical history rooted in real-world embedded systems development.

- **2012–2017 (Legacy Era)**: The original version was written in plain C as a lightweight, headers-only library. It was successfully deployed in several commercial products running on severely resource-constrained platforms, including **ARM 32-bit** and **AVR 8-bit** microcontrollers (such as ATMega, ESP32, and STM32). In those environments, every byte of RAM and CPU cycle mattered, so the algorithm relied heavily on bit-packing, grid-based scanning, and minimal memory usage. The library proved reliable in industrial computer vision tasks where full-featured solutions like OpenCV were simply too heavy or slow. It was particularly effective for contour detection in real-time applications such as robotics, quality control systems, and automated inspection lines. Developers appreciated its simplicity and predictable performance even on 8-bit MCUs with only a few kilobytes of RAM.

- **2026 (Modern Rewrite)**: The library has been completely redesigned and rewritten from the ground up in modern **C++23**. This major update brings a clean object-oriented interface, type-safe functional callbacks, better code organization, and improved safety, all while preserving the original performance philosophy and portability.

This evolution reflects the author’s 25+ years of experience in embedded systems, FPGA development, and low-level optimization. The core idea — **efficient contour detection without heavy dependencies** — remains unchanged, but the implementation is now much more modern, portable, and developer-friendly.

---


## 🛠 Technical Specifications

- **Language**: C++23 (with backward compatibility considerations for embedded toolchains)
- **Platform**: Primarily developed and tested on Linux. The codebase is OS-abstracted through `vhlibplatform`, making it easily portable to Windows, macOS, and bare-metal embedded targets (ARM, AVR, RISC-V, etc.).
- **License**: MIT
- **Author**: Viktor Glebov (V01G04A81)
- **Build System**: CMake 3.16+


### Dependencies
- [`vhlibplatform`](https://github.com/vigatron/vhlibplatform) — a lightweight cross-platform layer providing base primitive types, assertions, error handling, and system abstractions.


### Key Features
- Zero heavy external dependencies (no OpenCV, no `stb_image`, etc.)
- Callback-driven architecture for maximum flexibility with different image sources
- Highly optimized memory usage through bit-packing and grid-based processing
- Configurable grid cell size and noise tolerance (`cellsize`, `spccnt`, `minColorVal`)
- Support for real-time contour tracing via `SetPosCallback`

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

Memory interaction is managed seamlessly using `std::function` callbacks:
1.  `GetPixelsCallback` — Handles fetching pixel chunk ranges from your frame buffer.
2.  `SetPosCallback` — Triggers when an object boundary segment is resolved.

### Basic Usage Example

```cpp
#include <iostream>
#include <vector>
#include <functional>

#include "vhliboptimal.hpp"

namespace vhliboptimal {

// Callback to fetch a row/chunk of pixels (returns const reference)
const std::vector<uint8_t> & MyGetPixelsHandler(uint16_t x, uint16_t y, uint16_t length) {
    static std::vector<uint8_t> buffer;   // reused static buffer
    buffer.assign(length, 255);           // dummy: all white pixels
    // TODO: In real code fill buffer from your framebuffer / camera
    return buffer;
}

// Callback for contour position
void MySetPosHandler(
    uint8_t cmd,
    uint8_t dirh, uint8_t dirv,
    uint16_t cellx, uint16_t celly,
    uint16_t pxlx, uint16_t pxly)
{
    std::cout << "SetPos [cmd=" << (int)cmd 
              << ", dir=" << (int)dirh << "/" << (int)dirv 
              << "] cell(" << cellx << "," << celly << ") "
              << "px(" << pxlx << "," << pxly << ")\n";
}

} // namespace vhliboptimal

int main() {

    using namespace vhliboptimal;

    VHLibOptimal detector;

    // 1. Configuration
    stConfig cfg{};
    cfg.imageWidth   = 1920;
    cfg.imageHeight  = 1080;
    cfg.cellsize     = 8;      // Analysis block size in pixels
    cfg.spccnt       = 2;      // Max consecutive empty cells allowed
    cfg.minColorVal  = 128;    // Brightness threshold

    // 2. Setup
    verr result = detector.Setup(cfg, MyGetPixelsHandler, MySetPosHandler);
    if (result != vok) {
        std::cerr << "Setup failed!" << std::endl;
        return -1;
    }

    // 3. Run processing
    detector.SetLogLevel(LOG_LEVEL_BASE);
    result = detector.Run();

    if (result == vok) {
        std::cout << "Scan completed successfully!\n";
        std::cout << "Objects found: " << detector.GetObjectsCount() << "\n";
        std::cout << "Total spans: " << detector.GetSpansTotal() << "\n";
    } else {
        std::cerr << "Error during processing.\n";
    }

    return 0;
}
```