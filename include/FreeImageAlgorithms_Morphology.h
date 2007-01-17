#ifndef __FREEIMAGE_ALGORITHMS_MEDIAN_FILTER__
#define __FREEIMAGE_ALGORITHMS_MEDIAN_FILTER__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
 *	Dilates the particles in an image.
 *
 *  \param src FIBITMAP bitmap to perform the dilation operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Dilation(FIABITMAP src);

/*! \file 
 *	Erodes the particles in an image.
 *
 *  \param src FIBITMAP bitmap to perform the erosion operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Erosion(FIABITMAP src);

#ifdef __cplusplus
}
#endif

#endif