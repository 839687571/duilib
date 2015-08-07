#include "StdAfx.h"
#include "GdiplusPrep.h"

GdiplusPrep::GdiplusPrep(void)
{
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartIn, NULL);
}

GdiplusPrep::~GdiplusPrep(void)
{
    Gdiplus::GdiplusShutdown(gdiplusToken);
}
