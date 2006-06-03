#ifndef __FREEIMAGE_ALGORITHMS_ARITHMETIC__
#define __FREEIMAGE_ALGORITHMS_ARITHMETIC__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Transpose(FIBITMAP *src);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_MultiplyGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_DivideGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_AddGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_SubtractGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_MultiplyGreyLevelImageConstant(FIBITMAP* dst, double constant);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_DivideGreyLevelImageConstant(FIBITMAP* dst, double constant);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_AddGreyLevelImageConstant(FIBITMAP* dst, double constant);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_SubtractGreyLevelImageConstant(FIBITMAP* dst, double constant);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_ComplexConjugate(FIBITMAP* src);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_MultiplyComplexImages(FIBITMAP* dst, FIBITMAP* src);

#ifdef __cplusplus
}
#endif

#endif