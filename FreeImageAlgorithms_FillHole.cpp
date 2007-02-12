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


static inline void ErodeKernelRow(KernelIterator<unsigned char> &iterator, unsigned char *dst_ptr)
{
	register unsigned char *row_ptr;
	register unsigned char *kernel_ptr;

	Kernel<unsigned char> *kernel = iterator.GetKernel();

	int x_max_block_size = kernel->GetNumberOfBlocksOfEightInKernelRows();
	int x_reminder = kernel->GetRemainderAfterBlocksInRows();

	unsigned char *center_value = dst_ptr;


	switch(x_reminder) {

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

static inline void FillBlackNeighbours(Kernel<unsigned char> *kernel, unsigned char *dst_ptr)
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


static inline bool AreAnyNeighboursFG(unsigned char *first_pixel_ptr, int fg_val, const int src_pitch_in_pixels)
{
	unsigned char *ptr = first_pixel_ptr;

	if(ptr[1] == fg_val || ptr[2] == fg_val)
		return true;

	ptr += src_pitch_in_pixels;

	if(ptr[0] || ptr[2] || ptr[2])
		return true;

	ptr += src_pitch_in_pixels;

	if(ptr[0] || ptr[2] || ptr[2])
		return true;	

	return false;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Fillhole(FIBITMAP* src,
							 unsigned char white_on_black, unsigned char exclude_border_particles)
{
	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);

	int bg_val, fg_val;

	if(white_on_black) {
		bg_val = 0;
		fg_val = 255;
	}
	else {
		bg_val = 1;
		fg_val = 255;
	}

	FIBITMAP *dst = FreeImage_Clone(src);

	// First look around the 1 pixel edge of the image and mark any white pixels as 
	// a value of 2 if we are excluding border particles

	FreeImageAlgorithms_SetTernaryPalettePalette(dst, 
									FreeImageAlgorithms_GetRGBQUAD(255, 0, 0),
									2, FreeImageAlgorithms_GetRGBQUAD(0, 0, 255),
									255, FreeImageAlgorithms_GetRGBQUAD(0, 255, 0));

	// Do bottom row
	unsigned char *src_ptr = (unsigned char *) FreeImage_GetScanLine(src, 0);
	unsigned char *dst_ptr = (unsigned char *) FreeImage_GetScanLine(dst, 0);

	/*
	for(int i=0; i < width; i++) {
		
		unsigned char val = src_ptr[i];
		
		if(val == fg_val) {
			// We have a foreground value mark it if we are excluding borders
			if(exclude_border_particles)
				dst_ptr[i] = 2;
		}
	}

	// Do top row
	src_ptr = (unsigned char *) FreeImage_GetScanLine(src, height - 1);
	dst_ptr = (unsigned char *) FreeImage_GetScanLine(dst, height - 1);

	for(int i=0; i < width; i++) {
		
		unsigned char val = src_ptr[i];
		
		if(val == fg_val) { // Change to foreground		
			// We have a foreground value mark it if we are excluding borders
			if(exclude_border_particles)
				dst_ptr[i] = 2;
		}
	}

	// Do left row

	for(int i=0; i < height; i++) {
		
		src_ptr = (unsigned char *) FreeImage_GetScanLine(src, i);
		dst_ptr = (unsigned char *) FreeImage_GetScanLine(dst, i);

		unsigned char val = src_ptr[0];
		
		if(val == fg_val) { // Change to foreground		
			// We have a foreground value mark it if we are excluding borders
			if(exclude_border_particles)
				dst_ptr[0] = 2;
		}
	}


	// Do right row

	for(int i=0; i < height; i++) {
		
		src_ptr = (unsigned char *) FreeImage_GetScanLine(src, i);
		dst_ptr = (unsigned char *) FreeImage_GetScanLine(dst, i);

		unsigned char val = src_ptr[width - 1];
		
		if(val == fg_val) { // Change to foreground		
			// We have a foreground value mark it if we are excluding borders
			if(exclude_border_particles)
				dst_ptr[width - 1] = 2;
		}
	}
*/

	// Now we are going to loop through the image with a 3x3 kernel

	unsigned char *src_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(src);
	unsigned char *dst_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(dst);

	const int src_pitch_in_pixels = FreeImage_GetPitch(src) / sizeof(unsigned char);
	unsigned char *tmp_ptr, *kernel_src_centre_ptr, *kernel_dst_centre_ptr;

	for (register int y=0; y < height; y++)
	{		
		src_ptr = (src_first_pixel_address_ptr + y * src_pitch_in_pixels);
		dst_ptr = (dst_first_pixel_address_ptr + y * src_pitch_in_pixels);

		tmp_ptr = src_ptr;

		for (register int x=0; x < width; x+=2) 
		{
			kernel_src_centre_ptr = tmp_ptr + src_pitch_in_pixels - 1;
			kernel_dst_centre_ptr = dst_ptr + src_pitch_in_pixels - 1;
		
			// If BG pixel check neigbours
		
			if(*kernel_src_centre_ptr == bg_val)
			{
				//if(tmp_ptr[0] == fg_val || tmp_ptr[1] == fg_val || tmp_ptr[2] == fg_val)
				//	continue;

				//tmp_ptr += src_pitch_in_pixels;

				//if(tmp_ptr[0] == fg_val || tmp_ptr[2] == fg_val)
				//	continue;

				//tmp_ptr += src_pitch_in_pixels;

				//if(tmp_ptr[0] == fg_val || tmp_ptr[1] == fg_val || tmp_ptr[2] == fg_val)
				//	continue;

				// No pixels are touching fg so change the center to fg
		
				*kernel_dst_centre_ptr = fg_val;
			}
				
	
			//src_ptr++;
			//dst_ptr++;
		}
	}


	return dst;


	/*
	FIBITMAP *dst = FreeImageAlgorithms_CloneImageType(src->fib, dst_width, dst_height);

	const int src_pitch_in_pixels = FreeImage_GetPitch(src) / sizeof(unsigned char);
	const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(unsigned char);

	register unsigned char *dst_ptr;

	unsigned char *src_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(src);
	unsigned char *dst_first_pixel_address_ptr = (unsigned char*) FreeImage_GetBits(dst);
	
	for (register int y=0; y < dst_height; y++)
	{		
		src_ptr = (src_first_pixel_address_ptr + y * src_pitch_in_pixels);
		dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		for (register int x=0; x < dst_width; x++) 
		{
			// If Black pixel check neigbours
			if(!src_ptr[x])
			{
				if(!AreAnyNeighboursWhite(src_ptr, src_pitch_in_pixels))
					src_ptr[x] = 255;
			}
	
			src_ptr++;
			dst_ptr++;
		}
	}

	return dst;

	*/

};

