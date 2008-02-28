#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_LinearScale.h"

#include "FreeImageAlgorithms_Testing.h"


static void
TestFIA_LinearScaleTest(CuTest* tc)
{
	double min_found, max_found;

	const char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\Working Area\\Test Images\\cells.bmp";

	FIBITMAP *old_dib = FIA_LoadFIBFromFile(file);
	
    FIBITMAP *dib = FreeImage_ConvertToType(old_dib, FIT_INT16, 1);

    FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);

    PROFILE_START("LinearScale");

    FIBITMAP *scaled_dib;

    scaled_dib = FIA_LinearScaleToStandardType(dib, 0, 100, &min_found, &max_found);  

    PROFILE_STOP("LinearScale");


    FIA_SaveFIBToFile(scaled_dib, "C:\\Documents and Settings\\Pierce\\Desktop\\output.bmp", BIT8);

    FreeImage_Unload(scaled_dib);
	FreeImage_Unload(dib);
}

static void
TestFIA_LinearScaleRangeTest(CuTest* tc)
{
	const char *file = IMAGE_DIR "\\8bit_test.png";

	FIBITMAP *dib = FIA_LoadFIBFromFile(file);
	FIBITMAP *scaled_dib = FIA_StretchImageAcrossRange(dib, 200, 255); 

    FIA_SaveFIBToFile(scaled_dib,
        TEMP_DIR "\\scaled_range.bmp", BIT8);

	FreeImage_Unload(dib);
	FreeImage_Unload(scaled_dib);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsLinearScaleSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFIA_LinearScaleTest);
    //SUITE_ADD_TEST(suite, TestFIA_LinearScaleRangeTest);

	return suite;
}
