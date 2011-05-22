#include "CuTest.h"

#include "Constants.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "FreeImageAlgorithms_Testing.h"

char bitdepths[10][20] = {
							"8bit",
							"16bit",
							"24bit",
							"32bit"
						 };

static void
TestFIA_IOSave8Bit(CuTest* tc, FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH bit_depth, const char *extension)
{
	int err;
	char filepath[500];
	FIBITMAP *original_colour_dib = NULL, *greyscale_dib = NULL;
	FIBITMAP *float_dib = NULL, *bit16_dib = NULL;

	const char *file = TEST_DATA_DIR "red-kneed-tarantula.jpg";
	original_colour_dib = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, original_colour_dib != NULL);

	greyscale_dib = FreeImage_ConvertTo8Bits(original_colour_dib);

	CuAssertTrue(tc, greyscale_dib != NULL);

	if(bit_depth != BIT_NONE) {
		sprintf(filepath, TEST_DATA_OUTPUT_DIR "/IO/ForcedSave/8bitForcedTo%s.%s", bitdepths[bit_depth], extension);
		err = FIA_SaveFIBToFile (greyscale_dib, filepath, bit_depth);
	}
	else {
		sprintf(filepath, TEST_DATA_OUTPUT_DIR "/IO/SimpleSave/8bit.%s", extension);
		err = FIA_SimpleSaveFIBToFile (greyscale_dib, filepath);
	}

	CuAssertTrue(tc, err == FIA_SUCCESS);

	FreeImage_Unload(original_colour_dib);
	FreeImage_Unload(greyscale_dib);
}

static void
TestFIA_IOSaveU16Bit(CuTest* tc, FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH bit_depth, const char *extension)
{
	int err;
	char filepath[500];
	FIBITMAP *original_colour_dib = NULL, *greyscale_dib = NULL;

	const char *file = TEST_DATA_DIR "red-kneed-tarantula.jpg";
	original_colour_dib = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, original_colour_dib != NULL);

	FIA_InPlaceConvertToGreyscaleFloatType(&original_colour_dib, FIT_FLOAT);
	greyscale_dib = FIA_ConvertFloatTo16Bit(original_colour_dib, 0);

	CuAssertTrue(tc, greyscale_dib != NULL);

	if(bit_depth != BIT_NONE) {

	}
	else {
		sprintf(filepath, TEST_DATA_OUTPUT_DIR "/IO/SimpleSave/16Ubit.%s", extension);
		err = FIA_SimpleSaveFIBToFile (greyscale_dib, filepath);
	}

	CuAssertTrue(tc, err == FIA_SUCCESS);

	FreeImage_Unload(original_colour_dib);
	FreeImage_Unload(greyscale_dib);
}

static void
TestFIA_IOSave16Bit(CuTest* tc, FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH bit_depth, const char *extension)
{
	int err;
	char filepath[500];
	FIBITMAP *original_colour_dib = NULL, *greyscale_dib = NULL;

	const char *file = TEST_DATA_DIR "red-kneed-tarantula.jpg";
	original_colour_dib = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, original_colour_dib != NULL);

	FIA_InPlaceConvertToGreyscaleFloatType(&original_colour_dib, FIT_FLOAT);
	greyscale_dib = FIA_ConvertFloatTo16Bit(original_colour_dib, 1);

	CuAssertTrue(tc, greyscale_dib != NULL);

	if(bit_depth != BIT_NONE) {

	}
	else {
		sprintf(filepath, TEST_DATA_OUTPUT_DIR "/IO/SimpleSave/16bit.%s", extension);
		err = FIA_SimpleSaveFIBToFile (greyscale_dib, filepath);
	}

	CuAssertTrue(tc, err == FIA_SUCCESS);

	FreeImage_Unload(original_colour_dib);
	FreeImage_Unload(greyscale_dib);
}

static void
TestFIA_IOSave32BitFloat(CuTest* tc, FREEIMAGE_ALGORITHMS_SAVE_BITDEPTH bit_depth, const char *extension)
{
	int err;
	char filepath[500];
	FIBITMAP *original_colour_dib = NULL, *greyscale_dib = NULL;

	const char *file = TEST_DATA_DIR "red-kneed-tarantula.jpg";
	original_colour_dib = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, original_colour_dib != NULL);

	greyscale_dib = FIA_ConvertToGreyscaleFloatType(original_colour_dib, FIT_FLOAT);

	CuAssertTrue(tc, greyscale_dib != NULL);

	if(bit_depth != BIT_NONE) {

	}
	else {
		sprintf(filepath, TEST_DATA_OUTPUT_DIR "/IO/SimpleSave/32bitFloat.%s", extension);
		err = FIA_SimpleSaveFIBToFile (greyscale_dib, filepath);
	}

	CuAssertTrue(tc, err == FIA_SUCCESS);

	FreeImage_Unload(original_colour_dib);
	FreeImage_Unload(greyscale_dib);
}

static void
TestFIA_IOSave8BitJpegTest(CuTest* tc)
{
	TestFIA_IOSave8Bit(tc, BIT_NONE, "jpg");
}

static void
TestFIA_IOSave8BitPngTest(CuTest* tc)
{
	TestFIA_IOSave8Bit(tc, BIT_NONE, "png");
}

static void
TestFIA_IOSave8BitTifTest(CuTest* tc)
{
	TestFIA_IOSave8Bit(tc, BIT_NONE, "tif");
}

