#ifndef __FREEIMAGE_ALGORITHMS_FIND_MAXIMA__
#define __FREEIMAGE_ALGORITHMS_FIND_MAXIMA__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
*	Provides methods when working with images of particles.
*/ 

/** Data structure for describing a peak in an image.
*/
typedef struct
{
  FIAPOINT centre;
  float value;

} FIAPeak;

/** Data structure for describing a blob in an image.
*/
typedef struct
{
	FIARECT rect;
	int area;
	int center_x;
	int center_y;

} BLOBINFO;

/** Data structure for describing all blobs in an image.
*/
typedef struct
{
	int number_of_blobs;
	BLOBINFO* blobs;

} PARTICLEINFO;


/** \brief Find information about particles or blobs in an image.
 *
 *  \param src FIBITMAP Image with blobs must be a binary 8bit image.
 *  \param info PARTICLEINFO** Address of pointer to hold particle information the pointer should be NULL.
 *  \param white_on_black unsigned char Determines the background intensity value.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_ParticleInfo(FIBITMAP* src, PARTICLEINFO** info, unsigned char white_on_black);


/** \brief Frees the data returned by FreeImageAlgorithms_ParticleInfo.
 *
 *  \param info PARTICLEINFO* pointer to particle information.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FreeParticleInfo(PARTICLEINFO* info);


/** \brief Fills the hole in a particle or blob image.
 *
 *  Image data is an 8bit binary image.
 *
 *  \param src FIBITMAP Image with blobs must be a binary 8bit image.
 *  \param white_on_black unsigned char Determines the background intensity value.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Fillholes(FIBITMAP* src,
							 unsigned char white_on_black);


/** \brief Finds the maxima within particles or blobs.
 *
 *  The code finds maxima in particles or blobs. Plateaux are cound as are peaks but
 *  shoulders are not. Shoulder are lower intensity regions attached to a peak.
 *
 *  Image data is greyscale data.
 *
 *  \param src FIBITMAP Greyscale image of particles.
 *  \param mask FIBITMAP Mask to first apply to the image.
 *  \param threshold unsigned char Threshold value to apply initially to image.
 *  \param min_separation int The mininum separation between particles.
 *  \param peaks FIAPeak ** The address of the pointer to hold returned peak positions.
 *  \param number int Number of peaks to find. If 0 then all peaks are returned.
 *  \param peaks_found int* Number of peaks returned.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FindImageMaxima(FIBITMAP* src, FIBITMAP *mask, unsigned char threshold,
						int min_separation, FIAPeak **peaks, int number, int *peaks_found);


#ifdef __cplusplus
}
#endif

#endif
