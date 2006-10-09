#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#define BLOCKSIZE 8

template<class Tsrc>
class CONVOLUTION
{
public:
	FIBITMAP* Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, float *kernel);
private:
	inline void SumRow(Tsrc *ptr, int row);
	inline void SumKernel(int src_pitch_in_pixels);
	Tsrc *src_row_ptr;
	Tsrc *dst_ptr;
	float *kernel;
	int x_max_block_size;
	int x_reminder;
	int y_reminder;
	int kernel_width;
	int kernel_height;
};



template<class Tsrc> 
inline void CONVOLUTION<Tsrc>::SumRow(Tsrc *ptr, int kernel_index)
{
	int max_blocksize = this->x_max_block_size;

	for(int col=0; col < max_blocksize; col+=BLOCKSIZE)
		*dst_ptr += *(ptr + col) * this->kernel[kernel_index + col] + *(ptr + col + 1) * this->kernel[kernel_index + col + 1] + 
		*(ptr + col + 2) * this->kernel[kernel_index + col + 2] + *(ptr + col + 3) * this->kernel[kernel_index + col + 3] + 
		*(ptr + col + 4) * this->kernel[kernel_index + col + 4] + *(ptr + col + 5) * this->kernel[kernel_index + col + 5] + 
		*(ptr + col + 6) * this->kernel[kernel_index + col + 6] + *(ptr + col + 7) * this->kernel[kernel_index + col + 7];

	switch(this->x_reminder) {
		case 7: 
			*dst_ptr += *(ptr + max_blocksize + 6) * this->kernel[kernel_index + max_blocksize + 6] + 
				*(ptr + max_blocksize + 5) * this->kernel[kernel_index + max_blocksize + 5] + 
				*(ptr + max_blocksize + 4) * this->kernel[kernel_index + max_blocksize + 4] +
				*(ptr + max_blocksize + 3) * this->kernel[kernel_index + max_blocksize + 3] +
				*(ptr + max_blocksize + 2) * this->kernel[kernel_index + max_blocksize + 2] +
				*(ptr + max_blocksize + 1) * this->kernel[kernel_index + max_blocksize + 1] +
				*(ptr + max_blocksize) * this->kernel[kernel_index + max_blocksize]; 
			break;
		
		case 6:
			*dst_ptr += *(ptr + max_blocksize + 5) * this->kernel[kernel_index + max_blocksize + 5] + 
				*(ptr + max_blocksize + 4) * this->kernel[kernel_index + max_blocksize + 4] + 
				*(ptr + max_blocksize + 3) * this->kernel[kernel_index + max_blocksize + 3] +
				*(ptr + max_blocksize + 2) * this->kernel[kernel_index + max_blocksize + 2] +
				*(ptr + max_blocksize + 1) * this->kernel[kernel_index + max_blocksize + 1] + 
				*(ptr + max_blocksize) * this->kernel[kernel_index + max_blocksize];
			break;
		
		case 5:
			*dst_ptr += *(ptr + max_blocksize + 4) * this->kernel[kernel_index + max_blocksize + 4] + 
				*(ptr + max_blocksize + 3) * this->kernel[kernel_index + max_blocksize + 3] + 
				*(ptr + max_blocksize + 2) * this->kernel[kernel_index + max_blocksize + 2] + 
				*(ptr + max_blocksize + 1) * this->kernel[kernel_index + max_blocksize + 1] + 
				*(ptr + max_blocksize) * this->kernel[kernel_index + max_blocksize];
			break;

		case 4:
			*dst_ptr += *(ptr + max_blocksize + 3) * this->kernel[kernel_index + max_blocksize + 3] + 
				*(ptr + max_blocksize + 2) * this->kernel[kernel_index + max_blocksize + 2] + 
				*(ptr + max_blocksize + 1) * this->kernel[kernel_index + max_blocksize + 1] + 
				*(ptr + max_blocksize) * this->kernel[kernel_index + max_blocksize];
			break;
		
		case 3:
			*dst_ptr += *(ptr + max_blocksize + 2) * this->kernel[kernel_index + max_blocksize + 2] +
				*(ptr + max_blocksize + 1) * this->kernel[kernel_index + max_blocksize + 1] + 
				*(ptr + max_blocksize) * this->kernel[kernel_index + max_blocksize];
			break;

		case 2:
			*dst_ptr += *(ptr + max_blocksize + 1) * this->kernel[kernel_index + max_blocksize + 1] +
				*(ptr + max_blocksize) * this->kernel[kernel_index + max_blocksize];
			break; 

		case 1:
			*dst_ptr += *(ptr + max_blocksize) * this->kernel[kernel_index + max_blocksize]; 
	}
}

