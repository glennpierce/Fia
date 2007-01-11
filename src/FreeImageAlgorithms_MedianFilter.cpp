#include "FreeImageAlgorithms_MedianFilter.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>

#define BLOCKSIZE 8

struct ImageData
{
	int kernel_width;
	int kernel_height;
	int x_reminder;
	int y_reminder;
	int x_max_block_size;
	int y_max_block_size;
	int src_pitch_in_pixels;
	double *kernel_tmp_array;
};

/*
* This Quickselect routine is based on the algorithm described in
* "Numerical recipes in C", Second Edition,
* Cambridge University Press, 1992, Section 8.5, ISBN 0-521-43108-5
* This code by Nicolas Devillard - 1998. Public domain.
*/
//#define ELEM_SWAP(a,b)
//{
//	register float t=(a);(a)=(b);(b)=t;
//}

//typedef float float;

static inline float quick_select(double arr[], int n)
{
	int low, high;
	int median;
	int middle, ll, hh;

	low = 0 ; high = n-1 ; median = (low + high) / 2;

	for (;;) {

		if (high <= low) /* One element only */
			return arr[median] ;

		if (high == low + 1) { /* Two elements only */
		
			if (arr[low] > arr[high])
				SWAP(arr[low], arr[high]) ;
	

			return arr[median] ;
		}

		/* Find median of low, middle and high items; swap into position low */
		middle = (low + high) / 2;
		
		if (arr[middle] > arr[high])
			SWAP(arr[middle], arr[high]) ;

		if (arr[low] > arr[high])
			SWAP(arr[low], arr[high]) ;

		if (arr[middle] > arr[low])
			SWAP(arr[middle], arr[low]) ;

		/* Swap low item (now in position middle) into position (low+1) */
		SWAP(arr[middle], arr[low+1]) ;

		/* Nibble from each end towards middle, swapping items when stuck */
		ll = low + 1;
		hh = high;

		for (;;) {

			do ll++; while (arr[low] > arr[ll]) ;

			do hh--; while (arr[hh] > arr[low]) ;

			if (hh < ll)
				break;

			SWAP(arr[ll], arr[hh]) ;
		}

		/* Swap middle item (in position low) back into correct position */
		SWAP(arr[low], arr[hh]) ;

		/* Re-set active partition */
		if (hh <= median)
			low = ll;

		if (hh >= median)
			high = hh - 1;
	}
}
//#undef ELEM_SWAP




static inline void GetRow(double *ptr, double* kernel_data, ImageData *data)
{
	int x_max_block_size = data->x_max_block_size;
	register double *tmp; 
	register double *result = kernel_data;

	size_t double_size = sizeof(double);
	size_t blocksize_in_bytes = double_size * BLOCKSIZE;

	for(register int col=0; col < x_max_block_size; col+=BLOCKSIZE){
		 
		tmp = ptr + col;

		memcpy(result, tmp, blocksize_in_bytes);
	}

	tmp = ptr + x_max_block_size;
	result += x_max_block_size;

	switch(data->x_reminder) {
		case 7: 
			memcpy(result, tmp, double_size * 7);
			break;
		
		case 6:
			memcpy(result, tmp, double_size * 6);
			break;
		
		case 5:
			memcpy(result, tmp, double_size * 5);
			break;

		case 4:
			memcpy(result, tmp, double_size * 4);
			break;
		
		case 3:
			memcpy(result, tmp, double_size * 3);
			break;

		case 2:
			memcpy(result, tmp, double_size * 2);
			break; 

		case 1:
			memcpy(result, tmp, double_size * 1);
	}
}

//static inline void GetRowDataAndIncrement(double **tmp_ptr, double **array_ptr, ImageData *data)
//{
//	GetRow(tmp_ptr, array_ptr, data);
//	tmp_ptr += data->src_pitch_in_pixels;
//	array_ptr += data->kernel_width;
//}

