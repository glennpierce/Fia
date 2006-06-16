#ifndef __FREEIMAGE_ALGORITHMS__
#define __FREEIMAGE_ALGORITHMS__

//#define WIN32

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include "FreeImage.h"

#define FREEIMAGE_ALGORITHMS_SUCCESS 1
#define FREEIMAGE_ALGORITHMS_ERROR 0

#define U12BIT_MAX 4096
#define U14BIT_MAX 16384

#define BIT12_MAX 2047
#define BIT12_MIN (-BIT12_MAX - 1)  

#define BIT14_MAX 8191
#define BIT14_MIN (-BIT14_MAX - 1)

#endif
