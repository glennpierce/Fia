#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Filters.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Convolution.h"

#include <iostream>

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
TestFIA_ConvolutionTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river-gs.jpg";

	FIBITMAP *dib_src = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib_src != NULL);

    FIBITMAP* dib1 = FreeImage_ConvertToType(dib_src, FIT_DOUBLE, 0);

    CuAssertTrue(tc, dib1 != NULL);

	FIABITMAP *dib2 = FIA_SetBorder(dib1, 300, 300
        , BorderType_Constant, 0.0);

	CuAssertTrue(tc, dib2->fib != NULL);

	PROFILE_START("FreeImageAlgorithms_Convolve");

	FilterKernel convolve_kernel = FIA_NewKernel(10, 10, kernel, 48.0);

	FIBITMAP* dib3 = FIA_Convolve(dib2, convolve_kernel);

	CuAssertTrue(tc, dib3 != NULL);

	PROFILE_STOP("FreeImageAlgorithms_Convolve");

	FIA_SaveFIBToFile(dib3, TEMP_DIR "\\wallpaper_river_blured.bmp", BIT24);

	FreeImage_Unload(dib1);
	FIA_Unload(dib2);
	FreeImage_Unload(dib3);
}


static void
TestFIA_SobelTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\input.bmp";

    FIBITMAP *bit8_dib = NULL;
	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	PROFILE_START("FreeImageAlgorithms_Sobel");

	FIBITMAP *dib2 = FIA_Sobel(dib1);

	PROFILE_STOP("FreeImageAlgorithms_Sobel");

    bit8_dib = FreeImage_ConvertToStandardType(dib2, 0);

	//FIA_SetTernaryPalettePalette(dib2, FIA_RGBQUAD(0,0,0), 
	//		1, FIA_RGBQUAD(255,0,0), 2, FIA_RGBQUAD(0,255,0));

	FIA_SaveFIBToFile(bit8_dib, TEMP_DIR "\\cells_sobel.bmp", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
    FreeImage_Unload(bit8_dib);
}

static void
TestFIA_SobelAdvancedTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\input.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	PROFILE_START("FreeImageAlgorithms_SobelAdvanced");

	FIBITMAP *vertical_dib = NULL, *horizontal_dib = NULL, *mag_dib = NULL;

    int err = FIA_SobelAdvanced(dib1, &vertical_dib,
        &horizontal_dib, NULL);
     
    CuAssertTrue(tc, err == FREEIMAGE_ALGORITHMS_SUCCESS); 

	PROFILE_STOP("FreeImageAlgorithms_SobelAdvanced");

    FIBITMAP* bit8_dib = FreeImage_ConvertToStandardType(horizontal_dib, 0);

    if(vertical_dib != NULL)
	    FIA_SaveFIBToFile(vertical_dib,
            TEMP_DIR "\\sobel_vertical.bmp", BIT8);

    

    if(horizontal_dib != NULL)
        FIA_SaveFIBToFile(bit8_dib,
            TEMP_DIR "\\sobel_horizontal_dib.bmp", BIT8);

    if(mag_dib != NULL)
        FIA_SaveFIBToFile(mag_dib,
            TEMP_DIR "\\sobel_magnitude_dib.bmp", BIT8);

    if(vertical_dib != NULL)
	    FreeImage_Unload(vertical_dib);
    
    if(horizontal_dib != NULL)
	    FreeImage_Unload(horizontal_dib);
    
    if(mag_dib != NULL)
        FreeImage_Unload(mag_dib);
}

/*
static void
TestFIA_SeparableSobelTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	ProfileStart("FreeImageAlgorithms_SeparableSobel");

	FIBITMAP *dib2 = FIA_SeparableSobel(dib1);

	ProfileStop("FreeImageAlgorithms_SeparableSobel");

	FIA_SaveFIBToFile(dib2, TEMP_DIR "\\wallpaper_river_sobel_separable.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
}
*/


static void
TestFIA_MedianFilterTest(CuTest* tc)
{
	//char *file = IMAGE_DIR "\\wallpaper_river-gs-salted.jpg";
	//char *file = IMAGE_DIR "\\med.bmp";
	char *file = IMAGE_DIR "\\test_image_5sq.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertToGreyscale(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	FIBITMAP *dib3 = FreeImage_ConvertToType(dib2, FIT_FLOAT, 1);

	CuAssertTrue(tc, dib3 != NULL);

	FIABITMAP* dib4 = FIA_SetBorder(dib3, 1, 1
        ,BorderType_Constant, 0.0);

	PROFILE_START("MedianFilter");

	FIBITMAP* dib5 = FIA_MedianFilter(dib4, 1, 1);

	CuAssertTrue(tc, dib5 != NULL);

	PROFILE_STOP("MedianFilter");

    /*
	for(int y=0; y < FreeImage_GetHeight(dib5); y++) {
	
		float *bits = (float*) FreeImage_GetScanLine(dib5, y);

		for(int x=0; x < FreeImage_GetWidth(dib5); x++)
			std::cout << bits[x] << ", ";

		std::cout << std::endl;
	}
    */

	FIA_SaveFIBToFile(dib5, TEMP_DIR "\\salt_and_pepper_median.jpg", BIT8);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FIA_Unload(dib4);
	FreeImage_Unload(dib5);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsConvolutionSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFIA_ConvolutionTest);
	SUITE_ADD_TEST(suite, TestFIA_SobelTest);
    SUITE_ADD_TEST(suite, TestFIA_SobelAdvancedTest);
	SUITE_ADD_TEST(suite, TestFIA_MedianFilterTest);
   
	return suite;
}
