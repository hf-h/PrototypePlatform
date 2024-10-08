#include "Win32Defs.h"

#include "cutils/UtTypes.h"

#include "cutils/UtMath.c"
#include "cutils/UtAlloc.c"
#include "cutils/UtString.c"


#include "KeyboardInput.h"

#include "QPCTimings.c"

#include "Win32Input.c"

#include "Win32Render2dCpu.c"
#include "BMPIO.c"

#define WIN_CLASS_NAME "SE_WC"

#define PLACEHOLDER_TARGET_FPS 60

////Mouse controlled box select test / example///////

// not a good way to implement it generally
typedef struct SelectionRect {
    i32 sx, sy, ex, ey;
    i32 top, left;
    i32 width, height;
} SelectionRect;

void ProcessSelection(SelectionRect *rect) {
    if (rect->sx < rect->ex) {
        rect->left = rect->sx;
        rect->width = rect->ex - rect->sx;
    } else {
        rect->left = rect->ex;
        rect->width = rect->sx - rect->ex;
    }

    if (rect->sy < rect->ey) {
        rect->top = rect->sy;
        rect->height = rect->ey - rect->sy;
    } else {
        rect->top = rect->ey;
        rect->height = rect->sy - rect->ey;
    }
}

static BOOL InSelectionMode = FALSE;
static SelectionRect Selection = {0};

void DebugOnLMouseDown(i32 x, i32 y) {
    InSelectionMode = TRUE;

    Selection.sx = x;
    Selection.sy = y;
    Selection.ex = 0;
    Selection.ey = 0;
}

void DebugOnLMouseUp(i32 x, i32 y) {
    InSelectionMode = FALSE;

    Selection.ex = x;
    Selection.ey = y;
    ProcessSelection(&Selection);
}

void DebugOnMouseMove(i32 x, i32 y) {
    Selection.ex = x;
    Selection.ey = y;
    ProcessSelection(&Selection);
}

//////////////////////////////////////////////////

static BOOL Running = TRUE;
static KeyboardKey Keyboard[KEYBOARD_SIZE];
static R2dSurface RenderSurface;
static R2dTarget RenderTarget;

void OnResize(u64 resizeType, i32 width, i32 height) {
    switch (resizeType) {
        case SIZE_MINIMIZED: {
        } break;
        default: {
            if (NULL != RenderSurface.bmpData) {
                R2dDelTestRenderSurface(&RenderSurface);
            }
            if (NULL != RenderTarget.data) {
                R2dDelRenderTarget(&RenderTarget);
            }

            RenderSurface = R2dMkTestRenderSurface(RenderSurface.allocator, width, height);
            RenderTarget = R2dMkRenderTarget(RenderTarget.allocator, &RenderSurface);
        }
    }
}

// Custom handeling function for OS messages
BOOL HandleWindowMsg(HWND window, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result) {
    BOOL handled = TRUE;

    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC deviceContext = BeginPaint(window, &ps);
            R2dPaintWindow(&RenderTarget, &RenderSurface, deviceContext, &ps.rcPaint);
            EndPaint(window, &ps);            
        } break;
        case WM_KEYDOWN: {
            KeyboardKeyPressed(Keyboard, wParam);
        } break;
        case WM_KEYUP: {
            KeyboardKeyReleased(Keyboard, wParam);
        } break;
        case WM_SIZE: {
            OnResize(wParam, LOWORD(lParam), HIWORD(lParam));
        } break;
        case WM_LBUTTONDOWN: {
            DebugOnLMouseDown(LOWORD(lParam), HIWORD(lParam));
            *result = 0;
        } break;
        case WM_LBUTTONUP: {
            DebugOnLMouseUp(LOWORD(lParam), HIWORD(lParam));
            *result = 0;
        } break;
        case WM_MOUSEMOVE: {
            if (InSelectionMode) {
                DebugOnMouseMove(LOWORD(lParam), HIWORD(lParam));
                *result = 0;
            } else {
                handled = FALSE;
            }
        } break;            
        case WM_DESTROY: {
            Running = FALSE;
        } break;
        case WM_CLOSE: {
            Running = FALSE;
        } break;
        default: {
            handled = FALSE;
        }
    }

    return handled;
}

