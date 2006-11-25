#include "CuTest.h"

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Statistics.h"

#include "FreeImageAlgorithms_Testing.h"

#include <iostream>
#include <math.h>

static void
TestFreeImageAlgorithms_MonoAreaTest(CuTest* tc)
{
	double white_area, black_area;

	char *file = IMAGE_DIR "\\mask.bmp";

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	
	if(FreeImageAlgorithms_MonoImageFindWhiteFraction(dib, &white_area, &black_area) == FREEIMAGE_ALGORITHMS_ERROR)
		CuFail(tc, "Failed");

	FreeImage_Unload(dib);

	// float white_area = 0.540436;
	// white_area * 100 = 54.0436
	// 54.0436 + 0.5 = 54.5436
	// floor(54.5436) = 54
	// 54 / 100 = 0.54
	double x = floor(white_area*100+.05)/100;

	CuAssertTrue(tc, x == 0.54);
	CuAssertTrue(tc, white_area + black_area == 1.0);
}

static void
TestFreeImageAlgorithms_MonoComparisonTest(CuTest* tc)
{
	double white_area, black_area;

	char *file1 = IMAGE_DIR "\\texture.bmp";
	char *file2 = IMAGE_DIR "\\mask.bmp";

	FIBITMAP *dib1 = FreeImageAlgorithms_LoadFIBFromFile(file1);
	FIBITMAP *dib2 = FreeImageAlgorithms_LoadFIBFromFile(file2);
	
	int tp, tn, fp, fn;

	int error = FreeImageAlgorithms_MonoTrueFalsePositiveComparison(dib1, dib2,
													&tp, &tn, &fp, &fn);

	if(error == FREEIMAGE_ALGORITHMS_ERROR)
		CuFail(tc, "Failed");

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);

	std::cout << "True Positive: " << tp
		<< "\nTrue Negative: " << tn
		<< "\nFalse Positive: " << fp
		<< "\nFalse Negative: " << fn << std::endl;
	
	//CuAssertTrue(tc, x == 0.54);
	//CuAssertTrue(tc, white_area + black_area == 1.0);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsStatisticSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_MonoAreaTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_MonoComparisonTest);

	return suite;
}
