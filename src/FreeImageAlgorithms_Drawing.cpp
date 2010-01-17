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
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

#include <iostream>
#include <math.h>

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_renderer_primitives.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"
#include "agg_ellipse.h"
#include "agg_glyph_raster_bin.h"
#include "agg_renderer_raster_text.h"
#include "agg_embedded_raster_fonts.h"

template<class Rasterizer>
static void
RasterLine (Rasterizer& ras, double x1, double y1, double x2, double y2, double width)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double d = sqrt (dx * dx + dy * dy);

    dx = 0.5 * width * (y2 - y1) / d;
    dy = 0.5 * width * (x2 - x1) / d;

    ras.move_to_d (x1 - dx, y1 + dy);
    ras.line_to_d (x2 - dx, y2 + dy);
    ras.line_to_d (x2 + dx, y2 - dy);
    ras.line_to_d (x1 + dx, y1 - dy);
}

template<class RendererType>
static int
DrawLine (RendererType& renderer, FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, RGBQUAD colour, int line_width, int antialiased)
{
    // the Rasterizer definition
    agg::rasterizer_scanline_aa<> ras;
    ras.reset();
 
    agg::scanline_p8 scanline;

    RasterLine (ras, p1.x, p1.y, p2.x, p2.y, line_width);

    if (antialiased)
    {
        agg::render_scanlines_aa_solid(ras, scanline, renderer, agg::rgba8(colour.rgbRed, colour.rgbGreen, colour.rgbBlue));
    }
    else
    {
        agg::render_scanlines_bin_solid(ras, scanline, renderer, agg::rgba8 (colour.rgbRed, colour.rgbGreen, colour.rgbBlue));
    }

    return FIA_SUCCESS;
}

template<typename RendererType, typename ColorT>
static int
DrawEllipse (RendererType& renderer, FIBITMAP * src, FIARECT rect, const ColorT& colour, int solid, int line_width, int antialiased)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    if (rect.left < 0)
    {
        rect.left = 0;
        rect.right += rect.left;
    }

    if (rect.top < 0)
    {
        rect.top = 0;
        rect.bottom += rect.top;
    }

    if (rect.right >= width)
    {
        rect.right = width - 1;
    }

    if (rect.bottom >= height)
    {
        rect.bottom = height - 1;
    }

    // Allocate the framebuffer
    FIARECT tmp_rect = rect;

    // FreeImages are flipped
    tmp_rect.top = height - rect.top - 1;
    tmp_rect.bottom = height - rect.bottom - 1;

    // the Rasterizer definition
    agg::rasterizer_scanline_aa<> ras;
    ras.reset();

    agg::scanline_p8 scanline;

    agg::ellipse ellipse;

    int w = tmp_rect.right - tmp_rect.left;
    int h = tmp_rect.bottom - tmp_rect.top;

    double center_x = (int) (tmp_rect.left + w / 2.0);
    double center_y = (int) (tmp_rect.top + h / 2.0);

    ellipse.init(center_x, center_y, w / 2, h / 2, 360);
    
    if(solid) {
        ras.add_path(ellipse, 0);
    }
    else {      
        agg::conv_stroke<agg::ellipse> poly(ellipse);
        poly.width(line_width);
        ras.add_path(poly, 0);
    }

    if(antialiased)
        agg::render_scanlines_aa_solid(ras, scanline, renderer, colour);
    else
        agg::render_scanlines_bin_solid(ras, scanline, renderer, colour);

    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_DrawColourSolidEllipse (FIBITMAP * src, FIARECT rect, RGBQUAD colour, int antialiased)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);
    int bpp = FreeImage_GetBPP(src);

    unsigned char *buf = FreeImage_GetBits (src);

    // Create the rendering buffer
    agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

    if (type == FIT_BITMAP && bpp == 32)
    {
        typedef agg::pixfmt_bgra32                       pixfmt_type;
    	typedef agg::renderer_base < pixfmt_type >       renbase_type;
     
        pixfmt_type pixf(rbuf);
        renbase_type rbase(pixf);

        return DrawEllipse (rbase, src, rect, agg::rgba8 (colour.rgbRed, colour.rgbGreen, colour.rgbBlue), 1, 0, antialiased);
    }
    else if (type == FIT_BITMAP && bpp == 24)
    {
	    typedef agg::pixfmt_bgr24                        pixfmt_type;
    	typedef agg::renderer_base < pixfmt_type >       renbase_type;

        pixfmt_type pixf(rbuf);
        renbase_type rbase(pixf);

        return DrawEllipse (rbase, src, rect, agg::rgba8 (colour.rgbRed, colour.rgbGreen, colour.rgbBlue), 1, 0, antialiased);
    }

    return FIA_ERROR;
}


