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

#include "Constants.h"

FilterKernel DLL_CALLCONV
FIA_NewKernel(int x_radius, int y_radius, const double *values, double divider)
{
    FilterKernel kernel;

    kernel.x_radius = x_radius;
    kernel.y_radius = y_radius;
    kernel.values = values;
    kernel.divider = divider;

    return kernel;
}

FIBITMAP *
DLL_CALLCONV
FIA_Convolve(FIABITMAP * src, FilterKernel kernel)
{
    FIBITMAP *dst = NULL;
    FIABITMAP border_tmp;

    if (!src)
    {
        return NULL;
    }

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if (src_type == FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Error can not perform convolution on a complex image");
        return NULL;
    }

    border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);

    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;

    Kernel<double>*kern = new Kernel<double> (&border_tmp, kernel.x_radius,
            kernel.y_radius, kernel.values, kernel.divider);

    dst = kern->Convolve();

    FreeImage_Unload(border_tmp.fib);

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc(
                FIF_UNKNOWN,
                "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                src_type, FIT_BITMAP);
    }

    delete kern;

    return dst;
}

static FIBITMAP *
DLL_CALLCONV
FIA_Correlate(FIABITMAP * src, FilterKernel kernel, FIARECT search_area, FIBITMAP *mask)
{
    FIBITMAP *dst = NULL;
    FIABITMAP border_tmp;

    if (!src)
    {
        return NULL;
    }

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if (src_type == FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Error can not perform convolution on a complex image");
        return NULL;
    }

    border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;

    Kernel<double>*kern = new Kernel<double> (&border_tmp, kernel.x_radius,
            kernel.y_radius, kernel.values, kernel.divider);

	kern->SetSearchArea(search_area);
	kern->SetMask(mask);

    dst = kern->Correlate();

    FreeImage_Unload(border_tmp.fib);

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc(
                FIF_UNKNOWN,
                "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                src_type, FIT_BITMAP);
    }

    delete kern;

    return dst;
}

FIBITMAP *
DLL_CALLCONV
FIA_SeparableConvolve(FIABITMAP * src, FilterKernel horz_kernel,
        FilterKernel vert_kernel)
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

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if (src_type == FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Error can not perform convolution on a complex image");
        return NULL;
    }

    if (src_type == FIT_DOUBLE)
    {
        border_tmp.fib = FreeImage_Clone(src->fib);
    }
    else
    {
        border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    }

    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;

    Kernel<double>*kern1 = new Kernel<double> (&border_tmp,
            horz_kernel.x_radius, horz_kernel.y_radius, horz_kernel.values,
            horz_kernel.divider);

    tmp_dst = kern1->Convolve();

    tmp_border = FIA_SetZeroBorder(tmp_dst, xborder, yborder);

    Kernel<double>*kern2 = new Kernel<double> (tmp_border,
            vert_kernel.x_radius, vert_kernel.y_radius, vert_kernel.values,
            vert_kernel.divider);

    dst = kern2->Convolve();

    FIA_Unload(tmp_border);
    FreeImage_Unload(tmp_dst);
    FreeImage_Unload(border_tmp.fib);

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc(
                FIF_UNKNOWN,
                "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                src_type, FIT_BITMAP);
    }

    return dst;
}

