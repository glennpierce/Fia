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

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Utilities.h"

template<class Tsrc>
class BORDER
{
public:
	
    inline void SetImageRowToConstant(FIBITMAP *src, int row_start, int count, Tsrc val);
    inline void SetImageColToConstant(FIBITMAP *src, int col_start, int count, Tsrc val);	

    FIABITMAP* SetBorder(FIBITMAP *src, int xborder, int yborder, BorderType type, Tsrc constant);
};


static inline void CopyImageRow(FIBITMAP *src, int src_row,
                         int row_start, int count)
{
    int pitch = FreeImage_GetPitch(src);

    BYTE* src_bits, *dst_bits;

    src_bits = FreeImage_GetScanLine(src, src_row);

    for(int y = row_start; y < (row_start + count); y++) {

        dst_bits = FreeImage_GetScanLine(src, y);

        memcpy(dst_bits, src_bits, pitch);
    }
}

static inline void CopyImageCol(FIBITMAP *src, int src_col,
                         int col_start, int count)
{
    int height = FreeImage_GetHeight(src);
    int pitch = FreeImage_GetPitch(src);

    BYTE* bits = FreeImage_GetBits(src); 

    for(int y = 0; y < height; y++) {

        for(int x = col_start; x < (col_start + count); x++)
            bits[x] = bits[src_col];

        bits += pitch;
    }
}

template <typename Tsrc>
inline void BORDER<Tsrc>::SetImageRowToConstant(FIBITMAP *src, int row_start, int count, Tsrc val)
{
    int pitch = FreeImage_GetPitch(src);
    int width = FreeImage_GetWidth(src);

    Tsrc *dst_bits = NULL;

    for(register int y = row_start; y < (row_start + count); y++) {

        dst_bits = (Tsrc*) FreeImage_GetScanLine(src, y);

        for(register int x = 0; x < width; x++)
           dst_bits[x] = (Tsrc) val;
    }
}

template <typename Tsrc>
inline void BORDER<Tsrc>::SetImageColToConstant(FIBITMAP *src, int col_start, int count, Tsrc val)
{
    int height = FreeImage_GetHeight(src);
    int pitch = FreeImage_GetPitch(src);

    Tsrc* bits = (Tsrc*) FreeImage_GetBits(src);

    for(int y = 0; y < height; y++) {

        for(int x = col_start; x < (col_start + count); x++)
            bits[x] = (Tsrc) val;

        bits += pitch;
    }
}

