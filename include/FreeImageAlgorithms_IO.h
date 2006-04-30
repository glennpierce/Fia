#ifndef __FREEIMAGE_ALGORITHMS_IO__
#define __FREEIMAGE_ALGORITHMS_IO__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadFIBFromFile(const char *filepath);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SaveFIBToFile (FIBITMAP *dib, const char *filepath);

void DLL_CALLCONV
FreeImageAlgorithms_CopyBytesToFBitmap(FIBITMAP *src, char *data, int padded);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadGreyScaleFIBFromArrayData (char *data, int bpp, int width, int height, FREE_IMAGE_TYPE data_type, int padded);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadColourFIBFromArrayData (char *data, int bpp, int width, int height, int padded);

#ifdef __cplusplus
}
#endif

#endif