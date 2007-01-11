#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_MedianFilter.h"


static double kernel[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
						  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
						  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
						  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
						  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			 			  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
						  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
						  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
						  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
					  	  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

static double kernel3x3[] = {1.0, 1.0, 1.0,
							 1.0, 1.0, 1.0, 
							 1.0, 1.0, 1.0};



/*
static void
TestFreeImageAlgorithms_ConvolutionFixedTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *dib2 = FreeImage_ConvertToGreyscale(dib1);

	FIBITMAP *dib3 = FreeImage_ConvertToType(dib2, FIT_INT32, 1);

	CuAssertTrue(tc, dib3 != NULL);

	FIABITMAP dib4 = FreeImageAlgorithms_AddBorder(dib3, 10);

	ProfileStart("FreeImageAlgorithms_FixedConvolve");

	FIBITMAP* dib5 = FreeImageAlgorithms_Convolve(dib4, 10, 10, kernel, 48.0);

	CuAssertTrue(tc, dib5 != NULL);

	ProfileStop("FreeImageAlgorithms_FixedConvolve");

	FreeImageAlgorithms_SaveFIBToFile(dib5, TEMP_DIR "\\wallpaper_river_fixed_blured.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FreeImage_Unload(dib4.fib);
	FreeImage_Unload(dib5);
}
*/

static void
TestFreeImageAlgorithms_Convolution3x3RealTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *dib2 = FreeImage_ConvertToGreyscale(dib1);

	FIBITMAP *dib3 = FreeImage_ConvertToType(dib2, FIT_DOUBLE, 1);

	CuAssertTrue(tc, dib3 != NULL);

	FIABITMAP dib4 = FreeImageAlgorithms_AddBorder(dib3, 1);

	ProfileStart("FreeImageAlgorithms_RealConvolve");

	FIBITMAP* dib5 = FreeImageAlgorithms_Convolve(dib4, 1, 1, kernel3x3, 48.0);

	CuAssertTrue(tc, dib5 != NULL);

	ProfileStop("FreeImageAlgorithms_RealConvolve");

	FreeImageAlgorithms_SaveFIBToFile(dib5, TEMP_DIR "\\wallpaper_river_real_blured.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FreeImage_Unload(dib4.fib);
	FreeImage_Unload(dib5);
}


static void
TestFreeImageAlgorithms_ConvolutionRealTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *dib2 = FreeImage_ConvertToGreyscale(dib1);

	FIBITMAP *dib3 = FreeImage_ConvertToType(dib2, FIT_DOUBLE, 1);

	CuAssertTrue(tc, dib3 != NULL);

	FIABITMAP dib4 = FreeImageAlgorithms_AddBorder(dib3, 10);

	ProfileStart("FreeImageAlgorithms_RealConvolve");

	FIBITMAP* dib5 = FreeImageAlgorithms_Convolve(dib4, 10, 10, kernel, 48.0);

	CuAssertTrue(tc, dib5 != NULL);

	ProfileStop("FreeImageAlgorithms_RealConvolve");

	FreeImageAlgorithms_SaveFIBToFile(dib5, TEMP_DIR "\\wallpaper_river_real_blured.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FreeImage_Unload(dib4.fib);
	FreeImage_Unload(dib5);
}

static void
TestFreeImageAlgorithms_MedianFilterTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\salt_and_pepper.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *dib2 = FreeImage_ConvertToGreyscale(dib1);

	FIBITMAP *dib3 = FreeImage_ConvertToType(dib2, FIT_DOUBLE, 1);

	CuAssertTrue(tc, dib3 != NULL);

	FIABITMAP dib4 = FreeImageAlgorithms_AddBorder(dib3, 1);

	ProfileStart("MedianFilter");

	FIBITMAP* dib5 = FreeImageAlgorithms_MedianFilter(dib4, 1, 1);

	CuAssertTrue(tc, dib5 != NULL);

	ProfileStop("MedianFilter");

	FreeImageAlgorithms_SaveFIBToFile(dib5, TEMP_DIR "\\salt_and_pepper_median.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FreeImage_Unload(dib4.fib);
	FreeImage_Unload(dib5);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsConvolutionSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ConvolutionFixedTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_Convolution3x3RealTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_MedianFilterTest);

	return suite;
}