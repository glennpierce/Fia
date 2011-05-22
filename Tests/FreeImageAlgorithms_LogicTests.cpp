#include "CuTest.h"

#include "Constants.h"
#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Logic.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFIA_MaskTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";
	const char *mask_file = TEST_DATA_DIR "drone-bee-mask.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	FIBITMAP *mask = FIA_LoadFIBFromFile(mask_file);

	FIBITMAP *mask_8bit = FreeImage_ConvertTo8Bits(mask);
	FIA_SetGreyLevelPalette(mask_8bit);
	FIA_InPlaceThreshold(mask_8bit, 100, 255, 255);

	CuAssertTrue(tc, src != NULL);
	CuAssertTrue(tc, mask != NULL);
	CuAssertTrue(tc, mask_8bit != NULL);

	int err = FIA_MaskImage(src, mask_8bit);

	CuAssertTrue(tc, err != FIA_ERROR);

	FIA_SaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "Logic/dronebee-masked.jpg", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(mask);
	FreeImage_Unload(mask_8bit);
}

static void
TestFIA_LogicTest(CuTest* tc)
{
	const char *file1 = TEST_DATA_DIR "binary.bmp";
	const char *file2 = TEST_DATA_DIR "morphology_test.bmp";

	FIBITMAP *src1 = FIA_LoadFIBFromFile(file1);
	FIBITMAP *src2 = FIA_LoadFIBFromFile(file2);
	FIBITMAP *dst = NULL;

	CuAssertTrue(tc, src1 != NULL);
	CuAssertTrue(tc, src2 != NULL);

	FIA_SaveFIBToFile(src1, TEST_DATA_OUTPUT_DIR "Logic/src1.bmp", BIT8);
	FIA_SaveFIBToFile(src2, TEST_DATA_OUTPUT_DIR "Logic/src2.bmp", BIT8);

	dst = FIA_BinaryOr (src1, src2, 0);
	CuAssertTrue(tc, dst != NULL);
	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "Logic/Or.bmp", BIT8);

	dst = FIA_BinaryOr (src1, src2, 1);
	CuAssertTrue(tc, dst != NULL);
	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "Logic/Nor.bmp", BIT8);

	dst = FIA_BinaryAnd (src1, src2, 0);
	CuAssertTrue(tc, dst != NULL);
	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "Logic/And.bmp", BIT8);

	dst = FIA_BinaryAnd (src1, src2, 1);
	CuAssertTrue(tc, dst != NULL);
	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "Logic/Nand.bmp", BIT8);

	FreeImage_Unload(src1);
	FreeImage_Unload(src2);
	FreeImage_Unload(dst);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsLogicSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/Logic");

	//SUITE_ADD_TEST(suite, TestFIA_MaskTest);
	SUITE_ADD_TEST(suite, TestFIA_LogicTest);

	return suite;
}
