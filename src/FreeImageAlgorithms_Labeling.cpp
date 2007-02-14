#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution_Private.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

class BINARY_LABEL
{
public:
	
	FIBITMAP* Label(FIBITMAP* src, unsigned char bg_colour);
};

// White on black
FIBITMAP* BINARY_LABEL::Label(FIBITMAP* src, unsigned char bg_colour)
{
	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);
	int label = 2;

	FIBITMAP *dst = FreeImage_Clone(src);
	//FIBITMAP *dst = FreeImage_ConvertToType(src, FIT_INT16, 0);

	// Go through filled_src image find background pixels and copy then to src
	unsigned char *src_first_ptr = (unsigned char*) FreeImage_GetBits(src);
	unsigned char *dst_first_ptr = (unsigned char*) FreeImage_GetBits(dst);
	unsigned char *dst_ptr;
	unsigned int src_pitch_in_pixels = FreeImage_GetPitch(src) / sizeof(unsigned char);

	for (register int y=0; y < height; y++)
	{		
		dst_ptr = (dst_first_ptr + y * src_pitch_in_pixels);

		for (register int x=0; x < width; x++) 
		{
			// If bg pixel leave as 0
			if(dst_ptr[x] > label ) {
			
				// Do a fill on this region.
				FreeImageAlgorithms_InPlaceFloodFill(dst, x, y, label++);
			}
		}
	}

	return dst;
};


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Label(FIBITMAP* src, unsigned char bg_colour)
{
	BINARY_LABEL labeler;

	return labeler.Label(src, bg_colour);
};
