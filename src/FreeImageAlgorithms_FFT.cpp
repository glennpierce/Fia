#include "FreeImageAlgorithms_FFT.h"
 
#include "kiss_fftnd.h"

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


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_GetFFT(FIBITMAP *src, int inverse)
{
	int i=0, x, y;
	int dims[2];
	int ndims = 2;
	int width, height;
	unsigned char *bits;
	double *float_bits; 
	FIBITMAP *dst = NULL;
	
	kiss_fftnd_cfg st;
	kiss_fft_cpx* fftbuf;
	kiss_fft_cpx* fftoutbuf;
	
	// dims needs to be rows, cols, if you have contiguous rows)
	height = dims[0] = FreeImage_GetHeight(src);
	width = dims[1] = FreeImage_GetWidth(src);
	
	fftbuf = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	fftoutbuf = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) ); 
	
	st = kiss_fftnd_alloc (dims, ndims, inverse, 0, 0);

	for(y = 0; y < height; y++) { 
		
		bits = (unsigned char *) FreeImage_GetScanLine(src, y);
		
		for(x=0; x < width; x++) {
		
			fftbuf[i].r = bits[x];
   		    fftbuf[i].i = 0;
   		    
   		    i++;
		}
	}

	kiss_fftnd(st,fftbuf, fftoutbuf);

	if ( (dst = FreeImage_AllocateT(FIT_DOUBLE, dims[0], dims[1], 32, 0, 0, 0)) == NULL )
		return NULL;

	int xhalf = width / 2;
	int yhalf = height / 2;
	i = 0;

	for(y = yhalf; y < height; y++) { 
		
		float_bits = (double *) FreeImage_GetScanLine(dst, y);

		GetAbsoluteShiftedXValues(fftoutbuf, float_bits, width);

		fftoutbuf += width;
	}

	for(y = 0; y < yhalf; y++) { 
		
		float_bits = (double *) FreeImage_GetScanLine(dst, y);

		GetAbsoluteShiftedXValues(fftoutbuf, float_bits, width);
		
		fftoutbuf += width;
	}

	return dst;
}