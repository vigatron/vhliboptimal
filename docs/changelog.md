#### ChangeLog

##### 0.8.1

* STM32F4 tests passed with optimization ( CCMRAM enabled )
* STM32F7 tests passed with optimization ( DTCRAM & ITCRAM enabled )
* STM32H7 tests passed with optimization ( DTCRAM & ITCRAM enabled )

##### 0.8.0

* Zero-allocation / FIXED_GRID for bare-metal ( STM32 / ESP32 etc ...)
* API changes / Callbacks
* MemoryLayout as part of configuration
* Minor fixes & optimizations ...

##### 0.7.5

* TYPO FIXED: define fixed for correct 32/64-bit platform switch

##### 0.7.4

* BUGFIX: BitField::ResetSearchIndex() - fixed, curSearchWord / lastSearchsByte calculated correctly

##### 0.7.3

* VHLibOptimal::InitialScanImage() Added a one-cell empty border around the bitfield.
* VHLibOptimal::InitialScanImage() Reduced the active scan area by one cell on each side.
* BitField::FastIdxNonZero() optimized for uint32_t and uint64_t
* BitField::FastIdxNonZero() removed support for uint8_t and uint16_t