static int DLL_CALLCONV
FIA_NewKernelFromImage(FIBITMAP * src, FilterKernel * kernel)
{
    FIBITMAP *fib = NULL;
    int dimension_changed = 0;
    int width = FreeImage_GetWidth(src);
    int height = FreeImage_GetHeight(src);
	int new_width=width, new_height=height;

    if (width % 2 == 0)
    {
        // We need an odd sized image
        new_width = width - 1;
        dimension_changed = 1;
    }

    if (height % 2 == 0)
    {
        new_height = height - 1;
        dimension_changed = 1;
    }

    if (dimension_changed)
    {
        fib = FreeImage_Copy(src, 0, 0, new_width, new_height);
        width = FreeImage_GetWidth(fib);
        height = FreeImage_GetHeight(fib);
    }
    else
    {
        fib = FreeImage_Clone(src);
    }

    kernel->x_radius = width / 2;
    kernel->y_radius = height / 2;

    double *values = (double *) malloc(sizeof(double) * width * height);

    kernel->divider = 1.0;

	//int bpp = FreeImage_GetBPP(fib);
	//int ty = FreeImage_GetImageType(fib);

    FIBITMAP *double_dib = FreeImage_ConvertToType(fib, FIT_DOUBLE, 1);
    double *ptr = NULL;
    int i = 0;

    for (register int y = 0; y < height; y++)
    {
        ptr = (double *) FreeImage_GetScanLine(double_dib, y);

        for (register int x = 0; x < width; x++)
        {
            values[i++] = (double) ptr[x];
        }
    }

    FreeImage_Unload(double_dib);
    FreeImage_Unload(fib);

    kernel->values = values;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_KernelCorrelateImages(FIBITMAP * _src1, FIBITMAP * _src2, FIARECT search_area, FIBITMAP *mask,
        CORRELATION_PREFILTER filter, FIAPOINT * pt, double *max)
{
    FilterKernel kernel;
    FIABITMAP *tmp = NULL;

	*max = 0.0;

    pt->x = 0;
    pt->y = 0;

    FIBITMAP *src1 = FreeImage_Clone(_src1);
    FIBITMAP *src2 = FreeImage_Clone(_src2);

    if (src1 == NULL || src2 == NULL)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Conversion to standard image falied");
        
		goto CLEANUP;
    }

    unsigned int src1_width = FreeImage_GetWidth(src1);
    unsigned int src1_height = FreeImage_GetHeight(src1);
    unsigned int src2_width = FreeImage_GetWidth(src2);
    unsigned int src2_height = FreeImage_GetHeight(src2);

    if (src2_width > src1_width || src2_height > src1_height)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "_src2 image must be smaller or equal than src1");
        
		goto CLEANUP;
    }

    FREE_IMAGE_TYPE src1_type = FreeImage_GetImageType(src1);
    FREE_IMAGE_TYPE src2_type = FreeImage_GetImageType(src2);

    if (src1_type != src2_type)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Images must be of the same type");
        
		goto CLEANUP;
    }

    if (src1_type == FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Error can not perform correlation on a complex image");
        
		goto CLEANUP;
    }

    int bpp1 = FreeImage_GetBPP(src1);
    int bpp2 = FreeImage_GetBPP(src2);

    if (bpp1 != bpp2)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Images must have the same bpp");
        
		goto CLEANUP;
    }

    // Convert colour images to greyscale
    if (bpp1 >= 24 && src1_type == FIT_BITMAP)
    {
        FreeImage_Unload(src1);
        FreeImage_Unload(src2);

        src1 = FreeImage_ConvertToGreyscale(_src1);
        src2 = FreeImage_ConvertToGreyscale(_src2);
    }

    FIBITMAP *filtered_src1 = NULL;
    FIBITMAP *filtered_src2 = NULL;

    if (filter != NULL)
    {
        if ((filtered_src1 = filter(src1)) == NULL)
        {
            FreeImage_OutputMessageProc(FIF_UNKNOWN,
                    "Filter function returned NULL");
            
			goto CLEANUP;
        }

        if ((filtered_src2 = filter(src2)) == FIA_ERROR)
        {
            FreeImage_OutputMessageProc(FIF_UNKNOWN,
                    "Filter function returned NULL");
            
			goto CLEANUP;
        }
    }
    else
    {
        filtered_src1 = FreeImage_Clone(src1);
        filtered_src2 = FreeImage_Clone(src2);
    }

    int filtered_src1_width = FreeImage_GetWidth(filtered_src1);
    int filtered_src1_height = FreeImage_GetHeight(filtered_src1);

    if (filtered_src1_width != src1_width || filtered_src1_height
            != src1_height)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Filter function has changed the size of the source input");

        goto CLEANUP;
    }

	if(mask != NULL) {

		if (FreeImage_GetImageType (mask) != FIT_BITMAP)
		{
			FreeImage_OutputMessageProc (FIF_UNKNOWN,
										 "mask is not a FIT_BITMAP image");
			goto CLEANUP;
		}
	
		if(FIA_CheckSizesAreSame(filtered_src1, mask) == 0)
		{
			FreeImage_OutputMessageProc (FIF_UNKNOWN, "Background image and mask image are not the same size");

			goto CLEANUP;
		}
	}

    kernel.x_radius = 0;
    kernel.y_radius = 0;
    kernel.values = NULL;

    if (FIA_NewKernelFromImage(filtered_src2, &kernel) == FIA_ERROR)
    {
        goto CLEANUP;
    }

    tmp = FIA_SetZeroBorder(filtered_src1, kernel.x_radius, kernel.y_radius);

    FIBITMAP *dib = FIA_Correlate(tmp, kernel, search_area, mask);

	if(dib == NULL)
	{
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "FIA_Correlate returned NULL");

        goto CLEANUP;
    }

    double found_max = 0.0;

    FIA_FindMaxXY(dib, &found_max, pt);

    if (max != NULL)
        *max = found_max;

    pt->x -= kernel.x_radius;
    pt->y += kernel.y_radius;

    int height = src1_height;

    pt->y = height - pt->y - 1;

	if(src1 != NULL)
		FreeImage_Unload(src1);

	if(src2 != NULL)
		FreeImage_Unload(src2);

	if(filtered_src1 != NULL)
		FreeImage_Unload(filtered_src1);

	if(filtered_src2 != NULL)
		FreeImage_Unload(filtered_src2);

	if(dib != NULL)
		FreeImage_Unload(dib);

	if(tmp != NULL)
		FIA_Unload(tmp);

	if(kernel.values != NULL)
		free((void *) kernel.values);

    return FIA_SUCCESS;

