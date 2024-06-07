#include "cutils/UtTypes.h"
#include "cutils/UtMem.h"
#include "cutils/UtAlloc.h"

#include "Windows.h"

typedef u64 BMP_READ_ERR;

#define BMP_READ_ERR_NO_ERR                             0x0000
#define BMP_READ_ERR_OPEN_FILE                          0x0001
#define BMP_READ_ERR_ALLOCATION                         0x0002
#define BMP_READ_ERR_READ_FILE                          0x0003
#define BMP_READ_ERR_INCOMPATIBLE_DIB_HEADER_VERIONS    0x0004
#define BMP_READ_ERR_NOT_A_BMP                          0x0005
#define BMP_READ_ERR_INCOMPATIBLE_FORMAT                0x0006
#define BMP_READ_ERR_FAILED_TO_GET_FILE_INFO            0x0007

#define IMG_RESOURCE_PIXEL_SIZE sizeof(u32)
//ARGB color
typedef struct ImgResource {
    u32 width;
    u32 height;
    u8 *pixels;
} ImgResource;

BMP_READ_ERR ReadBMPFromFile(AL *al, const char *filePath, ImgResource **data) {
    BMP_READ_ERR result = BMP_READ_ERR_NO_ERR;

    HANDLE fh = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (NULL == fh) {
        return BMP_READ_ERR_OPEN_FILE;
    }

    LARGE_INTEGER fs;
    if (FALSE == GetFileSizeEx(fh, &fs)) {
        result = BMP_READ_ERR_FAILED_TO_GET_FILE_INFO;
        goto CLOSE_FH;
    }

    u8 *fb = Alloc(al, fs.QuadPart);
    DWORD bytesRead = 0;
    if (FALSE == ReadFile(fh, fb, fs.QuadPart, &bytesRead, NULL)) {
        goto CLEANUP;
    }

    if ('B' != fb[0] && 'M' != fb[1]) {
        result = BMP_READ_ERR_NOT_A_BMP;
        goto CLEANUP;
    }

    u32 dibHeaderSize = *(u32 *)(fb + 14);
    if (sizeof(BITMAPV5HEADER) != dibHeaderSize && sizeof(BITMAPV4HEADER) != dibHeaderSize) {
        result = BMP_READ_ERR_INCOMPATIBLE_DIB_HEADER_VERIONS;
        goto CLEANUP;
    }

    BITMAPV4HEADER *bmpHeader = (BITMAPV4HEADER *)(fb + 14);

    if (BI_BITFIELDS != bmpHeader->bV4V4Compression) {
        result = BMP_READ_ERR_INCOMPATIBLE_FORMAT;
        goto CLEANUP;
    }

    if (32 != bmpHeader->bV4BitCount) {
        result = BMP_READ_ERR_INCOMPATIBLE_FORMAT;
        goto CLEANUP;
    }

    const usize pixCount = bmpHeader->bV4SizeImage;
    const usize dataSize = sizeof(ImgResource) + (sizeof(u32) * pixCount);
    *data = Alloc(al, dataSize);

    ImgResource *re = *data;
    re->width  = bmpHeader->bV4Width;
    re->height = bmpHeader->bV4Height;
    re->pixels = (u8 *)(*data) + sizeof(ImgResource);

    u32 offsetToPix = *(u32 *)(fb + 0x0A);
    u32 *pixArray = (u32 *)(fb + offsetToPix);
    for (usize i = 0; i < re->width * re->height; i++) {
        re->pixels[(i * IMG_RESOURCE_PIXEL_SIZE) + 0] = bmpHeader->bV4AlphaMask | pixArray[i];
        re->pixels[(i * IMG_RESOURCE_PIXEL_SIZE) + 1] = bmpHeader->bV4RedMask   | pixArray[i];
        re->pixels[(i * IMG_RESOURCE_PIXEL_SIZE) + 2] = bmpHeader->bV4GreenMask | pixArray[i];
        re->pixels[(i * IMG_RESOURCE_PIXEL_SIZE) + 3] = bmpHeader->bV4BlueMask  | pixArray[i];
    }

CLEANUP:
    Free(al, fb, fs.QuadPart);
CLOSE_FH:
    CloseHandle(fh);
    return result;
}

