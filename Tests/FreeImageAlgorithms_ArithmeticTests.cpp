#include "CuTest.h"

#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Arithmetic.h"

#include "FreeImageAlgorithms_Testing.h"

static void
TestFreeImageAlgorithms_MultiplyTest(CuTest* tc)
{
	int x, y, width = 100, height = 100;
	FIBITMAP *dib, *dib2;
	double *bits;

	dib = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);

	CuAssertTrue(tc, dib != NULL);

	for(y = 0; y < height; y++) { 
		
		bits = (double *) FreeImage_GetScanLine(dib, y);
		
		for(x=0; x < width; x++)	
			bits[x] = 5;
	}

	FreeImageAlgorithms_MultiplyGreyLevelImageConstant(dib, 2.0);

	for(y = 0; y < height; y++) { 
		
		bits = (double *) FreeImage_GetScanLine(dib, y);
		
		for(x=0; x < width; x++)
			CuAssertTrue(tc, bits[x] == 10.0);
	}

	dib2 = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);

	CuAssertTrue(tc, dib2 != NULL);

	// Test multiplying two images
	for(y = 0; y < height; y++) { 
		
		bits = (double *) FreeImage_GetScanLine(dib2, y);
		
		for(x=0; x < width; x++)	
			bits[x] = 6;
	}

	FreeImageAlgorithms_MultiplyGreyLevelImages(dib, dib2);

	for(y = 0; y < height; y++) { 
		
		bits = (double *) FreeImage_GetScanLine(dib, y);
		
		for(x=0; x < width; x++)
			CuAssertTrue(tc, bits[x] == 60.0);
	}

	FreeImage_Unload(dib);
	FreeImage_Unload(dib2);
}


static void
TestFreeImageAlgorithms_DivideTest(CuTest* tc)
{
	int x, y, width = 100, height = 100;
	FIBITMAP *dib, *dib2;
	double *bits;

	dib = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);

	CuAssertTrue(tc, dib != NULL);

	for(y = 0; y < height; y++) { 
		
		bits = (double *) FreeImage_GetScanLine(dib, y);
		
		for(x=0; x < width; x++)	
			bits[x] = 5;
	}

	FreeImageAlgorithms_DivideGreyLevelImageConstant(dib, 2.0);

	for(y = 0; y < height; y++) { 
		
		bits = (double *) FreeImage_GetScanLine(dib, y);
		
		for(x=0; x < width; x++)
			CuAssertTrue(tc, bits[x] == 2.5);
	}

	dib2 = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);

	CuAssertTrue(tc, dib2 != NULL);

	// Test multiplying two images
	for(y = 0; y < height; y++) { 
		
		bits = (double *) FreeImage_GetScanLine(dib2, y);
		
		for(x=0; x < width; x++)	
			bits[x] = 10;
	}

	FreeImageAlgorithms_DivideGreyLevelImages(dib, dib2);

	for(y = 0; y < height; y++) { 
		
		bits = (double *) FreeImage_GetScanLine(dib, y);
		
		for(x=0; x < width; x++)
			CuAssertTrue(tc, bits[x] == 0.25);
	}

	FreeImage_Unload(dib);
	FreeImage_Unload(dib2);
}


static void
TestFreeImageAlgorithms_AddTest(CuTest* tc)
{
	int x, y, width, height, error;
	FIBITMAP *sum, *dib;
	double *bits;

    char *file = "C:\\Documents and Settings\\Pierce\\Desktop\\Slices\\slice.bmp";

	dib = FreeImageAlgorithms_LoadFIBFromFile(file);

    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);

    CuAssertTrue(tc, dib != NULL);

    // 32 ?
	sum = FreeImage_AllocateT(FIT_FLOAT, width, height, 0, 0, 0, 0);

	CuAssertTrue(tc, sum != NULL);

    error = FreeImageAlgorithms_AddGreyLevelImages(sum, dib);

    CuAssertTrue(tc, error == FREEIMAGE_ALGORITHMS_SUCCESS);

    FreeImage_Unload(dib);
    FreeImage_Unload(sum);

	CuAssertTrue(tc, sum != NULL);
}



CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsArithmaticSuite(void)
{
	CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_AddTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_MultiplyTest);
	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_DivideTest);

	return suite;
}
