#ifndef __FREEIMAGE_ALGORITHMS_ICS__
#define __FREEIMAGE_ALGORITHMS_ICS__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeImageAlgorithms.h"


/* Defines 'FreeImageIcsPointer' as a pointer to a structure that is not
 * defined here and so is not accessible to the outside world
 */
typedef struct FreeImageIcsStr FreeImageIcs, *FreeImageIcsPointer;


DLL_API int DLL_CALLCONV
FreeImageAlgorithms_IsIcsFile (const char *filepath);

DLL_API int DLL_CALLCONV 
FreeImageAlgorithms_OpenExistingIcsFileInWriteMode(FreeImageIcsPointer *fip, const char *filepath, int maintain_history);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_OpenIcsFile(FreeImageIcsPointer *fip, const char *filepath, const char *access_mode);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_CloseIcsFile(FreeImageIcsPointer fip);

DLL_API int DLL_CALLCONV
IsIcsFilePaddded (char *filepath);

DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadFIBFromIcsFile (const char *filepath, int padded);

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_SaveFIBToIcsFile (FIBITMAP *dib, const char *pathname);

#ifdef __cplusplus
}
#endif

#endif