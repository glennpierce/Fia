#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_LinearScale.h"

#include "FreeImageAlgorithms_Testing.h"


static void
TestFreeImageAlgorithms_LinearScaleTest(CuTest* tc)
{
	double min_found, max_found;

	char *file = TEST_IMAGE_DIRECTORY "\\8bit_test.png";

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *scaled_dib = FreeImageAlgorithms_LinearScaleToStandardType(dib, 0, 0, &min_found, &max_found);  

	FIBITMAP *dib2;

	FreeImage_Unload(dib);
	FreeImage_Unload(scaled_dib);

	CuAssertTrue(tc, min_found == 152.0);
	CuAssertTrue(tc, max_found == 240.0);

	dib = FreeImageAlgorithms_LoadFIBFromFile("C:\\Documents and Settings\\Pierce\\Desktop\\InfoTest.jpg");

	//ShowImage(dib);

	//FreeImageAlgorithms_FindMinMax(scaled_dib, &min_found, &max_found);

	//scaled_dib = FreeImage_Threshold(dib, 180);
	dib2 = FreeImage_ConvertTo8Bits(dib);

	scaled_dib = FreeImageAlgorithms_BinaryScaleToNewType(dib2, FIT_UINT16);
	
	//dib2 = FreeImage_ConvertTo8Bits(scaled_dib);
	ShowImage(scaled_dib);

	//FreeImageAlgorithms_FindMinMax(scaled_dib, &min_found, &max_found);

	//dib = FreeImage_ConvertToStandardType(scaled_dib, 1);

	//FreeImageAlgorithms_SaveFIBToFile( dib, "C:\\Documents and Settings\\Glenn\\Desktop\\bisto.jpg");

	//ShowImage(scaled_dib);

}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsLinearScaleSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_LinearScaleTest);

	return suite;
}
