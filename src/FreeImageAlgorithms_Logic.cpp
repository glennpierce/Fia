#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Logic.h"
#include "FreeImageAlgorithms_Utilities.h"
#include <limits>
#include <float.h>
#include <math.h>


template<typename Tsrc>
class LOGIC
{
public:

	// I know these are logic functions but oh well no need for a new class
	// + its only private.
	int MaskImage(FIBITMAP* src, FIBITMAP* mask);
};

static int CheckDimensions(FIBITMAP* src, FIBITMAP* mask)
{
	// Check src is the same size as dst
	int src_width = FreeImage_GetWidth(src);
	int src_height = FreeImage_GetHeight(src);
	int dst_width = FreeImage_GetWidth(mask);
	int dst_height = FreeImage_GetHeight(mask);

	if(src_width != dst_width || src_height != dst_height)
		return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

template<typename Tsrc> int 
LOGIC<Tsrc>::MaskImage(FIBITMAP* src, FIBITMAP* mask)
{
	if(mask == NULL || src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Have to be the same size
	if(CheckDimensions(src, mask) ==  FREEIMAGE_ALGORITHMS_ERROR)
		return  FREEIMAGE_ALGORITHMS_ERROR;

	// Mask has to be 8 bit 
	if(FreeImage_GetBPP(mask) != 8 || FreeImage_GetImageType(mask) != FIT_BITMAP)
		return FREEIMAGE_ALGORITHMS_ERROR;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	for(register int y = 0; y < height; y++) { 
		
		Tsrc *src_ptr = (Tsrc *)FreeImage_GetScanLine(src, y);
		unsigned char *mask_ptr = (unsigned char *)FreeImage_GetScanLine(mask, y);

		for(register int x=0; x < width; x++) {
			if(!mask_ptr[x])
				src_ptr[x] = 0;
		}
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

LOGIC<unsigned char>		logicUCharImage;
LOGIC<unsigned short>		logicUShortImage;
LOGIC<short>				logicShortImage;
LOGIC<unsigned long>		logicULongImage;
LOGIC<long>					logicLongImage;
LOGIC<float>				logicFloatImage;
LOGIC<double>				logicDoubleImage;

int DLL_CALLCONV 
FreeImageAlgorithms_MaskImage(FIBITMAP* src, FIBITMAP* mask)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	
			if(FreeImage_GetBPP(src) == 8)
				return  logicUCharImage.MaskImage(src, mask);
		case FIT_UINT16:	
			return logicUShortImage.MaskImage(src, mask);
		case FIT_INT16:		
			return logicShortImage.MaskImage(src, mask);
		case FIT_UINT32:	
			return logicULongImage.MaskImage(src, mask);
		case FIT_INT32:		
			return logicLongImage.MaskImage(src, mask);
		case FIT_FLOAT:	
			return logicFloatImage.MaskImage(src, mask);
		case FIT_DOUBLE:	
			return logicDoubleImage.MaskImage(src, mask);
	}

	return FREEIMAGE_ALGORITHMS_ERROR; 
}


int DLL_CALLCONV 
FreeImageAlgorithms_ReverseMaskImage(FIBITMAP* mask, unsigned char foreground_val)
{
    if(mask == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Mask has to be 8 bit 
	if(FreeImage_GetBPP(mask) != 8 || FreeImage_GetImageType(mask) != FIT_BITMAP)
		return FREEIMAGE_ALGORITHMS_ERROR;

	int width = FreeImage_GetWidth(mask);
	int height = FreeImage_GetHeight(mask);

	for(register int y = 0; y < height; y++) { 
		
		unsigned char *mask_ptr = (unsigned char *)FreeImage_GetScanLine(mask, y);

		for(register int x=0; x < width; x++) {
			if(!mask_ptr[x])
				mask_ptr[x] = foreground_val;
            else
                mask_ptr[x] = 0;
		}
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


