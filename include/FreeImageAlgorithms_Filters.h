#ifndef __FREEIMAGE_ALGORITHMS_FILTERS__
#define __FREEIMAGE_ALGORITHMS_FILTERS__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
 *	Provides a median filter function.
 *
 *  \param src FIBITMAP bitmap to perform the convolution on.
 *  \param kernel_x_radius for a kernel of width 3 the x radius would be 1.
 *  \param kernel_y_radius for a kernel of height 3 the y radius would be 1.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_MedianFilter(FIABITMAP* src, int kernel_x_radius, int kernel_y_radius);

/** \brief Perform a soble filtering.
 *
 *  \param src FIBITMAP bitmap to perform the sobel filter on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Sobel(FIBITMAP *src);

#ifdef __cplusplus
}
#endif

#endif