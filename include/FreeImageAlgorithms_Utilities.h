/*
 * Copyright 2007 Glenn Pierce
 *
 * This file is part of FIA.
 *
 * FIA is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FIA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public License
 * along with FIA.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __FREEIMAGE_ALGORITHMS_UTILITIES__
#define __FREEIMAGE_ALGORITHMS_UTILITIES__

/*! \file
	Provides various utility methods.
*/

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Make a FIAPOINT structure.
 *
 *  \param data Array of char data.
 *  \param x x point of point.
 *  \param y y point of point.
 *  \return FIAPOINT point.
*/
DLL_API FIAPOINT DLL_CALLCONV
MakeFIAPoint(int x, int y);

/** \brief Make a FIARECT structure.
 *
 *  \param data Array of char data.
 *  \param left left point of rect.
 *  \param top top point of rect.
 *  \param right right point of rect.
 *  \param bottom bottom point of rect.
 *  \return FIARECT rectangle.
*/
DLL_API FIARECT DLL_CALLCONV
MakeFIARect(int left, int top, int right, int bottom);

DLL_API int DLL_CALLCONV
FIARectIsEmpty (FIARECT rect);
/** \brief Make a FIARECT structure from an image.
 *
 *  \param data Array of char data.
 *  \param src Image to construct FIARECT from.
 *  \return FIARECT rectangle.
*/
DLL_API FIARECT DLL_CALLCONV
FIAImageRect(FIBITMAP *src);

/** \brief Checks the dimesions of two images are the same.
 *
 *  \param src1 First image to compare.
 *  \param src2 Second image to compare.
 *  \return FIARECT rectangle.
*/
DLL_API int DLL_CALLCONV
FIA_CheckSizesAreSame(FIBITMAP *fib1, FIBITMAP *fib2);

DLL_API int DLL_CALLCONV
_os_support(int feature);

DLL_API void DLL_CALLCONV
FIA_SSEFindFloatMinMax(const float *data, long n, float *min, float *max);

/** \brief Find the mininum and maximum values in a char array.
 *
 *  \param data Array of char data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindCharMinMax(const char *data, long n, char *min, char *max);

/** \brief Find the mininum and maximum values in a int array.
 *
 *  \param data Array of int data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindIntMinMax(const int *data, long n, int *min, int *max);

/** \brief Find the mininum and maximum values in a short array.
 *
 *  \param data Array of short data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindShortMinMax(const short *data, long n, short *min, short *max);

/** \brief Find the mininum and maximum values in a unsigned short array.
 *
 *  \param data Array of unsigned short data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindUShortMinMax(const unsigned short *data, long n, unsigned short *min, unsigned short *max);

/** \brief Find the mininum and maximum values in a long array.
 *
 *  \param data Array of long data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindLongMinMax(const long *data, long n, long *min, long *max);

/** \brief Find the mininum and maximum values in a unsigned long array.
 *
 *  \param data Array of unsigned long data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindULongMinMax(const unsigned long *data, long n, unsigned long *min, unsigned long *max);

/** \brief Find the mininum and maximum values in a float array.
 *
 *  \param data Array of float data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindFloatMinMax(const float *data, long n, float *min, float *max);

/** \brief Find the mininum and maximum values in a double array.
 *
 *  \param data Array of double data.
 *  \param n Number of entries in the array.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindDoubleMinMax(const double *data, long n, double *min, double *max);

/** \brief Find the maximum values in a char array.
 *
 *  \param data Array of char data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return char the max value found.
*/
DLL_API long DLL_CALLCONV
FIA_FindCharMax(const char *data, long n, char *max);

/** \brief Find the maximum values in a unsigned char array.
 *
 *  \param data Array of unsigned char data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return unsigned char the max value found.
*/
DLL_API long DLL_CALLCONV
FIA_FindUCharMax(const unsigned char *data, long n, unsigned char *max);

/** \brief Find the maximum values in a int array.
 *
 *  \param data Array of int data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return int the max value found.
*/
DLL_API long DLL_CALLCONV
FIA_FindIntMax(const int *data, long n, int *max);

/** \brief Find the maximum values in a short int array.
 *
 *  \param data Array of short int data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return short the max value found.
*/
DLL_API long DLL_CALLCONV
FIA_FindShortMax(const short *data, long n, short *max);

