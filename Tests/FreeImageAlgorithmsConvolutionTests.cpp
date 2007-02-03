#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"
#include "FreeImageAlgorithms_MedianFilter.h"

static const double kernel[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
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

static void
TestFreeImageAlgorithms_ConvolutionTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIABITMAP *dib2 = FreeImageAlgorithms_SetBorder(dib1, 10, 10);

	CuAssertTrue(tc, dib2->fib != NULL);

	ProfileStart("FreeImageAlgorithms_Convolve");

	FilterKernel convolve_kernel = FreeImageAlgorithms_NewKernel(10, 10, kernel, 48.0);

	FIBITMAP* dib3 = FreeImageAlgorithms_Convolve(dib2, convolve_kernel);

	CuAssertTrue(tc, dib3 != NULL);

	ProfileStop("FreeImageAlgorithms_Convolve");

	FreeImageAlgorithms_SaveFIBToFile(dib3, TEMP_DIR "\\wallpaper_river_blured_new.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImageAlgorithms_Unload(dib2);
	FreeImage_Unload(dib3);
}


static void
TestFreeImageAlgorithms_SobelTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	ProfileStart("FreeImageAlgorithms_Sobel");

	FIBITMAP *dib2 = FreeImageAlgorithms_Sobel(dib1);

	ProfileStop("FreeImageAlgorithms_Sobel");

	FreeImageAlgorithms_SaveFIBToFile(dib2, TEMP_DIR "\\wallpaper_river_sobel.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
}

/*
static void
TestFreeImageAlgorithms_SeparableSobelTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	ProfileStart("FreeImageAlgorithms_SeparableSobel");

	FIBITMAP *dib2 = FreeImageAlgorithms_SeparableSobel(dib1);

	ProfileStop("FreeImageAlgorithms_SeparableSobel");

	FreeImageAlgorithms_SaveFIBToFile(dib2, TEMP_DIR "\\wallpaper_river_sobel_separable.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
}
*/


static void
TestFreeImageAlgorithms_MedianFilterTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\salt_and_pepper.jpg";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertToGreyscale(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	FIBITMAP *dib3 = FreeImage_ConvertToType(dib2, FIT_DOUBLE, 1);

	CuAssertTrue(tc, dib3 != NULL);

	FIABITMAP* dib4 = FreeImageAlgorithms_SetBorder(dib3, 1, 1);

	ProfileStart("MedianFilter");

	FIBITMAP* dib5 = FreeImageAlgorithms_MedianFilter(dib4, 1, 1);

	CuAssertTrue(tc, dib5 != NULL);

	ProfileStop("MedianFilter");

	FreeImageAlgorithms_SaveFIBToFile(dib5, TEMP_DIR "\\salt_and_pepper_median.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FreeImageAlgorithms_Unload(dib4);
	FreeImage_Unload(dib5);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsConvolutionSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ConvolutionTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_SobelTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_SeparableSobelTest);

	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_Convolution3x3RealTest);
	//SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_MedianFilterTest);

	return suite;
}