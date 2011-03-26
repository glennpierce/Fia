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

#ifndef __FREEIMAGE_ALGORITHMS_FFT__
#define __FREEIMAGE_ALGORITHMS_FFT__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
*	Provides FFT methods.
*/ 

/** \brief Creates a HBITMAP
 *	
 *  \param src FIBITMAP 
 *	\param inverse Whether to perform a inverse fourier transform.
 *	\param shift Shift the Zero frequency to the center of the image.
 *  \return FIBITMAP* on success and NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_FFT(FIBITMAP *src);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_IFFT(FIBITMAP *src);

/** \brief Creates a FIT_DOUBLE absolute image from a complex image.
 *	
 *  \param src FIBITMAP complex image.
 *  \return FIBITMAP* on success and NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_ConvertComplexImageToAbsoluteValued(FIBITMAP *src);

/** \brief Creates a FIT_DOUBLE absolute image from a complex image.
 *	
 *  \param src FIBITMAP complex image.
 *  \return FIBITMAP* on success and NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_ConvertComplexImageToAbsoluteValuedSquared(FIBITMAP *src);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_ComplexImageToRealValued(FIBITMAP *src);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_ShiftImageEdgeToCenter(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FIA_InPlaceShiftImageEdgeToCenter(FIBITMAP **src);

#ifdef __cplusplus
}
#endif

#endif
