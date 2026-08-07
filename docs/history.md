## History & Evolution

The `vhliboptimal` library has deep roots in real-world embedded computer vision, evolving from extreme hardware constraints to modern software efficiency.

#### 2006 — The Extreme Embedded Roots (AVR + External SRAM)
The algorithm originated as a raster-to-vector engine for 8-bit AVR microcontrollers, initially tasked with recognizing character contours and geometric shapes on tiny 128x64 B&W displays. To handle image processing under severe memory constraints, the system utilized 32KB of external SRAM accessed via a multiplexed bus (74HC573 + ALE). The core engineering challenge was overcoming the performance bottleneck of this external memory bus.

#### 2010 ... 2012 — Road Signs Recognition
The grid-based `BitField` architecture was specifically designed during this period to minimize external bus accesses, keeping the heavy pathfinding logic strictly within the MCU's fast internal RAM. It was successfully tested on LPC2148 and AT91SAM7X256 platforms.

#### 2016 — Hardware-Accelerated Era (FPGA + STM32)
As tasks grew more complex, the algorithm was scaled and integrated into a **dual-camera stereo vision** system based on a Xilinx Spartan-6 FPGA + SDRAM, paired with an STM32F7 microcontroller.
* **Benchmark (2016):** Performance-critical parts implemented in Verilog on the Xilinx Spartan-6 processed shape contours from **two synchronized cameras at 60 FPS (VGA 640×480)**.
* The STM32 handled control logic and remaining processing in plain C. This tight parallel cooperation proved the algorithm's viability for demanding industrial robotics and automated inspection lines, where software-only solutions like OpenCV were too heavy or non-deterministic.

#### 2026 — Modern C++ Rewrite for SBCs
The library has been completely redesigned and rewritten from the ground up in modern **C++17**.
* **The main goal:** Adapt the battle-tested grid-based algorithm for today's affordable Single Board Computers (Raspberry Pi, Orange Pi, etc.), enabling real-time operation with **a single camera and pure software — no FPGA required**.
* **Important trade-off:** While the 2016 FPGA implementation remains a strong reference for raw speed and determinism (thanks to dedicated BRAM and hardware parallelism), the new C++ version delivers **practical real-time performance of approximately 10–25 FPS on 1080p** (depending on CPU, `cellsize`, and scene complexity). This makes it highly suitable for edge-AI, robotics, and automated sorting tasks on widely available commodity hardware.

It preserves the original philosophy of extreme efficiency born on 8-bit microcontrollers nearly 20 years ago, now running efficiently on general-purpose CPUs with AVX2 optimizations where available.
