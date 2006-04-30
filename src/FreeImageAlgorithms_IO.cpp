#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

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

void DLL_CALLCONV
FreeImageAlgorithms_CopyBytesToFBitmap(FIBITMAP *src, char *data, int padded)
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


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadFIBFromFile(const char *pathname)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN; 

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

	FreeImageAlgorithms_CopyBytesToFBitmap(dib, data, padded);

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

	FreeImageAlgorithms_CopyBytesToFBitmap(dib, data, padded);

	return dib;
}


int DLL_CALLCONV
FreeImageAlgorithms_SaveFIBToFile (FIBITMAP *dib, const char *filepath)
{
	FIBITMAP *standard_dib;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	int is_greyscale;
		
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