#ifndef __UIALGORITHM_H__
#define __UIALGORITHM_H__

#include "Effects.h"

namespace DuiLib
{

    class UIAlgorithm
    {
    public:
		bool gaussBlur(HWND hWnd, HDC hdcCompatible, HBITMAP hbm,  BlurEffectParam* param);

    private:
        bool gaussBlur(int *data, int width, int height, double sigma, int radius);
    };
}

#endif  // __UIALGORITHM_H__