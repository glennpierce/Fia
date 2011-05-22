#include "Constants.h"

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utils.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_LinearScale.h"
#include "FreeImageAlgorithms_Logic.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "profile.h"

#include "CuTest.h"

#include "FreeImageAlgorithms_Testing.h"

#include <iostream>

#include "profile.h"


static void LineTest(CuTest* tc)
{
	const char *file = "C:\\test.png";
	FIAPOINT p1, p2;
	int len;
	unsigned short values[2000];

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	
	p1 = MakeFIAPoint(500, 0);
	p2 = MakeFIAPoint(500, 1023);

	len = FIA_GetUShortPixelValuesForLine(src, p1, p2, values);

	FreeImage_Unload(src);
}


static void BorderTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	FIABITMAP *dst = FIA_SetBorder(src, 50, 50, BorderType_Mirror, 255);

	FIA_SaveFIBToFile(dst->fib, TEST_DATA_OUTPUT_DIR "/Utility/border_test_result.bmp", BIT8);
	
	FreeImage_Unload(src);
	FIA_Unload(dst);
}

/*
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
*/

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

/*
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
*/

static void ConvertFloatToTypeTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	FIBITMAP *dst;

	FIA_InPlaceConvertToGreyscaleFloatType(&src, FIT_FLOAT);

	FIA_AddGreyLevelImageConstant(src, 20000);
	
	FIA_SimpleSaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "/Utility/converted_original_float.tif");
	
	dst = FIA_ConvertFloatTypeToType(src, FIT_INT16, 0);
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(dst);
	CuAssertTrue(tc, type == FIT_INT16);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/converted_original_int16.tif");
	FreeImage_Unload(dst);

	///////////////////////////////
	dst = FIA_ConvertFloatTypeToType(src, FIT_INT16, 1);
	type = FreeImage_GetImageType(dst);
	CuAssertTrue(tc, type == FIT_INT16);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/converted_original_int16_scaled.tif");
	FreeImage_Unload(dst);

	////////////////////////////////////
	dst = FIA_ConvertFloatTypeToType(src, FIT_UINT16, 0);
	type = FreeImage_GetImageType(dst);
	CuAssertTrue(tc, type == FIT_UINT16);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/converted_original_uint16.tif");
	FreeImage_Unload(dst);

	//////////////////////////////////////
	dst = FIA_ConvertFloatTypeToType(src, FIT_UINT16, 1);
	type = FreeImage_GetImageType(dst);
	CuAssertTrue(tc, type == FIT_UINT16);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/converted_original_uint16_scaled.tif");
	FreeImage_Unload(dst);

	dst = FIA_ConvertFloatTypeToType(src, FIT_INT32, 0);
	type = FreeImage_GetImageType(dst);
	CuAssertTrue(tc, type == FIT_INT32);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/converted_original_int32.tif");

	double min_within_image, max_within_image;
	FIA_InplaceLinearScaleToStandardType (&dst, 0.0, 0.0, &min_within_image, &max_within_image);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/LinearScaleToStandardType.tif");
	FreeImage_Unload(dst);


	dst = FIA_ConvertFloatTypeToType(src, FIT_INT32, 1);
	type = FreeImage_GetImageType(dst);
	CuAssertTrue(tc, type == FIT_INT32);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/converted_original_int32_scaled.tif");
	FreeImage_Unload(dst);


	FreeImage_Unload(src);
}

static void CopyTest1(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "CopyTest.tif";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	FIBITMAP *dst = FIA_FastCopy(src, 1, 1, FreeImage_GetWidth(src) - 1, FreeImage_GetHeight(src) - 1);

	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/fastcopy_result.tif");
	
	FreeImage_Unload(src);
	FreeImage_Unload(dst);
}

static void HatchImageTest(CuTest* tc)
{
	FIBITMAP *dst = FIA_MakeHatchedImage (256, 256, 1, 10);
	FIA_SetBinaryPalette(dst);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/hatch1.bmp");
	FreeImage_Unload(dst);

	dst = FIA_MakeHatchedImage (256, 256, 2, 10);
	FIA_SetBinaryPalette(dst);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/hatch2.bmp");
	FreeImage_Unload(dst);

	dst = FIA_MakeHatchedImage (256, 256, 3, 50);
	FIA_SetBinaryPalette(dst);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/hatch3.bmp");
	FreeImage_Unload(dst);

	dst = FIA_MakeHatchedImage (256, 256, 4, 20);
	FIA_SetBinaryPalette(dst);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/hatch4.bmp");
	FreeImage_Unload(dst);

	dst = FIA_MakeHatchedImage (256, 256, 5, 15);
	FIA_SetBinaryPalette(dst);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/hatch5.bmp");
	FreeImage_Unload(dst);

	dst = FIA_MakeHatchedImage (256, 256, 6, 13);
	FIA_SetBinaryPalette(dst);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/hatch6.bmp");
	FreeImage_Unload(dst);

	dst = FIA_MakeHatchedImage (256, 256, 7, 5);
	FIA_SetBinaryPalette(dst);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/hatch7.bmp");
	FreeImage_Unload(dst);

}


