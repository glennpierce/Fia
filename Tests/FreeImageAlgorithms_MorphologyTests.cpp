#include "CuTest.h"

#include "Constants.h"
#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Morphology.h"

#include <iostream>
#include <fstream>

static double kernel_values[] = {1.0, 1.0, 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0, 1.0, 1.0};


static void
TestFIA_DilationTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "\\morpholology_test.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);

	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	FIABITMAP *border_dib = FIA_SetBorder(threshold_8bit_dib, 2, 2
        , BorderType_Constant, 0.0);

	PROFILE_START("DilationFilter");

	FilterKernel kernel = FIA_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FIA_BinaryDilation(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

	PROFILE_STOP("DilationFilter");

	FIA_SaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "\\dilation_result.jpg", BIT24);

	result_dib = FIA_BinaryOuterBorder(threshold_8bit_dib);

	FIA_SimpleSaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "morphology/outer_border_result.bmp");

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FIA_Unload(border_dib);
	FreeImage_Unload(result_dib);
}

static void
TestFIA_ErosionTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "\\morpholology_test.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);

	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	FIABITMAP *border_dib = FIA_SetBorder(threshold_8bit_dib, 2, 2
        , BorderType_Constant, 0.0);

	PROFILE_START("ErosionFilter");

	FilterKernel kernel = FIA_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FIA_BinaryErosion(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

	PROFILE_STOP("ErosionFilter");

	FIA_SaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "\\erosion_result.jpg", BIT24);

	result_dib = FIA_BinaryInnerBorder(threshold_8bit_dib);

	FIA_SimpleSaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "morphology/inner_border_result.bmp");

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FIA_Unload(border_dib);
	FreeImage_Unload(result_dib);
}

static void
TestFIA_OpeningTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "\\morpholology_test.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);

	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	FIABITMAP *border_dib = FIA_SetBorder(threshold_8bit_dib, 2, 2
        , BorderType_Constant, 0.0);

	FilterKernel kernel = FIA_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FIA_BinaryOpening(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

	FIA_SaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "\\opening_result.jpg", BIT24);

	// Test of convinience 3x3 function
	result_dib = FIA_Binary3x3Opening(threshold_8bit_dib);

	FIA_SimpleSaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "morphology/opening3x3_result.bmp");

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FIA_Unload(border_dib);
	FreeImage_Unload(result_dib);
}

static void
TestFIA_ClosingTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "\\morpholology_test.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);

	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	FIABITMAP *border_dib = FIA_SetBorder(threshold_8bit_dib, 2, 2
        , BorderType_Constant, 0.0);

	FilterKernel kernel = FIA_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FIA_BinaryClosing(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

	FIA_SaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "\\closing_result.jpg", BIT24);

	// Test of convinience 3x3 function
	result_dib = FIA_Binary3x3Closing(threshold_8bit_dib);

	FIA_SimpleSaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "morphology/closing3x3_result.bmp");

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FIA_Unload(border_dib);
	FreeImage_Unload(result_dib);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsMorphologySuite(void)
{
	CuSuite* suite = CuSuiteNew();

    MkDir(TEST_DATA_OUTPUT_DIR "/Morphology");
	
	SUITE_ADD_TEST(suite, TestFIA_DilationTest);
	SUITE_ADD_TEST(suite, TestFIA_ErosionTest);
	SUITE_ADD_TEST(suite, TestFIA_OpeningTest);
	SUITE_ADD_TEST(suite, TestFIA_ClosingTest);

	return suite;
}
