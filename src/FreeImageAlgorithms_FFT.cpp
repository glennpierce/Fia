/* 
 * Copyright 2007-2010 Glenn Pierce, Paul Barber,
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_FFT.h"

#include "kiss_fftnd.h"
#include <iostream>

template<class Tsrc> class FFT2D
{
    public:
        FIBITMAP* FFT(FIBITMAP *src);
};

// Do FFT for type X
static FFT2D<unsigned char> fftUCharImage;
static FFT2D<unsigned short> fftUShortImage;
static FFT2D<short> fftShortImage;
static FFT2D<unsigned long> fftULongImage;
static FFT2D<long> fftLongImage;
static FFT2D<float> fftFloatImage;
static FFT2D<double> fftDoubleImage;

/*
static inline void GetAbsoluteXValues(kiss_fft_cpx* fftbuf, double *out_values, int size)
{
	int x;

	for(x=0; x < size; x++) {
				
		*(out_values + x) = (double) sqrt(pow( (double)((fftbuf + x)->r), (double) 2.0) + 
										  pow( (double) ((fftbuf + x)->i), (double) 2.0));		  
	}
}

static inline void GetAbsoluteShiftedXValues(kiss_fft_cpx* fftbuf, double *out_values, int size)
{
	int x, xhalf = size / 2;

	for(x=0; x < xhalf; x++) {
				
		*(out_values + x + xhalf) = (double) sqrt(pow( (double)((fftbuf + x)->r), (double) 2.0) + 
										  pow( (double) ((fftbuf + x)->i), (double) 2.0));		  
	}

	for(x=xhalf; x < size; x++) {
				
		*(out_values + x - xhalf) = (double) sqrt(pow( (double)((fftbuf + x)->r), (double) 2.0) + 
										  pow( (double) ((fftbuf + x)->i), (double) 2.0));
	}
}


static inline void GetShiftedComplexXValues(kiss_fft_cpx* fftbuf, FICOMPLEX *out_values, int size)
{
	int x, xhalf = size / 2;
    FICOMPLEX *out_value;
    kiss_fft_cpx* buf_value;

	for(x=0; x <= xhalf; x++) {
				
        out_value = out_values + x + xhalf;
        buf_value = fftbuf + x;

		out_value->r = (double)buf_value->r;
		out_value->i = (double)buf_value->i;			  
	}

	for(x=xhalf + 1; x < size; x++) {
		
        out_value = out_values + x - xhalf;
        buf_value = fftbuf + x;

		out_value->r = (double)buf_value->r; 
		out_value->i = (double)buf_value->i; 
	}
}
*/

template<class Tsrc> FIBITMAP* 
FFT2D<Tsrc>::FFT(FIBITMAP *src)
{
	int height, width;

	int i=0, x, y;
	int dims[2];
	int ndims = 2;
    size_t bufsize;
	Tsrc *bits; 
	FICOMPLEX *outbits;
	FIBITMAP *dst = NULL;
	
	kiss_fftnd_cfg st;
	kiss_fft_cpx* fftbuf;
	kiss_fft_cpx* fftoutbuf;
    kiss_fft_cpx* tmp_fftoutbuf;

	// Dims needs to be {rows, cols}, if you have contiguous rows.
	dims[0] = height = FreeImage_GetHeight(src);
	dims[1] = width = FreeImage_GetWidth(src);
	
    bufsize = width * height * sizeof(kiss_fft_cpx);
	fftbuf = (kiss_fft_cpx*) malloc(bufsize);
	tmp_fftoutbuf = fftoutbuf = (kiss_fft_cpx*) malloc(bufsize); 
	
    CheckMemory(fftbuf);
    CheckMemory(fftoutbuf);

	memset(fftbuf,0,bufsize);
    memset(tmp_fftoutbuf,0,bufsize);

	st = kiss_fftnd_alloc (dims, ndims, 0, 0, 0);

	for(y = height - 1; y >= 0; y--) { 
		
		bits = (Tsrc *) FreeImage_GetScanLine(src, y);
		
		for(x=0; x < width; x++) {
		
			fftbuf[i].r = (float) bits[x];
   		    fftbuf[i].i = 0.0;
   		 
   		    i++;
		}
	}

	kiss_fftnd(st, fftbuf, tmp_fftoutbuf);

	if ( (dst = FreeImage_AllocateT(FIT_COMPLEX, width, height, 32, 0, 0, 0)) == NULL )
		goto Error;

	for(y = height - 1; y >= 0; y--) { 
		
		outbits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

		for(x=0; x < width; x++) {
				
			(outbits + x)->r = (double)((tmp_fftoutbuf + x)->r);
			(outbits + x)->i = (double)((tmp_fftoutbuf + x)->i);	  
		}

		tmp_fftoutbuf += width;
	}

Error:
 
    free(fftbuf);
    free(fftoutbuf);
    free(st);

	return dst;
}

