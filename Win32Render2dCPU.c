#include "Win32Defs.h"

//TODO: Make custom asserts
#include "assert.h"

#include "cutils/UtTypes.h"
#include "cutils/UtAlloc.h"

#include "Render2dCpu.c"

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

void R2dDelRenderTarget(R2dTarget *rt) {
    Free(rt->allocator, rt->data, rt->size);
}

R2dTarget R2dMkRenderTarget(AL *allocator, R2dSurface *rs) {
    R2dTarget rt;

    rt.allocator = allocator;

    rt.width = (u32)rs->bmpWidth;
    rt.height = (u32)rs->bmpHeight;
    rt.size = sizeof(PixColor) * rt.width * rt.height;
    rt.data = Alloc(allocator, rt.size);

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

void R2dPaintWindow(R2dTarget *rt, R2dSurface *rs, HDC deviceContext, RECT *windowRect) {
    assert((rt->height * rt->width) == (rs->bmpHeight * rs->bmpWidth));

    for (usize i = 0; i < rt->height * rt->width; i++) {
        ((u32 *)rs->bmpData)[i] = HexFromPixColor(rt->data[i]);
    }

    int windowWidth = windowRect->right - windowRect->left;
    int windowHeight = windowRect->bottom - windowRect->top;
    StretchDIBits(deviceContext,
                  0, 0, windowWidth, windowHeight,
                  0, 0, rs->bmpWidth, rs->bmpHeight,
                  rs->bmpData, &rs->bmpInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}
