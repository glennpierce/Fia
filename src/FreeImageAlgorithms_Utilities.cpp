#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>
#include <math.h>
#include <limits.h>
#include <float.h>

#include <xmmintrin.h>

/***
* int _os_support(int feature)
*   - Checks if OS Supports the capablity or not
*
* Entry:
*   feature: the feature we want to check if OS supports it.
*
* Exit:
*   Returns 1 if OS support exist and 0 when OS doesn't support it.
*
****************************************************************/

int DLL_CALLCONV
_os_support(int feature)
{
    __try {
        switch (feature) {
        case _CPU_FEATURE_SSE:
            __asm {
                xorps xmm0, xmm0        // executing SSE instruction
            }
            break;
        case _CPU_FEATURE_SSE2:
            __asm {
                xorpd xmm0, xmm0        // executing SSE2 instruction
            }
            break;
        case _CPU_FEATURE_3DNOW:
            __asm {
                pfrcp mm0, mm0          // executing 3DNow! instruction
                emms
            }
            break;
        case _CPU_FEATURE_MMX:
            __asm {
                pxor mm0, mm0           // executing MMX instruction
                emms
            }
            break;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION) {
            return 0;
        }
        return 0;
    }
    return 1;
}

void DLL_CALLCONV
FreeImageAlgorithms_SSEFindFloatMinMax(const float *data, long n, float *min, float *max)
{
	__m128 min128 = _mm_set_ps1(FLT_MAX);  // min128[0, 1, 2, 3] = FLT_MAX
    __m128 max128 = _mm_set_ps1(FLT_MIN);  // max128[0, 1, 2, 3] = FLT_MIN

    __m128* pSource = (__m128*) data;
   
	for ( int i = 0; i < n/4; i++ )
    {
        min128 =  _mm_min_ps(*pSource, min128);
        max128 =  _mm_max_ps(*pSource, max128);

        pSource++;
    }

	// extract minimum and maximum values from min128 and max128
    union u
    {
        __m128 m;
        float f[4];
    } x;

	x.m = min128;
    *min = min(x.f[0], min(x.f[1], min(x.f[2], x.f[3])));

    x.m = max128;
    *max = max(x.f[0], max(x.f[1], max(x.f[2], x.f[3])));
}


void DLL_CALLCONV
FreeImageAlgorithms_FindCharMinMax(const char *data, long n, char *min, char *max)
{
	MAXMIN(data, n, *max, *min);
}


void DLL_CALLCONV
FreeImageAlgorithms_FindIntMinMax(const int *data, long n, int *min, int *max)
{
	MAXMIN(data, n, *max, *min);
}

void DLL_CALLCONV
FreeImageAlgorithms_FindShortMinMax(const short *data, long n, short *min, short *max)
{
	MAXMIN(data, n, *max, *min);
}

void DLL_CALLCONV
FreeImageAlgorithms_FindUShortMinMax(const unsigned short *data, long n, unsigned short *min, unsigned short *max)
{
	MAXMIN(data, n, *max, *min);
}


void DLL_CALLCONV
FreeImageAlgorithms_FindLongMinMax(const long *data, long n, long *min, long *max)
{
	MAXMIN(data, n, *max, *min);
}

void DLL_CALLCONV
FreeImageAlgorithms_FindULongMinMax(const unsigned long *data, long n, unsigned long *min, unsigned long *max)
{
	MAXMIN(data, n, *max, *min);
}


void DLL_CALLCONV
FreeImageAlgorithms_FindFloatMinMax(const float *data, long n, float *min, float *max)
{
	MAXMIN(data, n, *max, *min);
}

void DLL_CALLCONV
FreeImageAlgorithms_FindDoubleMinMax(const double *data, long n, double *min, double *max)
{
	MAXMIN(data, n, *max, *min);
}


char DLL_CALLCONV
FreeImageAlgorithms_FindCharMax(const char *data, long n, char *max)
{
	return (char) FINDMAX(data, n, *max);
}

unsigned char DLL_CALLCONV
FreeImageAlgorithms_FindUCharMax(const unsigned char *data, long n, unsigned char *max)
{
	return (unsigned char) FINDMAX(data, n, *max);
}

int DLL_CALLCONV
FreeImageAlgorithms_FindIntMax(const int *data, long n, int *max)
{
	return (int) FINDMAX(data, n, *max);
}

