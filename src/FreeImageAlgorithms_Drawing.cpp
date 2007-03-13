#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Utilities.h"

#include <math.h>
#include "agg.h"

static void draw_line(agg::rasterizer& ras,
               double x1, double y1, 
               double x2, double y2,
               double width)
{

    double dx = x2 - x1;
    double dy = y2 - y1;
    double d = sqrt(dx*dx + dy*dy);
    
    dx = width * (y2 - y1) / d;
    dy = width * (x2 - x1) / d;

    ras.move_to_d(x1 - dx,  y1 + dy);
    ras.line_to_d(x2 - dx,  y2 + dy);
    ras.line_to_d(x2 + dx,  y2 - dy);
    ras.line_to_d(x1 + dx,  y1 - dy);
}

// Draws a orthogonal no aa line of width one pixel
// This is for drawing rectangles fast
static int orthogonal_draw_colour_line(FIBITMAP *src, int x1, int y1, int x2, int y2, RGBQUAD colour)
{
	if(!src)
		return FREEIMAGE_ALGORITHMS_ERROR;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
 
	// Draw from the left
	if(x2 < x1)
		SWAP(x1, x2);

	// Draw from the top
	if(y2 < y1)
		SWAP(y1, y2);

	if(x2 < 0 || y2 < 0)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(x1 < 0)
		x1 = 0;
		
	if(x2 >= width)
		x2 = width - 1;

	if(y1 < 0)
		y1 = 0;
		
	if(y2 >= height)
		y2 = height - 1;

	int bytespp = FreeImage_GetLine(src) / FreeImage_GetWidth(src); 
	int pitch = FreeImage_GetPitch(src);

	if(x1 != x2) {

		// We have a horizontal line
		// Make sure y's are the same
		if(y1 != y2)
			return FREEIMAGE_ALGORITHMS_ERROR;

		BYTE *bits = (BYTE *) FreeImage_GetScanLine(src, y1) + (x1 * bytespp);

		for(register int x = x1; x <= x2; x++) {
				
			bits[FI_RGBA_RED] = colour.rgbRed;
			bits[FI_RGBA_GREEN] =  colour.rgbGreen;
			bits[FI_RGBA_BLUE] = colour.rgbBlue;

			if(bytespp == 4)
				bits[FI_RGBA_ALPHA] = 0;

			// jump to next pixel
			bits += bytespp;
			
		}

		return FREEIMAGE_ALGORITHMS_SUCCESS;
	}

	if(y1 != y2) {

		// We have a verticle line
		// Make sure x's are the same
		if(x1 != x2)
			return FREEIMAGE_ALGORITHMS_ERROR;	

		// Get starting point
		BYTE *bits = (BYTE*) FreeImage_GetScanLine(src, y1) + (x1 * bytespp);

		while(y1 <= y2) {

			bits[FI_RGBA_RED] = colour.rgbRed;
                        bits[FI_RGBA_GREEN] =  colour.rgbGreen;
                        bits[FI_RGBA_BLUE] = colour.rgbBlue;

			if(bytespp == 4)
				bits[FI_RGBA_ALPHA] = 0;

			bits += pitch;

			y1++;
		}

		return FREEIMAGE_ALGORITHMS_SUCCESS;
	}

	return FREEIMAGE_ALGORITHMS_ERROR;
}

