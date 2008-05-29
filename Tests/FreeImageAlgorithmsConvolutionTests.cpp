#include "CuTest.h"

#include "Constants.h"

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
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

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
    FIBITMAP* dib4 = FreeImage_ConvertToStandardType(dib3, 1);

	CuAssertTrue(tc, dib3 != NULL);

	PROFILE_STOP("FreeImageAlgorithms_Convolve");

	FIA_SaveFIBToFile(dib4, TEST_DATA_OUTPUT_DIR "drone-bee-convolved.bmp", BIT24);

	FreeImage_Unload(dib1);
	FIA_Unload(dib2);
	FreeImage_Unload(dib3);
    FreeImage_Unload(dib4);
}


static void
TestFIA_SobelTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

    FIBITMAP *bit8_dib = NULL;
	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	PROFILE_START("FreeImageAlgorithms_Sobel");

	FIBITMAP *dib2 = FIA_Sobel(dib1);

	PROFILE_STOP("FreeImageAlgorithms_Sobel");

    bit8_dib = FreeImage_ConvertToStandardType(dib2, 0);

	//FIA_SetTernaryPalettePalette(dib2, FIA_RGBQUAD(0,0,0), 
	//		1, FIA_RGBQUAD(255,0,0), 2, FIA_RGBQUAD(0,255,0));

	FIA_SaveFIBToFile(bit8_dib, TEST_DATA_OUTPUT_DIR "drone-bee_sobel.bmp", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
    FreeImage_Unload(bit8_dib);
}

static void
TestFIA_SobelAdvancedTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);

	PROFILE_START("FreeImageAlgorithms_SobelAdvanced");

	FIBITMAP *vertical_dib = NULL, *horizontal_dib = NULL, *mag_dib = NULL;

    int err = FIA_SobelAdvanced(dib1, &vertical_dib,
        &horizontal_dib, NULL);
     
    CuAssertTrue(tc, err == FIA_SUCCESS); 

	PROFILE_STOP("FreeImageAlgorithms_SobelAdvanced");

    FIBITMAP* bit8_dib = FreeImage_ConvertToStandardType(horizontal_dib, 0);

    if(vertical_dib != NULL)
	    FIA_SaveFIBToFile(vertical_dib,
            TEST_DATA_OUTPUT_DIR "drone-bee_vertical.bmp", BIT8);

    if(horizontal_dib != NULL)
        FIA_SaveFIBToFile(bit8_dib,
            TEST_DATA_OUTPUT_DIR "drone-bee_sobel_horizontal_dib.bmp", BIT8);

    if(mag_dib != NULL)
        FIA_SaveFIBToFile(mag_dib,
            TEST_DATA_OUTPUT_DIR "drone-bee_sobel_magnitude_dib.bmp", BIT8);

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
	const char *file = IMAGE_DIR "\\wallpaper_river.jpg";

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
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

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

	FIA_SaveFIBToFile(dib5, TEST_DATA_OUTPUT_DIR  "drone-bee-median_filtered.jpg", BIT8);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FIA_Unload(dib4);
	FreeImage_Unload(dib5);
}

static void
TestFIA_CorrelateFilterTest(CuTest* tc)
{
    double max;
    const char *colour_file = TEST_DATA_DIR "drone-bee.jpg";
    const char *gs_file = TEST_DATA_DIR "drone-bee-greyscale.jpg";
	const char *file = TEST_DATA_DIR "drone-bee-greyscale-section.jpg";

    FIBITMAP *colour_src = FIA_LoadFIBFromFile(colour_file);
    FIBITMAP *gs_src = FIA_LoadFIBFromFile(gs_file);
	FIBITMAP *src = FIA_LoadFIBFromFile(file);
    FIBITMAP *colour_section = FreeImage_ConvertTo24Bits(src);
	
	CuAssertTrue(tc, src != NULL);
	CuAssertTrue(tc, gs_src != NULL);
	CuAssertTrue(tc, colour_src != NULL);

    FIARECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = FreeImage_GetHeight(colour_src);
	rect.right = FreeImage_GetWidth(colour_src);

	PROFILE_START("FIA_CorrelateImages");

	FIAPOINT pt;
	
	if(FIA_CorrelateImages(gs_src, src, &pt, &max) == FIA_ERROR) {
	    PROFILE_STOP("FIA_CorrelateImages");
	    goto TEST_ERROR;
	}
	
	PROFILE_STOP("FIA_CorrelateImages");

    if(FreeImage_Paste(colour_src, colour_section, pt.x, pt.y, 255) == 0) {
        printf("paste failed\n");
    }

	FIA_SaveFIBToFile(colour_src, TEST_DATA_OUTPUT_DIR  "correlated.jpg", BIT24);

	TEST_ERROR:
	
	FreeImage_Unload(src);
	FreeImage_Unload(colour_src);
	FreeImage_Unload(gs_src);
    FreeImage_Unload(colour_section);

    //178, 138
}


