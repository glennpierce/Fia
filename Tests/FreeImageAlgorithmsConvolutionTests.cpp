#include "CuTest.h"

#include "Constants.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Filters.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Statistics.h"
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

	FIA_SaveFIBToFile(dib4, TEST_DATA_OUTPUT_DIR "/Convolution/drone-bee-convolved.bmp", BIT24);

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

	FIA_SaveFIBToFile(bit8_dib, TEST_DATA_OUTPUT_DIR "/Convolution/drone-bee_sobel.bmp", BIT24);

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
            TEST_DATA_OUTPUT_DIR "/Convolution/drone-bee_vertical.bmp", BIT8);

    if(horizontal_dib != NULL)
        FIA_SaveFIBToFile(bit8_dib,
            TEST_DATA_OUTPUT_DIR "/Convolution/drone-bee_sobel_horizontal_dib.bmp", BIT8);

    if(mag_dib != NULL)
        FIA_SaveFIBToFile(mag_dib,
            TEST_DATA_OUTPUT_DIR "/Convolution/drone-bee_sobel_magnitude_dib.bmp", BIT8);

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

	FIA_SaveFIBToFile(dib5, TEST_DATA_OUTPUT_DIR  "/Convolution/drone-bee-median_filtered.jpg", BIT8);

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

	PROFILE_START("TestFIA_CorrelateFilterTest");

	FIAPOINT pt;

	if(FIA_KernelCorrelateImages(gs_src, src, NULL, &pt, &max) == FIA_ERROR) {
	    PROFILE_STOP("TestFIA_CorrelateFilterTest");
	    goto TEST_ERROR;
	}

	PROFILE_STOP("TestFIA_CorrelateFilterTest");


    if(FreeImage_Paste(colour_src, colour_section, pt.x, pt.y, 255) == 0) {
        printf("Paste failed for TestFIA_CorrelateFilterTest. Trying to paste at %d, %d\n",
        		pt.x, pt.y);
    }

	FIA_SaveFIBToFile(colour_src, TEST_DATA_OUTPUT_DIR  "/Convolution/kernel-correlated.jpg", BIT24);

	TEST_ERROR:

	FreeImage_Unload(src);
	FreeImage_Unload(colour_src);
	FreeImage_Unload(gs_src);
    FreeImage_Unload(colour_section);
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
    rect1.left = 50;
    rect1.top = 100;
    rect1.bottom = 210;
    rect1.right = 180;

    rect2.left = 105;
    rect2.top = 125;
    rect2.bottom = 200;
    rect2.right = 162;

    PROFILE_START("TestFIA_CorrelateRegionsTest");

    FIAPOINT pt;

    if(FIA_KernelCorrelateImageRegions(gs_src, rect1, gs_src, rect2, NULL, &pt, &max) == FIA_ERROR) {
        PROFILE_STOP("TestFIA_CorrelateRegionsTest");
        goto TEST_ERROR;
    }

    PROFILE_STOP("TestFIA_CorrelateRegionsTest");

    colour_section = FreeImage_Copy(colour_src, pt.x, pt.y, pt.x + 39, pt.y + 39);

    if(FreeImage_Paste(gs_src24, colour_section, pt.x, pt.y, 255) == 0) {
        printf("Paste failed for TestFIA_CorrelateRegionsTest. Trying to paste at %d, %d\n",
        		pt.x, pt.y);
    }

    FIA_SaveFIBToFile(gs_src24, TEST_DATA_OUTPUT_DIR  "/Convolution/kernel-correlated-region.jpg", BIT24);

    TEST_ERROR:

    FreeImage_Unload(colour_src);
    FreeImage_Unload(gs_src);
    FreeImage_Unload(gs_src24);
    FreeImage_Unload(colour_section);
}


static void
TestFIA_CorrelateFFTTest(CuTest* tc)
{
    const char *tissue1_file = TEST_DATA_DIR "gregarious-desert-locusts.jpg";
    const char *tissue2_file = TEST_DATA_DIR "gregarious-desert-locusts-section.jpg";

    FIAPOINT pt;

    pt.x = 0;
    pt.y = 0;

    FIBITMAP *src1 = FIA_LoadFIBFromFile(tissue1_file);
    FIBITMAP *gs_src1 = FreeImage_ConvertToGreyscale(src1);
    FIBITMAP *src2 = FIA_LoadFIBFromFile(tissue2_file);

    PROFILE_START("TestFIA_CorrelateFFTTest");

    FIA_FFTCorrelateImages(src1, src2, FIA_EdgeDetect, &pt);

    PROFILE_STOP("TestFIA_CorrelateFFTTest");

    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(src1), 400, 400,
                    FreeImage_GetBPP(src1), 0, 0, 0);

    if(FreeImage_GetBPP(joined_image) == 8)
        FIA_SetGreyLevelPalette(joined_image);

    FreeImage_Paste(joined_image, gs_src1, 0, 0, 256);
    FreeImage_Paste(joined_image, src2, pt.x, pt.y, 256);

    FIA_SaveFIBToFile(joined_image, TEST_DATA_OUTPUT_DIR  "/Convolution/correlated-fft.png", BIT24);

    FreeImage_Unload(src1);
    FreeImage_Unload(src2);
    FreeImage_Unload(gs_src1);
    FreeImage_Unload(joined_image);

    return;
}


