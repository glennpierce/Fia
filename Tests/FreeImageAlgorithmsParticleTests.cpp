#include "CuTest.h"

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
TestFreeImageAlgorithms_FillholeTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\fillhole_test.bmp";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	CuAssertTrue(tc, dib1 != NULL);
	
	FIBITMAP *threshold_dib = FreeImage_Threshold(dib1, 20);

	CuAssertTrue(tc, threshold_dib != NULL);

	FIBITMAP *threshold_8bit_dib = FreeImage_ConvertTo8Bits(threshold_dib);
	
	CuAssertTrue(tc, threshold_8bit_dib != NULL);

	PROFILE_START("FillholeTest");

	FIBITMAP* result_dib = FreeImageAlgorithms_Fillholes(threshold_8bit_dib, 1);

	CuAssertTrue(tc, result_dib != NULL);

	PROFILE_STOP("FillholeTest");
	
	FreeImageAlgorithms_SaveFIBToFile(result_dib, TEMP_DIR "\\fillhole_result.bmp", BIT8);

	FreeImage_Unload(dib1);
	FreeImage_Unload(threshold_dib);
	FreeImage_Unload(threshold_8bit_dib);
	FreeImage_Unload(result_dib);
}


static void
TestFreeImageAlgorithms_ParticleInfoTest(CuTest* tc)
{
	char *file = IMAGE_DIR "\\fillhole_test.bmp";
	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\particle-test.bmp";
	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\shouldbe.bmp";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	PROFILE_START("ParticleInfo");

	PARTICLEINFO *info;

	FreeImageAlgorithms_ParticleInfo(dib2, &info, 1);

	PROFILE_STOP("ParticleInfo");

	FIBITMAP *dst = FreeImage_ConvertTo24Bits(dib2);

	FIARECT centre;

	int height = FreeImage_GetHeight(dst);

	std::ofstream myfile ("C:\\Documents and Settings\\Pierce\\Desktop\\shouldbe.txt");

	for(int i=0; i < info->number_of_blobs; i++)
	{
		BLOBINFO blobinfo = info->blobs[i];

		FreeImageAlgorithms_DrawColourRect (dst, blobinfo.rect, FIA_RGBQUAD(255,0,0), 2);

		centre.left = blobinfo.center_x - 2;
		centre.right = blobinfo.center_x + 2;
		centre.top = blobinfo.center_y - 2;
		centre.bottom = blobinfo.center_y + 2;

		FreeImageAlgorithms_DrawColourSolidRect(dst, centre, FIA_RGBQUAD(0,255,0));

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

	FreeImageAlgorithms_SaveFIBToFile(dst, "C:\\Documents and Settings\\Pierce\\Desktop\\particle_rect.bmp", BIT24);

	FreeImageAlgorithms_FreeParticleInfo(info);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dst);
}


static void
TestFreeImageAlgorithms_FindImageMaximaTest(CuTest* tc)
{
    //char *file = IMAGE_DIR "\\CHT.bmp";
    char *file = IMAGE_DIR "\\finmax_in.bmp";
	//char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\test.bmp";
	//char *shouldbefile = "C:\\Documents and Settings\\Pierce\\Desktop\\shouldbe.bmp";
	
	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);
	//FIBITMAP *shouldbe_dib = FreeImageAlgorithms_LoadFIBFromFile(shouldbefile);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	PROFILE_START("FindImageMaxima");

	FIAPeak *peaks = NULL;
	int number_of_peaks;
	FIBITMAP *dib3 = FreeImageAlgorithms_FindImageMaxima(dib2, NULL, 2,
        0, &peaks, 0, &number_of_peaks);

	PROFILE_STOP("FindImageMaxima");

	FreeImageAlgorithms_SetGreyLevelPalette(dib3);

	FIBITMAP *dst = FreeImage_ConvertTo24Bits(dib3);

	FreeImageAlgorithms_SaveFIBToFile(dst, TEMP_DIR "\\find_image_maxima.bmp", BIT8); 

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
	FreeImage_Unload(dib3);
	FreeImage_Unload(dst);
}


static void
TestFreeImageAlgorithms_FindImageMaximaTest2(CuTest* tc)
{
    char *file = IMAGE_DIR "\\find_max_test.bmp";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file);

	CuAssertTrue(tc, dib1 != NULL);

	FIBITMAP *dib2 = FreeImage_ConvertTo8Bits(dib1);
	
	CuAssertTrue(tc, dib2 != NULL);
 
	PROFILE_START("FindImageMaxima2");

	FIAPeak *peaks = NULL;
	int number_of_peaks;
	FreeImageAlgorithms_FindImageMaxima2(dib2, 4, 10, &number_of_peaks);

	PROFILE_STOP("FindImageMaxima2");

	//FIBITMAP *dst = FreeImage_ConvertToStandardType(dib3, 1);

	//FreeImageAlgorithms_SaveFIBToFile(dst, TEMP_DIR "\\find_image_maxima2.bmp", BIT8); 

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);
//	FreeImage_Unload(dib3);
	//FreeImage_Unload(dst);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsParticleSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FillholeTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ParticleInfoTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FindImageMaximaTest);
    SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_FindImageMaximaTest2);

	return suite;
}