CLEANUP:

	if(src1 != NULL)
		FreeImage_Unload(src1);

	if(src2 != NULL)
		FreeImage_Unload(src2);

    if(filtered_src1 != NULL)
		FreeImage_Unload(filtered_src1);

	if(filtered_src2 != NULL)
		FreeImage_Unload(filtered_src2);

	if(dib != NULL)
		FreeImage_Unload(dib);

	if(tmp != NULL)
		FIA_Unload(tmp);

	if(kernel.values != NULL)
		free((void *) kernel.values);

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_KernelCorrelateImageRegions(FIBITMAP * src1, FIARECT rect1,
        FIBITMAP * src2, FIARECT rect2, FIARECT search_rect, FIBITMAP *mask, CORRELATION_PREFILTER filter,
        FIAPOINT * pt, double *max)
{
    FIBITMAP *src1_rgn = FIA_Copy(src1, rect1.left, rect1.top, rect1.right,
            rect1.bottom);
    FIBITMAP *src2_rgn = FIA_Copy(src2, rect2.left, rect2.top, rect2.right,
            rect2.bottom);

	*max = 0.0;

    pt->x = 0;
    pt->y = 0;

    if (src1_rgn == NULL || src2_rgn == NULL)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "NULL values passed");
        return FIA_ERROR;
    }

	// Search_rect is relative to the whole of src1
	// we need to make it relative to the src1 region
	search_rect.left -= rect1.left;
	search_rect.top -= rect1.top;
	search_rect.right -= rect1.left;
	search_rect.bottom -= rect1.top;

    int err = FIA_KernelCorrelateImages(src1_rgn, src2_rgn, search_rect, mask, filter, pt, max);

    if (err == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    // Add the point found to the start of the region searched
    if (src1 == src2)
    { // Images are the same image
        pt->x += rect1.left;
        pt->y += rect1.top;
    }
    else
    {

        // Images are different so we need to adjust for the selected region in both images
        pt->x = pt->x - rect2.left + rect1.left;
        pt->y = pt->y - rect2.top + rect1.top;
    }

    FreeImage_Unload(src1_rgn);
    FreeImage_Unload(src2_rgn);

    if (err == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    return FIA_SUCCESS;
}

FIBITMAP *
FIA_EdgeDetect(FIBITMAP * src)
{
    double kernel[] =
        { -1.0 / 8.0, -1.0 / 8.0, -1.0 / 8.0, -1.0 / 8.0, 1.0, -1.0 / 8.0, -1.0
                / 8.0, -1.0 / 8.0, -1.0 / 8.0 };

    FIABITMAP *src_bordered = FIA_SetBorder(src, 1, 1, BorderType_Copy, 0.0);

    FilterKernel convolve_kernel = FIA_NewKernel(1, 1, kernel, 1.0);

    FIBITMAP *dib = FIA_Convolve(src_bordered, convolve_kernel);

    FIA_Unload(src_bordered);

    return dib;
}

static FIBITMAP *
PadImage(FIBITMAP * src, int padded_width_size, int padded_height_size)
{
    // Must pad the images so circular convolution does not take place.
    int width = FreeImage_GetWidth(src);
    int height = FreeImage_GetHeight(src);

    assert (padded_width_size > width);
    assert (padded_height_size > height);

    FIBITMAP *border_src = FreeImage_Allocate(padded_width_size,
            padded_height_size, 8, 0, 0, 0);

    FIA_SetGreyLevelPalette(border_src);

    FreeImage_Paste(border_src, src, 0, 0, 256);

    return border_src;
}

int DLL_CALLCONV
FIA_FFTCorrelateImages(FIBITMAP * _src1, FIBITMAP * _src2,
        CORRELATION_PREFILTER filter, FIAPOINT * pt)
{
    FIBITMAP *src1 = FreeImage_Clone(_src1);
    FIBITMAP *src2 = FreeImage_Clone(_src2);

    pt->x = 0;
    pt->y = 0;

    FREE_IMAGE_TYPE src1_type = FreeImage_GetImageType(src1);
    FREE_IMAGE_TYPE src2_type = FreeImage_GetImageType(src2);

    if (src1_type != src2_type)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Images must be of the same type");
        return FIA_ERROR;
    }

    int bpp1 = FreeImage_GetBPP(src1);
    int bpp2 = FreeImage_GetBPP(src2);

    if (bpp1 != bpp2)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Images must have the same bpp");
        return FIA_ERROR;
    }

    // Convert colour images to greyscale
    if (bpp1 >= 24 && src1_type == FIT_BITMAP)
    {
        FIA_InPlaceConvertToGreyscale(&src1);
        FIA_InPlaceConvertToGreyscale(&src2);
    }

    FIA_InPlaceConvertToStandardType(&src1, 0);
    FIA_InPlaceConvertToStandardType(&src2, 0);

    FIBITMAP *filtered_src1 = NULL;
    FIBITMAP *filtered_src2 = NULL;

    if (filter != NULL)
    {
        filtered_src1 = filter(src1);
        filtered_src2 = filter(src2);

        if (filtered_src1 == NULL || filtered_src2 == NULL)
        {
            FreeImage_OutputMessageProc(FIF_UNKNOWN,
                    "Filter function returned NULL");
            return FIA_ERROR;
        }
    }
    else
    {
        filtered_src1 = FreeImage_Clone(src1);
        filtered_src2 = FreeImage_Clone(src2);
    }

    FIA_InPlaceConvertToStandardType(&filtered_src1, 0);
    FIA_InPlaceConvertToStandardType(&filtered_src2, 0);

