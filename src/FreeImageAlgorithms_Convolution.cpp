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

	int x_max_block_size = data->x_max_block_size;
	register double *tmp;
	register int kernel_tmp;

	for(register int col=0; col < x_max_block_size; col+=BLOCKSIZE){
		 
		tmp = ptr + col;
		kernel_tmp = kernel_index + col;

		sum += *(tmp) * kernel[kernel_index + col] + *(tmp + 1) * kernel[kernel_tmp + 1] + 
			   *(tmp + 2) * kernel[kernel_tmp + 2] + *(tmp + 3) * kernel[kernel_tmp + 3] + 
			   *(tmp + 4) * kernel[kernel_tmp + 4] + *(tmp + 5) * kernel[kernel_tmp + 5] + 
			   *(tmp + 6) * kernel[kernel_tmp + 6] + *(tmp + 7) * kernel[kernel_tmp + 7];
	}

	tmp = ptr + x_max_block_size;
	kernel_tmp = kernel_index + x_max_block_size;

	switch(data->x_reminder) {
		case 7: 
			sum += *(tmp + 6) * kernel[kernel_tmp + 6] + 
				   *(tmp + 5) * kernel[kernel_tmp + 5] + 
				   *(tmp + 4) * kernel[kernel_tmp + 4] +
				   *(tmp + 3) * kernel[kernel_tmp + 3] +
				   *(tmp + 2) * kernel[kernel_tmp + 2] +
				   *(tmp + 1) * kernel[kernel_tmp + 1] +
				   *(tmp) * kernel[kernel_tmp]; 
			break;
		
		case 6:
			sum += *(tmp + 5) * kernel[kernel_tmp + 5] + 
				   *(tmp + 4) * kernel[kernel_tmp + 4] + 
				   *(tmp + 3) * kernel[kernel_tmp + 3] +
				   *(tmp + 2) * kernel[kernel_tmp + 2] +
				   *(tmp + 1) * kernel[kernel_tmp + 1] + 
				   *(tmp) * kernel[kernel_tmp];
			break;
		
		case 5:
			sum += *(tmp + 4) * kernel[kernel_tmp + 4] + 
				   *(tmp + 3) * kernel[kernel_tmp + 3] + 
				   *(tmp + 2) * kernel[kernel_tmp + 2] + 
				   *(tmp + 1) * kernel[kernel_tmp + 1] + 
				   *(tmp) * kernel[kernel_tmp];
			break;

		case 4:
			sum += *(tmp + 3) * kernel[kernel_tmp + 3] + 
				   *(tmp + 2) * kernel[kernel_tmp + 2] + 
				   *(tmp + 1) * kernel[kernel_tmp + 1] + 
				   *(tmp) * kernel[kernel_tmp];
			break;
		
		case 3:
			sum += *(tmp + 2) * kernel[kernel_tmp + 2] +
				   *(tmp + 1) * kernel[kernel_tmp + 1] + 
				   *(tmp) * kernel[kernel_tmp];
			break;

		case 2:
			sum += *(tmp + 1) * kernel[kernel_tmp + 1] +
				   *(tmp) * kernel[kernel_tmp];
			break; 

		case 1:
			sum += *(tmp) * kernel[kernel_tmp]; 
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