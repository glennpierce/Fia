#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_LinearScale.h"

#include "FreeImageAlgorithms_Testing.h"


static void
TestFreeImageAlgorithms_LinearScaleTest(CuTest* tc)
{
	double min_found, max_found;

	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\Working Area\\Test Images\\cells.bmp";

	FIBITMAP *old_dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
    FIBITMAP *dib = FreeImage_ConvertToType(old_dib, FIT_INT16, 1);

    FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);

    PROFILE_START("LinearScale");

    FIBITMAP *scaled_dib;

	//for(int i=0; i < 1000; i++) {
		
        scaled_dib = FreeImageAlgorithms_LinearScaleToStandardType(dib, 0, 100, &min_found, &max_found);  

       
  //      if(i < 999)
//		    FreeImage_Unload(scaled_dib);
//	}

    PROFILE_STOP("LinearScale");


    FreeImageAlgorithms_SaveFIBToFile(scaled_dib, "C:\\Documents and Settings\\Pierce\\Desktop\\output.bmp", BIT8);

    FreeImage_Unload(scaled_dib);
	FreeImage_Unload(dib);
	//FreeImage_Unload(scaled_dib);

	//CuAssertTrue(tc, min_found == 152.0);
	//CuAssertTrue(tc, max_found == 240.0);
}

static void
TestFreeImageAlgorithms_LinearScaleRangeTest(CuTest* tc)
{
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
