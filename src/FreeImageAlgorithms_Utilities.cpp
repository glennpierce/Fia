#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>
#include <math.h>

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
FreeImageAlgorithms_FindFloatMinMax(const float *data, long n, float *min, float *max)
{
	MAXMIN(data, n, *max, *min);
}

void DLL_CALLCONV
FreeImageAlgorithms_FindDoubleMinMax(const double *data, long n, double *min, double *max)
{
	MAXMIN(data, n, *max, *min);
}


long DLL_CALLCONV
FreeImageAlgorithms_FindCharMax(const char *data, long n, char *max)
{
	return FINDMAX(data, n, *max);
}

long DLL_CALLCONV
FreeImageAlgorithms_FindUCharMax(const unsigned char *data, long n, unsigned char *max)
{
	return FINDMAX(data, n, *max);
}

long DLL_CALLCONV
FreeImageAlgorithms_FindIntMax(const int *data, long n, int *max)
{
	return FINDMAX(data, n, *max);
}

long DLL_CALLCONV
FreeImageAlgorithms_FindShortMax(const short *data, long n, short *max)
{
	return FINDMAX(data, n, *max);
}

long DLL_CALLCONV
FreeImageAlgorithms_FindUShortMax(const unsigned short *data, long n, unsigned short *max)
{
	return FINDMAX(data, n, *max);
}

long DLL_CALLCONV
FreeImageAlgorithms_FindFloatMax(const float *data, long n, float *max)
{
	return FINDMAX(data, n, *max);
}

long DLL_CALLCONV
FreeImageAlgorithms_FindDoubleMax(const double *data, long n, double *max)
{
	return FINDMAX(data, n, *max);
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
	}
}


int DLL_CALLCONV
FreeImageAlgorithms_IsGreyScale(FIBITMAP *src)
{
	if(FreeImage_GetImageType(src) == FIT_BITMAP && FreeImage_GetBPP(src) >= 24) {
	
		return 0;
	}
	else {
	
		return 1; 
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
   
   return len;
} 


int DLL_CALLCONV
FreeImageAlgorithms_GetCharPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, char *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetShortPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, short *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetUShortPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, unsigned short *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetFloatPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, float *values)
{
	return GetPixelValuesForLine (src, p1, p2, values);
}

/* Midpoint Line algorithm */
DLL_API int DLL_CALLCONV
FreeImageAlgorithms_GetRGBPixelValuesForLine (FIBITMAP *src, POINT p1, POINT p2, char *red_hist, char *green_hist, char *blue_hist) 
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
  		*red_hist++ = value.rgbRed;
  		*green_hist++ = value.rgbGreen;
  		*blue_hist++ = value.rgbBlue;
   
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
  		   *red_hist++ = value.rgbRed;
  		   *green_hist++ = value.rgbGreen;
  		   *blue_hist++ = value.rgbBlue; 
  			
    	   len++;
       	} 
   	}
   	else {
   
   		d = dy - 2 * dx; 
   		incrN = 2 * -dx; 		// N
   		incrNE = 2 * (dy - dx); // NE
  
   		tmp_y = 0;
   
  	   	FreeImage_GetPixelColor(src, x, y, &value);
  		*red_hist++ = value.rgbRed;
  		*green_hist++ = value.rgbGreen;
  		*blue_hist++ = value.rgbBlue; 
   
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
  			*red_hist++ = value.rgbRed;
  			*green_hist++ = value.rgbGreen;
  			*blue_hist++ = value.rgbBlue;
  		
    		len++;
       } 
   }
   
   return len;
} 


