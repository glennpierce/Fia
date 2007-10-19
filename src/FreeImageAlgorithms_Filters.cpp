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
#include "FreeImageAlgorithms_Error.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Filters.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"

#include <math.h>

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Sobel(FIBITMAP *src)
{
    FIBITMAP *magnitude_dib = NULL;

    int err = FreeImageAlgorithms_SobelAdvanced(src, NULL, NULL,
        &magnitude_dib);
     
    if(err != FREEIMAGE_ALGORITHMS_SUCCESS)
        return NULL;

	return magnitude_dib;
}

int DLL_CALLCONV
FreeImageAlgorithms_SobelAdvanced(FIBITMAP *src,
                                  FIBITMAP** vertical,
                                  FIBITMAP** horizontal,
                                  FIBITMAP** magnitude)
{
	double sobel_horizontal_kernel[] = {1.0, 2.0, 1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0};
    double sobel_vertical_kernel[] = {-1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0};

    FIABITMAP *src_bordered = FreeImageAlgorithms_SetBorder(src, 1, 1, BorderType_Copy, 0.0);

    FIBITMAP *vertical_tmp = NULL, *horizontal_tmp = NULL;

    if(vertical != NULL || magnitude != NULL) {
  
        FilterKernel convolve_kernel_left = FreeImageAlgorithms_NewKernel(1, 1,
		                                        sobel_vertical_kernel, 1.0);

        vertical_tmp = FreeImageAlgorithms_Convolve(src_bordered, convolve_kernel_left);
    }

    if(horizontal != NULL || magnitude != NULL) {

        FilterKernel convolve_kernel_top = FreeImageAlgorithms_NewKernel(1, 1,
		                                        sobel_horizontal_kernel, 1.0);
    
	    horizontal_tmp = FreeImageAlgorithms_Convolve(src_bordered, convolve_kernel_top);
    }

    FreeImageAlgorithms_Unload(src_bordered);

    // We need both vertical_tmp and horizontal_tmp to calculate the magnitude.
    if(magnitude != NULL) { 
        
        if (vertical_tmp == NULL || horizontal_tmp == NULL)
            return FREEIMAGE_ALGORITHMS_ERROR;

	    int dst_width = FreeImage_GetWidth(vertical_tmp);
	    int dst_height = FreeImage_GetHeight(vertical_tmp);

	    FIBITMAP* dst = FreeImage_AllocateT(FIT_DOUBLE, dst_width, dst_height,
										        8, 0, 0, 0);

	    const int dst_pitch_in_pixels = FreeImage_GetPitch(dst) / sizeof(double);
        register double *dst_ptr, *dib1_ptr, *dib2_ptr;

	    double *dst_first_pixel_address_ptr = (double*) FreeImage_GetBits(dst);
	    double *dib1_first_pixel_address_ptr = (double*) FreeImage_GetBits(vertical_tmp);
	    double *dib2_first_pixel_address_ptr = (double*) FreeImage_GetBits(horizontal_tmp);

	    for (register int y=0; y < dst_height; y++)
	    {		
		    dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);
		    dib1_ptr = (dib1_first_pixel_address_ptr + y * dst_pitch_in_pixels);
		    dib2_ptr = (dib2_first_pixel_address_ptr + y * dst_pitch_in_pixels);

		    for (register int x=0; x < dst_width; x++) 
		    {
			    *dst_ptr++ = sqrt((*dib1_ptr * *dib1_ptr) + (*dib2_ptr * *dib2_ptr));
			    ++dib1_ptr;
			    ++dib2_ptr;
		    }
	    }

        *magnitude = dst;
    }
    
    if(vertical != NULL)
        *vertical = vertical_tmp;
    else
        FreeImage_Unload(vertical_tmp);

    if(horizontal != NULL)
        *horizontal = horizontal_tmp;
    else
        FreeImage_Unload(horizontal_tmp);

    return FREEIMAGE_ALGORITHMS_SUCCESS;
}



// Test
// Seems slow but maybe not for large kernels.
/*
FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_SeparableSobel(FIBITMAP *src)
{
	double sobel_horz_values[] = {-1.0, 0.0, 1.0};

	double sobel_vert_values[] = {1.0, 2.0, 1.0};

	FilterKernel sobel_horz_kernel = FreeImageAlgorithms_NewKernel(1, 0,
		sobel_horz_values, 1.0);

	FilterKernel sobel_vert_kernel = FreeImageAlgorithms_NewKernel(0, 1,
		sobel_vert_values, 1.0);

	FIBITMAP* dst = FreeImageAlgorithms_SeparableConvolve(src, sobel_horz_kernel, sobel_vert_kernel);

	return dst;
}
*/
