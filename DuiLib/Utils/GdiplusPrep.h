#pragma once
#include <GdiPlus.h>
#pragma comment( lib, "GdiPlus.lib" )

class UILIB_API Gdiplus::RectF;
struct UILIB_API Gdiplus::GdiplusStartupInput;

class UILIB_API GdiplusPrep
{
public:
    GdiplusPrep(void);
    ~GdiplusPrep(void);

    Gdiplus::GdiplusStartupInput gdiplusStartIn;
    ULONG_PTR gdiplusToken;
};
