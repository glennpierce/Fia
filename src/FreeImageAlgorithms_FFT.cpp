#include "FreeImageAlgorithms_FFT.h"

#include "kiss_fftnd.h"

template<class Tsrc>
class FFT2D
{
public:
	kiss_fft_cpx*	TransformFromImage(FIBITMAP *src, int inverse);
	FIBITMAP*  TransformToComplexImage(FIBITMAP *src, int inverse, int shift);
	FIBITMAP*  TransformToImage(FIBITMAP *src, int inverse, int shift);
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



template<class Tsrc> kiss_fft_cpx*
FFT2D<Tsrc>::TransformFromImage(FIBITMAP *src, int inverse)
{
	int i=0, x, y;
	int dims[2];
	int ndims = 2;
	int width = 0, height = 0;
	Tsrc *bits;
	FIBITMAP *dst = NULL;
	
	kiss_fftnd_cfg st;
	kiss_fft_cpx* fftbuf;
	kiss_fft_cpx* fftoutbuf;

	// dims needs to be rows, cols, if you have contiguous rows)
	height = dims[0] = FreeImage_GetHeight(src);
	width = dims[1] = FreeImage_GetWidth(src);
	
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

	free(fftbuf);
	free(st);

	return fftoutbuf;
}


template<class Tsrc> FIBITMAP* 
FFT2D<Tsrc>::TransformToImage(FIBITMAP *src, int inverse, int shift)
{
	int height = FreeImage_GetHeight(src);
	int width = FreeImage_GetWidth(src);

	kiss_fft_cpx *fftoutbuf = TransformFromImage(src, inverse);

	return FreeImageAlgorithms_FFTArrayToImage(fftoutbuf, width, height, shift);
}


kiss_fft_cpx * DLL_CALLCONV
FreeImageAlgorithms_GetFFTArrayFromArray(kiss_fft_cpx* fftbuf, int width, int height, int inverse)
{
	int dims[2];

	dims[1] = width;
	dims[0] = height;

	kiss_fft_cpx *fftoutbuf = (kiss_fft_cpx*) malloc( width * height * sizeof(kiss_fft_cpx) ); 

	kiss_fftnd_cfg st = kiss_fftnd_alloc (dims, 2, inverse, 0, 0);

	kiss_fftnd(st, fftbuf, fftoutbuf);

	free(st);

	return fftoutbuf;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FFTArrayToImage(kiss_fft_cpx* fftbuf, int width, int height, int shift)
{
	FIBITMAP *dst = NULL;

	int i=0, y;
	double *float_bits; 	
	
	if ( (dst = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0)) == NULL )
		return NULL;

	if(shift >= 1) {

		int xhalf = width / 2;
		int yhalf = height / 2;
		i = 0;

		for(y = yhalf; y < height; y++) { 
		
			float_bits = (double *) FreeImage_GetScanLine(dst, y);

			GetAbsoluteShiftedXValues(fftbuf, float_bits, width);

			fftbuf += width;
		}

		for(y = 0; y < yhalf; y++) { 
		
			float_bits = (double *) FreeImage_GetScanLine(dst, y);

			GetAbsoluteShiftedXValues(fftbuf, float_bits, width);
		
			fftbuf += width;
		}
	}
	else {

		for(y = 0; y < height; y++) { 
		
			float_bits = (double *) FreeImage_GetScanLine(dst, y);

			GetAbsoluteXValues(fftbuf, float_bits, width);

			fftbuf += width;
		}

	}

	return dst;
}


kiss_fft_cpx* DLL_CALLCONV
FreeImageAlgorithms_GetFFTArrayFromImage(FIBITMAP *src, int inverse)
{
	if(!src) return NULL;

	// convert from src_type to FIT_BITMAP

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {

		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return scaleUCharImage.TransformFromImage(src, inverse);

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return scaleUShortImage.TransformFromImage(src, inverse);
	
		case FIT_INT16:		// array of short: signed 16-bit
			return scaleShortImage.TransformFromImage(src, inverse);
		
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return scaleULongImage.TransformFromImage(src, inverse);
		
		case FIT_INT32:		// array of long: signed 32-bit
			return scaleLongImage.TransformFromImage(src, inverse);
		
		case FIT_FLOAT:		// array of float: 32-bit
			return scaleFloatImage.TransformFromImage(src, inverse);
		
		case FIT_DOUBLE:	// array of double: 64-bit
			return scaleDoubleImage.TransformFromImage(src, inverse);
			
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to perform FFT for type %d.", src_type);

	return NULL;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_GetFFTImage(FIBITMAP *src, int inverse, int shift)
{
	if(!src) return NULL;

	// convert from src_type to FIT_BITMAP

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {

		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return scaleUCharImage.TransformToImage(src, inverse, shift);

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return scaleUShortImage.TransformToImage(src, inverse, shift);
	
		case FIT_INT16:		// array of short: signed 16-bit
			return scaleShortImage.TransformToImage(src, inverse, shift);
		
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return scaleULongImage.TransformToImage(src, inverse, shift);
		
		case FIT_INT32:		// array of long: signed 32-bit
			return scaleLongImage.TransformToImage(src, inverse, shift);
		
		case FIT_FLOAT:		// array of float: 32-bit
			return scaleFloatImage.TransformToImage(src, inverse, shift);
		
		case FIT_DOUBLE:	// array of double: 64-bit
			return scaleDoubleImage.TransformToImage(src, inverse, shift);
			
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to perform FFT for type %d.", src_type);

	return NULL;
}









/*
template<class Tsrc> FIBITMAP* 
FFT2D<Tsrc>::TransformToComplexImage(FIBITMAP *src, int inverse, int shift)
{
	int height = FreeImage_GetHeight(src);
	int width = FreeImage_GetWidth(src);

	kiss_fft_cpx *fftoutbuf = TransformFromImage(src, inverse);
	
	FIBITMAP *dst = NULL;
	int i=0, y;
	FICOMPLEX *bits; 	
	
	if ( (dst = FreeImage_AllocateT(FIT_COMPLEX, width, height, 32, 0, 0, 0)) == NULL )
		return NULL;

	if(shift >= 1) {

		int xhalf = width / 2;
		int yhalf = height / 2;
		i = 0;

		for(y = yhalf; y < height; y++) { 
		
			bits = (double *) FreeImage_GetScanLine(dst, y);

			GetShiftedComplexXValues(fftbuf, bits, width);
	
			fftbuf += width;
		}

		for(y = 0; y < yhalf; y++) { 
		
			float_bits = (double *) FreeImage_GetScanLine(dst, y);

			GetShiftedComplexXValues(fftbuf, bits, width);
	
			fftbuf += width;
		}
	}
	else {

		for(y = 0; y < height; y++) { 
		
			bits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

			for(x=0; x < size; x++) {
				
				*(bits + x).r = (double)((fftbuf + x)->r);
				*(bits + x).i = (double)((fftbuf + x)->i);	  
			}

			fftbuf += width;
		}

	}

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FFTComplexImage(FIBITMAP *src, int inverse, int shift)
{
	if(!src) return NULL;

	// convert from src_type to FIT_BITMAP

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {

		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				return scaleUCharImage.TransformToComplexImage(src, inverse, shift);

		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			return scaleUShortImage.TransformToComplexImage(src, inverse, shift);
	
		case FIT_INT16:		// array of short: signed 16-bit
			return scaleShortImage.TransformToComplexImage(src, inverse, shift);
		
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			return scaleULongImage.TransformToComplexImage(src, inverse, shift);
		
		case FIT_INT32:		// array of long: signed 32-bit
			return scaleLongImage.TransformToComplexImage(src, inverse, shift);
		
		case FIT_FLOAT:		// array of float: 32-bit
			return scaleFloatImage.TransformToComplexImage(src, inverse, shift);
		
		case FIT_DOUBLE:	// array of double: 64-bit
			return scaleDoubleImage.TransformToComplexImage(src, inverse, shift);
			
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to perform FFT for type %d.", src_type);

	return NULL;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_ConvertComplexImageTo8Bit(FIBITMAP *src)
{
	FIBITMAP *dst = NULL;
	unsigned x, y;
						
	unsigned width	= FreeImage_GetWidth(src);
	unsigned height = FreeImage_GetHeight(src);

	// allocate a 8-bit dib
	dst = FreeImage_AllocateT(FIT_BITMAP, width, height, 8, 0, 0, 0);
	
	if(!dst)
		return NULL;

	double min, max, scale;
	FICOMPLEX *bits; 	

	for(y = 0; y < height; y++) { 
		
		bits = (FICOMPLEX *) FreeImage_GetScanLine(dst, y);

		for(x=0; x < width; x++) {
				
			*(bits + x).r = (double)((fftbuf + x)->r);
			*(bits + x).i = (double)((fftbuf + x)->i);	  
		}
	}

}


*/