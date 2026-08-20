## Quick Start & Integration Example (v0.8.0+)

### Важные изменения относительно 0.7.x

- `CallbackGetSrcPxls` **убран**. Библиотека больше не ходит за пикселями сама.
- Пользователь сам заполняет `BitFieldSrc` (или использует встроенный BMP-парсер).
- `Setup()` теперь принимает **memory layout** + `callbackparent` + три колбэка.
- `Run()` вызывается без параметров (битфилд уже должен быть готов).
- Появился zero-allocation / FIXED_GRID режим через `VHMemoryLayout`.

---

### Callbacks (новый стиль)

```cpp
/**
 * CallbackBorder — обход контура фигуры
 */
typedef void (*CallbackBorder)(
    void*     userData,
    uint8_t   cmd,      // cmdStart / cmdMove / cmdStop
    uint8_t   dirh,     // dirLeft / dirRight
    uint8_t   dirv,     // dirUp / dirDown
    uint16_t  cellx,
    uint16_t  celly
);

/**
 * CallbackContent — горизонтальные / вертикальные spans внутри объекта
 */
typedef void (*CallbackContent)(
    void*     userData,
    uint32_t  cell1,
    uint32_t  cell2,
    uint8_t   dir       // 0 = Horizontal (L→R), 1 = Vertical (U→D)
);

/**
 * CallbackBenchmark — опциональные точки замеров
 */
typedef void (*CallbackBenchmark)(
    void*  userData,
    int    cmd,
    int    param
);
```

### Минимальный рабочий пример (dynamic allocation)

```cpp
#include "vhliboptimal.hpp"
#include <vector>
#include <cstdint>
#include <cstdio>

using namespace vhliboptimal;

// ===================== CALLBACKS =====================

void MyBorder(
    void* userData,
    uint8_t cmd, uint8_t dirh, uint8_t dirv,
    uint16_t cellx, uint16_t celly)
{
    // Здесь можно рисовать контур, собирать точки и т.д.
    // printf("Border cmd=%u cell(%u,%u)\n", cmd, cellx, celly);
}

void MyContent(
    void* userData,
    uint32_t cell1, uint32_t cell2, uint8_t dir)
{
    // Здесь можно копировать spans / заполнять маску объекта
}

void MyBenchmark(void* userData, int cmd, int param)
{
    // Опционально: замеры времени
}

// ===================== MAIN =====================

int main()
{
    VHLibOptimal detector;

    // 1. Конфигурация сканирования
    const stConfig cfg = {
        .spccnt         = 0,        // допустимые пустые ячейки в span
        .min_obj_width  = 2,
        .min_obj_height = 2,
        .max_obj_width  = 512,
        .max_obj_height = 512,
        .sortMode       = 0,
        .loglevel       = LOG_LEVEL_BASE
    };

    // 2. Выделяем буферы (динамический режим)
    //    Размеры можно узнать после создания детектора
    std::vector<uint8_t> mem_gridsrc (detector.MemBytesPerGrid());
    std::vector<uint8_t> mem_griddst (detector.MemBytesPerGrid());
    std::vector<uint8_t> mem_objects (detector.MemBytesPerObjs());
    std::vector<uint8_t> mem_spans   (detector.MemBytesPerSpns());

    const VHMemoryLayout::stMemLayout memcfg = {
        .memSrcGrid = { .ptr = mem_gridsrc.data(), .size = mem_gridsrc.size() },
        .memDstGrid = { .ptr = mem_griddst.data(), .size = mem_griddst.size() },
        .memObject  = { .ptr = mem_objects.data(), .size = mem_objects.size() },
        .memSpans   = { .ptr = mem_spans.data(),   .size = mem_spans.size()   }
    };

    // 3. Инициализация
    verr r = detector.Setup(
        cfg,
        memcfg,
        nullptr,                // userData (можно передать this / контекст)
        MyBorder,
        MyContent,
        MyBenchmark
    );

    if (r != vok) {
        printf("Setup failed\n");
        return 1;
    }

    // 4. Заполняем BitFieldSrc самостоятельно
    //    (пример: из своего бинарного/threshold изображения)
    BitField& bf = detector.BitFieldSrc();
    const CellsMatrix& cmtx = detector.GetCMatrix();

    bf.ClearArea(cmtx);

    // Допустим, у вас есть width/height исходной картинки
    // и функция isFilled(x, y)
    /*
    uint16_t levelcs = ...; // сколько раз сдвинуть (cell size = 1 << levelcs)

    for (uint16_t y = 0; y < img_h; y++) {
        for (uint16_t x = 0; x < img_w; x++) {
            if (isFilled(x, y)) {
                uint16_t cx = x >> levelcs;
                uint16_t cy = y >> levelcs;
                bf.SetCell(cmtx, cx, cy);
            }
        }
    }
    */

    // 5. Запуск
    detector.FrameReset();          // сбросить счётчики объектов/спанов
    r = detector.Run();

    if (r != vok) {
        printf("Run failed\n");
        return 2;
    }

    // 6. Результаты
    printf("Objects found: %u\n", detector.ObjectsCount());

    for (uint16_t i = 0; i < detector.ObjectsCount(); i++) {
        // Можно вызвать Border / ContentH / ContentV,
        // чтобы снова пройтись колбэками по уже найденной фигуре
        detector.Border(i);
        detector.ContentH(i);
        detector.ContentV(i);

        const VHOptimalFigure& fig = detector.Object(i);
        // fig.PosCells() и т.д.
    }

    return 0;
}
```

### Как заполнять BitFieldSrc (реальный паттерн из приложения)

```cpp
verr PrepareBitField(VHLibOptimal& detector, /* ваш источник пикселей */)
{
    const CellsMatrix& cmtx = detector.GetCMatrix();
    BitField& bf = detector.BitFieldSrc();

    bf.ClearArea(cmtx);

    // Автоопределение скейлера (если cellsize не равен 1)
    // uint16_t levelcs = CalcScaller(img_width, cmtx.CellsX());

    for (uint16_t y = 0; y < img_height; y++) {
        for (uint16_t x = 0; x < img_width; x++) {
            if (/* пиксель "заполнен" */) {
                uint16_t cx = x >> levelcs;
                uint16_t cy = y >> levelcs;
                bf.SetCell(cmtx, cx, cy);
            }
        }
    }
    return vok;
}
```

После этого просто:

```cpp
detector.FrameReset();
detector.Run();
```

### FIXED_GRID / zero-allocation режим (STM32 / ESP32)

В CMake:

```cmake
target_compile_definitions(vhlib_optimal PUBLIC
    VHLIB_OPTIMAL_GRID_FIXED
    VHLIB_OPTIMAL_GRID_LX=8
    VHLIB_OPTIMAL_GRID_LY=8
    VHLIB_OPTIMAL_OBJS_MAX=256
    VHLIB_OPTIMAL_SPNS_MAX=4096
)
```

Тогда буферы можно держать статически (или в .bss), а stMemLayout заполнять указателями на эти массивы. Динамических new/vector внутри библиотеки не будет.


### Типичный цикл обработки кадра

```cpp
// 1. Подготовить битфилд из камеры / фильтра / BMP
PrepareBitField(...);

// 2. Сбросить счётчики
detector.FrameReset();

// 3. Запустить поиск
detector.Run();

// 4. Пройтись по найденным объектам
for (uint16_t i = 0; i < detector.ObjectsCount(); ++i) {
    detector.Border(i);    // вызовет CallbackBorder
    detector.ContentH(i);  // вызовет CallbackContent (горизонталь)
    detector.ContentV(i);  // вызовет CallbackContent (вертикаль)
}
```
