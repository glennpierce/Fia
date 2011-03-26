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

#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_Utilities.h"
#include <limits>
#include <float.h>
#include <math.h>

template < class Tsrc > class ARITHMATIC
{
  public:
    int MaxOfTwoImages (FIBITMAP * dst, FIBITMAP * src);
    int AddImages (FIBITMAP * dst, FIBITMAP * src);
    int SubtractImages (FIBITMAP * dst, FIBITMAP * src);
    int DivideImages (FIBITMAP * dst, FIBITMAP * src);
    int MultiplyImages (FIBITMAP * dst, FIBITMAP * src);
    int MultiplyGreyLevelImageConstant (FIBITMAP * dst, double constant);
    int DivideGreyLevelImageConstant (FIBITMAP * dst, double constant);
    int AddGreyLevelImageConstant (FIBITMAP * dst, double constant);
    int SubtractGreyLevelImageConstant (FIBITMAP * dst, double constant);
    int SumOfAllPixels (FIBITMAP * src, FIBITMAP * mask, double *sum);
    double DifferenceMeasure (FIBITMAP * src1, FIBITMAP *src2);

    FIBITMAP *Transpose (FIBITMAP * src);
    FIBITMAP *Log (FIBITMAP * src);
};

static int
CheckDimensions (FIBITMAP * dst, FIBITMAP * src)
{
    // Check src is the same size as dst
    int src_width = FreeImage_GetWidth (src);
    int src_height = FreeImage_GetHeight (src);
    int dst_width = FreeImage_GetWidth (dst);
    int dst_height = FreeImage_GetHeight (dst);

    if (src_width != dst_width || src_height != dst_height)
        return FIA_ERROR;

    return FIA_SUCCESS;
}

template < typename Tsrc > int ARITHMATIC < Tsrc >::SumOfAllPixels (FIBITMAP * src,
                                                                    FIBITMAP * mask, double *sum)
{
    if (src == NULL)
        return FIA_ERROR;

    if (mask != NULL)
    {
        // Mask has to be the same size
        if (CheckDimensions (src, mask) == FIA_ERROR)
        {
            FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                         "Image source and mask have different dimensions");
            return FIA_ERROR;
        }

        // Mask has to be 8 bit 
        if (FreeImage_GetBPP (mask) != 8 || FreeImage_GetImageType (mask) != FIT_BITMAP)
        {
            FreeImage_OutputMessageProc (FIF_UNKNOWN, "Mask must be an 8bit FIT_BITMAP");
            return FIA_ERROR;
        }
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    *sum = 0.0;

    if (mask != NULL)
    {
        for(register int y = 0; y < height; y++)
        {
            Tsrc *src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);
            unsigned char *mask_ptr = (unsigned char *) FreeImage_GetScanLine (mask, y);

            for(register int x = 0; x < width; x++)
            {
                if (mask_ptr[x])
                    *sum += (double) src_ptr[x];
            }
        }
    }
    else
    {
        for(register int y = 0; y < height; y++)
        {
            Tsrc *src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

            for(register int x = 0; x < width; x++)
                *sum += (double) src_ptr[x];
        }
    }

    return FIA_SUCCESS;
}

template < class Tsrc > FIBITMAP * ARITHMATIC < Tsrc >::Transpose (FIBITMAP * src)
{

    FIBITMAP *dst;
    int x, y;
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);
    int bpp = FreeImage_GetBPP (src);
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    if ((dst = FreeImage_AllocateT (type, height, width, bpp, 0, 0, 0)) == NULL)
        return NULL;

    Tsrc in_pix, *in_bits, *out_bits;

    for(y = 0; y < height; y++)
    {

        in_bits = (Tsrc *) FreeImage_GetScanLine (src, y);

        for(x = 0; x < width; x++)
        {
            in_pix = in_bits[x];

            out_bits = (Tsrc *) FreeImage_GetScanLine (dst, width - x - 1);
            out_bits[height - y - 1] = in_pix;
        }
    }

    return dst;
}