template<class Tsrc>
inline void CONVOLUTION<Tsrc>::SumKernel(int src_pitch_in_pixels)
{
	int kernel_index = 0;
	Tsrc *tmp_ptr = this->src_row_ptr;
		
	for(register int row=0; row < x_max_block_size; row+=BLOCKSIZE)
	{  
		SumRow(tmp_ptr, kernel_index);
		tmp_ptr -= src_pitch_in_pixels;
		kernel_index += kernel_width; 
				
		SumRow(tmp_ptr, kernel_index);
		tmp_ptr -= src_pitch_in_pixels;
		kernel_index += kernel_width; 

		SumRow(tmp_ptr, kernel_index);
		tmp_ptr -= src_pitch_in_pixels;
		kernel_index += kernel_width; 
				
		SumRow(tmp_ptr, kernel_index);
		tmp_ptr -= src_pitch_in_pixels;
		kernel_index += kernel_width; 
				
		SumRow(tmp_ptr, kernel_index);
		tmp_ptr -= src_pitch_in_pixels;
		kernel_index += kernel_width; 
				
		SumRow(tmp_ptr, kernel_index);
		tmp_ptr -= src_pitch_in_pixels;
		kernel_index += kernel_width; 
				
		SumRow(tmp_ptr, kernel_index);
		tmp_ptr -= src_pitch_in_pixels;
		kernel_index += kernel_width; 
				
		SumRow(tmp_ptr, kernel_index);
		tmp_ptr -= src_pitch_in_pixels;
		kernel_index += kernel_width; 
	} 
		
	switch(y_reminder)
	{
		case 7:
			SumRow(tmp_ptr, kernel_index);
			tmp_ptr -= src_pitch_in_pixels;
			kernel_index += kernel_width; 
		case 6:
			SumRow(tmp_ptr, kernel_index);
			tmp_ptr -= src_pitch_in_pixels;
			kernel_index += kernel_width; 
		case 5:
			SumRow(tmp_ptr, kernel_index);
			tmp_ptr -= src_pitch_in_pixels;
			kernel_index += kernel_width; 
		case 4:
			SumRow(tmp_ptr, kernel_index);
			tmp_ptr -= src_pitch_in_pixels;
			kernel_index += kernel_width; 
		case 3:
			SumRow(tmp_ptr, kernel_index);
			tmp_ptr -= src_pitch_in_pixels;
			kernel_index += kernel_width; 
		case 2:
			SumRow(tmp_ptr, kernel_index);
			tmp_ptr -= src_pitch_in_pixels;
			kernel_index += kernel_width; 
		case 1:
			SumRow(tmp_ptr, kernel_index);
	}
}


template<class Tsrc> 
FIBITMAP* CONVOLUTION<Tsrc>::Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, float *kernel)
{
	// Border must be large enough to account for kernel radius
	if(src.border < MAX(kernel_x_radius, kernel_y_radius))
		return NULL;

	const int src_image_width = FreeImage_GetWidth(src.fib);
	const int src_image_height = FreeImage_GetHeight(src.fib);
	const int src_pitch_in_pixels = FreeImage_GetPitch(src.fib) / sizeof(Tsrc);
	const int dst_width = src_image_width - (2 * src.border);
	const int dst_height = src_image_height - (2 * src.border);

	this->kernel_width = (kernel_x_radius * 2) + 1;
	this->kernel_height = (kernel_y_radius * 2) + 1;
	this->x_reminder = kernel_width % BLOCKSIZE;
	this->y_reminder = kernel_height % BLOCKSIZE;
	this->x_max_block_size = (kernel_width / BLOCKSIZE) * BLOCKSIZE;  	
	this->kernel = kernel;

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src.fib, dst_width, dst_height);
	
	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(Tsrc);

	Tsrc *src_first_pixel_address_ptr = (Tsrc*) FreeImage_GetScanLine(src.fib, src_image_height);
	Tsrc *dst_first_pixel_address_ptr = (Tsrc*) FreeImage_GetScanLine(dst, dst_height);

	for (register int y=0; y < dst_height; y++)
	{		
		this->src_row_ptr = (src_first_pixel_address_ptr - y * src_pitch_in_pixels);
		this->dst_ptr = (dst_first_pixel_address_ptr - y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			SumKernel(src_pitch_in_pixels);

			*this->dst_ptr /= 440.0;

			this->src_row_ptr--;
			this->dst_ptr--;
		}
	}

	return dst;
}


CONVOLUTION<unsigned char>		convolveUCharImage;
CONVOLUTION<unsigned short>		convolveUShortImage;
CONVOLUTION<short>				convolveShortImage;
CONVOLUTION<unsigned long>		convolveULongImage;
CONVOLUTION<long>				convolveLongImage;
CONVOLUTION<float>				convolveFloatImage;
CONVOLUTION<double>				convolveDoubleImage;
		

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, float *kernel)
{
	FIBITMAP *dst = NULL;

	if(!src.fib) return NULL;

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src.fib);

	switch(type) {
	//	case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
	//		if(FreeImage_GetBPP(src.fib) == 8)
	//			dst = convolveUCharImage.Convolve(src, kernel_x_radius, kernel_y_radius, kernel);
	//		break;
	//	case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
	//		dst = convolveUShortImage.Convolve(src, kernel_x_radius, kernel_y_radius, kernel);
	//		break;
	//	case FIT_INT16:		// array of short: signed 16-bit
	//		dst = convolveShortImage.Convolve(src, kernel_x_radius, kernel_y_radius, kernel);
	//		break;
	//	case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
	//		dst = convolveULongImage.Convolve(src, kernel_x_radius, kernel_y_radius, kernel);
	//		break;
	//	case FIT_INT32:		// array of long: signed 32-bit
	//		dst = convolveLongImage.Convolve(src, kernel_x_radius, kernel_y_radius, kernel);
	//		break;
		case FIT_FLOAT:		// array of float: 32-bit
			dst = convolveFloatImage.Convolve(src, kernel_x_radius, kernel_y_radius, kernel);
			break;
	//	case FIT_DOUBLE:	// array of double: 64-bit
	//		dst = convolveDoubleImage.Convolve(src, kernel_x_radius, kernel_y_radius, kernel);
	//		break;
	//	case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
	//		break;
	}

	if(NULL == dst)
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convolve with type %d.", type);

	return dst;
}