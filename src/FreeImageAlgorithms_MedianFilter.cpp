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

#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Filters.h"
#include "FreeImageAlgorithms_Utilities.h"

#define BLOCKSIZE 8

template < class Tsrc > class FILTER
{
  public:
    FIBITMAP * MedianFilter (FIABITMAP * src, int kernel_x_radius, int kernel_y_radius);
    Tsrc GetMedianFromImage (FIBITMAP * src);

  private:

    inline Tsrc KernelMedian (Tsrc * src_row_ptr);
    inline void GetRow (Tsrc * ptr, Tsrc * kernel_data);
    inline Tsrc quick_select (Tsrc arr[], int n);

    int kernel_width;
    int kernel_height;
    int x_reminder;
    int y_reminder;
    int x_max_block_size;
    int y_max_block_size;
    int src_pitch_in_pixels;
    Tsrc *kernel_tmp_array;
};

template < typename Tsrc > inline Tsrc FILTER < Tsrc >::GetMedianFromImage (FIBITMAP * src)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);
    int total = width * height;

    Tsrc *data = new Tsrc[total];

    register Tsrc *src_ptr;

    for(register int y = 0; y < height; y++)
    {
        src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

        memcpy (data, src_ptr, sizeof (Tsrc) * width);
    }

    Tsrc ret = quick_select_median (data, total);

    delete[]data;

    return ret;
}

template < typename Tsrc > inline void FILTER < Tsrc >::GetRow (Tsrc * ptr, Tsrc * kernel_data)
{
    int x_max_block_size = this->x_max_block_size;
    register Tsrc *tmp;
    register Tsrc *result = kernel_data;

    size_t tsrc_size = sizeof (Tsrc);
    size_t blocksize_in_bytes = tsrc_size * BLOCKSIZE;

    for(register int col = 0; col < x_max_block_size; col += BLOCKSIZE)
    {
        tmp = ptr + col;

        memcpy (result, tmp, blocksize_in_bytes);
    }

    tmp = ptr + x_max_block_size;
    result += x_max_block_size;

    switch (this->x_reminder)
    {
        case 7:
        {
            memcpy (result, tmp, tsrc_size * 7);
            break;
        }

        case 6:
        {
            memcpy (result, tmp, tsrc_size * 6);
            break;
        }

        case 5:
        {
            memcpy (result, tmp, tsrc_size * 5);
            break;
        }

        case 4:
        {
            memcpy (result, tmp, tsrc_size * 4);
            break;
        }

        case 3:
        {
            memcpy (result, tmp, tsrc_size * 3);
            break;
        }

        case 2:
        {
            memcpy (result, tmp, tsrc_size * 2);
            break;
        }

        case 1:
        {
            memcpy (result, tmp, tsrc_size * 1);
        }
    }
}

