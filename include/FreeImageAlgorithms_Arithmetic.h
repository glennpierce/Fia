#ifndef __FREEIMAGE_ALGORITHMS_ARITHMETIC__
#define __FREEIMAGE_ALGORITHMS_ARITHMETIC__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Transpose(FIBITMAP *src);

/*
Adds src bitmap to dst. dst must have a higher bpp than src to accomedate the added values.
Ie dst can be a FLOAT image and src 16bpp. If constant is no zero that is added instead 
of the src bitmap. */
DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_Add(FIBITMAP* dst, FIBITMAP* src, double constant);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_Divide(FIBITMAP* dst, FIBITMAP* src, double constant);

#ifdef __cplusplus
}
#endif

#endif