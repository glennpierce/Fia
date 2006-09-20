

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "CuTest.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFreeImageAlgorithms_UtilityTest(CuTest* tc)
{
	double min, max;

	char *file = IMAGE_DIR "\\24bit_colour.jpg";

	FIBITMAP *src;

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	FreeImageAlgorithms_FindMinMaxForColourImage(dib, &min, &max);

	FreeImage_Unload(dib);

	CuAssertTrue(tc, min == 0.0);
	CuAssertTrue(tc, max == 255.0);
}

static void
TestFreeImageAlgorithms_UtilityCompareTest(CuTest* tc)
{
	double min, max;

	char *file = IMAGE_DIR "\\24bit_colour.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *dib2 = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *dib3 = FreeImageAlgorithms_LoadFIBFromFile(IMAGE_DIR "\\24bit_colour_slight_change.jpg");

	int res = FreeImageAlgorithms_BitwiseCompare(dib1, dib2);

	CuAssertTrue(tc, res == 1);

	res = FreeImageAlgorithms_BitwiseCompare(dib1, dib3);

	CuAssertTrue(tc, res == 0);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}



CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsUtilitySuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_UtilityTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_UtilityCompareTest);

	return suite;
}
