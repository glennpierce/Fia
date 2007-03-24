#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution_Private.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <math.h>

FilterKernel DLL_CALLCONV
FreeImageAlgorithms_NewKernel(int x_radius, int y_radius,
							  const double *values, double divider)
{
	FilterKernel kernel;

	kernel.x_radius = x_radius;
	kernel.y_radius = y_radius;
	kernel.values = values;
	kernel.divider = divider;

	return kernel;
}

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve(FIABITMAP *src, FilterKernel kernel)
{
	FIBITMAP *dst = NULL;
	FIABITMAP *tmp = NULL;
	FIABITMAP border_tmp;

	if(!src)
		return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if(src_type == FIT_COMPLEX)
        return NULL;

    border_tmp.fib = FreeImageAlgorithms_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    border_tmp.xborder = src->xborder;
	border_tmp.yborder = src->yborder;

	Kernel<double> *kern = new Kernel<double>(&border_tmp, kernel.x_radius,
					kernel.y_radius, kernel.values, kernel.divider);

	dst = kern->Convolve();

	FreeImage_Unload(border_tmp.fib);

	if(NULL == dst) {
		FreeImage_OutputMessageProc(
            FIF_UNKNOWN,
            "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
            src_type, FIT_BITMAP);
	}

	delete kern;

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_SeparableConvolve(FIABITMAP *src, FilterKernel kernel1, FilterKernel kernel2)
{
	FIBITMAP *tmp_dst = NULL, *dst = NULL;
	FIABITMAP *tmp = NULL, *tmp_border = NULL;
	FIABITMAP border_tmp;

	if(!src)
		return NULL;

	tmp = src;

	// convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

	switch(src_type) {
		
		case FIT_FLOAT:		// array of float: 32-bit
		case FIT_DOUBLE:	// array of double: 64-bit
			break;

		case FIT_INT16:		// array of short: signed 16-bit
		case FIT_UINT16:    // array of unsigned short: unsigned 16-bit
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
		case FIT_INT32:		// array of long: signed 32-bit
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit	
		{
			// Converting to float. Seems to be much faster.		
			border_tmp.fib = FreeImageAlgorithms_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
			border_tmp.xborder = kernel1.x_radius;
			border_tmp.yborder = kernel1.y_radius;
			
			tmp = &border_tmp;

			break;
		}

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	Kernel<double> *kern1 = new Kernel<double>(tmp, kernel1.x_radius,
								kernel1.y_radius, kernel1.values, kernel1.divider);

	tmp_dst = kern1->Convolve();
		
	tmp_border = FreeImageAlgorithms_SetBorder(tmp_dst, kernel2.x_radius, kernel2.y_radius);

	Kernel<double> *kern2 = new Kernel<double>(tmp_border, kernel2.x_radius,
				kernel2.y_radius, kernel2.values, kernel2.divider);

	dst = kern2->Convolve();

	FreeImageAlgorithms_Unload(tmp_border);
	FreeImage_Unload(tmp_dst);

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", src_type, FIT_BITMAP);
	}

	return dst;
}
