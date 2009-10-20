#ifndef __FREEIMAGE_ALGORITHMS_TESTING__
#define __FREEIMAGE_ALGORITHMS_TESTING__

#include "FreeImageAlgorithms.h"
#include "Constants.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

int MkDir(const char *path);

#ifdef __cplusplus
}
#endif

#endif
