#include "cutils/UtTypes.h"
#include "cutils/UtAlloc.h"
#include "Win32Defs.h"

#define BYTES_PER_PIXEL 4;

typedef struct R2dSurface {
    BITMAPINFO  bmpInfo;
    void        *bmpData;
    usize       bmpDataSize;
    HBITMAP     bmpHandle;
    HDC         bmpDeviceContext;
    i32         bmpWidth;
    i32         bmpHeight;
    AL          *allocator;
} R2dSurface;

typedef struct R2dTarget {
    usize size;
    u32 *data;
    
    u32 height;
    u32 width;
} R2dTarget;

BOOL _PixelOnSurface(u32 sWidth, u32  sHeight, i32 pixX, i32 pixY) {
    if (pixY >= sHeight || pixY < 0) {
        return FALSE;
    }
    if (pixX >= sWidth) {
        return FALSE;
    }

    return TRUE;
}

R2dTarget R2dTargetFromSurface(R2dSurface *rs) {
    R2dTarget rt;
    rt.data = (u32 *)rs->bmpData;
    rt.size = rs->bmpDataSize;
    rt.width = (u32)rs->bmpWidth;
    rt.height = (u32)rs->bmpHeight;

    return rt;
}

void R2dDelTestRenderSurface(R2dSurface *rs) {
    Free(rs->allocator, rs->bmpData, rs->bmpDataSize);
}

R2dSurface R2dMkTestRenderSurface(AL *allocator, i32 width, i32 height) {
    R2dSurface rs;

    rs.allocator = allocator;

    BITMAPINFO bitmapInfo;
    bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = -height;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    rs.bmpInfo = bitmapInfo;

    rs.bmpDataSize = (width * height) * BYTES_PER_PIXEL;
    rs.bmpData = Alloc(rs.allocator, rs.bmpDataSize);

    rs.bmpWidth = width;
    rs.bmpHeight = height;

    return rs;
}

void R2dPaintWindow(R2dSurface *rs, HDC deviceContext, RECT *windowRect) {
    int windowWidth = windowRect->right - windowRect->left;
    int windowHeight = windowRect->bottom - windowRect->top;
    StretchDIBits(deviceContext,
                  0, 0, windowWidth, windowHeight,
                  0, 0, rs->bmpWidth, rs->bmpHeight,
                  rs->bmpData, &rs->bmpInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}

void R2dClearTarget(R2dTarget *rt, u32 ARGB) {
    for (usize i = 0; i < rt->width * rt->height; i++) {
        rt->data[i] = ARGB;
    }
}

void R2dDebugClearSquare(R2dTarget *rt, i32 topLeftX, i32 topLeftY, u32 ARGB, i32 width, i32 height) {
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

        rt->data[currPixPos] = ARGB;
    }
}

void R2dRenderSquare(R2dTarget *rt, i32 topLeftX, i32 topLeftY, u32 *colorData, i32 width, i32 height) {
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

        u32 c = colorData[i];
        rt->data[currPixPos] = c;
    }
}
