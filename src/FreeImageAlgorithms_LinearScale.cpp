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

#include "FreeImageAlgorithms_LinearScale.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>
#include "assert.h"

/*  Convert a greyscale image to a 8-bit grayscale dib.
 *	Convert a greyscale image to a 8-bit grayscale dib. Conversion is done using either a linear scaling from [min, max] to [0, 255].
 */
template < class Tsrc > class LINEAR_SCALE
{
  public:
    FIBITMAP * convert (FIBITMAP * src, double min, double max, double *min_with_image,
                        double *max_within_image);
};

template < class Tdst > class STRETCH
{
  public:
    FIBITMAP * StretchImageToType (FIBITMAP * src, FREE_IMAGE_TYPE type, double max);
    FIBITMAP *StretchImageAcrossRange (FIBITMAP * src, Tdst dst_min, Tdst dst_max);
};

template < class Tdst > FIBITMAP * STRETCH < Tdst >::StretchImageToType (FIBITMAP * src,
                                                                         FREE_IMAGE_TYPE type,
                                                                         double max)
{
    FIBITMAP *dst = NULL;
    unsigned x, y;

    double src_min_found;
    double src_max_found;

    unsigned width = FreeImage_GetWidth (src);
    unsigned height = FreeImage_GetHeight (src);

    FIA_FindMinMax (src, &src_min_found, &src_max_found);

    if (max == 0.0)
    {
        FIA_GetMaxPosibleValueForGreyScaleType (type, &max);
    }

    double factor = max / src_max_found;

    dst = FreeImage_AllocateT (type, width, height, 0, 0, 0, 0);

    BYTE *src_bits;
    Tdst *dst_bits;

    // scale to 8-bit
    for(y = 0; y < height; y++)
    {
        src_bits = reinterpret_cast < BYTE * >(FreeImage_GetScanLine (src, y));
        dst_bits = reinterpret_cast < Tdst * >(FreeImage_GetScanLine (dst, y));

        for(x = 0; x < width; x++)
        {
            dst_bits[x] = static_cast < Tdst > (src_bits[x] * factor);
        }
    }

    return dst;
}

template < class Tdst > FIBITMAP * STRETCH < Tdst >::StretchImageAcrossRange (FIBITMAP * src,
                                                                              Tdst dst_min,
                                                                              Tdst dst_max)
{
    FIBITMAP *dst = NULL;

    if (src == NULL)
    {
        return NULL;
    }

    if (dst_min >= dst_max)
    {
        return NULL;
    }

    unsigned x, y;

    unsigned width = FreeImage_GetWidth (src);
    unsigned height = FreeImage_GetHeight (src);

    double min_found, max_found;

    FIA_FindMinMax (src, &min_found, &max_found);

    // compute the scaling factor
    double scale = (double) (dst_max - dst_min) / (max_found - min_found);

    dst = FIA_CloneImageType (src, width, height);

    BYTE *src_bits;
    Tdst *dst_bits;

    // scale to 8-bit
    for(y = 0; y < height; y++)
    {
        src_bits = reinterpret_cast < BYTE * >(FreeImage_GetScanLine (src, y));
        dst_bits = reinterpret_cast < Tdst * >(FreeImage_GetScanLine (dst, y));

        for(x = 0; x < width; x++)
        {
            dst_bits[x] = static_cast < Tdst > ((src_bits[x] - min_found) * scale + dst_min);
        }
    }

    return dst;
}

