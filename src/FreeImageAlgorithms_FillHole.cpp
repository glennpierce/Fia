#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution_Private.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Fillholes(FIBITMAP* src,
							 unsigned char white_on_black, unsigned char exclude_border_particles)
{
	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);
	const int number_of_pixels = width * height;

	unsigned char *src_ptr = (unsigned char *) FreeImage_GetBits(src);

	int bg_val, fg_val;

	if(white_on_black) {
		bg_val = 0;
		fg_val = 255;
	}
	else {
		bg_val = 1;
		fg_val = 255;
	}

	FIBITMAP *filled_src;

	// Find first background pixel
	for(int i=0; i < number_of_pixels; i++)
	{
		if(*src_ptr++ != bg_val)
			continue;
		
		// We now have the first bg_pixel val so we can flood fill this region 
		// with foreground.
		filled_src = FreeImageAlgorithms_FloodFill(src, i, i / width, fg_val);
		break;
	}
	
	FIBITMAP *dst = FreeImage_Clone(src);

	// Go through filled_src image find background pixels and copy then to src
	unsigned char *filled_src_first_ptr = (unsigned char*) FreeImage_GetBits(filled_src);
	unsigned char *dst_first_ptr = (unsigned char*) FreeImage_GetBits(dst);
	unsigned char *filled_src_ptr, *dst_ptr;
	unsigned int src_pitch_in_pixels = FreeImage_GetPitch(src) / sizeof(unsigned char);

	for (register int y=0; y < height; y++)
	{		
		filled_src_ptr = (filled_src_first_ptr + y * src_pitch_in_pixels);
		dst_ptr = (dst_first_ptr + y * src_pitch_in_pixels);

		for (register int x=0; x < width; x++) 
		{
			// If bg pixel copy to dst
			if(filled_src_ptr[x] == bg_val)
				dst_ptr[x] = fg_val;	
		}
	}

	FreeImage_Unload(filled_src);

	return dst;
};