/** \brief Find the maximum values in a unsigned short array.
 *
 *  \param data Array of unsigned short data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return unsigned short the max value found.
*/
DLL_API long DLL_CALLCONV
FIA_FindUShortMax(const unsigned short *data, long n, unsigned short *max);

/** \brief Find the maximum values in a float array.
 *
 *  \param data Array of float data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return float the max value found.
*/
DLL_API long DLL_CALLCONV
FIA_FindFloatMax(const float *data, long n, float *max);

/** \brief Find the maximum values in a double array.
 *
 *  \param data Array of double data.
 *  \param n Number of entries in the array.
 *  \param max Maximum value to found in the data.
 *  \return double the max value found.
*/
DLL_API long DLL_CALLCONV
FIA_FindDoubleMax(const double *data, long n, double *max);

/** \brief Find the mininum and maximum values in a greyscale FIBITMAP.
 *
 *  \param src FIBITMAP bitmap.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindMinMax(FIBITMAP *src, double *min, double *max);

/** \brief Find the maximum value and the given x y point in a greyscale FIBITMAP.
 *
 *  \param src FIBITMAP bitmap.
 *  \param max Maximum value to found in the data.
 *  \param FIAPOINT* the position that the maximum value is found at.
*/
DLL_API void DLL_CALLCONV
FIA_FindMaxXY(FIBITMAP *src, double *max, FIAPOINT *pt);

/** \brief Find the mininum and maximum values in a colour FIBITMAP.
 *
 *  \param src FIBITMAP bitmap.
 *  \param min Mininum value to found in the data.
 *  \param max Maximum value to found in the data.
*/
DLL_API void DLL_CALLCONV
FIA_FindMinMaxForColourImage(FIBITMAP *src, double *min, double *max);

/** \brief Reverse a array of char.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_CharArrayReverse(char *array, long size);

/** \brief Reverse a array of unsigned char.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_UCharArrayReverse(unsigned char *array, long size);

/** \brief Reverse a array of short int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_ShortArrayReverse(short *array, long size);

/** \brief Reverse a array of unsigned short int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_UShortArrayReverse(unsigned short *array, long size);

/** \brief Reverse a array of int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_IntArrayReverse(int *array, long size);

/** \brief Reverse a array of unsigned int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_UIntArrayReverse(unsigned int *array, long size);

/** \brief Reverse a array of long int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_LongArrayReverse(long *array, long size);

/** \brief Reverse a array of unsigned long int.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_ULongArrayReverse(unsigned long *array, long size);

/** \brief Reverse a array of float.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_FloatArrayReverse(float *array, long size);

/** \brief Reverse a array of double.
 *
 *  \param array Array to reverse.
 *  \param size Size of the array.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_DoubleArrayReverse(double *array, long size);

/** \brief Return 1 if a 16bit image is really 12bit.
 *
 *  \param src FIBITMAP bitmap.
 *  \return 1 if the image is 12bit 0 if not.
*/
DLL_API int DLL_CALLCONV
FIA_Is16BitReally12BitImage(FIBITMAP *src);

/** \brief Returns the maximum allowed value for a greyscale FREE_IMAGE_TYPE type.
 *
 *  \param type FREE_IMAGE_TYPE type.
 *  \param *max Returned value.
*/
DLL_API void DLL_CALLCONV
FIA_GetMaxPosibleValueForGreyScaleType(FREE_IMAGE_TYPE type, double *max);

/** \brief Returns the minimum allowed value for a greyscale FREE_IMAGE_TYPE type.
 *
 *  \param type FREE_IMAGE_TYPE type.
 *  \param *min Returned value.
*/
DLL_API void DLL_CALLCONV
FIA_GetMinPosibleValueForGreyScaleType(FREE_IMAGE_TYPE type, double *min);

/** \brief Returns the maximum allowed value for a FIBITMAP image.
 *
 *  \param src FIBITMAP bitmap.
 *  \param *max Returned value.
*/
DLL_API void DLL_CALLCONV
FIA_GetMaxPosibleValueForFib(FIBITMAP *src, double *max);

