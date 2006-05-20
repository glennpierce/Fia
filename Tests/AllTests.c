#include <stdio.h>

#include "CuTest.h"

CuSuite* CuGetFreeImageAlgorithmsColourSuite();
CuSuite* CuGetFreeImageAlgorithmsLinearScaleSuite();
CuSuite* CuGetFreeImageAlgorithmsUtilitySuite();
CuSuite* CuGetFreeImageAlgorithmsStatisticSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsColourSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsLinearScaleSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsUtilitySuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsStatisticSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	char string[10];

	RunAllTests();

	gets(string);

	return 0;
}
