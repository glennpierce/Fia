/*
 * Copyright 2007 Glenn Pierce
 *
 * This file is part of FreeImageAlgorithms.
 *
 * FreeImageAlgorithms is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeImageAlgorithms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 * 
 * You should have received a copy of the Lesser GNU General Public License
 * along with FreeImageAlgorithms.  If not, see <http://www.gnu.org/licenses/>.
*/

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

typedef FIBITMAP* (__cdecl *CORRELATION_PREFILTER) (FIBITMAP*);

/** \brief Create a kernel.
 *
 *  \param x_radius for a kernel of width 3 the x radius would be 1.
 *  \param y_radius for a kernel of height 3 the y radius would be 1.
 *  \param values Array representing the values of the kernel.
 *  \param divider The divider of the kernel);
 *  \return FilterKernel.
*/
DLL_API FilterKernel DLL_CALLCONV
FIA_NewKernel(int x_radius, int y_radius, 
							  const double *values, double divider);

/** \brief Convolve and image with a kernel.
 *
 *  \param src FIBITMAP bitmap to perform the convolution on.
 *  \param kernel FilterKernel The kernel created with FIA_NewKernel.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_Convolve(FIABITMAP *src, const FilterKernel kernel);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_SeparableConvolve(FIABITMAP *src, FilterKernel horz_kernel, FilterKernel vert_kernel);

DLL_API int DLL_CALLCONV
FIA_CorrelateImages(FIBITMAP *src1, FIBITMAP *src2, FIAPOINT *pt, double *max);

DLL_API int DLL_CALLCONV
FIA_CorrelateImageRegions(FIBITMAP *src1, FIARECT rect1, FIBITMAP *src2,  FIARECT rect2,
                FIAPOINT *pt, double *max);

DLL_API int DLL_CALLCONV
FIA_CorrelateImagesAlongRightEdge(FIBITMAP *src1, FIBITMAP *src2,
                unsigned int edge_thickness, FIAPOINT *pt, double *max);

DLL_API int DLL_CALLCONV
FIA_CorrelateImagesAlongBottomEdge(FIBITMAP *src1, FIBITMAP *src2,
                unsigned int edge_thickness, FIAPOINT *pt, double *max);

DLL_API int DLL_CALLCONV
FIA_CorrelateImagesFFT(FIBITMAP *src1, FIBITMAP *src2,
				CORRELATION_PREFILTER filter, FIAPOINT *pt, double *max);

DLL_API int DLL_CALLCONV
FIA_FFTCorrelateImagesAlongRightEdge(FIBITMAP *src1, FIBITMAP *src2, CORRELATION_PREFILTER filter,
                unsigned int edge_thickness, FIAPOINT *pt);
       
DLL_API FIBITMAP* __cdecl
FIA_EdgeDetect(FIBITMAP *src);

#ifdef __cplusplus
}
#endif

#endif