template < class Tsrc > double ARITHMATIC < Tsrc >::DifferenceMeasure (FIBITMAP * src1, FIBITMAP *src2)
{
    // Loop through the two images adding the differences of each pixel
    // and finally divide by total.
    int width1 = FreeImage_GetWidth(src1);
    int width2 = FreeImage_GetWidth(src2);
    int height1 = FreeImage_GetHeight(src1);
    int height2 = FreeImage_GetHeight(src2);

    if (FreeImage_GetImageType(src1) != FreeImage_GetImageType(src2))
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Images must be of the same type");
        return -1.0;
    }

    if (FreeImage_GetBPP(src1) != FreeImage_GetBPP(src2))
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "Images must be of the same bpp");
        return -1.0;
    }

    if (width1 != width2)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "DifferenceMeasure: input images are different sizes");
        return -1.0;
    }

    if (height1 != height2)
    {
        FreeImage_OutputMessageProc(FIF_UNKNOWN,
                "DifferenceMeasure: input images are different sizes");
        return -1.0;
    }

    Tsrc *src1_ptr, *src2_ptr;
    float sum = 0.0f;

    for(int y = 0; y < height1; y++)
    {
        src1_ptr = (Tsrc *) FreeImage_GetScanLine (src1, y);
        src2_ptr = (Tsrc *) FreeImage_GetScanLine (src2, y);

        for(int x = 0; x < width1; x++)
        {
            sum += (float) fabs((float)(src1_ptr[x] - src2_ptr[x]));
        }
    }

    return sum;
}

template < class Tsrc > FIBITMAP * ARITHMATIC < Tsrc >::Log (FIBITMAP * src)
{

    FIBITMAP *dst;
    int x, y;
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);
    int bpp = FreeImage_GetBPP (src);

    if ((dst = FreeImage_AllocateT (FIT_DOUBLE, width, height, bpp, 0, 0, 0)) == NULL)
        return NULL;

    Tsrc *in_bits;
    double *out_bits;

    for(y = 0; y < height; y++)
    {
        in_bits = (Tsrc *) FreeImage_GetScanLine (src, y);
        out_bits = (double *) FreeImage_GetScanLine (dst, y);

        for(x = 0; x < width; x++)
            out_bits[x] = (double) log ((double) in_bits[x]);
    }

    return dst;
}

// This function sets dst to have the max pixel value of src and dst
template < class Tsrc > int ARITHMATIC < Tsrc >::MaxOfTwoImages (FIBITMAP * dst, FIBITMAP * src)
{
    if (dst == NULL || src == NULL)
        return FIA_ERROR;

    if (CheckDimensions (dst, src) == FIA_ERROR)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination and source have different dimensions");
        return FIA_ERROR;
    }

    // Make sure dst is a double or float so it can hold all the results of
    // the arithmetic.
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (dst);

    if (type != FIT_DOUBLE && type != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    Tsrc *src_ptr;

    // Can be a float or a double dst image
    if (type == FIT_DOUBLE)
    {
        double *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (double *) FreeImage_GetScanLine (dst, y);
            src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

            for(register int x = 0; x < width; x++)
            {
                if (src_ptr[x] > dst_ptr[x])
                    dst_ptr[x] = (double) src_ptr[x];
            }
        }
    }
    else
    {

        float *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (float *) FreeImage_GetScanLine (dst, y);
            src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

            for(register int x = 0; x < width; x++)
            {
                if (src_ptr[x] > dst_ptr[x])
                    dst_ptr[x] = (float) src_ptr[x];
            }
        }
    }

    return FIA_SUCCESS;
}

template < class Tsrc > int ARITHMATIC < Tsrc >::MultiplyImages (FIBITMAP * dst, FIBITMAP * src)
{
    if (dst == NULL || src == NULL)
        return FIA_ERROR;

    if (CheckDimensions (dst, src) == FIA_ERROR)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination and source have different dimensions");
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    // Make dst a double so it can hold all the results of
    // the arithmetic.
    if (FreeImage_GetImageType (dst) != FIT_DOUBLE && FreeImage_GetImageType (dst) != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    for(register int y = 0; y < height; y++)
    {
        Tsrc *src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);
        double *dst_ptr = (double *) FreeImage_GetScanLine (dst, y);

        for(register int x = 0; x < width; x++)
            dst_ptr[x] *= src_ptr[x];
    }

    return FIA_SUCCESS;
}

