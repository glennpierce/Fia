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

#ifndef __FREEIMAGE_ALGORITHMS_LINEAR_SCALE__
#define __FREEIMAGE_ALGORITHMS_LINEAR_SCALE__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file
	Provides linear scaling of greylevel images.
*/

/** \brief Convert image of any type to a standard 8-bit greyscale image.
 *
 *  For standard images, a clone of the input image is returned.
 *  When the scale_linear parameter is TRUE, conversion is done by scaling linearly
 *  each pixel to an integer value between [0..255]. When it is FALSE, conversion is done
 *  by rounding each float pixel to an integer between [0..255]
 *  \param src Image to convert
 *  \param min Min value to stretch to.
 *  \param max Max value to stretch to.
 *  \param min_within_image Mininum value to found in original image.
 *  \param max_within_image Maximum value to found in original image.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_LinearScaleToStandardType(FIBITMAP *src, double min, double max, double *min_within_image, double *max_within_image);

/** \brief Stretches an image to the full range of an image type.
 *
 *  Stretches an image to the full range of greyscale values possible for the
 *  parameter type.
 *  \param src Image to convert
 *  \param type Type to stretch to.
 *  \param max Max value to stretch to.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_StretchImageToType(FIBITMAP *src, FREE_IMAGE_TYPE type, double max);


DLL_API FIBITMAP* DLL_CALLCONV
FIA_StretchImageAcrossRange(FIBITMAP *src, double min, double max);

DLL_API int DLL_CALLCONV
FIA_InplaceLinearScaleToStandardType(FIBITMAP **src, double min, double max, double* found_min, double* found_max);

#ifdef __cplusplus
}
#endif

#endif
