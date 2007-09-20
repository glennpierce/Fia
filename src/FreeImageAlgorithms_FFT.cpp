#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_FFT.h"

#include "kiss_fftnd.h"

template<class Tsrc>
class FFT2D
{
public:
	FIBITMAP*		TransformStandardToComplexImage(FIBITMAP *src, int inverse, int shift);
};


// Do FFT for type X
static FFT2D<unsigned char>		fftUCharImage;
static FFT2D<unsigned short>		fftUShortImage;
static FFT2D<short>			fftShortImage;
static FFT2D<unsigned long>		fftULongImage;
static FFT2D<long>			fftLongImage;
static FFT2D<float>			fftFloatImage;
static FFT2D<double>			fftDoubleImage;

static void GetAbsoluteXValues(kiss_fft_cpx* fftbuf, double *out_values, int size)
{
	int x;

	for(x=0; x < size; x++) {
				
		*(out_values + x) = (double) sqrt(pow( (double)((fftbuf + x)->r), (double) 2.0) + 
										  pow( (double) ((fftbuf + x)->i), (double) 2.0));
				  
	}
}

static void GetAbsoluteShiftedXValues(kiss_fft_cpx* fftbuf, double *out_values, int size)
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


static void GetShiftedComplexXValues(kiss_fft_cpx* fftbuf, FICOMPLEX *out_values, int size)
{
	int x, xhalf = size / 2;

	for(x=0; x < xhalf; x++) {
				
		(out_values + x + xhalf)->r = (double)((fftbuf + x)->r);
		(out_values + x + xhalf)->i = (double)((fftbuf + x)->i);			  
	}

	for(x=xhalf; x < size; x++) {
				
		(out_values + x - xhalf)->r = (double)((fftbuf + x)->r); 
		(out_values + x - xhalf)->i = (double)((fftbuf + x)->i); 
	}
}

template<class Tsrc> FIBITMAP* 
FFT2D<Tsrc>::TransformStandardToComplexImage(FIBITMAP *src, int inverse, int shift)
{
	int height, width;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

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

	// dims needs to be {rows, cols}, if you have contiguous rows.
	dims[0] = height = FreeImage_GetHeight(src);
	dims[1] = width = FreeImage_GetWidth(src);
	
    bufsize = width * height * sizeof(kiss_fft_cpx);
	fftbuf = (kiss_fft_cpx*) malloc(bufsize);
	tmp_fftoutbuf = fftoutbuf = (kiss_fft_cpx*) malloc(bufsize); 
	
    CheckMemory(fftbuf);
    CheckMemory(fftoutbuf);

	st = kiss_fftnd_alloc (dims, ndims, inverse, 0, 0);

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

	if(shift >= 1) {

		int yhalf = height / 2;

		for(y = yhalf - 1; y >= 0; y--) { 
		
			outbits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

			GetShiftedComplexXValues(tmp_fftoutbuf, outbits, width);
	
			tmp_fftoutbuf += width;
		}

		for(y = height -1; y >= yhalf; y--) { 
		
			outbits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

			GetShiftedComplexXValues(tmp_fftoutbuf, outbits, width);
	
			tmp_fftoutbuf += width;
		}
	}
	else {

		for(y = height - 1; y >= 0; y--) { 
		
			outbits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

			for(x=0; x < width; x++) {
				
				(outbits + x)->r = (double)((tmp_fftoutbuf + x)->r);
				(outbits + x)->i = (double)((tmp_fftoutbuf + x)->i);	  
			}

			tmp_fftoutbuf += width;
		}

	}

Error:
 
    free(fftbuf);
    free(fftoutbuf);
    free(st);


	return dst;
}


