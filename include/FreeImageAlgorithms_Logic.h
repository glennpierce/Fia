/*
 * Copyright 2007-2010 Glenn Pierce, Paul Barber,
 * Oxford University (Gray Institute for Radiation Oncology and Biology) 
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

#ifndef __FREEIMAGE_ALGORITHMS_LOGIC__
#define __FREEIMAGE_ALGORITHMS_LOGIC__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
	Provides various logic methods.
*/ 

/** \brief Masks an image.
 *
 *  This function masks the image data.
 *
 *  \param src FIBITMAP bitmap to mask.
 *  \param mask FIBITMAP bitmap used as mask. Must be an 8bit image.
*/
DLL_API int DLL_CALLCONV 
FIA_MaskImage(FIBITMAP* src, FIBITMAP* mask);

/** \brief Masks an image.
 *
 *  Reverses a mask image. Changes 0 pixels to foreground value and others to 0.
 *
 *  \param mask FIBITMAP Mask bitmap to mask.
 *  \param foreground_val int new value for the foreground pixel.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_ReverseMaskImage(FIBITMAP* mask, unsigned char foreground_val);

#ifdef __cplusplus
}
#endif

#endif
