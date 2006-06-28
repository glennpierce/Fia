#ifndef __FREEIMAGE_ALGORITHMS_ERROR__
#define __FREEIMAGE_ALGORITHMS_ERROR__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

typedef void (*FreeImageAlgorithms_OutputMessageFunction)(const char *msg);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_SetOutputMessage(FreeImageAlgorithms_OutputMessageFunction omf);

DLL_API void DLL_CALLCONV
FreeImageAlgorithms_SendOutputMessage(const char *message);

#ifdef __cplusplus
}
#endif

#endif






