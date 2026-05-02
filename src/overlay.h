#pragma once
#include <d2d1.h>
#include <dwmapi.h>
#include <dwrite.h>
#include <string>
#include <vector>
#include <windows.h>

class Overlay
{
private:
    HWND window = nullptr;

    ID2D1Factory*          d2d_factory = nullptr;
    ID2D1HwndRenderTarget* tar = nullptr;
    IDWriteFactory*        write_factory = nullptr;
    ID2D1SolidColorBrush*  brush = nullptr;
    IDWriteTextFormat*     format = nullptr;

    const wchar_t* font;
    float          size;

public:
    Overlay(const wchar_t* Font, float Font_Size)
        : window(nullptr)
        , d2d_factory(nullptr)
        , tar(nullptr)
        , write_factory(nullptr)
        , brush(nullptr)
        , format(nullptr)
        , font(Font)
        , size(Font_Size)
    {
    }

    ~Overlay()
    {
        if (tar)
        {
            begin_scene();
            clear_scene();
            end_scene();
            tar->Release();
            tar = nullptr;
        }
        if (format)
        {
            format->Release();
            format = nullptr;
        }
        if (write_factory)
        {
            write_factory->Release();
            write_factory = nullptr;
        }
        if (brush)
        {
            brush->Release();
            brush = nullptr;
        }
        if (d2d_factory)
        {
            d2d_factory->Release();
            d2d_factory = nullptr;
        }
    }

    bool init();
    bool startup_d2d();

    void begin_scene();
    void end_scene();
    void clear_scene();

    /* create helper functions here */
    void draw_text(int x, int y, const char* text, D2D1::ColorF color, ...);
};