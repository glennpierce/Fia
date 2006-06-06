#include "CuTest.h"

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Statistics.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFreeImageAlgorithms_StatisticTest(CuTest* tc)
{
	unsigned long histR[256], histG[256], histB[256];

	char *file = TEST_IMAGE_DIRECTORY "\\24bit_colour.jpg";

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	FreeImageAlgorithms_RGBHistogram(dib, 0, 255, 255, histR, histG, histB); 

	FreeImage_Unload(dib);

	//CuAssertTrue(tc, min == 0.0);
	//CuAssertTrue(tc, max == 255.0);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsStatisticSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_StatisticTest);

	return suite;
}