/** \brief Returns the minimum allowed value for a FIBITMAP image.
 *
 *  \param src FIBITMAP bitmap.
 *  \param *min Returned value.
*/
DLL_API void DLL_CALLCONV
FIA_GetMinPosibleValueForFib(FIBITMAP *src, double *min);

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
FIA_IsGreyScale(FIBITMAP *src);

/** \brief Gets the pixels values along a line on an image where the 
 *         image consists of char data.
 *
 *  \param src FIBITMAP bitmap.
 *  \param p1  Start point of line.
 *  \param p2  End point of line.
 *  \param type FREE_IMAGE_TYPE of the pixels required. Must match the image type. Assures user is using correct type.
 *  \param values Returned values along line.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_GetGreyScalePixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, void *values);

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
FIA_GetCharPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, char *values); 

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
FIA_GetUCharPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, unsigned char *values);

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
FIA_GetShortPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, short *values);

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
FIA_GetUShortPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, unsigned short *values);

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
FIA_GetFloatPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, float *values);

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
FIA_GetDoublePixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, double *values);

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
FIA_GetRGBPixelValuesForLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, BYTE *red_values, BYTE *green_values, BYTE *blue_values);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_CopyLeftTopWidthHeight ( FIBITMAP * src, int left, int top, int width, int height);
DLL_API int DLL_CALLCONV
FIA_IntersectingRect(FIARECT r1, FIARECT r2, FIARECT *r3);

DLL_API int DLL_CALLCONV
FIA_Paste(FIBITMAP *dst, FIBITMAP *src, int left, int bottom);

DLL_API int DLL_CALLCONV
FIA_PasteFromTopLeft (FIBITMAP *dst, FIBITMAP * src, int left, int top);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_Copy (FIBITMAP * src, int left, int top, int right, int bottom);

DLL_API void DLL_CALLCONV
FIA_RectChangeWidthHeight (FIARECT *rect, int width, int height);

DLL_API int DLL_CALLCONV
FIA_RectWidth (FIARECT *rect);

DLL_API int DLL_CALLCONV
FIA_RectHeight (FIARECT *rect);

DLL_API BYTE* DLL_CALLCONV
FIA_GetScanLineFromTop (FIBITMAP *src, int line);
DLL_API FIARECT DLL_CALLCONV
FIA_MakeFiaRectRelativeToImageBottomLeft (FIBITMAP *src, FIARECT rt);

/** \brief Performs a bitwise compare between two images.
 *		   Returns 1 for images that are the same.
 *
 *  \param dib1 first FIBITMAP bitmap.
 *  \param dib2 second FIBITMAP bitmap.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_BitwiseCompare(FIBITMAP *dib1, FIBITMAP *dib2);

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
FIA_CloneImageType(FIBITMAP *src, int width, int height);

/** \brief Converts to a float image even if the image is in colour.
 *
 *  \param src Image to convert.
 *  \param type FREE_IMAGE_TYPE type to convert to. Can be FIT_FLOAT or FIT_DOUBLE.
 *  \return FIT_FLOAT image.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_ConvertToGreyscaleFloatType(FIBITMAP *src, FREE_IMAGE_TYPE type);

DLL_API int DLL_CALLCONV
FIA_InPlaceConvertTo8Bit (FIBITMAP ** src);

DLL_API int DLL_CALLCONV
FIA_InPlaceConvertToGreyscaleFloatType(FIBITMAP **src, FREE_IMAGE_TYPE type);

DLL_API int DLL_CALLCONV
FIA_InPlaceConvertTo32Bit (FIBITMAP ** src);

DLL_API int DLL_CALLCONV
FIA_InPlaceConvertTo24Bit (FIBITMAP ** src);
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
FIA_Threshold(FIBITMAP *src, double min, double max, double new_value);

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
FIA_InPlaceThreshold(FIBITMAP *src, double min, double max, double new_value);


/** \brief Find if an image is 8bit.
 *
 *  \param src Image to check.
 *  \return int FREEIMAGE_ALGORITHMS_SUCCESS on success or FREEIMAGE_ALGORITHMS_ERROR on error.
*/
DLL_API int DLL_CALLCONV
FIA_Is8Bit(FIBITMAP *src);

