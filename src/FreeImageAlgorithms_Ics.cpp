#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Ics.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Private.h"
#include "FreeImageAlgorithms_MetaData.h"

#include <iostream>

// Old ics file save the data as all the red pixels then all the greens and then all the blues
// Yes crap - As this is slower as it requires three passes.
static FIBITMAP*
LoadOldICSColourImage(BYTE *data, int width, int height, int padded)
{
	FIBITMAP 	*dib; 
	BYTE *bits, *buffer;
	register int channel, x, y;
	int data_line_length;   

	if ( (dib = FreeImage_AllocateT (FIT_BITMAP, width, height, 24, 0, 0, 0)) == NULL )
		return NULL;
			
	if(padded)
		data_line_length = FreeImage_GetPitch(dib);
	else
		data_line_length = FreeImage_GetLine(dib);   
			
	bits = (BYTE *) FreeImage_GetBits(dib);
	buffer = (BYTE *) data;
			
	// blue == 0; green = 1; red == 2
	for(channel = 2; channel >= 0; channel-- )
	{
		for(y = 0; y < height; y++) {

			bits = (BYTE *) FreeImage_GetScanLine(dib, height - y - 1);

			for( x = channel; x < data_line_length; x+=3) {
				
				bits[x] = buffer[0];  
		
				buffer++;
			}
		}
	 }
	  
	return dib;
}


static FREE_IMAGE_TYPE
IcsTypeToFreeImageType (Ics_DataType icsType)
{
	switch (icsType)
	{
	 case Ics_sint8:
	 case Ics_uint8:
	 
		 return FIT_BITMAP;
		 
	 case Ics_sint16:
	 
		 return FIT_INT16;
		 
	 case Ics_uint16:
	 
		 return FIT_UINT16;
		 
	 case Ics_sint32:
	 
		 return FIT_INT32;
		 
	 case Ics_uint32:
	 
		 return FIT_UINT32;
		 
	 case Ics_real32:
	 
		 return FIT_FLOAT;
		 
	 case Ics_real64:
	 
		 return FIT_DOUBLE;
	}
	
	return FIT_UNKNOWN;
}


static Ics_DataType
FreeImageTypeToIcsType (FREE_IMAGE_TYPE fit)
{
	switch (fit)
	{
	 	case FIT_BITMAP:	
	
	 		return Ics_uint8;
	 
		case FIT_INT16:
		
			return Ics_sint16;
	
		case FIT_UINT16:
		
			return Ics_uint16;
			
		case FIT_INT32:
		
			return Ics_sint32;
			
		case FIT_UINT32:
		
			return Ics_uint32;
			
		case FIT_FLOAT:
		
			return Ics_real32;
			
		case FIT_DOUBLE:
		
			return Ics_real64;
	}		
	
	return Ics_unknown;
}

static int
GetIcsDataTypeBPP (Ics_DataType icsType)
{
	switch (icsType)
	{
	 case Ics_sint8:
		 return 8;
		 
	 case Ics_uint8:
		 return 8;
		 
	 case Ics_sint16:
		 return 16;
		 
	 case Ics_uint16:
		 return 16;
		 
	 case Ics_sint32:
		 return 32;
		 
	 case Ics_uint32:
		 return 32;
		 
	 case Ics_real32:
		 return 32;
		 
	 case Ics_real64:
		 return 64;
	}
	
	return 0;
}

// Checks to see that the ics image data is padded to be byte aligned.
// GCI does not save data with padded bytes!
// Assumes colour image data is 3 dimensions (Yes is a problem 
// if we have genuine > 3 dimensional data.
int DLL_CALLCONV
IsIcsFilePaddded (char *filepath)
{
	Ics_DataType dt;
	int ndims, bytes;
	size_t dims[ICS_MAXDIM];
	size_t dataSize, bufsize;

	ICS* ip;
	Ics_Error retval;

	if((retval = IcsOpen (&ip, filepath, "r")) != IcsErr_Ok)
   		return -1;

	IcsGetLayout (ip, &dt, &ndims, dims);
	
	bytes = GetIcsDataTypeBPP (dt) / 8; 
	
	if( ndims == 2)
		dataSize = dims[0] * dims[1] * bytes;  
	else if( ndims == 3 )
		dataSize = dims[0] * dims[1] * dims[2] * bytes;
	else
		return -1;

	bufsize = IcsGetDataSize (ip);

	if((retval = IcsClose (ip)) != IcsErr_Ok)
   		return -1;

	if(dataSize > bufsize)
		return 1;

	return 0;
}