// Red, Green and Blue are between 0 and 255
// Hue varies between 0 and 360
// Satuation between 0 and 1
// Value between 0 and 1
int DLL_CALLCONV
FreeImageAlgorithms_RGBToHSV( unsigned char red, unsigned char green, unsigned char blue,
							double *hue, double *satuation, double *value)
{
	unsigned char max, min, rgb[3];

	*hue = 0;
	*satuation = 0;
	*value = 0;

	rgb[0] = red, rgb[1] = green, rgb[2] = blue;
	MAXMIN( rgb , 3, max, min);

	if(max != min) {
	
		if(red == max)
			*hue = ((double) (green - blue) / (max - min)) * 60.0;
		
		if(green == max)
			*hue = (2 + (double) (blue - red) / (max - min)) * 60.0;
		
		if(blue == max)
			*hue = (4 + (double) (red - green) / (max - min)) * 60.0;
	}
		
	*value = (double) max / 255.0;
	
	if(max != 0)
		*satuation = ((double) (max - min) / max);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


// Red, Green and Blue are between 0 and 255
// Hue varies betww=een 0 and 360
// Satuation between 0 and 1
// Value between 0 and 1
int DLL_CALLCONV
FreeImageAlgorithms_HSVToRGB( double hue, double satuation, double value,
							unsigned char *red, unsigned char *green, unsigned char *blue)
{
	unsigned int h,p,q,t,v;
	double temp_hue, f;
	
	if(satuation == 0.0)
		return FREEIMAGE_ALGORITHMS_SUCCESS;	
	
	// The if statement is here as the % operator requires integers
	// and thus looses some accuracy.
	if(hue < 0 || hue > 360) {
	
		temp_hue = floor(hue + 0.5);
		temp_hue = (int) temp_hue % 360;
	}
	else
		temp_hue = hue;
	
	*red = (unsigned char) floor(value + 0.5);
	*green = *red;
	*blue = *red;
	
	h = (unsigned int) (temp_hue / 60);  
	f = (double) (temp_hue / 60) - h;
	p = (unsigned int) floor( (value * (1 - satuation) * 255) + 0.5);
	q = (unsigned int) floor( (value * (1 - f * satuation) * 255) + 0.5);
	t = (unsigned int) floor( (value * (1 - (1 - f) * satuation) * 255) + 0.5);
	
	v = (unsigned int) (value * 255.0);	
	
	switch (h) {

		case 6:
		case 0:
		
			*red = v;
			*green = t;
			*blue = p;
			
			break;
		
		case 1:
		
			*red = q;
			*green = v;
			*blue = p;
			
			break;
		
		case 2:
		
			*red = p;
			*green = v;
			*blue = t;
			
			break;
		
		case 3: 
		
			*red = p;
			*green = q;
			*blue = v;
			
			break;
		
		case 4:
		
			*red = t;
			*green = p;
			*blue = v;
			
			break;
			
		case 5:
		
			*red = v;
			*green = p;
			*blue = q;
	}
	
	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 


// Red, Green and Blue are between 0 and 255
// Hue varies between 0 and 360
// Satuation between 0 and 1
// Value between 0 and 1
int DLL_CALLCONV
FreeImageAlgorithms_RGBToHSL( unsigned char red, unsigned char green, unsigned char blue,
							double *hue, double *satuation, double *luminosity)
{
	unsigned char max, min, rgb[3];

	*hue = 0;
	*satuation = 0;
	*luminosity = 0;

	rgb[0] = red, rgb[1] = green, rgb[2] = blue;
	MAXMIN( rgb , 3, max, min);

	if(max != min) {
	
		if(red == max)
			*hue = ((double) (green - blue) / (max - min)) * 60.0;
		
		if(green == max)
			*hue = (2 + (double) (blue - red) / (max - min)) * 60.0;
		
		if(blue == max)
			*hue = (4 + (double) (red - green) / (max - min)) * 60.0;
	}
		
	*luminosity = (double) (max + min) / ( 510.0);
	
	if(*luminosity < 0.5)
		*satuation = ((double) (max - min) / (510.0 * *luminosity));
	else
		*satuation = ((double) (max - min) / (510.0 * (2 - (2 * *luminosity))));

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}





static double HSLToRGBColour (double q1, double q2, double hue)
{
	if (hue > 360.0)
		hue -= 360.0;
	else if (hue < 0.0)
		hue += 360.0;
	
	if (hue < 60.0)
		return (q1 + (q2-q1) *hue /60.0);
	else if (hue < 180.0) 
		return q2;
	else if (hue < 240.0) 
		return (q1 + (q2-q1) * (240.0 - hue) /60.0);
	else
		return q1;
}


int DLL_CALLCONV
FreeImageAlgorithms_HSLToRGB( double hue, double satuation, double luminosity,
							unsigned char *red, unsigned char *green, unsigned char *blue)
{
	double p1, p2;

	if (luminosity <= 0.5) 
		p2 = luminosity * (1 + satuation);
	else
		p2 = luminosity + satuation - (luminosity * satuation);

	p1 = 2.0 * luminosity - p2;

	if (satuation == 0.0)
	{
		*red = (unsigned int) ((luminosity * 255.0) + 0.5);
		*green = (unsigned int) ((luminosity * 255.0) + 0.5);
		*blue = (unsigned int) ((luminosity * 255.0) + 0.5);
	}
	else
	{
		*red = (unsigned int) ((HSLToRGBColour(p1, p2, hue+120.0) * 255.0) + 0.5);
		*green = (unsigned int) ((HSLToRGBColour(p1, p2, hue) * 255.0) + 0.5);
		*blue = (unsigned int) ((HSLToRGBColour(p1, p2, hue-120.0) * 255.0) + 0.5);
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}