static FIBITMAP* GetRandomImageRect(FIBITMAP *src, FIARECT *rect)
{
    int width = FreeImage_GetWidth(src);
    int height = FreeImage_GetHeight(src);
    int section_width = 400;
    int section_height = 400;
    int min = 50;

#ifdef WIN32
    rect->left = (int) (((float) rand() / RAND_MAX) * (width - min));
    rect->top = (int) (((float) rand() / RAND_MAX) * (height - min));
    rect->right = min(rect->left + section_width, width - 1);
    rect->bottom = min(rect->top + section_width, height - 1);
#else
	rect->left = (int) (((float) rand() / RAND_MAX) * (width - min));
    rect->top = (int) (((float) rand() / RAND_MAX) * (height - min));
    rect->right = std::min(rect->left + section_width, width - 1);
    rect->bottom = std::min(rect->top + section_width, height - 1);
#endif

    return FreeImage_Copy(src, rect->left, rect->top, rect->right, rect->bottom);
}


static FIBITMAP *
__cdecl
FIA_EdgeEnhancer(FIBITMAP * src)
{
    FIBITMAP *fib = FreeImage_Clone(src);

    FIABITMAP *bordered = FIA_SetBorder(fib, 3, 3, BorderType_Copy, 0.0);

    FIBITMAP *median_filtered = FIA_MedianFilter(bordered, 3, 3);

    FIBITMAP *sobel = FIA_Sobel(median_filtered);

    FreeImage_AdjustContrast(sobel, 100.0);

    FIA_Unload(bordered);
    FreeImage_Unload(median_filtered);

    return sobel;
}


static void
TestFIA_CorrelateFFTLetterTest(CuTest* tc)
{
    const char *file1 = TEST_DATA_DIR "correlation_test1.png";
    const char *file2 = TEST_DATA_DIR "correlation_test2.png";
    const char *file3 = TEST_DATA_DIR "correlation_test3.png";
    const char *file4 = TEST_DATA_DIR "correlation_test4.png";
    const char *file5 = TEST_DATA_DIR "correlation_test5.png";

    FIAPOINT pt;

    pt.x = 0;
    pt.y = 0;

    FIBITMAP *src1 = FIA_LoadFIBFromFile(file1);
    FIBITMAP *src2 = FIA_LoadFIBFromFile(file2);
    FIBITMAP *src3 = FIA_LoadFIBFromFile(file3);
    FIBITMAP *src4 = FIA_LoadFIBFromFile(file4);
    FIBITMAP *src5 = FIA_LoadFIBFromFile(file5);

    PROFILE_START("TestFIA_CorrelateFFTTest2");

    FIA_FFTCorrelateImages(src1, src2, NULL, &pt);

    PROFILE_STOP("TestFIA_CorrelateFFTTest2");

    FreeImage_Paste(src1, src3, pt.x, pt.y, 256);

    FIA_FFTCorrelateImages(src1, src4, NULL, &pt);

    FreeImage_Paste(src1, src5, pt.x, pt.y, 256);

    FIA_SaveFIBToFile(src1, TEST_DATA_OUTPUT_DIR  "/Convolution/correlated-fft2.png", BIT24);

    FreeImage_Unload(src1);
    FreeImage_Unload(src2);
    FreeImage_Unload(src3);
    FreeImage_Unload(src4);
    FreeImage_Unload(src5);

    return;
}

