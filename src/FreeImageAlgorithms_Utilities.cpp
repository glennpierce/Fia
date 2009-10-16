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
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>
#include <math.h>
#include <limits.h>
#include <float.h>

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
FIAImageRect (FIBITMAP * src)
{
    FIARECT rect;

    rect.left = 0;
    rect.top = 0;
    rect.right = FreeImage_GetWidth (src) - 1;
    rect.bottom = FreeImage_GetHeight (src) - 1;

    return rect;
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

template < class Tsrc > class FIND_MINMAX_FOR_DIB
{
  public:
    void find (FIBITMAP * src, double *min, double *max);
    void find_max_xy (FIBITMAP * src, double *max, FIAPOINT * pt);
};

template < class Tsrc > void FIND_MINMAX_FOR_DIB < Tsrc >::find (FIBITMAP * src,
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

FIND_MINMAX_FOR_DIB < unsigned char >minmaxUCharImage;
FIND_MINMAX_FOR_DIB < unsigned short >minmaxUShortImage;
FIND_MINMAX_FOR_DIB < short >minmaxShortImage;
FIND_MINMAX_FOR_DIB < unsigned long >minmaxULongImage;
FIND_MINMAX_FOR_DIB < long >minmaxLongImage;
FIND_MINMAX_FOR_DIB < float >minmaxFloatImage;
FIND_MINMAX_FOR_DIB < double >minmaxDoubleImage;

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
                minmaxUCharImage.find (src, min, max);
            }
            break;
        }

        case FIT_UINT16:
        {
            minmaxUShortImage.find (src, min, max);
            break;
        }

        case FIT_INT16:
        {
            minmaxShortImage.find (src, min, max);
            break;
        }

        case FIT_UINT32:
        {
            minmaxULongImage.find (src, min, max);
            break;
        }

        case FIT_INT32:
        {
            minmaxLongImage.find (src, min, max);
            break;
        }

        case FIT_FLOAT:
        {
            minmaxFloatImage.find (src, min, max);
            break;
        }

        case FIT_DOUBLE:
        {
            minmaxDoubleImage.find (src, min, max);
            break;
        }

        default:
        {
            break;
        }
    }
}

