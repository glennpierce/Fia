#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Morphology.h"

static double kernel_values[] = {1.0, 1.0, 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0, 1.0, 1.0,
								 1.0, 1.0, 1.0, 1.0, 1.0};


static void
TestFreeImageAlgorithms_DilationTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\morpholology_test.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);

	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	FIABITMAP *border_dib = FreeImageAlgorithms_SetBorder(threshold_8bit_dib, 2, 2);

	ProfileStart("DilationFilter");

	FilterKernel kernel = FreeImageAlgorithms_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FreeImageAlgorithms_BinaryDilation(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

	ProfileStop("DilationFilter");

	FreeImageAlgorithms_SaveFIBToFile(result_dib, TEMP_DIR "\\dilation_result.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FreeImageAlgorithms_Unload(border_dib);
	FreeImage_Unload(result_dib);
}

static void
TestFreeImageAlgorithms_ErosionTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\morpholology_test.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);

	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	FIABITMAP *border_dib = FreeImageAlgorithms_SetBorder(threshold_8bit_dib, 2, 2);

	ProfileStart("ErosionFilter");

	FilterKernel kernel = FreeImageAlgorithms_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FreeImageAlgorithms_BinaryErosion(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

	ProfileStop("ErosionFilter");

	FreeImageAlgorithms_SaveFIBToFile(result_dib, TEMP_DIR "\\erosion_result.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FreeImageAlgorithms_Unload(border_dib);
	FreeImage_Unload(result_dib);
}

static void
TestFreeImageAlgorithms_OpeningTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\morpholology_test.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);

	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	FIABITMAP *border_dib = FreeImageAlgorithms_SetBorder(threshold_8bit_dib, 2, 2);

	ProfileStart("OpeningFilter");

	FilterKernel kernel = FreeImageAlgorithms_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FreeImageAlgorithms_BinaryOpening(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

	ProfileStop("OpeningFilter");

	FreeImageAlgorithms_SaveFIBToFile(result_dib, TEMP_DIR "\\opening_result.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FreeImageAlgorithms_Unload(border_dib);
	FreeImage_Unload(result_dib);
}

static void
TestFreeImageAlgorithms_ClosingTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\morpholology_test.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);

	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	FIABITMAP *border_dib = FreeImageAlgorithms_SetBorder(threshold_8bit_dib, 2, 2);

	ProfileStart("ClosingFilter");

	FilterKernel kernel = FreeImageAlgorithms_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FreeImageAlgorithms_BinaryClosing(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

	ProfileStop("ClosingFilter");

	FreeImageAlgorithms_SaveFIBToFile(result_dib, TEMP_DIR "\\closing_result.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FreeImageAlgorithms_Unload(border_dib);
	FreeImage_Unload(result_dib);
}


static void
TestFreeImageAlgorithms_FillholeTest(CuTest* tc)
{
	//char *file = IMAGE_DIR "\\fillhole_test.bmp";
	char *file = "C:\\Documents and Settings\\Glenn\\Desktop\\particle-test.bmp";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);
	
	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	ProfileStart("FillholeTest");

	FIBITMAP* result_dib = FreeImageAlgorithms_Fillholes(threshold_8bit_dib, 1);

	CuAssertTrue(tc, result_dib != NULL);

	ProfileStop("FillholeTest");
	
	FreeImageAlgorithms_SaveFIBToFile(result_dib, TEMP_DIR "\\fillhole_result.bmp", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FreeImage_Unload(result_dib);
}

static void
TestFreeImageAlgorithms_FloodFillTest(CuTest* tc)
{
	char *file = "C:\\Documents and Settings\\Glenn\\Desktop\\particle-test.bmp";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	ProfileStart("FloodFillTest");

	FIBITMAP *dib3 = FreeImageAlgorithms_FloodFill(dib2, 20, 0, 2);

	ProfileStop("FloodFillTest");

	FreeImageAlgorithms_SetTernaryPalettePalette(dib3, 
									FreeImageAlgorithms_GetRGBQUAD(255, 0, 0),
									2, FreeImageAlgorithms_GetRGBQUAD(0, 0, 255),
									255, FreeImageAlgorithms_GetRGBQUAD(0, 255, 0));


	CuAssertTrue(tc, dib3 != NULL);

	FreeImageAlgorithms_SaveFIBToFile(dib3, TEMP_DIR "\\floodfill.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}


static void
TestFreeImageAlgorithms_LabelTest(CuTest* tc)
{
	//char *file = IMAGE_DIR "\\fillhole_test.bmp";
	char *file = "C:\\Documents and Settings\\Glenn\\Desktop\\particle-test.bmp";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);

	const int width = FreeImage_GetWidth(dib2);

	printf("width %d\n", width);

	ProfileStart("LabelTest");

	FIBITMAP *dib3 = FreeImageAlgorithms_Label(dib2, 0);

	ProfileStop("LabelTest");

	/*
	FreeImageAlgorithms_SetTernaryPalettePalette(dib3, 
									FreeImageAlgorithms_GetRGBQUAD(255, 0, 0),
									2, FreeImageAlgorithms_GetRGBQUAD(0, 0, 255),
									255, FreeImageAlgorithms_GetRGBQUAD(0, 255, 0));

	*/

	

	CuAssertTrue(tc, dib3 != NULL);

	FreeImageAlgorithms_SaveFIBToFile(dib3, TEMP_DIR "\\label.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}




CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsMorphologySuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_DilationTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ErosionTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_OpeningTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ClosingTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FillholeTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FloodFillTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_LabelTest);

	return suite;
}