/*
static void
TestFIA_CorrelateFFTAlongRightEdge(CuTest* tc)
{
	const char *left_file = TEST_DATA_DIR "spider-eating-a-fly.jpg";
    const char *right_file = TEST_DATA_DIR "spider-eating-a-fly-right_edge.jpg";

    FIBITMAP *left_src = FIA_LoadFIBFromFile(left_file);
    FIBITMAP *right_src = FIA_LoadFIBFromFile(right_file);

	CuAssertTrue(tc, FreeImage_GetBPP(left_src) == FreeImage_GetBPP(right_src));
    CuAssertTrue(tc, left_src != NULL);
    CuAssertTrue(tc, right_src != NULL);

    PROFILE_START("TestFIA_CorrelateFFTAlongRightEdge");

    FIAPOINT pt;

    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(left_src), 300, 400,
                    FreeImage_GetBPP(left_src), 0, 0, 0);

    if(FIA_FFTCorrelateImagesAlongRightEdge(left_src, right_src, NULL, 57, &pt) == FIA_ERROR) {
        PROFILE_STOP("TestFIA_CorrelateFFTAlongRightEdge");
        goto TEST_ERROR;
    }

    PROFILE_STOP("TestFIA_CorrelateFFTAlongRightEdge");

    if(FreeImage_GetBPP(joined_image) == 8)
        FIA_SetGreyLevelPalette(joined_image);

    if(FreeImage_Paste(joined_image, left_src, 0, 0, 256) == 0) {
        printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n",
        		pt.x, pt.y);
    }

    if(FreeImage_Paste(joined_image, right_src, pt.x, pt.y, 256) == 0) {
        printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n",
        		pt.x,pt.y);
    }

    FIA_SaveFIBToFile(joined_image, TEST_DATA_OUTPUT_DIR  "/Convolution/fft-correlated-right_edge.png", BIT24);

    TEST_ERROR:

    FreeImage_Unload(left_src);
    FreeImage_Unload(right_src);
    FreeImage_Unload(joined_image);

    return;
}
*/


static FIBITMAP* LoadTissueFile(const char *filepath)
{
    FIBITMAP *fib = FIA_LoadFIBFromFile(filepath);

    FIBITMAP *section = FIA_Copy(fib, 3, 3, FreeImage_GetWidth(fib) - 4, FreeImage_GetHeight(fib) - 4);

    FreeImage_Unload(fib);

    return section;
}


typedef struct
{
    FIARECT rect;
    FIBITMAP *fib;
    char path[250];

} Tile;

