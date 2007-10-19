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

#include "FreeImageAlgorithms_Error.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>
#include <assert.h>

static void CopyGreyScaleBytesToFIBitmap(FIBITMAP *src, BYTE *data, int padded, int vertical_flip)
{
	int data_line_length, height = FreeImage_GetHeight(src);
	
	if(padded)
		data_line_length = FreeImage_GetPitch(src);
	else
		data_line_length = FreeImage_GetLine(src);
	
	BYTE *bits;
	BYTE *data_row;
			
	int line;
		
	for(register int y = 0; y < height; y++) {

		if(vertical_flip)
			line = height - y - 1;
		else
			line = y;

		bits = (BYTE *) FreeImage_GetScanLine(src, line);

		data_row = data + (height - y - 1) * data_line_length;

		memcpy( bits, data_row, data_line_length ); 
	}
}

static void CopyColourBytesToFIBitmap(FIBITMAP *src, BYTE *data, int padded, int vertical_flip,
									  int red_byte, int green_byte, int blue_byte)
{
	int data_line_length;
	int height = FreeImage_GetHeight(src);
	int width = FreeImage_GetWidth(src);

	if(padded)
		data_line_length = FreeImage_GetPitch(src);
	else
		data_line_length = FreeImage_GetLine(src);
	
	// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit) 
	int bytespp = FreeImage_GetLine(src) / width; 

	BYTE *bits;
	BYTE *data_row;
			
	int line;

	for(register int y = 0; y < height; y++) {

		if(vertical_flip)
			line = height - y - 1;
		else
			line = y;

		bits = (BYTE *) FreeImage_GetScanLine(src, line);

		data_row = (BYTE *) data + (height - y - 1) * data_line_length;

		for(register int x = 0; x < width; x++) {
				
			bits[FI_RGBA_RED] = data_row[red_byte];
			bits[FI_RGBA_GREEN] = data_row[green_byte];
			bits[FI_RGBA_BLUE] = data_row[blue_byte];

			if(bytespp == 4)
				bits[FI_RGBA_ALPHA] = data_row[3];

			// jump to next pixel
			bits += bytespp;
			data_row += bytespp;
		}
	}
}

void DLL_CALLCONV
FreeImageAlgorithms_CopyBytesToFBitmap(FIBITMAP *src, BYTE *data, int padded, int vertical_flip, COLOUR_ORDER order)
{
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);
	int bpp = FreeImage_GetBPP(src);
	int red_byte, green_byte, blue_byte;

	if(order == COLOUR_ORDER_RGB) 
	{
		red_byte = FI_RGBA_RED;
		green_byte = FI_RGBA_GREEN;
		blue_byte = FI_RGBA_BLUE;
	}
	else {
	
		red_byte = FI_RGBA_BLUE;
		green_byte = FI_RGBA_GREEN;
		blue_byte = FI_RGBA_RED;
	}

	switch(bpp)
	{
		case 8:
		case 16:
			CopyGreyScaleBytesToFIBitmap(src, data, padded, vertical_flip);
			break;

		case 24:
			CopyColourBytesToFIBitmap(src, data, padded, vertical_flip, red_byte, green_byte, blue_byte);	
			break;

		case 32:
		{
			if(type == FIT_BITMAP)
				CopyColourBytesToFIBitmap(src, data, padded, vertical_flip, red_byte, green_byte, blue_byte);

			if(type == FIT_FLOAT)
				CopyGreyScaleBytesToFIBitmap(src, data, padded, vertical_flip);
			
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
FreeImageAlgorithms_LoadGreyScaleFIBFromArrayData (BYTE *data, int bpp, int width, int height,
                                                   FREE_IMAGE_TYPE data_type, int padded, int vertical_flip)
{
	FIBITMAP 	*dib;
	BYTE 		*data_ptr;
	
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

	FreeImageAlgorithms_CopyBytesToFBitmap(dib, data, padded, vertical_flip, COLOUR_ORDER_RGB);

	return dib;
}

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadColourFIBFromArrayData (BYTE *data, int bpp, int width, int height, int padded,
												int vertical_flip, COLOUR_ORDER colour_order)
{
	FIBITMAP 	*dib;
	BYTE 		*data_ptr;
	
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

	FreeImageAlgorithms_CopyBytesToFBitmap(dib, data, padded, vertical_flip, colour_order);

	return dib;
}


int DLL_CALLCONV
FreeImageAlgorithms_SaveFIBToFile (FIBITMAP *dib, const char *filepath, FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH bit_depth)
{
	FIBITMAP *standard_dib, *converted_dib;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	
	if(dib == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// try to guess the file format from the file extension
	fif = FreeImage_GetFIFFromFilename(filepath);
	
	// check that the plugin has writing capabilities ... 
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsWriting(fif)) { 
	
		standard_dib = FreeImage_ConvertToStandardType(dib, 0);

		if(bit_depth == BIT24)
			converted_dib = FreeImage_ConvertTo24Bits(standard_dib);
		else
			converted_dib = FreeImage_ConvertTo8Bits(standard_dib);

		if(converted_dib == NULL)
			return FREEIMAGE_ALGORITHMS_ERROR;

		if(!FreeImage_Save(fif, converted_dib, filepath, 0)) {
			FreeImageAlgorithms_SendOutputMessage("Unknown Error Saving File! FreeImage_Save Failed");
			return FREEIMAGE_ALGORITHMS_ERROR;
		}

		FreeImage_Unload(standard_dib); 
		FreeImage_Unload(converted_dib); 
	}
	else {
	
		FreeImageAlgorithms_SendOutputMessage("Error Saving File! Unsupported Type");
		return FREEIMAGE_ALGORITHMS_ERROR;
	}
	
	return FREEIMAGE_ALGORITHMS_SUCCESS;
}