static inline double KernelMedian(double *src_row_ptr, ImageData *data)
{
	register int kernel_index = 0;
	register double *array_ptr = data->kernel_tmp_array;
	register double *tmp_ptr = src_row_ptr;
	
	// Helps to Debug
	memset(array_ptr, 0, sizeof(double) * data->kernel_width * data->kernel_height);


	for(register int row=0; row < data->y_max_block_size; row+=BLOCKSIZE)
	{  
		GetRow(tmp_ptr, array_ptr, data);
		tmp_ptr += data->src_pitch_in_pixels;
		array_ptr += data->kernel_width;

		GetRow(tmp_ptr, array_ptr, data);
		tmp_ptr += data->src_pitch_in_pixels;
		array_ptr += data->kernel_width;

		GetRow(tmp_ptr, array_ptr, data);
		tmp_ptr += data->src_pitch_in_pixels;
		array_ptr += data->kernel_width;

		GetRow(tmp_ptr, array_ptr, data);
		tmp_ptr += data->src_pitch_in_pixels;
		array_ptr += data->kernel_width;

		GetRow(tmp_ptr, array_ptr, data);
		tmp_ptr += data->src_pitch_in_pixels;
		array_ptr += data->kernel_width;

		GetRow(tmp_ptr, array_ptr, data);
		tmp_ptr += data->src_pitch_in_pixels;
		array_ptr += data->kernel_width;

		GetRow(tmp_ptr, array_ptr, data);
		tmp_ptr += data->src_pitch_in_pixels;
		array_ptr += data->kernel_width;

		GetRow(tmp_ptr, array_ptr, data);
		tmp_ptr += data->src_pitch_in_pixels;
		array_ptr += data->kernel_width;
	} 
		
	switch(data->y_reminder)
	{
		case 7:
			GetRow(tmp_ptr, array_ptr, data);
			tmp_ptr += data->src_pitch_in_pixels;
			array_ptr += data->kernel_width;
		case 6:
			GetRow(tmp_ptr, array_ptr, data);
			tmp_ptr += data->src_pitch_in_pixels;
			array_ptr += data->kernel_width;  
		case 5:
			GetRow(tmp_ptr, array_ptr, data);
			tmp_ptr += data->src_pitch_in_pixels;
			array_ptr += data->kernel_width;
		case 4:
			GetRow(tmp_ptr, array_ptr, data);
			tmp_ptr += data->src_pitch_in_pixels;
			array_ptr += data->kernel_width; 
		case 3:
			GetRow(tmp_ptr, array_ptr, data);
			tmp_ptr += data->src_pitch_in_pixels;
			array_ptr += data->kernel_width;
		case 2:
			GetRow(tmp_ptr, array_ptr, data);
			tmp_ptr += data->src_pitch_in_pixels;
			array_ptr += data->kernel_width;
		case 1:
			GetRow(tmp_ptr, array_ptr, data); 
	}


	int kernel_length = data->kernel_width * data->kernel_height;

	// We must now find and return the median value.
	return quick_select(data->kernel_tmp_array, kernel_length);
}

FIBITMAP* FreeImageAlgorithms_MedianFilter(FIABITMAP src, int kernel_x_radius, int kernel_y_radius)
{
	if(!src.fib)
		return NULL;

	// Border must be large enough to account for kernel radius
	if(src.border < MAX(kernel_x_radius, kernel_y_radius))
		return NULL;

	const int src_image_width = FreeImage_GetWidth(src.fib);
	const int src_image_height = FreeImage_GetHeight(src.fib);
 
	const int dst_width = src_image_width - (2 * src.border);
	const int dst_height = src_image_height - (2 * src.border);

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src.fib, dst_width, dst_height);
	
	if(dst == NULL)
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convolve with type.");

	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(double);

	ImageData data;

	data.kernel_width = (kernel_x_radius * 2) + 1;
	data.kernel_height = (kernel_y_radius * 2) + 1;
	data.x_reminder = data.kernel_width % BLOCKSIZE;
	data.y_reminder = data.kernel_height % BLOCKSIZE;
	data.x_max_block_size = (data.kernel_width / BLOCKSIZE) * BLOCKSIZE;
	data.y_max_block_size = (data.kernel_height / BLOCKSIZE) * BLOCKSIZE;
	data.src_pitch_in_pixels = FreeImage_GetPitch(src.fib) / sizeof(double);
	data.kernel_tmp_array = (double*) malloc(sizeof(double) * data.kernel_width * data.kernel_height);
	
	double *src_first_pixel_address_ptr = (double*) FreeImage_GetBits(src.fib);
	double *dst_first_pixel_address_ptr = (double*) FreeImage_GetBits(dst);
	
	register double *dst_ptr;
	register double *src_row_ptr;

	for (register int y=0; y < dst_height; y++)
	{		
		src_row_ptr = (src_first_pixel_address_ptr + y * data.src_pitch_in_pixels);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr++ = KernelMedian(src_row_ptr, &data);
			src_row_ptr++;
		}
	}

	free(data.kernel_tmp_array);

	return dst;
}