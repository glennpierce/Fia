#include "CuTest.h"

#include "Constants.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFIA_IOSaveTest(CuTest* tc)
{
	int err;
	FIBITMAP *original_colour_dib = NULL, *greyscale_dib = NULL;
	FIBITMAP *float_dib = NULL, *bit16_dib = NULL;

	const char *file = TEST_DATA_DIR "red-kneed-tarantula.jpg";
	original_colour_dib = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, original_colour_dib != NULL);

	greyscale_dib = FreeImage_ConvertToGreyscale(original_colour_dib);

	CuAssertTrue(tc, greyscale_dib != NULL);

	float_dib = FIA_ConvertToGreyscaleFloatType(greyscale_dib, FIT_FLOAT);

	CuAssertTrue(tc, float_dib != NULL);

	bit16_dib = FIA_ConvertFloatTo16Bit(float_dib, 0);

	CuAssertTrue(tc, bit16_dib != NULL);

	err = FIA_SimpleSaveFIBToFile (original_colour_dib, TEST_DATA_OUTPUT_DIR "/IO/saved-colour-png.png");

	CuAssertTrue(tc, err == FIA_SUCCESS);

	err = FIA_SimpleSaveFIBToFile (bit16_dib, TEST_DATA_OUTPUT_DIR "/IO/saved-16bit-png.png");

	CuAssertTrue(tc, err == FIA_SUCCESS);

	err = FIA_SimpleSaveFIBToFile (bit16_dib, TEST_DATA_OUTPUT_DIR "/IO/saved-16bit-tif.tif");

	CuAssertTrue(tc, err == FIA_SUCCESS);

	err = FIA_SimpleSaveFIBToFile (float_dib, TEST_DATA_OUTPUT_DIR "/IO/saved-float-tif.tif");

	CuAssertTrue(tc, err == FIA_SUCCESS);

	err = FIA_SimpleSaveFIBToFile (float_dib, TEST_DATA_OUTPUT_DIR "/IO/saved-float-png.png");

	CuAssertTrue(tc, err == FIA_SUCCESS);

	FreeImage_Unload(original_colour_dib);
	FreeImage_Unload(greyscale_dib);
	FreeImage_Unload(float_dib);
	FreeImage_Unload(bit16_dib);
}

static void
TestFIA_IO(CuTest* tc)
{
	FIBITMAP *dib1 = NULL, *dib2 = NULL;
	FREE_IMAGE_TYPE type;
	int bpp, err;
    
    const char *file = TEST_DATA_DIR "drone-bee.jpg";
	dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	bpp = FreeImage_GetBPP(dib1);
	type = FreeImage_GetImageType(dib1);

	CuAssertTrue(tc, bpp == 24);
	CuAssertTrue(tc, type == FIT_BITMAP);
	
	err = FIA_SaveFIBToFile (dib1, TEST_DATA_OUTPUT_DIR "/IO/save-colour-test.bmp", BIT8);

	CuAssertTrue(tc, err == FIA_SUCCESS);

	dib2 = FIA_LoadFIBFromFile(TEST_DATA_OUTPUT_DIR "/IO/save-colour-test.bmp");

	err = FIA_BitwiseCompare(dib1, dib2);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
}

/*
static void
TestFIA_SaveBPPWithPalette(CuTest* tc)
{
	FIBITMAP *dib1 = NULL, *dib2 = NULL;
	FREE_IMAGE_TYPE type;
	int bpp, err;
    
    const char *file = TEST_DATA_DIR "fly.bmp";
	dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	bpp = FreeImage_GetBPP(dib1);
	type = FreeImage_GetImageType(dib1);

	CuAssertTrue(tc, bpp == 8);
	CuAssertTrue(tc, type == FIT_BITMAP);

	FIA_SetReverseRainBowPalette(dib1);
	err = FIA_SaveFIBToFile (dib1, TEST_DATA_OUTPUT_DIR "/IO/save-bmp-colour-test.bmp", BIT8);

	CuAssertTrue(tc, err == FIA_SUCCESS);

	FreeImage_Unload(dib1);
}

/*
static void
TestFIA_IOLoadColourArrayData(CuTest* tc)
{
	FIBITMAP *dib1 = NULL, *dib2 = NULL;
	FREE_IMAGE_TYPE type;
	int bpp, err;
    
    const char *file = TEST_DATA_DIR "lucy_pinder.jpg";
	dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	dib2 = FreeImage_AllocateT (FIT_BITMAP, FreeImage_GetWidth(dib1), FreeImage_GetHeight(dib1), 24, 0, 0, 0);

	PROFILE_START("CopyColourBytesToFIBitmap");

	for(int i=0; i < 500; i++) {

		CopyColourBytesToFIBitmap (dib2, FreeImage_GetBits(dib1), 0, 1, COLOUR_ORDER_RGB);
	}

	PROFILE_STOP("CopyColourBytesToFIBitmap");

	FIA_SaveFIBToFile (dib2, TEST_DATA_OUTPUT_DIR "/IO/save-colour-test.bmp", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
}
*/

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsIOSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/IO");

	SUITE_ADD_TEST(suite, TestFIA_IO);
//	SUITE_ADD_TEST(suite, TestFIA_IOLoadColourArrayData);
//	SUITE_ADD_TEST(suite, TestFIA_SaveBPPWithPalette);
	SUITE_ADD_TEST(suite, TestFIA_IOSaveTest);

	return suite;
}
