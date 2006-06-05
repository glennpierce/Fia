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

void TestFreeImageAlgorithms_FFTFunctions(CuTest* tc)
{
	FIBITMAP *fft_dib, *scaled_dib;

	//char *file = TEST_IMAGE_DIRECTORY "\\8bit_lehar.png";
	char *file = TEST_IMAGE_DIRECTORY "\\8bit_sin.png";

	double min_found = 0.0, max_found = 0.0;

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib != NULL);

	fft_dib = FreeImageAlgorithms_GetFFT(dib, 0, 0);  
	//scaled_dib = FreeImageAlgorithms_LinearScaleToStandardType(fft_dib, 0, 0, &min_found, &max_found);  
	scaled_dib = FreeImage_ConvertToStandardType(fft_dib, 1);

	CuAssertTrue(tc, fft_dib != NULL);
	CuAssertTrue(tc, scaled_dib != NULL);

	FreeImage_Unload(dib);
	FreeImage_Unload(fft_dib);
	
	ShowImage(scaled_dib);	

	FreeImage_Unload(scaled_dib);
}


CuSuite* CuGetFreeImageAlgorithmsFFTSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FFTFunctions);

	return suite;
}
