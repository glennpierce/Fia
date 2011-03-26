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

#ifdef _WINDOWS   // Hbitmap only used under windows.

#ifndef __FREEIMAGE_ALGORITHMS_HBITMAP__
#define __FREEIMAGE_ALGORITHMS_HBITMAP__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
	Provides various methods for converting FIBITMAP's to windows bitmaps.
*/ 

/** \brief Converts FIBITMAP to a HBITMAP
 *		   This does not produces a dibsection so the image
 *		   is located in video memory.
 *	
 *  \param dib FIBITMAP to convert.
 *  \return HBITMAP on success and NULL on error.
*/
DLL_API HBITMAP DLL_CALLCONV
FIA_FibToHBitmap(FIBITMAP *dib);

/** \brief Frees a HBITMAP
 *
 *  \param hbitmap HBITMAP to free.
*/
DLL_API void DLL_CALLCONV
FIA_FreeHBitmap(HBITMAP hbitmap);

/** \brief Converts a HBITMAP to a FIBITMAP
 *		   This does not produces a dibsection so the image
 *		   is located in video memory.
 *	
 *	\param hdc HDC assciated with the bitmap.
 *  \param bitmap HBITMAP hbitmap to convert.
 *  \return FIBITMAP* on success and NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_HBitmapToFIB(HDC hdc, HBITMAP bitmap);

/** \brief Converts FIBITMAP to a HBITMAP
 *		   This produces a dibsection so the image
 *		   is located in system memory.
 *	
 *  \param src FIBITMAP to convert.
 *	\param hdc HDC assciated with the bitmap.
 *  \param left the left point of the image subsection to convert.
 *  \param top the top point of the image subsection to convert.
 *  \param right the right point of the image subsection to convert.
 *  \param bottom the bottom point of the image subsection to convert.
 *  \return HBITMAP on success and NULL on error.
*/
DLL_API HBITMAP DLL_CALLCONV 
FIA_GetDibSection(FIBITMAP *src, HDC hdc, int left, int top, int right, int bottom);

/** \brief Converts FIBITMAP to an existing HBITMAP
 *		   This produces a dibsection so the image
 *		   is located in system memory.
 *	
 *  \param src FIBITMAP to convert.
 *	\param hbitmap hbitmap to copy the FIBITMAP to.
 *  \param left the left point of the image subsection to convert.
 *  \param top the top point of the image subsection to convert.
 *  \param right the right point of the image subsection to convert.
 *  \param bottom the bottom point of the image subsection to convert.
 *  \return HBITMAP on success and NULL on error.
*/
DLL_API int DLL_CALLCONV
FIA_CopyToDibSection(FIBITMAP *src, HBITMAP hbitmap, int left, int top, int right, int bottom);

/** \brief Creates a HBITMAP
 *	
 *  \param hdc HDC to use for dibsection.
 *	\param width Width of HBITMAP.
 *	\param height Height of HBITMAP.
 *	\param bpp BPP of HBITMAP.
 *  \param colours_used number of colours in the HBITMAP.
 *  \param bmiColours colour Palette.
 *  \return HBITMAP on success and NULL on error.
*/
DLL_API HBITMAP DLL_CALLCONV
FIA_CreateDibSection(HDC hdc, int width, int height, int bpp, int colours_used, RGBQUAD* bmiColours);

#ifdef __cplusplus
}
#endif

#endif // __FREEIMAGE_ALGORITHMS_HBITMAP__

#endif // _WINDOWS_