#ifdef GENERATE_DEBUG_IMAGES
    FIA_SaveFIBToFile(filtered_src1,  DEBUG_DATA_DIR "fft-prefiltered1.png", BIT8);
    FIA_SaveFIBToFile(filtered_src2,  DEBUG_DATA_DIR "fft-prefiltered2.png", BIT8);
#endif

    int src1_width = FreeImage_GetWidth(src1);
    int src1_height = FreeImage_GetHeight(src1);
    int filtered_src1_width = FreeImage_GetWidth(filtered_src1);
    int filtered_src1_height = FreeImage_GetHeight(filtered_src1);

    if (filtered_src1_width != src1_width && filtered_src1_height
            != src1_height)
    {
        FreeImage_OutputMessageProc(
                FIF_UNKNOWN,
                "Filter function has changed the size of the source input from %d,%d to %d,%d",
                src1_width, src1_height, filtered_src1_width,
                filtered_src1_height);

        return FIA_ERROR;
    }

    // Find the padded width and height.
    int pad_width = kiss_fft_next_fast_size(src1_width + FreeImage_GetWidth(
            filtered_src2) + 1);
    int pad_height = kiss_fft_next_fast_size(src1_height + FreeImage_GetHeight(
            filtered_src2) + 1);

    FIBITMAP *border_src1 = PadImage(filtered_src1, pad_width, pad_height);
    FIBITMAP *border_src2 = PadImage(filtered_src2, pad_width, pad_height);

    FIBITMAP *fft1 = FIA_FFT(border_src1);
    FIBITMAP *fft2 = FIA_FFT(border_src2);

