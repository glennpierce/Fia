#include "CuTest.h"

#include "Constants.h"
#include "FreeImage.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_Utilities.h"

#include "FreeImageAlgorithms_Testing.h"

static void TestFIA_MultiplyTest(CuTest* tc)
{
    int x, y, width = 100, height = 100;
    FIBITMAP *dib, *dib2;
    double *bits;
    
    dib = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);
    
    CuAssertTrue(tc, dib != NULL);
    
    for (y = 0; y < height; y++) {
        
        bits = (double *) FreeImage_GetScanLine(dib, y);
        
        for (x=0; x < width; x++) {
            bits[x] = 5;
        }
    }
    
    FIA_MultiplyGreyLevelImageConstant(dib, 2.0);
    
    for (y = 0; y < height; y++) {
        
        bits = (double *) FreeImage_GetScanLine(dib, y);
        
        for (x=0; x < width; x++) {
            CuAssertTrue(tc, bits[x] == 10.0);
        }
    }
    
    dib2 = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);
    
    CuAssertTrue(tc, dib2 != NULL);
    
    // Test multiplying two images
    for (y = 0; y < height; y++) {
        
        bits = (double *) FreeImage_GetScanLine(dib2, y);
        
        for (x=0; x < width; x++) {
            bits[x] = 6;
        }
    }
    
    FIA_MultiplyGreyLevelImages(dib, dib2);
    
    for (y = 0; y < height; y++) {
        
        bits = (double *) FreeImage_GetScanLine(dib, y);
        
        for (x=0; x < width; x++) {
            CuAssertTrue(tc, bits[x] == 60.0);
        }
    }
    
    FreeImage_Unload(dib);
    FreeImage_Unload(dib2);
}

static void TestFIA_DivideTest(CuTest* tc)
{
    int x, y, width = 100, height = 100;
    FIBITMAP *dib, *dib2;
    double *bits;
    
    dib = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);
    
    CuAssertTrue(tc, dib != NULL);
    
    for (y = 0; y < height; y++) {
        
        bits = (double *) FreeImage_GetScanLine(dib, y);
        
        for (x=0; x < width; x++) {
            bits[x] = 5;
        }
    }
    
    FIA_DivideGreyLevelImageConstant(dib, 2.0);
    
    for (y = 0; y < height; y++) {
        
        bits = (double *) FreeImage_GetScanLine(dib, y);
        
        for (x=0; x < width; x++) {
            CuAssertTrue(tc, bits[x] == 2.5);
        }
    }
    
    dib2 = FreeImage_AllocateT(FIT_DOUBLE, width, height, 32, 0, 0, 0);
    
    CuAssertTrue(tc, dib2 != NULL);
    
    // Test multiplying two images
    for (y = 0; y < height; y++) {
        
        bits = (double *) FreeImage_GetScanLine(dib2, y);
        
        for (x=0; x < width; x++) {
            bits[x] = 10;
        }
    }
    
    FIA_DivideGreyLevelImages(dib, dib2);
    
    for (y = 0; y < height; y++) {
        
        bits = (double *) FreeImage_GetScanLine(dib, y);
        
        for (x=0; x < width; x++) {
            CuAssertTrue(tc, bits[x] == 0.25);
        }
    }
    
    FreeImage_Unload(dib);
    FreeImage_Unload(dib2);
}

static void TestFIA_AddTest(CuTest* tc)
{
    int width, height, error;
    FIBITMAP *sum, *dib;
    
    const char *file= TEST_DATA_DIR "drone-bee-greyscale.jpg";
    
    dib = FIA_LoadFIBFromFile(file);
    
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    
    CuAssertTrue(tc, dib != NULL);
    
    // 32 ?
    sum = FreeImage_AllocateT(FIT_FLOAT, width, height, 0, 0, 0, 0);
    
    CuAssertTrue(tc, sum != NULL);
    
    error = FIA_AddGreyLevelImages(sum, dib);
    
    CuAssertTrue(tc, error == FIA_SUCCESS);
    
    FreeImage_Unload(dib);
    FreeImage_Unload(sum);
    
    CuAssertTrue(tc, sum != NULL);
}

static void TestFIA_SubtractTest(CuTest* tc)
{
    int width, height, error;
    FIBITMAP *sum, *dib;
    
    const char *file= TEST_DATA_DIR "fibres_greyscale_flip.jpg";
	const char *file2= TEST_DATA_DIR "fibres_greyscale.jpg";
    
    dib = FIA_LoadFIBFromFile(file);
    sum = FIA_LoadFIBFromFile(file2);
    
//    width = FreeImage_GetWidth(dib);
 //   height = FreeImage_GetHeight(dib);
    
    CuAssertTrue(tc, dib != NULL);
    
    // 32 ?
//    sum = FreeImage_AllocateT(FIT_FLOAT, width, height, 0, 0, 0, 0);
    
    CuAssertTrue(tc, sum != NULL);
    
	sum = FIA_ConvertToGreyscaleFloatType(sum, FIT_FLOAT);
	error = FIA_SubtractGreyLevelImages(sum, dib);
	sum = FreeImage_ConvertToStandardType (sum, 0);
    
    CuAssertTrue(tc, error == FIA_SUCCESS);
    
    CuAssertTrue(tc, sum != NULL);

	FIA_SimpleSaveFIBToFile(sum, TEST_DATA_OUTPUT_DIR "Arithmetic/Subtract.bmp");
    
    FreeImage_Unload(dib);
    FreeImage_Unload(sum);
}

CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsArithmaticSuite(void)
{
    CuSuite* suite = CuSuiteNew();
 
    MkDir(TEST_DATA_OUTPUT_DIR "/Arithmetic");
   
    SUITE_ADD_TEST(suite, TestFIA_AddTest);
    SUITE_ADD_TEST(suite, TestFIA_SubtractTest);
    SUITE_ADD_TEST(suite, TestFIA_MultiplyTest);
    SUITE_ADD_TEST(suite, TestFIA_DivideTest);
    
    return suite;
}
