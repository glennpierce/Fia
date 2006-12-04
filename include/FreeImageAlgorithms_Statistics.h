#ifndef __FREEIMAGE_ALGORITHMS_STATISTICS__
#define __FREEIMAGE_ALGORITHMS_STATISTICS__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \file 
	Provides various statistical methods for FIBITMAP's.
*/ 

/** \brief Equalise and image using histogram equalisation with random additions.
 *
 *  \param src FIBITMAP bitmap to perform the equalisation operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_HistEq_Random_Additions(FIBITMAP *src);

/** \brief Equalise and image using histogram equalisation.
 *
 *  \param src FIBITMAP bitmap to perform the equalisation operation on.
 *  \return FIBITMAP on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_HistEq(FIBITMAP *src);

/** \brief Calculate the greylevel average of all the pixels.
 *
 *  \param src FIBITMAP bitmap to perform the average calculation operation on. 
 *	           Must be a greylevel image.
 *  \return double Average value on success or 0.0 on error.
*/
DLL_API double DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelAverage(FIBITMAP *src);

/** \brief Return the histogram for a greylevel image.
 *
 *	This function is different from the FreeImage_GetHist as you can specify how
 *  to bin values.
 *
 *  \param src FIBITMAP bitmap to perform the histogram operation on.
 *  \param min The minimum value where binning or histogram counting begins.
 *  \param max The maximum value where binning or histogram counting ends.
 *  \param number_of_bins How many bins you want between min and max.
 *  \param hist Long pointer to the histogram data.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_Histogram(FIBITMAP *src, double min, double max, int number_of_bins, unsigned long *hist);

/** \brief Return the histogram for a rgb image.
 *
 *	This function is different from the FreeImage_GetHist as you can specify how
 *  to bin values.
 *
 *  \param src FIBITMAP bitmap to perform the histogram operation on.
 *  \param min The minimum value where binning or histogram counting begins.
 *  \param max The maximum value where binning or histogram counting ends.
 *  \param number_of_bins How many bins you want between min and max.
 *  \param rhist Long pointer to the red histogram data.
 *  \param ghist Long pointer to the green histogram data.
 *  \param bhist Long pointer to the blue histogram data.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_RGBHistogram(FIBITMAP *src, double min, double max, int number_of_bins,
			unsigned long *rhist, unsigned long *ghist, unsigned long *bhist);


/** \brief This function finds the the amount of white ie area in a monochrome image.
 *		   This works with 8 bit images by assuming everything above 1 is white.
 *  \param src FIBITMAP bitmap to perform the histogram operation on.
 *  \param white_area unsigned int * Counts of pixels above or equal to 1.
 */
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_MonoImageFindWhiteArea(FIBITMAP *src, unsigned int *white_area);

/** \brief This function finds the the amount of white ie area in a monochrome image.
 *		   This works with 8 bit images by assuming everything above 1 is white.
 *  \param src FIBITMAP bitmap to perform the histogram operation on.
 *  \param white_area double * Fraction of pixels above or equal to 1.
 *  \param black_area double * Fraction of pixels below 1.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
 */
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_MonoImageFindWhiteFraction(FIBITMAP *src, double *white_area, double *black_area);

/** \brief This function determines how a detail is present though two images.
 *
 *  \param src FIBITMAP bitmap to perform the comparison on.
 *  \param result FIBITMAP bitmap to perform the comparison on. This is the expected result image ie
 *						  gold standard.
 *  \param tp int * (True Positive) A detail present in src is also in result.
 *  \param tn int * (True Negative) A detail not in src is not in result ie two pixels that are 0.
 *  \param fp int * (False Positive) A detail not in src is in result.
 *  \param fn int * (False Negative) A detail in src is not in result.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
 */
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_MonoTrueFalsePositiveComparison(FIBITMAP *src, FIBITMAP *result,
													int *tp, int *tn, int *fp, int *fn);

#ifdef __cplusplus
}
#endif

#endif