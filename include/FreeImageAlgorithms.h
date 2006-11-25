#ifndef __FREEIMAGE_ALGORITHMS__
#define __FREEIMAGE_ALGORITHMS__

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include "FreeImage.h"

/** Data structure for FIABITMAP type (FIBITMAP with a border)
*/
typedef struct
{
	/// FIBITMAP bitmap.
	FIBITMAP *fib;
	/// border size.
	int border; 

} FIABITMAP;

#define FREEIMAGE_ALGORITHMS_SUCCESS 1
#define FREEIMAGE_ALGORITHMS_ERROR 0

#define U12BIT_MAX 4096
#define U14BIT_MAX 16384

#define BIT12_MAX 2047
#define BIT12_MIN (-BIT12_MAX - 1)  

#define BIT14_MAX 8191
#define BIT14_MIN (-BIT14_MAX - 1)

#define _CPU_FEATURE_MMX    0x0001
#define _CPU_FEATURE_SSE    0x0002
#define _CPU_FEATURE_SSE2   0x0004
#define _CPU_FEATURE_3DNOW  0x0008

typedef enum {BIT8, BIT24} FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH;

#endif
