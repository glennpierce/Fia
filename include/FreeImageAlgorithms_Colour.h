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

#ifndef __FREEIMAGE_ALGORITHMS_COLOUR__
#define __FREEIMAGE_ALGORITHMS_COLOUR__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

/*! \file 
*	Provides colour space functions.
*/

/** \brief Convert red, green and blue values to hue, satuation and value.
 *
 *  \param red Red value.
 *  \param green Green value.
 *  \param blue Blue value.
 *  \param hue Hue returned.
 *  \param satuation Satuation returned.
 *  \param value Value returned.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_RGBToHSV( unsigned char red, unsigned char green, unsigned char blue,
							double *hue, double *satuation, double *value);

/** \brief Convert hue, satuation and value values to red, green and blue.
 *
 *  \param hue Hue.
 *  \param satuation Satuation.
 *  \param value Value.
 *  \param red Red value returned.
 *  \param green Green value returned.
 *  \param blue Blue value returned.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_HSVToRGB( double hue, double satuation, double value,
							unsigned char *red, unsigned char *green, unsigned char *blue);

/** \brief Convert red, green and blue values to hue, satuation and luminosity.
 *
 *  \param red Red value.
 *  \param green Green value.
 *  \param blue Blue value.
 *  \param hue Hue returned.
 *  \param satuation Satuation returned.
 *  \param luminosity Value returned.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_RGBToHSL( unsigned char red, unsigned char green, unsigned char blue,
							double *hue, double *satuation, double *luminosity);

/** \brief Convert hue, satuation and luminosity values to red, green and blue.
 *
 *  \param hue Hue.
 *  \param satuation Satuation.
 *  \param luminosity Value.
 *  \param red Red value returned.
 *  \param green Green value returned.
 *  \param blue Blue value returned.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_HSLToRGB( double hue, double satuation, double luminosity,
							unsigned char *red, unsigned char *green, unsigned char *blue);

#ifdef __cplusplus
}
#endif

#endif
