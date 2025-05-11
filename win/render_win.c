#include "render.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>

struct _device_ctx {
    HWND hwnd;
    HDC sec;
    HDC pri;
};

LRESULT CALLBACK __win_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

struct _device_ctx *_render_build_device(const int device_left, const int device_top,
                                         const int device_width, const int device_height,
                                         const char *device_name)
{
    WNDCLASS wndClass = {CS_BYTEALIGNCLIENT, (WNDPROC)__win_proc, 0, 0, 0, NULL, NULL, NULL, NULL,
                         device_name};
    wndClass.hInstance = GetModuleHandle(NULL);

    if (!RegisterClass(&wndClass))
    {
        return NULL;
    }
    HWND window = CreateWindow(device_name, device_name,
                               WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 0, 0,
                               NULL, NULL, wndClass.hInstance, NULL);
    if (window == NULL)
    {
        return NULL;
    }
    HDC sec = GetDC((window));
    HDC pri = CreateCompatibleDC(sec);

    BITMAPINFO bitmapInfo = {{sizeof(BITMAPINFOHEADER), device_width, -device_height, 1, 32, BI_RGB,
                              device_width * device_height * 4, 0, 0, 0, 0}};
    LPVOID ptr;
    HBITMAP bitmapHandler = CreateDIBSection(pri, &bitmapInfo, DIB_RGB_COLORS, &ptr, 0, 0);
    if (bitmapHandler == NULL)
    {
        return NULL;
    }

    SetWindowPos(window, NULL, device_left, device_top, device_width, device_height,
                 (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));

    ShowWindow(window, SW_NORMAL);
    UpdateWindow(window);
    struct _device_ctx *res = (struct _device_ctx *)malloc(sizeof(struct _device_ctx));
    res->hwnd = window;
    res->sec = sec;
    res->pri = pri;
    return res;
}

render_ctx *render_new(int left, int top, int width, int height)
{
    render_ctx *ctx = (render_ctx *)malloc(sizeof(render_ctx));
    ctx->left = left;
    ctx->top = top;
    ctx->width = width;
    ctx->height = height;
    ctx->device = _render_build_device(left, top, width, height, "test");
    return ctx;
}

void render_loop(render_ctx *ctx)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void render_free(render_ctx *ctx)
{
    free(ctx->device);
    free(ctx);
}

void render_set_pixel(render_ctx *ctx, int x, int y,  color c)
{
    struct _device_ctx *dev = (struct _device_ctx *)ctx->device;
    SetPixel(dev->sec, x, y, RGB(255 * c.r, 255 * c.g, 255 * c.b));
}