#ifndef __FREEIMAGE_ALGORITHMS_UTILITIES__
#define __FREEIMAGE_ALGORITHMS_UTILITIES__

/*! \file 
	Provides various utility methods.
*/ 

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_API int DLL_CALLCONV
_os_support(int feature);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_SSEFindFloatMinMax(const float *data, long n, float *min, float *max);

/** \brief Find the mininum and maximum values in a char array.
 *
 *  \param data Array of char data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindCharMinMax(const char *data, long n, char *min, char *max);

/** \brief Find the mininum and maximum values in a int array.
 *
 *  \param data Array of int data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindIntMinMax(const int *data, long n, int *min, int *max);

/** \brief Find the mininum and maximum values in a short array.
 *
 *  \param data Array of short data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindShortMinMax(const short *data, long n, short *min, short *max);

/** \brief Find the mininum and maximum values in a unsigned short array.
 *
 *  \param data Array of unsigned short data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindUShortMinMax(const unsigned short *data, long n, unsigned short *min, unsigned short *max);

/** \brief Find the mininum and maximum values in a long array.
 *
 *  \param data Array of long data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindLongMinMax(const long *data, long n, long *min, long *max);

/** \brief Find the mininum and maximum values in a unsigned long array.
 *
 *  \param data Array of unsigned long data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindULongMinMax(const unsigned long *data, long n, unsigned long *min, unsigned long *max);

/** \brief Find the mininum and maximum values in a float array.
 *
 *  \param data Array of float data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindFloatMinMax(const float *data, long n, float *min, float *max);

/** \brief Find the mininum and maximum values in a double array.
 *
 *  \param data Array of double data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindDoubleMinMax(const double *data, long n, double *min, double *max);

/** \brief Find the maximum values in a char array.
 *
 *  \param data Array of char data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return char the max value found.
*/
DLL_API char DLL_CALLCONV
FreeImageAlgorithms_FindCharMax(const char *data, long n, char *max);

/** \brief Find the maximum values in a unsigned char array.
 *
 *  \param data Array of unsigned char data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return unsigned char the max value found.
*/
DLL_API unsigned char DLL_CALLCONV
FreeImageAlgorithms_FindUCharMax(const unsigned char *data, long n, unsigned char *max);

/** \brief Find the maximum values in a int array.
 *
 *  \param data Array of int data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return int the max value found.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_FindIntMax(const int *data, long n, int *max);

/** \brief Find the maximum values in a short int array.
 *
 *  \param data Array of short int data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return short the max value found.
*/
DLL_API short DLL_CALLCONV
FreeImageAlgorithms_FindShortMax(const short *data, long n, short *max);

/** \brief Find the maximum values in a unsigned short array.
 *
 *  \param data Array of unsigned short data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return unsigned short the max value found.
*/
DLL_API unsigned short DLL_CALLCONV
FreeImageAlgorithms_FindUShortMax(const unsigned short *data, long n, unsigned short *max);

/** \brief Find the maximum values in a float array.
 *
 *  \param data Array of float data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return float the max value found.
*/
DLL_API float DLL_CALLCONV
FreeImageAlgorithms_FindFloatMax(const float *data, long n, float *max);

/** \brief Find the maximum values in a double array.
 *
 *  \param data Array of double data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return double the max value found.
*/
DLL_API double DLL_CALLCONV
FreeImageAlgorithms_FindDoubleMax(const double *data, long n, double *max);

