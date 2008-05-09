#include "CuTest.h"

#include "Constants.h"

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
Test_FFT(CuTest* tc)
{
	FIBITMAP *fft_dib, *real_dib, *log_dib, *scaled_dib;
	const char *file = TEST_DATA_DIR "bumblebee.jpg";
	
	FIBITMAP *dib = FIA_LoadFIBFromFile(file);	
    CuAssertTrue(tc, dib != NULL);

	FIBITMAP *greyscale_dib = FreeImage_ConvertToGreyscale(dib);
    CuAssertTrue(tc, greyscale_dib != NULL);

	fft_dib = FIA_FFT(greyscale_dib, 0, 1);  
    CuAssertTrue(tc, fft_dib != NULL);

	real_dib = FIA_ConvertComplexImageToAbsoluteValued(fft_dib);
    CuAssertTrue(tc, real_dib != NULL);

	log_dib = FIA_Log(real_dib);
    CuAssertTrue(tc, log_dib != NULL);

    scaled_dib = FreeImage_ConvertToStandardType(log_dib, 1);

    FIA_SetRainBowPalette(scaled_dib);
    
	FIA_SaveFIBToFile(scaled_dib, TEST_DATA_OUTPUT_DIR "fft.jpg", BIT8);

	FreeImage_Unload(dib);
	FreeImage_Unload(greyscale_dib);
	FreeImage_Unload(real_dib);
	FreeImage_Unload(fft_dib);
	FreeImage_Unload(log_dib);
	FreeImage_Unload(scaled_dib);
}

CuSuite* 
DLL_CALLCONV CuGetFreeImageAlgorithmsFFTSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Test_FFT);

	return suite;
}
