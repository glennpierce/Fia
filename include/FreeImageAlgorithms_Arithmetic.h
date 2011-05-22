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

#ifndef __FREEIMAGE_ALGORITHMS_ARITHMETIC__
#define __FREEIMAGE_ALGORITHMS_ARITHMETIC__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
	GREY_LEVEL_ADD_ADD,
	GREY_LEVEL_ADD_AVERAGE,
	GREY_LEVEL_ADD_FILL_RANGE

} GREY_LEVEL_ADD_TO_COLOURTYPE;


/*! \file 
	Provides various arithmetic methods.
*/ 

/** \brief Transpose an image.
 *
 *  This function transposes the image data. Ie its row and columns are swapped.
 *
 *  \param src FIBITMAP bitmap to transpose.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_Transpose(FIBITMAP *src);

/** \brief Return the log image.
 *
 *  This function returns an image where the log of each pixel is taken.
 *
 *  \param src FIBITMAP bitmap to perform the log operation on.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_Log(FIBITMAP *src);

/** \brief Add 2 images, dst + src
 *
 *  \param dst FIBITMAP first bitmap, this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_Add (FIBITMAP * dst, FIBITMAP * src);

/** \brief Subtract an image from another, dst - src
 *
 *  \param dst FIBITMAP first bitmap, this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_Subtract (FIBITMAP * dst, FIBITMAP * src);

/** \brief Multiply 2 images, dst * src
 *
 *  \param dst FIBITMAP first bitmap, this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_Multiply (FIBITMAP * dst, FIBITMAP * src);

/** \brief Average (mean of) 2 images, dst and src
 *
 *  \param dst FIBITMAP first bitmap, this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_Average (FIBITMAP * dst, FIBITMAP * src);

/** \brief Add image to a constant
 *
 *  \param dst FIBITMAP first bitmap, this also serves as the output.
 *  \param constant double the constant to use
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_AddConst (FIBITMAP * dst, double constant);

/** \brief Subtract constant from an image
 *
 *  \param dst FIBITMAP first bitmap, this also serves as the output.
 *  \param constant double the constant to use
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_SubtractConst (FIBITMAP * dst, double constant);

/** \brief Multiply image by a constant
 *
 *  \param dst FIBITMAP first bitmap, this also serves as the output.
 *  \param constant double the constant to use
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_MultiplyConst (FIBITMAP * dst, double constant);

/** \brief Multiply two greylevel images.
 *
 *  \param dst FIBITMAP first bitmap to perform the multiply this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the multiply operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_MultiplyGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Divide two greylevel images.
 *
 *  \param dst FIBITMAP first bitmap to perform the divide this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the divide operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_DivideGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Add two greylevel images.
 *
 *  \param dst FIBITMAP first bitmap to perform the add this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the add operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_AddGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Subtract two greylevel images.
 *
 *  \param dst FIBITMAP first bitmap to perform the subtract this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the subtract operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_SubtractGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Multiply a greylevel image by a constant.
 *
 *  \param dst FIBITMAP bitmap to perform the multiply this also serves as the output.
 *  \param constant Constant used to perform the multiply.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_MultiplyGreyLevelImageConstant(FIBITMAP* dst, double constant);

/** \brief Divide a greylevel image by a constant.
 *
 *  \param dst FIBITMAP bitmap to perform the divide this also serves as the output.
 *  \param constant Constant used to perform the divide.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_DivideGreyLevelImageConstant(FIBITMAP* dst, double constant);

/** \brief Add a greylevel image by a constant.
 *
 *  \param dst FIBITMAP bitmap to perform the add this also serves as the output.
 *  \param constant Constant used to perform the add.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_AddGreyLevelImageConstant(FIBITMAP* dst, double constant);

/** \brief Subtract a greylevel image by a constant.
 *
 *  \param dst FIBITMAP bitmap to perform the subtract this also serves as the output.
 *  \param constant Constant used to perform the subtract.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_SubtractGreyLevelImageConstant(FIBITMAP* dst, double constant);

/** \brief Calculate the complex conjugate of a complex image.
 *
 *  \param src FIBITMAP bitmap must be of type complex.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_ComplexConjugate(FIBITMAP* src);

/** \brief Multiply two complex images.
 *
 *  \param dst FIBITMAP first bitmap to perform the multiply this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the multiply operation on.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_MultiplyComplexImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Returns the sum of all the pixels in an image where the mask allows.
 *
 *  \param dst FIBITMAP bitmap containing pixels to sum.
 *  \param src FIBITMAP mask bitmap to specify with pixels to sum.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_SumOfAllPixels(FIBITMAP* src, FIBITMAP* mask, double *sum);

/** \brief Returns the image containing the maximum equivilent pixels in the two images.
 *
 *  \param dst FIBITMAP bitmap containing the max pixels (serves as output).
 *  \param src FIBITMAP bitmap Source bitmap.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_GetMaxIntensityFromImages(FIBITMAP* dst, FIBITMAP* src);

DLL_API int DLL_CALLCONV
FIA_Add8BitImageToColourImage (FIBITMAP *colour_dib, FIBITMAP *greyscale_dib, GREY_LEVEL_ADD_TO_COLOURTYPE type);

DLL_API int DLL_CALLCONV
FIA_Overlay8BitImageOverColourImage (FIBITMAP *colour_dib, FIBITMAP *greyscale_dib, BYTE threshold);

#ifdef __cplusplus
}
#endif

#endif
