Copyright 2007 Glenn Pierce

This file is part of FreeImageAlgorithms.

FreeImageAlgorithms is free software: you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FreeImageAlgorithms is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
Lesser GNU General Public License for more details.

You should have received a copy of the Lesser GNU General Public License
along with FreeImageAlgorithms.  If not, see <http://www.gnu.org/licenses/>.


#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>
#include "assert.h"

/*  Threshold a greyscale image to a 8-bit grayscale dib.
 */
template<class Tsrc>
class THRESHOLD
{
public:
	int Threshold(FIBITMAP *src, Tsrc min, Tsrc max, Tsrc new_value);
	
};


template<class Tsrc> int 
THRESHOLD<Tsrc>::Threshold(FIBITMAP *src, Tsrc min, Tsrc max, Tsrc new_value)
{
	if(src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(!FreeImageAlgorithms_IsGreyScale(src))
		return FREEIMAGE_ALGORITHMS_ERROR;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	Tsrc *src_ptr = NULL;

	for(register int y = 0; y < height; y++) { 
		
		src_ptr = (Tsrc *)FreeImage_GetScanLine(src, y);

		for(register int x=0; x < width; x++) {

			if(src_ptr[x] >= min && src_ptr[x] <= max)
				src_ptr[x] = new_value;
		}
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

// Convert from type X to type BYTE
THRESHOLD<unsigned char>	thresholdUCharImage;
THRESHOLD<unsigned short>	thresholdUShortImage;
THRESHOLD<short>			thresholdShortImage;
THRESHOLD<unsigned long>	thresholdULongImage;
THRESHOLD<long>				thresholdLongImage;
THRESHOLD<float>			thresholdFloatImage;
THRESHOLD<double>			thresholdDoubleImage;


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Threshold(FIBITMAP *src, double min, double max, double new_value)
{
	FIBITMAP *dst = NULL;

	if(!src)
		return NULL;

	// convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	dst = FreeImage_Clone(src);

	int err;

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				err = thresholdUCharImage.Threshold(dst, (unsigned char) min, (unsigned char) max, (unsigned char) new_value);
			break;
		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			err = thresholdUShortImage.Threshold(dst, (unsigned short) min, (unsigned short) max, (unsigned short) new_value);
			break;
		case FIT_INT16:		// array of short: signed 16-bit
			err = thresholdShortImage.Threshold(dst, (short) min, (short) max, (short) new_value);
			break;
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			err = thresholdULongImage.Threshold(dst, (unsigned long) min, (unsigned long) max, (unsigned long) new_value);
			break;
		case FIT_INT32:		// array of long: signed 32-bit
			err = thresholdLongImage.Threshold(dst, (long) min, (long) max, (long) new_value);
			break;
		case FIT_FLOAT:		// array of float: 32-bit
			err = thresholdFloatImage.Threshold(dst, (float) min, (float) max, (float) new_value);
			break;
		case FIT_DOUBLE:	// array of double: 64-bit
			err = thresholdDoubleImage.Threshold(dst, min, max, new_value);
			break;
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	if(err = FREEIMAGE_ALGORITHMS_ERROR) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN,
			"FREE_IMAGE_TYPE: Unable to threshold type %d.\n", src_type);
		return NULL;
	}

	return dst;
}


int DLL_CALLCONV
FreeImageAlgorithms_InPlaceThreshold(FIBITMAP *src, double min, double max, double new_value)
{
	FIBITMAP *dst = NULL;

	if(!src)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	int err;

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				err = thresholdUCharImage.Threshold(src, (unsigned char) min, (unsigned char) max, (unsigned char) new_value);
			break;
		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			err = thresholdUShortImage.Threshold(src, (unsigned short) min, (unsigned short) max, (unsigned short) new_value);
			break;
		case FIT_INT16:		// array of short: signed 16-bit
			err = thresholdShortImage.Threshold(src, (short) min, (short) max, (short) new_value);
			break;
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			err = thresholdULongImage.Threshold(src, (unsigned long) min, (unsigned long) max, (unsigned long) new_value);
			break;
		case FIT_INT32:		// array of long: signed 32-bit
			err = thresholdLongImage.Threshold(src, (long) min, (long) max, (long) new_value);
			break;
		case FIT_FLOAT:		// array of float: 32-bit
			err = thresholdFloatImage.Threshold(src, (float) min, (float) max, (float) new_value);
			break;
		case FIT_DOUBLE:	// array of double: 64-bit
			err = thresholdDoubleImage.Threshold(src, min, max, new_value);
			break;
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	if(err = FREEIMAGE_ALGORITHMS_ERROR) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN,
			"FREE_IMAGE_TYPE: Unable to threshold type %d.\n", src_type);
		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}
