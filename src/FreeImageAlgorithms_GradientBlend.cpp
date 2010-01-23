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

static FIARECT SetRectRelativeToPoint(FIARECT rect, FIAPOINT pt)
{
        FIARECT r;

        r.left = rect.left - pt.x;
        r.right = rect.right - pt.x;
        r.top = rect.top - pt.y;
        r.bottom = rect.bottom - pt.y;

        return r;
}

static FIBITMAP *
FIA_SetAlphaValuesFromDistanceMapImage (FIBITMAP * src, FIBITMAP* alpha_values)
{
    FIBITMAP *colour_dib = FreeImage_ConvertTo32Bits(src);

    if(colour_dib == NULL) {

       FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_SetAlphaValuesFromDistanceMapImage "
                                                 "FreeImage_ConvertTo32Bits Failed");

       return NULL;
    }

    FreeImage_SetTransparent(colour_dib, 1);

    if(FreeImage_SetChannel(colour_dib, alpha_values, FICC_ALPHA) == 0) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_SetAlphaValuesFromDistanceMapImage "
                                                  "FreeImage_SetChannel Failed");

        return NULL;
    }

    if(!FreeImage_IsTransparent(colour_dib))
         return NULL;

    return colour_dib;
}

FIBITMAP* DLL_CALLCONV
FIA_GetGradientAlphaValuesForIntersection(FIARECT rect1, FIARECT rect2, FIARECT *intersect_rect)
{
    FIBITMAP *alpha = NULL, *blended = NULL;
    FIBITMAP *src2_region = NULL, *map = NULL, *map_region = NULL, *src1_cpy = NULL;

    int map_width, map_height;
    int intersect_width = 9, intersect_height = 0;

    FIARECT src1_intersection_rect, src2_intersection_rect, section_map_rect;

    if(!FIA_IntersectingRect(rect1, rect2, intersect_rect))
        goto CLEANUP;	
   
    src1_intersection_rect = SetRectRelativeToPoint(*intersect_rect, MakeFIAPoint(rect1.left, rect1.top));
    src2_intersection_rect = SetRectRelativeToPoint(*intersect_rect, MakeFIAPoint(rect2.left, rect2.top));

    intersect_width = intersect_rect->right - intersect_rect->left + 1;
    intersect_height = intersect_rect->bottom - intersect_rect->top + 1;

    if(rect2.left >= rect1.left && rect2.right <= rect1.right) {

        // Rect2 in completly within rect1 so we use all of the map
        section_map_rect.left = 0;
        section_map_rect.right = intersect_width - 1;

        map_width = intersect_width;
    }
    else if(rect2.right <= rect1.right) {
        section_map_rect.left = intersect_width;
        section_map_rect.right = intersect_width * 2;

        map_width = intersect_width * 2;
    }
    else {
        section_map_rect.left = 0;
        section_map_rect.right = intersect_width - 1;

        map_width = intersect_width * 2;
    }

    if(rect2.top >= rect1.top && rect2.bottom <= rect1.bottom) {
        // Rect2 in completly within rect1 so we use all of the map
        section_map_rect.top = 0;
        section_map_rect.bottom = intersect_height - 1;

        map_height = intersect_height;
    }
    else if(rect2.bottom <= rect1.bottom) {
        section_map_rect.top = intersect_height;
        section_map_rect.bottom = section_map_rect.top + intersect_height * 2;

        map_height = intersect_height * 2;
    }
    else {
        section_map_rect.top = 0;
        section_map_rect.bottom = intersect_height - 1;

        map_height = intersect_height * 2;
    }

    // Generate a distance map twice the size of this intersection
    // We only wish half the map to produce a one way gradient
    map = FIA_DistanceMap (map_width, map_height, 1);

    map_region = FIA_CopyLeftTopWidthHeight (map, section_map_rect.left, section_map_rect.top,
           intersect_width, intersect_height);

    if(map_region == NULL) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_Copy returned NULL");

        goto CLEANUP;
    }

    if(map != NULL)
        FreeImage_Unload(map);

	// Debug
	//FIBITMAP *std_map_region = FreeImage_ConvertToStandardType(map_region);
	//FIA_SaveFIBToFile(std_map_region, "C:\\map.bmp", BIT24);
	//FreeImage_Unload(std_map_region);
    return map_region;
	
CLEANUP:

    if(map != NULL)
        FreeImage_Unload(map);

    if(map_region != NULL)
        FreeImage_Unload(map_region);

    return NULL;	
}