#ifdef GENERATE_DEBUG_IMAGES
    FIBITMAP *i = FIA_IFFT(fft1);
    FIBITMAP *r = FIA_ComplexImageToRealValued(i);
    FIA_SaveFIBToFile(FreeImage_ConvertToStandardType(r, 1),  DEBUG_DATA_DIR "FIA_FFTCorrelateImages-fft.png", BIT24);
    FreeImage_Unload(i);
    FreeImage_Unload(r);
#endif

    FIA_ComplexConjugate(fft2);

    if (FIA_MultiplyComplexImages(fft1, fft2) == FIA_ERROR)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "FIA_MultiplyComplexImages Failed");
        return FIA_ERROR;
    }

    FIBITMAP *ifft = FIA_IFFT(fft1);

    FIBITMAP *real = FIA_ComplexImageToRealValued(ifft);

#ifdef GENERATE_DEBUG_IMAGES
    FIA_SaveFIBToFile(FreeImage_ConvertToStandardType(real, 1),  DEBUG_DATA_DIR "fft.png", BIT24);
#endif

    double max;

    FIA_FindMaxXY(real, &max, pt);

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

    FreeImage_Unload(real);
    FreeImage_Unload(ifft);
    FreeImage_Unload(fft1);
    FreeImage_Unload(fft2);
    FreeImage_Unload(src1);
    FreeImage_Unload(src2);
    FreeImage_Unload(filtered_src1);
    FreeImage_Unload(filtered_src2);
    FreeImage_Unload(border_src1);
    FreeImage_Unload(border_src2);

    return FIA_SUCCESS;
}

FIBITMAP* DLL_CALLCONV
FIA_PreCalculateCorrelationFFT(FIBITMAP *_src1, FIBITMAP *_src2, int pad_size, CORRELATION_PREFILTER filter)
{
    FIBITMAP *src1 = FreeImage_Clone(_src1);
    FIBITMAP *src2 = FreeImage_Clone(_src2);

    FREE_IMAGE_TYPE src1_type = FreeImage_GetImageType(src1);
    FREE_IMAGE_TYPE src2_type = FreeImage_GetImageType(src2);

    if (src1_type != src2_type)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Images must be of the same type");
        return FIA_ERROR;
    }

    int bpp1 = FreeImage_GetBPP(src1);
    int bpp2 = FreeImage_GetBPP(src2);

    if (bpp1 != bpp2)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Images must have the same bpp");
        return FIA_ERROR;
    }

    // Convert colour images to greyscale
    if (bpp1 >= 24 && src1_type == FIT_BITMAP)
    {
        FIA_InPlaceConvertToGreyscale(&src1);
    }

    FIA_InPlaceConvertToStandardType(&src1, 0);

    FIBITMAP *filtered_src1 = NULL;

    if (filter != NULL)
    {
        filtered_src1 = filter(src1);

        if (filtered_src1 == NULL)
        {
            FreeImage_OutputMessageProc(FIF_UNKNOWN,
                    "Filter function returned NULL");
            return FIA_ERROR;
        }
    }
    else
    {
        filtered_src1 = FreeImage_Clone(src1);
    }

    FIA_InPlaceConvertToStandardType(&filtered_src1, 0);

