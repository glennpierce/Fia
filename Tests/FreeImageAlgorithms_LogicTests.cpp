#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Logic.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFIA_MaskTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river-gs.jpg";
	char *mask_file = IMAGE_DIR "\\river-mask.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	FIBITMAP *mask = FIA_LoadFIBFromFile(mask_file);

	FIBITMAP *mask_8bit = FreeImage_ConvertTo8Bits(mask);
	FIA_SetGreyLevelPalette(mask_8bit);
	FIA_InPlaceThreshold(mask_8bit, 100, 255, 255);

	CuAssertTrue(tc, src != NULL);
	CuAssertTrue(tc, mask != NULL);
	CuAssertTrue(tc, mask_8bit != NULL);

	int err = FIA_MaskImage(src, mask_8bit);

	CuAssertTrue(tc, err != FREEIMAGE_ALGORITHMS_ERROR);

	FIA_SaveFIBToFile(src, TEMP_DIR "\\wallpaper_masked.jpg", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(mask);
	FreeImage_Unload(mask_8bit);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsLogicSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFIA_MaskTest);

	return suite;
}