short DLL_CALLCONV
FreeImageAlgorithms_FindShortMax(const short *data, long n, short *max)
{
	return (short) FINDMAX(data, n, *max);
}

unsigned short DLL_CALLCONV
FreeImageAlgorithms_FindUShortMax(const unsigned short *data, long n, unsigned short *max)
{
	return (unsigned short) FINDMAX(data, n, *max);
}

float DLL_CALLCONV
FreeImageAlgorithms_FindFloatMax(const float *data, long n, float *max)
{
	return (float) FINDMAX(data, n, *max);
}

double DLL_CALLCONV
FreeImageAlgorithms_FindDoubleMax(const double *data, long n, double *max)
{
	return (double) FINDMAX(data, n, *max);
}


template<class Tsrc>
class FIND_MINMAX_FOR_DIB
{
public:
	void find(FIBITMAP *src, double *min, double *max);
};

template<class Tsrc> void 
FIND_MINMAX_FOR_DIB<Tsrc>::find(FIBITMAP *src, double *min, double *max)
{
	if(!src)
		return;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	double temp_min, temp_max;
	Tsrc l_min, l_max;

	// Get the first two pixel values for initialisation
	Tsrc *bits = reinterpret_cast<Tsrc*>(FreeImage_GetScanLine(src, 0));
	temp_min = temp_max = bits[0];

	for(int y = 0; y < height; y++) {

		bits = reinterpret_cast<Tsrc*>(FreeImage_GetScanLine(src, y));

		MAXMIN(bits, width, l_max, l_min);

		if(l_max > temp_max) temp_max = l_max;
		if(l_min < temp_min) temp_min = l_min;
	}

	*min = temp_min;
	*max = temp_max;
}


FIND_MINMAX_FOR_DIB<unsigned char>		minmaxUCharImage;
FIND_MINMAX_FOR_DIB<unsigned short>		minmaxUShortImage;
FIND_MINMAX_FOR_DIB<short>				minmaxShortImage;
FIND_MINMAX_FOR_DIB<unsigned long>		minmaxULongImage;
FIND_MINMAX_FOR_DIB<long>				minmaxLongImage;
FIND_MINMAX_FOR_DIB<float>				minmaxFloatImage;
FIND_MINMAX_FOR_DIB<double>				minmaxDoubleImage;

void DLL_CALLCONV
FreeImageAlgorithms_FindMinMax(FIBITMAP *src, double *min, double *max)
{
	if(!src)
		return;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
			if(FreeImage_GetBPP(src) == 8)
				minmaxUCharImage.find(src, min, max);
			break;
		case FIT_UINT16:	
			minmaxUShortImage.find(src, min, max);
			break;
		case FIT_INT16:		
			minmaxShortImage.find(src, min, max);
			break;
		case FIT_UINT32:	
			minmaxULongImage.find(src, min, max);
			break;
		case FIT_INT32:		
			minmaxLongImage.find(src, min, max);
			break;
		case FIT_FLOAT:		
			minmaxFloatImage.find(src, min, max);
			break;	
		case FIT_DOUBLE:		
			minmaxDoubleImage.find(src, min, max);
			break;	
	}
}


static int FindMaxChannelValue(unsigned int pixel_value)
{
	unsigned char red_value = pixel_value >> FI_RGBA_RED_SHIFT;
	unsigned char green_value = pixel_value >> FI_RGBA_GREEN_SHIFT;
	unsigned char blue_value = pixel_value >> FI_RGBA_BLUE_SHIFT;

	return MAX(MAX(red_value, green_value), blue_value);
}


void DLL_CALLCONV
FreeImageAlgorithms_FindMinMaxForColourImage(FIBITMAP *src, double *min, double *max)
{
	if( !src || FreeImage_GetImageType(src) != FIT_BITMAP)
		return;

	if( FreeImage_GetBPP(src) != 24)
		return;

	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	unsigned int tmp_value;
	
	unsigned int *bits = (unsigned int *) FreeImage_GetBits(src); 
	
	double temp_min = FindMaxChannelValue(bits[0]);
	double temp_max = temp_min;

	for(int y = 0; y < height; y++) {

		bits = reinterpret_cast<unsigned int *>(FreeImage_GetScanLine(src, y));

		for(int x = 0; x < width; x++) {

			tmp_value = FindMaxChannelValue(bits[x]);

			if(tmp_value < temp_min)
				temp_min = tmp_value;

			if(tmp_value > temp_max)
				temp_max = tmp_value;
		}
	}

	*min = static_cast<double>(temp_min);
	*max = static_cast<double>(temp_max);
}

