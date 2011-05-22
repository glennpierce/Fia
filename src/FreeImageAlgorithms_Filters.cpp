/* 
 * Copyright 2007-2010 Glenn Pierce, Paul Barber,
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Filters.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"

#include <math.h>

FIBITMAP *DLL_CALLCONV
FIA_Sobel (FIBITMAP * src)
{
    FIBITMAP *magnitude_dib = NULL;

    int err = FIA_SobelAdvanced (src, NULL, NULL, &magnitude_dib);

    if (err != FIA_SUCCESS)
    {
        return NULL;
    }

    return magnitude_dib;
}

int DLL_CALLCONV
FIA_SobelAdvanced (FIBITMAP * src,
                   FIBITMAP ** vertical, FIBITMAP ** horizontal, FIBITMAP ** magnitude)
{
    double sobel_horizontal_kernel[] = { 1.0, 2.0, 1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0 };
    double sobel_vertical_kernel[] = { -1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0 };

    FIABITMAP *src_bordered = FIA_SetBorder (src, 1, 1, BorderType_Copy, 0.0);

    FIBITMAP *vertical_tmp = NULL, *horizontal_tmp = NULL;

    if (vertical != NULL || magnitude != NULL)
    {
        FilterKernel convolve_kernel_left = FIA_NewKernel (1, 1,
                                                           sobel_vertical_kernel, 1.0);

        vertical_tmp = FIA_Convolve (src_bordered, convolve_kernel_left);
    }

    if (horizontal != NULL || magnitude != NULL)
    {
        FilterKernel convolve_kernel_top = FIA_NewKernel (1, 1,
                                                          sobel_horizontal_kernel, 1.0);

        horizontal_tmp = FIA_Convolve (src_bordered, convolve_kernel_top);
    }

    FIA_Unload (src_bordered);

    // We need both vertical_tmp and horizontal_tmp to calculate the magnitude.
    if (magnitude != NULL)
    {
        if (vertical_tmp == NULL || horizontal_tmp == NULL)
        {
            return FIA_ERROR;
        }

        int dst_width = FreeImage_GetWidth (vertical_tmp);
        int dst_height = FreeImage_GetHeight (vertical_tmp);

        FIBITMAP *dst = FreeImage_AllocateT (FIT_DOUBLE, dst_width, dst_height,
                                             8, 0, 0, 0);

        const int dst_pitch_in_pixels = FreeImage_GetPitch (dst) / sizeof (double);
        register double *dst_ptr, *dib1_ptr, *dib2_ptr;

        double *dst_first_pixel_address_ptr = (double *) FreeImage_GetBits (dst);
        double *dib1_first_pixel_address_ptr = (double *) FreeImage_GetBits (vertical_tmp);
        double *dib2_first_pixel_address_ptr = (double *) FreeImage_GetBits (horizontal_tmp);

        for(register int y = 0; y < dst_height; y++)
        {
            dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);
            dib1_ptr = (dib1_first_pixel_address_ptr + y * dst_pitch_in_pixels);
            dib2_ptr = (dib2_first_pixel_address_ptr + y * dst_pitch_in_pixels);

            for(register int x = 0; x < dst_width; x++)
            {
                *dst_ptr++ = sqrt ((*dib1_ptr * *dib1_ptr) + (*dib2_ptr * *dib2_ptr));
                ++dib1_ptr;
                ++dib2_ptr;
            }
        }

        *magnitude = dst;
    }

    if (vertical != NULL)
    {
        *vertical = vertical_tmp;
    }
    else
    {
        FreeImage_Unload (vertical_tmp);
    }

    if (horizontal != NULL)
    {
        *horizontal = horizontal_tmp;
    }
    else
    {
        FreeImage_Unload (horizontal_tmp);
    }

    return FIA_SUCCESS;
}

static void DLL_CALLCONV
FIA_MakeCircularKernel (int radius, double *kernel)
{
    double  dsq, maxdsq;
	int x, y, width, abs_x, abs_y;
	double *kernel_ptr = kernel;

	width = 2*radius+1;
	maxdsq = radius*radius;

	for (x=0; x < width; x++) {

		abs_x = abs(x-radius);

		for (y=0; y < width; y++)
		{
			abs_y = abs(y-radius);

			dsq = (abs_x * abs_x) + (abs_y * abs_y);

			if (dsq<=maxdsq)
				kernel_ptr[y*width+x]=1.0;
			else 
				kernel_ptr[y*width+x]=0.0;
		}
	}
}

static void DLL_CALLCONV
FIA_MakeGaussianKernel (int radius, double *kernel)
{
	double  dsq, sigmasq;
	int x, y, width, abs_x, abs_y;
	double *kernel_ptr = kernel;

	width = 2*radius+1;
	sigmasq = pow(radius/2.0,2);

	for (x=0; x < width; x++) {

		abs_x = abs(x-radius);

		for (y=0; y < width; y++)
		{
			abs_y = abs(y-radius);

			dsq = (abs_x * abs_x) + (abs_y * abs_y);

			kernel_ptr[y*width+x] = (double) exp(-(dsq/sigmasq));
		}
	}
}

static void DLL_CALLCONV
FIA_MakeSquareKernel (int radius, double *kernel)
{
	int x, y, width;
	double *kernel_ptr = kernel;

	width = 2*radius+1;
	
	for (x=0; x < width; x++) {

		for (y=0; y < width; y++)
		{
			kernel_ptr[y*width+x] = (double) 1.0;
		}
	}
}

FIBITMAP* DLL_CALLCONV
FIA_Binning (FIBITMAP * src, FIA_BINNING_TYPE type, int radius)
{
	int size = radius * 2 + 1;
	double *kernel = (double*) malloc ((size*size)*sizeof(double));
	
	if(type == FIA_BINNING_GAUSSIAN)
		FIA_MakeGaussianKernel (radius, kernel);
	else if(type == FIA_BINNING_CIRCULAR)
		FIA_MakeCircularKernel (radius, kernel);
	else 
		FIA_MakeSquareKernel (radius, kernel);

    FIABITMAP *src_bordered = FIA_SetBorder (src, radius, radius, BorderType_Copy, 0.0);

    FilterKernel convolve_kernel = FIA_NewKernel (radius, radius, kernel, 1.0);

    FIBITMAP* binned_fib = FIA_Convolve (src_bordered, convolve_kernel);

    FIA_Unload (src_bordered);

	free(kernel);

    return binned_fib;
}