static void
TestFIA_IOSave16UBitPngTest(CuTest* tc)
{
	TestFIA_IOSaveU16Bit(tc, BIT_NONE, "png");
}

static void
TestFIA_IOSave16UBitTifTest(CuTest* tc)
{
	TestFIA_IOSaveU16Bit(tc, BIT_NONE, "tif");
}

static void
TestFIA_IOSave16BitPngTest(CuTest* tc)
{
	TestFIA_IOSave16Bit(tc, BIT_NONE, "png");
}

static void
TestFIA_IOSave16BitTifTest(CuTest* tc)
{
	TestFIA_IOSave16Bit(tc, BIT_NONE, "tif");
}

static void
TestFIA_IOSave32BitFloatPngTest(CuTest* tc)
{
	TestFIA_IOSave32BitFloat(tc, BIT_NONE, "png");
}

static void
TestFIA_IOSave32BitFloatTifTest(CuTest* tc)
{
	TestFIA_IOSave32BitFloat(tc, BIT_NONE, "tif");
}

static void
TestFIA_IOSave32BitFloatExrTest(CuTest* tc)
{
	TestFIA_IOSave32BitFloat(tc, BIT_NONE, "exr");
}

static void
TestFIA_IOSaveForced8BitPngTest(CuTest* tc)
{
	TestFIA_IOSave8Bit(tc, BIT8, "png");
}

static void
TestFIA_IOSaveForced16BitPngTest(CuTest* tc)
{
	TestFIA_IOSave8Bit(tc, BIT16, "png");
}

static void
TestFIA_IOSaveForced24BitPngTest(CuTest* tc)
{
	TestFIA_IOSave8Bit(tc, BIT24, "png");
}

static void
TestFIA_IOSaveForced32BitPngTest(CuTest* tc)
{
	TestFIA_IOSave8Bit(tc, BIT32, "png");
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



static void
TestFIA_Convert48BitsToStandardType(CuTest* tc)
{
	FIBITMAP *dib1 = NULL, *dib2 = NULL;
	FREE_IMAGE_TYPE type;
	int bpp, err;
    
    const char *file = TEST_DATA_DIR "IMG_3182.CR2";
	dib1 = FIA_LoadFIBFromFile(file);

	bpp = FreeImage_GetBPP(dib1);
	type = FreeImage_GetImageType(dib1);

	CuAssertTrue(tc, dib1 != NULL);
	
	dib2 = FIA_Convert48BitOr64BitRGBTo24BitColour(dib1);

	CuAssertTrue(tc, dib2 != NULL);
	
	bpp = FreeImage_GetBPP(dib2);
	type = FreeImage_GetImageType(dib2);

	err = FIA_SaveFIBToFile (dib2, TEST_DATA_OUTPUT_DIR "/IO/save-bmp-colour-test.bmp", BIT24);

	CuAssertTrue(tc, err == FIA_SUCCESS);

	FreeImage_Unload(dib1);
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
*/

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

	for(int i=0; i < 1000; i++) {

		//FIA_CopyColourBytesToFIBitmap (dib2, FreeImage_GetBits(dib1), 0, 1, COLOUR_ORDER_RGB);
		FIA_CopyColourBytesTo8BitFIBitmap (dib2, FreeImage_GetBits(dib1), 24, FI_RGBA_RED, 0, 1);

	}

	PROFILE_STOP("CopyColourBytesToFIBitmap");

	FIA_SaveFIBToFile (dib2, TEST_DATA_OUTPUT_DIR "/IO/save-colour-test.bmp", BIT24);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
}
*/


static void
TestFIA_LoadBinaryTest(CuTest* tc)
{
	int err;
	char filepath[500];
	FIBITMAP *dib = NULL;

	const char *file = TEST_DATA_DIR "binary.bmp";
	dib = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib != NULL);

	FIA_SimpleSaveFIBToFile(dib, TEST_DATA_OUTPUT_DIR "/IO/TestFIA_LoadBinaryTest.bmp");

	FreeImage_Unload(dib);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsIOSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/IO");
	MkDir(TEST_DATA_OUTPUT_DIR "/IO/SimpleSave");
	MkDir(TEST_DATA_OUTPUT_DIR "/IO/ForcedSave");

	SUITE_ADD_TEST(suite, TestFIA_LoadBinaryTest);
	//SUITE_ADD_TEST(suite, TestFIA_IOSave8BitJpegTest);

	/*
	SUITE_ADD_TEST(suite, TestFIA_IOSave8BitPngTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSave8BitTifTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSave16UBitPngTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSave16UBitTifTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSave16BitPngTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSave16BitTifTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSave32BitFloatPngTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSave32BitFloatTifTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSave32BitFloatExrTest);

	SUITE_ADD_TEST(suite, TestFIA_IOSaveForced8BitPngTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSaveForced24BitPngTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSaveForced16BitPngTest);
	SUITE_ADD_TEST(suite, TestFIA_IOSaveForced32BitPngTest);

	SUITE_ADD_TEST(suite, TestFIA_Convert48BitsToStandardType);
	*/

	//SUITE_ADD_TEST(suite, TestFIA_IO);
	//SUITE_ADD_TEST(suite, TestFIA_IOLoadColourArrayData);
	//SUITE_ADD_TEST(suite, TestFIA_SaveBPPWithPalette);

	return suite;
}
