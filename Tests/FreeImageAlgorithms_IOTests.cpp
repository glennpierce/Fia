#include "CuTest.h"

#include "Constants.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFIA_IO(CuTest* tc)
{
	FIBITMAP *dib1 = NULL, *dib2 = NULL;
	FREE_IMAGE_TYPE type;
	int bpp, err;
    
    const char *file = TEST_DATA_DIR "drone-bee.jpg";
	dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	bpp = FreeImage_GetBPP(dib1);
	type = FreeImage_GetImageType(dib1);

	CuAssertTrue(tc, bpp == 24);
	CuAssertTrue(tc, type == FIT_BITMAP);
	
	err = FIA_SaveFIBToFile (dib1, TEST_DATA_OUTPUT_DIR "save-colour-test.bmp", BIT8);

	CuAssertTrue(tc, err == FREEIMAGE_ALGORITHMS_SUCCESS);

	dib2 = FIA_LoadFIBFromFile(TEST_DATA_OUTPUT_DIR "save-colour-test.bmp");

	err = FIA_BitwiseCompare(dib1, dib2);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
}



CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsIOSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFIA_IO);
	
	return suite;
}