template < class Tsrc > FIBITMAP * LINEAR_SCALE < Tsrc >::convert (FIBITMAP * src, double min,
                                                                   double max,
                                                                   double *min_within_image,
                                                                   double *max_within_image)
{
    FIBITMAP *dst = NULL;

    unsigned int width = FreeImage_GetWidth (src);
    unsigned int height = FreeImage_GetHeight (src);

    double min_found = min, max_found = max;

    if (min_within_image != NULL)
	   *min_within_image = 0.0;
    if (max_within_image != NULL)
	    *max_within_image = 0.0;

    // If the user has not specifed min & max use the min and max pixels in the image.
    // Ie convert to standard type while scaling the range
    if (max_found == 0.0 && min_found == 0.0)
    {
        FIA_FindMinMax (src, &min_found, &max_found);
    }

    // We can scale as only one value present - return a clone
    if (min_found == max_found)
    {
        return FreeImage_Clone (src);
    }

    if (min_within_image != NULL)
    {
        *min_within_image = min_found;
    }

    if (max_within_image != NULL)
    {
        *max_within_image = max_found;
    }

    // compute the scaling factor
    double scale = 255.0 / (double) (max_found - min_found);

    // Scale is 1.0
    if (scale == 1.0)
    {
        return FreeImage_ConvertToStandardType (src, 0);
    }

    // allocate a 8-bit dib
    if ((dst = FreeImage_AllocateT (FIT_BITMAP, width, height, 8, 0, 0, 0)) == NULL)
    {
        return NULL;
    }

    if (FreeImage_GetImageType (src) == FIT_BITMAP)
    {
        FIA_CopyPalette (src, dst);
    }
    else
    {
        // Just use a standard Greyscale palette as the input is not an 8bit image
        FIA_SetGreyLevelPalette (dst);
    }

    Tsrc *src_bits, tmp_min = (Tsrc) min_found, tmp_max = (Tsrc) max_found;
    register Tsrc val;
    BYTE *dst_bits;

    // scale to 8-bit
    for(register unsigned int y = 0; y < height; y++)
    {
        src_bits = (Tsrc *) (FreeImage_GetScanLine (src, y));
        dst_bits = FreeImage_GetScanLine (dst, y);

        for(register unsigned int x = 0; x < width; x++)
        {
            val = src_bits[x];

            if ((double)val <= min_found)
            {

                dst_bits[x] = 0;
                continue;
            }
            else if ((double)val >= max_found)
            {
                dst_bits[x] = 255;
            }
            else
            {
                dst_bits[x] = (BYTE) (scale * ((double)val - min_found));
            }
        }
    }

    return dst;
}

// Convert from type X to type BYTE
static LINEAR_SCALE < unsigned char >scaleUCharImage;
static LINEAR_SCALE < unsigned short >scaleUShortImage;
static LINEAR_SCALE < short >scaleShortImage;
static LINEAR_SCALE < unsigned long >scaleULongImage;
static LINEAR_SCALE < long >scaleLongImage;
static LINEAR_SCALE < float >scaleFloatImage;
static LINEAR_SCALE < double >scaleDoubleImage;

FIBITMAP *DLL_CALLCONV
FIA_LinearScaleToStandardType (FIBITMAP * src, double min, double max, double *min_within_image,
                               double *max_within_image)
{
    FIBITMAP *dst = NULL;

    if (!src)
    {
        return NULL;
    }

    // convert from src_type to FIT_BITMAP

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
            {
                dst = scaleUCharImage.convert (src, min, max, min_within_image, max_within_image);
            }
            break;
        }
        case FIT_UINT16:
        {                       // array of unsigned short: unsigned 16-bit
            dst = scaleUShortImage.convert (src, min, max, min_within_image, max_within_image);
            break;
        }
        case FIT_INT16:
        {                       // array of short: signed 16-bit
            dst = scaleShortImage.convert (src, min, max, min_within_image, max_within_image);
            break;
        }
        case FIT_UINT32:
        {                       // array of unsigned long: unsigned 32-bit
            dst = scaleULongImage.convert (src, min, max, min_within_image, max_within_image);
            break;
        }
        case FIT_INT32:
        {                       // array of long: signed 32-bit
            dst = scaleLongImage.convert (src, min, max, min_within_image, max_within_image);
            break;
        }
        case FIT_FLOAT:
        {                       // array of float: 32-bit
            dst = scaleFloatImage.convert (src, min, max, min_within_image, max_within_image);
            break;
        }
        case FIT_DOUBLE:
        {                       // array of double: 64-bit
            dst = scaleDoubleImage.convert (src, min, max, min_within_image, max_within_image);
            break;
        }
        default:
        {
            break;
        }
    }

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     src_type, FIT_BITMAP);
    }

    return dst;
}