int DLL_CALLCONV
FIA_DrawSolidGreyscaleEllipse (FIBITMAP * src, FIARECT rect, unsigned char value, int antialiased)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    // Allocate the framebuffer
    unsigned char *buf = FreeImage_GetBits (src);

    switch (type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8) {
                
                typedef agg::pixfmt_gray8                        pixfmt_type;
                typedef agg::renderer_base < pixfmt_type >       renbase_type;

                // Create the rendering buffer
                agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

                pixfmt_type pixf(rbuf);
                renbase_type rbase(pixf);

                RGBQUAD colour = FIA_RGBQUAD ((unsigned char) value, (unsigned char) value, (unsigned char) value);

                return DrawEllipse (rbase, src, rect, agg::rgba8 (colour.rgbRed, colour.rgbGreen, colour.rgbBlue), 1, 0, antialiased);
            }

            break;
        }
        case FIT_UINT16:
        {      
            typedef agg::pixfmt_gray16                       pixfmt_type;
            typedef agg::renderer_base < pixfmt_type >       renbase_type;

            // Create the rendering buffer
            agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

            pixfmt_type pixf(rbuf);
            renbase_type rbase(pixf);

            return DrawEllipse (rbase, src, rect, agg::gray16(value), 1, 0, antialiased);

       }
        case FIT_INT16:
        {   
            break;
        }
        case FIT_UINT32:
        {             
            break;
        }
        case FIT_INT32:
        {
            break;
        }
        case FIT_FLOAT:
        {
            break;
        }
        case FIT_DOUBLE:
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



template<typename RendererType, typename ColorT>
static int
DrawPolygon (RendererType& renderer, FIBITMAP * src, FIAPOINT * points, int number_of_points, const ColorT& colour, int solid, int line_width, int antialiased)
{
    // the Rasterizer definition
    agg::rasterizer_scanline_aa<> ras;
    ras.reset();

    agg::scanline_p8 scanline;

    agg::path_storage ps;

    ps.move_to (points[0].x, points[0].y);

    for(int i = 1; i < number_of_points; i++)
    {
        ps.line_to (points[i].x, points[i].y);
    }

    if(solid) {
        ras.add_path(ps, 0);
    }
    else {      
        agg::conv_stroke<agg::path_storage> poly(ps);
        poly.width(line_width);
        ras.add_path(poly, 0);
    }

    if(antialiased)
        agg::render_scanlines_aa_solid(ras, scanline, renderer, colour);
    else
        agg::render_scanlines_bin_solid(ras, scanline, renderer, colour);

    return FIA_SUCCESS;
}


int DLL_CALLCONV
FIA_DrawSolidGreyscalePolygon (FIBITMAP * src, FIAPOINT * points,
                          int number_of_points, unsigned char value, int antialiased)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    // Allocate the framebuffer
    unsigned char *buf = FreeImage_GetBits (src);

    switch (type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8) {
                
                typedef agg::pixfmt_gray8                        pixfmt_type;
                typedef agg::renderer_base < pixfmt_type >       renbase_type;

                // Create the rendering buffer
                agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

                pixfmt_type pixf(rbuf);
                renbase_type rbase(pixf);

                RGBQUAD colour = FIA_RGBQUAD ((unsigned char) value, (unsigned char) value, (unsigned char) value);

                return DrawPolygon (rbase, src, points, number_of_points,
                            agg::rgba8 (colour.rgbRed, colour.rgbGreen, colour.rgbBlue), 1, 0, antialiased);
            }
        }
        default:
        {
            break;
        }
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_DrawColourSolidPolygon (FIBITMAP * src, FIAPOINT * points,
                          int number_of_points, RGBQUAD colour, int antialiased)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);
    int bpp = FreeImage_GetBPP(src);

    unsigned char *buf = FreeImage_GetBits (src);

    // Create the rendering buffer
    agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

    if (type == FIT_BITMAP && bpp == 32)
    {
        typedef agg::pixfmt_bgra32                       pixfmt_type;
    	typedef agg::renderer_base < pixfmt_type >       renbase_type;
     
        pixfmt_type pixf(rbuf);
        renbase_type rbase(pixf);


        return DrawPolygon (rbase, src, points, number_of_points,
                            agg::rgba8 (colour.rgbRed, colour.rgbGreen, colour.rgbBlue), 1, 0, antialiased);
    }
    else if (type == FIT_BITMAP && bpp == 24)
    {
	    typedef agg::pixfmt_bgr24                        pixfmt_type;
    	typedef agg::renderer_base < pixfmt_type >       renbase_type;

        pixfmt_type pixf(rbuf);
        renbase_type rbase(pixf);



        return DrawPolygon (rbase, src, points, number_of_points,
                            agg::rgba8 (colour.rgbRed, colour.rgbGreen, colour.rgbBlue), 1, 0, antialiased);
    }

    return FIA_ERROR;
}

