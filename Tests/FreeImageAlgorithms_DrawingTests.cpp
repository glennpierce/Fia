#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFreeImageAlgorithms_GSLineTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river-gs.jpg";

	FIBITMAP *src = FreeImageAlgorithms_LoadFIBFromFile(file);
	CuAssertTrue(tc, src != NULL);

	POINT p1, p2;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;

	FreeImageAlgorithms_DrawGreyscaleLine (src, p1, p2, 150, 1);

	FreeImageAlgorithms_SaveFIBToFile(src, TEMP_DIR "\\wallpaper_line8bit.bmp", BIT8);

	FreeImage_Unload(src);
}

static void
TestFreeImageAlgorithms_ColourLineTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FreeImageAlgorithms_LoadFIBFromFile(file);

	FIBITMAP *src32 = FreeImage_ConvertTo32Bits(src);

	CuAssertTrue(tc, src != NULL);

	POINT p1, p2;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;

	FreeImageAlgorithms_DrawColourLine (src32, p1, p2, RGB(255, 0, 0), 5);

	FreeImageAlgorithms_SaveFIBToFile(src32, TEMP_DIR "\\wallpaper_line32.jpg", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src32);
}


static void
TestFreeImageAlgorithms_Rect24bitTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FreeImageAlgorithms_LoadFIBFromFile(file);

	FIBITMAP *src24 = FreeImage_ConvertTo24Bits(src);

	CuAssertTrue(tc, src != NULL);

	POINT p1, p2;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;

	RECT rect;
	rect.left = 0;
	rect.top = 100;
	rect.bottom = 500;
	rect.right = 500;
	
	FreeImageAlgorithms_DrawColourRect (src24, rect, RGB(255,0,0), 7);

	FreeImageAlgorithms_SaveFIBToFile(src24, TEMP_DIR "\\wallpaper_rect24bit.jpg", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src24);
}


static void
TestFreeImageAlgorithms_Rect32bitTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FreeImageAlgorithms_LoadFIBFromFile(file);

	FIBITMAP *src32 = FreeImage_ConvertTo32Bits(src);

	CuAssertTrue(tc, src != NULL);

	POINT p1, p2;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;

	RECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.bottom = 500;
	rect.right = 500;
	
	FreeImageAlgorithms_DrawColourRect (src32, rect, RGB(255,0,0), 7);

	FreeImageAlgorithms_SaveFIBToFile(src32, TEMP_DIR "\\wallpaper_rect32bit.jpg", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(src32);
}


static void
TestFreeImageAlgorithms_SolidRectTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, src != NULL);

	POINT p1, p2;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;

	RECT rect;
	rect.left = 100;
	rect.top = 100;
	rect.bottom = 500;
	rect.right = 500;
	
	FreeImageAlgorithms_DrawColourSolidRect (src, rect, RGB(255,0,0));

	FreeImageAlgorithms_SaveFIBToFile(src, TEMP_DIR "\\wallpaper_solidrect24bit.jpg", BIT24);

	FreeImage_Unload(src);
}

static void
TestFreeImageAlgorithms_SolidGSRectTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, src != NULL);

	FIBITMAP *gs_src = FreeImage_ConvertTo8Bits(src);

	CuAssertTrue(tc, gs_src != NULL);

	RECT rect;
	rect.left = 10;
	rect.top = 10;
	rect.bottom = 12;
	rect.right = 12;
	
	FreeImageAlgorithms_Draw8BitSolidGreyscaleRect (gs_src, rect, 100);

	FreeImageAlgorithms_SaveFIBToFile(gs_src, TEMP_DIR "\\wallpaper_solidrect8bit.jpg", BIT24);

	FreeImage_Unload(src);
	FreeImage_Unload(gs_src);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsDrawingSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_GSLineTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ColourLineTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_Rect24bitTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_SolidRectTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_SolidGSRectTest);

	return suite;
}