static void
TestFIA_CorrelateRegionsTest(CuTest* tc)
{
    double max;
    
    const char *colour_file = TEST_DATA_DIR "drone-bee.jpg";
    const char *gs_file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

    FIBITMAP *colour_src = FIA_LoadFIBFromFile(colour_file);
    FIBITMAP *gs_src = FIA_LoadFIBFromFile(gs_file);
    FIBITMAP *gs_src24 = FreeImage_ConvertTo24Bits(gs_src);
    FIBITMAP *colour_section = NULL;
        
    CuAssertTrue(tc, gs_src != NULL);
    CuAssertTrue(tc, colour_src != NULL);

    FIARECT rect1, rect2;
    rect1.left = 0;
    rect1.top = 0;
    rect1.bottom = FreeImage_GetHeight(colour_src);
    rect1.right = FreeImage_GetWidth(colour_src);

    rect2.left = 100;
    rect2.top = 100;
    rect2.bottom = 139;
    rect2.right = 139;
        
    PROFILE_START("FIA_CorrelateImageRegions");

    FIAPOINT pt;
    
    if(FIA_CorrelateImageRegions(gs_src, rect1, gs_src, rect2, &pt, &max) == FIA_ERROR) {
        PROFILE_STOP("FIA_CorrelateImageRegions");
        goto TEST_ERROR;
    }
    
    PROFILE_STOP("FIA_CorrelateImageRegions");
    
    colour_section = FreeImage_Copy(colour_src, pt.x, pt.y, pt.x + 39, pt.y + 39);
    
    if(FreeImage_Paste(gs_src24, colour_section, pt.x, pt.y, 255) == 0) {
        printf("paste failed\n");
    }

    FIA_SaveFIBToFile(gs_src24, TEST_DATA_OUTPUT_DIR  "correlated-region.jpg", BIT24);

    TEST_ERROR:
    
    FreeImage_Unload(colour_src);
    FreeImage_Unload(gs_src);
    FreeImage_Unload(gs_src24);
    FreeImage_Unload(colour_section);
}

static void
TestFIA_CorrelateTissueRegionsTest(CuTest* tc)
{
    const char *tissue1_file = TEST_DATA_DIR "d12ob101.bmp";
    const char *tissue2_file = TEST_DATA_DIR "d12ob102.bmp";
    double max;
    FIAPOINT pt;
    
    FIBITMAP *src1 = FIA_LoadFIBFromFile(tissue1_file);
    FIBITMAP *src2 = FIA_LoadFIBFromFile(tissue2_file);
  
    PROFILE_START("FIA_CorrelateImageRegions");
    
    FIA_CorrelateImagesAlongRightEdge(src1, src2, 200, &pt, &max);
    
    PROFILE_STOP("FIA_CorrelateImageRegions");
    
    std::cout << "pt.x " << pt.x << " pt.y " << pt.y << " Max " << max << std::endl;
    
    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(src1), FreeImage_GetWidth(src1) * 2, FreeImage_GetHeight(src1) * 2, 
                    FreeImage_GetBPP(src1), 0, 0, 0);    
    
    if(FreeImage_Paste(joined_image, src1, 0, 0, 255) == 0) {
        printf("paste failed\n");
    }
    
    if(FreeImage_Paste(joined_image, src2, pt.x, pt.y, 255) == 0) {
        printf("paste failed\n");
    }
    
    FIA_SaveFIBToFile(joined_image, TEST_DATA_OUTPUT_DIR  "joined.png", BIT24);

    TEST_ERROR:
        
    return;
        
    FreeImage_Unload(src1);
    FreeImage_Unload(src2);
    FreeImage_Unload(joined_image);
    
}

static void
TestFIA_CorrelateTissueRegionsVerticalTest(CuTest* tc)
{
    const char *tissue1_file = TEST_DATA_DIR "d12ob101.bmp";
    const char *tissue2_file = TEST_DATA_DIR "d12ob104.bmp";
    double max;
    FIAPOINT pt;
    
    FIBITMAP *src1 = FIA_LoadFIBFromFile(tissue1_file);
    FIBITMAP *src2 = FIA_LoadFIBFromFile(tissue2_file);
  
    PROFILE_START("FIA_CorrelateImageRegions");
    
    FIA_CorrelateImagesAlongBottomEdge(src1, src2, 200, &pt, &max);
    
    PROFILE_STOP("FIA_CorrelateImageRegions");
    
    std::cout << "pt.x " << pt.x << " pt.y " << pt.y << " Max " << max << std::endl;
    
    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(src1), FreeImage_GetWidth(src1) * 3, FreeImage_GetHeight(src1) * 3, 
                    FreeImage_GetBPP(src1), 0, 0, 0);    
    
    if(FreeImage_Paste(joined_image, src1, 0 + 100, 0 + 100, 255) == 0) {
        printf("paste failed\n");
    }
    
    if(FreeImage_Paste(joined_image, src2, pt.x + 100, pt.y + 100, 255) == 0) {
        printf("paste failed\n");
    }
    
    FIA_SaveFIBToFile(joined_image, TEST_DATA_OUTPUT_DIR  "joined-vertical.png", BIT24);

    TEST_ERROR:
        
    return;
        
    FreeImage_Unload(src1);
    FreeImage_Unload(src2);
    FreeImage_Unload(joined_image);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsConvolutionSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFIA_ConvolutionTest);
	SUITE_ADD_TEST(suite, TestFIA_SobelTest);
    SUITE_ADD_TEST(suite, TestFIA_SobelAdvancedTest);
	SUITE_ADD_TEST(suite, TestFIA_MedianFilterTest);
    SUITE_ADD_TEST(suite, TestFIA_CorrelateFilterTest);
    //SUITE_ADD_TEST(suite, TestFIA_CorrelateRegionsTest);
    SUITE_ADD_TEST(suite, TestFIA_CorrelateTissueRegionsTest);
    SUITE_ADD_TEST(suite, TestFIA_CorrelateTissueRegionsVerticalTest);
    
	return suite;
}
