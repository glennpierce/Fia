#include "FreeImageAlgorithms_Colour.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <math.h>

RGBQUAD DLL_CALLCONV FIA_RGBQUAD(unsigned char red, unsigned char green,
								 unsigned char blue)
{
	RGBQUAD quad;

	quad.rgbRed = red;
	quad.rgbGreen = green;
	quad.rgbBlue = blue;
    quad.rgbReserved = 0;

	return quad;
}

int DLL_CALLCONV
FreeImageAlgorithms_CopyPaletteToRGBQUAD(FIBITMAP *src, RGBQUAD *palette)
{
	if( src == NULL || palette == NULL)
 		return FREEIMAGE_ALGORITHMS_ERROR;	
	
	// copy the palette
	memcpy(palette, FreeImage_GetPalette(src), FreeImage_GetColorsUsed(src) * sizeof(RGBQUAD));

  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV		   
FreeImageAlgorithms_CopyPaletteFromRGBQUAD(FIBITMAP *dst, RGBQUAD *palette)
{
	RGBQUAD *dst_palette; 

	if( dst == NULL || palette == NULL || FreeImage_GetBPP(dst) > 8)
 		return FREEIMAGE_ALGORITHMS_ERROR;

	if((dst_palette = FreeImage_GetPalette(dst)) == NULL) 
		return FREEIMAGE_ALGORITHMS_ERROR;

  	for (int i = 0; i < 256; i++) { 
  		dst_palette[i].rgbRed   = palette[i].rgbRed; 
  		dst_palette[i].rgbGreen = palette[i].rgbGreen; 
  		dst_palette[i].rgbBlue  = palette[i].rgbBlue;
  	}
  	
  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}
		   

int DLL_CALLCONV		   
FreeImageAlgorithms_CopyPalette(FIBITMAP *src, FIBITMAP *dst)
{
	RGBQUAD *src_palette, *dst_palette; 

	if( dst == NULL || src == NULL) {
 		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	if((src_palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;	

	if((dst_palette = FreeImage_GetPalette(dst)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;	 

  	for (int i = 0; i < 256; i++) { 
  		dst_palette[i].rgbRed   = src_palette[i].rgbRed; 
  		dst_palette[i].rgbGreen = src_palette[i].rgbGreen; 
  		dst_palette[i].rgbBlue  = src_palette[i].rgbBlue;
  	}
  	
  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageAlgorithms_ReversePaletteEntries(RGBQUAD *palette)
{
	int i;
    RGBQUAD copy_palette[256];

    if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

    memcpy(copy_palette, palette, sizeof(RGBQUAD) * 256);

	for (i=0; i < 256 ; i++)
		palette[i] = copy_palette[255 - i];
	
	return 1;
}

int DLL_CALLCONV
FreeImageAlgorithms_SetGreyLevelPalette(FIBITMAP *src)
{
	RGBQUAD *palette;
	
	if((palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	FreeImageAlgorithms_GetGreyLevelPalette(palette);

  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageAlgorithms_SetGreyLevelOverLoadPalette(FIBITMAP *src)
{
	RGBQUAD *palette;

	if((palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	FreeImageAlgorithms_GetGreyLevelOverLoadPalette(palette);

  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_SetTernaryPalettePalette(FIBITMAP *src, RGBQUAD background_colour,
									  int pos1, RGBQUAD colour1, int pos2, RGBQUAD colour2)
{
	RGBQUAD *palette;

	if((palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	FreeImageAlgorithms_GetTernaryPalette(palette, background_colour, pos1, colour1,
										  pos2, colour2);

  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageAlgorithms_SetLogColourPalette(FIBITMAP *src)
{	
	RGBQUAD *palette;

	if((palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

  	FreeImageAlgorithms_GetLogColourPalette(palette);

  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_SetFalseColourPalette(FIBITMAP *src, double wavelength)
{
	RGBQUAD *palette;

	if((palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	FreeImageAlgorithms_GetFalseColourPalette(palette, wavelength);

  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_SetRainBowPalette(FIBITMAP *src)
{
	RGBQUAD *palette;

	if((palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	FreeImageAlgorithms_GetRainBowPalette(palette);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_SetReverseRainBowPalette(FIBITMAP *src)
{
	RGBQUAD *palette;

	if((palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	FreeImageAlgorithms_GetReverseRainBowPalette(palette);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageAlgorithms_SetTemperaturePalette(FIBITMAP *src)
{
	RGBQUAD *palette;

	if((palette = FreeImage_GetPalette(src)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	FreeImageAlgorithms_GetTemperaturePalette(palette);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_SetSeismicColourPalette(FIBITMAP *src)
{
	RGBQUAD *palette;

	if((palette = FreeImage_GetPalette(src)) == NULL)
		return -1;

	FreeImageAlgorithms_GetSeismicColourPalette(palette);
 
  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelPalette(RGBQUAD *palette)
{
	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

  	for (int i = 0; i < 256; i++) { 
  		palette[i].rgbRed = i; 
  		palette[i].rgbGreen = i; 
  		palette[i].rgbBlue = i; 
  	}
  	
  	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 	


int DLL_CALLCONV
FreeImageAlgorithms_GetGreyLevelOverLoadPalette(RGBQUAD *palette)
{
	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	int i;

  	for (i = 0; i < 245; i++) { 
  		palette[i].rgbRed = i; 
  		palette[i].rgbGreen = i; 
  		palette[i].rgbBlue = i; 
  	}
  			
  	for (i = 245; i < 250; i++) {
  		palette[i].rgbRed = 0; 
  		palette[i].rgbGreen = 255; 
  		palette[i].rgbBlue = 0;
  	}
  		
  	for (i = 250; i < 256; i++) {
  		palette[i].rgbRed = 255; 
  		palette[i].rgbGreen = 0; 
  		palette[i].rgbBlue = 0;
  	}
  	
  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}
	

int DLL_CALLCONV
FreeImageAlgorithms_GetTernaryPalette(RGBQUAD *palette, RGBQUAD background_colour,
									  int pos1, RGBQUAD colour1, int pos2, RGBQUAD colour2)
{
	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	int i;

	// Mark as background colour
  	for (i = 0; i < 256; i++) { 
  		palette[i].rgbRed = background_colour.rgbRed; 
  		palette[i].rgbGreen = background_colour.rgbGreen; 
  		palette[i].rgbBlue = background_colour.rgbBlue; 
  	}
  			
	palette[pos1].rgbRed = colour1.rgbRed; 
  	palette[pos1].rgbGreen = colour1.rgbGreen; 
  	palette[pos1].rgbBlue = colour1.rgbBlue; 
  	
	palette[pos2].rgbRed = colour2.rgbRed; 
  	palette[pos2].rgbGreen = colour2.rgbGreen; 
  	palette[pos2].rgbBlue = colour2.rgbBlue; 

  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_GetRainBowPalette(RGBQUAD *palette)
{
	int i, p1, p2;
	
	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	p1 = 256 / 3;
	p2 = 2*p1;
  
  	// First do the blue bit
	for (i=0; i<p1; i++)
		palette[i].rgbBlue = 255;
	
	for (i=p1; i<p2; i++)
		palette[i].rgbBlue = 255 + ( (i - p1) * -255 / p1);

	for (i=p2; i<256; i++)
		palette[i].rgbBlue = 0;


	// Now add the green bit
	for (i=0; i<p1; i++)
		palette[i].rgbGreen = (i * 255 / p1);
	
	for (i=p1; i<p2; i++)
		palette[i].rgbGreen = 255;

	for (i=p2; i<256; i++)
		palette[i].rgbGreen = 255 + ( (i - p2) * -255 / p1);


	// Now add the red bit
	for (i=0; i<p1; i++)
		palette[i].rgbRed = 0;
			
	for (i=p1; i<p2; i++)
		palette[i].rgbRed = ((i - p2) * 255 / p1);
		
	for (i=p2; i<256; i++)
		palette[i].rgbRed = 255;

	// Now black and white at the ends
	palette[0].rgbRed = 0;
	palette[0].rgbGreen = 0;
	palette[0].rgbBlue = 0;
	
	palette[255].rgbRed = 255;
	palette[255].rgbGreen = 255;
	palette[255].rgbBlue = 255;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
} 


int DLL_CALLCONV
FreeImageAlgorithms_GetReverseRainBowPalette(RGBQUAD *palette)
{
    FreeImageAlgorithms_GetRainBowPalette(palette);

    return FreeImageAlgorithms_ReversePaletteEntries(palette);  
} 


int DLL_CALLCONV
FreeImageAlgorithms_GetLogColourPalette(RGBQUAD *palette)
{	
	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

  	for (int i = 1; i < 256; i++) {
  	
  		double level = floor ((255.0 * log10((double)i)/log10(255.0)) + 0.5);
  	
  		palette[i].rgbRed = (BYTE) level; 
  		palette[i].rgbGreen = (BYTE) level; 
  		palette[i].rgbBlue = (BYTE) level; 
  	}
  	
  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}
 

int DLL_CALLCONV
FreeImageAlgorithms_GetFalseColourPalette(RGBQUAD *palette, double wavelength)
{
	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	double red, green, blue;

	//Get RGB components for false colour image for a particular wavelength
	//The numbers come from a fortran program that BV found on the internet

	if ((wavelength >= 380) && (wavelength < 440)) {
		red = (440 - wavelength)/(440 - 380);
		green = 0;
		blue = 1;
	}
	else if ((wavelength >= 440) && (wavelength < 490)) {
		red = 0;
		green = (wavelength - 440)/(490 - 440);
		blue = 1;
	}
	else if ((wavelength >= 490) && (wavelength < 510)) {
		red = 0;
		green = 1;
		blue = (510 - wavelength )/(510 - 490);
	}
	else if ((wavelength >= 510) && (wavelength < 580)) {
		red = (wavelength - 510)/(580 - 510);
		green = 1;
		blue = 0;
	}
	else if ((wavelength >= 580) && (wavelength < 645)) {
		red = 1;
		green = (645 - wavelength )/(645 - 580);
		blue = 0;
	}
	else if ((wavelength >= 645) && (wavelength < 780)) {
		red = 1;
		green = 0;
		blue = 0;
	}
	else {		  //display as mono
	
		FreeImageAlgorithms_GetGreyLevelPalette(palette);
		
		return FREEIMAGE_ALGORITHMS_SUCCESS;
	}
	
	for (int i = 0; i < 256; i++) { 
  		palette[i].rgbRed = (BYTE) (red * i); 
  		palette[i].rgbGreen = (BYTE) (green * i); 
  		palette[i].rgbBlue = (BYTE) (blue * i);
  	}
  
  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageAlgorithms_GetOpticalDensityPalette(RGBQUAD *palette, unsigned char red,
	unsigned char green, unsigned char blue, int contrast, int entries)
{	
	int i;
	double h, s, l, slog, c;
	unsigned char r, g, b;

	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	slog = log( 1.0 / (8.0 * contrast) ) / 255.0;		 // to scale the od nicely 0-255
	
	FreeImageAlgorithms_RGBToHSL (red, green, blue, &h, &s, &l);
	
  	for (i=0; i<entries; i++) { 
  	
  		c = exp((double)i * slog); 
  	
  		if (c > 1.0)
  			c=1.0;
		else if (c < 0.0)
			c=0.0;
  	
  		FreeImageAlgorithms_HSLToRGB( h, s, c, &r, &g, &b); 
  	
  		palette[i].rgbRed = r; 
  		palette[i].rgbGreen = g; 
  		palette[i].rgbBlue = b; 
  	}
  	
  	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageAlgorithms_GetTemperaturePalette(RGBQUAD *palette)
{
	int i, p1, p2;

	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	memset(palette, 0, sizeof(RGBQUAD) * 256);

	p1 = 256 / 2;
	p2 = (int) ((3.0 * 256) / 4.0);

	// First do the red bit	
	for (i=0; i < 256; i++)
		palette[i].rgbRed = i;
		
	// Now add the green bit
	for (i=p1; i < 256; i++)
		palette[i].rgbGreen = ((i - p1) * 255 / p1);

  	// Now do the blue bit	

	for (i=p2; i < 256; i++)
		palette[i].rgbBlue = ( ((i - p2) * 255) / (256 - p2) );

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


static int DLL_CALLCONV
FreeImageAlgorithms_SeismicReversePaletteEntries(RGBQUAD *palette,
                                                 unsigned char start, unsigned char size)
{
	int i, max_element;

	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	max_element = (size / 2) - 1;

	for (i=start; i < max_element ; i++) {    

		INPLACESWAP(palette[i].rgbRed, palette[size - i].rgbRed);
		INPLACESWAP(palette[i].rgbGreen, palette[size - i].rgbGreen);
		INPLACESWAP(palette[i].rgbBlue, palette[size - i].rgbBlue);
	}
	
	return 1;
}

int DLL_CALLCONV
FreeImageAlgorithms_GetSeismicColourPalette(RGBQUAD *palette)
{
	int halfway, contrast = 6;

	if(palette == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	halfway = 128; 

	FreeImageAlgorithms_GetOpticalDensityPalette(palette, 0, 0, 255, contrast, halfway);

	FreeImageAlgorithms_SeismicReversePaletteEntries(palette, 0, halfway);  

	// Second Half
	FreeImageAlgorithms_GetOpticalDensityPalette(&(palette[halfway]), 255, 0, 0, contrast, halfway);  
	
  	
  	palette[0].rgbRed = 0;
  	palette[0].rgbGreen = 0;
  	palette[0].rgbBlue = 0;
  	
	palette[255].rgbRed = 255;
  	palette[255].rgbGreen = 255;
  	palette[255].rgbBlue = 255;
 
  	return 0;
}
