#ifndef RENDER_2D_CPU_DEFS_H
#define RENDER_2D_CPU_DEFS_H

#include "cutils/UtAlloc.h"
#include "cutils/UtTypes.h"

typedef struct PixColor {
    f32 R, G, B, A;
} PixColor;

typedef struct Sprite2D {
    u64 width, height;
    PixColor *data;
} Sprite2D;

typedef struct R2dTarget {
    AL *allocator;

    usize size;
    PixColor *data;
    
    u32 height;
    u32 width;
} R2dTarget;

#endif
