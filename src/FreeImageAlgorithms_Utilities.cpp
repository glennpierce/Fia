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
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Logic.h"
#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_Morphology.h"

#include <iostream>
#include <math.h>
#include <limits.h>
#include <float.h>

// Class that templates functions so that they work on all image types.
template < class Tsrc > class TemplateImageFunctionClass
{
  public:

    // Find min max
    void find (FIBITMAP * src, double *min, double *max);
    void find_max_xy (FIBITMAP * src, double *max, FIAPOINT * pt);

    // FastSimpleResample
    FIBITMAP* IntegerRescaleToHalf (FIBITMAP * src);
    FIBITMAP* ColourRescaleToHalf (FIBITMAP * src);
    FIBITMAP* FloatRescaleToHalf (FIBITMAP * src);
	FIBITMAP* ConvertToGreyscaleFloatTypeWithUntouchedRange (FIBITMAP *src, FREE_IMAGE_TYPE type);
	FIBITMAP* FIA_ConvertToImageType (FIABITMAP *src, FREE_IMAGE_TYPE type, BOOL scale_linear);
	FIBITMAP* FIA_ConvertFloatTypeToImageType (FIBITMAP *src, FREE_IMAGE_TYPE float_type, FREE_IMAGE_TYPE type, BOOL scale_linear);

    // Composite function for all image types
    FIBITMAP *Composite(FIBITMAP * fg, FIBITMAP * bg, FIBITMAP * normalised_alpha_values, FIBITMAP *mask);
    int Combine(FIBITMAP *dst, FIBITMAP *fg, FIBITMAP *mask);
};

static TemplateImageFunctionClass < unsigned char > UCharImage;
static TemplateImageFunctionClass < unsigned short > UShortImage;
static TemplateImageFunctionClass < short > ShortImage;
static TemplateImageFunctionClass < unsigned long > ULongImage;
static TemplateImageFunctionClass < long > LongImage;
static TemplateImageFunctionClass < float > FloatImage;
static TemplateImageFunctionClass < double > DoubleImage;

#ifdef _MSC_VER

#include <xmmintrin.h>

/***
 * int _os_support(int feature)
 *   - Checks if OS Supports the capablity or not
 *
 * Entry:
 *   feature: the feature we want to check if OS supports it.
 *
 * Exit:
 *   Returns 1 if OS support exist and 0 when OS doesn't support it.
 *
 ****************************************************************/

int DLL_CALLCONV
_os_support (int feature)
{
    __try
    {
        switch (feature)
        {
            case _CPU_FEATURE_SSE:
                __asm
            {
                xorps xmm0, xmm0        // executing SSE instruction
            }
                break;
            case _CPU_FEATURE_SSE2:
                __asm
            {
                xorpd xmm0, xmm0        // executing SSE2 instruction
            }
                break;
            case _CPU_FEATURE_3DNOW:
                __asm
            {
                pfrcp mm0, mm0  // executing 3DNow! instruction
              emms}
                break;
            case _CPU_FEATURE_MMX:
                __asm
            {
                pxor mm0, mm0   // executing MMX instruction
              emms}
                break;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        if (_exception_code () == STATUS_ILLEGAL_INSTRUCTION)
        {
            return 0;
        }
        return 0;
    }
    return 1;
}

void DLL_CALLCONV
FIA_SSEFindFloatMinMax (const float *data, long n, float *min, float *max)
{
    __m128 min128 = _mm_set_ps1 (FLT_MAX);      // min128[0, 1, 2, 3] = FLT_MAX
    __m128 max128 = _mm_set_ps1 (FLT_MIN);      // max128[0, 1, 2, 3] = FLT_MIN

    __m128 *pSource = (__m128 *) data;

    for(int i = 0; i < n / 4; i++)
    {
        min128 = _mm_min_ps (*pSource, min128);
        max128 = _mm_max_ps (*pSource, max128);

        pSource++;
    }

    // extract minimum and maximum values from min128 and max128
    union u
    {
        __m128 m;
        float f[4];
    } x;

    x.m = min128;
    *min = min (x.f[0], min (x.f[1], min (x.f[2], x.f[3])));

    x.m = max128;
    *max = max (x.f[0], max (x.f[1], max (x.f[2], x.f[3])));
}

/*******************************************************************************
 Rounding from a float to the nearest integer can be done several ways.
 Calling the ANSI C floor() routine then casting to an int is very slow.
 Manually adding 0.5 then casting to an int is also somewhat slow because
 truncation of the float is slow on Intel FPUs. The fastest choice is to
 use the FPU 'fistp' instruction which does the round and conversion
 in one instruction (not sure how many clocks). This function is almost
 10x faster than adding and casting.

 Caller is expected to range check 'v' before attempting to round.
 Valid range is INT_MIN to INT_MAX inclusive.
 *******************************************************************************/
/*
__forceinline int
Round (double v)
{
    assert (v >= INT_MIN && v <= INT_MAX);
    int result;

    __asm
    {
        fld v;
        Push 'v' into st (0) of FPU stack fistp result;
    Convert and store st (0) to integer and pop}

    return result;
}
*/

#endif //  _MSC_VER

FIAPOINT DLL_CALLCONV
MakeFIAPoint (int x, int y)
{
    FIAPOINT p;

    p.x = x;
    p.y = y;

    return p;
}

FIARECT DLL_CALLCONV
MakeFIARect (int left, int top, int right, int bottom)
{
    FIARECT rect;

    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;

    return rect;
}

FIARECT DLL_CALLCONV
MakeFIARectAlt (int left, int top, int width, int height)
{
    FIARECT rect;

    rect.left = left;
    rect.top = top;
    rect.right = left+width;
    rect.bottom = top+height;

    return rect;
}

int DLL_CALLCONV
FIARectIsEmpty (FIARECT rect)
{
    if(rect.left == 0 && rect.top == 0 && rect.right == 0 && rect.bottom == 0)
		return 1;

    return 0;
}

FIARECT DLL_CALLCONV
FIAImageRect (FIBITMAP * src)
{
    FIARECT rect;

    rect.left = 0;
    rect.top = 0;
    rect.right = FreeImage_GetWidth (src) - 1;
    rect.bottom = FreeImage_GetHeight (src) - 1;

    return rect;
}

int DLL_CALLCONV FIA_GetPitchInPixels(FIBITMAP *dib)
{
    int bpp = FreeImage_GetBPP(dib);
    int bytes_per_pixel = bpp / 8;

    return (FreeImage_GetPitch (dib) / bytes_per_pixel);
}

int DLL_CALLCONV 
FIA_CheckDimensions (FIBITMAP * dst, FIBITMAP * src)
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

void DLL_CALLCONV
FIA_FindCharMinMax (const char *data, long n, char *min, char *max)
{
    MAXMIN (data, n, *max, *min);
}

void DLL_CALLCONV
FIA_FindIntMinMax (const int *data, long n, int *min, int *max)
{
    MAXMIN (data, n, *max, *min);
}

void DLL_CALLCONV
FIA_FindShortMinMax (const short *data, long n, short *min, short *max)
{
    MAXMIN (data, n, *max, *min);
}

void DLL_CALLCONV
FIA_FindUShortMinMax (const unsigned short *data, long n, unsigned short *min, unsigned short *max)
{
    MAXMIN (data, n, *max, *min);
}

void DLL_CALLCONV
FIA_FindLongMinMax (const long *data, long n, long *min, long *max)
{
    MAXMIN (data, n, *max, *min);
}

void DLL_CALLCONV
FIA_FindULongMinMax (const unsigned long *data, long n, unsigned long *min, unsigned long *max)
{
    MAXMIN (data, n, *max, *min);
}

void DLL_CALLCONV
FIA_FindFloatMinMax (const float *data, long n, float *min, float *max)
{
    MAXMIN (data, n, *max, *min);
}

void DLL_CALLCONV
FIA_FindDoubleMinMax (const double *data, long n, double *min, double *max)
{
    MAXMIN (data, n, *max, *min);
}

long DLL_CALLCONV
FIA_FindCharMax (const char *data, long n, char *max)
{
    return FINDMAX (data, n, *max);
}

long DLL_CALLCONV
FIA_FindUCharMax (const unsigned char *data, long n, unsigned char *max)
{
    return FINDMAX (data, n, *max);
}

long DLL_CALLCONV
FIA_FindIntMax (const int *data, long n, int *max)
{
    return FINDMAX (data, n, *max);
}

long DLL_CALLCONV
FIA_FindShortMax (const short *data, long n, short *max)
{
    return FINDMAX (data, n, *max);
}

long DLL_CALLCONV
FIA_FindUShortMax (const unsigned short *data, long n, unsigned short *max)
{
    return FINDMAX (data, n, *max);
}

long DLL_CALLCONV
FIA_FindFloatMax (const float *data, long n, float *max)
{
    return FINDMAX (data, n, *max);
}

long DLL_CALLCONV
FIA_FindDoubleMax (const double *data, long n, double *max)
{
    return FINDMAX (data, n, *max);
}

template < class Tsrc > void TemplateImageFunctionClass < Tsrc >::find (FIBITMAP * src,
                                                                 double *min, double *max)
{
    if (!src)
    {
        return;
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    double temp_min, temp_max;
    Tsrc l_min, l_max;

    // Get the first two pixel values for initialisation
    Tsrc *bits = reinterpret_cast < Tsrc * >(FreeImage_GetScanLine (src, 0));

    temp_min = temp_max = bits[0];

    for(int y = 0; y < height; y++)
    {
        bits = reinterpret_cast < Tsrc * >(FreeImage_GetScanLine (src, y));

        MAXMIN (bits, width, l_max, l_min);

        if (l_max > temp_max)
        {
            temp_max = l_max;
        }

        if (l_min < temp_min)
        {
            temp_min = l_min;
        }
    }

    *min = temp_min;
    *max = temp_max;
}

void DLL_CALLCONV
FIA_FindMinMax (FIBITMAP * src, double *min, double *max)
{
    if (!src)
    {
        return;
    }

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
            {
                UCharImage.find (src, min, max);
            }
            else if (FreeImage_GetBPP (src) == 24 || FreeImage_GetBPP (src) == 32)
            {
				// colour images, just set 0 and 255
				*min = 0.0;
				*max = 255.0;
			}
            break;
        }

        case FIT_UINT16:
        {
            UShortImage.find (src, min, max);
            break;
        }

        case FIT_INT16:
        {
            ShortImage.find (src, min, max);
            break;
        }

        case FIT_UINT32:
        {
            ULongImage.find (src, min, max);
            break;
        }

        case FIT_INT32:
        {
            LongImage.find (src, min, max);
            break;
        }

        case FIT_FLOAT:
        {
            FloatImage.find (src, min, max);
            break;
        }

        case FIT_DOUBLE:
        {
            DoubleImage.find (src, min, max);
            break;
        }

        default:
        {
            break;
        }
    }
}

