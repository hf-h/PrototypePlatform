#include "cutils/UtTypes.h"
#include "cutils/UtAlloc.h"

#include "Win32Defs.h"
#include "Render2dCPUDefs.h"

#define BYTES_PER_PIXEL 4;

PixColor PixColorFromHex(u32 ARGB) {
    PixColor pc;

    pc.A = ((f32)((ARGB & 0xFF000000) >> 24)) / 255.f;
    pc.R = ((f32)((ARGB & 0x00FF0000) >> 16)) / 255.f;
    pc.G = ((f32)((ARGB & 0x0000FF00) >> 8)) / 255.f;
    pc.B = ((f32)(ARGB  & 0x000000FF)) / 255.f;

    return pc;
}

u32 HexFromPixColor(PixColor col) {
    u32 hex = 0;

    hex = hex | (u32)(col.A * 255.f) << 24;
    hex = hex | (u32)(col.R * 255.f) << 16;
    hex = hex | (u32)(col.G * 255.f) << 8;
    hex = hex | (u32)(col.B * 255.f);

    return hex;
}

BOOL _PixelOnSurface(u32 sWidth, u32  sHeight, i32 pixX, i32 pixY) {
    if (pixY >= sHeight || pixY < 0) {
        return FALSE;
    }
    if (pixX >= sWidth) {
        return FALSE;
    }

    return TRUE;
}

PixColor _ColorBlending(PixColor ApplyColor, PixColor ReceiveColor, f32 alpha) {
    PixColor pc;

    pc.R = alpha * ApplyColor.R + (1 - alpha) * ReceiveColor.R;
    pc.G = alpha * ApplyColor.G + (1 - alpha) * ReceiveColor.G;
    pc.B = alpha * ApplyColor.B + (1 - alpha) * ReceiveColor.B;

    return pc;
}

void R2dClearTarget(R2dTarget *rt, PixColor color) {
    for (usize i = 0; i < rt->width * rt->height; i++) {
        rt->data[i] = color;
    }
}

void R2dDebugClearSquare(R2dTarget *rt, i32 topLeftX, i32 topLeftY, PixColor color, i32 width, i32 height) {
    i32 scStart = topLeftX + (rt->width * topLeftY);

    i32 surfaceW = rt->width;
    i32 surfaceH = rt->height;

    for (i32 i = 0; i < width * height; i++) {
        i32 sqX = i % width;
        i32 sqY = i / width;
        i32 currPixPos = scStart + sqX + (sqY * surfaceW);

        if (FALSE == _PixelOnSurface(rt->width, rt->height, topLeftX + sqX, topLeftY + sqY)) {
            continue;
        }

        rt->data[currPixPos] = color;
    }
}

void R2dDebugRenderSpriteWithSetAlpha(R2dTarget *rt, i32 topLeftX, i32 topLeftY, Sprite2D sprite, f32 alpha) {
    i32 scStart = topLeftX + (rt->width * topLeftY);

    i32 surfaceW = rt->width;
    i32 surfaceH = rt->height;

    for (i32 i = 0; i < sprite.width * sprite.height; i++) {
        i32 sqX = i % sprite.width;
        i32 sqY = i / sprite.width;
        i32 currPixPos = scStart + sqX + (sqY * surfaceW);

        if (FALSE == _PixelOnSurface(rt->width, rt->height, topLeftX + sqX, topLeftY + sqY)) {
            continue;
        }

        PixColor c = sprite.data[i];
        rt->data[currPixPos] = _ColorBlending(c, rt->data[currPixPos], alpha);
    }
}

void R2dRenderSquare(R2dTarget *rt, i32 topLeftX, i32 topLeftY, PixColor *colorData, i32 width, i32 height) {
    i32 scStart = topLeftX + (rt->width * topLeftY);

    i32 surfaceW = rt->width;
    i32 surfaceH = rt->height;

    for (i32 i = 0; i < width * height; i++) {
        i32 sqX = i % width;
        i32 sqY = i / width;
        i32 currPixPos = scStart + sqX + (sqY * surfaceW);

        if (FALSE == _PixelOnSurface(rt->width, rt->height, topLeftX + sqX, topLeftY + sqY)) {
            continue;
        }

        PixColor c = colorData[i];
        rt->data[currPixPos] = _ColorBlending(c, rt->data[currPixPos], c.A);
    }
}

void R2dRenderSprite(R2dTarget *rt, i32 topLeftX, i32 topLeftY, Sprite2D sprite) {
    i32 scStart = topLeftX + (rt->width * topLeftY);

    i32 surfaceW = rt->width;
    i32 surfaceH = rt->height;

    for (i32 i = 0; i < sprite.width * sprite.height; i++) {
        i32 sqX = i % sprite.width;
        i32 sqY = i / sprite.width;
        i32 currPixPos = scStart + sqX + (sqY * surfaceW);

        if (FALSE == _PixelOnSurface(rt->width, rt->height, topLeftX + sqX, topLeftY + sqY)) {
            continue;
        }

        PixColor c = sprite.data[i];
        rt->data[currPixPos] = _ColorBlending(c, rt->data[currPixPos], c.A);
    }
}