static FIBITMAP* 
TransformComplexToComplexImage(FIBITMAP *src, int inverse, int shift)
{
	int height = FreeImage_GetHeight(src);
	int width = FreeImage_GetWidth(src);
	int i=0, x, y;
	int dims[2];
	int ndims = 2;
	FICOMPLEX *bits; 
	FICOMPLEX *outbits;
	FIBITMAP *dst = NULL;
	
	kiss_fftnd_cfg st;
	kiss_fft_cpx* fftbuf;
	kiss_fft_cpx* fftoutbuf;

	// dims needs to be rows, cols, if you have contiguous rows)
	dims[0] = height;
	dims[1] = width;
	
	fftbuf = (kiss_fft_cpx*) malloc( width * height * sizeof(kiss_fft_cpx) );
	fftoutbuf = (kiss_fft_cpx*) malloc( width * height * sizeof(kiss_fft_cpx) ); 
	
    CheckMemory(fftbuf);
    CheckMemory(fftoutbuf);

	st = kiss_fftnd_alloc (dims, ndims, inverse, 0, 0);

	for(y = 0; y < height; y++) { 
		
		bits = (FICOMPLEX *) FreeImage_GetScanLine(src, y);
		
		for(x=0; x < width; x++) {
		
			fftbuf[i].r = (float) bits[x].r;
   		    fftbuf[i].i = (float) bits[x].i;
   		    
   		    i++;
		}
	}

	kiss_fftnd(st, fftbuf, fftoutbuf);

	if ( (dst = FreeImage_AllocateT(FIT_COMPLEX, width, height, 32, 0, 0, 0)) == NULL )
		return NULL;

	if(shift >= 1) {

		int yhalf = height / 2;

		for(y = yhalf; y < height; y++) { 
		
			outbits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

			GetShiftedComplexXValues(fftoutbuf, outbits, width);
	
			fftoutbuf += width;
		}

		for(y = 0; y < yhalf; y++) { 
		
			outbits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

			GetShiftedComplexXValues(fftoutbuf, outbits, width);
	
			fftoutbuf += width;
		}
	}
	else {

		for(y = 0; y < height; y++) { 
		
			outbits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

			for(x=0; x < width; x++) {
				
				(outbits + x)->r = (double)((fftoutbuf + x)->r);
				(outbits + x)->i = (double)((fftoutbuf + x)->i);	  
			}

			fftoutbuf += width;
		}

	}

	free(st);

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FFT(FIBITMAP *src, int inverse, int shift)
{
	if(!src)
		return NULL;

	// convert from src_type to FIT_BITMAP

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {

		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return fftUCharImage.TransformStandardToComplexImage(src, inverse, shift);				

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return fftUShortImage.TransformStandardToComplexImage(src, inverse, shift);
	
		case FIT_INT16:		// array of short: signed 16-bit
			return fftShortImage.TransformStandardToComplexImage(src, inverse, shift);
		
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return fftULongImage.TransformStandardToComplexImage(src, inverse, shift);
		
		case FIT_INT32:		// array of long: signed 32-bit
			return fftLongImage.TransformStandardToComplexImage(src, inverse, shift);
		
		case FIT_FLOAT:		// array of float: 32-bit
			return fftFloatImage.TransformStandardToComplexImage(src, inverse, shift);
		
		case FIT_DOUBLE:	// array of double: 64-bit
			return fftDoubleImage.TransformStandardToComplexImage(src, inverse, shift);
			
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			return TransformComplexToComplexImage(src, inverse, shift);
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

	// allocate a 8-bit dib
	dst = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);
	
	if(!dst)
		return NULL;

	FICOMPLEX *src_bits; 	
	double	  *dst_bits; 

	if(squared) {

		for(y = 0; y < height; y++) { 
		
			src_bits = (FICOMPLEX *) FreeImage_GetScanLine(src, y);
			dst_bits = (double *) FreeImage_GetScanLine(dst, y);

			for(x=0; x < width; x++) {
				
				*(dst_bits + x) = (double) (pow( (double)((src_bits + x)->r), (double) 2.0) + 
							    pow( (double)((src_bits + x)->i), (double) 2.0));
		
			}
		}
	}
	else {

		for(y = 0; y < height; y++) { 
		
			src_bits = (FICOMPLEX *) FreeImage_GetScanLine(src, y);
			dst_bits = (double *) FreeImage_GetScanLine(dst, y);

			for(x=0; x < width; x++) {
				
				*(dst_bits + x) = (double) sqrt(pow( (double)((src_bits + x)->r), (double) 2.0) + 
										pow( (double)((src_bits + x)->i), (double) 2.0));
		
			}
		}
	}

	return dst;
}

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_ConvertComplexImageToAbsoluteValuedSquared(FIBITMAP *src)
{
	return ConvertComplexImageToAbsoluteValued(src, true);
}

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_ConvertComplexImageToAbsoluteValued(FIBITMAP *src)
{
	return ConvertComplexImageToAbsoluteValued(src, false);
}
