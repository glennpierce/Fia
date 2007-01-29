#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFreeImageAlgorithms_LineTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\wallpaper_river.jpg";

	FIBITMAP *src = FreeImageAlgorithms_LoadFIBFromFile(file);

	//FIBITMAP *src32 = FreeImage_ConvertTo32Bits(src);

	CuAssertTrue(tc, src != NULL);

	POINT p1, p2;

	p1.x = 10;
	p1.y = 10;
	p2.x = 500;
	p2.y = 800;

	FreeImageAlgorithms_DrawColourLine (src, p1, p2, RGB(255, 0, 0), 5);

	FreeImageAlgorithms_SaveFIBToFile(src, TEMP_DIR "\\wallpaper_line.jpg", BIT24);

	FreeImage_Unload(src);
	//FreeImage_Unload(src32);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsDrawingSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_LineTest);

	return suite;
}
