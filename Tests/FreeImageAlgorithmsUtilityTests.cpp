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
	const char *file = IMAGE_DIR "\\border_test.bmp";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	PROFILE_START("FreeImageAlgorithms_AddBorder");

	FIABITMAP *dst = FIA_SetBorder(src, 10, 10, BorderType_Mirror, 255);

	PROFILE_STOP("FreeImageAlgorithms_AddBorder");

	PROFILE_PRINT();

	FIA_SaveFIBToFile(dst->fib, TEMP_DIR "\\border_test_result.bmp", BIT8);
	
	FreeImage_Unload(src);
	FIA_Unload(dst);
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

	const char *file = IMAGE_DIR "\\24bit_colour.jpg";

	FIBITMAP *dib = FIA_LoadFIBFromFile(file);
	
	FIA_FindMinMaxForColourImage(dib, &min, &max);

	FreeImage_Unload(dib);

	CuAssertTrue(tc, min == 0.0);
	CuAssertTrue(tc, max == 255.0);
}

static void
TestFIA_UtilityCompareTest(CuTest* tc)
{
	const char *file = IMAGE_DIR "\\24bit_colour.jpg";

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


static void
TestFIA_DistanceTransformTest(CuTest* tc)
{
	const char *file = IMAGE_DIR "\\distance-transform-test.gif";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	FIBITMAP *dib3 = FIA_DistanceTransform(dib2);

	CuAssertTrue(tc, dib3 != NULL);

	FIA_SaveFIBToFile(dib3, TEMP_DIR "\\distance-transform.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsUtilitySuite(void)
{
	CuSuite* suite = CuSuiteNew();

    	SUITE_ADD_TEST(suite, BorderTest);
	SUITE_ADD_TEST(suite, TestFIA_UtilityTest);
	SUITE_ADD_TEST(suite, TestFIA_UtilityCompareTest);
	SUITE_ADD_TEST(suite, TestFIA_DistanceTransformTest);
	//SUITE_ADD_TEST(suite, TestFIA_FindMinMaxTest);
	//SUITE_ADD_TEST(suite, BorderTest);
	
	return suite;
}
