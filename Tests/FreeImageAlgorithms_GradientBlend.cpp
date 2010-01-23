#include "CuTest.h"

#include "Constants.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

#include <iostream>

/*
static void
TestFIA_IntersectingRect(CuTest* tc)
{
    FIARECT rect1 = MakeFIARect(505, 0, 1445, 583);
    FIARECT rect2 = MakeFIARect(0, 0, 1360, 1054);

	FIARECT intersection_rect;

	FIA_IntersectingRect(rect1, rect2, &intersection_rect);
   
	//CuAssertTrue(tc, dib != NULL);
}

static void
TestFIA_GetGradientBlendAlphaImageTest(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "BloodVessels/d9ob20_00009.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

	int left = -50;
	int top = 300;

    PROFILE_START("TestFIA_GetGradientBlendAlphaImageTest");

	FIARECT rect1 = MakeFIARect(0, 0, FreeImage_GetWidth(fib1) - 1, FreeImage_GetHeight(fib1) - 1);
	FIARECT rect2 = MakeFIARect(left, top, left + FreeImage_GetWidth(fib2) - 1, top + FreeImage_GetHeight(fib2) - 1);

	FIARECT intersect_rect;

    FIBITMAP *alpha = FIA_GetGradientBlendAlphaImage (fib2, rect1, rect2, &intersect_rect);

    PROFILE_STOP("TestFIA_GetGradientBlendAlphaImageTest");

    FIA_SaveFIBToFile(alpha, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_alpha_value.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
	FreeImage_Unload(alpha);
}

static void
TestFIA_GetGradientBlendAlphaImageTest2(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology1.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");
	FIBITMAP *fib3 = FreeImage_Rescale(fib2, 1360, 1024, FILTER_BOX);

    PROFILE_START("TestFIA_GetGradientBlendAlphaImageTest2");

	FIARECT rect1 = MakeFIARect(505, 0, 1445, 583);
	FIARECT rect2 = MakeFIARect(0, 0, 1360, 1024);

	FIARECT intersect_rect;

    FIBITMAP *alpha = FIA_GetGradientBlendAlphaImage (fib3, rect1, rect2, &intersect_rect);

    PROFILE_STOP("TestFIA_GetGradientBlendAlphaImageTest2");

    FIA_SaveFIBToFile(alpha, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_alpha_value_histology.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
	FreeImage_Unload(fib3);
	FreeImage_Unload(alpha);
}

static void
TestFIA_GetGradientBlendAlphaImageTest3(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology1.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");
	FIBITMAP *fib3 = FreeImage_Rescale(fib2, 1360, 1024, FILTER_BOX);

    PROFILE_START("TestFIA_GetGradientBlendAlphaImageTest2");

	FIARECT rect1 = MakeFIARect(505, 0, 1445, 583);
	FIARECT rect2 = MakeFIARect(600, 0, 1360, 1024);

	FIARECT intersect_rect;

    FIBITMAP *alpha = FIA_GetGradientBlendAlphaImage (fib3, rect1, rect2, &intersect_rect);

    PROFILE_STOP("TestFIA_GetGradientBlendAlphaImageTest2");

    FIA_SaveFIBToFile(alpha, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_alpha_value_histology2.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
	FreeImage_Unload(fib3);
	FreeImage_Unload(alpha);
}


static void
TestFIA_GetGradientBlendAlphaImageTest4(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology1.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");
    FIBITMAP *fib3 = FreeImage_Rescale(fib2, 1360, 1024, FILTER_BOX);

    PROFILE_START("TestFIA_GetGradientBlendAlphaImageTest4");

    FIARECT rect1 = MakeFIARect(505, 0, 1445, 583);
    FIARECT rect2 = MakeFIARect(600, 0, 1360, 1024);

    FIARECT intersect_rect;

    FIBITMAP *intersection_fib = FIA_GradientBlendedIntersectionImage (fib3, rect1, fib2, rect2, NULL, &intersect_rect);

    PROFILE_STOP("TestFIA_GetGradientBlendAlphaImageTest4");

    FIA_SaveFIBToFile(intersection_fib, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_intersection_fib.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(fib3);
    FreeImage_Unload(intersection_fib);
}

static void
TestFIA_GetGradientBlendAlphaImageListerHistologyTest(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "HistologyRS1.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "HistologyRS2.png");
   
    PROFILE_START("TestFIA_GetGradientBlendAlphaImageListerHistologyTest");

    FIARECT rect1 = MakeFIARect(0, 0, FreeImage_GetWidth(fib1), FreeImage_GetHeight(fib1));
    FIARECT rect2 = MakeFIARect(1224, 0, FreeImage_GetWidth(fib2), FreeImage_GetHeight(fib2));

    FIARECT intersect_rect;

    FIBITMAP *intersection_fib = FIA_GradientBlendedIntersectionImage (fib1, rect1, fib2, rect2, NULL, &intersect_rect);

    PROFILE_STOP("TestFIA_GetGradientBlendAlphaImageListerHistologyTest");

    FIA_SaveFIBToFile(intersection_fib, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_lister_histology_fib.png", BIT24);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(intersection_fib);
}

static void
TestFIA_GetGradientBlendAlphaImageTest5(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "drone-bee-left-blend.jpg");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "drone-bee-right-blend.jpg");

    PROFILE_START("TestFIA_GetGradientBlendAlphaImageTest5");

    FIARECT rect1 = MakeFIARect(0, 0, FreeImage_GetWidth(fib1) - 1, FreeImage_GetHeight(fib1) - 1);
    FIARECT rect2 = MakeFIARect(105, 0, FreeImage_GetWidth(fib2) - 1, FreeImage_GetHeight(fib2) - 1);

    FIARECT intersect_rect;

    FIBITMAP *intersection_fib = FIA_GradientBlendedIntersectionImage (fib1, rect1, fib2, rect2, NULL, &intersect_rect);

    PROFILE_STOP("TestFIA_GetGradientBlendAlphaImageTest5");

    FIA_SaveFIBToFile(intersection_fib, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_intersection_fib2.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(intersection_fib);
}

static void
TestFIA_GradientBlendPasteTest(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "BloodVessels/d9ob20_00009.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "BloodVessels/d9ob20_00010.png");
    FIBITMAP *fib3 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

    PROFILE_START("FIA_GradientBlendPasteFromTopLeft");

    //FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 80, 50);

    FIA_GradientBlendPasteFromTopLeft (fib1, fib3, -1, 100, NULL);

    PROFILE_STOP("FIA_GradientBlendPasteFromTopLeft");

    FIA_SaveFIBToFile(fib1, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_paste.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(fib3);
}

static void
TestFIA_GradientBlendPasteTest2(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "BloodVessels/d9ob20_00009.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

    PROFILE_START("TestFIA_GradientBlendPasteTest2");

    FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 10, 10, NULL);

    PROFILE_STOP("TestFIA_GradientBlendPasteTest2");

    FIA_SaveFIBToFile(fib1, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_paste2.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
}


static void
TestFIA_GradientBlendPasteTest3(CuTest* tc)
{
    FIBITMAP *fib1 = FreeImage_Allocate(2000,2000, 24, 0, 0, 0);
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

    PROFILE_START("TestFIA_GradientBlendPasteTest3");

    FIA_GradientBlendPasteFromTopLeft (fib1, fib2, -100, 10, NULL);
	FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 100, 500, NULL);
	FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 500, 1000, NULL);
	//FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 500, 500, NULL);

    PROFILE_STOP("TestFIA_GradientBlendPasteTest3");

    FIA_SaveFIBToFile(fib1, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_paste3.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
}

static void
TestFIA_GradientBlendPasteTest4(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "BloodVessels/d9ob20_00009.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

	int width = FreeImage_GetWidth(fib1);
	int height = FreeImage_GetHeight(fib1);

	FIBITMAP *mask = FreeImage_Allocate(width, height, 8, 0, 0, 0);

	int x, y;

	for(register int y = 0; y < height; y++)
	{
		BYTE *mask_ptr = (BYTE *) FIA_GetScanLineFromTop (mask, y);
	
		for(register int x = 0; x < width; x++)
		{
			if(y > x)
				mask_ptr[x] = 0;
			else
				mask_ptr[x] = 1;
		}
	}

    PROFILE_START("FIA_GradientBlendPasteFromTopLeft4");

    FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 80, 50, mask);

    PROFILE_STOP("FIA_GradientBlendPasteFromTopLeft4");

    FIA_SaveFIBToFile(fib1, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_paste4.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
}

static void
TestFIA_GradientBlendPasteTest5(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "drone-bee-left-blend.jpg");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "drone-bee-right-blend.jpg");

	int bpp = FreeImage_GetBPP(fib1);
	int width = FreeImage_GetWidth(fib1);
	int height = FreeImage_GetHeight(fib1);

	int background_width = width * 2;
	int background_height = height * 2;

	FIBITMAP *background = FreeImage_Allocate(background_width, background_height, bpp, 0, 0, 0);
	FIBITMAP *mask = FreeImage_Allocate(background_width, background_height, 8, 0, 0, 0);

	//
    PROFILE_START("FIA_GradientBlendPasteFromTopLeft4");

	FIA_GradientBlendPasteFromTopLeft (background, fib1, 0, 0, mask);
	FIA_DrawSolidGreyscaleRect(mask, MakeFIARect(0,0, width -1, height - 1), 250.0);
    FIA_GradientBlendPasteFromTopLeft (background, fib2, 105, 0, mask);

    PROFILE_STOP("FIA_GradientBlendPasteFromTopLeft4");

    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_mask_test.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
}

*/