// Draws a orthogonal no aa line of width one pixel
// This is for drawing rectangles fast without subpixel position as with agg.
template<typename ValueType>
static int
orthogonal_draw_line (FIBITMAP * src, int x1, int y1, int x2, int y2, ValueType value, RGBQUAD colour)
{
    if (!src)
    {
        return FIA_ERROR;
    }

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    // Draw from the left
    if (x2 < x1)
    {
        SWAP (x1, x2);
    }

    // Draw from the top
    if (y2 < y1)
    {
        SWAP (y1, y2);
    }

    if (x2 < 0 || y2 < 0)
    {
        return FIA_ERROR;
    }

    if (x1 < 0)
    {
        x1 = 0;
    }

    if (x2 >= width)
    {
        x2 = width - 1;
    }

    if (y1 < 0)
    {
        y1 = 0;
    }

    if (y2 >= height)
    {
        y2 = height - 1;
    }

    int bytespp = FreeImage_GetLine (src) / FreeImage_GetWidth (src);
    int pitch = FreeImage_GetPitch (src);

    bool greyscale_image = true;

    if(FreeImage_GetImageType(src) == FIT_BITMAP && FreeImage_GetBPP(src) > 8) {
	    greyscale_image = false;
    }

    if (x1 != x2)
    {
        // We have a horizontal line
        // Make sure y's are the same
        if (y1 != y2)
        {
            return FIA_ERROR;
        }

        BYTE *bits = (BYTE *) FreeImage_GetScanLine (src, y1) + (x1 * bytespp);

	    if(greyscale_image)
	    {
            	while (x1 <= x2)
            	{
                	*bits = value;  
                    // jump to next pixel
		            bits += bytespp;
                    x1++;
            	}
	    }
	    else {

		    for(register int x = x1; x <= x2; x++)
            	{
		        bits[FI_RGBA_RED] = colour.rgbRed;
		        bits[FI_RGBA_GREEN] = colour.rgbGreen;
		        bits[FI_RGBA_BLUE] = colour.rgbBlue;

		        if (bytespp == 4)
		        {
		            bits[FI_RGBA_ALPHA] = 0;
		        }

		        // jump to next pixel
		        bits += bytespp;
		    }
	    }
	
        return FIA_SUCCESS;
    }

    if (y1 != y2)
    {
        // We have a verticle line
        // Make sure x's are the same
        if (x1 != x2)
        {
            return FIA_ERROR;
        }

        // Get starting point
        BYTE *bits = (BYTE *) (FreeImage_GetScanLine (src, y1) + (x1 * bytespp));

	    if(greyscale_image)
	    {
		    while (y1 <= y2)
		    {
		        *bits = value;

		        bits += pitch;

		        y1++;
		    }
	    }
	    else {

		    while (y1 <= y2)
		    {
		        bits[FI_RGBA_RED] = colour.rgbRed;
		        bits[FI_RGBA_GREEN] = colour.rgbGreen;
		        bits[FI_RGBA_BLUE] = colour.rgbBlue;

		        if (bytespp == 4)
		        {
			        bits[FI_RGBA_ALPHA] = 0;
		        }

		        bits += pitch;

		        y1++;
		    }
	    }

        return FIA_SUCCESS;
    }

    return FIA_ERROR;
}

