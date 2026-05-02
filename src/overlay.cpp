#include "overlay.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

bool Overlay::init()
{
    window = FindWindowA("CEF-OSC-WIDGET", "NVIDIA GeForce Overlay");

    if (!window)
        return false;

    auto info = GetWindowLongA(window, -20);

    if (!info)
        return false;

    auto attrchange = SetWindowLongPtrA(window, -20, (LONG_PTR) (info | 0x20));

    if (!attrchange)
        return false;

    MARGINS margin;
    margin.cyBottomHeight = margin.cyTopHeight = margin.cxLeftWidth = margin.cxRightWidth = -1;

    if (DwmExtendFrameIntoClientArea(window, &margin) != S_OK)
        return false;

    if (!SetLayeredWindowAttributes(window, 0x000000, 0xFF, 0x02))
        return false;

    if (!SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, 0x0002 | 0x0001))
        return false;

    ShowWindow(window, SW_SHOW);

    return true;
}

bool Overlay::startup_d2d()
{
    RECT rc;
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);
    if (hr != S_OK || !d2d_factory)
        return false;

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&write_factory);
    if (hr != S_OK || !write_factory)
        return false;

    hr = write_factory->CreateTextFormat(font, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"en-us", &format);
    if (hr != S_OK || !format)
        return false;

    GetClientRect(window, &rc);
    hr = d2d_factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        D2D1::HwndRenderTargetProperties(window, D2D1::SizeU((UINT32)(rc.right - rc.left), (UINT32)(rc.bottom - rc.top))),
        &tar);
    if (hr != S_OK || !tar)
        return false;

    D2D1_BRUSH_PROPERTIES properties = {1.0f, D2D1::Matrix3x2F::Identity()};
    hr = tar->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), properties, &brush);
    if (hr != S_OK || !brush)
        return false;

    return true;
}

        void Overlay::begin_scene()
        {
            tar->BeginDraw();
        }

        void Overlay::end_scene()
        {
            tar->EndDraw();
        }

        void Overlay::clear_scene()
        {
            tar->Clear();
        }

void Overlay::draw_text(int x, int y, const char* text, D2D1::ColorF color, ...)
{
    char    buffer[4096];
    wchar_t out[2048];
    RECT    window_metrics;

    if (!tar || !format || !brush || !GetWindowRect(window, &window_metrics))
        return;

    va_list arg_list;
    va_start(arg_list, text);
    vsnprintf(buffer, sizeof(buffer), text, arg_list);
    va_end(arg_list);

    size_t length = strlen(buffer);
    if (length >= 2048)
        length = 2047;
    size_t n = mbstowcs(out, buffer, length);
    out[n != (size_t)-1 ? n : 0] = L'\0';

    brush->SetColor(color);
    float r = (float)(window_metrics.right - window_metrics.left);
    float b = (float)(window_metrics.bottom - window_metrics.top);
    tar->DrawText(out, (UINT32)wcslen(out), format, D2D1::RectF((float)x, (float)y, r, b), brush);
}