static void
TestFIA_CorrelateBloodTissueImages(CuTest* tc)
{
    FIARECT first_rect, rect;
    FIBITMAP* fibs[20];

    #define PREFIX "/home/glenn/TestData/4x6/"

    int width = 768;
    int height = 576;

    Tile tiles[] =
    {
            {MakeFIARect(0,0,width,height), NULL, PREFIX "d9ob20_00006.bmp"},
            {MakeFIARect(width-100, 0, 2*width-100, height), NULL, PREFIX "d9ob20_00005.bmp"},
            {MakeFIARect(-50, height - 185, width-50, 2 * height - 185), NULL, PREFIX "d9ob20_00011.bmp"},

    };

    const int number_of_images = sizeof(tiles) / sizeof(Tile);

    FIBITMAP *fib1 =  LoadTissueFile(tiles[0].path);
    FIBITMAP *fib2 =  LoadTissueFile(tiles[1].path);
    FIBITMAP *fib3 =  LoadTissueFile(tiles[2].path);

    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(fib1), 1000, 1000, FreeImage_GetBPP(fib1), 0, 0, 0);

    FIAPOINT pt;

    FIA_CorrelateImagesAroundOverlap(fib1, tiles[0].rect, fib2, tiles[1].rect, 30, CORRELATION_FFT, FIA_EdgeEnhancer, &pt);

    if(FIA_PasteFromTopLeft(joined_image, fib1, 0, 0) == 0) {
        printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n", pt.x, pt.y);
    }

    if(FIA_PasteFromTopLeft(joined_image, fib2, pt.x, pt.y) == 0) {
        printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n", pt.x,pt.y);
    }




    FIA_CorrelateImagesAroundOverlap(joined_image, tiles[0].rect, fib3, tiles[2].rect, 50, CORRELATION_FFT, FIA_EdgeEnhancer, &pt);



    std::cout << "pt.x " << pt.x << " pt.y: " << pt.y << std::endl;

    if(FIA_PasteFromTopLeft(joined_image, fib3, pt.x, pt.y) == 0) {
         printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n", pt.x,pt.y);
     }

    FIA_SaveFIBToFile(joined_image,  "/home/glenn/joined.png", BIT24);

  //  for(int i=0; i < number_of_images; i++) {

  //      Tile
  //  }


    /*
    FIBITMAP *fib = LoadTissueFile(images[8]);

    int width = FreeImage_GetWidth(fib);
    int height = FreeImage_GetHeight(fib);
    FIAPOINT pt;

    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(fib), 1000,
            1000, FreeImage_GetBPP(fib), 0, 0, 0);

    FIBITMAP *fib2 = LoadTissueFile(images[11]);

    PROFILE_START("TestFIA_CorrelateFFTTest3");

    FIARECT region1 = MakeFIARect(0, height-100, width-1, height-1);
    FIARECT region2 = MakeFIARect(0, 0, width-1, 50);

    FIA_CorrelateImageRegions(fib, region1, fib2, region2,
            CORRELATION_FFT, FIA_EdgeEnhancer, &pt);

    PROFILE_STOP("TestFIA_CorrelateFFTTest3");

    if(FIA_PasteFromTopLeft(joined_image, fib, 0, 0) == 0) {
           printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n",
                   pt.x, pt.y);
       }

       if(FIA_PasteFromTopLeft(joined_image, fib2, pt.x, pt.y) == 0) {
           printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n",
                   pt.x,pt.y);
       }

       std::cout << "pt.x " << pt.x << " pt.y: " << pt.y << std::endl;

 //      FIBITMAP *fib3 = FIA_LoadFIBFromFile(images[3]);

  //     if(FIA_FFTCorrelateImagesAlongRightEdge(fib2, fib3, FIA_EdgeDetect2, 40, &pt) == FIA_ERROR) {
                 //PROFILE_STOP("TestFIA_CorrelateFFTAlongRightEdge");
                 //goto TEST_ERROR;
  //           }

  //     if(FIA_PasteFromTopLeft(joined_image, fib3, pt.x, pt.y) == 0) {
  //             printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n",
   //                    pt.x,pt.y);
    //       }

           std::cout << "pt.x " << pt.x << " pt.y: " << pt.y << std::endl;

*/

       /*

    double measure = FIA_CorrelationDifferenceMeasure(joined_image, fib, pt);

    std::cout << "Measure " << measure << std::endl;

     if(measure >= 0.0 && measure < 1000.0) {

                  FIA_PasteFromTopLeft(joined_image, fib, pt.x, pt.y);
       }
*/

  //   FIA_SaveFIBToFile(joined_image,  "/home/glenn/joined.png", BIT24);


    /*
    for(int i=0; i < number_of_images; i++) {

          fibs[i] = GetRandomImageRect(original_fib, &rect);

          PROFILE_START("TestFIA_CorrelateFFTTest2");

          std::cout << "rect left: " << rect.left << " rect top: " << rect.top
              << " width: " << rect.right - rect.left + 1 << " height: " << rect.bottom - rect.top + 1 << std::endl;

          std::cout << "Correlating image " << i << std::endl;

          FIA_CorrelateImagesFFT(joined_image, fibs[i], FIA_EdgeDetect, &pt);

          std::cout << "pt.x " << pt.x << " pt.y: " << pt.y << std::endl;

          double measure = FIA_CorrelationDifferenceMeasure(joined_image, fibs[i], pt);

          std::cout << "Measure " << measure << std::endl;

          if(measure >= 0.0 && measure < 1000.0) {
              std::cout << "x: " << pt.x << " y: " << pt.y << std::endl;

              FIA_PasteFromTopLeft(joined_image, fibs[i], pt.x, pt.y);
          }

          PROFILE_STOP("TestFIA_CorrelateFFTTest2");
    }
*/



   // for(int i=0; i < number_of_images; i++) {

   //     FreeImage_Unload(fibs[i]);
  //  }

   // FreeImage_Unload(original_fib);
 //   FreeImage_Unload(joined_image);

    return;
}


static void
TestFIA_CorrelateSpiceSection(CuTest* tc)
{
    FIAPOINT pt;

    FIBITMAP *spice_fib = FIA_LoadFIBFromFile(TEST_DATA_DIR "CorrelationSections/spice.jpg");
    FIBITMAP *spice_section_fib = FIA_LoadFIBFromFile(TEST_DATA_DIR "CorrelationSections/spice-section.jpg");

    int spice_width = FreeImage_GetWidth(spice_fib);
    int spice_height = FreeImage_GetHeight(spice_fib);
    int section_width = FreeImage_GetWidth(spice_section_fib);
    int section_height = FreeImage_GetHeight(spice_section_fib);

    PROFILE_START("TestFIA_CorrelateSpiceSection");

    FIARECT region1 = MakeFIARect(20, 20, spice_width-1, spice_height-1);
    FIARECT region2 = MakeFIARect(30, 50, 300, section_height - 200);

    FIA_CorrelateImageRegions(spice_fib, region1, spice_section_fib, region2,
            CORRELATION_FFT, FIA_EdgeEnhancer, &pt);

    PROFILE_STOP("TestFIA_CorrelateSpiceSection");


    if(FIA_PasteFromTopLeft(spice_fib, spice_section_fib, pt.x, pt.y) == 0) {
           printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n",
                   pt.x,pt.y);
    }

    FIA_SaveFIBToFile(spice_fib, TEST_DATA_OUTPUT_DIR  "/Convolution/spice-section-correlate.png", BIT24);

    FreeImage_Unload(spice_fib);
    FreeImage_Unload(spice_section_fib);

    return;
}


