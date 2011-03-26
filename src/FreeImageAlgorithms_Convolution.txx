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

#ifndef __FREEIMAGE_ALGORITHMS_CONVOLUTION_PRIVATE__
#define __FREEIMAGE_ALGORITHMS_CONVOLUTION_PRIVATE__

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "FreeImageAlgorithms_Utils.h"

#define BLOCKSIZE 8

template < typename Tsrc > class Kernel;

template < typename Tsrc > class KernelIterator
{
  public:

    KernelIterator (Kernel < Tsrc > *kernel)
    {
        this->kernel = kernel;
        this->current_kernel_ptr = const_cast < Tsrc * >(kernel->KernelValues ());
        this->current_image_ptr = kernel->KernelFirstValuePtr ();
    }

    inline void Increment ()
    {
        this->current_kernel_ptr++;
        this->current_image_ptr++;
    }

    inline void IncrementByRow ()
    {
        this->current_kernel_ptr += kernel->KernelWidth ();
        this->current_image_ptr += kernel->ImagePitchInPixels ();
    }

    inline Tsrc GetKernelValue ()
    {
        return *(this->current_kernel_ptr);
    }

    inline Tsrc GetImageValue ()
    {
        return *(this->current_image_ptr);
    }

    inline Tsrc *GetKernelPtrValue ()
    {
        return this->current_kernel_ptr;
    }

    inline Tsrc *GetImagePtrValue ()
    {
        return this->current_image_ptr;
    }

    inline Kernel < Tsrc > *GetKernel ()
    {
        return this->kernel;
    }

  private:

    Tsrc * current_kernel_ptr;
    Tsrc *current_image_ptr;

    Kernel < Tsrc > *kernel;
};

