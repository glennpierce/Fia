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

static void
TestFIA_GradientBlendPasteTest1(CuTest* tc)
{
    FIBITMAP *background =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology_gradient_dest.png");
    FIBITMAP *src =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw-src.png");

    FIA_InPlaceConvertTo24Bit(&background);
    FIA_InPlaceConvertTo24Bit(&src);

	FIA_GradientBlendMosaicPaste (background, src, 780, 620);
    
    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended1.png", BIT24);

    FreeImage_Unload(background);
    FreeImage_Unload(src);
}

static void
TestFIA_GradientBlendPasteTest2(CuTest* tc)
{
    FIBITMAP *background =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology_gradient_dest2.png");
    FIBITMAP *src =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw2.png");

    FIA_InPlaceConvertTo24Bit(&background);
    FIA_InPlaceConvertTo24Bit(&src);

    FIA_GradientBlendMosaicPaste (background, src, 780, 525);
   
    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended2.png", BIT24);

    FreeImage_Unload(background);
    FreeImage_Unload(src);
}

static void
TestFIA_GradientBlendPasteTest3(CuTest* tc)
{
    FIBITMAP *background =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology_gradient_dest.png");
    FIBITMAP *src =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw-src.png");

    FIA_InPlaceConvertTo8Bit(&background);
    FIA_InPlaceConvertTo8Bit(&src);

    FIA_GradientBlendMosaicPaste (background, src, 780, 620);
    
    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended3.png", BIT24);

    FreeImage_Unload(background);
    FreeImage_Unload(src);
}

static void
TestFIA_GradientBlendPasteTest4(CuTest* tc)
{
    FIBITMAP *background =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology_gradient_dest.png");
    FIBITMAP *src =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw-src.png");

    FIA_InPlaceConvertToGreyscaleFloatType(&background, FIT_FLOAT);
    FIA_InPlaceConvertToGreyscaleFloatType(&src, FIT_FLOAT);

    FIA_GradientBlendMosaicPaste (background, src, 780, 620);
    
    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended4.png", BIT24);

    FreeImage_Unload(background);
    FreeImage_Unload(src);
}

static void
TestFIA_GradientBlendPasteTest5(CuTest* tc)
{
    FIBITMAP *background =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology_gradient_dest4.png");
    FIBITMAP *src =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

    FIA_InPlaceConvertTo24Bit(&background);
    FIA_InPlaceConvertTo24Bit(&src);

    FIA_GradientBlendMosaicPaste (background, src, 1050, 400);
    
    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended5.png", BIT24);

    FreeImage_Unload(background);
    FreeImage_Unload(src);
}

static void
TestFIA_GradientBlendPasteTest6(CuTest* tc)
{
    FIBITMAP *background =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology_gradient_dest5.png");
    FIBITMAP *src =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

    FIA_InPlaceConvertTo24Bit(&background);
    FIA_InPlaceConvertTo24Bit(&src);

    FIA_GradientBlendMosaicPaste (background, src, -300, 170);
    
    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended6.png", BIT24);

    FreeImage_Unload(background);
    FreeImage_Unload(src);
}

static void
TestFIA_GradientBlendPasteTest7(CuTest* tc)
{
    FIBITMAP *background =  FIA_LoadFIBFromFile(TEST_DATA_DIR "histology_gradient_dest5.png");
    FIBITMAP *src =  FIA_LoadFIBFromFile(TEST_DATA_DIR "jigsaw.png");

    FIA_InPlaceConvertTo24Bit(&background);
    FIA_InPlaceConvertTo24Bit(&src);

    FIA_GradientBlendMosaicPaste (background, src, -300, 300);
    
    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended7.png", BIT24);

    FreeImage_Unload(background);
    FreeImage_Unload(src);
}


static void
TestFIA_GradientBlendPasteTest8(CuTest* tc)
{
    FIBITMAP *src1 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "slide12bit1.png");
    FIBITMAP *src2 =  FIA_LoadFIBFromFile(TEST_DATA_DIR "slide12bit2.png");

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src1); 
	int bpp = FreeImage_GetBPP(src1);
	int width = FreeImage_GetWidth(src1);
	int height = FreeImage_GetHeight(src1); 

	FIBITMAP *background = FreeImage_AllocateT(type, width * 2, height, bpp, 0, 0, 0);

	FIA_PasteFromTopLeft(background, src1, 0, 0);
    FIA_GradientBlendMosaicPaste (background, src2, 922, 0);
    
    FIA_SaveFIBToFile(background, TEST_DATA_OUTPUT_DIR  "/GradientBlending/gradient_blended8.png", BIT16);

    FreeImage_Unload(background);
    FreeImage_Unload(src1);
	FreeImage_Unload(src2);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsGradientBlendSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/GradientBlending");

	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest1);
    SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest2);
    SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest3);
    SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest4);
    SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest5);
    SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest6);
    SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest7);
	SUITE_ADD_TEST(suite, TestFIA_GradientBlendPasteTest8);

	return suite;
}