template < class Tsrc > int ARITHMATIC < Tsrc >::DivideImages (FIBITMAP * dst, FIBITMAP * src)
{
    if (dst == NULL || src == NULL)
        return FIA_ERROR;

    if (CheckDimensions (dst, src) == FIA_ERROR)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination and source have different dimensions");
        return FIA_ERROR;
    }

    // Make dst a double so it can hold all the results of
    // the arithmetic.
    if (FreeImage_GetImageType (dst) != FIT_DOUBLE && FreeImage_GetImageType (dst) != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    double *dst_ptr = (double *) FreeImage_GetBits (dst);
    Tsrc *src_ptr = (Tsrc *) FreeImage_GetBits (src);

    if (dst_ptr == NULL || src_ptr == NULL)
        return FIA_ERROR;

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    for(register int y = 0; y < height; y++)
    {
        dst_ptr = (double *) FreeImage_GetScanLine (dst, y);
        src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

        for(register int x = 0; x < width; x++)
        {
            dst_ptr[x] = (double) dst_ptr[x] / src_ptr[x];
        }
    }

    return FIA_SUCCESS;
}

template < class Tsrc > int ARITHMATIC < Tsrc >::AddImages (FIBITMAP * dst, FIBITMAP * src)
{
    if (dst == NULL || src == NULL)
        return FIA_ERROR;

    if (CheckDimensions (dst, src) == FIA_ERROR)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination and source have different dimensions");
        return FIA_ERROR;
    }

    // Make sure dst is a double or float so it can hold all the results of
    // the arithmetic.
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (dst);

    if (type != FIT_DOUBLE && type != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    Tsrc *src_ptr;

    // Can be a float or a double dst image
    if (type == FIT_DOUBLE)
    {
        double *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (double *) FreeImage_GetScanLine (dst, y);
            src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (double) (dst_ptr[x] + src_ptr[x]);
        }
    }
    else
    {
        float *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (float *) FreeImage_GetScanLine (dst, y);
            src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (float) (dst_ptr[x] + src_ptr[x]);
        }
    }

    return FIA_SUCCESS;
}

template < class Tsrc > int ARITHMATIC < Tsrc >::SubtractImages (FIBITMAP * dst, FIBITMAP * src)
{
    if (dst == NULL || src == NULL)
        return FIA_ERROR;

    if (CheckDimensions (dst, src) == FIA_ERROR)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination and source have different dimensions");
        return FIA_ERROR;
    }

    // Make sure dst is a double or float so it can hold all the results of
    // the arithmetic.
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (dst);

    if (type != FIT_DOUBLE && type != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    Tsrc *src_ptr;

    // Can be a float or a double dst image
    if (type == FIT_DOUBLE)
    {
        double *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (double *) FreeImage_GetScanLine (dst, y);
            src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (double) (dst_ptr[x] - src_ptr[x]);
        }
    }
    else
    {
        float *dst_ptr;

        for(register int y = 0; y < height; y++)
        {

            dst_ptr = (float *) FreeImage_GetScanLine (dst, y);
            src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (float) (dst_ptr[x] - src_ptr[x]);
        }
    }

    return FIA_SUCCESS;
}

