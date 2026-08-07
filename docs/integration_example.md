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
                               uint8_t dirv, uint16_t cellx, uint16_t celly, 
                               uint16_t imgx, uint16_t imgy);
```

**3. CallbackContent — Process horizontal spans inside the figure**

```cpp
/**
 * CallbackContent - Called for each horizontal span inside the object
 * 
 * @param userData   User context pointer
 * @param cell1      Left / Top cell index
 * @param cell2      Right / Bottom cell index
 * @param dir        Direction 0: LR 1: UD
 */
typedef void (*CallbackContent)(void *userData, uint32_t cell1, uint32_t cell2, uint8_t dir);
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
                      uint16_t cellx, uint16_t celly, uint16_t imgx, uint16_t imgy) {
    std::cout << "Border [cmd=" << (int)cmd 
              << ", dir=" << (int)dirh << "/" << (int)dirv 
              << "] cell(" << cellx << "," << celly 
              << ") px(" << imgx << "," << imgy << ")\n";
}

// Callback for internal content spans
void MyContentCallback(void* userData, uint32_t cell1, uint32_t cell2, uint8_t dir) {
    std::cout << "Content span: cells " << cell1 << " to " << cell2 << "\n";
}

} // namespace vhliboptimal

int main() {

    using namespace vhliboptimal;

    VHLibOptimal detector;

    // 1. Configuration
    stConfig cfg;

    cfg.imageWidth      = 800;
    cfg.imageHeight     = 600;

    cfg.cellsize        = 8;        // Grid cell size in pixels
    cfg.spccnt          = 2;        // Max consecutive empty cells (noise tolerance)
    cfg.minColorVal     = 128;      // Brightness threshold

    cfg.min_obj_width   = 32;
    cfg.min_obj_height  = 32;

    cfg.max_obj_width   = 256;
    cfg.max_obj_height  = 256;

    cfg.loglevel        = vhliboptimal::LOG_LEVEL_BASE;


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
