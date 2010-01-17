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

#ifndef __FREEIMAGE_ALGORITHMS_DRAWING__
#define __FREEIMAGE_ALGORITHMS_DRAWING__

/*! \file 
	Provides methods for drawing on a freeimage bitmap.
*/ 

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Draw a line on an image.
 *
 *  \param src Image to draw on.
 *  \param p1 FIAPOINT point to start the line
 *  \param p2 FIAPOINT point to end the line.
 *  \param colour RGBQUAD colour of the line.
 *  \param line_width pixel width of the line.
 *  \param antialiased Whether the polygon is antialiased.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawColourLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, RGBQUAD colour,
                                    int line_width, int antialiased);

/** \brief Draw a rectangle outline in colour.
 *
 *  \param src Image to draw on.
 *  \param rect Rectangle to draw.
 *  \param colour RGBQUAD colour of the rectangle.
 *  \param line_width pixel width of the outline.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawColourRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour, int line_width);

/** \brief Draw a solid rectangle in colour.
 *
 *  \param src Image to draw on.
 *  \param rect Rectangle to draw.
 *  \param colour RGBQUAD colour of the rectangle.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawColourSolidRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour);

/** \brief Draw a solid rectangle in greyscale.
 *
 *  \param src Image to draw on.
 *  \param rect Rectangle to draw.
 *  \param value greyscale intensity of the rectangle.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawSolidGreyscaleRect (FIBITMAP *src, FIARECT rect, double value);

/** \brief Draw a greyscale line on an image.
 *
 *  \param src Image to draw on.
 *  \param p1 FIAPOINT point to start the line
 *  \param p2 FIAPOINT point to end the line.
 *  \param value greyscale intensity of the line.
 *  \param line_width pixel width of the line.
 *  \param antialiased Whether the polygon is antialiased.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawGreyscaleLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, double value,
                                       int line_width, int antialiased);

/** \brief Floodfills part of a greyscale image.
 *
 *	Works on integer type images not colour or floating point.
 *
 *  \param src Image to draw on.
 *  \param seed_x x position to start from.
 *  \param seed_y y position to start from.
 *  \param fill_colour greyscale intensity of the fill.
 *  \return FIBITMAP* return flood filled image or NULL.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_FloodFill(FIBITMAP* src, int seed_x, int seed_y, int fill_colour);

/** \brief Floodfills part of a greyscale image.
 *
 *	Works on integer type images not colour or floating point.
 *
 *  \param src Image to draw on.
 *  \param seed_x x position to start from.
 *  \param seed_y y position to start from.
 *  \param fill_colour greyscale intensity of the fill.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_InPlaceFloodFill(FIBITMAP* src, int seed_x, int seed_y, int fill_colour);


/** \brief Draw a polygon from an array of FIAPOINT's.
 *
 *  \param src Image to draw on.
 *  \param points FIAPOINT array.
 *  \param number_of_points int number of points.
 *  \param value intensity of the fill.
 *  \param antialiased Whether the polygon is antialiased.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawSolidGreyscalePolygon (FIBITMAP *src, FIAPOINT *points, int number_of_points,
                                          unsigned char value, int antialiased);

DLL_API int DLL_CALLCONV
FIA_DrawColourSolidPolygon (FIBITMAP * src, FIAPOINT * points,
                          int number_of_points, RGBQUAD colour, int antialiased);

/** \brief Draw a convexhull for points on an image.
 *
 *  \param src Image to draw on.
 *  \return FIBITMAP* image success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImage_ConvexHull(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FIA_DrawGreyscaleRect (FIBITMAP *src, FIARECT rect, double colour, int line_width);

DLL_API int DLL_CALLCONV
FIA_DrawSolidGreyscaleEllipse (FIBITMAP *src, FIARECT rect, unsigned char value, int antialiased);

DLL_API int DLL_CALLCONV
FIA_DrawColourSolidEllipse (FIBITMAP * src, FIARECT rect, RGBQUAD colour, int antialiased);

DLL_API int DLL_CALLCONV
FIA_DrawGreyScaleCheckerBoard (FIBITMAP *src, int square_size);

DLL_API int DLL_CALLCONV
FIA_DrawHorizontalColourText (FIBITMAP *src, int left, int top, const char *text, RGBQUAD colour);

DLL_API int DLL_CALLCONV
FIA_DrawHorizontalGreyscaleText (FIBITMAP * src, int left, int top, const char *text, unsigned char value);

#ifdef __cplusplus
}
#endif

#endif
