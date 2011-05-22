#include "CuTest.h"

#include "Constants.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Colour.h"
#include "FreeImageAlgorithms_IO.h"

#include <sys/stat.h>

static int MkDir(const char *path)
{
	#ifdef WIN32
	return CreateDirectory(path, NULL);
	#else
	return mkdir(path, 0777);
	#endif
}


static void
TestFIA_ColourFunctions(CuTest* tc)
{
	unsigned char red = 165, green = 202, blue = 68;
	double hue, satuation, value;

	FIA_RGBToHSV(red, green, blue, &hue, &satuation, &value);

	CuAssertTrue(tc, (int)(hue + 0.5) == 77);
	CuAssertTrue(tc, (int)(satuation * 100 + 0.5) == 66);
	CuAssertTrue(tc, (int)(value * 100 + 0.5) == 79);

	FIA_HSVToRGB(hue, satuation, value, &red, &green, &blue);

	CuAssertTrue(tc, red == 165);
	CuAssertTrue(tc, green == 201);
	CuAssertTrue(tc, blue == 68);
}

static void
TestFIA_ColourExtract(CuTest* tc)
{
    const char *file = TEST_DATA_DIR "bumblebee.jpg";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *R=NULL, *G=NULL, *B=NULL, *newdib=NULL;

	int error = FIA_ExtractColourPlanes(dib1, &R, &G, &B);

	error = FIA_ReplaceColourPlanes(&newdib, G, R, B);
	CuAssertTrue(tc, error == FIA_SUCCESS);
	FIA_SimpleSaveFIBToFile(newdib, TEST_DATA_OUTPUT_DIR "/Colour/NewColourReplaced.bmp"); 

	error = FIA_ReplaceColourPlanes(&dib1, R, B, NULL);
	CuAssertTrue(tc, error == FIA_SUCCESS);
	FIA_SimpleSaveFIBToFile(dib1, TEST_DATA_OUTPUT_DIR "/Colour/OldColourReplaced.bmp"); 

	FreeImage_Unload(dib1);
	FreeImage_Unload(newdib);
	FreeImage_Unload(B);
	FreeImage_Unload(G);
	FreeImage_Unload(R);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsColourSuite(void)
{
	CuSuite* suite = CuSuiteNew();
	
	MkDir(TEST_DATA_OUTPUT_DIR "/Colour");

	//SUITE_ADD_TEST(suite, TestFIA_ColourFunctions);
	SUITE_ADD_TEST(suite, TestFIA_ColourExtract);

	return suite;
}
