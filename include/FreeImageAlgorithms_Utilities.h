#ifndef __FREEIMAGE_ALGORITHMS_UTILITIES__
#define __FREEIMAGE_ALGORITHMS_UTILITIES__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindCharMinMax(const char *data, long n, char *min, char *max);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindIntMinMax(const int *data, long n, int *min, int *max);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindShortMinMax(const short *data, long n, short *min, short *max);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindUShortMinMax(const unsigned short *data, long n, unsigned short *min, unsigned short *max);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindFloatMinMax(const float *data, long n, float *min, float *max);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindDoubleMinMax(const double *data, long n, double *min, double *max);

DLL_API long DLL_CALLCONV
FreeImageAlgorithms_FindCharMax(const char *data, long n, char *max);

DLL_API long DLL_CALLCONV
FreeImageAlgorithms_FindUCharMax(const unsigned char *data, long n, unsigned char *max);

DLL_API long DLL_CALLCONV
FreeImageAlgorithms_FindIntMax(const int *data, long n, int *max);

DLL_API long DLL_CALLCONV
FreeImageAlgorithms_FindShortMax(const short *data, long n, short *max);

DLL_API long DLL_CALLCONV
FreeImageAlgorithms_FindUShortMax(const unsigned short *data, long n, unsigned short *max);

DLL_API long DLL_CALLCONV
FreeImageAlgorithms_FindFloatMax(const float *data, long n, float *max);

DLL_API long DLL_CALLCONV
FreeImageAlgorithms_FindDoubleMax(const double *data, long n, double *max);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_FindMinMax(FIBITMAP *src, double *min, double *max);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_CharArrayReverse(char *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_UCharArrayReverse(unsigned char *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_ShortArrayReverse(short *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_UShortArrayReverse(unsigned short *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_IntArrayReverse(int *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_UIntArrayReverse(unsigned int *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_LongArrayReverse(long *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_ULongArrayReverse(unsigned long *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_FloatArrayReverse(float *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_DoubleArrayReverse(double *array, long size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_Is16BitReally12BitImage(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_IsGreyScale(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetCharPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, char *values); 

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetShortPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, short *values); 

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetUShortPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, unsigned short *values); 

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetFloatPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, float *values); 

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetRGBPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, char *red_hist, char *green_hist, char *blue_hist);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_RGBToHSV( unsigned char red, unsigned char green, unsigned char blue,
							double *hue, double *satuation, double *value);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_HSVToRGB( double hue, double satuation, double value,
							unsigned char *red, unsigned char *green, unsigned char *blue);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_RGBToHSL( unsigned char red, unsigned char green, unsigned char blue,
							double *hue, double *satuation, double *luminosity);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_HSLToRGB( double hue, double satuation, double luminosity,
							unsigned char *red, unsigned char *green, unsigned char *blue);

#ifdef __cplusplus
}
#endif

#endif