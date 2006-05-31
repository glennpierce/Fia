#ifndef __FREEIMAGE_ALGORITHMS_TESTING__
#define __FREEIMAGE_ALGORITHMS_TESTING__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

#define TEST_IMAGE_DIRECTORY "..\\..\\..\\..\\Test Images"

DLL_API void DLL_CALLCONV
ShowImage(FIBITMAP *src);

DLL_API void DLL_CALLCONV
ShowImageFromFile(char *filepath);


#ifdef __cplusplus
}
#endif

#endif