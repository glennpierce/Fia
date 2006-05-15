#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_LinearScale.h"

#include "FreeImageAlgorithms_Testing.h"

void TestFreeImageAlgorithms_LinearScaleTest(CuTest* tc)
{
	double min_found, max_found;

	char *file = TEST_IMAGE_DIRECTORY "\\8bit_test.png";

	FIBITMAP *dib = FreeImageAlgorithms_LoadFIBFromFile(file);
	FIBITMAP *scaled_dib = FreeImageAlgorithms_LinearScaleToStandardType(dib, 0, 0, &min_found, &max_found);  

	FreeImage_Unload(dib);
	FreeImage_Unload(scaled_dib);

	CuAssertTrue(tc, min_found == 152.0);
	CuAssertTrue(tc, max_found == 240.0);
}


CuSuite* CuGetFreeImageAlgorithmsLinearScaleSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_LinearScaleTest);

	return suite;
}
