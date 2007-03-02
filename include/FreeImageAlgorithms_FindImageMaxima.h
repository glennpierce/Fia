#ifndef __FREEIMAGE_ALGORITHMS_FIND_MAXIMA__
#define __FREEIMAGE_ALGORITHMS_FIND_MAXIMA__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
*	Provides methods to find maxima in an image.
*/ 

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FindImageMaxima(FIBITMAP* src, FIBITMAP *mask, unsigned char threshold, int min_separation);

typedef struct
{
  POINT centre;
  float value;

} FIAPeak;

// you can specify the top n to find with number
// maxima above the threshold are found and the actual number found is returned
// the find all above the threshold, send number=0
// if threshold<0 it will default to 10.0

//int findImageMaxima_imageOut (IPIImageRef image, IPIImageRef mask,  int number, double threshold_in, int minSeparation, GlPeak **pCentresIn, IPIImageRef imageOut);
//int findImageMaxima_Thresholded (IPIImageRef image, IPIImageRef mask,  int number, double threshold_in, int minSeparation, GlPeak **pCentres);
//int findImageMaxima (IPIImageRef image, IPIImageRef mask,  int number, GlPeak *centres);


#ifdef __cplusplus
}
#endif

#endif