template < class Tsrc > int ARITHMATIC < Tsrc >::MultiplyGreyLevelImageConstant (FIBITMAP * dst,
                                                                                 double constant)
{
    if (dst == NULL)
        return FIA_ERROR;

    // Make sure dst is a double or float so it can hold all the results of
    // the arithmetic.
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (dst);

    if (type != FIT_DOUBLE && type != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (dst);
    int height = FreeImage_GetHeight (dst);

    Tsrc casted_constant = (Tsrc) constant;

    // Can be a float or a double dst image
    if (type == FIT_DOUBLE)
    {
        double *dst_ptr;

        for(register int y = 0; y < height; y++)
        {

            dst_ptr = (double *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (double) (dst_ptr[x] * casted_constant);
        }
    }
    else
    {
        float *dst_ptr;

        for(register int y = 0; y < height; y++)
        {

            dst_ptr = (float *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (float) (dst_ptr[x] * casted_constant);
        }
    }

    return FIA_SUCCESS;
}

template < class Tsrc > int ARITHMATIC < Tsrc >::DivideGreyLevelImageConstant (FIBITMAP * dst,
                                                                               double constant)
{
    if (dst == NULL)
        return FIA_ERROR;

    // Make sure dst is a double or float so it can hold all the results of
    // the arithmetic.
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (dst);

    if (type != FIT_DOUBLE && type != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (dst);
    int height = FreeImage_GetHeight (dst);

    // Can be a float or a double dst image
    if (type == FIT_DOUBLE)
    {
        double *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (double *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (double) (dst_ptr[x] / constant);
        }
    }
    else
    {

        float *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (float *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (float) (dst_ptr[x] / constant);
        }
    }

    return FIA_SUCCESS;
}

template < class Tsrc > int ARITHMATIC < Tsrc >::AddGreyLevelImageConstant (FIBITMAP * dst,
                                                                            double constant)
{
    if (dst == NULL)
        return FIA_ERROR;

    // Make sure dst is a double or float so it can hold all the results of
    // the arithmetic.
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (dst);

    if (type != FIT_DOUBLE && type != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (dst);
    int height = FreeImage_GetHeight (dst);

    Tsrc casted_constant = (Tsrc) constant;

    // Can be a float or a double dst image
    if (type == FIT_DOUBLE)
    {
        double *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (double *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (double) (dst_ptr[x] + casted_constant);
        }
    }
    else
    {
        float *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (float *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (float) (dst_ptr[x] + casted_constant);
        }
    }

    return FIA_SUCCESS;
}

template < class Tsrc > int ARITHMATIC < Tsrc >::SubtractGreyLevelImageConstant (FIBITMAP * dst,
                                                                                 double constant)
{
    if (dst == NULL)
        return FIA_ERROR;

    // Make sure dst is a double or float so it can hold all the results of
    // the arithmetic.
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (dst);

    if (type != FIT_DOUBLE && type != FIT_FLOAT)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination was not a FIT_FLOAT or FIT_DOUBLE");
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (dst);
    int height = FreeImage_GetHeight (dst);

    Tsrc casted_constant = (Tsrc) constant;

    // Can be a float or a double dst image
    if (type == FIT_DOUBLE)
    {
        double *dst_ptr;

        for(register int y = 0; y < height; y++)
        {

            dst_ptr = (double *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (double) (dst_ptr[x] - casted_constant);
        }
    }
    else
    {
        float *dst_ptr;

        for(register int y = 0; y < height; y++)
        {
            dst_ptr = (float *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (float) (dst_ptr[x] - casted_constant);
        }
    }

    return FIA_SUCCESS;
}

ARITHMATIC < unsigned char >arithmaticUCharImage;
ARITHMATIC < unsigned short >arithmaticUShortImage;
ARITHMATIC < short >arithmaticShortImage;
ARITHMATIC < unsigned long >arithmaticULongImage;
ARITHMATIC < long >arithmaticLongImage;
ARITHMATIC < float >arithmaticFloatImage;
ARITHMATIC < double >arithmaticDoubleImage;

ARITHMATIC < FICOMPLEX > arithmaticComplexImage;

FIBITMAP *DLL_CALLCONV
FIA_Transpose (FIBITMAP * src)
{
    FIBITMAP *dst = NULL;

    if (!src)
        return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
                dst = arithmaticUCharImage.Transpose (src);
            break;
        case FIT_UINT16:       // array of unsigned short: unsigned 16-bit
            dst = arithmaticUShortImage.Transpose (src);
            break;
        case FIT_INT16:        // array of short: signed 16-bit
            dst = arithmaticShortImage.Transpose (src);
            break;
        case FIT_UINT32:       // array of unsigned long: unsigned 32-bit
            dst = arithmaticULongImage.Transpose (src);
            break;
        case FIT_INT32:        // array of long: signed 32-bit
            dst = arithmaticLongImage.Transpose (src);
            break;
        case FIT_FLOAT:        // array of float: 32-bit
            dst = arithmaticFloatImage.Transpose (src);
            break;
        case FIT_DOUBLE:       // array of double: 64-bit
            dst = arithmaticDoubleImage.Transpose (src);
            break;
        case FIT_COMPLEX:      // array of double: 64-bit
            dst = arithmaticComplexImage.Transpose (src);
            break;
        default:
            break;
    }

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     src_type, FIT_BITMAP);
    }

    return dst;
}

double DLL_CALLCONV
FIA_DifferenceMeasure (FIBITMAP *src1, FIBITMAP *src2)
{
    if (!src1)
        return -1.0;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src1);

    switch (src_type)
    {
        case FIT_BITMAP:       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src1) == 8)
                return arithmaticUCharImage.DifferenceMeasure (src1, src2);
            else {

                FIBITMAP * gs1 = FreeImage_ConvertToGreyscale(src1);
                FIBITMAP * gs2 = FreeImage_ConvertToGreyscale(src2);

                double max =  arithmaticUCharImage.DifferenceMeasure (gs1, gs2);

                FreeImage_Unload(gs1);
                FreeImage_Unload(gs2);

                return max;
            }

        case FIT_UINT16:       // array of unsigned short: unsigned 16-bit
            return arithmaticUShortImage.DifferenceMeasure (src1, src2);
        case FIT_INT16:        // array of short: signed 16-bit
            return arithmaticShortImage.DifferenceMeasure (src1, src2);
        case FIT_UINT32:       // array of unsigned long: unsigned 32-bit
            return arithmaticULongImage.DifferenceMeasure (src1, src2);
        case FIT_INT32:        // array of long: signed 32-bit
            return arithmaticLongImage.DifferenceMeasure (src1, src2);
        case FIT_FLOAT:        // array of float: 32-bit
            return arithmaticFloatImage.DifferenceMeasure (src1, src2);
        case FIT_DOUBLE:       // array of double: 64-bit
            return arithmaticDoubleImage.DifferenceMeasure (src1, src2);
        case FIT_COMPLEX:      // array of double: 64-bit
            break;
        default:
            break;
    }

    return -1.0;
}

FIBITMAP *DLL_CALLCONV
FIA_Log (FIBITMAP * src)
{
    FIBITMAP *dst = NULL;

    if (!src)
        return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
                dst = arithmaticUCharImage.Log (src);
            break;
        case FIT_UINT16:       // array of unsigned short: unsigned 16-bit
            dst = arithmaticUShortImage.Log (src);
            break;
        case FIT_INT16:        // array of short: signed 16-bit
            dst = arithmaticShortImage.Log (src);
            break;
        case FIT_UINT32:       // array of unsigned long: unsigned 32-bit
            dst = arithmaticULongImage.Log (src);
            break;
        case FIT_INT32:        // array of long: signed 32-bit
            dst = arithmaticLongImage.Log (src);
            break;
        case FIT_FLOAT:        // array of float: 32-bit
            dst = arithmaticFloatImage.Log (src);
            break;
        case FIT_DOUBLE:       // array of double: 64-bit
            dst = arithmaticDoubleImage.Log (src);
            break;
        default:
            break;
    }

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     src_type, FIT_BITMAP);
    }

    return dst;
}

