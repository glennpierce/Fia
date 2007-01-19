#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#define BLOCKSIZE 3

template<class Tsrc>
class BINARY_MORPHOLOGY
{
public:
	FIBITMAP* BinaryDilation(FIABITMAP* src);
	FIBITMAP* BinaryErosion(FIABITMAP* src);

private:

	inline bool AreAnyNearestNeighboursZero(Tsrc *ptr, bool include_diagonals);
	inline bool AreAnyNearestNeighboursNonZero(Tsrc *ptr, bool include_diagonals);

	unsigned char *structuring_element;
	int src_pitch_in_pixels;
};




/*

*dst_ptr = *src_row_ptr;

			// If Black pixel check neigbours
			if(*src_row_ptr == 0) {

				if(AreAnyNearestNeighboursNonZero(src_row_ptr, true))
					*dst_ptr = 255;

			}
	
			dst_ptr++;
			src_row_ptr++;
*/

inline void DilateKernelRow(KernelIterator<unsigned char> &iterator)
{
	register unsigned char *row_ptr;
	register unsigned char *kernel_ptr;

	Kernel<unsigned char> *kernel = iterator.GetKernel();

	int x_max_block_size = kernel->GetNumberOfBlocksOfEightInKernelRows();
	int x_reminder = kernel->GetRemainderAfterBlocksInRows();

	unsigned char *center_value = kernel->KernelCenterValuePtr();

	for(register int col=0; col < x_max_block_size; col+=BLOCKSIZE){
		 
		row_ptr = iterator.GetImagePtrValue() + col;
		kernel_ptr = iterator.GetKernelPtrValue() + col;

		if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
			*center_value = 1; 	
			return;
		}

		if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
			*center_value = 1; 	
			return;
		}

		if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
			*center_value = 1; 	
			return;
		}

		if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
			*center_value = 1; 	
			return;
		}

		if(kernel_ptr[4] >  0 && row_ptr[4] > 0) {
			*center_value = 1; 	
			return;
		}

		if(kernel_ptr[5] >  0 && row_ptr[5] > 0) {
			*center_value = 1; 	
			return;
		}

		if(kernel_ptr[6] >  0 && row_ptr[6] > 0) {
			*center_value = 1; 	
			return;
		}

		if(kernel_ptr[7] >  0 && row_ptr[7] > 0) {
			*center_value = 1; 	
			return;
		}
	}

	if(x_reminder) {
		row_ptr = iterator.GetImagePtrValue() + x_max_block_size;
		kernel_ptr = iterator.GetKernelPtrValue() + x_max_block_size;
	}

	switch(x_reminder) {

		case 7: 
			
			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[5] >  0 && row_ptr[5] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[6] >  0 && row_ptr[6] > 0) {
				*center_value = 1; 	
				return;
			}

			break;
		
		case 6:

			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[5] >  0 && row_ptr[5] > 0) {
				*center_value = 1; 	
				return;
			}

			break;
		
		case 5:

			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] > 0) {
				*center_value = 1; 	
				return;
			}

			break;

		case 4:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
				*center_value = 1; 	
				return;
			}

			break;
		
		case 3:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 1; 	
				return;
			}

			break;

		case 2:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 1; 	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 1; 	
				return;
			}

			break; 

		case 1:

			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 1; 	
				return;
			}
	}
}

