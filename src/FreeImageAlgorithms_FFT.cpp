#include "FreeImageAlgorithms_FFT.h"

#include "kiss_fftnd.h"

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_GetFFT(FIBITMAP *src, int inverse)
{
	int i=0, x, y;
	int dims[2];
	int ndims = 2;
	unsigned char *bits;
	float *float_bits; 
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

	i=0;
	
    for(y = 0; y < dims[1]; y++) { 
		
		float_bits = (float *) FreeImage_GetScanLine(dst, y);
	
		for(x=0; x < dims[0]; x++) {
		
			float_bits[x] = (float) sqrt(pow(fftoutbuf[i].r, (float) 2.0) + pow(fftoutbuf[i].i, (float) 2.0));
			
   		    i++;
		}
	}
		
	return dst;
}