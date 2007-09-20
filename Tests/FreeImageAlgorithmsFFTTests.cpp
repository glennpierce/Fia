#include "CuTest.h"

#include "FreeImageAlgorithms_Testing.h"

#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_LinearScale.h"
#include "FreeImageAlgorithms_FFT.h"
#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "profile.h"
#include "math.h"

#include "kiss_fftnd.h"

static void
TestFreeImageAlgorithms_DisplayFFT(CuTest* tc)
{
	FIBITMAP *fft_dib, *real_dib, *log_dib;
	char *file = IMAGE_DIR "\\colour_lines.png";
		
	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);	
	FIBITMAP *greyscale_dib = FreeImage_ConvertToGreyscale(dib);

	PROFILE_START("FFT");

	fft_dib = FreeImageAlgorithms_FFT(greyscale_dib, 0, 1);  

	real_dib = FreeImageAlgorithms_ConvertComplexImageToAbsoluteValued(fft_dib);

	log_dib = FreeImageAlgorithms_Log(real_dib);

	FreeImageAlgorithms_SaveFIBToFile(log_dib, TEMP_DIR "\\fft.bmp", BIT8);

	PROFILE_STOP("FFT");

	FreeImage_Unload(dib);
	FreeImage_Unload(greyscale_dib);
	FreeImage_Unload(real_dib);
	FreeImage_Unload(fft_dib);
	FreeImage_Unload(log_dib);
}


/*
static void
TestFreeImageAlgorithms_Correlation(CuTest* tc)
{
	FIBITMAP *first_dib, *second_dib;
	FIBITMAP *gs_first_dib, *gs_second_dib;
	FIBITMAP *first_fft, *second_fft;
	FIBITMAP *real_dib, *scaled_dib, *result_fft;

	char *first_file = IMAGE_DIR "\\correlate1.png";
	char *second_file = IMAGE_DIR "\\correlate2.png";

	first_dib = FreeImageAlgorithms_LoadFIBFromFile(first_file);
	second_dib = FreeImageAlgorithms_LoadFIBFromFile(second_file);

	gs_first_dib = FreeImage_ConvertToGreyscale(first_dib);
	gs_second_dib = FreeImage_ConvertToGreyscale(second_dib);

	FreeImage_Unload(first_dib);
	FreeImage_Unload(second_dib);

	first_fft = FreeImageAlgorithms_FFT(gs_first_dib, 0, 1);  
	second_fft = FreeImageAlgorithms_FFT(gs_second_dib, 0, 1);  

	FreeImage_Unload(gs_first_dib);
	FreeImage_Unload(gs_second_dib);

	// Perform conjugate operation
	FreeImageAlgorithms_ComplexConjugate(second_fft);
	FreeImageAlgorithms_MultiplyComplexImages(first_fft, second_fft);

	result_fft = FreeImageAlgorithms_FFT(first_fft, 1, 0);  

	real_dib = FreeImageAlgorithms_ConvertComplexImageToAbsoluteValued(result_fft);

	//CreateLogDisplay(real_dib);

	scaled_dib = FreeImage_ConvertToStandardType(real_dib, 1);
	//scaled_dib = FreeImageAlgorithms_LinearScaleToStandardType(real_dib, 0, 0, &min_found, &max_found);  

	FreeImageAlgorithms_SetGreyLevelPalette(scaled_dib);

	FreeImage_Unload(real_dib);
	FreeImage_Unload(scaled_dib);
}
*/

static void
TestFreeImageAlgorithms_FFTTest(CuTest* tc)
{
	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(IMAGE_DIR "\\FFTTest.bmp");
	
    FIBITMAP* complex_fft = FreeImageAlgorithms_FFT(dib, 0, 1); 
    
    CuAssertTrue(tc, complex_fft != NULL);

	FreeImage_Unload(dib);
	FreeImage_Unload(complex_fft);
}


CuSuite* 
DLL_CALLCONV CuGetFreeImageAlgorithmsFFTSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FFTTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_DisplayFFT);

	return suite;
}