static void
TestFIA_CorrelateBloodTissueImagesTwoImages(CuTest* tc)
{
    int width = 768;
    int height = 576;

    FIBITMAP *fib1 =  LoadTissueFile(TEST_DATA_DIR "BloodVessels/d9ob20_00008.png");
    FIBITMAP *fib2 =  LoadTissueFile(TEST_DATA_DIR "BloodVessels/d9ob20_00007.png");

    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(fib1),
            2 * width, 2 * height, FreeImage_GetBPP(fib1), 0, 0, 0);

    FIAPOINT pt;

    FIARECT rect1 = MakeFIARect(width-350,0,width-1,height-1);
    FIARECT rect2 = MakeFIARect(0, 0, 100, height-1);

    FIA_CorrelateImageRegions(fib1, rect1, fib2, rect2, CORRELATION_FFT, FIA_EdgeEnhancer, &pt);

    double measure;

    FIA_CorrelationDifferenceMeasure(fib1, fib2, pt, &measure);

    std::cout << "Difference Measure: " << measure << std::endl;

    //FIA_CorrelateImages(fib1, fib2, CORRELATION_FFT, FIA_EdgeEnhancer, &pt);

    std::cout << "pt.x " << pt.x << " pt.y: " << pt.y << std::endl;

    if(FIA_PasteFromTopLeft(joined_image, fib1, 0, 0) == 0) {
        printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n", pt.x, pt.y);
    }

    if(FIA_PasteFromTopLeft(joined_image, fib2, pt.x, pt.y) == 0) {
        printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n", pt.x,pt.y);
    }

    FIA_SaveFIBToFile(joined_image,  TEST_DATA_OUTPUT_DIR  "/Convolution/blood-vessel-two-image-join.png", BIT24);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(joined_image);

    return;
}

static FIBITMAP* Resize(FIBITMAP *fib)
{
    return FreeImage_Rescale(fib, FreeImage_GetWidth(fib) / 2, FreeImage_GetHeight(fib) / 2, FILTER_BOX);
}

