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
#include "FreeImageAlgorithms_Error.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution.txx"

#include "FreeImageAlgorithms_IO.h"

#include <math.h>

FilterKernel DLL_CALLCONV
FIA_NewKernel(int x_radius, int y_radius,
							  const double *values, double divider)
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
	FIBITMAP *dst = NULL;
	FIABITMAP border_tmp;

	if(!src)
		return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if(src_type == FIT_COMPLEX) {
        FIA_SendOutputMessage("Error can not perform convolution on a complex image");
        return NULL;
    }

    border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    border_tmp.xborder = src->xborder;
	border_tmp.yborder = src->yborder;

	Kernel<double> *kern = new Kernel<double>(&border_tmp, kernel.x_radius,
					kernel.y_radius, kernel.values, kernel.divider);

	dst = kern->Convolve();

	FreeImage_Unload(border_tmp.fib);

	if(NULL == dst) {
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
	FIBITMAP *dst = NULL;
	FIABITMAP border_tmp;

	if(!src)
		return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if(src_type == FIT_COMPLEX) {
        FIA_SendOutputMessage("Error can not perform convolution on a complex image");
        return NULL;
    }

    border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    border_tmp.xborder = src->xborder;
	border_tmp.yborder = src->yborder;

	Kernel<double> *kern = new Kernel<double>(&border_tmp, kernel.x_radius,
					kernel.y_radius, kernel.values, kernel.divider);

	dst = kern->Correlate();

	FreeImage_Unload(border_tmp.fib);

	if(NULL == dst) {
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
	FIBITMAP *tmp_dst = NULL, *dst = NULL;
	FIABITMAP *tmp_border = NULL;
	FIABITMAP border_tmp;
    int xborder = src->xborder;
    int yborder = src->yborder;

    if(!src)
		return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if(src_type == FIT_COMPLEX) {
        FIA_SendOutputMessage("Error can not perform convolution on a complex image");
        return NULL;
    }

    if(src_type == FIT_DOUBLE)
        border_tmp.fib = FreeImage_Clone(src->fib);
    else
        border_tmp.fib = FIA_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    
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

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN,
            "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", src_type, FIT_BITMAP);
	}

	return dst;
}


static int DLL_CALLCONV
FIA_NewKernelFromImage(FIBITMAP *src, FilterKernel *kernel)
{
    int width = FreeImage_GetWidth(src);
    int height = FreeImage_GetHeight(src);
    
    if(width % 2 == 0) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Image width must be an odd value");
        return FREEIMAGE_ALGORITHMS_ERROR;
    }
    
    if(height % 2 == 0) {
        FreeImage_OutputMessageProc(FIF_UNKNOWN, "Image height must be an odd value");
        return FREEIMAGE_ALGORITHMS_ERROR;
    }

	kernel->x_radius = width / 2;
	kernel->y_radius = height / 2;
	
	double *values = (double*) malloc(sizeof(double) * width * height);
	kernel->divider = 1.0;
    FIBITMAP *double_dib = FreeImage_ConvertToType(src, FIT_DOUBLE, 0);
    double *ptr = NULL;
    int i=0;

    for(register int y=0; y < height; y++) { 

	    ptr = (double *)FreeImage_GetScanLine(double_dib, y);

	    for(register int x=0; x < width; x++) {
            values[i++] = (double) ptr[x];
        }
	}

    FreeImage_Unload(double_dib);

    kernel->values = values;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


FIAPOINT DLL_CALLCONV
FIA_CorrelateImages(FIBITMAP *src1, FIARECT rect, FIBITMAP *src2)
{
    FilterKernel kernel;
    FIABITMAP *tmp = NULL;
    FIAPOINT pt;
    double max;
    pt.x = 0;
    pt.y = 0;
    
    kernel.x_radius = 0;
    kernel.y_radius = 0;
    kernel.values = NULL;
    
	FIA_NewKernelFromImage(src2, &kernel);
	
	printf("radius x: %d radius y: %d\n", kernel.x_radius, kernel.y_radius);
	
    tmp = FIA_SetZeroBorder(src1, kernel.x_radius, kernel.y_radius);

    FIBITMAP *dib = FIA_Correlate(tmp, kernel);

    FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);
    printf("Type %d\n", type);
    FIBITMAP *t = FreeImage_ConvertToStandardType(dib, 1);

    FIA_SaveFIBToFile(t, "/home/glenn/correlated.jpg", BIT8);
    
    FIA_FindMaxXY(dib, &max, &pt);

    pt.x -= kernel.x_radius;
    pt.y += kernel.y_radius;

    int height = FreeImage_GetHeight(src1);
    pt.y = height - pt.y - 1;

    FreeImage_Unload(dib);
    FIA_Unload(tmp);
    free((void*)kernel.values);

	return pt;
}