#ifdef GENERATE_DEBUG_IMAGES
    FIA_SaveFIBToFile(filtered_src1,  DEBUG_DATA_DIR "fft-prefiltered1.png", BIT8);
#endif

    int src1_width = FreeImage_GetWidth(src1);
    int src1_height = FreeImage_GetHeight(src1);
    int filtered_src1_width = FreeImage_GetWidth(filtered_src1);
    int filtered_src1_height = FreeImage_GetHeight(filtered_src1);

    if(!FIA_CheckSizesAreSame(src1, filtered_src1))
    {
        FreeImage_OutputMessageProc(
                FIF_UNKNOWN,
                "Filter function has changed the size of the source input from %d,%d to %d,%d",
                src1_width, src1_height, filtered_src1_width,
                filtered_src1_height);

        return FIA_ERROR;
    }

    // Find the padded width and height.
    int pad_width = kiss_fft_next_fast_size(src1_width + pad_size );
    int pad_height = kiss_fft_next_fast_size(src1_height + pad_size);

    FIBITMAP *border_src1 = PadImage(filtered_src1, pad_width, pad_height);

    FIBITMAP *fft = FIA_FFT(border_src1);

    FreeImage_Unload(src1);
    FreeImage_Unload(src2);
    FreeImage_Unload(filtered_src1);
    FreeImage_Unload(border_src1);

#ifdef GENERATE_DEBUG_IMAGES
    FIBITMAP *ifft = FIA_IFFT(fft);
    FIBITMAP *real = FIA_ComplexImageToRealValued(ifft);

    FIA_SaveFIBToFile(FreeImage_ConvertToStandardType(real, 1),  DEBUG_DATA_DIR "fft-pre-generated.png", BIT24);

    FreeImage_Unload(ifft);
    FreeImage_Unload(real);

#endif

    return fft;
}