template < class Tsrc > void TemplateImageFunctionClass < Tsrc >::find_max_xy (FIBITMAP * src,
                                                                        double *max, FIAPOINT * pt)
{
	pt->x = 0;
	pt->y = 0;
	*max = 0.0;

    if (src == NULL)
        return;

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    double temp_max;
    Tsrc l_max = 0;

    // Get the first two pixel values for initialisation
    Tsrc *bits = reinterpret_cast < Tsrc * >(FreeImage_GetScanLine (src, 0));

    temp_max = bits[0];

    int x;

    for(int y = 0; y < height; y++)
    {
        bits = reinterpret_cast < Tsrc * >(FreeImage_GetScanLine (src, y));

        x = FINDMAX (bits, width, l_max);

        if (l_max > temp_max)
        {
            temp_max = l_max;
            pt->x = x;
            pt->y = y;
        }
    }

    *max = temp_max;
}

void DLL_CALLCONV
FIA_FindMaxXY (FIBITMAP * src, double *max, FIAPOINT * pt)
{
	pt->x = 0;
	pt->y = 0;
	*max = 0.0;

    if (!src)
        return;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
            {
                UCharImage.find_max_xy (src, max, pt);
            }
            break;
        }

        case FIT_UINT16:
        {
            UShortImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_INT16:
        {
            ShortImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_UINT32:
        {
            ULongImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_INT32:
        {
            LongImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_FLOAT:
        {
            FloatImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_DOUBLE:
        {
            DoubleImage.find_max_xy (src, max, pt);
            break;
        }

        default:
        {
            break;
        }
    }
}

static int
FindMaxChannelValue (unsigned int pixel_value)
{
    unsigned char red_value = pixel_value >> FI_RGBA_RED_SHIFT;
    unsigned char green_value = pixel_value >> FI_RGBA_GREEN_SHIFT;
    unsigned char blue_value = pixel_value >> FI_RGBA_BLUE_SHIFT;

    return MAX (MAX (red_value, green_value), blue_value);
}

void DLL_CALLCONV
FIA_FindMinMaxForColourImage (FIBITMAP * src, double *min, double *max)
{
    if (!src || FreeImage_GetImageType (src) != FIT_BITMAP)
    {
        return;
    }

    if (FreeImage_GetBPP (src) != 24)
    {
        return;
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    unsigned int tmp_value;

    unsigned int *bits = (unsigned int *) FreeImage_GetBits (src);

    double temp_min = FindMaxChannelValue (bits[0]);
    double temp_max = temp_min;

    for(int y = 0; y < height; y++)
    {
        bits = reinterpret_cast < unsigned int *>(FreeImage_GetScanLine (src, y));

        for(int x = 0; x < width; x++)
        {
            tmp_value = FindMaxChannelValue (bits[x]);

            if (tmp_value < temp_min)
            {
                temp_min = tmp_value;
            }

            if (tmp_value > temp_max)
            {
                temp_max = tmp_value;
            }
        }
    }

    *min = static_cast < double >(temp_min);
    *max = static_cast < double >(temp_max);
}

int DLL_CALLCONV
FIA_CharArrayReverse (char *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_UCharArrayReverse (unsigned char *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_ShortArrayReverse (short *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_UShortArrayReverse (unsigned short *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_IntArrayReverse (int *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_UIntArrayReverse (unsigned int *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_LongArrayReverse (long *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_ULongArrayReverse (unsigned long *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_FloatArrayReverse (float *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_DoubleArrayReverse (double *array, long size)
{
    return ArrayReverse (array, size);
}

int DLL_CALLCONV
FIA_IsGreyScale (FIBITMAP * src)
{
    FREE_IMAGE_TYPE image_type = FreeImage_GetImageType (src);

    switch (image_type)
    {
        case FIT_RGB16:
        case FIT_RGBA16:
        case FIT_RGBF:
        case FIT_RGBAF:
        {
            return 0;
        }

        case FIT_BITMAP:
        {
            int bpp = FreeImage_GetBPP (src);

            if (bpp >= 16)
            {
                return 0;
            }
        }

        default:
        {
            break;
        }
    }

    return 1;
}

void DLL_CALLCONV
FIA_GetMaxPosibleValueForGreyScaleType (FREE_IMAGE_TYPE type, double *max)
{
    switch (type)
    {
        case FIT_BITMAP:
        {
            *max = UCHAR_MAX;
            break;
        }

        case FIT_UINT16:
        {
            *max = USHRT_MAX;
            break;
        }

        case FIT_INT16:
        {
            *max = SHRT_MAX;
            break;
        }

        case FIT_UINT32:
        {
            *max = UINT_MAX;
            break;
        }

        case FIT_INT32:
        {
            *max = INT_MAX;
            break;
        }

        case FIT_FLOAT:
        {
            *max = FLT_MAX;
            break;
        }

        default:
        {
            break;
        }
    }
}

void DLL_CALLCONV
FIA_GetMinPosibleValueForGreyScaleType (FREE_IMAGE_TYPE type, double *min)
{
    switch (type)
    {
        case FIT_BITMAP:
        {
            *min = 0;
            break;
        }

        case FIT_UINT16:
        {
            *min = 0;
            break;
        }

        case FIT_INT16:
        {
            *min = SHRT_MIN;
            break;
        }

        case FIT_UINT32:
        {
            *min = 0;
            break;
        }

        case FIT_INT32:
        {
            *min = INT_MIN;
            break;
        }

        case FIT_FLOAT:
        {
            *min = FLT_MIN;
            break;
        }

        default:
        {
            break;
        }
    }
}

void DLL_CALLCONV
FIA_GetMaxPosibleValueForFib (FIBITMAP * src, double *max)
{
    if (!src)
    {
        return;
    }

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit               
            *max = UCHAR_MAX;
            break;
        }
        case FIT_UINT16:
        {
            *max = USHRT_MAX;
            break;
        }
        case FIT_INT16:
        {
            *max = SHRT_MAX;
            break;
        }
        case FIT_UINT32:
        {
            *max = UINT_MAX;
            break;
        }
        case FIT_INT32:
        {
            *max = INT_MAX;
            break;
        }
        case FIT_FLOAT:
        {
            *max = FLT_MAX;
            break;
        }
        default:
        {
            break;
        }
    }
}

void DLL_CALLCONV
FIA_GetMinPosibleValueForFib (FIBITMAP * src, double *min)
{
    if (!src)
    {
        return;
    }

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            *min = 0;
            break;
        }

        case FIT_UINT16:
        {
            *min = 0;
            break;
        }

        case FIT_INT16:
        {
            *min = SHRT_MIN;
            break;
        }

        case FIT_UINT32:
        {
            *min = 0;
            break;
        }

        case FIT_INT32:
        {
            *min = INT_MIN;
            break;
        }

        case FIT_FLOAT:
        {
            *min = FLT_MIN;
            break;
        }

        default:
        {
            break;
        }
    }
}

int DLL_CALLCONV
FIA_Is16BitReally12BitImage (FIBITMAP * src)
{
    unsigned int x, width, height, *bptr;

    // Future work! Can proceed 8 pixels at once using xmm registers (128 bit registers).
    // Is Assembly Code possible in labwindows ? 

    // We want to know if there are some bits set at 0xF000 ie 1111 0000 0000 0000
    // and we will use the advantage of 32bit CPU: 
#define MASK_12BIT 0xF000F000   // 1111 0000 0000 0000   1111 0000 0000 0000
    
    if (!src || FreeImage_GetImageType (src) == FIT_BITMAP)
    {
        return 0;
    }

    if (FreeImage_GetBPP (src) != 16)
    {
        return 0;
    }

    width = FreeImage_GetWidth (src);
    height = FreeImage_GetHeight (src);

    // we need to change ptr size to 32bit 
    // bits should be aligned to 4 Bytes 
    bptr = (unsigned int *) FreeImage_GetBits (src);

    x = (width * height) / 2;   // 2 pixels at once 

    do
    {                           // this loop will skip one conditional jump 
        // let's avoid [] operator 
        if (*bptr++ & MASK_12BIT)
        {
            return 0;
        }

    }
    while (--x);

    // There can be one leftover
    if (width & 1)
    {
        if (*(short *) bptr & (short) MASK_12BIT)
        {
            return 0;
        }
    }

    return 1;
}

/* Gets the values of the pixels along a line calculated using the Midpoint Line algorithm */
template < class T > int
GetPixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, T * values)
{
    T value;
    int swapped = 0, dx, dy, abs_dy, incrN, incrE, incrNE, d, x, y, slope, tmp_y, len = 0;

    // If necessary, switch the points so we're 
    // always drawing from left to right. 
    if (p2.x < p1.x)
    {
        swapped = 1;
        SWAP (p1, p2);
    }

    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    abs_dy = abs(dy);

    if (dy < 0)
    {
        slope = -1;
        dy = -dy;
    }
    else
    {
        slope = 1;
    }

    x = p1.x;
    y = p1.y;

    if (abs_dy <= dx)
    {
        d = 2 * dy - dx;
        incrE = 2 * dy;         // E
        incrNE = 2 * (dy - dx); // NE

        T *bits = (T *) FreeImage_GetScanLine (src, y);

        value = bits[x];

        *values++ = value;

        while (x <= p2.x)
        {
            if (d <= 0)
            {
                d += incrE;     // Going to E
                x++;
            }
            else
            {
                d += incrNE;    // Going to NE
                x++;
                y += slope;
            }

            T *bits = (T *) FreeImage_GetScanLine (src, y);

            value = bits[x];

            *values++ = value;
            len++;
        }
    }
    else
    {
        len = 1;
        d = dy - 2 * dx;
        incrN = 2 * -dx;        // N
        incrNE = 2 * (dy - dx); // NE

        tmp_y = 0;

        T *bits = (T *) FreeImage_GetScanLine (src, y);

        value = bits[x];

        *values++ = value;

        for(tmp_y = 0; tmp_y < abs_dy; tmp_y++)
        {
            if (d > 0)
            {

                d += incrN;     // Going to N
                y += slope;
            }
            else
            {
                d += incrNE;    // Going to NE
                y += slope;
                x++;
            }

            T *bits = (T *) FreeImage_GetScanLine (src, y);

            value = bits[x];

            *values++ = value;
            len++;
        }
    }

    return len;
}

int DLL_CALLCONV
FIA_GetGreyScalePixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, void *values)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);
	int bpp = FreeImage_GetBPP(src);

	switch (src_type)
    {
        case FIT_BITMAP:       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
        {
            if (FreeImage_GetBPP (src) == 8)
            {
				return GetPixelValuesForLine (src, p1, p2, (BYTE*) values);
            }

            break;
        }
        
        case FIT_UINT16:       // array of unsigned short: unsigned 16-bit
        {
            return GetPixelValuesForLine (src, p1, p2, (unsigned short*) values);
        }
        
        case FIT_INT16:        // array of short: signed 16-bit
        {
            return GetPixelValuesForLine (src, p1, p2, (short*) values);
        }
        
        case FIT_UINT32:       // array of unsigned long: unsigned 32-bit
        {
            return GetPixelValuesForLine (src, p1, p2, (unsigned int*) values);
        }
        
        case FIT_INT32:        // array of long: signed 32-bit
        {
            return GetPixelValuesForLine (src, p1, p2, (int*) values);
        }

        case FIT_FLOAT:        // array of float: 32-bit
        {
            return GetPixelValuesForLine (src, p1, p2, (float*) values);
        }

        case FIT_DOUBLE:       // array of double: 64-bit
        {
            return GetPixelValuesForLine (src, p1, p2, (double*) values);
        }

        case FIT_COMPLEX:      // array of FICOMPLEX: 2 x 64-bit
        {
            break;
        }

        default:
        {
            break;
        }
    }

    return FIA_ERROR; 
}

int DLL_CALLCONV
FIA_GetCharPixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, char *values)
{
    return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FIA_GetUCharPixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, unsigned char *values)
{
    return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FIA_GetShortPixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, short *values)
{
    return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FIA_GetUShortPixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, unsigned short *values)
{
    return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FIA_GetFloatPixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, float *values)
{
    return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FIA_GetDoublePixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, double *values)
{
    return GetPixelValuesForLine (src, p1, p2, values);
}

/* Midpoint Line algorithm */
int DLL_CALLCONV
FIA_GetRGBPixelValuesForLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2,
                              BYTE * red_values, BYTE * green_values, BYTE * blue_values)
{
    RGBQUAD value;
    int dx, dy, abs_dy, incrN, incrE, incrNE, d, x, y, slope, tmp_y, len = 0;

    // If necessary, switch the points so we're 
    // always drawing from left to right. 
    if (p2.x < p1.x)
    {
        int t;

        t = p2.x;
        p2.x = p1.x;
        p1.x = t;

        t = p2.y;
        p2.y = p1.y;
        p1.y = t;
    }

    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
	abs_dy = abs(dy);

    if (dy < 0)
    {
        slope = -1;
        dy = -dy;
    }
    else
    {
        slope = 1;
    }

    x = p1.x;
    y = p1.y;

    if (abs_dy <= dx)
    {
        d = 2 * dy - dx;
        incrE = 2 * dy;         // E
        incrNE = 2 * (dy - dx); // NE

        FreeImage_GetPixelColor (src, x, y, &value);
        *red_values++ = value.rgbRed;
        *green_values++ = value.rgbGreen;
        *blue_values++ = value.rgbBlue;

		x++;
		len++;

        while (x <= p2.x)
        {
            if (d <= 0)
            {
                d += incrE;     // Going to E
                x++;
            }
            else
            {
                d += incrNE;    // Going to NE
                x++;
                y += slope;
            }

            FreeImage_GetPixelColor (src, x, y, &value);
            *red_values++ = value.rgbRed;
            *green_values++ = value.rgbGreen;
            *blue_values++ = value.rgbBlue;

            len++;
        }
    }
    else
    {
        d = dy - 2 * dx;
        incrN = 2 * -dx;        // N
        incrNE = 2 * (dy - dx); // NE

        tmp_y = 0;

        FreeImage_GetPixelColor (src, x, y, &value);
        *red_values++ = value.rgbRed;
        *green_values++ = value.rgbGreen;
        *blue_values++ = value.rgbBlue;

		len++;

        for(tmp_y = 0; tmp_y < abs_dy; tmp_y++)
        {
            if (d > 0)
            {
                d += incrN;     // Going to N
                y += slope;
            }
            else
            {
                d += incrNE;    // Going to NE
                y += slope;
                x++;
            }

            FreeImage_GetPixelColor (src, x, y, &value);
            *red_values++ = value.rgbRed;
            *green_values++ = value.rgbGreen;
            *blue_values++ = value.rgbBlue;

            len++;
        }
    }

    return len;
}

// Find two intersecting rects
int DLL_CALLCONV
FIA_IntersectingRect(FIARECT r1, FIARECT r2, FIARECT *r3)
{
    int fIntersect = (r2.left <= r1.right && r2.right >= r1.left &&
            r2.top <= r1.bottom && r2.bottom >= r1.top);

	MakeFIARect(0,0,0,0);

    if(fIntersect)
    {
#ifdef WIN32

		r3->left = max(r1.left, r2.left);
        r3->top = max(r1.top, r2.top);
        r3->right = min(r1.right, r2.right);
        r3->bottom = min(r1.bottom, r2.bottom);
#else
        r3->left = std::max(r1.left, r2.left);
        r3->top = std::max(r1.top, r2.top);
        r3->right = std::min(r1.right, r2.right);
        r3->bottom = std::min(r1.bottom, r2.bottom);
#endif
	}

    return fIntersect;
}

static FIARECT SetRectRelativeToPoint(FIARECT rect, FIAPOINT pt)
{
	FIARECT r;

	r.left = rect.left - pt.x;
	r.right = rect.right - pt.x;
	r.top = rect.top - pt.y;
	r.bottom = rect.bottom - pt.y;

	return r;
}

FIARECT DLL_CALLCONV
FIA_MakeFiaRectRelativeToImageBottomLeft (FIBITMAP *src, FIARECT rt)
{
	FIARECT rect = rt;
	int height =  FreeImage_GetHeight(src);
	rect.top = height - 1 - rt.top;
	rect.bottom = height - 1 - rt.bottom;

	return rect;	
}

BYTE* DLL_CALLCONV
FIA_GetScanLineFromTop (FIBITMAP *src, int line)
{
	return (BYTE*) FreeImage_GetScanLine(src, FreeImage_GetHeight(src) - 1 - line);	
}

int DLL_CALLCONV
FIA_PasteFromTopLeft (FIBITMAP * dst, FIBITMAP * src, int left, int top)
{
	int lines, dst_start = 0;
	int src_width = FreeImage_GetWidth (src);
    int src_height = FreeImage_GetHeight (src);
	int dst_width = FreeImage_GetWidth (dst);
    int dst_height = FreeImage_GetHeight (dst);
    int dst_pitch = FreeImage_GetPitch (dst);
	FIARECT src_rect, intersect_rect;
    FIAPOINT pt;

	if (FreeImage_GetImageType (dst) != FreeImage_GetImageType (src))
    {
		FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Destination and src image are not of the same type");
        return FIA_ERROR;
    }

    int src_line_bytes = FreeImage_GetLine (src);

    // calculate the number of bytes per pixel
    int bytespp = FreeImage_GetLine (dst) / FreeImage_GetWidth (dst);

	if(FIA_IntersectingRect(MakeFIARect(0, 0, dst_width - 1, dst_height - 1),
		MakeFIARect(left, top, left + src_width - 1, top + src_height - 1), &intersect_rect) == 0)
	{
		FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Destination and src image do not intersect");
		return FIA_ERROR;
	}

	pt.x = left;
	pt.y = top;
	src_rect = SetRectRelativeToPoint(intersect_rect, pt);

	dst_start = bytespp * intersect_rect.left;
    src_line_bytes = bytespp * (intersect_rect.right - intersect_rect.left + 1);

    BYTE *dst_bits = NULL;
    BYTE *src_bits;

	lines = intersect_rect.bottom - intersect_rect.top + 1;

    for(int i = 0; i < lines; i++)
    {
        src_bits = FIA_GetScanLineFromTop (src, src_rect.top + i) + (src_rect.left * bytespp);
		dst_bits = FIA_GetScanLineFromTop (dst, intersect_rect.top + i) + dst_start;
        memcpy (dst_bits, src_bits, src_line_bytes);
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_Paste (FIBITMAP * dst, FIBITMAP * src, int left, int bottom)
{
	return FIA_PasteFromTopLeft (dst, src, left,
		FreeImage_GetHeight(dst) - bottom - FreeImage_GetHeight(src));
}

FIBITMAP* DLL_CALLCONV
FIA_Copy ( FIBITMAP * src, int left, int top, int right, int bottom)
{
	// I have assumed clients want to copy the pixel of left and right
	// This is different to FreeImage_Copy which
	// assumes the returned bitmap is defined by a width equal to (right - left)
	right += 1;
	bottom += 1;

    int max_right = FreeImage_GetWidth(src);
    int max_bottom = FreeImage_GetHeight(src);

    if(left < 0)
        left = 0;

    if(top < 0)
        top = 0;

    if(right > max_right)
        right = max_right;

    if(bottom > max_bottom)
        bottom = max_bottom;

    return FreeImage_Copy(src, left, top, right, bottom);
}


// I have called this FastCopy for now for backwood compatibilty.
// I will change this though
FIBITMAP* DLL_CALLCONV
FIA_FastCopy ( FIBITMAP * src, int left, int top, int right, int bottom)
{
    int max_right = FreeImage_GetWidth(src);
    int max_bottom = FreeImage_GetHeight(src);

    if(left < 0)
        left = 0;

    if(top < 0)
        top = 0;

    if(right > max_right)
        right = max_right;

    if(bottom > max_bottom)
        bottom = max_bottom;

	int width = right - left;
	int height = bottom - top;

	int bpp = FreeImage_GetBPP(src);
	int bytes_per_pixel = bpp / 8;
	int left_offset = bytes_per_pixel * left;
	int width_in_bytes = width * bytes_per_pixel;

	FIBITMAP *tmp = FIA_CloneImageType(src, width, height);

	BYTE *dst_ptr;
	BYTE *src_ptr;

	for(register int y = 0; y < height; y++)
	{
		src_ptr = (BYTE*) FIA_GetScanLineFromTop (src, y + top);
		dst_ptr = (BYTE*) FIA_GetScanLineFromTop (tmp, y);
        
		memcpy(dst_ptr, src_ptr + left_offset, width_in_bytes);
	}

	return tmp;
}

void DLL_CALLCONV
FIA_RectChangeWidthHeight (FIARECT *rect, int width, int height)
{
	rect->right = rect->left + width - 1;
	rect->bottom = rect->top + height - 1;
}

int DLL_CALLCONV
FIA_RectWidth (FIARECT *rect)
{
	return rect->right - rect->left + 1;
}

int DLL_CALLCONV
FIA_RectHeight (FIARECT *rect)
{
	return rect->bottom - rect->top + 1;
}

FIBITMAP* DLL_CALLCONV
FIA_CopyLeftTopWidthHeight ( FIBITMAP * src, int left, int top, int width, int height)
{
	return FIA_Copy (src, left, top, left + width - 1, top + height - 1);
}

int DLL_CALLCONV
FIA_BitwiseCompare (FIBITMAP * dib1, FIBITMAP * dib2)
{
    if (!dib1 || !dib2)
    {
        return 0;
    }

    if (FreeImage_GetBPP (dib1) != FreeImage_GetBPP (dib2))
    {
        return 0;
    }

    if (FreeImage_GetImageType (dib1) != FreeImage_GetImageType (dib2))
    {
        return 0;
    }

    if (FreeImage_GetWidth (dib1) != FreeImage_GetWidth (dib2))
    {
        return 0;
    }

    if (FreeImage_GetHeight (dib1) != FreeImage_GetHeight (dib2))
    {
        return 0;
    }

    if (FreeImage_GetPitch (dib1) != FreeImage_GetPitch (dib2))
    {
        return 0;
    }

    int size = (FreeImage_GetPitch (dib1) * FreeImage_GetHeight (dib1)) / 4;

    int *ptr1 = (int *) FreeImage_GetBits (dib1);
    int *ptr2 = (int *) FreeImage_GetBits (dib2);

    for(int i = 0; i < size; i++)
    {
        if (*ptr1++ != *ptr2++)
        {
            return 0;
        }
    }

    return 1;
}

FIBITMAP *DLL_CALLCONV
FIA_CloneImageType (FIBITMAP * src, int width, int height)
{
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);
    int bpp = FreeImage_GetBPP (src);
    unsigned red_mask = FreeImage_GetRedMask (src);
    unsigned green_mask = FreeImage_GetGreenMask (src);
    unsigned blue_mask = FreeImage_GetBlueMask (src);

    FIBITMAP *dst = FreeImage_AllocateT (type, width, height, bpp, red_mask,
                                         green_mask, blue_mask);

    // If 8bit image we must clone the palette
    if (bpp <= 8)
    {
        FIA_CopyPalette (src, dst);
    }

    return dst;
}

void DLL_CALLCONV
FIA_Unload (FIABITMAP * src)
{
    FreeImage_Unload (src->fib);
    src->fib = NULL;
    free (src);
}

template <class Tsrc> FIBITMAP*
TemplateImageFunctionClass <Tsrc>::ConvertToGreyscaleFloatTypeWithUntouchedRange (FIBITMAP *src, FREE_IMAGE_TYPE type)
{
    if (src == NULL)
        return NULL;

	if (type != FIT_FLOAT && type != FIT_DOUBLE)
    {
        return NULL;
    }

	FIBITMAP *tmp = NULL;

	if(!FIA_IsGreyScale(src))
		tmp = FreeImage_ConvertTo8Bits(src);
	else
		tmp = FreeImage_Clone(src);

    int width = FreeImage_GetWidth (tmp);
    int height = FreeImage_GetHeight (tmp);

	FIBITMAP *dst = NULL;
    Tsrc *src_ptr;
    
	if(type == FIT_FLOAT)
	{
		float *dst_ptr;
		dst = FreeImage_AllocateT(FIT_FLOAT, width, height, 32, 0, 0, 0);  

		for(register int y = 0; y < height; y++)
		{
			src_ptr = (Tsrc*) FreeImage_GetScanLine (tmp, y);
			dst_ptr = (float*) FreeImage_GetScanLine (dst, y);
        
			for(register int x = 0; x < width; x++)
				dst_ptr[x] = (float) (src_ptr[x]);
		}
	}
	else
	{
		double *dst_ptr;
		dst = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);  

		for(register int y = 0; y < height; y++)
		{
			src_ptr = (Tsrc*) FreeImage_GetScanLine (tmp, y);
			dst_ptr = (double*) FreeImage_GetScanLine (dst, y);
        
			for(register int x = 0; x < width; x++)
				dst_ptr[x] = (double) (src_ptr[x]);
		}
	}

	FreeImage_Unload(tmp);

    return dst;
}


FIBITMAP *DLL_CALLCONV
FIA_ConvertToGreyscaleFloatType(FIBITMAP *src, FREE_IMAGE_TYPE type)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

    switch (src_type)
    {
        case FIT_BITMAP:
             return UCharImage.ConvertToGreyscaleFloatTypeWithUntouchedRange (src, type);
        case FIT_UINT16:
            return UShortImage.ConvertToGreyscaleFloatTypeWithUntouchedRange (src, type);
        case FIT_INT16:
            return ShortImage.ConvertToGreyscaleFloatTypeWithUntouchedRange (src, type);
        case FIT_UINT32:
            return ULongImage.ConvertToGreyscaleFloatTypeWithUntouchedRange (src, type);
        case FIT_INT32:
            return LongImage.ConvertToGreyscaleFloatTypeWithUntouchedRange (src, type);
		case FIT_FLOAT:
		case FIT_DOUBLE:
			return FreeImage_ConvertToType(src, type, 0);    
        default:
            break;
    }

    return NULL;
}

int DLL_CALLCONV
FIA_InPlaceConvertToGreyscaleFloatType(FIBITMAP **src, FREE_IMAGE_TYPE type)
{
	FIBITMAP *dst = FIA_ConvertToGreyscaleFloatType (*src, type);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}


FIBITMAP *DLL_CALLCONV
FIA_ConvertToGreyscaleFloatTypeAndScaleToUnity (FIBITMAP * src, FREE_IMAGE_TYPE type)
{
    if (src == NULL || (type != FIT_FLOAT && type != FIT_DOUBLE))
    {
        return NULL;
    }

    FIBITMAP *gs_dib = NULL, *dst = NULL;

    int bpp = FreeImage_GetBPP (src);

    if (bpp >= 24 && FreeImage_GetImageType (src) == FIT_BITMAP)
    {                           // Colour
        gs_dib = FreeImage_ConvertToGreyscale (src);
    }
    else if (bpp < 8)
    {
        gs_dib = FreeImage_ConvertTo8Bits (src);
    }
    else
    {
        gs_dib = FreeImage_Clone (src);
    }

    if (gs_dib != NULL)
    {
        dst = FreeImage_ConvertToType (gs_dib, type, 0);
        FreeImage_Unload (gs_dib);
        return dst;
    }

    return NULL;
}


int DLL_CALLCONV
FIA_InPlaceConvertToGreyscaleFloatTypeAndScaleToUnity (FIBITMAP ** src, FREE_IMAGE_TYPE type)
{
    FIBITMAP *dst = FIA_ConvertToGreyscaleFloatType (*src, type);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_Is8Bit (FIBITMAP * src)
{
    if (FreeImage_GetBPP (src) == 8 && FreeImage_GetImageType (src) == FIT_BITMAP)
    {
        return 1;
    }

    return 0;
}

void
CheckMemory (void *ptr)
{
    if (ptr == NULL)
    {

        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Memory allocation failed this is most likely a freeimagealgorithms bug.");
        exit (-1);
    }
}

int DLL_CALLCONV
FIA_GetPixelValue (FIBITMAP * src, int x, int y, double *val)
{
    int bpp = FreeImage_GetBPP (src);
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    BYTE *line_ptr = FreeImage_GetScanLine (src, y);

    // bpp >> 3;   // Divide by 8
    switch (bpp)
    {
        case 8:
        {
            *val = *(line_ptr + x);
            return FIA_SUCCESS;
        }

        case 16:
        {
            if (type == FIT_INT16)
                *val = *((short *) line_ptr + x);
            else if (type == FIT_UINT16)
                *val = *((unsigned short *) line_ptr + x);

            return FIA_SUCCESS;
        }

        case 32:
        {
            if (type == FIT_FLOAT)
                *val = *((float *) line_ptr + x);
			else if(type == FIT_INT32)
				*val = *((int *) line_ptr + x);
			else if(type == FIT_UINT32)
				*val = *((unsigned int *) line_ptr + x);

            return FIA_SUCCESS;
        }

        case 64:
        {
            if (type == FIT_DOUBLE)
                *val = *((double *) line_ptr + x);

            return FIA_SUCCESS;
        }
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_SetPixelValue (FIBITMAP * src, int x, int y, double val)
{
	if (src==NULL || !FreeImage_HasPixels(src))
		return FIA_ERROR;

	if (x<0 || x>=FreeImage_GetWidth(src))
		return FIA_ERROR;
	if (y<0 || y>=FreeImage_GetHeight(src))
		return FIA_ERROR;
    
	int bpp = FreeImage_GetBPP (src);
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    BYTE *line_ptr = FreeImage_GetScanLine (src, y);

    // bpp >> 3;   // Divide by 8
    switch (bpp)
    {
        case 8:
        {
            *(line_ptr + x) = val;
            return FIA_SUCCESS;
        }

        case 16:
        {
            if (type == FIT_INT16)
                *((short *) line_ptr + x) = val;
            else if (type == FIT_UINT16)
                *((unsigned short *) line_ptr + x) = val;

            return FIA_SUCCESS;
        }

        case 32:
        {
            if (type == FIT_FLOAT)
                *((float *) line_ptr + x) = val;
			else if(type == FIT_INT32)
				*((int *) line_ptr + x) = val;
			else if(type == FIT_UINT32)
				*((unsigned int *) line_ptr + x) = val;

            return FIA_SUCCESS;
        }

        case 64:
        {
            if (type == FIT_DOUBLE)
                *((double *) line_ptr + x) = val;

            return FIA_SUCCESS;
        }
    }

    return FIA_ERROR;
}


int DLL_CALLCONV
FIA_GetPixelValueFromTopLeft (FIBITMAP *src, int x, int y, double *val)
{
    y = FreeImage_GetHeight(src) - y - 1;

    return FIA_GetPixelValue (src, x, y, val);
}

int DLL_CALLCONV
FIA_SetPixelIndexFromTopLeft (FIBITMAP *src, int x, int y, BYTE val)
{
    y = FreeImage_GetHeight(src) - y - 1;

	return FreeImage_SetPixelIndex(src, x, y, &val);
}

int DLL_CALLCONV
FIA_GetPixelColour (FIBITMAP * src, int x, int y, RGBQUAD *val)
{
	return FreeImage_GetPixelColor(src, x, y, val);
}

int DLL_CALLCONV
FIA_SetPixelColour (FIBITMAP *src, int x, int y, RGBQUAD val)
{
	return FreeImage_SetPixelColor(src, x, y, &val);
}

int DLL_CALLCONV
FIA_SetPixelColourFromTopLeft (FIBITMAP *src, int x, int y, RGBQUAD val)
{
	y = FreeImage_GetHeight(src) - y - 1;

	return FIA_SetPixelColour(src, x, y, val);
}

int DLL_CALLCONV
FIA_GetPixelColourFromTopLeft (FIBITMAP *src, int x, int y, RGBQUAD *val)
{
	y = FreeImage_GetHeight(src) - y - 1;

	return FreeImage_GetPixelColor(src, x, y, val);
}

int DLL_CALLCONV
FIA_InPlaceConvertTo8Bit (FIBITMAP ** src)
{
    FIBITMAP *dst = FreeImage_ConvertTo8Bits (*src);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_InPlaceConvertTo32Bit (FIBITMAP ** src)
{
    FIBITMAP *dst = FreeImage_ConvertTo32Bits (*src);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_InPlaceConvertTo24Bit (FIBITMAP ** src)
{
    FIBITMAP *dst = FreeImage_ConvertTo24Bits (*src);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_InPlaceConvertToStandardType (FIBITMAP ** src, int scale)
{
    FIBITMAP *dst = FreeImage_ConvertToStandardType (*src, scale);

    if(FreeImage_GetBPP(*src) == 8)
        FIA_CopyPalette(*src, dst);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}


int DLL_CALLCONV
FIA_InPlaceConvertToGreyscale (FIBITMAP ** src)
{
    FIBITMAP *dst = FreeImage_ConvertToGreyscale (*src);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_InPlaceConvertToInt32Type (FIBITMAP ** src, int scale)
{
    FIBITMAP *dst = FreeImage_ConvertToType (*src, FIT_INT32, 0);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}

FIBITMAP *DLL_CALLCONV
FIA_ConvertFloatTo16Bit (FIBITMAP * src, int sign)
{
    if (src == NULL)
        return NULL;

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    // Mask has to be 8 bit 
    if (type != FIT_FLOAT)
        return NULL;

    type = FIT_UINT16;

    if(sign)
        type = FIT_INT16;

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FIBITMAP *dst = FreeImage_AllocateT (type, width, height, 16, 0, 0, 0);

    double factor = 1.0, min, max, min16 = 0.0, max16;
    FIA_FindMinMax(src, &min, &max);
    FIA_GetMinPosibleValueForFib(dst, &min16);
    FIA_GetMaxPosibleValueForFib(dst, &max16);

    factor = (max16 - min16) / (max - min);
   
    if (sign)
    {      
        for(register int y = 0; y < height; y++)
        {
            float *src_ptr = (float *) FreeImage_GetScanLine (src, y);
            short *dst_ptr = (short *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (short) (src_ptr[x] * factor + min16);
        }
    }
    else
    {
        for(register int y = 0; y < height; y++)
        {
            float *src_ptr = (float *) FreeImage_GetScanLine (src, y);
            unsigned short *dst_ptr = (unsigned short *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (unsigned short) (src_ptr[x] * factor + min16);
        }
    }

    return dst;
}

int DLL_CALLCONV
FIA_InPlaceConvertInt16ToUInt16 (FIBITMAP ** src)
{
    FIBITMAP *dst = FIA_ConvertInt16ToUInt16 (*src);

    FreeImage_Unload (*src);
    *src = dst;

    return FIA_SUCCESS;
}

FIBITMAP *DLL_CALLCONV
FIA_ConvertInt16ToUInt16 (FIBITMAP * src)
{
    if (src == NULL)
        return NULL;

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    // Mask has to be 8 bit 
    if (type != FIT_INT16)
        return NULL;

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FIBITMAP *dst = FreeImage_AllocateT (FIT_UINT16, width, height, 16, 0, 0, 0);

    unsigned short factor = -SHRT_MIN;

    for(register int y = 0; y < height; y++)
    {
        short *src_ptr = (short *) FreeImage_GetScanLine (src, y);
        unsigned short *dst_ptr = (unsigned short *) FreeImage_GetScanLine (dst,
                                                                            y);

        for(register int x = 0; x < width; x++)
            dst_ptr[x] = (unsigned short) (src_ptr[x] + factor);
    }

    return dst;
}


FIBITMAP *DLL_CALLCONV
FIA_Convert48BitOr64BitRGBTo24BitColour(FIBITMAP * src)
{
    if (src == NULL)
        return NULL;

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    if (type != FIT_RGB16 && type != FIT_RGBA16)
        return NULL;

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FIBITMAP *dst = FreeImage_AllocateT (FIT_BITMAP, width, height, 24, 0, 0, 0);

    int bytespp = FreeImage_GetLine (src) / FreeImage_GetWidth (src);
    int channels = bytespp / 2;
    double factor = 255.0 / 65535.0;

    unsigned short *src_pixel_ptr = NULL;
    BYTE *dst_pixel_ptr = NULL;

    for(register int y = 0; y < height; y++)
    {
        unsigned short *src_ptr = (unsigned short *) FreeImage_GetScanLine (src, y);
        BYTE *dst_ptr = (BYTE *) FreeImage_GetScanLine (dst, y);

        for(register int x = 0; x < width; x++) {

            src_pixel_ptr = &src_ptr[channels * x];
            dst_pixel_ptr = &dst_ptr[3 * x];

            dst_pixel_ptr[FI_RGBA_RED] = (BYTE) (src_pixel_ptr[FI_RGBA_BLUE] * factor);
            dst_pixel_ptr[FI_RGBA_GREEN] = (BYTE) (src_pixel_ptr[FI_RGBA_GREEN] * factor);
            dst_pixel_ptr[FI_RGBA_BLUE] = (BYTE) (src_pixel_ptr[FI_RGBA_RED] * factor);

            if (channels == 4)
                dst_pixel_ptr[FI_RGBA_ALPHA] = (BYTE) (src_pixel_ptr[FI_RGBA_ALPHA] * factor);
        }
    }

    return dst;
}

template <class Tsrc> FIBITMAP*
TemplateImageFunctionClass <Tsrc>::FIA_ConvertFloatTypeToImageType (FIBITMAP *src, FREE_IMAGE_TYPE float_type, FREE_IMAGE_TYPE type, BOOL scale_linear)
{
	if(float_type != FIT_FLOAT && float_type != FIT_DOUBLE) 
		return NULL;

    double src_min_found;
    double src_max_found;
	double min_possible_for_type;
	double max_possible_for_type;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	//unsigned int src_pitch_in_pixels = FIA_GetPitchInPixels(src);
	//unsigned int dst_pitch_in_pixels = FIA_GetPitchInPixels(src);

	FIA_GetMinPosibleValueForGreyScaleType (type, &min_possible_for_type);
	FIA_GetMaxPosibleValueForGreyScaleType (type, &max_possible_for_type);

	FIBITMAP *dst = FreeImage_AllocateT(type, width, height, 32, 0, 0, 0);  
	Tsrc *dst_ptr = NULL;

	if(float_type == FIT_FLOAT)
	{
		float *src_ptr = NULL;

		if(scale_linear)
		{
			FIA_FindMinMax (src, &src_min_found, &src_max_found);
			double src_range = src_max_found - src_min_found;
			double dst_range = max_possible_for_type - min_possible_for_type;
			double factor = dst_range / src_range;

			for(register int y = 0; y < height; y++)
			{
				src_ptr = (float*) FreeImage_GetScanLine (src, y);
				dst_ptr = (Tsrc*) FreeImage_GetScanLine (dst, y);
        
				for(register int x = 0; x < width; x++) {
					 dst_ptr[x] = static_cast <Tsrc> ((src_ptr[x] - src_min_found) * factor + min_possible_for_type);
				}
			}
		}
		else
		{
			for(register int y = 0; y < height; y++)
			{
				src_ptr = (float*) FreeImage_GetScanLine (src, y);
				dst_ptr = (Tsrc*) FreeImage_GetScanLine (dst, y);
        
				for(register int x = 0; x < width; x++) {
					#ifdef WIN32
					dst_ptr[x] = static_cast <Tsrc> (max(min(max_possible_for_type, src_ptr[x]), min_possible_for_type));
					#else
					dst_ptr[x] = static_cast <Tsrc> ( (Tsrc) std::max(std::min( (Tsrc) max_possible_for_type, (Tsrc) src_ptr[x]),
                                                      (Tsrc) min_possible_for_type));
					#endif
				}
			}
		}
	}
	else if(float_type == FIT_DOUBLE)
	{ 
		double *src_ptr = NULL;

		if(scale_linear)
		{
			FIA_FindMinMax (src, &src_min_found, &src_max_found);
			double src_range = src_max_found - src_min_found;
			double dst_range = max_possible_for_type - min_possible_for_type;
			double factor = dst_range / src_range;

			for(register int y = 0; y < height; y++)
			{
				src_ptr = (double*) FreeImage_GetScanLine (src, y);
				dst_ptr = (Tsrc*) FreeImage_GetScanLine (dst, y);
        
				for(register int x = 0; x < width; x++) {
					 dst_ptr[x] = static_cast <Tsrc> ((src_ptr[x] - src_min_found) * factor + min_possible_for_type);
				}
			}
		}
		else
		{
			for(register int y = 0; y < height; y++)
			{
				src_ptr = (double*) FreeImage_GetScanLine (src, y);
				dst_ptr = (Tsrc*) FreeImage_GetScanLine (dst, y);
        
				for(register int x = 0; x < width; x++) {
					#ifdef WIN32
					dst_ptr[x] = static_cast <Tsrc> (max(min(max_possible_for_type, src_ptr[x]), min_possible_for_type));
					#else
					dst_ptr[x] = static_cast <Tsrc> (std::max(std::min(max_possible_for_type, src_ptr[x]), min_possible_for_type));
					#endif
				}
			}
		}
	}

    return dst;
}

FIBITMAP *DLL_CALLCONV
FIA_ConvertFloatTypeToType(FIBITMAP *src, FREE_IMAGE_TYPE type, BOOL scale_linear)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);
	int bpp = FreeImage_GetBPP(src);

    switch (type)
    {
        case FIT_BITMAP:
			if(bpp = 8)
				return UCharImage.FIA_ConvertFloatTypeToImageType (src, src_type, type, scale_linear);
        case FIT_UINT16:
            return UShortImage.FIA_ConvertFloatTypeToImageType (src, src_type, type, scale_linear);
        case FIT_INT16:
            return ShortImage.FIA_ConvertFloatTypeToImageType (src, src_type, type, scale_linear);
        case FIT_UINT32:
            return ULongImage.FIA_ConvertFloatTypeToImageType (src, src_type, type, scale_linear);
        case FIT_INT32:
            return LongImage.FIA_ConvertFloatTypeToImageType (src, src_type, type, scale_linear);
        default:
            break;
    }

    return NULL;
}

template < typename Tsrc > FIBITMAP * TemplateImageFunctionClass <
    Tsrc >::IntegerRescaleToHalf (FIBITMAP * src)
{
    int src_width = FreeImage_GetWidth (src);
    int src_height = FreeImage_GetHeight (src);

    int dst_width = src_width / 2;
    int dst_height = src_height / 2;

    FIBITMAP *dst = FIA_CloneImageType (src, dst_width, dst_height);

    long average;
    int startx = 0, starty = 0, startxplus1;

    for(register int y = 0; y < dst_height; y++)
    {
        Tsrc *dst_ptr = (Tsrc *) FreeImage_GetScanLine (dst, y);

        starty = y * 2;

        Tsrc *src_ptr = (Tsrc *) FreeImage_GetScanLine (src, starty);
        Tsrc *src_next_line_ptr = (Tsrc *) FreeImage_GetScanLine (src, starty + 1);

        for(register int x = 0; x < dst_width; x++)
        {
            startx = x * 2;
            startxplus1 = startx + 1;

            average = src_ptr[startx] + src_ptr[startxplus1]
                + src_next_line_ptr[startx] + src_next_line_ptr[startxplus1];

            dst_ptr[x] = (Tsrc) (average >> 2);
        }
    }

    return dst;
}

static FIBITMAP *
ColourRescaleToHalf (FIBITMAP * src)
{
    int src_width = FreeImage_GetWidth (src);
    int src_height = FreeImage_GetHeight (src);

    int dst_width = src_width / 2;
    int dst_height = src_height / 2;

    FIBITMAP *dst = FIA_CloneImageType (src, dst_width, dst_height);

    int startx = 0, starty = 0, startxplus1;
    int dstx;

    // Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit) 
    int bytespp = FreeImage_GetLine (src) / src_width;
    register int tmp, tmp2;

    for(register int y = 0; y < dst_height; y++)
    {
        BYTE *dst_ptr = (BYTE *) FreeImage_GetScanLine (dst, y);

        starty = y * 2;

        BYTE *src_ptr = (BYTE *) FreeImage_GetScanLine (src, starty);
        BYTE *src_next_line_ptr = (BYTE *) FreeImage_GetScanLine (src, starty + 1);

        for(register int x = 0; x < dst_width; x++)
        {

            dstx = x * bytespp;
            startx = dstx * 2;
            startxplus1 = startx + bytespp;

            tmp = startx + FI_RGBA_RED;
            tmp2 = startxplus1 + FI_RGBA_RED;
            dst_ptr[dstx + FI_RGBA_RED] = ((src_ptr[tmp] + src_ptr[tmp2]
                                            + src_next_line_ptr[tmp] + src_next_line_ptr[tmp2])
                                           >> 2);

            tmp = startx + FI_RGBA_GREEN;
            tmp2 = startxplus1 + FI_RGBA_GREEN;
            dst_ptr[dstx + FI_RGBA_GREEN] = ((src_ptr[tmp] + src_ptr[tmp2]
                                              + src_next_line_ptr[tmp] + src_next_line_ptr[tmp2])
                                             >> 2);

            tmp = startx + FI_RGBA_BLUE;
            tmp2 = startxplus1 + FI_RGBA_BLUE;
            dst_ptr[dstx + FI_RGBA_BLUE] = ((src_ptr[tmp] + src_ptr[tmp2]
                                             + src_next_line_ptr[tmp] + src_next_line_ptr[tmp2])
                                            >> 2);

            if (bytespp == 4)
            {

                tmp = startx + FI_RGBA_ALPHA;
                tmp2 = startxplus1 + FI_RGBA_ALPHA;
                dst_ptr[dstx + FI_RGBA_ALPHA] = ((src_ptr[tmp] + src_ptr[tmp2]
                                                  + src_next_line_ptr[tmp]
                                                  + src_next_line_ptr[tmp2]) >> 2);
            }
        }
    }

    return dst;
}

template < typename Tsrc > FIBITMAP * TemplateImageFunctionClass <
    Tsrc >::FloatRescaleToHalf (FIBITMAP * src)
{
    int src_width = FreeImage_GetWidth (src);
    int src_height = FreeImage_GetHeight (src);

    int dst_width = src_width / 2;
    int dst_height = src_height / 2;

    FIBITMAP *dst = FIA_CloneImageType (src, dst_width, dst_height);

    double average;
    int startx = 0, starty = 0, startxplus1;

    for(register int y = 0; y < dst_height; y++)
    {
        Tsrc *dst_ptr = (Tsrc *) FreeImage_GetScanLine (dst, y);

        starty = y * 2;

        Tsrc *src_ptr = (Tsrc *) FreeImage_GetScanLine (src, starty);
        Tsrc *src_next_line_ptr = (Tsrc *) FreeImage_GetScanLine (src, starty + 1);

        for(register int x = 0; x < dst_width; x++)
        {
            startx = x * 2;
            startxplus1 = startx + 1;

            average = src_ptr[startx] + src_ptr[startxplus1]
                + src_next_line_ptr[startx] + src_next_line_ptr[startxplus1];

            dst_ptr[x] = (Tsrc) (average / 4.0);
        }
    }

    return dst;
}


FIBITMAP *DLL_CALLCONV
FIA_RescaleToHalf (FIBITMAP * src)
{
    FIBITMAP *dst = NULL;

    if (!src)
        return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
        {
            if (FreeImage_GetBPP (src) == 8)
            {
                dst = UCharImage.IntegerRescaleToHalf (src);
            }
            else
            {
                dst = ColourRescaleToHalf (src);
            }
            break;
        }

        case FIT_UINT16:       // array of unsigned short: unsigned 16-bit
        {
            dst = UShortImage.IntegerRescaleToHalf (src);
            break;
        }

        case FIT_INT16:        // array of short: signed 16-bit
        {
            dst = ShortImage.IntegerRescaleToHalf (src);
            break;
        }

        case FIT_UINT32:       // array of unsigned long: unsigned 32-bit
        {
            dst = ULongImage.IntegerRescaleToHalf (src);
            break;
        }

        case FIT_INT32:        // array of long: signed 32-bit
        {
            dst = LongImage.IntegerRescaleToHalf (src);
            break;
        }

        case FIT_FLOAT:        // array of float: 32-bit
        {
            dst = FloatImage.FloatRescaleToHalf (src);
            break;
        }

        case FIT_DOUBLE:       // array of double: 64-bit
        {
            dst = DoubleImage.FloatRescaleToHalf (src);
            break;
        }

        case FIT_COMPLEX:      // array of FICOMPLEX: 2 x 64-bit
        {
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
int DLL_CALLCONV
FIA_CheckSizesAreSame(FIBITMAP *fib1, FIBITMAP *fib2)
{
    int width1 = FreeImage_GetWidth(fib1);
    int height1 = FreeImage_GetHeight(fib1);

    int width2 = FreeImage_GetWidth(fib2);
    int height2 = FreeImage_GetHeight(fib2);

    if(width1 == width2 && height1 == height2)
        return 1;

    return 0;
}

template < typename Tsrc > FIBITMAP * TemplateImageFunctionClass <
    Tsrc >::Composite(FIBITMAP * fg, FIBITMAP * bg, FIBITMAP * normalised_alpha_values, FIBITMAP *mask)
{
	if (FreeImage_GetImageType (fg) != FreeImage_GetImageType (bg))
    {
		FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Foreground and background image are not of the same type");
        return NULL;
    }

	if (FreeImage_GetImageType (normalised_alpha_values) != FIT_FLOAT)
    {
		FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Normalised_alpha_values is not a FIT_FLOAT image");
        return NULL;
    }

	if(mask != NULL) {
		if (FreeImage_GetImageType (mask) != FIT_BITMAP)
		{
			FreeImage_OutputMessageProc (FIF_UNKNOWN,
										 "mask is not a FIT_BITMAP image");
			return NULL;
		}
	}

	if(FIA_CheckSizesAreSame(fg, bg) == 0) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Foreground and background image are not the same size");

        return NULL;
    }

	if(FIA_CheckSizesAreSame(fg, normalised_alpha_values) == 0) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Foreground and normalised_alpha_values image are not the same size");

        return NULL;
    }

	if(mask != NULL) {

		if(FIA_CheckSizesAreSame(fg, mask) == 0) {

			FreeImage_OutputMessageProc (FIF_UNKNOWN, "Foreground and mask image are not the same size");

			return NULL;
		}
	}

    int fg_width = FreeImage_GetWidth (fg);
    int fg_height = FreeImage_GetHeight (fg);

    FIBITMAP *dst = FIA_CloneImageType (fg, fg_width, fg_height);

	if(mask != NULL) {

		for(register int y = 0; y < fg_height; y++)
		{
			Tsrc *dst_ptr = (Tsrc *) FIA_GetScanLineFromTop (dst, y);
			Tsrc *fg_ptr = (Tsrc *) FIA_GetScanLineFromTop (fg, y);
			Tsrc *bg_ptr = (Tsrc *) FIA_GetScanLineFromTop (bg, y);
			BYTE *mask_ptr = (BYTE *) FIA_GetScanLineFromTop (mask, y);
			float *alpha_ptr = (float *) FIA_GetScanLineFromTop (normalised_alpha_values, y);

			for(register int x = 0; x < fg_width; x++)
			{
				if(mask_ptr[x] > 0)
					dst_ptr[x] = (Tsrc) ((alpha_ptr[x] * fg_ptr[x]) + ((1 - alpha_ptr[x]) * bg_ptr[x]));
				else
					dst_ptr[x] = (Tsrc) fg_ptr[x];
			}
		}
	}
	else {
		for(register int y = 0; y < fg_height; y++)
		{
			Tsrc *dst_ptr = (Tsrc *) FIA_GetScanLineFromTop (dst, y);
			Tsrc *fg_ptr = (Tsrc *) FIA_GetScanLineFromTop (fg, y);
			Tsrc *bg_ptr = (Tsrc *) FIA_GetScanLineFromTop (bg, y);
			float *alpha_ptr = (float *) FIA_GetScanLineFromTop (normalised_alpha_values, y);

			for(register int x = 0; x < fg_width; x++)
			{
				dst_ptr[x] = (Tsrc) ((alpha_ptr[x] * fg_ptr[x]) + ((1 - alpha_ptr[x]) * bg_ptr[x]));
			}
		}
	}

    return dst;
}


template < typename Tsrc > int TemplateImageFunctionClass
  <Tsrc >::Combine(FIBITMAP *dst, FIBITMAP *fg, FIBITMAP *mask)
{
	if (FreeImage_GetImageType (fg) != FreeImage_GetImageType (dst))
    {
		FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "Foreground and background image are not of the same type");
        return FIA_ERROR;
    }

	if(mask != NULL) {
		if (FreeImage_GetImageType (mask) != FIT_BITMAP)
		{
			FreeImage_OutputMessageProc (FIF_UNKNOWN,
										 "mask is not a FIT_BITMAP image");
			return FIA_ERROR;
		}
	}

	if(FIA_CheckSizesAreSame(fg, dst) == 0) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Foreground and background image are not the same size");

        return FIA_ERROR;
    }

	if(FIA_CheckSizesAreSame(fg, mask) == 0) {

		FreeImage_OutputMessageProc (FIF_UNKNOWN, "Foreground and mask image are not the same size");

		return FIA_ERROR;
	}

    int fg_width = FreeImage_GetWidth (fg);
    int fg_height = FreeImage_GetHeight (fg);

    bool greyscale_image = true;
    
    if(FreeImage_GetImageType(fg) == FIT_BITMAP && FreeImage_GetBPP(fg) > 8) {
	    greyscale_image = false;
    }
    
    if(greyscale_image) {
	    for(register int y = 0; y < fg_height; y++)
	    {
		    Tsrc *dst_ptr = (Tsrc *) FIA_GetScanLineFromTop (dst, y);
		    Tsrc *fg_ptr = (Tsrc *) FIA_GetScanLineFromTop (fg, y);
		    BYTE *mask_ptr = (BYTE *) FIA_GetScanLineFromTop (mask, y);

		    for(register int x = 0; x < fg_width; x++)
		    {
			    if(mask_ptr[x] > 0)
				    dst_ptr[x] = (Tsrc) fg_ptr[x];
		    }
	    }
    }
    else {
    
        int bytespp = FreeImage_GetLine (dst) / FreeImage_GetWidth (dst);
        
        for(register int y = 0; y < fg_height; y++)
	    {
		    BYTE *dst_ptr = (BYTE *) FIA_GetScanLineFromTop (dst, y);
		    BYTE *fg_ptr = (BYTE *) FIA_GetScanLineFromTop (fg, y);
		    BYTE *mask_ptr = (BYTE *) FIA_GetScanLineFromTop (mask, y);

		    for(register int x = 0; x < fg_width; x++)
		    {
			    if(mask_ptr[x] > 0) {
		
			      dst_ptr[FI_RGBA_RED] = fg_ptr[FI_RGBA_RED];
			      dst_ptr[FI_RGBA_GREEN] = fg_ptr[FI_RGBA_GREEN];
			      dst_ptr[FI_RGBA_BLUE] = fg_ptr[FI_RGBA_BLUE];
			    
			      if (bytespp == 4)
			          dst_ptr[FI_RGBA_ALPHA] = 0;   
                }
				  
				dst_ptr += bytespp;
				fg_ptr += bytespp;
		    }
	    }
    }
    
    return FIA_SUCCESS;
}


FIBITMAP *DLL_CALLCONV
FIA_Composite(FIBITMAP * fg, FIBITMAP * bg, FIBITMAP * normalised_alpha_values, FIBITMAP *mask)
{
    FIBITMAP *dst = NULL;

    if (!fg)
        return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (fg);

    switch (src_type)
    {
        case FIT_BITMAP:       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
        {
            if (FreeImage_GetBPP (fg) == 8)
            {
                dst = UCharImage.Composite (fg, bg, normalised_alpha_values, mask);
            }
            else
            {
				// Do the composite for each channel
				FIBITMAP* red_channel_fg = FreeImage_GetChannel(fg, FICC_RED);
				FIBITMAP* red_channel_bg = FreeImage_GetChannel(bg, FICC_RED);

				FIBITMAP* green_channel_fg = FreeImage_GetChannel(fg, FICC_GREEN);
				FIBITMAP* green_channel_bg = FreeImage_GetChannel(bg, FICC_GREEN);

				FIBITMAP* blue_channel_fg = FreeImage_GetChannel(fg, FICC_BLUE);
				FIBITMAP* blue_channel_bg = FreeImage_GetChannel(bg, FICC_BLUE);

				FIBITMAP* dst1 = UCharImage.Composite (red_channel_fg, red_channel_bg, normalised_alpha_values, mask);
				FIBITMAP* dst2 = UCharImage.Composite (green_channel_fg, green_channel_bg, normalised_alpha_values, mask);
				FIBITMAP* dst3 = UCharImage.Composite (blue_channel_fg, blue_channel_bg, normalised_alpha_values, mask);

				dst = FIA_CloneImageType(fg, FreeImage_GetWidth(fg), FreeImage_GetHeight(fg));

				FreeImage_SetChannel(dst, dst1, FICC_RED);
				FreeImage_SetChannel(dst, dst2, FICC_GREEN);
				FreeImage_SetChannel(dst, dst3, FICC_BLUE);

				FreeImage_Unload(red_channel_fg);
				FreeImage_Unload(red_channel_bg);
				FreeImage_Unload(green_channel_fg);
				FreeImage_Unload(green_channel_bg);
				FreeImage_Unload(blue_channel_fg);
				FreeImage_Unload(blue_channel_bg);
				FreeImage_Unload(dst1);
				FreeImage_Unload(dst2);
				FreeImage_Unload(dst3);
            }
            break;
        }

        case FIT_UINT16:       // array of unsigned short: unsigned 16-bit
        {
            dst = UShortImage.Composite (fg, bg, normalised_alpha_values, mask);
            break;
        }

        case FIT_INT16:        // array of short: signed 16-bit
        {
            dst = ShortImage.Composite (fg, bg, normalised_alpha_values, mask);
            break;
        }

        case FIT_UINT32:       // array of unsigned long: unsigned 32-bit
        {
            dst = ULongImage.Composite (fg, bg, normalised_alpha_values, mask);
            break;
        }

        case FIT_INT32:        // array of long: signed 32-bit
        {
            dst = LongImage.Composite (fg, bg, normalised_alpha_values, mask);
            break;
        }

        case FIT_FLOAT:        // array of float: 32-bit
        {
            dst = FloatImage.Composite (fg, bg, normalised_alpha_values, mask);
            break;
        }

        case FIT_DOUBLE:       // array of double: 64-bit
        {
            dst = DoubleImage.Composite (fg, bg, normalised_alpha_values, mask);
            break;
        }

        case FIT_COMPLEX:      // array of FICOMPLEX: 2 x 64-bit
        {
            break;
        }

        default:
        {
            break;
        }
    }

    if (dst == NULL)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN,
                                     "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.",
                                     src_type, FIT_BITMAP);
    }

    return dst;
}

int DLL_CALLCONV
FIA_Combine(FIBITMAP *dst, FIBITMAP *fg, FIBITMAP *mask)
{
    if (fg == NULL || dst == NULL)
        return FIA_ERROR;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (fg);

    switch (src_type)
    {
        case FIT_BITMAP:       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
        {
            return UCharImage.Combine (dst, fg, mask);
        }

        case FIT_UINT16:       // array of unsigned short: unsigned 16-bit
        {
            return UShortImage.Combine (dst, fg, mask);
        }

        case FIT_INT16:        // array of short: signed 16-bit
        {
            return ShortImage.Combine (dst, fg, mask);
        }

        case FIT_UINT32:       // array of unsigned long: unsigned 32-bit
        {
            return ULongImage.Combine (dst, fg, mask);
        }

        case FIT_INT32:        // array of long: signed 32-bit
        {
            return LongImage.Combine (dst, fg, mask);
        }

        case FIT_FLOAT:        // array of float: 32-bit
        {
            return FloatImage.Combine (dst, fg, mask);
        }

        case FIT_DOUBLE:       // array of double: 64-bit
        {
            return DoubleImage.Combine (dst, fg, mask);
        }

        case FIT_COMPLEX:      // array of FICOMPLEX: 2 x 64-bit
        {
            break;
        }

        default:
        {
            break;
        }
    }

    FreeImage_OutputMessageProc (FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to combine.");

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_CompositeRegion(FIBITMAP * fg, FIBITMAP * bg, FIARECT rect)
{
	FIBITMAP *bg_region = FIA_Copy(bg, rect.left, rect.top, rect.right, rect.bottom);

	FIBITMAP *bg_24bit = FreeImage_ConvertTo24Bits(bg_region);

	FIBITMAP *blended = FreeImage_Composite(fg, 1, NULL, bg_24bit);

	if(blended == NULL) {

		if(FIA_CheckSizesAreSame(fg, bg_region) == 0) {

			FreeImage_OutputMessageProc (FIF_UNKNOWN, "FreeImage_Composite failed. Images not the same size."
				"fg (%dx%d) bg (%dx%d)", FreeImage_GetWidth(fg), FreeImage_GetHeight(fg),
										 FreeImage_GetWidth(bg_24bit), FreeImage_GetHeight(bg_24bit));

			goto CleanUp;
		}

	}

    FIA_PasteFromTopLeft(bg, blended, rect.left, rect.top);

	FreeImage_Unload(bg_region);
	FreeImage_Unload(bg_24bit);
	FreeImage_Unload(blended);

	return FIA_SUCCESS;

CleanUp:

	FreeImage_Unload(bg_region);
	FreeImage_Unload(bg_24bit);
	FreeImage_Unload(blended);

	return FIA_ERROR;
}

FIBITMAP *DLL_CALLCONV
FIA_MakeHatchedImage (int width, int height, int hatchType, int spacing)
{
	FIBITMAP *out;
	BYTE val=1;
	int antialias_diagonals=0;  // antialias with a value of 1 prints nothing
	int i, j;

	out = FreeImage_Allocate(width, height, 8);

	FIA_DrawSolidGreyscaleRect(out, MakeFIARect(0,0,width-1,height-1), 0.0);

	switch (hatchType)
		{
		case 1: // horiz lines
			for (j=0; j<height; j+=spacing)
				//FIA_DrawOnePixelIndexLineFromTopLeft (out, MakeFIAPoint(0, j), MakeFIAPoint(width, j), 1.0);
				FIA_DrawGreyscaleLine (out, MakeFIAPoint(0, j), MakeFIAPoint(width, j), (double)val, 1, 0);

			break;
		case 2: // vert lines
			for (i=0; i<width; i+=spacing)
				FIA_DrawGreyscaleLine (out, MakeFIAPoint(i, 0), MakeFIAPoint(i, height), (double)val, 1, 0);
			
			break;
		case 3: // horiz and vert lines
			for (j=0; j<height; j+=spacing)
				FIA_DrawGreyscaleLine (out, MakeFIAPoint(0, j), MakeFIAPoint(width, j), (double)val, 1, 0);
			for (i=0; i<width; i+=spacing)
				FIA_DrawGreyscaleLine (out, MakeFIAPoint(i, 0), MakeFIAPoint(i, height), (double)val, 1, 0);
			
			break;
		case 4:  // diag lines
			for (j=0; (j<(height+width)); j+=spacing)
				FIA_DrawGreyscaleLine (out, MakeFIAPoint(j, 0), MakeFIAPoint(0, j), (double)val, 1, antialias_diagonals);
			
			break;
		case 5:  // other diag lines
			for (j=0; j<(height+width); j+=spacing)
				FIA_DrawGreyscaleLine (out, MakeFIAPoint(0, j-width), MakeFIAPoint(width, j), (double)val, 1, antialias_diagonals);
			
			break;
		case 6:  // cross hatched
			for (j=0; (j<(height+width)); j+=spacing)
				FIA_DrawGreyscaleLine (out, MakeFIAPoint(j, 0), MakeFIAPoint(0, j), (double)val, 1, antialias_diagonals);
			for (j=0; j<(height+width); j+=spacing)
				FIA_DrawGreyscaleLine (out, MakeFIAPoint(0, j-width), MakeFIAPoint(width, j), (double)val, 1, antialias_diagonals);
			
			break;
		case 7:   // dots
			for (j=0; j<height; j+=spacing)
				for (i=0; i<width; i+=spacing)
					FreeImage_SetPixelIndex (out, i, j, &val);

			break;
		default:
			
			break;
		}

	return out;
}


FIBITMAP *DLL_CALLCONV
FIA_BlendMaskWithImage(FIBITMAP *mask, FIBITMAP *src, RGBQUAD colour, int outline, int hatch, int opacity)
{
	// mask is expected to be a 8 bit image, vals 0 and 1 and is not changed by this function (except the border is set to 3)
	// src can be greyscale, palettised or RGB, output will be 24 bit RGB
	// colour is the required blend/overlay colour
	// outline = 1 plots an outline around the mask, outline = 0 does not
	// hatch is 10 (no hatch) to 7 (dots), see FIA_MakeHatchedImage
	// opacity is 0 to 3
	FIBITMAP *image=NULL, *maskedInner=NULL, *maskedOuter=NULL, *reversedMask=NULL, *r=NULL, *g=NULL, *b=NULL, *ri=NULL, *gi=NULL, *bi=NULL;

	// Checks
	if (!FreeImage_HasPixels(mask) || !FreeImage_HasPixels(src))
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Mask or image do not have any pixels");
        return NULL;
    }

    if (FreeImage_GetImageType (mask)!=FIT_BITMAP || FreeImage_GetBPP(mask)!=8)
    {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Mask is not a 8-bit FIT_BITMAP image");
        return NULL;
    }

    if(FIA_CheckSizesAreSame(mask, src) == 0) {
        FreeImage_OutputMessageProc (FIF_UNKNOWN, "Mask and Image are not the same size");
        return NULL;
    }

	if (FreeImage_GetImageType (src)!=FIT_BITMAP)	
	{
		FreeImage_OutputMessageProc (FIF_UNKNOWN, "Image is not a FIT_BITMAP image");
		return NULL;
	}

	if (FreeImage_GetImageType (src)==FIT_BITMAP && FreeImage_GetBPP(src)>16)  // already RGB colour
		image = FreeImage_Clone(src);
	else
		image = FreeImage_ConvertTo24Bits(src);

	// colour planes of the RGB bits within the mask
	maskedInner = FreeImage_Clone(image);
	FIA_MaskImage (maskedInner, mask);
	ri = FreeImage_GetChannel(maskedInner, FICC_RED);
	gi = FreeImage_GetChannel(maskedInner, FICC_GREEN);
	bi = FreeImage_GetChannel(maskedInner, FICC_BLUE);
	FreeImage_Unload(maskedInner);

	// average these colour planes to perform a blend, result goes into r g and b
	if (opacity==0) // 
	{
		r = FreeImage_Clone (ri);
		g = FreeImage_Clone (gi);
		b = FreeImage_Clone (bi);
	}
	else			   // average once for high opacity
	{

		// calc the three colour planes of mask with the correct colour
		r = FreeImage_Clone (mask);
		g = FreeImage_Clone (mask);
		b = FreeImage_Clone (mask);
		FIA_MultiplyConst (r, colour.rgbRed);
		FIA_MultiplyConst (g, colour.rgbGreen);
		FIA_MultiplyConst (b, colour.rgbBlue);
	
		FIA_Average (r, ri);
		FIA_Average (g, gi);
		FIA_Average (b, bi);
  		
		if (opacity < 3)   // average again for med opacity
		{
			FIA_Average (r, ri);
			FIA_Average (g, gi);
			FIA_Average (b, bi);
		}
		if (opacity < 2)   // average again for low opacity
		{
			FIA_Average (r, ri);
			FIA_Average (g, gi);
			FIA_Average (b, bi);
		}
	}
	FreeImage_Unload (ri);
	FreeImage_Unload (gi);
	FreeImage_Unload (bi);

	// colour planes of the RGB bits _outside_ the mask
	reversedMask = FreeImage_Clone (mask);
	FIA_ReverseMaskImage (reversedMask, 1);
	maskedOuter = FreeImage_Clone (image);
	FIA_MaskImage (maskedOuter, reversedMask);
	FreeImage_Unload (reversedMask);
	ri = FreeImage_GetChannel(maskedOuter, FICC_RED);
	gi = FreeImage_GetChannel(maskedOuter, FICC_GREEN);
	bi = FreeImage_GetChannel(maskedOuter, FICC_BLUE);
	FreeImage_Unload(maskedOuter);

	// add these (containing data outside the mask) to the blend images (containing data inside the mask)
	FIA_Add (r, ri);
	FIA_Add (g, gi);
	FIA_Add (b, bi);
	FreeImage_Unload (ri);
	FreeImage_Unload (gi);
	FreeImage_Unload (bi);
	
	// make an RGB from these
	maskedInner = FreeImage_Allocate(FreeImage_GetWidth(image), FreeImage_GetHeight(image), 24);

	// r, g, b must not be palletised at this point (FREE_IMAGE_COLOR_TYPE FIC_PALETTE) as FreeImage_SetChannel will fail, must be FIC_MINISBLACK
	// to be considered greyscale images in freeimage, it must have the greyscalwe palette
	FIA_SetGreyLevelPalette (r);
	FIA_SetGreyLevelPalette (g);
	FIA_SetGreyLevelPalette (b);
	FreeImage_SetChannel (maskedInner, r, FICC_RED);
	FreeImage_SetChannel (maskedInner, g, FICC_GREEN);
	FreeImage_SetChannel (maskedInner, b, FICC_BLUE);
	FreeImage_Unload (r);
	FreeImage_Unload (g);
	FreeImage_Unload (b);

	if (outline)  // do overlay of the outline in the required colour
	{
		// find the inside border to the mask
		reversedMask = FIA_BinaryInnerBorder (mask);
	
		// calc the three colour planes of a new outline mask with the correct colour
		r = FreeImage_Clone (reversedMask);
		g = FreeImage_Clone (reversedMask);
		b = FreeImage_Clone (reversedMask);
		FIA_MultiplyConst (r, colour.rgbRed);
		FIA_MultiplyConst (g, colour.rgbGreen);
		FIA_MultiplyConst (b, colour.rgbBlue);

		// reverse this outline mask and use it to get the image data from the blended image, extract these colour planes
		FIA_ReverseMaskImage(reversedMask, 1);
		FIA_MaskImage(maskedInner, reversedMask);
		FreeImage_Unload (reversedMask);
		ri = FreeImage_GetChannel(maskedInner, FICC_RED);
		gi = FreeImage_GetChannel(maskedInner, FICC_GREEN);
		bi = FreeImage_GetChannel(maskedInner, FICC_BLUE);

		// add the new outline to the surrounding image data
		FIA_Add (r, ri);
		FIA_Add (g, gi);
		FIA_Add (b, bi);
		FreeImage_Unload (ri);
		FreeImage_Unload (gi);
		FreeImage_Unload (bi);

		// make an RGB from these
		FIA_SetGreyLevelPalette (r);
		FIA_SetGreyLevelPalette (g);
		FIA_SetGreyLevelPalette (b);
		FreeImage_SetChannel (maskedInner, r, FICC_RED);
		FreeImage_SetChannel (maskedInner, g, FICC_GREEN);
		FreeImage_SetChannel (maskedInner, b, FICC_BLUE);
		FreeImage_Unload (r);
		FreeImage_Unload (g);
		FreeImage_Unload (b);
	}
	
	if (hatch)  // do overlay of the outline in the required colour
	{
		if (hatch==7) reversedMask = FIA_MakeHatchedImage(FreeImage_GetWidth(mask), FreeImage_GetHeight(mask), hatch, 3);   // dots
		else reversedMask = reversedMask = FIA_MakeHatchedImage(FreeImage_GetWidth(mask), FreeImage_GetHeight(mask), hatch, 20);   // store in reversed mask temporarily 
		FIA_MaskImage(reversedMask, mask);		// get just the bits of hatch inside the mask

		// calc the three colour planes of a new outline mask with the correct colour
		r = FreeImage_Clone (reversedMask);
		g = FreeImage_Clone (reversedMask);
		b = FreeImage_Clone (reversedMask);
		FIA_MultiplyConst (r, colour.rgbRed);
		FIA_MultiplyConst (g, colour.rgbGreen);
		FIA_MultiplyConst (b, colour.rgbBlue);

		// reverse this outline mask and use it to get the image data from the blended image, extract these colour planes
		FIA_ReverseMaskImage(reversedMask, 1);
		FIA_MaskImage(maskedInner, reversedMask);
		FreeImage_Unload(reversedMask);
		ri = FreeImage_GetChannel(maskedInner, FICC_RED);
		gi = FreeImage_GetChannel(maskedInner, FICC_GREEN);
		bi = FreeImage_GetChannel(maskedInner, FICC_BLUE);

		// add the new outline to the surrounding image data
		FIA_Add (r, ri);
		FIA_Add (g, gi);
		FIA_Add (b, bi);
		FreeImage_Unload (ri);
		FreeImage_Unload (gi);
		FreeImage_Unload (bi);

		// make an RGB from these
		FreeImage_SetChannel (maskedInner, r, FICC_RED);
		FreeImage_SetChannel (maskedInner, g, FICC_GREEN);
		FreeImage_SetChannel (maskedInner, b, FICC_BLUE);
		FreeImage_Unload (r);
		FreeImage_Unload (g);
		FreeImage_Unload (b);
	}
	
	return (maskedInner);
}

