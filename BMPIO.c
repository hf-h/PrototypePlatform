#include "cutils/UtTypes.h"
#include "cutils/UtMem.h"
#include "cutils/UtAlloc.h"

#include "Render2dCPUDefs.h"

#include "Windows.h"

#define BMP_IHEADER_SIZE 14

typedef u64 BMP_WRITE_ERR;

#define BMP_WRITE_ERR_NO_ERR             0x0000
#define BMP_WRITE_ERR_OPEN_FILE          0x0001
#define BMP_WRITE_ERR_ALLOCATION         0x0002
#define BMP_WRITE_ERR_WRITE_FILE         0x0003

usize InitGeneralBmpHeader(void *fileStart, u32 fileSize, u32 pixelArrayOffset) {
    u8 *writeHead = (u8 *)fileStart;
    usize offset = 0;

    // First 2 bytes must be the letters BM to identify this as a "moddern" bmp file
    writeHead[0] = 'B';
    writeHead[1] = 'M';

    offset += 2;

    // 4 bytes describing the bmp files size in bytes
    *(u32 *)(writeHead + offset) = fileSize;

    offset += 4;

    // 2 * 2 byte fields reserved for information from the appication that created the image, left as 0 here
    *(u32 *)(writeHead + offset) = 0;

    offset += 4;

    // Offset from the start of the file(I really hope its the start) to the start of the pixel array
    *(u32 *)(writeHead + offset) = pixelArrayOffset;

    offset += 4;

    return offset;
}

usize InitDIBHeader(void *dibStart, i32 width, i32 height) {
    ZeroMem(dibStart, sizeof(BITMAPV4HEADER));
    BITMAPV4HEADER *dibH = (BITMAPV4HEADER *)dibStart;

    dibH->bV4Size = sizeof(BITMAPV4HEADER);
    dibH->bV4Width = width;
    dibH->bV4Height = height;
    dibH->bV4Planes = 1;
    dibH->bV4BitCount = 32;

    dibH->bV4V4Compression = BI_BITFIELDS;
    dibH->bV4SizeImage = 0;
    dibH->bV4XPelsPerMeter = 0;
    dibH->bV4YPelsPerMeter = 0;
    dibH->bV4ClrUsed = 0;
    dibH->bV4ClrImportant = 0;

    dibH->bV4RedMask = 0xff0000;
    dibH->bV4GreenMask = 0xff00;
    dibH->bV4BlueMask = 0xff;
    dibH->bV4AlphaMask = 0xff000000u;

    return sizeof(BITMAPV4HEADER);
}

BMP_WRITE_ERR BMPIOPixelsToBmp(AL *al, u8 **mem, usize *memSize, u8 *pixelData, usize pixelDataSize, usize width, usize height) {
    const u32 fileSize = BMP_IHEADER_SIZE + sizeof(BITMAPV4HEADER) + pixelDataSize;

    *mem = Alloc(al, fileSize);
    if (NULL == mem) {
        return BMP_WRITE_ERR_ALLOCATION;
    }
    u8 *writeHead = *mem;
    *memSize = fileSize;

    writeHead += InitGeneralBmpHeader(writeHead, fileSize, BMP_IHEADER_SIZE + sizeof(BITMAPV4HEADER));

    ZeroMem(writeHead, sizeof(BITMAPV4HEADER));

    //DIB header
    writeHead += InitDIBHeader(writeHead, width, height);

    //Pixel array
    CopyMem(writeHead, pixelData, pixelDataSize);

    return BMP_WRITE_ERR_NO_ERR;
}

