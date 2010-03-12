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
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Colour.h"

#include <iostream>
#include <math.h>

// Red, Green and Blue are between 0 and 255
// Hue varies between 0 and 360
// Satuation between 0 and 1
// Value between 0 and 1
int DLL_CALLCONV
FIA_RGBToHSV (unsigned char red, unsigned char green, unsigned char blue,
              double *hue, double *satuation, double *value)
{
    unsigned char max, min, rgb[3];

    *hue = 0;
    *satuation = 0;
    *value = 0;

    rgb[0] = red, rgb[1] = green, rgb[2] = blue;
    MAXMIN (rgb, 3, max, min);

    if (max != min)
    {
        if (red == max)
        {
            *hue = ((double) (green - blue) / (max - min)) * 60.0;
        }

        if (green == max)
        {
            *hue = (2 + (double) (blue - red) / (max - min)) * 60.0;
        }

        if (blue == max)
        {
            *hue = (4 + (double) (red - green) / (max - min)) * 60.0;
        }
    }

    *value = (double) max / 255.0;

    if (max != 0)
    {
        *satuation = ((double) (max - min) / max);
    }

    return FIA_SUCCESS;
}

// Red, Green and Blue are between 0 and 255
// Hue varies betww=een 0 and 360
// Satuation between 0 and 1
// Value between 0 and 1
int DLL_CALLCONV
FIA_HSVToRGB (double hue, double satuation, double value,
              unsigned char *red, unsigned char *green, unsigned char *blue)
{
    unsigned int h, p, q, t, v;
    double temp_hue, f;

    if (satuation == 0.0)
        return FIA_SUCCESS;

    // The if statement is here as the % operator requires integers
    // and thus looses some accuracy.
    if (hue < 0 || hue > 360)
    {
        temp_hue = floor (hue + 0.5);
        temp_hue = (int) temp_hue % 360;
    }
    else
    {
        temp_hue = hue;
    }

    *red = (unsigned char) floor (value + 0.5);
    *green = *red;
    *blue = *red;

    h = (unsigned int) (temp_hue / 60);
    f = (double) (temp_hue / 60.0) - h;
    p = (unsigned int) floor ((value * (1 - satuation) * 255) + 0.5);
    q = (unsigned int) floor ((value * (1 - f * satuation) * 255) + 0.5);
    t = (unsigned int) floor ((value * (1 - (1 - f) * satuation) * 255) + 0.5);

    v = (unsigned int) (value * 255.0);

    switch (h)
    {
        case 6:
        case 0:
        {
            *red = v;
            *green = t;
            *blue = p;
            break;
        }

        case 1:
        {
            *red = q;
            *green = v;
            *blue = p;
            break;
        }

        case 2:
        {
            *red = p;
            *green = v;
            *blue = t;
            break;
        }

        case 3:
        {
            *red = p;
            *green = q;
            *blue = v;
            break;
        }

        case 4:
        {
            *red = t;
            *green = p;
            *blue = v;
            break;
        }

        case 5:
        {
            *red = v;
            *green = p;
            *blue = q;
            break;
        }
    }

    return FIA_SUCCESS;
}

// Red, Green and Blue are between 0 and 255
// Hue varies between 0 and 360
// Satuation between 0 and 1
// Value between 0 and 1
int DLL_CALLCONV
FIA_RGBToHSL (unsigned char red, unsigned char green, unsigned char blue,
              double *hue, double *satuation, double *luminosity)
{
    unsigned char max, min, rgb[3];

    *hue = 0;
    *satuation = 0;
    *luminosity = 0;

    rgb[0] = red, rgb[1] = green, rgb[2] = blue;
    MAXMIN (rgb, 3, max, min);

    if (max != min)
    {
        if (red == max)
            *hue = ((double) (green - blue) / (max - min)) * 60.0;

        if (green == max)
            *hue = (2 + (double) (blue - red) / (max - min)) * 60.0;

        if (blue == max)
            *hue = (4 + (double) (red - green) / (max - min)) * 60.0;
    }

    *luminosity = (double) (max + min) / (510.0);

    if (*luminosity < 0.5)
    {
        *satuation = ((double) (max - min) / (510.0 * *luminosity));
    }
    else
    {
        *satuation = ((double) (max - min) / (510.0 * (2 - (2 * *luminosity))));
    }

    return FIA_SUCCESS;
}

static double
HSLToRGBColour (double q1, double q2, double hue)
{
    if (hue > 360.0)
    {
        hue -= 360.0;
    }
    else if (hue < 0.0)
    {
        hue += 360.0;
    }

    if (hue < 60.0)
    {
        return (q1 + (q2 - q1) * hue / 60.0);
    }
    else if (hue < 180.0)
    {
        return q2;
    }
    else if (hue < 240.0)
    {
        return (q1 + (q2 - q1) * (240.0 - hue) / 60.0);
    }
    else
    {
        return q1;
    }
}

int DLL_CALLCONV
FIA_HSLToRGB (double hue, double satuation, double luminosity,
              unsigned char *red, unsigned char *green, unsigned char *blue)
{
    double p1, p2;

    if (luminosity <= 0.5)
    {
        p2 = luminosity * (1 + satuation);
    }
    else
    {
        p2 = luminosity + satuation - (luminosity * satuation);
    }

    p1 = 2.0 * luminosity - p2;

    if (satuation == 0.0)
    {
        *red = (unsigned int) ((luminosity * 255.0) + 0.5);
        *green = (unsigned int) ((luminosity * 255.0) + 0.5);
        *blue = (unsigned int) ((luminosity * 255.0) + 0.5);
    }
    else
    {
        *red = (unsigned int) ((HSLToRGBColour (p1, p2, hue + 120.0) * 255.0) + 0.5);
        *green = (unsigned int) ((HSLToRGBColour (p1, p2, hue) * 255.0) + 0.5);
        *blue = (unsigned int) ((HSLToRGBColour (p1, p2, hue - 120.0) * 255.0) + 0.5);
    }

    return FIA_SUCCESS;
}
