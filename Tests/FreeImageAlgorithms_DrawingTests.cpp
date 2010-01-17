#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFIA_GreyscaleElipseTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);

    FIARECT rect, rect2;
	rect.left = 50;
	rect.top = 100;
	rect.bottom = 200;
	rect.right = 200;

    rect2.left = 200;
	rect2.top = 100;
	rect2.bottom = 200;
	rect2.right = 300;

    FIA_DrawSolidGreyscaleEllipse (src, rect, 200, 1);
    FIA_DrawSolidGreyscaleEllipse (src, rect2, 200, 0);
    FIA_DrawGreyscaleRect (src, rect, 200, 2);

	FIA_SaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_GreyscaleElipseTest.bmp", BIT8);

	FreeImage_Unload(src);
}


static void
TestFIA_GreyscaleU16bitElipseTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);

    FIBITMAP *src2 = FreeImage_ConvertToType(src, FIT_UINT16, 1);

    FIARECT rect;

    rect.left = 100;
	rect.top = 100;
	rect.bottom = 200;
	rect.right = 300;

    FIA_DrawSolidGreyscaleEllipse (src2, rect, 200, 1);
 
	FIA_SaveFIBToFile(src2, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_GreyscaleU16bitElipseTest.bmp", BIT8);

	FreeImage_Unload(src);
    FreeImage_Unload(src2);
}

static void
TestFIA_ColourElipseTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "bumblebee.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);

    FIARECT rect, rect2;
	rect.left = 50;
	rect.top = 100;
	rect.bottom = 200;
	rect.right = 200;

    rect2.left = 200;
	rect2.top = 100;
	rect2.bottom = 200;
	rect2.right = 300;

    FIA_DrawColourSolidEllipse (src, rect, FIA_RGBQUAD(0, 255, 0), 1);
    FIA_DrawColourSolidEllipse (src, rect2, FIA_RGBQUAD(0, 0, 255), 0);
   
	FIA_SaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_ColourElipseTest.bmp", BIT24);

	FreeImage_Unload(src);
}

static void
TestFIA_ConvexHullTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "particle.bmp";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);
	
    FIBITMAP *hull_dib = FreeImage_ConvexHull (src);

    CuAssertTrue(tc, hull_dib != NULL);

	FIA_SaveFIBToFile(hull_dib, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_ConvexHullTest.bmp", BIT8);

	FreeImage_Unload(src);
    FreeImage_Unload(hull_dib);
}

static void
TestFIA_ColourPolygonTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "fly.bmp";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);
	
    FIBITMAP *dst = FreeImage_ConvertTo24Bits(src);

    FIAPOINT poly_array[4];

    poly_array[0].x = 10;
    poly_array[0].y = 10;
    poly_array[1].x = 30;
    poly_array[1].y = 200;
    poly_array[2].x = 100;
    poly_array[2].y = 80;
    poly_array[3].x = 60;
    poly_array[3].y = 10;  

    FIA_DrawColourSolidPolygon (dst, poly_array, 4, FIA_RGBQUAD(0, 255, 0), 1);

    CuAssertTrue(tc, dst != NULL);

	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_ColourPolygonTest.bmp", BIT24);

	FreeImage_Unload(src);
    FreeImage_Unload(dst);
}

static void
TestFIA_GreyscalePolygonTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);
	
    FIAPOINT poly_array[4];

    poly_array[0].x = 10;
    poly_array[0].y = 10;
    poly_array[1].x = 30;
    poly_array[1].y = 200;
    poly_array[2].x = 100;
    poly_array[2].y = 80;
    poly_array[3].x = 60;
    poly_array[3].y = 10;  

    FIA_DrawSolidGreyscalePolygon (src, poly_array, 4, 255, 1);

    CuAssertTrue(tc, src != NULL);

	FIA_SaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_GreyscalePolygonTest.bmp", BIT24);

	FreeImage_Unload(src);
}

static void
TestFIA_GSLineTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);

	FIAPOINT p1, p2;

	p1.x = 10;
	p1.y = 10;
    	p2.x = p1.x;
    	p2.y = 200;

    	FIA_DrawGreyscaleLine (src, p1, p2, 150, 3, 0);
 
	FIA_SaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_GSLineTest.jpg", BIT8);

	FreeImage_Unload(src);
}