template<typename ValueType>
static int
DrawRectangle (FIBITMAP * src, FIARECT rect, ValueType value, RGBQUAD color_value, int line_width)
{
    int err = FIA_ERROR;

    for(int i = 0; i < line_width; i++)
    {
        // Top
        err = orthogonal_draw_line (src, rect.left - line_width + 1, rect.top + i, rect.right
                                           + line_width - 1, rect.top + i, value, color_value);

        // Bottom
        err = orthogonal_draw_line (src, rect.left - line_width + 1, rect.bottom - i,
                                           rect.right + line_width - 1, rect.bottom - i, value, color_value);

        // Left
        err = orthogonal_draw_line (src, rect.left - i, rect.top, rect.left - i, rect.bottom,
                                           value, color_value);

        // Right
        err = orthogonal_draw_line (src, rect.right + i, rect.top, rect.right + i,
                                           rect.bottom, value, color_value);
    }

    if (err == FIA_ERROR)
    {
        return FIA_ERROR;
    }

    return FIA_SUCCESS;
}

template<typename ValueType>
static int
DrawSolidRectangle (FIBITMAP * src, FIARECT rect, ValueType value, RGBQUAD colour)
{
    // Seems that Anti grain method is to slow probably  because it is too advanced
    // Does accurate drawing etc with anti aliasing.
    // We just want a simple rectangle with no antialising or sub pixel position.
    // Would we ever want that for rectangles ?
  
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    if (rect.left < 0)
    {
        rect.left = 0;
        rect.right += rect.left;
    }

    if (rect.top < 0)
    {
        rect.top = 0;
        rect.bottom += rect.top;
    }

    if (rect.right >= width)
    {
        rect.right = width - 1;
    }

    if (rect.bottom >= height)
    {
        rect.bottom = height - 1;
    }

    // Allocate the framebuffer
    FIARECT tmp_rect = rect;

    // FreeImages are flipped
    tmp_rect.top = height - rect.top - 1;
    tmp_rect.bottom = height - rect.bottom - 1;

    int right = tmp_rect.left + (tmp_rect.right - tmp_rect.left);

    bool greyscale_image = true;

    if(FreeImage_GetImageType(src) == FIT_BITMAP && FreeImage_GetBPP(src) > 8) {
	    greyscale_image = false;
    }
   
    if(greyscale_image)
    {
        // Allocate the framebuffer
        ValueType *buf = NULL;
        
        for(register int y = tmp_rect.bottom; y <= tmp_rect.top; y++)
        {
            buf = (ValueType*) FreeImage_GetScanLine (src, y);

            for(register int x = tmp_rect.left; x <= right; x++)
                buf[x] = value;
        }
    }
    else {

        BYTE *buf = NULL;

        int bytespp = FreeImage_GetLine (src) / FreeImage_GetWidth (src);
        int pitch = FreeImage_GetPitch (src);

        for(register int y = tmp_rect.bottom; y <= tmp_rect.top; y++)
        {
            buf = (BYTE*) FreeImage_GetScanLine (src, y);
            buf += (tmp_rect.left * bytespp);

            for(register int x = tmp_rect.left; x <= right; x++) {
        
                buf[FI_RGBA_RED] = colour.rgbRed;
		        buf[FI_RGBA_GREEN] = colour.rgbGreen;
		        buf[FI_RGBA_BLUE] = colour.rgbBlue;

		        if (bytespp == 4)
		        {
			        buf[x + FI_RGBA_ALPHA] = 0;
		        }

		        buf += bytespp;

            }
        }

    }

    return FIA_SUCCESS;
}


