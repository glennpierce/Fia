/*
 * Copyright 2007-2010 Glenn Pierce, Paul Barber,
 * Oxford University (Gray Institute for Radiation Oncology and Biology) 
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

#include <algorithm>
#include <math.h>
#include <float.h>
#include <iostream>

#define INF 1E10

template < class T > static inline T
square (const T & x)
{
    return x * x;
}

/* dt of 1d function using squared distance */
static float *
dt (float *f, int n)
{
    float *d = new float[n];
    int *v = new int[n];
    float *z = new float[n + 1];
    int k = 0;

    v[0] = 0;
    z[0] = -INF;
    z[1] = INF;

    for(int q = 1; q <= n - 1; q++)
    {
        float s = ((f[q] + square (q)) - (f[v[k]] + square (v[k]))) / (2 * q - 2 * v[k]);

        while (s <= z[k])
        {
            k--;
            s = ((f[q] + square (q)) - (f[v[k]] + square (v[k]))) / (2 * q - 2 * v[k]);
        }
        k++;
        v[k] = q;
        z[k] = s;
        z[k + 1] = INF;
    }

    k = 0;
    for(int q = 0; q <= n - 1; q++)
    {
        while (z[k + 1] < q)
        {
            k++;
        }

        d[q] = square (q - v[k]) + f[v[k]];
    }

    delete[]v;
    delete[]z;
    return d;
}

/* dt of 2d function using squared distance */
static void
dt2d (FIBITMAP * src)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);
    float *f = new float[MAX (width, height)];
    register int x, y;

    register float *src_ptr;

    // transform along columns
    for(x = 0; x < width; x++)
    {
        for(y = 0; y < height; y++)
        {

            src_ptr = (float *) FreeImage_GetScanLine (src, y);
            f[y] = src_ptr[x];
        }

        float *d = dt (f, height);

        for(y = 0; y < height; y++)
        {

            src_ptr = (float *) FreeImage_GetScanLine (src, y);
            src_ptr[x] = d[y];
        }

        delete[]d;
    }

    // transform along rows
    for(y = 0; y < height; y++)
    {
        src_ptr = (float *) FreeImage_GetScanLine (src, y);

        for(x = 0; x < width; x++)
        {
            f[x] = src_ptr[x];
        }

        float *d = dt (f, width);

        for(x = 0; x < width; x++)
        {
            src_ptr[x] = d[x];
        }

        delete[]d;
    }

    delete f;
}

/* dt of binary image using squared distance */
FIBITMAP *DLL_CALLCONV
FIA_DistanceTransform (FIBITMAP * src)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);
    float *out_ptr;
    unsigned char *src_ptr;

    FIBITMAP *out = FIA_ConvertToGreyscaleFloatType (src, FIT_FLOAT);

    for(register int y = 0; y < height; y++)
    {
        src_ptr = (unsigned char *) FreeImage_GetScanLine (src, y);
        out_ptr = (float *) FreeImage_GetScanLine (out, y);

        for(register int x = 0; x < width; x++)
        {

            if (src_ptr[x] == 0)
            {
                out_ptr[x] = 0.0f;
            }
            else
            {
                out_ptr[x] = INF;
            }
        }
    }

    dt2d (out);

    // take square roots
    for(register int y = 0; y < height; y++)
    {
        out_ptr = (float *) FreeImage_GetScanLine (out, y);

        for(register int x = 0; x < width; x++)
        {
            out_ptr[x] = sqrt (out_ptr[x]);
        }
    }

    FIBITMAP *ret = FreeImage_ConvertToStandardType (out, 1);

    FreeImage_Unload (out);

    return ret;
}