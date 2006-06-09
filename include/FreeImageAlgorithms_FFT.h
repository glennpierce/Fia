#ifndef __FREEIMAGE_ALGORITHMS_FFT__
#define __FREEIMAGE_ALGORITHMS_FFT__

#include "FreeImageAlgorithms.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../src/KissFFT/kiss_fftnd.h"

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_GetFFTImage(FIBITMAP *src, int inverse, int shift);

DLL_API kiss_fft_cpx* DLL_CALLCONV
FreeImageAlgorithms_GetFFTArrayFromImage(FIBITMAP *src, int inverse);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FFTArrayToImage(kiss_fft_cpx* fftbuf, int width, int height, int shift);

DLL_API kiss_fft_cpx * DLL_CALLCONV
FreeImageAlgorithms_GetFFTArrayFromArray(kiss_fft_cpx* fftbuf, int width, int height, int inverse);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FFTComplexImage(FIBITMAP *src, int inverse, int shift);

#ifdef __cplusplus
}
#endif

#endif