static void
TestFIA_CorrelateBloodTissueImagesWithNoKnowledge(CuTest* tc)
{
    #define PREFIX "/home/glenn/TestData/4x6/"

    int width = 768;
    int height = 576;

    const char* files[] =
    {
            /*
        TEST_DATA_DIR "CorrelationSections/1.png",
        TEST_DATA_DIR "CorrelationSections/2.png",
        TEST_DATA_DIR "CorrelationSections/3.png",
        TEST_DATA_DIR "CorrelationSections/4.png",
        TEST_DATA_DIR "CorrelationSections/5.png",
        TEST_DATA_DIR "CorrelationSections/6.png",
        TEST_DATA_DIR "CorrelationSections/7.png",
        TEST_DATA_DIR "CorrelationSections/8.png",
        TEST_DATA_DIR "CorrelationSections/9.png",
        TEST_DATA_DIR "CorrelationSections/10.png",
        TEST_DATA_DIR "CorrelationSections/11.png",
        TEST_DATA_DIR "CorrelationSections/12.png",
        TEST_DATA_DIR "CorrelationSections/13.png",
        TEST_DATA_DIR "CorrelationSections/14.png",
*/

        TEST_DATA_DIR "CorrelationSections/9.png",
        TEST_DATA_DIR "CorrelationSections/2.png"

            //PREFIX "d9ob20_00002.bmp",
            //PREFIX "d9ob20_00003.bmp",
            //PREFIX "d9ob20_00005.bmp",
            //PREFIX "d9ob20_00007.bmp",
            //PREFIX "d9ob20_00006.bmp",
            //PREFIX "d9ob20_00008.bmp",
            //PREFIX "d9ob20_00009.bmp",
          //  PREFIX "d9ob20_00010.bmp",
          //  PREFIX "d9ob20_00011.bmp",
            //PREFIX "d9ob20_00012.bmp",
            //PREFIX "d9ob20_00013.bmp",
            //PREFIX "d9ob20_00014.bmp",
            //PREFIX "d9ob20_00015.bmp",
            //PREFIX "d9ob20_00016.bmp"
    };

    const int number_of_images = sizeof(files) / sizeof(char *);

    FIAPOINT *points = (FIAPOINT*) malloc(sizeof(FIAPOINT) * number_of_images);
    double factor = 0.0, max = 0.0;

    points[0].x = 0;
    points[1].y = 0;

    FIBITMAP *fib1 = NULL, *fib2 = NULL;
    //FIBITMAP *thumb1 = NULL, *thumb2 = NULL;

    FIAPOINT pt;

    PROFILE_START("TestFIA_CorrelateBloodTissueImagesWithNoKnowledge");

    for(int i=0; i < number_of_images -1; i++) {

        fib1 =  FIA_LoadFIBFromFile(files[i]);

     //   thumb1 = Resize(fib1);

     //   FreeImage_Unload(fib1);

        for(int j=i+1; j < number_of_images; j++) {

            fib2 =  FIA_LoadFIBFromFile(files[j]);

//            thumb2 = Resize(fib2);

  //          FreeImage_Unload(fib2);

            //FIA_FFTCorrelateImages(thumb1, thumb2, FIA_EdgeEnhancer, &pt);

            FIARECT rect1 = MakeFIARect(FreeImage_GetWidth(fib1) - 30, 0, FreeImage_GetWidth(fib1) - 1, FreeImage_GetHeight(fib1));
            //FIARECT rect1 = FIAImageRect (fib1);

            FIA_CorrelateImageEdgesWithImage(fib1, rect1, fib2,
                    20, CORRELATION_KERNEL, NULL, &pt);

            double factor;

            FIA_CorrelationDifferenceMeasure(fib1, fib2, pt, &factor);

            if(factor > max) {
                max = factor;
                points[j] = pt;
            }

        }

        std::cout << files[i] << std::endl;
    }

    PROFILE_STOP("TestFIA_CorrelateBloodTissueImagesWithNoKnowledge");

    int min_x = 99999999, min_y = 99999999, max_x = -99999999, max_y = -99999999;

    for(int i=0; i < number_of_images; i++) {

          if(points[i].x < min_x)
              min_x = points[i].x;

          if(points[i].y < min_y)
              min_y = points[i].y;

          if(points[i].x > max_x)
              max_x = points[i].x;

          if(points[i].y > max_y)
              max_y = points[i].y;
    }

    std::cout << "Min x:" << min_x << " Min y:" << min_y  << " Max x:" << max_x  << " Max y:" << max_y << std::endl;

    free(points);


    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(fib1),
               2 * FreeImage_GetWidth(fib1), 2 * FreeImage_GetHeight(fib1), FreeImage_GetBPP(fib1), 0, 0, 0);


    if(FIA_PasteFromTopLeft(joined_image, fib1, 0, 0) == 0) {
              printf("Paste failed for FIA_PasteFromTopLeft. Trying to paste at %d, %d\n", 0, 0);
          }

       if(FIA_PasteFromTopLeft(joined_image, fib2, pt.x, pt.y) == 0) {
           printf("Paste failed for FIA_PasteFromTopLeft. Trying to paste at %d, %d\n", pt.x,pt.y);
       }

       FIA_SaveFIBToFile(joined_image,  "/home/glenn/joined.png", BIT24);

    /*
    FIBITMAP *joined_image = FreeImage_AllocateT(FreeImage_GetImageType(fib1),
            2 * width, 2 * height, FreeImage_GetBPP(fib1), 0, 0, 0);

    FIAPOINT pt;

    FIARECT rect1 = MakeFIARect(0,0,width,height);
    FIARECT rect2 = MakeFIARect(-50, height - 300, width-50, 2 * height - 300);

    FIA_CorrelateImagesAroundOverlap(fib1, rect1, fib2, rect2, 100, CORRELATION_FFT, FIA_EdgeEnhancer, &pt);

    std::cout << "pt.x " << pt.x << " pt.y: " << pt.y << std::endl;

    if(FIA_PasteFromTopLeft(joined_image, fib1, 0, 0) == 0) {
        printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n", pt.x, pt.y);
    }

    if(FIA_PasteFromTopLeft(joined_image, fib2, pt.x, pt.y) == 0) {
        printf("Paste failed for TestFIA_CorrelateFFTAlongRightEdge. Trying to paste at %d, %d\n", pt.x,pt.y);
    }

    FIA_SaveFIBToFile(joined_image,  "/home/glenn/joined.png", BIT24);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(joined_image);
*/

    return;
}

