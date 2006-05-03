#ifndef __FREEIMAGE_ALGORITHMS_PRIVATE__
#define __FREEIMAGE_ALGORITHMS_PRIVATE__

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include "libics.h"

struct FreeImageIcsStr
{
	struct _ICS* ip;
	int iterator;
	int number_of_history_strings;
	char access_mode[5];
	char filepath[500];

	// Buffer need for write mode.
	// This has to be deleted only after Close has been called, which is why it is here.
	BYTE *buf; 
};


FIBITMAP* GetIcsDimensionXYImage(ICS *ics, ...);


int
CopyHistoryStringsToArray(FreeImageIcsPointer fip, char *** history_strings, int *number_of_strings);

int 
ReOpenExistingIcsFileInWriteMode(FreeImageIcsPointer fip, bool maintain_history);

#endif