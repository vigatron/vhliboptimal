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

---

## Key Features

* Zero heavy external dependencies
* Three flexible C-style callbacks for maximum integration flexibility
* Support for multiple image sources via `srcimgid`
* Highly optimized grid-based scanning with bit-packing
* Configurable cell size and noise tolerance
* Real-time contour and content processing via callbacks


---

## History & Evolution

The `vhliboptimal` library has deep roots in real-world embedded computer vision, evolving from extreme hardware constraints to modern software efficiency.

#### 2006–2012 — The Extreme Embedded Roots (AVR + External SRAM)
The algorithm originated as a raster-to-vector engine for 8-bit AVR microcontrollers, initially tasked with recognizing character contours and geometric shapes on tiny 128x64 B&W displays. To handle image processing, the system utilized 32KB of external SRAM accessed via a multiplexed bus (74HC573 + ALE). 
The core engineering challenge was the severe bottleneck of the external memory bus. The grid-based `BitField` architecture was specifically designed here to minimize external bus accesses, keeping the heavy pathfinding logic strictly within the MCU's fast internal RAM.
* **Legacy Artifact:** A surviving single-header (`.h`) version of this original plain C implementation is preserved as a historical reference at [electrolviv/optimal](https://github.com/electrolviv/optimal).

#### 2016 — Hardware-Accelerated Era (FPGA + STM32)
As tasks grew more complex, the algorithm was scaled and integrated into a **dual-camera stereo vision** system based on a Xilinx Spartan-6 FPGA + SDRAM, paired with an STM32F7 microcontroller.

- **Proven Benchmark (2016):** Performance-critical parts in Verilog on Xilinx Spartan-6 processed shape contours from **two synchronized cameras at 60 FPS (VGA 640×480)**. 
STM32F7 handled higher-level logic. This hybrid solution delivered hard real-time performance with minimal jitter.

- The STM32 handled control logic and remaining processing in plain C. This tight parallel cooperation proved the algorithm's viability for demanding industrial robotics and automated inspection lines, where software-only solutions like OpenCV were too heavy, slow, or non-deterministic.

#### 2026 — Modern C++ Rewrite for SBCs

The library has been completely redesigned and rewritten from the ground up in modern **C++23**.  

The main goal of this update is to adapt the battle-tested algorithm for modern affordable Single Board Computers (such as Raspberry Pi, Orange Pi, and similar), enabling real-time operation with **a single camera and pure software — no FPGA required**.  

Thanks to intelligent grid-based downsampling (`cellsize` 8–16 pixels), the library achieves practical real-time performance of approximately **10–20 FPS on 1080p** for edge-AI, robotics, and automated sorting tasks, while preserving the core philosophy of extreme efficiency that originated on 8-bit microcontrollers nearly 15 years ago.


---

## 🛠 Technical Specifications

- **Language**: C++23 (Strict requirement. *Note: Legacy C implementations for 8-bit/32-bit MCUs are not part of this codebase.*)
- **Target Platforms**: 
  - **Desktop/OS**: Linux (Primary), Windows, macOS.
  - **Modern SBCs**: Raspberry Pi, Orange Pi, and similar ARM-based boards.
  - **Modern MCUs**: STM32H7, STM32MP1, and other modern ARM Cortex-M/A cores with C++23 compiler support.
  - *(Legacy bare-metal targets like AVR or older STM32 families are not supported in this C++23 rewrite).*
- **Build System**: CMake 3.16+
- **License**: MIT
- **Author**: Viktor Glebov (V01G04A81)

---

### Dependencies
- No heavy third-party dependencies. Requires only [`vhlibplatform`](https://github.com/vigatron/vhlibplatform) — a lightweight cross-platform layer providing base primitive types, assertions, error handling, and system abstractions.

---

### Limitations & Trade-offs
- **Image Type**: Best suited for binary or high-contrast images (a direct inheritance from its B&W display origins).
- **Threading**: Currently single-threaded (multi-threading support is planned for future releases).
- **Resolution vs. Performance**: To achieve real-time FPS on SBCs, the algorithm relies on grid-based downsampling (`cellsize` 8-16px). Fine image details smaller than the configured cell size will be intentionally lost to preserve CPU cycles.


> **Best Practices for Optimal Results**
The algorithm was originally proven on pristine, uncompressed RAW video streams. When using modern compressed sources (e.g., MJPEG/MP4 webcams on SBCs), compression artifacts and blurring can degrade contour accuracy.
Recommendation: For best results, apply a lightweight pre-processing step (e.g., hardware-accelerated thresholding, sharpening, or edge-enhancement) before passing the frame to vhliboptimal, or tune minColorVal and spccnt to be more tolerant of digital noise.


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

 * Copyright: © 2012 – 2026 V01G04A81 / Viktor Glebov
