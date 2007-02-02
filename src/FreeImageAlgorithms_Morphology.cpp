#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution_Private.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

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


inline void DilateKernelRow(KernelIterator<unsigned char> &iterator, unsigned char *dst_ptr)
{
	register unsigned char *row_ptr;
	register unsigned char *kernel_ptr;

	Kernel<unsigned char> *kernel = iterator.GetKernel();

	int x_max_block_size = kernel->GetNumberOfBlocksOfEightInKernelRows();
	int x_reminder = kernel->GetRemainderAfterBlocksInRows();

	unsigned char *center_value = dst_ptr;

	for(register int col=0; col < x_max_block_size; col+=BLOCKSIZE){
		 
		row_ptr = iterator.GetImagePtrValue() + col;
		kernel_ptr = iterator.GetKernelPtrValue() + col;

		if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
			*center_value = 255;	
			return;
		}

		if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
			*center_value = 255;	
			return;
		}

		if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
			*center_value = 255;	
			return;
		}

		if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
			*center_value = 255;	
			return;
		}

		if(kernel_ptr[4] >  0 && row_ptr[4] > 0) {
			*center_value = 255;	
			return;
		}

		if(kernel_ptr[5] >  0 && row_ptr[5] > 0) {
			*center_value = 255;	
			return;
		}

		if(kernel_ptr[6] >  0 && row_ptr[6] > 0) {
			*center_value = 255;	
			return;
		}

		if(kernel_ptr[7] >  0 && row_ptr[7] > 0) {
			*center_value = 255;	
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
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[5] >  0 && row_ptr[5] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[6] >  0 && row_ptr[6] > 0) {
				*center_value = 255;	
				return;
			}

			break;
		
		case 6:

			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[5] >  0 && row_ptr[5] > 0) {
				*center_value = 255;	
				return;
			}

			break;
		
		case 5:

			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] > 0) {
				*center_value = 255;	
				return;
			}

			break;

		case 4:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] > 0) {
				*center_value = 255;	
				return;
			}

			break;
		
		case 3:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] > 0) {
				*center_value = 255;	
				return;
			}

			break;

		case 2:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 255;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] > 0) {
				*center_value = 255;	
				return;
			}

			break; 

		case 1:

			if(kernel_ptr[0] >  0 && row_ptr[0] > 0) {
				*center_value = 255;	
				return;
			}
	}
}

inline void BinaryDilateKernel(Kernel<unsigned char> *kernel, unsigned char *dst_ptr)
{
	KernelIterator<unsigned char> iterator = kernel->Begin();

	for(register int row=0; row < kernel->GetNumberOfBlocksOfEightInKernelColoumns();
		row+=BLOCKSIZE)
	{  
		DilateKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
		
		DilateKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();

		DilateKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();

		DilateKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
			
		DilateKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
		
		DilateKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
		
		DilateKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
		
		DilateKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow(); 
	} 
		
	switch(kernel->GetRemainderAfterBlocksInColoumns())
	{
		case 7:
			DilateKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 6:
			DilateKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 5:
			DilateKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 4:
			DilateKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 3:
			DilateKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 2:
			DilateKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 1:
			DilateKernelRow(iterator, dst_ptr);
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
	
	int kernel_size = (kernel.x_radius * 2 + kernel.y_radius * 2) + 1;
	unsigned char *vals = new unsigned char[kernel_size];

	for(int i=0; i < kernel_size; i++)
		vals[i] = kernel.values[i];

	Kernel<unsigned char> *kern = new Kernel<unsigned char>(src, kernel.x_radius,
										kernel.y_radius, vals, 1.0);
 
	for (register int y=0; y < dst_height; y++)
	{		
		kern->Move(0,y);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr = kern->KernelCenterValue();

			// If Black pixel check neigbours
			if(*dst_ptr == 0)
				BinaryDilateKernel(kern, dst_ptr);

			dst_ptr++;
			kern->Increment();
		}
	}

	delete kern;

	return dst;
};


