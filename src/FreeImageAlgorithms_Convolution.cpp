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
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution.txx"
#include "FreeImageAlgorithms_Statistics.h"

#include "FreeImageAlgorithms_IO.h"

#include <math.h>
#include <iostream>

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

FIBITMAP* DLL_CALLCONV
FIA_Convolve(FIABITMAP *src, FilterKernel kernel)
{
    FIBITMAP *dst= NULL;
    FIABITMAP border_tmp;
    
    if (!src) {
        return NULL;
    }
    
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);
    
    if (src_type == FIT_COMPLEX) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Error can not perform convolution on a complex image");
        return NULL;
    }
    
    border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;
    
    Kernel<double> *kern = new Kernel<double>(&border_tmp, kernel.x_radius,
            kernel.y_radius, kernel.values, kernel.divider);
    
    dst = kern->Convolve();
    
    FreeImage_Unload(border_tmp.fib);
    
    if (NULL == dst) {
        FreeImage_OutputMessageProc(
                FIF_UNKNOWN,
                "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                src_type, FIT_BITMAP);
    }
    
    delete kern;
    
    return dst;
}

static FIBITMAP* DLL_CALLCONV
FIA_Correlate(FIABITMAP *src, FilterKernel kernel)
{
    FIBITMAP *dst= NULL;
    FIABITMAP border_tmp;
    
    if (!src) {
        return NULL;
    }
    
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);
    
    if (src_type == FIT_COMPLEX) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Error can not perform convolution on a complex image");
        return NULL;
    }
    
    border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;
    
    Kernel<double> *kern = new Kernel<double>(&border_tmp, kernel.x_radius,
            kernel.y_radius, kernel.values, kernel.divider);
    
    dst = kern->Correlate();
    
    FreeImage_Unload(border_tmp.fib);
    
    if (NULL == dst) {
        FreeImage_OutputMessageProc(
                FIF_UNKNOWN,
                "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                src_type, FIT_BITMAP);
    }
    
    delete kern;
    
    return dst;
}

FIBITMAP* DLL_CALLCONV
FIA_SeparableConvolve(FIABITMAP *src, FilterKernel horz_kernel, FilterKernel vert_kernel)
{
    FIBITMAP *tmp_dst= NULL, *dst= NULL;
    FIABITMAP *tmp_border= NULL;
    FIABITMAP border_tmp;
    int xborder = src->xborder;
    int yborder = src->yborder;
    
    if (!src) {
        return NULL;
    }
    
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);
    
    if (src_type == FIT_COMPLEX) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Error can not perform convolution on a complex image");
        return NULL;
    }
    
    if (src_type == FIT_DOUBLE) {
        border_tmp.fib = FreeImage_Clone(src->fib);
    } else {
        border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    }
    
    border_tmp.xborder = src->xborder;
    border_tmp.yborder = src->yborder;
    
    Kernel<double> *kern1 = new Kernel<double>(&border_tmp, horz_kernel.x_radius,
            horz_kernel.y_radius, horz_kernel.values, horz_kernel.divider);
    
    tmp_dst = kern1->Convolve();
    
    tmp_border = FIA_SetZeroBorder(tmp_dst, xborder, yborder);
    
    Kernel<double> *kern2 = new Kernel<double>(tmp_border, vert_kernel.x_radius,
            vert_kernel.y_radius, vert_kernel.values, vert_kernel.divider);
    
    dst = kern2->Convolve();
    
    FIA_Unload(tmp_border);
    FreeImage_Unload(tmp_dst);
    FreeImage_Unload(border_tmp.fib);
    
    if (NULL == dst) {
        FreeImage_OutputMessageProc(
                FIF_UNKNOWN,
                "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                src_type, FIT_BITMAP);
    }
    
    return dst;
}

