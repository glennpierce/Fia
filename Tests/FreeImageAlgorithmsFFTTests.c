#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_LinearScale.h"
#include "FreeImageAlgorithms_FFT.h"

static void
TestFreeImageAlgorithms_FFTFunctions(CuTest* tc)
{
	FIBITMAP *fft_dib, *scaled_dib;
	kiss_fft_cpx* fft_array, *inversed_array;
	int width, height;

	//char *file = TEST_IMAGE_DIRECTORY "\\8bit_lehar.png";
	//char *file = TEST_IMAGE_DIRECTORY "\\8bit_sin.png";
	//char *file = TEST_IMAGE_DIRECTORY "\\sinsum.png";
	//char *file = TEST_IMAGE_DIRECTORY "\\sin_non_periodic.png";
	//char *file = TEST_IMAGE_DIRECTORY "\\sin_diagonal.png";
	//char *file = TEST_IMAGE_DIRECTORY "\\sin2d_colour.png";
	char *file = TEST_IMAGE_DIRECTORY "\\square.gif";
	//char *file = "C:\\Documents and Settings\\Pierce\\My Documents\\Test Images\\rjl.jpg";

	double min_found = 0.0, max_found = 0.0;

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib != NULL);

	//fft_dib = FreeImageAlgorithms_FFTComplexImage(dib, 0, 1);


	fft_dib = FreeImageAlgorithms_GetFFTImage(dib, 0, 1);  

	//fft_array = FreeImageAlgorithms_GetFFTArrayFromImage(dib, 0);

	//width = FreeImage_GetWidth(dib);
	//height = FreeImage_GetHeight(dib);

	//FreeImage_Unload(dib);

	//inversed_array = FreeImageAlgorithms_GetFFTArrayFromArray(fft_array, width, height, 1);

	//fft_dib = FreeImageAlgorithms_FFTArrayToImage(inversed_array, width, height, 0);

	//scaled_dib = FreeImageAlgorithms_LinearScaleToStandardType(fft_dib, 0, 0, &min_found, &max_found);  
	
	
	//scaled_dib = FreeImage_ConvertToStandardType(fft_dib, 1);

	
	//dib = FreeImageAlgorithms_GetFFTImage(fft_dib, 1, 0);  


	//CuAssertTrue(tc, fft_dib != NULL);
	//CuAssertTrue(tc, scaled_dib != NULL);

	ShowImage(fft_dib);

	//FreeImage_Unload(dib);
	FreeImage_Unload(fft_dib);
	//FreeImage_Unload(scaled_dib);
}


CuSuite* 
DLL_CALLCONV CuGetFreeImageAlgorithmsFFTSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FFTFunctions);

	return suite;
}