int DLL_CALLCONV
FreeImageAlgorithms_CharArrayReverse(char *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_UCharArrayReverse(unsigned char *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_ShortArrayReverse(short *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_UShortArrayReverse(unsigned short *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_IntArrayReverse(int *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_UIntArrayReverse(unsigned int *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_LongArrayReverse(long *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_ULongArrayReverse(unsigned long *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_FloatArrayReverse(float *array, long size)
{
	return ArrayReverse(array, size);
}

int DLL_CALLCONV
FreeImageAlgorithms_DoubleArrayReverse(double *array, long size)
{
	return ArrayReverse(array, size);
}


int DLL_CALLCONV
FreeImageAlgorithms_IsGreyScale(FIBITMAP *src)
{
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(src);

	switch(image_type)
	{
		case FIT_RGB16:
		case FIT_RGBA16:
		case FIT_RGBF:
		case FIT_RGBAF:
		{
			return 0;
		}

		case FIT_BITMAP:
		{
			int bpp = FreeImage_GetBPP(src);

			if(bpp >= 16)
				return 0;
		}
	}

	return 1;
}

void DLL_CALLCONV
FreeImageAlgorithms_GetMaxPosibleValueForGreyScaleType(FREE_IMAGE_TYPE type, double *max)
{
	switch(type) {
		case FIT_BITMAP:
			*max = UCHAR_MAX;
			break;

		case FIT_UINT16:	
			*max = USHRT_MAX;
			break;

		case FIT_INT16:		
			*max = SHRT_MAX;
			break;

		case FIT_UINT32:	
			*max = UINT_MAX;
			break;

		case FIT_INT32:		
			*max = INT_MAX;
			break;

		case FIT_FLOAT:		
			*max = FLT_MAX;
			break;	
	}
}

void DLL_CALLCONV
FreeImageAlgorithms_GetMinPosibleValueForGreyScaleType(FREE_IMAGE_TYPE type, double *min)
{
	switch(type) {
		case FIT_BITMAP:
			*min = 0;
			break;

		case FIT_UINT16:	
			*min = 0;
			break;

		case FIT_INT16:		
			*min = SHRT_MIN;
			break;

		case FIT_UINT32:	
			*min = 0;
			break;

		case FIT_INT32:		
			*min = INT_MIN;
			break;

		case FIT_FLOAT:		
			*min = FLT_MIN;
			break;	
	}
}

void DLL_CALLCONV
FreeImageAlgorithms_GetMaxPosibleValueForFib(FIBITMAP *src, double *max)
{
	if(!src)
		return;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit		
			*max = UCHAR_MAX;
			break;

		case FIT_UINT16:	
			*max = USHRT_MAX;
			break;

		case FIT_INT16:		
			*max = SHRT_MAX;
			break;

		case FIT_UINT32:	
			*max = UINT_MAX;
			break;

		case FIT_INT32:		
			*max = INT_MAX;
			break;

		case FIT_FLOAT:		
			*max = FLT_MAX;
			break;	
	}
}


void DLL_CALLCONV
FreeImageAlgorithms_GetMinPosibleValueForFib(FIBITMAP *src, double *min)
{
	if(!src)
		return;

	FREE_IMAGE_TYPE src_type = FreeImage_GetImageType(src);

	switch(src_type) {
		case FIT_BITMAP:	// standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
				*min = 0;
			break;

		case FIT_UINT16:	
			*min = 0;
			break;

		case FIT_INT16:		
			*min = SHRT_MIN;
			break;

		case FIT_UINT32:	
			*min = 0;
			break;

		case FIT_INT32:		
			*min = INT_MIN;
			break;

		case FIT_FLOAT:		
			*min = FLT_MIN;
			break;	
	}
}

int DLL_CALLCONV
FreeImageAlgorithms_Is16BitReally12BitImage(FIBITMAP *src)
{
	unsigned int x, width, height, *bptr;

	// Future work! Can proceed 8 pixels at once using xmm registers (128 bit registers).
	// Is Assembly Code possible in labwindows ? 

	// We want to know if there are some bits set at 0xF000 ie 1111 0000 0000 0000
	// and we will use the advantage of 32bit CPU: 
	#define MASK_12BIT 0xF000F000	  // 1111 0000 0000 0000   1111 0000 0000 0000

	if( !src || FreeImage_GetImageType(src) == FIT_BITMAP)
		return 0;

	if( FreeImage_GetBPP(src) != 16)
		return 0;

	width  = FreeImage_GetWidth(src);
	height = FreeImage_GetHeight(src);
	
	// we need to change ptr size to 32bit 
	// bits should be aligned to 4 Bytes 
	bptr = (unsigned int *) FreeImage_GetBits(src); 
	
	x = (width * height) / 2; // 2 pixels at once 
		
	do { // this loop will skip one conditional jump 

		// let's avoid [] operator 
		if(*bptr++ & MASK_12BIT)
			return 0;
						
	} while(--x);
		
	// There can be one leftover
	if(width & 1) { 
			
		if(*(short*)bptr & (short) MASK_12BIT)
			return 0;
	} 

	return 1;
}


/* Gets the values of the pixels along a line calculated using the Midpoint Line algorithm */
template <class T> int
GetPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, T *values) 
{  
	T value;
   	int swapped = 0, dx, dy, incrN, incrE, incrNE, d, x, y, slope, tmp_y, len = 0;
  
	// If necessary, switch the points so we're 
	// always drawing from left to right. 
	if (p2.x < p1.x) { 
		swapped = 1;
    	SWAP(p1, p2);
	}
   
   	dx = p2.x - p1.x; 				   
   	dy = p2.y - p1.y; 
   
   	if(dy < 0) {
   		slope = -1;
   		dy = -dy;
   	}
   	else {
   		slope = 1;
   	}
   
   	x = p1.x; 
    y = p1.y;
   
   	if (abs(dy) <= dx) { 
 
   		d = 2 * dy - dx; 
   		incrE = 2 * dy; 		// E
   		incrNE = 2 * (dy - dx); // NE
  	   	
		T *bits = (T *)FreeImage_GetScanLine(src, y);
		value = bits[x];

    	*values++ = value;
   
       	while (x < p2.x) { 
    
           if (d <= 0) { 
               d += incrE; 		// Going to E
               x++; 
           } 
           else { 
           
               d += incrNE; 	// Going to NE
               x++; 
               y += slope; 
           } 
           
           T *bits = (T *)FreeImage_GetScanLine(src, y);
		   value = bits[x];

    	   *values++ = value;
    	   len++;
       	} 
   	}
   	else {
   
   		d = dy - 2 * dx; 
   		incrN = 2 * -dx; 		// N
   		incrNE = 2 * (dy - dx); // NE
  
   		tmp_y = 0;
   
  	   	T *bits = (T *)FreeImage_GetScanLine(src, y);
		value = bits[x];

    	*values++ = value;
   
       	for ( tmp_y = 0; tmp_y < abs(p2.y - p1.y); tmp_y++ ) { 
    
       		if (d > 0) {
           
            	d += incrN; 		// Going to N
            	y += slope;
           	} 
           	else { 
           
           		d += incrNE; 		// Going to NE
            	y += slope; 
            	x++;
          	} 
           
           	T *bits = (T *)FreeImage_GetScanLine(src, y);
			value = bits[x];

    		*values++ = value;
    		len++;
       } 
   }
   
	// Reverse if the line draws from right to left.
	//if(swapped > 0) 
	//	FreeImageAlgorithms_ArrayReverse(values, len);

   return len;
} 


int DLL_CALLCONV
FreeImageAlgorithms_GetCharPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, char *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FreeImageAlgorithms_GetUCharPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, unsigned char *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FreeImageAlgorithms_GetShortPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, short *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FreeImageAlgorithms_GetUShortPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, unsigned short *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FreeImageAlgorithms_GetFloatPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, float *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

int DLL_CALLCONV
FreeImageAlgorithms_GetDoublePixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, double *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}


/* Midpoint Line algorithm */
int DLL_CALLCONV
FreeImageAlgorithms_GetRGBPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, char *red_values, char *green_values, char *blue_values) 
{  
	RGBQUAD value;
   	int dx, dy, incrN, incrE, incrNE, d, x, y, slope, tmp_y, len = 0; 
   
	// If necessary, switch the points so we're 
	// always drawing from left to right. 
	if (p2.x < p1.x) { 
    	int t; 
       
    	t = p2.x; 
    	p2.x = p1.x; 
    	p1.x = t;
       
    	t = p2.y; 
    	p2.y = p1.y; 
    	p1.y = t; 
   	} 
   
   	dx = p2.x - p1.x; 				   
   	dy = p2.y - p1.y; 
   
   	if(dy < 0) {
   		slope = -1;
   		dy = -dy;
   	}
   	else {
   		slope = 1;
   	}
   
   	x = p1.x; 
    y = p1.y;
   
   	if (abs(dy) <= dx) { 
 
   		d = 2 * dy - dx; 
   		incrE = 2 * dy; 		// E
   		incrNE = 2 * (dy - dx); // NE
  
  	   	FreeImage_GetPixelColor(src, x, y, &value);
  		*red_values++ = value.rgbRed;
  		*green_values++ = value.rgbGreen;
  		*blue_values++ = value.rgbBlue;
   
       	while (x < p2.x) { 
    
           if (d <= 0) { 
               d += incrE; 		// Going to E
               x++; 
           } 
           else { 
           
               d += incrNE; 	// Going to NE
               x++; 
               y += slope; 
           } 
           
           FreeImage_GetPixelColor(src, x, y, &value);
  		   *red_values++ = value.rgbRed;
  		   *green_values++ = value.rgbGreen;
  		   *blue_values++ = value.rgbBlue; 
  			
    	   len++;
       	} 
   	}
   	else {
   
   		d = dy - 2 * dx; 
   		incrN = 2 * -dx; 		// N
   		incrNE = 2 * (dy - dx); // NE
  
   		tmp_y = 0;
   
  	   	FreeImage_GetPixelColor(src, x, y, &value);
  		*red_values++ = value.rgbRed;
  		*green_values++ = value.rgbGreen;
  		*blue_values++ = value.rgbBlue; 
   
       	for ( tmp_y = 0; tmp_y < abs(p2.y - p1.y); tmp_y++ ) { 
    
       		if (d > 0) {
           
            	d += incrN; 		// Going to N
            	y += slope;
           	} 
           	else { 
           
           		d += incrNE; 		// Going to NE
            	y += slope; 
            	x++;
          	} 
           
           	FreeImage_GetPixelColor(src, x, y, &value);
  			*red_values++ = value.rgbRed;
  			*green_values++ = value.rgbGreen;
  			*blue_values++ = value.rgbBlue;
  		
    		len++;
       } 
   }
   
   return len;
} 


void DLL_CALLCONV
FreeImageAlgorithms_GetDistanceMap (int width, int height, int *distance_map) 
{  
	int xcentre = (int) (width / 2 + 0.5);
	int ycentre = (int) (height / 2 + 0.5);
		
	int tmp_min;

	distance_map = (int *) malloc(width * height * sizeof(float));

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (x <= xcentre)
				tmp_min = x;
			else
				tmp_min = width - x;

			if (y <= ycentre)
				distance_map[y * width + x] = MIN(tmp_min, y);
			else
				distance_map[y * width + x] = MIN(tmp_min, height - y);
		}
	}
}


int DLL_CALLCONV
FreeImageAlgorithms_SimplePaste(FIBITMAP *dst, FIBITMAP *src, int left, int top)
{
	int src_height = FreeImage_GetHeight(src);
	int dst_height = FreeImage_GetHeight(dst);
	int dst_pitch =  FreeImage_GetPitch(dst);

	int src_line_bytes = FreeImage_GetLine(src);

	// calculate the number of bytes per pixel
	int bytespp = FreeImage_GetLine(dst) / FreeImage_GetWidth(dst);

	int dst_scan_line = dst_height - src_height - top;
	BYTE *dst_bits = FreeImage_GetScanLine(dst, dst_scan_line);
	dst_bits += (left * bytespp);

	BYTE *src_bits;

	for(int i=0; i < src_height; i++)
	{
		src_bits = FreeImage_GetScanLine(src, i);
		memcpy(dst_bits, src_bits, src_line_bytes);
		dst_bits += dst_pitch;
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageAlgorithms_BitwiseCompare(FIBITMAP *dib1, FIBITMAP *dib2)
{
	if(!dib1 || !dib2)
		return 0;

	if(FreeImage_GetBPP(dib1) != FreeImage_GetBPP(dib2))
		return 0;

	if(FreeImage_GetImageType(dib1) != FreeImage_GetImageType(dib2))
		return 0;

	if(FreeImage_GetWidth(dib1) != FreeImage_GetWidth(dib2))
		return 0;

	if(FreeImage_GetHeight(dib1) != FreeImage_GetHeight(dib2))
		return 0;

	if(FreeImage_GetPitch(dib1) != FreeImage_GetPitch(dib2))
		return 0;

	int size = (FreeImage_GetPitch(dib1) * FreeImage_GetHeight(dib1)) / 4;

	int *ptr1 = (int *) FreeImage_GetBits(dib1);
	int *ptr2 = (int *) FreeImage_GetBits(dib2);

	for(int i=0; i < size; i++)
	{
		if(*ptr1++ != *ptr2++)
			return 0;
	}

	return 1;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_CloneImageType(FIBITMAP *src, int width, int height)
{
	FREE_IMAGE_TYPE type = FreeImage_GetImageType(src);
	int bpp = FreeImage_GetBPP(src);
	unsigned red_mask = FreeImage_GetRedMask(src);
	unsigned green_mask = FreeImage_GetGreenMask(src);
	unsigned blue_mask = FreeImage_GetBlueMask(src);

	FIBITMAP *dst = FreeImage_AllocateT(type, width, height, 
		bpp, red_mask, green_mask, blue_mask);

	// If 8bit image we must clone the palette
	if(bpp <= 8)
		FreeImageAlgorithms_CopyPalette(src, dst);

	return dst;
}

FIABITMAP DLL_CALLCONV
FreeImageAlgorithms_SetBorder(FIBITMAP *src, int xborder, int yborder)
{
	int width = FreeImage_GetWidth(src);
	int height = FreeImage_GetHeight(src);

	FIABITMAP dst;
	
	dst.fib = FreeImageAlgorithms_CloneImageType(src, width + (2 *xborder), height + (2 * yborder));

	dst.xborder = xborder;
	dst.yborder = yborder;

	FreeImageAlgorithms_SimplePaste(dst.fib, src, xborder, yborder);

	return dst;
}


FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_FloatThreshold(FIBITMAP *src, float t, BYTE min, BYTE max)
{
	FIBITMAP *dst;
	float *src_bits;
	BYTE *dst_bits;
	int height,width;
	
	if (!src)
		return NULL;

	if(FreeImage_GetImageType(src) != FIT_FLOAT &&
		FreeImage_GetImageType(src) != FIT_DOUBLE)
		return NULL;

	width = FreeImage_GetWidth(src);
	height = FreeImage_GetHeight(src);

	dst = FreeImage_Allocate(width, height, 8, 0, 0, 0);
	FreeImageAlgorithms_SetGreyLevelPalette(dst);

	for (int y=0; y < height; y++)
	{
		src_bits = (float*) FreeImage_GetScanLine(src, y);
		dst_bits = (BYTE*) FreeImage_GetScanLine(dst, y);
			
		for (int x=0; x < width; x++)
		{
			if(src_bits[x] < t)
				dst_bits[x] = min;
			else
				dst_bits[x] = max;
		}
	}

	return dst;	
}

FIBITMAP* DLL_CALLCONV
FreeImageAlgorithms_ConvertToGreyscaleFloatType(FIBITMAP *src, FREE_IMAGE_TYPE type)
{
	if(src == NULL || (type != FIT_FLOAT && type != FIT_DOUBLE))
		return NULL;

	FIBITMAP *gs_dib = NULL, *dst = NULL;

	if(FreeImage_GetBPP(src) >= 24 && FreeImage_GetImageType(src) == FIT_BITMAP) // Colour
		gs_dib = FreeImage_ConvertToGreyscale(src);

	if(gs_dib != NULL) {
		dst = FreeImage_ConvertToType(gs_dib, type, 1); 
		FreeImage_Unload(gs_dib);
	}
	else
		dst = FreeImage_ConvertToType(src, type, 1); 

	return dst;
}