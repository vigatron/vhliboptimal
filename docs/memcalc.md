# Memory calculation




## MCU List

Hardware Support / Target MCUs:

* **STM32 Series:** ARM® Cortex®-M (F1, F4, F7, H7)
* **ESP32 Series:** Xtensa® / RISC-V dual-core processors



**Grid** Размер сетки

*   128 x  128 =   16384   div8      2048       2K /   4K
*   256 x  256 =   65536   div8      8192       8K /  16K
*   512 x  512 =  262144   div8     32768      32K /  64K
*  1024 x 1024 = 1048576   div8    131072     128K / 256K


**CellID**

*   128 x  128 = 14-bit       16384 max
*   256 x  256 = 16-bit       65536 max
*   512 x  512 = 18-bit      262144 max
*  1024 x 1024 = 20-bit     1048576 max


**BitField**

При конвертации в битовый режим получаем преимущества:

* Объем памяти для BitField при разрешении 256x256 в байтах = 32x256 = 8192
* Для работы алгоритма необходимо x2 BitField. Суммарно 8192x2 = 16384


**Span**  4 байта / 32-bit для отрезка

* uint16_t cellid;    // Координата ячейки
* uint16_t cellwd;    // Длинна


**Object**

 * uint32_t     cellid;
 * uint16_t     width;
 * uint16_t     height;

 4 байта / 32-bit на 1 фигуру


*Worst Case*

Размер изображения: 256x256 , через точку : 128x256 = 32768 spans max



### Configuration

**Отказ от динамической памяти**

1024:   4K