static int 
DrawColourRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour, int line_width) 
{  
	int err;

	for(int i=0; i < line_width; i++) {
	
		// Top
		err = orthogonal_draw_colour_line(src, rect.left - line_width + 1, rect.top + i, rect.right + line_width - 1, rect.top + i, colour);
	
		// Bottom
		err = orthogonal_draw_colour_line(src, rect.left - line_width + 1, rect.bottom - i, rect.right + line_width - 1, rect.bottom - i, colour);

		// Left
		err = orthogonal_draw_colour_line(src, rect.left - i, rect.top, rect.left - i, rect.bottom, colour);

		// Right
		err = orthogonal_draw_colour_line(src, rect.right + i, rect.top, rect.right + i, rect.bottom, colour);
	}

	if(err == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

static int 
Draw24BitSolidColourRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	// Allocate the framebuffer
	unsigned char* buf = FreeImage_GetBits(src);

    	// Create the rendering buffer 
    	agg::rendering_buffer rbuf(buf, width, height, FreeImage_GetPitch(src));

    	// Create the rendering buffer 
    	agg::renderer<agg::span_bgr24> ren(rbuf);
    	agg::rasterizer ras;

    	ras.move_to_d(rect.left, rect.top);
	ras.line_to_d(rect.right, rect.top);
	ras.line_to_d(rect.right, rect.bottom);
	ras.line_to_d(rect.left, rect.bottom);
	ras.line_to_d(rect.left, rect.top);

    	ras.render(ren, agg::rgba8(colour.rgbRed, colour.rgbGreen, colour.rgbBlue));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

static int 
Draw32BitSolidColourRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	// Allocate the framebuffer
	unsigned char* buf = FreeImage_GetBits(src);

    	// Create the rendering buffer 
    	agg::rendering_buffer rbuf(buf, width, height, FreeImage_GetPitch(src));

    	// Create the renderer 
    	agg::renderer<agg::span_bgra32> ren(rbuf);
    	agg::rasterizer ras;

    	ras.move_to_d(rect.left, rect.top);
	ras.line_to_d(rect.right, rect.top);
	ras.line_to_d(rect.right, rect.bottom);
	ras.line_to_d(rect.left, rect.bottom);
	ras.line_to_d(rect.left, rect.top);

    	ras.render(ren, agg::rgba8(colour.rgbRed, colour.rgbGreen, colour.rgbBlue));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

static int DLL_CALLCONV
Draw8BitSolidGreyscaleRect (FIBITMAP *src, FIARECT rect, unsigned char value) 
{  
	// Seems that Anti grain method is to slow probably  because it is too advanced
	// Does accurate drawing etc with anti aliasing.
	// We just want a simple rectangle.

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	// Allocate the framebuffer
	unsigned char* buf = NULL; 
	FIARECT tmp_rect = rect;

	// FreeImages are flipped
	tmp_rect.top = height - rect.top - 1;
	tmp_rect.bottom = height - rect.bottom - 1;

	for(register int y=tmp_rect.bottom; y <= tmp_rect.top; y++) {

		buf = FreeImage_GetScanLine(src, y);

		memset(buf + tmp_rect.left, value, tmp_rect.right - tmp_rect.left + 1);
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

int DLL_CALLCONV
FreeImageAlgorithms_DrawSolidGreyscaleRect (FIBITMAP *src, FIARECT rect, double value) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	// Allocate the framebuffer
	unsigned char* buf = NULL; 
	FIARECT tmp_rect = rect;

	// FreeImages are flipped
	tmp_rect.top = height - rect.top - 1;
	tmp_rect.bottom = height - rect.bottom - 1;

	int bpp = FreeImage_GetBPP(src);
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);

	if(type == FIT_BITMAP && bpp == 8)
		return Draw8BitSolidGreyscaleRect  (src, tmp_rect, (unsigned char) value); 

	return FREEIMAGE_ALGORITHMS_ERROR;
} 

static int 
Draw24BitColourLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, RGBQUAD colour, int line_width) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	// Allocate the framebuffer
	unsigned char* buf = FreeImage_GetBits(src);

    	// Create the rendering buffer 
    	agg::rendering_buffer rbuf(buf, width, height, FreeImage_GetPitch(src));

    	// Create the rendering buffer 
    	agg::renderer<agg::span_bgr24> ren(rbuf);
    	agg::rasterizer ras;

    	// Setup the rasterizer
    	ras.filling_rule(agg::fill_even_odd);

	draw_line(ras, p1.x, p1.y, p2.x, p2.y, line_width);

    	ras.render(ren, agg::rgba8(colour.rgbRed, colour.rgbGreen, colour.rgbBlue));

	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

static int 
Draw32BitColourLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, RGBQUAD colour, int line_width) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	// Allocate the framebuffer
	unsigned char* buf = FreeImage_GetBits(src);

    	// Create the rendering buffer 
    	agg::rendering_buffer rbuf(buf, width, height, FreeImage_GetPitch(src));

    	// Create the rendering buffer 
    	agg::renderer<agg::span_bgra32> ren(rbuf);
    	agg::rasterizer ras;

    	// Setup the rasterizer
    	ras.filling_rule(agg::fill_even_odd);

	draw_line(ras, p1.x, p1.y, p2.x, p2.y, line_width);

    	ras.render(ren, agg::rgba8(colour.rgbRed, colour.rgbGreen, colour.rgbBlue));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 