template < typename Tsrc > class Kernel
{
  public:
    Kernel (FIABITMAP * src, int x_radius, int y_radius, const Tsrc * values, double divider);

	inline Tsrc* GetPtrToLine (int line)
    {
        return (const_cast < Tsrc * >(this->src_first_pixel_address_ptr) +
			 ((line + this->y_amount_to_image) * this->src_pitch_in_pixels));
    }

	/*
	inline Tsrc* GetPtrToLineFromTopLeft (int line)
    {
        return (const_cast < Tsrc * >(this->src_first_pixel_address_ptr) +
			 (((this->src_image_height - 1 - line) + this->y_amount_to_image) * this->src_pitch_in_pixels));
    }
    */
    
    inline void Move (int x, int y)
    {
		this->current_src_ptr = GetPtrToLine (y) + (x_amount_to_image + x);

        this->current_src_center_ptr = this->current_src_ptr + (y_radius * this->src_pitch_in_pixels) - x_radius;
    }

	/*
	inline void MoveFromTopLeft (int x, int y)
    {
        this->current_src_ptr = GetPtrToLineFromTopLeft (y) + (x_amount_to_image + x);

        this->current_src_center_ptr = this->current_src_ptr + (y_radius * this->src_pitch_in_pixels) - x_radius;
    }
	*/
	
	inline void SetSearchArea (FIARECT search_area)
    {
        this->search_area = search_area;
    }
    
    inline void SetMask (FIBITMAP *mask)
    {
        this->mask = mask;
    }

    inline void MoveUpRow ()
    {
        this->current_src_ptr += this->src_pitch_in_pixels;
        this->current_src_center_ptr += this->src_pitch_in_pixels;
    }

    inline void Increment ()
    {
        ++(this->current_src_ptr);
        ++(this->current_src_center_ptr);
    }

    inline int ImageWidth ()
    {
        return this->src_image_width;
    }
    inline int ImageHeight ()
    {
        return this->src_image_height;
    }
    inline int KernelWidth ()
    {
        return this->kernel_width;
    }
    inline int ImagePitchInPixels ()
    {
        return this->src_pitch_in_pixels;
    }
    inline const Tsrc *KernelValues ()
    {
        return this->values;
    }
    inline Tsrc GetNumberOfBlocksOfEightInKernelRows ()
    {
        return this->x_max_block_size;
    }
    inline Tsrc GetNumberOfBlocksOfEightInKernelColoumns ()
    {
        return this->y_max_block_size;
    }
    inline Tsrc GetRemainderAfterBlocksInRows ()
    {
        return this->x_reminder;
    }
    inline Tsrc GetRemainderAfterBlocksInColoumns ()
    {
        return this->y_reminder;
    }
    inline Tsrc *ImageStartPtr ()
    {
        return this->src_first_pixel_address_ptr;
    }
    inline Tsrc *KernelFirstValuePtr ()
    {
        return this->current_src_ptr;
    }
    inline Tsrc KernelFirstValue ()
    {
        return *(this->current_src_ptr);
    }
    inline Tsrc *KernelCenterValuePtr ()
    {
        return this->current_src_center_ptr;
    }
    inline Tsrc KernelCenterValue ()
    {
        return *(this->current_src_center_ptr);
    }
    inline KernelIterator < Tsrc > Begin ()
    {
        return KernelIterator < Tsrc > (this);
    }

    FIBITMAP *Convolve ();
    FIBITMAP *Correlate ();

  private:

    inline void ConvolveKernel ();
    inline void CorrelateKernel ();
    inline void ConvolveKernelRow (KernelIterator < Tsrc > &iterator);
    inline double AddKernelRow (KernelIterator < Tsrc > &iterator);
    inline double ImageAverageAtKernel (void);
    inline void CorrelateKernelRow (KernelIterator < Tsrc > &iterator, double average);

    FIBITMAP *dib;
    FIBITMAP *mask;
    FIARECT search_area;
    const int xborder;
    const int yborder;
    const int x_radius;
    const int y_radius;
    const double divider;

    const int src_image_width;
    const int src_image_height;
    const int kernel_width;
    const int kernel_height;
    const int x_reminder;
    const int y_reminder;
    const int x_max_block_size;
    const int y_max_block_size;
    const int src_pitch_in_pixels;
    const FREE_IMAGE_TYPE src_image_type;
    const Tsrc *values;
    int x_amount_to_image;
    int y_amount_to_image;
    double kernel_average;

    Tsrc *src_first_pixel_address_ptr;

    double sum;
    double correlation_sum_squared;

    Tsrc *current_src_center_ptr;
    Tsrc *current_src_ptr;
};

// The following code does a lot of loop unrolling for performance.
template < typename Tsrc > Kernel < Tsrc >::Kernel (FIABITMAP * src, int x_radius, int y_radius,
                                                    const Tsrc * values, double divider):
xborder (src->xborder),
yborder (src->yborder),
x_radius (x_radius),
y_radius (y_radius),
divider (divider),
src_image_width (FreeImage_GetWidth (src->fib)),
src_image_height (FreeImage_GetHeight (src->fib)),
kernel_width (x_radius * 2 + 1),
kernel_height (y_radius * 2 + 1),
x_reminder (kernel_width % BLOCKSIZE),
y_reminder (kernel_height % BLOCKSIZE),
x_max_block_size ((kernel_width / BLOCKSIZE) * BLOCKSIZE),
y_max_block_size ((kernel_height / BLOCKSIZE) * BLOCKSIZE),
src_pitch_in_pixels (FreeImage_GetPitch (src->fib) / sizeof (Tsrc)),
src_image_type (FreeImage_GetImageType (src->fib)),
values (values)
{
    // Border must be large enough to account for kernel radius
    if (xborder < x_radius || yborder < y_radius)
        return;

	this->search_area = FIA_EMPTY_RECT;
	this->mask = NULL;
    this->dib = src->fib;

    this->src_first_pixel_address_ptr = (Tsrc *) FreeImage_GetBits (this->dib);
    this->current_src_ptr = const_cast < Tsrc * >(this->src_first_pixel_address_ptr);

    // Amount we need to move in x to get pass the border and onto the image.
    this->x_amount_to_image = this->xborder - this->x_radius;
    this->y_amount_to_image = this->yborder - this->y_radius;

    this->Move (0, 0);
}

