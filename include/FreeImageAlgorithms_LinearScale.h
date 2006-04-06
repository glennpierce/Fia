#ifndef __FREEIMAGE_ALGORITHMS_LINEAR_SCALE__
#define __FREEIMAGE_ALGORITHMS_LINEAR_SCALE__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LinearScaleToStandardType(FIBITMAP *src, double min, double max, double *min_within_image, double *max_within_image);


#ifdef __cplusplus
}
#endif

#endif