FIBITMAP* DLL_CALLCONV
FIA_GetGradientBlendAlphaImage (FIBITMAP* src2, FIARECT rect1, FIARECT rect2, FIARECT *intersect_rect)
{
    FIARECT intersection_rect, src1_intersection_rect, src2_intersection_rect;

    FIBITMAP *alpha = NULL, *blended = NULL;
    FIBITMAP *src2_region = NULL, *map_region = NULL, *src1_cpy = NULL, *src2_32bit = NULL;

    // Make sure the passed in rect 2 does not exceed the size of the corresponding image.
    FIA_RectChangeWidthHeight (&rect2, FreeImage_GetWidth(src2), FreeImage_GetHeight(src2));

    map_region = FIA_GetGradientAlphaValuesForIntersection(rect1, rect2, &intersection_rect);

    if(map_region == NULL) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_GetGradientAlphaValuesForIntersection returned NULL");

        goto CLEANUP;
    }

    src1_intersection_rect = SetRectRelativeToPoint(intersection_rect, MakeFIAPoint(rect1.left, rect1.top));
    src2_intersection_rect = SetRectRelativeToPoint(intersection_rect, MakeFIAPoint(rect2.left, rect2.top));

    *intersect_rect = src1_intersection_rect;

    src2_region = FIA_Copy(src2, src2_intersection_rect.left, src2_intersection_rect.top,
                src2_intersection_rect.right, src2_intersection_rect.bottom);

    if(src2_region == NULL) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_Copy returned NULL (src2_region)");

        goto CLEANUP;
    }
	
    if(FIA_CheckSizesAreSame(src2_region, map_region) == 0) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_CheckSizesAreSame Failed "
                "Src2 Region: width: %d height: %d - Map Region: width: %d height: %d",
                FreeImage_GetWidth(src2_region), FreeImage_GetHeight(src2_region),
                FreeImage_GetWidth(map_region), FreeImage_GetHeight(map_region));

        goto CLEANUP;
    }

    if(FIA_CheckSizesAreSame(src2_region, map_region) == 0) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_CheckSizesAreSame Failed "
                "Src2 Region: width: %d height: %d - Map Region: width: %d height: %d",
                FreeImage_GetWidth(src2_region), FreeImage_GetHeight(src2_region),
                FreeImage_GetWidth(map_region), FreeImage_GetHeight(map_region));

        goto CLEANUP;
    }

    FIA_InPlaceConvertToStandardType(&map_region, 1);

    alpha = FIA_SetAlphaValuesFromDistanceMapImage (src2_region, map_region);
	
    if(alpha == NULL) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_SetAlphaValuesFromDistanceMapImage Failed");

        goto CLEANUP;
    }

    // Paste the alspha value region into the original src image (convertedted to 32 bit colour of course.
    src2_32bit = FreeImage_ConvertTo32Bits(src2);

    FIA_PasteFromTopLeft(src2_32bit, alpha, src2_intersection_rect.left, src2_intersection_rect.top);

    if(src2_region != NULL)
        FreeImage_Unload(src2_region);

    if(map_region != NULL)
        FreeImage_Unload(map_region);

    if(alpha != NULL)
        FreeImage_Unload(alpha);

    if(blended != NULL)
        FreeImage_Unload(blended);

    return src2_32bit;

CLEANUP:

    if(src2_region != NULL)
        FreeImage_Unload(src2_region);

    if(map_region != NULL)
        FreeImage_Unload(map_region);

    if(alpha != NULL)
        FreeImage_Unload(alpha);

    if(blended != NULL)
        FreeImage_Unload(blended);

    return NULL;
}

