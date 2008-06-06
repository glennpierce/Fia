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
Test_Shift(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "points.bmp";
	
	FIBITMAP *dib = FIA_LoadFIBFromFile(file);	
    CuAssertTrue(tc, dib != NULL);
	
	FIBITMAP *ddib = FreeImage_ConvertToType(dib, FIT_DOUBLE, 0);

	FIBITMAP* shifted_dib = FIA_ShiftImageEdgeToCenter(ddib);
	CuAssertTrue(tc, shifted_dib != NULL);

	FIA_InPlaceConvertToStandardType(&shifted_dib, 1);

	FIA_SaveFIBToFile(shifted_dib, TEST_DATA_OUTPUT_DIR "/FFT/shift.bmp", BIT8);

	FreeImage_Unload(shifted_dib);
	FreeImage_Unload(dib);
}


static void
Test_FFT(CuTest* tc)
{
	FIBITMAP *fft_dib, *real_dib, *log_dib;
	const char *file = TEST_DATA_DIR "bumblebee.jpg";
	
	FIBITMAP *dib = FIA_LoadFIBFromFile(file);	
    CuAssertTrue(tc, dib != NULL);

	FIBITMAP *greyscale_dib = FreeImage_ConvertToGreyscale(dib);
    CuAssertTrue(tc, greyscale_dib != NULL);

	fft_dib = FIA_FFT(greyscale_dib);  
    CuAssertTrue(tc, fft_dib != NULL);

	real_dib = FIA_ConvertComplexImageToAbsoluteValuedSquared(fft_dib);
    CuAssertTrue(tc, real_dib != NULL);

	FIA_InPlaceShiftImageEdgeToCenter(&real_dib);

	log_dib = FIA_Log(real_dib);
    CuAssertTrue(tc, log_dib != NULL);

    FIA_InPlaceConvertToStandardType(&log_dib, 1);
    FIA_SetRainBowPalette(log_dib);
	FIA_SaveFIBToFile(log_dib, TEST_DATA_OUTPUT_DIR "/FFT/fft.png", BIT8);

	FreeImage_Unload(dib);
	FreeImage_Unload(greyscale_dib);
	FreeImage_Unload(real_dib);
	FreeImage_Unload(fft_dib);
	FreeImage_Unload(log_dib);
}

CuSuite* 
DLL_CALLCONV CuGetFreeImageAlgorithmsFFTSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/FFT");

	SUITE_ADD_TEST(suite, Test_Shift);
	SUITE_ADD_TEST(suite, Test_FFT);

	return suite;
}
