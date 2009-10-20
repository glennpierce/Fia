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

#include "FreeImageAlgorithms_Colour.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <math.h>

RGBQUAD DLL_CALLCONV
FIA_RGBQUAD (unsigned char red, unsigned char green, unsigned char blue)
{
    RGBQUAD quad;

    quad.rgbRed = red;
    quad.rgbGreen = green;
    quad.rgbBlue = blue;
    quad.rgbReserved = 0;

    return quad;
}

static bool
IsRGBQUADZero (RGBQUAD quad)
{
    if (quad.rgbRed != 0 || quad.rgbGreen != 0 || quad.rgbBlue != 0)
    {
        return false;
    }

    return true;
}

int DLL_CALLCONV
FIA_CopyPaletteToRGBQUAD (FIBITMAP * src, RGBQUAD * palette)
{
    if (src == NULL || palette == NULL)
    {
        return FIA_ERROR;
    }

    // copy the palette
    memcpy (palette, FreeImage_GetPalette (src), FreeImage_GetColorsUsed (src) * sizeof (RGBQUAD));

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_CopyPaletteFromRGBQUAD (FIBITMAP * dst, RGBQUAD * palette)
{
    RGBQUAD *dst_palette;

    if (dst == NULL || palette == NULL || FreeImage_GetBPP (dst) > 8)
    {
        return FIA_ERROR;
    }

    if ((dst_palette = FreeImage_GetPalette (dst)) == NULL)
    {
        return FIA_ERROR;
    }

    for(int i = 0; i < 256; i++)
    {
        dst_palette[i].rgbRed = palette[i].rgbRed;
        dst_palette[i].rgbGreen = palette[i].rgbGreen;
        dst_palette[i].rgbBlue = palette[i].rgbBlue;
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_CopyPalette (FIBITMAP * src, FIBITMAP * dst)
{
    RGBQUAD *src_palette, *dst_palette;

    if (dst == NULL || src == NULL)
    {
        return FIA_ERROR;
    }

    if ((src_palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    if ((dst_palette = FreeImage_GetPalette (dst)) == NULL)
    {
        return FIA_ERROR;
    }

    for(int i = 0; i < 256; i++)
    {
        dst_palette[i].rgbRed = src_palette[i].rgbRed;
        dst_palette[i].rgbGreen = src_palette[i].rgbGreen;
        dst_palette[i].rgbBlue = src_palette[i].rgbBlue;
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_ReversePaletteEntries (RGBQUAD * palette)
{
    int i;
    RGBQUAD copy_palette[256];

    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    memcpy (copy_palette, palette, sizeof (RGBQUAD) * 256);

    for(i = 0; i < 256; i++)
    {
        palette[i] = copy_palette[255 - i];
    }

    return 1;
}

int DLL_CALLCONV
FIA_SetGreyLevelPalette (FIBITMAP * src)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetGreyLevelPalette (palette);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetGreyLevelOverLoadPalette (FIBITMAP * src)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetGreyLevelOverLoadPalette (palette);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetTernaryPalettePalette (FIBITMAP * src, RGBQUAD background_colour,
                              int pos1, RGBQUAD colour1, int pos2, RGBQUAD colour2)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetTernaryPalette (palette, background_colour, pos1, colour1, pos2, colour2);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetLogColourPalette (FIBITMAP * src)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetLogColourPalette (palette);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetFalseColourPalette (FIBITMAP * src, double wavelength)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetFalseColourPalette (palette, wavelength);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetPileUpPalette (FIBITMAP * src, RGBQUAD colour1, RGBQUAD colour2,
                      RGBQUAD colour3, BYTE * size)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetPileUpPalette (palette, colour1, colour2, colour3, size);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetRainBowPalette (FIBITMAP * src)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetRainBowPalette (palette);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetReverseRainBowPalette (FIBITMAP * src)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetReverseRainBowPalette (palette);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetTemperaturePalette (FIBITMAP * src)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetTemperaturePalette (palette);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_SetSeismicColourPalette (FIBITMAP * src)
{
    RGBQUAD *palette;

    if ((palette = FreeImage_GetPalette (src)) == NULL)
    {
        return -1;
    }

    FIA_GetSeismicColourPalette (palette);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetGreyLevelPalette (RGBQUAD * palette)
{
    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    for(int i = 0; i < 256; i++)
    {
        palette[i].rgbRed = i;
        palette[i].rgbGreen = i;
        palette[i].rgbBlue = i;
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetGreyLevelOverLoadPalette (RGBQUAD * palette)
{
    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    int i;

    for(i = 0; i < 245; i++)
    {
        palette[i].rgbRed = i;
        palette[i].rgbGreen = i;
        palette[i].rgbBlue = i;
    }

    for(i = 245; i < 250; i++)
    {
        palette[i].rgbRed = 0;
        palette[i].rgbGreen = 255;
        palette[i].rgbBlue = 0;
    }

    for(i = 250; i < 256; i++)
    {
        palette[i].rgbRed = 255;
        palette[i].rgbGreen = 0;
        palette[i].rgbBlue = 0;
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetTernaryPalette (RGBQUAD * palette, RGBQUAD background_colour,
                       int pos1, RGBQUAD colour1, int pos2, RGBQUAD colour2)
{
    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    int i;

    // Mark as background colour
    for(i = 0; i < 256; i++)
    {
        palette[i].rgbRed = background_colour.rgbRed;
        palette[i].rgbGreen = background_colour.rgbGreen;
        palette[i].rgbBlue = background_colour.rgbBlue;
    }

    palette[pos1].rgbRed = colour1.rgbRed;
    palette[pos1].rgbGreen = colour1.rgbGreen;
    palette[pos1].rgbBlue = colour1.rgbBlue;

    palette[pos2].rgbRed = colour2.rgbRed;
    palette[pos2].rgbGreen = colour2.rgbGreen;
    palette[pos2].rgbBlue = colour2.rgbBlue;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetPileUpPalette (RGBQUAD * palette, RGBQUAD colour1, RGBQUAD colour2,
                      RGBQUAD colour3, BYTE * size)
{
    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    FIA_GetGreyLevelPalette (palette);

    // Set default to red blue green
    if (IsRGBQUADZero (colour1) && IsRGBQUADZero (colour2) && IsRGBQUADZero (colour3))
    {

        colour1 = FIA_RGBQUAD (255, 0, 0);
        colour2 = FIA_RGBQUAD (0, 255, 0);
        colour3 = FIA_RGBQUAD (0, 0, 255);
    }

    int top = 255;

    // Do first pileup
    for(int i = 0; i < size[0]; i++)
    {
        palette[top - i] = colour1;
    }

    top -= size[0];

    // Do second pileup
    for(int i = 0; i < size[1]; i++)
    {
        palette[top - i] = colour2;
    }

    top -= size[1];

    // Do third pileup
    for(int i = 0; i < size[2]; i++)
    {
        palette[top - i] = colour3;
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetRainBowPalette (RGBQUAD * palette)
{
    int i, p1, p2;

    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    p1 = 256 / 3;
    p2 = 2 * p1;

    // First do the blue bit
    for(i = 0; i < p1; i++)
    {
        palette[i].rgbBlue = 255;
    }

    for(i = p1; i < p2; i++)
    {
        palette[i].rgbBlue = 255 + ((i - p1) * -255 / p1);
    }

    for(i = p2; i < 256; i++)
    {
        palette[i].rgbBlue = 0;
    }

    // Now add the green bit
    for(i = 0; i < p1; i++)
    {
        palette[i].rgbGreen = (i * 255 / p1);
    }

    for(i = p1; i < p2; i++)
    {
        palette[i].rgbGreen = 255;
    }

    for(i = p2; i < 256; i++)
    {
        palette[i].rgbGreen = 255 + ((i - p2) * -255 / p1);
    }

    // Now add the red bit
    for(i = 0; i < p1; i++)
    {
        palette[i].rgbRed = 0;
    }

    for(i = p1; i < p2; i++)
    {
        palette[i].rgbRed = ((i - p2) * 255 / p1);
    }

    for(i = p2; i < 256; i++)
    {
        palette[i].rgbRed = 255;
    }

    // Now black and white at the ends
    palette[0].rgbRed = 0;
    palette[0].rgbGreen = 0;
    palette[0].rgbBlue = 0;

    palette[255].rgbRed = 255;
    palette[255].rgbGreen = 255;
    palette[255].rgbBlue = 255;

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetReverseRainBowPalette (RGBQUAD * palette)
{
    FIA_GetRainBowPalette (palette);

    return FIA_ReversePaletteEntries (palette);
}

int DLL_CALLCONV
FIA_GetLogColourPalette (RGBQUAD * palette)
{
    if (palette == NULL)
        return FIA_ERROR;

    for(int i = 1; i < 256; i++)
    {

        double level = floor ((255.0 * log10 ((double) i) / log10 (255.0)) + 0.5);

        palette[i].rgbRed = (BYTE) level;
        palette[i].rgbGreen = (BYTE) level;
        palette[i].rgbBlue = (BYTE) level;
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetFalseColourPalette (RGBQUAD * palette, double wavelength)
{
    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    double red, green, blue;

    //Get RGB components for false colour image for a particular wavelength
    //The numbers come from a fortran program that BV found on the internet

    if ((wavelength >= 380) && (wavelength < 440))
    {
        red = (440 - wavelength) / (440 - 380);
        green = 0;
        blue = 1;
    }
    else if ((wavelength >= 440) && (wavelength < 490))
    {
        red = 0;
        green = (wavelength - 440) / (490 - 440);
        blue = 1;
    }
    else if ((wavelength >= 490) && (wavelength < 510))
    {
        red = 0;
        green = 1;
        blue = (510 - wavelength) / (510 - 490);
    }
    else if ((wavelength >= 510) && (wavelength < 580))
    {
        red = (wavelength - 510) / (580 - 510);
        green = 1;
        blue = 0;
    }
    else if ((wavelength >= 580) && (wavelength < 645))
    {
        red = 1;
        green = (645 - wavelength) / (645 - 580);
        blue = 0;
    }
    else if ((wavelength >= 645) && (wavelength < 780))
    {
        red = 1;
        green = 0;
        blue = 0;
    }
    else
    {                           //display as mono

        FIA_GetGreyLevelPalette (palette);

        return FIA_SUCCESS;
    }

    for(int i = 0; i < 256; i++)
    {
        palette[i].rgbRed = (BYTE) (red * i);
        palette[i].rgbGreen = (BYTE) (green * i);
        palette[i].rgbBlue = (BYTE) (blue * i);
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetOpticalDensityPalette (RGBQUAD * palette, unsigned char red,
                              unsigned char green, unsigned char blue, int contrast, int entries)
{
    int i;
    double h, s, l, slog, c;
    unsigned char r, g, b;

    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    slog = log (1.0 / (8.0 * contrast)) / 255.0;        // to scale the od nicely 0-255

    FIA_RGBToHSL (red, green, blue, &h, &s, &l);

    for(i = 0; i < entries; i++)
    {
        c = exp ((double) i * slog);

        if (c > 1.0)
        {
            c = 1.0;
        }
        else if (c < 0.0)
        {
            c = 0.0;
        }

        FIA_HSLToRGB (h, s, c, &r, &g, &b);

        palette[i].rgbRed = r;
        palette[i].rgbGreen = g;
        palette[i].rgbBlue = b;
    }

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_GetTemperaturePalette (RGBQUAD * palette)
{
    int i, p1, p2;

    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    memset (palette, 0, sizeof (RGBQUAD) * 256);

    p1 = 256 / 2;
    p2 = (int) ((3.0 * 256) / 4.0);

    // First do the red bit     
    for(i = 0; i < 256; i++)
    {
        palette[i].rgbRed = i;
    }

    // Now add the green bit
    for(i = p1; i < 256; i++)
    {
        palette[i].rgbGreen = ((i - p1) * 255 / p1);
    }

    // Now do the blue bit      

    for(i = p2; i < 256; i++)
    {
        palette[i].rgbBlue = (((i - p2) * 255) / (256 - p2));
    }

    return FIA_SUCCESS;
}

static int DLL_CALLCONV
FIA_SeismicReversePaletteEntries (RGBQUAD * palette, unsigned char start, unsigned char size)
{
    int i, max_element;

    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    max_element = (size / 2) - 1;

    for(i = start; i < max_element; i++)
    {
        INPLACESWAP (palette[i].rgbRed, palette[size - i].rgbRed);
        INPLACESWAP (palette[i].rgbGreen, palette[size - i].rgbGreen);
        INPLACESWAP (palette[i].rgbBlue, palette[size - i].rgbBlue);
    }

    return 1;
}

int DLL_CALLCONV
FIA_GetSeismicColourPalette (RGBQUAD * palette)
{
    int halfway, contrast = 6;

    if (palette == NULL)
    {
        return FIA_ERROR;
    }

    halfway = 128;

    FIA_GetOpticalDensityPalette (palette, 0, 0, 255, contrast, halfway);

    FIA_SeismicReversePaletteEntries (palette, 0, halfway);

    // Second Half
    FIA_GetOpticalDensityPalette (&(palette[halfway]), 255, 0, 0, contrast, halfway);

    palette[0].rgbRed = 0;
    palette[0].rgbGreen = 0;
    palette[0].rgbBlue = 0;

    palette[255].rgbRed = 255;
    palette[255].rgbGreen = 255;
    palette[255].rgbBlue = 255;

    return 0;
}