static void HighlightCorners(FIBITMAP *fib)
{
    int width = FreeImage_GetWidth(fib);
    int height = FreeImage_GetHeight(fib);

    FIARECT rect;

    rect.left = 0;
    rect.top = 0;
    rect.right = 5;
    rect.bottom = 5;

    FIA_DrawColourSolidRect (fib, rect, FIA_RGBQUAD(255,0,0));

    rect.left = 0;
    rect.top = height - 6;
    rect.right = 5;
    rect.bottom = height - 1;

    FIA_DrawColourSolidRect (fib, rect, FIA_RGBQUAD(255,0,0));

    rect.left = width - 6;
    rect.top = 0;
    rect.right = width - 1;
    rect.bottom = 5;

    FIA_DrawColourSolidRect(fib, rect, FIA_RGBQUAD(255, 0, 0));

    rect.left = width - 6;
    rect.top = height - 6;
    rect.right = width - 1;
    rect.bottom = height - 1;

    FIA_DrawColourSolidRect(fib, rect, FIA_RGBQUAD(255, 0, 0));

}

static void
TestFIA_CorrelateEdgeTest(CuTest* tc)
{
    const char *file = TEST_DATA_DIR "/CorrelationSections/spice.jpg";

    const char *edges[] = {

        TEST_DATA_DIR "/CorrelationSections/spice-edge1.jpg",
        TEST_DATA_DIR "/CorrelationSections/spice-edge2.jpg",
        TEST_DATA_DIR "/CorrelationSections/spice-edge3.jpg",
        TEST_DATA_DIR "/CorrelationSections/spice-edge4.jpg",
        TEST_DATA_DIR "/CorrelationSections/spice-middle.jpg"
    };

    FIAPOINT pt;

    pt.x = 0;
    pt.y = 0;

    FIBITMAP *file_dib = NULL, *edge_dib = NULL;

    const int number_of_images = sizeof(edges) / sizeof(char *);

    int max_width = 0, max_height = 0;

    for(int i=0; i < number_of_images; i++) {

           edge_dib = FIA_LoadFIBFromFile(edges[i]);

           if(FreeImage_GetWidth(edge_dib) > max_width)
               max_width = FreeImage_GetWidth(edge_dib);

           if(FreeImage_GetHeight(edge_dib) > max_height)
               max_height = FreeImage_GetHeight(edge_dib);

           FreeImage_Unload(edge_dib);
       }

#ifdef WIN32
    int max_dimension = max(max_width, max_height);
#else
	int max_dimension = std::max(max_width, max_height);
#endif

    file_dib = FIA_LoadFIBFromFile(file);

        edge_dib = FIA_LoadFIBFromFile(edges[0]);

        FIBITMAP* fft = FIA_PreCalculateCorrelationFFT(file_dib, edge_dib, max_dimension, FIA_EdgeEnhancer);

        FreeImage_Unload(edge_dib);

    for(int i=0; i < number_of_images; i++) {

        edge_dib = FIA_LoadFIBFromFile(edges[i]);

        PROFILE_START("TestFIA_CorrelateEdgeTest");

        //FIA_CorrelateImages(file_dib, edge_dib, CORRELATION_FFT, FIA_EdgeEnhancer, &pt);

        FIA_FFTCorrelateImageWithPreCorrelationFFT(fft, file_dib, edge_dib, max_dimension, FIA_EdgeEnhancer, &pt);

        std::cout << pt.x << ", " << pt.y << std::endl;

        PROFILE_STOP("TestFIA_CorrelateEdgeTest");

        HighlightCorners(edge_dib);

        FIA_PasteFromTopLeft(file_dib, edge_dib, pt.x, pt.y);

        FreeImage_Unload(edge_dib);
    }

    FIA_SaveFIBToFile(file_dib, TEST_DATA_OUTPUT_DIR  "/Convolution/correlation-edge-result.png", BIT24);

    FreeImage_Unload(file_dib);

    return;
}


