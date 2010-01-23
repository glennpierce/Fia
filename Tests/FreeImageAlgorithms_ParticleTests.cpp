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


static void
TestFIA_ParticleInfoTest(CuTest* tc)
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
    const char *file = IMAGE_DIR "\\finmax_in.bmp";
	
	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	PROFILE_START("FindImageMaxima");

	FIAPeak *peaks = NULL;
	int number_of_peaks;
	FIBITMAP *dib3 = FIA_FindImageMaxima(dib2, NULL, 2,
        0, &peaks, 0, &number_of_peaks);

	PROFILE_STOP("FindImageMaxima");

	FIA_SetGreyLevelPalette(dib3);

	FIBITMAP *dst = FreeImage_ConvertTo24Bits(dib3);

	FIA_SaveFIBToFile(dst, TEMP_DIR "\\find_image_maxima.bmp", BIT8); 

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FreeImage_Unload(dst);
}


static void
TestFIA_FindImageMaximaTest2(CuTest* tc)
{
    const char *file = IMAGE_DIR "\\timeLapse041.bmp";

	FIBITMAP *dib1 = FIA_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	PROFILE_START("MultiscaleProducts");

	FIBITMAP *dst = FIA_MultiscaleProducts(dib2, 2, 3);

    FIA_InPlaceThreshold(dst, 0, 5, 0);
    FIA_InPlaceThreshold(dst, 1, 255, 255);

	PROFILE_STOP("MultiscaleProducts");

	FIA_SaveFIBToFile(dst, TEMP_DIR "\\find_image_maxima2.bmp", BIT8); 

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

	SUITE_ADD_TEST(suite, TestFIA_FillholeTest);
	SUITE_ADD_TEST(suite, TestFIA_ParticleInfoTest);
	//SUITE_ADD_TEST(suite, TestFIA_FindImageMaximaTest);
    //SUITE_ADD_TEST(suite, TestFIA_FindImageMaximaTest2);

	return suite;
}