template < typename Tsrc > inline Tsrc FILTER < Tsrc >::KernelMedian (Tsrc * src_row_ptr)
{
    register Tsrc *array_ptr = this->kernel_tmp_array;
    register Tsrc *tmp_ptr = src_row_ptr;

    for(register int row = 0; row < this->y_max_block_size; row += BLOCKSIZE)
    {
        GetRow (tmp_ptr, array_ptr);
        tmp_ptr += this->src_pitch_in_pixels;
        array_ptr += this->kernel_width;

        GetRow (tmp_ptr, array_ptr);
        tmp_ptr += this->src_pitch_in_pixels;
        array_ptr += this->kernel_width;

        GetRow (tmp_ptr, array_ptr);
        tmp_ptr += this->src_pitch_in_pixels;
        array_ptr += this->kernel_width;

        GetRow (tmp_ptr, array_ptr);
        tmp_ptr += this->src_pitch_in_pixels;
        array_ptr += this->kernel_width;

        GetRow (tmp_ptr, array_ptr);
        tmp_ptr += this->src_pitch_in_pixels;
        array_ptr += this->kernel_width;

        GetRow (tmp_ptr, array_ptr);
        tmp_ptr += this->src_pitch_in_pixels;
        array_ptr += this->kernel_width;

        GetRow (tmp_ptr, array_ptr);
        tmp_ptr += this->src_pitch_in_pixels;
        array_ptr += this->kernel_width;

        GetRow (tmp_ptr, array_ptr);
        tmp_ptr += this->src_pitch_in_pixels;
        array_ptr += this->kernel_width;
    }

    switch (this->y_reminder)
    {
        case 7:
        {
            GetRow (tmp_ptr, array_ptr);
            tmp_ptr += this->src_pitch_in_pixels;
            array_ptr += this->kernel_width;
        }
        case 6:
        {
            GetRow (tmp_ptr, array_ptr);
            tmp_ptr += this->src_pitch_in_pixels;
            array_ptr += this->kernel_width;
        }
        case 5:
        {
            GetRow (tmp_ptr, array_ptr);
            tmp_ptr += this->src_pitch_in_pixels;
            array_ptr += this->kernel_width;
        }
        case 4:
        {
            GetRow (tmp_ptr, array_ptr);
            tmp_ptr += this->src_pitch_in_pixels;
            array_ptr += this->kernel_width;
        }
        case 3:
        {
            GetRow (tmp_ptr, array_ptr);
            tmp_ptr += this->src_pitch_in_pixels;
            array_ptr += this->kernel_width;
        }
        case 2:
        {
            GetRow (tmp_ptr, array_ptr);
            tmp_ptr += this->src_pitch_in_pixels;
            array_ptr += this->kernel_width;
        }
        case 1:
        {
            GetRow (tmp_ptr, array_ptr);
        }
    }

    int kernel_length = this->kernel_width * this->kernel_height;

    // We must now find and return the median value.
    return quick_select_median (this->kernel_tmp_array, kernel_length);
}

template < typename Tsrc > FIBITMAP * FILTER < Tsrc >::MedianFilter (FIABITMAP * src,
                                                                     int kernel_x_radius,
                                                                     int kernel_y_radius)
{
    // Border must be large enough to account for kernel radius
    if (src->xborder < kernel_x_radius || src->yborder < kernel_y_radius)
    {
        return NULL;
    }

    const int src_image_width = FreeImage_GetWidth (src->fib);
    const int src_image_height = FreeImage_GetHeight (src->fib);

    const int dst_width = src_image_width - (2 * src->xborder);
    const int dst_height = src_image_height - (2 * src->yborder);

    FIBITMAP *dst = FIA_CloneImageType (src->fib, dst_width, dst_height);

    const int dst_pitch_in_pixels = FreeImage_GetPitch (dst) / sizeof (Tsrc);

    this->kernel_width = (kernel_x_radius * 2) + 1;
    this->kernel_height = (kernel_y_radius * 2) + 1;
    this->x_reminder = this->kernel_width % BLOCKSIZE;
    this->y_reminder = this->kernel_height % BLOCKSIZE;
    this->x_max_block_size = (this->kernel_width / BLOCKSIZE) * BLOCKSIZE;
    this->y_max_block_size = (this->kernel_height / BLOCKSIZE) * BLOCKSIZE;
    this->src_pitch_in_pixels = FreeImage_GetPitch (src->fib) / sizeof (Tsrc);

    Tsrc *src_first_pixel_address_ptr = (Tsrc *) FreeImage_GetBits (src->fib);
    Tsrc *dst_first_pixel_address_ptr = (Tsrc *) FreeImage_GetBits (dst);

    this->kernel_tmp_array
        = (Tsrc *) malloc (sizeof (Tsrc) * this->kernel_width * this->kernel_height);

    CheckMemory (this->kernel_tmp_array);

    register Tsrc *dst_ptr;
    register Tsrc *src_row_ptr;

    // Amount we need to move in x to get pass the border and onto the image.
    int x_amount_to_image = src->xborder - kernel_x_radius;
    int y_amount_to_image = src->yborder - kernel_y_radius;

    for(register int y = 0; y < dst_height; y++)
    {
        src_row_ptr = src_first_pixel_address_ptr + (y + y_amount_to_image)
            * this->src_pitch_in_pixels + x_amount_to_image;

        dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

        for(register int x = 0; x < dst_width; x++)
        {
            *dst_ptr++ = KernelMedian (src_row_ptr);
            src_row_ptr++;
        }
    }

    free (this->kernel_tmp_array);

    return dst;
}

