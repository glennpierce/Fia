#ifndef __FREEIMAGE_ALGORITHMS_CONVOLUTION__
#define __FREEIMAGE_ALGORITHMS_CONVOLUTION__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
*	Provides a convolution function.
*/

typedef struct
{
	int x_radius;
	int y_radius;
	const double *values;
	double divider;

} FilterKernel;

DLL_API FilterKernel DLL_CALLCONV
FreeImageAlgorithms_NewKernel(int x_radius, int y_radius, 
							  const double *values, double divider);

/** \brief Convolve and image with a kernel.
 *
 *  \param src FIBITMAP bitmap to perform the convolution on.
 *  \param kernel_x_radius for a kernel of width 3 the x radius would be 1.
 *  \param kernel_y_radius for a kernel of height 3 the y radius would be 1.
 *  \param kernel array representing the values of the kernel.
 *  \param divider The divider of the kernel);
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve(FIABITMAP *src, const FilterKernel kernel);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_SeparableConvolve(FIABITMAP *src, FilterKernel kernel1, FilterKernel kernel2);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Sobel(FIBITMAP *src);

#ifdef __cplusplus
}
#endif

#endif