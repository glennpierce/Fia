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

#ifndef __FREEIMAGE_ALGORITHMS__
#define __FREEIMAGE_ALGORITHMS__

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include "FreeImage.h"

//#define DONT_PROFILE

#include "profile.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
#define __cdecl
#endif

/** Data structure for FIABITMAP type (FIBITMAP with a border)
*/
typedef struct
{
	/// FIBITMAP bitmap.
	FIBITMAP *fib;
	/// border size.
	int xborder; 
	int yborder; 

} FIABITMAP;

typedef struct
{
	int left;
	int top;
	int right;
	int bottom;

} FIARECT;

typedef struct
{
	int x;
	int y;

} FIAPOINT;

typedef enum
{
    BorderType_Constant,
    BorderType_Copy,
    BorderType_Mirror

} BorderType;

typedef enum
{
	COLOUR_ORDER_RGB,
	COLOUR_ORDER_BGR

} COLOUR_ORDER;

#define FIA_SUCCESS 1
#define FIA_ERROR 0

#define U12BIT_MAX 4096
#define U14BIT_MAX 16384

#define BIT12_MAX 2047
#define BIT12_MIN (-BIT12_MAX - 1)  

#define BIT14_MAX 8191
#define BIT14_MIN (-BIT14_MAX - 1)

#define _CPU_FEATURE_MMX    0x0001
#define _CPU_FEATURE_SSE    0x0002
#define _CPU_FEATURE_SSE2   0x0004
#define _CPU_FEATURE_3DNOW  0x0008

typedef enum {BIT8, BIT24} FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH;

/** \brief Disposes of FIABITMAP's.
 *
 *  Disposes of FIABITMAP's (which have a border added to an image).
 *
 *  \param src FIABITMAP bitmap.
*/
DLL_API void DLL_CALLCONV
FIA_Unload(FIABITMAP* src);


/** \brief Adds a border to an image.
 *
 *  This function returns an image with a black (zero) border added to an image.
 *
 *  \param src FIBITMAP bitmap to add a border to.
 *  \param xborder Size of the border.
 *  \param yborder Size of the border.
 *  \param type BorderType type of the border can be BorderType_Constant,
 *              BorderType_Copy or BorderType_Mirror.
 *  \param constant double the value to use for the border.
 *  \return A FIABITMAP structure containing a FIBITMAP and a border size.
*/
DLL_API FIABITMAP* DLL_CALLCONV
FIA_SetBorder(FIBITMAP *src, int xborder, int yborder, BorderType type, double constant);

/** \brief Adds a zero border to an image.
 *
 *  This function returns an image with a black (zero) border added to an image.
 *
 *  \param src FIBITMAP bitmap to add a border to.
 *  \param border Size of the border.
 *  \return A FIABITMAP structure containing a FIBITMAP and a border size.
*/
DLL_API FIABITMAP* DLL_CALLCONV
FIA_SetZeroBorder(FIBITMAP *src, int xborder, int yborder);

#ifdef __cplusplus
}
#endif

#endif
