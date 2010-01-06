#include "Constants.h"

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
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	FIABITMAP *dst = FIA_SetBorder(src, 10, 10, BorderType_Mirror, 255);

	FIA_SaveFIBToFile(dst->fib, TEST_DATA_OUTPUT_DIR "/Utility/border_test_result.bmp", BIT8);
	
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
TestFIA_MetaDataTest(CuTest* tc)
{
        const char *file = TEST_DATA_DIR "drone-bee.jpg";

        FIBITMAP *dib = FIA_LoadFIBFromFile(file);

	
	FITAG *tag = FreeImage_CreateTag();

	if(tag) {
		// fill the tag members
 		// note that the FIMD_XMP model accept a single key named “XMLPacket”
		char *value = "<GlennsStuff>Hmm</GlennsStuff>";
 		
		int ret;

		ret = FreeImage_SetTagKey(tag, "XMLPacket");
		std::cout << ret << std::endl;

		ret = FreeImage_SetTagLength(tag, strlen(value) + 1);
		std::cout << ret << std::endl;

		ret = FreeImage_SetTagCount(tag, strlen(value) + 1);
		std::cout << ret << std::endl;

		ret = FreeImage_SetTagType(tag, FIDT_ASCII);
		std::cout << ret << std::endl;
		
		// the tag value must be stored after
		// the tag data type, tag count and tag length have been filled.
		ret = FreeImage_SetTagValue(tag, value);
		std::cout << ret << std::endl;

		ret = FreeImage_SetMetadata(FIMD_XMP, dib, FreeImage_GetTagKey(tag), tag);
		std::cout << "Set MetaData: " << ret << std::endl;
	}


	int count = FreeImage_GetMetadataCount(FIMD_COMMENTS, dib);

	std::cout << "Count: " << count << std::endl;

	FreeImage_Save(FIF_JPEG, dib, TEST_DATA_OUTPUT_DIR "/Utility/metadata.jpg", 0);
	FreeImage_Save(FIF_PNG, dib, TEST_DATA_OUTPUT_DIR "/Utility/metadata.png", 0);
	//FIA_SaveFIBToFile(dib, TEST_DATA_OUTPUT_DIR "/Utility/metadata.png", BIT8);

	// destroy the tag
        FreeImage_DeleteTag(tag);

        FreeImage_Unload(dib);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsUtilitySuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/Utility");

    	SUITE_ADD_TEST(suite, BorderTest);
	SUITE_ADD_TEST(suite, TestFIA_UtilityTest);
	SUITE_ADD_TEST(suite, TestFIA_DistanceTransformTest);
	SUITE_ADD_TEST(suite, TestFIA_MetaDataTest);


	return suite;
}