static inline void ErodeKernelRow(KernelIterator<unsigned char> &iterator, unsigned char *dst_ptr)
{
	register unsigned char *row_ptr;
	register unsigned char *kernel_ptr;

	Kernel<unsigned char> *kernel = iterator.GetKernel();

	int x_max_block_size = kernel->GetNumberOfBlocksOfEightInKernelRows();
	int x_reminder = kernel->GetRemainderAfterBlocksInRows();

	unsigned char *center_value = dst_ptr;

	for(register int col=0; col < x_max_block_size; col+=BLOCKSIZE){
		 
		row_ptr = iterator.GetImagePtrValue() + col;
		kernel_ptr = iterator.GetKernelPtrValue() + col;

		if(kernel_ptr[0] >  0 && row_ptr[0] == 0) {
			*center_value = 0;	
			return;
		}

		if(kernel_ptr[1] >  0 && row_ptr[1] == 0) {
			*center_value = 0;	
			return;
		}

		if(kernel_ptr[2] >  0 && row_ptr[2] == 0) {
			*center_value = 0;	
			return;
		}

		if(kernel_ptr[3] >  0 && row_ptr[3] == 0) {
			*center_value = 0;	
			return;
		}

		if(kernel_ptr[4] >  0 && row_ptr[4] == 0) {
			*center_value = 0;	
			return;
		}

		if(kernel_ptr[5] >  0 && row_ptr[5] == 0) {
			*center_value = 0;	
			return;
		}

		if(kernel_ptr[6] >  0 && row_ptr[6] > 0) {
			*center_value = 0;	
			return;
		}

		if(kernel_ptr[7] >  0 && row_ptr[7] > 0) {
			*center_value = 0;	
			return;
		}
	}

	if(x_reminder) {
		row_ptr = iterator.GetImagePtrValue() + x_max_block_size;
		kernel_ptr = iterator.GetKernelPtrValue() + x_max_block_size;
	}

	switch(x_reminder) {

		case 7: 
			
			if(kernel_ptr[0] >  0 && row_ptr[0] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[5] >  0 && row_ptr[5] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[6] >  0 && row_ptr[6] == 0) {
				*center_value = 0;	
				return;
			}

			break;
		
		case 6:

			if(kernel_ptr[0] >  0 && row_ptr[0] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[5] >  0 && row_ptr[5] == 0) {
				*center_value = 0;	
				return;
			}

			break;
		
		case 5:

			if(kernel_ptr[0] >  0 && row_ptr[0] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[4] >  0 && row_ptr[4] == 0) {
				*center_value = 0;	
				return;
			}

			break;

		case 4:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[3] >  0 && row_ptr[3] == 0) {
				*center_value = 0;	
				return;
			}

			break;
		
		case 3:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[2] >  0 && row_ptr[2] == 0) {
				*center_value = 0;	
				return;
			}

			break;

		case 2:
			
			if(kernel_ptr[0] >  0 && row_ptr[0] == 0) {
				*center_value = 0;	
				return;
			}

			if(kernel_ptr[1] >  0 && row_ptr[1] == 0) {
				*center_value = 0;	
				return;
			}

			break; 

		case 1:

			if(kernel_ptr[0] >  0 && row_ptr[0] == 0) {
				*center_value = 0;	
				return;
			}
	}
}

static inline void BinaryErodeKernel(Kernel<unsigned char> *kernel, unsigned char *dst_ptr)
{
	KernelIterator<unsigned char> iterator = kernel->Begin();

	for(register int row=0; row < kernel->GetNumberOfBlocksOfEightInKernelColoumns();
		row+=BLOCKSIZE)
	{  
		ErodeKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
		
		ErodeKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();

		ErodeKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();

		ErodeKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
			
		ErodeKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
		
		ErodeKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
		
		ErodeKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow();
		
		ErodeKernelRow(iterator, dst_ptr);
		iterator.IncrementByRow(); 
	} 
		
	switch(kernel->GetRemainderAfterBlocksInColoumns())
	{
		case 7:
			ErodeKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 6:
			ErodeKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 5:
			ErodeKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 4:
			ErodeKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 3:
			ErodeKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 2:
			ErodeKernelRow(iterator, dst_ptr);
			iterator.IncrementByRow();
		case 1:
			ErodeKernelRow(iterator, dst_ptr);
	}
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_BinaryErosion(FIABITMAP* src, FilterKernel kernel)
{
	const int dst_width = FreeImage_GetWidth(src->fib) - (2 * kernel.x_radius);
	const int dst_height = FreeImage_GetHeight(src->fib) - (2 * kernel.y_radius);

	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src->fib, dst_width, dst_height);

	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(unsigned char);

	register unsigned char *dst_ptr;

	unsigned char *dst_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(dst);
	
	int kernel_size = (kernel.x_radius * 2 + kernel.y_radius * 2) + 1;
	unsigned char *vals = new unsigned char[kernel_size];

	for(int i=0; i < kernel_size; i++)
		vals[i] = kernel.values[i];

	Kernel<unsigned char> *kern = new Kernel<unsigned char>(src, kernel.x_radius,
										kernel.y_radius, vals, 1.0);
 
	for (register int y=0; y < dst_height; y++)
	{		
		kern->Move(0,y);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			*dst_ptr = kern->KernelCenterValue();

			// If White pixel check neigbours
			if(*dst_ptr > 0)
				BinaryErodeKernel(kern, dst_ptr);

			dst_ptr++;
			kern->Increment();
		}
	}

	delete kern;

	return dst;
};


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_BinaryOpening(FIABITMAP* src, FilterKernel kernel)
{
	// Erosion followed by a dilation.

	FIBITMAP* tmp = FreeImageAlgorithms_BinaryErosion(src, kernel);

	FIABITMAP *border_dib = FreeImageAlgorithms_SetBorder(tmp,
								kernel.x_radius, kernel.y_radius);

	FreeImage_Unload(tmp);
	
	tmp = FreeImageAlgorithms_BinaryDilation(border_dib, kernel);

	FreeImageAlgorithms_Unload(border_dib);

	return tmp;
};

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_BinaryClosing(FIABITMAP* src, FilterKernel kernel)
{
	// Dialation followed by a erosion.

	FIBITMAP* tmp = FreeImageAlgorithms_BinaryDilation(src, kernel);

	FIABITMAP *border_dib = FreeImageAlgorithms_SetBorder(tmp,
								kernel.x_radius, kernel.y_radius);

	FreeImage_Unload(tmp);
	
	tmp = FreeImageAlgorithms_BinaryErosion(border_dib, kernel);

	FreeImageAlgorithms_Unload(border_dib);

	return tmp;
};