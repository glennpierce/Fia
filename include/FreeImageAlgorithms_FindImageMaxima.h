#ifndef __FREEIMAGE_ALGORITHMS_FIND_MAXIMA__
#define __FREEIMAGE_ALGORITHMS_FIND_MAXIMA__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
*	Provides methods to find maxima in an image.
*/ 

typedef struct
{
  POINT centre;
  float value;

} FIAPeak;

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FindImageMaxima(FIBITMAP* src, FIBITMAP *mask, unsigned char threshold, int min_separation, FIAPeak **peaks, int number, int *peaks_found);


#ifdef __cplusplus
}
#endif

#endif