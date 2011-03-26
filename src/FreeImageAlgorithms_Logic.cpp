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
#include "FreeImageAlgorithms_Logic.h"
#include "FreeImageAlgorithms_Utilities.h"
#include <limits>
#include <float.h>
#include <math.h>

template < typename Tsrc > class LOGIC
{
  public:

    // I know these are logic functions but oh well no need for a new class
    // + its only private.
    int MaskImage (FIBITMAP * src, FIBITMAP * mask);
    int InvertMaskImage (FIBITMAP * mask);
};

static int
CheckDimensions (FIBITMAP * src, FIBITMAP * mask)
{
    // Check src is the same size as dst
    int src_width = FreeImage_GetWidth (src);
    int src_height = FreeImage_GetHeight (src);
    int dst_width = FreeImage_GetWidth (mask);
    int dst_height = FreeImage_GetHeight (mask);

    if (src_width != dst_width || src_height != dst_height)
        return FIA_ERROR;

    return FIA_SUCCESS;
}

template < typename Tsrc > int LOGIC < Tsrc >::MaskImage (FIBITMAP * src, FIBITMAP * mask)
{
    if (mask == NULL || src == NULL)
    {
        return FIA_ERROR;
    }

    // Have to be the same size
    if (CheckDimensions (src, mask) == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    // Mask has to be 8 bit 
    if (FreeImage_GetBPP (mask) != 8 || FreeImage_GetImageType (mask) != FIT_BITMAP)
    {
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    bool greyscale_image = true;

    if(FreeImage_GetImageType(src) == FIT_BITMAP && FreeImage_GetBPP(src) > 8) {
	    greyscale_image = false;
    }
    
    if(greyscale_image) {
        for(register int y = 0; y < height; y++)
        {
            Tsrc *src_ptr = (Tsrc *) FreeImage_GetScanLine (src, y);
            unsigned char *mask_ptr = (unsigned char *) FreeImage_GetScanLine (mask, y);

            for(register int x = 0; x < width; x++)
            {
                if (!mask_ptr[x])
                    src_ptr[x] = 0;
            }
        }
    }
    else {
        
        int bytespp = FreeImage_GetLine (src) / FreeImage_GetWidth (src);
    
        for(register int y = 0; y < height; y++)
        {
            BYTE *src_ptr = (BYTE *) FreeImage_GetScanLine (src, y);
            BYTE *mask_ptr = (BYTE *) FreeImage_GetScanLine (mask, y);

            for(register int x = 0; x < width; x++)
            {
                if (!mask_ptr[x]) {

                    src_ptr[FI_RGBA_RED] = 0;
		            src_ptr[FI_RGBA_GREEN] = 0;
		            src_ptr[FI_RGBA_BLUE] = 0;

		            if (bytespp == 4)
			          src_ptr[FI_RGBA_ALPHA] = 0;   
                }
                
                src_ptr += bytespp;
            }
        }
    }
    
    return FIA_SUCCESS;
}

LOGIC < unsigned char >logicUCharImage;
LOGIC < unsigned short >logicUShortImage;
LOGIC < short >logicShortImage;
LOGIC < unsigned long >logicULongImage;
LOGIC < long >logicLongImage;
LOGIC < float >logicFloatImage;
LOGIC < double >logicDoubleImage;

int DLL_CALLCONV
FIA_MaskImage (FIBITMAP * src, FIBITMAP * mask)
{
    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType (src);

    switch (src_type)
    {
        case FIT_BITMAP:
        {
            return logicUCharImage.MaskImage (src, mask);
        }
        case FIT_UINT16:
        {
            return logicUShortImage.MaskImage (src, mask);
        }
        case FIT_INT16:
        {
            return logicShortImage.MaskImage (src, mask);
        }
        case FIT_UINT32:
        {
            return logicULongImage.MaskImage (src, mask);
        }
        case FIT_INT32:
        {
            return logicLongImage.MaskImage (src, mask);
        }
        case FIT_FLOAT:
        {
            return logicFloatImage.MaskImage (src, mask);
        }
        case FIT_DOUBLE:
        {
            return logicDoubleImage.MaskImage (src, mask);
        }
        default:
        {
            break;
        }
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_ReverseMaskImage (FIBITMAP * mask, unsigned char foreground_val)
{
    if (mask == NULL)
    {
        return FIA_ERROR;
    }

    // Mask has to be 8 bit 
    if (FreeImage_GetBPP (mask) != 8 || FreeImage_GetImageType (mask) != FIT_BITMAP)
    {
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (mask);
    int height = FreeImage_GetHeight (mask);

    for(register int y = 0; y < height; y++)
    {
        unsigned char *mask_ptr = (unsigned char *) FreeImage_GetScanLine (mask, y);

        for(register int x = 0; x < width; x++)
        {
            if (!mask_ptr[x])
            {
                mask_ptr[x] = foreground_val;
            }
            else
            {
                mask_ptr[x] = 0;
            }
        }
    }

    return FIA_SUCCESS;
}
