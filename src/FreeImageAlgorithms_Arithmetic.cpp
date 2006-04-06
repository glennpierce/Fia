#include "FreeImageAlgorithms_Arithmetic.h"
#include "FreeImageAlgorithms_Utilities.h"
#include <limits>
#include <float.h>

template<class Tsrc>
class TRANSPOSE
{
public:
	FIBITMAP* convert(FIBITMAP *src);
};


template<class Tsrc> FIBITMAP* 
TRANSPOSE<Tsrc>::convert(FIBITMAP *src) {

	FIBITMAP *dst;
	int x, y;	
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);
	int bpp = FreeImage_GetBPP(src);
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);

	if ( (dst = FreeImage_AllocateT(type, height, width, bpp, 0, 0, 0)) == NULL )
		return NULL;

	Tsrc in_pix, *in_bits, *out_bits;  
		
	for(y = 0; y < height; y++) { 
		
		in_bits = (Tsrc *)FreeImage_GetScanLine(src, y);
	
		for(x=0; x < width; x++) {
		
			in_pix = in_bits[x];
	
			out_bits = (Tsrc *) FreeImage_GetScanLine(dst, width - x - 1);
			out_bits[height - y - 1] = in_pix; 
		}
	}
	
	return dst;
}

// Convert from type X to type BYTE
TRANSPOSE<unsigned char>		transposeUCharImage;
TRANSPOSE<unsigned short>		transposeUShortImage;
TRANSPOSE<short>				transposeShortImage;
TRANSPOSE<unsigned long>		transposeULongImage;
TRANSPOSE<long>					transposeLongImage;
TRANSPOSE<float>				transposeFloatImage;
TRANSPOSE<double>				transposeDoubleImage;


DLL_API FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_Transpose(FIBITMAP *src)
{
	FIBITMAP *dst = NULL;

	if(!src)
		return NULL;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				dst = transposeUCharImage.convert(src);
			break;
		case FIT_UINT16:	// array of unsigned short: unsigned 16-bit
			dst = transposeUShortImage.convert(src);
			break;
		case FIT_INT16:		// array of short: signed 16-bit
			dst = transposeShortImage.convert(src);
			break;
		case FIT_UINT32:	// array of unsigned long: unsigned 32-bit
			dst = transposeULongImage.convert(src);
			break;
		case FIT_INT32:		// array of long: signed 32-bit
			dst = transposeLongImage.convert(src);
			break;
		case FIT_FLOAT:		// array of float: 32-bit
			dst = transposeFloatImage.convert(src);
			break;
		case FIT_DOUBLE:	// array of double: 64-bit
			dst = transposeDoubleImage.convert(src);
			break;
		case FIT_COMPLEX:	// array of FICOMPLEX: 2 x 64-bit
			break;
	}

	if(NULL == dst) {
		FreeImage_OutputMessageProc(FIF_UNKNOWN, "FREE_IMAGE_TYPE: Unable to convert from type %d to type %d.\n No such conversion exists.", src_type, FIT_BITMAP);
	}

	return dst;
}



template<class Tsrc>
class ARITHMATIC
{
public:
	static double GetTypeMax(FIBITMAP* src);
	static int CheckParameters(FIBITMAP* dst, FIBITMAP* src, Tsrc constant);
	int Add(FIBITMAP* dst, FIBITMAP* src, Tsrc constant);
	int Divide(FIBITMAP* dst, FIBITMAP* src, Tsrc constant);
};


template<class Tsrc> double 
ARITHMATIC<Tsrc>::GetTypeMax(FIBITMAP* src)
{
	int bpp = FreeImage_GetBPP(src);
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);	

	switch(bpp)
	{
		case 8:
			return UCHAR_MAX;

		case 16:
		{
			if(src_type == FIT_UINT16)
				return USHRT_MAX;
			else
				return SHRT_MAX;
		}

		case 32:
		{
			if(src_type == FIT_INT32)
				return INT_MAX;
			else if(src_type == FIT_UINT32)
				return UINT_MAX;
			else if(src_type == FIT_FLOAT)
				return FLT_MAX;
		}
	}

	return 0;
}


