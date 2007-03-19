#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Filters.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"

#include <math.h>

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Sobel(FIBITMAP *src)
{
	double sobel_left_kernel[] = {-1.0, 0.0, 1.0,
								  -2.0, 0.0, 2.0,
								  -1.0, 0.0, 1.0};

	double sobel_top_kernel[] = { 1.0, 2.0, 1.0,
								  0.0, 0.0, 0.0,
								 -1.0, -2.0, -1.0};

	FIABITMAP *src_bordered = FreeImageAlgorithms_SetBorder(src, 1, 1);

	FilterKernel convolve_kernel_left = FreeImageAlgorithms_NewKernel(1, 1,
		sobel_left_kernel, 1.0);

	FilterKernel convolve_kernel_top = FreeImageAlgorithms_NewKernel(1, 1,
		sobel_top_kernel, 1.0);

	FIBITMAP* dib1 = FreeImageAlgorithms_Convolve(src_bordered, convolve_kernel_left);

	FIBITMAP* dib2 = FreeImageAlgorithms_Convolve(src_bordered, convolve_kernel_top);

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
