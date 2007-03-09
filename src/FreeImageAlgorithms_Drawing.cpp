#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Utilities.h"

#include <math.h>
#include "agg.h"

void draw_line(agg::rasterizer& ras,
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

static int 
Draw24BitColourRect (FIBITMAP *src, RECT rect, COLORREF colour, int line_width) 
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

	// Adding line_width seems to account for anti aliasing or sub pixel positioning
	draw_line(ras, rect.left+line_width, rect.top, rect.left+line_width, rect.bottom, line_width);
	draw_line(ras, rect.left, rect.top, rect.right, rect.top, line_width);
	draw_line(ras, rect.left, rect.bottom, rect.right, rect.bottom, line_width);
	draw_line(ras, rect.right-line_width, rect.bottom, rect.right-line_width, rect.top, line_width);

    ras.render(ren, agg::rgba8(GetRValue(colour), GetGValue(colour), GetBValue(colour)));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 


static int 
Draw32BitColourRect (FIBITMAP *src, RECT rect, COLORREF colour, int line_width) 
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

	// Adding line_width seems to account for anti aliasing or sub pixel positioning
	draw_line(ras, rect.left+line_width, rect.top, rect.left+line_width, rect.bottom, line_width);
	draw_line(ras, rect.left, rect.top, rect.right, rect.top, line_width);
	draw_line(ras, rect.left, rect.bottom, rect.right, rect.bottom, line_width);
	draw_line(ras, rect.right-line_width, rect.bottom, rect.right-line_width, rect.top, line_width);

    ras.render(ren, agg::rgba8(GetRValue(colour), GetGValue(colour), GetBValue(colour)));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 


static int 
Draw24BitSolidColourRect (FIBITMAP *src, RECT rect, COLORREF colour) 
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

    ras.render(ren, agg::rgba8(GetRValue(colour), GetGValue(colour), GetBValue(colour)));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

static int 
Draw32BitSolidColourRect (FIBITMAP *src, RECT rect, COLORREF colour) 
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

    ras.render(ren, agg::rgba8(GetRValue(colour), GetGValue(colour), GetBValue(colour)));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

int DLL_CALLCONV
FreeImageAlgorithms_Draw8BitSolidGreyscaleRect (FIBITMAP *src, RECT rect, unsigned char value) 
{  
	// Seems that Anti grain method is to slow probably  because it is too advanced
	// Does accurate drawing etc with anti aliasing.
	// We just want a simple rectangle.

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	// Allocate the framebuffer
	unsigned char* buf = NULL; 
	RECT tmp_rect = rect;

	// FreeImages are flipped
	tmp_rect.top = height - rect.top;
	tmp_rect.bottom = height - rect.bottom;

	for(register int y=tmp_rect.bottom; y < tmp_rect.top; y++) {

		buf = FreeImage_GetScanLine(src, y);

		memset(buf + tmp_rect.left, value, tmp_rect.right - tmp_rect.left);
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 


static int 
Draw24BitColourLine (FIBITMAP *src, POINT p1, POINT p2, COLORREF colour, int line_width) 
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

    ras.render(ren, agg::rgba8(GetRValue(colour), GetGValue(colour), GetBValue(colour)));

	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 

static int 
Draw32BitColourLine (FIBITMAP *src, POINT p1, POINT p2, COLORREF colour, int line_width) 
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

    ras.render(ren, agg::rgba8(GetRValue(colour), GetGValue(colour), GetBValue(colour)));
 
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 


int DLL_CALLCONV
FreeImageAlgorithms_DrawColourLine (FIBITMAP *src, POINT p1, POINT p2, COLORREF colour, int line_width) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	POINT p1_tmp = p1, p2_tmp = p2;

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

int DLL_CALLCONV
FreeImageAlgorithms_DrawColourRect (FIBITMAP *src, RECT rect, COLORREF colour, int line_width) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	RECT tmp_rect = rect;

	// FreeImages are flipped
	tmp_rect.top = height - rect.top;
	tmp_rect.bottom = height - rect.bottom;

	int bpp = FreeImage_GetBPP(src);
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);

	if(type == FIT_BITMAP && bpp == 32)
		return Draw32BitColourRect (src, tmp_rect, colour, line_width); 

	if(type == FIT_BITMAP && bpp == 24)
		return Draw24BitColourRect (src, tmp_rect, colour, line_width); 

	return FREEIMAGE_ALGORITHMS_ERROR;
} 

int DLL_CALLCONV
FreeImageAlgorithms_DrawColourSolidRect (FIBITMAP *src, RECT rect, COLORREF colour) 
{  
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	RECT tmp_rect = rect;

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