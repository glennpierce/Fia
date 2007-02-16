#ifndef __FREEIMAGE_ALGORITHMS_TESTING__
#define __FREEIMAGE_ALGORITHMS_TESTING__

#include "FreeImageAlgorithms.h"

#ifdef _HOMEWORKING_
#define IMAGE_DIR "C:\\Documents and Settings\\Glenn\\Desktop\\Working Area\\Test Images"
#else
#define IMAGE_DIR "C:\\Documents and Settings\\Pierce\\Desktop\\Working Area\\Test Images"
#endif

#define TEMP_DIR "C:\\Temp\\FreeImageAlgorithms_Tests"

#ifdef __cplusplus
extern "C" {
#endif

void
ShowImage(FIBITMAP *src);

void
ShowImageFromFile(char *filepath);


#ifdef __cplusplus
}
#endif

#endif