static void
TestFIA_GradientBlend(CuTest* tc)
{
    FIBITMAP *fib1 =  LoadTissueFile(TEST_DATA_DIR "BloodVessels/d9ob20_00009.png");
    FIBITMAP *fib2 =  LoadTissueFile(TEST_DATA_DIR "BloodVessels/d9ob20_00010.png");

    int width = FreeImage_GetWidth(fib1);
    int height = FreeImage_GetHeight(fib1);

    FIAPOINT pt;

    FIARECT rect1 = MakeFIARect(width-150,0,width-1,height-1);
    FIARECT rect2 = MakeFIARect(0, 0, 40, 40);

    FIA_CorrelateImageRegions(fib1, rect1, fib2, rect2, CORRELATION_FFT, FIA_EdgeEnhancer, &pt);

    rect1.left = 0;
    rect1.top = 0;
    rect1.right = width - 1;
    rect1.bottom = height - 1;

    rect2.left = pt.x;
    rect2.top = pt.y;
    rect2.right = rect2.left + width - 1;
    rect2.bottom = rect2.top + height - 1;

    PROFILE_START("FIA_GradientBlend");

    FIA_GradientBlend (fib1, rect1, fib2, rect2);

    PROFILE_STOP("FIA_GradientBlend");

    FIA_SaveFIBToFile(fib1, TEST_DATA_OUTPUT_DIR  "/Convolution/gradient_blended.png", BIT32);

    FreeImage_Unload(fib1);
}

static void
TestFIA_GradientBlendPasteTest(CuTest* tc)
{
    FIBITMAP *fib1 =  LoadTissueFile(TEST_DATA_DIR "BloodVessels/d9ob20_00009.png");
    FIBITMAP *fib2 =  LoadTissueFile(TEST_DATA_DIR "BloodVessels/d9ob20_00010.png");
    FIBITMAP *fib3 =  LoadTissueFile(TEST_DATA_DIR "jigsaw.png");

    PROFILE_START("FIA_GradientBlendPasteFromTopLeft");

    //FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 80, 50);

    FIA_GradientBlendPasteFromTopLeft (fib1, fib3, -1, 100);

    PROFILE_STOP("FIA_GradientBlendPasteFromTopLeft");

    FIA_SaveFIBToFile(fib1, TEST_DATA_OUTPUT_DIR  "/Convolution/gradient_blended_paste.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(fib3);
}

static void
TestFIA_GradientBlendFloatImagePasteTest(CuTest* tc)
{
    FIBITMAP *fib1 =  LoadTissueFile(TEST_DATA_DIR "BloodVessels/d9ob20_00009.png");
    FIBITMAP *fib2 =  LoadTissueFile(TEST_DATA_DIR "BloodVessels/d9ob20_00010.png");
    FIBITMAP *fib3 =  LoadTissueFile(TEST_DATA_DIR "jigsaw.png");

	FIA_InPlaceConvertToGreyscaleFloatType(&fib1, FIT_FLOAT);
	FIA_InPlaceConvertToGreyscaleFloatType(&fib2, FIT_FLOAT);
	FIA_InPlaceConvertToGreyscaleFloatType(&fib3, FIT_FLOAT);

    PROFILE_START("FIA_GradientBlendPasteFromTopLeft");

    //FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 80, 50);

    FIA_GradientBlendPasteFromTopLeft (fib1, fib3, -1, 100);

    PROFILE_STOP("FIA_GradientBlendPasteFromTopLeft");

    FIA_SaveFIBToFile(fib1, TEST_DATA_OUTPUT_DIR  "/Convolution/gradient_blended_float_paste.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(fib3);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsConvolutionSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/Convolution");

    //SUITE_ADD_TEST(suite, TestFIA_CorrelateBloodTissueImages);
    //SUITE_ADD_TEST(suite, TestFIA_CorrelateBloodTissueImagesTwoImages);
	//SUITE_ADD_TEST(suite, TestFIA_CorrelateBloodTissueImagesWithNoKnowledge);

    // Done
    //SUITE_ADD_TEST(suite, TestFIA_GradientBlend);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendFloatImagePasteTest);
	//SUITE_ADD_TEST(suite, TestFIA_CorrelateEdgeTest);
    //SUITE_ADD_TEST(suite, TestFIA_CorrelateSpiceSection);
    //SUITE_ADD_TEST(suite, TestFIA_CorrelateFFTTest);
	//SUITE_ADD_TEST(suite, TestFIA_CorrelateFFTLetterTest);
	//SUITE_ADD_TEST(suite, TestFIA_CorrelateRegionsTest);
	//SUITE_ADD_TEST(suite, TestFIA_SobelTest);
	//SUITE_ADD_TEST(suite, TestFIA_SobelAdvancedTest);
	//SUITE_ADD_TEST(suite, TestFIA_ConvolutionTest);
	//SUITE_ADD_TEST(suite, TestFIA_MedianFilterTest);
	//SUITE_ADD_TEST(suite, TestFIA_CorrelateFilterTest);

	return suite;
}