static void
TestFIA_Colour32bitLineTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "bumblebee.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	FIBITMAP *src32 = FreeImage_ConvertTo32Bits(src);

	CuAssertTrue(tc, src != NULL);

	FIAPOINT p1, p2, p3;

	p1.x = 10;
	p1.y = 10;
	p2.x = 200;
	p2.y = 300;
	p3.x = 300;
	p3.y = 300;
    
	FIA_DrawColourLine (src32, p1, p2, FIA_RGBQUAD(255, 0, 0), 5, 0);
        FIA_DrawColourLine (src32, p1, p3, FIA_RGBQUAD(0, 0, 255), 8, 1);

	FIA_SaveFIBToFile(src32, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_Colour32bitLineTest.jpg", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src32);
}

static void
TestFIA_Colour24bitLineTest(CuTest* tc)
{
        const char *file = TEST_DATA_DIR "bumblebee.jpg";

        FIBITMAP *src = FIA_LoadFIBFromFile(file);

        FIBITMAP *src24 = FreeImage_ConvertTo24Bits(src);

        CuAssertTrue(tc, src != NULL);

        FIAPOINT p1, p2, p3;

        p1.x = 10;
        p1.y = 10;
        p2.x = 200;
        p2.y = 300;

        FIA_DrawColourLine (src24, p1, p2, FIA_RGBQUAD(255, 0, 0), 5, 0);

        FIA_SaveFIBToFile(src24, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_Colour24BitLineTest.jpg", BIT24);

        FreeImage_Unload(src);
        FreeImage_Unload(src24);
}

static void
TestFIA_Rect24bitTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	FIBITMAP *src24 = FreeImage_ConvertTo24Bits(src);

	CuAssertTrue(tc, src != NULL);

	FIARECT rect;
	rect.left = 165;
	rect.top = 118;
	rect.bottom = 156;
	rect.right = 200;
	
	FIA_DrawColourRect (src24, rect, FIA_RGBQUAD(255,0,0), 1);

	FIA_SaveFIBToFile(src24, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_Rect24bitTest.bmp", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src24);
}

static void
TestFIA_Rect32bitTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "bumblebee.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	FIBITMAP *src32 = FreeImage_ConvertTo32Bits(src);

	CuAssertTrue(tc, src != NULL);

	FIARECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.bottom = 200;
	rect.right = 200;
	
	FIA_DrawColourRect (src32, rect, FIA_RGBQUAD(255,0,0), 7);

	FIA_SaveFIBToFile(src32, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_Rect32bitTest.jpg", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src32);
}

static void
TestFIA_GsRectTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, src != NULL);

	FIARECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.bottom = 200;
	rect.right = 200;
	
	FIA_DrawGreyscaleRect (src, rect, 200, 5);

	FIA_SaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_GsRectTest.jpg", BIT8);

	FreeImage_Unload(src);
}

static void
TestFIA_SolidRectTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "bumblebee.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, src != NULL);

	FIARECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.bottom = 200;
	rect.right = 200;
	
	FIA_DrawColourSolidRect (src, rect, FIA_RGBQUAD(255,0,0));

	FIA_SaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_SolidRectTest.jpg", BIT24);

	FreeImage_Unload(src);
}


static void
TestFIA_SolidGSRectTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "drone-bee-greyscale.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, src != NULL);

	FIBITMAP *gs_src = FreeImage_ConvertTo8Bits(src);

	CuAssertTrue(tc, gs_src != NULL);

	FIARECT rect;
	rect.left = 10;
	rect.top = 10;
	rect.bottom = 200;
	rect.right = 200;

	FIA_DrawSolidGreyscaleRect (gs_src, rect, 100);

	FIA_SaveFIBToFile(gs_src, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_SolidGSRectTest.jpg", BIT8);
  
	FreeImage_Unload(src);
	FreeImage_Unload(gs_src);
}

static void
TestFIA_ColourTextTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "fly.bmp";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);
	
    FIBITMAP *dst = FreeImage_ConvertTo24Bits(src);

    FIA_DrawHorizontalColourText (dst, 10, 10, "A quick brown fox jumps over the lazy dog 0123456789", FIA_RGBQUAD(0, 255, 0));

    CuAssertTrue(tc, dst != NULL);

	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_ColourTextTest.bmp", BIT24);

	FreeImage_Unload(src);
    FreeImage_Unload(dst);
}

