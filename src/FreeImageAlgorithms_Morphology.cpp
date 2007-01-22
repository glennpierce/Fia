#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#define BLOCKSIZE 3

template<class Tsrc>
class MORPHOLOGY
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