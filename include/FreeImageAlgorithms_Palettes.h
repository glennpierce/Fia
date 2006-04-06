#ifndef __FREEIMAGE_ALGORITHMS_PALETTES__
#define __FREEIMAGE_ALGORITHMS_PALETTES__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_CopyPaletteToRGBQUAD(FIBITMAP *src, RGBQUAD *palette);

DLL_API int DLL_CALLCONV		   
FreeImageAlgorithms_CopyPaletteFromRGBQUAD(FIBITMAP *dst, RGBQUAD *palette);

DLL_API int DLL_CALLCONV		   
FreeImageAlgorithms_CopyPalette(FIBITMAP *src, FIBITMAP *dst);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_ReversePaletteEntries(RGBQUAD *palette, unsigned char start, unsigned char size);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetGreyLevelPalette(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetLogColourPalette(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetRainBowPalette(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetTemperaturePalette(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetFalseColourPalette(FIBITMAP *src, double wavelength);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetGreyLevelOverLoadPalette(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetSeismicColourPalette(FIBITMAP *src);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelPalette(RGBQUAD *palette);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelOverLoadPalette(RGBQUAD *palette);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetRainBowPalette(RGBQUAD *palette);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetLogColourPalette(RGBQUAD *palette);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetFalseColourPalette(RGBQUAD *palette, double wavelength);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetTemperaturePalette(RGBQUAD *palette);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetSeismicColourPalette(RGBQUAD *palette);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetOpticalDensityPalette(RGBQUAD *palette, unsigned char red,
	unsigned char green, unsigned char blue, int contrast, int entries);

#ifdef __cplusplus
}
#endif

#endif