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

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Error.h"

#include <stdio.h>
#include <stdarg.h>

static FIA_OutputMessageFunction errorFunction = NULL;

void DLL_CALLCONV
FIA_SetOutputMessage(FIA_OutputMessageFunction omf)
{
	errorFunction = omf;
}

void DLL_CALLCONV
FIA_SendOutputMessage(const char *fmt, ...)
{
	if(errorFunction == NULL)
		return;

	va_list ap;
	char message[500];
	
	va_start(ap, fmt);

	vsnprintf(message, 500, fmt, ap);

	va_end(ap);

	errorFunction(message);
}
