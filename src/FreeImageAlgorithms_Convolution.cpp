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
#include "FreeImageAlgorithms_Filters.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution.txx"
#include "FreeImageAlgorithms_Statistics.h"

#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_FFT.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_LinearScale.h"

#include "kiss_fft.h"

#include <math.h>
#include <iostream>

FilterKernel DLL_CALLCONV
FIA_NewKernel (int x_radius, int y_radius, const double *values, double divider)
{
    FilterKernel kernel;

    kernel.x_radius = x_radius;
    kernel.y_radius = y_radius;
    kernel.values = values;
    kernel.divider = divider;

    return kernel;
}

FIBITMAP *DLL_CALLCONV
FIA_Convolve (FIABITMAP * src, FilterKernel kernel)
{
    FIBITMAP *dst = NULL;
    FIABITMAP border_tmp;

    if (!src)
    {
        return NULL;
    }

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src->fib);

    if (src_type == FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Error can not perform convolution on a complex image");
        return NULL;
    }

    border_tmp.fib = FIA_ConvertToGreyscaleFloatType (src->fib, FIT_DOUBLE);

    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;

    Kernel < double >*kern = new Kernel < double >(&border_tmp, kernel.x_radius,
                                                   kernel.y_radius, kernel.values, kernel.divider);

    dst = kern->Convolve ();

    FreeImage_Unload (border_tmp.fib);

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     src_type, FIT_BITMAP);
    }

    delete kern;

    return dst;
}

static FIBITMAP *DLL_CALLCONV
FIA_Correlate (FIABITMAP * src, FilterKernel kernel)
{
    FIBITMAP *dst = NULL;
    FIABITMAP border_tmp;

    if (!src)
    {
        return NULL;
    }

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src->fib);

    if (src_type == FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Error can not perform convolution on a complex image");
        return NULL;
    }

    border_tmp.fib = FIA_ConvertToGreyscaleFloatType (src->fib, FIT_DOUBLE);
    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;

    Kernel < double >*kern = new Kernel < double >(&border_tmp, kernel.x_radius,
                                                   kernel.y_radius, kernel.values, kernel.divider);

    dst = kern->Correlate ();

    FreeImage_Unload (border_tmp.fib);

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     src_type, FIT_BITMAP);
    }

    delete kern;

    return dst;
}

FIBITMAP *DLL_CALLCONV
FIA_SeparableConvolve (FIABITMAP * src, FilterKernel horz_kernel, FilterKernel vert_kernel)
{
    FIBITMAP *tmp_dst = NULL, *dst = NULL;
    FIABITMAP *tmp_border = NULL;
    FIABITMAP border_tmp;
    int xborder = src->xborder;
    int yborder = src->yborder;

    if (!src)
    {
        return NULL;
    }

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src->fib);

    if (src_type == FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Error can not perform convolution on a complex image");
        return NULL;
    }

    if (src_type == FIT_DOUBLE)
    {
        border_tmp.fib = FreeImage_Clone (src->fib);
    }
    else
    {
        border_tmp.fib = FIA_ConvertToGreyscaleFloatType (src->fib, FIT_DOUBLE);
    }

    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;

    Kernel < double >*kern1 = new Kernel < double >(&border_tmp, horz_kernel.x_radius,
                                                    horz_kernel.y_radius, horz_kernel.values,
                                                    horz_kernel.divider);

    tmp_dst = kern1->Convolve ();

    tmp_border = FIA_SetZeroBorder (tmp_dst, xborder, yborder);

    Kernel < double >*kern2 = new Kernel < double >(tmp_border, vert_kernel.x_radius,
                                                    vert_kernel.y_radius, vert_kernel.values,
                                                    vert_kernel.divider);

    dst = kern2->Convolve ();

    FIA_Unload (tmp_border);
    FreeImage_Unload (tmp_dst);
    FreeImage_Unload (border_tmp.fib);

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     src_type, FIT_BITMAP);
    }

    return dst;
}

