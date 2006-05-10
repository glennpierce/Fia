#include <stdio.h>

#include "CuTest.h"

CuSuite* CuGetFreeImageAlgorithmsSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsSuite());

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