int DLL_CALLCONV
FreeImageAlgorithms_DrawColourLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, RGBQUAD colour, int line_width) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	FIAPOINT p1_tmp = p1, p2_tmp = p2;

	// FreeImages are flipped
	p1_tmp.y = height - p1.y;
	p2_tmp.y = height - p2.y;

	int bpp = FreeImage_GetBPP(src);
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);

	if(type == FIT_BITMAP && bpp == 32)
		return Draw32BitColourLine (src, p1_tmp, p2_tmp, colour, line_width); 

	if(type == FIT_BITMAP && bpp == 24)
		return Draw24BitColourLine (src, p1_tmp, p2_tmp, colour, line_width); 

	return FREEIMAGE_ALGORITHMS_ERROR;
} 


static int 
Draw8BitGreyscaleLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, unsigned char value, int line_width) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	// Allocate the framebuffer
	unsigned char* buf = FreeImage_GetBits(src);

    	// Create the rendering buffer 
    	agg::rendering_buffer rbuf(buf, width, height, FreeImage_GetPitch(src));

    	// Create the rendering buffer 
    	agg::renderer<agg::span_mono8> ren(rbuf);
    	agg::rasterizer ras;

    	// Setup the rasterizer
    	ras.filling_rule(agg::fill_even_odd);

	draw_line(ras, p1.x, p1.y, p2.x, p2.y, line_width);

    	ras.render(ren, agg::rgba8(value, value, value));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

int DLL_CALLCONV
FreeImageAlgorithms_DrawGreyscaleLine (FIBITMAP *src, FIAPOINT p1, FIAPOINT p2, double value, int line_width) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	FIAPOINT p1_tmp = p1, p2_tmp = p2;

	// FreeImages are flipped
	p1_tmp.y = height - p1.y;
	p2_tmp.y = height - p2.y;

	int bpp = FreeImage_GetBPP(src);
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);

	if(type == FIT_BITMAP && bpp == 8)
		return Draw8BitGreyscaleLine (src, p1_tmp, p2_tmp, (unsigned char) value, line_width); 

	return FREEIMAGE_ALGORITHMS_ERROR;
} 

int DLL_CALLCONV
FreeImageAlgorithms_DrawColourRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour, int line_width) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	FIARECT tmp_rect = rect;

	// FreeImages are flipped
	tmp_rect.top = height - rect.top - 1;
	tmp_rect.bottom = height - rect.bottom - 1;

	int bpp = FreeImage_GetBPP(src);
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);

	return DrawColourRect (src, tmp_rect, colour, line_width); 
} 

int DLL_CALLCONV
FreeImageAlgorithms_DrawColourSolidRect (FIBITMAP *src, FIARECT rect, RGBQUAD colour) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	FIARECT tmp_rect = rect;

	// FreeImages are flipped
	tmp_rect.top = height - rect.top;
	tmp_rect.bottom = height - rect.bottom;

	int bpp = FreeImage_GetBPP(src);
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);

	if(type == FIT_BITMAP && bpp == 32)
		return Draw32BitSolidColourRect (src, tmp_rect, colour); 

	if(type == FIT_BITMAP && bpp == 24)
		return Draw24BitSolidColourRect (src, tmp_rect, colour); 

	return FREEIMAGE_ALGORITHMS_ERROR;
} 