int DLL_CALLCONV
FIA_MultiplyGreyLevelImages (FIBITMAP * dst, FIBITMAP * src)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (src) == 8)
                return arithmaticUCharImage.MultiplyImages (dst, src);
        case FIT_UINT16:
            return arithmaticUShortImage.MultiplyImages (dst, src);
        case FIT_INT16:
            return arithmaticShortImage.MultiplyImages (dst, src);
        case FIT_UINT32:
            return arithmaticULongImage.MultiplyImages (dst, src);
        case FIT_INT32:
            return arithmaticLongImage.MultiplyImages (dst, src);
        case FIT_FLOAT:
            return arithmaticFloatImage.MultiplyImages (dst, src);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.MultiplyImages (dst, src);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_DivideGreyLevelImages (FIBITMAP * dst, FIBITMAP * src)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (src) == 8)
                return arithmaticUCharImage.DivideImages (dst, src);
        case FIT_UINT16:
            return arithmaticUShortImage.DivideImages (dst, src);
        case FIT_INT16:
            return arithmaticShortImage.DivideImages (dst, src);
        case FIT_UINT32:
            return arithmaticULongImage.DivideImages (dst, src);
        case FIT_INT32:
            return arithmaticLongImage.DivideImages (dst, src);
        case FIT_FLOAT:
            return arithmaticFloatImage.DivideImages (dst, src);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.DivideImages (dst, src);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_GetMaxIntensityFromImages (FIBITMAP * dst, FIBITMAP * src)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (src) == 8)
                return arithmaticUCharImage.MaxOfTwoImages (dst, src);
        case FIT_UINT16:
            return arithmaticUShortImage.MaxOfTwoImages (dst, src);
        case FIT_INT16:
            return arithmaticShortImage.MaxOfTwoImages (dst, src);
        case FIT_UINT32:
            return arithmaticULongImage.MaxOfTwoImages (dst, src);
        case FIT_INT32:
            return arithmaticLongImage.MaxOfTwoImages (dst, src);
        case FIT_FLOAT:
            return arithmaticFloatImage.MaxOfTwoImages (dst, src);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.MaxOfTwoImages (dst, src);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_AddGreyLevelImages (FIBITMAP * dst, FIBITMAP * src)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (src) == 8)
                return arithmaticUCharImage.AddImages (dst, src);
        case FIT_UINT16:
            return arithmaticUShortImage.AddImages (dst, src);
        case FIT_INT16:
            return arithmaticShortImage.AddImages (dst, src);
        case FIT_UINT32:
            return arithmaticULongImage.AddImages (dst, src);
        case FIT_INT32:
            return arithmaticLongImage.AddImages (dst, src);
        case FIT_FLOAT:
            return arithmaticFloatImage.AddImages (dst, src);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.AddImages (dst, src);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_SubtractGreyLevelImages (FIBITMAP * dst, FIBITMAP * src)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (src) == 8)
                return arithmaticUCharImage.SubtractImages (dst, src);
        case FIT_UINT16:
            return arithmaticUShortImage.SubtractImages (dst, src);
        case FIT_INT16:
            return arithmaticShortImage.SubtractImages (dst, src);
        case FIT_UINT32:
            return arithmaticULongImage.SubtractImages (dst, src);
        case FIT_INT32:
            return arithmaticLongImage.SubtractImages (dst, src);
        case FIT_FLOAT:
            return arithmaticFloatImage.SubtractImages (dst, src);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.SubtractImages (dst, src);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_MultiplyGreyLevelImageConstant (FIBITMAP * dst, double constant)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (dst);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (dst) == 8)
                return arithmaticUCharImage.MultiplyGreyLevelImageConstant (dst, constant);
        case FIT_UINT16:
            return arithmaticUShortImage.MultiplyGreyLevelImageConstant (dst, constant);
        case FIT_INT16:
            return arithmaticShortImage.MultiplyGreyLevelImageConstant (dst, constant);
        case FIT_UINT32:
            return arithmaticULongImage.MultiplyGreyLevelImageConstant (dst, constant);
        case FIT_INT32:
            return arithmaticLongImage.MultiplyGreyLevelImageConstant (dst, constant);
        case FIT_FLOAT:
            return arithmaticFloatImage.MultiplyGreyLevelImageConstant (dst, constant);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.MultiplyGreyLevelImageConstant (dst, constant);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_DivideGreyLevelImageConstant (FIBITMAP * dst, double constant)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (dst);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (dst) == 8)
                return arithmaticUCharImage.DivideGreyLevelImageConstant (dst, constant);
        case FIT_UINT16:
            return arithmaticUShortImage.DivideGreyLevelImageConstant (dst, constant);
        case FIT_INT16:
            return arithmaticShortImage.DivideGreyLevelImageConstant (dst, constant);
        case FIT_UINT32:
            return arithmaticULongImage.DivideGreyLevelImageConstant (dst, constant);
        case FIT_INT32:
            return arithmaticLongImage.DivideGreyLevelImageConstant (dst, constant);
        case FIT_FLOAT:
            return arithmaticFloatImage.DivideGreyLevelImageConstant (dst, constant);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.DivideGreyLevelImageConstant (dst, constant);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_AddGreyLevelImageConstant (FIBITMAP * dst, double constant)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (dst);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (dst) == 8)
                return arithmaticUCharImage.AddGreyLevelImageConstant (dst, constant);
        case FIT_UINT16:
            return arithmaticUShortImage.AddGreyLevelImageConstant (dst, constant);
        case FIT_INT16:
            return arithmaticShortImage.AddGreyLevelImageConstant (dst, constant);
        case FIT_UINT32:
            return arithmaticULongImage.AddGreyLevelImageConstant (dst, constant);
        case FIT_INT32:
            return arithmaticLongImage.AddGreyLevelImageConstant (dst, constant);
        case FIT_FLOAT:
            return arithmaticFloatImage.AddGreyLevelImageConstant (dst, constant);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.AddGreyLevelImageConstant (dst, constant);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_SubtractGreyLevelImageConstant (FIBITMAP * dst, double constant)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (dst);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (dst) == 8)
                return arithmaticUCharImage.SubtractGreyLevelImageConstant (dst, constant);
        case FIT_UINT16:
            return arithmaticUShortImage.SubtractGreyLevelImageConstant (dst, constant);
        case FIT_INT16:
            return arithmaticShortImage.SubtractGreyLevelImageConstant (dst, constant);
        case FIT_UINT32:
            return arithmaticULongImage.SubtractGreyLevelImageConstant (dst, constant);
        case FIT_INT32:
            return arithmaticLongImage.SubtractGreyLevelImageConstant (dst, constant);
        case FIT_FLOAT:
            return arithmaticFloatImage.SubtractGreyLevelImageConstant (dst, constant);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.SubtractGreyLevelImageConstant (dst, constant);
        default:
            break;
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_ComplexConjugate (FIBITMAP * src)
{
    if (src == NULL)
        return FIA_ERROR;

    if (FreeImage_GetImageType (src) != FIT_COMPLEX)
        return FIA_ERROR;

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FICOMPLEX *src_ptr = NULL;

    for(register int y = 0; y < height; y++)
    {
        src_ptr = (FICOMPLEX *) FreeImage_GetScanLine (src, y);

        for(register int x = 0; x < width; x++)
        {
            src_ptr[x].i = -src_ptr[x].i;
        }
    }

    return FIA_SUCCESS;
}

