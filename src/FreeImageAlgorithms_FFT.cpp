#include "FreeImageAlgorithms_FFT.h"

#include "kiss_fftnd.h"

template<class Tsrc>
class FFT2D
{
public:
	FIBITMAP*		TransformStandardToComplexImage(FIBITMAP *src, int inverse, int shift);
};


// Do FFT for type X
FFT2D<unsigned char>		scaleUCharImage;
FFT2D<unsigned short>		scaleUShortImage;
FFT2D<short>				scaleShortImage;
FFT2D<unsigned long>		scaleULongImage;
FFT2D<long>					scaleLongImage;
FFT2D<float>				scaleFloatImage;
FFT2D<double>				scaleDoubleImage;

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
	int height = FreeImage_GetHeight(src);
	int width = FreeImage_GetWidth(src);

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	int i=0, x, y;
	int dims[2];
	int ndims = 2;
	Tsrc *bits; 
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
	
	st = kiss_fftnd_alloc (dims, ndims, inverse, 0, 0);

	for(y = 0; y < height; y++) { 
		
		bits = (Tsrc *) FreeImage_GetScanLine(src, y);
		
		for(x=0; x < width; x++) {
		
			fftbuf[i].r = (float) bits[x];
   		    fftbuf[i].i = 0.0;
   		    
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


/*
static FIBITMAP* 
TransformRGBToComplexImage(FIBITMAP *src, int inverse, int shift)
{
	int height = FreeImage_GetHeight(src);
	int width = FreeImage_GetWidth(src);
	int i=0, x, y;
	int dims[2];
	int ndims = 2;
	unsigned char *bits; 
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
*/


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
				return scaleUCharImage.TransformStandardToComplexImage(src, inverse, shift);				

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return scaleUShortImage.TransformStandardToComplexImage(src, inverse, shift);
	
		case FIT_INT16:		// array of short: signed 16-bit
			return scaleShortImage.TransformStandardToComplexImage(src, inverse, shift);
		
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return scaleULongImage.TransformStandardToComplexImage(src, inverse, shift);
		
		case FIT_INT32:		// array of long: signed 32-bit
			return scaleLongImage.TransformStandardToComplexImage(src, inverse, shift);
		
		case FIT_FLOAT:		// array of float: 32-bit
			return scaleFloatImage.TransformStandardToComplexImage(src, inverse, shift);
		
		case FIT_DOUBLE:	// array of double: 64-bit
			return scaleDoubleImage.TransformStandardToComplexImage(src, inverse, shift);
			
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			return TransformComplexToComplexImage(src, inverse, shift);
	}

	FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to perform FFT for type %d.", src_type);

	return NULL;
}



FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_ConvertComplexImageToAbsoluteValued(FIBITMAP *src)
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
				
			*(dst_bits + x) = (double) sqrt(pow( (double)((src_bits + x)->r), (double) 2.0) + 
										pow( (double)((src_bits + x)->i), (double) 2.0));
		
		}
	}

	return dst;
}

