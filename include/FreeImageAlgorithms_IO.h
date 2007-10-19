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

#ifndef __FREEIMAGE_ALGORITHMS_IO__
#define __FREEIMAGE_ALGORITHMS_IO__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

/*! \file 
	Provides various input and output methods.
*/ 

/** \brief Load a FIBITMAP from a file.
 *	The file is loaded using a plugin determined from the filename extension.
 *	
 *  \param filepath OS Path to the file to be loaded.
 *
 *  \return FIBITMAP* on success NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadFIBFromFile(const char *filepath);


/** \brief Save a FIBITMAP to a file.
 *	The file to be saved using a plugin determined from the filename extension.
 *	
 *  \param dib FreeImage Bitmap to be saved.
 *  \param filepath OS Path to the file to be saved.
 *  \param bit_depth The bit depth which is either BIT8 or BIT24
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SaveFIBToFile (FIBITMAP *dib, const char *filepath, FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH bit_depth);

/** \brief Copy an array of bytes to FIBITMAP
 *	
 *  \param src FreeImage Bitmap to copy bytes to.
 *  \param data bytes to copy.
 *  \param padded Is the data padded to 32 bit boundaries.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_CopyBytesToFBitmap(FIBITMAP *src, BYTE *data, int padded);

/** \brief Load a greyscale FIBITMAP from an array of bytes
 *	
 *  \param data bytes to copy.
 *  \param bpp of resulting image.
 *  \param width of resulting image.
 *  \param height of resulting image.
 *  \param data_type FREE_IMAGE_TYPE of resulting image.
 *  \param padded Is the data padded to 32 bit boundaries.
 *  \param vertical_flip int Should the image be vertically flipped.
 *  \return FIBITMAP* on success and NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadGreyScaleFIBFromArrayData (BYTE *data, int bpp, int width, int height,
												   FREE_IMAGE_TYPE data_type, int padded, int vertical_flip);

/** \brief Load a rgb FIBITMAP from an array of bytes
 *	
 *  \param data bytes to copy.
 *  \param bpp of resulting image.
 *  \param width of resulting image.
 *  \param height of resulting image.
 *  \param padded Is the data padded to 32 bit boundaries.
 *  \param vertical_flip int Should the image be vertically flipped.
 *  \return FIBITMAP* on success and NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadColourFIBFromArrayData (BYTE *data, int bpp, int width, int height,
												int padded, int vertical_flip, COLOUR_ORDER colour_order);

#ifdef __cplusplus
}
#endif

#endif
