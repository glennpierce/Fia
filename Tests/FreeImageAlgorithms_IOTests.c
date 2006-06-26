#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFreeImageAlgorithms_IO(CuTest* tc)
{
	FIBITMAP *dib;
	int err;

	dib = FreeImage_AllocateT(FIT_BITMAP, 13000, 12000, 24, 0, 0, 0);

	CuAssertTrue(tc, dib != NULL);

	err = FreeImageAlgorithms_SaveFIBToFile (dib, "C:\\Documents and Settings\\Glenn\\Desktop\\test.jpg");

	CuAssertTrue(tc, err == FREEIMAGE_ALGORITHMS_SUCCESS);

	FreeImage_Unload(dib);
}



CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsIOSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_IO);
	
	return suite;
}