static void CopyTest(CuTest* tc)
{
        const char *file = TEST_DATA_DIR "fedora.jpg";

        FIBITMAP *src = FIA_LoadFIBFromFile(file);

        FIBITMAP *src2 = NULL; 

	int width =  FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
	int i;

	PROFILE_START("FreeImage_Copy");

	for(i=0; i < 100; i++) {
		src2 = FreeImage_Copy(src, 1, 1, width - 1, height - 1);
		FreeImage_Unload(src2);
	}

	PROFILE_STOP("FreeImage_Copy");

        FreeImage_Unload(src);
}
static void CopyTestRect(CuTest* tc)
{
        const char *file = TEST_DATA_DIR "test.bmp";

        FIBITMAP *src = FIA_LoadFIBFromFile(file);

        FIBITMAP *src2 = NULL; 

	int width =  FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
	int i;

	PROFILE_START("FreeImage_Copy");

	src2 = FIA_CopyLeftTopWidthHeight (src, 64, 64, 128, 128);

	PROFILE_STOP("FreeImage_Copy");

	FIA_SimpleSaveFIBToFile(src2, TEST_DATA_OUTPUT_DIR "/Utility/CopyLeftTopWidthHeight.bmp");

    FreeImage_Unload(src);
    FreeImage_Unload(src2);
}

static void FastCopyTest(CuTest* tc)
{
        const char *file = TEST_DATA_DIR "fedora.jpg";

        FIBITMAP *src = FIA_LoadFIBFromFile(file);

        FIBITMAP *src2 = NULL;

        int width =  FreeImage_GetWidth(src);
        int height = FreeImage_GetHeight(src);
        int i;

        PROFILE_START("FIA_FastCopy");

        for(i=0; i < 100; i++) {
                src2 = FIA_FastCopy(src, 1, 1, width - 1, height - 1);
                FreeImage_Unload(src2);
        }

        PROFILE_STOP("FIA_FastCopy");

        FreeImage_Unload(src);
}

static void AlphaCombineTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "test.bmp";
	const char *file2 = TEST_DATA_DIR "test_mask.bmp";

	double opacity = 0.5;
	int hatchType = 7;

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	FIBITMAP *mask = FIA_LoadFIBFromFile(file2);
	FIBITMAP *hatch = FIA_MakeHatchedImage (FreeImage_GetWidth(src), FreeImage_GetHeight(src), hatchType, 5);
	FIBITMAP *revhatch = NULL;
	FIBITMAP *border = FIA_BinaryInnerBorder(mask);

	// bg to show through where alpha is low
	FIA_Add(hatch, border);
	FIA_MultiplyConst(hatch, 255);

	// alpha channel
	revhatch = FreeImage_Clone(hatch);
	FIA_ReverseMaskImage(revhatch, 255);
	FIBITMAP *imfloat = FreeImage_ConvertToType(revhatch, FIT_FLOAT, 0);  // convert to float normalises 0-255 to 0.0-1.0
	FIA_MultiplyConst(imfloat, (1.0-opacity));

	FIBITMAP *dst = FIA_Composite(src, hatch, imfloat, mask);

	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/composite.bmp");

	FreeImage_Unload(hatch);
	FreeImage_Unload(mask);
	FreeImage_Unload(src);
	FreeImage_Unload(revhatch);
	FreeImage_Unload(border);
}

static void BlendMaskWithImageTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "test.bmp";
	const char *file2 = TEST_DATA_DIR "test_mask.bmp";

	double opacity = 0.5;
	int hatchType = 7;
	RGBQUAD col;

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	FIBITMAP *mask = FIA_LoadFIBFromFile(file2);

	col.rgbRed = 100;
	col.rgbGreen = 2;
	col.rgbBlue = 230;
//	col.rgbRed = 0;
//	col.rgbGreen = 255;
//	col.rgbBlue = 0;

//	FIA_InPlaceConvertTo24Bit(&src);  // colour image test
	FIA_SetRainBowPalette(src);  // palette test

	FIBITMAP *dst = FIA_BlendMaskWithImage(mask, src, col, 1, 5, 3);

	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Utility/blendMask.bmp");

	FreeImage_Unload(mask);
	FreeImage_Unload(src);
}

static void
TestFIA_ThresholdTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "test.bmp";

	FIBITMAP *dib = FIA_LoadFIBFromFile(file);
	FIBITMAP *dst = FIA_Threshold(dib, 3.7, 1000000.0, 128.5); 

	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "Utility/threshold.bmp");

	FreeImage_Unload(dib);
	FreeImage_Unload(dst);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsUtilitySuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/Utility");

//	SUITE_ADD_TEST(suite, CopyTest);
	SUITE_ADD_TEST(suite, CopyTestRect);
	//SUITE_ADD_TEST(suite, FastCopyTest);
	//SUITE_ADD_TEST(suite, HatchImageTest);
	//SUITE_ADD_TEST(suite, AlphaCombineTest);
	//SUITE_ADD_TEST(suite, ConvertFloatToTypeTest);
	//SUITE_ADD_TEST(suite, BlendMaskWithImageTest);
    //SUITE_ADD_TEST(suite, TestFIA_ThresholdTest);


    //SUITE_ADD_TEST(suite, BorderTest);
	//SUITE_ADD_TEST(suite, ConvertFloatToTypeTest);
    //SUITE_ADD_TEST(suite, BorderTest);
	//SUITE_ADD_TEST(suite, BorderTest2);

	//SUITE_ADD_TEST(suite, TestFIA_UtilityTest);
	//SUITE_ADD_TEST(suite, LineTest);
	//SUITE_ADD_TEST(suite, TestFIA_DistanceTransformTest2);
	//SUITE_ADD_TEST(suite, PasteTest);

	return suite;
}