int DLL_CALLCONV
FIA_DrawSolidGreyscaleRect (FIBITMAP * src, FIARECT rect, double value)
{
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    RGBQUAD colour = FIA_RGBQUAD (0, 0, 0);

    switch (type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
                return DrawSolidRectangle (src, rect, (unsigned char) value, colour);
            break;
        }
        case FIT_UINT16:
        {                       // array of unsigned short: unsigned 16-bit
            return DrawSolidRectangle (src, rect, (unsigned short) value, colour);
            break;
        }
        case FIT_INT16:
        {                       // array of short: signed 16-bit
            return DrawSolidRectangle (src, rect, (short) value, colour);
            break;
        }
        case FIT_UINT32:
        {                       // array of unsigned long: unsigned 32-bit
            return DrawSolidRectangle (src, rect, (unsigned long) value, colour);
            break;
        }
        case FIT_INT32:
        {                       // array of long: signed 32-bit
            return DrawSolidRectangle (src, rect, (long) value, colour);
            break;
        }
        case FIT_FLOAT:
        {                       // array of float: 32-bit
            return DrawSolidRectangle (src, rect, (float) value, colour);
            break;
        }
        case FIT_DOUBLE:
        {                       // array of double: 64-bit
            return DrawSolidRectangle (src, rect, (double) value, colour);
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
FIA_DrawColourSolidRect (FIBITMAP * src, FIARECT rect, RGBQUAD colour)
{
    return DrawSolidRectangle (src, rect, 0, colour);
}

int DLL_CALLCONV
FIA_DrawColourLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, RGBQUAD colour,
                    int line_width, int antialiased)
{
    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);

    FIAPOINT p1_tmp = p1, p2_tmp = p2;

    // FreeImages are flipped
    p1_tmp.y = height - p1.y;
    p2_tmp.y = height - p2.y;

    int bpp = FreeImage_GetBPP (src);
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    // Allocate the framebuffer
    unsigned char *buf = FreeImage_GetBits (src);

    // Create the rendering buffer
    agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

    if (type == FIT_BITMAP && bpp == 32)
    {
        typedef agg::pixfmt_bgra32                       pixfmt_type;
    	typedef agg::renderer_base < pixfmt_type >       renbase_type;

    	pixfmt_type pixf(rbuf);
    	renbase_type rbase(pixf);

        return DrawLine (rbase, src, p1_tmp, p2_tmp, colour, line_width, antialiased);
    }

    if (type == FIT_BITMAP && bpp == 24)
    {
	typedef agg::pixfmt_bgr24                        pixfmt_type;
    	typedef agg::renderer_base < pixfmt_type >       renbase_type;

    	// Create the rendering buffer
    	agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

    	pixfmt_type pixf(rbuf);
    	renbase_type rbase(pixf);

    	return DrawLine (rbase, src, p1_tmp, p2_tmp, colour, line_width, antialiased);
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_DrawGreyscaleLine (FIBITMAP * src, FIAPOINT p1, FIAPOINT p2, double value,
                       int line_width, int antialiased)
{
    int width = FreeImage_GetWidth(src);
    int height = FreeImage_GetHeight (src);

    FIAPOINT p1_tmp = p1, p2_tmp = p2;

    // FreeImages are flipped
    p1_tmp.y = height - p1.y;
    p2_tmp.y = height - p2.y;

    int bpp = FreeImage_GetBPP (src);
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    // Allocate the framebuffer
    unsigned char *buf = FreeImage_GetBits (src);

    // Create the rendering buffer
    agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

    if (type == FIT_BITMAP && bpp == 8)
    {
	    typedef agg::pixfmt_gray8                        pixfmt_type;
        typedef agg::renderer_base < pixfmt_type >       renbase_type;

        // Create the rendering buffer
        agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

        pixfmt_type pixf(rbuf);
        renbase_type rbase(pixf);

	    RGBQUAD colour;

	    colour.rgbRed = (unsigned char) value;
	    colour.rgbGreen = (unsigned char) value;
	    colour.rgbBlue = (unsigned char) value;

        return DrawLine (rbase, src, p1_tmp, p2_tmp, colour, line_width, antialiased);
    }

    return FIA_ERROR;
}

int DLL_CALLCONV
FIA_DrawColourRect (FIBITMAP * src, FIARECT rect, RGBQUAD colour, int line_width)
{
    int height = FreeImage_GetHeight (src);

    FIARECT tmp_rect = rect;

    // FreeImages are flipped
    tmp_rect.top = height - rect.top - 1;
    tmp_rect.bottom = height - rect.bottom - 1;

    return DrawRectangle (src, tmp_rect, 0, colour, line_width);
}


int DLL_CALLCONV
FIA_DrawGreyscaleRect (FIBITMAP * src, FIARECT rect, double value, int line_width)
{
    int height = FreeImage_GetHeight (src);

    FIARECT tmp_rect = rect;

    // FreeImages are flipped
    tmp_rect.top = height - rect.top - 1;
    tmp_rect.bottom = height - rect.bottom - 1;

    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    RGBQUAD colour = FIA_RGBQUAD (0, 0, 0);

    switch (type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8)
                return DrawRectangle (src, tmp_rect, (unsigned char) value, colour, line_width);
            break;
        }
        case FIT_UINT16:
        {                       // array of unsigned short: unsigned 16-bit
            return DrawRectangle (src, tmp_rect, (unsigned short) value, colour, line_width);
            break;
        }
        case FIT_INT16:
        {                       // array of short: signed 16-bit
            return DrawRectangle (src, tmp_rect, (short) value, colour, line_width);
            break;
        }
        case FIT_UINT32:
        {                       // array of unsigned long: unsigned 32-bit
            return DrawRectangle (src, tmp_rect, (unsigned long) value, colour, line_width);
            break;
        }
        case FIT_INT32:
        {                       // array of long: signed 32-bit
            return DrawRectangle (src, tmp_rect, (long) value, colour, line_width);
            break;
        }
        case FIT_FLOAT:
        {                       // array of float: 32-bit
            return DrawRectangle (src, tmp_rect, (float) value, colour, line_width);
            break;
        }
        case FIT_DOUBLE:
        {                       // array of double: 64-bit
            return DrawRectangle (src, tmp_rect, (double) value, colour, line_width);
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
FIA_DrawGreyScaleCheckerBoard (FIBITMAP * src, int square_size)
{
	int row;   // Row number, from 0 to cols
    int col;   // Column number, from 0 to rows
	int x, y;   // Top-left corner of square
	double value;

	// Minimum grid size
	if(square_size < 5)
		square_size = 5;

	// Sret Maximum grid size
	if(square_size > 500)
		square_size = 500;

	int width =  FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	int cols = width / square_size;
	int rows = height / square_size;

	for ( row = 0;  row < rows + 1;  row++ ) {

         for ( col = 0;  col < cols + 1;  col++) {

            x = col * square_size;
            y = row * square_size;

            if ( (row % 2) == (col % 2) )
               value = 255.0;
            else
               value = 0.0;

			if(FIA_DrawSolidGreyscaleRect (src, MakeFIARect(x, y, x + square_size, y + square_size), value) == FIA_ERROR)
				return FIA_ERROR;
         }
      }

	return FIA_SUCCESS;
}


int DLL_CALLCONV
FIA_DrawHorizontalColourText (FIBITMAP *src, int left, int top, const char *text, RGBQUAD colour)
{
    typedef agg::pixfmt_bgr24 pixfmt;
    typedef agg::renderer_base<pixfmt> ren_base;
    typedef agg::glyph_raster_bin<agg::rgba8> glyph_gen;

    int width = FreeImage_GetWidth (src);
    int height = FreeImage_GetHeight (src);
    unsigned char *buf = FreeImage_GetBits (src);

    // Create the rendering buffer
    agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

    glyph_gen glyph(0);
    pixfmt pixf(rbuf);
    ren_base rbase(pixf);

    agg::renderer_raster_htext_solid<ren_base, glyph_gen> renderer(rbase, glyph);
  
    renderer.color(agg::rgba8(colour.rgbRed, colour.rgbGreen, colour.rgbBlue));

    glyph.font(agg::verdana18_bold);

    renderer.render_text(left, top, text, false);
    
    return FIA_SUCCESS;
}

int DLL_CALLCONV
FIA_DrawHorizontalGreyscaleText (FIBITMAP * src, int left, int top, const char *text, unsigned char value)
{
    FREE_IMAGE_TYPE type = FreeImage_GetImageType (src);

    switch (type)
    {
        case FIT_BITMAP:
        {                       // standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
            if (FreeImage_GetBPP (src) == 8) {
                
                typedef agg::pixfmt_gray8 pixfmt;
                typedef agg::renderer_base<pixfmt> ren_base;
                typedef agg::glyph_raster_bin<agg::rgba8> glyph_gen;

                int width = FreeImage_GetWidth (src);
                int height = FreeImage_GetHeight (src);
                unsigned char *buf = FreeImage_GetBits (src);

                // Create the rendering buffer
                agg::rendering_buffer rbuf (buf, width, height, FreeImage_GetPitch (src));

                glyph_gen glyph(0);
                pixfmt pixf(rbuf);
                ren_base rbase(pixf);

                agg::renderer_raster_htext_solid<ren_base, glyph_gen> renderer(rbase, glyph);
              
                renderer.color(agg::rgba8(value, value,value));

                glyph.font(agg::verdana18_bold);

                renderer.render_text(left, top, text, false);
            }

            break;
        }
        case FIT_UINT16:
        {      
           break;
        }
        case FIT_INT16:
        {   
            break;
        }
        case FIT_UINT32:
        {             
            break;
        }
        case FIT_INT32:
        {
            break;
        }
        case FIT_FLOAT:
        {
            break;
        }
        case FIT_DOUBLE:
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
