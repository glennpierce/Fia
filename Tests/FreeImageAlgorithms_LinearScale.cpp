#include "CuTest.h"

#include "Constants.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_LinearScale.h"

#include "FreeImageAlgorithms_Testing.h"


static void
TestFIA_LinearScaleTest(CuTest* tc)
{
	double min_found, max_found;

	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *old_dib = FIA_LoadFIBFromFile(file);
	
//    FIBITMAP *dib = FreeImage_ConvertToType(old_dib, FIT_INT16, 1);
    FIBITMAP *dib = FreeImage_Clone(old_dib);

    PROFILE_START("LinearScale");

    FIBITMAP *scaled_dib;

//    scaled_dib = FIA_LinearScaleToStandardType(dib, 0, 100.00, &min_found, &max_found); 
//    scaled_dib = FIA_LinearScaleToStandardType(dib, 196.74, 240.35, &min_found, &max_found); // some float values cause over- or underload
//    scaled_dib = FIA_LinearScaleToStandardType(dib, 10.98, 255.00, &min_found, &max_found); // some float values cause over- or underload
    scaled_dib = FIA_LinearScaleToStandardType(dib, 92.73, 172.03, &min_found, &max_found); // some float values cause over- or underload

//	scaled_dib = FIA_LinearScaleToStandardType(dib, 0, 0, &min_found, &max_found); // some float values cause over- or underload
//	printf("FIA_LinearScaleToStandardType: min_found %f, max_found %f\n", min_found, max_found);

    PROFILE_STOP("LinearScale");


    FIA_SaveFIBToFile(scaled_dib,  TEST_DATA_OUTPUT_DIR "/LinearScale/drone-bee-linear-scaled.jpg", BIT8);

    FreeImage_Unload(scaled_dib);
	FreeImage_Unload(dib);
}

static void
TestFIA_LinearScaleRangeTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *dib = FIA_LoadFIBFromFile(file);
	FIBITMAP *scaled_dib = FIA_StretchImageAcrossRange(dib, 200, 255); 

    FIA_SaveFIBToFile(scaled_dib,
        TEST_DATA_OUTPUT_DIR "/LinearScale/drone-bee-linear-range-scaled.jpg", BIT8);

	FreeImage_Unload(dib);
	FreeImage_Unload(scaled_dib);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsLinearScaleSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/LinearScale");

	SUITE_ADD_TEST(suite, TestFIA_LinearScaleTest);
    SUITE_ADD_TEST(suite, TestFIA_LinearScaleRangeTest);

	return suite;
}
