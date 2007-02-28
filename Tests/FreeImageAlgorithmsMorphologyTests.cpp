#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Morphology.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_FindImageMaxima.h"

#include <iostream>

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

	FilterKernel kernel = FreeImageAlgorithms_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FreeImageAlgorithms_BinaryOpening(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

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

	FilterKernel kernel = FreeImageAlgorithms_NewKernel(2, 2, kernel_values, 1.0);

	FIBITMAP* result_dib = FreeImageAlgorithms_BinaryClosing(border_dib, kernel);

	CuAssertTrue(tc, result_dib != NULL);

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
TestFreeImageAlgorithms_ParticleInfoTest(CuTest* tc)
{
	//char *file = IMAGE_DIR "\\fillhole_test.bmp";
	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\particle-test.bmp";
	
	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	ProfileStart("ParticleInfo");

	PARTICLEINFO *info;

	for(int i=0; i < 30; i++) {

		FreeImageAlgorithms_ParticleInfo(dib2, &info, 1);
	}

	FIBITMAP *dst = FreeImage_ConvertTo24Bits(dib2);

	for(int i=0; i < info->number_of_blobs; i++)
	{
		BLOBINFO blobinfo = info->blobs[i];

		RECT rect;
		rect.left = blobinfo.left;
		rect.bottom = FreeImage_GetHeight(dst) - blobinfo.bottom;
		rect.right = blobinfo.right;
		rect.top = FreeImage_GetHeight(dst) - blobinfo.top;

		FreeImageAlgorithms_DrawColourRect (dst, rect, RGB(255,0,0), 2);
	}
	
	std::cout << "Number of particles " << info->number_of_blobs << std::endl;

	FreeImageAlgorithms_SaveFIBToFile(dst, "C:\\Documents and Settings\\Pierce\\Desktop\\particle_rect.jpg", BIT24);

	ProfileStop("ParticleInfo");

	FreeImageAlgorithms_FreeParticleInfo(info);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dst);
}


/*
static void
TestFreeImageAlgorithms_FindImageMaximaTest(CuTest* tc)
{
	char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\test.bmp";
	
	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	ProfileStart("FindImageMaxima");

	FIBITMAP *dib3 = FreeImageAlgorithms_FindImageMaxima(dib2, 2);
	
	FreeImageAlgorithms_SetGreyLevelPalette(dib3);

	FIBITMAP *dst = FreeImage_ConvertTo24Bits(dib3);

	FreeImageAlgorithms_SaveFIBToFile(dst, "C:\\Documents and Settings\\Pierce\\Desktop\\find_image_maxima.jpg", BIT24);

	ProfileStop("FindImageMaxima");

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FreeImage_Unload(dst);
}
*/

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsMorphologySuite(void)
{
	CuSuite* suite = CuSuiteNew();

	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_DilationTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ErosionTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_OpeningTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ClosingTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FillholeTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FloodFillTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ParticleInfoTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FindImageMaximaTest);

	return suite;
}