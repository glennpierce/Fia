#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include "profile.h"

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

template<class Tsrc>
class CONVOLVER
{
public:
	FIBITMAP* Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius,
				   Tsrc *kernel, Tsrc divider);

private:
	
	inline Tsrc SumKernel(Tsrc *src_row_ptr, Tsrc* kernel);
	inline Tsrc SumRow(Tsrc *ptr, Tsrc *kernel, int kernel_index);

	int kernel_width;
	int kernel_height;
	int x_reminder;
	int y_reminder;
	int x_max_block_size;
	int src_pitch_in_pixels;
};


template<typename Tsrc>
inline Tsrc CONVOLVER<Tsrc>::SumRow(Tsrc *ptr, Tsrc *kernel, int kernel_index)
{
	register Tsrc sum = 0;

	int x_max_block_size = this->x_max_block_size;
	register Tsrc *tmp;
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

	switch(this->x_reminder) {
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

template<typename Tsrc>
inline Tsrc CONVOLVER<Tsrc>::SumKernel(Tsrc *src_row_ptr, Tsrc* kernel)
{
	register int kernel_index = 0;
	register Tsrc sum = 0;
	register Tsrc *tmp_ptr = src_row_ptr;
		
	for(register int row=0; row < this->x_max_block_size; row+=BLOCKSIZE)
	{  
		sum += SumRow(tmp_ptr, kernel, kernel_index);
		tmp_ptr += this->src_pitch_in_pixels;
		kernel_index += this->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index);
		tmp_ptr += this->src_pitch_in_pixels;
		kernel_index += this->kernel_width; 

		sum += SumRow(tmp_ptr, kernel, kernel_index);
		tmp_ptr += this->src_pitch_in_pixels;
		kernel_index += this->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index);
		tmp_ptr += this->src_pitch_in_pixels;
		kernel_index += this->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index);
		tmp_ptr += this->src_pitch_in_pixels;
		kernel_index += this->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index);
		tmp_ptr += this->src_pitch_in_pixels;
		kernel_index += this->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index);
		tmp_ptr += this->src_pitch_in_pixels;
		kernel_index += this->kernel_width; 
				
		sum += SumRow(tmp_ptr, kernel, kernel_index);
		tmp_ptr += this->src_pitch_in_pixels;
		kernel_index += this->kernel_width; 
	} 
		
	switch(this->y_reminder)
	{
		case 7:
			sum += SumRow(tmp_ptr, kernel, kernel_index);
			tmp_ptr += this->src_pitch_in_pixels;
			kernel_index += this->kernel_width; 
		case 6:
			sum += SumRow(tmp_ptr, kernel, kernel_index);
			tmp_ptr += this->src_pitch_in_pixels;
			kernel_index += this->kernel_width; 
		case 5:
			sum += SumRow(tmp_ptr, kernel, kernel_index);
			tmp_ptr += this->src_pitch_in_pixels;
			kernel_index += this->kernel_width; 
		case 4:
			sum += SumRow(tmp_ptr, kernel, kernel_index);
			tmp_ptr += this->src_pitch_in_pixels;
			kernel_index += this->kernel_width; 
		case 3:
			sum += SumRow(tmp_ptr, kernel, kernel_index);
			tmp_ptr += this->src_pitch_in_pixels;
			kernel_index += this->kernel_width; 
		case 2:
			sum += SumRow(tmp_ptr, kernel, kernel_index);
			tmp_ptr += this->src_pitch_in_pixels;
			kernel_index += this->kernel_width; 
		case 1:
			sum += SumRow(tmp_ptr, kernel, kernel_index);
	}

	return sum;
}

template<typename Tsrc>
FIBITMAP* CONVOLVER<Tsrc>::Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius,
				   Tsrc *kernel, Tsrc divider)
{
	// Border must be large enough to account for kernel radius
	if(src.border < MAX(kernel_x_radius, kernel_y_radius))
		return NULL;

	const int bpp = FreeImage_GetBPP(src.fib);
	const FREE_IMAGE_TYPE type = FreeImage_GetImageType(src.fib);

	if(type == FIT_BITMAP && bpp >= 24)
		return NULL;

	if(type == FIT_COMPLEX)
		return NULL;

	const int src_image_width = FreeImage_GetWidth(src.fib);
	const int src_image_height = FreeImage_GetHeight(src.fib);
 
	const int dst_width = src_image_width - (2 * src.border);
	const int dst_height = src_image_height - (2 * src.border);

	FIBITMAP *dst;
	
	if(type == FIT_FLOAT || type == FIT_DOUBLE)
		dst = FreeImage_AllocateT(FIT_DOUBLE, dst_width, dst_height, 0, 0, 0, 0);
	else
		dst = FreeImage_AllocateT(FIT_INT32, dst_width, dst_height, 0, 0, 0, 0);
	
	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(Tsrc);

	this->kernel_width = (kernel_x_radius * 2) + 1;
	this->kernel_height = (kernel_y_radius * 2) + 1;
	this->x_reminder = this->kernel_width % BLOCKSIZE;
	this->y_reminder = this->kernel_height % BLOCKSIZE;
	this->x_max_block_size = (this->kernel_width / BLOCKSIZE) * BLOCKSIZE;
	this->src_pitch_in_pixels = FreeImage_GetPitch(src.fib) / sizeof(Tsrc);

	Tsrc *src_first_pixel_address_ptr = (Tsrc*) FreeImage_GetBits(src.fib);
	Tsrc *dst_first_pixel_address_ptr = (Tsrc*) FreeImage_GetBits(dst);
	
	register Tsrc *dst_ptr;
	register Tsrc *src_row_ptr;

	for (register int y=0; y < dst_height; y++)
	{		
		src_row_ptr = (src_first_pixel_address_ptr + y * this->src_pitch_in_pixels);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr++ = SumKernel(src_row_ptr, kernel) / divider;
			src_row_ptr++;
		}
	}

	return dst;
}

CONVOLVER<int>		fixedPointImage;
CONVOLVER<double>	realImage;

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, double *kernel, double divider)
{
	FIBITMAP *dst = NULL;

	if(!src.fib)
		return NULL;

	// convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src.fib);

	switch(src_type) {
		
		case FIT_INT16:		// array of short: signed 16-bit
		case FIT_UINT16:    // array of unsigned short: unsigned 16-bit
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
		case FIT_INT32:		// array of long: signed 32-bit
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit	
		{
			int no_kernel_elements = (kernel_x_radius * 2 + 1) * (kernel_y_radius * 2 + 1);
			int *fixed_kernel = new int[no_kernel_elements];
	
			// Convert real array of kernel values to a fixed int array.
			// We dont want to floating point conversions going on 
			// for performance reasons.
			for(int i=0; i < no_kernel_elements; i++)
				fixed_kernel[i] = (int) kernel[i];

			dst = fixedPointImage.Convolve(src, kernel_x_radius, kernel_y_radius,
				fixed_kernel, (int) divider);

			break;
		}

		case FIT_FLOAT:		// array of float: 32-bit
		case FIT_DOUBLE:	// array of double: 64-bit
			dst = realImage.Convolve(src, kernel_x_radius, kernel_y_radius, kernel, divider);
			break;
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", src_type, FIT_BITMAP);
	}

	return dst;
}