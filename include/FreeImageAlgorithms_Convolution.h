/* 
 * Copyright 2007-2010 Glenn Pierce, Paul Barber,
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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

typedef enum {CORRELATION_KERNEL, CORRELATION_FFT} CorrelationType;

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
FIA_KernelCorrelateImages(FIBITMAP *src1, FIBITMAP *src2, FIARECT search_area, FIBITMAP *mask,
						  CORRELATION_PREFILTER filter, FIAPOINT *pt, double *max);

/** \brief Correlate two regions from two two images
 *
 *  \param src1 FIBITMAP Background bitmap to perform the correlation on.
 *  \param rect1 FIARECT The region of background bitmap to perform the correlation on.
 *	\param src2 FIBITMAP Src bitmap to perform the correlation on.
 *  \param rect2 FIARECT The region of src bitmap to perform the correlation on.
 *  \param search_rect FIARECT The area the src image is shifted over the background image.
		If we know where approx the image should be placed. The rectangle is relative to the background image.
 *  \param mask FIBITMAP A mask of the area the src image is shifted over the background image.
		If we know where approx the image should be placed. The rectangle is relative to the background image.
 *  \param pt FIAPOINT The point where the src image should be place relative to the background image.
 *  \param max double The correlation factor found. Close to 1.0 the better the correlation.
 *  \return FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_KernelCorrelateImageRegions(FIBITMAP *src1, FIARECT rect1, FIBITMAP *src2,  FIARECT rect2,
        FIARECT search_rect, FIBITMAP *mask, CORRELATION_PREFILTER filter, FIAPOINT *pt, double *max);

DLL_API int DLL_CALLCONV
FIA_FFTCorrelateImages(FIBITMAP *src1, FIBITMAP *src2,
				CORRELATION_PREFILTER filter, FIAPOINT *pt);

DLL_API int DLL_CALLCONV
FIA_FFTCorrelateImageRegions(FIBITMAP * src1, FIARECT rect1, FIBITMAP * src2,
        FIARECT rect2, CORRELATION_PREFILTER filter, FIAPOINT * pt);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_PreCalculateCorrelationFFT(FIBITMAP *_src1, FIBITMAP *_src2, int pad_size, CORRELATION_PREFILTER filter);

DLL_API int DLL_CALLCONV
FIA_FFTCorrelateImageWithPreCorrelationFFT(FIBITMAP * fft_fib, FIBITMAP *_src1, FIBITMAP *_src2, int pad_size,
        CORRELATION_PREFILTER filter, FIAPOINT * pt);

DLL_API FIBITMAP* __cdecl
FIA_EdgeDetect(FIBITMAP *src);


DLL_API int DLL_CALLCONV
FIA_CorrelateImages(FIBITMAP * _src1, FIBITMAP * _src2, CorrelationType type,
        CORRELATION_PREFILTER filter, FIAPOINT * pt);

DLL_API int DLL_CALLCONV
FIA_CorrelateImageRegions(FIBITMAP * src1, FIARECT region1, FIBITMAP * src2, FIARECT region2,
        CorrelationType type, CORRELATION_PREFILTER filter, FIAPOINT *pt);


#ifdef __cplusplus
}
#endif

#endif
