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
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_Convolution_Private.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <math.h>

FilterKernel DLL_CALLCONV
FreeImageAlgorithms_NewKernel(int x_radius, int y_radius,
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
FreeImageAlgorithms_Convolve(FIABITMAP *src, FilterKernel kernel)
{
	FIBITMAP *dst = NULL;
	FIABITMAP *tmp = NULL;
	FIABITMAP border_tmp;

	if(!src)
		return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if(src_type == FIT_COMPLEX)
        return NULL;

    border_tmp.fib = FreeImageAlgorithms_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
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


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_SeparableConvolve(FIABITMAP *src, FilterKernel horz_kernel, FilterKernel vert_kernel)
{
	FIBITMAP *tmp_dst = NULL, *dst = NULL;
	FIABITMAP *tmp = NULL, *tmp_border = NULL;
	FIABITMAP border_tmp;
    int xborder = src->xborder;
    int yborder = src->yborder;

    if(!src)
		return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src->fib);

    if(src_type == FIT_COMPLEX)
        return NULL;

    if(src_type == FIT_DOUBLE)
        border_tmp.fib = FreeImage_Clone(src->fib);
    else
        border_tmp.fib = FreeImageAlgorithms_ConvertToGreyscaleFloatType(src->fib, FIT_DOUBLE);
    
    border_tmp.xborder = src->xborder;
	border_tmp.yborder = src->yborder;

	Kernel<double> *kern1 = new Kernel<double>(&border_tmp, horz_kernel.x_radius,
								horz_kernel.y_radius, horz_kernel.values, horz_kernel.divider);

	tmp_dst = kern1->Convolve();
		
	tmp_border = FreeImageAlgorithms_SetZeroBorder(tmp_dst, xborder, yborder);

	Kernel<double> *kern2 = new Kernel<double>(tmp_border, vert_kernel.x_radius,
				vert_kernel.y_radius, vert_kernel.values, vert_kernel.divider);

	dst = kern2->Convolve();

	FreeImageAlgorithms_Unload(tmp_border);
	FreeImage_Unload(tmp_dst);
    FreeImage_Unload(border_tmp.fib);

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN,
            "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", src_type, FIT_BITMAP);
	}

	return dst;
}
