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
class GREYLEVEL_ARITHMATIC
{
public:
	int AddImages(FIBITMAP* dst, FIBITMAP* src);
	int SubtractImages(FIBITMAP* dst, FIBITMAP* src);
	int DivideImages(FIBITMAP* dst, FIBITMAP* src);
	int MultiplyImages(FIBITMAP* dst, FIBITMAP* src);
};


static int CheckDimensions(FIBITMAP* dst, FIBITMAP* src)
{
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
GREYLEVEL_ARITHMATIC<Tsrc>::MultiplyImages(FIBITMAP* dst, FIBITMAP* src)
{
	if(dst == NULL || src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(CheckDimensions(dst, src) ==  FREEIMAGE_ALGORITHMS_ERROR)
		return  FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_DOUBLE)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double *dst_ptr = (double *) FreeImage_GetBits(dst);
	Tsrc *src_ptr = (Tsrc *) FreeImage_GetBits(src);

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	for(int i=0; i < number_of_pixels; i++)
		*dst_ptr++ = (double) *dst_ptr * *src_ptr++;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


template<class Tsrc> int 
GREYLEVEL_ARITHMATIC<Tsrc>::DivideImages(FIBITMAP* dst, FIBITMAP* src)
{
	if(dst == NULL || src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(CheckDimensions(dst, src) ==  FREEIMAGE_ALGORITHMS_ERROR)
		return  FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_DOUBLE)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double *dst_ptr = (double *) FreeImage_GetBits(dst);
	Tsrc *src_ptr = (Tsrc *) FreeImage_GetBits(src);

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	for(int i=0; i < number_of_pixels; i++)
		*dst_ptr++ = (double) *dst_ptr / *src_ptr++;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

template<class Tsrc> int 
GREYLEVEL_ARITHMATIC<Tsrc>::AddImages(FIBITMAP* dst, FIBITMAP* src)
{
	if(dst == NULL || src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(CheckDimensions(dst, src) ==  FREEIMAGE_ALGORITHMS_ERROR)
		return  FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_DOUBLE)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double *dst_ptr = (double *) FreeImage_GetBits(dst);
	Tsrc *src_ptr = (Tsrc *) FreeImage_GetBits(src);

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	for(int i=0; i < number_of_pixels; i++)
		*dst_ptr++ = (double) *dst_ptr + *src_ptr++;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

template<class Tsrc> int 
GREYLEVEL_ARITHMATIC<Tsrc>::SubtractImages(FIBITMAP* dst, FIBITMAP* src)
{
	if(dst == NULL || src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(CheckDimensions(dst, src) ==  FREEIMAGE_ALGORITHMS_ERROR)
		return  FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_DOUBLE)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double *dst_ptr = (double *) FreeImage_GetBits(dst);
	Tsrc *src_ptr = (Tsrc *) FreeImage_GetBits(src);

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	for(int i=0; i < number_of_pixels; i++)
		*dst_ptr++ = (double) *dst_ptr - *src_ptr++;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

GREYLEVEL_ARITHMATIC<unsigned char>		arithmaticUCharImage;
GREYLEVEL_ARITHMATIC<unsigned short>	arithmaticUShortImage;
GREYLEVEL_ARITHMATIC<short>				arithmaticShortImage;
GREYLEVEL_ARITHMATIC<unsigned long>		arithmaticULongImage;
GREYLEVEL_ARITHMATIC<long>				arithmaticLongImage;
GREYLEVEL_ARITHMATIC<float>				arithmaticFloatImage;
GREYLEVEL_ARITHMATIC<double>			arithmaticDoubleImage;


int DLL_CALLCONV 
FreeImageAlgorithms_MultiplyGreyLevelImages(FIBITMAP* dst, FIBITMAP* src)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	
			if(FreeImage_GetBPP(src) == 8)
				return  arithmaticUCharImage.MultiplyImages(dst, src);
		case FIT_UINT16:	
			return arithmaticUShortImage.MultiplyImages(dst, src);
		case FIT_INT16:		
			return arithmaticShortImage.MultiplyImages(dst, src);
		case FIT_UINT32:	
			return arithmaticULongImage.MultiplyImages(dst, src);
		case FIT_INT32:		
			return arithmaticLongImage.MultiplyImages(dst, src);
		case FIT_FLOAT:	
			return arithmaticFloatImage.MultiplyImages(dst, src);
		case FIT_DOUBLE:	
			return arithmaticDoubleImage.MultiplyImages(dst, src);
	}

	return FREEIMAGE_ALGORITHMS_ERROR; 
}


int DLL_CALLCONV 
FreeImageAlgorithms_DivideGreyLevelImages(FIBITMAP* dst, FIBITMAP* src)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	
			if(FreeImage_GetBPP(src) == 8)
				return  arithmaticUCharImage.DivideImages(dst, src);
		case FIT_UINT16:	
			return arithmaticUShortImage.DivideImages(dst, src);
		case FIT_INT16:		
			return arithmaticShortImage.DivideImages(dst, src);
		case FIT_UINT32:	
			return arithmaticULongImage.DivideImages(dst, src);
		case FIT_INT32:		
			return arithmaticLongImage.DivideImages(dst, src);
		case FIT_FLOAT:	
			return arithmaticFloatImage.DivideImages(dst, src);
		case FIT_DOUBLE:	
			return arithmaticDoubleImage.DivideImages(dst, src);
	}

	return FREEIMAGE_ALGORITHMS_ERROR; 
}

int DLL_CALLCONV 
FreeImageAlgorithms_AddGreyLevelImages(FIBITMAP* dst, FIBITMAP* src)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	
			if(FreeImage_GetBPP(src) == 8)
				return  arithmaticUCharImage.AddImages(dst, src);
		case FIT_UINT16:	
			return arithmaticUShortImage.AddImages(dst, src);
		case FIT_INT16:		
			return arithmaticShortImage.AddImages(dst, src);
		case FIT_UINT32:	
			return arithmaticULongImage.AddImages(dst, src);
		case FIT_INT32:		
			return arithmaticLongImage.AddImages(dst, src);
		case FIT_FLOAT:	
			return arithmaticFloatImage.AddImages(dst, src);
		case FIT_DOUBLE:	
			return arithmaticDoubleImage.AddImages(dst, src);
	}

	return FREEIMAGE_ALGORITHMS_ERROR; 
}