template < class Tsrc > void FIND_MINMAX_FOR_DIB < Tsrc >::find_max_xy (FIBITMAP * src,
                                                                        double *max, FIAPOINT * pt)
{
    if (!src)
    {
        return;
    }

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
    if (!src)
        return;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
            {
                minmaxUCharImage.find_max_xy (src, max, pt);
            }
            break;
        }

        case FIT_UINT16:
        {
            minmaxUShortImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_INT16:
        {
            minmaxShortImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_UINT32:
        {
            minmaxULongImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_INT32:
        {
            minmaxLongImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_FLOAT:
        {
            minmaxFloatImage.find_max_xy (src, max, pt);
            break;
        }

        case FIT_DOUBLE:
        {
            minmaxDoubleImage.find_max_xy (src, max, pt);
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
int IntersectingRect(FIARECT r1, FIARECT r2, FIARECT *r3)
{
    int fIntersect = (r2.left <= r1.right && r2.right >= r1.left &&
            r2.top <= r1.bottom && r2.bottom >= r1.top);

	MakeFIARect(0,0,0,0);

    if(fIntersect)
    {
        r3->left = std::max(r1.left, r2.left);
        r3->top = std::max(r1.top, r2.top);
        r3->right = std::min(r1.right, r2.right);
        r3->bottom = std::min(r1.bottom, r2.bottom);
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

	if(IntersectingRect(MakeFIARect(0, 0, dst_width - 1, dst_height - 1),
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
    int max_right = FreeImage_GetWidth(src) - 1;
    int max_bottom = FreeImage_GetHeight(src) - 1;

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

FIBITMAP *DLL_CALLCONV
FIA_ConvertToGreyscaleFloatType (FIBITMAP * src, FREE_IMAGE_TYPE type)
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
FIA_InPlaceConvertToGreyscaleFloatType (FIBITMAP ** src, FREE_IMAGE_TYPE type)
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

            if (type == FIT_DOUBLE)
                *val = *((double *) line_ptr + x);

            return FIA_SUCCESS;
        }
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_InPlaceConvertToStandardType (FIBITMAP ** src, int scale)
{
    FIBITMAP *dst = FreeImage_ConvertToStandardType (*src, scale);

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

FIBITMAP *DLL_CALLCONV
FIA_ConvertFloatTo16Bit (FIBITMAP * src, int sign)
{
    if (src == NULL)
        return NULL;

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    // Mask has to be 8 bit
    if (type != FIT_FLOAT)
        return NULL;

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FIBITMAP *dst = NULL;

    if (sign)
    {
        dst = FreeImage_AllocateT (FIT_INT16, width, height, 16, 0, 0, 0);

        for(register int y = 0; y < height; y++)
        {
            float *src_ptr = (float *) FreeImage_GetScanLine (src, y);
            short *dst_ptr = (short *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (short) src_ptr[x];
        }
    }
    else
    {
        dst = FreeImage_AllocateT (FIT_UINT16, width, height, 16, 0, 0, 0);

        for(register int y = 0; y < height; y++)
        {
            float *src_ptr = (float *) FreeImage_GetScanLine (src, y);
            unsigned short *dst_ptr = (unsigned short *) FreeImage_GetScanLine (dst, y);

            for(register int x = 0; x < width; x++)
                dst_ptr[x] = (unsigned short) src_ptr[x];
        }
    }

    return dst;
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

template < class Tsrc > class FastSimpleResample
{
  public:
    FIBITMAP * IntegerRescaleToHalf (FIBITMAP * src);
    FIBITMAP *ColourRescaleToHalf (FIBITMAP * src);
    FIBITMAP *FloatRescaleToHalf (FIBITMAP * src);
};

template < typename Tsrc > FIBITMAP * FastSimpleResample <
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

template < typename Tsrc > FIBITMAP * FastSimpleResample <
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

FastSimpleResample < unsigned char >fastResampleUCharImage;
FastSimpleResample < unsigned short >fastResampleUShortImage;
FastSimpleResample < short >fastResampleShortImage;
FastSimpleResample < unsigned long >fastResampleULongImage;
FastSimpleResample < long >fastResampleLongImage;
FastSimpleResample < float >fastResampleFloatImage;
FastSimpleResample < double >fastResampleDoubleImage;

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
                dst = fastResampleUCharImage.IntegerRescaleToHalf (src);
            }
            else
            {
                dst = ColourRescaleToHalf (src);
            }
            break;
        }

        case FIT_UINT16:       // array of unsigned short: unsigned 16-bit
        {
            dst = fastResampleUShortImage.IntegerRescaleToHalf (src);
            break;
        }

        case FIT_INT16:        // array of short: signed 16-bit
        {
            dst = fastResampleShortImage.IntegerRescaleToHalf (src);
            break;
        }

        case FIT_UINT32:       // array of unsigned long: unsigned 32-bit
        {
            dst = fastResampleULongImage.IntegerRescaleToHalf (src);
            break;
        }

        case FIT_INT32:        // array of long: signed 32-bit
        {
            dst = fastResampleLongImage.IntegerRescaleToHalf (src);
            break;
        }

        case FIT_FLOAT:        // array of float: 32-bit
        {
            dst = fastResampleFloatImage.FloatRescaleToHalf (src);
            break;
        }

        case FIT_DOUBLE:       // array of double: 64-bit
        {
            dst = fastResampleDoubleImage.FloatRescaleToHalf (src);
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

static FIBITMAP *
FIA_SetAlphaValuesFromDistanceMapImage (FIBITMAP * src, FIBITMAP* alpha_values)
{
    FIBITMAP *colour_dib = FreeImage_ConvertTo32Bits(src);

    FreeImage_SetTransparent(colour_dib, 1);

    FreeImage_SetChannel(colour_dib, alpha_values, FICC_ALPHA);

    if(!FreeImage_IsTransparent(colour_dib))
         return FIA_ERROR;

    return colour_dib;
}

FIBITMAP *DLL_CALLCONV
FIA_GradientBlend (FIBITMAP * src1, FIARECT rect1, FIBITMAP* src2, FIARECT rect2)
{
    FIARECT intersection_rect, map_rect;
    FIBITMAP *alpha = NULL, *src1_24 = NULL, *blended = NULL, *src1_region = NULL;
    FIBITMAP *src2_region = NULL, *map = NULL, *map_region = NULL, *src1_cpy = NULL;
    int map_width,  map_height;

    if(!IntersectingRect(rect1, rect2, &intersection_rect))
        return FIA_ERROR;

    src1_cpy = FreeImage_Clone(src1);

    int intersect_width = intersection_rect.right - intersection_rect.left + 1;
    int intersect_height = intersection_rect.bottom - intersection_rect.top + 1;

    map_rect.left = intersection_rect.left;
    map_rect.top = intersection_rect.top;
    map_rect.right = map_rect.left + (2 * intersect_width) - 1;
    map_rect.bottom = map_rect.top + (2 * intersect_height) - 1;

    // Generate a distance map twice the size of this intersection
    // We only wish half the map to produce a one way gradient
    map = FIA_DistanceMapForRectangle (map_rect, 1);

    FIARECT src1_intersection_rect = SetRectRelativeToPoint(intersection_rect, MakeFIAPoint(rect1.left, rect1.top));
    FIARECT src2_intersection_rect = SetRectRelativeToPoint(intersection_rect, MakeFIAPoint(rect2.left, rect2.top));

    src1_region = FIA_Copy(src1, src1_intersection_rect.left, src1_intersection_rect.top,
            src1_intersection_rect.right, src1_intersection_rect.bottom);

    src2_region = FIA_Copy(src2, src2_intersection_rect.left, src2_intersection_rect.top,
                src2_intersection_rect.right, src2_intersection_rect.bottom);

    if(FIA_CheckSizesAreSame(src1_region, src2_region) == 0) {
        goto CLEANUP;
    }

    map_width = FreeImage_GetWidth(map);
    map_height = FreeImage_GetHeight(map);

    if(rect2.left < rect1.left) {
        map_rect.left = map_width / 2;
        map_rect.right = map_width - 1;
    }
    else {
        map_rect.left = 0;
        map_rect.right = map_width / 2 - 1;
    }

    if(rect2.top < rect1.top) {
        map_rect.top = map_height / 2;
        map_rect.bottom = map_height - 1;
    }
    else {
        map_rect.top = 0;
        map_rect.bottom = map_height / 2 - 1;
    }

    map_region = FIA_Copy(map, map_rect.left, map_rect.top,
            map_rect.right, map_rect.bottom);

    FIA_InPlaceConvertToStandardType(&map_region, 1);

    alpha = FIA_SetAlphaValuesFromDistanceMapImage (src2_region, map_region);

    src1_24 = FreeImage_ConvertTo24Bits(src1_region);

    blended = FreeImage_Composite(alpha, 1, NULL, src1_24);

    FIA_PasteFromTopLeft(src1_cpy, blended,  src1_intersection_rect.left, src1_intersection_rect.top);

CLEANUP:

    if(src1_region != NULL)
        FreeImage_Unload(src1_region);

    if(src2_region != NULL)
        FreeImage_Unload(src2_region);

    if(map != NULL)
        FreeImage_Unload(map);

    if(map_region != NULL)
        FreeImage_Unload(map_region);

    if(alpha != NULL)
        FreeImage_Unload(alpha);

    if(blended != NULL)
        FreeImage_Unload(blended);

    if(src1_24 != NULL)
        FreeImage_Unload(src1_24);

    return src1_cpy;
}
