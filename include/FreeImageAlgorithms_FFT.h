#ifndef __FREEIMAGE_ALGORITHMS_FFT__
#define __FREEIMAGE_ALGORITHMS_FFT__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FFT(FIBITMAP *src, int inverse, int shift);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_ConvertComplexImageToAbsoluteValued(FIBITMAP *src);

#ifdef __cplusplus
}
#endif

#endif