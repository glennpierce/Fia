#ifndef __FREEIMAGE_ALGORITHMS_FFT__
#define __FREEIMAGE_ALGORITHMS_FFT__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_GetFFT(FIBITMAP *src, int inverse, int shift);

#ifdef __cplusplus
}
#endif

#endif