static FIBITMAP*
LoadFIBFromGreyScaleIcsFile (ICS *ip, int padded)
{
	FIBITMAP *dib;
	FREE_IMAGE_TYPE type;
	Ics_DataType dt;
	Ics_Error retval;
	size_t bufsize;
	char *buf;
	int ndims, width, height, bpp;
	int dims[ICS_MAXDIM];

	IcsGetLayout (ip, &dt, &ndims, (size_t *) dims);

	if( ndims == 3 )
		return NULL;

	type = IcsTypeToFreeImageType (dt);   
	bpp = GetIcsDataTypeBPP (dt); 
	width = dims[0];
	height = dims[1];
	
	bufsize = IcsGetDataSize (ip);
	buf = (char *) malloc (bufsize);

	if (buf == NULL)
   		return NULL;

	retval = IcsGetData (ip, buf, bufsize);

	if (retval != IcsErr_Ok)
   		return NULL;
	
	if ( (dib = FreeImageAlgorithms_LoadGreyScaleFIBFromArrayData(buf, bpp, width, height, type, padded)) == NULL)
			return NULL;
		
	free(buf);

	return dib;
}


static FIBITMAP*
LoadFIBFromColourIcsFile (ICS *ip, int padded)
{
	FIBITMAP *dib;
	FREE_IMAGE_TYPE type;
	Ics_DataType dt;
	Ics_Error retval;
	size_t bufsize;
	BYTE *buf;
	int ndims, width, height, channels = 1, bpp;
	int dims[ICS_MAXDIM];

	IcsGetLayout (ip, &dt, &ndims, (size_t *) dims);

	if( ndims != 3 )
		return NULL;

	type = IcsTypeToFreeImageType (dt);   
	bpp = GetIcsDataTypeBPP (dt); 
	width = dims[0];
	height = dims[1]; 
	channels = dims[2];
	
	bufsize = IcsGetDataSize (ip);
	buf = (BYTE *) malloc (bufsize);

	if (buf == NULL)
   		return NULL;

	retval = IcsGetData (ip, buf, bufsize);

	if (retval != IcsErr_Ok)
   		return NULL;
	
	if ( (dib = LoadOldICSColourImage(buf, width, height, padded)) == NULL)
		return NULL;
		
	free(buf);
	
	return dib;
}


static void
GetOldICSColourImageData(FIBITMAP *dib, void *data)
{
	register int channel, x, y;  
	int height = FreeImage_GetHeight(dib);
	int pitch = FreeImage_GetPitch(dib);

	BYTE *bits = FreeImage_GetBits(dib);
	BYTE *buffer = (BYTE *) data;		
	
	for(channel = 2; channel >= 0; channel-- )
	{
		for(y = 0; y < height; y++) {

			bits = (BYTE *) FreeImage_GetScanLine(dib, height - y - 1);

			for( x = channel; x < pitch; x+=3)
				*buffer++ = bits[x];
		}
	}
}


static void
FreeImage_GetBitsVerticalFlip(FIBITMAP *dib, BYTE *bytes)
{
	register int y;
	BYTE *scanline_bytes;
	int height = FreeImage_GetHeight(dib);
	int pitch = FreeImage_GetPitch(dib); 
	
	for( y = 0; y < height; y++) {

		scanline_bytes = (BYTE *) FreeImage_GetScanLine(dib, height - y - 1);

		memcpy( bytes, scanline_bytes, pitch ); 
				
		bytes += pitch;
	}
}

