#include "StdAfx.h"
#include "Algorithms.h"
#include <math.h>

namespace DuiLib
{
    bool UIAlgorithm::gaussBlur(int *data, int width, int height, double sigma, int radius)   
    {   
        double *gaussMatrix, gaussSum = 0.0, _2sigma2 = 2 * sigma * sigma;   
        int x, y, xx, yy, xxx, yyy;   
        double *pdbl, a, r, g, b, d;   
        unsigned char *bbb, *pout, *poutb;   
        pout = poutb = (unsigned char *)LocalAlloc(LMEM_FIXED, width * height * 4);   
        if (!pout) return 0;   
        gaussMatrix = pdbl = (double *)LocalAlloc(LMEM_FIXED, (radius * 2 + 1) * (radius * 2 + 1) * sizeof(double));   
        if (!gaussMatrix) {   
            LocalFree(pout);   
            return false;   
        }   
        for (y = -radius; y <= radius; y++) {   
            for (x = -radius; x <= radius; x++) {   
                a = exp(-(double)(x * x + y * y) / _2sigma2);   
                *pdbl++ = a;   
                gaussSum += a;   
            }   
        }   
        pdbl = gaussMatrix;   
        for (y = -radius; y <= radius; y++) {   
            for (x = -radius; x <= radius; x++) {   
                *pdbl++ /= gaussSum;   
            }   
        }   
        for (y = 0; y < height; y++) {   
            for (x = 0; x < width; x++) {   
                a = r = g = b = 0.0;   
                pdbl = gaussMatrix;   
                for (yy = -radius; yy <= radius; yy++) {   
                    yyy = y + yy;   
                    if (yyy >= 0 && yyy < height) {   
                        for (xx = -radius; xx <= radius; xx++) {   
                            xxx = x + xx;   
                            if (xxx >= 0 && xxx < width) {   
                                bbb = (unsigned char *)&data[xxx + yyy * width];   
                                d = *pdbl;   
                                b += d * bbb[0];   
                                g += d * bbb[1];   
                                r += d * bbb[2];   
                                a += d * bbb[3];   
                            }   
                            pdbl++;   
                        }   
                    } else {   
                        pdbl += (radius * 2 + 1);   
                    }   
                }   
                *pout++ = (unsigned char)b;   
                *pout++ = (unsigned char)g;   
                *pout++ = (unsigned char)r;   
                *pout++ = (unsigned char)a;   
            }   
        }   
        RtlMoveMemory(data, poutb, width * height * 4);   
        LocalFree(gaussMatrix);   
        LocalFree(poutb);   
        return true;   
    }  

	bool UIAlgorithm::gaussBlur(HWND hWnd, HDC hdcCompatible, HBITMAP hbm,  BlurEffectParam* param)
    {
        RECT rect;
        ::GetWindowRect(hWnd, &rect);

        int w = rect.right - rect.left;
        int h = rect.bottom - rect.top;

        int * data = new int[w * h];

        BITMAPINFO infoHeader;
        memset(&infoHeader.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
        infoHeader.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        infoHeader.bmiHeader.biWidth = w;
        infoHeader.bmiHeader.biHeight = -h;
        infoHeader.bmiHeader.biPlanes = 1;
        infoHeader.bmiHeader.biBitCount = 32; // 这里建议用32，经过测试在我的机器上比24速度快比较多
        infoHeader.bmiHeader.biCompression = BI_RGB;	

        ::GetDIBits(hdcCompatible, hbm, 0, h, data, &infoHeader, DIB_RGB_COLORS);
        if(!gaussBlur(data, w, h, param->dSigma, param->nRadius))
            return false;

        SetDIBits(hdcCompatible, hbm, 0, abs(infoHeader.bmiHeader.biHeight), data, &infoHeader, DIB_RGB_COLORS);  

        return true;
    }

}
