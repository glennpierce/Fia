#ifndef __FREEIMAGE_ALGORITHMS_FFT__
#define __FREEIMAGE_ALGORITHMS_FFT__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
*	Provides FFT methods.
*/ 

/** \brief Creates a HBITMAP
 *	
 *  \param src FIBITMAP 
 *	\param inverse Whether to perform a inverse fourier transform.
 *	\param shift Shift the Zero frequency to the center of the image.
 *  \return FIBITMAP* on success and NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FFT(FIBITMAP *src, int inverse, int shift);

/** \brief Creates a FIT_DOUBLE absolute image from a complex image.
 *	
 *  \param src FIBITMAP complex image.
 *  \return FIBITMAP* on success and NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_ConvertComplexImageToAbsoluteValued(FIBITMAP *src);

#ifdef __cplusplus
}
#endif

#endif