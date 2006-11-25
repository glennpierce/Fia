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

/*
 * This function finds the the amount of white ie area in a monochrome image.
 * This works with 8 bit images by assuming everything above 1 is white.
 */
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_MonoImageFindWhiteFraction(FIBITMAP *src, double *white_area, double *black_area);

// This function determines how a detail is present though two images.
// tp (True Positive) A detail present in src is also in result.
// tn (True Negative) A detail not in src is not in result ie two pixels that are 0.
// fp (False Positive) A detail not in src is in result.
// fn (False Negative) A detail in src is not in result.
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_MonoTrueFalsePositiveComparison(FIBITMAP *src, FIBITMAP *result,
													int *tp, int *tn, int *fp, int *fn);

#ifdef __cplusplus
}
#endif

#endif