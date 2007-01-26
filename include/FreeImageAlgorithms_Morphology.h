#ifndef __FREEIMAGE_ALGORITHMS_MORPHOLOGY_FILTER__
#define __FREEIMAGE_ALGORITHMS_MORPHOLOGY_FILTER__

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
 *	Dilates the particles in an image.
 *
 *  \param src FIBITMAP bitmap to perform the dilation operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_BinaryDilation(FIABITMAP* src, FilterKernel kernel);

/*! \file 
 *	Erodes the particles in an image.
 *
 *  \param src FIBITMAP bitmap to perform the erosion operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_BinaryErosion(FIABITMAP* src, FilterKernel kernel);

/*! \file 
 *	Erodes and then performs dialation.
 *
 *  \param src FIBITMAP bitmap to perform the erosion operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_BinaryOpening(FIABITMAP* src, FilterKernel kernel);

/*! \file 
 *	Diates then performs an erosion.
 *
 *  \param src FIBITMAP bitmap to perform the erosion operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_BinaryClosing(FIABITMAP* src, FilterKernel kernel);

#ifdef __cplusplus
}
#endif

#endif