#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <math.h>

FilterKernel DLL_CALLCONV
FreeImageAlgorithms_NewKernel(int x_radius, int y_radius,
							  double *values, double divider)
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
			border_tmp.xborder = src->xborder;
			border_tmp.yborder = src->yborder;
			
			tmp = &border_tmp;

			break;
		}

		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	Kernel<double> *kern = new Kernel<double>(tmp, kernel.x_radius,
					kernel.y_radius, kernel.values, kernel.divider);

	dst = kern->Convolve();

	FreeImage_Unload(border_tmp.fib);

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", src_type, FIT_BITMAP);
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


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Sobel(FIBITMAP *src)
{
	double sobel_left_kernel[] = {-1.0, 0.0, 1.0,
								  -2.0, 0.0, 2.0,
								  -1.0, 0.0, 1.0};

	double sobel_top_kernel[] = { 1.0, 2.0, 1.0,
								  0.0, 0.0, 0.0,
								 -1.0, -2.0, -1.0};

	PROFILE_START("Adding Border"); 

	FIABITMAP *src_bordered = FreeImageAlgorithms_SetBorder(src, 10, 10);

	PROFILE_STOP("Adding Border");

	PROFILE_START("Making Kernels"); 

	FilterKernel convolve_kernel_left = FreeImageAlgorithms_NewKernel(1, 1,
		sobel_left_kernel, 1.0);

	FilterKernel convolve_kernel_top = FreeImageAlgorithms_NewKernel(1, 1,
		sobel_top_kernel, 1.0);

	PROFILE_STOP("Making Kernels"); 


	PROFILE_START("Sobel Left Edge"); 

	FIBITMAP* dib1 = FreeImageAlgorithms_Convolve(src_bordered, convolve_kernel_left);

	PROFILE_STOP("Sobel Left Edge"); 

	PROFILE_START("Sobel Top Edge"); 

	FIBITMAP* dib2 = FreeImageAlgorithms_Convolve(src_bordered, convolve_kernel_top);

	PROFILE_STOP("Sobel Top Edge"); 

	int dst_width = FreeImage_GetWidth(dib1);
	int dst_height = FreeImage_GetHeight(dib1);

	FIBITMAP *dst = FreeImage_AllocateT(FIT_DOUBLE, dst_width, dst_height,
										8, 0, 0, 0);

	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(double);

	register double *dst_ptr, *dib1_ptr, *dib2_ptr;

	double *dst_first_pixel_address_ptr = (double*) FreeImage_GetBits(dst);
	double *dib1_first_pixel_address_ptr = (double*) FreeImage_GetBits(dib1);
	double *dib2_first_pixel_address_ptr = (double*) FreeImage_GetBits(dib2);

	for (register int y=0; y < dst_height; y++)
	{		
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);
		dib1_ptr = (dib1_first_pixel_address_ptr + y * dst_pitch_in_pixels);
		dib2_ptr = (dib2_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr++ = sqrt((*dib1_ptr * *dib1_ptr) + (*dib2_ptr * *dib2_ptr));
			++dib1_ptr;
			++dib2_ptr;
		}
	}

	FreeImageAlgorithms_Unload(src_bordered);
	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);

	return dst;
}

// Test
// Seems slow but maybe not for large kernels.
/*
FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_SeparableSobel(FIBITMAP *src)
{
	double sobel_horz_values[] = {-1.0, 0.0, 1.0};

	double sobel_vert_values[] = {1.0, 2.0, 1.0};

	FilterKernel sobel_horz_kernel = FreeImageAlgorithms_NewKernel(1, 0,
		sobel_horz_values, 1.0);

	FilterKernel sobel_vert_kernel = FreeImageAlgorithms_NewKernel(0, 1,
		sobel_vert_values, 1.0);

	FIBITMAP* dst = FreeImageAlgorithms_SeparableConvolve(src, sobel_horz_kernel, sobel_vert_kernel);

	return dst;
}
*/