// src1 mask is a bitmap that specifies that only pixels corresponding to
// the non zero mask values will be blended
FIBITMAP* DLL_CALLCONV
FIA_GradientBlendedIntersectionImage (FIBITMAP * src1, FIARECT rect1, FIBITMAP* src2, FIARECT rect2, 
									  FIBITMAP *src1_mask, FIARECT *intersect_image_rect)
{
    FIARECT intersection_rect, src1_intersection_rect,  src2_intersection_rect;

    FIBITMAP *alpha = NULL, *blended = NULL, *src1_region = NULL;
    FIBITMAP *src2_region = NULL, *map_region = NULL, *src1_cpy = NULL, *src1_mask_region = NULL;

    // Make sure the passed in rect 2 does not exceed the size of the corresponding image.
    FIA_RectChangeWidthHeight (&rect1, FreeImage_GetWidth(src1), FreeImage_GetHeight(src1));
    FIA_RectChangeWidthHeight (&rect2, FreeImage_GetWidth(src2), FreeImage_GetHeight(src2));

    map_region = FIA_GetGradientAlphaValuesForIntersection(rect1, rect2, &intersection_rect);

// Debug
        FIBITMAP *std_map_region = FreeImage_ConvertToStandardType(map_region);
        FIA_SaveFIBToFile(std_map_region, "/home/glenn/map.bmp", BIT24);
        FreeImage_Unload(std_map_region);


    if(map_region == NULL) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_GetGradientAlphaValuesForIntersection returned NULL");

        goto CLEANUP;
    }

    src1_intersection_rect = SetRectRelativeToPoint(intersection_rect, MakeFIAPoint(rect1.left, rect1.top));
    src2_intersection_rect = SetRectRelativeToPoint(intersection_rect, MakeFIAPoint(rect2.left, rect2.top));

    *intersect_image_rect = src1_intersection_rect;

    src1_region = FIA_Copy(src1, src1_intersection_rect.left, src1_intersection_rect.top,
            src1_intersection_rect.right, src1_intersection_rect.bottom);

    if(src1_region == NULL) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_Copy returned NULL (src1_region)");

        goto CLEANUP;
    }

	if(src1_mask != NULL) {
		src1_mask_region = FIA_Copy(src1_mask, src1_intersection_rect.left, src1_intersection_rect.top,
				src1_intersection_rect.right, src1_intersection_rect.bottom);

		if(src1_mask_region == NULL) {

			FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_Copy returned NULL (src1_mask_region)");

			goto CLEANUP;
		}
	}
	
    src2_region = FIA_Copy(src2, src2_intersection_rect.left, src2_intersection_rect.top,
                src2_intersection_rect.right, src2_intersection_rect.bottom);

    if(src2_region == NULL) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_Copy returned NULL (src2_region)");

        goto CLEANUP;
    }

    if(FIA_CheckSizesAreSame(src1_region, src2_region) == 0) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_CheckSizesAreSame Failed");

        goto CLEANUP;
    }

    if(FIA_CheckSizesAreSame(src2_region, map_region) == 0) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FIA_CheckSizesAreSame Failed "
                "Src2 Region: width: %d height: %d - Map Region: width: %d height: %d",
                FreeImage_GetWidth(src2_region), FreeImage_GetHeight(src2_region),
                FreeImage_GetWidth(map_region), FreeImage_GetHeight(map_region));

        goto CLEANUP;
    }

    blended = FIA_Composite(src2_region, src1_region, map_region, src1_mask_region);

    if(blended == NULL) {

        FreeImage_OutputMessageProc (FIF_UNKNOWN, "FreeImage_Composite returned NULL");

        goto CLEANUP;
    }

    if(src1_region != NULL)
        FreeImage_Unload(src1_region);

	if(src1_mask_region != NULL)
        FreeImage_Unload(src1_mask_region);

    if(src2_region != NULL)
        FreeImage_Unload(src2_region);

    if(map_region != NULL)
        FreeImage_Unload(map_region);

    if(alpha != NULL)
        FreeImage_Unload(alpha);

    return blended;

CLEANUP:

    if(src1_region != NULL)
        FreeImage_Unload(src1_region);

    if(src2_region != NULL)
        FreeImage_Unload(src2_region);

    if(map_region != NULL)
        FreeImage_Unload(map_region);

    if(alpha != NULL)
        FreeImage_Unload(alpha);

    return NULL;
}


int DLL_CALLCONV
FIA_GradientBlend (FIBITMAP * src1, FIARECT rect1, FIBITMAP* src2, FIARECT rect2, FIBITMAP *mask)
{
    FIARECT intersect_image_rect;

    FIBITMAP* blended = FIA_GradientBlendedIntersectionImage (src1, rect1, src2, rect2, mask, &intersect_image_rect);

    FIA_PasteFromTopLeft(src1, blended,  intersect_image_rect.left, intersect_image_rect.top);

    if(blended != NULL)
        FreeImage_Unload(blended);

    return FIA_SUCCESS;
}


