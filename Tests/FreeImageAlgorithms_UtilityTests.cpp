#include "Constants.h"

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "profile.h"

#include "CuTest.h"

#include "FreeImageAlgorithms_Testing.h"

#include <iostream>

#include "profile.h"


static void BorderTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	FIABITMAP *dst = FIA_SetBorder(src, 50, 50, BorderType_Mirror, 255);

	FIA_SaveFIBToFile(dst->fib, TEST_DATA_OUTPUT_DIR "/Utility/border_test_result.bmp", BIT8);
	
	FreeImage_Unload(src);
	FIA_Unload(dst);
}


static void BorderTest2(CuTest* tc)
{
	int err;
	FIARECT rect;

	FIBITMAP *dst = FreeImage_Allocate(10, 3, 8, 0, 0, 0);
	FIBITMAP *src = FreeImage_Allocate(8, 1, 8, 0, 0, 0);

	rect.left = 0;
	rect.top = 0;
	rect.right = 8;
	rect.bottom = 1;

	FIA_DrawSolidGreyscaleRect(src, rect, 255);
	
	err = FIA_SimplePaste(dst, src, 1, 1);

	CuAssertTrue(tc, err == FIA_SUCCESS);

	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/border2_test_result.bmp", BIT8);
	
	FreeImage_Unload(src);
	FreeImage_Unload(dst);
}

/*
static void
TestFIA_FindMinMaxTest(CuTest* tc)
{
	const int total = 1000000;

	float *data = (float*) _aligned_malloc(total * sizeof(float), 16);

	for(int i=0; i < total; i++)
		data[i] = rand();

	float min, max, min_fast, max_fast;

	FIA_FindFloatMinMax(data, total, &min, &max);
	FIA_SSEFindFloatMinMax(data, total, &min_fast, &max_fast);

	_aligned_free(data);

	CuAssertTrue(tc, min == min_fast);
	CuAssertTrue(tc, max == max_fast);
}
*/


static void
TestFIA_UtilityTest(CuTest* tc)
{
	double min, max;

	const char *file = TEST_DATA_DIR "drone-bee.jpg";

	FIBITMAP *dib = FIA_LoadFIBFromFile(file);
	
	FIA_FindMinMaxForColourImage(dib, &min, &max);

	FreeImage_Unload(dib);

	CuAssertTrue(tc, min == 0.0);
	CuAssertTrue(tc, max == 255.0);
}

/*
static void
TestFIA_UtilityCompareTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee.jpg";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	FIBITMAP *dib2 = FIA_LoadFIBFromFile(file);
	FIBITMAP *dib3 = FIA_LoadFIBFromFile(IMAGE_DIR "\\24bit_colour_slight_change.jpg");

	int res = FIA_BitwiseCompare(dib1, dib2);

	CuAssertTrue(tc, res == 1);

	res = FIA_BitwiseCompare(dib1, dib3);

	CuAssertTrue(tc, res == 0);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}
*/

static void
TestFIA_DistanceTransformTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	FIBITMAP *dib3 = FIA_DistanceTransform(dib2);

	CuAssertTrue(tc, dib3 != NULL);

	FIA_SaveFIBToFile(dib3, TEST_DATA_OUTPUT_DIR "/Utility/distance-transform.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}

static void
TestFIA_DistanceTransformTest2(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "mask.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	FIBITMAP *dib3 = FIA_DistanceTransform(dib2);

	CuAssertTrue(tc, dib3 != NULL);

	FIA_SaveFIBToFile(dib3, TEST_DATA_OUTPUT_DIR "/Utility/distance-transform2.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}

static void PasteTest(CuTest* tc)
{
	const char *file1 = TEST_DATA_DIR "drone-bee-greyscale.jpg";
	const char *file2 = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file1);
	FIBITMAP *dst = FIA_LoadFIBFromFile(file2);

	FIA_SimplePaste(dst, src, -100, -100);
	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/paste_result.bmp", BIT8);
	
	FreeImage_Unload(src);
	FreeImage_Unload(dst);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsUtilitySuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/Utility");

    //SUITE_ADD_TEST(suite, BorderTest);
	//SUITE_ADD_TEST(suite, BorderTest2);

	//SUITE_ADD_TEST(suite, TestFIA_UtilityTest);
	SUITE_ADD_TEST(suite, TestFIA_DistanceTransformTest);
	SUITE_ADD_TEST(suite, TestFIA_DistanceTransformTest2);
	//SUITE_ADD_TEST(suite, PasteTest);

	return suite;
}