static void
TestFIA_GradientBlendPasteTest6(CuTest* tc)
{
    FIBITMAP *background = FreeImage_Allocate(2000,2000, 24, 0, 0, 0);
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "HistologyRS1.png");
    FIBITMAP *fib3 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "HistologyRS2.png");

    FIA_InPlaceConvertTo24Bit(&fib2);
    FIA_InPlaceConvertTo24Bit(&fib3);

    std::cout << FreeImage_GetBPP(fib2) << std::endl;

    int width = FreeImage_GetWidth(fib2);

    FIA_GradientBlendPasteFromTopLeft (background, fib2, fib3, 0, 0, width - 300, 0);
    
    //FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 0, 0, NULL);
    //FIA_GradientBlendPasteFromTopLeft (fib1, fib2, width - 100, 0, NULL);
    //FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 2 * width - 200, 0, NULL);

    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_paste6.png", BIT32);

    FreeImage_Unload(background);
    FreeImage_Unload(fib2);
}

/*

static void
TestFIA_GradientBlendFloatImagePasteTest(CuTest* tc)
{
    FIBITMAP *fib1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "BloodVessels/d9ob20_00009.png");
    FIBITMAP *fib2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "BloodVessels/d9ob20_00010.png");
    FIBITMAP *fib3 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

	FIA_InPlaceConvertToGreyscaleFloatType(&fib1, FIT_FLOAT);
	FIA_InPlaceConvertToGreyscaleFloatType(&fib2, FIT_FLOAT);
	FIA_InPlaceConvertToGreyscaleFloatType(&fib3, FIT_FLOAT);

    PROFILE_START("FIA_GradientBlendPasteFromTopLeft");

    //FIA_GradientBlendPasteFromTopLeft (fib1, fib2, 80, 50);

    FIA_GradientBlendPasteFromTopLeft (fib1, fib3, -1, 100, NULL);

    PROFILE_STOP("FIA_GradientBlendPasteFromTopLeft");

    FIA_SaveFIBToFile(fib1, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended_float_paste.png", BIT32);

    FreeImage_Unload(fib1);
    FreeImage_Unload(fib2);
    FreeImage_Unload(fib3);
}
*/

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsGradientBlendSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/GradientBlending");

    /*
	SUITE_ADD_TEST(suite, TestFIA_IntersectingRect);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest6);
    SUITE_ADD_TEST(suite, TestFIA_GradientBlend);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest2);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest3);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest4);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendFloatImagePasteTest);
	SUITE_ADD_TEST(suite, TestFIA_GetGradientBlendAlphaImageTest);	
	SUITE_ADD_TEST(suite, TestFIA_GetGradientBlendAlphaImageTest2);
	SUITE_ADD_TEST(suite, TestFIA_GetGradientBlendAlphaImageTest3);
    SUITE_ADD_TEST(suite, TestFIA_GetGradientBlendAlphaImageTest4);
    SUITE_ADD_TEST(suite, TestFIA_GetGradientBlendAlphaImageTest5);
	SUITE_ADD_TEST(suite, TestFIA_GetGradientBlendAlphaImageListerHistologyTest);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest5);
    */
    
    SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest6);

	return suite;
}
