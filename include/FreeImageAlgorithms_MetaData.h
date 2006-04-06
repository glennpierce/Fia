#ifndef __FREEIMAGE_ALGORITHMS_METADATA__
#define __FREEIMAGE_ALGORITHMS_METADATA__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Ics.h"

#define ICS_LINE_LENGTH 256

typedef int FreeImageIcsHistoryIterator;

/*
Determines if the ics file is opened in write mode.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_IsIcsFileInWriteMode(FreeImageIcsPointer fip);

/*
Get the number of history strings within the ics file.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryStringCount(FreeImageIcsPointer fip);

/*
Get the iterator which points to the first history string.
*/
DLL_API FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryBeginIterator(FreeImageIcsPointer fip);

/*
Get the iterator which points to the last history string.
*/
DLL_API FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryEndIterator(FreeImageIcsPointer fip);

/*
Get the next iterator.
*/
DLL_API FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryNextIterator(FreeImageIcsPointer fip);


/*
Split a history string into key and value.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SplitHistoryString(char *history_string, char *key, char *value);

/*
Join a key and value to construct a well formatted ics history string.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_JoinKeyValueIntoHistoryString(char *history_string, char *key, char *value);

/*
Get the iterator which points to history string with the key specified.
*/
DLL_API FreeImageIcsHistoryIterator DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryIteratorForKey(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator start_iterator, char *key);

/*
Get the string pointed to by iterator.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryString(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator iterator, char *history_string);

/*
Get the key and value pointed to by iterator.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetIcsHistoryKeyValue(FreeImageIcsPointer fip, FreeImageIcsHistoryIterator iterator, char *key, char *value);

/*
Get the first history string value for the corresponding key.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetFirstIcsHistoryValueWithKey(FreeImageIcsPointer fip, char *key, char *value);

/*
Sets all the history string in one go.
The var argument must end in NULL.
Old history strings are removed.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetIcsHistoryStrings(FreeImageIcsPointer fip, ...);

/*
Sets all the history keys and values in one go.
The var argument must end in NULL.
Old history strings are removed.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetIcsHistoryKeyValueStrings(FreeImageIcsPointer fip, ...);


/*
Add history_String.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_AddIcsHistoryString(FreeImageIcsPointer fip, char *history_string);

/*
Add key value to history.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_AddIcsHistoryKeyValue(FreeImageIcsPointer fip, char *key, char *value);


/*
Add more than one history strings in one go.
The var argument must end in NULL.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_AddIcsHistoryKeyValueStrings(FreeImageIcsPointer fip, ...);

/*
Add history strings stored in an array.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SetIcsHistoryKeyValueStringsFromArray(FreeImageIcsPointer fip, char **history_strings, int number_of_items);

/*
Replace history string stored in an array.
*/
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_ReplaceIcsHistoryValueForKey(FreeImageIcsPointer fip, char *key, char *value);


#ifdef __cplusplus
}
#endif

#endif