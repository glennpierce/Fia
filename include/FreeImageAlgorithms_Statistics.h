#ifndef __FREEIMAGE_ALGORITHMS_STATISTICS__
#define __FREEIMAGE_ALGORITHMS_STATISTICS__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_HistEq_Random_Additions(FIBITMAP *src);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_HistEq(FIBITMAP *src);

DLL_API double DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelAverage(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_Histogram(FIBITMAP *src, double min, double max, int number_of_bins, unsigned long *hist);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_RGBHistogram(FIBITMAP *src, double min, double max, int number_of_bins,
			unsigned long *rhist, unsigned long *ghist, unsigned long *bhist);

#ifdef __cplusplus
}
#endif

#endif