template < typename Tsrc > inline void Kernel < Tsrc >::ConvolveKernelRow (KernelIterator < Tsrc >
                                                                           &iterator)
{
    register double *tmp, *kernel_ptr;

    int x_max_block_size = this->x_max_block_size;
    int x_reminder = this->x_reminder;

    for(register int col = 0; col < x_max_block_size; col += BLOCKSIZE)
    {
        tmp = iterator.GetImagePtrValue () + col;
        kernel_ptr = iterator.GetKernelPtrValue () + col;

        sum += tmp[0] * kernel_ptr[0] + tmp[1] * kernel_ptr[1] + tmp[2] * kernel_ptr[2] + tmp[3]
            * kernel_ptr[3] + tmp[4] * kernel_ptr[4] + tmp[5] * kernel_ptr[5] + tmp[6]
            * kernel_ptr[6] + tmp[7] * kernel_ptr[7];
    }

    if (x_reminder)
    {
        tmp = iterator.GetImagePtrValue () + x_max_block_size;
        kernel_ptr = iterator.GetKernelPtrValue () + x_max_block_size;
    }

    switch (x_reminder)
    {
        case 7:
        {
            sum += tmp[6] * kernel_ptr[6] + tmp[5] * kernel_ptr[5] + tmp[4] * kernel_ptr[4]
                + tmp[3] * kernel_ptr[3] + tmp[2] * kernel_ptr[2] + tmp[1] * kernel_ptr[1]
                + tmp[0] * kernel_ptr[0];
            break;
        }
        case 6:
        {
            sum += tmp[5] * kernel_ptr[5] + tmp[4] * kernel_ptr[4] + tmp[3] * kernel_ptr[3]
                + tmp[2] * kernel_ptr[2] + tmp[1] * kernel_ptr[1] + tmp[0] * kernel_ptr[0];
            break;
        }
        case 5:
        {
            sum += tmp[4] * kernel_ptr[4] + tmp[3] * kernel_ptr[3] + tmp[2] * kernel_ptr[2]
                + tmp[1] * kernel_ptr[1] + tmp[0] * kernel_ptr[0];
            break;
        }
        case 4:
        {
            sum += tmp[3] * kernel_ptr[3] + tmp[2] * kernel_ptr[2] + tmp[1] * kernel_ptr[1]
                + tmp[0] * kernel_ptr[0];
            break;
        }
        case 3:
        {
            sum += tmp[2] * kernel_ptr[2] + tmp[1] * kernel_ptr[1] + tmp[0] * kernel_ptr[0];
            break;
        }
        case 2:
        {
            sum += tmp[1] * kernel_ptr[1] + tmp[0] * kernel_ptr[0];
            break;
        }
        case 1:
        {
            sum += tmp[0] * kernel_ptr[0];
        }
    }
}