// Convert from type X to type BYTE
STRETCH < unsigned char >stretchUCharImage;
STRETCH < unsigned short >stretchUShortImage;
STRETCH < short >stretchShortImage;
STRETCH < unsigned long >stretchULongImage;
STRETCH < long >stretchLongImage;
STRETCH < float >stretchFloatImage;
STRETCH < double >stretchDoubleImage;

FIBITMAP *DLL_CALLCONV
FIA_StretchImageToType (FIBITMAP * src, FREE_IMAGE_TYPE type, double max)
{
    FIBITMAP *dst = NULL;

    if (!src)
    {
        return NULL;
    }

    switch (type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
            {
                dst = stretchUCharImage.StretchImageToType (src, type, max);
            }
            break;
        }
        case FIT_UINT16:
        {                       // array of unsigned short: unsigned 16-bit
            dst = stretchUShortImage.StretchImageToType (src, type, max);
            break;
        }
        case FIT_INT16:
        {                       // array of short: signed 16-bit
            dst = stretchShortImage.StretchImageToType (src, type, max);
            break;
        }
        case FIT_UINT32:
        {                       // array of unsigned long: unsigned 32-bit
            dst = stretchULongImage.StretchImageToType (src, type, max);
            break;
        }
        case FIT_INT32:
        {                       // array of long: signed 32-bit
            dst = stretchLongImage.StretchImageToType (src, type, max);
            break;
        }
        case FIT_FLOAT:
        {                       // array of float: 32-bit
            dst = stretchFloatImage.StretchImageToType (src, type, max);
            break;
        }
        case FIT_DOUBLE:
        {                       // array of double: 64-bit
            dst = stretchDoubleImage.StretchImageToType (src, type, max);
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
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     type, FIT_BITMAP);
    }

    return dst;
}

FIBITMAP *DLL_CALLCONV
FIA_StretchImageAcrossRange (FIBITMAP * src, double min, double max)
{
    FIBITMAP *dst = NULL;

    if (!src)
    {
        return NULL;
    }

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    switch (type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
            {
                dst =
                    stretchUCharImage.StretchImageAcrossRange (src, (unsigned char) min,
                                                               (unsigned char) max);
            }
            break;
        }
        case FIT_UINT16:
        {                       // array of unsigned short: unsigned 16-bit
            dst =
                stretchUShortImage.StretchImageAcrossRange (src, (unsigned short) min,
                                                            (unsigned short) max);
            break;
        }
        case FIT_INT16:
        {                       // array of short: signed 16-bit
            dst = stretchShortImage.StretchImageAcrossRange (src, (short) min, (short) max);
            break;
        }
        case FIT_UINT32:
        {                       // array of unsigned long: unsigned 32-bit
            dst =
                stretchULongImage.StretchImageAcrossRange (src, (unsigned long) min,
                                                           (unsigned long) max);
            break;
        }
        case FIT_INT32:
        {                       // array of long: signed 32-bit
            dst = stretchLongImage.StretchImageAcrossRange (src, (long) min, (long) max);
            break;
        }
        case FIT_FLOAT:
        {                       // array of float: 32-bit
            dst = stretchFloatImage.StretchImageAcrossRange (src, (float) min, (float) max);
            break;
        }
        case FIT_DOUBLE:
        {                       // array of double: 64-bit
            dst = stretchDoubleImage.StretchImageAcrossRange (src, type, max);
            break;
        }
        default:
        {
            break;
        }
    }

    if (NULL == dst)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     type, FIT_BITMAP);
    }

    return dst;
}

int DLL_CALLCONV
FIA_InplaceLinearScaleToStandardType (FIBITMAP ** src, double min, double max, double *found_min,
                                       double *found_max)
{
    FIBITMAP *dst = FIA_LinearScaleToStandardType (*src, min, max, found_min, found_max);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}
