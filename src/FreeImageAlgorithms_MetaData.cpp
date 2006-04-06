#include "FreeImageAlgorithms_MetaData.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Private.h"

#include <iostream>

static char
*strlwr(char *s)
{
   if (s != NULL)
   {
      char *p;

      for (p = s; *p && p; p++)
         *p = tolower(*p);
   }
   
   return s;
}

int DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryStringCount(FreeImageIcsPointer fip)
{
	int number_of_history_strings;
	
	/* Get ICS history */
	if (IcsGetNumHistoryStrings  (fip->ip, &number_of_history_strings) != IcsErr_Ok)
		return 0;

	if(!number_of_history_strings)
		return 0;
	
	return number_of_history_strings;
}


FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryBeginIterator(FreeImageIcsPointer fip)
{
	fip->number_of_history_strings = FreeImageAlgorithms_GetIcsHistoryStringCount(fip);

	if(!fip->number_of_history_strings)
		return FREEIMAGE_ALGORITHMS_ERROR;

	fip->iterator = 1;

	return fip->iterator;
}

FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryEndIterator(FreeImageIcsPointer fip)
{
	return fip->number_of_history_strings + 1;
}


FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryNextIterator(FreeImageIcsPointer fip)
{
	if(fip->iterator <= fip->number_of_history_strings)
		++(fip->iterator);

	return fip->iterator;
}


int DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryString(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator iterator, char *history_string)
{
	if(!fip->number_of_history_strings)
		return FREEIMAGE_ALGORITHMS_ERROR;

	IcsGetHistoryString (fip->ip, history_string, (iterator > 1) ? IcsWhich_Next : IcsWhich_First);	
	
	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryKeyValue(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator iterator, char *key, char *value)
{
	char history_string[ICS_LINE_LENGTH];

	FreeImageAlgorithms_GetIcsHistoryString(fip, iterator, history_string);

	if(FreeImageAlgorithms_SplitHistoryString(history_string, key, value) == FREEIMAGE_ALGORITHMS_ERROR) // can't find the splitting character
		return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryIteratorForKey(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator start_iterator, char *key)
{
	char found_value[ICS_LINE_LENGTH];
	char search_key[ICS_LINE_LENGTH];
	char found_key[ICS_LINE_LENGTH];

	strcpy(search_key, key);

	while(start_iterator != FreeImageAlgorithms_GetIcsHistoryEndIterator(fip)) {

		FreeImageAlgorithms_GetIcsHistoryKeyValue(fip, start_iterator, found_key, found_value);

		strlwr(found_key);
		strlwr(search_key);

		if(strcmp(search_key, found_key) == 0)	
			return start_iterator;

		start_iterator = FreeImageAlgorithms_GetIcsHistoryNextIterator(fip);
	}

	return FREEIMAGE_ALGORITHMS_ERROR;
}


int DLL_CALLCONV
FreeImageAlgorithms_SplitHistoryString(char *history_string, char *key, char *value)
{
	char *s;

	if((s = strchr (history_string, 0x09)) == NULL) // Finds the splitting character
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	strncpy (key, history_string, (s - history_string)); // Copy the key
	key[(s - history_string)] = '\0'; // Add NULL char
	
	strcpy(value, (s+1));  // copy the value

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_JoinKeyValueIntoHistoryString(char *history_string, char *key, char *value)
{
	strcpy(history_string, key);

	size_t length = strlen(history_string);
	
	history_string[length] = 0x09;
	history_string[length + 1] = '\0';
	strcat(history_string, value);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_GetFirstIcsHistoryValueWithKey(FreeImageIcsPointer fip, char *key, char *value)
{
	char found_value[ICS_LINE_LENGTH];
	char found_key[ICS_LINE_LENGTH];

	FreeImageIcsHistoryIterator iterator = FreeImageAlgorithms_GetIcsHistoryBeginIterator(fip);

	FreeImageIcsHistoryIterator found_iterator = FreeImageAlgorithms_GetIcsHistoryIteratorForKey(fip, iterator, key);

	FreeImageAlgorithms_GetIcsHistoryKeyValue(fip, found_iterator, found_key, found_value);

	strcpy(value, found_value);

	return FREEIMAGE_ALGORITHMS_ERROR;
}


int DLL_CALLCONV
FreeImageAlgorithms_ReplaceIcsHistoryValueForKey(FreeImageIcsPointer fip, char *key, char *value)
{
	char history_key[ICS_LINE_LENGTH], search_key[ICS_LINE_LENGTH];
	char history_value[ICS_LINE_LENGTH], history_string[ICS_LINE_LENGTH];
	char **history_strings = NULL;
	int number_of_strings;

	strcpy(search_key, key);
	strlwr(search_key);

	if (IcsGetNumHistoryStrings (fip->ip, &number_of_strings) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Save history to array location
	history_strings = (char **) malloc(sizeof(char*) * number_of_strings);

	CopyHistoryStringsToArray(fip, &history_strings, &number_of_strings);

	if(ReOpenExistingIcsFileInWriteMode(fip, 0) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	for(int i=0; i < number_of_strings; i++) {

		if(FreeImageAlgorithms_SplitHistoryString(history_strings[i], history_key, history_value) == FREEIMAGE_ALGORITHMS_ERROR) 
			return FREEIMAGE_ALGORITHMS_ERROR;  // can't find the splitting character

		strlwr(history_key);
	
		if(strcmp(search_key, history_key) == 0) {

			FreeImageAlgorithms_JoinKeyValueIntoHistoryString(history_string, key, value);
			IcsAddHistory (fip->ip, NULL, history_string);	
		}
		else
			IcsAddHistory (fip->ip, NULL, history_strings[i]);	

		free(history_strings[i]);
	}

	free(history_strings);

	return FREEIMAGE_ALGORITHMS_ERROR;
}


int DLL_CALLCONV
FreeImageAlgorithms_SetIcsHistoryKeyValueStrings(FreeImageIcsPointer fip, ...)
{
	if(ReOpenExistingIcsFileInWriteMode(fip, 0) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	char *history_key, *history_string;

	va_list ap;
	va_start(ap, fip);

	while((history_key = va_arg(ap, char*)) != NULL)
	{
		history_string = va_arg(ap, char*);
		IcsAddHistory (fip->ip, history_key, history_string);
	}
			
	va_end(ap);

	return FREEIMAGE_ALGORITHMS_SUCCESS; 
}


int DLL_CALLCONV
FreeImageAlgorithms_SetIcsHistoryStrings(FreeImageIcsPointer fip, ...)
{
	if(ReOpenExistingIcsFileInWriteMode(fip, 0) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	char *history_string;

	va_list ap;
	va_start(ap, fip);

	while((history_string = va_arg(ap, char*)) != NULL)
		IcsAddHistory (fip->ip, NULL, history_string);
			
	va_end(ap);

	return FREEIMAGE_ALGORITHMS_SUCCESS; 
}


int DLL_CALLCONV
FreeImageAlgorithms_SetIcsHistoryKeyValueStringsFromArray(FreeImageIcsPointer fip, char **history_strings, int number_of_items)
{
	if(ReOpenExistingIcsFileInWriteMode(fip, 0) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	for(int i=0; i < number_of_items; i++)
		IcsAddHistory (fip->ip, NULL, history_strings[i]);	

	return FREEIMAGE_ALGORITHMS_SUCCESS; 
}


int DLL_CALLCONV
FreeImageAlgorithms_AddIcsHistoryString(FreeImageIcsPointer fip, char *history_string)
{
	if(ReOpenExistingIcsFileInWriteMode(fip, 1) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	IcsAddHistory (fip->ip, NULL, history_string);	

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}


int DLL_CALLCONV
FreeImageAlgorithms_AddIcsHistoryKeyValue(FreeImageIcsPointer fip, char *key, char *value)
{
	if(ReOpenExistingIcsFileInWriteMode(fip, 1) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	IcsAddHistory (fip->ip, key, value);

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}


int DLL_CALLCONV
FreeImageAlgorithms_AddIcsHistoryKeyValueStrings(FreeImageIcsPointer fip, ...)
{
	char **history_strings = NULL;

	if(ReOpenExistingIcsFileInWriteMode(fip, 1) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	char *history_key, *history_string;

	va_list ap;
	va_start(ap, fip);

	while((history_key = va_arg(ap, char*)) != NULL)
	{
		history_string = va_arg(ap, char*);
		IcsAddHistory (fip->ip, history_key, history_string);
	}
			
	va_end(ap);

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}