/** \brief Compute a distance transform of an image.
 *
 *  \param src Image to transform.
 *  \return FIBITMAP* Returns FIBITMAP* on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_DistanceTransform(FIBITMAP *src);

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
FIA_GetPixelValue(FIBITMAP *src, int x, int y, double* val);

DLL_API int DLL_CALLCONV
FIA_InPlaceConvertToStandardType(FIBITMAP **src, int scale);

DLL_API int DLL_CALLCONV
FIA_InPlaceConvertToGreyscale(FIBITMAP **src);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_ConvertFloatTo16Bit(FIBITMAP *src, int sign);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_ConvertInt16ToUInt16(FIBITMAP *src);

/** \brief Rescales an image to half its size.
 *
 *	This is much much faster than the freeimage rescale function.
 *  Obviously it is also much more limited.
 *
 *  \param src Image to resize.
 *  \return FIBITMAP* Returns FIBITMAP* on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_RescaleToHalf(FIBITMAP *src);

DLL_API FIBITMAP* DLL_CALLCONV
FIA_GradientBlendedIntersectionImage (FIBITMAP * src1, FIARECT rect1, FIBITMAP* src2, FIARECT rect2,
									  FIBITMAP *mask,
									  FIARECT *intersect_image_rect);

/** \brief Gradient blends one image into another
 *
 *  This function blends the src image into dest.
 *  If the images are overlapping then the gradient goes from zero to its maximum value 
 *	in the space of the overlap region of the tow images.
 *
 *	If src is completly within the destination image then the behaviour is slightly difference.
 *	The gradient goes from zero at the edge and reaches its maximum at the center 
 *	of the intersection.
 *
 *  \param dst Image to blend into.
 *  \param src Image to blend.
 *  \return FIBITMAP* Returns FIBITMAP* on success or NULL on error.
*/
DLL_API int DLL_CALLCONV
FIA_GradientBlend (FIBITMAP * src1, FIARECT rect1, FIBITMAP* src2, FIARECT rect2, FIBITMAP *mask);

/** \brief Gradient blends one image into another
 *
 *  This function blends the src image into dest.
 *  If the images are overlapping then the gradient goes from zero to its maximum value 
 *	in the space of the overlap region of the tow images.
 *
 *	If src is completly within the destination image then the behaviour is slightly difference.
 *	The gradient goes from zero at the edge and reaches its maximum at the center 
 *	of the intersection.
 *
 *  \param dst Image to blend into.
 *  \param src Image to blend.
 *  \return FIBITMAP* Returns FIBITMAP* on success or NULL on error.
*/

/** \brief Gradient blends one image into another
 *
 *  This function blends the src image into dest.
 *  If the images are overlapping then the gradient goes from zero to its maximum value 
 *	in the space of the overlap region of the tow images.
 *
 *	If src is completly within the destination image then the behaviour is slightly difference.
 *	The gradient goes from zero at the edge and reaches its maximum at the center 
 *	of the intersection.
 *
 *  The return value is a 32 bit image with the alpha values set appropriately.
 *  The intersect_rect of where src need to be pasted into dst is also returned.
 *
 *  \param dst Image to blend into.
 *  \param src Image to blend.
 *  \return FIBITMAP* Returns FIBITMAP* on success or NULL on error.
*/
DLL_API FIBITMAP* DLL_CALLCONV
FIA_GetGradientBlendAlphaImage (FIBITMAP* src2, FIARECT rect1, FIARECT rect2, FIARECT *intersect_rect);

DLL_API int DLL_CALLCONV
FIA_CompositeRegion(FIBITMAP * fg, FIBITMAP * bg, FIARECT rect, FIBITMAP *mask);

DLL_API FIBITMAP *DLL_CALLCONV
FIA_Composite(FIBITMAP * fg, FIBITMAP * bg, FIBITMAP * normalised_alpha_values, FIBITMAP *mask);

DLL_API int DLL_CALLCONV
FIA_Combine(FIBITMAP *dst, FIBITMAP *fg, FIBITMAP *mask);

DLL_API int DLL_CALLCONV
FIA_GradientBlendMosaicPaste (FIBITMAP* dst, FIBITMAP* src, int x, int y);
#ifdef __cplusplus
}
#endif

#endif