template < typename Tsrc > inline void Kernel < Tsrc >::ConvolveKernel ()
{
    this->sum = 0.0f;

    KernelIterator < Tsrc > iterator = this->Begin ();

    for(register int row = 0; row < this->y_max_block_size; row += BLOCKSIZE)
    {
        ConvolveKernelRow (iterator);
        iterator.IncrementByRow ();

        ConvolveKernelRow (iterator);
        iterator.IncrementByRow ();

        ConvolveKernelRow (iterator);
        iterator.IncrementByRow ();

        ConvolveKernelRow (iterator);
        iterator.IncrementByRow ();

        ConvolveKernelRow (iterator);
        iterator.IncrementByRow ();

        ConvolveKernelRow (iterator);
        iterator.IncrementByRow ();

        ConvolveKernelRow (iterator);
        iterator.IncrementByRow ();

        ConvolveKernelRow (iterator);
        iterator.IncrementByRow ();
    }

    switch (this->y_reminder)
    {
        case 7:
        {
            ConvolveKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 6:
        {
            ConvolveKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 5:
        {
            ConvolveKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 4:
        {
            ConvolveKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 3:
        {
            ConvolveKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 2:
        {
            ConvolveKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 1:
        {
            ConvolveKernelRow (iterator);
        }
    }
}

template < typename Tsrc > FIBITMAP * Kernel < Tsrc >::Convolve ()
{
    const int dst_width = src_image_width - (2 * this->xborder);
    const int dst_height = src_image_height - (2 * this->yborder);

    FIBITMAP *dst = FIA_CloneImageType (this->dib, dst_width, dst_height);

    const int dst_pitch_in_pixels = FreeImage_GetPitch (dst) / sizeof (Tsrc);

    register double *dst_ptr;

    double *dst_first_pixel_address_ptr = (Tsrc *) FreeImage_GetBits (dst);

    for(register int y = 0; y < dst_height; y++)
    {
        this->Move (0, y);
        dst_ptr = (dst_first_pixel_address_ptr + y * dst_pitch_in_pixels);

        for(register int x = 0; x < dst_width; x++)
        {
            this->ConvolveKernel ();
            *dst_ptr++ = this->sum / this->divider;
            this->Increment ();
        }
    }

    return dst;
}

// Adds the pixel values of the original image pixels
// that are covered by the kernel row.
template < typename Tsrc > inline double Kernel < Tsrc >::AddKernelRow (KernelIterator < Tsrc >
                                                                        &iterator)
{
    register double *tmp;

    int x_max_block_size = this->x_max_block_size;
    int x_reminder = this->x_reminder;
    double sum = 0.0;

    for(register int col = 0; col < x_max_block_size; col += BLOCKSIZE)
    {
        tmp = iterator.GetImagePtrValue () + col;

        sum += tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7];
    }

    if (x_reminder)
    {
        tmp = iterator.GetImagePtrValue () + x_max_block_size;
    }

    switch (x_reminder)
    {
        case 7:
        {
            sum += tmp[6] + tmp[5] + tmp[4] + tmp[3] + tmp[2] + tmp[1] + tmp[0];
            break;
        }

        case 6:
        {
            sum += tmp[5] + tmp[4] + tmp[3] + tmp[2] + tmp[1] + tmp[0];
            break;
        }

        case 5:
        {
            sum += tmp[4] + tmp[3] + tmp[2] + tmp[1] + tmp[0];
            break;
        }

        case 4:
        {
            sum += tmp[3] + tmp[2] + tmp[1] + tmp[0];
            break;
        }

        case 3:
        {
            sum += tmp[2] + tmp[1] + tmp[0];
            break;
        }
        case 2:
        {
            sum += tmp[1] + tmp[0];
            break;
        }
        case 1:
        {
            sum += tmp[0];
        }
    }

    return sum;
}

template < typename Tsrc > inline void Kernel < Tsrc >::CorrelateKernelRow (KernelIterator < Tsrc >
                                                                            &iterator, double avg)
{
    register double *tmp, *kernel_ptr;

    int x_max_block_size = this->x_max_block_size;
    int x_reminder = this->x_reminder;
    double kavg = this->kernel_average;
    double var0, var1, var2, var3, var4, var5, var6, var7;

    for(register int col = 0; col < x_max_block_size; col += BLOCKSIZE)
    {
        tmp = iterator.GetImagePtrValue () + col;
        kernel_ptr = iterator.GetKernelPtrValue () + col;

        var0 = tmp[0] - avg;
        var1 = tmp[1] - avg;
        var2 = tmp[2] - avg;
        var3 = tmp[3] - avg;
        var4 = tmp[4] - avg;
        var5 = tmp[5] - avg;
        var6 = tmp[6] - avg;
        var7 = tmp[7] - avg;

        this->sum += (var0 * (kernel_ptr[0] - kavg) + var1 * (kernel_ptr[1] - kavg) + var2
                      * (kernel_ptr[2] - kavg) + var3 * (kernel_ptr[3] - kavg) +
                      var4 * (kernel_ptr[4] - kavg) + var5 * (kernel_ptr[5] - kavg) +
                      var6 * (kernel_ptr[6] - kavg) + var7 * (kernel_ptr[7] - kavg));

        this->correlation_sum_squared += ((var0 * var0) + (var1 * var1) + (var2 * var2) + (var3
                                                                                           * var3) +
                                          (var4 * var4) + (var5 * var5) + (var6 * var6) +
                                          (var7 * var7));
    }

    if (x_reminder)
    {
        tmp = iterator.GetImagePtrValue () + x_max_block_size;
        kernel_ptr = iterator.GetKernelPtrValue () + x_max_block_size;

        var0 = tmp[0] - avg;
        var1 = tmp[1] - avg;
        var2 = tmp[2] - avg;
        var3 = tmp[3] - avg;
        var4 = tmp[4] - avg;
        var5 = tmp[5] - avg;
        var6 = tmp[6] - avg;
        var7 = tmp[7] - avg;
    }

    switch (x_reminder)
    {
        case 7:
        {
            this->sum += ((tmp[6] - avg) * (kernel_ptr[6] - kavg) + (tmp[5] - avg) * (kernel_ptr[5]
                                                                                      - kavg) +
                          (tmp[4] - avg) * (kernel_ptr[4] - kavg) + (tmp[3] -
                                                                     avg) * (kernel_ptr[3] - kavg) +
                          (tmp[2] - avg) * (kernel_ptr[2] - kavg) + (tmp[1] -
                                                                     avg) * (kernel_ptr[1] - kavg) +
                          (tmp[0] - avg) * (kernel_ptr[0] - kavg));

            this->correlation_sum_squared += ((var6 * var6) + (var5 * var5) + (var4 * var4) + (var3
                                                                                               *
                                                                                               var3)
                                              + (var2 * var2) + (var1 * var1) + (var0 * var0));

            break;
        }
        case 6:
        {
            this->sum += ((tmp[5] - avg) * (kernel_ptr[5] - kavg) + (tmp[4] - avg) * (kernel_ptr[4]
                                                                                      - kavg) +
                          (tmp[3] - avg) * (kernel_ptr[3] - kavg) + (tmp[2] -
                                                                     avg) * (kernel_ptr[2] - kavg) +
                          (tmp[1] - avg) * (kernel_ptr[1] - kavg) + (tmp[0] -
                                                                     avg) * (kernel_ptr[0] - kavg));

            this->correlation_sum_squared += ((var5 * var5) + (var4 * var4) + (var3 * var3) + (var2
                                                                                               *
                                                                                               var2)
                                              + (var1 * var1) + (var0 * var0));

            break;
        }
        case 5:
        {
            this->sum += ((tmp[4] - avg) * (kernel_ptr[4] - kavg) + (tmp[3] - avg) * (kernel_ptr[3]
                                                                                      - kavg) +
                          (tmp[2] - avg) * (kernel_ptr[2] - kavg) + (tmp[1] -
                                                                     avg) * (kernel_ptr[1] - kavg) +
                          (tmp[0] - avg) * (kernel_ptr[0] - kavg));

            this->correlation_sum_squared += ((var4 * var4) + (var3 * var3) + (var2 * var2) + (var1
                                                                                               *
                                                                                               var1)
                                              + (var0 * var0));

            break;
        }
        case 4:
        {
            this->sum += ((tmp[3] - avg) * (kernel_ptr[3] - kavg) + (tmp[2] - avg) * (kernel_ptr[2]
                                                                                      - kavg) +
                          (tmp[1] - avg) * (kernel_ptr[1] - kavg) + (tmp[0] -
                                                                     avg) * (kernel_ptr[0] - kavg));

            this->correlation_sum_squared += ((var3 * var3) + (var2 * var2) + (var1 * var1) + (var0
                                                                                               *
                                                                                               var0));

            break;
        }
        case 3:
        {
            this->sum += ((tmp[2] - avg) * (kernel_ptr[2] - kavg) + (tmp[1] - avg) * (kernel_ptr[1]
                                                                                      - kavg) +
                          (tmp[0] - avg) * (kernel_ptr[0] - kavg));

            this->correlation_sum_squared += ((var2 * var2) + (var1 * var1) + (var0 * var0));

            break;
        }
        case 2:
        {
            this->sum += ((tmp[1] - avg) * (kernel_ptr[1] - kavg) + (tmp[0] - avg) * (kernel_ptr[0]
                                                                                      - kavg));

            this->correlation_sum_squared += ((var1 * var1) + (var0 * var0));

            break;
        }
        case 1:
        {
            this->sum += (tmp[0] - avg) * (kernel_ptr[0] - kavg);

            this->correlation_sum_squared += (var0 * var0);
        }
    }
}

// Calculates the average value of the pixels in the original image
// of the pixels covered by the kernel.
template < typename Tsrc > inline double Kernel < Tsrc >::ImageAverageAtKernel (void)
{
    double sum = 0.0f;
    int kernel_size = kernel_width * kernel_height;

    KernelIterator < Tsrc > iterator = this->Begin ();

    for(register int row = 0; row < this->y_max_block_size; row += BLOCKSIZE)
    {
        sum += AddKernelRow (iterator);
        iterator.IncrementByRow ();

        sum += AddKernelRow (iterator);
        iterator.IncrementByRow ();

        sum += AddKernelRow (iterator);
        iterator.IncrementByRow ();

        sum += AddKernelRow (iterator);
        iterator.IncrementByRow ();

        sum += AddKernelRow (iterator);
        iterator.IncrementByRow ();

        sum += AddKernelRow (iterator);
        iterator.IncrementByRow ();

        sum += AddKernelRow (iterator);
        iterator.IncrementByRow ();

        sum += AddKernelRow (iterator);
        iterator.IncrementByRow ();
    }

    switch (this->y_reminder)
    {
        case 7:
        {
            sum += AddKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 6:
        {
            sum += AddKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 5:
        {
            sum += AddKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 4:
        {
            sum += AddKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 3:
        {
            sum += AddKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 2:
        {
            sum += AddKernelRow (iterator);
            iterator.IncrementByRow ();
        }
        case 1:
        {
            sum += AddKernelRow (iterator);
        }
    }

    return sum / kernel_size;
}

template < typename Tsrc > inline void Kernel < Tsrc >::CorrelateKernel ()
{
    this->sum = 0.0f;
    this->correlation_sum_squared = 0.0f;
    double average = ImageAverageAtKernel ();

    KernelIterator < Tsrc > iterator = this->Begin ();

    for(register int row = 0; row < this->y_max_block_size; row += BLOCKSIZE)
    {
        CorrelateKernelRow (iterator, average);
        iterator.IncrementByRow ();

        CorrelateKernelRow (iterator, average);
        iterator.IncrementByRow ();

        CorrelateKernelRow (iterator, average);
        iterator.IncrementByRow ();

        CorrelateKernelRow (iterator, average);
        iterator.IncrementByRow ();

        CorrelateKernelRow (iterator, average);
        iterator.IncrementByRow ();

        CorrelateKernelRow (iterator, average);
        iterator.IncrementByRow ();

        CorrelateKernelRow (iterator, average);
        iterator.IncrementByRow ();

        CorrelateKernelRow (iterator, average);
        iterator.IncrementByRow ();
    }

    switch (this->y_reminder)
    {
        case 7:
        {
            CorrelateKernelRow (iterator, average);
            iterator.IncrementByRow ();
        }
        case 6:
        {
            CorrelateKernelRow (iterator, average);
            iterator.IncrementByRow ();
        }
        case 5:
        {
            CorrelateKernelRow (iterator, average);
            iterator.IncrementByRow ();
        }
        case 4:
        {
            CorrelateKernelRow (iterator, average);
            iterator.IncrementByRow ();
        }
        case 3:
        {
            CorrelateKernelRow (iterator, average);
            iterator.IncrementByRow ();
        }
        case 2:
        {
            CorrelateKernelRow (iterator, average);
            iterator.IncrementByRow ();
        }
        case 1:
        {
            CorrelateKernelRow (iterator, average);
        }
    }
}

template < typename Tsrc > FIBITMAP * Kernel < Tsrc >::Correlate ()
{
    const int dst_width = src_image_width - (2 * this->xborder);
    const int dst_height = src_image_height - (2 * this->yborder);

    FIBITMAP *dst = FIA_CloneImageType (this->dib, dst_width, dst_height);

    const int dst_pitch_in_pixels = FreeImage_GetPitch (dst) / sizeof (Tsrc);

    register double *dst_ptr;

    double *dst_first_pixel_address_ptr = (Tsrc *) FreeImage_GetBits (dst);

    this->kernel_average = 0.0;
    int kernel_size = kernel_width * kernel_height;

    for(int i = 0; i < kernel_size; i++)
    {
        this->kernel_average += this->values[i];
    }

    this->kernel_average /= kernel_size;

    double kernel_normalise_sum = 0.0;

    for(int i = 0; i < kernel_size; i++)
    {
        kernel_normalise_sum += ((this->values[i] - this->kernel_average) * 
				(this->values[i] - this->kernel_average));
    }

	register int search_bottom = 0;
	int search_top = dst_height;
	int search_left = 0;
	int search_right = dst_width;
	
	FIARECT rect = FIAImageRect(dst);
	
	if(!FIARectIsEmpty(this->search_area))
		rect = this->search_area;
	
	// Passed rectangle is relative to top left
	// as that is what clients usually expect
	rect = FIA_MakeFiaRectRelativeToImageBottomLeft (dst, rect);
	
	if(rect.top > dst_height)
		rect.top = dst_height;
			
	if(rect.left < 0)
		rect.left = 0;
			
	if(rect.right > dst_width)
		rect.right = dst_width;
			
	if(rect.bottom < 0)
		rect.bottom = 0;

	if(this->mask != NULL)
	{
		BYTE *mask_ptr = NULL;
	
		for(register int y = rect.bottom; y < rect.top; y++)
		{
			this->Move (rect.left, y);
			dst_ptr = (double *) FreeImage_GetScanLine (dst, y);
			mask_ptr = (BYTE *) FreeImage_GetScanLine (this->mask, y);

			for(register int x = rect.left; x < rect.right; x++)
			{
				if(mask_ptr[x] == 0) {
					this->Increment ();
					continue;
				}
				
				this->CorrelateKernel ();
				double dominator = sqrt (this->correlation_sum_squared * kernel_normalise_sum);

				dst_ptr[x] = this->sum / dominator;
				this->Increment ();
			}
		}
    }
    else
    {
		for(register int y = rect.bottom; y < rect.top; y++)
		{
			this->Move (rect.left, y);
			dst_ptr = (double *) FreeImage_GetScanLine (dst, y);

			for(register int x = rect.left; x < rect.right; x++)
			{
				this->CorrelateKernel ();
				double dominator = sqrt (this->correlation_sum_squared * kernel_normalise_sum);

				dst_ptr[x] = this->sum / dominator;
				this->Increment ();
			}
		}
    }

    return dst;
}

#endif
