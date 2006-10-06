#include <stdio.h>

#include "CuTest.h"
#include "FreeImageAlgorithms.h"

CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsColourSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsLinearScaleSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsUtilitySuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsStatisticSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsFFTSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsArithmaticSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsIOSuite(void);


void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsColourSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsLinearScaleSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsUtilitySuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsStatisticSuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsFFTSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsArithmaticSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsIOSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int __cdecl main(void)
{
	char string[10];

	RunAllTests();

	gets(string);

	return 0;
}