template <typename Tsrc> FIABITMAP*
BORDER<Tsrc>::SetBorder(FIBITMAP *src, int xborder, int yborder, BorderType type, Tsrc constant)
{
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
    int dst_width = width + (2 *xborder);
    int dst_height = height + (2 * yborder);

	FIABITMAP* dst = (FIABITMAP*) malloc(sizeof(FIABITMAP));
	
    CheckMemory(dst);

	dst->fib = FreeImageAlgorithms_CloneImageType(src, dst_width, dst_height);

	dst->xborder = xborder;
	dst->yborder = yborder;

	FreeImageAlgorithms_SimplePaste(dst->fib, src, xborder, yborder);

    if(type == BorderType_Constant && constant != 0.0) {
   
        // Set the bottom line of the image to constant value
        this->SetImageRowToConstant(dst->fib, 0, yborder, constant);

        // Set the top line of the image to constant value
        this->SetImageRowToConstant(dst->fib, dst_height - yborder, yborder, constant);

        // Set the left pixels of the image
        this->SetImageColToConstant(dst->fib, 0, xborder, constant);

        // Set the right pixels of the image
        this->SetImageColToConstant(dst->fib, dst_width - xborder, xborder, constant);
    }
    else if (type == BorderType_Copy) {

        // Get the bottom line of the original image and copy into bottom border
        CopyImageRow(dst->fib, yborder, 0, yborder);

        // Get the top line of the original image and copy into top border
        CopyImageRow(dst->fib, dst_height - yborder - 1, dst_height - yborder, yborder);

        // Get the left pixels of the image and copy into left border
        CopyImageCol(dst->fib, xborder, 0, xborder);

        // Get the right pixels of the image and copy into right border
        CopyImageCol(dst->fib, dst_width - xborder - 1, dst_width - xborder, xborder);
    }
    else if (type == BorderType_Mirror) {

        int pitch = FreeImage_GetPitch(dst->fib);
        Tsrc* src_bits, *dst_bits;

        // Get the bottom line of the original image and copy into bottom border

        int border_row_start = yborder - 1; // First row on border for bottom
        int image_row_start = yborder;

        for(int i = 0; i < yborder; i++) {

            dst_bits = (Tsrc*) FreeImage_GetScanLine(dst->fib, border_row_start);
            src_bits = (Tsrc*) FreeImage_GetScanLine(dst->fib, image_row_start);

            memcpy(dst_bits, src_bits, pitch);

            border_row_start--;
            image_row_start++;
        }

        // Top 

        border_row_start = dst_height - yborder; // First row on border fort bottom
        image_row_start = border_row_start - 1;

        for(int i = 0; i < yborder; i++) {

            dst_bits = (Tsrc*) FreeImage_GetScanLine(dst->fib, border_row_start);
            src_bits = (Tsrc*) FreeImage_GetScanLine(dst->fib, image_row_start);

            memcpy(dst_bits, src_bits, pitch);

            border_row_start++;
            image_row_start--;
        }

    
        // Left
        dst_bits = (Tsrc*) FreeImage_GetBits(dst->fib);

        for(int y = 0; y < dst_height; y++) {

            dst_bits = (Tsrc*) FreeImage_GetScanLine(dst->fib, y);

            for(int x = 0; x < xborder; x++) {
                dst_bits[x] = dst_bits[2 * xborder - 1 - x];
            }
        }
      
        // Right
        dst_bits = (Tsrc*) FreeImage_GetBits(dst->fib);
        border_row_start = dst_width - xborder;

        for(int y = 0; y < dst_height; y++) {

            dst_bits = (Tsrc*) FreeImage_GetScanLine(dst->fib, y);

            for(int x = 0; x < xborder; x++) {
                dst_bits[border_row_start + x] = dst_bits[border_row_start - 1 - x];
            }
        }

    }

	return dst;
}


static BORDER<unsigned char>		borderUCharImage;
static BORDER<unsigned short>		borderUShortImage;
static BORDER<short>				borderShortImage;
static BORDER<unsigned long>		borderULongImage;
static BORDER<long>				    borderLongImage;
static BORDER<float>				borderFloatImage;
static BORDER<double>				borderDoubleImage;


FIABITMAP* DLL_CALLCONV
FreeImageAlgorithms_SetBorder(FIBITMAP *src, int xborder, int yborder, BorderType type, double constant)
{
	FIABITMAP *dst = NULL;

	if(!src)
        return NULL;

    FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				dst = borderUCharImage.SetBorder(src, xborder, yborder, type, (unsigned char) constant);
			break;
		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			dst = borderUShortImage.SetBorder(src, xborder, yborder, type, (unsigned short) constant);
			break;
		case FIT_INT16:		// array of short: signed 16-bit
			dst = borderShortImage.SetBorder(src, xborder, yborder, type, (short) constant);
			break;
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			dst = borderULongImage.SetBorder(src, xborder, yborder, type, (unsigned long) constant);
			break;
		case FIT_INT32:		// array of long: signed 32-bit
			dst = borderLongImage.SetBorder(src, xborder, yborder, type, (long) constant);
			break;
		case FIT_FLOAT:		// array of float: 32-bit
			dst = borderFloatImage.SetBorder(src, xborder, yborder, type, (float) constant);
			break;
		case FIT_DOUBLE:	// array of double: 64-bit
			dst = borderDoubleImage.SetBorder(src, xborder, yborder, type, constant);
			break;
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN,
            "FREE_IMAGE_TYPE: Unable to set border for type %d.", type);
	}

	return dst;
}

FIABITMAP* DLL_CALLCONV
FreeImageAlgorithms_SetZeroBorder(FIBITMAP *src, int xborder, int yborder)
{
    return FreeImageAlgorithms_SetBorder(src, xborder, yborder, BorderType_Constant, 0.0);
}
