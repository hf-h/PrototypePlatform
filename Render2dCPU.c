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

void R2dClearSurface(R2dSurface *rs, u32 ARGB) {
    for (usize i = 0; i < rs->bmpWidth * rs->bmpHeight; i++) {
        ((u32 *)rs->bmpData)[i] = ARGB;
    }
}

void R2dClearSquare(R2dSurface *rs, i32 topLeftX, i32 topLeftY, u32 ARGB, i32 width, i32 height) {
    i32 scStart = topLeftX + (rs->bmpWidth * topLeftY);

    i32 surfaceW = rs->bmpWidth;
    i32 surfaceH = rs->bmpHeight;

    for (i32 i = 0; i < width * height; i++) {
        i32 sqX = i % width;
        i32 sqY = i / width;
        i32 currPixPos = scStart + sqX + (sqY * surfaceW);

        ((u32 *)rs->bmpData)[currPixPos] = ARGB;
    }
}

void R2dRenderSquare(R2dSurface *rs, i32 topLeftX, i32 topLeftY, u32 *colorData, i32 width, i32 height) {
    i32 scStart = topLeftX + (rs->bmpWidth * topLeftY);

    i32 surfaceW = rs->bmpWidth;
    i32 surfaceH = rs->bmpHeight;

    for (i32 i = 0; i < width * height; i++) {
        i32 sqX = i % width;
        i32 sqY = i / width;
        i32 currPixPos = scStart + sqX + (sqY * surfaceW);

        u32 c = colorData[i];
        ((u32 *)rs->bmpData)[currPixPos] = c;
    }
}