int DLL_CALLCONV
FIA_FFTCorrelateImageWithPreCorrelationFFT(FIBITMAP * fft1_fib, FIBITMAP *_src1, FIBITMAP *_src2, int pad_size,
        CORRELATION_PREFILTER filter, FIAPOINT * pt)
{
    if(FreeImage_GetImageType(fft1_fib) != FIT_COMPLEX)
        return FIA_ERROR;

    FIBITMAP *fft_fib = FreeImage_Clone(fft1_fib);
    FIBITMAP *src1 = FreeImage_Clone(_src1);
    FIBITMAP *src2 = FreeImage_Clone(_src2);

     pt->x = 0;
     pt->y = 0;

     FREE_IMAGE_TYPE src1_type = FreeImage_GetImageType(src1);
     FREE_IMAGE_TYPE src2_type = FreeImage_GetImageType(src2);

     if (src1_type != src2_type)
     {
         FreeImage_OutputMessageProc(FIF_UNKNOWN,
                 "Images must be of the same type");
         return FIA_ERROR;
     }

     int bpp1 = FreeImage_GetBPP(src1);
     int bpp2 = FreeImage_GetBPP(src2);

     if (bpp1 != bpp2)
     {
         FreeImage_OutputMessageProc(FIF_UNKNOWN,
                 "Images must have the same bpp");
         return FIA_ERROR;
     }

     // Convert colour images to greyscale
     if (bpp2 >= 24 && src2_type == FIT_BITMAP)
     {
         FIA_InPlaceConvertToGreyscale(&src2);
     }

     FIA_InPlaceConvertToStandardType(&src2, 0);

     FIBITMAP *filtered_src2 = NULL;

     if (filter != NULL)
     {
         filtered_src2 = filter(src2);

         if (filtered_src2 == NULL)
         {
             FreeImage_OutputMessageProc(FIF_UNKNOWN,
                     "Filter function returned NULL");
             return FIA_ERROR;
         }
     }
     else
     {
         filtered_src2 = FreeImage_Clone(src2);
     }

     FIA_InPlaceConvertToStandardType(&filtered_src2, 0);

 #ifdef GENERATE_DEBUG_IMAGES
     FIA_SaveFIBToFile(filtered_src2,  DEBUG_DATA_DIR "fft-prefiltered2.png", BIT8);
 #endif

     int src1_width = FreeImage_GetWidth(src1);
     int src1_height = FreeImage_GetHeight(src1);
     int filtered_src_width = FreeImage_GetWidth(filtered_src2);
     int filtered_src_height = FreeImage_GetHeight(filtered_src2);

     if(!FIA_CheckSizesAreSame(src2, filtered_src2))
     {
         FreeImage_OutputMessageProc(
                 FIF_UNKNOWN,
                 "Filter function has changed the size of the source input from %d,%d to %d,%d",
                 src1_width, src1_height, filtered_src_width,
                 filtered_src_height);

         return FIA_ERROR;
     }

     // Find the padded width and height.
     int pad_width = kiss_fft_next_fast_size(src1_width + pad_size);
     int pad_height = kiss_fft_next_fast_size(src1_height + pad_size);

     FIBITMAP *border_src2 = PadImage(filtered_src2, pad_width, pad_height);

     FIBITMAP *fft2 = FIA_FFT(border_src2);

     FIA_ComplexConjugate(fft2);

     if (FIA_MultiplyComplexImages(fft_fib, fft2) == FIA_ERROR)
     {
         FreeImage_OutputMessageProc(FIF_UNKNOWN,
                 "FIA_MultiplyComplexImages Failed");
         return FIA_ERROR;
     }

     FIBITMAP *ifft = FIA_IFFT(fft_fib);

     FIBITMAP *real = FIA_ComplexImageToRealValued(ifft);

 #ifdef GENERATE_DEBUG_IMAGES
     FIA_SaveFIBToFile(FreeImage_ConvertToStandardType(real, 1),  DEBUG_DATA_DIR "fft.png", BIT24);
 #endif

     double max;

     FIA_FindMaxXY(real, &max, pt);

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

     FreeImage_Unload(real);
     FreeImage_Unload(fft_fib);
     FreeImage_Unload(ifft);
     FreeImage_Unload(fft2);
     FreeImage_Unload(src1);
     FreeImage_Unload(src2);
     FreeImage_Unload(filtered_src2);
     FreeImage_Unload(border_src2);

     return FIA_SUCCESS;
 }

int DLL_CALLCONV
FIA_FFTCorrelateImageRegions(FIBITMAP * src1, FIARECT rect1, FIBITMAP * src2,
        FIARECT rect2, CORRELATION_PREFILTER filter, FIAPOINT * pt)
{
    FIBITMAP *src1_rgn = FIA_Copy(src1, rect1.left, rect1.top, rect1.right,
            rect1.bottom);
    FIBITMAP *src2_rgn = FIA_Copy(src2, rect2.left, rect2.top, rect2.right,
            rect2.bottom);

    pt->x = 0;
    pt->y = 0;

    if (src1_rgn == NULL || src2_rgn == NULL)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "NULL values passed");
        return FIA_ERROR;
    }

    int err = FIA_FFTCorrelateImages(src1_rgn, src2_rgn, filter, pt);

    if (err == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    // Add the point found to the start of the region searched
    if (src1 == src2)
    { // Images are the same image
        pt->x += rect1.left;
        pt->y += rect1.top;
    }
    else
    {

        // Images are different so we need to adjust for the selected region in both images
        pt->x = pt->x - rect2.left + rect1.left;
        pt->y = pt->y - rect2.top + rect1.top;
    }

    FreeImage_Unload(src1_rgn);
    FreeImage_Unload(src2_rgn);

    if (err == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    return FIA_SUCCESS;
}
