#include "CuTest.h"

#include "Constants.h"
#include "FreeImage.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Drawing.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_Particle.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"

#include <iostream>
#include <fstream>

static void
TestFIA_FillholeTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "fillhole.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);
	
	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	PROFILE_START("FillholeTest");

	FIBITMAP* result_dib = FIA_Fillholes(threshold_8bit_dib, 1);

	CuAssertTrue(tc, result_dib != NULL);

	PROFILE_STOP("FillholeTest");
	
	FIA_SaveFIBToFile(result_dib, TEST_DATA_OUTPUT_DIR "/Particle/fillhole_result.bmp", BIT8);

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FreeImage_Unload(result_dib);
}

static void TestFIA_ParticleInfoTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "particle.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	PROFILE_START("ParticleInfo");

	PARTICLEINFO *info;

	FIA_ParticleInfo(dib2, &info, 1);

	PROFILE_STOP("ParticleInfo");

	FIBITMAP *dst = FreeImage_ConvertTo24Bits(dib2);
	FIARECT centre;

	std::ofstream myfile (TEST_DATA_OUTPUT_DIR  "shouldbe.txt");

	for(int i=0; i < info->number_of_blobs; i++)
	{
		BLOBINFO blobinfo = info->blobs[i];

		FIA_DrawColourRect (dst, blobinfo.rect, FIA_RGBQUAD(255,0,0), 2);

		centre.left = blobinfo.center_x - 2;
		centre.right = blobinfo.center_x + 2;
		centre.top = blobinfo.center_y - 2;
		centre.bottom = blobinfo.center_y + 2;

		FIA_DrawColourSolidRect(dst, centre, FIA_RGBQUAD(0,255,0));
		FIA_SetPixelColourFromTopLeft(dst, blobinfo.center_x, blobinfo.center_y, FIA_RGBQUAD(0,0,255));

		myfile << "left  "
			<< blobinfo.rect.left << "  top  "  << blobinfo.rect.top
			<< "  width  " << blobinfo.rect.right - blobinfo.rect.left + 1
			<< "  height  " << blobinfo.rect.bottom - blobinfo.rect.top + 1
			<< "  area: " << blobinfo.area
			<< "  centre x: " << blobinfo.center_x
			<< "  centre y: " << blobinfo.center_y << std::endl;
	}
	
	myfile << "Number of particles " << info->number_of_blobs << std::endl;

	myfile.close();

	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Particle/particle_found.jpg", BIT24);

	FIA_FreeParticleInfo(info);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dst);
}