FIBITMAP* DLL_CALLCONV
FIA_ShiftImageEdgeToCenter(FIBITMAP *src)
{
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
	int xhalf = (width / 2);
	int yhalf = (height / 2);
	int bytes_pp = FreeImage_GetBPP(src) / 8;
	int xhalf_bytes = xhalf * bytes_pp;

	//std::cout << "width " << width << std::endl;
	//std::cout << "xhalf " << xhalf << std::endl;
	
	FIBITMAP *dst = FIA_CloneImageType(src, width, height);

	for(int y=0; y < yhalf; y++) { 
		
		BYTE* srcbits = (BYTE *) FIA_GetScanLineFromTop(src, y);
		BYTE* outbits = (BYTE *) FIA_GetScanLineFromTop(dst, y + yhalf);

		memcpy(outbits + xhalf_bytes, srcbits, xhalf_bytes);
		memcpy(outbits, srcbits + xhalf_bytes, xhalf_bytes);
	}

	for(int y=yhalf; y < height; y++) { 
		
		BYTE* srcbits = (BYTE *) FIA_GetScanLineFromTop(src, y);
		BYTE* outbits = (BYTE *) FIA_GetScanLineFromTop(dst, y - yhalf);

		memcpy(outbits + xhalf_bytes, srcbits, xhalf_bytes);
		memcpy(outbits, srcbits + xhalf_bytes, xhalf_bytes);
	}

	return dst;
}

int DLL_CALLCONV
FIA_InPlaceShiftImageEdgeToCenter(FIBITMAP **src)
{
    FIBITMAP *dst = FIA_ShiftImageEdgeToCenter(*src);

    FreeImage_Unload(*src);
    *src = dst;

    return FIA_SUCCESS;
}

FIBITMAP* DLL_CALLCONV
FIA_IFFT(FIBITMAP *src)
{
	int height, width;

	int i=0, x, y;
	int dims[2];
	int ndims = 2;
    size_t bufsize;
	FICOMPLEX *bits; 
	FICOMPLEX *outbits;
	FIBITMAP *dst = NULL;
	
	kiss_fftnd_cfg st;
	kiss_fft_cpx* fftbuf;
	kiss_fft_cpx* fftoutbuf;
    kiss_fft_cpx* tmp_fftoutbuf;

	// Dims needs to be {rows, cols}, if you have contiguous rows.
    dims[0] = height = FreeImage_GetHeight(src);
    dims[1] = width = FreeImage_GetWidth(src);
	
    bufsize = width * height * sizeof(kiss_fft_cpx);
	fftbuf = (kiss_fft_cpx*) malloc(bufsize);
	tmp_fftoutbuf = fftoutbuf = (kiss_fft_cpx*) malloc(bufsize); 
	
    memset(fftbuf,0,bufsize);
    memset(tmp_fftoutbuf,0,bufsize);

	st = kiss_fftnd_alloc (dims, ndims, 1, 0, 0);
	
	for(y = height - 1; y >= 0; y--) { 
			
		bits = (FICOMPLEX*) FreeImage_GetScanLine(src, y);
			
		for(x=0; x < width; x++) {	

			fftbuf[i].r = (float) bits[x].r;
   			fftbuf[i].i = (float) bits[x].i;
	        
   			i++;
		}
	}

	kiss_fftnd(st, fftbuf, tmp_fftoutbuf);

	if ( (dst = FreeImage_AllocateT(FIT_COMPLEX, width, height, 32, 0, 0, 0)) == NULL )
		goto Error;

	for(y = height - 1; y >= 0; y--) { 
		
		outbits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

		for(x=0; x < width; x++) {
				
			outbits[x].r = (double)((tmp_fftoutbuf + x)->r);  
			outbits[x].i = (double)((tmp_fftoutbuf + x)->i); 
		}

		tmp_fftoutbuf += width;
	}

	return dst;

Error:
 
    free(fftbuf);
    free(fftoutbuf);
    free(st);

	return dst;
}