template<class Tsrc> int 
ARITHMATIC<Tsrc>::CheckParameters(FIBITMAP* dst, FIBITMAP* src, Tsrc constant)
{
	if(dst == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);
	FREE_IMAGE_TYPE dst_type = FreeImage_GetImageType(dst);

	if( dst_type == FIT_COMPLEX || !FreeImageAlgorithms_IsGreyScale(dst))
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(constant)
		return FREEIMAGE_ALGORITHMS_SUCCESS;

	int src_bpp = FreeImage_GetBPP(src);
	int dst_bpp = FreeImage_GetBPP(dst);
	
	// destination bpp has to be greater than the src's
	if(dst_bpp < src_bpp)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if( src_type == FIT_COMPLEX || !FreeImageAlgorithms_IsGreyScale(src))
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Check src is the same size as dst
	int src_width = FreeImage_GetWidth(src);
	int src_height = FreeImage_GetHeight(src);
	int dst_width = FreeImage_GetWidth(dst);
	int dst_height = FreeImage_GetHeight(dst);

	if(src_width != dst_width || src_height != dst_height)
			return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


template<class Tsrc> int 
ARITHMATIC<Tsrc>::Add(FIBITMAP* dst, FIBITMAP* src, Tsrc constant)
{
	if(ARITHMATIC<Tsrc>::CheckParameters(dst, src, constant) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	Tsrc *dst_ptr = (Tsrc *) FreeImage_GetBits(dst);
	Tsrc *src_ptr;

	if(src != NULL)
		src_ptr = (Tsrc *) FreeImage_GetBits(src);

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	double max = ARITHMATIC<Tsrc>::GetTypeMax(dst);

	// large data type to hold all pssible values
	double pix;

	for(int i=0; i < number_of_pixels; i++) {
			
		if(src != NULL)
			pix = *dst_ptr++ + *src_ptr++;
		else {	
			if(constant)
				pix = *src_ptr++ + constant;
		}
						
		if(pix > max)
			pix = max;	
				
		*dst_ptr++ = (Tsrc) pix;
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


template<class Tsrc> int 
ARITHMATIC<Tsrc>::Divide(FIBITMAP* dst, FIBITMAP* src, Tsrc constant)
{
	if(ARITHMATIC<Tsrc>::CheckParameters(dst, src, constant) == FREEIMAGE_ALGORITHMS_ERROR)
		return FREEIMAGE_ALGORITHMS_ERROR;

	Tsrc *dst_ptr = (Tsrc *) FreeImage_GetBits(dst);
	Tsrc *src_ptr;

	if(src != NULL)
		src_ptr = (Tsrc *) FreeImage_GetBits(src);

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	double max = ARITHMATIC<Tsrc>::GetTypeMax(dst);

	// large data type to hold all pssible values
	double pix;

	for(int i=0; i < number_of_pixels; i++) {
			
		if(src != NULL)
			pix = (double) *dst_ptr++ / *src_ptr++;
		else {	
			if(constant)
				pix = (double) *src_ptr++ / constant;
		}
						
		if(pix > max)
			pix = max;	
				
		*dst_ptr++ = (Tsrc) pix;
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


ARITHMATIC<unsigned char>		arithmaticUCharImage;
ARITHMATIC<unsigned short>		arithmaticUShortImage;
ARITHMATIC<short>				arithmaticShortImage;
ARITHMATIC<unsigned long>		arithmaticULongImage;
ARITHMATIC<long>				arithmaticLongImage;
ARITHMATIC<float>				arithmaticFloatImage;
ARITHMATIC<double>				arithmaticDoubleImage;


int DLL_CALLCONV 
FreeImageAlgorithms_Add(FIBITMAP* dst, FIBITMAP* src, double constant)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	
			if(FreeImage_GetBPP(src) == 8)
				return arithmaticUCharImage.Add(dst, src, (unsigned char) constant);
		case FIT_UINT16:	
			return arithmaticUShortImage.Add(dst, src, (unsigned short) constant);
		case FIT_INT16:		
			return arithmaticShortImage.Add(dst, src, (short) constant);
		case FIT_UINT32:	
			return arithmaticULongImage.Add(dst, src, (unsigned long) constant);
		case FIT_INT32:		
			return arithmaticLongImage.Add(dst, src, (long) constant);
		case FIT_FLOAT:	
			return arithmaticFloatImage.Add(dst, src, (float) constant);	
	}

	return FREEIMAGE_ALGORITHMS_ERROR; 
}


int DLL_CALLCONV 
FreeImageAlgorithms_Divide(FIBITMAP* dst, FIBITMAP* src, double constant)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	
			if(FreeImage_GetBPP(src) == 8)
				return  arithmaticUCharImage.Divide(dst, src, (unsigned char) constant);
		case FIT_UINT16:	
			return arithmaticUShortImage.Divide(dst, src, (unsigned short) constant);
		case FIT_INT16:		
			return arithmaticShortImage.Divide(dst, src, (short) constant);
		case FIT_UINT32:	
			return arithmaticULongImage.Divide(dst, src, (unsigned long) constant);
		case FIT_INT32:		
			return arithmaticLongImage.Divide(dst, src, (long) constant);
		case FIT_FLOAT:	
			return arithmaticFloatImage.Divide(dst, src, (float) constant);
	}

	return FREEIMAGE_ALGORITHMS_ERROR; 
}



