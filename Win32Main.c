#include "Win32Defs.h"

#include "cutils/UtTypes.h"

#include "cutils/UtAlloc.c"


#include "KeyboardInput.h"

#include "QPCTimings.c"
#include "Win32Input.c"
#include "Render2DCpu.c"

#define WIN_CLASS_NAME "SE_WC"

#define PLACEHOLDER_TARGET_FPS 60

static BOOL Running = TRUE;
static KeyboardKey Keyboard[KEYBOARD_SIZE];
static R2dSurface RenderSurface;

void OnResize(i32 width, i32 height) {
    if (NULL != RenderSurface.bmpData) {
        R2dDelTestRenderSurface(&RenderSurface);
    }

    RenderSurface = R2dMkTestRenderSurface(RenderSurface.allocator, width, height);
}

// Custom handeling function for OS messages
BOOL HandleWindowMsg(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    BOOL handled = TRUE;

    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC deviceContext = BeginPaint(window, &ps);
            R2dPaintWindow(&RenderSurface, deviceContext, &ps.rcPaint);
            EndPaint(window, &ps);            
        } break;
        case WM_KEYDOWN: {
            KeyboardKeyPressed(Keyboard, wParam);
        } break;
        case WM_KEYUP: {
            KeyboardKeyReleased(Keyboard, wParam);
        } break;
        case WM_SIZE: {
            //TODO: Handle type of size msg (wParam)
            OnResize(LOWORD(lParam), HIWORD(lParam));
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

    if (FALSE == HandleWindowMsg(window, msg, wParam, lParam)) {
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

    R2dPaintWindow(&RenderSurface, deviceContext, &clientRect);

    ReleaseDC(winHandle, deviceContext);
}

void ApplicationLoop(HWND winHandle, AL *sysAlloc, i64 targetFps) {
    QPCTimings timings = MkQPCTimings();
    InitKeyboard(Keyboard);

    u32 *color = Alloc(sysAlloc, sizeof(u32) * 10000);
    for (usize i = 0; i < 10000; i++) {
        color[i] = 0xFFEAEAEA;
    }

    MSG msg;
    while (Running) {

        QPCFrameStart(&timings);

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            WinMsgCallback(msg.hwnd, msg.message, msg.wParam, msg.lParam);
        }

        R2dClearSurface(&RenderSurface, 0xFFAAFFAA);

        R2dRenderSquare(&RenderSurface, 10, 10, color, 100, 100);

        R2dClearSquare(&RenderSurface, 500, 400, 0xFFEE0000, 300, 300);

        ApplicationPaint(winHandle);
        
        QPCFrameEnd(&timings);

        timings.elapsedTimeMicroSeconds.QuadPart += TickSleep(targetFps, timings.elapsedTimeMicroSeconds.QuadPart);
    }
}

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR cmd, int showCode) {
    AL sysAlloc = AlMakeWin32Alloc();

    RenderSurface = R2dMkTestRenderSurface(&sysAlloc, 100, 100);

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
