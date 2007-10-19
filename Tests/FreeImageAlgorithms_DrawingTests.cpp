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
	char *file = IMAGE_DIR "\\wallpaper_river-gs.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);

    FIARECT rect;
	rect.left = 50;
	rect.top = 100;
	rect.bottom = 500;
	rect.right = 500;

    FIA_DrawSolidGreyscaleEllipse (src, rect, 200, 1);

    FIA_DrawGreyscaleRect (src, rect, 200, 2);

	FIA_SaveFIBToFile(src, TEMP_DIR "\\wallpaper_river-gs-elipse.bmp", BIT8);

	FreeImage_Unload(src);
}

static void
TestFIA_ConvexHullTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\particle-test.bmp";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);

    PROFILE_START("ConvexHull");

	FIBITMAP *hull_dib = FreeImage_ConvexHull (src);

    PROFILE_STOP("ConvexHull");

    CuAssertTrue(tc, hull_dib != NULL);

	FIA_SaveFIBToFile(hull_dib, TEMP_DIR "\\convexhull.bmp", BIT8);

	FreeImage_Unload(src);
    FreeImage_Unload(hull_dib);
}

static void
TestFIA_GSLineTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river-gs.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);

	FIAPOINT p1, p2, p3, p4;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;
	p3.x = 500;
	p3.y = 10;
    p4.x = p1.x;
    p4.y = 30;

	//FIA_DrawGreyscaleLine (src, p1, p2, 150, 2, 0);

	//FIA_DrawGreyscaleLine (src, p1, p3, 150, 1, 1);

    FIA_DrawGreyscaleLine (src, p1, p4, 150, 1, 0);
 
	FIA_SaveFIBToFile(src, TEMP_DIR "\\wallpaper_line8bit.bmp", BIT8);

	FreeImage_Unload(src);
}

static void
TestFIA_ColourLineTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	FIBITMAP *src32 = FreeImage_ConvertTo32Bits(src);

	CuAssertTrue(tc, src != NULL);

	FIAPOINT p1, p2, p3;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;
    p3.x = 500;
	p3.y = 600;

    
	FIA_DrawColourLine (src32, p1, p2, FIA_RGBQUAD(255, 0, 0), 5, 0);

    FIA_DrawColourLine (src32, p1, p3, FIA_RGBQUAD(255, 0, 0), 5, 1);


    

	FIA_SaveFIBToFile(src32, TEMP_DIR "\\wallpaper_line32.bmp", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src32);
}


static void
TestFIA_Rect24bitTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	FIBITMAP *src24 = FreeImage_ConvertTo24Bits(src);

	CuAssertTrue(tc, src != NULL);

	FIARECT rect;
	rect.left = 0;
	rect.top = 100;
	rect.bottom = 500;
	rect.right = 500;
	
	FIA_DrawColourRect (src24, rect, FIA_RGBQUAD(255,0,0), 7);

	FIA_SaveFIBToFile(src24, TEMP_DIR "\\wallpaper_rect24bit.bmp", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src24);
}


static void
TestFIA_Rect32bitTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	FIBITMAP *src32 = FreeImage_ConvertTo32Bits(src);

	CuAssertTrue(tc, src != NULL);

	FIARECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.bottom = 500;
	rect.right = 500;
	
	FIA_DrawColourRect (src32, rect, FIA_RGBQUAD(255,0,0), 7);

	FIA_SaveFIBToFile(src32, TEMP_DIR "\\wallpaper_rect32bit.bmp", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src32);
}

static void
TestFIA_GsRectTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river-gs.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, src != NULL);

	FIAPOINT p1, p2;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;

	FIARECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.bottom = 500;
	rect.right = 500;
	
	FIA_DrawGreyscaleRect (src, rect, 200, 5);

	FIA_SaveFIBToFile(src, TEMP_DIR "\\wallpaper_gsrect.bmp", BIT8);

	FreeImage_Unload(src);
}

static void
TestFIA_SolidRectTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, src != NULL);

	FIARECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.bottom = 500;
	rect.right = 500;
	
	FIA_DrawColourSolidRect (src, rect, FIA_RGBQUAD(255,0,0));

	FIA_SaveFIBToFile(src, TEMP_DIR "\\wallpaper_solidrect24bit.bmp", BIT24);

	FreeImage_Unload(src);
}

static void
TestFIA_SolidGSRectTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, src != NULL);

	FIBITMAP *gs_src = FreeImage_ConvertTo8Bits(src);

	CuAssertTrue(tc, gs_src != NULL);

	FIARECT rect;
	rect.left = 10;
	rect.top = 10;
	rect.bottom = 500;
	rect.right = 500;

	FIA_DrawSolidGreyscaleRect (gs_src, rect, 100);

	FIA_SaveFIBToFile(gs_src, TEMP_DIR "\\wallpaper_solidrect8bit.bmp", BIT8);
  
	FreeImage_Unload(src);
	FreeImage_Unload(gs_src);
}

static void
TestFIA_FloodFillTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\particle-test.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);

	CuAssertTrue(tc, dib2 != NULL);

	PROFILE_START("FloodFillTest");

	FIBITMAP *dib3 = FIA_FloodFill(dib2, 20, 0, 2);

	PROFILE_STOP("FloodFillTest");

	CuAssertTrue(tc, dib3 != NULL);

	FIA_SaveFIBToFile(dib3, TEMP_DIR "\\floodfill.jpg", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsDrawingSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFIA_GSLineTest);
	SUITE_ADD_TEST(suite, TestFIA_ColourLineTest);
	SUITE_ADD_TEST(suite, TestFIA_Rect24bitTest);
    SUITE_ADD_TEST(suite, TestFIA_GsRectTest);
	SUITE_ADD_TEST(suite, TestFIA_SolidRectTest);
	SUITE_ADD_TEST(suite, TestFIA_SolidGSRectTest);
	SUITE_ADD_TEST(suite, TestFIA_FloodFillTest);
    SUITE_ADD_TEST(suite, TestFIA_ConvexHullTest);
    SUITE_ADD_TEST(suite, TestFIA_GreyscaleElipseTest);

	return suite;
}