// Callback for OS messages
LRESULT CALLBACK WinMsgCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 1;

    if (FALSE == HandleWindowMsg(window, msg, wParam, lParam, &result)) {
        // If we did not handle the msg leave it to the default window proc
        result = DefWindowProcA(window, msg, wParam, lParam);
    }

    return result;
}

i64 TickSleep(i64 targetFps, i64 timeElapsedMicroS) {
    i64 targetTickLen = 1000000 / targetFps;
    i64 sleepMicro = targetTickLen - timeElapsedMicroS;
    i64 sleepTimeMilli = sleepMicro / 1000;

    if (0 >= sleepTimeMilli) {
        return 0;
    }

    Sleep(sleepTimeMilli);

    return sleepMicro;
}

void ApplicationPaint(HWND winHandle) {
    HDC deviceContext = GetDC(winHandle);

    RECT clientRect;
    GetClientRect(winHandle, &clientRect);

    R2dPaintWindow(&RenderTarget, &RenderSurface, deviceContext, &clientRect);

    ReleaseDC(winHandle, deviceContext);
}

void ApplicationLoop(HWND winHandle, AL *sysAlloc, i64 targetFps) {
    QPCTimings timings = MkQPCTimings();
    InitKeyboard(Keyboard);

    AL temp_alloc = AlMakeArena(sysAlloc, Megabyte(10));

    // Asset loading tests
    Sprite2D *sprite;
    BMPIOReadFromFile(&temp_alloc, sysAlloc, "test.bmp", &sprite);

    //Clear(&temp_alloc);

    PixColor bgColor = PixColorFromHex(0xFFAAFFAA);

    PixColor sq1Color = PixColorFromHex(0xFFFF0000);
    PixColor sq2Color = PixColorFromHex(0xFF00FF00);
    PixColor sq3Color = PixColorFromHex(0xFF0000FF);

    PixColor selectionColor = PixColorFromHex(0xFFEE00FF);

    MSG msg;
    while (Running) {

        QPCFrameStart(&timings);

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            WinMsgCallback(msg.hwnd, msg.message, msg.wParam, msg.lParam);
        }

        // Render test
        R2dClearTarget(&RenderTarget, bgColor);
        R2dDebugClearSquare(&RenderTarget, 500, 400,  sq1Color, 300, 300);
        R2dDebugClearSquare(&RenderTarget, 800, 400,  sq2Color, 300, 300);
        R2dDebugClearSquare(&RenderTarget, 1100, 400, sq3Color, 300, 300);

        R2dRenderSquare(&RenderTarget, 0, 0, sprite->data, sprite->width, sprite->height);
        R2dDebugRenderSpriteWithSetAlpha(&RenderTarget, 1000, 0, *sprite, 0.4f);

        R2dDebugClearSquare(&RenderTarget, Selection.left, Selection.top, selectionColor, Selection.width, Selection.height);        

        ApplicationPaint(winHandle);
        
        QPCFrameEnd(&timings);

        timings.elapsedTimeMicroSeconds.QuadPart += TickSleep(targetFps, timings.elapsedTimeMicroSeconds.QuadPart);
    }
}

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR cmd, int showCode) {
    AL sysAlloc = AlMakeWin32Alloc();

    RenderSurface = R2dMkTestRenderSurface(&sysAlloc, 100, 100);
    RenderTarget = R2dMkRenderTarget(&sysAlloc, &RenderSurface);

    WNDCLASS wc = {0};
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WinMsgCallback;
    wc.hInstance = instance;
    wc.lpszClassName = WIN_CLASS_NAME;

    if (FALSE == RegisterClass(&wc)) {
        OutputDebugStringA("Failed to register window class\n");
        return 1;
    }

    HWND windowHandle = CreateWindowEx(0
                                       , WIN_CLASS_NAME
                                       , "PLACEHOLDER"
                                       , WS_OVERLAPPEDWINDOW | WS_VISIBLE
                                       , CW_USEDEFAULT
                                       , CW_USEDEFAULT
                                       , CW_USEDEFAULT
                                       , CW_USEDEFAULT
                                       , 0
                                       , 0
                                       , instance
                                       , 0);
    if (NULL == windowHandle) {
        OutputDebugStringA("Failed to create window\n");
        return 1;
    }

    ApplicationLoop(windowHandle, &sysAlloc, PLACEHOLDER_TARGET_FPS);

    return 0;
}
