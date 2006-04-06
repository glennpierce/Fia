#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Ics.h"

#include <iostream>

static void CopyGreyScaleBytesToFIBitmap(FIBITMAP *src, char *data, int padded)
{
	register int y;

	int data_line_length, height = FreeImage_GetHeight(src);
	
	if(padded)
		data_line_length = FreeImage_GetPitch(src);
	else
		data_line_length = FreeImage_GetLine(src);
	
	char *bits;
	char *data_row;
			
	for( y = 0; y < height; y++) {

		bits = (char *) FreeImage_GetScanLine(src, y);

		data_row = data + (height - y - 1) * data_line_length;

		memcpy( bits, data_row, data_line_length ); 
	}
}

static void CopyColourBytesToFIBitmap(FIBITMAP *src, char *data, int padded)
{
	register int x, y;

	int data_line_length;
	int height = FreeImage_GetHeight(src);
	int width = FreeImage_GetWidth(src);

	if(padded)
		data_line_length = FreeImage_GetPitch(src);
	else
		data_line_length = FreeImage_GetLine(src);
	
	// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit) 
	int bytespp = FreeImage_GetLine(src) / width; 

	char *bits;
	char *data_row;
			
	for( y = 0; y < height; y++) {

		bits = (char *) FreeImage_GetScanLine(src, y);

		data_row = (char *) data + (height - y - 1) * data_line_length;

		for( x = 0; x < width; x++) {
				
			bits[FI_RGBA_RED] = data_row[0];
			bits[FI_RGBA_GREEN] = data_row[1];
			bits[FI_RGBA_BLUE] = data_row[2];

			if(bytespp == 4)
				bits[FI_RGBA_ALPHA] = data_row[3];

			// jump to next pixel
			bits += bytespp;
			data_row += bytespp;
		}
	}
}

static void CopyBytesToFBITMAP(FIBITMAP *src, char *data, int padded)
{
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);
	int bpp = FreeImage_GetBPP(src);

	switch(bpp)
	{
		case 8:
		case 16:
			CopyGreyScaleBytesToFIBitmap(src, data, padded);
			break;

		case 24:
			CopyColourBytesToFIBitmap(src, data, padded);	
			break;

		case 32:
		{
			if(type == FIT_BITMAP)
				CopyColourBytesToFIBitmap(src, data, padded);

			if(type == FIT_FLOAT)
				CopyGreyScaleBytesToFIBitmap(src, data, padded);
			
			break;
		}
	}
}


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

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadFIBFromFile(const char *pathname, int padded)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN; 
	
	if(FreeImageAlgorithms_IsIcsFile (pathname) ) {
	
		return FreeImageAlgorithms_LoadFIBFromIcsFile (pathname, padded);
	}

	// check the file signature and deduce its format 
	// (the second argument is currently not used by FreeImage)	
	fif = FreeImage_GetFileType(pathname, 0);
		
	if(fif == FIF_UNKNOWN) { 
		// no signature ? // try to guess the file format from the file extension
			
		fif = FreeImage_GetFIFFromFilename(pathname);
	} // check that the plugin has reading capabilities ... 
		
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) { 
		
		// load the file
		return FreeImage_Load(fif, pathname, 0); 
	}

	return NULL;
}	


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadGreyScaleFIBFromArrayData (char *data, int bpp, int width, int height, FREE_IMAGE_TYPE data_type, int padded)
{
	FIBITMAP 	*dib;
	char 		*data_ptr;
	
	// Check the data type and convert to bpp
	if ( bpp < 0 )
		return NULL;

	data_ptr = data;
	
	switch (bpp)
	{
		case 8:
			
			// Allocate a new dib
			if ( (dib = FreeImage_Allocate (width, height, 8, 0, 0, 0)) == NULL )
				return NULL;
	
			FreeImageAlgorithms_SetGreyLevelPalette(dib);

			break;
			
		case 16:
			
			if(data_type == FIT_UINT16) {
			
				if ( (dib = FreeImage_AllocateT(FIT_UINT16, width, height, 16, 0, 0, 0)) == NULL )
					return NULL;
			}
			
			if(data_type == FIT_INT16) {
			
				if ( (dib = FreeImage_AllocateT(FIT_INT16, width, height, 16, 0, 0, 0)) == NULL )
					return NULL;
			}
		
			break;
			
	
		case 32:
		
			if(data_type == FIT_FLOAT) {
			
				if ( (dib = FreeImage_AllocateT(FIT_FLOAT, width, height, 32, 0, 0, 0)) == NULL )
					return NULL;
			}
			else {
			
				return NULL;
			}
			
			break;
	}

	CopyBytesToFBITMAP(dib, data, padded);

	return dib;
}

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadColourFIBFromArrayData (char *data, int bpp, int width, int height, int padded)
{
	FIBITMAP 	*dib;
	char 		*data_ptr;
	
	// Check the data type and convert to bpp
	if ( bpp < 0 )
		return NULL;

	data_ptr = data;
	
	switch (bpp)
	{
		case 8:
		case 16:
			
			return NULL;
			
		case 24:
		
			// Allocate a new dib
			if ( (dib = FreeImage_AllocateT (FIT_BITMAP, width, height, 24, 0, 0, 0)) == NULL )
				return NULL;
			
			break;
		
		case 32:
		
			if ( (dib = FreeImage_Allocate (width, height, 32, 0, 0, 0)) == NULL )
					return NULL;
			
			break;
	}

	CopyBytesToFBITMAP(dib, data, padded);

	return dib;
}


static int get_file_extension(const char *filepath, char *file_ext)
{
    char *cptr = strrchr(filepath, '.');
    
    if (!cptr) // no extension
		return -1;
		
	strcpy(file_ext, cptr);

	return 1;
}


int DLL_CALLCONV
FreeImageAlgorithms_SaveFIBToFile (FIBITMAP *dib, const char *filepath)
{
	FIBITMAP *standard_dib;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	char file_ext[10];
	int is_greyscale;

	/* Get file extension */
	get_file_extension(filepath, file_ext); 

	if( strcmp(file_ext, ".ics") == 0 ) {
	
		return FreeImageAlgorithms_SaveFIBToIcsFile (dib, filepath);
	}
		
	// try to guess the file format from the file extension
	fif = FreeImage_GetFIFFromFilename(filepath);
	
	// check that the plugin has writing capabilities ... 
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsWriting(fif)) { 
		
		is_greyscale = FreeImageAlgorithms_IsGreyScale(dib);
		
		// Some formats can only save 8, & 24 bit images like jpeg
		// So convert colour 
		if(is_greyscale && FreeImage_GetBPP(dib) != 8)
			standard_dib = FreeImage_ConvertToStandardType(dib, 1);
		else if(!is_greyscale && FreeImage_GetBPP(dib) != 24)
			standard_dib = FreeImage_ConvertTo24Bits(dib);
		else
			standard_dib = FreeImage_Clone(dib);

		if(!FreeImage_Save(fif, standard_dib, filepath, 0))
			return FREEIMAGE_ALGORITHMS_ERROR;
	
		FreeImage_Unload(standard_dib); 
	}
	else {
	
		return FREEIMAGE_ALGORITHMS_ERROR;
	}
	
	return FREEIMAGE_ALGORITHMS_SUCCESS;
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


RGBQUAD* DLL_CALLCONV 
FreeImageAlgorithms_GetHBitmapPalette(HBITMAP hbitmap)
{
	BITMAPINFO *bmi;

	GetObject(hbitmap, sizeof(BITMAPINFO), &bmi);

	return bmi->bmiColors;
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