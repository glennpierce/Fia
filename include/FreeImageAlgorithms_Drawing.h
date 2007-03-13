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
 *  \param p1 POINT point to start the line
 *  \param p2 POINT point to end the line.
 *  \param colour COLORREF colour of the line.
 *  \param line_width pixel width of the line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DrawColourLine (FIBITMAP *src, POINT p1, POINT p2, COLORREF colour, int line_width);

/** \brief Draw a rectangle outline in colour.
 *
 *  \param src Image to draw on.
 *  \param rect Rectangle to draw.
 *  \param colour COLORREF colour of the rectangle.
 *  \param line_width pixel width of the outline.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DrawColourRect (FIBITMAP *src, RECT rect, COLORREF colour, int line_width);

/** \brief Draw a solid rectangle in colour.
 *
 *  \param src Image to draw on.
 *  \param rect Rectangle to draw.
 *  \param colour COLORREF colour of the rectangle.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DrawColourSolidRect (FIBITMAP *src, RECT rect, COLORREF colour);

/** \brief Draw a solid rectangle in greyscale.
 *
 *  \param src Image to draw on.
 *  \param rect Rectangle to draw.
 *  \param value greyscale intensity of the rectangle.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DrawSolidGreyscaleRect (FIBITMAP *src, RECT rect, double value);

/** \brief Draw a greyscale line on an image.
 *
 *  \param src Image to draw on.
 *  \param p1 POINT point to start the line
 *  \param p2 POINT point to end the line.
 *  \param value greyscale intensity of the line.
 *  \param line_width pixel width of the line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DrawGreyscaleLine (FIBITMAP *src, POINT p1, POINT p2, double value, int line_width);

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
FreeImageAlgorithms_FloodFill(FIBITMAP* src, int seed_x, int seed_y, int fill_colour);

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
FreeImageAlgorithms_InPlaceFloodFill(FIBITMAP* src, int seed_x, int seed_y, int fill_colour);

#ifdef __cplusplus
}
#endif

#endif