FIBITMAP* DLL_CALLCONV
FIA_FFT(FIBITMAP *src)
{
	if(!src)
		return NULL;

	// Convert from src_type to FIT_BITMAP
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch (src_type)
    {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return fftUCharImage.FFT(src);				
			break;
			
		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return fftUShortImage.FFT(src);
	
		case FIT_INT16:		// array of short: signed 16-bit
			return fftShortImage.FFT(src);
		
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return fftULongImage.FFT(src);
		
		case FIT_INT32:		// array of long: signed 32-bit
			return fftLongImage.FFT(src);
		
		case FIT_FLOAT:		// array of float: 32-bit
			return fftFloatImage.FFT(src);
		
		case FIT_DOUBLE:	// array of double: 64-bit
			return fftDoubleImage.FFT(src);
				
		default:
			break;
	}

	FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to perform FFT for type %d.", src_type);

	return NULL;
}


static FIBITMAP*
ConvertComplexImageToAbsoluteValued(FIBITMAP *src, bool squared)
{
	FIBITMAP *dst = NULL;
	unsigned x, y;
						
	unsigned width	= FreeImage_GetWidth(src);
	unsigned height = FreeImage_GetHeight(src);

	// Allocate a double bit dib
	dst = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);
	
	if(!dst)
		return NULL;

	FICOMPLEX *src_bits, *src_bit; 	
	double	  *dst_bits; 

	if(squared) {

		for(y = 0; y < height; y++) { 
		
			src_bits = (FICOMPLEX *) FreeImage_GetScanLine(src, y);
			dst_bits = (double *) FreeImage_GetScanLine(dst, y);

			for(x=0; x < width; x++) {
				
                src_bit = src_bits + x;

				*(dst_bits + x) = (double) ((src_bit->r * src_bit->r) + (src_bit->i * src_bit->i));
			}
		}
	}
	else {

		for(y = 0; y < height; y++) { 
		
			src_bits = (FICOMPLEX *) FreeImage_GetScanLine(src, y);
			dst_bits = (double *) FreeImage_GetScanLine(dst, y);

			for(x=0; x < width; x++) {
				
                src_bit = src_bits + x;

				*(dst_bits + x) = sqrt((double) ((src_bit->r * src_bit->r) + (src_bit->i * src_bit->i)));
			}
		}
	}

	return dst;
}

FIBITMAP* DLL_CALLCONV
FIA_ConvertComplexImageToAbsoluteValuedSquared(FIBITMAP *src)
{
	return ConvertComplexImageToAbsoluteValued(src, true);
}

FIBITMAP* DLL_CALLCONV
FIA_ConvertComplexImageToAbsoluteValued(FIBITMAP *src)
{
	return ConvertComplexImageToAbsoluteValued(src, false);
}

FIBITMAP* DLL_CALLCONV
FIA_ComplexImageToRealValued(FIBITMAP *src)
{
	FIBITMAP *dst = NULL;
	unsigned x, y;
						
	unsigned width	= FreeImage_GetWidth(src);
	unsigned height = FreeImage_GetHeight(src);

	// allocate a 8-bit dib
	dst = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);
	
	if(!dst)
		return NULL;

	FICOMPLEX *src_bits; 	
	double	  *dst_bits; 

	for(y = 0; y < height; y++) { 
		
		src_bits = (FICOMPLEX *) FreeImage_GetScanLine(src, y);
		dst_bits = (double *) FreeImage_GetScanLine(dst, y);

		for(x=0; x < width; x++) {
			dst_bits[x] = (double) src_bits[x].r;
		}
	}

	return dst;
}
