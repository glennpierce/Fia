/*
 * Copyright 2007 Glenn Pierce
 *
 * This file is part of FreeImageAlgorithms.
 *
 * FreeImageAlgorithms is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeImageAlgorithms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 * 
 * You should have received a copy of the Lesser GNU General Public License
 * along with FreeImageAlgorithms.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __FREEIMAGE_ALGORITHMS_ERROR__
#define __FREEIMAGE_ALGORITHMS_ERROR__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"

/*! \file 
*	Provides Setting up of a error handler.
*/

typedef void (*FIA_OutputMessageFunction)(const char *msg);

/** \brief Handles FreeImageAlgorithm Errors.
 *	
 *	\param omf function to be called on errors.
*/
DLL_API void DLL_CALLCONV
FIA_SetOutputMessage(FIA_OutputMessageFunction omf);

/** \brief Sends a FreeImageAlgorithm Error.
 *	
 *	\param fmt const char * Format of the error message.
*/
DLL_API void DLL_CALLCONV
FIA_SendOutputMessage(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif






