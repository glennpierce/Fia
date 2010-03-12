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

#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

// Assume that a class is already given for the object:
//    Point with coordinates {float x, y;}
//===================================================================

// isLeft(): tests if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2 on the line
//            <0 for P2 right of the line
//    See: the January 2001 Algorithm on Area of Triangles
inline int
isLeft (FIAPOINT P0, FIAPOINT P1, FIAPOINT P2)
{
    return (P1.x - P0.x) * (P2.y - P0.y) - (P2.x - P0.x) * (P1.y - P0.y);
}

// chainHull_2D(): Andrew's monotone chain 2D convex hull algorithm
//     Input:  P[] = an array of 2D points
//                   presorted by increasing x- and y-coordinates
//             n = the number of points in P[]
//     Output: H[] = an array of the convex hull vertices (max is n)
//     Return: the number of points in H[]
int
ChainHull_2D (FIAPOINT * P, int n, FIAPOINT * H)
{
    // the output array H[] will be used as the stack
    int bot = 0, top = (-1);    // indices for bottom and top of the stack
    int i;                      // array scan index

    // Get the indices of points with min x-coord and min|max y-coord
    int minmin = 0, minmax;
    int xmin = P[0].x;

    for(i = 1; i < n; i++)
    {
        if (P[i].x != xmin)
        {
            break;
        }
    }

    minmax = i - 1;

    if (minmax == n - 1)
    {                           // degenerate case: all x-coords == xmin
        H[++top] = P[minmin];

        if (P[minmax].y != P[minmin].y)
        {                       // a nontrivial segment
            H[++top] = P[minmax];
        }

        H[++top] = P[minmin];   // add polygon endpoint

        return top + 1;
    }

    // Get the indices of points with max x-coord and min|max y-coord
    int maxmin, maxmax = n - 1;
    int xmax = P[n - 1].x;

    for(i = n - 2; i >= 0; i--)
    {
        if (P[i].x != xmax)
        {
            break;
        }
    }

    maxmin = i + 1;

    // Compute the lower hull on the stack H
    H[++top] = P[minmin];       // push minmin point onto stack
    i = minmax;

    while (++i <= maxmin)
    {
        // the lower line joins P[minmin] with P[maxmin]
        if (isLeft (P[minmin], P[maxmin], P[i]) >= 0 && i < maxmin)
            continue;           // ignore P[i] above or on the lower line

        while (top > 0)         // there are at least 2 points on the stack
        {
            // test if P[i] is left of the line at the stack top
            if (isLeft (H[top - 1], H[top], P[i]) > 0)
            {
                break;          // P[i] is a new hull vertex
            }
            else
            {
                top--;          // pop top point off stack
            }
        }

        H[++top] = P[i];        // push P[i] onto stack
    }

    // Next, compute the upper hull on the stack H above the bottom hull
    if (maxmax != maxmin)
    {                           // if distinct xmax points
        H[++top] = P[maxmax];   // push maxmax point onto stack
    }

    bot = top;                  // the bottom point of the upper hull stack
    i = maxmin;

    while (--i >= minmax)
    {
        // the upper line joins P[maxmax] with P[minmax]
        if (isLeft (P[maxmax], P[minmax], P[i]) >= 0 && i > minmax)
        {
            continue;           // ignore P[i] below or on the upper line
        }

        while (top > bot)       // at least 2 points on the upper stack
        {
            // test if P[i] is left of the line at the stack top
            if (isLeft (H[top - 1], H[top], P[i]) > 0)
                break;          // P[i] is a new hull vertex
            else
                top--;          // pop top point off stack
        }
        H[++top] = P[i];        // push P[i] onto stack
    }

    if (minmax != minmin)
    {
        H[++top] = P[minmin];   // push joining endpoint onto stack
    }

    return top + 1;
}

static inline int __cdecl
ComparePoints (const void *element1, const void *element2)
{
    FIAPOINT point1 = *(FIAPOINT *) element1;
    FIAPOINT point2 = *(FIAPOINT *) element2;

    if (point1.x > point2.x)
    {
        return 1;
    }
    else if (point1.x < point2.x)
    {
        return -1;
    }
    else
    {
        // Same x sort by y
        if (point1.y > point2.y)
        {
            return 1;
        }
        else if (point1.y < point2.y)
        {
            return -1;
        }
    }

    return 0;
}

FIBITMAP *DLL_CALLCONV
FreeImage_ConvexHull (FIBITMAP * src)
{
    FIBITMAP *tmp = FIA_ConvertToGreyscaleFloatType (src, FIT_FLOAT);

    int width = FreeImage_GetWidth (tmp);
    int height = FreeImage_GetHeight (tmp);

    FIBITMAP *dst = FreeImage_Allocate (width, height, 8, 0, 0, 0);

    FIA_SetGreyLevelPalette (dst);

    FIAPOINT *sort_array = new FIAPOINT[width * height];
    FIAPOINT *hull_array = new FIAPOINT[width * height];

    float *ptr;
    register int i = 0;

    FIAPOINT pt;

    // Copy image pixels to 1d array
    for(register int y = 0; y < height; y++)
    {
        ptr = (float *) FreeImage_GetScanLine (tmp, y);

        for(register int x = 0; x < width; x++)
        {
            if (ptr[x])
            {
                pt.x = x;
                pt.y = y;
                sort_array[i++] = pt;
            }
        }
    }

    // sort the array by x and then y
    // Sort the peaks
    qsort (sort_array, i, sizeof (FIAPOINT), ComparePoints);

    int number_of_points = ChainHull_2D (sort_array, i, hull_array);

    delete sort_array;

    FIA_DrawSolidGreyscalePolygon (dst, hull_array, number_of_points, 255, 0);

    delete hull_array;

    FreeImage_Unload (tmp);

    return dst;
}