/*
static void
TestFIA_FindImageMaximaTest(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "maxima.tif";
	
	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	PROFILE_START("FindImageMaxima");

	FIAPeak *peaks = NULL;
	int number_of_peaks;

	//FIBITMAP *dib3 = FIA_FindImageMaxima(dib1, NULL, 30.0, 4, 1, &peaks, 0, &number_of_peaks);
	FIBITMAP *dib3 = FIA_FindImageMaxima(dib1, NULL, 20.0, 1, 1, &peaks, 0, &number_of_peaks);

	PROFILE_STOP("FindImageMaxima");

	FIA_SetTemperaturePalette(dib3);

	FIA_SimpleSaveFIBToFile(dib3, TEST_DATA_OUTPUT_DIR "/Particle/find_image_maxima.tif"); 

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib3);
}


static void
TestFIA_FindImageMaximaTest2(CuTest* tc)
{
	//const char *file = TEST_DATA_DIR "Debug.bmp";
	//const char *mask = TEST_DATA_DIR "DebugMask.bmp";

	const char *file = "C:\\Debug.bmp";
	const char *mask = "C:\\DebugMask.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);
	FIBITMAP *mask_dib = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	PROFILE_START("FindImageMaxima");

	FIAPeak *peaks = NULL;
	int number_of_peaks;

	FIBITMAP *dib3 = FIA_FindImageMaxima(dib1, mask_dib, 20.0, 5.0, 1, &peaks, 0, &number_of_peaks);

	PROFILE_STOP("FindImageMaxima");

	//FIA_DrawSolidGreyscaleEllipse(dib3 , MakeFIARect(10,10,16,16), 255, 0);
	//FIA_DrawSolidGreyscaleEllipse(dib3 , MakeFIARect(20,20,30,30), 255, 0);

	//FIA_SetTemperaturePalette(dib3);

	//FIBITMAP *dib4 = FreeImage_Allocate(100,100,8,0,0,0);
 
	//FIA_DrawSolidGreyscaleRect(dib4 , MakeFIARect(10,10,20,20), 255);
	//FIA_DrawSolidGreyscaleEllipse (dib4 , MakeFIARect(30,30,40,40), 255, 0);

	//FIA_SetTemperaturePalette(dib4);

	//FIA_SimpleSaveFIBToFile(dib4, TEST_DATA_OUTPUT_DIR "/Particle/Draw.tif"); 

	FIA_SimpleSaveFIBToFile(dib3, TEST_DATA_OUTPUT_DIR "/Particle/TestFIA_FindImageMaximaTest2.tif"); 

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib3);
	FreeImage_Unload(mask_dib);
}


static void TestFIA_ParticleInfoTest2(CuTest* tc)
{
	const char *file = TEST_DATA_DIR "AccImageResult.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	PROFILE_START("ParticleInfo");

	PARTICLEINFO *info;

	FIA_ParticleInfo(dib2, &info, 1);

	PROFILE_STOP("ParticleInfo");

	FIBITMAP *dst = FreeImage_ConvertTo24Bits(dib2);
	FIARECT centre;

	for(int i=0; i < info->number_of_blobs; i++)
	{
		BLOBINFO blobinfo = info->blobs[i];

		FIA_DrawColourRect (dst, blobinfo.rect, FIA_RGBQUAD(255,0,0), 2);

		centre.left = blobinfo.center_x - 2;
		centre.right = blobinfo.center_x + 2;
		centre.top = blobinfo.center_y - 2;
		centre.bottom = blobinfo.center_y + 2;

		FIA_DrawColourSolidRect(dst, centre, FIA_RGBQUAD(0,255,0));
	}
	
	FIA_SaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Particle/particlefind.bmp", BIT24);

	FIA_FreeParticleInfo(info);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dst);
}

static void
TestFIA_MultiscaleProductsTest(CuTest* tc)
{
    const char *file = TEST_DATA_DIR "test.tif";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	PROFILE_START("MultiscaleProducts");

	FIBITMAP *dst = FIA_MultiscaleProducts(dib2, 2, 3);

// PRB NOt sure what these thresholds are supposed to do!
//    FIA_InPlaceThreshold(dst, 0, 5, 0);
  //  FIA_InPlaceThreshold(dst, 1, 255, 255);
	//	FIA_InPlaceThreshold(dst, 1e-10, 1e10, 1.0);
//	FIA_InPlaceThreshold(dst, -1e10, -1e-10, 1.0);

	PROFILE_STOP("MultiscaleProducts");

	FIA_SetBinaryPalette(dst);
	FIA_SimpleSaveFIBToFile(dst, TEST_DATA_OUTPUT_DIR "/Particle/multiscaleProducts.bmp"); 

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dst);
}
*/


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsParticleSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	MkDir(TEST_DATA_OUTPUT_DIR "/Particle");

	FIA_EnableOldBrokenCodeCompatibility();

	//SUITE_ADD_TEST(suite, TestFIA_FillholeTest);
	//SUITE_ADD_TEST(suite, TestFIA_ParticleInfoTest);
	//SUITE_ADD_TEST(suite, TestFIA_MultiscaleProductsTest);
	//SUITE_ADD_TEST(suite, TestFIA_ParticleInfoTest2);

	//SUITE_ADD_TEST(suite, TestFIA_FindImageMaximaTest);
    //SUITE_ADD_TEST(suite, TestFIA_FindImageMaximaTest2);

	return suite;
}




