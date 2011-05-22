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

#ifndef __FREEIMAGE_ALGORITHMS_DRAWING__
#define __FREEIMAGE_ALGORITHMS_DRAWING__

/*! \file 
	Provides methods for drawing on a freeimage bitmap.
*/ 

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
                FIA_AGG_FONT_GSE_4x6,
                FIA_AGG_FONT_GSE_4x8,
                FIA_AGG_FONT_GSE_5x7,
                FIA_AGG_FONT_GSE_5x9,
                FIA_AGG_FONT_GSE_6x12,
                FIA_AGG_FONT_GSE_6x9,
                FIA_AGG_FONT_GSE_7x11,
                FIA_AGG_FONT_GSE_7x11_BOLD,
                FIA_AGG_FONT_GSE_7x15,
                FIA_AGG_FONT_GSE_7x15_BOLD,
                FIA_AGG_FONT_GSE_8x16,
                FIA_AGG_FONT_GSE_8x16_BOLD,

                FIA_AGG_FONT_MCS11_PROP,
                FIA_AGG_FONT_MCS11_PROP_CONDENSED,
                FIA_AGG_FONT_MCS12_PROP,
                FIA_AGG_FONT_MCS13_PROP,
                FIA_AGG_FONT_MCS_5x10_MONO,
                FIA_AGG_FONT_MCS_5x11_MONO,
                FIA_AGG_FONT_MCS_6x10_MONO,
                FIA_AGG_FONT_MCS_6x11_MONO,
                FIA_AGG_FONT_MCS_7x12_MONO_HIGH,
                FIA_AGG_FONT_MCS_7x12_MONO_LOW,

                FIA_AGG_FONT_VERDANA_12,
                FIA_AGG_FONT_VERDANA_12_BOLD,
                FIA_AGG_FONT_VERDANA_13,
                FIA_AGG_FONT_VERDANA_13_BOLD,
                FIA_AGG_FONT_VERDANA_14,
                FIA_AGG_FONT_VERDANA_14_BOLD,
                FIA_AGG_FONT_VERDANA_16,
                FIA_AGG_FONT_VERDANA_16_BOLD,
                FIA_AGG_FONT_VERDANA_17,
                FIA_AGG_FONT_VERDANA_17_BOLD,
                FIA_AGG_FONT_VERDANA_18,
                FIA_AGG_FONT_VERDANA_18_BOLD

             } FIA_AggEmbeddedFont;

/** \brief Draw a line on an image.
 *
 *  \param src Image to draw on.
 *  \param p1 FIAPOINT point to start the line
 *  \param p2 FIAPOINT point to end the line.
 *  \param colour RGBQUAD colour of the line.
 *  \param line_width pixel width of the line.
 *  \param antialiased Whether the polygon is antialiased.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
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
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawColourRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour, int line_width);

/** \brief Draw a solid rectangle in colour.
 *
 *  \param src Image to draw on.
 *  \param rect Rectangle to draw.
 *  \param colour RGBQUAD colour of the rectangle.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawColourSolidRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour);

/** \brief Draw a solid rectangle in greyscale.
 *
 *  \param src Image to draw on.
 *  \param rect Rectangle to draw.
 *  \param value greyscale intensity of the rectangle.
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
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
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DrawGreyscaleLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, double value,
                                       int line_width, int antialiased);


/** \brief Draw a line on an image on an 8bit image of width 1 pixel.
 *
 *  \param src Image to draw on.
 *  \param p1 FIAPOINT point to start the line
 *  \param p2 FIAPOINT point to end the line.
 *  \param value greyscale intensity of the line.
 *  \return int len of line drawn.
*/
DLL_API int DLL_CALLCONV
FIA_DrawOnePixelIndexLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, BYTE value);

DLL_API int DLL_CALLCONV
FIA_DrawOnePixelIndexLineFromTopLeft (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, BYTE value);

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
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
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
 *  \return int FIA_SUCCESS on success or FIA_ERROR on error.
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
#define FIA_ConvexHull FreeImage_ConvexHull

DLL_API int DLL_CALLCONV
FIA_DrawGreyscaleRect (FIBITMAP *src, FIARECT rect, double colour, int line_width);

DLL_API int DLL_CALLCONV
FIA_DrawSolidGreyscaleEllipse (FIBITMAP *src, FIARECT rect, unsigned char value, int antialiased);

DLL_API int DLL_CALLCONV
FIA_DrawColourSolidEllipse (FIBITMAP * src, FIARECT rect, RGBQUAD colour, int antialiased);

DLL_API int DLL_CALLCONV
FIA_DrawGreyScaleCheckerBoard (FIBITMAP *src, int square_size);

DLL_API int DLL_CALLCONV
FIA_DrawHorizontalColourText (FIBITMAP *src, int left, int top, FIA_AggEmbeddedFont font, const char *text, RGBQUAD colour);

DLL_API int DLL_CALLCONV
FIA_DrawHorizontalGreyscaleText (FIBITMAP * src, int left, int top, FIA_AggEmbeddedFont font, const char *text, unsigned char value);

DLL_API FIA_Matrix * DLL_CALLCONV
FIA_MatrixNew();

DLL_API FIA_Matrix * DLL_CALLCONV
FIA_MatrixNewWithValues(double v0, double v1, double v2, 
                                        double v3, double v4, double v5);

DLL_API int DLL_CALLCONV
FIA_MatrixDestroy(FIA_Matrix *matrix);

DLL_API int DLL_CALLCONV
FIA_MatrixSetValues(FIA_Matrix *matrix, double v0, double v1, double v2, 
                                        double v3, double v4, double v5);
                
DLL_API int DLL_CALLCONV
FIA_MatrixScale(FIA_Matrix *matrix, double x, double y, FIA_MatrixOrder order);
      
DLL_API int DLL_CALLCONV
FIA_MatrixRotate(FIA_Matrix *matrix, double a, FIA_MatrixOrder order);

DLL_API int DLL_CALLCONV
FIA_MatrixTranslate(FIA_Matrix *matrix, double x, double y, FIA_MatrixOrder order);

DLL_API int DLL_CALLCONV
FIA_MatrixInvert(FIA_Matrix *matrix);

DLL_API FIBITMAP * DLL_CALLCONV
FIA_AffineTransform(FIBITMAP *src, int image_dst_width, int image_dst_height, 
  FIA_Matrix *matrix, RGBQUAD colour, int retain_background);
           
DLL_API int DLL_CALLCONV
FIA_DrawImageFromSrcToDst(FIBITMAP *dst, FIBITMAP *src, FIA_Matrix *matrix,
              int dstLeft, int dstTop, int dstWidth, int dstHeight,
              int srcLeft, int srcTop, int srcWidth, int srcHeight, RGBQUAD colour, int retain_background);
               
DLL_API int DLL_CALLCONV
FIA_DrawImageToDst(FIBITMAP *dst, FIBITMAP *src, FIA_Matrix *matrix,
              int dstLeft, int dstTop, int dstWidth, int dstHeight, RGBQUAD colour, int retain_background);

#ifdef __cplusplus
}
#endif

#endif
