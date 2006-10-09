#ifndef __FREEIMAGE_ALGORITHMS_CONVOLUTION__
#define __FREEIMAGE_ALGORITHMS_CONVOLUTION__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   int	 matrixWidth;		   /* convolution matrix width			*/
   int	 matrixHeight;		   /* convolution matrix heigth			*/
   float *matrixElements;	   /* pointer to convolution matrix		*/
   float divider;			   /* divider (0.0 means matrix sum)	*/

} FreeImageAlgorithmsConvoMatrix, *FreeImageAlgorithmsConvoMatrixPtr;

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, float *kernel);

#ifdef __cplusplus
}
#endif

#endif