BMP_WRITE_ERR BMPIOWriteToFile(const char *fileName, void *data, usize dataSize) {
    BMP_WRITE_ERR result = BMP_WRITE_ERR_NO_ERR;
    const usize pixStride = sizeof(u32);

    HANDLE fh = CreateFileA(fileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
    if (INVALID_HANDLE_VALUE == fh) {
        result = BMP_WRITE_ERR_OPEN_FILE;
        goto CLEANUP;
    }

    DWORD bytesWritten = 0;
    if (FALSE == WriteFile(fh, data, dataSize, &bytesWritten, NULL)) {
        result = BMP_WRITE_ERR_WRITE_FILE;
        goto CLEANUP;
    }

    if (bytesWritten != dataSize) {
        result = BMP_WRITE_ERR_WRITE_FILE;
        goto CLEANUP;
    }

CLEANUP:
    CloseHandle(fh);
    return result;
}

typedef u64 BMP_READ_ERR;

#define BMP_READ_ERR_NO_ERR                             0x0000
#define BMP_READ_ERR_OPEN_FILE                          0x0001
#define BMP_READ_ERR_ALLOCATION                         0x0002
#define BMP_READ_ERR_READ_FILE                          0x0003
#define BMP_READ_ERR_INCOMPATIBLE_DIB_HEADER_VERIONS    0x0004
#define BMP_READ_ERR_NOT_A_BMP                          0x0005
#define BMP_READ_ERR_INCOMPATIBLE_FORMAT                0x0006
#define BMP_READ_ERR_FAILED_TO_GET_FILE_INFO            0x0007

DWORD _ShiftsToFirstByte(DWORD maskVal) {
    switch (maskVal) {
        case 0x000000FF:
            return 0;
        case 0x0000FF00:
            return 0x8;
        case 0x00FF0000:
            return 0x10;
        case 0xFF000000:
            return 0x18;
    }

    return 0;
}

BMP_READ_ERR BMPIOReadFromFile(AL *readAlloc, AL *saveAlloc, const char *filePath, Sprite2D **data) {
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

    u8 *fb = Alloc(readAlloc, fs.QuadPart);
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
    const usize dataSize = sizeof(Sprite2D) + (sizeof(PixColor) * pixCount);
    *data = Alloc(saveAlloc, dataSize);

    Sprite2D *sp = *data;
    sp->width  = bmpHeader->bV4Width;
    sp->height = bmpHeader->bV4Height;
    sp->data = (PixColor *)(*data) + sizeof(Sprite2D);

    u32 offsetToPix = *(u32 *)(fb + 0x0A);
    u32 *pixArray = (u32 *)(fb + offsetToPix);

    usize pixX = 0;
    usize pixY = sp->height - 1;

    usize bmpPixIdx = pixX + (pixY * sp->width);

    for (usize i = 0; i < sp->width * sp->height; i++) {
        DWORD colorValue;
        DWORD shiftAmount;

        colorValue = bmpHeader->bV4AlphaMask & pixArray[bmpPixIdx];
        shiftAmount = _ShiftsToFirstByte(bmpHeader->bV4AlphaMask);

        sp->data[i].A = (f32)(colorValue >> shiftAmount) / 255.f;

        colorValue = bmpHeader->bV4RedMask   & pixArray[bmpPixIdx];
        shiftAmount = _ShiftsToFirstByte(bmpHeader->bV4RedMask);

        sp->data[i].R = (f32)(colorValue >> shiftAmount) / 255.f;

        colorValue = bmpHeader->bV4GreenMask & pixArray[bmpPixIdx];
        shiftAmount = _ShiftsToFirstByte(bmpHeader->bV4GreenMask);

        sp->data[i].G = (f32)(colorValue >> shiftAmount) / 255.f;

        colorValue = bmpHeader->bV4BlueMask  & pixArray[bmpPixIdx];
        shiftAmount = _ShiftsToFirstByte(bmpHeader->bV4BlueMask);

        sp->data[i].B = (f32)(colorValue >> shiftAmount) / 255.f;

        pixX += 1;
        pixY -= pixX / sp->width;

        pixX = pixX % sp->width;

        bmpPixIdx = pixX + (pixY * sp->width);
    }

CLEANUP:
    Free(readAlloc, fb, fs.QuadPart);
CLOSE_FH:
    CloseHandle(fh);
    return result;
}