static int DLL_CALLCONV
FIA_NewKernelFromImage (FIBITMAP * src, FilterKernel * kernel)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    if (width % 2 == 0)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Image width must be an odd value");
        return FIA_ERROR;
    }

    if (height % 2 == 0)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Image height must be an odd value");
        return FIA_ERROR;
    }

    kernel->x_radius = width / 2;
    kernel->y_radius = height / 2;

    double *values = (double *) malloc (sizeof (double) * width * height);

    kernel->divider = 1.0;
    FIBITMAP *double_dib = FreeImage_ConvertToType (src, FIT_DOUBLE, 0);
    double *ptr = NULL;
    int i = 0;

    for(register int y = 0; y < height; y++)
    {
        ptr = (double *) FreeImage_GetScanLine (double_dib, y);

        for(register int x = 0; x < width; x++)
        {
            values[i++] = (double) ptr[x];
        }
    }

    FreeImage_Unload (double_dib);

    kernel->values = values;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_CorrelateImages (FIBITMAP * _src1, FIBITMAP * _src2, FIAPOINT * pt, double *max)
{
    FilterKernel kernel;
    FIABITMAP *tmp = NULL;

    pt->x = 0;
    pt->y = 0;

    FIBITMAP *src1 = FreeImage_Clone (_src1);
    FIBITMAP *src2 = FreeImage_Clone (_src2);

    if (src1 == NULL || src2 == NULL)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Conversion to standard image falied");
        return FIA_ERROR;
    }

    unsigned int src1_width = FreeImage_GetWidth (src1);
    unsigned int src1_height = FreeImage_GetHeight (src1);
    unsigned int src2_width = FreeImage_GetWidth (src2);
    unsigned int src2_height = FreeImage_GetHeight (src2);

    if (src2_width > src1_width || src2_height > src1_height)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Images must have the same dimensions");
        return FIA_ERROR;
    }

    FREE_IMAGE_TYPE src1_type = FreeImage_GetImageType (src1);
    FREE_IMAGE_TYPE src2_type = FreeImage_GetImageType (src2);

    if (src1_type != src2_type)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Images must be of the same type");
        return FIA_ERROR;
    }

    if (src1_type == FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Error can not perform correlation on a complex image");
        return NULL;
    }

    int bpp1 = FreeImage_GetBPP (src1);
    int bpp2 = FreeImage_GetBPP (src2);

    if (bpp1 != bpp2)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Images must have the same bpp");
        return FIA_ERROR;
    }

    // Convert colour images to greyscale
    if (bpp1 >= 24 && src1_type == FIT_BITMAP)
    {
        FreeImage_Unload (src1);
        FreeImage_Unload (src2);

        src1 = FreeImage_ConvertToGreyscale (_src1);
        src2 = FreeImage_ConvertToGreyscale (_src2);
    }

    // Increase the contrast for better correlation ?
    if (bpp1 == 8)
    {
        FreeImage_AdjustContrast (src1, 100.0);
        FreeImage_AdjustContrast (src2, 100.0);
    }

    kernel.x_radius = 0;
    kernel.y_radius = 0;
    kernel.values = NULL;

    if (FIA_NewKernelFromImage (src2, &kernel) == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    tmp = FIA_SetZeroBorder (src1, kernel.x_radius, kernel.y_radius);

    FIBITMAP *dib = FIA_Correlate (tmp, kernel);

    FIA_SaveFIBToFile(dib, "/home/glenn/correlated.jpg", BIT24);

    FIA_FindMaxXY (dib, max, pt);

    pt->x -= kernel.x_radius;
    pt->y += kernel.y_radius;

    int height = src1_height;

    pt->y = height - pt->y - 1;

    FreeImage_Unload (dib);
    FIA_Unload (tmp);
    free ((void *) kernel.values);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_CorrelateImageRegions (FIBITMAP * src1, FIARECT rect1, FIBITMAP * src2, FIARECT rect2,
                           FIAPOINT * pt, double *max)
{
    FIBITMAP *src1_rgn = FreeImage_Copy (src1, rect1.left, rect1.top, rect1.right, rect1.bottom);
    FIBITMAP *src2_rgn = FreeImage_Copy (src2, rect2.left, rect2.top, rect2.right, rect2.bottom);

    pt->x = 0;
    pt->y = 0;

    if (src1_rgn == NULL || src2_rgn == NULL)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "NULL values passed");
        return FIA_ERROR;
    }

    int err = FIA_CorrelateImages (src1_rgn, src2_rgn, pt, max);

    if (err == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    // Add the point found to the start of the region searched
    pt->x += rect1.left;
    pt->y += rect1.top;

    FreeImage_Unload (src1_rgn);
    FreeImage_Unload (src2_rgn);

    if (err == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    return FIA_SUCCESS;
}

/*
int DLL_CALLCONV
FIA_CorrelateImagesAlongRightEdge(FIBITMAP *src1, FIBITMAP *src2,
                unsigned int edge_thickness, FIAPOINT *pt, double *max)
{
    FIARECT search_rect;
    FIARECT kernel_rect;

    int src1_width = FreeImage_GetWidth(src1);
    int src1_height = FreeImage_GetHeight(src1);

    if(src1_width <= 41 || src1_height <= 81) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Image too small to correlate at edge. Needs to be at least 41 * 81 pixels");
        return FIA_ERROR;
    }

    search_rect.left = src1_width - edge_thickness - 1;
    search_rect.top = 0;
    search_rect.right = src1_width;
    search_rect.bottom = src1_height;

    // Get the second image (middle left)
    kernel_rect.left = 0;
    kernel_rect.top = (src1_height / 2) - 40;
    kernel_rect.right = 41;
    kernel_rect.bottom = kernel_rect.top + 81;

    FIBITMAP *src1_rgn = FreeImage_Copy(src1, search_rect.left, search_rect.top, search_rect.right, search_rect.bottom);
    // Get the image to search for
    FIBITMAP *rgn = FreeImage_Copy(src2, kernel_rect.left, kernel_rect.top, kernel_rect.right, kernel_rect.bottom);

    if (rgn == NULL) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "NULL value returned");
        return FIA_ERROR;
    }

    if(FIA_CorrelateImages(src1_rgn, rgn, pt, max) == FIA_ERROR)
        return FIA_ERROR;

    FreeImage_Unload(rgn);
    FreeImage_Unload(src1_rgn);

    // Add the point found to the start of the region searched
    pt->x += (search_rect.left - kernel_rect.left);
    pt->y += (search_rect.top - kernel_rect.top);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_CorrelateImagesAlongBottomEdge(FIBITMAP *src1, FIBITMAP *src2,
                unsigned int edge_thickness, FIAPOINT *pt, double *max)
{
    FIARECT search_rect;
    FIARECT kernel_rect;

    int src1_width = FreeImage_GetWidth(src1);
    int src1_height = FreeImage_GetHeight(src1);

    if(src1_width <= 41 || src1_height <= 81) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Image too small to correlate at edge. Needs to be at least 41 * 81 pixels");
        return FIA_ERROR;
    }

    search_rect.left = 0;
    search_rect.top = src1_height - edge_thickness -1;
    search_rect.right = src1_width;
    search_rect.bottom = src1_height;

    // Get the second image (middle left)
    kernel_rect.left = (src1_width / 2) - 40;
    kernel_rect.top = 0;
    kernel_rect.right = kernel_rect.left + 81;
    kernel_rect.bottom = 41;

    FIBITMAP *src1_rgn = FreeImage_Copy(src1, search_rect.left, search_rect.top, search_rect.right, search_rect.bottom);
    // Get the image to search for
    FIBITMAP *rgn = FreeImage_Copy(src2, kernel_rect.left, kernel_rect.top, kernel_rect.right, kernel_rect.bottom);

    if (rgn == NULL) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "NULL value returned");
        return FIA_ERROR;
    }

    if(FIA_CorrelateImages(src1_rgn, rgn, pt, max) == FIA_ERROR)
        return FIA_ERROR;

    FreeImage_Unload(rgn);
    FreeImage_Unload(src1_rgn);

    // Add the point found to the start of the region searched
    pt->x += (search_rect.left - kernel_rect.left);
    pt->y += (search_rect.top - kernel_rect.top);

    return FIA_SUCCESS;
}
*/

FIBITMAP *__cdecl
FIA_EdgeDetect (FIBITMAP * src)
{
    double kernel[] = { -1.0 / 8.0, -1.0 / 8.0, -1.0 / 8.0,
                        -1.0 / 8.0, 1.0, -1.0 / 8.0,
                        -1.0 / 8.0, -1.0 / 8.0, -1.0 / 8.0
    };

    FIABITMAP *src_bordered = FIA_SetBorder (src, 1, 1, BorderType_Copy, 0.0);

    FilterKernel convolve_kernel = FIA_NewKernel (1, 1, kernel, 1.0);

    FIBITMAP *dib = FIA_Convolve (src_bordered, convolve_kernel);

    FIA_Unload (src_bordered);

    return dib;
}

static FIBITMAP *
PadImage (FIBITMAP * src, int padded_width_size, int padded_height_size)
{
    // Must pad the images so circular convolution does not take place.
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    assert (padded_width_size > width);
    assert (padded_height_size > height);

    FIBITMAP *border_src = FreeImage_Allocate (padded_width_size, padded_height_size, 8, 0, 0, 0);

    FIA_SetGreyLevelPalette (border_src);

    FreeImage_Paste (border_src, src, 0, 0, 256);

    return border_src;
}

int DLL_CALLCONV
FIA_CorrelateImagesFFT (FIBITMAP * _src1, FIBITMAP * _src2,
                        CORRELATION_PREFILTER filter, FIAPOINT * pt, double *max)
{
    FIBITMAP *src1 = FreeImage_Clone (_src1);
    FIBITMAP *src2 = FreeImage_Clone (_src2);

    FREE_IMAGE_TYPE src1_type = FreeImage_GetImageType (src1);
    FREE_IMAGE_TYPE src2_type = FreeImage_GetImageType (src2);

    if (src1_type != src2_type)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Images must be of the same type");
        return FIA_ERROR;
    }

    int bpp1 = FreeImage_GetBPP (src1);
    int bpp2 = FreeImage_GetBPP (src2);

    if (bpp1 != bpp2)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Images must have the same bpp");
        return FIA_ERROR;
    }

    // Convert colour images to greyscale
    if (bpp1 >= 24 && src1_type == FIT_BITMAP)
    {
        FIA_InPlaceConvertToGreyscale (&src1);
        FIA_InPlaceConvertToGreyscale (&src2);
    }

    StatisticReport report;

    FIA_InPlaceConvertToGreyscaleFloatType (&src1, FIT_DOUBLE);
    FIA_InPlaceConvertToGreyscaleFloatType (&src2, FIT_DOUBLE);

    FIA_StatisticReport (src1, &report);

    // Subtract avergae value from each pixel
    FIA_SubtractGreyLevelImageConstant (src1, report.mean);

    FIA_StatisticReport (src2, &report);
    FIA_SubtractGreyLevelImageConstant (src2, report.mean);

    FIA_InPlaceConvertToStandardType (&src1, 0);
    FIA_InPlaceConvertToStandardType (&src2, 0);

    double found_min, found_max;

    FIBITMAP *filtered_src1 = NULL;
    FIBITMAP *filtered_src2 = NULL;

    if (filter != NULL)
    {
        if ((filtered_src1 = filter (src1)) == NULL)
        {
            FreeImage_OutputMessageProc (FIF_UNKNOWN, "Filter function returned NULL");
            return FIA_ERROR;
        }

        if ((filtered_src2 = filter (src2)) == FIA_ERROR)
        {
            FreeImage_OutputMessageProc (FIF_UNKNOWN, "Filter function returned NULL");
            return FIA_ERROR;
        }
    }
    else
    {
        filtered_src1 = FreeImage_Clone (src1);
        filtered_src2 = FreeImage_Clone (src2);
    }

    FIA_FindMinMax (filtered_src1, &found_min, &found_max);

    FIA_InPlaceConvertToStandardType (&filtered_src1, 0);
    FIA_InPlaceConvertToStandardType (&filtered_src2, 0);

    int src1_width = FreeImage_GetWidth (src1);
    int src1_height = FreeImage_GetHeight (src1);
    int filtered_src1_width = FreeImage_GetWidth (filtered_src1);

    if (filtered_src1_width != src1_width)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Filter function has changed the size of the source input");
        return FIA_ERROR;
    }

    // Find the padded width and height.
    int pad_width = kiss_fft_next_fast_size (src1_width + FreeImage_GetWidth (filtered_src2) + 1);
    int pad_height = kiss_fft_next_fast_size (src1_height + FreeImage_GetHeight (filtered_src2) + 1);

    FIBITMAP *border_src1 = PadImage (filtered_src1, pad_width, pad_height);
    FIBITMAP *border_src2 = PadImage (filtered_src2, pad_width, pad_height);

    FIBITMAP *fft1 = FIA_FFT (border_src1);
    FIBITMAP *fft2 = FIA_FFT (border_src2);

    FIA_ComplexConjugate (fft2);

    if (FIA_MultiplyComplexImages (fft1, fft2) == FIA_ERROR)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_MultiplyComplexImages Failed");
        return FIA_ERROR;
    }

    FIBITMAP *ifft = FIA_IFFT (fft1);

    FIBITMAP *real = FIA_ComplexImageToRealValued (ifft);

    FIA_FindMaxXY (real, max, pt);

    if (pt->x > src1_width)
    {
        pt->x = pt->x - pad_width;
    }

    // FIBITMAPS start 0 at bottom row
    pt->y = pad_height - pt->y - 1;

    if (pt->y > src1_height)
    {
        pt->y = pt->y - pad_height;
    }

    FreeImage_Unload (real);
    FreeImage_Unload (ifft);
    FreeImage_Unload (fft1);
    FreeImage_Unload (fft2);
    FreeImage_Unload (src1);
    FreeImage_Unload (src2);
    FreeImage_Unload (filtered_src1);
    FreeImage_Unload (filtered_src2);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_FFTCorrelateImagesAlongRightEdge (FIBITMAP * src1, FIBITMAP * src2,
                                      CORRELATION_PREFILTER filter, unsigned int edge_thickness,
                                      FIAPOINT * pt)
{
    FIARECT left_rect, right_rect;

    int src1_width = FreeImage_GetWidth (src1);
    int src1_height = FreeImage_GetHeight (src1);

    left_rect.left = src1_width - edge_thickness;
    left_rect.top = 0;
    left_rect.right = src1_width;
    left_rect.bottom = src1_height;

    // Get the second image edge
    right_rect.left = 0;
    right_rect.top = 0;
    right_rect.right = edge_thickness;
    right_rect.bottom = FreeImage_GetHeight (src2);

    FIBITMAP *src1_rgn =
        FreeImage_Copy (src1, left_rect.left, left_rect.top, left_rect.right, left_rect.bottom);
    // Get the image to search for
    FIBITMAP *rgn =
        FreeImage_Copy (src2, right_rect.left, right_rect.top, right_rect.right, right_rect.bottom);

    if (rgn == NULL)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "NULL value returned");
        return FIA_ERROR;
    }

    double max;

    if (FIA_CorrelateImagesFFT (src1_rgn, rgn, filter, pt, &max) == FIA_ERROR)
        return FIA_ERROR;

    // Add the point found to the start of the region searched
    pt->x += left_rect.left;
    pt->y += left_rect.top;

    FreeImage_Unload (rgn);
    FreeImage_Unload (src1_rgn);

    return FIA_SUCCESS;
}
