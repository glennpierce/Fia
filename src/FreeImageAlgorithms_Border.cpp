#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Utilities.h"

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

static inline void SetImageRowToConstant(FIBITMAP *src, int row_start, int count, double val)
{
    int pitch = FreeImage_GetPitch(src);

    BYTE *dst_bits;

    for(int y = row_start; y < (row_start + count); y++) {

        dst_bits = FreeImage_GetScanLine(src, y);

        memset(dst_bits, val, pitch);
    }
}

static inline void SetImageColToConstant(FIBITMAP *src, int col_start, int count, double val)
{
    int height = FreeImage_GetHeight(src);
    int pitch = FreeImage_GetPitch(src);

    BYTE* bits = FreeImage_GetBits(src);

    for(int y = 0; y < height; y++) {

        for(int x = col_start; x < (col_start + count); x++)
            bits[x] = val;

        bits += pitch;
    }
}


FIABITMAP* DLL_CALLCONV
FreeImageAlgorithms_SetBorder(FIBITMAP *src, int xborder, int yborder, BorderType type, double constant)
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

    BYTE *dst_bits, *src_bits;
    int src_pitch = FreeImage_GetPitch(src);
    int dst_pitch = FreeImage_GetPitch(dst->fib);

    if(type == BorderType_Constant && constant != 0.0) {
   
        double val = constant, max_possible;
        
        FreeImageAlgorithms_GetMaxPosibleValueForFib(src, &max_possible);

        if(val > max_possible);
            val = max_possible;

        // Set the bottom line of the image to constant value
        SetImageRowToConstant(dst->fib, 0, yborder, val);

        // Set the top line of the image to constant value
        SetImageRowToConstant(dst->fib, dst_height - yborder, yborder, val);

        // Set the left pixels of the image
        SetImageColToConstant(dst->fib, 0, xborder, val);

        // Set the right pixels of the image
        SetImageColToConstant(dst->fib, dst_width - xborder, xborder, val);

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
        BYTE* src_bits, *dst_bits;

        // Get the bottom line of the original image and copy into bottom border

        int border_row_start = yborder - 1; // First row on border for bottom
        int image_row_start = yborder;

        for(int i = 0; i < yborder; i++) {

            dst_bits = FreeImage_GetScanLine(dst->fib, border_row_start);
            src_bits = FreeImage_GetScanLine(dst->fib, image_row_start);

            memcpy(dst_bits, src_bits, pitch);

            border_row_start--;
            image_row_start++;
        }

        // Top 

        border_row_start = dst_height - yborder; // First row on border fort bottom
        image_row_start = border_row_start - 1;

        for(int i = 0; i < yborder; i++) {

            dst_bits = FreeImage_GetScanLine(dst->fib, border_row_start);
            src_bits = FreeImage_GetScanLine(dst->fib, image_row_start);

            memcpy(dst_bits, src_bits, pitch);

            border_row_start++;
            image_row_start--;
        }

        // Left
        dst_bits = FreeImage_GetBits(dst->fib);

        for(int y = 0; y < dst_height; y++) {

            for(int x = 0; x < xborder; x++) {
                dst_bits[x] = dst_bits[2 * xborder - 1 - x];
            }

            dst_bits += pitch;  
        }
      
        // Right
        dst_bits = FreeImage_GetBits(dst->fib);
        border_row_start = dst_width - xborder;

        for(int y = 0; y < dst_height; y++) {

            for(int x = 0; x < xborder; x++) {
                dst_bits[border_row_start + x] = dst_bits[border_row_start - 1 - x];
            }

            dst_bits += pitch;  
        }

    }

	return dst;
}

FIABITMAP* DLL_CALLCONV
FreeImageAlgorithms_SetZeroBorder(FIBITMAP *src, int xborder, int yborder)
{
    return FreeImageAlgorithms_SetBorder(src, xborder, yborder, BorderType_Constant, 0.0);
}