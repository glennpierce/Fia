#ifndef __FREEIMAGE_ALGORITHMS_MEDIAN_FILTER__
#define __FREEIMAGE_ALGORITHMS_MEDIAN_FILTER__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
 *	Fills the particles in an image.
 *
 *  \param src FIBITMAP bitmap to perform the fillhole operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Dilation(FIABITMAP src);

#ifdef __cplusplus
}
#endif

#endif