// (a + ib) (c + id) 
// = ac + ibc + ida + i^2bd
// = ac + ibc + ida - bd
// = (ac - bd) + i(bc + da)
int DLL_CALLCONV
FIA_MultiplyComplexImages (FIBITMAP * dst, FIBITMAP * src)
{
    if (dst == NULL || src == NULL)
        return FIA_ERROR;

    if (CheckDimensions (dst, src) == FIA_ERROR)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Image destination and source have different dimensions");
        return FIA_ERROR;
    }

    // Make dst a double so it can hold all the results of
    // the arithmatic.
    if (FreeImage_GetImageType (dst) != FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Destination image must be of type FIT_COMPLEX");
        return FIA_ERROR;
    }

    if (FreeImage_GetImageType (src) != FIT_COMPLEX)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Source image must be of type FIT_COMPLEX");
        return FIA_ERROR;
    }

    FICOMPLEX *dst_ptr = NULL;
    FICOMPLEX *src_ptr = NULL;

    double tmp;
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    for(register int y = 0; y < height; y++)
    {
        src_ptr = (FICOMPLEX *) FreeImage_GetScanLine (src, y);
        dst_ptr = (FICOMPLEX *) FreeImage_GetScanLine (dst, y);

        for(register int x = 0; x < width; x++)
        {
            // real part = ac - bd
            tmp = (dst_ptr[x].r * src_ptr[x].r) - (dst_ptr[x].i * src_ptr[x].i);

            // imaginary part = bc + da
            dst_ptr[x].i = (dst_ptr[x].i * src_ptr[x].r) + (src_ptr[x].i * dst_ptr[x].r);
            dst_ptr[x].r = tmp;
        }
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SumOfAllPixels (FIBITMAP * src, FIBITMAP * mask, double *sum)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
            if (FreeImage_GetBPP (src) == 8)
                return arithmaticUCharImage.SumOfAllPixels (src, mask, sum);
        case FIT_UINT16:
            return arithmaticUShortImage.SumOfAllPixels (src, mask, sum);
        case FIT_INT16:
            return arithmaticShortImage.SumOfAllPixels (src, mask, sum);
        case FIT_UINT32:
            return arithmaticULongImage.SumOfAllPixels (src, mask, sum);
        case FIT_INT32:
            return arithmaticLongImage.SumOfAllPixels (src, mask, sum);
        case FIT_FLOAT:
            return arithmaticFloatImage.SumOfAllPixels (src, mask, sum);
        case FIT_DOUBLE:
            return arithmaticDoubleImage.SumOfAllPixels (src, mask, sum);
        default:
            break;
    }

    return FIA_ERROR;
}
