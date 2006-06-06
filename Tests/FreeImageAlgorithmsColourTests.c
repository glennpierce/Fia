#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"

#include "FreeImageAlgorithms_Colour.h"

static void
TestFreeImageAlgorithms_ColourFunctions(CuTest* tc)
{
	unsigned char red = 165, green = 202, blue = 68;
	double hue, satuation, value;

	FreeImageAlgorithms_RGBToHSV(red, green, blue, &hue, &satuation, &value);

	CuAssertTrue(tc, (int)(hue + 0.5) == 77);
	CuAssertTrue(tc, (int)(satuation * 100 + 0.5) == 66);
	CuAssertTrue(tc, (int)(value * 100 + 0.5) == 79);

	FreeImageAlgorithms_HSVToRGB(hue, satuation, value, &red, &green, &blue);

	CuAssertTrue(tc, red == 165);
	CuAssertTrue(tc, green == 202);
	CuAssertTrue(tc, blue == 68);
}


CuSuite* DLL_CALLCONV
CuGetFreeImageAlgorithmsColourSuite(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestFreeImageAlgorithms_ColourFunctions);

	return suite;
}
