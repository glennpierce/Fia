#include "CuTest.h"

#include "FreeImageAlgorithms_Testing.h"

#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_LinearScale.h"
#include "FreeImageAlgorithms_FFT.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "math.h"

static void CreateLogDisplay(FIBITMAP *src)
{
	int i;

	double min = 0.0, max = 0.0;

	double *src_ptr = (double *) FreeImage_GetBits(src);

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	FreeImageAlgorithms_FindMinMax(src, &min, &max);

	for(i=0; i < number_of_pixels; i++)
		*src_ptr++ = (int) ((255.0 * log10(*src_ptr)/log10(max)) + 0.5);
}

static void
TestFreeImageAlgorithms_FFTFunctions(CuTest* tc)
{
	FIBITMAP *fft_dib, *real_dib, *scaled_dib;
	char *file = IMAGE_DIR "\\colour_lines.png";
		
	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);	
	FIBITMAP *greyscale_dib = FreeImage_ConvertToGreyscale(dib);

	ProfileStart("FFT");

	fft_dib = FreeImageAlgorithms_FFT(greyscale_dib, 0, 1);  

	ProfileStop("FFT");

	ProfilePrint();

//	real_dib = FreeImageAlgorithms_ConvertComplexImageToAbsoluteValued(fft_dib);

//	CreateLogDisplay(real_dib);

//	scaled_dib = FreeImage_ConvertToStandardType(real_dib, 0);

//	FreeImageAlgorithms_SetGreyLevelPalette(scaled_dib);
	
//	ShowImage(scaled_dib);

	FreeImage_Unload(dib);
	FreeImage_Unload(greyscale_dib);
//	FreeImage_Unload(real_dib);
	FreeImage_Unload(fft_dib);
//	FreeImage_Unload(scaled_dib);
}



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

	ShowImage(scaled_dib);

	FreeImage_Unload(real_dib);
	FreeImage_Unload(scaled_dib);
}


CuSuite* 
DLL_CALLCONV CuGetFreeImageAlgorithmsFFTSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FFTFunctions);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_Correlation);

	return suite;
}