static void
TestFIA_GreyscaleTextTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "fly.bmp";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);
	
    FIA_DrawHorizontalGreyscaleText (src, 10, 10, "A quick brown fox jumps over the lazy dog 0123456789", 0);

    CuAssertTrue(tc, src != NULL);

	FIA_SaveFIBToFile(src, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_GreyscaleTextTest.bmp", BIT8);

	FreeImage_Unload(src);
}

static void
TestFIA_FloodFillTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "particle.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	FIBITMAP *dib3 = FIA_FloodFill(dib2, 20, 1, 255);

    FIA_SetGreyLevelPalette(dib3);
    
	CuAssertTrue(tc, dib3 != NULL);

	FIA_SaveFIBToFile(dib3, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_FloodFillTest.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}

/*
static int FIA_DrawImage(FIBITMAP *dst, FIBITMAP *src, FIARECT dst_rect)
{
    int dst_width = FIA_RectWidth (dst_rect);
    int dst_height = FIA_RectHeight (dst_rect);

    int src_width = FreeImage_GetWidth(src);
    int src_height = FreeImage_GetHeight(src);

    float scale_factor_x = (float) dst_width / src_width;
    float scale_factor_y = (float) dst_height / src_height;

    int src_size_x_to_resize =  FreeImage_GetWidth(dst) / scale_factor_x;

    if(src_size_x_to_resize <= 1)
        return FIA_ERROR;

    int src_size_y_to_resize =  FreeImage_GetHeight(dst) / scale_factor_y;

    if(src_size_y_to_resize <= 1)
        return FIA_ERROR;

    FIBITMAP *section = FIA_Copy(src, 0, 0, src_size_x_to_resize, src_size_y_to_resize);

    FreeImage_Rescale(section,
}
*/

/*
static void
TestFIA_AffineTransformTest(CuTest* tc)
{
        const char *file = TEST_DATA_DIR "fly.bmp";

        FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

        CuAssertTrue(tc, dib1 != NULL);

        FIBITMAP *dib3 = FIA_FloodFill(dib2, 20, 1, 255);

        FIA_SetGreyLevelPalette(dib3);

        CuAssertTrue(tc, dib3 != NULL);

        FIA_SaveFIBToFile(dib3, TEST_DATA_OUTPUT_DIR "Drawing/TestFIA_FloodFillTest.jpg", BIT24);

        FreeImage_Unload(dib1);
        FreeImage_Unload(dib2);
        FreeImage_Unload(dib3);
}
*/

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsDrawingSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/Drawing");

	SUITE_ADD_TEST(suite, TestFIA_Colour24bitLineTest);
	SUITE_ADD_TEST(suite, TestFIA_Colour32bitLineTest);
	SUITE_ADD_TEST(suite, TestFIA_GSLineTest);
	SUITE_ADD_TEST(suite, TestFIA_Rect24bitTest);
	SUITE_ADD_TEST(suite, TestFIA_Rect32bitTest);
	SUITE_ADD_TEST(suite, TestFIA_GsRectTest);
    SUITE_ADD_TEST(suite, TestFIA_SolidGSRectTest);
    SUITE_ADD_TEST(suite, TestFIA_SolidRectTest);
    SUITE_ADD_TEST(suite, TestFIA_GreyscaleU16bitElipseTest);
    SUITE_ADD_TEST(suite, TestFIA_GreyscaleElipseTest);
    SUITE_ADD_TEST(suite, TestFIA_ColourElipseTest);
    SUITE_ADD_TEST(suite, TestFIA_ConvexHullTest);
    SUITE_ADD_TEST(suite, TestFIA_GreyscalePolygonTest);
    SUITE_ADD_TEST(suite, TestFIA_ColourPolygonTest);
    SUITE_ADD_TEST(suite, TestFIA_ColourTextTest);
    SUITE_ADD_TEST(suite, TestFIA_GreyscaleTextTest);
    SUITE_ADD_TEST(suite, TestFIA_FloodFillTest);

    // SUITE_ADD_TEST(suite, TestFIA_AffineTransformTest);

	return suite;
}
