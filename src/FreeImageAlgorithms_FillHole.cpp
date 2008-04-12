/*
 * Copyright 2007 Glenn Pierce
 *
 * This file is part of FreeImageAlgorithms.
 *
 * FreeImageAlgorithms is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeImageAlgorithms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 * 
 * You should have received a copy of the Lesser GNU General Public License
 * along with FreeImageAlgorithms.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Particle.h"
#include "FreeImageAlgorithms_Utilities.h"

FIBITMAP* DLL_CALLCONV
FIA_Fillholes(FIBITMAP* src,
							 unsigned char white_on_black)
{
	const int width = FreeImage_GetWidth(src);
	const int height = FreeImage_GetHeight(src);

	unsigned char *src_ptr;

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
	register int y;

	// Find first background pixel
	// To be a valid image I am assumming one of the first pixels on
	// the left side of the image is background.
	// Ie if there is a large strip of foreground on the left the image
	// is not valis for filling holes.
	for(y=0; y < height - 1; y++)
	{
		src_ptr = (unsigned char *) FreeImage_GetScanLine(src, y);

		if(src_ptr[0] == bg_val)
			break;
	}

	filled_src = FIA_FloodFill(src, 0, y, fg_val);

	FIBITMAP *dst = FreeImage_Clone(src);

	// Go through filled_src image find background pixels and copy then to dst
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

