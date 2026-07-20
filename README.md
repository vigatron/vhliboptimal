# vhliboptimal

![Language](https://img.shields.io/badge/Language-C%2B%2B23-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Cross--platform-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A high-performance C++23 library for fast shape contour detection and image outline recognition using optimized grid-based scanning.

## 📌 Project Overview

`vhliboptimal` is designed for efficient object boundary extraction and image segmentation. Instead of relying on heavy, resource-intensive computer vision algorithms, it processes images by breaking them down into a lightweight grid configuration (**Cells Matrix**) and tracking connectivity via highly optimized bit arrays (**BitFields**). This drastically reduces both memory footprint and CPU overhead.

### Evolution & Background
*   **2012–2017**: The legacy version (developed under the `optimal` project) was written in **Plain C** (public headers-only) and successfully deployed in commercial products for embedded platforms (**ARM 32-bit** and **AVR 8-bit** microcontrollers such as ATMega, ESP32, STM32) with severe RAM and CPU constraints.
*   **2026**: The architecture has been completely modernized and rewritten in **C++23**, providing a safe object-oriented interface, type-safe functional callbacks, and deep data structure optimizations while preserving its highly portable nature.

---

## 🛠 Technical Specifications

*   **Language:** C++23
*   **Platform:** Developed and tested on Linux. The codebase is OS-abstracted, making it easily portable to Windows or bare-metal embedded targets.
*   **License:** MIT
*   **Author:** Viktor Glebov (V01G04A81)

### Dependencies
*   **vhlibplatform** ([github.com/vigatron/vhlibplatform](https://github.com/vigatron/vhlibplatform)) — A cross-platform layer handling base primitive types and system abstractions.

---

## 🧩 Architecture & Key Components

The library operates completely abstracted from raw graphic decoders or UI frameworks (like OpenCV or `stb_image`). It processes data streams through an abstract coordinate grid:

*   **`CellsMatrix`**: Manages the spatial geometry of the grid. Images are analyzed in configurable blocks (`cellsize`), decreasing overall data dimensionality.
*   **`BitField`**: A packed bit array tracking filled/empty cells. It allows instantaneous pathfinding operations and cell clearing during figure extraction loops.
*   **`VHOptimalFigure`**: Encapsulates a single extracted shape, containing its bounding box, sorted sequential contours, and analytical span strings.

### Key Data Structures (`src/vhliboptimalstructs.hpp`)
*   `stConfig` — Scanning parameters (image boundaries, grid cell size, color thresholding tolerance `minColorVal`, and max consecutive empty spaces `spccnt`).
*   `strect` — Bounding box structure representation (`x1, y1` to `x2, y2`).
*   `stspan` — Horizontal/vertical segments representing the continuous boundaries of a shape.

---

## 🚀 Quick Start & Integration Example

Memory interaction is managed seamlessly using `std::function` callbacks:
1.  `GetPixelsCallback` — Handles fetching pixel chunk ranges from your frame buffer.
2.  `SetPosCallback` — Triggers when an object boundary segment is resolved.

### Basic Usage Example

```cpp
#include <iostream>
#include <vector>
#include "vhliboptimal.hpp"

// Example callback for loading source pixels
std::vector<uint8_t> MyGetPixelsHandler(uint16_t x, uint16_t y, uint16_t length) {
    // In production: Return actual pixel chunks from your frame buffer/texture array
    return std::vector<uint8_t>(length, 255); 
}

// Example callback for handling detected object contours
void MySetPosHandler(uint8_t r, uint8_t g, uint8_t b, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    std::cout << "Contour boundary detected: (" << x1 << ", " << y1 << ") -> (" << x2 << ", " << y2 << ")\n";
}

int main() {
    using namespace vhliboptimal;

    VHLibOptimal detector;

    // 1. Configure properties
    stConfig cfg;
    cfg.imageWidth = 1920;
    cfg.imageHeight = 1080;
    cfg.cellsize = 8;        // Analyze via 8x8 pixel macro-blocks
    cfg.spccnt = 2;          // Allow a maximum gap of 2 empty cells inside a single figure
    cfg.minColorVal = 128;   // Pixel value cutoff floor (values >= 128 are treated as active)

    // 2. Setup configuration and attach handlers
    if (detector.Setup(cfg, MyGetPixelsHandler, MySetPosHandler) != vok) {
        std::cerr << "Initialization failed! Invalid parameters or callbacks." << std::endl;
        return -1;
    }

    // 3. Run execution pipeline
    detector.SetLogLevel(LOG_LEVEL_BASE); // Enable basic console updates
    verr result = detector.Run();

    if (result == vok) {
        std::cout << "Scan completed successfully.\n";
        std::cout << "Objects found: " << detector.GetObjectsCount() << "\n";
        std::cout << "Total spans calculated: " << detector.GetSpansTotal() << "\n";
    } else {
        std::cerr << "An error occurred during image processing execution.\n";
    }

    return 0;
}
```