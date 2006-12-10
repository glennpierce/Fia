#ifndef __FREEIMAGE_ALGORITHMS_ERROR__
#define __FREEIMAGE_ALGORITHMS_ERROR__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

/*! \file 
*	Provides Setting up of a error handler.
*/

typedef void (*FreeImageAlgorithms_OutputMessageFunction)(const char *msg);

/** \brief Handles FreeImageAlgorithm Errors.
 *	
 *	\param omf function to be called on errors.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_SetOutputMessage(FreeImageAlgorithms_OutputMessageFunction omf);

/** \brief Sends a FreeImageAlgorithm Error.
 *	
 *	\param message send an error from the FreeImageAlgorithms library.
*/
DLL_API void DLL_CALLCONV
FreeImageAlgorithms_SendOutputMessage(const char *message, ...);

#ifdef __cplusplus
}
#endif

#endif






