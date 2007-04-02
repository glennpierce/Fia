#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "profile.h"

#include "CuTest.h"

#include "FreeImageAlgorithms_Testing.h"

#include <iostream>

#include "profile.h"


static void BorderTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river-gs.jpg";

	FIBITMAP *src = FreeImageAlgorithms_LoadFIBFromFile(file);

	ProfileStart("FreeImageAlgorithms_AddBorder");

	FIABITMAP *dst = FreeImageAlgorithms_SetBorder(src, 10, 10, BorderType_Copy, 255);

	ProfileStop("FreeImageAlgorithms_AddBorder");

	ProfilePrint();

	FreeImageAlgorithms_SaveFIBToFile(dst->fib, TEMP_DIR "\\border_test.bmp", BIT8);
	
	FreeImage_Unload(src);
	FreeImageAlgorithms_Unload(dst);
}


/*
static void
TestFreeImageAlgorithms_FindMinMaxTest(CuTest* tc)
{
	const int total = 1000000;

	float *data = (float*) _aligned_malloc(total * sizeof(float), 16);

	for(int i=0; i < total; i++)
		data[i] = rand();

	float min, max, min_fast, max_fast;

	FreeImageAlgorithms_FindFloatMinMax(data, total, &min, &max);
	FreeImageAlgorithms_SSEFindFloatMinMax(data, total, &min_fast, &max_fast);

	_aligned_free(data);

	CuAssertTrue(tc, min == min_fast);
	CuAssertTrue(tc, max == max_fast);
}
*/


static void
TestFreeImageAlgorithms_UtilityTest(CuTest* tc)
{
	double min, max;

	char *file = IMAGE_DIR "\\24bit_colour.jpg";

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	FreeImageAlgorithms_FindMinMaxForColourImage(dib, &min, &max);

	FreeImage_Unload(dib);

	CuAssertTrue(tc, min == 0.0);
	CuAssertTrue(tc, max == 255.0);
}

static void
TestFreeImageAlgorithms_UtilityCompareTest(CuTest* tc)
{
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


static void
TestFreeImageAlgorithms_DistanceTransformTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\distance-transform-test.gif";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	FIBITMAP *dib3 = FreeImageAlgorithms_DistanceTransform(dib2);

	CuAssertTrue(tc, dib3 != NULL);

	FreeImageAlgorithms_SaveFIBToFile(dib3, TEMP_DIR "\\distance-transform.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsUtilitySuite(void)
{
	CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, BorderTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_UtilityTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_UtilityCompareTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_DistanceTransformTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FindMinMaxTest);
	//SUITE_ADD_TEST(suite, BorderTest);
	
	return suite;
}
