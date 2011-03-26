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
