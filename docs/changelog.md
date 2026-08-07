#### ChangeLog

##### 0.7.5
* TYPO FIXED: define fixed for correct 32/64-bit platform switch

##### 0.7.4
* BUGFIX: BitField::ResetSearchIndex() - segmentation fault fixed, curSearchWord / lastSearchsByte calculated correctly

##### 0.7.3
* VHLibOptimal::InitialScanImage() Added a one-cell empty border around the bitfield.
* VHLibOptimal::InitialScanImage() Reduced the active scan area by one cell on each side.
* BitField::FastIdxNonZero() optimized for uint32_t and uint64_t
* BitField::FastIdxNonZero() removed support for uint8_t and uint16_t