/** \brief Find the mininum and maximum values in a greyscale FIBITMAP.
 *
 *  \param src FIBITMAP bitmap.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindMinMax(FIBITMAP *src, double *min, double *max);

/** \brief Find the mininum and maximum values in a colour FIBITMAP.
 *
 *  \param src FIBITMAP bitmap.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindMinMaxForColourImage(FIBITMAP *src, double *min, double *max);

/** \brief Reverse a array of char.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_CharArrayReverse(char *array, long size);

/** \brief Reverse a array of unsigned char.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_UCharArrayReverse(unsigned char *array, long size);

/** \brief Reverse a array of short int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_ShortArrayReverse(short *array, long size);

/** \brief Reverse a array of unsigned short int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_UShortArrayReverse(unsigned short *array, long size);

/** \brief Reverse a array of int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_IntArrayReverse(int *array, long size);

/** \brief Reverse a array of unsigned int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_UIntArrayReverse(unsigned int *array, long size);

/** \brief Reverse a array of long int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_LongArrayReverse(long *array, long size);

/** \brief Reverse a array of unsigned long int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_ULongArrayReverse(unsigned long *array, long size);

/** \brief Reverse a array of float.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_FloatArrayReverse(float *array, long size);

/** \brief Reverse a array of double.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DoubleArrayReverse(double *array, long size);

/** \brief Return 1 if a 16bit image is really 12bit.
 *
 *  \param src FIBITMAP bitmap.
 *  \return 1 if the image is 12bit 0 if not.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_Is16BitReally12BitImage(FIBITMAP *src);

/** \brief Returns the maximum allowed value for a greyscale FREE_IMAGE_TYPE type.
 *
 *  \param type FREE_IMAGE_TYPE type.
 *  \param *max Returned value.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_GetMaxPosibleValueForGreyScaleType(FREE_IMAGE_TYPE type, double *max);

/** \brief Returns the minimum allowed value for a greyscale FREE_IMAGE_TYPE type.
 *
 *  \param type FREE_IMAGE_TYPE type.
 *  \param *min Returned value.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_GetMinPosibleValueForGreyScaleType(FREE_IMAGE_TYPE type, double *min);

/** \brief Returns the maximum allowed value for a FIBITMAP image.
 *
 *  \param src FIBITMAP bitmap.
 *  \param *max Returned value.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_GetMaxPosibleValueForFib(FIBITMAP *src, double *max);

/** \brief Returns the minimum allowed value for a FIBITMAP image.
 *
 *  \param src FIBITMAP bitmap.
 *  \param *min Returned value.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_GetMinPosibleValueForFib(FIBITMAP *src, double *min);

/** \brief Returns 1 if the FIBITMAP is a greyscale image.
 *
 *	This function returns 0 for 16 bit and higher colour images.
 *	It returns 1 for greyscale images or palettised images. Note the palatte could
 *  be colour so be careful.
 *
 *  \param src FIBITMAP bitmap.
 *  \return 1 if the image is greyscale.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_IsGreyScale(FIBITMAP *src);

/** \brief Gets the pixels values along a line on an image where the 
 *         image consists of char data.
 *
 *  \param src FIBITMAP bitmap.
 *  \param p1  Start point of line.
 *  \param p2  End point of line.
 *  \param values Returned values along line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetCharPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, char *values); 

/** \brief Gets the pixels values along a line on an image where the 
 *         image consists of unsigned char data.
 *
 *  \param src FIBITMAP bitmap.
 *  \param p1  Start point of line.
 *  \param p2  End point of line.
 *  \param values Returned values along line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetUCharPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, unsigned char *values);

/** \brief Gets the pixels values along a line on an image where the 
 *         image consists of short int data.
 *
 *  \param src FIBITMAP bitmap.
 *  \param p1  Start point of line.
 *  \param p2  End point of line.
 *  \param values Returned values along line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetShortPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, short *values); 

/** \brief Gets the pixels values along a line on an image where the 
 *         image consists of unsigned short data.
 *
 *  \param src FIBITMAP bitmap.
 *  \param p1  Start point of line.
 *  \param p2  End point of line.
 *  \param values Returned values along line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetUShortPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, unsigned short *values); 

/** \brief Gets the pixels values along a line on an image where the 
 *         image consists of float data.
 *
 *  \param src FIBITMAP bitmap.
 *  \param p1  Start point of line.
 *  \param p2  End point of line.
 *  \param values Returned values along line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetFloatPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, float *values); 

/** \brief Gets the pixels values along a line on an image where the 
 *         image consists of double data.
 *
 *  \param src FIBITMAP bitmap.
 *  \param p1  Start point of line.
 *  \param p2  End point of line.
 *  \param values Returned values along line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetDoublePixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, double *values); 

/** \brief Gets the pixels values along a line on an image where the 
 *         image is a colour image.
 *
 *  \param src FIBITMAP bitmap.
 *  \param p1  Start point of line.
 *  \param p2  End point of line.
 *  \param red_values Returned red values along line.
 *  \param green_values Returned green values along line.
 *  \param blue_values Returned blue values along line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetRGBPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, char *red_values, char *green_values, char *blue_values);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_GetDistanceMap (int width, int height, float *distance_map);

// Hopefully this is a temp solution until FreeImage paste supports greyscale that is
// above 8bits.
// This function is very limited.
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SimplePaste(FIBITMAP *dst, FIBITMAP *src, int left, int top);

/** \brief Performs a bitwise compare between two images.
 *		   Returns 1 for images that are the same.
 *
 *  \param dib1 first FIBITMAP bitmap.
 *  \param dib2 second FIBITMAP bitmap.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_BitwiseCompare(FIBITMAP *dib1, FIBITMAP *dib2);

/** \brief Clones an image type.
 *
 *  This function clones the type not the image data. Ie it clones the bpp, pitch etc.
 *
 *  \param src FIBITMAP bitmap to clone.
 *  \param width Width of the bitmap.
 *  \param height Height of the bitmap.
 *  \return FIBITMAP* on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_CloneImageType(FIBITMAP *src, int width, int height);

/** \brief Converts to a float image even if the image is in colour.
 *
 *  \param src Image to convert.
 *  \param type FREE_IMAGE_TYPE type to convert to. Can be FIT_FLOAT or FIT_DOUBLE.
 *  \return FIT_FLOAT image.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_ConvertToGreyscaleFloatType(FIBITMAP *src, FREE_IMAGE_TYPE type);

/** \brief Performs a threshold on greyscale images.
 *
 *	This function performs a threshold on the src image.
 *	An 8bit image is returned not a 1bit image.
 *  The new value is used for all values between min and max.
 *  Below min pixels are replaced by 0 and above max they are replaced by 255. 
 *
 *  \param src Image to threshold.
 *  \param min double minimum value to threshold.
 *  \param max double maximum value to threshold.
 *  \param new_value double new value to use.
 *  \return FIBITMAP* Returns FIBITMAP* on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Threshold(FIBITMAP *src, double min, double max, double new_value);

/** \brief Performs a in place threshold on greyscale images.
 *
 *	This function performs a threshold on the src image.
 *	An 8bit image is returned not a 1bit image.
 *  The new value is used for all values between min and max.
 *  Below min pixels are replaced by 0 and above max they are replaced by 255. 
 *
 *  \param src Image to threshold.
 *  \param min double minimum value to threshold.
 *  \param max double maximum value to threshold.
 *  \param new_value double new value to use.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_InPlaceThreshold(FIBITMAP *src, double min, double max, double new_value);


/** \brief Find if an image is 8bit.
 *
 *  \param src Image to check.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_Is8Bit(FIBITMAP *src);

/** \brief Compute a distance transform of an image.
 *
 *  \param src Image to transform.
 *  \return FIBITMAP* Returns FIBITMAP* on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_DistanceTransform(FIBITMAP *src);

/** \brief Get the value of a particular pixel.
 * 
 *	Does not check the position is valid works with all greyscale types.
 *
 *  \param src Image to get pixel value from.
 *  \param x int The x position.
 *  \param y int The y position.
 *  \param val float* The returned value.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetPixelValve(FIBITMAP *src, int x, int y, float* val);

#ifdef __cplusplus
}
#endif

#endif
