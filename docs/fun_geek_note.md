### 💡 A Fun Geek Note: 2016 FPGA vs 2026 C++ (The Branching Dilemma)

You might wonder: *How does the modern C++ version compare to the 2016 FPGA implementation?*  
The answer lies in the fundamental difference between hardware pipelines and software execution when dealing with **unpredictable branching** during contour tracing.

#### 🛑 The Dilemma: CPU Reality
Tracing irregular shapes creates chaotic control flow. Branch mispredictions (incurring a 15–25 cycle penalty on modern cores) and data-dependent bitwise operations on dynamic `BitField` indices create significant pipeline stalls. SIMD (AVX2/NEON) helps a lot during the initial grid scanning, but is nearly useless in the extraction and tracing phase.

#### ⚙️ The 2016 FPGA Advantage
The 2016 Spartan-6 implementation used a hardware Finite State Machine (FSM) and dedicated Block RAM (BRAM), evaluating neighbor states instantly with minimal latency and perfect determinism. Even running at a modest ~166 MHz, it achieved outstanding efficiency in the core loop. 

The FPGA version utilized massive hardware parallelism, processing bitfields instantly via dedicated BRAM. *The original FPGA implementation demonstrated that the algorithm maps efficiently to hardware because of its regular grid-based data flow and compact BitField representation.*

######  Legacy Artifact
A surviving header (`.h`) file of this original plain C implementation is preserved as a historical reference at [electrolviv/optimal](https://github.com/electrolviv/optimal).


#### 🔄 Architectural Shift: From Hardware Pipelines to Commodity Software
The modern C++ implementation preserves the same memory-efficient architecture while targeting commodity CPUs and SBCs. However, transitioning to modern C++ on general-purpose CPUs introduces critical microarchitectural differences (cache hierarchy, memory walls, and heavy dependency on branch prediction). 

While pure FPS on a 2022 laptop or an ARM SBC might currently seem lower than the dedicated 2016 hardware pipeline, the trade-off shifts the value to **extreme maintainability, rapid integration, and deployment** on low-cost single-board computers (like Orange Pi/Raspberry Pi) without requiring expensive custom FPGA tooling.

#### 🚀 The 2026 Reality
Thanks to aggressive grid downsampling, careful `BitField` design, and raw CPU clock speeds, the C++ version delivers practical real-time performance (depending on CPU, resolution, and scene complexity) on affordable SBCs. 

It preserves the original philosophy of extreme efficiency born on 8-bit microcontrollers nearly 20 years ago, now running efficiently on general-purpose CPUs with AVX2 optimizations where available. While the 2016 FPGA version remains superior in raw tracing latency and absolute timing predictability, the 2026 rewrite brings that hard-earned efficiency to the modern software edge.
