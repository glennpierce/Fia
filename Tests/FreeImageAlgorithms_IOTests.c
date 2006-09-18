#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"


#include "FreeImageAlgorithms_Testing.h"

static void DLL_CALLCONV
TestFreeImageAlgorithms_IO(CuTest* tc)
{
	FIBITMAP *dib1, *dib2;
	FREE_IMAGE_TYPE type;
	int bpp, err;

	dib1 = FreeImageAlgorithms_LoadFIBFromFile(IMAGE_DIR "\\colour_lines.png");

	CuAssertTrue(tc, dib1 != NULL);

	bpp = FreeImage_GetBPP(dib1);
	type = FreeImage_GetImageType(dib1);

	CuAssertTrue(tc, bpp == 24);
	CuAssertTrue(tc, type == FIT_BITMAP);
	
	err = FreeImageAlgorithms_SaveFIBToFile (dib1, "C:\\temp\\colour_lines_test.bmp", 1);

	CuAssertTrue(tc, err == FREEIMAGE_ALGORITHMS_SUCCESS);

	dib2 = FreeImageAlgorithms_LoadFIBFromFile("C:\\temp\\colour_lines_test.bmp");

	err = FreeImageAlgorithms_BitwiseCompare(dib1, dib2);

	CuAssertTrue(tc, err == 1);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
}



CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsIOSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_IO);
	
	return suite;
}
