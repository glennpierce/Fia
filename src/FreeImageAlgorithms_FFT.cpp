#include "FreeImageAlgorithms_FFT.h"

#include "kiss_fftnd.h"


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_GetFFT(FIBITMAP *src, int inverse, int shift)
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

	

	

	//int total_pixels = dims[0] * dims[1];
	//int half = (total_pixels / 2) + (dims[0] / 2);
	//i = half + 1;
	


	int xhalf = dims[0] / 2;
	int yhalf = dims[1] / 2;
	i = 0;

	for(y = yhalf; y > 0; y--) { 
		
		float_bits = (double *) FreeImage_GetScanLine(dst, y);

		for(x=0; x < xhalf; x++) {
				
			float_bits[x + xhalf] = (double) sqrt(pow( (double)fftoutbuf[i + x].r, (double) 2.0) + 
										  pow( (double) fftoutbuf[i + x].i, (double) 2.0));
				  
		}

		for(x=xhalf; x < dims[0]; x++) {
				
			float_bits[x - xhalf] = (double) sqrt(pow( (double)fftoutbuf[i + x].r, (double) 2.0) + 
										  pow( (double) fftoutbuf[i + x].i, (double) 2.0));
		}

			i += dims[0];
	}


	for(y = dims[1] - 1; y > yhalf; y--) { 
		
		float_bits = (double *) FreeImage_GetScanLine(dst, y);

		for(x=0; x < xhalf; x++) {
				
			float_bits[x + xhalf] = (double) sqrt(pow( (double)fftoutbuf[i + x].r, (double) 2.0) + 
										  pow( (double) fftoutbuf[i + x].i, (double) 2.0));
				  
		}

		for(x=xhalf; x < dims[0]; x++) {
				
			float_bits[x - xhalf] = (double) sqrt(pow( (double)fftoutbuf[i + x].r, (double) 2.0) + 
										  pow( (double) fftoutbuf[i + x].i, (double) 2.0));
		}

			i += dims[0];
	}
	

//float_bits = (double *) FreeImage_GetScanLine(dst, 0);
//	float_bits[dims[0] - 1] = 2000000.0;





//	for(i=half + 1; i < total_pixels; i++) {
		
		//x = i % dims[0];
		//y = i / dimes[0];

//		*float_bits++ = (double) sqrt(pow( (double)fftoutbuf[i].r, (double) 2.0) + pow( (double) fftoutbuf[i].i, (double) 2.0));

//	}


	/*
	for(y = 0; y < dims[1]; y++) { 
		
		float_bits = (double *) FreeImage_GetScanLine(dst, y);
	
		for(x=0; x < dims[0]; x++) {
		
			float_bits[x] = (double) sqrt(pow( (double)fftoutbuf[i].r, (double) 2.0) + pow( (double) fftoutbuf[i].i, (double) 2.0));
			
   		    i++;
		}
	}

*/
	


	//int first_half_size = dims[0] / 2;
	//int second_half_size = dims[0] - first_half_size;
	//float *shifted_scanline = (float*) malloc(sizeof(float) * first_half_size);

	
/*

    for(y = 0; y < dims[1]; y++) { 
		
		float_bits = (double *) FreeImage_GetScanLine(dst, y);
	
		for(x=0; x < dims[0]; x++) {
		
			float_bits[x] = (double) sqrt(pow( (double)fftoutbuf[i].r, (double) 2.0) + pow( (double) fftoutbuf[i].i, (double) 2.0));
			
   		    i++;
		}

		//if(shift > 0) {
		//	memcpy(shifted_scanline, float_bits, first_half_size);
		//	memmove(float_bits, float_bits + second_half_size, second_half_size);
		//	memcpy(float_bits + second_half_size, shifted_scanline, first_half_size);
		//}
	}
		
	//free(shifted_scanline);
*/



	return dst;
}