static int
SaveFIBToIcsPointer (FIBITMAP *src, ICS* ics)
{
	Ics_Error err;
	int ndims = 2, dims[3], channels = 1;
	size_t bufsize_in_bytes;
	
	FIBITMAP *dib = FreeImage_Clone(src);

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);
	int width = FreeImage_GetWidth(dib); 
	int height = FreeImage_GetHeight(dib);
	int pitch = FreeImage_GetPitch(dib);
	int bpp = FreeImage_GetBPP(dib);
	int bytes_per_pixel = bpp / 8;
	Ics_DataType dt = FreeImageTypeToIcsType(type); 
	
	// ICS doesn't support 24bit images
	if(bpp == 24 && type == FIT_BITMAP) {
	
		channels = 3;	 /* number of samples per pixel */
		dt = Ics_uint8;  /* channel Size */
		ndims = 3;
		
		dims[0] = pitch / bytes_per_pixel; 
        dims[1] = height;
        dims[2] = channels; 
		
		bufsize_in_bytes = pitch * height; 
	}
	else {

		dims[0] = pitch / bytes_per_pixel;
		dims[1] = height;
	
		bufsize_in_bytes = (height * pitch);
	}
	
	BYTE *bits =(BYTE*) malloc(bufsize_in_bytes);

	if(bpp == 24 && ndims == 3)
		GetOldICSColourImageData(dib, bits);
	else
		FreeImage_GetBitsVerticalFlip(dib, bits);

	if( IcsSetLayout(ics, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if( (err = IcsSetData(ics, bits, bufsize_in_bytes)) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
		
	if( IcsSetCompression (ics, IcsCompr_gzip, 0) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if( IcsClose (ics) != IcsErr_Ok) {
		
		FreeImage_Unload(dib);
		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	free(bits);
		
	FreeImage_Unload(dib);
	
	return FREEIMAGE_ALGORITHMS_SUCCESS; 
}

int DLL_CALLCONV
FreeImageAlgorithms_IsIcsFile (const char *filepath)
{
	return IcsVersion (filepath) ? 1 : 0;
}


int
CopyHistoryStringsToArray(FreeImageIcsPointer fip, char *** history_strings, int *number_of_strings)
{
	if (IcsGetNumHistoryStrings (fip->ip, number_of_strings) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(*number_of_strings <= 0)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Save history to array location
	*history_strings = (char **) malloc(sizeof(char*) * *number_of_strings);

	for(int i=0; i < *number_of_strings; i++) {
		
		(*history_strings)[i] = (char *) malloc(sizeof(char) * 40);
		IcsGetHistoryString (fip->ip, (*history_strings)[i], (i > 0) ? IcsWhich_Next : IcsWhich_First);	
	}
		
	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int 
ReOpenExistingIcsFileInWriteMode(FreeImageIcsPointer fip, bool maintain_history)
{
	Ics_DataType dt;
	Ics_Error err;
	size_t bufsize;
	int ndims;
	int dims[ICS_MAXDIM];
	char **history_strings = NULL;
	int number_of_strings;

	if(FreeImageAlgorithms_IsIcsFileInWriteMode(fip))
		return FREEIMAGE_ALGORITHMS_SUCCESS;  

	if (IcsClose (fip->ip) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	if(IcsOpen (&(fip->ip), fip->filepath, "r") != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if (IcsGetLayout (fip->ip, &dt, &ndims, (size_t *) dims) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	bufsize = IcsGetDataSize (fip->ip);

	if((fip->buf = (BYTE *) malloc (bufsize)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if ((err = IcsGetData (fip->ip, fip->buf, bufsize)) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	if (IcsGetNumHistoryStrings (fip->ip, &number_of_strings) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(maintain_history) {

		// Save history to array location
		history_strings = (char **) malloc(sizeof(char*) * number_of_strings);

		CopyHistoryStringsToArray(fip, &history_strings, &number_of_strings);
	}

	if (IcsClose (fip->ip) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	// Write the file again
	if(IcsOpen (&(fip->ip), fip->filepath, "w2") != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if( IcsSetLayout(fip->ip, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if( IcsSetData(fip->ip, fip->buf, bufsize) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if(maintain_history) {

		for(int i=0; i < number_of_strings; i++) {
			IcsAddHistory (fip->ip, NULL, history_strings[i]);	
			free(history_strings[i]);
		}

		free(history_strings);
	}

	if( IcsSetCompression (fip->ip, IcsCompr_gzip, 0) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}


int DLL_CALLCONV
FreeImageAlgorithms_IsIcsFileInWriteMode(FreeImageIcsPointer fip)
{
	if(strcmp(fip->access_mode, "w") == 0 || strcmp(fip->access_mode, "w2") == 0)
		return 1;

	return 0;
}


static bool file_exists(const char *filepath)
{
    FILE *file_p;

    if((file_p = fopen(filepath, "r")))
    {
       fclose(file_p);
       return true;
    }
    return false;
}


int DLL_CALLCONV
FreeImageAlgorithms_OpenIcsFile(FreeImageIcsPointer *fip, const char *filepath, const char *access_mode)
{
	*fip = (FreeImageIcsPointer) malloc(sizeof(FreeImageIcs));

	(*fip)->buf = NULL;
	(*fip)->number_of_history_strings = 0;
	(*fip)->iterator = 1;

	strcpy((*fip)->filepath, filepath);
	strcpy((*fip)->access_mode, access_mode);

	if(strcmp(access_mode, "r") == 0) {

		// Justs looks at the extension
		if(!IcsVersion (filepath))
			return FREEIMAGE_ALGORITHMS_ERROR;

		if(IcsOpen (&((*fip)->ip), filepath, "r") != IcsErr_Ok)
			return FREEIMAGE_ALGORITHMS_ERROR;
	}
	else {

		if(file_exists(filepath)) {
		
			if(!IcsVersion (filepath))
				return FREEIMAGE_ALGORITHMS_ERROR;

			if(ReOpenExistingIcsFileInWriteMode(*fip, 1) == FREEIMAGE_ALGORITHMS_ERROR)
				return FREEIMAGE_ALGORITHMS_ERROR;
		}
		else {

			// No such file do a norm open.
			if(IcsOpen (&((*fip)->ip), filepath, access_mode) != IcsErr_Ok)
				return FREEIMAGE_ALGORITHMS_ERROR;
		}
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}


int DLL_CALLCONV
FreeImageAlgorithms_CloseIcsFile(FreeImageIcsPointer fip)
{
	Ics_Error err = IcsClose (fip->ip);

	if(fip->buf != NULL) {
		free(fip->buf);
		fip->buf = NULL;
	}

	free(fip);
    
	if(err != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;		

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_LoadFIBFromIcsFile (const char *filepath, int padded)
{
	FIBITMAP *dib;
	ICS* ip;
	Ics_DataType dt;
	int ndims, channels = 1;
	int dims[ICS_MAXDIM];

	if(!IcsVersion (filepath))
		return NULL;
	
	if(IcsOpen (&ip, filepath, "r") != IcsErr_Ok)
   		return NULL;
   		
	IcsGetLayout (ip, &dt, &ndims, (size_t *) dims);

	if( ndims == 3 ) {
		dib = LoadFIBFromColourIcsFile (ip, padded); 
	}
	else {
		dib = LoadFIBFromGreyScaleIcsFile (ip, padded); 
	}
	
	if(IcsClose (ip) != IcsErr_Ok)
		return NULL;

	return dib;
}

int DLL_CALLCONV
FreeImageAlgorithms_SaveFIBToIcsFile (FIBITMAP *dib, const char *pathname)
{
	ICS* ip;
	Ics_Error retval;
	FIBITMAP *standard_dib;

	retval = IcsOpen (&ip, pathname, "w2");

	if (retval != IcsErr_Ok) {
   		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	if(!FreeImageAlgorithms_IsGreyScale(dib) && FreeImage_GetBPP(dib) != 24)
		standard_dib = FreeImage_ConvertTo24Bits(dib);
	else
		standard_dib = FreeImage_Clone(dib);

	if ( SaveFIBToIcsPointer (standard_dib, ip) == FREEIMAGE_ALGORITHMS_ERROR ) {
		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	FreeImage_Unload(standard_dib);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}
