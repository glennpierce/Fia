#include <stdio.h>

#include "CuTest.h"
#include "FreeImage.h"

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Testing.h"

#include "current_function.hpp"
#include <iostream>

#ifndef WIN32
#include <sys/stat.h>
#endif


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
CuSuite* DLL_CALLCONV CuGetFreeImageAlgorithmsGradientBlendSuite(void);

int MkDir(const char *path)
{
	#ifdef WIN32
	return CreateDirectory(path, NULL);
	#else
	return mkdir(path, 0777);
	#endif
}

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	current_function_helper();

	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsColourSuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsUtilitySuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsStatisticSuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsIOSuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsMorphologySuite());
	//CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsLogicSuite());
    //CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsParticleSuite());
    //CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsLinearScaleSuite());
    //CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsDrawingSuite());
    //CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsFFTSuite());
    //CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsArithmaticSuite());
    //CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsConvolutionSuite());
    CuSuiteAddSuite(suite, CuGetFreeImageAlgorithmsGradientBlendSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	PROFILE_PRINT();
}

static void OnError(FREE_IMAGE_FORMAT fif, const char *msg)
{
    std::cout << "Error: " << msg << std::endl;
}

int __cdecl main(void)
{

    FreeImage_SetOutputMessage(OnError);

	MkDir(DEBUG_DATA_DIR);
	MkDir(TEST_DATA_OUTPUT_DIR);

	RunAllTests();

	return 0;
}
