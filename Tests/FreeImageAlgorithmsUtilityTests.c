

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "CuTest.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFreeImageAlgorithms_UtilityTest(CuTest* tc)
{
	double min, max;

	char *file = TEST_IMAGE_DIRECTORY "\\24bit_colour.jpg";

	FIBITMAP *src;

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	FreeImageAlgorithms_FindMinMaxForColourImage(dib, &min, &max);

	FreeImage_Unload(dib);

	CuAssertTrue(tc, min == 0.0);
	CuAssertTrue(tc, max == 255.0);

	dib = FreeImageAlgorithms_LoadFIBFromFile("C:\\Documents and Settings\\Glenn\\Desktop\\test.jpg");

	src = FreeImageAlgorithms_LoadFIBFromFile("C:\\Documents and Settings\\Glenn\\Desktop\\cool.jpg");

	FreeImageAlgorithms_SimplePaste(dib, src, 40, 40);

	FreeImageAlgorithms_SaveFIBToFile(dib, "C:\\Documents and Settings\\Glenn\\Desktop\\yes.jpg");

	FreeImage_Unload(dib);
	FreeImage_Unload(src);

}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsUtilitySuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_UtilityTest);

	return suite;
}
