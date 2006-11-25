#ifndef __FREEIMAGE_ALGORITHMS_LINEAR_SCALE__
#define __FREEIMAGE_ALGORITHMS_LINEAR_SCALE__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
	Provides linear scaling of greylevel images.
*/ 

/** \brief Convert image of any type to a standard 8-bit greyscale image.
 *
 *  For standard images, a clone of the input image is returned.
 *  When the scale_linear parameter is TRUE, conversion is done by scaling linearly 
 *  each pixel to an integer value between [0..255]. When it is FALSE, conversion is done 
 *  by rounding each float pixel to an integer between [0..255]
 *  \param src Image to convert
 *  \param min Min value to stretch to.
 *  \param max Max value to stretch to.
 *  \param min_within_image Mininum value to found in original image.
 *  \param max_within_image Maximum value to found in original image.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LinearScaleToStandardType(FIBITMAP *src, double min, double max, double *min_within_image, double *max_within_image);

/** \brief Stretches an image to the full range of an image type.
 *
 *  Stretches an image to the full range of greyscale values possible for the 
 *  parameter type.
 *  \param src Image to convert
 *  \param type Type to stretch to.
 *  \param max Max value to stretch to.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_StretchImageToType(FIBITMAP *src, FREE_IMAGE_TYPE type, double max);

#ifdef __cplusplus
}
#endif

#endif