inline void BinaryDilateKernel(Kernel<unsigned char> *kernel)
{
	KernelIterator<unsigned char> iterator = kernel->Begin();

	for(register int row=0; row < kernel->GetNumberOfBlocksOfEightInKernelColoumns();
		row+=BLOCKSIZE)
	{  
		DilateKernelRow(iterator);
		iterator.IncrementByRow();
		
		DilateKernelRow(iterator);
		iterator.IncrementByRow();

		DilateKernelRow(iterator);
		iterator.IncrementByRow();

		DilateKernelRow(iterator);
		iterator.IncrementByRow();
			
		DilateKernelRow(iterator);
		iterator.IncrementByRow();
		
		DilateKernelRow(iterator);
		iterator.IncrementByRow();
		
		DilateKernelRow(iterator);
		iterator.IncrementByRow();
		
		DilateKernelRow(iterator);
		iterator.IncrementByRow(); 
	} 
		
	switch(kernel->GetRemainderAfterBlocksInColoumns())
	{
		case 7:
			DilateKernelRow(iterator);
			iterator.IncrementByRow();

		case 6:
			DilateKernelRow(iterator);
			iterator.IncrementByRow();
		case 5:
			DilateKernelRow(iterator);
			iterator.IncrementByRow();
		case 4:
			DilateKernelRow(iterator);
			iterator.IncrementByRow();
		case 3:
			DilateKernelRow(iterator);
			iterator.IncrementByRow();
		case 2:
			DilateKernelRow(iterator);
			iterator.IncrementByRow();
		case 1:
			DilateKernelRow(iterator);
	}
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_BinaryDilation(FIABITMAP* src, FilterKernel kernel)
{
	const int dst_width = FreeImage_GetWidth(src->fib) - (2 * kernel.x_radius);
	const int dst_height = FreeImage_GetHeight(src->fib) - (2 * kernel.y_radius);

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src->fib, dst_width, dst_height);

	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(unsigned char);

	register unsigned char *dst_ptr;

	unsigned char *dst_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(dst);
	
	int kernel_size = kernel.x_radius * 2 + kernel.y_radius * 2;
	unsigned char *vals = new unsigned char[kernel_size];

	for(int i=0; i < kernel_size; i++)
		vals[i] = kernel.values[i];

	Kernel<unsigned char> *kern = new Kernel<unsigned char>(src, kernel.x_radius,
										kernel.y_radius, vals);
 
	/*
	for (register int y=0; y < dst_height; y++)
	{		
		kern->Move(0,y);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr = kern->KernelCenterValue();

			// If Black pixel check neigbours
			if(kern->KernelCenterValue() == 0)
				BinaryDilateKernel(kern);

			kern->Increment();
		}
	}

	delete kern;
*/
	return dst;
};
















