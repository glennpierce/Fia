#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#define BLOCKSIZE 8

struct ImageData
{
	int kernel_width;
	int kernel_height;
	int x_reminder;
	int y_reminder;
	int x_max_block_size;
	int src_pitch_in_pixels;
};

static inline double SumRow(double *ptr, double *kernel, int kernel_index, ImageData *data)
{
	register double sum = 0.0;

	register int x_max_block_size = data->x_max_block_size;

	for(register int col=0; col < x_max_block_size; col+=BLOCKSIZE)
		sum += *(ptr + col) * kernel[kernel_index + col] + *(ptr + col + 1) * kernel[kernel_index + col + 1] + 
		*(ptr + col + 2) * kernel[kernel_index + col + 2] + *(ptr + col + 3) * kernel[kernel_index + col + 3] + 
		*(ptr + col + 4) * kernel[kernel_index + col + 4] + *(ptr + col + 5) * kernel[kernel_index + col + 5] + 
		*(ptr + col + 6) * kernel[kernel_index + col + 6] + *(ptr + col + 7) * kernel[kernel_index + col + 7];

	switch(data->x_reminder) {
		case 7: 
			sum += *(ptr + x_max_block_size + 6) * kernel[kernel_index + x_max_block_size + 6] + 
				*(ptr + x_max_block_size + 5) * kernel[kernel_index + x_max_block_size + 5] + 
				*(ptr + x_max_block_size + 4) * kernel[kernel_index + x_max_block_size + 4] +
				*(ptr + x_max_block_size + 3) * kernel[kernel_index + x_max_block_size + 3] +
				*(ptr + x_max_block_size + 2) * kernel[kernel_index + x_max_block_size + 2] +
				*(ptr + x_max_block_size + 1) * kernel[kernel_index + x_max_block_size + 1] +
				*(ptr + x_max_block_size) * kernel[kernel_index + x_max_block_size]; 
			break;
		
		case 6:
			sum += *(ptr + x_max_block_size + 5) * kernel[kernel_index + x_max_block_size + 5] + 
				*(ptr + x_max_block_size + 4) * kernel[kernel_index + x_max_block_size + 4] + 
				*(ptr + x_max_block_size + 3) * kernel[kernel_index + x_max_block_size + 3] +
				*(ptr + x_max_block_size + 2) * kernel[kernel_index + x_max_block_size + 2] +
				*(ptr + x_max_block_size + 1) * kernel[kernel_index + x_max_block_size + 1] + 
				*(ptr + x_max_block_size) * kernel[kernel_index + x_max_block_size];
			break;
		
		case 5:
			sum += *(ptr + x_max_block_size + 4) * kernel[kernel_index + x_max_block_size + 4] + 
				*(ptr + x_max_block_size + 3) * kernel[kernel_index + x_max_block_size + 3] + 
				*(ptr + x_max_block_size + 2) * kernel[kernel_index + x_max_block_size + 2] + 
				*(ptr + x_max_block_size + 1) * kernel[kernel_index + x_max_block_size + 1] + 
				*(ptr + x_max_block_size) * kernel[kernel_index + x_max_block_size];
			break;

		case 4:
			sum += *(ptr + x_max_block_size + 3) * kernel[kernel_index + x_max_block_size + 3] + 
				*(ptr + x_max_block_size + 2) * kernel[kernel_index + x_max_block_size + 2] + 
				*(ptr + x_max_block_size + 1) * kernel[kernel_index + x_max_block_size + 1] + 
				*(ptr + x_max_block_size) * kernel[kernel_index + x_max_block_size];
			break;
		
		case 3:
			sum += *(ptr + x_max_block_size + 2) * kernel[kernel_index + x_max_block_size + 2] +
				*(ptr + x_max_block_size + 1) * kernel[kernel_index + x_max_block_size + 1] + 
				*(ptr + x_max_block_size) * kernel[kernel_index + x_max_block_size];
			break;

		case 2:
			sum += *(ptr + x_max_block_size + 1) * kernel[kernel_index + x_max_block_size + 1] +
				*(ptr + x_max_block_size) * kernel[kernel_index + x_max_block_size];
			break; 

		case 1:
			sum += *(ptr + x_max_block_size) * kernel[kernel_index + x_max_block_size]; 
	}

	return sum;
}

static inline double SumKernel(double *src_row_ptr, double* kernel, ImageData *data)
{
	register int kernel_index = 0;
	register double sum = 0.0;
	register double *tmp_ptr = src_row_ptr;
		
	for(register int row=0; row < data->x_max_block_size; row+=BLOCKSIZE)
	{  
		sum += SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 

		sum += SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index, data);
		tmp_ptr += data->src_pitch_in_pixels;
		kernel_index += data->kernel_width; 
	} 
		
	switch(data->y_reminder)
	{
		case 7:
			sum += SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 6:
			sum += SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 5:
			sum += SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 4:
			sum += SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 3:
			sum += SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 2:
			sum += SumRow(tmp_ptr, kernel, kernel_index, data);
			tmp_ptr += data->src_pitch_in_pixels;
			kernel_index += data->kernel_width; 
		case 1:
			sum += SumRow(tmp_ptr, kernel, kernel_index, data);
	}

	return sum;
}

FIBITMAP* Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, double *kernel, double divider)
{
	// Border must be large enough to account for kernel radius
	if(src.border < MAX(kernel_x_radius, kernel_y_radius))
		return NULL;

	const int src_image_width = FreeImage_GetWidth(src.fib);
	const int src_image_height = FreeImage_GetHeight(src.fib);
 
	const int dst_width = src_image_width - (2 * src.border);
	const int dst_height = src_image_height - (2 * src.border);

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src.fib, dst_width, dst_height);
	
	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(double);

	ImageData data;

	data.kernel_width = (kernel_x_radius * 2) + 1;
	data.kernel_height = (kernel_y_radius * 2) + 1;
	data.x_reminder = data.kernel_width % BLOCKSIZE;
	data.y_reminder = data.kernel_height % BLOCKSIZE;
	data.x_max_block_size = (data.kernel_width / BLOCKSIZE) * BLOCKSIZE;
	data.src_pitch_in_pixels = FreeImage_GetPitch(src.fib) / sizeof(double);

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
			*dst_ptr++ = SumKernel(src_row_ptr, kernel, &data) / divider;
			src_row_ptr++;
		}
	}

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, double *kernel, double divider)
{
	FIBITMAP *dst = NULL;

	if(!src.fib)
		return NULL;

	dst = Convolve(src, kernel_x_radius, kernel_y_radius, kernel, divider);

	if(dst == NULL)
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convolve with type.");

	return dst;
}