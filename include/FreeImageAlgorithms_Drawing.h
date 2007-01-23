#ifndef __FREEIMAGE_ALGORITHMS_DRAWING__
#define __FREEIMAGE_ALGORITHMS_DRAWING__

/*! \file 
	Provides various utility methods.
*/ 

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Draw a line on an image.
 *
 *	This function performs a in place threshold on the src image.
 *	An 8bit image is return not a 1bit image.
 *
 *  \param src Image to draw on.
 *  \param p1 POINT point to start the line
 *  \param p2 POINT point to end the line.
 *  \param colour COLORREF colour of the line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DrawColourLine (FIBITMAP *src, POINT p1, POINT p2, COLORREF colour, int line_width);


DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DrawColourRect (FIBITMAP *src, RECT rect, COLORREF colour, int line_width);

#ifdef __cplusplus
}
#endif

#endif