int DLL_CALLCONV 
FreeImageAlgorithms_SubtractGreyLevelImages(FIBITMAP* dst, FIBITMAP* src)
{
	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	
			if(FreeImage_GetBPP(src) == 8)
				return  arithmaticUCharImage.SubtractImages(dst, src);
		case FIT_UINT16:	
			return arithmaticUShortImage.SubtractImages(dst, src);
		case FIT_INT16:		
			return arithmaticShortImage.SubtractImages(dst, src);
		case FIT_UINT32:	
			return arithmaticULongImage.SubtractImages(dst, src);
		case FIT_INT32:		
			return arithmaticLongImage.SubtractImages(dst, src);
		case FIT_FLOAT:	
			return arithmaticFloatImage.SubtractImages(dst, src);
		case FIT_DOUBLE:	
			return arithmaticDoubleImage.SubtractImages(dst, src);
	}

	return FREEIMAGE_ALGORITHMS_ERROR; 
}


int DLL_CALLCONV 
FreeImageAlgorithms_MultiplyGreyLevelImageConstant(FIBITMAP* dst, double constant)
{
	if(dst == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_DOUBLE)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double *dst_ptr = (double *) FreeImage_GetBits(dst);

	int number_of_pixels = FreeImage_GetWidth(dst) * FreeImage_GetHeight(dst);

	for(int i=0; i < number_of_pixels; i++)
		*dst_ptr++ = (double) *dst_ptr * constant;

	return FREEIMAGE_ALGORITHMS_SUCCESS;	
}

