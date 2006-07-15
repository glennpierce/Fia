#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "CuTest.h"

CuSuite* CuGetFreeImageAlgorithmsColourSuite();
CuSuite* CuGetFreeImageAlgorithmsLinearScaleSuite();
CuSuite* CuGetFreeImageAlgorithmsUtilitySuite();
CuSuite* CuGetFreeImageAlgorithmsStatisticSuite();
CuSuite* CuGetFreeImageAlgorithmsFFTSuite();
CuSuite* CuGetFreeImageAlgorithmsArithmaticSuite();
CuSuite* CuGetFreeImageAlgorithmsIOSuite();


void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsColourSuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsLinearScaleSuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsUtilitySuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsStatisticSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsFFTSuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsArithmaticSuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsIOSuite());

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

#ifdef __cplusplus
}
#endif