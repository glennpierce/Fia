#include "FreeImageAlgorithms_HBitmap.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>

HBITMAP DLL_CALLCONV
FreeImageAlgorithms_FibToHBitmap(FIBITMAP *dib)
{
	DWORD error;

	HDC dc = GetDC(NULL);

	if(dc == NULL) {
		std::cout <<"NULL DC" << std::endl;
		return NULL;
	}

	HBITMAP bitmap = CreateDIBitmap(dc, FreeImage_GetInfoHeader(dib),
		CBM_INIT, FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS); 

	if(bitmap == NULL) {

		error = GetLastError();
		std::cout << error << std::endl;
 
		return NULL;
	}

	ReleaseDC(NULL, dc);		
	DeleteDC (dc);

	return bitmap;
}


void DLL_CALLCONV
FreeImageAlgorithms_FreeHBitmap(HBITMAP hbitmap)
{
	DeleteObject(hbitmap);
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_HBitmapToFIB(HDC hdc, HBITMAP bitmap)
{
	FIBITMAP *dib = NULL;
	BITMAP bm;
	
	if(bitmap) { 

  		GetObject(bitmap, sizeof(BITMAP), (LPSTR) &bm);
 
  		dib = FreeImage_Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel, 0, 0, 0);
  
  		GetDIBits(hdc, bitmap, 0, FreeImage_GetHeight(dib), 
  			FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS);
	}

	return dib;
}


HBITMAP DLL_CALLCONV 
FreeImageAlgorithms_GetDibSection(FIBITMAP *src, HDC hdc, int left, int top, int right, int bottom)
{
	if(!src) 
		return NULL;

	unsigned bpp = FreeImage_GetBPP(src);

	if(bpp <=4)
		return NULL;

	// normalize the rectangle
	if(right < left)
		INPLACESWAP(left, right);

	if(bottom < top)
		INPLACESWAP(top, bottom);

	// check the size of the sub image
	int src_width  = FreeImage_GetWidth(src);
	int src_height = FreeImage_GetHeight(src);
	int src_pitch = FreeImage_GetPitch(src);

	if((left < 0) || (right > src_width) || (top < 0) || (bottom > src_height))
		return NULL;

	// allocate the sub image
	int dst_width = (right - left);
	int dst_height = (bottom - top);

	BITMAPINFO *info = (BITMAPINFO *) malloc(sizeof(BITMAPINFO) + (FreeImage_GetColorsUsed(src) * sizeof(RGBQUAD)));
	BITMAPINFOHEADER *bih = (BITMAPINFOHEADER *)info;

	bih->biSize = sizeof(BITMAPINFOHEADER);
	bih->biWidth = dst_width;
	bih->biHeight = dst_height;
	bih->biPlanes = 1;
	bih->biBitCount = bpp;
	bih->biCompression = BI_RGB;
	bih->biSizeImage = 0;
    bih->biXPelsPerMeter = 0;
    bih->biYPelsPerMeter = 0;
   	bih->biClrUsed = 0;           // Always use the whole palette.
    bih->biClrImportant = 0;

	// copy the palette
	if(bpp < 16)
		memcpy(info->bmiColors, FreeImage_GetPalette(src), FreeImage_GetColorsUsed(src) * sizeof(RGBQUAD));

	BYTE *dst_bits;

	HBITMAP hbitmap = CreateDIBSection(hdc, info, DIB_RGB_COLORS, (void **) &dst_bits, NULL, 0);

	free(info);

	if(hbitmap == NULL || dst_bits == NULL)
		return NULL;

	// get the pointers to the bits and such
	BYTE *src_bits = FreeImage_GetScanLine(src, src_height - top - dst_height);

	// calculate the number of bytes per pixel
	unsigned bytespp = FreeImage_GetLine(src) / FreeImage_GetWidth(src);
	
	// point to x = left
	src_bits += (left * bytespp);	

	int dst_line = (dst_width * bpp + 7) / 8;
	int dst_pitch = (dst_line + 3) & ~3;

	for(int y = 0; y < dst_height; y++)
		memcpy(dst_bits + (y * dst_pitch), src_bits + (y * src_pitch), dst_line);

	return hbitmap;
}


int DLL_CALLCONV
FreeImageAlgorithms_CopyToDibSection(FIBITMAP *src, HBITMAP hbitmap, int left, int top, int right, int bottom)
{
	BYTE *src_bits;
	int src_width, src_height, src_pitch;
	unsigned bytespp;
	BYTE *dst_bits;
	int y, dst_width, dst_height;
	int dst_line;
	int bpp;
	BITMAP bm;

	if(!src) 
		return FREEIMAGE_ALGORITHMS_ERROR;

	GetObject(hbitmap, sizeof(BITMAP), &bm);

	dst_bits = (BYTE*) bm.bmBits;

	if(dst_bits == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// normalize the rectangle
	if(right < left)
		INPLACESWAP(left, right);

	if(bottom < top)
		INPLACESWAP(top, bottom);

	// check the size of the sub image
	src_width  = FreeImage_GetWidth(src);
	src_height = FreeImage_GetHeight(src);
	src_pitch = FreeImage_GetPitch(src);

	if((left < 0) || (right > src_width) || (top < 0) || (bottom > src_height))
		return FREEIMAGE_ALGORITHMS_ERROR;

	// allocate the sub image
	dst_width = right - left;
	dst_height = bottom - top;

	// get the pointers to the bits and such
	src_bits = FreeImage_GetScanLine(src, src_height - top - dst_height);

	// calculate the number of bytes per pixel
	bytespp = FreeImage_GetLine(src) / src_width;
	
	// point to x = left
	src_bits += (left * bytespp);	

	bpp = FreeImage_GetBPP(src);

	dst_line = (dst_width * bpp + 7) / 8;

	dst_bits += ((bm.bmHeight - dst_height) * bm.bmWidthBytes);

	for(y = 0; y < dst_height; y++)
		memcpy(dst_bits + (y * bm.bmWidthBytes), src_bits + (y * src_pitch), dst_line);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


HBITMAP DLL_CALLCONV
FreeImageAlgorithms_CreateDibSection(HDC hdc, int width, int height, int bpp, int colours_used, RGBQUAD* bmiColours)
{
	HBITMAP hbitmap;
	BITMAPINFO *info;
	BITMAPINFOHEADER *bih;

	void *bits;

	if(bpp > 8)
		colours_used = 0;	

	info = (BITMAPINFO *) malloc(sizeof(BITMAPINFO) + (colours_used * sizeof(RGBQUAD)));
	bih = (BITMAPINFOHEADER *)info;

	bih->biSize = sizeof(BITMAPINFOHEADER);
	bih->biWidth = width;
	bih->biHeight = height;
	bih->biPlanes = 1;
	bih->biBitCount = bpp;
	bih->biCompression = BI_RGB;
	bih->biSizeImage = 0;
    bih->biXPelsPerMeter = 0;
    bih->biYPelsPerMeter = 0;
   	bih->biClrUsed = 0;           // Always use the whole palette.
    bih->biClrImportant = 0;

	// copy the palette
	if(bpp < 16)
		memcpy(info->bmiColors, bmiColours, colours_used * sizeof(RGBQUAD));

	hbitmap = CreateDIBSection(hdc, info, DIB_RGB_COLORS, &bits, NULL, 0);

	free(info);

	if(hbitmap == NULL || bits == NULL)
		return NULL;

	return hbitmap;
}