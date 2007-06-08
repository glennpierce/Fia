#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_LinearScale.h"

#include "FreeImageAlgorithms_Testing.h"


static void
TestFreeImageAlgorithms_LinearScaleTest(CuTest* tc)
{
	double min_found, max_found;

	char *file = IMAGE_DIR "\\test.jpg";

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	for(int i=0; i < 1000; i++) {
		FIBITMAP *scaled_dib = FreeImageAlgorithms_LinearScaleToStandardType(dib, 0, 255, &min_found, &max_found);  
		FreeImage_Unload(scaled_dib);
	}
	FreeImage_Unload(dib);
	//FreeImage_Unload(scaled_dib);

	//CuAssertTrue(tc, min_found == 152.0);
	//CuAssertTrue(tc, max_found == 240.0);
}

static void
TestFreeImageAlgorithms_LinearScaleRangeTest(CuTest* tc)
{
	double min_found, max_found;

	char *file = IMAGE_DIR "\\8bit_test.png";

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *scaled_dib = FreeImageAlgorithms_StretchImageAcrossRange(dib, 200, 255); 

    FreeImageAlgorithms_SaveFIBToFile(scaled_dib,
        TEMP_DIR "\\scaled_range.bmp", BIT8);

	FreeImage_Unload(dib);
	FreeImage_Unload(scaled_dib);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsLinearScaleSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_LinearScaleTest);
    //SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_LinearScaleRangeTest);

	return suite;
}
