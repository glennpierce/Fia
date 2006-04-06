#ifndef __FREEIMAGE_ALGORITHMS_HBITMAP__
#define __FREEIMAGE_ALGORITHMS_HBITMAP__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

DLL_API HBITMAP DLL_CALLCONV
FreeImageAlgorithms_FibToHBitmap(FIBITMAP *dib);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FreeHBitmap(HBITMAP hbitmap);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_HBitmapToFIB(HDC hdc, HBITMAP bitmap);

DLL_API HBITMAP DLL_CALLCONV 
FreeImageAlgorithms_GetDibSection(FIBITMAP *src, HDC hdc, int left, int top, int right, int bottom);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_CopyToDibSection(FIBITMAP *src, HBITMAP hbitmap, int left, int top, int right, int bottom);

DLL_API HBITMAP DLL_CALLCONV
FreeImageAlgorithms_CreateDibSection(HDC hdc, int width, int height, int bpp, int colours_used, RGBQUAD* bmiColours);

#ifdef __cplusplus
}
#endif

#endif