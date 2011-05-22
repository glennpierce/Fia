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
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_ReverseMaskImage(FIBITMAP* mask, unsigned char foreground_val);

/** \brief Logical Or or Union.
 *
 *  Calculate the logical Or (union) or Nor.
 *
 *  \param src1 FIBITMAP First source image.
 *  \param src1 FIBITMAP Second source image.
 *  \param Not int Set to 1 or TRUE to perform Nor instead of Or.
 *  \return FIBITMAP* destination image.
*/
DLL_API FIBITMAP *DLL_CALLCONV
FIA_BinaryOr (FIBITMAP *src1, FIBITMAP *src2, int Not);

/** \brief Logical And or Intersection
 *
 *  Calculate the logical And (intersection) or Nand.
 *
 *  \param src1 FIBITMAP First source image.
 *  \param src1 FIBITMAP Second source image.
 *  \param Not int Set to 1 or TRUE to perform Nand instead of And.
 *  \return FIBITMAP* destination image.
*/
DLL_API FIBITMAP *DLL_CALLCONV
FIA_BinaryAnd (FIBITMAP *src1, FIBITMAP *src2, int Not);


#ifdef __cplusplus
}
#endif

#endif
