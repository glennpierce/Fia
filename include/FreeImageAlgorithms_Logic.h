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

/** \brief Masks an image.
 *
 *  Reverses a mask image. Changes 0 pixels to foreground value and others to 0.
 *
 *  \param mask FIBITMAP Mask bitmap to mask.
 *  \param foreground_val int new value for the foreground pixel.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_ReverseMaskImage(FIBITMAP* mask, unsigned char foreground_val);

#ifdef __cplusplus
}
#endif

#endif
