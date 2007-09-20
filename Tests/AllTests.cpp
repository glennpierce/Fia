#include <stdio.h>

#include "CuTest.h"
#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Error.h"

#include <iostream>

CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsColourSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsLinearScaleSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsUtilitySuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsStatisticSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsFFTSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsArithmaticSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsIOSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsConvolutionSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsMorphologySuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsLogicSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsDrawingSuite(void);
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsParticleSuite(void);

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

    /*
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsColourSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsUtilitySuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsStatisticSuite());
	
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsArithmaticSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsIOSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsConvolutionSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsMorphologySuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsLogicSuite());
    CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsParticleSuite());
	CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsDrawingSuite());
    CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsLinearScaleSuite());
    */

    CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsFFTSuite());
    //CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsArithmaticSuite());
    

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	PROFILE_PRINT();
}

static void OnError(const char *msg)
{
    std::cout << "Error: " << msg << std::endl;
}

int __cdecl main(void)
{
	char string[10];

    FreeImageAlgorithms_SetOutputMessage(OnError);

	RunAllTests();

	gets(string);

	return 0;
}