static int DLL_CALLCONV
FIA_NewKernelFromImage(FIBITMAP *src, FilterKernel *kernel)
{
    int width = FreeImage_GetWidth(src);
    int height = FreeImage_GetHeight(src);
    
    if (width % 2 == 0) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Image width must be an odd value");
        return FIA_ERROR;
    }
    
    if (height % 2 == 0) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Image height must be an odd value");
        return FIA_ERROR;
    }
    
    kernel->x_radius = width / 2;
    kernel->y_radius = height / 2;
    
    double *values = (double*) malloc(sizeof(double) * width * height);
    kernel->divider = 1.0;
    FIBITMAP *double_dib = FreeImage_ConvertToType(src, FIT_DOUBLE, 0);
    double *ptr= NULL;
    int i=0;
    
    for (register int y=0; y < height; y++) {
        
        ptr = (double *)FreeImage_GetScanLine(double_dib, y);
        
        for (register int x=0; x < width; x++) {
            values[i++] = (double) ptr[x];
        }
    }
    
    FreeImage_Unload(double_dib);
    
    kernel->values = values;
    
    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_CorrelateImages(FIBITMAP *src1, FIBITMAP *src2, FIAPOINT *pt, double *max)
{
    FilterKernel kernel;
    FIABITMAP *tmp= NULL;
    pt->x = 0;
    pt->y = 0;
    
    unsigned int src1_width = FreeImage_GetWidth(src1);
    unsigned int src1_height = FreeImage_GetHeight(src1);
    unsigned int src2_width = FreeImage_GetWidth(src2);
    unsigned int src2_height = FreeImage_GetHeight(src2);
    
    if (src2_width > src1_width || src2_height > src1_height) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Images must have the same dimensions");
        return FIA_ERROR;
    }
    
    FREE_IMAGE_TYPE src1_type = FreeImage_GetImageType(src1);
    FREE_IMAGE_TYPE src2_type = FreeImage_GetImageType(src2);
    
    if (src1_type != src2_type) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Images must be of the same type");
        return FIA_ERROR;
    }
    
    if (src1_type == FIT_COMPLEX) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Error can not perform correlation on a complex image");
        return NULL;
    }
    
    if (FreeImage_GetBPP(src1) >= 24 && src1_type == FIT_BITMAP) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Error can not perform correlation on a colour image");
        return NULL;
    }
    
    kernel.x_radius = 0;
    kernel.y_radius = 0;
    kernel.values = NULL;
    
    if (FIA_NewKernelFromImage(src2, &kernel) == FIA_ERROR) {
        return FIA_ERROR;
    }
    
    tmp = FIA_SetZeroBorder(src1, kernel.x_radius, kernel.y_radius);
    
    FIBITMAP *dib = FIA_Correlate(tmp, kernel);
    
    FIA_FindMaxXY(dib, max, pt);
    
    pt->x -= kernel.x_radius;
    pt->y += kernel.y_radius;
    
    int height = src1_height;
    pt->y = height - pt->y - 1;
    
    FreeImage_Unload(dib);
    FIA_Unload(tmp);
    free((void*)kernel.values);
    
    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_CorrelateImageRegions(FIBITMAP *src1, FIARECT rect1, FIBITMAP *src2, FIARECT rect2,
        FIAPOINT *pt, double *max)
{
    FIBITMAP *src1_rgn = FreeImage_Copy(src1, rect1.left, rect1.top, rect1.right, rect1.bottom);
    FIBITMAP *src2_rgn = FreeImage_Copy(src2, rect2.left, rect2.top, rect2.right, rect2.bottom);
    
    if (src1_rgn == NULL || src2_rgn == NULL) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "NULL values passed");
        return FIA_ERROR;
    }
    
    int err = FIA_CorrelateImages(src1_rgn, src2_rgn, pt, max);
    
    // Add the point found to the start of the region searched
    pt->x += rect1.left;
    pt->y += rect1.top;
    
    FreeImage_Unload(src1_rgn);
    FreeImage_Unload(src2_rgn);
    
    if (err == FIA_ERROR) {
        return FIA_ERROR;
    }
    
    return FIA_SUCCESS;
}