/*


template<typename Tsrc>
inline bool MORPHOLOGY<Tsrc>::AreAnyNearestNeighboursNonZero(Tsrc *ptr, bool include_diagonals)
{
	const int points_to_top_left = this->src_pitch_in_pixels + this->src_pitch_in_pixels;

	if(*(ptr + points_to_top_left - 1))  // top
		return true; 

	if(*(ptr + this->src_pitch_in_pixels))  // centre left
		return true; 

	if(*(ptr + this->src_pitch_in_pixels - 2))  // centre right
		return true; 

	if(*(ptr + 1))  // bottom
		return true; 

	if(include_diagonals)
	{	
		if(*(ptr + points_to_top_left))  // top left
			return true; 

		if(*(ptr + points_to_top_left - 2))  // top right
			return true; 

		if(*ptr)  // bottom left
			return true; 
			
		if(*(ptr + 2))  // bottom right
			return true; 
	}

	return false;
}

template<typename Tsrc>
inline bool MORPHOLOGY<Tsrc>::AreAnyNearestNeighboursZero(Tsrc *ptr, bool include_diagonals)
{
	const int points_to_top_left = this->src_pitch_in_pixels + this->src_pitch_in_pixels;

	if(*(ptr + points_to_top_left - 1) == 0)  // top
		return true; 

	if(*(ptr + this->src_pitch_in_pixels) == 0)  // centre left
		return true; 

	if(*(ptr + this->src_pitch_in_pixels - 2) == 0)  // centre right
		return true; 

	if(*(ptr + 1) == 0)  // bottom
		return true; 

	if(include_diagonals)
	{	
		if(*(ptr + points_to_top_left) == 0)  // top left
			return true; 

		if(*(ptr + points_to_top_left - 2) == 0)  // top right
			return true; 

		if(*ptr == 0)  // bottom left
			return true; 
			
		if(*(ptr + 2) == 0)  // bottom right
			return true; 
	}

	return false;
}


template<typename Tsrc>
FIBITMAP* MORPHOLOGY<Tsrc>::BinaryDilation(FIABITMAP* src)
{
	// Border must be large enough to account for kernel radius 1
	if(src->xborder < 1 || src->yborder < 1)
		return NULL;

	const int src_image_width = FreeImage_GetWidth(src->fib);
	const int src_image_height = FreeImage_GetHeight(src->fib);

	const int dst_width = src_image_width - (2 * src->xborder);
	const int dst_height = src_image_height - (2 * src->yborder);

	// Clone and add border to image
	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src->fib, dst_width, dst_height);

	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(Tsrc);
	this->src_pitch_in_pixels = FreeImage_GetPitch(src->fib) / sizeof(Tsrc);

	Tsrc *src_first_pixel_address_ptr = (Tsrc*) FreeImage_GetBits(src->fib);
	unsigned char *dst_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(dst);
	
	register unsigned char *dst_ptr;
	register Tsrc *src_row_ptr;

	for (register int y=0; y < dst_height; y++)
	{		
		src_row_ptr = (src_first_pixel_address_ptr + y * this->src_pitch_in_pixels);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr = *src_row_ptr;

			// If Black pixel check neigbours
			if(*src_row_ptr == 0) {

				if(AreAnyNearestNeighboursNonZero(src_row_ptr, true))
					*dst_ptr = 255;

			}
	
			dst_ptr++;
			src_row_ptr++;
		}
	}

	return dst;
}

template<typename Tsrc>
FIBITMAP* MORPHOLOGY<Tsrc>::BinaryErosion(FIABITMAP* src)
{
	// Border must be large enough to account for kernel radius 1
	if(src->xborder < 1 || src->yborder < 1)
		return NULL;

	const int src_image_width = FreeImage_GetWidth(src->fib);
	const int src_image_height = FreeImage_GetHeight(src->fib);

	const int dst_width = src_image_width - (2 * src->xborder);
	const int dst_height = src_image_height - (2 * src->yborder);

	// Clone and add border to image
	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src->fib, dst_width, dst_height);

	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(Tsrc);
	this->src_pitch_in_pixels = FreeImage_GetPitch(src->fib) / sizeof(Tsrc);

	Tsrc *src_first_pixel_address_ptr = (Tsrc*) FreeImage_GetBits(src->fib);
	unsigned char *dst_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(dst);
	
	register unsigned char *dst_ptr;
	register Tsrc *src_row_ptr;

	for (register int y=0; y < dst_height; y++)
	{		
		src_row_ptr = (src_first_pixel_address_ptr + y * this->src_pitch_in_pixels);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr = *src_row_ptr;

			// If White pixel check neigbours
			if(*src_row_ptr > 0) {

				if(AreAnyNearestNeighboursZero(src_row_ptr, true))
					*dst_ptr = 0;

			}
	
			dst_ptr++;
			src_row_ptr++;
		}
	}

	return dst;
}

MORPHOLOGY<unsigned char>		filterUCharImage;
MORPHOLOGY<unsigned short>		filterUShortImage;
MORPHOLOGY<short>				filterShortImage;
MORPHOLOGY<unsigned long>		filterULongImage;
MORPHOLOGY<long>				filterLongImage;
MORPHOLOGY<float>				filterFloatImage;
MORPHOLOGY<double>				filterDoubleImage;

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Dilation(FIABITMAP* src)
{
	FIBITMAP *dst = NULL;

	if(!src->fib)
		return NULL;

	// convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

	switch(src_type) {
		
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src->fib) == 8)
				dst = filterUCharImage.BinaryDilation(src);
			break;
	}

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to perform filter on type.", src_type);
	}

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Erosion(FIABITMAP* src)
{
	FIBITMAP *dst = NULL;

	if(!src->fib)
		return NULL;

	// convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

	switch(src_type) {
		
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src->fib) == 8)
				dst = filterUCharImage.BinaryErosion(src);
			break;
	}

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to perform filter on type.", src_type);
	}

	return dst;
}

*/