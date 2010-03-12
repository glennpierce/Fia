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

#ifndef __FREEIMAGE_ALGORITHMS_ARITHMETIC__
#define __FREEIMAGE_ALGORITHMS_ARITHMETIC__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/** \brief Multiply two greylevel images.
 *
 *  \param dst FIBITMAP first bitmap to perform the multiply this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the multiply operation on.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_MultiplyGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Divide two greylevel images.
 *
 *  \param dst FIBITMAP first bitmap to perform the divide this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the divide operation on.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_DivideGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Add two greylevel images.
 *
 *  \param dst FIBITMAP first bitmap to perform the add this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the add operation on.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_AddGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Subtract two greylevel images.
 *
 *  \param dst FIBITMAP first bitmap to perform the subtract this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the subtract operation on.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_SubtractGreyLevelImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Multiply a greylevel image by a constant.
 *
 *  \param dst FIBITMAP bitmap to perform the multiply this also serves as the output.
 *  \param constant Constant used to perform the multiply.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_MultiplyGreyLevelImageConstant(FIBITMAP* dst, double constant);

/** \brief Divide a greylevel image by a constant.
 *
 *  \param dst FIBITMAP bitmap to perform the divide this also serves as the output.
 *  \param constant Constant used to perform the divide.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_DivideGreyLevelImageConstant(FIBITMAP* dst, double constant);

/** \brief Add a greylevel image by a constant.
 *
 *  \param dst FIBITMAP bitmap to perform the add this also serves as the output.
 *  \param constant Constant used to perform the add.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_AddGreyLevelImageConstant(FIBITMAP* dst, double constant);

/** \brief Subtract a greylevel image by a constant.
 *
 *  \param dst FIBITMAP bitmap to perform the subtract this also serves as the output.
 *  \param constant Constant used to perform the subtract.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_SubtractGreyLevelImageConstant(FIBITMAP* dst, double constant);

/** \brief Calculate the complex conjugate of a complex image.
 *
 *  \param src FIBITMAP bitmap must be of type complex.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_ComplexConjugate(FIBITMAP* src);

/** \brief Multiply two complex images.
 *
 *  \param dst FIBITMAP first bitmap to perform the multiply this also serves as the output.
 *  \param src FIBITMAP second bitmap to perform the multiply operation on.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_MultiplyComplexImages(FIBITMAP* dst, FIBITMAP* src);

/** \brief Returns the sum of all the pixels in an image where the mask allows.
 *
 *  \param dst FIBITMAP bitmap containing pixels to sum.
 *  \param src FIBITMAP mask bitmap to specify with pixels to sum.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_SumOfAllPixels(FIBITMAP* src, FIBITMAP* mask, double *sum);

/** \brief Returns the image containing the maximum equivilent pixels in the two images.
 *
 *  \param dst FIBITMAP bitmap containing the max pixels (serves as output).
 *  \param src FIBITMAP bitmap Source bitmap.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FIA_GetMaxIntensityFromImages(FIBITMAP* dst, FIBITMAP* src);



#ifdef __cplusplus
}
#endif

#endif
