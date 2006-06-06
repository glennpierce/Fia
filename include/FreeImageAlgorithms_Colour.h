#ifndef __FREEIMAGE_ALGORITHMS_COLOUR__
#define __FREEIMAGE_ALGORITHMS_COLOUR__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

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