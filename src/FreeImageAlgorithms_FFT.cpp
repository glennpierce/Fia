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
	unsigned char *bits;
	double *float_bits; 
	FIBITMAP *dst = NULL;
	
	kiss_fftnd_cfg st;
	kiss_fft_cpx* fftbuf;
	kiss_fft_cpx* fftoutbuf;
	
	dims[0] = FreeImage_GetWidth(src);
	dims[1] = FreeImage_GetHeight(src);
	
	fftbuf = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
	fftoutbuf = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) ); 
	
	st = kiss_fftnd_alloc (dims, ndims, inverse, 0, 0);

	for(y = 0; y < dims[1]; y++) { 
		
		bits = (unsigned char *) FreeImage_GetScanLine(src, y);
		
		for(x=0; x < dims[0]; x++) {
		
			fftbuf[i].r = bits[x];
   		    fftbuf[i].i = 0;
   		    
   		    i++;
		}
	}

	kiss_fftnd(st,fftbuf, fftoutbuf);

	if ( (dst = FreeImage_AllocateT(FIT_DOUBLE, dims[0], dims[1], 32, 0, 0, 0)) == NULL )
		return NULL;

	int xhalf = dims[0] / 2;
	int yhalf = dims[1] / 2;
	i = 0;

	for(y = yhalf; y < dims[1]; y++) { 
		
		float_bits = (double *) FreeImage_GetScanLine(dst, y);

		GetAbsoluteShiftedXValues(fftoutbuf, float_bits, dims[0]);

		fftoutbuf += dims[0];
	}

	for(y = 0; y < yhalf; y++) { 
		
		float_bits = (double *) FreeImage_GetScanLine(dst, y);

		GetAbsoluteShiftedXValues(fftoutbuf, float_bits, dims[0]);
		
		fftoutbuf += dims[0];
	}

	return dst;
}