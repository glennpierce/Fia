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

#ifndef __FREEIMAGE_ALGORITHMS_PALETTES__
#define __FREEIMAGE_ALGORITHMS_PALETTES__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
	Provides various palette modification methods.
*/ 

/** \brief Get a RGBQUAD from the three colours.
 *
 *  \param red Red colour.
 *  \param green Green colour.
 *  \param blue Blue colour.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API RGBQUAD DLL_CALLCONV FIA_RGBQUAD(unsigned char red, unsigned char green,
										 unsigned char blue);

/** \brief Copies the palette for an image to an array of RGBQUAD.
 *
 *  \param src Image containing palette.
 *  \param palette  RGBQUAD Array to copy palette to.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_CopyPaletteToRGBQUAD(FIBITMAP *src, RGBQUAD *palette);

/** \brief Copies the palette from an array of RGBQUAD to a FIBITMAP.
 *
 *  \param dst Image to copy palette to.
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV		   
FreeImageAlgorithms_CopyPaletteFromRGBQUAD(FIBITMAP *dst, RGBQUAD *palette);

/** \brief Copies the palette from one FIBITMAP to another.
 *
 *  \param src Image containing palette.
 *  \param dst Image to copy palette to.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV		   
FreeImageAlgorithms_CopyPalette(FIBITMAP *src, FIBITMAP *dst);

/** \brief Reverses an array of RGBQUAD elements.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_ReversePaletteEntries(RGBQUAD *palette);

/** \brief Set the pallete of a FIBITMAP image to a greylevel palette.
 *
 *  \param src Image to set palette to greyscale.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetGreyLevelPalette(FIBITMAP *src);

/** \brief Set the pallete of a FIBITMAP image to a log palette.
 *
 *  \param src Image to set palette to log.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetLogColourPalette(FIBITMAP *src);

/** \brief Set the pallete of a FIBITMAP image to a rainbow palette.
 *
 *  \param src Image to set palette to rainbow.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetRainBowPalette(FIBITMAP *src);

/** \brief Set the pallete of a FIBITMAP image to a reverse rainbow palette.
 *
 *  \param src Image to set palette to rainbow.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetReverseRainBowPalette(FIBITMAP *src);

/** \brief Set the pallete of a FIBITMAP image to a temperature palette.
 *
 *  \param src Image to set palette to temperature.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetTemperaturePalette(FIBITMAP *src);

/** \brief Set the pallete of a FIBITMAP image to a pileup palette.
 *
 *  \param src Image to set palette to temperature.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetPileUpPalette(FIBITMAP *src, RGBQUAD colour1, RGBQUAD colour2,
                                     RGBQUAD colour3, BYTE *size);

/** \brief Set the pallete of a FIBITMAP image to a false colour palette.
 *
 *  \param src Image to set palette to false colour.
 *  \param wavelength Wavelength of the false colour.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetFalseColourPalette(FIBITMAP *src, double wavelength);

/** \brief Set the pallete of a FIBITMAP image to a greyscale overload palette.
 *
 *  \param src Image to set palette to greyscale overload.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetGreyLevelOverLoadPalette(FIBITMAP *src);


/** \brief Set a pallete specify three unique colours and a background.
 *
 *	Useful for displaying binary images.
 *
 *  \param src Image to set palette to greyscale overload.
 *  \param background_colour RGBQUAD representing the background.
 *  \param pos1 int Position in palette for first colour.
 *  \param colour1 RGBQUAD representing the first colour.
 *  \param pos2 int Position in palette for second colour.
 *  \param colour2 RGBQUAD representing the second colour.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetTernaryPalettePalette(FIBITMAP *src, RGBQUAD background_colour,
									  int pos1, RGBQUAD colour1, int pos2, RGBQUAD colour2);


/** \brief Set the pallete of a FIBITMAP image to a seismic palette.
 *
 *  \param src Image to set palette to seismic.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetSeismicColourPalette(FIBITMAP *src);

/** \brief Get a greyscale pallete into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelPalette(RGBQUAD *palette);

/** \brief Get a greyscale overload pallete into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelOverLoadPalette(RGBQUAD *palette);


/** \brief Get a pallete specify three unique colours and a background.
 *
 *	Useful for displaying binary images.
 *
 *  \param  palette RGBQUAD* array.
 *  \param background_colour RGBQUAD representing the background.
 *  \param pos1 int Position in palette for first colour.
 *  \param colour1 RGBQUAD representing the first colour.
 *  \param pos2 int Position in palette for second colour.
 *  \param colour2 RGBQUAD representing the second colour.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetTernaryPalette(RGBQUAD *palette, RGBQUAD background_colour,
									  int pos1, RGBQUAD colour1, int pos2, RGBQUAD colour2);

/** \brief Get a rainbow pallete into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetRainBowPalette(RGBQUAD *palette);

/** \brief Get a rainbow pallete in reverse into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetReverseRainBowPalette(RGBQUAD *palette);

/** \brief Get a log pallete into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetLogColourPalette(RGBQUAD *palette);

/** \brief Get a false colour pallete into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \param wavelength of the false colour palette.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetFalseColourPalette(RGBQUAD *palette, double wavelength);

/** \brief Get a temperature pallete into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetTemperaturePalette(RGBQUAD *palette);

/** \brief Get a seismic pallete into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetSeismicColourPalette(RGBQUAD *palette);

/** \brief Get a ileup palette into a RGBQUAD array.
 *
 *  \param  palette RGBQUAD* array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetPileUpPalette(RGBQUAD *palette,
                                     RGBQUAD colour1, RGBQUAD colour2, RGBQUAD colour3, BYTE *size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetOpticalDensityPalette(RGBQUAD *palette, unsigned char red,
	unsigned char green, unsigned char blue, int contrast, int entries);

#ifdef __cplusplus
}
#endif

#endif