FILTER < unsigned char >filterUCharImage;
FILTER < unsigned short >filterUShortImage;
FILTER < short >filterShortImage;
FILTER < unsigned long >filterULongImage;
FILTER < long >filterLongImage;
FILTER < float >filterFloatImage;
FILTER < double >filterDoubleImage;

FIBITMAP *DLL_CALLCONV
FIA_MedianFilter (FIABITMAP * src, int kernel_x_radius, int kernel_y_radius)
{
    FIBITMAP *dst = NULL;

    if (!src->fib)
    {
        return NULL;
    }

    // convert from src_type to FIT_BITMAP
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src->fib);

    switch (src_type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src->fib) == 8)
            {
                dst = filterUCharImage.MedianFilter (src, kernel_x_radius, kernel_y_radius);
            }
            break;
        }
        case FIT_UINT16:
        {                       // array of unsigned short: unsigned 16-bit
            dst = filterUShortImage.MedianFilter (src, kernel_x_radius, kernel_y_radius);
            break;
        }
        case FIT_INT16:
        {                       // array of short: signed 16-bit
            dst = filterShortImage.MedianFilter (src, kernel_x_radius, kernel_y_radius);
            break;
        }
        case FIT_UINT32:
        {                       // array of unsigned long: unsigned 32-bit
            dst = filterULongImage.MedianFilter (src, kernel_x_radius, kernel_y_radius);
            break;
        }
        case FIT_INT32:
        {                       // array of long: signed 32-bit
            dst = filterLongImage.MedianFilter (src, kernel_x_radius, kernel_y_radius);
            break;
        }
        case FIT_FLOAT:
        {                       // array of float: 32-bit
            dst = filterFloatImage.MedianFilter (src, kernel_x_radius, kernel_y_radius);
            break;
        }
        case FIT_DOUBLE:
        {                       // array of double: 64-bit
            dst = filterDoubleImage.MedianFilter (src, kernel_x_radius, kernel_y_radius);
            break;
        }
        default:
        {                       // array of FICOMPLEX: 2 x 64-bit
            break;
        }
    }

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to perform filter on type.",
                                     src_type);
    }

    return dst;
}

double DLL_CALLCONV
FIA_GetMedianFromImage (FIBITMAP * src)
{
    if (!src)
    {
        return 0.0;
    }

    // convert from src_type to FIT_BITMAP
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
            {
                return filterUCharImage.GetMedianFromImage (src);
            }
            break;
        }
        case FIT_UINT16:
        {                       // array of unsigned short: unsigned 16-bit
            return filterUShortImage.GetMedianFromImage (src);
            break;
        }
        case FIT_INT16:
        {                       // array of short: signed 16-bit
            return filterShortImage.GetMedianFromImage (src);
            break;
        }
        case FIT_UINT32:
        {                       // array of unsigned long: unsigned 32-bit
            return filterULongImage.GetMedianFromImage (src);
            break;
        }
        case FIT_INT32:
        {                       // array of long: signed 32-bit
            return filterLongImage.GetMedianFromImage (src);
            break;
        }
        case FIT_FLOAT:
        {                       // array of float: 32-bit
            return filterFloatImage.GetMedianFromImage (src);
            break;
        }
        case FIT_DOUBLE:
        {                       // array of double: 64-bit
            return filterDoubleImage.GetMedianFromImage (src);
            break;
        }
        default:
        {
            break;
        }
    }

    return 0.0;
}
