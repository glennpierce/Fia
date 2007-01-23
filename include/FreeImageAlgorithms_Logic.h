#ifndef __FREEIMAGE_ALGORITHMS_LOGIC__
#define __FREEIMAGE_ALGORITHMS_LOGIC__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
	Provides various logic methods.
*/ 

/** \brief Masks an image.
 *
 *  This function masks the image data.
 *
 *  \param src FIBITMAP bitmap to mask.
 *  \param mask FIBITMAP bitmap used as mask. Must be an 8bit image.
*/
DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_MaskImage(FIBITMAP* src, FIBITMAP* mask);

#ifdef __cplusplus
}
#endif

#endif