int DLL_CALLCONV 
FreeImageAlgorithms_DivideGreyLevelImageConstant(FIBITMAP* dst, double constant)
{
	if(dst == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_DOUBLE)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double *dst_ptr = (double *) FreeImage_GetBits(dst);

	int number_of_pixels = FreeImage_GetWidth(dst) * FreeImage_GetHeight(dst);

	for(int i=0; i < number_of_pixels; i++)
		*dst_ptr++ = (double) *dst_ptr / constant;

	return FREEIMAGE_ALGORITHMS_SUCCESS;	
}

int DLL_CALLCONV 
FreeImageAlgorithms_AddGreyLevelImageConstant(FIBITMAP* dst, double constant)
{
	if(dst == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_DOUBLE)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double *dst_ptr = (double *) FreeImage_GetBits(dst);

	int number_of_pixels = FreeImage_GetWidth(dst) * FreeImage_GetHeight(dst);

	for(int i=0; i < number_of_pixels; i++)
		*dst_ptr++ = (double) *dst_ptr + constant;

	return FREEIMAGE_ALGORITHMS_SUCCESS;	
}


int DLL_CALLCONV 
FreeImageAlgorithms_SubtractGreyLevelImageConstant(FIBITMAP* dst, double constant)
{
	if(dst == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_DOUBLE)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double *dst_ptr = (double *) FreeImage_GetBits(dst);

	int number_of_pixels = FreeImage_GetWidth(dst) * FreeImage_GetHeight(dst);

	for(int i=0; i < number_of_pixels; i++)
		*dst_ptr++ = (double) *dst_ptr - constant;

	return FREEIMAGE_ALGORITHMS_SUCCESS;	
}


int DLL_CALLCONV 
FreeImageAlgorithms_ComplexConjugate(FIBITMAP* src)
{
	if(src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(FreeImage_GetImageType(src) != FIT_COMPLEX)
		return FREEIMAGE_ALGORITHMS_ERROR;

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	FICOMPLEX *src_ptr = (FICOMPLEX *) FreeImage_GetBits(src);

	for(int i=0; i < number_of_pixels; i++) {
		
		src_ptr->i = -src_ptr->i;
		src_ptr++;
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;	
}


// (a + ib) (c + id)
// = ac + ibc + ida + i^2bd
// = ac + ibc + ida - bd
// = ac - bd + i(bc + da)
int DLL_CALLCONV 
FreeImageAlgorithms_MultiplyComplexImages(FIBITMAP* dst, FIBITMAP* src)
{
	if(dst == NULL || src == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(CheckDimensions(dst, src) ==  FREEIMAGE_ALGORITHMS_ERROR)
		return  FREEIMAGE_ALGORITHMS_ERROR;

	// Make dst a double so it can hold all the results of
	// the arithmatic.
	if(FreeImage_GetImageType(dst) != FIT_COMPLEX)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(FreeImage_GetImageType(src) != FIT_COMPLEX)
		return FREEIMAGE_ALGORITHMS_ERROR;

	FICOMPLEX *dst_ptr = (FICOMPLEX *) FreeImage_GetBits(dst);
	FICOMPLEX *src_ptr = (FICOMPLEX *) FreeImage_GetBits(src);

	int number_of_pixels = FreeImage_GetWidth(src) * FreeImage_GetHeight(src);

	double tmp;

	for(int i=0; i < number_of_pixels; i++) {

		// real part = ac - bd
		tmp = (dst_ptr->r * src_ptr->r) - (dst_ptr->i * src_ptr->i);

		// imaginary part = bc + da
		dst_ptr->i = (dst_ptr->i * src_ptr->r) + (src_ptr->i * dst_ptr->r);
		dst_ptr->r = tmp;

		dst_ptr++;
		src_ptr++;
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}