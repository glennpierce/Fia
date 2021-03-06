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

#ifndef __FREEIMAGE_ALGORITHMS_FILTERS__
#define __FREEIMAGE_ALGORITHMS_FILTERS__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	FIA_BINNING_SQUARE,
	FIA_BINNING_CIRCULAR,
	FIA_BINNING_GAUSSIAN

} FIA_BINNING_TYPE;

/*! \file 
 *	Provides a median filter function.
 *
 *  \param src FIBITMAP bitmap to perform the convolution on.
 *  \param kernel_x_radius for a kernel of width 3 the x radius would be 1.
 *  \param kernel_y_radius for a kernel of height 3 the y radius would be 1.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_MedianFilter(FIABITMAP* src, int kernel_x_radius, int kernel_y_radius);

/** \brief Perform a sobel filtering.
 *
 *  \param src FIBITMAP bitmap to perform the sobel filter on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_Sobel(FIBITMAP *src);

#define SOBEL_HORIZONTAL 1     // (0000 0001)
#define SOBEL_VERTICAL   2     // (0000 0010) 
#define SOBEL_MAGNITUDE  4     // (0000 0100) 

/** \brief Perform a advanced sobel filtering.
 *
 *  Return images can be NULL. If not the image has to be freed by the user.
 *
 *  \param src FIBITMAP bitmap to perform the sobel filter on.
 *  \param vertical FIBITMAP return image of the vertical stage of the sobel filter, can be NULL.
 *  \param horizontal FIBITMAP return image of the horizontal stage of the sobel filter, can be NULL.
 *  \param magnitude FIBITMAP return image of the magnitude image from both directions, can be NULL.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API int DLL_CALLCONV
FIA_SobelAdvanced(FIBITMAP *src,
                                  FIBITMAP** vertical,
                                  FIBITMAP** horizontal,
                                  FIBITMAP** magnitude);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_Binning (FIBITMAP * src, FIA_BINNING_TYPE, int radius);

#ifdef __cplusplus
}
#endif

#endif