/*
int DLL_CALLCONV
FIA_GradientBlendRGBA (FIBITMAP * src1, FIARECT rect1, FIBITMAP* src2, FIARECT rect2)
{
    FIARECT intersect_image_rect, src2_intersection_rect;
    int intersec_width, intersec_height;

    // Check image in RGBA
    if(FreeImage_GetImageType(src1) != FIT_BITMAP || FreeImage_GetBPP(src1) != 32) {

	FreeImage_OutputMessageProc (FIF_UNKNOWN, "Passed image in not a 32bit FIT_BITMAP");
	return FIA_ERROR;
    }

    FIBITMAP* blended = FIA_GradientBlendedIntersectionImage (src1, rect1, src2, rect2, NULL, &intersect_image_rect);

    // We have an intersection now. We must mask the alpha values out of this region as 255 ie fully opaque
    
    src2_intersection_rect = SetRectRelativeToPoint(intersection_rect, MakeFIAPoint(rect2.left, rect2.top));
	
    intersec_width = src2_intersection_rect.right - src2_intersection_rect.left + 1;
    intersec_width = src2_intersection_rect.bottom - src2_intersection_rect.top + 1;

    FIBITMAP *alpha_dib = FreeImage_Allocate(FreeImage_GetWidth(dib2), FreeImage_GetHeight(dib2), 8, 0, 0, 0);

    // Make the default opaque foreground
    FIA_DrawSolidGreyscaleRect (alpha_dib, MakeFIARect(0,0, FreeImage_GetWidth(alpha_dib) - 1, FreeImage_GetHeight(alpha_dib) - 1), 255);

    // Make the intersection a 




  FreeImage_SetTransparent(dib2, 1);

  FIBITMAP *alpha_dib = FreeImage_Allocate(FreeImage_GetWidth(dib1), FreeImage_GetHeight(dib1), 8, 0, 0, 0);

  FIA_DrawSolidGreyscaleRect (alpha_dib, MakeFIARect(0,0, FreeImage_GetWidth(alpha_dib) - 1, FreeImage_GetHeight(alpha_dib) - 1), 255);
  FIA_DrawSolidGreyscaleRect (alpha_dib, MakeFIARect(50,50, 200, 200), 120);

  if(FreeImage_SetChannel(dib2, alpha_dib, FICC_ALPHA) == 0) {
        std::cout << "Error calling FreeImage_SetChannel" << std::endl;

        return;
  }

  if(!FreeImage_IsTransparent(dib2)) {
        std::cout << "Not transparent" << std::endl;
    return;
  }

  FIA_DrawColourSolidRect(dst, MakeFIARect(0,0, FreeImage_GetWidth(dst) - 1,^M
        FreeImage_GetHeight(dst) - 1), FIA_RGBQUAD(255,255,0));^M
^M




















    FIA_PasteFromTopLeft(src1, blended,  intersect_image_rect.left, intersect_image_rect.top);

    if(blended != NULL)
        FreeImage_Unload(blended);

    return FIA_SUCCESS;
}
*/

int DLL_CALLCONV
FIA_GradientBlendPasteFromTopLeft (FIBITMAP * background, FIBITMAP *src1, FIBITMAP* src2, int src1_left, int src1_top,
									 		  int src2_left, int src2_top)
{
    int dst_width = FreeImage_GetWidth(background);
    int dst_height = FreeImage_GetHeight(background);

    int src1_width = FreeImage_GetWidth(src2);
    int src1_height = FreeImage_GetHeight(src2);

    int src2_width = FreeImage_GetWidth(src2);
    int src2_height = FreeImage_GetHeight(src2);

    FIARECT src1_rect = MakeFIARect(src1_left,src1_top, src1_left+ src1_width - 1, src1_top + src1_height - 1);
    FIARECT src2_rect = MakeFIARect(src2_left,src2_top, src2_left + src2_width - 1, src2_top + src2_height - 1);

    FIARECT intersect_image_rect;

    FIBITMAP* intersection_image = FIA_GradientBlendedIntersectionImage (src1, src1_rect, src2, src2_rect, NULL, &intersect_image_rect);

    // Paste first image onto background
    // Then paste the second image
    // The paste the gradient section
    FIA_PasteFromTopLeft(background, src1, src1_left, src1_top);
    FIA_PasteFromTopLeft(background, src2, src2_left, src2_top);
    FIA_PasteFromTopLeft(background, intersection_image, src1_left + intersect_image_rect.left, src1_top + intersect_image_rect.top);

    if(intersection_image != NULL)
        FreeImage_Unload(intersection_image);

     return FIA_SUCCESS;
}

/*
int DLL_CALLCONV
FIA_GradientBlendRGBAPasteFromTopLeft (FIBITMAP * dst, FIBITMAP* src, int left, int top)
{
    int dst_width = FreeImage_GetWidth(dst);
    int dst_height = FreeImage_GetHeight(dst);

    int src_width = FreeImage_GetWidth(src);
    int src_height = FreeImage_GetHeight(src);

    FIARECT dst_rect = MakeFIARect(0,0,dst_width-1,dst_height-1);
    FIARECT src_rect = MakeFIARect(left,top,left + src_width-1, top + src_height-1);

    return FIA_GradientBlend (dst, dst_rect, src, src_rect, NULL);
}

*/
