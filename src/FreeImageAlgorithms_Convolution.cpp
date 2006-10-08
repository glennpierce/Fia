#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

template<class Tsrc>
class CONVOLUTION
{
public:
	FIBITMAP* Convolve(FIABITMAP src, int kernel_x_radius, int kernel_y_radius, float *kernel);
private:
	inline void SumRow(int row, int size_of_maxblocks, int reminder);
	Tsrc *src_row_ptr;
	Tsrc *tmp_ptr;
	Tsrc *dst_ptr;
	float *kernel;

};

template<class Tsrc> 
inline void CONVOLUTION<Tsrc>::SumRow(int kernel_index, int size_of_maxblocks, int reminder)
{
	for(int col=0; col < size_of_maxblocks; col+=8)
		*dst_ptr += *(tmp_ptr + col) * this->kernel[kernel_index + col] + *(tmp_ptr + col + 1) * this->kernel[kernel_index + col + 1] + 
		*(tmp_ptr + col + 2) * this->kernel[kernel_index + col + 2] + *(tmp_ptr + col + 3) * this->kernel[kernel_index + col + 3] + 
		*(tmp_ptr + col + 4) * this->kernel[kernel_index + col + 4] + *(tmp_ptr + col + 5) * this->kernel[kernel_index + col + 5] + 
		*(tmp_ptr + col + 6) * this->kernel[kernel_index + col + 6] + *(tmp_ptr + col + 7) * this->kernel[kernel_index + col + 7];

	switch(reminder) {
		case 7: 
			*dst_ptr += *(tmp_ptr + size_of_maxblocks + 6) + *(tmp_ptr + size_of_maxblocks + 5) + *(tmp_ptr + size_of_maxblocks + 4) + \
				*(tmp_ptr + size_of_maxblocks + 3) + *(tmp_ptr + size_of_maxblocks + 2) + \
				*(tmp_ptr + size_of_maxblocks + 1) + *(tmp_ptr + size_of_maxblocks); 
			break;
		
		case 6:
			*dst_ptr += *(tmp_ptr + size_of_maxblocks + 5) + *(tmp_ptr + size_of_maxblocks + 4) + *(tmp_ptr + size_of_maxblocks + 3) + \
				*(tmp_ptr + size_of_maxblocks + 2) + *(tmp_ptr + size_of_maxblocks + 1) + *(tmp_ptr + size_of_maxblocks);
			break;
		
		case 5:
			*dst_ptr += *(tmp_ptr + size_of_maxblocks + 4) + *(tmp_ptr + size_of_maxblocks + 3) + *(tmp_ptr + size_of_maxblocks + 2) + \
				*(tmp_ptr + size_of_maxblocks + 1) + *(tmp_ptr + size_of_maxblocks);
			break;

		case 4:
			*dst_ptr += *(tmp_ptr + size_of_maxblocks + 3) + *(tmp_ptr + size_of_maxblocks + 2) + *(tmp_ptr + size_of_maxblocks + 1) + \
				*(tmp_ptr + size_of_maxblocks);
			break;
		
		case 3:
			*dst_ptr += *(tmp_ptr + size_of_maxblocks + 2) + *(tmp_ptr + size_of_maxblocks + 1) + *(tmp_ptr + size_of_maxblocks);
			break;

		case 2:
			*dst_ptr += *(tmp_ptr + size_of_maxblocks + 1) + *(tmp_ptr + size_of_maxblocks);
			break; 

		case 1:
			*dst_ptr += *(tmp_ptr + size_of_maxblocks); 
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
	const int pitch = FreeImage_GetPitch(src.fib);
	const int pitch_in_pixels = pitch / sizeof(Tsrc);
	const int blocksize = 8;
	const int kernel_width = (kernel_x_radius * 2) + 1;
	const int kernel_height = (kernel_y_radius * 2) + 1;
	const int x_blocks = kernel_width / blocksize;  
	const int x_reminder = kernel_width % blocksize;
	const int y_blocks = kernel_height / blocksize;  
	const int x_max_block_size = (kernel_width / blocksize) * blocksize; 
	const int y_max_block_size = (kernel_height / blocksize) * blocksize;  
	const int y_reminder = kernel_height - y_max_block_size; 	
	const int x_range = src_image_width - kernel_width;
	const int y_range = src_image_height - kernel_height;
	this->kernel = kernel;

	int dst_row = 0;
	int kernel_index;

	const int dst_width = src_image_width - (2 * src.border);
	const int dst_height = src_image_height - (2 * src.border);

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src.fib, dst_width, dst_height);

	for (register int y=kernel_height; y < src_image_height; y++)
	{		
		this->src_row_ptr = (Tsrc*) FreeImage_GetScanLine(src.fib, y);
		this->dst_ptr = (Tsrc*) FreeImage_GetScanLine(dst, dst_row++);
	
		for (register int x=0; x < x_range; x++) 
		{
			kernel_index = 0;
			this->tmp_ptr = this->src_row_ptr;
			*(this->dst_ptr) = 0.0; 
		
			for(register int row=0; row < x_max_block_size; row+=8)
			{  
				SumRow(kernel_index, x_max_block_size, x_reminder);
				this->tmp_ptr -= pitch_in_pixels;
				kernel_index += kernel_width; 
				
				SumRow(kernel_index, x_max_block_size, x_reminder);
				this->tmp_ptr -= pitch_in_pixels;
				kernel_index += kernel_width; 

				SumRow(kernel_index, x_max_block_size, x_reminder);
				this->tmp_ptr -= pitch_in_pixels;
				kernel_index += kernel_width; 
				
				SumRow(kernel_index, x_max_block_size, x_reminder);
				this->tmp_ptr += pitch_in_pixels;
				kernel_index += kernel_width; 
				
				SumRow(kernel_index, x_max_block_size, x_reminder);
				this->tmp_ptr -= pitch_in_pixels;
				kernel_index += kernel_width; 
				
				SumRow(kernel_index, x_max_block_size, x_reminder);
				this->tmp_ptr -= pitch_in_pixels;
				kernel_index += kernel_width; 
				
				SumRow(kernel_index, x_max_block_size, x_reminder);
				this->tmp_ptr -= pitch_in_pixels;
				kernel_index += kernel_width; 
				
				SumRow(kernel_index, x_max_block_size, x_reminder);
				this->tmp_ptr -= pitch_in_pixels;
				kernel_index += kernel_width; 
			} 
		
			switch(y_reminder) { \
				case 7:
					SumRow(kernel_index, x_max_block_size, x_reminder);
					this->tmp_ptr -= pitch_in_pixels;
					kernel_index += kernel_width; 
				case 6:
					SumRow(kernel_index, x_max_block_size, x_reminder);
					this->tmp_ptr -= pitch_in_pixels;
					kernel_index += kernel_width; 
				case 5:
					SumRow(kernel_index, x_max_block_size, x_reminder);
					this->tmp_ptr -= pitch_in_pixels;
					kernel_index += kernel_width; 
				case 4:
					SumRow(kernel_index, x_max_block_size, x_reminder);
					this->tmp_ptr -= pitch_in_pixels;
					kernel_index += kernel_width; 
				case 3:
					SumRow(kernel_index, x_max_block_size, x_reminder);
					this->tmp_ptr -= pitch_in_pixels;
					kernel_index += kernel_width; 
				case 2:
					SumRow(kernel_index, x_max_block_size, x_reminder);
					this->tmp_ptr -= pitch_in_pixels;
					kernel_index += kernel_width; 
				case 1:
					SumRow(kernel_index, x_max_block_size, x_reminder);
			}

			(*this->dst_ptr) /= 440.0;

			this->src_row_ptr++;
			this->dst_ptr++;
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