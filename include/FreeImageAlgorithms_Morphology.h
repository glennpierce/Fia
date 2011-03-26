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

#ifndef __FREEIMAGE_ALGORITHMS_MORPHOLOGY_FILTER__
#define __FREEIMAGE_ALGORITHMS_MORPHOLOGY_FILTER__

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
 *	Dilates the particles in an image.
 *
 *  \param src FIBITMAP bitmap to perform the dilation operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_BinaryDilation(FIABITMAP* src, FilterKernel kernel);

/*! \file 
 *	Erodes the particles in an image.
 *
 *  \param src FIBITMAP bitmap to perform the erosion operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_BinaryErosion(FIABITMAP* src, FilterKernel kernel);

/*! \file 
 *	Erodes and then performs dialation.
 *
 *  \param src FIBITMAP bitmap to perform the erosion operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_BinaryOpening(FIABITMAP* src, FilterKernel kernel);

/*! \file 
 *	Diates then performs an erosion.
 *
 *  \param src FIBITMAP bitmap to perform the erosion operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_BinaryClosing(FIABITMAP* src, FilterKernel kernel);

#ifdef __cplusplus
}
#endif

#endif
